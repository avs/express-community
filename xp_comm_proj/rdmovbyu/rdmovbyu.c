/*
 * Movie-BYU ASCII file reader for AVS/Express
 * Adapted from the AVS geometry filter, in
 * /usr/avs/filters/byu.c
 *
 * Adapted by I. Curington
 * Revision:
 * 28 June 1997  First adaption pass as omodule
 *  7 July 1997  polyhedron optimization
 *
 */

/*
			Copyright (c) 1997 by
			Advanced Visual Systems Inc.
			All Rights Reserved

	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

*/
/*-----------------------------------------------------*
 *                                                     *
 *               ****  read Movie BYU  ****            *
 *                                                     *
 * read a file (ascii) in the Movie-BYU format.        *
 *                                                     *
 *  Modified from old AVS5 filter                      *
 *  June 1997  I. Curington  <ianc@acm.org>            *
 *-----------------------------------------------------*/

#include "xp_comm_proj/rdmovbyu/gen.h"

#include <stdio.h>

#include <avs/util.h>
#include <avs/err.h>
#include <avs/om.h>
#include <avs/math.h>
#include <avs/dtype.h>
#include <avs/fld.h>

#define ERR_RETURN(A) {ERRerror("read_byu",1,ERR_ORIG, A); return(0);}
typedef float FLOAT3[3];
typedef int *IPTR;
#define ABS(D)	((D)<0 ? -(D) : (D))
#define BUFFER_SIZE 200

/*
 * Set Debug Flag (uncomment next line)
 * #define DEBUG 1
 * #define MAKE_CELL_DATA 1
 */
/*#define DEBUG 0*/

#ifdef DEBUG
   #define DEBUG_PRINT(a) {printf(a);}
#else
   #define DEBUG_PRINT(a) {}
#endif

/*--------------------------------------------------------------------
 *      OM module Entry Point, Resolve Parameters
 *--------------------------------------------------------------------*/

int read_byu_update(OMobj_id elem_id, OMevent_mask event_mask, int seq_num)
{
    OMobj_id filename, out;
    char *str;
    int Result;

    str = NULL;
    filename = OMfind_subobj(elem_id, OMstr_to_name("filename"),
			      OM_OBJ_RD);
    OMget_str_val(filename, &str, 0);
    out = OMfind_subobj(elem_id, OMstr_to_name("out"), OM_OBJ_RW);

    Result = FUNCread_byu(str, out);

    if (str) free(str);

    return(Result);
}

/*--------------------------------------------------------------------
 *      Read MOVIE BYU module
 *--------------------------------------------------------------------*/
int FUNCread_byu ( char *filename, OMobj_id out)
{

   int nparts, nverts, npolys, nconns;
   int *parray;
   FLOAT3 *varray;
   IPTR *iarray;
   int *sizearray;
   int *pol_sizearray;
   int *poly_connect_list;
#ifdef MAKE_CELL_DATA
   int *mat_array;
   int *loc_mat_array;
#endif
   int i, j, k, n, d;
   int c;
   float l;
   FILE *pf;
   FILE  *FILEfopen();
   char  line[BUFFER_SIZE];
   int dtype, nnodes;
   float *node_data;
   OMobj_id poly_id;
   int total_con_size, total_pol_size;
   int veclen;
   int ntri, nquad, npol, npt, nline;
   int interrupt;
   int conn_min, conn_max;
   int abs_d;
   int cs;

   /**************
    ***  body  ***
    **************/
   DEBUG_PRINT(" start body\n");

   interrupt = 0;

   if (!filename) {
        DEBUG_PRINT(" no filename\n");
	return(1); /* silent exit */
   }

   /*************************
    * Open File             *
    *************************/
   if (!(pf = (FILE *)FILEfopen(filename, SIO_R_TXT))) {
	ERRerror("read movie-byu",1,ERR_ORIG,"Error opening file %s", filename);
	/*fclose(pf);*/
	return(0);
   }
   DEBUG_PRINT(" file opened\n");
   	
   /* Read First Lines */
   fgets (line, BUFFER_SIZE, pf);
   /*  skip comments.  */
   byu_skip_comment(line,pf);
   sscanf(line,"%d %d %d %d", &nparts, &nverts, &npolys, &nconns);

   DEBUG_PRINT(" got 1st line\n");
#ifdef DEBUG
   printf("   nparts = %d, nverts = %d, npolys = %d, nconns = %d\n",
           nparts, nverts, npolys, nconns );
#endif

   /*************************
    * Allocate Temp Arrays  *
    * Scan in Geometry      *
    *************************/
   parray = (int *) malloc(nparts*2*sizeof(int));
   DEBUG_PRINT(" got 1st malloc parray\n");
   if ( parray == NULL ) ERR_RETURN("Could not allocate memory for parts\n");
   for (i=0; i<nparts; i++) {
      if (fscanf(pf,"%d %d", &parray[i*2], &parray[i*2+1]) != 2) {
         fclose(pf);
	 ERR_RETURN("Not enough parts\n");
      }
      parray[i*2]--;
      parray[i*2+1]--;
      if ( (i%100)==0) {
           OMstatus_check(10*i/nparts,"movie byu parts",&interrupt);
           if (interrupt) return(1);
      }
   }
   DEBUG_PRINT(" got parts list\n");
   varray = (FLOAT3 *) malloc(nverts*sizeof(FLOAT3));
   if ( varray == NULL ) ERR_RETURN("Could not allocate memory for verts\n");
   for (i=0; i<nverts; i++) {
      if (fscanf(pf,"%f %f %f",&varray[i][0],&varray[i][1],&varray[i][2])!=3) {
         fclose(pf);
	 ERR_RETURN("Not enough verts\n");
      }
      if ( (i%100)==0) {
           OMstatus_check(40*i/nverts+10,"movie byu verts",&interrupt);
           if (interrupt) return(1);
      }
   }
   DEBUG_PRINT(" got vertex list\n");
   iarray = (IPTR *) malloc(npolys*sizeof(IPTR));
   if ( iarray == NULL ) ERR_RETURN("Could not allocate memory for ptr array\n");
   sizearray = (int *) malloc(npolys*sizeof(int));
   if ( sizearray == NULL ) ERR_RETURN("Could not allocate memory for size array\n");
   total_con_size = 0;
   for (i=0; i<npolys; i++) {
      iarray[i] = (int *) malloc(1);
      n = 0;
      do {
	 if (fscanf(pf,"%d", &d) != 1) {
            fclose(pf);
	    ERR_RETURN("Not enough faces\n");
	 }
	 n++;
	 iarray[i] = (int *) realloc((char *) iarray[i], 
					n*sizeof(int));
         abs_d = ABS(d);
	 iarray[i][n-1] = abs_d;

         total_con_size++;

      } while (d>=0);
      sizearray[i] = n;

      if ( i== 0 )
           conn_min = conn_max = abs_d;
      else
      {
           if ( conn_min > abs_d ) conn_min = abs_d;
           if ( conn_max < abs_d ) conn_max = abs_d;
      }
      if ( (i%100)==0) {
           OMstatus_check(40*i/npolys+50,"movie byu polys",&interrupt);
           if (interrupt) return(1);
      }
   }

   /*************************
    * Close File            *
    *************************/
   fclose(pf);


   DEBUG_PRINT(" got connect list\n");

   /*************************
    * Build AVS Structure   *
    *************************/
   npol = ntri = nquad = npt = nline = 0;
   for (i=0; i<npolys; i++) {
       n = sizearray[i];
       if ( n == 1 ) npt++;
       if ( n == 2 ) n++;
       if ( n == 3 ) ntri++;
       if ( n == 4 ) nquad++;
       if ( n  > 4 ) npol++;
   }
#ifdef DEBUG
   printf("   npol = %d, ntri = %d, nquad = %d, npt = %d, nline = %d\n",
           npol, ntri, nquad, npt, nline );
   printf("   conn_min = %d, conn_max = %d\n",conn_min, conn_max);
#endif

   if ( (npol+ntri+nquad+npt+nline) != npolys ) ERR_RETURN("error in tri/quad/poly count\n");

   poly_connect_list = (int *) malloc( total_con_size * sizeof(int));
   if ( poly_connect_list == NULL ) ERR_RETURN("Could not allocate memory for connect list\n");


#ifdef MAKE_CELL_DATA
   mat_array = (int *) malloc( npolys*sizeof(int));
   if ( mat_array == NULL ) ERR_RETURN("Could not allocate memory for material array\n");

   loc_mat_array = (int *) malloc( npolys*sizeof(int));
   if ( loc_mat_array == NULL ) ERR_RETURN("Could not allocate memory for local material array\n");

   /*
    * build global material list
    */
   for (i=0; i<npolys; i++) mat_array[i] = 0;
   for (i=0; i<nparts; i++) {
           for (j=parray[i*2]; j<=parray[i*2+1]; j++) {
               mat_array[j] = i + 1;
           }
   }
#endif

   DEBUG_PRINT(" built new arrays\n");

 /*****================================================
  ***** Old AVS5 Geom Calls, now removed
  *****
  *****   for (i=0; i<nparts; i++) {
  *****      xobj = GEOMcreate_obj(GEOM_POLYHEDRON,NULL);
  *****      GEOMadd_vertices_with_data(xobj, varray, NULL, NULL, nverts,0);
  *****      for (j=parray[i*2]; j<=parray[i*2+1]; j++) {
  *****        GEOMadd_polygon(xobj,sizearray[j],iarray[j],0,0);
  *****      }
  *****      GEOMgen_normals(xobj,0);
  *****      GEOMcvt_polyh_to_polytri(xobj,GEOM_SURFACE|GEOM_WIREFRAME);
  *****      GEOMwrite_obj(xobj, fileno(stdout), 0);
  *****      GEOMdestroy_obj(xobj);
  *****   }
  ================================================*****/


   /**************
    *** CLEAR  ***
    **************/
    if (FLDset_ncell_sets (out, 0) != 1) {
		ERR_RETURN("Error setting ncell_sets\n");
    }
    if (FLDset_nnodes (out, 0) != 1) {
		ERR_RETURN("Error setting nnodes\n")
    }
    if (FLDset_node_data_ncomp (out, 0) != 1) {
		ERR_RETURN("Error setting nnode_data\n");
    }
   DEBUG_PRINT(" out field cleared\n");

   /*************************
    * Build Field Structure *
    *************************/

   /*
    * Define number of nodes in the field
    */
   if (FLDset_nnodes(out, nverts) != 1) {
      ERR_RETURN("Error setting nnodes\n");
   }

   /*
    * Define nspace of field
    */
   if (FLDset_nspace(out, 3) != 1) {
      ERR_RETURN("Error setting nspace\n");
   }

   /*
    * Define coordinates of field
    */
   if (FLDset_coord(out, (float *) &(varray[0][0]),
                    nverts*3, OM_SET_ARRAY_COPY) != 1) {
      ERR_RETURN("Error setting coords\n");
   }
   OMstatus_check(92,"movie byu FLD",&interrupt);

   cs = 0;
   if ( npol > 0 ) /********** polyhedron cell set *************/
   {
       pol_sizearray = (int *) malloc(npolys*sizeof(int));
       if ( pol_sizearray == NULL ) ERR_RETURN("Could not allocate memory for size array\n");

       k = 0;
       for (i=0; i<npolys; i++) {
           n = sizearray[i];
           if ( n > 4 )
           {
               pol_sizearray[k] = n;
               for (j=0; j<n; j++)
               {
                   poly_connect_list[k++] = iarray[i][j] - 1;
               }
           }
       }
       total_pol_size = k;

       /*
        * Add a polyhedron cell set (general purpose)
        */
       if (FLDadd_cell_set(out, "Polyhedron") != 1) {
          ERR_RETURN("Error adding polyhedron cell_set\n");
       }
   
       /*
        * Get object id of polyhedron cell set
        */
       if (FLDget_cell_set(out, cs, &poly_id) != 1) {
          ERR_RETURN("Error getting polyhedron cell_set\n");
       }

       /*
        * Set number of cells in the cell set
        */
       if (FLDset_npolys(poly_id, npol) != 1) {
          ERR_RETURN("Error setting polyhedron ncells\n");
       }
       /*
        * Set the polygon connectivity array
        */
       if (FLDset_poly_nnodes(poly_id, pol_sizearray, npol, OM_SET_ARRAY_COPY) != 1) {
		    ERR_RETURN("cannot set polyhedron poly_nnodes\n");
       }
       OMstatus_check(94,"movie byu FLD",&interrupt);
       if (FLDset_poly_connect(poly_id, poly_connect_list, total_pol_size,
                               OM_SET_ARRAY_COPY) != 1) {
          ERR_RETURN("Error setting polyhedron poly_connect\n");
       }
       cs++;
   }
   if ( ntri > 0 ) /********** triangle cell set *************/
   {
       k = 0;
       for (i=0; i<npolys; i++) {
           n = sizearray[i];
           if ( n == 3 )
           {
#ifdef MAKE_CELL_DATA
               loc_mat_array[k] = mat_array[i];
#endif
               for (j=0; j<n; j++)
               {
                   poly_connect_list[k++] = iarray[i][j] - 1;
               }
           }
       }
       /*
        * Add a tri cell set
        */
       if (FLDadd_cell_set(out, "Tri") != 1) {
          ERR_RETURN("Error adding tri cell_set\n");
       }
   
       /*
        * Get object id of cell set
        */
       if (FLDget_cell_set(out, cs, &poly_id) != 1) {
          ERR_RETURN("Error getting tri cell_set\n");
       }

       /*
        * Set number of cells in the cell set
        */
       if (FLDset_ncells(poly_id, ntri) != 1) {
          ERR_RETURN("Error setting tri ncells\n");
       }
       /*
        * Set the connectivity array
        */
       if (FLDset_node_connect(poly_id, poly_connect_list, ntri * 3,
                            OM_SET_ARRAY_COPY) != 1) {
		ERR_RETURN("cannot get cell connectivity");
       }

       /*
        * Set the cell data from material parts list
        */
#ifdef MAKE_CELL_DATA
       dtype = DTYPE_INT;
       veclen = 1;
       if (FLDset_cell_data_ncomp(poly_id, 1) != 1) {
          ERR_RETURN("Error setting cell data ncomp\n");
       }
       if (FLDset_cell_data_comp(poly_id, cs, veclen, "material","tag") != 1) {
          ERR_RETURN("Error setting cell data comp veclen\n");
       }
       if (FLDset_cell_data(poly_id, cs, (char *)loc_mat_array, dtype, ntri,
                 OM_SET_ARRAY_COPY) != 1) {
	    ERR_RETURN("Error setting material cell data");
       }
#endif

       OMstatus_check(95,"movie byu FLD",&interrupt);
       cs++;
   }

   if ( nquad > 0 ) /********** quad cell set *************/
   {
       k = 0;
       for (i=0; i<npolys; i++) {
           n = sizearray[i];
           if ( n == 4 )
           {
#ifdef MAKE_CELL_DATA
               loc_mat_array[k] = mat_array[i];
#endif
               for (j=0; j<n; j++)
               {
                   poly_connect_list[k++] = iarray[i][j] - 1;
               }
           }
       }
       /*
        * Add a quad cell set
        */
       if (FLDadd_cell_set(out, "Quad") != 1) {
          ERR_RETURN("Error adding quad cell_set\n");
       }
   
       /*
        * Get object id of cell set
        */
       if (FLDget_cell_set(out, cs, &poly_id) != 1) {
          ERR_RETURN("Error getting quad cell_set\n");
       }

       /*
        * Set number of cells in the cell set
        */
       if (FLDset_ncells(poly_id, nquad) != 1) {
          ERR_RETURN("Error setting quad ncells\n");
       }
       /*
        * Set the connectivity array
        */
       if (FLDset_node_connect(poly_id, poly_connect_list, nquad * 4,
                            OM_SET_ARRAY_COPY) != 1) {
		ERR_RETURN("cannot get cell connectivity");
       }

       /*
        * Set the cell data from material parts list
        */
#ifdef MAKE_CELL_DATA
       dtype = DTYPE_INT;
       veclen = 1;
       if (FLDset_cell_data_ncomp(poly_id, 1) != 1) {
          ERR_RETURN("Error setting cell data ncomp\n");
       }
       if (FLDset_cell_data_comp(poly_id, cs, veclen, "material","tag") != 1) {
          ERR_RETURN("Error setting cell data comp veclen\n");
       }
       if (FLDset_cell_data(poly_id, cs, (char *)loc_mat_array, dtype, nquad,
                 OM_SET_ARRAY_COPY) != 1) {
	    ERR_RETURN("Error setting material cell data");
       }
#endif

       OMstatus_check(95,"movie byu FLD",&interrupt);
       cs++;
   }
   if ( npt > 0 ) /********** point cell set *************/
   {
       DEBUG_PRINT(" points found but ignored\n");
   }
   if ( nline > 0 ) /********** line cell set *************/
   {
       DEBUG_PRINT(" lines found but ignored\n");
   }


   OMstatus_check(97,"movie byu FLD",&interrupt);

   OMstatus_check(99,"movie byu FLD",&interrupt);

   DEBUG_PRINT(" out field finished building\n");

   /*************************
    * Free Memory           *
    *************************/
   if (parray) free (parray);
   DEBUG_PRINT(" parray memory freed\n");
   if (varray) free (varray);
           for (i=0; i<npolys; i++) {
             if( iarray[i] ) free (iarray[i]);
           }
   if (iarray) free (iarray);
   DEBUG_PRINT(" iarray memory freed\n");
   if (sizearray) free (sizearray);
   DEBUG_PRINT(" sizearray memory freed\n");

   if (npol > 0 && pol_sizearray) free (pol_sizearray);
   DEBUG_PRINT(" pol_sizearray memory freed\n");
   if (poly_connect_list) free (poly_connect_list);
   DEBUG_PRINT(" poly_connect_list memory freed\n");
#ifdef MAKE_CELL_DATA
   if (mat_array) free (mat_array);
   if (loc_mat_array) free (loc_mat_array);
#endif

   DEBUG_PRINT(" all memory freed, method finished.\n");

   return(1);
}

/*--------------------------------------------------------------------
 *      Scan Comment Lines
 *--------------------------------------------------------------------*/
byu_skip_comment(line,pf)
   char *line;
   FILE *pf;
{
   while (line[0] == '$' || line[0] == '#') 
	fgets (line, BUFFER_SIZE, pf);

}


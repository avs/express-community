/* A reader for FLUENT UNIVERSAL file format. 
   This code segment reads the Fluent Universal format, output by Fluent v4.5
   This code is the structured code of Fluent. It only handles single block
   curvilinear grids. It is assumed here that the velocities are stored in
   curvi-linear mode. See Fluent's USE CYLINDRICAL VELOCITIES flag. No
   attempt is made here to convert from cartesian velocities. We used the
   cylindrical velocities in our code because we wanted to do LIC analysis in
   each cross-plane.


   This reader supports the ASCII output from Fluent. Binary is not supported.

   Reading Fluent's document "Appendix A, pp 1-52, Description of the FLUENT
   Universal File" May 10, 1997, is recommended.

Original version by:

Dr. Jean M. Favre,                            email:favre@cscs.ch
http://www.cscs.ch/people/Favre.html
CSCS, Swiss Center for Scientific Computing | Tel:   +41 (91) 610.82.40
Via Cantonale, 6928 Manno, Switzerland      | Fax:   +41 (91) 610.82.82

Written: Thu Mar 22 2001

modifications: Tobias Schiebeck, IAC
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xp_comm_proj/rdfluent/gen.h"

/* #define TEST */

int
read_fluent(OMobj_id read_fluent_id, OMevent_mask event_mask, int seq_num)
{
   OMobj_id out_id;
   char  *filename = NULL;
   char inpline[80], *found;
   int out_ndim, *out_dims, out_nspace, out_nnodes;
   int out_comp_count, out_veclen, out_data_type, out_ndata;
   int *c_iblank, I, j, k, dxdy, foo;
   int code, i, dimx, dimy, dimz, conv;
   FILE *fp;

   float *xc, *yc, *zc, *out_coord;
   float null_value = 0;

   float  *out_data[14];
   int labels_indices[14]; /* stores the index to retrieve the variable's name
                              from the array Fluent_Names */
   char *Fluent_Names[] = {"STATIC PRESSURE",
                           "U-VELOCITY",
                           "V-VELOCITY",
                           "W-VELOCITY",
                           "ENTHALPHY",
                           "DENSITY",
                           "SPECIES",
                           "TURB. KINETIC ENERGY",
                           "TURB. ENERGY DISSIPATION",
                           "REYNOLDS STRESSES",
                           "VOF",
                           "SEC PHASE VEL",
                           "SOLID PRESSURE",
                           "GRANULAR TEMP",
						   "UNKNOWN"
                           };

	/* initialize label_indices with "UNKNOWN" */
	memset(labels_indices,14,14);

   /* Get filename's value */
   if (OMget_name_str_val(read_fluent_id, OMstr_to_name("filename"), &filename, 0) != 1)
      filename = NULL;

   fp = fopen(filename, "r");

   do {
     fgets(inpline, 80, fp);
     found = strstr(inpline, "GRID SIZE");
   } while ((found == NULL) && (!feof(fp)));

   if (!found)
   {
	   fprintf(stderr, "Invalid file format (GRID SIZE not found)\n");
	   return(0);
   }
   		
   fgets(inpline, 80, fp);
   conv=sscanf(inpline, "%d %d %d", &dimx, &dimy, &dimz);
   if (conv !=3)
   {
	   fprintf(stderr, "Invalid file format (GRID SIZE invalid)\n");
	   return(0);
   }

#ifdef TEST   
   fprintf(stderr, "GRID SIZE read = %d %d %d\n", dimx, dimy, dimz);
#endif

   /* Get mesh id */
   out_id = OMfind_subobj(read_fluent_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Set mesh dimensionality, out_ndim can be 1,2 or 3 */
   out_ndim = 3;
   FLDset_ndim (out_id, out_ndim);

   /* Set mesh dims array */
   out_dims = (int *)ARRalloc(NULL, DTYPE_INT, out_ndim, NULL);

   /*** fill in dims array with your values ***/

   out_dims[0] = dimx-1;
   out_dims[1] = dimy-1;
   out_dims[2] = dimz-1;

   FLDset_dims (out_id, out_dims);
   if (out_dims)
      ARRfree((char *)out_dims);

   out_nspace = 3;
   FLDset_nspace (out_id, out_nspace);
   FLDset_grid_type (out_id, 0);

   /* Set mesh coordinates */
   /* first allocate out_coord array */
   FLDget_nnodes (out_id, &out_nnodes);

   found = NULL;
   do {
     fgets(inpline, 80, fp);
     found = strstr(inpline, "GRID");
   } while ((found == NULL) && (!feof(fp)));

   if (!found)
   {
	   fprintf(stderr, "Invalid file format (GRID not found)\n");
	   return(0);
   }

   xc = (float *)ARRalloc(NULL, DTYPE_FLOAT, out_nnodes, NULL);
   yc = (float *)ARRalloc(NULL, DTYPE_FLOAT, out_nnodes, NULL);
   zc = (float *)ARRalloc(NULL, DTYPE_FLOAT, out_nnodes, NULL);
   c_iblank = (int *)ARRalloc(NULL, DTYPE_INT, dimx*dimy*dimz, NULL);


   for(i=0; i < out_nnodes; i++){
     conv=fscanf(fp,"%15e8", xc+i);
	 if (conv!=1)
	 {
	 	if (xc)
		{
			ARRfree(xc);
			xc=NULL;
		}
	 	if (yc)
		{
			ARRfree(yc);
			yc=NULL;
		}
	 	if (zc)
		{
			ARRfree(zc);
			zc=NULL;
		}
	 	if (c_iblank)
		{
			ARRfree(c_iblank);
			c_iblank=NULL;
		}
		fprintf(stderr, "Invalid file format (GRID invalid - xc)\n");
		return(0);		
	 }	 
   }
   for(i=0; i < out_nnodes; i++){
     conv=fscanf(fp,"%15e8", yc+i);
	 if (conv!=1)
	 {
	 	if (xc)
		{
			ARRfree(xc);
			xc=NULL;
		}
	 	if (yc)
		{
			ARRfree(yc);
			yc=NULL;
		}
	 	if (zc)
		{
			ARRfree(zc);
			zc=NULL;
		}
	 	if (c_iblank)
		{
			ARRfree(c_iblank);
			c_iblank=NULL;
		}
		fprintf(stderr, "Invalid file format (GRID invalid - yc)\n");
		return(0);		
	 }	 
   }
   for(i=0; i < out_nnodes; i++){
     conv=fscanf(fp,"%15e8", zc+i);
 	 if (conv!=1)
	 {
	 	if (xc)
		{
			ARRfree(xc);
			xc=NULL;
		}
	 	if (yc)
		{
			ARRfree(yc);
			yc=NULL;
		}
	 	if (zc)
		{
			ARRfree(zc);
			zc=NULL;
		}
	 	if (c_iblank)
		{
			ARRfree(c_iblank);
			c_iblank=NULL;
		}
		fprintf(stderr, "Invalid file format (GRID invalid - zc)\n");
		return(0);		
	 }	 
   }

   found = NULL;
   do {
     fgets(inpline, 80, fp);
     found = strstr(inpline, "CELL TYPES");
   } while ((found == NULL) && (!feof(fp)));

   if (!found)
   {
	 	if (xc)
		{
			ARRfree(xc);
			xc=NULL;
		}
	 	if (yc)
		{
			ARRfree(yc);
			yc=NULL;
		}
	 	if (zc)
		{
			ARRfree(zc);
			zc=NULL;
		}
	 	if (c_iblank)
		{
			ARRfree(c_iblank);
			c_iblank=NULL;
		}
 	    fprintf(stderr, "Invalid file format (CELL TYPES not found)\n");
	    return(0);
   }

   for(i=0; i < dimx*dimy*dimz; i++){
     conv=fscanf(fp,"%d", c_iblank+i);
  	 if (conv!=1)
	 {
	 	if (xc)
		{
			ARRfree(xc);
			xc=NULL;
		}
	 	if (yc)
		{
			ARRfree(yc);
			yc=NULL;
		}
	 	if (zc)
		{
			ARRfree(zc);
			zc=NULL;
		}
	 	if (c_iblank)
		{
			ARRfree(c_iblank);
			c_iblank=NULL;
		}
		fprintf(stderr, "Invalid file format (CELLS invalid)\n");
		return(0);		
	 }	 

   }
   fgets(inpline, 80, fp);

   while(inpline[0] != '#')
     fgets(inpline, 80, fp); /* read the hash sign */

   for(i=0; i < dimx*dimy*dimz; i++){
     if((c_iblank[i] == 74)) /* Fluent's code to say this is part of the
                                flow, i.e. not a solid part */
       c_iblank[i] = 0;
     else
       c_iblank[i] = 1;
   }


   out_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                              out_nspace*out_nnodes, NULL);

   /*** fill in out_coord array with X[,Y,Z] values at each node ***/
   for(i=0; i < out_nnodes; i++){
     out_coord[3*i]   = xc[i];
     out_coord[3*i+1] = yc[i];
     out_coord[3*i+2] = zc[i];
   }

	if (xc)
	{
		ARRfree(xc);
		xc=NULL;
	}
 	if (yc)
	{
		ARRfree(yc);
		yc=NULL;
	}
 	if (zc)
	{
		ARRfree(zc);
		zc=NULL;
	}

   FLDset_coord (out_id, out_coord, out_nspace*out_nnodes, OM_SET_ARRAY_FREE);

   out_id = OMfind_subobj(read_fluent_id, OMstr_to_name("out"), OM_OBJ_RW);

   out_ndata = out_nnodes;

   out_veclen = 1;
   out_data_type = DTYPE_FLOAT;

/*
   Component 0 is special. We store a code to extract the solid boundaries
   we then use thresh_null followed by external_faces to get all solid
   boundaries combined into a single object
*/
   {
   out_data[0] = (float *)ARRalloc(NULL, out_data_type, out_ndata, NULL);

   I = 0;
   if(dimz == 1){ /* this is a 2D problem only */
     for(j=0; j < (dimy-1); j++){
       for(i=0; i < (dimx-1); i++){
         foo = c_iblank[(j+1) * dimx + i+1] +
                     c_iblank[(j+1) * dimx + i  ] +
                     c_iblank[ j    * dimx + i+1] +
                     c_iblank[ j    * dimx + i  ] ;

       if(foo)
         out_data[0][I] = null_value;
       else
         out_data[0][I] = 1;
       I++;
       }
     }
   }
   else{
     dxdy = dimx*dimy;

     for(k=0; k < (dimz-1); k++){
       for(j=0; j < (dimy-1); j++){
         for(i=0; i < (dimx-1); i++){
           foo = c_iblank[(k+1)* dxdy + (j+1) * dimx + i+1] +
                       c_iblank[(k+1)* dxdy + (j+1) * dimx + i  ] +
                       c_iblank[(k+1)* dxdy + (j  ) * dimx + i+1] +
                       c_iblank[(k+1)* dxdy + (j  ) * dimx + i  ] +
                       c_iblank[(k  )* dxdy + (j+1) * dimx + i+1] +
                       c_iblank[(k  )* dxdy + (j+1) * dimx + i  ] +
                       c_iblank[(k  )* dxdy + (j  ) * dimx + i+1] +
                       c_iblank[(k  )* dxdy + (j  ) * dimx + i  ] ;
         if(foo == 0)
              out_data[0][I] = null_value;
         else
              out_data[0][I] = 1;
         I++;
         }
       }
     }
   }
   } /* special node_data to store boundary cel code */


 	if (c_iblank)
	{
		ARRfree(c_iblank);
		c_iblank=NULL;
	}

   out_comp_count = 1;
   do {
     fgets(inpline, 80, fp);
     conv=sscanf(inpline,"%6d", &code);
	 if (conv!=1)
	 	/* label code invalid but we don't care, and assume it is 
		   set to -1 (no labels, as they are optional anyway), so 
		   don't read the rest of input 
		*/
		break;  
#ifdef TEST   
     fprintf(stderr,"code = %d\n", code);
#endif

     if(code == -1) break;
/* don't read the rest of input because Fluent says labels are optional */

	 if ((code>=266) && (code<280))   
     	labels_indices[out_comp_count] = code-266;
	 else 
	 	/* label code invalid but we don't care, and assume it is 
		   set to -1 (no labels, as they are optional anyway), so 
		   don't read the rest of input 
		*/
	 	break; 

#ifdef TEST   
     fprintf(stderr,"scanning for %d scalars for %s\n", out_nnodes,
                     Fluent_Names[labels_indices[out_comp_count]]);
#endif

     out_data[out_comp_count] = (float *)ARRalloc(NULL, out_data_type,
                                                  out_ndata, NULL);

     for(i=0; i < out_nnodes; i++){
       conv = fscanf(fp,"%15e8", &out_data[out_comp_count][i]);
	   if (conv!=1)
	   		break;
     }
	 if (i<out_nnodes)
	 {
	    /*
			something has gone wrong reading the last set of out_data
			delete this dataset and stop reading
		*/
		if (out_data[out_comp_count])
		{
			ARRfree(out_data[out_comp_count]);
			out_data[out_comp_count]=NULL;
		}	
	 	break;
	 } 
	 
     found = NULL;
     do {
       fgets(inpline, 80, fp);
       found = strstr(inpline, "#");
     } while ((found == NULL) && (!feof(fp)));
	 if (!found)
	 	break; 

     out_comp_count++;
   } while (code != -1);

   fclose(fp);

   FLDset_node_data_ncomp (out_id, out_comp_count);

/* set node_dat[0] first */
   FLDset_node_data_comp(out_id, 0, 1, "boundary_cell_code", NULL);
   FLDset_node_null_data(out_id, 0, (void *)&null_value, out_data_type);
   FLDset_node_null_flag(out_id, 0, 1);
   FLDset_node_data     (out_id, 0, (char *)out_data[0], out_data_type,
                         out_ndata, OM_SET_ARRAY_FREE);

/* followed by al the other real node_data[] fields */

   for(i=1; i < out_comp_count; i++){
     FLDset_node_data_comp(out_id, i, out_veclen,
                           Fluent_Names[labels_indices[i]], NULL);

     FLDset_node_data     (out_id, i, (char *)out_data[i], out_data_type,
                           out_ndata, OM_SET_ARRAY_FREE);
   }

   return(1);
}

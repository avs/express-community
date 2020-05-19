/*
   Write VGF
   
   This module takes triangle cell sets from the input
   field structure, and writes them out to a file in 
   VGF - dVision ASCII file format.

   Inputs:
      filename - string for file to write
      in: field with on triangle cell_set and two node_data.
          primary design goal is isosurface module output

   Outputs:
      err: integer indicating read error (0 = no error, 1 = error)
      err_str: string indicating error condition, or "" for no error.

      ASCII file written.

   The V user interface macro maps this to a label area
   where visibility is controlled with the err int.

   Author: I. Curington 25 January, 1999
   Advanced Visual Systems, Inc.
   ianc@avs.com

   This module is implemented in C++.

   Updated 11 Feb 99, multiple input data types
   Updated 17 Feb 99, GD objects, ARR field arrays
*/

#include "xp_comm_proj/wr_vgf/gen.h"

#include "avs/gd_def.h"
#include "avs/f_utils.h"
#include "avs/math.h"
#include <stdlib.h>
#include <stdio.h>

#define CELL_TRI       4
#define CELL_QUAD      5
#define CELL_POLYTRI  10
#define CELL_POLYLINE  3

#define LINES_PER_STATUS 500

/*
 * uncomment the following line for messages
 * #define DEBUG
 */
/*
  #define DEBUG 
 */

   /***********************/
   /* OM Entry Point      */
   /***********************/
int
WriteVGF_WriteVGFMods_WriteVGFCore::upd(OMevent_mask event_mask, int seq_num)
{
   // filename (OMXstr read req notify)
   // in (Field read)

   FILE  *outfile;
   unsigned long int npolys;
   int   i, j, k, p;
   float *in_coords;
   float nf1, nf2, nf3;
   int   interrupt;
   float raw[20], rawnd[20];
   int   nnodes;
   int   nspace;
   int   cs, cell_type[100];
   int   veclen, nnode_data, nd, nd_id;
   float nd_min, nd_max, *node_data;
   int   *in_ncl;
   float t;
   float *xfm, *xlate;
   int   size;
   char  topname[80];
   char  groupname[80];
   int   nf;
   

   /***********************/
   /* Function's Body     */
   /***********************/

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"starting write vgf\n");
#endif
      strcpy(topname,top_name); // from input parameter block


      // ******************
      //  WRITE VGF HEADER
      // ******************

      if( (outfile = (FILE *)FILEfopen(filename,SIO_W_TXT)) == (FILE *) NULL  ) {
                err = 1;
                err_str = "write_vgf: could not open file for vgf write";
                return(1);
      }     

      //
      // write header
      //
      fprintf(outfile,"DIV-VIZ2\n");
      fprintf(outfile,"// AVS/Express generated VGF File\n");
      fprintf(outfile,"// http://www.avs.com\n//\n");
      fprintf(outfile,"HEADER\n(\n  VERSION=2:08\n");
      fprintf(outfile,"  DATE=02:02:99\n  TIME=00:00\n  UNIT=m\n");
      fprintf(outfile,")\n{\n}\n\nOBJECT (NAME=\"%s\")\n{\n", topname);


      /*
       *  Loop over all input fields
       */

      for (nf=0; nf < nobjs; nf++) 
      {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"### starting field ARR loop\n");
#endif

      interrupt = 0;
      nnodes = in[nf].nnodes;
      nspace = in[nf].nspace;

      // check we have found some facets
      if (nnodes == 0)
      {
         fprintf(outfile,"} // end of object\n");
         fprintf(outfile,"// ### End of File ###\n");
       	 fclose(outfile);
         err = 0;
	 return(1);
      }

      // out.coordinates.values
      // get coords array
      in_coords = (float*)in[nf].coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
      if (in_coords == NULL)
      {
         err = 1;
         err_str = "write vgf: problem assigning coordinates array";
	 fprintf(outfile," *** FILE WRITE STOPPED EARLY, COORD ACCESS ERROR! ***\n");
       	 fclose(outfile);
         return(1);
      }

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"alloced coords\n");
#endif

      // check if node data is present.
      nnode_data = in[nf].nnode_data;
      if (nnode_data > 0)
      {
           veclen = in[nf].node_data[0].veclen;
           nd_id = in[nf].node_data[0].id;
           node_data = (float *)
		in[nf].node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RD,DTYPE_FLOAT,&size );

	   nd_min = (float)in[nf].node_data[0].min;
	   nd_max = (float)in[nf].node_data[0].max;
      } else {
           veclen = 0;
           nd_id = 0;
      }

      // get the transformation for the field
      xfm   = (float*)in[nf].xform.mat.ret_array_ptr(OM_GET_ARRAY_RD);
      xlate = (float*)in[nf].xform.xlate.ret_array_ptr(OM_GET_ARRAY_RD);



      // supports only cell_sets of specific types,
      // so scan structure for exact matches.
      // after that, we can simply check the cell_type.

      cs = -1;
      for (i=0; i< in[nf].ncell_sets; i++)
      {
          // check for cell_set of type "Tri",
          if (        in[nf].cell_set[i].ncells > 0        &&
                      in[nf].cell_set[i].cell_ndim   == 2  &&
                      in[nf].cell_set[i].cell_nnodes == 3  &&
                      in[nf].cell_set[i].cell_order  == 1  &&
                      in[nf].cell_set[i].poly_flag   == 0 )
          {
		cell_type[i] = CELL_TRI;
		cs = i;

          // check for cell_set of type "Polytri",
          } else if ( in[nf].cell_set[i].ncells > 0        &&
                      in[nf].cell_set[i].cell_ndim   == 2  &&
                      in[nf].cell_set[i].cell_nnodes == 3  &&
                      in[nf].cell_set[i].cell_order  == 1  &&
                      in[nf].cell_set[i].poly_flag   == 1 )
          {
		cell_type[i] = CELL_POLYTRI;
		cs = i;

          // check for cell_set of type "Polyline",
          } else if ( in[nf].cell_set[i].ncells > 0        &&
                      in[nf].cell_set[i].cell_ndim   == 1  &&
                      in[nf].cell_set[i].cell_nnodes == 2  &&
                      in[nf].cell_set[i].cell_order  == 1  &&
                      in[nf].cell_set[i].poly_flag   == 1 )
          {
		cell_type[i] = CELL_POLYLINE;
		cs = i;

          // check for cell_set of type "Quad",
          } else if ( in[nf].cell_set[i].ncells > 0        &&
                      in[nf].cell_set[i].cell_ndim   == 2  &&
                      in[nf].cell_set[i].cell_nnodes == 4  &&
                      in[nf].cell_set[i].cell_order  == 1  &&
                      in[nf].cell_set[i].poly_flag   == 0 )
          {
		cell_type[i] = CELL_QUAD;
		cs = i;

          } else {
		cell_type[i] = -1;
              // skip to next, no type recognized
          }

      }
      if ( cs == -1 )
      {
             err = 1;
             err_str = "write vgf: no cell sets of the right type";
	 fprintf(outfile," *** FILE WRITE STOPPED EARLY! ***\n");
       	 fclose(outfile);
             return(1);
      }


#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"established valid cell set\n");
#endif

      // now that we have the data structure set up,
      // write the file contents

      fprintf(outfile,"  GEOGROUP\n\t(\n");
      fprintf(outfile,"\t\tNAME=\"%s%d\"\n", topname, nf);
      fprintf(outfile,"\t\tF_MATERIAL=\"cmap:A333333Dcccccc\"\n");
      fprintf(outfile,"\t\tB_MATERIAL=F_MATERIAL\n");

      if ( veclen == 1 || (veclen==2 && nd_id == GD_UV_DATA_ID) )
      {
	fprintf(outfile,"\t\tVERTEX=2D_TEXTURE\n\t)\n\t{");
      } else if ( veclen == 3 && nd_id == GD_NORMAL_DATA_ID ) {
	fprintf(outfile,"\t\tVERTEX=NORMALS\n\t)\n\t{");
      } else {
	veclen = 0;
      }

      // loop over all cell sets, processing for output
      for (cs=0; cs < in[nf].ncell_sets; cs++)
      {
          npolys = in[nf].cell_set[cs].ncells;

          // in.cell_set[cs].node_connect_list
          // get node connect list
	  if (cell_type[cs]==CELL_POLYTRI || cell_type[cs]==CELL_POLYLINE)
            in_ncl = (int*)in[nf].cell_set[cs].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
	  else
            in_ncl = (int*)in[nf].cell_set[cs].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);

          if (in_ncl == NULL)
          {
             if (in_coords) ARRfree((char*)in_coords);
             if (nnode_data > 0) ARRfree((char*)node_data);
		ARRfree((char*)xfm);
      		ARRfree((char*)xlate);
	     fclose(outfile);
             err = 1;
             err_str = "write_vgf: problem allocating node connect list";
             return(1);
          }

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"number of polys = %d\n",npolys);
#endif

	  // process each cell type in turn:
	  switch (cell_type[cs])
	  {
		case CELL_TRI:
		case CELL_QUAD:
			fprintf(outfile,"\t\tPMESH {\n\t\t  VERTEX_POOL {\n");

			// loop over vertices
      			for (i = 0; i < nnodes; i++)
      			{
         			raw[0] = in_coords[i*nspace+0];
         			raw[1] = in_coords[i*nspace+1];
         			if (nspace == 3)
             			  raw[2] = in_coords[i*nspace+2];
         			else
             			  raw[2] = 0.0;
				MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
				VEC_ADD(raw, raw, xlate);

				fprintf(outfile,"\t\t\t{ %f, %f, %f",
					raw[0], raw[1], raw[2]);

				if ( nnode_data > 0 )
				{
				    if (veclen==1)
				    {
					t = (node_data[i]-nd_min)/(nd_max - nd_min);
					fprintf(outfile, " ,%f, %f",t,t);
				    } else if (veclen==2) {
					fprintf(outfile,", %f, %f",
					  node_data[i*2], node_data[i*2+1]);
				    } else if (veclen==3) {
					fprintf(outfile,", %f, %f, %f",
					  node_data[i*3], node_data[i*3+1], node_data[i*3+2]);
				    }
				}
				fprintf(outfile," }\n");
			}
			fprintf(outfile,"\t\t} // end of vertex pool\n");
			if (cell_type[cs]==CELL_TRI)
				fprintf(outfile,"\t\t  CONNECTION_LIST {\n");
			else if (cell_type[cs]==CELL_QUAD)
				fprintf(outfile,"\t\t  CONNECTION_LIST (PCOUNT=4) {\n");

                        // loop over geometric primitives within this cell set
      			for (i = 0; i < npolys; i++)
      			{
				if (cell_type[cs]==CELL_TRI)
					fprintf(outfile,"\t\t\t{ %d, %d, %d }\n",
					  in_ncl[i*3], in_ncl[i*3+1], in_ncl[i*3+2]);
				else if (cell_type[cs]==CELL_QUAD)
					fprintf(outfile,"\t\t\t{ %d, %d, %d, %d }\n",
					  in_ncl[i*4],   in_ncl[i*4+1],
					  in_ncl[i*4+2], in_ncl[i*4+3]);

				// status and interrupt check
         			if ( (i%LINES_PER_STATUS)==0) {
           			OMstatus_check((int)(100*i/(float)npolys),
					"write vgf",&interrupt);
           			if (interrupt) {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got an interrupt, stopping");
#endif
                   			ARRfree((char*)in_coords);
                   			ARRfree((char*)in_ncl);
      					ARRfree((char*)xfm);
      					ARRfree((char*)xlate);
					fprintf(outfile," *** FILE WRITE INTERRUPTED! ***\n");
                   			fclose(outfile);
                   			return(1);
         			  }
         			}
      			} // end of primitive loop
			fprintf(outfile,"\t\t} // end of connection_list\n");
			fprintf(outfile,"\t\t} // end of PMESH\n");
			break;


		case CELL_POLYTRI:
		case CELL_POLYLINE:

          		npolys = in[nf].cell_set[cs].npolys;
                        // loop over geometric primitives within this cell set
      			for (p = 0; p < npolys; p++)
      			{
			   if (cell_type[cs]==CELL_POLYTRI)
				fprintf(outfile,"\tPOLYSTRIP {\n");
			   else if (cell_type[cs]==CELL_POLYLINE)
				fprintf(outfile,"\tLINE (THICKNESS=5) {\n");
			   else
				fprintf(outfile,"\tUNKNOWN-POLY {\n");


			   for (i=in_ncl[p*2]; i<=in_ncl[p*2+1]; i++)
			   {
         			raw[0] = in_coords[i*nspace+0];
         			raw[1] = in_coords[i*nspace+1];
         			if (nspace == 3)
             			  raw[2] = in_coords[i*nspace+2];
         			else
             			  raw[2] = 0.0;
				MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
				VEC_ADD(raw, raw, xlate);

				fprintf(outfile,"\t\t{ %f, %f, %f",
					raw[0], raw[1], raw[2]);

				if ( nnode_data > 0 && cell_type[cs]==CELL_POLYTRI)
				{
				    if (veclen==1)
				    {
					t = (node_data[i]-nd_min)/(nd_max - nd_min);
					fprintf(outfile, " ,%f, %f",t,t);
				    } else if (veclen==2) {
					fprintf(outfile,", %f, %f",
					  node_data[i*2], node_data[i*2+1]);
				    } else if (veclen==3) {
					fprintf(outfile,", %f, %f, %f",
					  node_data[i*3], node_data[i*3+1], node_data[i*3+2]);
				    }
				}
				fprintf(outfile," }\n");

				// status and interrupt check
         			if ( (i%LINES_PER_STATUS)==0) {
           			OMstatus_check((int)(100*i/(float)npolys),
					"write vgf",&interrupt);
           			if (interrupt) {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got an interrupt, stopping");
#endif
                   			ARRfree((char*)in_coords);
                   			ARRfree((char*)in_ncl);
      					ARRfree((char*)xfm);
      					ARRfree((char*)xlate);
					fprintf(outfile," *** FILE WRITE INTERRUPTED! ***\n");
                   			fclose(outfile);
                   			return(1);
         			  }
         			}
      			   } // end of vertex loop
      			} // end of primitive loop
    			fprintf(outfile,"\t\t} // end of poly\n");
			break;

		default:
			break;
	  } // end of switch

    } // end of cell set loop

    fprintf(outfile,"\t} // end of geogroup\n");

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"freeing memory for current field\n");
#endif

      // free arrays, communicating changes to OM
      ARRfree((char*)in_coords);
      ARRfree((char*)xfm);
      ARRfree((char*)xlate);
      ARRfree((char*)in_ncl);
      if (nnode_data > 0) ARRfree((char*)node_data);


    } // end of ARR field loop

    fprintf(outfile,"} // end of object\n");
    fprintf(outfile,"// ### End of File ###\n");


    // close the file, finished writing
    fclose(outfile);

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"file closed\n");
#endif


   // clean up user interface
   err = 0;
   err_str = "";

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"finished, returning\n\n");
#endif

   // return 1 for success
   return(1);
}
// =================== end of source file ================


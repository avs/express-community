/*
   Write Maya OBJ file 
   
   This module takes triangle cell sets from the input
   field structure, and writes them out to a file in 
   OBJ - Alias/Wavefront MAYA object ASCII file format.

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

  Conditions:

   This module is implemented in C++.

   Updated 11 Feb 99, multiple input data types
   Updated 17 Feb 99, GD objects, ARR field arrays
   Updated 20 July 99, derived from WriteMAYA module
   Updated 22 July 99, additional cell sets, group per set
   Updated  3 Aug 99, added degenerate triangle cull
*/

#include "xp_comm_proj/wr_maya/gen.h"

#include "avs/gd_def.h"
#include "avs/f_utils.h"
#include "avs/math.h"
#include "avs/port.h"
#include <stdlib.h>
#include <stdio.h>

#define CELL_TRI       4
#define CELL_QUAD      5
#define CELL_POLYTRI  10
#define CELL_POLYLINE  3
#define CELL_LINE      2

#define LINES_PER_STATUS 500

int check_degen(int i, float *in_coords, int nspace);

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
WriteMaya_WriteMayaMods_WriteMayaCore::update(OMevent_mask event_mask, int seq_num)
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
   int   gcount;
   

   /***********************/
   /* Function's Body     */
   /***********************/

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"starting write maya\n");
#endif
      strcpy(topname,top_name); // from input parameter block


      // ******************
      //  WRITE MAYA HEADER
      // ******************

      if( (outfile = (FILE *)FILEfopen(filename,SIO_W_TXT)) == (FILE *) NULL  ) {
                err = 1;
                err_str = "write_maya: could not open file for maya write";
                return(1);
      }     

      //
      // write header
      //
      fprintf(outfile,"# AVS/Express generated Alias/Wavefront MAYA OBJ File\n");
      fprintf(outfile,"# http://www.avs.com\n");
      fprintf(outfile,"# OBJECT name=\"%s\"\n\n", topname);
      fprintf(outfile,"mtllib master.mtl\n");


      /*
       *  Loop over all input fields
       */

      gcount = 0; // keep track of global node count in output stream

      for (nf=0; nf < nobjs; nf++) 
      {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"### starting field ARR loop\n");
#endif

      interrupt = 0;
      nnodes = in[nf].nnodes;
      nspace = in[nf].nspace;

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"### got nnodes and nspace\n");
#endif

      // check we have found some facets
      if (nnodes == 0)
      {
         fprintf(outfile,"# end of object\n");
         fprintf(outfile,"# End of File\n");
       	 fclose(outfile);
         err = 0;
	 return(1);
      }

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"### nnodes not zero\n");
#endif

      // out.coordinates.values
      // get coords array
      in_coords = (float*)in[nf].coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
      if (in_coords == NULL)
      {
         err = 1;
         err_str = "write maya: problem assigning coordinates array";
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

          // check for cell_set of type "Line",
          } else if ( in[nf].cell_set[i].ncells > 0        &&
                      in[nf].cell_set[i].cell_ndim   == 1  &&
                      in[nf].cell_set[i].cell_nnodes == 2  &&
                      in[nf].cell_set[i].cell_order  == 1  &&
                      in[nf].cell_set[i].poly_flag   == 0 )
          {
		cell_type[i] = CELL_LINE;
		cs = i;

          } else {
		cell_type[i] = -1;
              // skip to next, no type recognized
          }

      }
      if ( cs == -1 )
      {
             err = 1;
             err_str = "write maya: no cell sets of the right type";
	 fprintf(outfile," *** FILE WRITE STOPPED EARLY! ***\n");
       	 fclose(outfile);
             return(1);
      }


#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"established valid cell set\n");
#endif

      // now that we have the data structure set up,
      // write the file contents
      fprintf(outfile,"g %s_%d\n", topname, nf);

      if ( veclen == 1 || (veclen==2 && nd_id == GD_UV_DATA_ID) )
      {
	fprintf(outfile,"# object contains VERTEX=2D_TEXTURE\n");
      } else if ( veclen == 3 && nd_id == GD_NORMAL_DATA_ID ) {
	fprintf(outfile,"# object contains VERTEX=NORMALS\n");
      } else {
	veclen = 0;
      }


        fprintf(outfile,"# start of vertex list\n");
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

		fprintf(outfile,"v %f %f %f\n",
				raw[0], raw[1], raw[2]);

		if ( nnode_data > 0 )
		{
		    if (veclen==1)
		    {
			t = (node_data[i]-nd_min)/(nd_max - nd_min);
			fprintf(outfile, "vt %f %f 0.0\n",t,t);
		    } else if (veclen==2) {
			fprintf(outfile,"vt %f %f 0.0\n",
				node_data[i*2], node_data[i*2+1]);
		    } else if (veclen==3) {
			fprintf(outfile,"vn %f %f %f\n",
				node_data[i*3], node_data[i*3+1], node_data[i*3+2]);
		    }
		}
		/* fprintf(outfile,"# end of node\n"); */
	}
	fprintf(outfile,"# end of vertex list\n");
	

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
             err_str = "write_maya: problem allocating node connect list";
             return(1);
          }

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"number of polys = %d\n",npolys);
#endif

	  // delimit each cell set with a group name in file
	  // namestructure: g(group) objectname_f1_s1

          fprintf(outfile,"g %s_f%d_s%d\n", topname, nf, cs);

	  // process each cell type in turn:
	  switch (cell_type[cs])
	  {
		case CELL_LINE:
		case CELL_TRI:
		case CELL_QUAD:

			if (cell_type[cs]==CELL_TRI)
				fprintf(outfile,"#  start of triangle face list\n");
			else if (cell_type[cs]==CELL_QUAD)
				fprintf(outfile,"#  start of quad face list\n");
			else if (cell_type[cs]==CELL_LINE)
				fprintf(outfile,"#  start of disjoint line list\n");

                        // loop over geometric primitives within this cell set
      			for (i = 0; i < npolys; i++)
      			{
				if (cell_type[cs]==CELL_TRI)
					fprintf(outfile,"f %d %d %d\n",
					  (in_ncl[i*3]+1+gcount),
					  (in_ncl[i*3+1]+1+gcount),
					  (in_ncl[i*3+2]+1+gcount));
				else if (cell_type[cs]==CELL_QUAD)
					fprintf(outfile,"f %d %d %d %d\n",
					  (in_ncl[i*4]+1+gcount),
					  (in_ncl[i*4+1]+1+gcount),
					  (in_ncl[i*4+2]+1+gcount),
					  (in_ncl[i*4+3]+1+gcount));
				else if (cell_type[cs]==CELL_LINE)
					fprintf(outfile,"l %d %d\n",
					  (in_ncl[i*2]+1+gcount),
					  (in_ncl[i*2+1]+1+gcount));

				// status and interrupt check
         			if ( (i%LINES_PER_STATUS)==0) {
           			OMstatus_check((int)(100*i/(float)npolys),
					"write maya",&interrupt);
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
			fprintf(outfile,"# end of face list\n");
			break;


		case CELL_POLYTRI:
		case CELL_POLYLINE:

          		npolys = in[nf].cell_set[cs].npolys;
                        // loop over geometric primitives within this cell set
      			for (p = 0; p < npolys; p++)
      			{
			   if (cell_type[cs]==CELL_POLYTRI)
				fprintf(outfile,"# start of POLYTriSTRIP\n");
			   else if (cell_type[cs]==CELL_POLYLINE)
				fprintf(outfile,"# start of PolyLINE\n");


			   for (i=in_ncl[p*2]; i<=in_ncl[p*2+1]; i++)
			   {

				// write triangle or polyline connectivity
				// NOTE: will contain degenerates, no checking here...
			   	if (i <= in_ncl[p*2+1] - 2) {
			   	  if (cell_type[cs]==CELL_POLYTRI &&
				      check_degen(i,in_coords,nspace)) { 
			   	    if ((i-in_ncl[p*2])%2==1) // tristrip parity ordering
					fprintf(outfile,"f %d %d %d\n",
						i+1+gcount,i+2+gcount,i+3+gcount);
				    else
					fprintf(outfile,"f %d %d %d\n",
						i+3+gcount,i+2+gcount,i+1+gcount);
				  }
				}
			   	if (i <= in_ncl[p*2+1] - 1) {
			   	  if (cell_type[cs]==CELL_POLYLINE)
					fprintf(outfile,"l %d %d\n",
						i+1+gcount,i+2+gcount);
				}

				// status and interrupt check
         			if ( (i%LINES_PER_STATUS)==0) {
           			OMstatus_check((int)(100*i/(float)npolys),
					"write maya",&interrupt);
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
    			fprintf(outfile,"# end of poly\n");
			break;

		default:
			fprintf(outfile,"# ???? UNKNOWN-POLY\n");
			break;
	  } // end of switch

    } // end of cell set loop

    fprintf(outfile,"# end of cell set group\n");

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"freeing memory for current field\n");
#endif

      // free arrays, communicating changes to OM
      ARRfree((char*)in_coords);
      ARRfree((char*)xfm);
      ARRfree((char*)xlate);
      ARRfree((char*)in_ncl);
      if (nnode_data > 0) ARRfree((char*)node_data);


      // update global node count, across fields
      gcount += nnodes;

    } // end of ARR field loop

    fprintf(outfile,"# End of File\n");


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

// function to check for degenerate triangles
//   checks if any two coordinates are coincident
int
check_degen(int i, float *in_coords, int nspace) 
{

	float r1, r2, r3;
	int p, q;


        p = 0; q = 1; // first compare
	r1 =     _ABS( in_coords[(i+p)*nspace+0] - in_coords[(i+q)*nspace+0]) + 
	         _ABS( in_coords[(i+p)*nspace+1] - in_coords[(i+q)*nspace+1]);
        if (nspace == 3)
	   r1 += _ABS( in_coords[(i+p)*nspace+2] - in_coords[(i+q)*nspace+2]);


        p = 1; q = 2; // second compare
	r2 =     _ABS( in_coords[(i+p)*nspace+0] - in_coords[(i+q)*nspace+0]) + 
	         _ABS( in_coords[(i+p)*nspace+1] - in_coords[(i+q)*nspace+1]);
        if (nspace == 3)
	   r2 += _ABS( in_coords[(i+p)*nspace+2] - in_coords[(i+q)*nspace+2]);



        p = 0; q = 2; // third compare
	r3 =     _ABS( in_coords[(i+p)*nspace+0] - in_coords[(i+q)*nspace+0]) + 
	         _ABS( in_coords[(i+p)*nspace+1] - in_coords[(i+q)*nspace+1]);
        if (nspace == 3)
	   r3 += _ABS( in_coords[(i+p)*nspace+2] - in_coords[(i+q)*nspace+2]);

        if ( r1 <= ( 1.0e-12 ) ||  
             r2 <= ( 1.0e-12 ) ||  
             r3 <= ( 1.0e-12 ) ) 

		return(0);
	else
		return(1);
}


// =================== end of source file ================



/*
   Write POV 
   
   This module takes triangle cell sets from the input
   field structure, and writes them out to a file in 
   POV - Persistance-of-Vision Ray Tracer ASCII file format.
   To get your copy of POVray, to the web site
    http://www.povray.org

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

   Author: I. Curington 8 July, 1999
   Advanced Visual Systems, Inc.
   ianc@avs.com

   This module is implemented in C++.

   Updated 11 Feb 99, multiple input data types
   Updated 17 Feb 99, GD objects, ARR field arrays
   Updated  8 Feb 99, Derived POV version from Write VGF module
*/

#include "xp_comm_proj/wr_pov/gen.h"

#include "avs/gd_def.h"
#include "avs/f_utils.h"
#include "avs/math.h"
#include <stdlib.h>
#include <stdio.h>

#define CELL_TRI       4
#define CELL_QUAD      5
#define CELL_POLYTRI  10
#define CELL_POLYLINE  3
#define CELL_LINE      2

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
WritePov_WritePovMods_WritePovCore::update(OMevent_mask event_mask, int seq_num)
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
   int   npolynodes;
   

   /***********************/
   /* Function's Body     */
   /***********************/

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"starting write pov\n");
#endif
      strcpy(topname,top_name); // from input parameter block


      // ******************
      //  WRITE POV HEADER
      // ******************

      if( (outfile = (FILE *)FILEfopen(filename,SIO_W_TXT)) == (FILE *) NULL  ) {
                err = 1;
                err_str = "write_pov: could not open file for pov write";
                return(1);
      }     

      //
      // write header
      //
      fprintf(outfile,"// AVS/Express generated POV File\n");
      fprintf(outfile,"//         http://www.avs.com\n");
      fprintf(outfile,"// POV raytracer version 3.1 object description file\n");
      fprintf(outfile,"//         http://www.povray.org\n");
      fprintf(outfile,"// module version 1.0, July 8, 1999\n");
      fprintf(outfile,"// OBJECT NAME=\"%s\"\n\n", topname);

      //
      // write out some default scene setting definitions
      //  you will likely want to change these by hand once the 
      //  file is written, or in the future they could be
      //  either parameters into the module, or come from the
      //  express viewer system.
      //
      //  Note, there is only a small chance the camera will
      //  be looking at your object!
      //
      fprintf(outfile,"//## Start of Sample Camera & Light Data\n\n");
      fprintf(outfile,"#global_settings { assumed_gamma 2.2 }\n");
      fprintf(outfile,"#include \"colors.inc\"\n");
      fprintf(outfile,"#include \"shapes.inc\"\n\n");
      fprintf(outfile,"camera {\n");
      fprintf(outfile,"\tlocation <30,30,170>\n");
      fprintf(outfile,"\tlook_at <30,30,0>\n");
      fprintf(outfile,"\tangle 55\n}\n");
      fprintf(outfile,"light_source { <50, 50, 70> color White }\n");
      fprintf(outfile,"background { color SkyBlue }\n");
      fprintf(outfile,"#declare line_thickness = 0.5;\n\n");
      fprintf(outfile,"//## End of Sample Camera & Light Data\n\n");


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
         fprintf(outfile,"// end of field (null)\n");
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
         err_str = "write pov: problem assigning coordinates array";
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
             err_str = "write pov: no cell sets of the right type";
         fprintf(outfile," *** FILE WRITE STOPPED EARLY! ***\n");
         fclose(outfile);
             return(1);
      }


#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"established valid cell set\n");
#endif

      // now that we have the data structure set up,
      // write the file contents

      fprintf(outfile,"// Field %d, Name=\"%s\"\nunion {\n", nf, topname);


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
             err_str = "write_pov: problem allocating node connect list";
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

                        // loop over geometric primitives within this cell set
                        for (i = 0; i < npolys; i++)
                        {
                                if (cell_type[cs]==CELL_TRI)
                                {
                                        npolynodes = 3;
                                        fprintf(outfile,"triangle {\n");
                                }
                                else if (cell_type[cs]==CELL_QUAD)
                                {
                                        npolynodes = 4;
                                        fprintf(outfile,"polygon {\n\t4,\n");
                                }
                                for (j=0; j < npolynodes; j++) {
                                        k = in_ncl[i*npolynodes+j];
                                        raw[0] = in_coords[k*nspace+0];
                                        raw[1] = in_coords[k*nspace+1];
                                        if (nspace == 3)
                                                raw[2] = in_coords[k*nspace+2];
                                        else
                                                raw[2] = 0.0;
                                        MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
                                        VEC_ADD(raw, raw, xlate);
                                        fprintf(outfile,"\t< %f, %f, %f >",
                                                        raw[0], raw[1], raw[2]);
                                        if (npolynodes==4 && j < npolynodes-1)
                                                 fprintf(outfile,",\n");
                                        else
                                                 fprintf(outfile,"\n");
                                }
                                fprintf(outfile,"}\n");

                                // status and interrupt check
                                if ( (i%LINES_PER_STATUS)==0) {
                                OMstatus_check((int)(100*i/(float)npolys),
                                        "write pov",&interrupt);
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
                        fprintf(outfile,"// end of tri cell set\n\n");
                        break;

                case CELL_POLYLINE:

                        npolys = in[nf].cell_set[cs].npolys;

                        // loop over geometric primitives within this cell set
                        for (p = 0; p < npolys; p++)
                        {
                           fprintf(outfile,"// start of polyline cell set\n");

                           for (i=in_ncl[p*2]; i<=(in_ncl[p*2+1])-1; i++)
                           {
                                /* starting end of cylinder */
                                raw[0] = in_coords[i*nspace+0];
                                raw[1] = in_coords[i*nspace+1];
                                if (nspace == 3)
                                  raw[2] = in_coords[i*nspace+2];
                                else
                                  raw[2] = 0.0;
                                MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
                                VEC_ADD(raw, raw, xlate);

                                fprintf(outfile,"cylinder {\n");

                                fprintf(outfile,"\t< %f, %f, %f >,",
                                        raw[0], raw[1], raw[2]);

                                /* the other end of cylinder */
                                raw[0] = in_coords[(i+1)*nspace+0];
                                raw[1] = in_coords[(i+1)*nspace+1];
                                if (nspace == 3)
                                  raw[2] = in_coords[(i+1)*nspace+2];
                                else
                                  raw[2] = 0.0;
                                MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
                                VEC_ADD(raw, raw, xlate);


                                fprintf(outfile," < %f, %f, %f >\n",
                                        raw[0], raw[1], raw[2]);

                                fprintf(outfile,"\tline_thickness\n\topen\n");

                                fprintf(outfile,"}\n");

                                // status and interrupt check
                                if ( (i%LINES_PER_STATUS)==0) {
                                OMstatus_check((int)(100*i/(float)npolys),
                                        "write pov",&interrupt);
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
                        fprintf(outfile,"// end polyline\n");
                        break;

                case CELL_POLYTRI:

                        npolys = in[nf].cell_set[cs].npolys;

                        // loop over geometric primitives within this cell set
                        for (p = 0; p < npolys; p++)
                        {
                           fprintf(outfile,"// start of polytri cell set\n");

                           for (i=in_ncl[p*2]; i<=(in_ncl[p*2+1])-2; i++)
                           {
                                /* first vertex of triangle */
                                raw[0] = in_coords[i*nspace+0];
                                raw[1] = in_coords[i*nspace+1];
                                if (nspace == 3)
                                  raw[2] = in_coords[i*nspace+2];
                                else
                                  raw[2] = 0.0;
                                MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
                                VEC_ADD(raw, raw, xlate);

                                fprintf(outfile,"triangle {\n");

                                fprintf(outfile,"\t< %f, %f, %f >,",
                                        raw[0], raw[1], raw[2]);

                                /* middle point */
                                raw[0] = in_coords[(i+1)*nspace+0];
                                raw[1] = in_coords[(i+1)*nspace+1];
                                if (nspace == 3)
                                  raw[2] = in_coords[(i+1)*nspace+2];
                                else
                                  raw[2] = 0.0;
                                MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
                                VEC_ADD(raw, raw, xlate);

                                fprintf(outfile," < %f, %f, %f >,",
                                        raw[0], raw[1], raw[2]);

                                /* last point of triangle */
                                raw[0] = in_coords[(i+2)*nspace+0];
                                raw[1] = in_coords[(i+2)*nspace+1];
                                if (nspace == 3)
                                  raw[2] = in_coords[(i+2)*nspace+2];
                                else
                                  raw[2] = 0.0;
                                MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
                                VEC_ADD(raw, raw, xlate);

                                fprintf(outfile," < %f, %f, %f >\n",
                                        raw[0], raw[1], raw[2]);

                                fprintf(outfile,"}\n");

                                // status and interrupt check
                                if ( (i%LINES_PER_STATUS)==0) {
                                OMstatus_check((int)(100*i/(float)npolys),
                                        "write pov",&interrupt);
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
                        fprintf(outfile,"// end polytri\n");
                        break;

                case CELL_LINE:

                       // loop over geometric primitives within this cell set
                        for (i = 0; i < npolys; i++)
                        {
                                fprintf(outfile,"// start of disjoint line cell set\n");

                                j = in_ncl[i*2];    // pull connectivity
                                k = in_ncl[i*2+1];

                                /* starting end of cylinder */
                                raw[0] = in_coords[j*nspace+0];
                                raw[1] = in_coords[j*nspace+1];
                                if (nspace == 3)
                                  raw[2] = in_coords[j*nspace+2];
                                else
                                  raw[2] = 0.0;
                                MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
                                VEC_ADD(raw, raw, xlate);

                                fprintf(outfile,"cylinder {\n");

                                fprintf(outfile,"\t< %f, %f, %f >,",
                                        raw[0], raw[1], raw[2]);

                                /* the other end of cylinder */
                                raw[0] = in_coords[(k)*nspace+0];
                                raw[1] = in_coords[(k)*nspace+1];
                                if (nspace == 3)
                                  raw[2] = in_coords[(k)*nspace+2];
                                else
                                  raw[2] = 0.0;
                                MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
                                VEC_ADD(raw, raw, xlate);


                                fprintf(outfile," < %f, %f, %f >\n",
                                        raw[0], raw[1], raw[2]);

                                fprintf(outfile,"\tline_thickness\n\topen\n");

                                fprintf(outfile,"}\n");


                                // status and interrupt check
                                if ( (i%LINES_PER_STATUS)==0) {
                                OMstatus_check((int)(100*i/(float)npolys),
                                        "write pov",&interrupt);
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

                        fprintf(outfile,"// end line cell set\n");
                        break;

                default:
                        break;
          } // end of switch

         fprintf(outfile,"// end of cell set\n");

    } // end of cell set loop


#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"freeing memory for current field\n");
#endif

      // free arrays, communicating changes to OM
      ARRfree((char*)in_coords);
      ARRfree((char*)xfm);
      ARRfree((char*)xlate);
      ARRfree((char*)in_ncl);
      if (nnode_data > 0) ARRfree((char*)node_data);

      // set object rendering attributes per field
      // choose some arbitrary surface color and lighting characteristic
      fprintf(outfile,"\npigment { color Green }\n");
      fprintf(outfile,"finish {\n\tambient 0.2\n\tdiffuse 0.8\n}\n");
      fprintf(outfile,"} // end of field union\n");


    } // end of ARR field loop

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


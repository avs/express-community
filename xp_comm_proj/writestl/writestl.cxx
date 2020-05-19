
/*
   Write STL
   
   This module takes triangle cell sets from the input
   field structure, and writes them out to a file in 
   STL (Stereo Lithography) file format.


   Inputs:
      filename - string for file to write
      in: field with on triangle cell_set and two node_data.
          primary design goal is isosurface module output
      trigger: notifies module that it should begin processing
               of the Mesh.

   Outputs:
      err: integer indicating read error (0 = no error, 1 = error)
      err_str: string indicating error condition, or "" for no error.

   The V user interface macro maps this to a label area
   where visibility is controlled with the err int.

   Author: I. Curington 25 January, 1999
   Advanced Visual Systems, Inc.
   ianc@avs.com

   This module is implemented in C++.


*/

#include "xp_comm_proj/writestl/gen.hxx"
#include "avs/gd_def.h"
#include "avs/f_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


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
WriteSTL_WriteSTLMods_write_stl::update(OMevent_mask event_mask, int seq_num) {
   // filename (OMXstr read req notify)
   // in (Field read)

   FILE *outfile;
   unsigned long int ntri;
   int i, j;
   char line[256], tmpstr[256], token[256];
   float *in_coords;
   float nf1, nf2, nf3;
   int   interrupt;
   float raw_stl_bin[20];
   int   nnodes;
   int   nspace;
   unsigned char attrib[2];
   int   cs;

   // variables required to calculate tri normals
   float side_vec1[3];
   float side_vec2[3];
   float normal_len;


   /***********************/
   /* Function's Body     */
   /***********************/

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"starting write stl\n");
#endif

      if ( (!(trigger.valid_obj())) ||
           (!(filename.valid_obj())) )
      {
         // return 0 for failure
         err = 1;
         err_str = "write_stl: input parameters are not valid";
         return(1);
      }

      interrupt = 0;
      nnodes = in.nnodes;
      nspace = in.nspace;

      // check we have found some facets
      if (nnodes == 0)
      {
         err = 1;
         err_str = "write_stl: file does not contain any nodes";
      	 return(1);
      }

      // out.coordinates.values
      // get coords array
      in_coords = (float*)in.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
      if (in_coords == NULL)
      {
         err = 1;
         err_str = "write stl: problem assigning coordinates array";
         return(1);
      }

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"alloced coords\n");
#endif

      // supports only cell_set of type "Tri", search for the right one!

      cs = -1;
      for (i=0; i< in.ncell_sets; i++)
      {
          if ( in.cell_set[i].ncells < 1       ||
               in.cell_set[i].cell_ndim != 2   ||
               in.cell_set[i].cell_nnodes != 3 ||
               in.cell_set[i].cell_order != 1  ||
               in.cell_set[i].poly_flag != 0 )
          {
              // skip to next
          }
          else
          {
               cs = i;
          }

      }
      if ( cs == -1 )
      {
             err = 1;
             err_str = "write stl: no cell sets of the right type";
             return(1);

      }

      // in.cell_set[cs].node_connect_list
      // get node connect list
      int *in_ncl = (int*)in.cell_set[cs].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
      if (in_ncl == NULL)
      {
         if (in_coords) ARRfree((char*)in_coords);
         err = 1;
         err_str = "write_stl: problem allocating node connect list";
         return(1);
      }

      attrib[0] = attrib[1] = 0;
      ntri = in.cell_set[cs].ncells;


#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"established cell set\n");
#endif

      // now that we have the data structure set up,
      // write the file contents

      // ******************
      // BINARY STL WRITE 
      // ******************

      if( (outfile = (FILE *)FILEfopen(filename,SIO_W_BIN)) == (FILE *) NULL  ) {
                err = 1;
                err_str = "write_stl: could not open file for binary stl write";
                return(1);
      }     

      for (i=0; i<80; i++) line[i] = ' ';
      strcpy(line,"AVS/Express Binary STL Facet File");
      fwrite(line, sizeof(char),80,outfile);
      fwrite ((char *)(&ntri), sizeof(unsigned long int), 1, outfile);


#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"number of facets = %d\n",ntri);
#endif

      for (i = 0; i < ntri; i++)
      {

         if (nspace==3)
         {
            j = nspace * in_ncl[i*3];
            raw_stl_bin[3] = in_coords[j+0];
            raw_stl_bin[4] = in_coords[j+1];
            raw_stl_bin[5] = in_coords[j+2];

            j = nspace * in_ncl[i*3+1];
            raw_stl_bin[6] = in_coords[j+0];
            raw_stl_bin[7] = in_coords[j+1];
            raw_stl_bin[8] = in_coords[j+2];

            j = nspace * in_ncl[i*3+2];
            raw_stl_bin[9] = in_coords[j+0];
            raw_stl_bin[10] = in_coords[j+1];
            raw_stl_bin[11] = in_coords[j+2];
         }
         else
         {
            j = nspace * in_ncl[i*3];
            raw_stl_bin[3] = in_coords[j+0];
            raw_stl_bin[4] = in_coords[j+1];
            raw_stl_bin[5] = 0.0;

            j = nspace * in_ncl[i*3+1];
            raw_stl_bin[6] = in_coords[j+0];
            raw_stl_bin[7] = in_coords[j+1];
            raw_stl_bin[8] = 0.0;

            j = nspace * in_ncl[i*3+2];
            raw_stl_bin[9] = in_coords[j+0];
            raw_stl_bin[10] = in_coords[j+1];
            raw_stl_bin[11] = 0.0;
         }


         //calculate normals from triangle vertexes

         // get vectors along two edges of the triangle
         side_vec1[0] = raw_stl_bin[6] - raw_stl_bin[3];
         side_vec1[1] = raw_stl_bin[7] - raw_stl_bin[4];
         side_vec1[2] = raw_stl_bin[8] - raw_stl_bin[5];

         side_vec2[0] = raw_stl_bin[9] - raw_stl_bin[6];
         side_vec2[1] = raw_stl_bin[10] - raw_stl_bin[7];
         side_vec2[2] = raw_stl_bin[11] - raw_stl_bin[8];

         // calculate cross product
         raw_stl_bin[0] = side_vec1[1]*side_vec2[2] - side_vec1[2]*side_vec2[1];
         raw_stl_bin[1] = side_vec1[2]*side_vec2[0] - side_vec1[0]*side_vec2[2];
         raw_stl_bin[2] = side_vec1[0]*side_vec2[1] - side_vec1[1]*side_vec2[0];

         // normalize cross product result to get facet normal
         normal_len = sqrt( raw_stl_bin[0]*raw_stl_bin[0] + raw_stl_bin[1]*raw_stl_bin[1] + raw_stl_bin[2]*raw_stl_bin[2] );

         raw_stl_bin[0] /= normal_len;
         raw_stl_bin[1] /= normal_len;
         raw_stl_bin[2] /= normal_len;


         // write triangle data to file
         fwrite(raw_stl_bin,sizeof(float),12,outfile);

         // skip over short-int attribute block
         fwrite(attrib,  sizeof(char),2,outfile);


         if ( (i%2000)==0) {
           OMstatus_check((int)(100*i/(float)ntri),"write stl",&interrupt);
           if (interrupt) {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got an interrupt, stopping");
#endif
                   ARRfree((char*)in_coords);
                   ARRfree((char*)in_ncl);
                   fclose(outfile);
                   return(1);
           }
         }
      }

    // close the file, finished writing
    fclose(outfile);

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"file closed\n");
#endif

      // free arrays, communicating changes to OM
      ARRfree((char*)in_coords);
      ARRfree((char*)in_ncl);

   // clean up user interface
   err = 0;
   err_str = "";

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"mem free, finished\n");
#endif

   // return 1 for success
   return(1);
}
// =================== end of source file ================



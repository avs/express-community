/*
   Read SLP, SLA, STL

   This module parses a Pro/Engineer RENDER format file and puts it
   into a Field with one cell_set - a triangle one. The RENDER file
   tends to have the extension .SLP. As far as I know the RENDER
   format has only two data components - NORMAL and COLOR.
   These are both read and put into the node_data, since the data is
   based on the triangle vertices. I also assume there is only one solid
   in the file - I don't know if there can be more than one...

   This module is based on my AVS5 reader, except that I have attempted
   to do some error checking, and also have not hardcoded the maximum
   number of triangles that can be read in. These are now counted and
   malloc'ed.

   Inputs:
      filename: the name of the .SLP or .STL file to read.
   Outputs:
      out: field with on triangle cell_set and two node_data.
      err: integer indicating read error (0 = no error, 1 = error)
      err_str: string indicating error condition, or "" for no error.

   Jeremy Maccelari
   Visual Numerics (Pty) Ltd   ---   The Visualisation Experts
   Tel:   +27-11-468-2050 (after June/July 466-0319)
   Fax:   +27-11-468-2051 (after June/July 466-0320)
   email: visuals@solo.pipex.co.za
   WWW:   http://users.iafrica.com/v/vi/visuals/vn.html
   mail:  P.O. Box 11033           street: 9 Pannevis Road
          Vorna Valley                     Vorna Valley
          1686     R.S.A                   Midrand   R.S.A

   Renamed to "read_slp-stl" by I. Curington, July 97
   added parsing to cover both SLP and STL variations, 15 July 97 IC
   added error checking status, interrupt controls, and user interface.
   added binary file format for STL, 16 July 97, IC
   added f_utils.h,stdlib, stdio includes for clean compile, 17 July 97, JM
   changed FILEfopen() to SIO_R_BIN for binary file read, 22 July 97, JM
   changed printf() to ERRverror() so it works in external proc on PC, 22 July 97, JM
   moved into Templates lib for VMCD, xp_demo_2, Sept. 97, IC
   
*/

//#include "express.h"
#include "xp_comm_proj/rd_slpst/gen.h"
#include "avs/gd_def.h"
#include "avs/f_utils.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef XP_HAS_LONG
#define XP_HAS_LONG
typedef int xp_long;
typedef unsigned int xp_ulong;
#endif
#ifndef XP_HAS_BYTE
#define XP_HAS_BYTE
typedef unsigned char xp_byte;
#endif

int stl_detect_bin (FILE *infile, int *need_swap );

/*
 * uncomment the following line for messages
 * #define DEBUG
 */
// #define DEBUG 

/*
 * bigendian - littlendian swap macro (assume IEEE floating point)
 */
#define  BYTE_SWAP(a) { tbyte=(char *) &(a);   \
			tb=tbyte[0];           \
			tbyte[0]=tbyte[3];     \
			tbyte[3]=tb;           \
			tb=tbyte[1];           \
                        tbyte[1]=tbyte[2];     \
			tbyte[2]=tb; } 

   /***********************/
   /* OM Entry Point      */
   /***********************/

int
ReadSLPSTL_ReadSLPSTLMods_ReadSLPSTLFile::upd(OMevent_mask event_mask, int seq_num) {
   // filename (OMXstr read req notify)
   // out (Field write)

   FILE *infile;
   char solidname[256];
   float colour[3];
   xp_long i,j, norm,nnodes, vert;
   xp_long ntri;
   long ntr;
   int   stl_status;
   char line[256], tmpstr[256], token[256];
   char ds1[64], ds2[64];
   int  slp_mode, bin_mode;
   float *out_normals;
   float *out_colours;
   float *out_coords;
   float nf1, nf2, nf3;
   int   interrupt;
   char  *tbyte, tb;
   int   need_swap;
   float raw_stl_bin[20];
   

   /***********************/
   /* Function's Body     */
   /***********************/

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"starting slp\n");
#endif
   slp_mode = 0;
   interrupt = 0;

// make sure file is opened Binary
   if ((infile = (FILE *)FILEfopen(filename,SIO_R_BIN)) != (FILE *) NULL  )
   {
      if (stl_detect_bin(infile, &need_swap))
      {
            slp_mode = 0;
            bin_mode = 1;
      }
      else
      {
            bin_mode = 0;
            fclose(infile);
	    if( (infile = (FILE *)FILEfopen(filename,SIO_R_BIN)) == (FILE *) NULL  ) {
                out.nnodes = 0;
                err = 1;
                err_str = "read_slp: could not reopen file for binary stl access";
                return(1);
	    }     
      }     

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"need_swap = %d %d\n",need_swap,sizeof(long));
#endif
need_swap=0;
     // initialise ntri
      ntri = j = 0;
      // count facet blocks in file. As far as I know all facets are triangles. 
      if (bin_mode != 1)
      {
          while (!feof(infile))
          {
             fgets(line,256,infile);
             sscanf(line,"%s",ds1);
             if ( (strcmp(ds1,"facet") == 0) || (strcmp(ds1,"FACET") == 0) ) ntri++;
             if ( (strcmp(ds1,"color") == 0) || (strcmp(ds1,"COLOR") == 0) ) slp_mode = 1;
    
             if ( ((j++)%2000)==0) {
               OMstatus_check((j/2000)%100,"slp scan",&interrupt);
               if (interrupt) return(1);
             }
          }
      }
      else
      {
         stl_status = fread( line, sizeof(char),80,infile);
         stl_status = fread( &ntr, sizeof(int),1,infile);
	 ntri=(int)ntr;
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"b4 finished initial file scan\n");
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"number of facets = %d\n",ntri);
#endif
         if (need_swap) BYTE_SWAP( ntri );
      }

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"finished initial file scan\n");
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"number of facets = %d\n",ntri);
#endif

      // check we have found some facets
      if (ntri == 0)
      {
         out.nnodes = 0;
         fclose(infile);
         err = 1;
         err_str = "read_slp: file does not contain any facet lines";
	 return(1);
      }

      // initialise out field
      // init header
      nnodes = 3*ntri;

#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"nnodes = %d\n",nnodes);
#endif
      
      out.nnodes = nnodes; // three nodes per triangle
#ifdef DEBUG
//ERRverror("",ERR_PRINT|ERR_NO_HEADER,"out nnodes = %d\n",out.nnodes);
#endif
      out.nspace = 3;
      // out.coordinates.values
      // get coords array
      out_coords = (float*)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
      if (out_coords == NULL)
      {
         out.nnodes = 0;
         fclose(infile);
         err = 1;
         err_str = "read slp: problem allocating coordinates array";
         return(1);
      }
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"alloced coords\n");
#endif

      // only one cell_set of type Tri 
      out.ncell_sets = 1;
      out.cell_set[0].ncells = ntri;
      out.cell_set[0].cell_ndim = 2;
      out.cell_set[0].cell_nnodes = 3;
      out.cell_set[0].cell_order = 1;
      out.cell_set[0].poly_flag = 0;

      // out.cell_set[0].node_connect_list
      // get node connect list
      xp_long *out_ncl = (xp_long*)out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
      if (out_ncl == NULL)
      {
         fclose(infile);
         if (out_coords) ARRfree((char*)out_coords);
         out.nnodes = 0;
         err = 1;
         err_str = "read_slp: problem allocating node connect list";
         return(1);
      }

      for (i = 0; i < nnodes; i++) out_ncl[i] = i;
      ARRfree((char*)out_ncl);
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"established cell set\n");
#endif

      if (slp_mode)
      {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"SLP Mode\n");
#endif
          // init node_data:
          //   0 is normals, id = GD_NORMAL_DATA_ID, veclen = 3
          //   1 is colours, id = GD_COLOR_DATA_ID, veclen = 3
          out.nnode_data = 2;
          // out.node_data[0] - the normals
          out.node_data[0].id = GD_NORMAL_DATA_ID;
          out.node_data[0].veclen = 3;
          out_normals=
               (float*)out.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT);
          if (out_normals == NULL)
          {
             fclose(infile);
             if (out_coords) ARRfree((char*)out_coords);
             out.nnodes = 0;
             err = 1;
             err_str = "read_slp: problem allocating normals array";
             return(1);
          }
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"established node data for normals\n");
#endif

          // out.node_data[1]
          out.node_data[1].id = GD_COLOR_DATA_ID;
          out.node_data[1].veclen = 3;
          out_colours=
               (float*)out.node_data[1].values.ret_typed_array_ptr(OM_GET_ARRAY_WR ,OM_TYPE_FLOAT);
          if (out_colours == NULL)
          {
             fclose(infile);
             if (out_coords) ARRfree((char*)out_coords);
             if (out_normals) ARRfree((char*)out_normals);
             out.nnodes = 0;
             err = 1;
             err_str = "read_slp: problem allocating normals array";
             return(1);
          }
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"established node data for color\n");
#endif
      }
      else
      {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"STL Mode\n");
#endif
          out.nnode_data = 0;
      }

      // now that we have the data structure set up,
      // read in the file contents and populate the structure

    if ( bin_mode == 0 )
    {
      // ******************
      // ASCII Read
      // ******************

      // rewind file
      fseek(infile,0,0);

      // get solid line
      fgets(line,256,infile);
      sscanf(line,"%s %s",ds1,solidname);
      // check the first line is a solid line
      if ( (strcmp(ds1,"solid") != 0) && (strcmp(ds1,"SOLID") != 0) )
      {
         fclose(infile);
         if (out_coords) ARRfree((char*)out_coords);
         if (slp_mode)
         {
             if (out_normals) ARRfree((char*)out_normals);
             if (out_colours) ARRfree((char*)out_colours);
         }
         out.nnodes = 0;
         err = 1;
         err_str = "read_slp: file does not begin with a solid line";
	 return(1);
      }
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got solid\n");
#endif

      if (slp_mode)
      {
          /* get solid colors */
          fgets(line,256,infile);

          // the next line should be a colour line
          if ( ( strstr(line,"color") != NULL) || ( strstr(line,"COLOR") != NULL) )
          {
            sscanf(line,"%s %f %f %f",ds1,&colour[0],&colour[1],&colour[2]);
          }
          else
          {
             fclose(infile);
             if (out_coords) ARRfree((char*)out_coords);
             if (out_normals) ARRfree((char*)out_normals);
             if (out_colours) ARRfree((char*)out_colours);
             out.nnodes = 0;
             err = 1;
             err_str = "read_slp second line is not a colour";
	     return(1);
          }
      }

      /* solid loop */
      norm = vert = j = 0; 
      /* get first facet line */
      fgets(line,256,infile); 
      do {
         if (slp_mode)
         {
             // check for a facet
             if ( (strstr(line,"facet") == NULL) && (strstr(line,"FACET") == NULL) )
             {
	        fclose(infile);
                if (out_coords) ARRfree((char*)out_coords);
                if (slp_mode)
                {
                    if (out_normals) ARRfree((char*)out_normals);
                    if (out_colours) ARRfree((char*)out_colours);
                }
                out.nnodes = 0;
                err = 1;
                err_str = "read_slp: facet line expected";
                return(1);
             }
#ifdef DEBUG
// ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got SLP facet\n");
#endif

             /* normal loop */
             for (i = 0; i < 3; i++) // 3 normals per facet
             {
                fgets(line,256,infile);
                /* get normal data */
                sscanf(line,"%s %f %f %f\n", ds1,
                       &out_normals[norm*3+0],
                       &out_normals[norm*3+1],
                       &out_normals[norm*3+2]);
                if ( (strstr(ds1,"normal") == NULL) && (strstr(ds1,"NORMAL") == NULL) )
                {
                   fclose(infile);
                   if (out_coords)  ARRfree((char*)out_coords);
                   if (out_normals) ARRfree((char*)out_normals);
                   if (out_colours) ARRfree((char*)out_colours);
                   out.nnodes = 0;
                   err = 1;
                   err_str = "read_slp: normal line expected";
                   return(1);
                }
                norm++;
#ifdef DEBUG
// ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got 3 SLP normals\n");
#endif
             }
         }
         else
         {
             // check for a STL format facet
             if ( (strstr(line,"facet normal") == NULL) && (strstr(line,"FACET NORMAL") == NULL) )
             {
	              fclose(infile);
                if (out_coords) ARRfree((char*)out_coords);
                if (slp_mode)
                {
                    if (out_normals) ARRfree((char*)out_normals);
                    if (out_colours) ARRfree((char*)out_colours);
                }
                out.nnodes = 0;
                err = 1;
                err_str = "read_slp: stl facet normal line expected";
                return(1);
             }
             sscanf(line,"%s %s %f %f %f\n", ds1, ds2, &nf1, &nf2, &nf3);
             /*
              * normal not used at this point, maybe later...
              */
             norm++;
#ifdef DEBUG
// ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got STL normal\n");
#endif
         }

         fgets(line,256,infile);
         if ( (strstr(line,"outer loop") == NULL) && (strstr(line,"OUTER LOOP") == NULL) )
         {
            fclose(infile);
            if (out_coords)  ARRfree((char*)out_coords);
            if (slp_mode)
            {
                if (out_normals) ARRfree((char*)out_normals);
                if (out_colours) ARRfree((char*)out_colours);
            }
            out.nnodes = 0;
            err = 1;
            err_str = "read_slp: outer loop line expected";
            return(1);
         }
#ifdef DEBUG
// ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got outer loop\n");
#endif

         /* vertex loop */
         for (i = 0; i < 3; i++)
         {
            fgets(line,256,infile);
            /* get vertex data */
            sscanf(line,"%s %f %f %f",ds1,
                  &out_coords[vert*3+0],
                  &out_coords[vert*3+1],
                  &out_coords[vert*3+2]);
            if ( (strstr(ds1,"vertex") == NULL) && (strstr(ds1,"VERTEX") == NULL) )
            {
               fclose(infile);
               if (out_coords) ARRfree((char*)out_coords);
               if (slp_mode)
               {
                   if (out_normals) ARRfree((char*)out_normals);
                   if (out_colours) ARRfree((char*)out_colours);
               }
               out.nnodes = 0;
               err = 1;
               err_str = "read_slp: vertex line expected";
               return(1);
            }
            // set colours for this cell
            if (slp_mode)
            {
                out_colours[vert*3+0] = colour[0];
                out_colours[vert*3+1] = colour[1];
                out_colours[vert*3+2] = colour[2]; 
            };
            vert++;
         }
#ifdef DEBUG
// ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got verts\n");
#endif

         fgets(line,256,infile);
         if ( (strstr(line,"endloop") == NULL) && (strstr(line,"ENDLOOP") == NULL) )
         {
            fclose(infile);
            if (out_coords) ARRfree((char*)out_coords);
            if (slp_mode)
            {
                if (out_normals) ARRfree((char*)out_normals);
                if (out_colours) ARRfree((char*)out_colours);
            }
            out.nnodes = 0;
            err = 1;
            err_str = "read_slp: endloop line expected";
            return(1);
         }
#ifdef DEBUG
// ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got end loop\n");
#endif

         /* get endfacet */
         fgets(line,256,infile);
         if ( (strstr(line,"endfacet") == NULL) && (strstr(line,"ENDFACET") == NULL) )
         {
            fclose(infile);
            if (out_coords) ARRfree((char*)out_coords);
            if (slp_mode)
            {
                if (out_normals) ARRfree((char*)out_normals);
                if (out_colours) ARRfree((char*)out_colours);
            }
            out.nnodes = 0;
            err = 1;
            err_str = "read_slp: endfacet line expected";
            return(1);
         }
#ifdef DEBUG
// ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got end facet\n");
#endif

         /* get either facet or endsolid or color */
         fgets(line,256,infile);

         if (slp_mode)
         {
             /* check for new color */
             if ( (strstr(line,"color") != NULL) || (strstr(line,"COLOR") != NULL) )
             {
                sscanf(line,"%s %f %f %f",ds1,&colour[0],&colour[1],&colour[2]);
                // get facet line
                fgets(line,256,infile);
             }
         }

         if ( ((j++)%2000)==0) {
           OMstatus_check((100*j/ntri),"read slp",&interrupt);
           if (interrupt) return(1);
         }

      } while ( (strstr(line,"endsolid") == NULL) && (strstr(line,"ENDSOLID") == NULL) );
    }
    else
    {
      // ******************
      // BINARY STL Read
      // ******************
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"reading binary stl with %d tris\n",ntri);
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"file pos = %d",ftell(infile));
#endif

      for (j = 0; j < ntri; j++)
      {
         stl_status = fread(raw_stl_bin,sizeof(float),12,infile);
#ifdef DEBUG
//ERRverror("",ERR_PRINT|ERR_NO_HEADER,"stl_status = %d\n",stl_status);
#endif
         if (stl_status != 12) {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"stl_status = %d\n",stl_status);
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got to raw_stl_bin facet %d out of %d\n",j,ntri);
#endif
                   fclose(infile);
                   ARRfree((char*)out_coords);
                   out.nnodes = 0;
                   err = 1;
                   err_str = "read_slp: binary stl read ended early";
                   return(1);
         }

         if (need_swap) {
              for (i=0; i<12; i++ ) BYTE_SWAP( raw_stl_bin[i] );
         }
         nf1 = raw_stl_bin[0];
         nf2 = raw_stl_bin[1];
         nf3 = raw_stl_bin[2];

         out_coords[j*9+0] = raw_stl_bin[3];
         out_coords[j*9+1] = raw_stl_bin[4];
         out_coords[j*9+2] = raw_stl_bin[5];

         out_coords[j*9+3] = raw_stl_bin[6];
         out_coords[j*9+4] = raw_stl_bin[7];
         out_coords[j*9+5] = raw_stl_bin[8];

         out_coords[j*9+6] = raw_stl_bin[9];
         out_coords[j*9+7] = raw_stl_bin[10];
         out_coords[j*9+8] = raw_stl_bin[11];

         // skip over short-int attribute block
         stl_status = fread( line,  sizeof(char),2,infile);
         if (stl_status != 2) {
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"got to pad facet %d out of %d\n",j,ntri);
#endif
                   fclose(infile);
                   ARRfree((char*)out_coords);
                   out.nnodes = 0;
                   err = 1;
                   err_str = "read_slp: binary stl read ended early";
                   return(1);
         }


         if ( (j%2000)==0) {
           OMstatus_check((100*j/ntri),"read stl bin",&interrupt);
           if (interrupt) return(1);
         }
      }

    } // end of binary case

      fclose(infile);
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"file closed\n");
#endif

      // free arrays, communicating changes to OM
      if (out_coords) ARRfree((char*)out_coords);
      if (slp_mode)
      {
          if (out_normals) ARRfree((char*)out_normals);
          if (out_colours) ARRfree((char*)out_colours);
      }
   }
   else
   {
      out.nnodes = 0;
      err = 1;
      err_str = "read_slp: can't open input file";
      return(1);
   }
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"mem free\n");
#endif

   // clean up
   err = 0;
   err_str = "";

   // return 1 for success
   return(1);
}

/* check if file is ASCII or Binary */
int stl_detect_bin (FILE *infile, int *need_swap )
{
      int flag, stat, i;
      char head[1000];
      int ntris; 
      flag = 0;

      /* read 1st 1K chars */
      for (i=0; i<1000; i++) head[i] = '\0';
      stat = fread( head, sizeof(char),999,infile);
      /* blank the 1st 80 in case of header */
      for (i=0; i<80; i++) head[i] = ' ';

      /* search for common keyword */
      if ( (strstr(head,"vertex") != NULL) || (strstr(head,"VERTEX") != NULL) )
      {
            /* found keywords, assume ascii */
            flag = 0;
      }
      else
      {
            /* no keywords, assume binary */
            flag = 1;
      }

      *need_swap = 0;
      if (flag)
      {
          // rewind file
         fseek(infile,0,0);
         stat = fread( head,  sizeof(char),80,infile);
         stat = fread( &ntris, sizeof(int),1,infile);
#ifdef DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"chk number of facets = %d\n",ntris);
#endif

         if ((int)ntris < 0)
	 	*need_swap = 1;
	 
         if ((int)ntris > 50000000)
		*need_swap = 2;
		
      }

	
      // rewind file
      fseek(infile,0,0);

      return(flag);
}


/*
   This module reads in various Datamine .dm binary files and
   maps them onto AVS Field objects which can be viewed. The
   module uses my TDMFile and TDMVariable classes defined in
   dmfile.hxx.

   The .dm file format is developed by Mineral Industries 
   Computing Ltd of the UK for their Datamine product. Many
   thanks to Ben Heather of Datamine for giving me the
   specification for the file format. Their Web site is 
   www.datamine.co.uk.

   Inputs:
      filenames: an array of .dm files to read
      dir: the directory the files are in
      model_as_point: whether to read in a model file as
         blocks (hex cell set) = 0, or points = 1. Since the
         .dm file can easily contain a few hundred thousand
         blocks this will stop large models from killing your
         machine
      model_scale: scale factor for the model blocks to make
         them larger (>1.0) than actual size, or smaller (<1.0)
   
   Outputs:
      nFlds: the .dm files in AVS Field format. This is equal
         to the number of filenames passed
      fld[nFlds]: the fields. This module maps point, wireframe
         triangle, wireframe points, perimeter, drill hole and
         model (block) datasets onto AVS unstructured fields. All
         other types are ignored and the field set to 0.
      err: error flag
      err_str: error message

   Revisions:
      99-09-16: Written by J. Maccelari, visualn@iafrica.com
      99-05-03: moved loading to LoadFile, JM
      99-05-06: fixed wframetriangle node data section, JM
      99-05-06: added dir, JM
      00-06-15: added IAC_DEBUG preprocessor definitions, AWD
      00-06-15: added support for AVS/Express environment variables in filenames, AWD
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define XP_WIDE_API
#include <fld/Xfld.h>
#include <avs/f_utils.h>

#include "xp_comm_proj/rd_dmine/dm.hxx"
#include "xp_comm_proj/rd_dmine/dmfile.hxx"
#include "xp_comm_proj/rd_dmine/gen.hxx"

#define PI 3.141592654

int
ReadDatamine_ReadDatamineMods_ReadDatamineCore::update(OMevent_mask event_mask, int seq_num)
{
   // dir (OMXstr read req notify)
   // filenames (OMXstr_array read req notify)
   // model_as_point (OMXbool read req)
   // model_scale (OMXfloat read req)
   // nFlds (OMXint write)
   // fld (Field_array write)
   // err (OMXint write)
   // err_str (OMXstr write)
   xp_long d, f, v, n, *iptr, nd, nf, nl, nn;
   float a, b, *fptr, lxy, lx, ly, lz, length, ms, x, y, z;
   FILE *in;
   char *dname, *fname, *fnameptr, msg[1024], tmpstr[129];
   TDMFile *DMFile, *PtsFile;

   char dir_buf[AVS_PATH_MAX];

   
   /***********************/
   /* Function's Body     */
   /***********************/
   // ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: ReadDatamine_ReadDatamineMods_ReadDatamineCore::update\n");

   // reset errors
   err = 0;
   err_str = "";
   nFlds = (int)0;

   // check float size
   if (sizeof(float) != SIZE_OF_FLOAT)
   {
      sprintf(msg,"Read DM: incorrect size of float");
      err = 1;
      err_str = msg;
      return(1);
   }

   if (!dir.valid_obj())
   {
      sprintf(msg,"Read DM: dir invalid");
      err = 1;
      err_str = msg;
      return(1);
   }

   // Use AVS/Express function to map enviroment variables
   FILEmap_variables((char *)dir, dir_buf);

   dname = (char*)malloc((strlen((char*)dir_buf)+8)*sizeof(char));
   strcpy(dname, (char*)dir_buf);

   if (!filenames.valid_obj())
   {
      sprintf(msg,"Read DM: filenames invalid");
      err = 1;
      err_str = msg;
      if (dname) free((char*)dname);
      return(1);
   }

   if (filenames.get_array_size(&nf) != OM_STAT_SUCCESS)
   {
      sprintf(msg,"Read DM: cannot get number of filenames");
      err = 1;
      err_str = msg;
      if (dname) free((char*)dname);
      return(1);
   }

   if (nf <= 0)
   {
      sprintf(msg,"Read DM: no filenames");
      err = 1;
      err_str = msg;
      if (dname) free((char*)dname);
      return(1);
   }

   
#ifdef IAC_DEBUG
   fprintf(stdout,"%d fields\n",nf);
#endif
   nFlds = (int)nf;

   // read each file in
   for (f = 0; f < nf; f++)
   {
      fname = NULL;
      d = OMget_str_array_val((OMobj_id)filenames, f, &fname, 0);
      if (d!=OM_STAT_SUCCESS) {
         if (fname) free((char*)fname);
         nFlds = 0;
         sprintf(msg,"Read DM: cannot get element %i of filename array", f);
         err = 1;
         err_str = msg;
         if (dname) free((char*)dname);
         return(1);
      }

      fnameptr = (char*)malloc((strlen(dname)+strlen(fname)+8)*sizeof(char));
#ifdef MSDOS
      sprintf(fnameptr,"%s\\%s",dname,fname);
#else
      sprintf(fnameptr,"%s/%s",dname,fname);
#endif
      if (fname) free((char*)fname);
      fname = (char*)malloc((strlen(fnameptr)+8)*sizeof(char));
      strcpy(fname,fnameptr);
      if (fnameptr) free((char*)fnameptr);
#ifdef IAC_DEBUG
      fprintf(stdout,"Opening %s\n",fname);
#endif
      if ((in = fopen(fname,"rb")) == NULL)
      {
         if (fname) free((char*)fname);
         nFlds = 0;
         sprintf(msg,"Read DM: cannot open file %s",fname);
         err = 1;
         err_str = msg;
         if (dname) free((char*)dname);
         return(1);
      }
      fclose(in);
   
      // create new DM file
      DMFile = new TDMFile;
      // fill it
      DMFile->LoadFile(fname); 

      // If the file is a wireframe triangle, read in the points file which is the filename less the last two
      // letters and with pt added, so fredtr becomes fredpt
      // Dont' worry about clicking on a wireframe points file, since this will just read in the points
      // without the connectivity. We don't have a rule to get the triangle file.
#ifdef IAC_DEBUG
      fprintf(stdout,"Checking file type\n");
#endif
      if (DMFile->GetFileType() == wframetriangle)
      {
         char *ptr, ptsname[1024];
   
         strcpy(ptsname,fname);
         ptr = strstr(ptsname,".dm");
         *(ptr-1) = 't';
         *(ptr-2) = 'p';
#ifdef IAC_DEBUG
         fprintf(stdout,"Read DM: opening wireframe points file %s\n",ptsname);
#endif
         if ((in = fopen(ptsname,"rb")) == NULL)
         {
#ifdef IAC_DEBUG
            fprintf(stdout,"Read DM: cannot open wireframe points file %s\n",ptsname);
#endif
            if (dname) free((char*)dname);
            return(1);
         }
         fclose(in);
         PtsFile = new TDMFile;
         PtsFile->LoadFile(ptsname);
      }
      else
         PtsFile = NULL;

      // fill the field
#ifdef IAC_DEBUG
      fprintf(stdout,"Filling field\n");
#endif
      switch (DMFile->GetFileType())
      {
         case perimeter: // line mesh
            DEBUG_PRINT("perimeter");
            // do coords
            fld[f].nnodes = nn = DMFile->Vars[0].GetNData();
            fld[f].nspace = 3;
            fptr = (float*)fld[f].coordinates.values.ret_typed_array_ptr(
               OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
            for (n = 0; n < nn; n++)
            {
               for (v = 0; v < DMFile->nVars; v++)
               {
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"XP      ") == 0)
                     fptr[n*3+0] = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"YP      ") == 0)
                     fptr[n*3+1] = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"ZP      ") == 0)
                     fptr[n*3+2] = DMFile->Vars[v].fData[n];
               }
            }
            if (fptr) ARRfree((char*)fptr);
            // now do line cell set
            fld[f].ncell_sets = 1;
            fld[f].cell_set[0].cell_ndim = 1;
            fld[f].cell_set[0].cell_nnodes = 2;
            fld[f].cell_set[0].cell_order = 1;
            fld[f].cell_set[0].poly_flag = 0;
            // count segments
            for (v = 0; v < DMFile->nVars; v++)
               if (strcmp(DMFile->Vars[v].GetName(tmpstr),"PVALUE  ") == 0)
                  break;
            for (n = 1, nl = 0; n < nn; n++)
               if (DMFile->Vars[v].fData[n] == DMFile->Vars[v].fData[n-1])
                  nl++;
#ifdef IAC_DEBUG
            fprintf(stdout,"nlines = %d\n",nl);
#endif
            fld[f].cell_set[0].ncells = nl;
            iptr = (xp_long*)fld[f].cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
            for (n = 1, nl = 0; n < nn; n++)
               if (DMFile->Vars[v].fData[n] == DMFile->Vars[v].fData[n-1])
               {
                  iptr[2*nl+0] = n-1;
                  iptr[2*nl+1] = n;
                  nl++;
               }
            if (iptr) ARRfree((char*)iptr);
            // now do data
            // count numeric datasets
            for (v = 0, nd = 0; v < DMFile->nVars; v++)
               if (DMFile->Vars[v].TypeIsNumerical())
                  nd++;
            fld[f].nnode_data = nd;
            for (v = 0, d = 0; v < DMFile->nVars; v++)
            {
               if (DMFile->Vars[v].TypeIsNumerical())
               {
                  fld[f].node_data[d].veclen = 1;
                  fld[f].node_data[d].labels = DMFile->Vars[v].GetName(tmpstr);
                  fptr = (float*)fld[f].node_data[d].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
                  for (n = 0; n < nn; n++)
                     fptr[n] = DMFile->Vars[v].fData[n];
                  if (fptr) ARRfree((char*)fptr);
                  d++;
               }
            }
            break;
         case blockmodel: // point mesh - use scat_to_grid + hex glyph to get
                          // boxes
            DEBUG_PRINT("blockmodel");
            if ((int)model_as_point == 0)
            {
               // do coords
               ms = (float) model_scale;
               fld[f].nnodes = nn = 8*DMFile->Vars[0].GetNData();
               fld[f].nspace = 3;
               fptr = (float*)fld[f].coordinates.values.ret_typed_array_ptr(
                  OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
               for (n = 0; n < nn/8; n++)
               {
                  for (v = 0; v < DMFile->nVars; v++)
                  {
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"XC      ") == 0)
                        x = DMFile->Vars[v].fData[n];
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"XINC    ") == 0)
                        lx = DMFile->Vars[v].fData[n];
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"YC      ") == 0)
                        y = DMFile->Vars[v].fData[n];
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"YINC    ") == 0)
                        ly = DMFile->Vars[v].fData[n];
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"ZC      ") == 0)
                        z = DMFile->Vars[v].fData[n];
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"ZINC    ") == 0)
                        lz = DMFile->Vars[v].fData[n];
                  }
                  fptr[n*3*8+0] = x - ms*lx/2.;
                  fptr[n*3*8+1] = y - ms*ly/2.;
                  fptr[n*3*8+2] = z + ms*lz/2.;
                  fptr[n*3*8+3] = x + ms*lx/2.;
                  fptr[n*3*8+4] = y - ms*ly/2.;
                  fptr[n*3*8+5] = z + ms*lz/2.;
                  fptr[n*3*8+6] = x + ms*lx/2.;
                  fptr[n*3*8+7] = y + ms*ly/2.;
                  fptr[n*3*8+8] = z + ms*lz/2.;
                  fptr[n*3*8+9] = x - ms*lx/2.;
                  fptr[n*3*8+10] = y + ms*ly/2.;
                  fptr[n*3*8+11] = z + ms*lz/2.;
                  fptr[n*3*8+12] = x - ms*lx/2.;
                  fptr[n*3*8+13] = y - ms*ly/2.;
                  fptr[n*3*8+14] = z - ms*lz/2.;
                  fptr[n*3*8+15] = x + ms*lx/2.;
                  fptr[n*3*8+16] = y - ms*ly/2.;
                  fptr[n*3*8+17] = z - ms*lz/2.;
                  fptr[n*3*8+18] = x + ms*lx/2.;
                  fptr[n*3*8+19] = y + ms*ly/2.;
                  fptr[n*3*8+20] = z - ms*lz/2.;
                  fptr[n*3*8+21] = x - ms*lx/2.;
                  fptr[n*3*8+22] = y + ms*ly/2.;
                  fptr[n*3*8+23] = z - ms*lz/2.;
               }
               if (fptr) ARRfree((char*)fptr);
               // now do point cell set
               fld[f].ncell_sets = 1;
               fld[f].cell_set[0].ncells = nn/8;
               fld[f].cell_set[0].cell_ndim = 3;
               fld[f].cell_set[0].cell_nnodes = 8;
               fld[f].cell_set[0].cell_order = 1;
               fld[f].cell_set[0].poly_flag = 0;
               iptr = (xp_long*)fld[f].cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
               for (n = 0; n < nn/8; n++)
               {
                  iptr[n*8+0] = n*8+0;
                  iptr[n*8+1] = n*8+1;
                  iptr[n*8+2] = n*8+2;
                  iptr[n*8+3] = n*8+3;
                  iptr[n*8+4] = n*8+4;
                  iptr[n*8+5] = n*8+5;
                  iptr[n*8+6] = n*8+6;
                  iptr[n*8+7] = n*8+7;
               }
               if (iptr) ARRfree((char*)iptr);
               // now do data
               // count numeric datasets
               for (v = 0, nd = 0; v < DMFile->nVars; v++)
                  if (DMFile->Vars[v].TypeIsNumerical())
                     nd++;
               fld[f].nnode_data = nd;
               for (v = 0, d = 0; v < DMFile->nVars; v++)
               {
                  if (DMFile->Vars[v].TypeIsNumerical())
                  {
                     fld[f].node_data[d].veclen = 1;
                     fld[f].node_data[d].labels = DMFile->Vars[v].GetName(tmpstr);
                     fptr = (float*)fld[f].node_data[d].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
                     for (n = 0; n < nn/8; n++)
                     {
                        fptr[n*8+0] = DMFile->Vars[v].fData[n];
                        fptr[n*8+1] = DMFile->Vars[v].fData[n];
                        fptr[n*8+2] = DMFile->Vars[v].fData[n];
                        fptr[n*8+3] = DMFile->Vars[v].fData[n];
                        fptr[n*8+4] = DMFile->Vars[v].fData[n];
                        fptr[n*8+5] = DMFile->Vars[v].fData[n];
                        fptr[n*8+6] = DMFile->Vars[v].fData[n];
                        fptr[n*8+7] = DMFile->Vars[v].fData[n];
                     }
                     if (fptr) ARRfree((char*)fptr);
                     d++;
                  }
               }
            }
            else
            {
               // do coords
               fld[f].nnodes = nn = DMFile->Vars[0].GetNData();
               fld[f].nspace = 3;
               fptr = (float*)fld[f].coordinates.values.ret_typed_array_ptr(
                  OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
               for (n = 0; n < nn; n++)
               {
                  for (v = 0; v < DMFile->nVars; v++)
                  {
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"XC      ") == 0)
                        x = DMFile->Vars[v].fData[n];
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"YC      ") == 0)
                        y = DMFile->Vars[v].fData[n];
                     if (strcmp(DMFile->Vars[v].GetName(tmpstr),"ZC      ") == 0)
                        z = DMFile->Vars[v].fData[n];
                  }
                  fptr[n*3+0] = x;
                  fptr[n*3+1] = y;
                  fptr[n*3+2] = z;
               }
               if (fptr) ARRfree((char*)fptr);
               // now do point cell set
               fld[f].ncell_sets = 1;
               fld[f].cell_set[0].ncells = nn;
               fld[f].cell_set[0].cell_ndim = 0;
               fld[f].cell_set[0].cell_nnodes = 1;
               fld[f].cell_set[0].cell_order = 1;
               fld[f].cell_set[0].poly_flag = 0;
               iptr = (xp_long*)fld[f].cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
               for (n = 0; n < nn; n++)
                  iptr[n] = n;
               if (iptr) ARRfree((char*)iptr);
               // now do data
               // count numeric datasets
               for (v = 0, nd = 0; v < DMFile->nVars; v++)
                  if (DMFile->Vars[v].TypeIsNumerical())
                     nd++;
               fld[f].nnode_data = nd;
               for (v = 0, d = 0; v < DMFile->nVars; v++)
               {
                  if (DMFile->Vars[v].TypeIsNumerical())
                  {
                     fld[f].node_data[d].veclen = 1;
                     fld[f].node_data[d].labels = DMFile->Vars[v].GetName(tmpstr);
                     fptr = (float*)fld[f].node_data[d].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
                     for (n = 0; n < nn; n++)
                        fptr[n] = DMFile->Vars[v].fData[n];
                     if (fptr) ARRfree((char*)fptr);
                     d++;
                  }
               }
             }
            break;
         case point: // point mesh
            DEBUG_PRINT("point");
            // do coords
            fld[f].nnodes = nn = DMFile->Vars[0].GetNData();
            fld[f].nspace = 3;
            fptr = (float*)fld[f].coordinates.values.ret_typed_array_ptr(
               OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
            for (n = 0; n < nn; n++)
            {
               for (v = 0; v < DMFile->nVars; v++)
               {
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"XPT     ") == 0)
                     fptr[n*3+0] = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"YPT     ") == 0)
                     fptr[n*3+1] = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"ZPT     ") == 0)
                     fptr[n*3+2] = DMFile->Vars[v].fData[n];
               }
            }
            if (fptr) ARRfree((char*)fptr);
            // now do point cell set
            fld[f].ncell_sets = 1;
            fld[f].cell_set[0].ncells = nn;
            fld[f].cell_set[0].cell_ndim = 0;
            fld[f].cell_set[0].cell_nnodes = 1;
            fld[f].cell_set[0].cell_order = 1;
            fld[f].cell_set[0].poly_flag = 0;
            iptr = (xp_long*)fld[f].cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
            for (n = 0; n < nn; n++)
               iptr[n] = n;
            if (iptr) ARRfree((char*)iptr);
            // now do data
            // count numeric datasets
            for (v = 0, nd = 0; v < DMFile->nVars; v++)
               if (DMFile->Vars[v].TypeIsNumerical())
                  nd++;
            fld[f].nnode_data = nd;
            for (v = 0, d = 0; v < DMFile->nVars; v++)
            {
               if (DMFile->Vars[v].TypeIsNumerical())
               {
                  fld[f].node_data[d].veclen = 1;
                  fld[f].node_data[d].labels = DMFile->Vars[v].GetName(tmpstr);
                  fptr = (float*)fld[f].node_data[d].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
                  for (n = 0; n < nn; n++)
                     fptr[n] = DMFile->Vars[v].fData[n];
                  if (fptr) ARRfree((char*)fptr);
                  d++;
               }
            }
            break;
         case wframetriangle: // triangle mesh
            DEBUG_PRINT("wireframe");
            // do coords from wframepoints
#ifdef IAC_DEBUG
            fprintf(stdout,"doing coords\n");
#endif
            fld[f].nnodes = nn = PtsFile->Vars[0].GetNData();
#ifdef IAC_DEBUG
            fprintf(stdout,"doing %d coords\n", nn);
#endif
            fld[f].nspace = 3;
            fptr = (float*)fld[f].coordinates.values.ret_typed_array_ptr(
               OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
            for (n = 0; n < nn; n++)
            {
               for (v = 0; v < PtsFile->nVars; v++)
               {
                  if (strcmp(PtsFile->Vars[v].GetName(tmpstr),"XP      ") == 0)
                     fptr[n*3+0] = PtsFile->Vars[v].fData[n];
                  if (strcmp(PtsFile->Vars[v].GetName(tmpstr),"YP      ") == 0)
                     fptr[n*3+1] = PtsFile->Vars[v].fData[n];
                  if (strcmp(PtsFile->Vars[v].GetName(tmpstr),"ZP      ") == 0)
                     fptr[n*3+2] = PtsFile->Vars[v].fData[n];
               }
            }
            if (fptr) ARRfree((char*)fptr);
            // now do tri cell set from wframetriangle
#ifdef IAC_DEBUG
            fprintf(stdout,"doing cellset\n");
#endif
            nn = DMFile->Vars[0].GetNData();
            fld[f].ncell_sets = 1;
            fld[f].cell_set[0].ncells = nn;
            fld[f].cell_set[0].cell_ndim = 2;
            fld[f].cell_set[0].cell_nnodes = 3;
            fld[f].cell_set[0].cell_order = 1;
            fld[f].cell_set[0].poly_flag = 0;
            iptr = (xp_long*)fld[f].cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
            for (n = 0; n < nn; n++)
            {
               for (v = 0; v < DMFile->nVars; v++)
               {
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"PID1    ") == 0)
                     iptr[n*3+0] = (xp_long)DMFile->Vars[v].fData[n] - 1;
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"PID2    ") == 0)
                     iptr[n*3+1] = (xp_long)DMFile->Vars[v].fData[n] - 1;
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"PID3    ") == 0)
                     iptr[n*3+2] = (xp_long)DMFile->Vars[v].fData[n] - 1;
               }
            }
            if (iptr) ARRfree((char*)iptr);
            // do cell data
            // count numeric datasets
#ifdef IAC_DEBUG
            fprintf(stdout,"doing cell data\n");
#endif
            for (v = 0, nd = 0; v < DMFile->nVars; v++)
               if (DMFile->Vars[v].TypeIsNumerical())
                  nd++;
#ifdef IAC_DEBUG
            fprintf(stdout,"doing %d cell data\n",nd);
#endif
            fld[f].cell_set[0].ncell_data = nd;
            for (v = 0, d = 0; v < DMFile->nVars; v++)
            {
               if (DMFile->Vars[v].TypeIsNumerical())
               {
                  // fprintf(stdout,"doing cell data %d\n",d);
                  fld[f].cell_set[0].cell_data[d].veclen = 1;
                  fld[f].cell_set[0].cell_data[d].labels = DMFile->Vars[v].GetName(tmpstr);
                  fptr = (float*)fld[f].cell_set[0].cell_data[d].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
                  for (n = 0; n < nn; n++)
                     fptr[n] = DMFile->Vars[v].fData[n];
                  if (fptr) ARRfree((char*)fptr);
                  d++;
               }
            }
            // do node data
            // count numeric datasets
#ifdef IAC_DEBUG
            fprintf(stdout,"doing node data\n");
#endif
            for (v = 0, nd = 0; v < PtsFile->nVars; v++)
               if (PtsFile->Vars[v].TypeIsNumerical())
                  nd++;
#ifdef IAC_DEBUG
            fprintf(stdout,"doing %d node data\n",nd);
#endif
            nn = PtsFile->Vars[0].GetNData();
            fld[f].nnode_data = nd;
            for (v = 0, d = 0; v < PtsFile->nVars; v++)
            {
               if (PtsFile->Vars[v].TypeIsNumerical())
               {
                  // fprintf(stdout,"doing node data %d\n",d);
                  fld[f].node_data[d].veclen = 1;
                  // fprintf(stdout,"doing node data %d labels\n",d);
                  fld[f].node_data[d].labels = PtsFile->Vars[v].GetName(tmpstr);
                  fptr = (float*)fld[f].node_data[d].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
                  // fprintf(stdout,"doing %d node data %d data\n",nn,d);
                  for (n = 0; n < nn; n++)
                     fptr[n] = PtsFile->Vars[v].fData[n];
                  // fprintf(stdout,"done node data %d data\n",d);
                  if (fptr) ARRfree((char*)fptr);
                  d++;
               }
            }
#ifdef IAC_DEBUG
            // fprintf(stdout,"done\n");
#endif
            break;
         case wframepoints: // point mesh
            DEBUG_PRINT("wireframepoints");
            // do coords
            fld[f].nnodes = nn = DMFile->Vars[0].GetNData();
            fld[f].nspace = 3;
            fptr = (float*)fld[f].coordinates.values.ret_typed_array_ptr(
               OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
            for (n = 0; n < nn; n++)
            {
               for (v = 0; v < DMFile->nVars; v++)
               {
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"XP      ") == 0)
                     fptr[n*3+0] = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"YP      ") == 0)
                     fptr[n*3+1] = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"ZP      ") == 0)
                     fptr[n*3+2] = DMFile->Vars[v].fData[n];
               }
            }
            if (fptr) ARRfree((char*)fptr);
            // now do point cell set
            fld[f].ncell_sets = 1;
            fld[f].cell_set[0].ncells = nn;
            fld[f].cell_set[0].cell_ndim = 0;
            fld[f].cell_set[0].cell_nnodes = 1;
            fld[f].cell_set[0].cell_order = 1;
            fld[f].cell_set[0].poly_flag = 0;
            iptr = (xp_long*)fld[f].cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
            for (n = 0; n < nn; n++)
               iptr[n] = n;
            if (iptr) ARRfree((char*)iptr);
            // now do data
            // count numeric datasets
            for (v = 0, nd = 0; v < DMFile->nVars; v++)
               if (DMFile->Vars[v].TypeIsNumerical())
                  nd++;
            fld[f].nnode_data = nd;
            for (v = 0, d = 0; v < DMFile->nVars; v++)
            {
               if (DMFile->Vars[v].TypeIsNumerical())
               {
                  fld[f].node_data[d].veclen = 1;
                  fld[f].node_data[d].labels = DMFile->Vars[v].GetName(tmpstr);
                  fptr = (float*)fld[f].node_data[d].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
                  for (n = 0; n < nn; n++)
                     fptr[n] = DMFile->Vars[v].fData[n];
                  if (fptr) ARRfree((char*)fptr);
                  d++;
               }
            }
            break;
         case drillhole: // line mesh
            DEBUG_PRINT("drillhole");
            // do coords
            fld[f].nnodes = nn = 2*DMFile->Vars[0].GetNData();
            fld[f].nspace = 3;
            fptr = (float*)fld[f].coordinates.values.ret_typed_array_ptr(
               OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
            for (n = 0; n < nn/2; n++)
            {
               // get center and length
               for (v = 0; v < DMFile->nVars; v++)
               {
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"A0      ") == 0)
                     a = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"B0      ") == 0)
                     b = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"LENGTH  ") == 0)
                     length = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"X       ") == 0)
                     x = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"Y       ") == 0)
                     y = DMFile->Vars[v].fData[n];
                  if (strcmp(DMFile->Vars[v].GetName(tmpstr),"Z       ") == 0)
                     z = DMFile->Vars[v].fData[n];
               }
               // make segment in initial position
               float radsa, radsb;
               radsa = a*2.*PI/360.;
               radsb = b*2.*PI/360.;
               lxy = length/2.*cos(-radsb);
               lx = lxy*sin(radsa);
               ly = lxy*cos(radsa);
               lz = length/2.*sin(-radsb);
               fptr[n*6+0] =  lx + x;
               fptr[n*6+1] =  ly + y;
               fptr[n*6+2] =  lz + z;
               fptr[n*6+3] = -lx + x;
               fptr[n*6+4] = -ly + y;
               fptr[n*6+5] = -lz + z;
            }
            if (fptr) ARRfree((char*)fptr);
            // now do point cell set
            fld[f].ncell_sets = 1;
            fld[f].cell_set[0].ncells = nn/2;
            fld[f].cell_set[0].cell_ndim = 1;
            fld[f].cell_set[0].cell_nnodes = 2;
            fld[f].cell_set[0].cell_order = 1;
            fld[f].cell_set[0].poly_flag = 0;
            iptr = (xp_long*)fld[f].cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
            for (n = 0; n < (nn/2); n++)
            {
               iptr[n*2+0] = n*2;
               iptr[n*2+1] = n*2+1;
            }
            if (iptr) ARRfree((char*)iptr);
            // now do data
            // count numeric datasets
            for (v = 0, nd = 0; v < DMFile->nVars; v++)
               if (DMFile->Vars[v].TypeIsNumerical())
                  nd++;
            fld[f].nnode_data = nd;
            for (v = 0, d = 0; v < DMFile->nVars; v++)
            {
               if (DMFile->Vars[v].TypeIsNumerical())
               {
                  fld[f].node_data[d].veclen = 1;
                  fld[f].node_data[d].labels = DMFile->Vars[v].GetName(tmpstr);
                  xp_long di;
                  fptr = (float*)fld[f].node_data[d].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,&di);
                  nn = DMFile->Vars[0].GetNData();
                  for (n = 0; n < nn; n++)
                  {
                     fptr[n*2+0] = DMFile->Vars[v].fData[n];
                     fptr[n*2+1] = DMFile->Vars[v].fData[n];
                  }
                  if (fptr) ARRfree((char*)fptr);
                  d++;
               }
            }
            break;
         default:
            DEBUG_PRINT("invalid filetype. creating empty field");
            fld[f].nnodes = 0;
            fld[f].ncell_sets = 0;
            fld[f].nnode_data = 0;
            break;
      }

      // free memory
      if (fname) free((char*)fname);
      if (DMFile) delete DMFile;
      if (PtsFile) delete PtsFile;
   } // end of files for

   // free memory
   if (dname) free((char*)dname);

   // return 1 for success
   return(1);
}


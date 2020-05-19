/*
   Implementation of the TDMFile and TDMVar classes. Look at dmfile.hxx
   for full info.

   Revisions:
   99-04-11: Written by Jeremy Maccelari, visualn@iafrica.com
   99-04-26: cleaned up dumping to stdout from LoadFile, JM
   99-05-03: added byte_swapping, JM
   99-05-06: fixed byte swapping for variables, JM
   00-06-15: altered byte swapping routine so it reads test data correctly, AWD
   00-06-15: added IAC_DEBUG preprocessor definitions, AWD
   01-01-09: fixed major leak in ~TDMVariable, JM
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dm.hxx"
#include "dmfile.hxx"

// ---------------------------------------- Utility functions -------------------------------------------------
void VISswap_4_byte_ptr(char* ptr)
{
   char tmp;

   tmp = ptr[0];
   ptr[0] = ptr[3];
   ptr[3] = tmp;
   tmp = ptr[1];
   ptr[1] = ptr[2];
   ptr[2] = tmp;
}

// ---------------------------------------- TDMVariable class -------------------------------------------------
TDMVariable::TDMVariable()
{
   // public data
   fData = NULL;
   cData = NULL;
   // private data
   strcpy(DefaultAlphanumerical,"");
   DefaultNumerical = 0.0;
   LogicalRecPos = 0;
   strcpy(Name,"");
   nData = 0;
   strcpy(Type,"");
   strcpy(Unit,"");
   WordNumber = 0;
}

TDMVariable::~TDMVariable()
{
   // DEBUG_PRINT("deleting TDMVariable");
   // DEBUG_PRINT("freeing cData");
   if (cData)
   {
      for (int d = 0; d < nData; d++)
         delete [] cData[d];
      delete [] cData;
   }
   // DEBUG_PRINT("freeing fData");
   if (fData)
      delete [] fData;
}

float TDMVariable::GetDefaultNumerical()
{
   return(DefaultNumerical);
}

char *TDMVariable::GetDefaultAlphanumerical(char *def)
{
   strcpy(def,DefaultAlphanumerical);
   return(def);
}

int TDMVariable::GetLogicalRecPos()
{
   return(LogicalRecPos);
}

char *TDMVariable::GetName(char *name)
{
   strcpy(name,Name);
   return(name);
}

int TDMVariable::GetNData()
{
   return(nData);
}

char *TDMVariable::GetType(char *type)
{
   strcpy(type,Type);
   return(type);
}

char *TDMVariable::GetUnit(char *unit)
{
   strcpy(unit,Unit);
   return(unit);
}

int TDMVariable::GetWordNumber()
{
   return(WordNumber);
}

void TDMVariable::SetDefaultAlphanumericalFromBuf(char *buf,int varNo)
{
   int i, j;
   char def[SIZE_OF_WORD+1];

   for (i = ((34+(7*varNo))*SIZE_OF_WORD), j = 0; i < ((35+(7*varNo))*SIZE_OF_WORD); i++)
   {
      def[j++] = buf[i];
   }
   def[j] = 0x00;
   strcpy(DefaultAlphanumerical,def);
#ifdef IAC_DEBUG
   fprintf(stdout,"Default alphanumerical = %s\n",GetDefaultAlphanumerical(def));
#endif
}

void TDMVariable::SetDefaultNumericalFromBuf(char *buf,int varNo)
{
   float f;

   memcpy(&f,(buf+((34+(7*varNo))*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   DefaultNumerical = f;
#ifdef IAC_DEBUG
   fprintf(stdout,"Default numerical = %.3f\n",GetDefaultNumerical());
#endif
}

void TDMVariable::SetLogicalRecPosFromBuf(char *buf, int varNo)
{
   float f;

   memcpy(&f,(buf+((31+(7*varNo))*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   LogicalRecPos = (int)f;
#ifdef IAC_DEBUG
   fprintf(stdout,"Field position in logical record (0 = implicit) = %d\n",GetLogicalRecPos());
#endif
}

void TDMVariable::SetNameFromBuf(char *buf, int varNo)
{
   int i, j;
   char name[(2*SIZE_OF_WORD)+1];

   for (i = ((28+(7*varNo))*SIZE_OF_WORD), j = 0; i < ((30+(7*varNo))*SIZE_OF_WORD); i++)
   {
      name[j++] = buf[i];
   }
   name[j] = 0x00;
   strcpy(Name,name);
#ifdef IAC_DEBUG
   fprintf(stdout,"Field name = *%s*\n",GetName(name));
#endif
}

void TDMVariable::SetNData(int ndata)
{
   int d;

   // DEBUG_PRINT("freeing cData");
   if (cData)
   {
      for (d = 0; d < nData; d++)
         delete [] cData[d];
      delete [] cData;
   }
   // DEBUG_PRINT("freeing fData");
   if (fData)
      delete [] fData;
   nData = ndata;

   // do one word strings, since I don't have an example to check them
   // DEBUG_PRINT("newing cData");
   cData = new char*[nData];
   for (d = 0; d < nData; d++)
      cData[d] = new char[SIZE_OF_WORD+1];

   // DEBUG_PRINT("newing fData");
   fData = new float[nData];

#ifdef IAC_DEBUG
   fprintf(stdout,"NData = %d\n",GetNData());
#endif
}


void TDMVariable::SetTypeFromBuf(char *buf, int varNo)
{
   int i, j;
   char type[SIZE_OF_WORD+1];

   for (i = ((30+(7*varNo))*SIZE_OF_WORD), j = 0; i < ((31+(7*varNo))*SIZE_OF_WORD); i++)
   {
      type[j++] = buf[i];
   }
   type[j] = 0x00;
   strcpy(Type,type);

#ifdef IAC_DEBUG
   fprintf(stdout,"Field type = *%s*\n",GetType(type));
#endif
}

void TDMVariable::SetUnitFromBuf(char *buf, int varNo)
{
   int i, j;
   char unit[SIZE_OF_WORD+1];

   for (i = ((33+(7*varNo))*SIZE_OF_WORD), j = 0; i < ((34+(7*varNo))*SIZE_OF_WORD); i++)
   {
      unit[j++] = buf[i];
   }
   unit[j] = 0x00;
   strcpy(Unit,unit);

#ifdef IAC_DEBUG
   fprintf(stdout,"Unit = *%s*\n",GetUnit(unit));
#endif
}

void TDMVariable::SetWordNumberFromBuf(char *buf, int varNo)
{
   float f;

   memcpy(&f,(buf+((32+(7*varNo))*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   WordNumber = (int)f;

#ifdef IAC_DEBUG
   fprintf(stdout,"Field word number (1 = numeric) = %d\n",GetWordNumber());
#endif
}

bool TDMVariable::TypeIsNumerical()
{
   if (strcmp(Type,"N   ") == 0)
      return(true);
   else
      return(false);
}



// ---------------------------------------- TDMFile class -----------------------------------------------------
TDMFile::TDMFile()
{
   ByteSwapped = false;
   strcpy(Description,"");
   strcpy(DirName,"");
   strcpy(FileName,"");
   FileType = invalid;
   LastModDate = 0;
   LogicalDataRecLen = 0;
   NLastPageRecs = 0;
   NPhysicalPages = 0;
   nVars = 0;
   OtherPerms = 2;
   strcpy(Owner,"");
   OwnerPerms = 0;
   Vars = NULL;
}

TDMFile::~TDMFile()
{
   // DEBUG_PRINT("deleting TDMFile");
   if (Vars)
      delete [] Vars;
}

bool TDMFile::GetByteSwapped()
{
   return(ByteSwapped);
}

char *TDMFile::GetDescription(char *desc)
{
   strcpy(desc,Description);
   return(desc);
}

char *TDMFile::GetDirName(char *name)
{
   strcpy(name,DirName);
   return(name);
}

char *TDMFile::GetFileName(char *name)
{
   strcpy(name,FileName);
   return(name);
}

FileTypes TDMFile::GetFileType()
{
   return(FileType);
}

char *TDMFile::GetOwner(char *owner)
{
   strcpy(owner,Owner);
   return(owner);
}

int TDMFile::GetLastModDate()
{
   return(LastModDate);
}

int TDMFile::GetLogicalDataRecLen()
{
   return(LogicalDataRecLen);
}

int TDMFile::GetNLastPageRecs()
{
   return(NLastPageRecs);
}

int TDMFile::GetNPhysicalPages()
{
   return(NPhysicalPages);
}

int TDMFile::GetOtherPerms()
{
   return(OtherPerms);
}

int TDMFile::GetOwnerPerms()
{
   return(OwnerPerms);
}

void TDMFile::LoadFile(char *fname)
{
   FILE *in;
   int d, i, j, nd, np, nr, nv, p, r, v;
   char buf[SIZE_OF_BUFFER], tmpstr[129];
   float df;

#ifdef IAC_DEBUG
   fprintf(stdout,"file is %s\n",fname);
#endif
   if ((in = fopen(fname,"rb")) == NULL)
   {
#ifdef IAC_DEBUG
      fprintf(stdout,"Read DM: cannot open file %s",fname);
#endif
      return;
   }
   
   // get header
   if (fread(buf,sizeof(char),SIZE_OF_BUFFER,in) != SIZE_OF_BUFFER)
   { 
      if (in) fclose(in);
#ifdef IAC_DEBUG
      fprintf(stdout,"Read DM: cannot get header page");
#endif
      return; 
   }

   // fill it
   SetByteSwapped(buf);
   SetFileNameFromBuf(buf); 
   SetDescriptionFromBuf(buf); 
   SetOwnerFromBuf(buf); 
   SetOwnerPermsFromBuf(buf); 
   SetOtherPermsFromBuf(buf); 
   SetLastModDateFromBuf(buf); 
   // don't print LogicalDataRecLen yet, since it is updated when we check for implicit variables
   SetLogicalDataRecLenFromBuf(buf);
#ifdef IAC_DEBUG
   fprintf(stdout,"Number of words in each logical data record = %d (INITIAL)\n",GetLogicalDataRecLen());
#endif
   nv = nVars = (int)GetLogicalDataRecLen();
   Vars = new TDMVariable[nVars];
   SetNPhysicalPagesFromBuf(buf); 
   SetNLastPageRecsFromBuf(buf); 
   for (v = 0; v < nv; v++)
   {
      Vars[v].ByteSwapped = GetByteSwapped();
      Vars[v].SetNameFromBuf(buf,v); 
      Vars[v].SetTypeFromBuf(buf,v); 
      Vars[v].SetLogicalRecPosFromBuf(buf,v); 
      // if the variable is implicit, we decrement the LogicalRecordLength, since it is not
      // stored in the data records
      if (Vars[v].GetLogicalRecPos() == 0) DecrementLogicalDataRecLen();
      Vars[v].SetWordNumberFromBuf(buf,v); 
      Vars[v].SetUnitFromBuf(buf,v); 
      if (Vars[v].TypeIsNumerical())
      {
         Vars[v].SetDefaultNumericalFromBuf(buf,v); 
      }
      else
      {
         Vars[v].SetDefaultAlphanumericalFromBuf(buf,v); 
      }
         
   }

#ifdef IAC_DEBUG
   fprintf(stdout,"Number of variables = %d\n",nVars);
   fprintf(stdout,"Number of words in each logical data record = %d\n",GetLogicalDataRecLen());
#endif

   for (v = 0; v < nv; v++)
   {
      if (GetLogicalDataRecLen() > 0)
         nd = (GetNPhysicalPages() - 2)*(508/GetLogicalDataRecLen()) + GetNLastPageRecs();
      else
         nd = 0;
      Vars[v].SetNData(nd);
   }

   // Now actually read in the data from the file
   // number of data pages = number of pages less the first one we have already read in
   np = GetNPhysicalPages() - 1;
   if (np < 0)
      np = 0;
#ifdef IAC_DEBUG
   fprintf(stdout,"Number of data pages = %d\n",np);
#endif
   d = 0;
   for (p = 0; p < np; p++)
   {
      // get this page and process it
#ifdef IAC_DEBUG
      // fprintf(stdout,"processing page %d\n",p);
#endif
      if (fread(buf,sizeof(char),SIZE_OF_BUFFER,in) != SIZE_OF_BUFFER)
      {
#ifdef IAC_DEBUG
         fprintf(stdout,"error getting page %d\n",p);
#endif
         fclose(in);
         nVars = 0;
         if (Vars) delete [] Vars;
         return;
      }
      if (GetLogicalDataRecLen() > 0)
         nr = 508/GetLogicalDataRecLen();
      else
         nr = 0;
      if (p == (np-1))
         nr = GetNLastPageRecs();
      // loop over records
      nv = nVars;
      for (r = 0; r < nr; r++)
      {
#ifdef IAC_DEBUG
         // fprintf(stdout,"processing record %d\n",r);
#endif
         // loop over variables for this record
         for (v = 0; v < nv; v++)
         {
            if (Vars[v].TypeIsNumerical())
            {
               // check if variable is implicit or not
               if (Vars[v].GetLogicalRecPos() != 0)
               {
                  memcpy(&df,buf + (((r*GetLogicalDataRecLen()) + (Vars[v].GetLogicalRecPos()-1))*SIZE_OF_WORD),1*sizeof(float));
                  if (ByteSwapped)
                     VISswap_4_byte_ptr((char*)&df);
                  Vars[v].fData[d] = df;
               }
               else
               {
                  Vars[v].fData[d] = Vars[v].GetDefaultNumerical();
               }
            }
            else
            {
               // don't know if this works, since I don't have test data
               if (Vars[v].GetLogicalRecPos() != 0)
               {
                  for (i = 0, j = 0; i < 4; i++)
                  {
                     tmpstr[j++] = buf[((r*GetLogicalDataRecLen()) + (Vars[v].GetLogicalRecPos()-1))*SIZE_OF_WORD+i];
                  }
                  tmpstr[j] = 0x00;
                  strcpy(Vars[v].cData[d],tmpstr);
               }
               else
               {
                  strcpy(Vars[v].cData[d],Vars[v].GetDefaultAlphanumerical(tmpstr));
               }
            }
         }
         d++;
      }
   }

#ifdef IAC_DEBUG
   fprintf(stdout,"Did %d pages with %d records each (last page had %d records) to give %d data\n",p,508/GetLogicalDataRecLen(),r,d);
#endif

   SetFileType();
   fclose(in);
#ifdef IAC_DEBUG
   fprintf(stdout,"Done load file\n");
#endif
}

void TDMFile::PrintFile(char *fname)
{
   int d, v;
   char tmpstr[129];

   FILE *out;
   
   if ((out = fopen(fname,"w")) == NULL)
      return;

   for (d = 0; d < Vars[0].GetNData(); d++)
   {
      for (v = 0; v < nVars; v++)
      {
         if (Vars[v].TypeIsNumerical())
            if ((Vars[v].GetName(tmpstr)[0] == 'X') ||
                (Vars[v].GetName(tmpstr)[0] == 'Y') ||
                (Vars[v].GetName(tmpstr)[0] == 'Z'))
               fprintf(out,"%9.1f ",Vars[v].fData[d]);
            else
               if (fabs(Vars[v].fData[d]) < 1.0e7)
                  fprintf(out,"%8.3f ",Vars[v].fData[d]);
               else
                  fprintf(out,"%8.3f ",0.0);
         else
            fprintf(out,"%5s ",Vars[v].cData[d]);
      }
      fprintf(out,"\n");
   }
   fclose(out);
}



/*
void TDMFile::SetByteSwapped(char *buf)
{
   float f;

   // get date and check it is sensible, if not assume byte swapping
   memcpy(&f, (buf+(24*SIZE_OF_WORD)), 1*sizeof(float));
   // note the following date check is not Y10k compliant
   if ((f < (float)720101.) || (f > (float)99991231))
      ByteSwapped = true;

#ifdef IAC_DEBUG
   if (ByteSwapped) {
      fprintf(stdout, "File is byte swapped. Date : %f\n", f);
      VISswap_4_byte_ptr((char*)&f);
      fprintf(stdout, "Date should be : %f", f);
   }
   else
   {
      fprintf(stdout,"File is not byte swapped. Date : %f\n", f);
   }

   ByteSwapped = false;
#endif
}
*/

/*
void TDMFile::SetByteSwapped(char *buf)
{
   int date;
   unsigned char test_arr[4] = {0x0A, 0x0B, 0x0D, 0x0E};
   int test_int = *(int *)test_arr;
   int mach_order;

   // Test endianess of machine
   if (test_int==0x0A0B0D0E) {
      //Big-endian data
      mach_order = 1;
   } else {
      mach_order = 0;
   }
   
   // get date and check it is sensible, if not assume byte swapping
   memcpy( (char *)&date, (buf+(24*SIZE_OF_WORD)), 1*sizeof(int));

   // Test endianess of data
   // note the following date check is not Y10k compliant
   if (mach_order) {
      if ((date < 720101) || (date > 99991231)) {
         ByteSwapped = true;
      } else {
         ByteSwapped = false;
      }
   }
   else
   {
      if ((date < 720101) || (date > 99991231)) {
         ByteSwapped = false;
      } else {
         ByteSwapped = true;
      }
   }


#ifdef IAC_DEBUG
   if (ByteSwapped) {
      fprintf(stdout, "File is byte swapped. Date : %d\n", date);
      VISswap_4_byte_ptr((char*)&date);
      fprintf(stdout, "Date should be : %d\n", date);
   }
   else
   {
      fprintf(stdout,"File is not byte swapped. Date : %d\n", date);
   }
#endif
}
*/

void TDMFile::SetByteSwapped(char *buf)
{
   int date;

   // get date and check it is sensible, if not assume byte swapping
   memcpy( (char *)&date, (buf+(24*SIZE_OF_WORD)), 1*sizeof(int));

   // Test endianess of data
   // note the following date check is not Y10k compliant
   if ((date < 720101) || (date > 99991231)) {
      ByteSwapped = false;
   } else {
      ByteSwapped = true;
   }

#ifdef IAC_DEBUG
   if (ByteSwapped) {
      fprintf(stdout, "File is byte swapped. Date : %d\n", date);
      VISswap_4_byte_ptr((char*)&date);
      fprintf(stdout, "Date should be : %d\n", date);
   }
   else
   {
      fprintf(stdout,"File is not byte swapped. Date : %d\n", date);
   }
#endif
}



void TDMFile::SetDescriptionFromBuf(char *buf)
{
   int i, j;
   char desc[(16*SIZE_OF_WORD)+1];

   // get description
   for (i = (4*SIZE_OF_WORD), j = 0; i < (20*SIZE_OF_WORD); i++)
   {
      desc[j++] = buf[i];
   }
   desc[j] = 0x00;
   strcpy(Description,desc);
#ifdef IAC_DEBUG
   fprintf(stdout,"Description = %s\n",GetDescription(desc));
#endif
}

void TDMFile::SetFileNameFromBuf(char *buf)
{
   int i, j;
   char dname[(2*SIZE_OF_WORD)+1], name[(2*SIZE_OF_WORD)+1];

   // get filename
   for (i = (0*SIZE_OF_WORD), j = 0; i < (2*SIZE_OF_WORD); i++)
   {
      name[j++] = buf[i];
   }
   name[j] = 0x00;
   strcpy(FileName,name);

   // get directory name
   for (i = (2*SIZE_OF_WORD), j = 0; i < (4*SIZE_OF_WORD); i++)
   {
      dname[j++] = buf[i];
   }
   dname[j] = 0x00;
   strcpy(DirName,dname);

#ifdef IAC_DEBUG
   fprintf(stdout,"Filename = %s\n",GetFileName(name)); 
   fprintf(stdout,"Directory name = %s\n",GetDirName(dname));
#endif
}

void TDMFile::SetFileType()
{
   int v;
   char name[(2*SIZE_OF_WORD)+1];

   if (nVars > 0)
   {
      // Use goto to avoid too many nested if ... else's. Not quite kosher but if
      // you don't like it, please nest this lot for me...
      // We look for a distinctive variable name for each file type
      FileType = invalid;
      // perimeter, XP, YP, ZP, PTN, PVALUE
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"PTN     ") == 0)
         {
            DEBUG_PRINT("perimeter");
            FileType = perimeter;
            goto Found;
         }
      }
      // plot, X, Y, S1, S2, CODE, COLOUR, XMIN, XMAX, XSCALE, YSCALE, XORIG, YORIG, CHARSIZE, ASPRATIO
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"S1      ") == 0)
         {
            DEBUG_PRINT("plot");
            FileType = plot;
            goto Found;
         }
      }
      // drillhole, BHID, FROM, TO, LENGTH, X, Y, Z, A0, B0
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"BHID    ") == 0)
         {
            DEBUG_PRINT("drillhole");
            FileType = drillhole;
            goto Found;
         }
      }
      // blockmodel, IJK, XC, YC, ZC, XINC, YINC, ZINC, XMORIG, YMORIG, NX, NY, NZ
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"IJK     ") == 0)
         {
            DEBUG_PRINT("blockmodel");
            FileType = blockmodel;
            goto Found;
         }
      }
      // wframepoints, XP, YP, ZP, PID
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"PID     ") == 0)
         {
            DEBUG_PRINT("wframepoints");
            FileType = wframepoints;
            goto Found;
         }
      }
      // wframetriangle, TRIANGLE, PID1, PID2, PID3
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"TRIANGLE") == 0)
         {
            DEBUG_PRINT("wframetriangle");
            FileType = wframetriangle;
            goto Found;
         }
      }
      // sectiondefinition, XCENTRE, YCENTRE, ZCENTRE, SDIP, SAZI, HSIZE, VSIZE
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"XCENTRE ") == 0)
         {
            DEBUG_PRINT("sectiondefinition");
            FileType = sectiondefinition;
            goto Found;
         }
      }
      // catalogue, FILENAM
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"FILENAM ") == 0)
         {
            DEBUG_PRINT("catalogue");
            FileType = catalogue;
            goto Found;
         }
      }
      // scheduling, BLOCKID, VOLUME, TONNES, DENSITY, SLOT, PERCENT, DRAW, START, END, LENGTH, AREA
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"BLOCKID ") == 0)
         {
            DEBUG_PRINT("scheduling");
            FileType = scheduling;
            goto Found;
         }
      }
      // results, MODEL, BLOCKID, DENSITY, VOLUME, TONNES
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"MODEL   ") == 0)
         {
            DEBUG_PRINT("results");
            FileType = results;
            goto Found;
         }
      }
      // rosette, ROSNUM, ROSXPOS, ROSYPOS, ROSZMIN, ROSZMAX, ROSAZIM, ROSFANG, ROSBWID
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"ROSNUM  ") == 0)
         {
            DEBUG_PRINT("rosette");
            FileType = rosette;
            goto Found;
         }
      }
      // drivestats, PVALUE, VOLUME, TONNES, LENGTH, ZMIN, ZMAX, GRADIENT, XAREA
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"XAREA   ") == 0)
         {
            DEBUG_PRINT("drivestats");
            FileType = drivestats;
            goto Found;
         }
      }
      // point, XPT, YPT, ZPT
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"XPT     ") == 0)
         {
            DEBUG_PRINT("point");
            FileType = point;
            goto Found;
         }
      }
      // dependency, PNUM1, PNUM2
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"PNUM1   ") == 0)
         {
            DEBUG_PRINT("dependency");
            FileType = dependency;
            goto Found;
         }
      }
      // plotterpen, COLOUR, PEN
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"PEN     ") == 0)
         {
            DEBUG_PRINT("plotterpen");
            FileType = plotterpen;
            goto Found;
         }
      }
      // plotfilter, FIELD, TEST, IN, OUT, PEN
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"FIELD   ") == 0)
         {
            DEBUG_PRINT("plotfilter");
            FileType = plotfilter;
            goto Found;
         }
      }
      // validation ATTNAME, ATTTYPE, VALUE, MIN, MAX, DEFAULT
      for (v = 0; v < nVars; v++)
      {
         if (strcmp(Vars[v].GetName(name),"ATTNAME ") == 0)
         {
            DEBUG_PRINT("validation");
            FileType = validation;
            goto Found;
         }
      }
      // All ifs goto here
      Found:;
   }
   else
   {
      DEBUG_PRINT("nVars <= 0 : filetype invalid");
      FileType = invalid;
   }
}

void TDMFile::SetLastModDateFromBuf(char *buf)
{
   float f;

   memcpy(&f,(buf+(24*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   LastModDate = (int)f;
#ifdef IAC_DEBUG
   fprintf(stdout,"Last file modification date = %d\n",GetLastModDate());
#endif
}

void TDMFile::SetLogicalDataRecLenFromBuf(char *buf)
{
   float f;

   memcpy(&f,(buf+(25*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   LogicalDataRecLen = (int)f;
#ifdef IAC_DEBUG
   fprintf(stdout,"Logical data record length = %d\n",GetLogicalDataRecLen());
#endif
}

void TDMFile::DecrementLogicalDataRecLen()
{
   LogicalDataRecLen--;
}

void TDMFile::SetNLastPageRecsFromBuf(char *buf)
{
   float f;

   memcpy(&f,(buf+(27*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   NLastPageRecs = (int)f;
#ifdef IAC_DEBUG
   fprintf(stdout,"Number of logical records in last page of file = %d\n",GetNLastPageRecs());
#endif
}

void TDMFile::SetNPhysicalPagesFromBuf(char *buf)
{
   float f;

   memcpy(&f,(buf+(26*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   NPhysicalPages = (int)f;
#ifdef IAC_DEBUG
   fprintf(stdout,"Number of physical pages in file = %d\n",GetNPhysicalPages());
#endif
}

void TDMFile::SetOwnerFromBuf(char *buf)
{
   int i, j;
   char owner[(2*SIZE_OF_WORD)+1];

   for (i = (20*SIZE_OF_WORD), j = 0; i < (22*SIZE_OF_WORD); i++)
   {
      owner[j++] = buf[i];
   }
   owner[j] = 0x00;
   strcpy(Owner,owner);
#ifdef IAC_DEBUG
   fprintf(stdout,"Owner = %s\n",GetOwner(owner));
#endif
}

void TDMFile::SetOtherPermsFromBuf(char *buf)
{
   float f;

   memcpy(&f,(buf+(23*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   OtherPerms = (int)f;
#ifdef IAC_DEBUG
   fprintf(stdout,"Other perms = %d\n",GetOtherPerms());
#endif
}

void TDMFile::SetOwnerPermsFromBuf(char *buf)
{
   float f;

   memcpy(&f,(buf+(22*SIZE_OF_WORD)),1*sizeof(float));
   if (ByteSwapped)
      VISswap_4_byte_ptr((char*)&f);
   OwnerPerms = (int)f;
#ifdef IAC_DEBUG
   fprintf(stdout,"Owner perms = %d\n",GetOwnerPerms());
#endif
}


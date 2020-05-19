
// by James S Perrin  Time-stamp: <Tuesday 29/11/11 15:48:25 james>
// read_DCMLIST.cxx
// read the DCMLIST file from the specified dir usin DCMTK 
// this file lists infos about the DIOCM files present in the dir */
// use BSD style dir access
// - fixed spaces in PatID
// - fixed same imageID

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef MSDOS
#include <sys/types.h>
#include <sys/dir.h>
#ifndef __USE_BSD
#define dirent direct
#endif
#else
#endif
#include "gen.h"
#include "read_DCMLIST.h"


//#define DEBUG

static int intcmp(int a, int b)
{
  int ret;

  if(a > b)
    ret = 1;
  else if ( a < b)
    ret = -1;
  else
    ret = 0;

  return ret;
}

static int floatcmp(float a, float b)
{
  int ret;

  if(a > b)
    ret = 1;
  else if ( a < b)
    ret = -1;
  else
    ret = 0;

  return ret;
}

/* comp file infos -  qsort function that sort on patients ID, study, series, aqu, img */
static int comp_file_infos(const void *a, const void *b)
{
  int ret;

  ret = strcmp( ((dicom_file_info *)a)->id, ((dicom_file_info *)b)->id);

  if(ret == 0)
  {
    ret = intcmp( ((dicom_file_info *)a)->studyID, ((dicom_file_info *)b)->studyID);

    if(ret == 0)
    {
      ret = intcmp( ((dicom_file_info *)a)->seriesID, ((dicom_file_info *)b)->seriesID);

      if(ret == 0)
      {
	ret  = intcmp( ((dicom_file_info *)a)->aquID , ((dicom_file_info *)b)->aquID );

	if(ret == 0)
	{
	  /* if imageID present use */
	  if( (((dicom_file_info *)a)->imageID !=-1 && ((dicom_file_info *)b)->imageID!=-1 ) &&
	      (((dicom_file_info *)a)->imageID != ((dicom_file_info *)b)->imageID) )
	     ret  = intcmp( ((dicom_file_info *)a)->imageID , ((dicom_file_info *)b)->imageID );
	  else /* use imagePos first value */
	  {
	    float ap, bp; /* don't need to bother with pythag just need to know which is "larger" */
	    ap = ((dicom_file_info *)a)->imagePos[0] + ((dicom_file_info *)a)->imagePos[1] + ((dicom_file_info *)a)->imagePos[2];
	    bp = ((dicom_file_info *)b)->imagePos[0] + ((dicom_file_info *)b)->imagePos[1] + ((dicom_file_info *)b)->imagePos[2];
	    ret  = floatcmp(ap, bp);
	  }

	}
      }
    }
  }

  return ret;
}

#ifdef MSDOS
static int comp_not_DCMLIST(const WIN32_FIND_DATA *dir)
{
  int ret = 0;

  if((strcmp(dir->cFileName, ".")!=0) && (strcmp(dir->cFileName, "..")!=0) && (strcmp(dir->cFileName, "DCMLIST")!=0))
    ret = 1;

  return ret;
}
#else
#ifndef __USE_BSD
static int comp_not_DCMLIST(struct dirent *de)
#else
static int comp_not_DCMLIST(const struct dirent *de)
#endif
{
  int ret = 0;

  if((strcmp(de->d_name, ".")!=0) && (strcmp(de->d_name, "..")!=0) && (strcmp(de->d_name, "DCMLIST")!=0))
    ret = 1;

  return ret;
}
#endif

// }}}

// {{{ ********* READ DCMLIST **********

// though we can determine how many of each struct we need we don't allocate
// as arrays as we may need to add substract structs
// the next ptr is then set to the next elem in the array.
// infos created by this function must therefore be deleted as arrays
int ReadDICOM_read_DCMLIST::read_DCMLIST(int &npats, int &_nseries, int &_nfiles, dicom_pat_info **pats, dicom_file_info **dfiles)
{
  FILE *fp;
  int i, j, k;
  dicom_series_info *curr_series;
  dicom_study_info *curr_study;
  dicom_pat_info *curr_pat;
  char dirStr[256];
#ifdef MSDOS
  sprintf(dirStr, "%s\\DCMLIST", (char *)dirname);
#else
  sprintf(dirStr, "%s/DCMLIST", (char *)dirname);
#endif
  fp = fopen(dirStr,"r");

  if(fp)
  {
    _nseries = 0;

    /* get files */
    fscanf(fp, "%d\n", &_nfiles);
    *dfiles = new dicom_file_info [_nfiles];

    for(i=0; i<_nfiles; i++)
    {
      fgets((*dfiles)[i].filename, 256, fp); // use this as it ignores space
      (*dfiles)[i].filename[strlen((*dfiles)[i].filename)-1] = '\0'; // remove \n

      if(i<(_nfiles-1))
	(*dfiles)[i].next = &(*dfiles)[i+1];
      else
	(*dfiles)[i].next = NULL;
    }

    /* get pats */
    fscanf(fp, "%d", &npats);
    curr_pat = NULL;

    for(i=0; i<npats; i++)
    {
      if(curr_pat)
      {
	curr_pat->next = new dicom_pat_info;
	curr_pat = curr_pat->next;
      } else {
	curr_pat = *pats = new dicom_pat_info;
      }

      fscanf(fp, "%s %s %d %d %d", curr_pat->name, curr_pat->id, &curr_pat->nstudies,
	     &curr_pat->start, &curr_pat->end);
      curr_study = curr_pat->studies = NULL;

      /* get studies */
      for(j=0; j< curr_pat->nstudies; j++)
      {
	if(curr_study)
	{
	  curr_study->next = new dicom_study_info;
	  curr_study = curr_study->next;
	} else {
	  curr_study = curr_pat->studies = new dicom_study_info;
	}

	fscanf(fp, "%d %s %d %d %d", &curr_study->studyID, curr_study->studyDate, &curr_study->nseries, &curr_study->start, &curr_study->end);
	curr_series = curr_study->series = NULL;
	_nseries+=curr_study->nseries;

	/* get series */
	for(k=0; k< curr_study->nseries; k++)
	{
	  if(curr_series)
	  {
	    curr_series->next = new dicom_series_info;
	    curr_series = curr_series->next;
	  } else {
	    curr_series = curr_study->series = new dicom_series_info;
	  }

	  fscanf(fp, "%d %s %d %d", &curr_series->seriesID, curr_series->seriesDate,
		 &curr_series->start, &curr_series->end);
	  curr_series->nfiles = curr_series->end - curr_series->start +1;
	}

	curr_series->next = NULL;
      }

      curr_study->next = NULL;
    }

    curr_pat->next = NULL; /* make sure last next = NULL*/

    fclose(fp);
  } else {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ERROR: can't open %s to read", dirStr);
    return 0;
  };

  return 1;
}

// }}}

// {{{ ********* RESCAN DIR *********

#ifndef WIN32
#ifndef _DIRENT_HAVE_D_TYPE
#error "Require dirent.d_type"
#endif

// recursive_scandir() - get all files under given dir, ignore 'DCMLIST' 
// dirname - assume  has trailing "/"
// return - 1 if process was interrupted by user
int recursive_scandir(dicom_file_info **_dfiles, int *_maxnfiles, int *_curfile, char *dirname)
{
  // First check for XP interrupt so we can abort on runaway scanning
  int interrupt = 0;
  OMstatus_check( 5, "DICOM rescan", &interrupt);
  if(interrupt == 1) return 1;

  // UN*X version (BSD style)
  int f;
  struct dirent **namelist;
  int _thisnfiles = scandir((char *)dirname, &namelist, comp_not_DCMLIST, alphasort);

  if(_thisnfiles == -1)
  {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ERROR: rescan dir : can't open %s", (char *)dirname);
    return 0;
  }

  // resize dfiles
  *_maxnfiles += _thisnfiles;
  *_dfiles = (dicom_file_info *)realloc(*_dfiles, *_maxnfiles * sizeof(dicom_file_info));

  for(f=0; f<_thisnfiles; f++)
  {
    if(namelist[f]->d_type == DT_DIR)
    {
      char *newdirname;
      newdirname = (char *)malloc(strlen(dirname)+strlen(namelist[f]->d_name)+2);
      sprintf(newdirname, "%s%s/", dirname, namelist[f]->d_name);

      interrupt = recursive_scandir(_dfiles, _maxnfiles, _curfile, newdirname);
      free(newdirname);

      if(interrupt == 1) break;
    } else if(namelist[f]->d_type == DT_REG) {
      sprintf((*_dfiles)[*_curfile].filename, "%s%s", dirname, namelist[f]->d_name);
      (*_curfile)++;
    }
  }

  free(namelist);

  return interrupt;
}
#endif

int ReadDICOM_read_DCMLIST::rescan_dir(int &npats, int &_nseries, int &_nfiles, dicom_pat_info **_pats, dicom_file_info **_dfiles)
{
  FILE *fp;
  int interrupt = 0;
  int f, i;
  int size, val;
  int group, elem, len;
  int found_PATGRP, found_PNAME, found_IDENT, found_IDGRP, found_SOPID, found_RELGRP;
  int found_STUDID, found_SERID, found_STUDDATE, found_SERDATE, found_AQUID, found_IMGID, found_IMGPOS;
  int end_of_PATGRP, end_of_IDGRP, end_of_RELGRP;
  dicom_file_info *dfiles;
  dicom_series_info *series, *curr_series;
  dicom_study_info *studies, *curr_study;
  dicom_pat_info *pats, *curr_pat;
  char tmpStr[256];
  char tmpBuff[256];
  char tmpVR[3];

  // {{{ ********* GET FILE LIST *********

#ifndef WIN32
  // UN*X version (BSD style)
  char *newdirname;
  int _maxnfiles = 0;
  int _curfile = 0;

  newdirname = (char *)malloc(strlen((char *)dirname)+2);

  strcpy(newdirname, (char *)dirname);
  if(newdirname[strlen(dirname)-1] != '/')
    newdirname[strlen(dirname)] = '/';

  interrupt = recursive_scandir(_dfiles, &_maxnfiles, &_curfile, newdirname);

  free(newdirname);

  _nfiles = _curfile;
  dfiles = *_dfiles;
#else
  // MSDOS version
  // designed to work with an eFilm database
  // DICOM->patients->studies->images
  //      ->DCMLIST
  char dDirStr[256], pDirStr[256], sDirStr[256];
  WIN32_FIND_DATA pFileData, sFileData, iFileData;
  HANDLE phandle, shandle, ihandle;
  struct _winFile {
    char name[256];
    _winFile *next;
  } *winFiles = NULL, *winFilePtr;

  _nfiles = 0;

  // make sure we get the full path name
  GetLongPathName((char *)dirname, dDirStr, 256);
  strcpy(tmpStr,dDirStr);
  strcat(tmpStr,"\\*");

  // get patient directories
  phandle = FindFirstFile(tmpStr, &pFileData);
  if(phandle == INVALID_HANDLE_VALUE)
  {
    printf ("Invalid File Handle. Get Last Error reports %d\n", GetLastError ());
  } else {
    // get study directories
    do
    {
      if(comp_not_DCMLIST(&pFileData))
      {
	strcpy(pDirStr,dDirStr);
	strcat(pDirStr, "\\");
	strcat(pDirStr,pFileData.cFileName);
	strcpy(tmpStr,pDirStr);
	strcat(tmpStr,"\\*");
	shandle = FindFirstFile(tmpStr, &sFileData);
	
	if(shandle == INVALID_HANDLE_VALUE)
	{
	  printf ("Invalid File Handle. Get Last Error reports %d\n", GetLastError ());
	} else {
	  // get images
	  do
	  {
	    if(comp_not_DCMLIST(&sFileData))
	    {
	      strcpy(sDirStr,pDirStr);
	      strcat(sDirStr, "\\");
	      strcat(sDirStr,sFileData.cFileName);
	      strcpy(tmpStr,sDirStr);
	      strcat(tmpStr,"\\*.dcm");
	      ihandle = FindFirstFile(tmpStr, &iFileData);

	      if(ihandle == INVALID_HANDLE_VALUE)
	      {
		printf ("Invalid File Ihandle. Get Last Error reports %d\n", GetLastError ());
	      } else {
		if(comp_not_DCMLIST(&iFileData))
		{
		  if(winFiles==NULL) // doing it this way this will only test once per study
		  {
		    winFilePtr = winFiles = new _winFile;
		    strcpy(winFilePtr->name, sDirStr);
		    strcat(winFilePtr->name, "\\");
		    strcat(winFilePtr->name, iFileData.cFileName);
		    _nfiles = 1;
		  } else {
		    winFilePtr->next =  new _winFile;
		    winFilePtr = winFilePtr->next;
		    strcpy(winFilePtr->name, sDirStr);
		    strcat(winFilePtr->name, "\\");
		    strcat(winFilePtr->name, iFileData.cFileName);
		    _nfiles++;
		  }
		}
		while(FindNextFile(ihandle, &iFileData))
		{
		  if(comp_not_DCMLIST(&iFileData))
		  {
		    winFilePtr->next =  new _winFile;
		    winFilePtr = winFilePtr->next;
		    strcpy(winFilePtr->name, sDirStr);
		    strcat(winFilePtr->name, "\\");
		    strcat(winFilePtr->name, iFileData.cFileName);
		    _nfiles++;
		  }
		}

		FindClose(ihandle);
	      } // if images
	    }
	  } while(FindNextFile(shandle, &sFileData)); // while studies

	  FindClose(shandle);
	} // if studies
      }
    } while(FindNextFile(phandle, &pFileData)); // while patients

    FindClose(phandle);
  } // if patients

  printf("Done! Found %d dcm files\n", _nfiles);

  if(winFiles) winFilePtr->next = NULL;
  dfiles = *_dfiles =  new dicom_file_info [_nfiles];

  winFilePtr=winFiles;
  f = 0;
  while(winFilePtr)
  {
    strcpy(dfiles[f++].filename,winFilePtr->name);
    winFilePtr = winFilePtr->next;
  }

  winFilePtr = winFiles;
  while(winFilePtr)
  {
    winFiles =  winFilePtr->next;
    delete winFilePtr;
    winFilePtr = winFiles;
  }


#endif

  // user aborted scan
  if(interrupt == 1)
    return 0;

  // }}}

  // {{{ ********* SCAN FILES *********
  DcmFileFormat fileformat;
  DcmDataset *dataset;
  OFCondition status;
  OFString dcm_string;
  Uint32 dcm_int;
  float dcm_float;

  int nDCMfiles = 0;
  
  for(f=0; f<_nfiles; f++)
  {

    // check for user interrupt
    if(f & 0x7f) 
    {
      OMstatus_check( 25+(f*65)/_nfiles, "DICOM rescan", &interrupt);
      if(interrupt == 1) 
	return 0;
    }

    // initialise all variables
    dfiles[f].id[0] = 0x7f; // sorted to end of list if not a valid DCM file
    dfiles[f].studyID = -1;
    dfiles[f].seriesID = -1;
    dfiles[f].aquID = -1;
    dfiles[f].imageID = -1;
    dfiles[f].imagePos[0] = -1; dfiles[f].imagePos[1] = -1; dfiles[f].imagePos[2] = -1;
    strcpy(dfiles[f].studyDate, "00000101"); // initialise with dummy date
    strcpy(dfiles[f].seriesDate, "00000101"); // initialise with dummy date

    status = fileformat.loadFile(dfiles[f].filename);

    if(status.good())
    {
      nDCMfiles++;
      dataset = fileformat.getDataset();
      size=0;

      /* Group 8 Identity */
      if(dataset->findAndGetOFString(DCM_StudyDate, dcm_string).good())
      {
	dcm_string.copy(dfiles[f].studyDate, 8);
	dfiles[f].studyDate[8] = '\0';
      }
      
      if(dataset->findAndGetOFString(DCM_SeriesDate, dcm_string).good())
      {
	dcm_string.copy(dfiles[f].seriesDate, 8);
	dfiles[f].seriesDate[8] = '\0';
      }
      
      /* Group 10 Pat */
      if(dataset->findAndGetOFString(DCM_PatientsName, dcm_string).good())
      {
	dcm_string.copy(dfiles[f].name, 63);
	dfiles[f].name[63] = '\0';
      }
      
      if(dataset->findAndGetOFString(DCM_PatientID, dcm_string).good())
      {
	dcm_string.copy(dfiles[f].id, 31);
	dfiles[f].id[31] = '\0';
      }
      
      /* Group 20 Relationship */
      if(dataset->findAndGetOFString(DCM_StudyID, dcm_string).good())
	dfiles[f].studyID = atoi(dcm_string.c_str());
      
      if(dataset->findAndGetOFString(DCM_SeriesNumber, dcm_string).good())
	dfiles[f].seriesID = atoi(dcm_string.c_str());

      if(dataset->findAndGetOFString(DCM_AcquisitionNumber, dcm_string).good())
	dfiles[f].aquID = atoi(dcm_string.c_str());

      if(dataset->findAndGetOFString(DCM_InstanceNumber, dcm_string).good())
	dfiles[f].imageID = atoi(dcm_string.c_str());


      if(dataset->findAndGetOFStringArray(DCM_ImagePositionPatient, dcm_string).good())
      {
	dcm_string.copy(tmpStr, 255); 
	sscanf(tmpStr,"%f\\%f\\%f",
	       &dfiles[f].imagePos[0],&dfiles[f].imagePos[1],&dfiles[f].imagePos[2]);
#ifdef DEBUG
	puts("Found IMGPOS");
	printf("IMGPOS: %s %f %f %f\n",tmpStr,
	       dfiles[f].imagePos[0],dfiles[f].imagePos[1],dfiles[f].imagePos[2]);
#endif
      }

    } else {
      printf("Not a DICOM file %s\n", dfiles[f].filename);
    }

  } /* for f */

  // }}}

  // {{{ ********* SORT DATA **********

  qsort(dfiles, _nfiles, sizeof(dicom_file_info), comp_file_infos);
  OMstatus_check( 95, "DICOM rescan", NULL);

  // change to number of valid DICOM files which should be at the
  // start of the array after sorting
  _nfiles = nDCMfiles;

  // add next pointers in only after sorting the files
  for(f=0; f<_nfiles; f++)
  {
    if(f<(_nfiles-1))
      dfiles[f].next = &dfiles[f+1];
    else
      dfiles[f].next = NULL;
  }

  // }}}

  // {{{ ********* SCAN DATA **********

  if(_nfiles)
  {
    npats = 1;
    curr_pat = *_pats = pats = new dicom_pat_info;
    strcpy(curr_pat->id,dfiles[0].id);
    strcpy(curr_pat->name,dfiles[0].name);
    curr_pat->start = 0;
    curr_pat->next = NULL;

    /* find all patients */
    for(i=0; i<nDCMfiles; i++)
    {
      if( strcmp(curr_pat->id, dfiles[i].id) !=0)
      {
	npats++;
	curr_pat->end = i-1;
	curr_pat = curr_pat->next = new dicom_pat_info;
	strcpy(curr_pat->id, dfiles[i].id);
	strcpy(curr_pat->name, dfiles[i].name);
	curr_pat->start = i;
	curr_pat->next = NULL;
	curr_pat->nstudies = 0;
      }
    }
    curr_pat->end = i-1;

    /* find all studies for each patient */
    curr_pat = pats;

    while(curr_pat!=NULL)
    {
      curr_pat->nstudies = 1;
      curr_study = curr_pat->studies = new dicom_study_info;
      curr_study->studyID = dfiles[curr_pat->start].studyID;
      strcpy(curr_study->studyDate, dfiles[curr_pat->start].studyDate);
      curr_study->start = curr_pat->start;
      curr_study->next = NULL;

      for(i=curr_pat->start; i<=curr_pat->end; i++)
      {
	if( curr_study->studyID != dfiles[i].studyID )
	{
	  curr_pat->nstudies++;
	  curr_study->end = i-1;
	  curr_study = curr_study->next = new dicom_study_info;
	  curr_study->studyID = dfiles[i].studyID;
	  strcpy(curr_study->studyDate, dfiles[i].studyDate);
	  curr_study->start = i;
	  curr_study->next = NULL;
	  curr_study->nseries = 0;
	}
      }
      curr_study->end = i-1;
      curr_pat = curr_pat->next;
    }

    /* find all series for each study */
    curr_pat = pats;
    _nseries = 0;

    while(curr_pat!=NULL)
    {
      curr_study = curr_pat->studies;

      while(curr_study!=NULL)
      {
	_nseries++;
	curr_study->nseries = 1;
	curr_series = curr_study->series = new dicom_series_info;
	curr_series->seriesID = dfiles[curr_study->start].seriesID;
	strcpy( curr_series->seriesDate, dfiles[curr_study->start].seriesDate);
	curr_series->start = curr_study->start;
	curr_series->next = NULL;

	for(i=curr_study->start; i<=curr_study->end; i++)
	{
	  if( curr_series->seriesID != dfiles[i].seriesID )
	  {
	    _nseries++;
	    curr_study->nseries++;
	    curr_series->end = i-1;
	    curr_series->nfiles = i - curr_series->start;
	    curr_series = curr_series->next = new dicom_series_info;
	    curr_series->seriesID = dfiles[i].seriesID;
	    strcpy(curr_series->seriesDate, dfiles[i].seriesDate);
	    curr_series->start = i;
	    curr_series->next = NULL;
	  }
	}
	curr_series->end = i-1;
	curr_series->nfiles = i - curr_series->start;
	curr_study = curr_study->next;
      }
      curr_pat = curr_pat->next;
    }
  }
  // }}}

  _nfiles = nDCMfiles;

  return 1;
}

// }}}

// {{{ ********* CREATE EXPRESS DATA **********

int ReadDICOM_read_DCMLIST::output_infos(int npats, int _nseries, int _nfiles, dicom_pat_info *pats, dicom_file_info *dfiles)
{
  int i, s;
  dicom_file_info *curr_file;
  dicom_series_info *curr_series;
  dicom_study_info *curr_study;
  dicom_pat_info *curr_pat;
  int series_st_en_size;
  int *series_st_en_arr = NULL;

  nfiles = _nfiles;
  nseries = _nseries;

  /* create filename list */
  curr_file = dfiles;
  i = 0;
  if(_nfiles)
  {
    while(curr_file!=NULL)
    {
      filenames.set_str_array_val(i++, curr_file->filename);
      curr_file = curr_file->next;
    }
  } else {
     filenames.set_obj_ref(OMnull_obj);
  }

  /* create series */
  char seriesStr[256], stDateStr[11], serDateStr[11];

  if(_nseries)
    series_st_en_arr = (int *)series_st_en.ret_array_ptr(OM_GET_ARRAY_WR,&series_st_en_size);
  else
  {
    series_st_en.set_obj_ref(OMnull_obj);
    return 1;
  }

  if(series_st_en_arr)
  {
    curr_pat = pats;
    s = 0;

    while(curr_pat!=NULL)
    {
      curr_study = curr_pat->studies;

      while(curr_study!=NULL)
      {
	sprintf(stDateStr, "%c%c/%c%c/%c%c%c%c",curr_study->studyDate[6],curr_study->studyDate[7],
		curr_study->studyDate[4],curr_study->studyDate[5],
		curr_study->studyDate[0],curr_study->studyDate[1],curr_study->studyDate[2],curr_study->studyDate[3]);
	curr_series=curr_study->series;
	
	while(curr_series!=NULL)
	{
	  sprintf(serDateStr, "%c%c/%c%c/%c%c%c%c",curr_series->seriesDate[6],curr_series->seriesDate[7],
		  curr_series->seriesDate[4],curr_series->seriesDate[5],
		  curr_series->seriesDate[0],curr_series->seriesDate[1],curr_series->seriesDate[2],curr_series->seriesDate[3]);
	  /* create series string */
	  sprintf(seriesStr, "%-32s %-10s %16d (%s) %2d (%s) %4d", curr_pat->name, curr_pat->id,
		  curr_study->studyID, stDateStr, curr_series->seriesID, serDateStr, curr_series->nfiles);
	  series.set_str_array_val(s, seriesStr);

	  /* create series_st_en */
	  series_st_en_arr[s*2] = curr_series->start;
	  series_st_en_arr[s*2+1] = curr_series->end;
	  s++;
	  curr_series = curr_series->next;
	}
	curr_study = curr_study->next;
      }
      curr_pat = curr_pat->next;
    }

    ARRfree(series_st_en_arr);
  } else {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ERROR: can't alloc series_st_en array");
  }

  return 1;
}

// }}}

// {{{ ********* WRITE DCMLIST **********

int ReadDICOM_read_DCMLIST::write_DCMLIST(int npats, int _nseries, int _nfiles, dicom_pat_info *pats, dicom_file_info *dfiles)
{
  FILE *fp;
  dicom_file_info *curr_file;
  dicom_series_info *curr_series;
  dicom_study_info *curr_study;
  dicom_pat_info  *curr_pat;
  char dirStr[256];
  int l, len;

#ifdef MSDOS
  sprintf(dirStr, "%s\\DCMLIST", (char *)dirname);
#else
  sprintf(dirStr, "%s/DCMLIST", (char *)dirname);
#endif

  // if no DICOM files found
  if(_nfiles == 0)
  {
    return 1;
  }

  fp = fopen(dirStr,"w");

  if(fp)
  {
    fprintf(fp,"%d\n", _nfiles);
    curr_file = dfiles;
    while(curr_file!=NULL)
    {
      fprintf(fp,"%s\n", curr_file->filename);
      curr_file = curr_file->next;
    }

    /* dump pats */
    fprintf(fp,"%d\n", npats);

    curr_pat = pats;
    while(curr_pat!=NULL)
    {
      /* replace space in name and id with '_', last space to even up len doesn't matter */
      len = strlen(curr_pat->name) -1;
      for(l=0; l<len; l++)
	if(curr_pat->name[l]==' ' || curr_pat->name[l]=='\t')
	  curr_pat->name[l]='_';

      len = strlen(curr_pat->id) -1;
      for(l=0; l<len; l++)
	if(curr_pat->id[l]==' ' || curr_pat->id[l]=='\t')
	  curr_pat->id[l]='_';

      fprintf(fp,"%s %s %d %d %d\n", curr_pat->name, curr_pat->id, curr_pat->nstudies,
	      curr_pat->start, curr_pat->end);

      curr_study=curr_pat->studies;

      while(curr_study!=NULL)
      {
	fprintf(fp,"%d %s %d %d %d\n", curr_study->studyID, curr_study->studyDate,
		curr_study->nseries, curr_study->start, curr_study->end);

	curr_series=curr_study->series;

	while(curr_series!=NULL)
	{
	  fprintf(fp,"%d %s %d %d\n", curr_series->seriesID, curr_series->seriesDate,
		  curr_series->start, curr_series->end);
	  curr_series = curr_series->next;
	}

	curr_study = curr_study->next;
      }

      curr_pat = curr_pat->next;
    }

    fclose(fp);
  } else {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ERROR: can't open %s to write", dirStr);
  };

  return 1;
}

// }}}

// {{{ ********* read_DCMLIST::read *********
int ReadDICOM_read_DCMLIST::read(OMevent_mask event_mask, int seq_num)
{
  // dirname (OMXstr read req notify)
  // rescan (OMXint read req)
  // nfiles (OMXint write)
  // filenames (OMXstr_array write)
  // nseries (OMXint write)
  // series (OMXstr_array write)
  // series_st_en (OMXint_array write)

  FILE *fp;
  int i, j, k, s;
  int npats, _nseries, _nfiles;
  int interrupt;
  dicom_file_info *dfiles = NULL;
  dicom_series_info *curr_series, *prev_series;
  dicom_study_info *curr_study, *prev_study;
  dicom_pat_info *pats = NULL, *curr_pat, *prev_pat;

  // test to see if rescan triggered method, if seq_num =0 we're in startup
  if(seq_num && rescan.changed(seq_num))
  {
    if(rescan_dir(npats, _nseries, _nfiles, &pats, &dfiles))
    {
      write_DCMLIST(npats, _nseries, _nfiles, pats, dfiles);
      output_infos(npats, _nseries, _nfiles, pats, dfiles);

      // {{{ free data structs
      if(_nfiles)
      {
	curr_pat = pats;

	for(i=0; i<npats; i++)
	{
	  curr_study = curr_pat->studies;

	  for(j=0; j<curr_pat->nstudies; j++)
	  {
	    curr_series = curr_study->series;

	    for(k=0; k<curr_study->nseries; k++)
	    {
	      prev_series = curr_series;
	      curr_series = curr_series->next;
	      delete prev_series;
	    }

	    prev_study = curr_study;
	    curr_study = curr_study->next;
	    delete prev_study;
	  }

	  prev_pat = curr_pat;
	  curr_pat = curr_pat->next;
	  delete prev_pat;
	}
      }

      if(dfiles) free(dfiles);

      // }}}
    } else {
      if(dfiles) free(dfiles);
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ERROR: Unable to rescan dir %s", (char *)dirname);
    }

  } else {
    if(read_DCMLIST(npats, _nseries, _nfiles, &pats, &dfiles))
    {
      output_infos(npats, _nseries, _nfiles, pats, dfiles);

      // {{{ free data structs

      curr_pat = pats;

      for(i=0; i<npats; i++)
      {
	curr_study = curr_pat->studies;

	for(j=0; j<curr_pat->nstudies; j++)
	{
	  curr_series = curr_study->series;

	  for(k=0; k<curr_study->nseries; k++)
	  {
	    prev_series = curr_series;
	    curr_series = curr_series->next;
	    delete prev_series;
	  }

	  prev_study = curr_study;
	  curr_study = curr_study->next;
	  delete prev_study;
	}

	prev_pat = curr_pat;
	curr_pat = curr_pat->next;
	delete prev_pat;
      }

      delete [] dfiles;

      // }}}
    } else {
#ifdef MSDOS
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ERROR: Unable to read %s\\DCMLIST", (char *)dirname);
#else
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ERROR: Unable to read %s/DCMLIST", (char *)dirname);
#endif
    }
  }

  OMstatus_check( 100, "Read DCMLIST", &interrupt);

  return 1;
}
// }}}

// {{{ ********* ESP::REMOVE *********
int ReadDICOM_read_DCMLIST::remove(OMevent_mask event_mask, int seq_num)
{
  // dirname (OMXstr read req)
  // delete (OMXint read notify)
  // selected_series (OMXstr read req)
  // nfiles (OMXint write)
  // filenames (OMXstr_array write)
  // nseries (OMXint write)
  // series (OMXstr_array write)
  // series_st_en (OMXint_array write)

  // selected_series (OMXint read req)
  FILE *fp;
  int i, j, k, s, ss;
  int rmfiles, stfile, enfile;
  int npats, _nseries, _nfiles;
  dicom_file_info *dfiles, *use_dfiles;
  dicom_series_info *curr_series, *prev_series;
  dicom_study_info *curr_study, *prev_study;
  dicom_pat_info *pats, *curr_pat, *prev_pat;

  ss = selected_series;

  if(read_DCMLIST(npats, _nseries, _nfiles, &pats, &dfiles))
  {
    // {{{ ********* DELETE SERIES ********

    // find series, note s[1:nseries] and ss[0:nseries-1]
    // curr_XXXX will point to pat->study->series to be removed
    // prev_XXXX will point to prev pat, study and series
    // or NULL if curr_XXXX at start of each list
    s = -1;
    prev_pat = NULL;
    curr_pat = pats;

    while(curr_pat!=NULL && s<ss)
    {
      prev_study = NULL;
      curr_study = curr_pat->studies;

      while(curr_study!=NULL && s<ss)
      {
	if((s+curr_study->nseries)>=ss)
	{
	  prev_series = NULL;
	  curr_series = curr_study->series;

	  s++;
	  while(s<ss)
	  {
	    prev_series = curr_series;
	    curr_series = curr_series->next;
	    s++;
	  }

	  break;
	} else {
	  s += curr_study->nseries;
	}

	prev_study = curr_study;
	curr_study = curr_study->next;
      }

      if(s<ss) // break not occured
      {
	prev_pat = curr_pat;
	curr_pat = curr_pat->next;
      }
    }

    // delete files
    stfile = curr_series->start;
    enfile = curr_series->end;
    rmfiles = curr_series->nfiles;
    for(i=stfile; i<=enfile; i++)
      ::remove(dfiles[i].filename);

    // skip deleted files
    if(stfile!=0)
    {
      dfiles[stfile-1].next = &dfiles[enfile+1];
      use_dfiles = dfiles;
    } else {
      use_dfiles = &dfiles[enfile+1];
    }

    // reposition start of list if necessary
    if(!prev_series)
      curr_study->series = curr_series->next;
    else
      prev_series->next = curr_series->next;

    // delete series
    prev_series = curr_series;
    curr_series = curr_series->next;
    delete prev_series;
    curr_study->nseries--;

    // adjust remaining series in this list
    while(curr_series!=NULL)
    {
      curr_series->start-=rmfiles;
      curr_series->end-=rmfiles;
      curr_series = curr_series->next;
    }

    // check if study is empty
    if(curr_study->nseries==0)
    {
      if(!prev_study)
	curr_pat->studies = curr_study->next;
      else
	prev_study->next = curr_study->next;

      prev_study = curr_study;
      curr_study = curr_study->next;
      delete prev_study;
      curr_pat->nstudies--;
    }else {
      curr_study->end-=rmfiles;
      curr_study = curr_study->next;
    }

    // adjust remaining studies in this list
    while(curr_study!=NULL)
    {
      curr_study->start-=rmfiles;
      curr_study->end-=rmfiles;
      curr_series = curr_study->series;
      while(curr_series!=NULL)
      {
	curr_series->start-=rmfiles;
	curr_series->end-=rmfiles;
	curr_series = curr_series->next;
      }
      curr_study = curr_study->next;
    }

    // check if this pat is empty
    if(curr_pat->nstudies==0)
    {
      if(!prev_pat)
	pats = curr_pat->next;
      else
	prev_pat->next = curr_pat->next;

      prev_pat = curr_pat;
      curr_pat = curr_pat->next;
      delete prev_pat;
      npats--;
    }else {
      curr_pat->end-=rmfiles;
      curr_pat = curr_pat->next;
    }

    // adjust remaining pats in this list
    while(curr_pat!=NULL)
    {
      curr_pat->start-=rmfiles;
      curr_pat->end-=rmfiles;
      curr_study = curr_pat->studies;
      while(curr_study!=NULL)
      {
	curr_study->start-=rmfiles;
	curr_study->end-=rmfiles;
	curr_series = curr_study->series;
	while(curr_series!=NULL)
	{
	  curr_series->start-=rmfiles;
	  curr_series->end-=rmfiles;
	  curr_series = curr_series->next;
	}
	curr_study = curr_study->next;
      }
      curr_pat = curr_pat->next;
    }

    // check if we have deleted last pat
    if(npats==0)
    {
      pats = NULL;
      use_dfiles=NULL;
    }

    // fix other data values ptrs
    _nseries--;
    _nfiles-=rmfiles;

    // }}}

    write_DCMLIST(npats, _nseries, _nfiles, pats, use_dfiles);
    output_infos(npats, _nseries, _nfiles, pats, use_dfiles);

    // {{{ free data structs

    curr_pat = pats;

    for(i=0; i<npats; i++)
    {
      curr_study = curr_pat->studies;

      for(j=0; j<curr_pat->nstudies; j++)
      {
	curr_series = curr_study->series;

	for(k=0; k<curr_study->nseries; k++)
	{
	  prev_series = curr_series;
	  curr_series = curr_series->next;
	  delete prev_series;
	}

	prev_study = curr_study;
	curr_study = curr_study->next;
	delete prev_study;
      }

      prev_pat = curr_pat;
      curr_pat = curr_pat->next;
      delete prev_pat;
    }

    delete [] dfiles;

    // }}}
  }

  // return 1 for success
  return(1);
}
// }}}

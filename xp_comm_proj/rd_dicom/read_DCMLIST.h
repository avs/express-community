
// read_DCMLIST.h

#ifndef ESP_READ_DCMLIST_H
#define ESP_READ_DCMLIST_H

typedef struct _DICOM_FILE_INFO
{
  char filename[256];
  char name[64];
  char id[32];
  int studyID;
  int seriesID;
  int aquID;
  int imageID;
  char studyDate[9];
  char seriesDate[9];
  float imagePos[3];
  struct _DICOM_FILE_INFO *next;
} dicom_file_info;

typedef struct _DICOM_SERIES_INFO
{
  int seriesID;
  char seriesDate[9];
  int nfiles;
  int start;
  int end;
  dicom_file_info *dfiles;
  struct _DICOM_SERIES_INFO *next;
} dicom_series_info;

typedef struct _DICOM_STUDY_INFO
{
  int studyID;
  char studyDate[9];
  int nseries;
  int start;
  int end;
  dicom_series_info *series;
  struct _DICOM_STUDY_INFO *next;
} dicom_study_info;

typedef struct _DICOM_PAT_INFO
{
  char name[64];
  char id[32];
  int nstudies;
  int start;
  int end;
  dicom_study_info *studies;
  struct _DICOM_PAT_INFO *next;
} dicom_pat_info;

#endif

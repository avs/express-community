// read_dicom_files.cxx
// by James S Perrin


#define XP_WIDE_API	/* Use Wide APIs */

#include "gen.h"
#include <avs/gd_def.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef MSDOS
#include <string.h>
#else
#include <strings.h>
#endif

#include <avs/fld.h>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimage/diregist.h"
#include "dcmtk/dcmimgle/dcmimage.h"

#ifndef WIN32
typedef unsigned char byte;
#endif

#define ERR(A)  { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A); \
}

#define ERR1(A,p1) { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A, p1); \
}

#define ERR_RETURN(A)  { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A); \
 return(0); \
}

#define ERR_RETURN1(A,p1) { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A, p1); \
 return(0); \
}

static void read_time(DcmDataset *dataset, const DcmTagKey &tagKey, OMXstr *strobj)
{
  char tmpStr[512];
  OFString dcm_string;
  
  if(dataset->findAndGetOFString(tagKey, dcm_string).good())
  {
    int hour, min, sec, msec;
    dcm_string.copy(tmpStr, dcm_string.length());
    tmpStr[dcm_string.length()<512?dcm_string.length():511] = '\0';
    sscanf(tmpStr, "%2d%2d%2d.%d", &hour, &min, &sec, &msec);
    sprintf(tmpStr, "%02d:%02d:%02d", hour, min, sec);
    strobj->set_str_val(tmpStr);
  } else
    strobj->set_str_val("");
}

static void read_date(DcmDataset *dataset, const DcmTagKey &tagKey, OMXstr *strobj)
{
  char tmpStr[512];
  OFString dcm_string;
  
  if(dataset->findAndGetOFString(tagKey, dcm_string).good())
  {
    int year,month,day;
    dcm_string.copy(tmpStr, dcm_string.length());
    tmpStr[dcm_string.length()<512?dcm_string.length():511] = '\0';
    sscanf(tmpStr, "%4d%2d%2d", &year, &month, &day);
    sprintf(tmpStr, "%02d/%02d/%04d", day, month, year);
    strobj->set_str_val(tmpStr);
  } else
    strobj->set_str_val("");
}

static void read_string(DcmDataset *dataset, const DcmTagKey &tagKey, OMXstr *strobj)
{
  char tmpStr[512];
  OFString dcm_string;
  
  if(dataset->findAndGetOFString(tagKey, dcm_string).good())
  {
    dcm_string.copy(tmpStr, dcm_string.length());
    tmpStr[dcm_string.length()<512?dcm_string.length():511] = '\0';
    strobj->set_str_val(tmpStr);
  } else
    strobj->set_str_val("");
}

static void read_ushort(DcmDataset *dataset, const DcmTagKey &tagKey, OMXint *intobj)
{
  Uint16 tmpInt;
  
  if(dataset->findAndGetUint16(tagKey, tmpInt).good())
    *intobj = (int)tmpInt;
  else
    *intobj = 0;
}

static void read_float(DcmDataset *dataset, const DcmTagKey &tagKey, OMXfloat *fltobj)
{
  char tmpStr[512];
  OFString dcm_string;
  
  if(dataset->findAndGetOFString(tagKey, dcm_string).good())
  {
    dcm_string.copy(tmpStr, dcm_string.length());
    tmpStr[dcm_string.length()<512?dcm_string.length():511] = '\0';
    *fltobj = atof(tmpStr);
  } else
    *fltobj = 0;
}

void ReadDICOM_read_dicom_files::read_PatientInfo(DcmDataset *dataset)
{ 
  read_string(dataset, DCM_PatientsName, &header.patient.PatientsName);
  read_string(dataset, DCM_PatientID, &header.patient.PatientID);
  read_date(dataset, DCM_PatientsBirthDate, &header.patient.PatientsBirthDate);
  read_string(dataset, DCM_PatientsSex, &header.patient.PatientsSex);
  read_time(dataset, DCM_PatientsBirthTime, &header.patient.PatientsBirthTime);
  read_string(dataset, DCM_PatientComments, &header.patient.PatientComments);
  read_string(dataset, DCM_OtherPatientIDs, &header.patient.OtherPatientID);
  read_string(dataset, DCM_OtherPatientNames, &header.patient.OtherPatientNames);
  read_string(dataset, DCM_EthnicGroup, &header.patient.EthnicGroup);  
}

void ReadDICOM_read_dicom_files::read_GenStudyInfo(DcmDataset *dataset)
{
  read_string(dataset, DCM_StudyInstanceUID, &header.generalStudy.StudyInstanceUID);
  read_string(dataset, DCM_StudyID, &header.generalStudy.StudyID);
  read_date(dataset, DCM_StudyDate, &header.generalStudy.StudyDate);
  read_time(dataset, DCM_StudyTime, &header.generalStudy.StudyTime);
  read_string(dataset, DCM_ReferringPhysiciansName, &header.generalStudy.ReferringPhysiciansName);
  read_string(dataset, DCM_AccessionNumber, &header.generalStudy.AccessionNumber);
  read_string(dataset, DCM_StudyDescription, &header.generalStudy.StudyDescription);
  read_string(dataset, DCM_NameOfPhysiciansReadingStudy, &header.generalStudy.NameofPhysiciansReadingStudy);
  read_string(dataset, DCM_ReferencedStudySequence, &header.generalStudy.ReferencedStudySequence);
}

void ReadDICOM_read_dicom_files::read_GenSeriesInfo(DcmDataset *dataset)
{
  read_string(dataset, DCM_Modality, &header.generalSeries.Modality);
  read_string(dataset, DCM_SeriesInstanceUID, &header.generalSeries.SeriesInstanceUID);
  read_string(dataset, DCM_SeriesNumber, &header.generalSeries.SeriesNumber);
  read_string(dataset, DCM_Laterality, &header.generalSeries.Laterality);
  read_date(dataset, DCM_SeriesDate, &header.generalSeries.SeriesDate);
  read_time(dataset, DCM_SeriesTime, &header.generalSeries.SeriesTime);
  read_string(dataset, DCM_PerformingPhysiciansName, &header.generalSeries.PerformingPhysiciansName);
  read_string(dataset, DCM_ProtocolName, &header.generalSeries.ProtocolName);
  read_string(dataset, DCM_SeriesDescription, &header.generalSeries.SeriesDescription);
  read_string(dataset, DCM_OperatorsName, &header.generalSeries.OperatorsName);
  read_string(dataset, DCM_ReferencedStudySequence, &header.generalSeries.ReferencedStudyComponentSequence);
  read_string(dataset, DCM_BodyPartExamined, &header.generalSeries.BodyPartExamined);
  read_string(dataset, DCM_PatientPosition, &header.generalSeries.PatientPosition);
  read_ushort(dataset, DCM_SmallestPixelValueInSeries, &header.generalSeries.SmallestPixelValueinSeries);
  read_ushort(dataset, DCM_LargestPixelValueInSeries, &header.generalSeries.LargestPixelValueinSeries);
}

void ReadDICOM_read_dicom_files::read_GenEquipInfo(DcmDataset *dataset)
{
  read_string(dataset, DCM_Manufacturer, &header.generalEquipment.Manufacturer);
  read_string(dataset, DCM_InstitutionName, &header.generalEquipment.InstitutionName);
  read_string(dataset, DCM_InstitutionAddress, &header.generalEquipment.InstitutionAddress);
  read_string(dataset, DCM_StationName, &header.generalEquipment.StationName);
  read_string(dataset, DCM_InstitutionalDepartmentName, &header.generalEquipment.InstitutionalDepartmentName);
  read_string(dataset, DCM_ManufacturersModelName, &header.generalEquipment.ManufacturersModelName);
  read_string(dataset, DCM_DeviceSerialNumber, &header.generalEquipment.DeviceSerialNumber);
  read_string(dataset, DCM_SoftwareVersions, &header.generalEquipment.SoftwareVersions);
  read_string(dataset, DCM_SpatialResolution, &header.generalEquipment.SpatialResolution);
  read_date(dataset, DCM_DateOfLastCalibration, &header.generalEquipment.DateofLastCalibration);
  read_time(dataset, DCM_TimeOfLastCalibration, &header.generalEquipment.TimeofLastCalibration);
  read_ushort(dataset, DCM_PixelPaddingValue, &header.generalEquipment.PixelPaddingValue);
}

void ReadDICOM_read_dicom_files::read_ImageSeries(DcmDataset *dataset)
{
  // Get only the values we won't already have obtained to read the series
  read_string(dataset, DCM_PhotometricInterpretation, &header.imageSeries.Interpretation);
  read_float(dataset, DCM_WindowCenter, &header.imageSeries.WindowCentre);
  read_float(dataset, DCM_WindowWidth, &header.imageSeries.WindowWidth);
}

int ReadDICOM_read_dicom_files::read(OMevent_mask event_mask, int seq_num)
{
  // filenames (OMXstr_array read req)
  // flip (OMXint read)
  // busy (OMXint write)
  // done (OMXint write)
  // header (DICOM_Header write)
  // volume (DICOM_Volume write)

  xp_long i;
  char *filename;
  char interp[80]="";
  char *modality=NULL;
  Uint16 bitsAlloc=0;
  Uint16 bitsStore=0;
  Uint16 pixelRep=0; // 0 unsigned 1 signed
  xp_long height=0;
  xp_long width=0;
  xp_long nImages;
  float sliceSpacing = 1;
  float xSpacing = 1;
  float ySpacing = 1;
  int transpose = 1; // by default images are transposed to form the volume
    
  // DCMTK vars
  DcmFileFormat fileformat;
  DcmDataset *dataset;
  DicomImage *image;
  OFCondition status;
  OFString dcm_string;
  float dcm_float;

  // Sanity Checks
  filenames.get_array_size(&nImages);

  if(nImages == 0) return 1;

  // ******* Open up first image to get infos ************
  filename = filenames.ret_str_array_val(0, NULL);

  if(filename == NULL)
    ERR_RETURN("Can't open a file for reading");
  
  status = fileformat.loadFile(filename);

  if(!status.good())
    ERR_RETURN1("Can't open file %s for reading", filename);

  dataset = fileformat.getDataset();
  

  image = new DicomImage(&fileformat, dataset->getOriginalXfer() );

  if(image == NULL || image->getStatus() != EIS_Normal)
    ERR_RETURN1("Can't open image in file %s for reading", filename);

  // Get basic image dimensions
  width = image->getWidth();
  height = image->getHeight();

  // Further detail from the dataset
  if(!(dataset->findAndGetUint16(DCM_BitsAllocated, bitsAlloc).good()))
    bitsAlloc = 16;
  
  if(!(dataset->findAndGetUint16(DCM_BitsStored, bitsStore).good()))
    bitsStore = bitsAlloc;
  
  if(!(dataset->findAndGetUint16(DCM_PixelRepresentation, pixelRep).good()))
    pixelRep = 266;

  //  printf("Bits Alloc %d Stored %d as %s(%d)\n", bitsAlloc, bitsStore, pixelRep?"signed":"unsigned", pixelRep);

  // Try and determine the spacing of the image slices
  if(dataset->findAndGetOFString(DCM_PixelSpacing, dcm_string, 0).good())
  {
    char tmpStr[256];
    dcm_string.copy(tmpStr, dcm_string.length());
    xSpacing = atof(tmpStr);
    
    dataset->findAndGetOFString(DCM_PixelSpacing, dcm_string, 1);
    dcm_string.copy(tmpStr, dcm_string.length());
    ySpacing = atof(tmpStr);
  }
    
  if(dataset->findAndGetFloat32(DCM_SpacingBetweenSlices, dcm_float).good()) {
    sliceSpacing = dcm_float;
  }
  else if(nImages>1 && dataset->findAndGetOFStringArray(DCM_ImagePositionPatient, dcm_string).good())
  {
    char tmpStr[256];
    float x,y,z,x2,y2,z2;
    
    dcm_string.copy(tmpStr, dcm_string.length());
    sscanf(tmpStr, "%f\\%f\\%f", &x, &y, &z);

    // Get the next image
    delete image;
    fileformat.clear();
    
    filename = filenames.ret_str_array_val(1, NULL);
    
    if(filename == NULL)
      ERR_RETURN("Can't open a file for reading");
    
    status = fileformat.loadFile(filename);
    
    if(!status.good())
      ERR_RETURN1("Can't open file %s for reading", filename);
    
    dataset = fileformat.getDataset();
    image = new DicomImage(&fileformat, dataset->getOriginalXfer() );

    if(nImages>1 && dataset->findAndGetOFStringArray(DCM_ImagePositionPatient, dcm_string).good())
    {
      dcm_string.copy(tmpStr, dcm_string.length());
      sscanf(tmpStr, "%f\\%f\\%f", &x2, &y2, &z2);

      x -= x2; y -= y2; z -= z2;
      sliceSpacing = sqrt(x*x+y*y+z*z);
    }
  }

  // Get additional meta data
  read_PatientInfo(dataset);
  read_GenStudyInfo(dataset);
  read_GenSeriesInfo(dataset);
  read_GenEquipInfo(dataset);
  read_ImageSeries(dataset);
  header.imageSeries.Rows = height;
  header.imageSeries.Columns = width;
  header.imageSeries.Slices = nImages;
  header.imageSeries.BitsAllocated = bitsAlloc;
  header.imageSeries.XSpacing = xSpacing;
  header.imageSeries.YSpacing = ySpacing;
  header.imageSeries.SliceSpacing = sliceSpacing;

  delete image;
  fileformat.clear();

  if(nImages>1)
  {
    // complete imageSeries header for volumes
    header.imageSeries.Slices = nImages;
    header.imageSeries.SliceSpacing = sliceSpacing;

    volume.nspace = 3;
    volume.ndim = 3;
    xp_long *volume_dims = (xp_long *)volume.dims.ret_array_ptr(OM_GET_ARRAY_WR);
    volume_dims[0] = width;
    volume_dims[1] = height;
    volume_dims[2] = nImages;
    ARRfree(volume_dims);
    
    float *volume_points = (float *)volume.points.ret_array_ptr(OM_GET_ARRAY_WR);
    
    volume_points[0] = volume_points[1] = volume_points[2] = 0;
    volume_points[3] = xSpacing*(width -1);
    volume_points[4] = ySpacing*(height -1);
    volume_points[5] = sliceSpacing*(nImages -1);
    ARRfree((char *)volume_points);
  } else { // if a single image just load it as 2D
    transpose = 0;
    volume.nspace = 2;
    volume.ndim = 2;
    xp_long *volume_dims = (xp_long *)volume.dims.ret_array_ptr(OM_GET_ARRAY_WR);
    volume_dims[0] = width;
    volume_dims[1] = height;
    ARRfree(volume_dims);
    
    float *volume_points = (float *)volume.points.ret_array_ptr(OM_GET_ARRAY_WR);
    
    volume_points[0] = volume_points[1] = 0;
    volume_points[2] = xSpacing*(width -1);
    volume_points[3] = ySpacing*(width -1);
    ARRfree((char *)volume_points);
  }
  
  int  volume_data_type;
  
  if(bitsAlloc <= 8) {
    bitsAlloc = 8;
    volume_data_type = OM_TYPE_BYTE;
  } else if (bitsAlloc <= 16) {
    bitsAlloc = 16;
    volume_data_type = OM_TYPE_SHORT; 
  } else if (bitsAlloc <= 32) {
    bitsAlloc = 32;
    volume_data_type = OM_TYPE_INT; 
  }

  volume.nnode_data = 1;
  volume.node_data[0].veclen = 1;
  OMset_data_type(volume.node_data[0].values.obj_id(), volume_data_type);
  char *volume_node_data = (char *)volume.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
  
  xp_long imageSize = width*height*OMtype_size(volume_data_type);
  char *volPtr, *imgPtr;
  xp_long v, nV = width*height;
  byte *binPtr, *boutPtr;
  short *sinPtr, *soutPtr;
  unsigned short *usinPtr;
  int *iinPtr, *ioutPtr;
  
  for(i=0; i<nImages; i++)
  {
    filename = filenames.ret_str_array_val(i, NULL);
    
    if(filename == NULL)
      ERR_RETURN("Can't open a file for reading");
    
    status = fileformat.loadFile(filename);
    
    if(!status.good())
      ERR_RETURN1("Can't open file %s for reading", filename);
    
    dataset = fileformat.getDataset();
    
    image = new DicomImage(&fileformat, dataset->getOriginalXfer() );
    
    if(image == NULL || image->getStatus() != EIS_Normal)
      ERR_RETURN1("Can't open image in file %s for reading", filename);

    //image->setMinMaxWindow();
    imgPtr = (char *)image->getOutputData(bitsAlloc);

    if(imgPtr)
    {
      
      if(transpose)
      {
	volPtr = volume_node_data+(i+1)*imageSize - OMtype_size(volume_data_type); 
	
	// Images needs to be transposed so as to appear correctly when
	// stored in a volume
	switch (volume_data_type)
	{
	case OM_TYPE_BYTE:
	  {
	    binPtr = (byte *)imgPtr; boutPtr = (byte *)volPtr;
	    
	    for(v=0; v<nV; v++) *boutPtr-- = *binPtr++;
	  }
	  break;
	case OM_TYPE_SHORT:
	  {
	    sinPtr = (short *)imgPtr; soutPtr = (short *)volPtr;
	    usinPtr = (unsigned short *)imgPtr;
	    
	    // Grrr Express doesn't support unsigned shorts so subtract
	    // 32768 from each value
	    // If not 16 bits Stored we don't need to convert unsigned data
	    if(bitsAlloc == 16 && bitsStore == 16 && pixelRep == 1)
	    {
	      for(v=0; v<nV; v++) *soutPtr-- = ((short)*usinPtr++) ^ 0x8000;
	    } else {
	      for(v=0; v<nV; v++) *soutPtr-- = *sinPtr++;
	    }
	  }
	  break;
	case OM_TYPE_FLOAT:
	  {
	    iinPtr = (int *)imgPtr; ioutPtr = (int *)volPtr;
	    
	    for(v=0; v<nV; v++) *ioutPtr-- = *iinPtr++;
	  }
	  break;
	}
      } else {
	volPtr = volume_node_data+i*imageSize;
	
	// tranpose not required, eg single image
	if(bitsAlloc == 16 && bitsStore == 16 && pixelRep == 1)
	{
	  soutPtr = (short *)volPtr;
	  usinPtr = (unsigned short *)imgPtr;
	  
	  for(v=0; v<nV; v++) *soutPtr++ = (short) ((int)(*usinPtr++) -32768);
	} else
	  memcpy(volPtr, imgPtr, imageSize);
      }

    }
    
    delete image;
    fileformat.clear();

    if(i & 0x0f) OMstatus_check((100*i)/nImages, "Read DICOM Images", NULL);

  }

  if(volume_node_data) ARRfree(volume_node_data);
  
   // return 1 for success
  return(1);
}

/******************************************************************************/
/*									      */
/*	Project  : P A P Y R U S  Toolkit				      */
/*	File     : PapyEnumModules3.h					      */
/*	Function : contains the declarations of the modules names.	      */
/*	Authors  : Christian Girard					      */
/*	History  : 04.1994	version 3.0				      */
/*								   	      */
/*	(C) 1994-1996 The University Hospital of Geneva			      */
/*		       All Rights Reserved				      */
/*									      */
/******************************************************************************/


#ifndef PapyEnumModulesH 
#define PapyEnumModulesH

#ifndef FILENAME83		/* this is for the normal machines ... */

#include "PapyEnumImagesModules3.h"

#ifndef   PapyEnumMiscModulesH
#include "PapyEnumMiscModules3.h"
#endif

#else				/* FILENAME83 defined for the DOS machines */

#include "PAPEIM3.h"

#ifndef   PapyEnumMiscModulesH
#include "PAPEMM3.h"
#endif

#endif 				/* FILENAME83 defined */


/* 	enumeration of the modules	*/

enum modules {
Audio,
BasicAnnotationPresentation,
BasicFilmBoxPresentation,
BasicFilmBoxRelationship,
BasicFilmSessionPresentation,
ContrastBolus,
CurveIdentification,
FileReference,
FrameOfReference,
GeneralEquipment,
GeneralPatientSummary,
GeneralSeriesSummary,
GeneralVisitSummary,
GeneralImage,
GeneralStudy,
GeneralStudySummary,
IconImage,
IdentifyingImageSequence,
ImageIdentification,
ImagePointer,
ImageSequencePap,
InternalImagePointerSequence,
LUTIdentification,
ModalityLUT,
Multi_frameOverlay,
Multi_Frame,
OverlayIdentification,
PatientDemographic,
PatientIdentification,
PatientMedical,
Patient,
PatientRelationship,
PatientStudy,
PatientSummary,
PixelOffset,
Printer,
PrintJob,
ResultIdentification,
ResultsImpression,
ResultRelationship,
SCImageEquipment,
SCImage,
SOPCommon,
StudyAcquisition,
StudyClassification,
StudyComponentAcquisition,
StudyComponent,
StudyComponentRelationship,
StudyContent,
StudyIdentification,
StudyRead,
StudyRelationship,
StudyScheduling,
USImage,
USFrameofReference,
USRegionCalibration,
VisitAdmission,
VisitDischarge,
VisitIdentification,
VisitRelationship,
VisitScheduling,
VisitStatus,
VOILUT,
ImagePlane,
InterpretationApproval,
InterpretationTranscription,
InterpretationState,
InterpretationRecording,
InterpretationIdentification,
InterpretationRelationship,
Cine,
CRImage,
CRSeries,
CTImage,
Curve,
GeneralSeries,
ImageOverlayBoxRelationship,
ImageBoxRelationship,
ImageBoxPixelPresentation,
ImageOverlayBoxPresentation,
ImagePixel,
OverlayPlane,
NMSPECTAcquisitionImage,
NMSeries,
NMMulti_gatedAcquisitionImage,
NMImage,
NMEquipment,
MRImage,
ExternalVisitReferenceSequence,
ExternalStudyFileReferenceSequence,
ExternalPatientFileReferenceSequence,
ExternalPapyrus_FileReferenceSequence,
BasicFilmSessionRelationship,
UINOverlaySequence,
END_MODULE
};

#endif	    /* PapyEnumModulesH */

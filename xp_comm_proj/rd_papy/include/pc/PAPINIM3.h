/********************************************************************************/
/*										*/
/*	Project  : P A P Y R U S  Toolkit					*/
/*	File     : PapyInitModules3.h						*/
/*	Function : declaration of the init fct.                                 */
/*	Authors  : Christian Girard						*/
/*		   Marianne Logean						*/
/*										*/
/*	History  : 06.1994	version 3.0					*/
/*	           06.1995	version 3.1					*/
/*										*/
/*	(C) 1994, 1995 The University Hospital of Geneva      			*/
/*		       All Rights Reserved					*/
/*										*/
/********************************************************************************/

#ifndef PapyInitModules3H 
#define PapyInitModules3H
#endif

/* ------------------------- includes ------------------------------------------*/


#ifndef Papyrus3H
#include "Papyrus3.h"
#endif

/* ------------------------- functions definition ------------------------------*/

#ifdef _NO_PROTO
extern void init_group2();
extern void init_group8();
extern void init_group10();
extern void init_group18();
extern void init_group20();
extern void init_group28();
extern void init_group32();
extern void init_group38();
extern void init_group41();
extern void init_group88();
extern void init_group2000();
extern void init_group2010();
extern void init_group2020();
extern void init_group2030();
extern void init_group2040();
extern void init_group2100();
extern void init_group2110();
extern void init_group4008();
extern void init_group5000();
extern void init_group6000();
extern void init_group7FE0();
extern void init_uinoverlay ();
extern void init_Audio ();
extern void init_BasicAnnotationPresentation ();
extern void init_BasicFilmBoxPresentation ();
extern void init_BasicFilmBoxRelationship ();
extern void init_BasicFilmSessionPresentation ();
extern void init_ContrastBolus ();
extern void init_CurveIdentification ();
extern void init_FileReference ();
extern void init_FrameOfReference ();
extern void init_GeneralEquipment ();
extern void init_GeneralPatientSummary ();
extern void init_GeneralSeriesSummary ();
extern void init_GeneralVisitSummary ();
extern void init_GeneralImage ();
extern void init_GeneralStudy ();
extern void init_GeneralStudySummary ();
extern void init_IconImage ();
extern void init_IdentifyingImageSequence ();
extern void init_ImageIdentification ();
extern void init_ImagePointer ();
extern void init_ImageSequence ();
extern void init_InternalImagePointerSequence ();
extern void init_LUTIdentification ();
extern void init_ModalityLUT ();
extern void init_Multi_frameOverlay ();
extern void init_Multi_Frame ();
extern void init_OverlayIdentification ();
extern void init_PatientDemographic ();
extern void init_PatientIdentification ();
extern void init_PatientMedical ();
extern void init_Patient () ;
extern void init_PatientRelationship ();
extern void init_PatientStudy ();
extern void init_PatientSummary ();
extern void init_PixelOffset ();
extern void init_Printer ();
extern void init_PrintJob ();
extern void init_ResultIdentification ();
extern void init_ResultsImpression ();
extern void init_ResultRelationship ();
extern void init_SCImageEquipment ();
extern void init_SCImage ();
extern void init_SOPCommon ();
extern void init_StudyAcquisition ();
extern void init_StudyClassification ();
extern void init_StudyComponentAcquisition ();
extern void init_StudyComponent ();
extern void init_StudyComponentRelationship ();
extern void init_StudyContent ();
extern void init_StudyIdentification ();
extern void init_StudyRead ();
extern void init_StudyRelationship ();
extern void init_StudyScheduling ();
extern void init_USImage ();
extern void init_USFrameofReference ();
extern void init_USRegionCalibration ();
extern void init_VisitAdmission ();
extern void init_VisitDischarge ();
extern void init_VisitIdentification ();
extern void init_VisitRelationship ();
extern void init_VisitScheduling ();
extern void init_VisitStatus ();
extern void init_VOILUT ();
extern void init_ImagePlane ();
extern void init_InterpretationApproval ();
extern void init_InterpretationTranscription ();
extern void init_InterpretationState ();
extern void init_InterpretationRecording ();
extern void init_InterpretationIdentification ();
extern void init_InterpretationRelationship ();
extern void init_Cine ();
extern void init_CRImage ();
extern void init_CRSeries ();
extern void init_CTImage ();
extern void init_Curve ();
extern void init_GeneralSeries ();
extern void init_ImageOverlayBoxRelationship ();
extern void init_ImageBoxRelationship ();
extern void init_ImageBoxPixelPresentation ();
extern void init_ImageOverlayBoxPresentation ();
extern void init_ImagePixel ();
extern void init_OverlayPlane ();
extern void init_NMSPECTAcquisitionImage ();
extern void init_NMSeries ();
extern void init_NMMulti_gatedAcquisitionImage ();
extern void init_NMImage ();
extern void init_NMEquipment ();
extern void init_MRImage ();
extern void init_ExternalVisitReferenceSequence ();
extern void init_ExternalStudyFileReferenceSequence ();
extern void init_ExternalPatientFileReferenceSequence ();
extern void init_ExternalPapyrus_FileReferenceSequence ();
extern void init_BasicFilmSessionRelationship ();
extern void init_UINOverlaySequence ();
#else
extern void init_group2(ELEMENT[]);
extern void init_group8(ELEMENT[]);
extern void init_group10(ELEMENT[]);
extern void init_group18(ELEMENT[]);
extern void init_group20(ELEMENT[]);
extern void init_group28(ELEMENT[]);
extern void init_group32(ELEMENT[]);
extern void init_group38(ELEMENT[]);
extern void init_group41(ELEMENT[]);
extern void init_group88(ELEMENT[]);
extern void init_group2000(ELEMENT[]);
extern void init_group2010(ELEMENT[]);
extern void init_group2020(ELEMENT[]);
extern void init_group2030(ELEMENT[]);
extern void init_group2040(ELEMENT[]);
extern void init_group2100(ELEMENT[]);
extern void init_group2110(ELEMENT[]);
extern void init_group4008(ELEMENT[]);
extern void init_group5000(ELEMENT[]);
extern void init_group6000(ELEMENT[]);
extern void init_group7FE0(ELEMENT[]);
extern void init_uinoverlay (ELEMENT[]);
extern void init_Audio (ELEMENT[]);
extern void init_BasicAnnotationPresentation (ELEMENT[]);
extern void init_BasicFilmBoxPresentation (ELEMENT[]);
extern void init_BasicFilmBoxRelationship (ELEMENT[]);
extern void init_BasicFilmSessionPresentation (ELEMENT[]);
extern void init_ContrastBolus (ELEMENT[]);
extern void init_CurveIdentification (ELEMENT[]);
extern void init_FileReference (ELEMENT[]);
extern void init_FrameOfReference (ELEMENT[]);
extern void init_GeneralEquipment (ELEMENT[]);
extern void init_GeneralPatientSummary (ELEMENT[]);
extern void init_GeneralSeriesSummary (ELEMENT[]);
extern void init_GeneralVisitSummary (ELEMENT[]);
extern void init_GeneralImage (ELEMENT[]);
extern void init_GeneralStudy (ELEMENT[]);
extern void init_GeneralStudySummary (ELEMENT[]);
extern void init_IconImage (ELEMENT[]);
extern void init_IdentifyingImageSequence (ELEMENT[]);
extern void init_ImageIdentification (ELEMENT[]);
extern void init_ImagePointer (ELEMENT[]);
extern void init_ImageSequence (ELEMENT[]);
extern void init_InternalImagePointerSequence (ELEMENT[]);
extern void init_LUTIdentification (ELEMENT[]);
extern void init_ModalityLUT (ELEMENT[]);
extern void init_Multi_frameOverlay (ELEMENT[]);
extern void init_Multi_Frame (ELEMENT[]);
extern void init_OverlayIdentification (ELEMENT[]);
extern void init_PatientDemographic (ELEMENT[]);
extern void init_PatientIdentification (ELEMENT[]);
extern void init_PatientMedical (ELEMENT[]);
extern void init_Patient (ELEMENT[]) ;
extern void init_PatientRelationship (ELEMENT[]);
extern void init_PatientStudy (ELEMENT[]);
extern void init_PatientSummary (ELEMENT[]);
extern void init_PixelOffset (ELEMENT[]);
extern void init_Printer (ELEMENT[]);
extern void init_PrintJob (ELEMENT[]);
extern void init_ResultIdentification (ELEMENT[]);
extern void init_ResultsImpression (ELEMENT[]);
extern void init_ResultRelationship (ELEMENT[]);
extern void init_SCImageEquipment (ELEMENT[]);
extern void init_SCImage (ELEMENT[]);
extern void init_SOPCommon (ELEMENT[]);
extern void init_StudyAcquisition (ELEMENT[]);
extern void init_StudyClassification (ELEMENT[]);
extern void init_StudyComponentAcquisition (ELEMENT[]);
extern void init_StudyComponent (ELEMENT[]);
extern void init_StudyComponentRelationship (ELEMENT[]);
extern void init_StudyContent (ELEMENT[]);
extern void init_StudyIdentification (ELEMENT[]);
extern void init_StudyRead (ELEMENT[]);
extern void init_StudyRelationship (ELEMENT[]);
extern void init_StudyScheduling (ELEMENT[]);
extern void init_USImage (ELEMENT[]);
extern void init_USFrameofReference (ELEMENT[]);
extern void init_USRegionCalibration (ELEMENT[]);
extern void init_VisitAdmission (ELEMENT[]);
extern void init_VisitDischarge (ELEMENT[]);
extern void init_VisitIdentification (ELEMENT[]);
extern void init_VisitRelationship (ELEMENT[]);
extern void init_VisitScheduling (ELEMENT[]);
extern void init_VisitStatus (ELEMENT[]);
extern void init_VOILUT (ELEMENT[]);
extern void init_ImagePlane (ELEMENT[]);
extern void init_InterpretationApproval (ELEMENT[]);
extern void init_InterpretationTranscription (ELEMENT[]);
extern void init_InterpretationState (ELEMENT[]);
extern void init_InterpretationRecording (ELEMENT[]);
extern void init_InterpretationIdentification (ELEMENT[]);
extern void init_InterpretationRelationship (ELEMENT[]);
extern void init_Cine (ELEMENT[]);
extern void init_CRImage (ELEMENT[]);
extern void init_CRSeries (ELEMENT[]);
extern void init_CTImage (ELEMENT[]);
extern void init_Curve (ELEMENT[]);
extern void init_GeneralSeries (ELEMENT[]);
extern void init_ImageOverlayBoxRelationship (ELEMENT[]);
extern void init_ImageBoxRelationship (ELEMENT[]);
extern void init_ImageBoxPixelPresentation (ELEMENT[]);
extern void init_ImageOverlayBoxPresentation (ELEMENT[]);
extern void init_ImagePixel (ELEMENT[]);
extern void init_OverlayPlane (ELEMENT[]);
extern void init_NMSPECTAcquisitionImage (ELEMENT[]);
extern void init_NMSeries (ELEMENT[]);
extern void init_NMMulti_gatedAcquisitionImage (ELEMENT[]);
extern void init_NMImage (ELEMENT[]);
extern void init_NMEquipment (ELEMENT[]);
extern void init_MRImage (ELEMENT[]);
extern void init_ExternalVisitReferenceSequence (ELEMENT[]);
extern void init_ExternalStudyFileReferenceSequence (ELEMENT[]);
extern void init_ExternalPatientFileReferenceSequence (ELEMENT[]);
extern void init_ExternalPapyrus_FileReferenceSequence (ELEMENT[]);
extern void init_BasicFilmSessionRelationship (ELEMENT[]);
extern void init_UINOverlaySequence (ELEMENT[]);
#endif



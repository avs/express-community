/********************************************************************************/
/*									      	*/
/*	Project  : P A P Y R U S  Toolkit				      	*/
/*	File     : PapyEnumImagesModules3.h					*/
/*	Function : contains the declarations of the modules needed for the  	*/
/*		   image objects .						*/
/*	Authors  : Christian Girard					      	*/
/*	History  : 08.1994	version 3.0				      	*/
/*								   	      	*/
/*	(C) 1994-1996 The University Hospital of Geneva			      	*/
/*		       All Rights Reserved				      	*/
/*									      	*/
/********************************************************************************/


#ifndef PapyEnumImagesModulesH 
#define PapyEnumImagesModulesH



/*	Module : Patient				*/

enum {
papPatientsNameP,
papPatientIDP,
papPatientsBirthDateP,
papPatientsSexP,
papReferencedPatientSequenceP,
papPatientsBirthTimeP,
papOtherPatientID,
papOtherPatientNamesP,
papEthnicGroupP,
papPatientCommentsP,
papEndPatient
};


/*	Module : General Study				*/

enum {
papStudyInstanceUIDGS,
papStudyDateGS,
papStudyTimeGS,
papReferringPhysiciansNameGS,
papStudyIDGS,
papAccessionNumberGS,
papStudyDescriptionGS,
papNameofPhysiciansReadingStudyGS,
papReferencedStudySequenceGS,
papEndGeneralStudy
};


/*	Module : Patient Study				*/

enum {
papAdmittingDiagnosesDescription,
papPatientsAge,
papPatientsSizePS,
papPatientsWeightPS,
papOccupation,
papAdditionalPatientsHistory,
papEndPatientStudy
};


/*	Module : General Series				*/

enum {
papModalityGS,
papSeriesInstanceUIDGS,
papSeriesNumberGS,
papLaterality,
papSeriesDate,
papSeriesTime,
papPerformingPhysiciansNameGS,
papProtocolName,
papSeriesDescription,
papOperatorsName,
papReferencedStudyComponentSequenceGS,
papBodyPartExaminedGS,
papPatientPosition,
papSmallestPixelValueinSeries,
papLargestPixelValueinSeries,
papEndGeneralSeries
};


/*	Module : CR Series				*/

enum {
papBodyPartExaminedCRS,
papViewPosition,
papFilterTypeCRS,
papCollimatorgridnameCRS,
papFocalSpotCRS,
papPlateType,
papPhosphorType,
papEndCRSeries
};


/*	Module : NM Series				*/

enum {
papRadionuclide,
papNuclearMedicineSeriesType,
papScanVelocity,
papWholeBodyTechnique,
papScanLength,
papRadiopharmaceutical,
papEnergyWindowCenterline,
papEnergyWindowTotalWidth,
papInterventionDrugName,
papInterventionalDrugStartTime,
papEffectiveSeriesDuration,
papSyringecounts,
papTriggerSourceorTypeNMS,
papRadionuclideRoute,
papRadionuclideVolume,
papRadionuclideStartTime,
papRadionuclideStopTime,
papRadionuclideTotalDose,
papGantryDetectorTiltNMS,
papIsotopeNumber,
papEndNMSeries
};


/*	Module : Frame Of Reference			*/

enum {
papFrameofReferenceUID,
papPositionReferenceIndicator,
papEndFrameOfReference
};


/*	Module : General Equipment			*/

enum {
papManufacturerGE,
papInstitutionNameGE,
papInstitutionAddressGE,
papStationName,
papInstitutionalDepartmentName,
papManufacturersModelName,
papDeviceSerialNumberGE,
papSoftwareVersionsGE,
papSpatialResolution,
papDateofLastCalibration,
papTimeofLastCalibration,
papPixelPaddingValue,
papEndGeneralEquipment
};


/*	Module : NM Equipment				*/

enum {
papCenterofRotationOffset,
papFieldofViewShape,
papFieldofViewDimensions,
papCollimatorgridnameNME,
papCollimatorType,
papFocalDistance,
papXFocusCenter,
papYFocusCenter,
papEndNMEquipment
};


/*	Module : SC Image Equipment			*/

enum {
papConversionType,
papModalitySIE,
papSecondaryCaptureDeviceID,
papSecondaryCaptureDeviceManufacturer,
papSecondaryCaptureDeviceManufacturersModelName,
papSecondaryCaptureDeviceSoftwareVersion,
papVideoImageFormatAcquired,
papDigitalImageFormatAcquired,
papEndSCImageEquipment
};


/*	Module : General Image				*/

enum {
papImageNumberGI,
papPatientOrientation,
papImageDate,
papImageTime,
papImageTypeGI,
papAcquisitionNumberGI,
papAcquisitionDate,
papAcquisitionTime,
papReferencedImageSequenceGI,
papDerivationDescription,
papSourceImageSequence,
papImagesinAcquisition,
papImageComments,
papEndGeneralImage
};


/*	Module : Image Plane				*/

enum {
papPixelSpacing,
papImageOrientationPatient,
papImagePositionPatient,
papSliceThickness,
papSliceLocation,
papEndImagePlane
};


/*	Module : Image Pixel				*/

enum {
papSamplesperPixelIP,
papPhotometricInterpretationIP,
papRows,
papColumns,
papBitsAllocatedIP,
papBitsStoredIP,
papHighBitIP,
papPixelRepresentationIP,
papPixelData,
papPlanarConfiguration,
papPixelAspectRatio,
papSmallestImagePixelValue,
papLargestImagePixelValue,
papRedPaletteColorLookupTableDescriptor,
papGreenPaletteColorLookupTableDescriptor,
papBluePaletteColorLookupTableDescriptor,
papRedPaletteColorLookupTableData,
papGreenPaletteColorLookupTableData,
papBluePaletteColorLookupTableData,
papEndImagePixel
};


/*	Module : Contrast Bolus				*/

enum {
papContrastBolusAgent,
papContrastBolusRoute,
papContrastBolusVolume,
papContrastBolusStartTime,
papContrastBolusStopTime,
papContrastBolusTotalDose,
papEndContrastBolus
};


/*	Module : Cine					*/

enum {
papFrameTimeC,
papFrameTimeVector,
papStartTrim,
papStopTrim,
papRecommendedDisplayFrameRate,
papCineRate,
papFrameDelay,
papEffectiveDuration,
papActualFrameDurationC,
papEndCine
};


/*	Module : Multi_Frame				*/

enum {
papNumberofFrames,
papFrameIncrementPointerMF,
papEndMulti_Frame
};



/*	Module : CR Image				*/

enum {
papKVPCRI,
papPlateID,
papDistanceSourcetoDetectorCRI,
papDistanceSourcetoPatientCRI,
papExposureTimeCRI,
papXrayTubeCurrentCRI,
papExposureCRI,
papGeneratorPowerCRI,
papAcquisitionDeviceProcessingDescription,
papAcquisitionDeviceProcessingCode,
papCassetteOrientation,
papCassetteSize,
papExposuresonPlate,
papRelativeXrayExposure,
papSensitivity,
papEndCRImage
};


/*	Module : CT Image				*/

enum {
papImageTypeCTI,
papSamplesperPixelCTI,
papPhotometricInterpretationCTI,
papBitsAllocatedCTI,
papBitsStoredCTI,
papHighBitCTI,
papRescaleInterceptCTI,
papRescaleSlopeCTI,
papKVPCTI,
papAcquisitionNumberCTI,
papScanOptionsCTI,
papDataCollectionDiameter,
papReconstructionDiameterCTI,
papDistanceSourcetoDetectorCTI,
papDistanceSourcetoPatientCTI,
papGantryDetectorTiltCTI,
papTableHeightCTI,
papRotationDirectionCTI,
papExposureTimeCTI,
papXrayTubeCurrentCTI,
papExposureCTI,
papFilterTypeCTI,
papGeneratorPowerCTI,
papFocalSpotCTI,
papConvolutionKernelCTI,
papEndCTImage
};


/*	Module : MR Image				*/

enum {
papImageTypeMRI,
papSamplesperPixelMRI,
papPhotometricInterpretationMRI,
papBitsAllocatedMRI,
papScanningSequence,
papSequenceVariant,
papScanOptionsMRI,
papRepetitionTime,
papEchoTime,
papEchoTrainLength,
papInversionTime,
papTriggerTimeMRI,
papSequenceName,
papAngioFlag,
papNumberofAverages,
papImagingFrequency,
papImagedNucleus,
papEchoNumber,
papMagneticFieldStrength,
papSpacingBetweenSlices,
papNumberofPhaseEncodingSteps,
papPercentSampling,
papPercentPhaseFieldofView,
papPixelBandwidth,
papNominalIntervalMRI,
papBeatRejectionFlagMRI,
papLowRRValueMRI,
papHighRRValueMRI,
papIntervalsAcquiredMRI,
papIntervalsRejectedMRI,
papPVCRejectionMRI,
papSkipBeatsMRI,
papHeartRateMRI,
papCardiacNumberofImagesMRI,
papTriggerWindow,
papReconstructionDiameterMRI,
papReceivingCoil,
papTransmittingCoil,
papAcquisitionMatrix,
papPhaseEncodingDirection,
papFlipAngle,
papSAR,
papVariableFlipAngleFlag,
papdBdt,
papTemporalPositionIdentifier,
papNumberofTemporalPositions,
papTemporalResolution,
papEndMRImage
};


/*	Module : NM Image				*/

enum {
papFrameIncrementPointerNMI,
papZoomFactorNMI,
papCountsAccumulated,
papReferencedOverlaySequenceNMI,
papReferencedCurveSequenceNMI,
papAcquisitionTerminationCondition,
papReconstructionDiameterNMI,
papDistanceSourcetoDetectorNMI,
papTableHeightNMI,
papAngularPositionNMI,
papRadialPositionNMI,
papConvolutionKernelNMI,
papActualFrameDurationNMI,
papCountRate,
papPreprocessingFunctionNMI,
papPostprocessingFunction,
papPhaseNumber,
papIntervalNumber,
papTimeSlotNumber,
papAngleNumberNMI,
papZoomCenterNMI,
papCorrectedImage,
papEndNMImage
};


/*	Module : NM SPECT Acquisition Image		*/

enum {
papFrameIncrementPointerNSAI,
papAngularStep,
papZoomFactorNSAI,
papScanArc,
papReconstructionDiameterNSAI,
papDistanceSourcetoDetectorNSAI,
papTableHeightNSAI,
papTableTraverse,
papRotationDirectionNSAI,
papAngularPositionNSAI,
papRadialPositionNSAI,
papRotationOffset,
papAngleNumberNSAI,
papZoomCenterNSAI,
papEndNMSPECTAcquisitionImage
};


/*	Module : NM Multi_gated Acquisition Image	*/

enum {
papFrameIncrementPointerNMAI,
papTriggerSourceorTypeNMAI,
papFrameTimeNMAI,
papTriggerTimeNMAI,
papNominalIntervalNMAI,
papFramingType,
papBeatRejectionFlagNMAI,
papLowRRValueNMAI,
papHighRRValueNMAI,
papIntervalsAcquiredNMAI,
papIntervalsRejectedNMAI,
papPVCRejectionNMAI,
papSkipBeatsNMAI,
papHeartRateNMAI,
papCardiacNumberofImagesNMAI,
papEndNMMulti_gatedAcquisitionImage
};


/*	Module : US Region Calibration			*/

enum {
papSequenceofUltrasoundRegions,
papEndUSRegionCalibration
};


/*	Module : US Image				*/

enum {
papPhotometricInterpretationUSI,
papPixelRepresentationUSI,
papFrameIncrementPointerUSI,
papImageTypeUSI,
papNumberofStages,
papNumberofViewsinStage,
papReferencedOverlaySequenceUSI,
papReferencedCurveSequenceUSI,
papStageName,
papStageNumber,
papViewNumber,
papNumberofEventTimers,
papEventElapsedTimes,
papEventTimerNames,
papTransducerPosition,
papTransducerOrientation,
papAnatomicStructure,
papTriggerTimeUSI,
papNominalIntervalUSI,
papBeatRejectionFlagUSI,
papLowRRValueUSI,
papHighRRValueUSI,
papHeartRateUSI,
papOutputPower,
papTransducerData,
papTransducerType,
papFocusDepth,
papPreprocessingFunctionUSI,
papMechanicalIndex,
papBoneThermalIndex,
papCranialThermalIndex,
papSoftTissueThermalIndex,
papSoftTissuefocusThermalIndex,
papSoftTissuesurfaceThermalIndex,
papDepthofScanField,
papImageTransformationMatrix,
papImageTranslationVector,
papEndUSImage
};


/*	Module : SC Image				*/

enum {
papDateofSecondaryCapture,
papTimeofSecondaryCapture,
papEndSCImage
};


/*	Module : Overlay Identification			*/

enum {
papOverlayNumber,
papOverlayDate,
papOverlayTime,
papReferencedImageSequenceOI,
papEndOverlayIdentification
};


/*	Module : Overlay Plane				*/

enum {
papRowsOP,
papColumnsOP,
papOverlayType,
papOrigin,
papBitsAllocatedOP,
papBitPosition,
papOverlayData,
papROIArea,
papROIMean,
papROIStandardDeviation,
papOverlayDescriptorGray,
papOverlayDescriptorRed,
papOverlayDescriptorGreen,
papOverlayDescriptorBlue,
papOverlaysGray,
papOverlaysRed,
papOverlaysGreen,
papOverlaysBlue,
papEndOverlayPlane
};


/*	Module : Multi_frame Overlay			*/

enum {
papNumberofFramesinOverlay,
papEndMulti_frameOverlay
};


/*	Module : Curve Identification			*/

enum {
papCurveNumber,
papCurveDate,
papCurveTime,
papReferencedImageSequenceCI,
papReferencedOverlaySequenceCI,
papReferencedCurveSequenceCI,
papEndCurveIdentification
};


/*	Module : Curve					*/

enum {
papCurveDimensions,
papNumberofPoints,
papTypeofData,
papDataValueRepresentation,
papCurveData,
papCurveDescription,
papAxisUnits,
papAxisLabels,
papMinimumCoordinateValue,
papMaximumCoordinateValue,
papCurveRange,
papCurveDataDescriptor,
papCoordinateStartValue,
papCoordinateStepValue,
papEndCurve
};


/*	Module : General Patient Summary			*/

enum {
papPatientsNameGPS,
papPatientsID,
papPatientsBirthDateGPS,
papPatientsSexGPS,
papPatientsHeight,
papPatientsWeightGPS,
papEndGeneralPatientSummary
};


/*	Module : General Visit Summary			*/

enum {
papCurrentPatientLocationGVS,
papPatientsInstitutionResidenceGVS,
papInstitutionNameVS,
papEndGeneralVisitSummary
};


/*	Module : General Study Summary			*/

enum {
papStudyDateGSS,
papStudyTimeGSS,
papStudyUID,
papStudyIDGSS,
papAccessionnumberGSS,
papReferringPhysiciansNameGSS,
papEndGeneralStudySummary
};


/*	Module : General Series Summary			*/

enum {
papModalityGSS,
papSeriesInstanceUIDGSS,
papSeriesNumberGSS,
papNumberofimages,
papEndGeneralSeriesSummary
};


/*	Module : Icon Image				*/

enum {
papSamplesperPixelII,
papPhotometricInterpretationII,
papRowsII,
papColumnsII,
papBitsAllocatedII,
papBitsStoredII,
papHighBitII,
papPixelRepresentationII,
papRedPaletteColorLookuptableDescriptors,
papBluePaletteColorLookupTableDescriptors,
papGreenPaletteColorLookupTableDescriptors,
papRedPaletteColorLookupTableDataII,
papBluePaletteColorLookupTableDataII,
papGreenPaletteColorLookupTableDataII,
papPixelDataII,
papEndIconImage
};


/*	Module : Image Identification			*/

enum {
papReferencedImageSOPClassUIDII,
papReferencedImageSOPInstanceUID,
papImageNumberII,
papEndImageIdentification
};


/*	Module : Image Pointer				*/

enum {
papImagePointer,
papEndImagePointer
};


/*	Module : Image Sequence				*/

enum {
papImageSequence,
papEndImageSequence
};


/*	Module : Internal Image Pointer Sequence	*/

enum {
papPointerSequence,
papEndInternalImagePointerSequence
};


/*	Module : Pixel Offset				*/

enum {
papPixelOffset,
papEndPixelOffset
};


/*	Module : Audio					*/

enum {
papAudioType,
papAudioSampleFormat,
papNumberofChannels,
papNumberofSamples,
papSampleRate,
papTotalTime,
papAudioSampleData,
papReferencedImageSequenceAudio,
papAudioComments,
papEndAudio
};


/*	Module : Modality LUT				*/

enum {
papModalityLUTSequence,
papRescaleInterceptML,
papRescaleSlopeML,
papRescaleType,
papEndModalityLUT
};


/*	Module : VOI LUT				*/

enum {
papVOILUTSequence,
papWindowCenter,
papWindowWidth,
papWindowCenterWidthExplanation,
papEndVOILUT
};


/*	Module : SOP Common				*/

enum {
papSOPClassUID,
papSOPInstanceUID,
papSpecificCharacterSet,
papInstanceCreationDate,
papInstanceCreationTime,
papInstanceCreatorUID,
papEndSOPCommon
};


/*	Module : UIN Overlay Sequence			*/

enum {
papOwnerID,
papUINOverlaySequence,
papEndUINOverlaySequence
};

#endif	    /* PapyEnumImagesModulesH */

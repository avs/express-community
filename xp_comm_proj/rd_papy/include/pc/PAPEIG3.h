/******************************************************************************/
/*									      */
/*	Project  : P A P Y R U S  Toolkit				      */
/*	File     : PapyEnumImageGroups3.h				      */
/*	Function : contains the declarations of the groups names and of the   */
/*		   elements names					      */
/*	Authors  : Christian Girard					      */
/*	History  : 04.1994	version 3.0				      */
/*								   	      */
/*	(C) 1994-1996 The University Hospital of Geneva			      */
/*		       All Rights Reserved				      */
/*									      */
/******************************************************************************/


#ifndef PapyEnumImageGroups3H 
#define PapyEnumImageGroups3H




/*	Group2		*/

enum {
papGroupLength,
papFileMetaInformationVersionGr,
papMediaStorageSOPClassUIDGr,
papMediaStorageSOPInstanceUIDGr,
papTransferSyntaxUIDGr,
papImplementationClassUIDGr,
papImplementationVersionNameGr,
papSourceApplicationEntityTitleGr,
papPrivateInformationCreatorUIDGr,
papPrivateInformationGr,
papEndGroup2
};

/*	Group8		*/

enum {
papLengthtoEndGr = 1,
papSpecificCharacterSetGr,
papImageTypeGr,
papRecognitionCodeGr,
papInstanceCreationDateGr,
papInstanceCreationTimeGr,
papInstanceCreatorUIDGr,
papSOPClassUIDGr,
papSOPInstanceUIDGr,
papStudyDateGr,
papSeriesDateGr,
papAcquisitionDateGr,
papImageDateGr,
papOverlayDateGr,
papCurveDateGr,
papStudyTimeGr,
papSeriesTimeGr,
papAcquisitionTimeGr,
papImageTimeGr,
papOverlayTimeGr,
papCurveTimeGr,
papDataSetTypeGr,
papDataSetSubtypeGr,
papNuclearMedicineSeriesTypeGr,
papAccessionNumberGr,
papQueryRetrieveLevelGr,
papRetrieveAETitleGr,
papFailedSOPInstanceUIDListGr,
papModalityGr,
papConversionTypeGr,
papManufacturerGr,
papInstitutionNameGr,
papInstitutionAddressGr,
papInstitutionCodeSequenceGr,
papReferringPhysiciansNameGr,
papReferringPhysiciansAddressGr,
papReferringPhysiciansTelephoneNumbersGr,
papCodeValueGr,
papCodingSchemeDesignatorGr,
papCodeMeaningGr,
papStationNameGr,
papStudyDescriptionGr,
papProcedureCodeSequenceGr,
papSeriesDescriptionGr,
papInstitutionalDepartmentNameGr,
papPerformingPhysiciansNameGr,
papNameofPhysiciansReadingStudyGr,
papOperatorsNameGr,
papAdmittingDiagnosesDescriptionGr,
papAdmittingDiagnosisCodeSequenceGr,
papManufacturersModelNameGr,
papReferencedResultsSequenceGr,
papReferencedStudySequenceGr,
papReferencedStudyComponentSequenceGr,
papReferencedSeriesSequenceGr,
papReferencedPatientSequenceGr,
papReferencedVisitSequenceGr,
papReferencedOverlaySequenceGr,
papReferencedImageSequenceGr,
papReferencedCurveSequenceGr,
papReferencedSOPClassUID8Gr,
papReferencedSOPInstanceUID8Gr,
papReferencedFrameNumberGr,
papDerivationDescriptionGr,
papSourceImageSequenceGr,
papStageNameGr,
papStageNumberGr,
papNumberofStagesGr,
papViewNumberGr,
papNumberofEventTimersGr,
papNumberofViewsinStageGr,
papEventElapsedTimesGr,
papEventTimerNamesGr,
papStartTrimGr,
papStopTrimGr,
papRecommendedDisplayFrameRateGr,
papTransducerPositionGr,
papTransducerOrientationGr,
papAnatomicStructureGr,
papComments8Gr,
papEndGroup8
};

/*	Group10		*/

enum {
papPatientsNameGr = 1,
papPatientIDGr,
papIssuerofPatientIDGr,
papPatientsBirthDateGr,
papPatientsBirthTimeGr,
papPatientsSexGr,
papPatientsInsurancePlanCodeSequenceGr,
papOtherPatientIDsGr,
papOtherPatientNamesGr,
papPatientsBirthNameGr,
papPatientsAgeGr,
papPatientsSizeGr,
papPatientsWeightGr,
papPatientsAddressGr,
papInsurancePlanIdentificationGr,
papPatientsMothersBirthNameGr,
papMilitaryRankGr,
papBranchofServiceGr,
papMedicalRecordLocatorGr,
papMedicalAlertsGr,
papContrastAllergiesGr,
papCountryofResidenceGr,
papRegionofResidenceGr,
papPatientsTelephoneNumbersGr,
papEthnicGroupGr,
papOccupationGr,
papSmokingStatusGr,
papAdditionalPatientHistoryGr,
papPregnancyStatusGr,
papLastMenstrualDateGr,
papPatientsReligiousPreferenceGr,
papPatientCommentsGr,
papEndGroup10
};

/*	Group18		*/

enum {
papContrastBolusAgentGr = 1,
papBodyPartExaminedGr,
papScanningSequenceGr,
papSequenceVariantGr,
papScanOptionsGr,
papMRAcquisitionTypeGr,
papSequenceNameGr,
papAngioFlagGr,
papRadionuclideGr,
papRadiopharmaceuticalGr,
papEnergyWindowCenterlineGr,
papEnergyWindowTotalWidthGr,
papInterventionDrugNameGr,
papInterventionDrugStartTimeGr,
papCineRateGr,
papSliceThicknessGr,
papKVPGr,
papCountsAccumulatedGr,
papAcquisitionTerminationConditionGr,
papEffectiveSeriesDurationGr,
papRepetitionTimeGr,
papEchoTimeGr,
papInversionTimeGr,
papNumberofAveragesGr,
papImagingFrequencyGr,
papImagedNucleusGr,
papEchoNumbersGr,
papMagneticFieldStrengthGr,
papSpacingBetweenSlicesGr,
papNumberofPhaseEncodingStepsGr,
papDataCollectionDiameterGr,
papEchoTrainLengthGr,
papPercentSamplingGr,
papPercentPhaseFieldofViewGr,
papPixelBandwidthGr,
papDeviceSerialNumberGr,
papPlateIDGr,
papSecondaryCaptureDeviceIDGr,
papDateofSecondaryCaptureGr,
papTimeofSecondaryCaptureGr,
papSecondaryCaptureDeviceManufacturerGr,
papSecondaryCaptureDeviceManufacturersModelNameGr,
papSecondaryCaptureDeviceSoftwareVersionsGr,
papSoftwareVersionsGr,
papVideoImageFormatAcquiredGr,
papDigitalImageFormatAcquiredGr,
papProtocolNameGr,
papContrastBolusRouteGr,
papContrastBolusVolumeGr,
papContrastBolusStartTimeGr,
papContrastBolusStopTimeGr,
papContrastBolusTotalDoseGr,
papSyringecountsGr,
papSpatialResolutionGr,
papTriggerTimeGr,
papTriggerSourceorTypeGr,
papNominalIntervalGr,
papFrameTimeGr,
papFramingTypeGr,
papFrameTimeVectorGr,
papFrameDelayGr,
papRadionuclideRouteGr,
papRadionuclideVolumeGr,
papRadionuclideStartTimeGr,
papRadionuclideStopTimeGr,
papRadionuclideTotalDoseGr,
papBeatRejectionFlagGr,
papLowRRValueGr,
papHighRRValueGr,
papIntervalsAcquiredGr,
papIntervalsRejectedGr,
papPVCRejectionGr,
papSkipBeatsGr,
papHeartRateGr,
papCardiacNumberofImagesGr,
papTriggerWindowGr,
papReconstructionDiameterGr,
papDistanceSourcetoDetectorGr,
papDistanceSourcetoPatientGr,
papGantryDetectorTiltGr,
papTableHeightGr,
papTableTraverseGr,
papRotationDirectionGr,
papAngularPositionGr,
papRadialPositionGr,
papScanArcGr,
papAngularStepGr,
papCenterofRotationOffsetGr,
papRotationOffsetGr,
papFieldofViewShapeGr,
papFieldofViewDimensionsGr,
papExposureTimeGr,
papXrayTubeCurrentGr,
papExposureGr,
papFilterTypeGr,
papGeneratorPowerGr,
papCollimatorgridNameGr,
papCollimatorTypeGr,
papFocalDistanceGr,
papXFocusCenterGr,
papYFocusCenterGr,
papFocalSpotsGr,
papDateofLastCalibrationGr,
papTimeofLastCalibrationGr,
papConvolutionKernelGr,
papUpperLowerPixelValuesGr,
papActualFrameDurationGr,
papCountRateGr,
papReceivingCoilGr,
papTransmittingCoilGr,
papPlateTypeGr,
papPhosphorTypeGr,
papScanVelocityGr,
papWholeBodyTechniqueGr,
papScanLengthGr,
papAcquisitionMatrixGr,
papPhaseEncodingDirectionGr,
papFlipAngleGr,
papVariableFlipAngleFlagGr,
papSARGr,
papdBdtGr,
papAcquisitionDeviceProcessingDescriptionGr,
papAcquisitionDeviceProcessingCodeGr,
papCassetteOrientationGr,
papCassetteSizeGr,
papExposuresonPlateGr,
papRelativeXrayExposureGr,
papComments18Gr,
papOutputPowerGr,
papTransducerDataGr,
papFocusDepthGr,
papPreprocessingFunctionGr,
papPostprocessingFunctionGr,
papMechanicalIndexGr,
papThermalIndexGr,
papCranialThermalIndexGr,
papSoftTissueThermalIndexGr,
papSoftTissuefocusThermalIndexGr,
papSoftTissuesurfaceThermalIndexGr,
papDepthofScanFieldGr,
papPatientPositionGr,
papViewPositionGr,
papImageTransformationMatrixGr,
papImageTranslationVectorGr,
papSensitivityGr,
papSequenceofUltrasoundRegionsGr,
papRegionSpatialFormatGr,
papRegionDataTypeGr,
papRegionFlagsGr,
papRegionLocationMinX0Gr,
papRegionLocationMinY0Gr,
papRegionLocationMaxX1Gr,
papRegionLocationMaxY1Gr,
papReferencePixelX0Gr,
papReferencePixelY0Gr,
papPhysicalUnitsXDirectionGr,
papPhysicalUnitsYDirectionGr,
papReferencePixelPhysicalValueXGr,
papReferencePixelPhysicalValueYGr,
papPhysicalDeltaXGr,
papPhysicalDeltaYGr,
papTransducerFrequencyGr,
papTransducerTypeGr,
papPulseRepetitionFrequencyGr,
papDopplerCorrectionAngleGr,
papSterringAngleGr,
papDopplerSampleVolumeXPositionGr,
papDopplerSampleVolumeYPositionGr,
papTMLinePositionX0Gr,
papTMLinePositionY0Gr,
papTMLinePositionX1Gr,
papTMLinePositionY1Gr,
papPixelComponentOrganizationGr,
papPixelComponentMaskGr,
papPixelComponentRangeStartGr,
papPixelComponentRangeStopGr,
papPixelComponentPhysicalUnitsGr,
papPixelComponentDataTypeGr,
papNumberofTableBreakPointsGr,
papTableofXBreakPointsGr,
papTableofYBreakPointsGr,
papEndGroup18
};

/*	Group20		*/

enum {
papStudyInstanceUIDGr = 1,
papSeriesInstanceUIDGr,
papStudyIDGr,
papSeriesNumberGr,
papAcquisitionNumberGr,
papImageNumberGr,
papIsotopeNumberGr,
papPhaseNumberGr,
papIntervalNumberGr,
papTimeSlotNumberGr,
papAngleNumberGr,
papPatientOrientationGr,
papOverlayNumberGr,
papCurveNumberGr,
papLUTNumberGr,
papImagePosition20Gr,
papImagePositionPatientGr,
papImageOrientationGr,
papImageOrientationPatientGr,
papLocationGr,
papFrameofReferenceUIDGr,
papLateralityGr,
papImageGeometryTypeGr,
papMaskingImageGr,
papTemporalPositionIdentifierGr,
papNumberofTemporalPositionsGr,
papTemporalResolutionGr,
papSeriesinStudyGr,
papAcquisitionsinSeriesGr,
papImagesinAcquisitionGr,
papImagesinSeriesGr,
papAcquisitioninStudyGr,
papImagesinStudyGr,
papReferenceGr,
papPositionReferenceIndicatorGr,
papSliceLocationGr,
papOtherStudyNumbersGr,
papNumberofPatientRelatedStudiesGr,
papNumberofPatientRelatedSeriesGr,
papNumberofPatientRelatedImagesGr,
papNumberofStudyRelatedSeriesGr,
papNumberofStudyRelatedImagesGr,
papSourceImageIDsGr,
papModifyingDeviceIDGr,
papModifiedImageIDGr,
papModifiedImageDateGr,
papModifyingDeviceManufacturerGr,
papModifiedImageTimeGr,
papModifiedImageDescriptionGr,
papImageCommentsGr,
papOriginalImageIdentificationGr,
papOriginalImageIdentificationNomenclatureGr,
papEndGroup20
};

/*	Group28		*/

enum {
papSamplesperPixelGr = 1,
papPhotometricInterpretationGr,
papImageDimensionsGr,
papPlanarConfigurationGr,
papNumberofFramesGr,
papFrameIncrementPointerGr,
papRowsGr,
papColumnsGr,
papPixelSpacingGr,
papZoomFactorGr,
papZoomCenterGr,
papPixelAspectRatioGr,
papImageFormatGr,
papManipulatedImageGr,
papCorrectedImageGr,
papCompressionCodeGr,
papBitsAllocatedGr,
papBitsStoredGr,
papHighBitGr,
papPixelRepresentationGr,
papSmallestValidPixelValueGr,
papLargestValidPixelValueGr,
papSmallestImagePixelValueGr,
papLargestImagePixelValueGr,
papSmallestPixelValueinSeriesGr,
papLargestPixelValueinSeriesGr,
papPixelPaddingValueGr,
papImageLocationGr,
papWindowCenterGr,
papWindowWidthGr,
papRescaleInterceptGr,
papRescaleSlopeGr,
papRescaletypeGr,
papWindowCenterWidthExplanationGr,
papGrayScaleGr,
papGrayLookupTableDescriptorGr,
papRedPaletteColorLookupTableDescriptorGr,
papGreenPaletteColorLookupTableDescriptorGr,
papBluePaletteColorLookupTableDescriptorGr,
papGrayLookupTableDataGr,
papRedPaletteColorLookupTableDataGr,
papGreenPaletteColorLookupTableDataGr,
papBluePaletteColorLookupTableDataGr,
papRedPaletteCLUTDataGr,
papGreenPaletteCLUTDataGr,
papBluePaletteCLUTDataGr,
papModalityLUTSequenceGr,
papLUTDescriptorGr,
papLUTExplanationGr,
papModalityLUTTypeGr,
papLUTDataGr,
papVOILUTSequenceGr,
papComments28Gr,
papEndGroup28
};

/*	Group41		*/

enum {
papOwnerIDGr = 1,
papComments41Gr,
papPointerSequenceGr,
papImagePointerGr,
papPixelOffsetGr,
papImageIdentifierSequenceGr,
papExternalPAPYRUSFileReferenceSequenceGr,
papNumberofimagesGr,
papReferencedSOPClassUID41Gr,
papReferencedSOPInstanceUID41Gr,
papReferencedFileNameGr,
papReferencedFilePathGr,
papReferencedImageSOPClassUIDGr,
papReferencedImageSOPInstanceUIDGr,
papImageSequenceGr,
papEndGroup41
};

/*	Group6000		*/

enum {
papRows6000Gr = 1,
papColumns6000Gr,
papNumberofFramesinOverlayGr,
papOverlayTypeGr,
papOriginGr,
papBitsAllocated6000Gr,
papBitPositionGr,
papOverlayFormatGr,
papOverlayLocationGr,
papOverlayDescriptorGrayGr,
papOverlayDescriptorRedGr,
papOverlayDescriptorGreenGr,
papOverlayDescriptorBlueGr,
papOverlaysGrayGr,
papOverlaysRedGr,
papOverlaysGreenGr,
papOverlaysBlueGr,
papROIAreaGr,
papROIMeanGr,
papROIStandardDeviationGr,
papOverlayDataGr,
papComments6000Gr,
papEndGroup6000
};

/*	UINOVERLAY	 Group 6XXX (odd)	*/

enum {
/* papGroupLength */
/* papOwnerId */
papOverlayIdGr = 2,
papLinkedOverlaysGr,
papOverlayRowsGr,
papOverlayColumnsGr,
papUINOverlayTypeGr,
papOverlayOriginGr,
papEditableGr,
papOverlayFontGr,
papOverlayStyleGr,
papOverlayFontSizeGr,
papOverlayColorGr,
papShadowSizeGr,
papFillPatternGr,
papOverlayPenSizeGr,
papLabelGr,
papPostItTextGr,
papAnchorPointGr,
papRoiTypeGr,
papAttachedAnnotationGr,
papContourPointsGr,
papMaskDataGr,
papUINOverlaySequenceGr,
papEndUINOverlay
};

/*	Group7FE0		*/

enum {
papPixelDataGr = 1,
papEndGroup7FE0
};


#endif

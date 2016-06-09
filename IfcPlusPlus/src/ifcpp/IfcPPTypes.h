/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/
#pragma once
#include "IFC4/include/IfcAbsorbedDoseMeasure.h"
#include "IFC4/include/IfcAccelerationMeasure.h"
#include "IFC4/include/IfcActionRequestTypeEnum.h"
#include "IFC4/include/IfcActionSourceTypeEnum.h"
#include "IFC4/include/IfcActionTypeEnum.h"
#include "IFC4/include/IfcActorSelect.h"
#include "IFC4/include/IfcActuatorTypeEnum.h"
#include "IFC4/include/IfcAddressTypeEnum.h"
#include "IFC4/include/IfcAirTerminalBoxTypeEnum.h"
#include "IFC4/include/IfcAirTerminalTypeEnum.h"
#include "IFC4/include/IfcAirToAirHeatRecoveryTypeEnum.h"
#include "IFC4/include/IfcAlarmTypeEnum.h"
#include "IFC4/include/IfcAmountOfSubstanceMeasure.h"
#include "IFC4/include/IfcAnalysisModelTypeEnum.h"
#include "IFC4/include/IfcAnalysisTheoryTypeEnum.h"
#include "IFC4/include/IfcAngularVelocityMeasure.h"
#include "IFC4/include/IfcAppliedValueSelect.h"
#include "IFC4/include/IfcArcIndex.h"
#include "IFC4/include/IfcAreaDensityMeasure.h"
#include "IFC4/include/IfcAreaMeasure.h"
#include "IFC4/include/IfcArithmeticOperatorEnum.h"
#include "IFC4/include/IfcAssemblyPlaceEnum.h"
#include "IFC4/include/IfcAudioVisualApplianceTypeEnum.h"
#include "IFC4/include/IfcAxis2Placement.h"
#include "IFC4/include/IfcBSplineCurveForm.h"
#include "IFC4/include/IfcBSplineSurfaceForm.h"
#include "IFC4/include/IfcBeamTypeEnum.h"
#include "IFC4/include/IfcBenchmarkEnum.h"
#include "IFC4/include/IfcBendingParameterSelect.h"
#include "IFC4/include/IfcBinary.h"
#include "IFC4/include/IfcBoilerTypeEnum.h"
#include "IFC4/include/IfcBoolean.h"
#include "IFC4/include/IfcBooleanOperand.h"
#include "IFC4/include/IfcBooleanOperator.h"
#include "IFC4/include/IfcBoxAlignment.h"
#include "IFC4/include/IfcBuildingElementPartTypeEnum.h"
#include "IFC4/include/IfcBuildingElementProxyTypeEnum.h"
#include "IFC4/include/IfcBuildingSystemTypeEnum.h"
#include "IFC4/include/IfcBurnerTypeEnum.h"
#include "IFC4/include/IfcCableCarrierFittingTypeEnum.h"
#include "IFC4/include/IfcCableCarrierSegmentTypeEnum.h"
#include "IFC4/include/IfcCableFittingTypeEnum.h"
#include "IFC4/include/IfcCableSegmentTypeEnum.h"
#include "IFC4/include/IfcCardinalPointReference.h"
#include "IFC4/include/IfcChangeActionEnum.h"
#include "IFC4/include/IfcChillerTypeEnum.h"
#include "IFC4/include/IfcChimneyTypeEnum.h"
#include "IFC4/include/IfcClassificationReferenceSelect.h"
#include "IFC4/include/IfcClassificationSelect.h"
#include "IFC4/include/IfcCoilTypeEnum.h"
#include "IFC4/include/IfcColour.h"
#include "IFC4/include/IfcColourOrFactor.h"
#include "IFC4/include/IfcColumnTypeEnum.h"
#include "IFC4/include/IfcCommunicationsApplianceTypeEnum.h"
#include "IFC4/include/IfcComplexNumber.h"
#include "IFC4/include/IfcComplexPropertyTemplateTypeEnum.h"
#include "IFC4/include/IfcCompoundPlaneAngleMeasure.h"
#include "IFC4/include/IfcCompressorTypeEnum.h"
#include "IFC4/include/IfcCondenserTypeEnum.h"
#include "IFC4/include/IfcConnectionTypeEnum.h"
#include "IFC4/include/IfcConstraintEnum.h"
#include "IFC4/include/IfcConstructionEquipmentResourceTypeEnum.h"
#include "IFC4/include/IfcConstructionMaterialResourceTypeEnum.h"
#include "IFC4/include/IfcConstructionProductResourceTypeEnum.h"
#include "IFC4/include/IfcContextDependentMeasure.h"
#include "IFC4/include/IfcControllerTypeEnum.h"
#include "IFC4/include/IfcCooledBeamTypeEnum.h"
#include "IFC4/include/IfcCoolingTowerTypeEnum.h"
#include "IFC4/include/IfcCoordinateReferenceSystemSelect.h"
#include "IFC4/include/IfcCostItemTypeEnum.h"
#include "IFC4/include/IfcCostScheduleTypeEnum.h"
#include "IFC4/include/IfcCountMeasure.h"
#include "IFC4/include/IfcCoveringTypeEnum.h"
#include "IFC4/include/IfcCrewResourceTypeEnum.h"
#include "IFC4/include/IfcCsgSelect.h"
#include "IFC4/include/IfcCurtainWallTypeEnum.h"
#include "IFC4/include/IfcCurvatureMeasure.h"
#include "IFC4/include/IfcCurveFontOrScaledCurveFontSelect.h"
#include "IFC4/include/IfcCurveInterpolationEnum.h"
#include "IFC4/include/IfcCurveOnSurface.h"
#include "IFC4/include/IfcCurveOrEdgeCurve.h"
#include "IFC4/include/IfcCurveStyleFontSelect.h"
#include "IFC4/include/IfcDamperTypeEnum.h"
#include "IFC4/include/IfcDataOriginEnum.h"
#include "IFC4/include/IfcDate.h"
#include "IFC4/include/IfcDateTime.h"
#include "IFC4/include/IfcDayInMonthNumber.h"
#include "IFC4/include/IfcDayInWeekNumber.h"
#include "IFC4/include/IfcDefinitionSelect.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcDerivedUnitEnum.h"
#include "IFC4/include/IfcDescriptiveMeasure.h"
#include "IFC4/include/IfcDimensionCount.h"
#include "IFC4/include/IfcDirectionSenseEnum.h"
#include "IFC4/include/IfcDiscreteAccessoryTypeEnum.h"
#include "IFC4/include/IfcDistributionChamberElementTypeEnum.h"
#include "IFC4/include/IfcDistributionPortTypeEnum.h"
#include "IFC4/include/IfcDistributionSystemEnum.h"
#include "IFC4/include/IfcDocumentConfidentialityEnum.h"
#include "IFC4/include/IfcDocumentSelect.h"
#include "IFC4/include/IfcDocumentStatusEnum.h"
#include "IFC4/include/IfcDoorPanelOperationEnum.h"
#include "IFC4/include/IfcDoorPanelPositionEnum.h"
#include "IFC4/include/IfcDoorStyleConstructionEnum.h"
#include "IFC4/include/IfcDoorStyleOperationEnum.h"
#include "IFC4/include/IfcDoorTypeEnum.h"
#include "IFC4/include/IfcDoorTypeOperationEnum.h"
#include "IFC4/include/IfcDoseEquivalentMeasure.h"
#include "IFC4/include/IfcDuctFittingTypeEnum.h"
#include "IFC4/include/IfcDuctSegmentTypeEnum.h"
#include "IFC4/include/IfcDuctSilencerTypeEnum.h"
#include "IFC4/include/IfcDuration.h"
#include "IFC4/include/IfcDynamicViscosityMeasure.h"
#include "IFC4/include/IfcElectricApplianceTypeEnum.h"
#include "IFC4/include/IfcElectricCapacitanceMeasure.h"
#include "IFC4/include/IfcElectricChargeMeasure.h"
#include "IFC4/include/IfcElectricConductanceMeasure.h"
#include "IFC4/include/IfcElectricCurrentMeasure.h"
#include "IFC4/include/IfcElectricDistributionBoardTypeEnum.h"
#include "IFC4/include/IfcElectricFlowStorageDeviceTypeEnum.h"
#include "IFC4/include/IfcElectricGeneratorTypeEnum.h"
#include "IFC4/include/IfcElectricMotorTypeEnum.h"
#include "IFC4/include/IfcElectricResistanceMeasure.h"
#include "IFC4/include/IfcElectricTimeControlTypeEnum.h"
#include "IFC4/include/IfcElectricVoltageMeasure.h"
#include "IFC4/include/IfcElementAssemblyTypeEnum.h"
#include "IFC4/include/IfcElementCompositionEnum.h"
#include "IFC4/include/IfcEnergyMeasure.h"
#include "IFC4/include/IfcEngineTypeEnum.h"
#include "IFC4/include/IfcEvaporativeCoolerTypeEnum.h"
#include "IFC4/include/IfcEvaporatorTypeEnum.h"
#include "IFC4/include/IfcEventTriggerTypeEnum.h"
#include "IFC4/include/IfcEventTypeEnum.h"
#include "IFC4/include/IfcExternalSpatialElementTypeEnum.h"
#include "IFC4/include/IfcFanTypeEnum.h"
#include "IFC4/include/IfcFastenerTypeEnum.h"
#include "IFC4/include/IfcFillStyleSelect.h"
#include "IFC4/include/IfcFilterTypeEnum.h"
#include "IFC4/include/IfcFireSuppressionTerminalTypeEnum.h"
#include "IFC4/include/IfcFlowDirectionEnum.h"
#include "IFC4/include/IfcFlowInstrumentTypeEnum.h"
#include "IFC4/include/IfcFlowMeterTypeEnum.h"
#include "IFC4/include/IfcFontStyle.h"
#include "IFC4/include/IfcFontVariant.h"
#include "IFC4/include/IfcFontWeight.h"
#include "IFC4/include/IfcFootingTypeEnum.h"
#include "IFC4/include/IfcForceMeasure.h"
#include "IFC4/include/IfcFrequencyMeasure.h"
#include "IFC4/include/IfcFurnitureTypeEnum.h"
#include "IFC4/include/IfcGeographicElementTypeEnum.h"
#include "IFC4/include/IfcGeometricProjectionEnum.h"
#include "IFC4/include/IfcGeometricSetSelect.h"
#include "IFC4/include/IfcGlobalOrLocalEnum.h"
#include "IFC4/include/IfcGloballyUniqueId.h"
#include "IFC4/include/IfcGridPlacementDirectionSelect.h"
#include "IFC4/include/IfcGridTypeEnum.h"
#include "IFC4/include/IfcHatchLineDistanceSelect.h"
#include "IFC4/include/IfcHeatExchangerTypeEnum.h"
#include "IFC4/include/IfcHeatFluxDensityMeasure.h"
#include "IFC4/include/IfcHeatingValueMeasure.h"
#include "IFC4/include/IfcHumidifierTypeEnum.h"
#include "IFC4/include/IfcIdentifier.h"
#include "IFC4/include/IfcIlluminanceMeasure.h"
#include "IFC4/include/IfcInductanceMeasure.h"
#include "IFC4/include/IfcInteger.h"
#include "IFC4/include/IfcIntegerCountRateMeasure.h"
#include "IFC4/include/IfcInterceptorTypeEnum.h"
#include "IFC4/include/IfcInternalOrExternalEnum.h"
#include "IFC4/include/IfcInventoryTypeEnum.h"
#include "IFC4/include/IfcIonConcentrationMeasure.h"
#include "IFC4/include/IfcIsothermalMoistureCapacityMeasure.h"
#include "IFC4/include/IfcJunctionBoxTypeEnum.h"
#include "IFC4/include/IfcKinematicViscosityMeasure.h"
#include "IFC4/include/IfcKnotType.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcLaborResourceTypeEnum.h"
#include "IFC4/include/IfcLampTypeEnum.h"
#include "IFC4/include/IfcLanguageId.h"
#include "IFC4/include/IfcLayerSetDirectionEnum.h"
#include "IFC4/include/IfcLayeredItem.h"
#include "IFC4/include/IfcLengthMeasure.h"
#include "IFC4/include/IfcLibrarySelect.h"
#include "IFC4/include/IfcLightDistributionCurveEnum.h"
#include "IFC4/include/IfcLightDistributionDataSourceSelect.h"
#include "IFC4/include/IfcLightEmissionSourceEnum.h"
#include "IFC4/include/IfcLightFixtureTypeEnum.h"
#include "IFC4/include/IfcLineIndex.h"
#include "IFC4/include/IfcLinearForceMeasure.h"
#include "IFC4/include/IfcLinearMomentMeasure.h"
#include "IFC4/include/IfcLinearStiffnessMeasure.h"
#include "IFC4/include/IfcLinearVelocityMeasure.h"
#include "IFC4/include/IfcLoadGroupTypeEnum.h"
#include "IFC4/include/IfcLogical.h"
#include "IFC4/include/IfcLogicalOperatorEnum.h"
#include "IFC4/include/IfcLuminousFluxMeasure.h"
#include "IFC4/include/IfcLuminousIntensityDistributionMeasure.h"
#include "IFC4/include/IfcLuminousIntensityMeasure.h"
#include "IFC4/include/IfcMagneticFluxDensityMeasure.h"
#include "IFC4/include/IfcMagneticFluxMeasure.h"
#include "IFC4/include/IfcMassDensityMeasure.h"
#include "IFC4/include/IfcMassFlowRateMeasure.h"
#include "IFC4/include/IfcMassMeasure.h"
#include "IFC4/include/IfcMassPerLengthMeasure.h"
#include "IFC4/include/IfcMaterialSelect.h"
#include "IFC4/include/IfcMeasureValue.h"
#include "IFC4/include/IfcMechanicalFastenerTypeEnum.h"
#include "IFC4/include/IfcMedicalDeviceTypeEnum.h"
#include "IFC4/include/IfcMemberTypeEnum.h"
#include "IFC4/include/IfcMetricValueSelect.h"
#include "IFC4/include/IfcModulusOfElasticityMeasure.h"
#include "IFC4/include/IfcModulusOfLinearSubgradeReactionMeasure.h"
#include "IFC4/include/IfcModulusOfRotationalSubgradeReactionMeasure.h"
#include "IFC4/include/IfcModulusOfRotationalSubgradeReactionSelect.h"
#include "IFC4/include/IfcModulusOfSubgradeReactionMeasure.h"
#include "IFC4/include/IfcModulusOfSubgradeReactionSelect.h"
#include "IFC4/include/IfcModulusOfTranslationalSubgradeReactionSelect.h"
#include "IFC4/include/IfcMoistureDiffusivityMeasure.h"
#include "IFC4/include/IfcMolecularWeightMeasure.h"
#include "IFC4/include/IfcMomentOfInertiaMeasure.h"
#include "IFC4/include/IfcMonetaryMeasure.h"
#include "IFC4/include/IfcMonthInYearNumber.h"
#include "IFC4/include/IfcMotorConnectionTypeEnum.h"
#include "IFC4/include/IfcNonNegativeLengthMeasure.h"
#include "IFC4/include/IfcNormalisedRatioMeasure.h"
#include "IFC4/include/IfcNullStyle.h"
#include "IFC4/include/IfcNumericMeasure.h"
#include "IFC4/include/IfcObjectReferenceSelect.h"
#include "IFC4/include/IfcObjectTypeEnum.h"
#include "IFC4/include/IfcObjectiveEnum.h"
#include "IFC4/include/IfcOccupantTypeEnum.h"
#include "IFC4/include/IfcOpeningElementTypeEnum.h"
#include "IFC4/include/IfcOutletTypeEnum.h"
#include "IFC4/include/IfcPHMeasure.h"
#include "IFC4/include/IfcParameterValue.h"
#include "IFC4/include/IfcPerformanceHistoryTypeEnum.h"
#include "IFC4/include/IfcPermeableCoveringOperationEnum.h"
#include "IFC4/include/IfcPermitTypeEnum.h"
#include "IFC4/include/IfcPhysicalOrVirtualEnum.h"
#include "IFC4/include/IfcPileConstructionEnum.h"
#include "IFC4/include/IfcPileTypeEnum.h"
#include "IFC4/include/IfcPipeFittingTypeEnum.h"
#include "IFC4/include/IfcPipeSegmentTypeEnum.h"
#include "IFC4/include/IfcPlanarForceMeasure.h"
#include "IFC4/include/IfcPlaneAngleMeasure.h"
#include "IFC4/include/IfcPlateTypeEnum.h"
#include "IFC4/include/IfcPointOrVertexPoint.h"
#include "IFC4/include/IfcPositiveInteger.h"
#include "IFC4/include/IfcPositiveLengthMeasure.h"
#include "IFC4/include/IfcPositivePlaneAngleMeasure.h"
#include "IFC4/include/IfcPositiveRatioMeasure.h"
#include "IFC4/include/IfcPowerMeasure.h"
#include "IFC4/include/IfcPresentableText.h"
#include "IFC4/include/IfcPresentationStyleSelect.h"
#include "IFC4/include/IfcPressureMeasure.h"
#include "IFC4/include/IfcProcedureTypeEnum.h"
#include "IFC4/include/IfcProcessSelect.h"
#include "IFC4/include/IfcProductRepresentationSelect.h"
#include "IFC4/include/IfcProductSelect.h"
#include "IFC4/include/IfcProfileTypeEnum.h"
#include "IFC4/include/IfcProjectOrderTypeEnum.h"
#include "IFC4/include/IfcProjectedOrTrueLengthEnum.h"
#include "IFC4/include/IfcProjectionElementTypeEnum.h"
#include "IFC4/include/IfcPropertySetDefinitionSelect.h"
#include "IFC4/include/IfcPropertySetDefinitionSet.h"
#include "IFC4/include/IfcPropertySetTemplateTypeEnum.h"
#include "IFC4/include/IfcProtectiveDeviceTrippingUnitTypeEnum.h"
#include "IFC4/include/IfcProtectiveDeviceTypeEnum.h"
#include "IFC4/include/IfcPumpTypeEnum.h"
#include "IFC4/include/IfcRadioActivityMeasure.h"
#include "IFC4/include/IfcRailingTypeEnum.h"
#include "IFC4/include/IfcRampFlightTypeEnum.h"
#include "IFC4/include/IfcRampTypeEnum.h"
#include "IFC4/include/IfcRatioMeasure.h"
#include "IFC4/include/IfcReal.h"
#include "IFC4/include/IfcRecurrenceTypeEnum.h"
#include "IFC4/include/IfcReflectanceMethodEnum.h"
#include "IFC4/include/IfcReinforcingBarRoleEnum.h"
#include "IFC4/include/IfcReinforcingBarSurfaceEnum.h"
#include "IFC4/include/IfcReinforcingBarTypeEnum.h"
#include "IFC4/include/IfcReinforcingMeshTypeEnum.h"
#include "IFC4/include/IfcResourceObjectSelect.h"
#include "IFC4/include/IfcResourceSelect.h"
#include "IFC4/include/IfcRoleEnum.h"
#include "IFC4/include/IfcRoofTypeEnum.h"
#include "IFC4/include/IfcRotationalFrequencyMeasure.h"
#include "IFC4/include/IfcRotationalMassMeasure.h"
#include "IFC4/include/IfcRotationalStiffnessMeasure.h"
#include "IFC4/include/IfcRotationalStiffnessSelect.h"
#include "IFC4/include/IfcSIPrefix.h"
#include "IFC4/include/IfcSIUnitName.h"
#include "IFC4/include/IfcSanitaryTerminalTypeEnum.h"
#include "IFC4/include/IfcSectionModulusMeasure.h"
#include "IFC4/include/IfcSectionTypeEnum.h"
#include "IFC4/include/IfcSectionalAreaIntegralMeasure.h"
#include "IFC4/include/IfcSegmentIndexSelect.h"
#include "IFC4/include/IfcSensorTypeEnum.h"
#include "IFC4/include/IfcSequenceEnum.h"
#include "IFC4/include/IfcShadingDeviceTypeEnum.h"
#include "IFC4/include/IfcShearModulusMeasure.h"
#include "IFC4/include/IfcShell.h"
#include "IFC4/include/IfcSimplePropertyTemplateTypeEnum.h"
#include "IFC4/include/IfcSimpleValue.h"
#include "IFC4/include/IfcSizeSelect.h"
#include "IFC4/include/IfcSlabTypeEnum.h"
#include "IFC4/include/IfcSolarDeviceTypeEnum.h"
#include "IFC4/include/IfcSolidAngleMeasure.h"
#include "IFC4/include/IfcSolidOrShell.h"
#include "IFC4/include/IfcSoundPowerLevelMeasure.h"
#include "IFC4/include/IfcSoundPowerMeasure.h"
#include "IFC4/include/IfcSoundPressureLevelMeasure.h"
#include "IFC4/include/IfcSoundPressureMeasure.h"
#include "IFC4/include/IfcSpaceBoundarySelect.h"
#include "IFC4/include/IfcSpaceHeaterTypeEnum.h"
#include "IFC4/include/IfcSpaceTypeEnum.h"
#include "IFC4/include/IfcSpatialZoneTypeEnum.h"
#include "IFC4/include/IfcSpecificHeatCapacityMeasure.h"
#include "IFC4/include/IfcSpecularExponent.h"
#include "IFC4/include/IfcSpecularHighlightSelect.h"
#include "IFC4/include/IfcSpecularRoughness.h"
#include "IFC4/include/IfcStackTerminalTypeEnum.h"
#include "IFC4/include/IfcStairFlightTypeEnum.h"
#include "IFC4/include/IfcStairTypeEnum.h"
#include "IFC4/include/IfcStateEnum.h"
#include "IFC4/include/IfcStrippedOptional.h"
#include "IFC4/include/IfcStructuralActivityAssignmentSelect.h"
#include "IFC4/include/IfcStructuralCurveActivityTypeEnum.h"
#include "IFC4/include/IfcStructuralCurveMemberTypeEnum.h"
#include "IFC4/include/IfcStructuralSurfaceActivityTypeEnum.h"
#include "IFC4/include/IfcStructuralSurfaceMemberTypeEnum.h"
#include "IFC4/include/IfcStyleAssignmentSelect.h"
#include "IFC4/include/IfcSubContractResourceTypeEnum.h"
#include "IFC4/include/IfcSurfaceFeatureTypeEnum.h"
#include "IFC4/include/IfcSurfaceOrFaceSurface.h"
#include "IFC4/include/IfcSurfaceSide.h"
#include "IFC4/include/IfcSurfaceStyleElementSelect.h"
#include "IFC4/include/IfcSwitchingDeviceTypeEnum.h"
#include "IFC4/include/IfcSystemFurnitureElementTypeEnum.h"
#include "IFC4/include/IfcTankTypeEnum.h"
#include "IFC4/include/IfcTaskDurationEnum.h"
#include "IFC4/include/IfcTaskTypeEnum.h"
#include "IFC4/include/IfcTemperatureGradientMeasure.h"
#include "IFC4/include/IfcTemperatureRateOfChangeMeasure.h"
#include "IFC4/include/IfcTendonAnchorTypeEnum.h"
#include "IFC4/include/IfcTendonTypeEnum.h"
#include "IFC4/include/IfcText.h"
#include "IFC4/include/IfcTextAlignment.h"
#include "IFC4/include/IfcTextDecoration.h"
#include "IFC4/include/IfcTextFontName.h"
#include "IFC4/include/IfcTextFontSelect.h"
#include "IFC4/include/IfcTextPath.h"
#include "IFC4/include/IfcTextTransformation.h"
#include "IFC4/include/IfcThermalAdmittanceMeasure.h"
#include "IFC4/include/IfcThermalConductivityMeasure.h"
#include "IFC4/include/IfcThermalExpansionCoefficientMeasure.h"
#include "IFC4/include/IfcThermalResistanceMeasure.h"
#include "IFC4/include/IfcThermalTransmittanceMeasure.h"
#include "IFC4/include/IfcThermodynamicTemperatureMeasure.h"
#include "IFC4/include/IfcTime.h"
#include "IFC4/include/IfcTimeMeasure.h"
#include "IFC4/include/IfcTimeOrRatioSelect.h"
#include "IFC4/include/IfcTimeSeriesDataTypeEnum.h"
#include "IFC4/include/IfcTimeStamp.h"
#include "IFC4/include/IfcTorqueMeasure.h"
#include "IFC4/include/IfcTransformerTypeEnum.h"
#include "IFC4/include/IfcTransitionCode.h"
#include "IFC4/include/IfcTranslationalStiffnessSelect.h"
#include "IFC4/include/IfcTransportElementTypeEnum.h"
#include "IFC4/include/IfcTrimmingPreference.h"
#include "IFC4/include/IfcTrimmingSelect.h"
#include "IFC4/include/IfcTubeBundleTypeEnum.h"
#include "IFC4/include/IfcURIReference.h"
#include "IFC4/include/IfcUnit.h"
#include "IFC4/include/IfcUnitEnum.h"
#include "IFC4/include/IfcUnitaryControlElementTypeEnum.h"
#include "IFC4/include/IfcUnitaryEquipmentTypeEnum.h"
#include "IFC4/include/IfcValue.h"
#include "IFC4/include/IfcValveTypeEnum.h"
#include "IFC4/include/IfcVaporPermeabilityMeasure.h"
#include "IFC4/include/IfcVectorOrDirection.h"
#include "IFC4/include/IfcVibrationIsolatorTypeEnum.h"
#include "IFC4/include/IfcVoidingFeatureTypeEnum.h"
#include "IFC4/include/IfcVolumeMeasure.h"
#include "IFC4/include/IfcVolumetricFlowRateMeasure.h"
#include "IFC4/include/IfcWallTypeEnum.h"
#include "IFC4/include/IfcWarpingConstantMeasure.h"
#include "IFC4/include/IfcWarpingMomentMeasure.h"
#include "IFC4/include/IfcWarpingStiffnessSelect.h"
#include "IFC4/include/IfcWasteTerminalTypeEnum.h"
#include "IFC4/include/IfcWindowPanelOperationEnum.h"
#include "IFC4/include/IfcWindowPanelPositionEnum.h"
#include "IFC4/include/IfcWindowStyleConstructionEnum.h"
#include "IFC4/include/IfcWindowStyleOperationEnum.h"
#include "IFC4/include/IfcWindowTypeEnum.h"
#include "IFC4/include/IfcWindowTypePartitioningEnum.h"
#include "IFC4/include/IfcWorkCalendarTypeEnum.h"
#include "IFC4/include/IfcWorkPlanTypeEnum.h"
#include "IFC4/include/IfcWorkScheduleTypeEnum.h"

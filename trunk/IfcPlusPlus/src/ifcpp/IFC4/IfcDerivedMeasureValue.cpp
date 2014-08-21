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
#include <map>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "include/IfcAbsorbedDoseMeasure.h"
#include "include/IfcAccelerationMeasure.h"
#include "include/IfcAngularVelocityMeasure.h"
#include "include/IfcAreaDensityMeasure.h"
#include "include/IfcCompoundPlaneAngleMeasure.h"
#include "include/IfcCurvatureMeasure.h"
#include "include/IfcDoseEquivalentMeasure.h"
#include "include/IfcDynamicViscosityMeasure.h"
#include "include/IfcElectricCapacitanceMeasure.h"
#include "include/IfcElectricChargeMeasure.h"
#include "include/IfcElectricConductanceMeasure.h"
#include "include/IfcElectricResistanceMeasure.h"
#include "include/IfcElectricVoltageMeasure.h"
#include "include/IfcEnergyMeasure.h"
#include "include/IfcForceMeasure.h"
#include "include/IfcFrequencyMeasure.h"
#include "include/IfcHeatFluxDensityMeasure.h"
#include "include/IfcHeatingValueMeasure.h"
#include "include/IfcIlluminanceMeasure.h"
#include "include/IfcInductanceMeasure.h"
#include "include/IfcIntegerCountRateMeasure.h"
#include "include/IfcIonConcentrationMeasure.h"
#include "include/IfcIsothermalMoistureCapacityMeasure.h"
#include "include/IfcKinematicViscosityMeasure.h"
#include "include/IfcLinearForceMeasure.h"
#include "include/IfcLinearMomentMeasure.h"
#include "include/IfcLinearStiffnessMeasure.h"
#include "include/IfcLinearVelocityMeasure.h"
#include "include/IfcLuminousFluxMeasure.h"
#include "include/IfcLuminousIntensityDistributionMeasure.h"
#include "include/IfcMagneticFluxDensityMeasure.h"
#include "include/IfcMagneticFluxMeasure.h"
#include "include/IfcMassDensityMeasure.h"
#include "include/IfcMassFlowRateMeasure.h"
#include "include/IfcMassPerLengthMeasure.h"
#include "include/IfcModulusOfElasticityMeasure.h"
#include "include/IfcModulusOfLinearSubgradeReactionMeasure.h"
#include "include/IfcModulusOfRotationalSubgradeReactionMeasure.h"
#include "include/IfcModulusOfSubgradeReactionMeasure.h"
#include "include/IfcMoistureDiffusivityMeasure.h"
#include "include/IfcMolecularWeightMeasure.h"
#include "include/IfcMomentOfInertiaMeasure.h"
#include "include/IfcMonetaryMeasure.h"
#include "include/IfcPHMeasure.h"
#include "include/IfcPlanarForceMeasure.h"
#include "include/IfcPowerMeasure.h"
#include "include/IfcPressureMeasure.h"
#include "include/IfcRadioActivityMeasure.h"
#include "include/IfcRotationalFrequencyMeasure.h"
#include "include/IfcRotationalMassMeasure.h"
#include "include/IfcRotationalStiffnessMeasure.h"
#include "include/IfcSectionModulusMeasure.h"
#include "include/IfcSectionalAreaIntegralMeasure.h"
#include "include/IfcShearModulusMeasure.h"
#include "include/IfcSoundPowerLevelMeasure.h"
#include "include/IfcSoundPowerMeasure.h"
#include "include/IfcSoundPressureLevelMeasure.h"
#include "include/IfcSoundPressureMeasure.h"
#include "include/IfcSpecificHeatCapacityMeasure.h"
#include "include/IfcTemperatureGradientMeasure.h"
#include "include/IfcTemperatureRateOfChangeMeasure.h"
#include "include/IfcThermalAdmittanceMeasure.h"
#include "include/IfcThermalConductivityMeasure.h"
#include "include/IfcThermalExpansionCoefficientMeasure.h"
#include "include/IfcThermalResistanceMeasure.h"
#include "include/IfcThermalTransmittanceMeasure.h"
#include "include/IfcTorqueMeasure.h"
#include "include/IfcVaporPermeabilityMeasure.h"
#include "include/IfcVolumetricFlowRateMeasure.h"
#include "include/IfcWarpingConstantMeasure.h"
#include "include/IfcWarpingMomentMeasure.h"
#include "include/IfcValue.h"
#include "include/IfcDerivedMeasureValue.h"

// TYPE IfcDerivedMeasureValue = SELECT	(IfcAbsorbedDoseMeasure	,IfcAccelerationMeasure	,IfcAngularVelocityMeasure	,IfcAreaDensityMeasure	,IfcCompoundPlaneAngleMeasure	,IfcCurvatureMeasure	,IfcDoseEquivalentMeasure	,IfcDynamicViscosityMeasure	,IfcElectricCapacitanceMeasure	,IfcElectricChargeMeasure	,IfcElectricConductanceMeasure	,IfcElectricResistanceMeasure	,IfcElectricVoltageMeasure	,IfcEnergyMeasure	,IfcForceMeasure	,IfcFrequencyMeasure	,IfcHeatFluxDensityMeasure	,IfcHeatingValueMeasure	,IfcIlluminanceMeasure	,IfcInductanceMeasure	,IfcIntegerCountRateMeasure	,IfcIonConcentrationMeasure	,IfcIsothermalMoistureCapacityMeasure	,IfcKinematicViscosityMeasure	,IfcLinearForceMeasure	,IfcLinearMomentMeasure	,IfcLinearStiffnessMeasure	,IfcLinearVelocityMeasure	,IfcLuminousFluxMeasure	,IfcLuminousIntensityDistributionMeasure	,IfcMagneticFluxDensityMeasure	,IfcMagneticFluxMeasure	,IfcMassDensityMeasure	,IfcMassFlowRateMeasure	,IfcMassPerLengthMeasure	,IfcModulusOfElasticityMeasure	,IfcModulusOfLinearSubgradeReactionMeasure	,IfcModulusOfRotationalSubgradeReactionMeasure	,IfcModulusOfSubgradeReactionMeasure	,IfcMoistureDiffusivityMeasure	,IfcMolecularWeightMeasure	,IfcMomentOfInertiaMeasure	,IfcMonetaryMeasure	,IfcPHMeasure	,IfcPlanarForceMeasure	,IfcPowerMeasure	,IfcPressureMeasure	,IfcRadioActivityMeasure	,IfcRotationalFrequencyMeasure	,IfcRotationalMassMeasure	,IfcRotationalStiffnessMeasure	,IfcSectionModulusMeasure	,IfcSectionalAreaIntegralMeasure	,IfcShearModulusMeasure	,IfcSoundPowerLevelMeasure	,IfcSoundPowerMeasure	,IfcSoundPressureLevelMeasure	,IfcSoundPressureMeasure	,IfcSpecificHeatCapacityMeasure	,IfcTemperatureGradientMeasure	,IfcTemperatureRateOfChangeMeasure	,IfcThermalAdmittanceMeasure	,IfcThermalConductivityMeasure	,IfcThermalExpansionCoefficientMeasure	,IfcThermalResistanceMeasure	,IfcThermalTransmittanceMeasure	,IfcTorqueMeasure	,IfcVaporPermeabilityMeasure	,IfcVolumetricFlowRateMeasure	,IfcWarpingConstantMeasure	,IfcWarpingMomentMeasure);
shared_ptr<IfcDerivedMeasureValue> IfcDerivedMeasureValue::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	// read TYPE
	if( arg.size() == 0 ){ return shared_ptr<IfcDerivedMeasureValue>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcDerivedMeasureValue>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcDerivedMeasureValue>();
	}
	shared_ptr<IfcDerivedMeasureValue> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}

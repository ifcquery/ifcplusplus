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
#include "include/IfcDerivedMeasureValue.h"
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
#include "include/IfcMeasureValue.h"
#include "include/IfcAmountOfSubstanceMeasure.h"
#include "include/IfcAreaMeasure.h"
#include "include/IfcComplexNumber.h"
#include "include/IfcContextDependentMeasure.h"
#include "include/IfcCountMeasure.h"
#include "include/IfcDescriptiveMeasure.h"
#include "include/IfcElectricCurrentMeasure.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcLuminousIntensityMeasure.h"
#include "include/IfcMassMeasure.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcNumericMeasure.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPositivePlaneAngleMeasure.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcRatioMeasure.h"
#include "include/IfcSolidAngleMeasure.h"
#include "include/IfcThermodynamicTemperatureMeasure.h"
#include "include/IfcTimeMeasure.h"
#include "include/IfcVolumeMeasure.h"
#include "include/IfcSimpleValue.h"
#include "include/IfcBoolean.h"
#include "include/IfcDate.h"
#include "include/IfcDateTime.h"
#include "include/IfcDuration.h"
#include "include/IfcIdentifier.h"
#include "include/IfcInteger.h"
#include "include/IfcLabel.h"
#include "include/IfcLogical.h"
#include "include/IfcReal.h"
#include "include/IfcText.h"
#include "include/IfcTime.h"
#include "include/IfcTimeStamp.h"
#include "include/IfcAppliedValueSelect.h"
#include "include/IfcMetricValueSelect.h"
#include "include/IfcValue.h"

// TYPE IfcValue 
IfcValue::IfcValue() {}
IfcValue::~IfcValue() {}
shared_ptr<IfcValue> IfcValue::createObjectFromStepData( const std::string& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	// Read SELECT TYPE
	if( arg.size() == 0 ){ return shared_ptr<IfcValue>(); }
	if( arg[0] == '#' )
	{
		int id=atoi( arg.substr(1,arg.length()-1).c_str() );
		std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it_entity = map.find( id );
		if( it_entity != map.end() )
		{
			shared_ptr<IfcValue> type_object = dynamic_pointer_cast<IfcValue>(it_entity->second);
			return type_object;
		}
		else
		{
			std::stringstream strs;
			strs << "Object width id " << id << " not found";
			throw IfcPPException( strs.str() );
		}
	}
	else if( arg.compare("$")==0 )
	{
		return shared_ptr<IfcValue>();
	}
	else if( arg.compare("*")==0 )
	{
		return shared_ptr<IfcValue>();
	}
	else
	{
		// inline arguments
		std::string keyword;
		std::string inline_arg;
		tokenizeInlineArgument( arg, keyword, inline_arg );
		shared_ptr<IfcPPObject> result_object;
		readInlineTypeOrEntity( keyword, inline_arg, result_object, map );
		if( result_object )
		{
			shared_ptr<IfcPPObject> result_ptr( result_object );
			shared_ptr<IfcValue> result_ptr_self = dynamic_pointer_cast<IfcValue>( result_ptr );
			if( result_ptr_self )
			{
				return result_ptr_self;
			}
		}
		std::stringstream strs;
		strs << "unhandled inline argument: " << arg << " in function IfcValue::readStepData" << std::endl;
		throw IfcPPException( strs.str() );
	}
	return shared_ptr<IfcValue>();
}

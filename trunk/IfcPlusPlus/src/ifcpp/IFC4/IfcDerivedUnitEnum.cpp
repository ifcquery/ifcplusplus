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

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcDerivedUnitEnum.h"

// TYPE IfcDerivedUnitEnum = ENUMERATION OF	(ANGULARVELOCITYUNIT	,AREADENSITYUNIT	,COMPOUNDPLANEANGLEUNIT	,DYNAMICVISCOSITYUNIT	,HEATFLUXDENSITYUNIT	,INTEGERCOUNTRATEUNIT	,ISOTHERMALMOISTURECAPACITYUNIT	,KINEMATICVISCOSITYUNIT	,LINEARVELOCITYUNIT	,MASSDENSITYUNIT	,MASSFLOWRATEUNIT	,MOISTUREDIFFUSIVITYUNIT	,MOLECULARWEIGHTUNIT	,SPECIFICHEATCAPACITYUNIT	,THERMALADMITTANCEUNIT	,THERMALCONDUCTANCEUNIT	,THERMALRESISTANCEUNIT	,THERMALTRANSMITTANCEUNIT	,VAPORPERMEABILITYUNIT	,VOLUMETRICFLOWRATEUNIT	,ROTATIONALFREQUENCYUNIT	,TORQUEUNIT	,MOMENTOFINERTIAUNIT	,LINEARMOMENTUNIT	,LINEARFORCEUNIT	,PLANARFORCEUNIT	,MODULUSOFELASTICITYUNIT	,SHEARMODULUSUNIT	,LINEARSTIFFNESSUNIT	,ROTATIONALSTIFFNESSUNIT	,MODULUSOFSUBGRADEREACTIONUNIT	,ACCELERATIONUNIT	,CURVATUREUNIT	,HEATINGVALUEUNIT	,IONCONCENTRATIONUNIT	,LUMINOUSINTENSITYDISTRIBUTIONUNIT	,MASSPERLENGTHUNIT	,MODULUSOFLINEARSUBGRADEREACTIONUNIT	,MODULUSOFROTATIONALSUBGRADEREACTIONUNIT	,PHUNIT	,ROTATIONALMASSUNIT	,SECTIONAREAINTEGRALUNIT	,SECTIONMODULUSUNIT	,SOUNDPOWERLEVELUNIT	,SOUNDPOWERUNIT	,SOUNDPRESSURELEVELUNIT	,SOUNDPRESSUREUNIT	,TEMPERATUREGRADIENTUNIT	,TEMPERATURERATEOFCHANGEUNIT	,THERMALEXPANSIONCOEFFICIENTUNIT	,WARPINGCONSTANTUNIT	,WARPINGMOMENTUNIT	,USERDEFINED);
IfcDerivedUnitEnum::IfcDerivedUnitEnum() {}
IfcDerivedUnitEnum::~IfcDerivedUnitEnum() {}
shared_ptr<IfcPPObject> IfcDerivedUnitEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDerivedUnitEnum> copy_self( new IfcDerivedUnitEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDerivedUnitEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDERIVEDUNITENUM("; }
	if( m_enum == ENUM_ANGULARVELOCITYUNIT )
	{
		stream << ".ANGULARVELOCITYUNIT.";
	}
	else if( m_enum == ENUM_AREADENSITYUNIT )
	{
		stream << ".AREADENSITYUNIT.";
	}
	else if( m_enum == ENUM_COMPOUNDPLANEANGLEUNIT )
	{
		stream << ".COMPOUNDPLANEANGLEUNIT.";
	}
	else if( m_enum == ENUM_DYNAMICVISCOSITYUNIT )
	{
		stream << ".DYNAMICVISCOSITYUNIT.";
	}
	else if( m_enum == ENUM_HEATFLUXDENSITYUNIT )
	{
		stream << ".HEATFLUXDENSITYUNIT.";
	}
	else if( m_enum == ENUM_INTEGERCOUNTRATEUNIT )
	{
		stream << ".INTEGERCOUNTRATEUNIT.";
	}
	else if( m_enum == ENUM_ISOTHERMALMOISTURECAPACITYUNIT )
	{
		stream << ".ISOTHERMALMOISTURECAPACITYUNIT.";
	}
	else if( m_enum == ENUM_KINEMATICVISCOSITYUNIT )
	{
		stream << ".KINEMATICVISCOSITYUNIT.";
	}
	else if( m_enum == ENUM_LINEARVELOCITYUNIT )
	{
		stream << ".LINEARVELOCITYUNIT.";
	}
	else if( m_enum == ENUM_MASSDENSITYUNIT )
	{
		stream << ".MASSDENSITYUNIT.";
	}
	else if( m_enum == ENUM_MASSFLOWRATEUNIT )
	{
		stream << ".MASSFLOWRATEUNIT.";
	}
	else if( m_enum == ENUM_MOISTUREDIFFUSIVITYUNIT )
	{
		stream << ".MOISTUREDIFFUSIVITYUNIT.";
	}
	else if( m_enum == ENUM_MOLECULARWEIGHTUNIT )
	{
		stream << ".MOLECULARWEIGHTUNIT.";
	}
	else if( m_enum == ENUM_SPECIFICHEATCAPACITYUNIT )
	{
		stream << ".SPECIFICHEATCAPACITYUNIT.";
	}
	else if( m_enum == ENUM_THERMALADMITTANCEUNIT )
	{
		stream << ".THERMALADMITTANCEUNIT.";
	}
	else if( m_enum == ENUM_THERMALCONDUCTANCEUNIT )
	{
		stream << ".THERMALCONDUCTANCEUNIT.";
	}
	else if( m_enum == ENUM_THERMALRESISTANCEUNIT )
	{
		stream << ".THERMALRESISTANCEUNIT.";
	}
	else if( m_enum == ENUM_THERMALTRANSMITTANCEUNIT )
	{
		stream << ".THERMALTRANSMITTANCEUNIT.";
	}
	else if( m_enum == ENUM_VAPORPERMEABILITYUNIT )
	{
		stream << ".VAPORPERMEABILITYUNIT.";
	}
	else if( m_enum == ENUM_VOLUMETRICFLOWRATEUNIT )
	{
		stream << ".VOLUMETRICFLOWRATEUNIT.";
	}
	else if( m_enum == ENUM_ROTATIONALFREQUENCYUNIT )
	{
		stream << ".ROTATIONALFREQUENCYUNIT.";
	}
	else if( m_enum == ENUM_TORQUEUNIT )
	{
		stream << ".TORQUEUNIT.";
	}
	else if( m_enum == ENUM_MOMENTOFINERTIAUNIT )
	{
		stream << ".MOMENTOFINERTIAUNIT.";
	}
	else if( m_enum == ENUM_LINEARMOMENTUNIT )
	{
		stream << ".LINEARMOMENTUNIT.";
	}
	else if( m_enum == ENUM_LINEARFORCEUNIT )
	{
		stream << ".LINEARFORCEUNIT.";
	}
	else if( m_enum == ENUM_PLANARFORCEUNIT )
	{
		stream << ".PLANARFORCEUNIT.";
	}
	else if( m_enum == ENUM_MODULUSOFELASTICITYUNIT )
	{
		stream << ".MODULUSOFELASTICITYUNIT.";
	}
	else if( m_enum == ENUM_SHEARMODULUSUNIT )
	{
		stream << ".SHEARMODULUSUNIT.";
	}
	else if( m_enum == ENUM_LINEARSTIFFNESSUNIT )
	{
		stream << ".LINEARSTIFFNESSUNIT.";
	}
	else if( m_enum == ENUM_ROTATIONALSTIFFNESSUNIT )
	{
		stream << ".ROTATIONALSTIFFNESSUNIT.";
	}
	else if( m_enum == ENUM_MODULUSOFSUBGRADEREACTIONUNIT )
	{
		stream << ".MODULUSOFSUBGRADEREACTIONUNIT.";
	}
	else if( m_enum == ENUM_ACCELERATIONUNIT )
	{
		stream << ".ACCELERATIONUNIT.";
	}
	else if( m_enum == ENUM_CURVATUREUNIT )
	{
		stream << ".CURVATUREUNIT.";
	}
	else if( m_enum == ENUM_HEATINGVALUEUNIT )
	{
		stream << ".HEATINGVALUEUNIT.";
	}
	else if( m_enum == ENUM_IONCONCENTRATIONUNIT )
	{
		stream << ".IONCONCENTRATIONUNIT.";
	}
	else if( m_enum == ENUM_LUMINOUSINTENSITYDISTRIBUTIONUNIT )
	{
		stream << ".LUMINOUSINTENSITYDISTRIBUTIONUNIT.";
	}
	else if( m_enum == ENUM_MASSPERLENGTHUNIT )
	{
		stream << ".MASSPERLENGTHUNIT.";
	}
	else if( m_enum == ENUM_MODULUSOFLINEARSUBGRADEREACTIONUNIT )
	{
		stream << ".MODULUSOFLINEARSUBGRADEREACTIONUNIT.";
	}
	else if( m_enum == ENUM_MODULUSOFROTATIONALSUBGRADEREACTIONUNIT )
	{
		stream << ".MODULUSOFROTATIONALSUBGRADEREACTIONUNIT.";
	}
	else if( m_enum == ENUM_PHUNIT )
	{
		stream << ".PHUNIT.";
	}
	else if( m_enum == ENUM_ROTATIONALMASSUNIT )
	{
		stream << ".ROTATIONALMASSUNIT.";
	}
	else if( m_enum == ENUM_SECTIONAREAINTEGRALUNIT )
	{
		stream << ".SECTIONAREAINTEGRALUNIT.";
	}
	else if( m_enum == ENUM_SECTIONMODULUSUNIT )
	{
		stream << ".SECTIONMODULUSUNIT.";
	}
	else if( m_enum == ENUM_SOUNDPOWERLEVELUNIT )
	{
		stream << ".SOUNDPOWERLEVELUNIT.";
	}
	else if( m_enum == ENUM_SOUNDPOWERUNIT )
	{
		stream << ".SOUNDPOWERUNIT.";
	}
	else if( m_enum == ENUM_SOUNDPRESSURELEVELUNIT )
	{
		stream << ".SOUNDPRESSURELEVELUNIT.";
	}
	else if( m_enum == ENUM_SOUNDPRESSUREUNIT )
	{
		stream << ".SOUNDPRESSUREUNIT.";
	}
	else if( m_enum == ENUM_TEMPERATUREGRADIENTUNIT )
	{
		stream << ".TEMPERATUREGRADIENTUNIT.";
	}
	else if( m_enum == ENUM_TEMPERATURERATEOFCHANGEUNIT )
	{
		stream << ".TEMPERATURERATEOFCHANGEUNIT.";
	}
	else if( m_enum == ENUM_THERMALEXPANSIONCOEFFICIENTUNIT )
	{
		stream << ".THERMALEXPANSIONCOEFFICIENTUNIT.";
	}
	else if( m_enum == ENUM_WARPINGCONSTANTUNIT )
	{
		stream << ".WARPINGCONSTANTUNIT.";
	}
	else if( m_enum == ENUM_WARPINGMOMENTUNIT )
	{
		stream << ".WARPINGMOMENTUNIT.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcDerivedUnitEnum> IfcDerivedUnitEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDerivedUnitEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDerivedUnitEnum>(); }
	shared_ptr<IfcDerivedUnitEnum> type_object( new IfcDerivedUnitEnum() );
	if( boost::iequals( arg, L".ANGULARVELOCITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_ANGULARVELOCITYUNIT;
	}
	else if( boost::iequals( arg, L".AREADENSITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_AREADENSITYUNIT;
	}
	else if( boost::iequals( arg, L".COMPOUNDPLANEANGLEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_COMPOUNDPLANEANGLEUNIT;
	}
	else if( boost::iequals( arg, L".DYNAMICVISCOSITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_DYNAMICVISCOSITYUNIT;
	}
	else if( boost::iequals( arg, L".HEATFLUXDENSITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_HEATFLUXDENSITYUNIT;
	}
	else if( boost::iequals( arg, L".INTEGERCOUNTRATEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_INTEGERCOUNTRATEUNIT;
	}
	else if( boost::iequals( arg, L".ISOTHERMALMOISTURECAPACITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_ISOTHERMALMOISTURECAPACITYUNIT;
	}
	else if( boost::iequals( arg, L".KINEMATICVISCOSITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_KINEMATICVISCOSITYUNIT;
	}
	else if( boost::iequals( arg, L".LINEARVELOCITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_LINEARVELOCITYUNIT;
	}
	else if( boost::iequals( arg, L".MASSDENSITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MASSDENSITYUNIT;
	}
	else if( boost::iequals( arg, L".MASSFLOWRATEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MASSFLOWRATEUNIT;
	}
	else if( boost::iequals( arg, L".MOISTUREDIFFUSIVITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MOISTUREDIFFUSIVITYUNIT;
	}
	else if( boost::iequals( arg, L".MOLECULARWEIGHTUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MOLECULARWEIGHTUNIT;
	}
	else if( boost::iequals( arg, L".SPECIFICHEATCAPACITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_SPECIFICHEATCAPACITYUNIT;
	}
	else if( boost::iequals( arg, L".THERMALADMITTANCEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_THERMALADMITTANCEUNIT;
	}
	else if( boost::iequals( arg, L".THERMALCONDUCTANCEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_THERMALCONDUCTANCEUNIT;
	}
	else if( boost::iequals( arg, L".THERMALRESISTANCEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_THERMALRESISTANCEUNIT;
	}
	else if( boost::iequals( arg, L".THERMALTRANSMITTANCEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_THERMALTRANSMITTANCEUNIT;
	}
	else if( boost::iequals( arg, L".VAPORPERMEABILITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_VAPORPERMEABILITYUNIT;
	}
	else if( boost::iequals( arg, L".VOLUMETRICFLOWRATEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_VOLUMETRICFLOWRATEUNIT;
	}
	else if( boost::iequals( arg, L".ROTATIONALFREQUENCYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_ROTATIONALFREQUENCYUNIT;
	}
	else if( boost::iequals( arg, L".TORQUEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_TORQUEUNIT;
	}
	else if( boost::iequals( arg, L".MOMENTOFINERTIAUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MOMENTOFINERTIAUNIT;
	}
	else if( boost::iequals( arg, L".LINEARMOMENTUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_LINEARMOMENTUNIT;
	}
	else if( boost::iequals( arg, L".LINEARFORCEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_LINEARFORCEUNIT;
	}
	else if( boost::iequals( arg, L".PLANARFORCEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_PLANARFORCEUNIT;
	}
	else if( boost::iequals( arg, L".MODULUSOFELASTICITYUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MODULUSOFELASTICITYUNIT;
	}
	else if( boost::iequals( arg, L".SHEARMODULUSUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_SHEARMODULUSUNIT;
	}
	else if( boost::iequals( arg, L".LINEARSTIFFNESSUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_LINEARSTIFFNESSUNIT;
	}
	else if( boost::iequals( arg, L".ROTATIONALSTIFFNESSUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_ROTATIONALSTIFFNESSUNIT;
	}
	else if( boost::iequals( arg, L".MODULUSOFSUBGRADEREACTIONUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MODULUSOFSUBGRADEREACTIONUNIT;
	}
	else if( boost::iequals( arg, L".ACCELERATIONUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_ACCELERATIONUNIT;
	}
	else if( boost::iequals( arg, L".CURVATUREUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_CURVATUREUNIT;
	}
	else if( boost::iequals( arg, L".HEATINGVALUEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_HEATINGVALUEUNIT;
	}
	else if( boost::iequals( arg, L".IONCONCENTRATIONUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_IONCONCENTRATIONUNIT;
	}
	else if( boost::iequals( arg, L".LUMINOUSINTENSITYDISTRIBUTIONUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_LUMINOUSINTENSITYDISTRIBUTIONUNIT;
	}
	else if( boost::iequals( arg, L".MASSPERLENGTHUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MASSPERLENGTHUNIT;
	}
	else if( boost::iequals( arg, L".MODULUSOFLINEARSUBGRADEREACTIONUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MODULUSOFLINEARSUBGRADEREACTIONUNIT;
	}
	else if( boost::iequals( arg, L".MODULUSOFROTATIONALSUBGRADEREACTIONUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_MODULUSOFROTATIONALSUBGRADEREACTIONUNIT;
	}
	else if( boost::iequals( arg, L".PHUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_PHUNIT;
	}
	else if( boost::iequals( arg, L".ROTATIONALMASSUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_ROTATIONALMASSUNIT;
	}
	else if( boost::iequals( arg, L".SECTIONAREAINTEGRALUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_SECTIONAREAINTEGRALUNIT;
	}
	else if( boost::iequals( arg, L".SECTIONMODULUSUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_SECTIONMODULUSUNIT;
	}
	else if( boost::iequals( arg, L".SOUNDPOWERLEVELUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_SOUNDPOWERLEVELUNIT;
	}
	else if( boost::iequals( arg, L".SOUNDPOWERUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_SOUNDPOWERUNIT;
	}
	else if( boost::iequals( arg, L".SOUNDPRESSURELEVELUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_SOUNDPRESSURELEVELUNIT;
	}
	else if( boost::iequals( arg, L".SOUNDPRESSUREUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_SOUNDPRESSUREUNIT;
	}
	else if( boost::iequals( arg, L".TEMPERATUREGRADIENTUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_TEMPERATUREGRADIENTUNIT;
	}
	else if( boost::iequals( arg, L".TEMPERATURERATEOFCHANGEUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_TEMPERATURERATEOFCHANGEUNIT;
	}
	else if( boost::iequals( arg, L".THERMALEXPANSIONCOEFFICIENTUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_THERMALEXPANSIONCOEFFICIENTUNIT;
	}
	else if( boost::iequals( arg, L".WARPINGCONSTANTUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_WARPINGCONSTANTUNIT;
	}
	else if( boost::iequals( arg, L".WARPINGMOMENTUNIT." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_WARPINGMOMENTUNIT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDerivedUnitEnum::ENUM_USERDEFINED;
	}
	return type_object;
}

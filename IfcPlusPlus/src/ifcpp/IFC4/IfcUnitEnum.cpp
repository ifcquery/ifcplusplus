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
#include "include/IfcUnitEnum.h"

// TYPE IfcUnitEnum = ENUMERATION OF	(ABSORBEDDOSEUNIT	,AMOUNTOFSUBSTANCEUNIT	,AREAUNIT	,DOSEEQUIVALENTUNIT	,ELECTRICCAPACITANCEUNIT	,ELECTRICCHARGEUNIT	,ELECTRICCONDUCTANCEUNIT	,ELECTRICCURRENTUNIT	,ELECTRICRESISTANCEUNIT	,ELECTRICVOLTAGEUNIT	,ENERGYUNIT	,FORCEUNIT	,FREQUENCYUNIT	,ILLUMINANCEUNIT	,INDUCTANCEUNIT	,LENGTHUNIT	,LUMINOUSFLUXUNIT	,LUMINOUSINTENSITYUNIT	,MAGNETICFLUXDENSITYUNIT	,MAGNETICFLUXUNIT	,MASSUNIT	,PLANEANGLEUNIT	,POWERUNIT	,PRESSUREUNIT	,RADIOACTIVITYUNIT	,SOLIDANGLEUNIT	,THERMODYNAMICTEMPERATUREUNIT	,TIMEUNIT	,VOLUMEUNIT	,USERDEFINED);
IfcUnitEnum::IfcUnitEnum() {}
IfcUnitEnum::~IfcUnitEnum() {}
shared_ptr<IfcPPObject> IfcUnitEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcUnitEnum> copy_self( new IfcUnitEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcUnitEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCUNITENUM("; }
	if( m_enum == ENUM_ABSORBEDDOSEUNIT )
	{
		stream << ".ABSORBEDDOSEUNIT.";
	}
	else if( m_enum == ENUM_AMOUNTOFSUBSTANCEUNIT )
	{
		stream << ".AMOUNTOFSUBSTANCEUNIT.";
	}
	else if( m_enum == ENUM_AREAUNIT )
	{
		stream << ".AREAUNIT.";
	}
	else if( m_enum == ENUM_DOSEEQUIVALENTUNIT )
	{
		stream << ".DOSEEQUIVALENTUNIT.";
	}
	else if( m_enum == ENUM_ELECTRICCAPACITANCEUNIT )
	{
		stream << ".ELECTRICCAPACITANCEUNIT.";
	}
	else if( m_enum == ENUM_ELECTRICCHARGEUNIT )
	{
		stream << ".ELECTRICCHARGEUNIT.";
	}
	else if( m_enum == ENUM_ELECTRICCONDUCTANCEUNIT )
	{
		stream << ".ELECTRICCONDUCTANCEUNIT.";
	}
	else if( m_enum == ENUM_ELECTRICCURRENTUNIT )
	{
		stream << ".ELECTRICCURRENTUNIT.";
	}
	else if( m_enum == ENUM_ELECTRICRESISTANCEUNIT )
	{
		stream << ".ELECTRICRESISTANCEUNIT.";
	}
	else if( m_enum == ENUM_ELECTRICVOLTAGEUNIT )
	{
		stream << ".ELECTRICVOLTAGEUNIT.";
	}
	else if( m_enum == ENUM_ENERGYUNIT )
	{
		stream << ".ENERGYUNIT.";
	}
	else if( m_enum == ENUM_FORCEUNIT )
	{
		stream << ".FORCEUNIT.";
	}
	else if( m_enum == ENUM_FREQUENCYUNIT )
	{
		stream << ".FREQUENCYUNIT.";
	}
	else if( m_enum == ENUM_ILLUMINANCEUNIT )
	{
		stream << ".ILLUMINANCEUNIT.";
	}
	else if( m_enum == ENUM_INDUCTANCEUNIT )
	{
		stream << ".INDUCTANCEUNIT.";
	}
	else if( m_enum == ENUM_LENGTHUNIT )
	{
		stream << ".LENGTHUNIT.";
	}
	else if( m_enum == ENUM_LUMINOUSFLUXUNIT )
	{
		stream << ".LUMINOUSFLUXUNIT.";
	}
	else if( m_enum == ENUM_LUMINOUSINTENSITYUNIT )
	{
		stream << ".LUMINOUSINTENSITYUNIT.";
	}
	else if( m_enum == ENUM_MAGNETICFLUXDENSITYUNIT )
	{
		stream << ".MAGNETICFLUXDENSITYUNIT.";
	}
	else if( m_enum == ENUM_MAGNETICFLUXUNIT )
	{
		stream << ".MAGNETICFLUXUNIT.";
	}
	else if( m_enum == ENUM_MASSUNIT )
	{
		stream << ".MASSUNIT.";
	}
	else if( m_enum == ENUM_PLANEANGLEUNIT )
	{
		stream << ".PLANEANGLEUNIT.";
	}
	else if( m_enum == ENUM_POWERUNIT )
	{
		stream << ".POWERUNIT.";
	}
	else if( m_enum == ENUM_PRESSUREUNIT )
	{
		stream << ".PRESSUREUNIT.";
	}
	else if( m_enum == ENUM_RADIOACTIVITYUNIT )
	{
		stream << ".RADIOACTIVITYUNIT.";
	}
	else if( m_enum == ENUM_SOLIDANGLEUNIT )
	{
		stream << ".SOLIDANGLEUNIT.";
	}
	else if( m_enum == ENUM_THERMODYNAMICTEMPERATUREUNIT )
	{
		stream << ".THERMODYNAMICTEMPERATUREUNIT.";
	}
	else if( m_enum == ENUM_TIMEUNIT )
	{
		stream << ".TIMEUNIT.";
	}
	else if( m_enum == ENUM_VOLUMEUNIT )
	{
		stream << ".VOLUMEUNIT.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcUnitEnum> IfcUnitEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcUnitEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcUnitEnum>(); }
	shared_ptr<IfcUnitEnum> type_object( new IfcUnitEnum() );
	if( boost::iequals( arg, L".ABSORBEDDOSEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ABSORBEDDOSEUNIT;
	}
	else if( boost::iequals( arg, L".AMOUNTOFSUBSTANCEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_AMOUNTOFSUBSTANCEUNIT;
	}
	else if( boost::iequals( arg, L".AREAUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_AREAUNIT;
	}
	else if( boost::iequals( arg, L".DOSEEQUIVALENTUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_DOSEEQUIVALENTUNIT;
	}
	else if( boost::iequals( arg, L".ELECTRICCAPACITANCEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICCAPACITANCEUNIT;
	}
	else if( boost::iequals( arg, L".ELECTRICCHARGEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICCHARGEUNIT;
	}
	else if( boost::iequals( arg, L".ELECTRICCONDUCTANCEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICCONDUCTANCEUNIT;
	}
	else if( boost::iequals( arg, L".ELECTRICCURRENTUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICCURRENTUNIT;
	}
	else if( boost::iequals( arg, L".ELECTRICRESISTANCEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICRESISTANCEUNIT;
	}
	else if( boost::iequals( arg, L".ELECTRICVOLTAGEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICVOLTAGEUNIT;
	}
	else if( boost::iequals( arg, L".ENERGYUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ENERGYUNIT;
	}
	else if( boost::iequals( arg, L".FORCEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_FORCEUNIT;
	}
	else if( boost::iequals( arg, L".FREQUENCYUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_FREQUENCYUNIT;
	}
	else if( boost::iequals( arg, L".ILLUMINANCEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ILLUMINANCEUNIT;
	}
	else if( boost::iequals( arg, L".INDUCTANCEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_INDUCTANCEUNIT;
	}
	else if( boost::iequals( arg, L".LENGTHUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_LENGTHUNIT;
	}
	else if( boost::iequals( arg, L".LUMINOUSFLUXUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_LUMINOUSFLUXUNIT;
	}
	else if( boost::iequals( arg, L".LUMINOUSINTENSITYUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_LUMINOUSINTENSITYUNIT;
	}
	else if( boost::iequals( arg, L".MAGNETICFLUXDENSITYUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_MAGNETICFLUXDENSITYUNIT;
	}
	else if( boost::iequals( arg, L".MAGNETICFLUXUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_MAGNETICFLUXUNIT;
	}
	else if( boost::iequals( arg, L".MASSUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_MASSUNIT;
	}
	else if( boost::iequals( arg, L".PLANEANGLEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_PLANEANGLEUNIT;
	}
	else if( boost::iequals( arg, L".POWERUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_POWERUNIT;
	}
	else if( boost::iequals( arg, L".PRESSUREUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_PRESSUREUNIT;
	}
	else if( boost::iequals( arg, L".RADIOACTIVITYUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_RADIOACTIVITYUNIT;
	}
	else if( boost::iequals( arg, L".SOLIDANGLEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_SOLIDANGLEUNIT;
	}
	else if( boost::iequals( arg, L".THERMODYNAMICTEMPERATUREUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_THERMODYNAMICTEMPERATUREUNIT;
	}
	else if( boost::iequals( arg, L".TIMEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_TIMEUNIT;
	}
	else if( boost::iequals( arg, L".VOLUMEUNIT." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_VOLUMEUNIT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_USERDEFINED;
	}
	return type_object;
}

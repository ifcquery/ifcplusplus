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

// TYPE IfcUnitEnum 
IfcUnitEnum::IfcUnitEnum() {}
IfcUnitEnum::~IfcUnitEnum() {}
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
shared_ptr<IfcUnitEnum> IfcUnitEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcUnitEnum>(); }
	shared_ptr<IfcUnitEnum> type_object( new IfcUnitEnum() );
	if( _stricmp( arg.c_str(), ".ABSORBEDDOSEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ABSORBEDDOSEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".AMOUNTOFSUBSTANCEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_AMOUNTOFSUBSTANCEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".AREAUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_AREAUNIT;
	}
	else if( _stricmp( arg.c_str(), ".DOSEEQUIVALENTUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_DOSEEQUIVALENTUNIT;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICCAPACITANCEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICCAPACITANCEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICCHARGEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICCHARGEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICCONDUCTANCEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICCONDUCTANCEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICCURRENTUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICCURRENTUNIT;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICRESISTANCEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICRESISTANCEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICVOLTAGEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ELECTRICVOLTAGEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".ENERGYUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ENERGYUNIT;
	}
	else if( _stricmp( arg.c_str(), ".FORCEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_FORCEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".FREQUENCYUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_FREQUENCYUNIT;
	}
	else if( _stricmp( arg.c_str(), ".ILLUMINANCEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_ILLUMINANCEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".INDUCTANCEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_INDUCTANCEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".LENGTHUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_LENGTHUNIT;
	}
	else if( _stricmp( arg.c_str(), ".LUMINOUSFLUXUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_LUMINOUSFLUXUNIT;
	}
	else if( _stricmp( arg.c_str(), ".LUMINOUSINTENSITYUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_LUMINOUSINTENSITYUNIT;
	}
	else if( _stricmp( arg.c_str(), ".MAGNETICFLUXDENSITYUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_MAGNETICFLUXDENSITYUNIT;
	}
	else if( _stricmp( arg.c_str(), ".MAGNETICFLUXUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_MAGNETICFLUXUNIT;
	}
	else if( _stricmp( arg.c_str(), ".MASSUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_MASSUNIT;
	}
	else if( _stricmp( arg.c_str(), ".PLANEANGLEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_PLANEANGLEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".POWERUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_POWERUNIT;
	}
	else if( _stricmp( arg.c_str(), ".PRESSUREUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_PRESSUREUNIT;
	}
	else if( _stricmp( arg.c_str(), ".RADIOACTIVITYUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_RADIOACTIVITYUNIT;
	}
	else if( _stricmp( arg.c_str(), ".SOLIDANGLEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_SOLIDANGLEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".THERMODYNAMICTEMPERATUREUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_THERMODYNAMICTEMPERATUREUNIT;
	}
	else if( _stricmp( arg.c_str(), ".TIMEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_TIMEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".VOLUMEUNIT." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_VOLUMEUNIT;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcUnitEnum::ENUM_USERDEFINED;
	}
	return type_object;
}

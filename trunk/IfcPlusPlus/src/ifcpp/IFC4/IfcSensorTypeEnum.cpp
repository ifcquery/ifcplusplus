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
#include "include/IfcSensorTypeEnum.h"

// TYPE IfcSensorTypeEnum 
IfcSensorTypeEnum::IfcSensorTypeEnum() {}
IfcSensorTypeEnum::~IfcSensorTypeEnum() {}
void IfcSensorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSENSORTYPEENUM("; }
	if( m_enum == ENUM_CONDUCTANCESENSOR )
	{
		stream << ".CONDUCTANCESENSOR.";
	}
	else if( m_enum == ENUM_CONTACTSENSOR )
	{
		stream << ".CONTACTSENSOR.";
	}
	else if( m_enum == ENUM_FIRESENSOR )
	{
		stream << ".FIRESENSOR.";
	}
	else if( m_enum == ENUM_FLOWSENSOR )
	{
		stream << ".FLOWSENSOR.";
	}
	else if( m_enum == ENUM_GASSENSOR )
	{
		stream << ".GASSENSOR.";
	}
	else if( m_enum == ENUM_HEATSENSOR )
	{
		stream << ".HEATSENSOR.";
	}
	else if( m_enum == ENUM_HUMIDITYSENSOR )
	{
		stream << ".HUMIDITYSENSOR.";
	}
	else if( m_enum == ENUM_IONCONCENTRATIONSENSOR )
	{
		stream << ".IONCONCENTRATIONSENSOR.";
	}
	else if( m_enum == ENUM_LEVELSENSOR )
	{
		stream << ".LEVELSENSOR.";
	}
	else if( m_enum == ENUM_LIGHTSENSOR )
	{
		stream << ".LIGHTSENSOR.";
	}
	else if( m_enum == ENUM_MOISTURESENSOR )
	{
		stream << ".MOISTURESENSOR.";
	}
	else if( m_enum == ENUM_MOVEMENTSENSOR )
	{
		stream << ".MOVEMENTSENSOR.";
	}
	else if( m_enum == ENUM_PHSENSOR )
	{
		stream << ".PHSENSOR.";
	}
	else if( m_enum == ENUM_PRESSURESENSOR )
	{
		stream << ".PRESSURESENSOR.";
	}
	else if( m_enum == ENUM_RADIATIONSENSOR )
	{
		stream << ".RADIATIONSENSOR.";
	}
	else if( m_enum == ENUM_RADIOACTIVITYSENSOR )
	{
		stream << ".RADIOACTIVITYSENSOR.";
	}
	else if( m_enum == ENUM_SMOKESENSOR )
	{
		stream << ".SMOKESENSOR.";
	}
	else if( m_enum == ENUM_SOUNDSENSOR )
	{
		stream << ".SOUNDSENSOR.";
	}
	else if( m_enum == ENUM_TEMPERATURESENSOR )
	{
		stream << ".TEMPERATURESENSOR.";
	}
	else if( m_enum == ENUM_WINDSENSOR )
	{
		stream << ".WINDSENSOR.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSensorTypeEnum> IfcSensorTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcSensorTypeEnum>(); }
	shared_ptr<IfcSensorTypeEnum> type_object( new IfcSensorTypeEnum() );
	if( _stricmp( arg.c_str(), ".CONDUCTANCESENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_CONDUCTANCESENSOR;
	}
	else if( _stricmp( arg.c_str(), ".CONTACTSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_CONTACTSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".FIRESENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_FIRESENSOR;
	}
	else if( _stricmp( arg.c_str(), ".FLOWSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_FLOWSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".GASSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_GASSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".HEATSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_HEATSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".HUMIDITYSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_HUMIDITYSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".IONCONCENTRATIONSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_IONCONCENTRATIONSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".LEVELSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_LEVELSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".LIGHTSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_LIGHTSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".MOISTURESENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_MOISTURESENSOR;
	}
	else if( _stricmp( arg.c_str(), ".MOVEMENTSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_MOVEMENTSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".PHSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_PHSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".PRESSURESENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_PRESSURESENSOR;
	}
	else if( _stricmp( arg.c_str(), ".RADIATIONSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_RADIATIONSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".RADIOACTIVITYSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_RADIOACTIVITYSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".SMOKESENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_SMOKESENSOR;
	}
	else if( _stricmp( arg.c_str(), ".SOUNDSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_SOUNDSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".TEMPERATURESENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_TEMPERATURESENSOR;
	}
	else if( _stricmp( arg.c_str(), ".WINDSENSOR." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_WINDSENSOR;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

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

// TYPE IfcSensorTypeEnum = ENUMERATION OF	(CONDUCTANCESENSOR	,CONTACTSENSOR	,FIRESENSOR	,FLOWSENSOR	,GASSENSOR	,HEATSENSOR	,HUMIDITYSENSOR	,IONCONCENTRATIONSENSOR	,LEVELSENSOR	,LIGHTSENSOR	,MOISTURESENSOR	,MOVEMENTSENSOR	,PHSENSOR	,PRESSURESENSOR	,RADIATIONSENSOR	,RADIOACTIVITYSENSOR	,SMOKESENSOR	,SOUNDSENSOR	,TEMPERATURESENSOR	,WINDSENSOR	,USERDEFINED	,NOTDEFINED);
IfcSensorTypeEnum::IfcSensorTypeEnum() {}
IfcSensorTypeEnum::~IfcSensorTypeEnum() {}
shared_ptr<IfcPPObject> IfcSensorTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSensorTypeEnum> copy_self( new IfcSensorTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcSensorTypeEnum> IfcSensorTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSensorTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSensorTypeEnum>(); }
	shared_ptr<IfcSensorTypeEnum> type_object( new IfcSensorTypeEnum() );
	if( boost::iequals( arg, L".CONDUCTANCESENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_CONDUCTANCESENSOR;
	}
	else if( boost::iequals( arg, L".CONTACTSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_CONTACTSENSOR;
	}
	else if( boost::iequals( arg, L".FIRESENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_FIRESENSOR;
	}
	else if( boost::iequals( arg, L".FLOWSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_FLOWSENSOR;
	}
	else if( boost::iequals( arg, L".GASSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_GASSENSOR;
	}
	else if( boost::iequals( arg, L".HEATSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_HEATSENSOR;
	}
	else if( boost::iequals( arg, L".HUMIDITYSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_HUMIDITYSENSOR;
	}
	else if( boost::iequals( arg, L".IONCONCENTRATIONSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_IONCONCENTRATIONSENSOR;
	}
	else if( boost::iequals( arg, L".LEVELSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_LEVELSENSOR;
	}
	else if( boost::iequals( arg, L".LIGHTSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_LIGHTSENSOR;
	}
	else if( boost::iequals( arg, L".MOISTURESENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_MOISTURESENSOR;
	}
	else if( boost::iequals( arg, L".MOVEMENTSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_MOVEMENTSENSOR;
	}
	else if( boost::iequals( arg, L".PHSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_PHSENSOR;
	}
	else if( boost::iequals( arg, L".PRESSURESENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_PRESSURESENSOR;
	}
	else if( boost::iequals( arg, L".RADIATIONSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_RADIATIONSENSOR;
	}
	else if( boost::iequals( arg, L".RADIOACTIVITYSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_RADIOACTIVITYSENSOR;
	}
	else if( boost::iequals( arg, L".SMOKESENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_SMOKESENSOR;
	}
	else if( boost::iequals( arg, L".SOUNDSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_SOUNDSENSOR;
	}
	else if( boost::iequals( arg, L".TEMPERATURESENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_TEMPERATURESENSOR;
	}
	else if( boost::iequals( arg, L".WINDSENSOR." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_WINDSENSOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSensorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

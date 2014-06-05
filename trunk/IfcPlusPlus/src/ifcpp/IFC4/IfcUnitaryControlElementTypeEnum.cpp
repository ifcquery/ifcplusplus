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
#include "include/IfcUnitaryControlElementTypeEnum.h"

// TYPE IfcUnitaryControlElementTypeEnum 
IfcUnitaryControlElementTypeEnum::IfcUnitaryControlElementTypeEnum() {}
IfcUnitaryControlElementTypeEnum::~IfcUnitaryControlElementTypeEnum() {}
void IfcUnitaryControlElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCUNITARYCONTROLELEMENTTYPEENUM("; }
	if( m_enum == ENUM_ALARMPANEL )
	{
		stream << ".ALARMPANEL.";
	}
	else if( m_enum == ENUM_CONTROLPANEL )
	{
		stream << ".CONTROLPANEL.";
	}
	else if( m_enum == ENUM_GASDETECTIONPANEL )
	{
		stream << ".GASDETECTIONPANEL.";
	}
	else if( m_enum == ENUM_INDICATORPANEL )
	{
		stream << ".INDICATORPANEL.";
	}
	else if( m_enum == ENUM_MIMICPANEL )
	{
		stream << ".MIMICPANEL.";
	}
	else if( m_enum == ENUM_HUMIDISTAT )
	{
		stream << ".HUMIDISTAT.";
	}
	else if( m_enum == ENUM_THERMOSTAT )
	{
		stream << ".THERMOSTAT.";
	}
	else if( m_enum == ENUM_WEATHERSTATION )
	{
		stream << ".WEATHERSTATION.";
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
shared_ptr<IfcUnitaryControlElementTypeEnum> IfcUnitaryControlElementTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcUnitaryControlElementTypeEnum>(); }
	shared_ptr<IfcUnitaryControlElementTypeEnum> type_object( new IfcUnitaryControlElementTypeEnum() );
	if( _stricmp( arg.c_str(), ".ALARMPANEL." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_ALARMPANEL;
	}
	else if( _stricmp( arg.c_str(), ".CONTROLPANEL." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_CONTROLPANEL;
	}
	else if( _stricmp( arg.c_str(), ".GASDETECTIONPANEL." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_GASDETECTIONPANEL;
	}
	else if( _stricmp( arg.c_str(), ".INDICATORPANEL." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_INDICATORPANEL;
	}
	else if( _stricmp( arg.c_str(), ".MIMICPANEL." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_MIMICPANEL;
	}
	else if( _stricmp( arg.c_str(), ".HUMIDISTAT." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_HUMIDISTAT;
	}
	else if( _stricmp( arg.c_str(), ".THERMOSTAT." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_THERMOSTAT;
	}
	else if( _stricmp( arg.c_str(), ".WEATHERSTATION." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_WEATHERSTATION;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

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
#include "include/IfcSpatialZoneTypeEnum.h"

// TYPE IfcSpatialZoneTypeEnum 
IfcSpatialZoneTypeEnum::IfcSpatialZoneTypeEnum() {}
IfcSpatialZoneTypeEnum::~IfcSpatialZoneTypeEnum() {}
void IfcSpatialZoneTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSPATIALZONETYPEENUM("; }
	if( m_enum == ENUM_CONSTRUCTION )
	{
		stream << ".CONSTRUCTION.";
	}
	else if( m_enum == ENUM_FIRESAFETY )
	{
		stream << ".FIRESAFETY.";
	}
	else if( m_enum == ENUM_LIGHTING )
	{
		stream << ".LIGHTING.";
	}
	else if( m_enum == ENUM_OCCUPANCY )
	{
		stream << ".OCCUPANCY.";
	}
	else if( m_enum == ENUM_SECURITY )
	{
		stream << ".SECURITY.";
	}
	else if( m_enum == ENUM_THERMAL )
	{
		stream << ".THERMAL.";
	}
	else if( m_enum == ENUM_TRANSPORT )
	{
		stream << ".TRANSPORT.";
	}
	else if( m_enum == ENUM_VENTILATION )
	{
		stream << ".VENTILATION.";
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
shared_ptr<IfcSpatialZoneTypeEnum> IfcSpatialZoneTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcSpatialZoneTypeEnum>(); }
	shared_ptr<IfcSpatialZoneTypeEnum> type_object( new IfcSpatialZoneTypeEnum() );
	if( _stricmp( arg.c_str(), ".CONSTRUCTION." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_CONSTRUCTION;
	}
	else if( _stricmp( arg.c_str(), ".FIRESAFETY." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_FIRESAFETY;
	}
	else if( _stricmp( arg.c_str(), ".LIGHTING." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_LIGHTING;
	}
	else if( _stricmp( arg.c_str(), ".OCCUPANCY." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_OCCUPANCY;
	}
	else if( _stricmp( arg.c_str(), ".SECURITY." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_SECURITY;
	}
	else if( _stricmp( arg.c_str(), ".THERMAL." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_THERMAL;
	}
	else if( _stricmp( arg.c_str(), ".TRANSPORT." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_TRANSPORT;
	}
	else if( _stricmp( arg.c_str(), ".VENTILATION." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_VENTILATION;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

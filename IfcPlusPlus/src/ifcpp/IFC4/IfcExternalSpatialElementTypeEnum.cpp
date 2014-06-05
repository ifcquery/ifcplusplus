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
#include "include/IfcExternalSpatialElementTypeEnum.h"

// TYPE IfcExternalSpatialElementTypeEnum 
IfcExternalSpatialElementTypeEnum::IfcExternalSpatialElementTypeEnum() {}
IfcExternalSpatialElementTypeEnum::~IfcExternalSpatialElementTypeEnum() {}
void IfcExternalSpatialElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCEXTERNALSPATIALELEMENTTYPEENUM("; }
	if( m_enum == ENUM_EXTERNAL )
	{
		stream << ".EXTERNAL.";
	}
	else if( m_enum == ENUM_EXTERNAL_EARTH )
	{
		stream << ".EXTERNAL_EARTH.";
	}
	else if( m_enum == ENUM_EXTERNAL_WATER )
	{
		stream << ".EXTERNAL_WATER.";
	}
	else if( m_enum == ENUM_EXTERNAL_FIRE )
	{
		stream << ".EXTERNAL_FIRE.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	else if( m_enum == ENUM_NOTDEFIEND )
	{
		stream << ".NOTDEFIEND.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcExternalSpatialElementTypeEnum> IfcExternalSpatialElementTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcExternalSpatialElementTypeEnum>(); }
	shared_ptr<IfcExternalSpatialElementTypeEnum> type_object( new IfcExternalSpatialElementTypeEnum() );
	if( _stricmp( arg.c_str(), ".EXTERNAL." ) == 0 )
	{
		type_object->m_enum = IfcExternalSpatialElementTypeEnum::ENUM_EXTERNAL;
	}
	else if( _stricmp( arg.c_str(), ".EXTERNAL_EARTH." ) == 0 )
	{
		type_object->m_enum = IfcExternalSpatialElementTypeEnum::ENUM_EXTERNAL_EARTH;
	}
	else if( _stricmp( arg.c_str(), ".EXTERNAL_WATER." ) == 0 )
	{
		type_object->m_enum = IfcExternalSpatialElementTypeEnum::ENUM_EXTERNAL_WATER;
	}
	else if( _stricmp( arg.c_str(), ".EXTERNAL_FIRE." ) == 0 )
	{
		type_object->m_enum = IfcExternalSpatialElementTypeEnum::ENUM_EXTERNAL_FIRE;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcExternalSpatialElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFIEND." ) == 0 )
	{
		type_object->m_enum = IfcExternalSpatialElementTypeEnum::ENUM_NOTDEFIEND;
	}
	return type_object;
}

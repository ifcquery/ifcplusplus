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
#include "include/IfcObjectTypeEnum.h"

// TYPE IfcObjectTypeEnum 
IfcObjectTypeEnum::IfcObjectTypeEnum() {}
IfcObjectTypeEnum::~IfcObjectTypeEnum() {}
void IfcObjectTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOBJECTTYPEENUM("; }
	if( m_enum == ENUM_PRODUCT )
	{
		stream << ".PRODUCT.";
	}
	else if( m_enum == ENUM_PROCESS )
	{
		stream << ".PROCESS.";
	}
	else if( m_enum == ENUM_CONTROL )
	{
		stream << ".CONTROL.";
	}
	else if( m_enum == ENUM_RESOURCE )
	{
		stream << ".RESOURCE.";
	}
	else if( m_enum == ENUM_ACTOR )
	{
		stream << ".ACTOR.";
	}
	else if( m_enum == ENUM_GROUP )
	{
		stream << ".GROUP.";
	}
	else if( m_enum == ENUM_PROJECT )
	{
		stream << ".PROJECT.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcObjectTypeEnum> IfcObjectTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcObjectTypeEnum>(); }
	shared_ptr<IfcObjectTypeEnum> type_object( new IfcObjectTypeEnum() );
	if( _stricmp( arg.c_str(), ".PRODUCT." ) == 0 )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PRODUCT;
	}
	else if( _stricmp( arg.c_str(), ".PROCESS." ) == 0 )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PROCESS;
	}
	else if( _stricmp( arg.c_str(), ".CONTROL." ) == 0 )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_CONTROL;
	}
	else if( _stricmp( arg.c_str(), ".RESOURCE." ) == 0 )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_RESOURCE;
	}
	else if( _stricmp( arg.c_str(), ".ACTOR." ) == 0 )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_ACTOR;
	}
	else if( _stricmp( arg.c_str(), ".GROUP." ) == 0 )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_GROUP;
	}
	else if( _stricmp( arg.c_str(), ".PROJECT." ) == 0 )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PROJECT;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

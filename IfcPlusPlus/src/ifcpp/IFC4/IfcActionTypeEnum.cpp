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
#include "include/IfcActionTypeEnum.h"

// TYPE IfcActionTypeEnum 
IfcActionTypeEnum::IfcActionTypeEnum() {}
IfcActionTypeEnum::~IfcActionTypeEnum() {}
void IfcActionTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCACTIONTYPEENUM("; }
	if( m_enum == ENUM_PERMANENT_G )
	{
		stream << ".PERMANENT_G.";
	}
	else if( m_enum == ENUM_VARIABLE_Q )
	{
		stream << ".VARIABLE_Q.";
	}
	else if( m_enum == ENUM_EXTRAORDINARY_A )
	{
		stream << ".EXTRAORDINARY_A.";
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
shared_ptr<IfcActionTypeEnum> IfcActionTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcActionTypeEnum>(); }
	shared_ptr<IfcActionTypeEnum> type_object( new IfcActionTypeEnum() );
	if( _stricmp( arg.c_str(), ".PERMANENT_G." ) == 0 )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_PERMANENT_G;
	}
	else if( _stricmp( arg.c_str(), ".VARIABLE_Q." ) == 0 )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_VARIABLE_Q;
	}
	else if( _stricmp( arg.c_str(), ".EXTRAORDINARY_A." ) == 0 )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_EXTRAORDINARY_A;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

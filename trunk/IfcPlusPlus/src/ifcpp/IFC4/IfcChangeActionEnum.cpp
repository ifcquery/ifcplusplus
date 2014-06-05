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
#include "include/IfcChangeActionEnum.h"

// TYPE IfcChangeActionEnum 
IfcChangeActionEnum::IfcChangeActionEnum() {}
IfcChangeActionEnum::~IfcChangeActionEnum() {}
void IfcChangeActionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCHANGEACTIONENUM("; }
	if( m_enum == ENUM_NOCHANGE )
	{
		stream << ".NOCHANGE.";
	}
	else if( m_enum == ENUM_MODIFIED )
	{
		stream << ".MODIFIED.";
	}
	else if( m_enum == ENUM_ADDED )
	{
		stream << ".ADDED.";
	}
	else if( m_enum == ENUM_DELETED )
	{
		stream << ".DELETED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcChangeActionEnum> IfcChangeActionEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcChangeActionEnum>(); }
	shared_ptr<IfcChangeActionEnum> type_object( new IfcChangeActionEnum() );
	if( _stricmp( arg.c_str(), ".NOCHANGE." ) == 0 )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_NOCHANGE;
	}
	else if( _stricmp( arg.c_str(), ".MODIFIED." ) == 0 )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_MODIFIED;
	}
	else if( _stricmp( arg.c_str(), ".ADDED." ) == 0 )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_ADDED;
	}
	else if( _stricmp( arg.c_str(), ".DELETED." ) == 0 )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_DELETED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

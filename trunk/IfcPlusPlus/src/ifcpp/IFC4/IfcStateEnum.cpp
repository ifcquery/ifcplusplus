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
#include "include/IfcStateEnum.h"

// TYPE IfcStateEnum 
IfcStateEnum::IfcStateEnum() {}
IfcStateEnum::~IfcStateEnum() {}
void IfcStateEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTATEENUM("; }
	if( m_enum == ENUM_READWRITE )
	{
		stream << ".READWRITE.";
	}
	else if( m_enum == ENUM_READONLY )
	{
		stream << ".READONLY.";
	}
	else if( m_enum == ENUM_LOCKED )
	{
		stream << ".LOCKED.";
	}
	else if( m_enum == ENUM_READWRITELOCKED )
	{
		stream << ".READWRITELOCKED.";
	}
	else if( m_enum == ENUM_READONLYLOCKED )
	{
		stream << ".READONLYLOCKED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcStateEnum> IfcStateEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcStateEnum>(); }
	shared_ptr<IfcStateEnum> type_object( new IfcStateEnum() );
	if( _stricmp( arg.c_str(), ".READWRITE." ) == 0 )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READWRITE;
	}
	else if( _stricmp( arg.c_str(), ".READONLY." ) == 0 )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READONLY;
	}
	else if( _stricmp( arg.c_str(), ".LOCKED." ) == 0 )
	{
		type_object->m_enum = IfcStateEnum::ENUM_LOCKED;
	}
	else if( _stricmp( arg.c_str(), ".READWRITELOCKED." ) == 0 )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READWRITELOCKED;
	}
	else if( _stricmp( arg.c_str(), ".READONLYLOCKED." ) == 0 )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READONLYLOCKED;
	}
	return type_object;
}

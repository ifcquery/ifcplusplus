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
shared_ptr<IfcPPObject> IfcStateEnum::getDeepCopy()
{
	shared_ptr<IfcStateEnum> copy_self( new IfcStateEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcStateEnum> IfcStateEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcStateEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcStateEnum>(); }
	shared_ptr<IfcStateEnum> type_object( new IfcStateEnum() );
	if( boost::iequals( arg, L".READWRITE." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READWRITE;
	}
	else if( boost::iequals( arg, L".READONLY." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READONLY;
	}
	else if( boost::iequals( arg, L".LOCKED." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_LOCKED;
	}
	else if( boost::iequals( arg, L".READWRITELOCKED." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READWRITELOCKED;
	}
	else if( boost::iequals( arg, L".READONLYLOCKED." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READONLYLOCKED;
	}
	return type_object;
}

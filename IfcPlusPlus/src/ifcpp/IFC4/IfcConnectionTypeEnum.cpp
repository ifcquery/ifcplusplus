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
#include "include/IfcConnectionTypeEnum.h"

// TYPE IfcConnectionTypeEnum 
IfcConnectionTypeEnum::IfcConnectionTypeEnum() {}
IfcConnectionTypeEnum::~IfcConnectionTypeEnum() {}
shared_ptr<IfcPPObject> IfcConnectionTypeEnum::getDeepCopy()
{
	shared_ptr<IfcConnectionTypeEnum> copy_self( new IfcConnectionTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcConnectionTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONNECTIONTYPEENUM("; }
	if( m_enum == ENUM_ATPATH )
	{
		stream << ".ATPATH.";
	}
	else if( m_enum == ENUM_ATSTART )
	{
		stream << ".ATSTART.";
	}
	else if( m_enum == ENUM_ATEND )
	{
		stream << ".ATEND.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcConnectionTypeEnum> IfcConnectionTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcConnectionTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcConnectionTypeEnum>(); }
	shared_ptr<IfcConnectionTypeEnum> type_object( new IfcConnectionTypeEnum() );
	if( boost::iequals( arg, L".ATPATH." ) )
	{
		type_object->m_enum = IfcConnectionTypeEnum::ENUM_ATPATH;
	}
	else if( boost::iequals( arg, L".ATSTART." ) )
	{
		type_object->m_enum = IfcConnectionTypeEnum::ENUM_ATSTART;
	}
	else if( boost::iequals( arg, L".ATEND." ) )
	{
		type_object->m_enum = IfcConnectionTypeEnum::ENUM_ATEND;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcConnectionTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

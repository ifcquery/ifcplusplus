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
#include "include/IfcWorkCalendarTypeEnum.h"

// TYPE IfcWorkCalendarTypeEnum = ENUMERATION OF	(FIRSTSHIFT	,SECONDSHIFT	,THIRDSHIFT	,USERDEFINED	,NOTDEFINED);
IfcWorkCalendarTypeEnum::IfcWorkCalendarTypeEnum() {}
IfcWorkCalendarTypeEnum::~IfcWorkCalendarTypeEnum() {}
shared_ptr<IfcPPObject> IfcWorkCalendarTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWorkCalendarTypeEnum> copy_self( new IfcWorkCalendarTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWorkCalendarTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWORKCALENDARTYPEENUM("; }
	if( m_enum == ENUM_FIRSTSHIFT )
	{
		stream << ".FIRSTSHIFT.";
	}
	else if( m_enum == ENUM_SECONDSHIFT )
	{
		stream << ".SECONDSHIFT.";
	}
	else if( m_enum == ENUM_THIRDSHIFT )
	{
		stream << ".THIRDSHIFT.";
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
shared_ptr<IfcWorkCalendarTypeEnum> IfcWorkCalendarTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWorkCalendarTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWorkCalendarTypeEnum>(); }
	shared_ptr<IfcWorkCalendarTypeEnum> type_object( new IfcWorkCalendarTypeEnum() );
	if( boost::iequals( arg, L".FIRSTSHIFT." ) )
	{
		type_object->m_enum = IfcWorkCalendarTypeEnum::ENUM_FIRSTSHIFT;
	}
	else if( boost::iequals( arg, L".SECONDSHIFT." ) )
	{
		type_object->m_enum = IfcWorkCalendarTypeEnum::ENUM_SECONDSHIFT;
	}
	else if( boost::iequals( arg, L".THIRDSHIFT." ) )
	{
		type_object->m_enum = IfcWorkCalendarTypeEnum::ENUM_THIRDSHIFT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcWorkCalendarTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWorkCalendarTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

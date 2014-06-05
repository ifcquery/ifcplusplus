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
#include "include/IfcWorkScheduleTypeEnum.h"

// TYPE IfcWorkScheduleTypeEnum 
IfcWorkScheduleTypeEnum::IfcWorkScheduleTypeEnum() {}
IfcWorkScheduleTypeEnum::~IfcWorkScheduleTypeEnum() {}
void IfcWorkScheduleTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWORKSCHEDULETYPEENUM("; }
	if( m_enum == ENUM_ACTUAL )
	{
		stream << ".ACTUAL.";
	}
	else if( m_enum == ENUM_BASELINE )
	{
		stream << ".BASELINE.";
	}
	else if( m_enum == ENUM_PLANNED )
	{
		stream << ".PLANNED.";
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
shared_ptr<IfcWorkScheduleTypeEnum> IfcWorkScheduleTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcWorkScheduleTypeEnum>(); }
	shared_ptr<IfcWorkScheduleTypeEnum> type_object( new IfcWorkScheduleTypeEnum() );
	if( _stricmp( arg.c_str(), ".ACTUAL." ) == 0 )
	{
		type_object->m_enum = IfcWorkScheduleTypeEnum::ENUM_ACTUAL;
	}
	else if( _stricmp( arg.c_str(), ".BASELINE." ) == 0 )
	{
		type_object->m_enum = IfcWorkScheduleTypeEnum::ENUM_BASELINE;
	}
	else if( _stricmp( arg.c_str(), ".PLANNED." ) == 0 )
	{
		type_object->m_enum = IfcWorkScheduleTypeEnum::ENUM_PLANNED;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcWorkScheduleTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcWorkScheduleTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

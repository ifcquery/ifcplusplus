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
#include "include/IfcWorkPlanTypeEnum.h"

// TYPE IfcWorkPlanTypeEnum = ENUMERATION OF	(ACTUAL	,BASELINE	,PLANNED	,USERDEFINED	,NOTDEFINED);
IfcWorkPlanTypeEnum::IfcWorkPlanTypeEnum() {}
IfcWorkPlanTypeEnum::~IfcWorkPlanTypeEnum() {}
shared_ptr<IfcPPObject> IfcWorkPlanTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWorkPlanTypeEnum> copy_self( new IfcWorkPlanTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWorkPlanTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWORKPLANTYPEENUM("; }
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
shared_ptr<IfcWorkPlanTypeEnum> IfcWorkPlanTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWorkPlanTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWorkPlanTypeEnum>(); }
	shared_ptr<IfcWorkPlanTypeEnum> type_object( new IfcWorkPlanTypeEnum() );
	if( boost::iequals( arg, L".ACTUAL." ) )
	{
		type_object->m_enum = IfcWorkPlanTypeEnum::ENUM_ACTUAL;
	}
	else if( boost::iequals( arg, L".BASELINE." ) )
	{
		type_object->m_enum = IfcWorkPlanTypeEnum::ENUM_BASELINE;
	}
	else if( boost::iequals( arg, L".PLANNED." ) )
	{
		type_object->m_enum = IfcWorkPlanTypeEnum::ENUM_PLANNED;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcWorkPlanTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWorkPlanTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

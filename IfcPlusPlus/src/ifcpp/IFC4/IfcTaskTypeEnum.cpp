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
#include "include/IfcTaskTypeEnum.h"

// TYPE IfcTaskTypeEnum 
IfcTaskTypeEnum::IfcTaskTypeEnum() {}
IfcTaskTypeEnum::~IfcTaskTypeEnum() {}
shared_ptr<IfcPPObject> IfcTaskTypeEnum::getDeepCopy()
{
	shared_ptr<IfcTaskTypeEnum> copy_self( new IfcTaskTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTaskTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTASKTYPEENUM("; }
	if( m_enum == ENUM_ATTENDANCE )
	{
		stream << ".ATTENDANCE.";
	}
	else if( m_enum == ENUM_CONSTRUCTION )
	{
		stream << ".CONSTRUCTION.";
	}
	else if( m_enum == ENUM_DEMOLITION )
	{
		stream << ".DEMOLITION.";
	}
	else if( m_enum == ENUM_DISMANTLE )
	{
		stream << ".DISMANTLE.";
	}
	else if( m_enum == ENUM_DISPOSAL )
	{
		stream << ".DISPOSAL.";
	}
	else if( m_enum == ENUM_INSTALLATION )
	{
		stream << ".INSTALLATION.";
	}
	else if( m_enum == ENUM_LOGISTIC )
	{
		stream << ".LOGISTIC.";
	}
	else if( m_enum == ENUM_MAINTENANCE )
	{
		stream << ".MAINTENANCE.";
	}
	else if( m_enum == ENUM_MOVE )
	{
		stream << ".MOVE.";
	}
	else if( m_enum == ENUM_OPERATION )
	{
		stream << ".OPERATION.";
	}
	else if( m_enum == ENUM_REMOVAL )
	{
		stream << ".REMOVAL.";
	}
	else if( m_enum == ENUM_RENOVATION )
	{
		stream << ".RENOVATION.";
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
shared_ptr<IfcTaskTypeEnum> IfcTaskTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTaskTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTaskTypeEnum>(); }
	shared_ptr<IfcTaskTypeEnum> type_object( new IfcTaskTypeEnum() );
	if( boost::iequals( arg, L".ATTENDANCE." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_ATTENDANCE;
	}
	else if( boost::iequals( arg, L".CONSTRUCTION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_CONSTRUCTION;
	}
	else if( boost::iequals( arg, L".DEMOLITION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_DEMOLITION;
	}
	else if( boost::iequals( arg, L".DISMANTLE." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_DISMANTLE;
	}
	else if( boost::iequals( arg, L".DISPOSAL." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_DISPOSAL;
	}
	else if( boost::iequals( arg, L".INSTALLATION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_INSTALLATION;
	}
	else if( boost::iequals( arg, L".LOGISTIC." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_LOGISTIC;
	}
	else if( boost::iequals( arg, L".MAINTENANCE." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_MAINTENANCE;
	}
	else if( boost::iequals( arg, L".MOVE." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_MOVE;
	}
	else if( boost::iequals( arg, L".OPERATION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_OPERATION;
	}
	else if( boost::iequals( arg, L".REMOVAL." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_REMOVAL;
	}
	else if( boost::iequals( arg, L".RENOVATION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_RENOVATION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

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
#include "include/IfcRampTypeEnum.h"

// TYPE IfcRampTypeEnum 
IfcRampTypeEnum::IfcRampTypeEnum() {}
IfcRampTypeEnum::~IfcRampTypeEnum() {}
shared_ptr<IfcPPObject> IfcRampTypeEnum::getDeepCopy()
{
	shared_ptr<IfcRampTypeEnum> copy_self( new IfcRampTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRampTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCRAMPTYPEENUM("; }
	if( m_enum == ENUM_STRAIGHT_RUN_RAMP )
	{
		stream << ".STRAIGHT_RUN_RAMP.";
	}
	else if( m_enum == ENUM_TWO_STRAIGHT_RUN_RAMP )
	{
		stream << ".TWO_STRAIGHT_RUN_RAMP.";
	}
	else if( m_enum == ENUM_QUARTER_TURN_RAMP )
	{
		stream << ".QUARTER_TURN_RAMP.";
	}
	else if( m_enum == ENUM_TWO_QUARTER_TURN_RAMP )
	{
		stream << ".TWO_QUARTER_TURN_RAMP.";
	}
	else if( m_enum == ENUM_HALF_TURN_RAMP )
	{
		stream << ".HALF_TURN_RAMP.";
	}
	else if( m_enum == ENUM_SPIRAL_RAMP )
	{
		stream << ".SPIRAL_RAMP.";
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
shared_ptr<IfcRampTypeEnum> IfcRampTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRampTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRampTypeEnum>(); }
	shared_ptr<IfcRampTypeEnum> type_object( new IfcRampTypeEnum() );
	if( boost::iequals( arg, L".STRAIGHT_RUN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_STRAIGHT_RUN_RAMP;
	}
	else if( boost::iequals( arg, L".TWO_STRAIGHT_RUN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_TWO_STRAIGHT_RUN_RAMP;
	}
	else if( boost::iequals( arg, L".QUARTER_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_QUARTER_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".TWO_QUARTER_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_TWO_QUARTER_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".HALF_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_HALF_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".SPIRAL_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_SPIRAL_RAMP;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

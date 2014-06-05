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
shared_ptr<IfcRampTypeEnum> IfcRampTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcRampTypeEnum>(); }
	shared_ptr<IfcRampTypeEnum> type_object( new IfcRampTypeEnum() );
	if( _stricmp( arg.c_str(), ".STRAIGHT_RUN_RAMP." ) == 0 )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_STRAIGHT_RUN_RAMP;
	}
	else if( _stricmp( arg.c_str(), ".TWO_STRAIGHT_RUN_RAMP." ) == 0 )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_TWO_STRAIGHT_RUN_RAMP;
	}
	else if( _stricmp( arg.c_str(), ".QUARTER_TURN_RAMP." ) == 0 )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_QUARTER_TURN_RAMP;
	}
	else if( _stricmp( arg.c_str(), ".TWO_QUARTER_TURN_RAMP." ) == 0 )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_TWO_QUARTER_TURN_RAMP;
	}
	else if( _stricmp( arg.c_str(), ".HALF_TURN_RAMP." ) == 0 )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_HALF_TURN_RAMP;
	}
	else if( _stricmp( arg.c_str(), ".SPIRAL_RAMP." ) == 0 )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_SPIRAL_RAMP;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

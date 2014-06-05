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
#include "include/IfcDoorStyleOperationEnum.h"

// TYPE IfcDoorStyleOperationEnum 
IfcDoorStyleOperationEnum::IfcDoorStyleOperationEnum() {}
IfcDoorStyleOperationEnum::~IfcDoorStyleOperationEnum() {}
void IfcDoorStyleOperationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDOORSTYLEOPERATIONENUM("; }
	if( m_enum == ENUM_SINGLE_SWING_LEFT )
	{
		stream << ".SINGLE_SWING_LEFT.";
	}
	else if( m_enum == ENUM_SINGLE_SWING_RIGHT )
	{
		stream << ".SINGLE_SWING_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_SINGLE_SWING )
	{
		stream << ".DOUBLE_DOOR_SINGLE_SWING.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT )
	{
		stream << ".DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT )
	{
		stream << ".DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_SWING_LEFT )
	{
		stream << ".DOUBLE_SWING_LEFT.";
	}
	else if( m_enum == ENUM_DOUBLE_SWING_RIGHT )
	{
		stream << ".DOUBLE_SWING_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_DOUBLE_SWING )
	{
		stream << ".DOUBLE_DOOR_DOUBLE_SWING.";
	}
	else if( m_enum == ENUM_SLIDING_TO_LEFT )
	{
		stream << ".SLIDING_TO_LEFT.";
	}
	else if( m_enum == ENUM_SLIDING_TO_RIGHT )
	{
		stream << ".SLIDING_TO_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_SLIDING )
	{
		stream << ".DOUBLE_DOOR_SLIDING.";
	}
	else if( m_enum == ENUM_FOLDING_TO_LEFT )
	{
		stream << ".FOLDING_TO_LEFT.";
	}
	else if( m_enum == ENUM_FOLDING_TO_RIGHT )
	{
		stream << ".FOLDING_TO_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_FOLDING )
	{
		stream << ".DOUBLE_DOOR_FOLDING.";
	}
	else if( m_enum == ENUM_REVOLVING )
	{
		stream << ".REVOLVING.";
	}
	else if( m_enum == ENUM_ROLLINGUP )
	{
		stream << ".ROLLINGUP.";
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
shared_ptr<IfcDoorStyleOperationEnum> IfcDoorStyleOperationEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcDoorStyleOperationEnum>(); }
	shared_ptr<IfcDoorStyleOperationEnum> type_object( new IfcDoorStyleOperationEnum() );
	if( _stricmp( arg.c_str(), ".SINGLE_SWING_LEFT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_SINGLE_SWING_LEFT;
	}
	else if( _stricmp( arg.c_str(), ".SINGLE_SWING_RIGHT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_SINGLE_SWING_RIGHT;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_DOOR_SINGLE_SWING." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_DOUBLE_DOOR_SINGLE_SWING;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_SWING_LEFT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_DOUBLE_SWING_LEFT;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_SWING_RIGHT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_DOUBLE_SWING_RIGHT;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_DOOR_DOUBLE_SWING." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_DOUBLE_DOOR_DOUBLE_SWING;
	}
	else if( _stricmp( arg.c_str(), ".SLIDING_TO_LEFT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_SLIDING_TO_LEFT;
	}
	else if( _stricmp( arg.c_str(), ".SLIDING_TO_RIGHT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_SLIDING_TO_RIGHT;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_DOOR_SLIDING." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_DOUBLE_DOOR_SLIDING;
	}
	else if( _stricmp( arg.c_str(), ".FOLDING_TO_LEFT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_FOLDING_TO_LEFT;
	}
	else if( _stricmp( arg.c_str(), ".FOLDING_TO_RIGHT." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_FOLDING_TO_RIGHT;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_DOOR_FOLDING." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_DOUBLE_DOOR_FOLDING;
	}
	else if( _stricmp( arg.c_str(), ".REVOLVING." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_REVOLVING;
	}
	else if( _stricmp( arg.c_str(), ".ROLLINGUP." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_ROLLINGUP;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcDoorStyleOperationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

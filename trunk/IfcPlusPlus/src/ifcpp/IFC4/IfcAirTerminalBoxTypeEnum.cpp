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
#include "include/IfcAirTerminalBoxTypeEnum.h"

// TYPE IfcAirTerminalBoxTypeEnum 
IfcAirTerminalBoxTypeEnum::IfcAirTerminalBoxTypeEnum() {}
IfcAirTerminalBoxTypeEnum::~IfcAirTerminalBoxTypeEnum() {}
void IfcAirTerminalBoxTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCAIRTERMINALBOXTYPEENUM("; }
	if( m_enum == ENUM_CONSTANTFLOW )
	{
		stream << ".CONSTANTFLOW.";
	}
	else if( m_enum == ENUM_VARIABLEFLOWPRESSUREDEPENDANT )
	{
		stream << ".VARIABLEFLOWPRESSUREDEPENDANT.";
	}
	else if( m_enum == ENUM_VARIABLEFLOWPRESSUREINDEPENDANT )
	{
		stream << ".VARIABLEFLOWPRESSUREINDEPENDANT.";
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
shared_ptr<IfcAirTerminalBoxTypeEnum> IfcAirTerminalBoxTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcAirTerminalBoxTypeEnum>(); }
	shared_ptr<IfcAirTerminalBoxTypeEnum> type_object( new IfcAirTerminalBoxTypeEnum() );
	if( _stricmp( arg.c_str(), ".CONSTANTFLOW." ) == 0 )
	{
		type_object->m_enum = IfcAirTerminalBoxTypeEnum::ENUM_CONSTANTFLOW;
	}
	else if( _stricmp( arg.c_str(), ".VARIABLEFLOWPRESSUREDEPENDANT." ) == 0 )
	{
		type_object->m_enum = IfcAirTerminalBoxTypeEnum::ENUM_VARIABLEFLOWPRESSUREDEPENDANT;
	}
	else if( _stricmp( arg.c_str(), ".VARIABLEFLOWPRESSUREINDEPENDANT." ) == 0 )
	{
		type_object->m_enum = IfcAirTerminalBoxTypeEnum::ENUM_VARIABLEFLOWPRESSUREINDEPENDANT;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAirTerminalBoxTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAirTerminalBoxTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

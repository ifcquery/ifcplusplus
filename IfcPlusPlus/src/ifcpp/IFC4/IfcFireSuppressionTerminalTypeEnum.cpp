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
#include "include/IfcFireSuppressionTerminalTypeEnum.h"

// TYPE IfcFireSuppressionTerminalTypeEnum 
IfcFireSuppressionTerminalTypeEnum::IfcFireSuppressionTerminalTypeEnum() {}
IfcFireSuppressionTerminalTypeEnum::~IfcFireSuppressionTerminalTypeEnum() {}
void IfcFireSuppressionTerminalTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFIRESUPPRESSIONTERMINALTYPEENUM("; }
	if( m_enum == ENUM_BREECHINGINLET )
	{
		stream << ".BREECHINGINLET.";
	}
	else if( m_enum == ENUM_FIREHYDRANT )
	{
		stream << ".FIREHYDRANT.";
	}
	else if( m_enum == ENUM_HOSEREEL )
	{
		stream << ".HOSEREEL.";
	}
	else if( m_enum == ENUM_SPRINKLER )
	{
		stream << ".SPRINKLER.";
	}
	else if( m_enum == ENUM_SPRINKLERDEFLECTOR )
	{
		stream << ".SPRINKLERDEFLECTOR.";
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
shared_ptr<IfcFireSuppressionTerminalTypeEnum> IfcFireSuppressionTerminalTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcFireSuppressionTerminalTypeEnum>(); }
	shared_ptr<IfcFireSuppressionTerminalTypeEnum> type_object( new IfcFireSuppressionTerminalTypeEnum() );
	if( _stricmp( arg.c_str(), ".BREECHINGINLET." ) == 0 )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_BREECHINGINLET;
	}
	else if( _stricmp( arg.c_str(), ".FIREHYDRANT." ) == 0 )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_FIREHYDRANT;
	}
	else if( _stricmp( arg.c_str(), ".HOSEREEL." ) == 0 )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_HOSEREEL;
	}
	else if( _stricmp( arg.c_str(), ".SPRINKLER." ) == 0 )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_SPRINKLER;
	}
	else if( _stricmp( arg.c_str(), ".SPRINKLERDEFLECTOR." ) == 0 )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_SPRINKLERDEFLECTOR;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

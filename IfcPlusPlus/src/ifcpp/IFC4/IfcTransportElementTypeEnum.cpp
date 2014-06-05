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
#include "include/IfcTransportElementTypeEnum.h"

// TYPE IfcTransportElementTypeEnum 
IfcTransportElementTypeEnum::IfcTransportElementTypeEnum() {}
IfcTransportElementTypeEnum::~IfcTransportElementTypeEnum() {}
void IfcTransportElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRANSPORTELEMENTTYPEENUM("; }
	if( m_enum == ENUM_ELEVATOR )
	{
		stream << ".ELEVATOR.";
	}
	else if( m_enum == ENUM_ESCALATOR )
	{
		stream << ".ESCALATOR.";
	}
	else if( m_enum == ENUM_MOVINGWALKWAY )
	{
		stream << ".MOVINGWALKWAY.";
	}
	else if( m_enum == ENUM_CRANEWAY )
	{
		stream << ".CRANEWAY.";
	}
	else if( m_enum == ENUM_LIFTINGGEAR )
	{
		stream << ".LIFTINGGEAR.";
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
shared_ptr<IfcTransportElementTypeEnum> IfcTransportElementTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcTransportElementTypeEnum>(); }
	shared_ptr<IfcTransportElementTypeEnum> type_object( new IfcTransportElementTypeEnum() );
	if( _stricmp( arg.c_str(), ".ELEVATOR." ) == 0 )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_ELEVATOR;
	}
	else if( _stricmp( arg.c_str(), ".ESCALATOR." ) == 0 )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_ESCALATOR;
	}
	else if( _stricmp( arg.c_str(), ".MOVINGWALKWAY." ) == 0 )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_MOVINGWALKWAY;
	}
	else if( _stricmp( arg.c_str(), ".CRANEWAY." ) == 0 )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_CRANEWAY;
	}
	else if( _stricmp( arg.c_str(), ".LIFTINGGEAR." ) == 0 )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_LIFTINGGEAR;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

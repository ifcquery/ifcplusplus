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
#include "include/IfcRampFlightTypeEnum.h"

// TYPE IfcRampFlightTypeEnum 
IfcRampFlightTypeEnum::IfcRampFlightTypeEnum() {}
IfcRampFlightTypeEnum::~IfcRampFlightTypeEnum() {}
void IfcRampFlightTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCRAMPFLIGHTTYPEENUM("; }
	if( m_enum == ENUM_STRAIGHT )
	{
		stream << ".STRAIGHT.";
	}
	else if( m_enum == ENUM_SPIRAL )
	{
		stream << ".SPIRAL.";
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
shared_ptr<IfcRampFlightTypeEnum> IfcRampFlightTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcRampFlightTypeEnum>(); }
	shared_ptr<IfcRampFlightTypeEnum> type_object( new IfcRampFlightTypeEnum() );
	if( _stricmp( arg.c_str(), ".STRAIGHT." ) == 0 )
	{
		type_object->m_enum = IfcRampFlightTypeEnum::ENUM_STRAIGHT;
	}
	else if( _stricmp( arg.c_str(), ".SPIRAL." ) == 0 )
	{
		type_object->m_enum = IfcRampFlightTypeEnum::ENUM_SPIRAL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcRampFlightTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcRampFlightTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

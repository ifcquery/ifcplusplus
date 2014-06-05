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
#include "include/IfcActuatorTypeEnum.h"

// TYPE IfcActuatorTypeEnum 
IfcActuatorTypeEnum::IfcActuatorTypeEnum() {}
IfcActuatorTypeEnum::~IfcActuatorTypeEnum() {}
void IfcActuatorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCACTUATORTYPEENUM("; }
	if( m_enum == ENUM_ELECTRICACTUATOR )
	{
		stream << ".ELECTRICACTUATOR.";
	}
	else if( m_enum == ENUM_HANDOPERATEDACTUATOR )
	{
		stream << ".HANDOPERATEDACTUATOR.";
	}
	else if( m_enum == ENUM_HYDRAULICACTUATOR )
	{
		stream << ".HYDRAULICACTUATOR.";
	}
	else if( m_enum == ENUM_PNEUMATICACTUATOR )
	{
		stream << ".PNEUMATICACTUATOR.";
	}
	else if( m_enum == ENUM_THERMOSTATICACTUATOR )
	{
		stream << ".THERMOSTATICACTUATOR.";
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
shared_ptr<IfcActuatorTypeEnum> IfcActuatorTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcActuatorTypeEnum>(); }
	shared_ptr<IfcActuatorTypeEnum> type_object( new IfcActuatorTypeEnum() );
	if( _stricmp( arg.c_str(), ".ELECTRICACTUATOR." ) == 0 )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_ELECTRICACTUATOR;
	}
	else if( _stricmp( arg.c_str(), ".HANDOPERATEDACTUATOR." ) == 0 )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_HANDOPERATEDACTUATOR;
	}
	else if( _stricmp( arg.c_str(), ".HYDRAULICACTUATOR." ) == 0 )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_HYDRAULICACTUATOR;
	}
	else if( _stricmp( arg.c_str(), ".PNEUMATICACTUATOR." ) == 0 )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_PNEUMATICACTUATOR;
	}
	else if( _stricmp( arg.c_str(), ".THERMOSTATICACTUATOR." ) == 0 )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_THERMOSTATICACTUATOR;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

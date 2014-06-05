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
#include "include/IfcProtectiveDeviceTypeEnum.h"

// TYPE IfcProtectiveDeviceTypeEnum 
IfcProtectiveDeviceTypeEnum::IfcProtectiveDeviceTypeEnum() {}
IfcProtectiveDeviceTypeEnum::~IfcProtectiveDeviceTypeEnum() {}
void IfcProtectiveDeviceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROTECTIVEDEVICETYPEENUM("; }
	if( m_enum == ENUM_CIRCUITBREAKER )
	{
		stream << ".CIRCUITBREAKER.";
	}
	else if( m_enum == ENUM_EARTHLEAKAGECIRCUITBREAKER )
	{
		stream << ".EARTHLEAKAGECIRCUITBREAKER.";
	}
	else if( m_enum == ENUM_EARTHINGSWITCH )
	{
		stream << ".EARTHINGSWITCH.";
	}
	else if( m_enum == ENUM_FUSEDISCONNECTOR )
	{
		stream << ".FUSEDISCONNECTOR.";
	}
	else if( m_enum == ENUM_RESIDUALCURRENTCIRCUITBREAKER )
	{
		stream << ".RESIDUALCURRENTCIRCUITBREAKER.";
	}
	else if( m_enum == ENUM_RESIDUALCURRENTSWITCH )
	{
		stream << ".RESIDUALCURRENTSWITCH.";
	}
	else if( m_enum == ENUM_VARISTOR )
	{
		stream << ".VARISTOR.";
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
shared_ptr<IfcProtectiveDeviceTypeEnum> IfcProtectiveDeviceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTypeEnum>(); }
	shared_ptr<IfcProtectiveDeviceTypeEnum> type_object( new IfcProtectiveDeviceTypeEnum() );
	if( _stricmp( arg.c_str(), ".CIRCUITBREAKER." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_CIRCUITBREAKER;
	}
	else if( _stricmp( arg.c_str(), ".EARTHLEAKAGECIRCUITBREAKER." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_EARTHLEAKAGECIRCUITBREAKER;
	}
	else if( _stricmp( arg.c_str(), ".EARTHINGSWITCH." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_EARTHINGSWITCH;
	}
	else if( _stricmp( arg.c_str(), ".FUSEDISCONNECTOR." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_FUSEDISCONNECTOR;
	}
	else if( _stricmp( arg.c_str(), ".RESIDUALCURRENTCIRCUITBREAKER." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_RESIDUALCURRENTCIRCUITBREAKER;
	}
	else if( _stricmp( arg.c_str(), ".RESIDUALCURRENTSWITCH." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_RESIDUALCURRENTSWITCH;
	}
	else if( _stricmp( arg.c_str(), ".VARISTOR." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_VARISTOR;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

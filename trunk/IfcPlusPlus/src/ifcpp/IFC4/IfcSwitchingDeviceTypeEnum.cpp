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
#include "include/IfcSwitchingDeviceTypeEnum.h"

// TYPE IfcSwitchingDeviceTypeEnum 
IfcSwitchingDeviceTypeEnum::IfcSwitchingDeviceTypeEnum() {}
IfcSwitchingDeviceTypeEnum::~IfcSwitchingDeviceTypeEnum() {}
void IfcSwitchingDeviceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSWITCHINGDEVICETYPEENUM("; }
	if( m_enum == ENUM_CONTACTOR )
	{
		stream << ".CONTACTOR.";
	}
	else if( m_enum == ENUM_DIMMERSWITCH )
	{
		stream << ".DIMMERSWITCH.";
	}
	else if( m_enum == ENUM_EMERGENCYSTOP )
	{
		stream << ".EMERGENCYSTOP.";
	}
	else if( m_enum == ENUM_KEYPAD )
	{
		stream << ".KEYPAD.";
	}
	else if( m_enum == ENUM_MOMENTARYSWITCH )
	{
		stream << ".MOMENTARYSWITCH.";
	}
	else if( m_enum == ENUM_SELECTORSWITCH )
	{
		stream << ".SELECTORSWITCH.";
	}
	else if( m_enum == ENUM_STARTER )
	{
		stream << ".STARTER.";
	}
	else if( m_enum == ENUM_SWITCHDISCONNECTOR )
	{
		stream << ".SWITCHDISCONNECTOR.";
	}
	else if( m_enum == ENUM_TOGGLESWITCH )
	{
		stream << ".TOGGLESWITCH.";
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
shared_ptr<IfcSwitchingDeviceTypeEnum> IfcSwitchingDeviceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcSwitchingDeviceTypeEnum>(); }
	shared_ptr<IfcSwitchingDeviceTypeEnum> type_object( new IfcSwitchingDeviceTypeEnum() );
	if( _stricmp( arg.c_str(), ".CONTACTOR." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_CONTACTOR;
	}
	else if( _stricmp( arg.c_str(), ".DIMMERSWITCH." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_DIMMERSWITCH;
	}
	else if( _stricmp( arg.c_str(), ".EMERGENCYSTOP." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_EMERGENCYSTOP;
	}
	else if( _stricmp( arg.c_str(), ".KEYPAD." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_KEYPAD;
	}
	else if( _stricmp( arg.c_str(), ".MOMENTARYSWITCH." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_MOMENTARYSWITCH;
	}
	else if( _stricmp( arg.c_str(), ".SELECTORSWITCH." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_SELECTORSWITCH;
	}
	else if( _stricmp( arg.c_str(), ".STARTER." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_STARTER;
	}
	else if( _stricmp( arg.c_str(), ".SWITCHDISCONNECTOR." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_SWITCHDISCONNECTOR;
	}
	else if( _stricmp( arg.c_str(), ".TOGGLESWITCH." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_TOGGLESWITCH;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

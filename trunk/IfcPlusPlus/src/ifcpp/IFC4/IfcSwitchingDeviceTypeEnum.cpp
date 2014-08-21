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

// TYPE IfcSwitchingDeviceTypeEnum = ENUMERATION OF	(CONTACTOR	,DIMMERSWITCH	,EMERGENCYSTOP	,KEYPAD	,MOMENTARYSWITCH	,SELECTORSWITCH	,STARTER	,SWITCHDISCONNECTOR	,TOGGLESWITCH	,USERDEFINED	,NOTDEFINED);
IfcSwitchingDeviceTypeEnum::IfcSwitchingDeviceTypeEnum() {}
IfcSwitchingDeviceTypeEnum::~IfcSwitchingDeviceTypeEnum() {}
shared_ptr<IfcPPObject> IfcSwitchingDeviceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSwitchingDeviceTypeEnum> copy_self( new IfcSwitchingDeviceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcSwitchingDeviceTypeEnum> IfcSwitchingDeviceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSwitchingDeviceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSwitchingDeviceTypeEnum>(); }
	shared_ptr<IfcSwitchingDeviceTypeEnum> type_object( new IfcSwitchingDeviceTypeEnum() );
	if( boost::iequals( arg, L".CONTACTOR." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_CONTACTOR;
	}
	else if( boost::iequals( arg, L".DIMMERSWITCH." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_DIMMERSWITCH;
	}
	else if( boost::iequals( arg, L".EMERGENCYSTOP." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_EMERGENCYSTOP;
	}
	else if( boost::iequals( arg, L".KEYPAD." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_KEYPAD;
	}
	else if( boost::iequals( arg, L".MOMENTARYSWITCH." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_MOMENTARYSWITCH;
	}
	else if( boost::iequals( arg, L".SELECTORSWITCH." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_SELECTORSWITCH;
	}
	else if( boost::iequals( arg, L".STARTER." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_STARTER;
	}
	else if( boost::iequals( arg, L".SWITCHDISCONNECTOR." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_SWITCHDISCONNECTOR;
	}
	else if( boost::iequals( arg, L".TOGGLESWITCH." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_TOGGLESWITCH;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSwitchingDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

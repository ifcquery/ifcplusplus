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
shared_ptr<IfcPPObject> IfcProtectiveDeviceTypeEnum::getDeepCopy()
{
	shared_ptr<IfcProtectiveDeviceTypeEnum> copy_self( new IfcProtectiveDeviceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcProtectiveDeviceTypeEnum> IfcProtectiveDeviceTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTypeEnum>(); }
	shared_ptr<IfcProtectiveDeviceTypeEnum> type_object( new IfcProtectiveDeviceTypeEnum() );
	if( boost::iequals( arg, L".CIRCUITBREAKER." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_CIRCUITBREAKER;
	}
	else if( boost::iequals( arg, L".EARTHLEAKAGECIRCUITBREAKER." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_EARTHLEAKAGECIRCUITBREAKER;
	}
	else if( boost::iequals( arg, L".EARTHINGSWITCH." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_EARTHINGSWITCH;
	}
	else if( boost::iequals( arg, L".FUSEDISCONNECTOR." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_FUSEDISCONNECTOR;
	}
	else if( boost::iequals( arg, L".RESIDUALCURRENTCIRCUITBREAKER." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_RESIDUALCURRENTCIRCUITBREAKER;
	}
	else if( boost::iequals( arg, L".RESIDUALCURRENTSWITCH." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_RESIDUALCURRENTSWITCH;
	}
	else if( boost::iequals( arg, L".VARISTOR." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_VARISTOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

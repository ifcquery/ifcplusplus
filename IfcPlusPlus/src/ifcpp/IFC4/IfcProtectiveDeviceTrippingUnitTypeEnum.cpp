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
#include "include/IfcProtectiveDeviceTrippingUnitTypeEnum.h"

// TYPE IfcProtectiveDeviceTrippingUnitTypeEnum 
IfcProtectiveDeviceTrippingUnitTypeEnum::IfcProtectiveDeviceTrippingUnitTypeEnum() {}
IfcProtectiveDeviceTrippingUnitTypeEnum::~IfcProtectiveDeviceTrippingUnitTypeEnum() {}
shared_ptr<IfcPPObject> IfcProtectiveDeviceTrippingUnitTypeEnum::getDeepCopy()
{
	shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum> copy_self( new IfcProtectiveDeviceTrippingUnitTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcProtectiveDeviceTrippingUnitTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROTECTIVEDEVICETRIPPINGUNITTYPEENUM("; }
	if( m_enum == ENUM_ELECTRONIC )
	{
		stream << ".ELECTRONIC.";
	}
	else if( m_enum == ENUM_ELECTROMAGNETIC )
	{
		stream << ".ELECTROMAGNETIC.";
	}
	else if( m_enum == ENUM_RESIDUALCURRENT )
	{
		stream << ".RESIDUALCURRENT.";
	}
	else if( m_enum == ENUM_THERMAL )
	{
		stream << ".THERMAL.";
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
shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum> IfcProtectiveDeviceTrippingUnitTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum>(); }
	shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum> type_object( new IfcProtectiveDeviceTrippingUnitTypeEnum() );
	if( boost::iequals( arg, L".ELECTRONIC." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_ELECTRONIC;
	}
	else if( boost::iequals( arg, L".ELECTROMAGNETIC." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_ELECTROMAGNETIC;
	}
	else if( boost::iequals( arg, L".RESIDUALCURRENT." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_RESIDUALCURRENT;
	}
	else if( boost::iequals( arg, L".THERMAL." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_THERMAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

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
#include "include/IfcElectricFlowStorageDeviceTypeEnum.h"

// TYPE IfcElectricFlowStorageDeviceTypeEnum 
IfcElectricFlowStorageDeviceTypeEnum::IfcElectricFlowStorageDeviceTypeEnum() {}
IfcElectricFlowStorageDeviceTypeEnum::~IfcElectricFlowStorageDeviceTypeEnum() {}
shared_ptr<IfcPPObject> IfcElectricFlowStorageDeviceTypeEnum::getDeepCopy()
{
	shared_ptr<IfcElectricFlowStorageDeviceTypeEnum> copy_self( new IfcElectricFlowStorageDeviceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcElectricFlowStorageDeviceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICFLOWSTORAGEDEVICETYPEENUM("; }
	if( m_enum == ENUM_BATTERY )
	{
		stream << ".BATTERY.";
	}
	else if( m_enum == ENUM_CAPACITORBANK )
	{
		stream << ".CAPACITORBANK.";
	}
	else if( m_enum == ENUM_HARMONICFILTER )
	{
		stream << ".HARMONICFILTER.";
	}
	else if( m_enum == ENUM_INDUCTORBANK )
	{
		stream << ".INDUCTORBANK.";
	}
	else if( m_enum == ENUM_UPS )
	{
		stream << ".UPS.";
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
shared_ptr<IfcElectricFlowStorageDeviceTypeEnum> IfcElectricFlowStorageDeviceTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElectricFlowStorageDeviceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElectricFlowStorageDeviceTypeEnum>(); }
	shared_ptr<IfcElectricFlowStorageDeviceTypeEnum> type_object( new IfcElectricFlowStorageDeviceTypeEnum() );
	if( boost::iequals( arg, L".BATTERY." ) )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_BATTERY;
	}
	else if( boost::iequals( arg, L".CAPACITORBANK." ) )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_CAPACITORBANK;
	}
	else if( boost::iequals( arg, L".HARMONICFILTER." ) )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_HARMONICFILTER;
	}
	else if( boost::iequals( arg, L".INDUCTORBANK." ) )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_INDUCTORBANK;
	}
	else if( boost::iequals( arg, L".UPS." ) )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_UPS;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

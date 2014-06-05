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
shared_ptr<IfcElectricFlowStorageDeviceTypeEnum> IfcElectricFlowStorageDeviceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcElectricFlowStorageDeviceTypeEnum>(); }
	shared_ptr<IfcElectricFlowStorageDeviceTypeEnum> type_object( new IfcElectricFlowStorageDeviceTypeEnum() );
	if( _stricmp( arg.c_str(), ".BATTERY." ) == 0 )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_BATTERY;
	}
	else if( _stricmp( arg.c_str(), ".CAPACITORBANK." ) == 0 )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_CAPACITORBANK;
	}
	else if( _stricmp( arg.c_str(), ".HARMONICFILTER." ) == 0 )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_HARMONICFILTER;
	}
	else if( _stricmp( arg.c_str(), ".INDUCTORBANK." ) == 0 )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_INDUCTORBANK;
	}
	else if( _stricmp( arg.c_str(), ".UPS." ) == 0 )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_UPS;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcElectricFlowStorageDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

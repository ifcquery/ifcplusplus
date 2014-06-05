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
#include "include/IfcElectricApplianceTypeEnum.h"

// TYPE IfcElectricApplianceTypeEnum 
IfcElectricApplianceTypeEnum::IfcElectricApplianceTypeEnum() {}
IfcElectricApplianceTypeEnum::~IfcElectricApplianceTypeEnum() {}
void IfcElectricApplianceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICAPPLIANCETYPEENUM("; }
	if( m_enum == ENUM_DISHWASHER )
	{
		stream << ".DISHWASHER.";
	}
	else if( m_enum == ENUM_ELECTRICCOOKER )
	{
		stream << ".ELECTRICCOOKER.";
	}
	else if( m_enum == ENUM_FREESTANDINGELECTRICHEATER )
	{
		stream << ".FREESTANDINGELECTRICHEATER.";
	}
	else if( m_enum == ENUM_FREESTANDINGFAN )
	{
		stream << ".FREESTANDINGFAN.";
	}
	else if( m_enum == ENUM_FREESTANDINGWATERHEATER )
	{
		stream << ".FREESTANDINGWATERHEATER.";
	}
	else if( m_enum == ENUM_FREESTANDINGWATERCOOLER )
	{
		stream << ".FREESTANDINGWATERCOOLER.";
	}
	else if( m_enum == ENUM_FREEZER )
	{
		stream << ".FREEZER.";
	}
	else if( m_enum == ENUM_FRIDGE_FREEZER )
	{
		stream << ".FRIDGE_FREEZER.";
	}
	else if( m_enum == ENUM_HANDDRYER )
	{
		stream << ".HANDDRYER.";
	}
	else if( m_enum == ENUM_KITCHENMACHINE )
	{
		stream << ".KITCHENMACHINE.";
	}
	else if( m_enum == ENUM_MICROWAVE )
	{
		stream << ".MICROWAVE.";
	}
	else if( m_enum == ENUM_PHOTOCOPIER )
	{
		stream << ".PHOTOCOPIER.";
	}
	else if( m_enum == ENUM_REFRIGERATOR )
	{
		stream << ".REFRIGERATOR.";
	}
	else if( m_enum == ENUM_TUMBLEDRYER )
	{
		stream << ".TUMBLEDRYER.";
	}
	else if( m_enum == ENUM_VENDINGMACHINE )
	{
		stream << ".VENDINGMACHINE.";
	}
	else if( m_enum == ENUM_WASHINGMACHINE )
	{
		stream << ".WASHINGMACHINE.";
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
shared_ptr<IfcElectricApplianceTypeEnum> IfcElectricApplianceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcElectricApplianceTypeEnum>(); }
	shared_ptr<IfcElectricApplianceTypeEnum> type_object( new IfcElectricApplianceTypeEnum() );
	if( _stricmp( arg.c_str(), ".DISHWASHER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_DISHWASHER;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICCOOKER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_ELECTRICCOOKER;
	}
	else if( _stricmp( arg.c_str(), ".FREESTANDINGELECTRICHEATER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREESTANDINGELECTRICHEATER;
	}
	else if( _stricmp( arg.c_str(), ".FREESTANDINGFAN." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREESTANDINGFAN;
	}
	else if( _stricmp( arg.c_str(), ".FREESTANDINGWATERHEATER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREESTANDINGWATERHEATER;
	}
	else if( _stricmp( arg.c_str(), ".FREESTANDINGWATERCOOLER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREESTANDINGWATERCOOLER;
	}
	else if( _stricmp( arg.c_str(), ".FREEZER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREEZER;
	}
	else if( _stricmp( arg.c_str(), ".FRIDGE_FREEZER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FRIDGE_FREEZER;
	}
	else if( _stricmp( arg.c_str(), ".HANDDRYER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_HANDDRYER;
	}
	else if( _stricmp( arg.c_str(), ".KITCHENMACHINE." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_KITCHENMACHINE;
	}
	else if( _stricmp( arg.c_str(), ".MICROWAVE." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_MICROWAVE;
	}
	else if( _stricmp( arg.c_str(), ".PHOTOCOPIER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_PHOTOCOPIER;
	}
	else if( _stricmp( arg.c_str(), ".REFRIGERATOR." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_REFRIGERATOR;
	}
	else if( _stricmp( arg.c_str(), ".TUMBLEDRYER." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_TUMBLEDRYER;
	}
	else if( _stricmp( arg.c_str(), ".VENDINGMACHINE." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_VENDINGMACHINE;
	}
	else if( _stricmp( arg.c_str(), ".WASHINGMACHINE." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_WASHINGMACHINE;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

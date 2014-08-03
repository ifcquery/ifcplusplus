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
shared_ptr<IfcPPObject> IfcElectricApplianceTypeEnum::getDeepCopy()
{
	shared_ptr<IfcElectricApplianceTypeEnum> copy_self( new IfcElectricApplianceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcElectricApplianceTypeEnum> IfcElectricApplianceTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElectricApplianceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElectricApplianceTypeEnum>(); }
	shared_ptr<IfcElectricApplianceTypeEnum> type_object( new IfcElectricApplianceTypeEnum() );
	if( boost::iequals( arg, L".DISHWASHER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_DISHWASHER;
	}
	else if( boost::iequals( arg, L".ELECTRICCOOKER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_ELECTRICCOOKER;
	}
	else if( boost::iequals( arg, L".FREESTANDINGELECTRICHEATER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREESTANDINGELECTRICHEATER;
	}
	else if( boost::iequals( arg, L".FREESTANDINGFAN." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREESTANDINGFAN;
	}
	else if( boost::iequals( arg, L".FREESTANDINGWATERHEATER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREESTANDINGWATERHEATER;
	}
	else if( boost::iequals( arg, L".FREESTANDINGWATERCOOLER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREESTANDINGWATERCOOLER;
	}
	else if( boost::iequals( arg, L".FREEZER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FREEZER;
	}
	else if( boost::iequals( arg, L".FRIDGE_FREEZER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_FRIDGE_FREEZER;
	}
	else if( boost::iequals( arg, L".HANDDRYER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_HANDDRYER;
	}
	else if( boost::iequals( arg, L".KITCHENMACHINE." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_KITCHENMACHINE;
	}
	else if( boost::iequals( arg, L".MICROWAVE." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_MICROWAVE;
	}
	else if( boost::iequals( arg, L".PHOTOCOPIER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_PHOTOCOPIER;
	}
	else if( boost::iequals( arg, L".REFRIGERATOR." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_REFRIGERATOR;
	}
	else if( boost::iequals( arg, L".TUMBLEDRYER." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_TUMBLEDRYER;
	}
	else if( boost::iequals( arg, L".VENDINGMACHINE." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_VENDINGMACHINE;
	}
	else if( boost::iequals( arg, L".WASHINGMACHINE." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_WASHINGMACHINE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcElectricApplianceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

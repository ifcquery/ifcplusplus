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
#include "include/IfcConstructionEquipmentResourceTypeEnum.h"

// TYPE IfcConstructionEquipmentResourceTypeEnum 
IfcConstructionEquipmentResourceTypeEnum::IfcConstructionEquipmentResourceTypeEnum() {}
IfcConstructionEquipmentResourceTypeEnum::~IfcConstructionEquipmentResourceTypeEnum() {}
shared_ptr<IfcPPObject> IfcConstructionEquipmentResourceTypeEnum::getDeepCopy()
{
	shared_ptr<IfcConstructionEquipmentResourceTypeEnum> copy_self( new IfcConstructionEquipmentResourceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcConstructionEquipmentResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONSTRUCTIONEQUIPMENTRESOURCETYPEENUM("; }
	if( m_enum == ENUM_DEMOLISHING )
	{
		stream << ".DEMOLISHING.";
	}
	else if( m_enum == ENUM_EARTHMOVING )
	{
		stream << ".EARTHMOVING.";
	}
	else if( m_enum == ENUM_ERECTING )
	{
		stream << ".ERECTING.";
	}
	else if( m_enum == ENUM_HEATING )
	{
		stream << ".HEATING.";
	}
	else if( m_enum == ENUM_LIGHTING )
	{
		stream << ".LIGHTING.";
	}
	else if( m_enum == ENUM_PAVING )
	{
		stream << ".PAVING.";
	}
	else if( m_enum == ENUM_PUMPING )
	{
		stream << ".PUMPING.";
	}
	else if( m_enum == ENUM_TRANSPORTING )
	{
		stream << ".TRANSPORTING.";
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
shared_ptr<IfcConstructionEquipmentResourceTypeEnum> IfcConstructionEquipmentResourceTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcConstructionEquipmentResourceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcConstructionEquipmentResourceTypeEnum>(); }
	shared_ptr<IfcConstructionEquipmentResourceTypeEnum> type_object( new IfcConstructionEquipmentResourceTypeEnum() );
	if( boost::iequals( arg, L".DEMOLISHING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_DEMOLISHING;
	}
	else if( boost::iequals( arg, L".EARTHMOVING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_EARTHMOVING;
	}
	else if( boost::iequals( arg, L".ERECTING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_ERECTING;
	}
	else if( boost::iequals( arg, L".HEATING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_HEATING;
	}
	else if( boost::iequals( arg, L".LIGHTING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_LIGHTING;
	}
	else if( boost::iequals( arg, L".PAVING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_PAVING;
	}
	else if( boost::iequals( arg, L".PUMPING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_PUMPING;
	}
	else if( boost::iequals( arg, L".TRANSPORTING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_TRANSPORTING;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

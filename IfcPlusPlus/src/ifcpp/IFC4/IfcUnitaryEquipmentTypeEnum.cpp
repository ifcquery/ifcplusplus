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
#include "include/IfcUnitaryEquipmentTypeEnum.h"

// TYPE IfcUnitaryEquipmentTypeEnum = ENUMERATION OF	(AIRHANDLER	,AIRCONDITIONINGUNIT	,DEHUMIDIFIER	,SPLITSYSTEM	,ROOFTOPUNIT	,USERDEFINED	,NOTDEFINED);
IfcUnitaryEquipmentTypeEnum::IfcUnitaryEquipmentTypeEnum() {}
IfcUnitaryEquipmentTypeEnum::~IfcUnitaryEquipmentTypeEnum() {}
shared_ptr<IfcPPObject> IfcUnitaryEquipmentTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcUnitaryEquipmentTypeEnum> copy_self( new IfcUnitaryEquipmentTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcUnitaryEquipmentTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCUNITARYEQUIPMENTTYPEENUM("; }
	if( m_enum == ENUM_AIRHANDLER )
	{
		stream << ".AIRHANDLER.";
	}
	else if( m_enum == ENUM_AIRCONDITIONINGUNIT )
	{
		stream << ".AIRCONDITIONINGUNIT.";
	}
	else if( m_enum == ENUM_DEHUMIDIFIER )
	{
		stream << ".DEHUMIDIFIER.";
	}
	else if( m_enum == ENUM_SPLITSYSTEM )
	{
		stream << ".SPLITSYSTEM.";
	}
	else if( m_enum == ENUM_ROOFTOPUNIT )
	{
		stream << ".ROOFTOPUNIT.";
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
shared_ptr<IfcUnitaryEquipmentTypeEnum> IfcUnitaryEquipmentTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcUnitaryEquipmentTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcUnitaryEquipmentTypeEnum>(); }
	shared_ptr<IfcUnitaryEquipmentTypeEnum> type_object( new IfcUnitaryEquipmentTypeEnum() );
	if( boost::iequals( arg, L".AIRHANDLER." ) )
	{
		type_object->m_enum = IfcUnitaryEquipmentTypeEnum::ENUM_AIRHANDLER;
	}
	else if( boost::iequals( arg, L".AIRCONDITIONINGUNIT." ) )
	{
		type_object->m_enum = IfcUnitaryEquipmentTypeEnum::ENUM_AIRCONDITIONINGUNIT;
	}
	else if( boost::iequals( arg, L".DEHUMIDIFIER." ) )
	{
		type_object->m_enum = IfcUnitaryEquipmentTypeEnum::ENUM_DEHUMIDIFIER;
	}
	else if( boost::iequals( arg, L".SPLITSYSTEM." ) )
	{
		type_object->m_enum = IfcUnitaryEquipmentTypeEnum::ENUM_SPLITSYSTEM;
	}
	else if( boost::iequals( arg, L".ROOFTOPUNIT." ) )
	{
		type_object->m_enum = IfcUnitaryEquipmentTypeEnum::ENUM_ROOFTOPUNIT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcUnitaryEquipmentTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcUnitaryEquipmentTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

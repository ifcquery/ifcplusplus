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
#include "include/IfcUnitaryControlElementTypeEnum.h"

// TYPE IfcUnitaryControlElementTypeEnum 
IfcUnitaryControlElementTypeEnum::IfcUnitaryControlElementTypeEnum() {}
IfcUnitaryControlElementTypeEnum::~IfcUnitaryControlElementTypeEnum() {}
shared_ptr<IfcPPObject> IfcUnitaryControlElementTypeEnum::getDeepCopy()
{
	shared_ptr<IfcUnitaryControlElementTypeEnum> copy_self( new IfcUnitaryControlElementTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcUnitaryControlElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCUNITARYCONTROLELEMENTTYPEENUM("; }
	if( m_enum == ENUM_ALARMPANEL )
	{
		stream << ".ALARMPANEL.";
	}
	else if( m_enum == ENUM_CONTROLPANEL )
	{
		stream << ".CONTROLPANEL.";
	}
	else if( m_enum == ENUM_GASDETECTIONPANEL )
	{
		stream << ".GASDETECTIONPANEL.";
	}
	else if( m_enum == ENUM_INDICATORPANEL )
	{
		stream << ".INDICATORPANEL.";
	}
	else if( m_enum == ENUM_MIMICPANEL )
	{
		stream << ".MIMICPANEL.";
	}
	else if( m_enum == ENUM_HUMIDISTAT )
	{
		stream << ".HUMIDISTAT.";
	}
	else if( m_enum == ENUM_THERMOSTAT )
	{
		stream << ".THERMOSTAT.";
	}
	else if( m_enum == ENUM_WEATHERSTATION )
	{
		stream << ".WEATHERSTATION.";
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
shared_ptr<IfcUnitaryControlElementTypeEnum> IfcUnitaryControlElementTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcUnitaryControlElementTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcUnitaryControlElementTypeEnum>(); }
	shared_ptr<IfcUnitaryControlElementTypeEnum> type_object( new IfcUnitaryControlElementTypeEnum() );
	if( boost::iequals( arg, L".ALARMPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_ALARMPANEL;
	}
	else if( boost::iequals( arg, L".CONTROLPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_CONTROLPANEL;
	}
	else if( boost::iequals( arg, L".GASDETECTIONPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_GASDETECTIONPANEL;
	}
	else if( boost::iequals( arg, L".INDICATORPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_INDICATORPANEL;
	}
	else if( boost::iequals( arg, L".MIMICPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_MIMICPANEL;
	}
	else if( boost::iequals( arg, L".HUMIDISTAT." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_HUMIDISTAT;
	}
	else if( boost::iequals( arg, L".THERMOSTAT." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_THERMOSTAT;
	}
	else if( boost::iequals( arg, L".WEATHERSTATION." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_WEATHERSTATION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

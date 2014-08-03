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
#include "include/IfcTankTypeEnum.h"

// TYPE IfcTankTypeEnum 
IfcTankTypeEnum::IfcTankTypeEnum() {}
IfcTankTypeEnum::~IfcTankTypeEnum() {}
shared_ptr<IfcPPObject> IfcTankTypeEnum::getDeepCopy()
{
	shared_ptr<IfcTankTypeEnum> copy_self( new IfcTankTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTankTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTANKTYPEENUM("; }
	if( m_enum == ENUM_BASIN )
	{
		stream << ".BASIN.";
	}
	else if( m_enum == ENUM_BREAKPRESSURE )
	{
		stream << ".BREAKPRESSURE.";
	}
	else if( m_enum == ENUM_EXPANSION )
	{
		stream << ".EXPANSION.";
	}
	else if( m_enum == ENUM_FEEDANDEXPANSION )
	{
		stream << ".FEEDANDEXPANSION.";
	}
	else if( m_enum == ENUM_PRESSUREVESSEL )
	{
		stream << ".PRESSUREVESSEL.";
	}
	else if( m_enum == ENUM_STORAGE )
	{
		stream << ".STORAGE.";
	}
	else if( m_enum == ENUM_VESSEL )
	{
		stream << ".VESSEL.";
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
shared_ptr<IfcTankTypeEnum> IfcTankTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTankTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTankTypeEnum>(); }
	shared_ptr<IfcTankTypeEnum> type_object( new IfcTankTypeEnum() );
	if( boost::iequals( arg, L".BASIN." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_BASIN;
	}
	else if( boost::iequals( arg, L".BREAKPRESSURE." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_BREAKPRESSURE;
	}
	else if( boost::iequals( arg, L".EXPANSION." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_EXPANSION;
	}
	else if( boost::iequals( arg, L".FEEDANDEXPANSION." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_FEEDANDEXPANSION;
	}
	else if( boost::iequals( arg, L".PRESSUREVESSEL." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_PRESSUREVESSEL;
	}
	else if( boost::iequals( arg, L".STORAGE." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_STORAGE;
	}
	else if( boost::iequals( arg, L".VESSEL." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_VESSEL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

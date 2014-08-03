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
#include "include/IfcBuildingSystemTypeEnum.h"

// TYPE IfcBuildingSystemTypeEnum 
IfcBuildingSystemTypeEnum::IfcBuildingSystemTypeEnum() {}
IfcBuildingSystemTypeEnum::~IfcBuildingSystemTypeEnum() {}
shared_ptr<IfcPPObject> IfcBuildingSystemTypeEnum::getDeepCopy()
{
	shared_ptr<IfcBuildingSystemTypeEnum> copy_self( new IfcBuildingSystemTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBuildingSystemTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBUILDINGSYSTEMTYPEENUM("; }
	if( m_enum == ENUM_FENESTRATION )
	{
		stream << ".FENESTRATION.";
	}
	else if( m_enum == ENUM_FOUNDATION )
	{
		stream << ".FOUNDATION.";
	}
	else if( m_enum == ENUM_LOADBEARING )
	{
		stream << ".LOADBEARING.";
	}
	else if( m_enum == ENUM_OUTERSHELL )
	{
		stream << ".OUTERSHELL.";
	}
	else if( m_enum == ENUM_SHADING )
	{
		stream << ".SHADING.";
	}
	else if( m_enum == ENUM_TRANSPORT )
	{
		stream << ".TRANSPORT.";
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
shared_ptr<IfcBuildingSystemTypeEnum> IfcBuildingSystemTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBuildingSystemTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBuildingSystemTypeEnum>(); }
	shared_ptr<IfcBuildingSystemTypeEnum> type_object( new IfcBuildingSystemTypeEnum() );
	if( boost::iequals( arg, L".FENESTRATION." ) )
	{
		type_object->m_enum = IfcBuildingSystemTypeEnum::ENUM_FENESTRATION;
	}
	else if( boost::iequals( arg, L".FOUNDATION." ) )
	{
		type_object->m_enum = IfcBuildingSystemTypeEnum::ENUM_FOUNDATION;
	}
	else if( boost::iequals( arg, L".LOADBEARING." ) )
	{
		type_object->m_enum = IfcBuildingSystemTypeEnum::ENUM_LOADBEARING;
	}
	else if( boost::iequals( arg, L".OUTERSHELL." ) )
	{
		type_object->m_enum = IfcBuildingSystemTypeEnum::ENUM_OUTERSHELL;
	}
	else if( boost::iequals( arg, L".SHADING." ) )
	{
		type_object->m_enum = IfcBuildingSystemTypeEnum::ENUM_SHADING;
	}
	else if( boost::iequals( arg, L".TRANSPORT." ) )
	{
		type_object->m_enum = IfcBuildingSystemTypeEnum::ENUM_TRANSPORT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcBuildingSystemTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcBuildingSystemTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

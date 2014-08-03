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
#include "include/IfcCrewResourceTypeEnum.h"

// TYPE IfcCrewResourceTypeEnum 
IfcCrewResourceTypeEnum::IfcCrewResourceTypeEnum() {}
IfcCrewResourceTypeEnum::~IfcCrewResourceTypeEnum() {}
shared_ptr<IfcPPObject> IfcCrewResourceTypeEnum::getDeepCopy()
{
	shared_ptr<IfcCrewResourceTypeEnum> copy_self( new IfcCrewResourceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCrewResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCREWRESOURCETYPEENUM("; }
	if( m_enum == ENUM_OFFICE )
	{
		stream << ".OFFICE.";
	}
	else if( m_enum == ENUM_SITE )
	{
		stream << ".SITE.";
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
shared_ptr<IfcCrewResourceTypeEnum> IfcCrewResourceTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCrewResourceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCrewResourceTypeEnum>(); }
	shared_ptr<IfcCrewResourceTypeEnum> type_object( new IfcCrewResourceTypeEnum() );
	if( boost::iequals( arg, L".OFFICE." ) )
	{
		type_object->m_enum = IfcCrewResourceTypeEnum::ENUM_OFFICE;
	}
	else if( boost::iequals( arg, L".SITE." ) )
	{
		type_object->m_enum = IfcCrewResourceTypeEnum::ENUM_SITE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCrewResourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCrewResourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

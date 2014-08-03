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
#include "include/IfcOpeningElementTypeEnum.h"

// TYPE IfcOpeningElementTypeEnum 
IfcOpeningElementTypeEnum::IfcOpeningElementTypeEnum() {}
IfcOpeningElementTypeEnum::~IfcOpeningElementTypeEnum() {}
shared_ptr<IfcPPObject> IfcOpeningElementTypeEnum::getDeepCopy()
{
	shared_ptr<IfcOpeningElementTypeEnum> copy_self( new IfcOpeningElementTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcOpeningElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOPENINGELEMENTTYPEENUM("; }
	if( m_enum == ENUM_OPENING )
	{
		stream << ".OPENING.";
	}
	else if( m_enum == ENUM_RECESS )
	{
		stream << ".RECESS.";
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
shared_ptr<IfcOpeningElementTypeEnum> IfcOpeningElementTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcOpeningElementTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcOpeningElementTypeEnum>(); }
	shared_ptr<IfcOpeningElementTypeEnum> type_object( new IfcOpeningElementTypeEnum() );
	if( boost::iequals( arg, L".OPENING." ) )
	{
		type_object->m_enum = IfcOpeningElementTypeEnum::ENUM_OPENING;
	}
	else if( boost::iequals( arg, L".RECESS." ) )
	{
		type_object->m_enum = IfcOpeningElementTypeEnum::ENUM_RECESS;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcOpeningElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcOpeningElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

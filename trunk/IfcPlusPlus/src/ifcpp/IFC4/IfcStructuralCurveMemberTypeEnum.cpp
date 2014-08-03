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
#include "include/IfcStructuralCurveMemberTypeEnum.h"

// TYPE IfcStructuralCurveMemberTypeEnum 
IfcStructuralCurveMemberTypeEnum::IfcStructuralCurveMemberTypeEnum() {}
IfcStructuralCurveMemberTypeEnum::~IfcStructuralCurveMemberTypeEnum() {}
shared_ptr<IfcPPObject> IfcStructuralCurveMemberTypeEnum::getDeepCopy()
{
	shared_ptr<IfcStructuralCurveMemberTypeEnum> copy_self( new IfcStructuralCurveMemberTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcStructuralCurveMemberTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTRUCTURALCURVEMEMBERTYPEENUM("; }
	if( m_enum == ENUM_RIGID_JOINED_MEMBER )
	{
		stream << ".RIGID_JOINED_MEMBER.";
	}
	else if( m_enum == ENUM_PIN_JOINED_MEMBER )
	{
		stream << ".PIN_JOINED_MEMBER.";
	}
	else if( m_enum == ENUM_CABLE )
	{
		stream << ".CABLE.";
	}
	else if( m_enum == ENUM_TENSION_MEMBER )
	{
		stream << ".TENSION_MEMBER.";
	}
	else if( m_enum == ENUM_COMPRESSION_MEMBER )
	{
		stream << ".COMPRESSION_MEMBER.";
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
shared_ptr<IfcStructuralCurveMemberTypeEnum> IfcStructuralCurveMemberTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcStructuralCurveMemberTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcStructuralCurveMemberTypeEnum>(); }
	shared_ptr<IfcStructuralCurveMemberTypeEnum> type_object( new IfcStructuralCurveMemberTypeEnum() );
	if( boost::iequals( arg, L".RIGID_JOINED_MEMBER." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_RIGID_JOINED_MEMBER;
	}
	else if( boost::iequals( arg, L".PIN_JOINED_MEMBER." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_PIN_JOINED_MEMBER;
	}
	else if( boost::iequals( arg, L".CABLE." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_CABLE;
	}
	else if( boost::iequals( arg, L".TENSION_MEMBER." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_TENSION_MEMBER;
	}
	else if( boost::iequals( arg, L".COMPRESSION_MEMBER." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_COMPRESSION_MEMBER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

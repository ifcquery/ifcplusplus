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
#include "include/IfcMemberTypeEnum.h"

// TYPE IfcMemberTypeEnum = ENUMERATION OF	(BRACE	,CHORD	,COLLAR	,MEMBER	,MULLION	,PLATE	,POST	,PURLIN	,RAFTER	,STRINGER	,STRUT	,STUD	,USERDEFINED	,NOTDEFINED);
IfcMemberTypeEnum::IfcMemberTypeEnum() {}
IfcMemberTypeEnum::~IfcMemberTypeEnum() {}
shared_ptr<IfcPPObject> IfcMemberTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMemberTypeEnum> copy_self( new IfcMemberTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcMemberTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCMEMBERTYPEENUM("; }
	if( m_enum == ENUM_BRACE )
	{
		stream << ".BRACE.";
	}
	else if( m_enum == ENUM_CHORD )
	{
		stream << ".CHORD.";
	}
	else if( m_enum == ENUM_COLLAR )
	{
		stream << ".COLLAR.";
	}
	else if( m_enum == ENUM_MEMBER )
	{
		stream << ".MEMBER.";
	}
	else if( m_enum == ENUM_MULLION )
	{
		stream << ".MULLION.";
	}
	else if( m_enum == ENUM_PLATE )
	{
		stream << ".PLATE.";
	}
	else if( m_enum == ENUM_POST )
	{
		stream << ".POST.";
	}
	else if( m_enum == ENUM_PURLIN )
	{
		stream << ".PURLIN.";
	}
	else if( m_enum == ENUM_RAFTER )
	{
		stream << ".RAFTER.";
	}
	else if( m_enum == ENUM_STRINGER )
	{
		stream << ".STRINGER.";
	}
	else if( m_enum == ENUM_STRUT )
	{
		stream << ".STRUT.";
	}
	else if( m_enum == ENUM_STUD )
	{
		stream << ".STUD.";
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
shared_ptr<IfcMemberTypeEnum> IfcMemberTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcMemberTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcMemberTypeEnum>(); }
	shared_ptr<IfcMemberTypeEnum> type_object( new IfcMemberTypeEnum() );
	if( boost::iequals( arg, L".BRACE." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_BRACE;
	}
	else if( boost::iequals( arg, L".CHORD." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_CHORD;
	}
	else if( boost::iequals( arg, L".COLLAR." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_COLLAR;
	}
	else if( boost::iequals( arg, L".MEMBER." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_MEMBER;
	}
	else if( boost::iequals( arg, L".MULLION." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_MULLION;
	}
	else if( boost::iequals( arg, L".PLATE." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_PLATE;
	}
	else if( boost::iequals( arg, L".POST." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_POST;
	}
	else if( boost::iequals( arg, L".PURLIN." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_PURLIN;
	}
	else if( boost::iequals( arg, L".RAFTER." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_RAFTER;
	}
	else if( boost::iequals( arg, L".STRINGER." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_STRINGER;
	}
	else if( boost::iequals( arg, L".STRUT." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_STRUT;
	}
	else if( boost::iequals( arg, L".STUD." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_STUD;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcMemberTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

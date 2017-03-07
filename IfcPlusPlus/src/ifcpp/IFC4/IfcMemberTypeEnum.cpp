/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
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

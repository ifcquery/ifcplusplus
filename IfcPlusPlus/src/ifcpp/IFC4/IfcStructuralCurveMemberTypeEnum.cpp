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
#include "include/IfcStructuralCurveMemberTypeEnum.h"

// TYPE IfcStructuralCurveMemberTypeEnum = ENUMERATION OF	(RIGID_JOINED_MEMBER	,PIN_JOINED_MEMBER	,CABLE	,TENSION_MEMBER	,COMPRESSION_MEMBER	,USERDEFINED	,NOTDEFINED);
IfcStructuralCurveMemberTypeEnum::IfcStructuralCurveMemberTypeEnum() {}
IfcStructuralCurveMemberTypeEnum::~IfcStructuralCurveMemberTypeEnum() {}
shared_ptr<IfcPPObject> IfcStructuralCurveMemberTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
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
shared_ptr<IfcStructuralCurveMemberTypeEnum> IfcStructuralCurveMemberTypeEnum::createObjectFromSTEP( const std::wstring& arg )
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

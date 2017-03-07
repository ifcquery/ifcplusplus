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
#include "include/IfcActionRequestTypeEnum.h"

// TYPE IfcActionRequestTypeEnum = ENUMERATION OF	(EMAIL	,FAX	,PHONE	,POST	,VERBAL	,USERDEFINED	,NOTDEFINED);
IfcActionRequestTypeEnum::IfcActionRequestTypeEnum() {}
IfcActionRequestTypeEnum::~IfcActionRequestTypeEnum() {}
shared_ptr<IfcPPObject> IfcActionRequestTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcActionRequestTypeEnum> copy_self( new IfcActionRequestTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcActionRequestTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCACTIONREQUESTTYPEENUM("; }
	if( m_enum == ENUM_EMAIL )
	{
		stream << ".EMAIL.";
	}
	else if( m_enum == ENUM_FAX )
	{
		stream << ".FAX.";
	}
	else if( m_enum == ENUM_PHONE )
	{
		stream << ".PHONE.";
	}
	else if( m_enum == ENUM_POST )
	{
		stream << ".POST.";
	}
	else if( m_enum == ENUM_VERBAL )
	{
		stream << ".VERBAL.";
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
shared_ptr<IfcActionRequestTypeEnum> IfcActionRequestTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcActionRequestTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcActionRequestTypeEnum>(); }
	shared_ptr<IfcActionRequestTypeEnum> type_object( new IfcActionRequestTypeEnum() );
	if( boost::iequals( arg, L".EMAIL." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_EMAIL;
	}
	else if( boost::iequals( arg, L".FAX." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_FAX;
	}
	else if( boost::iequals( arg, L".PHONE." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_PHONE;
	}
	else if( boost::iequals( arg, L".POST." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_POST;
	}
	else if( boost::iequals( arg, L".VERBAL." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_VERBAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

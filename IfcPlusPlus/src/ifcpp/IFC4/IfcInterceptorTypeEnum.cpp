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
#include "include/IfcInterceptorTypeEnum.h"

// TYPE IfcInterceptorTypeEnum = ENUMERATION OF	(CYCLONIC	,GREASE	,OIL	,PETROL	,USERDEFINED	,NOTDEFINED);
IfcInterceptorTypeEnum::IfcInterceptorTypeEnum() {}
IfcInterceptorTypeEnum::~IfcInterceptorTypeEnum() {}
shared_ptr<IfcPPObject> IfcInterceptorTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcInterceptorTypeEnum> copy_self( new IfcInterceptorTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcInterceptorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCINTERCEPTORTYPEENUM("; }
	if( m_enum == ENUM_CYCLONIC )
	{
		stream << ".CYCLONIC.";
	}
	else if( m_enum == ENUM_GREASE )
	{
		stream << ".GREASE.";
	}
	else if( m_enum == ENUM_OIL )
	{
		stream << ".OIL.";
	}
	else if( m_enum == ENUM_PETROL )
	{
		stream << ".PETROL.";
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
shared_ptr<IfcInterceptorTypeEnum> IfcInterceptorTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcInterceptorTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcInterceptorTypeEnum>(); }
	shared_ptr<IfcInterceptorTypeEnum> type_object( new IfcInterceptorTypeEnum() );
	if( boost::iequals( arg, L".CYCLONIC." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_CYCLONIC;
	}
	else if( boost::iequals( arg, L".GREASE." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_GREASE;
	}
	else if( boost::iequals( arg, L".OIL." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_OIL;
	}
	else if( boost::iequals( arg, L".PETROL." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_PETROL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

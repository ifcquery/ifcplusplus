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
#include "include/IfcChangeActionEnum.h"

// TYPE IfcChangeActionEnum = ENUMERATION OF	(NOCHANGE	,MODIFIED	,ADDED	,DELETED	,NOTDEFINED);
IfcChangeActionEnum::IfcChangeActionEnum() {}
IfcChangeActionEnum::~IfcChangeActionEnum() {}
shared_ptr<IfcPPObject> IfcChangeActionEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcChangeActionEnum> copy_self( new IfcChangeActionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcChangeActionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCHANGEACTIONENUM("; }
	if( m_enum == ENUM_NOCHANGE )
	{
		stream << ".NOCHANGE.";
	}
	else if( m_enum == ENUM_MODIFIED )
	{
		stream << ".MODIFIED.";
	}
	else if( m_enum == ENUM_ADDED )
	{
		stream << ".ADDED.";
	}
	else if( m_enum == ENUM_DELETED )
	{
		stream << ".DELETED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcChangeActionEnum> IfcChangeActionEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcChangeActionEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcChangeActionEnum>(); }
	shared_ptr<IfcChangeActionEnum> type_object( new IfcChangeActionEnum() );
	if( boost::iequals( arg, L".NOCHANGE." ) )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_NOCHANGE;
	}
	else if( boost::iequals( arg, L".MODIFIED." ) )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_MODIFIED;
	}
	else if( boost::iequals( arg, L".ADDED." ) )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_ADDED;
	}
	else if( boost::iequals( arg, L".DELETED." ) )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_DELETED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcChangeActionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

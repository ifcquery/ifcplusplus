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
#include "include/IfcRailingTypeEnum.h"

// TYPE IfcRailingTypeEnum = ENUMERATION OF	(HANDRAIL	,GUARDRAIL	,BALUSTRADE	,USERDEFINED	,NOTDEFINED);
IfcRailingTypeEnum::IfcRailingTypeEnum() {}
IfcRailingTypeEnum::~IfcRailingTypeEnum() {}
shared_ptr<IfcPPObject> IfcRailingTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRailingTypeEnum> copy_self( new IfcRailingTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRailingTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCRAILINGTYPEENUM("; }
	if( m_enum == ENUM_HANDRAIL )
	{
		stream << ".HANDRAIL.";
	}
	else if( m_enum == ENUM_GUARDRAIL )
	{
		stream << ".GUARDRAIL.";
	}
	else if( m_enum == ENUM_BALUSTRADE )
	{
		stream << ".BALUSTRADE.";
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
shared_ptr<IfcRailingTypeEnum> IfcRailingTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRailingTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRailingTypeEnum>(); }
	shared_ptr<IfcRailingTypeEnum> type_object( new IfcRailingTypeEnum() );
	if( boost::iequals( arg, L".HANDRAIL." ) )
	{
		type_object->m_enum = IfcRailingTypeEnum::ENUM_HANDRAIL;
	}
	else if( boost::iequals( arg, L".GUARDRAIL." ) )
	{
		type_object->m_enum = IfcRailingTypeEnum::ENUM_GUARDRAIL;
	}
	else if( boost::iequals( arg, L".BALUSTRADE." ) )
	{
		type_object->m_enum = IfcRailingTypeEnum::ENUM_BALUSTRADE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRailingTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcRailingTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

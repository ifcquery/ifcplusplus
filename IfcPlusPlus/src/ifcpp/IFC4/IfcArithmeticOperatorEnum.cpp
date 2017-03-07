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
#include "include/IfcArithmeticOperatorEnum.h"

// TYPE IfcArithmeticOperatorEnum = ENUMERATION OF	(ADD	,DIVIDE	,MULTIPLY	,SUBTRACT);
IfcArithmeticOperatorEnum::IfcArithmeticOperatorEnum() {}
IfcArithmeticOperatorEnum::~IfcArithmeticOperatorEnum() {}
shared_ptr<IfcPPObject> IfcArithmeticOperatorEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcArithmeticOperatorEnum> copy_self( new IfcArithmeticOperatorEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcArithmeticOperatorEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCARITHMETICOPERATORENUM("; }
	if( m_enum == ENUM_ADD )
	{
		stream << ".ADD.";
	}
	else if( m_enum == ENUM_DIVIDE )
	{
		stream << ".DIVIDE.";
	}
	else if( m_enum == ENUM_MULTIPLY )
	{
		stream << ".MULTIPLY.";
	}
	else if( m_enum == ENUM_SUBTRACT )
	{
		stream << ".SUBTRACT.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcArithmeticOperatorEnum> IfcArithmeticOperatorEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcArithmeticOperatorEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcArithmeticOperatorEnum>(); }
	shared_ptr<IfcArithmeticOperatorEnum> type_object( new IfcArithmeticOperatorEnum() );
	if( boost::iequals( arg, L".ADD." ) )
	{
		type_object->m_enum = IfcArithmeticOperatorEnum::ENUM_ADD;
	}
	else if( boost::iequals( arg, L".DIVIDE." ) )
	{
		type_object->m_enum = IfcArithmeticOperatorEnum::ENUM_DIVIDE;
	}
	else if( boost::iequals( arg, L".MULTIPLY." ) )
	{
		type_object->m_enum = IfcArithmeticOperatorEnum::ENUM_MULTIPLY;
	}
	else if( boost::iequals( arg, L".SUBTRACT." ) )
	{
		type_object->m_enum = IfcArithmeticOperatorEnum::ENUM_SUBTRACT;
	}
	return type_object;
}

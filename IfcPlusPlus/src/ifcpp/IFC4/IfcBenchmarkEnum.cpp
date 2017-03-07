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
#include "include/IfcBenchmarkEnum.h"

// TYPE IfcBenchmarkEnum = ENUMERATION OF	(GREATERTHAN	,GREATERTHANOREQUALTO	,LESSTHAN	,LESSTHANOREQUALTO	,EQUALTO	,NOTEQUALTO	,INCLUDES	,NOTINCLUDES	,INCLUDEDIN	,NOTINCLUDEDIN);
IfcBenchmarkEnum::IfcBenchmarkEnum() {}
IfcBenchmarkEnum::~IfcBenchmarkEnum() {}
shared_ptr<IfcPPObject> IfcBenchmarkEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBenchmarkEnum> copy_self( new IfcBenchmarkEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBenchmarkEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBENCHMARKENUM("; }
	if( m_enum == ENUM_GREATERTHAN )
	{
		stream << ".GREATERTHAN.";
	}
	else if( m_enum == ENUM_GREATERTHANOREQUALTO )
	{
		stream << ".GREATERTHANOREQUALTO.";
	}
	else if( m_enum == ENUM_LESSTHAN )
	{
		stream << ".LESSTHAN.";
	}
	else if( m_enum == ENUM_LESSTHANOREQUALTO )
	{
		stream << ".LESSTHANOREQUALTO.";
	}
	else if( m_enum == ENUM_EQUALTO )
	{
		stream << ".EQUALTO.";
	}
	else if( m_enum == ENUM_NOTEQUALTO )
	{
		stream << ".NOTEQUALTO.";
	}
	else if( m_enum == ENUM_INCLUDES )
	{
		stream << ".INCLUDES.";
	}
	else if( m_enum == ENUM_NOTINCLUDES )
	{
		stream << ".NOTINCLUDES.";
	}
	else if( m_enum == ENUM_INCLUDEDIN )
	{
		stream << ".INCLUDEDIN.";
	}
	else if( m_enum == ENUM_NOTINCLUDEDIN )
	{
		stream << ".NOTINCLUDEDIN.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBenchmarkEnum> IfcBenchmarkEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBenchmarkEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBenchmarkEnum>(); }
	shared_ptr<IfcBenchmarkEnum> type_object( new IfcBenchmarkEnum() );
	if( boost::iequals( arg, L".GREATERTHAN." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_GREATERTHAN;
	}
	else if( boost::iequals( arg, L".GREATERTHANOREQUALTO." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_GREATERTHANOREQUALTO;
	}
	else if( boost::iequals( arg, L".LESSTHAN." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_LESSTHAN;
	}
	else if( boost::iequals( arg, L".LESSTHANOREQUALTO." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_LESSTHANOREQUALTO;
	}
	else if( boost::iequals( arg, L".EQUALTO." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_EQUALTO;
	}
	else if( boost::iequals( arg, L".NOTEQUALTO." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_NOTEQUALTO;
	}
	else if( boost::iequals( arg, L".INCLUDES." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_INCLUDES;
	}
	else if( boost::iequals( arg, L".NOTINCLUDES." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_NOTINCLUDES;
	}
	else if( boost::iequals( arg, L".INCLUDEDIN." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_INCLUDEDIN;
	}
	else if( boost::iequals( arg, L".NOTINCLUDEDIN." ) )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_NOTINCLUDEDIN;
	}
	return type_object;
}

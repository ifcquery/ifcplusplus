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
#include "include/IfcActionTypeEnum.h"

// TYPE IfcActionTypeEnum = ENUMERATION OF	(PERMANENT_G	,VARIABLE_Q	,EXTRAORDINARY_A	,USERDEFINED	,NOTDEFINED);
IfcActionTypeEnum::IfcActionTypeEnum() {}
IfcActionTypeEnum::~IfcActionTypeEnum() {}
shared_ptr<IfcPPObject> IfcActionTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcActionTypeEnum> copy_self( new IfcActionTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcActionTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCACTIONTYPEENUM("; }
	if( m_enum == ENUM_PERMANENT_G )
	{
		stream << ".PERMANENT_G.";
	}
	else if( m_enum == ENUM_VARIABLE_Q )
	{
		stream << ".VARIABLE_Q.";
	}
	else if( m_enum == ENUM_EXTRAORDINARY_A )
	{
		stream << ".EXTRAORDINARY_A.";
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
shared_ptr<IfcActionTypeEnum> IfcActionTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcActionTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcActionTypeEnum>(); }
	shared_ptr<IfcActionTypeEnum> type_object( new IfcActionTypeEnum() );
	if( boost::iequals( arg, L".PERMANENT_G." ) )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_PERMANENT_G;
	}
	else if( boost::iequals( arg, L".VARIABLE_Q." ) )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_VARIABLE_Q;
	}
	else if( boost::iequals( arg, L".EXTRAORDINARY_A." ) )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_EXTRAORDINARY_A;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcActionTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

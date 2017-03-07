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
#include "include/IfcFastenerTypeEnum.h"

// TYPE IfcFastenerTypeEnum = ENUMERATION OF	(GLUE	,MORTAR	,WELD	,USERDEFINED	,NOTDEFINED);
IfcFastenerTypeEnum::IfcFastenerTypeEnum() {}
IfcFastenerTypeEnum::~IfcFastenerTypeEnum() {}
shared_ptr<IfcPPObject> IfcFastenerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFastenerTypeEnum> copy_self( new IfcFastenerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFastenerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFASTENERTYPEENUM("; }
	if( m_enum == ENUM_GLUE )
	{
		stream << ".GLUE.";
	}
	else if( m_enum == ENUM_MORTAR )
	{
		stream << ".MORTAR.";
	}
	else if( m_enum == ENUM_WELD )
	{
		stream << ".WELD.";
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
shared_ptr<IfcFastenerTypeEnum> IfcFastenerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFastenerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFastenerTypeEnum>(); }
	shared_ptr<IfcFastenerTypeEnum> type_object( new IfcFastenerTypeEnum() );
	if( boost::iequals( arg, L".GLUE." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_GLUE;
	}
	else if( boost::iequals( arg, L".MORTAR." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_MORTAR;
	}
	else if( boost::iequals( arg, L".WELD." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_WELD;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

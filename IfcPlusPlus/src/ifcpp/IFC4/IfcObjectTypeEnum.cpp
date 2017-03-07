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
#include "include/IfcObjectTypeEnum.h"

// TYPE IfcObjectTypeEnum = ENUMERATION OF	(PRODUCT	,PROCESS	,CONTROL	,RESOURCE	,ACTOR	,GROUP	,PROJECT	,NOTDEFINED);
IfcObjectTypeEnum::IfcObjectTypeEnum() {}
IfcObjectTypeEnum::~IfcObjectTypeEnum() {}
shared_ptr<IfcPPObject> IfcObjectTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcObjectTypeEnum> copy_self( new IfcObjectTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcObjectTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOBJECTTYPEENUM("; }
	if( m_enum == ENUM_PRODUCT )
	{
		stream << ".PRODUCT.";
	}
	else if( m_enum == ENUM_PROCESS )
	{
		stream << ".PROCESS.";
	}
	else if( m_enum == ENUM_CONTROL )
	{
		stream << ".CONTROL.";
	}
	else if( m_enum == ENUM_RESOURCE )
	{
		stream << ".RESOURCE.";
	}
	else if( m_enum == ENUM_ACTOR )
	{
		stream << ".ACTOR.";
	}
	else if( m_enum == ENUM_GROUP )
	{
		stream << ".GROUP.";
	}
	else if( m_enum == ENUM_PROJECT )
	{
		stream << ".PROJECT.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcObjectTypeEnum> IfcObjectTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcObjectTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcObjectTypeEnum>(); }
	shared_ptr<IfcObjectTypeEnum> type_object( new IfcObjectTypeEnum() );
	if( boost::iequals( arg, L".PRODUCT." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PRODUCT;
	}
	else if( boost::iequals( arg, L".PROCESS." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PROCESS;
	}
	else if( boost::iequals( arg, L".CONTROL." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_CONTROL;
	}
	else if( boost::iequals( arg, L".RESOURCE." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_RESOURCE;
	}
	else if( boost::iequals( arg, L".ACTOR." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_ACTOR;
	}
	else if( boost::iequals( arg, L".GROUP." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_GROUP;
	}
	else if( boost::iequals( arg, L".PROJECT." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PROJECT;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

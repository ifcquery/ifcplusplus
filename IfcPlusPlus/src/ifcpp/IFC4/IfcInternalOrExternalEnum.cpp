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
#include "include/IfcInternalOrExternalEnum.h"

// TYPE IfcInternalOrExternalEnum = ENUMERATION OF	(INTERNAL	,EXTERNAL	,EXTERNAL_EARTH	,EXTERNAL_WATER	,EXTERNAL_FIRE	,NOTDEFINED);
IfcInternalOrExternalEnum::IfcInternalOrExternalEnum() {}
IfcInternalOrExternalEnum::~IfcInternalOrExternalEnum() {}
shared_ptr<IfcPPObject> IfcInternalOrExternalEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcInternalOrExternalEnum> copy_self( new IfcInternalOrExternalEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcInternalOrExternalEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCINTERNALOREXTERNALENUM("; }
	if( m_enum == ENUM_INTERNAL )
	{
		stream << ".INTERNAL.";
	}
	else if( m_enum == ENUM_EXTERNAL )
	{
		stream << ".EXTERNAL.";
	}
	else if( m_enum == ENUM_EXTERNAL_EARTH )
	{
		stream << ".EXTERNAL_EARTH.";
	}
	else if( m_enum == ENUM_EXTERNAL_WATER )
	{
		stream << ".EXTERNAL_WATER.";
	}
	else if( m_enum == ENUM_EXTERNAL_FIRE )
	{
		stream << ".EXTERNAL_FIRE.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcInternalOrExternalEnum> IfcInternalOrExternalEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcInternalOrExternalEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcInternalOrExternalEnum>(); }
	shared_ptr<IfcInternalOrExternalEnum> type_object( new IfcInternalOrExternalEnum() );
	if( boost::iequals( arg, L".INTERNAL." ) )
	{
		type_object->m_enum = IfcInternalOrExternalEnum::ENUM_INTERNAL;
	}
	else if( boost::iequals( arg, L".EXTERNAL." ) )
	{
		type_object->m_enum = IfcInternalOrExternalEnum::ENUM_EXTERNAL;
	}
	else if( boost::iequals( arg, L".EXTERNAL_EARTH." ) )
	{
		type_object->m_enum = IfcInternalOrExternalEnum::ENUM_EXTERNAL_EARTH;
	}
	else if( boost::iequals( arg, L".EXTERNAL_WATER." ) )
	{
		type_object->m_enum = IfcInternalOrExternalEnum::ENUM_EXTERNAL_WATER;
	}
	else if( boost::iequals( arg, L".EXTERNAL_FIRE." ) )
	{
		type_object->m_enum = IfcInternalOrExternalEnum::ENUM_EXTERNAL_FIRE;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcInternalOrExternalEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

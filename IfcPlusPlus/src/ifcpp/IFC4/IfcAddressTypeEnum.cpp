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
#include "include/IfcAddressTypeEnum.h"

// TYPE IfcAddressTypeEnum = ENUMERATION OF	(OFFICE	,SITE	,HOME	,DISTRIBUTIONPOINT	,USERDEFINED);
IfcAddressTypeEnum::IfcAddressTypeEnum() {}
IfcAddressTypeEnum::~IfcAddressTypeEnum() {}
shared_ptr<IfcPPObject> IfcAddressTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAddressTypeEnum> copy_self( new IfcAddressTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcAddressTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCADDRESSTYPEENUM("; }
	if( m_enum == ENUM_OFFICE )
	{
		stream << ".OFFICE.";
	}
	else if( m_enum == ENUM_SITE )
	{
		stream << ".SITE.";
	}
	else if( m_enum == ENUM_HOME )
	{
		stream << ".HOME.";
	}
	else if( m_enum == ENUM_DISTRIBUTIONPOINT )
	{
		stream << ".DISTRIBUTIONPOINT.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcAddressTypeEnum> IfcAddressTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcAddressTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcAddressTypeEnum>(); }
	shared_ptr<IfcAddressTypeEnum> type_object( new IfcAddressTypeEnum() );
	if( boost::iequals( arg, L".OFFICE." ) )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_OFFICE;
	}
	else if( boost::iequals( arg, L".SITE." ) )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_SITE;
	}
	else if( boost::iequals( arg, L".HOME." ) )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_HOME;
	}
	else if( boost::iequals( arg, L".DISTRIBUTIONPOINT." ) )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_DISTRIBUTIONPOINT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_USERDEFINED;
	}
	return type_object;
}

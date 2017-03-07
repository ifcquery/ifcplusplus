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
#include "include/IfcSubContractResourceTypeEnum.h"

// TYPE IfcSubContractResourceTypeEnum = ENUMERATION OF	(PURCHASE	,WORK	,USERDEFINED	,NOTDEFINED);
IfcSubContractResourceTypeEnum::IfcSubContractResourceTypeEnum() {}
IfcSubContractResourceTypeEnum::~IfcSubContractResourceTypeEnum() {}
shared_ptr<IfcPPObject> IfcSubContractResourceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSubContractResourceTypeEnum> copy_self( new IfcSubContractResourceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSubContractResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSUBCONTRACTRESOURCETYPEENUM("; }
	if( m_enum == ENUM_PURCHASE )
	{
		stream << ".PURCHASE.";
	}
	else if( m_enum == ENUM_WORK )
	{
		stream << ".WORK.";
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
shared_ptr<IfcSubContractResourceTypeEnum> IfcSubContractResourceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSubContractResourceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSubContractResourceTypeEnum>(); }
	shared_ptr<IfcSubContractResourceTypeEnum> type_object( new IfcSubContractResourceTypeEnum() );
	if( boost::iequals( arg, L".PURCHASE." ) )
	{
		type_object->m_enum = IfcSubContractResourceTypeEnum::ENUM_PURCHASE;
	}
	else if( boost::iequals( arg, L".WORK." ) )
	{
		type_object->m_enum = IfcSubContractResourceTypeEnum::ENUM_WORK;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSubContractResourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSubContractResourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

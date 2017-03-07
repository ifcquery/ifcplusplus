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
#include "include/IfcHeatExchangerTypeEnum.h"

// TYPE IfcHeatExchangerTypeEnum = ENUMERATION OF	(PLATE	,SHELLANDTUBE	,USERDEFINED	,NOTDEFINED);
IfcHeatExchangerTypeEnum::IfcHeatExchangerTypeEnum() {}
IfcHeatExchangerTypeEnum::~IfcHeatExchangerTypeEnum() {}
shared_ptr<IfcPPObject> IfcHeatExchangerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcHeatExchangerTypeEnum> copy_self( new IfcHeatExchangerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcHeatExchangerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCHEATEXCHANGERTYPEENUM("; }
	if( m_enum == ENUM_PLATE )
	{
		stream << ".PLATE.";
	}
	else if( m_enum == ENUM_SHELLANDTUBE )
	{
		stream << ".SHELLANDTUBE.";
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
shared_ptr<IfcHeatExchangerTypeEnum> IfcHeatExchangerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcHeatExchangerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcHeatExchangerTypeEnum>(); }
	shared_ptr<IfcHeatExchangerTypeEnum> type_object( new IfcHeatExchangerTypeEnum() );
	if( boost::iequals( arg, L".PLATE." ) )
	{
		type_object->m_enum = IfcHeatExchangerTypeEnum::ENUM_PLATE;
	}
	else if( boost::iequals( arg, L".SHELLANDTUBE." ) )
	{
		type_object->m_enum = IfcHeatExchangerTypeEnum::ENUM_SHELLANDTUBE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcHeatExchangerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcHeatExchangerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

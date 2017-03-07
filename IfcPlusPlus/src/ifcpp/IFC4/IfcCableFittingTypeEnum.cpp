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
#include "include/IfcCableFittingTypeEnum.h"

// TYPE IfcCableFittingTypeEnum = ENUMERATION OF	(CONNECTOR	,ENTRY	,EXIT	,JUNCTION	,TRANSITION	,USERDEFINED	,NOTDEFINED);
IfcCableFittingTypeEnum::IfcCableFittingTypeEnum() {}
IfcCableFittingTypeEnum::~IfcCableFittingTypeEnum() {}
shared_ptr<IfcPPObject> IfcCableFittingTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCableFittingTypeEnum> copy_self( new IfcCableFittingTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCableFittingTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCABLEFITTINGTYPEENUM("; }
	if( m_enum == ENUM_CONNECTOR )
	{
		stream << ".CONNECTOR.";
	}
	else if( m_enum == ENUM_ENTRY )
	{
		stream << ".ENTRY.";
	}
	else if( m_enum == ENUM_EXIT )
	{
		stream << ".EXIT.";
	}
	else if( m_enum == ENUM_JUNCTION )
	{
		stream << ".JUNCTION.";
	}
	else if( m_enum == ENUM_TRANSITION )
	{
		stream << ".TRANSITION.";
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
shared_ptr<IfcCableFittingTypeEnum> IfcCableFittingTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCableFittingTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCableFittingTypeEnum>(); }
	shared_ptr<IfcCableFittingTypeEnum> type_object( new IfcCableFittingTypeEnum() );
	if( boost::iequals( arg, L".CONNECTOR." ) )
	{
		type_object->m_enum = IfcCableFittingTypeEnum::ENUM_CONNECTOR;
	}
	else if( boost::iequals( arg, L".ENTRY." ) )
	{
		type_object->m_enum = IfcCableFittingTypeEnum::ENUM_ENTRY;
	}
	else if( boost::iequals( arg, L".EXIT." ) )
	{
		type_object->m_enum = IfcCableFittingTypeEnum::ENUM_EXIT;
	}
	else if( boost::iequals( arg, L".JUNCTION." ) )
	{
		type_object->m_enum = IfcCableFittingTypeEnum::ENUM_JUNCTION;
	}
	else if( boost::iequals( arg, L".TRANSITION." ) )
	{
		type_object->m_enum = IfcCableFittingTypeEnum::ENUM_TRANSITION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCableFittingTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCableFittingTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

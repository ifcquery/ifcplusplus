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
#include "include/IfcEvaporatorTypeEnum.h"

// TYPE IfcEvaporatorTypeEnum = ENUMERATION OF	(DIRECTEXPANSION	,DIRECTEXPANSIONSHELLANDTUBE	,DIRECTEXPANSIONTUBEINTUBE	,DIRECTEXPANSIONBRAZEDPLATE	,FLOODEDSHELLANDTUBE	,SHELLANDCOIL	,USERDEFINED	,NOTDEFINED);
IfcEvaporatorTypeEnum::IfcEvaporatorTypeEnum() {}
IfcEvaporatorTypeEnum::~IfcEvaporatorTypeEnum() {}
shared_ptr<IfcPPObject> IfcEvaporatorTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcEvaporatorTypeEnum> copy_self( new IfcEvaporatorTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcEvaporatorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCEVAPORATORTYPEENUM("; }
	if( m_enum == ENUM_DIRECTEXPANSION )
	{
		stream << ".DIRECTEXPANSION.";
	}
	else if( m_enum == ENUM_DIRECTEXPANSIONSHELLANDTUBE )
	{
		stream << ".DIRECTEXPANSIONSHELLANDTUBE.";
	}
	else if( m_enum == ENUM_DIRECTEXPANSIONTUBEINTUBE )
	{
		stream << ".DIRECTEXPANSIONTUBEINTUBE.";
	}
	else if( m_enum == ENUM_DIRECTEXPANSIONBRAZEDPLATE )
	{
		stream << ".DIRECTEXPANSIONBRAZEDPLATE.";
	}
	else if( m_enum == ENUM_FLOODEDSHELLANDTUBE )
	{
		stream << ".FLOODEDSHELLANDTUBE.";
	}
	else if( m_enum == ENUM_SHELLANDCOIL )
	{
		stream << ".SHELLANDCOIL.";
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
shared_ptr<IfcEvaporatorTypeEnum> IfcEvaporatorTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcEvaporatorTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcEvaporatorTypeEnum>(); }
	shared_ptr<IfcEvaporatorTypeEnum> type_object( new IfcEvaporatorTypeEnum() );
	if( boost::iequals( arg, L".DIRECTEXPANSION." ) )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_DIRECTEXPANSION;
	}
	else if( boost::iequals( arg, L".DIRECTEXPANSIONSHELLANDTUBE." ) )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_DIRECTEXPANSIONSHELLANDTUBE;
	}
	else if( boost::iequals( arg, L".DIRECTEXPANSIONTUBEINTUBE." ) )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_DIRECTEXPANSIONTUBEINTUBE;
	}
	else if( boost::iequals( arg, L".DIRECTEXPANSIONBRAZEDPLATE." ) )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_DIRECTEXPANSIONBRAZEDPLATE;
	}
	else if( boost::iequals( arg, L".FLOODEDSHELLANDTUBE." ) )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_FLOODEDSHELLANDTUBE;
	}
	else if( boost::iequals( arg, L".SHELLANDCOIL." ) )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_SHELLANDCOIL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

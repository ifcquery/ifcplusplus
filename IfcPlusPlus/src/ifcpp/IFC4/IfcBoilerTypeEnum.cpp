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
#include "include/IfcBoilerTypeEnum.h"

// TYPE IfcBoilerTypeEnum = ENUMERATION OF	(WATER	,STEAM	,USERDEFINED	,NOTDEFINED);
IfcBoilerTypeEnum::IfcBoilerTypeEnum() {}
IfcBoilerTypeEnum::~IfcBoilerTypeEnum() {}
shared_ptr<IfcPPObject> IfcBoilerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBoilerTypeEnum> copy_self( new IfcBoilerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBoilerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBOILERTYPEENUM("; }
	if( m_enum == ENUM_WATER )
	{
		stream << ".WATER.";
	}
	else if( m_enum == ENUM_STEAM )
	{
		stream << ".STEAM.";
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
shared_ptr<IfcBoilerTypeEnum> IfcBoilerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBoilerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBoilerTypeEnum>(); }
	shared_ptr<IfcBoilerTypeEnum> type_object( new IfcBoilerTypeEnum() );
	if( boost::iequals( arg, L".WATER." ) )
	{
		type_object->m_enum = IfcBoilerTypeEnum::ENUM_WATER;
	}
	else if( boost::iequals( arg, L".STEAM." ) )
	{
		type_object->m_enum = IfcBoilerTypeEnum::ENUM_STEAM;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcBoilerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcBoilerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

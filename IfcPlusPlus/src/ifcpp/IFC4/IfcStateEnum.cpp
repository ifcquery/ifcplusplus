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
#include "include/IfcStateEnum.h"

// TYPE IfcStateEnum = ENUMERATION OF	(READWRITE	,READONLY	,LOCKED	,READWRITELOCKED	,READONLYLOCKED);
IfcStateEnum::IfcStateEnum() {}
IfcStateEnum::~IfcStateEnum() {}
shared_ptr<IfcPPObject> IfcStateEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStateEnum> copy_self( new IfcStateEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcStateEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTATEENUM("; }
	if( m_enum == ENUM_READWRITE )
	{
		stream << ".READWRITE.";
	}
	else if( m_enum == ENUM_READONLY )
	{
		stream << ".READONLY.";
	}
	else if( m_enum == ENUM_LOCKED )
	{
		stream << ".LOCKED.";
	}
	else if( m_enum == ENUM_READWRITELOCKED )
	{
		stream << ".READWRITELOCKED.";
	}
	else if( m_enum == ENUM_READONLYLOCKED )
	{
		stream << ".READONLYLOCKED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcStateEnum> IfcStateEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcStateEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcStateEnum>(); }
	shared_ptr<IfcStateEnum> type_object( new IfcStateEnum() );
	if( boost::iequals( arg, L".READWRITE." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READWRITE;
	}
	else if( boost::iequals( arg, L".READONLY." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READONLY;
	}
	else if( boost::iequals( arg, L".LOCKED." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_LOCKED;
	}
	else if( boost::iequals( arg, L".READWRITELOCKED." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READWRITELOCKED;
	}
	else if( boost::iequals( arg, L".READONLYLOCKED." ) )
	{
		type_object->m_enum = IfcStateEnum::ENUM_READONLYLOCKED;
	}
	return type_object;
}

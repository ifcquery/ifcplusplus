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
#include "include/IfcPlateTypeEnum.h"

// TYPE IfcPlateTypeEnum = ENUMERATION OF	(CURTAIN_PANEL	,SHEET	,USERDEFINED	,NOTDEFINED);
IfcPlateTypeEnum::IfcPlateTypeEnum() {}
IfcPlateTypeEnum::~IfcPlateTypeEnum() {}
shared_ptr<IfcPPObject> IfcPlateTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPlateTypeEnum> copy_self( new IfcPlateTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPlateTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPLATETYPEENUM("; }
	if( m_enum == ENUM_CURTAIN_PANEL )
	{
		stream << ".CURTAIN_PANEL.";
	}
	else if( m_enum == ENUM_SHEET )
	{
		stream << ".SHEET.";
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
shared_ptr<IfcPlateTypeEnum> IfcPlateTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPlateTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPlateTypeEnum>(); }
	shared_ptr<IfcPlateTypeEnum> type_object( new IfcPlateTypeEnum() );
	if( boost::iequals( arg, L".CURTAIN_PANEL." ) )
	{
		type_object->m_enum = IfcPlateTypeEnum::ENUM_CURTAIN_PANEL;
	}
	else if( boost::iequals( arg, L".SHEET." ) )
	{
		type_object->m_enum = IfcPlateTypeEnum::ENUM_SHEET;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPlateTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPlateTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

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
#include "include/IfcWasteTerminalTypeEnum.h"

// TYPE IfcWasteTerminalTypeEnum = ENUMERATION OF	(FLOORTRAP	,FLOORWASTE	,GULLYSUMP	,GULLYTRAP	,ROOFDRAIN	,WASTEDISPOSALUNIT	,WASTETRAP	,USERDEFINED	,NOTDEFINED);
IfcWasteTerminalTypeEnum::IfcWasteTerminalTypeEnum() {}
IfcWasteTerminalTypeEnum::~IfcWasteTerminalTypeEnum() {}
shared_ptr<IfcPPObject> IfcWasteTerminalTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWasteTerminalTypeEnum> copy_self( new IfcWasteTerminalTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWasteTerminalTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWASTETERMINALTYPEENUM("; }
	if( m_enum == ENUM_FLOORTRAP )
	{
		stream << ".FLOORTRAP.";
	}
	else if( m_enum == ENUM_FLOORWASTE )
	{
		stream << ".FLOORWASTE.";
	}
	else if( m_enum == ENUM_GULLYSUMP )
	{
		stream << ".GULLYSUMP.";
	}
	else if( m_enum == ENUM_GULLYTRAP )
	{
		stream << ".GULLYTRAP.";
	}
	else if( m_enum == ENUM_ROOFDRAIN )
	{
		stream << ".ROOFDRAIN.";
	}
	else if( m_enum == ENUM_WASTEDISPOSALUNIT )
	{
		stream << ".WASTEDISPOSALUNIT.";
	}
	else if( m_enum == ENUM_WASTETRAP )
	{
		stream << ".WASTETRAP.";
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
shared_ptr<IfcWasteTerminalTypeEnum> IfcWasteTerminalTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWasteTerminalTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWasteTerminalTypeEnum>(); }
	shared_ptr<IfcWasteTerminalTypeEnum> type_object( new IfcWasteTerminalTypeEnum() );
	if( boost::iequals( arg, L".FLOORTRAP." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_FLOORTRAP;
	}
	else if( boost::iequals( arg, L".FLOORWASTE." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_FLOORWASTE;
	}
	else if( boost::iequals( arg, L".GULLYSUMP." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_GULLYSUMP;
	}
	else if( boost::iequals( arg, L".GULLYTRAP." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_GULLYTRAP;
	}
	else if( boost::iequals( arg, L".ROOFDRAIN." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_ROOFDRAIN;
	}
	else if( boost::iequals( arg, L".WASTEDISPOSALUNIT." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_WASTEDISPOSALUNIT;
	}
	else if( boost::iequals( arg, L".WASTETRAP." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_WASTETRAP;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWasteTerminalTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

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
#include "include/IfcWindowPanelPositionEnum.h"

// TYPE IfcWindowPanelPositionEnum = ENUMERATION OF	(LEFT	,MIDDLE	,RIGHT	,BOTTOM	,TOP	,NOTDEFINED);
IfcWindowPanelPositionEnum::IfcWindowPanelPositionEnum() {}
IfcWindowPanelPositionEnum::~IfcWindowPanelPositionEnum() {}
shared_ptr<IfcPPObject> IfcWindowPanelPositionEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWindowPanelPositionEnum> copy_self( new IfcWindowPanelPositionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWindowPanelPositionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWINDOWPANELPOSITIONENUM("; }
	if( m_enum == ENUM_LEFT )
	{
		stream << ".LEFT.";
	}
	else if( m_enum == ENUM_MIDDLE )
	{
		stream << ".MIDDLE.";
	}
	else if( m_enum == ENUM_RIGHT )
	{
		stream << ".RIGHT.";
	}
	else if( m_enum == ENUM_BOTTOM )
	{
		stream << ".BOTTOM.";
	}
	else if( m_enum == ENUM_TOP )
	{
		stream << ".TOP.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcWindowPanelPositionEnum> IfcWindowPanelPositionEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWindowPanelPositionEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWindowPanelPositionEnum>(); }
	shared_ptr<IfcWindowPanelPositionEnum> type_object( new IfcWindowPanelPositionEnum() );
	if( boost::iequals( arg, L".LEFT." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_LEFT;
	}
	else if( boost::iequals( arg, L".MIDDLE." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_MIDDLE;
	}
	else if( boost::iequals( arg, L".RIGHT." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_RIGHT;
	}
	else if( boost::iequals( arg, L".BOTTOM." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_BOTTOM;
	}
	else if( boost::iequals( arg, L".TOP." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_TOP;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

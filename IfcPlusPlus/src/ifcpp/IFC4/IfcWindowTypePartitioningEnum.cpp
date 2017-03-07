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
#include "include/IfcWindowTypePartitioningEnum.h"

// TYPE IfcWindowTypePartitioningEnum = ENUMERATION OF	(SINGLE_PANEL	,DOUBLE_PANEL_VERTICAL	,DOUBLE_PANEL_HORIZONTAL	,TRIPLE_PANEL_VERTICAL	,TRIPLE_PANEL_BOTTOM	,TRIPLE_PANEL_TOP	,TRIPLE_PANEL_LEFT	,TRIPLE_PANEL_RIGHT	,TRIPLE_PANEL_HORIZONTAL	,USERDEFINED	,NOTDEFINED);
IfcWindowTypePartitioningEnum::IfcWindowTypePartitioningEnum() {}
IfcWindowTypePartitioningEnum::~IfcWindowTypePartitioningEnum() {}
shared_ptr<IfcPPObject> IfcWindowTypePartitioningEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWindowTypePartitioningEnum> copy_self( new IfcWindowTypePartitioningEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWindowTypePartitioningEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWINDOWTYPEPARTITIONINGENUM("; }
	if( m_enum == ENUM_SINGLE_PANEL )
	{
		stream << ".SINGLE_PANEL.";
	}
	else if( m_enum == ENUM_DOUBLE_PANEL_VERTICAL )
	{
		stream << ".DOUBLE_PANEL_VERTICAL.";
	}
	else if( m_enum == ENUM_DOUBLE_PANEL_HORIZONTAL )
	{
		stream << ".DOUBLE_PANEL_HORIZONTAL.";
	}
	else if( m_enum == ENUM_TRIPLE_PANEL_VERTICAL )
	{
		stream << ".TRIPLE_PANEL_VERTICAL.";
	}
	else if( m_enum == ENUM_TRIPLE_PANEL_BOTTOM )
	{
		stream << ".TRIPLE_PANEL_BOTTOM.";
	}
	else if( m_enum == ENUM_TRIPLE_PANEL_TOP )
	{
		stream << ".TRIPLE_PANEL_TOP.";
	}
	else if( m_enum == ENUM_TRIPLE_PANEL_LEFT )
	{
		stream << ".TRIPLE_PANEL_LEFT.";
	}
	else if( m_enum == ENUM_TRIPLE_PANEL_RIGHT )
	{
		stream << ".TRIPLE_PANEL_RIGHT.";
	}
	else if( m_enum == ENUM_TRIPLE_PANEL_HORIZONTAL )
	{
		stream << ".TRIPLE_PANEL_HORIZONTAL.";
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
shared_ptr<IfcWindowTypePartitioningEnum> IfcWindowTypePartitioningEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWindowTypePartitioningEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWindowTypePartitioningEnum>(); }
	shared_ptr<IfcWindowTypePartitioningEnum> type_object( new IfcWindowTypePartitioningEnum() );
	if( boost::iequals( arg, L".SINGLE_PANEL." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_SINGLE_PANEL;
	}
	else if( boost::iequals( arg, L".DOUBLE_PANEL_VERTICAL." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_DOUBLE_PANEL_VERTICAL;
	}
	else if( boost::iequals( arg, L".DOUBLE_PANEL_HORIZONTAL." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_DOUBLE_PANEL_HORIZONTAL;
	}
	else if( boost::iequals( arg, L".TRIPLE_PANEL_VERTICAL." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_TRIPLE_PANEL_VERTICAL;
	}
	else if( boost::iequals( arg, L".TRIPLE_PANEL_BOTTOM." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_TRIPLE_PANEL_BOTTOM;
	}
	else if( boost::iequals( arg, L".TRIPLE_PANEL_TOP." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_TRIPLE_PANEL_TOP;
	}
	else if( boost::iequals( arg, L".TRIPLE_PANEL_LEFT." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_TRIPLE_PANEL_LEFT;
	}
	else if( boost::iequals( arg, L".TRIPLE_PANEL_RIGHT." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_TRIPLE_PANEL_RIGHT;
	}
	else if( boost::iequals( arg, L".TRIPLE_PANEL_HORIZONTAL." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_TRIPLE_PANEL_HORIZONTAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWindowTypePartitioningEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

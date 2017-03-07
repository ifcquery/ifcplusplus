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
#include "include/IfcWindowStyleConstructionEnum.h"

// TYPE IfcWindowStyleConstructionEnum = ENUMERATION OF	(ALUMINIUM	,HIGH_GRADE_STEEL	,STEEL	,WOOD	,ALUMINIUM_WOOD	,PLASTIC	,OTHER_CONSTRUCTION	,NOTDEFINED);
IfcWindowStyleConstructionEnum::IfcWindowStyleConstructionEnum() {}
IfcWindowStyleConstructionEnum::~IfcWindowStyleConstructionEnum() {}
shared_ptr<IfcPPObject> IfcWindowStyleConstructionEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWindowStyleConstructionEnum> copy_self( new IfcWindowStyleConstructionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWindowStyleConstructionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWINDOWSTYLECONSTRUCTIONENUM("; }
	if( m_enum == ENUM_ALUMINIUM )
	{
		stream << ".ALUMINIUM.";
	}
	else if( m_enum == ENUM_HIGH_GRADE_STEEL )
	{
		stream << ".HIGH_GRADE_STEEL.";
	}
	else if( m_enum == ENUM_STEEL )
	{
		stream << ".STEEL.";
	}
	else if( m_enum == ENUM_WOOD )
	{
		stream << ".WOOD.";
	}
	else if( m_enum == ENUM_ALUMINIUM_WOOD )
	{
		stream << ".ALUMINIUM_WOOD.";
	}
	else if( m_enum == ENUM_PLASTIC )
	{
		stream << ".PLASTIC.";
	}
	else if( m_enum == ENUM_OTHER_CONSTRUCTION )
	{
		stream << ".OTHER_CONSTRUCTION.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcWindowStyleConstructionEnum> IfcWindowStyleConstructionEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWindowStyleConstructionEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWindowStyleConstructionEnum>(); }
	shared_ptr<IfcWindowStyleConstructionEnum> type_object( new IfcWindowStyleConstructionEnum() );
	if( boost::iequals( arg, L".ALUMINIUM." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_ALUMINIUM;
	}
	else if( boost::iequals( arg, L".HIGH_GRADE_STEEL." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_HIGH_GRADE_STEEL;
	}
	else if( boost::iequals( arg, L".STEEL." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_STEEL;
	}
	else if( boost::iequals( arg, L".WOOD." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_WOOD;
	}
	else if( boost::iequals( arg, L".ALUMINIUM_WOOD." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_ALUMINIUM_WOOD;
	}
	else if( boost::iequals( arg, L".PLASTIC." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_PLASTIC;
	}
	else if( boost::iequals( arg, L".OTHER_CONSTRUCTION." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_OTHER_CONSTRUCTION;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

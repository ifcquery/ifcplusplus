/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcArithmeticOperatorEnum.h"

// TYPE IfcArithmeticOperatorEnum 
IfcArithmeticOperatorEnum::IfcArithmeticOperatorEnum() {}
IfcArithmeticOperatorEnum::~IfcArithmeticOperatorEnum() {}
shared_ptr<IfcPPObject> IfcArithmeticOperatorEnum::getDeepCopy()
{
	shared_ptr<IfcArithmeticOperatorEnum> copy_self( new IfcArithmeticOperatorEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcArithmeticOperatorEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCARITHMETICOPERATORENUM("; }
	if( m_enum == ENUM_ADD )
	{
		stream << ".ADD.";
	}
	else if( m_enum == ENUM_DIVIDE )
	{
		stream << ".DIVIDE.";
	}
	else if( m_enum == ENUM_MULTIPLY )
	{
		stream << ".MULTIPLY.";
	}
	else if( m_enum == ENUM_SUBTRACT )
	{
		stream << ".SUBTRACT.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcArithmeticOperatorEnum> IfcArithmeticOperatorEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcArithmeticOperatorEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcArithmeticOperatorEnum>(); }
	shared_ptr<IfcArithmeticOperatorEnum> type_object( new IfcArithmeticOperatorEnum() );
	if( boost::iequals( arg, L".ADD." ) )
	{
		type_object->m_enum = IfcArithmeticOperatorEnum::ENUM_ADD;
	}
	else if( boost::iequals( arg, L".DIVIDE." ) )
	{
		type_object->m_enum = IfcArithmeticOperatorEnum::ENUM_DIVIDE;
	}
	else if( boost::iequals( arg, L".MULTIPLY." ) )
	{
		type_object->m_enum = IfcArithmeticOperatorEnum::ENUM_MULTIPLY;
	}
	else if( boost::iequals( arg, L".SUBTRACT." ) )
	{
		type_object->m_enum = IfcArithmeticOperatorEnum::ENUM_SUBTRACT;
	}
	return type_object;
}

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
#include "include/IfcRampTypeEnum.h"

// TYPE IfcRampTypeEnum = ENUMERATION OF	(STRAIGHT_RUN_RAMP	,TWO_STRAIGHT_RUN_RAMP	,QUARTER_TURN_RAMP	,TWO_QUARTER_TURN_RAMP	,HALF_TURN_RAMP	,SPIRAL_RAMP	,USERDEFINED	,NOTDEFINED);
IfcRampTypeEnum::IfcRampTypeEnum() {}
IfcRampTypeEnum::~IfcRampTypeEnum() {}
shared_ptr<IfcPPObject> IfcRampTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRampTypeEnum> copy_self( new IfcRampTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRampTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCRAMPTYPEENUM("; }
	if( m_enum == ENUM_STRAIGHT_RUN_RAMP )
	{
		stream << ".STRAIGHT_RUN_RAMP.";
	}
	else if( m_enum == ENUM_TWO_STRAIGHT_RUN_RAMP )
	{
		stream << ".TWO_STRAIGHT_RUN_RAMP.";
	}
	else if( m_enum == ENUM_QUARTER_TURN_RAMP )
	{
		stream << ".QUARTER_TURN_RAMP.";
	}
	else if( m_enum == ENUM_TWO_QUARTER_TURN_RAMP )
	{
		stream << ".TWO_QUARTER_TURN_RAMP.";
	}
	else if( m_enum == ENUM_HALF_TURN_RAMP )
	{
		stream << ".HALF_TURN_RAMP.";
	}
	else if( m_enum == ENUM_SPIRAL_RAMP )
	{
		stream << ".SPIRAL_RAMP.";
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
shared_ptr<IfcRampTypeEnum> IfcRampTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRampTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRampTypeEnum>(); }
	shared_ptr<IfcRampTypeEnum> type_object( new IfcRampTypeEnum() );
	if( boost::iequals( arg, L".STRAIGHT_RUN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_STRAIGHT_RUN_RAMP;
	}
	else if( boost::iequals( arg, L".TWO_STRAIGHT_RUN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_TWO_STRAIGHT_RUN_RAMP;
	}
	else if( boost::iequals( arg, L".QUARTER_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_QUARTER_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".TWO_QUARTER_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_TWO_QUARTER_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".HALF_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_HALF_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".SPIRAL_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_SPIRAL_RAMP;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

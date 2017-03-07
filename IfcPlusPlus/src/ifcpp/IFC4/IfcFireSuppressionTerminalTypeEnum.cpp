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
#include "include/IfcFireSuppressionTerminalTypeEnum.h"

// TYPE IfcFireSuppressionTerminalTypeEnum = ENUMERATION OF	(BREECHINGINLET	,FIREHYDRANT	,HOSEREEL	,SPRINKLER	,SPRINKLERDEFLECTOR	,USERDEFINED	,NOTDEFINED);
IfcFireSuppressionTerminalTypeEnum::IfcFireSuppressionTerminalTypeEnum() {}
IfcFireSuppressionTerminalTypeEnum::~IfcFireSuppressionTerminalTypeEnum() {}
shared_ptr<IfcPPObject> IfcFireSuppressionTerminalTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFireSuppressionTerminalTypeEnum> copy_self( new IfcFireSuppressionTerminalTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFireSuppressionTerminalTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFIRESUPPRESSIONTERMINALTYPEENUM("; }
	if( m_enum == ENUM_BREECHINGINLET )
	{
		stream << ".BREECHINGINLET.";
	}
	else if( m_enum == ENUM_FIREHYDRANT )
	{
		stream << ".FIREHYDRANT.";
	}
	else if( m_enum == ENUM_HOSEREEL )
	{
		stream << ".HOSEREEL.";
	}
	else if( m_enum == ENUM_SPRINKLER )
	{
		stream << ".SPRINKLER.";
	}
	else if( m_enum == ENUM_SPRINKLERDEFLECTOR )
	{
		stream << ".SPRINKLERDEFLECTOR.";
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
shared_ptr<IfcFireSuppressionTerminalTypeEnum> IfcFireSuppressionTerminalTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFireSuppressionTerminalTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFireSuppressionTerminalTypeEnum>(); }
	shared_ptr<IfcFireSuppressionTerminalTypeEnum> type_object( new IfcFireSuppressionTerminalTypeEnum() );
	if( boost::iequals( arg, L".BREECHINGINLET." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_BREECHINGINLET;
	}
	else if( boost::iequals( arg, L".FIREHYDRANT." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_FIREHYDRANT;
	}
	else if( boost::iequals( arg, L".HOSEREEL." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_HOSEREEL;
	}
	else if( boost::iequals( arg, L".SPRINKLER." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_SPRINKLER;
	}
	else if( boost::iequals( arg, L".SPRINKLERDEFLECTOR." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_SPRINKLERDEFLECTOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

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
#include "include/IfcTransformerTypeEnum.h"

// TYPE IfcTransformerTypeEnum = ENUMERATION OF	(CURRENT	,FREQUENCY	,INVERTER	,RECTIFIER	,VOLTAGE	,USERDEFINED	,NOTDEFINED);
IfcTransformerTypeEnum::IfcTransformerTypeEnum() {}
IfcTransformerTypeEnum::~IfcTransformerTypeEnum() {}
shared_ptr<IfcPPObject> IfcTransformerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTransformerTypeEnum> copy_self( new IfcTransformerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTransformerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRANSFORMERTYPEENUM("; }
	if( m_enum == ENUM_CURRENT )
	{
		stream << ".CURRENT.";
	}
	else if( m_enum == ENUM_FREQUENCY )
	{
		stream << ".FREQUENCY.";
	}
	else if( m_enum == ENUM_INVERTER )
	{
		stream << ".INVERTER.";
	}
	else if( m_enum == ENUM_RECTIFIER )
	{
		stream << ".RECTIFIER.";
	}
	else if( m_enum == ENUM_VOLTAGE )
	{
		stream << ".VOLTAGE.";
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
shared_ptr<IfcTransformerTypeEnum> IfcTransformerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTransformerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTransformerTypeEnum>(); }
	shared_ptr<IfcTransformerTypeEnum> type_object( new IfcTransformerTypeEnum() );
	if( boost::iequals( arg, L".CURRENT." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_CURRENT;
	}
	else if( boost::iequals( arg, L".FREQUENCY." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_FREQUENCY;
	}
	else if( boost::iequals( arg, L".INVERTER." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_INVERTER;
	}
	else if( boost::iequals( arg, L".RECTIFIER." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_RECTIFIER;
	}
	else if( boost::iequals( arg, L".VOLTAGE." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_VOLTAGE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

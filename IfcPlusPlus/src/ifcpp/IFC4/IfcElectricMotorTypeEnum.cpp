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
#include "include/IfcElectricMotorTypeEnum.h"

// TYPE IfcElectricMotorTypeEnum = ENUMERATION OF	(DC	,INDUCTION	,POLYPHASE	,RELUCTANCESYNCHRONOUS	,SYNCHRONOUS	,USERDEFINED	,NOTDEFINED);
IfcElectricMotorTypeEnum::IfcElectricMotorTypeEnum() {}
IfcElectricMotorTypeEnum::~IfcElectricMotorTypeEnum() {}
shared_ptr<IfcPPObject> IfcElectricMotorTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcElectricMotorTypeEnum> copy_self( new IfcElectricMotorTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcElectricMotorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICMOTORTYPEENUM("; }
	if( m_enum == ENUM_DC )
	{
		stream << ".DC.";
	}
	else if( m_enum == ENUM_INDUCTION )
	{
		stream << ".INDUCTION.";
	}
	else if( m_enum == ENUM_POLYPHASE )
	{
		stream << ".POLYPHASE.";
	}
	else if( m_enum == ENUM_RELUCTANCESYNCHRONOUS )
	{
		stream << ".RELUCTANCESYNCHRONOUS.";
	}
	else if( m_enum == ENUM_SYNCHRONOUS )
	{
		stream << ".SYNCHRONOUS.";
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
shared_ptr<IfcElectricMotorTypeEnum> IfcElectricMotorTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElectricMotorTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElectricMotorTypeEnum>(); }
	shared_ptr<IfcElectricMotorTypeEnum> type_object( new IfcElectricMotorTypeEnum() );
	if( boost::iequals( arg, L".DC." ) )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_DC;
	}
	else if( boost::iequals( arg, L".INDUCTION." ) )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_INDUCTION;
	}
	else if( boost::iequals( arg, L".POLYPHASE." ) )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_POLYPHASE;
	}
	else if( boost::iequals( arg, L".RELUCTANCESYNCHRONOUS." ) )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_RELUCTANCESYNCHRONOUS;
	}
	else if( boost::iequals( arg, L".SYNCHRONOUS." ) )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_SYNCHRONOUS;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

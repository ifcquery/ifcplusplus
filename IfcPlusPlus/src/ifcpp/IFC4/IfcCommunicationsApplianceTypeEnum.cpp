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
#include "include/IfcCommunicationsApplianceTypeEnum.h"

// TYPE IfcCommunicationsApplianceTypeEnum = ENUMERATION OF	(ANTENNA	,COMPUTER	,FAX	,GATEWAY	,MODEM	,NETWORKAPPLIANCE	,NETWORKBRIDGE	,NETWORKHUB	,PRINTER	,REPEATER	,ROUTER	,SCANNER	,USERDEFINED	,NOTDEFINED);
IfcCommunicationsApplianceTypeEnum::IfcCommunicationsApplianceTypeEnum() {}
IfcCommunicationsApplianceTypeEnum::~IfcCommunicationsApplianceTypeEnum() {}
shared_ptr<IfcPPObject> IfcCommunicationsApplianceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCommunicationsApplianceTypeEnum> copy_self( new IfcCommunicationsApplianceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCommunicationsApplianceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOMMUNICATIONSAPPLIANCETYPEENUM("; }
	if( m_enum == ENUM_ANTENNA )
	{
		stream << ".ANTENNA.";
	}
	else if( m_enum == ENUM_COMPUTER )
	{
		stream << ".COMPUTER.";
	}
	else if( m_enum == ENUM_FAX )
	{
		stream << ".FAX.";
	}
	else if( m_enum == ENUM_GATEWAY )
	{
		stream << ".GATEWAY.";
	}
	else if( m_enum == ENUM_MODEM )
	{
		stream << ".MODEM.";
	}
	else if( m_enum == ENUM_NETWORKAPPLIANCE )
	{
		stream << ".NETWORKAPPLIANCE.";
	}
	else if( m_enum == ENUM_NETWORKBRIDGE )
	{
		stream << ".NETWORKBRIDGE.";
	}
	else if( m_enum == ENUM_NETWORKHUB )
	{
		stream << ".NETWORKHUB.";
	}
	else if( m_enum == ENUM_PRINTER )
	{
		stream << ".PRINTER.";
	}
	else if( m_enum == ENUM_REPEATER )
	{
		stream << ".REPEATER.";
	}
	else if( m_enum == ENUM_ROUTER )
	{
		stream << ".ROUTER.";
	}
	else if( m_enum == ENUM_SCANNER )
	{
		stream << ".SCANNER.";
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
shared_ptr<IfcCommunicationsApplianceTypeEnum> IfcCommunicationsApplianceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCommunicationsApplianceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCommunicationsApplianceTypeEnum>(); }
	shared_ptr<IfcCommunicationsApplianceTypeEnum> type_object( new IfcCommunicationsApplianceTypeEnum() );
	if( boost::iequals( arg, L".ANTENNA." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_ANTENNA;
	}
	else if( boost::iequals( arg, L".COMPUTER." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_COMPUTER;
	}
	else if( boost::iequals( arg, L".FAX." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_FAX;
	}
	else if( boost::iequals( arg, L".GATEWAY." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_GATEWAY;
	}
	else if( boost::iequals( arg, L".MODEM." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_MODEM;
	}
	else if( boost::iequals( arg, L".NETWORKAPPLIANCE." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_NETWORKAPPLIANCE;
	}
	else if( boost::iequals( arg, L".NETWORKBRIDGE." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_NETWORKBRIDGE;
	}
	else if( boost::iequals( arg, L".NETWORKHUB." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_NETWORKHUB;
	}
	else if( boost::iequals( arg, L".PRINTER." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_PRINTER;
	}
	else if( boost::iequals( arg, L".REPEATER." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_REPEATER;
	}
	else if( boost::iequals( arg, L".ROUTER." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_ROUTER;
	}
	else if( boost::iequals( arg, L".SCANNER." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_SCANNER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

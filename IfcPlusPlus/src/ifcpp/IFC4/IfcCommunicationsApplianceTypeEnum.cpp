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
#include "include/IfcCommunicationsApplianceTypeEnum.h"

// TYPE IfcCommunicationsApplianceTypeEnum 
IfcCommunicationsApplianceTypeEnum::IfcCommunicationsApplianceTypeEnum() {}
IfcCommunicationsApplianceTypeEnum::~IfcCommunicationsApplianceTypeEnum() {}
shared_ptr<IfcPPObject> IfcCommunicationsApplianceTypeEnum::getDeepCopy()
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
shared_ptr<IfcCommunicationsApplianceTypeEnum> IfcCommunicationsApplianceTypeEnum::createObjectFromStepData( const std::wstring& arg )
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

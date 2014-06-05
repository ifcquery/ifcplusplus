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
shared_ptr<IfcCommunicationsApplianceTypeEnum> IfcCommunicationsApplianceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcCommunicationsApplianceTypeEnum>(); }
	shared_ptr<IfcCommunicationsApplianceTypeEnum> type_object( new IfcCommunicationsApplianceTypeEnum() );
	if( _stricmp( arg.c_str(), ".ANTENNA." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_ANTENNA;
	}
	else if( _stricmp( arg.c_str(), ".COMPUTER." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_COMPUTER;
	}
	else if( _stricmp( arg.c_str(), ".FAX." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_FAX;
	}
	else if( _stricmp( arg.c_str(), ".GATEWAY." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_GATEWAY;
	}
	else if( _stricmp( arg.c_str(), ".MODEM." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_MODEM;
	}
	else if( _stricmp( arg.c_str(), ".NETWORKAPPLIANCE." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_NETWORKAPPLIANCE;
	}
	else if( _stricmp( arg.c_str(), ".NETWORKBRIDGE." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_NETWORKBRIDGE;
	}
	else if( _stricmp( arg.c_str(), ".NETWORKHUB." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_NETWORKHUB;
	}
	else if( _stricmp( arg.c_str(), ".PRINTER." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_PRINTER;
	}
	else if( _stricmp( arg.c_str(), ".REPEATER." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_REPEATER;
	}
	else if( _stricmp( arg.c_str(), ".ROUTER." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_ROUTER;
	}
	else if( _stricmp( arg.c_str(), ".SCANNER." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_SCANNER;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcCommunicationsApplianceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

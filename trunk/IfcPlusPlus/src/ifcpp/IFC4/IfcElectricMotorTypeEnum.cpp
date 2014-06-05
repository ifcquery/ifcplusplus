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
#include "include/IfcElectricMotorTypeEnum.h"

// TYPE IfcElectricMotorTypeEnum 
IfcElectricMotorTypeEnum::IfcElectricMotorTypeEnum() {}
IfcElectricMotorTypeEnum::~IfcElectricMotorTypeEnum() {}
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
shared_ptr<IfcElectricMotorTypeEnum> IfcElectricMotorTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcElectricMotorTypeEnum>(); }
	shared_ptr<IfcElectricMotorTypeEnum> type_object( new IfcElectricMotorTypeEnum() );
	if( _stricmp( arg.c_str(), ".DC." ) == 0 )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_DC;
	}
	else if( _stricmp( arg.c_str(), ".INDUCTION." ) == 0 )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_INDUCTION;
	}
	else if( _stricmp( arg.c_str(), ".POLYPHASE." ) == 0 )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_POLYPHASE;
	}
	else if( _stricmp( arg.c_str(), ".RELUCTANCESYNCHRONOUS." ) == 0 )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_RELUCTANCESYNCHRONOUS;
	}
	else if( _stricmp( arg.c_str(), ".SYNCHRONOUS." ) == 0 )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_SYNCHRONOUS;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcElectricMotorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

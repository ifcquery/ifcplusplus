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
shared_ptr<IfcPPObject> IfcElectricMotorTypeEnum::getDeepCopy()
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
shared_ptr<IfcElectricMotorTypeEnum> IfcElectricMotorTypeEnum::createObjectFromStepData( const std::wstring& arg )
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

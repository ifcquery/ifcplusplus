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
#include "include/IfcElectricDistributionBoardTypeEnum.h"

// TYPE IfcElectricDistributionBoardTypeEnum 
IfcElectricDistributionBoardTypeEnum::IfcElectricDistributionBoardTypeEnum() {}
IfcElectricDistributionBoardTypeEnum::~IfcElectricDistributionBoardTypeEnum() {}
shared_ptr<IfcPPObject> IfcElectricDistributionBoardTypeEnum::getDeepCopy()
{
	shared_ptr<IfcElectricDistributionBoardTypeEnum> copy_self( new IfcElectricDistributionBoardTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcElectricDistributionBoardTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICDISTRIBUTIONBOARDTYPEENUM("; }
	if( m_enum == ENUM_CONSUMERUNIT )
	{
		stream << ".CONSUMERUNIT.";
	}
	else if( m_enum == ENUM_DISTRIBUTIONBOARD )
	{
		stream << ".DISTRIBUTIONBOARD.";
	}
	else if( m_enum == ENUM_MOTORCONTROLCENTRE )
	{
		stream << ".MOTORCONTROLCENTRE.";
	}
	else if( m_enum == ENUM_SWITCHBOARD )
	{
		stream << ".SWITCHBOARD.";
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
shared_ptr<IfcElectricDistributionBoardTypeEnum> IfcElectricDistributionBoardTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElectricDistributionBoardTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElectricDistributionBoardTypeEnum>(); }
	shared_ptr<IfcElectricDistributionBoardTypeEnum> type_object( new IfcElectricDistributionBoardTypeEnum() );
	if( boost::iequals( arg, L".CONSUMERUNIT." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_CONSUMERUNIT;
	}
	else if( boost::iequals( arg, L".DISTRIBUTIONBOARD." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_DISTRIBUTIONBOARD;
	}
	else if( boost::iequals( arg, L".MOTORCONTROLCENTRE." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_MOTORCONTROLCENTRE;
	}
	else if( boost::iequals( arg, L".SWITCHBOARD." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_SWITCHBOARD;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

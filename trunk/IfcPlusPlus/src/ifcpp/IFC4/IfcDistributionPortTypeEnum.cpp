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
#include "include/IfcDistributionPortTypeEnum.h"

// TYPE IfcDistributionPortTypeEnum 
IfcDistributionPortTypeEnum::IfcDistributionPortTypeEnum() {}
IfcDistributionPortTypeEnum::~IfcDistributionPortTypeEnum() {}
shared_ptr<IfcPPObject> IfcDistributionPortTypeEnum::getDeepCopy()
{
	shared_ptr<IfcDistributionPortTypeEnum> copy_self( new IfcDistributionPortTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDistributionPortTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDISTRIBUTIONPORTTYPEENUM("; }
	if( m_enum == ENUM_CABLE )
	{
		stream << ".CABLE.";
	}
	else if( m_enum == ENUM_CABLECARRIER )
	{
		stream << ".CABLECARRIER.";
	}
	else if( m_enum == ENUM_DUCT )
	{
		stream << ".DUCT.";
	}
	else if( m_enum == ENUM_PIPE )
	{
		stream << ".PIPE.";
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
shared_ptr<IfcDistributionPortTypeEnum> IfcDistributionPortTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDistributionPortTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDistributionPortTypeEnum>(); }
	shared_ptr<IfcDistributionPortTypeEnum> type_object( new IfcDistributionPortTypeEnum() );
	if( boost::iequals( arg, L".CABLE." ) )
	{
		type_object->m_enum = IfcDistributionPortTypeEnum::ENUM_CABLE;
	}
	else if( boost::iequals( arg, L".CABLECARRIER." ) )
	{
		type_object->m_enum = IfcDistributionPortTypeEnum::ENUM_CABLECARRIER;
	}
	else if( boost::iequals( arg, L".DUCT." ) )
	{
		type_object->m_enum = IfcDistributionPortTypeEnum::ENUM_DUCT;
	}
	else if( boost::iequals( arg, L".PIPE." ) )
	{
		type_object->m_enum = IfcDistributionPortTypeEnum::ENUM_PIPE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDistributionPortTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDistributionPortTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

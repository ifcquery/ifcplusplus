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
#include "include/IfcCostScheduleTypeEnum.h"

// TYPE IfcCostScheduleTypeEnum 
IfcCostScheduleTypeEnum::IfcCostScheduleTypeEnum() {}
IfcCostScheduleTypeEnum::~IfcCostScheduleTypeEnum() {}
void IfcCostScheduleTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOSTSCHEDULETYPEENUM("; }
	if( m_enum == ENUM_BUDGET )
	{
		stream << ".BUDGET.";
	}
	else if( m_enum == ENUM_COSTPLAN )
	{
		stream << ".COSTPLAN.";
	}
	else if( m_enum == ENUM_ESTIMATE )
	{
		stream << ".ESTIMATE.";
	}
	else if( m_enum == ENUM_TENDER )
	{
		stream << ".TENDER.";
	}
	else if( m_enum == ENUM_PRICEDBILLOFQUANTITIES )
	{
		stream << ".PRICEDBILLOFQUANTITIES.";
	}
	else if( m_enum == ENUM_UNPRICEDBILLOFQUANTITIES )
	{
		stream << ".UNPRICEDBILLOFQUANTITIES.";
	}
	else if( m_enum == ENUM_SCHEDULEOFRATES )
	{
		stream << ".SCHEDULEOFRATES.";
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
shared_ptr<IfcCostScheduleTypeEnum> IfcCostScheduleTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcCostScheduleTypeEnum>(); }
	shared_ptr<IfcCostScheduleTypeEnum> type_object( new IfcCostScheduleTypeEnum() );
	if( _stricmp( arg.c_str(), ".BUDGET." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_BUDGET;
	}
	else if( _stricmp( arg.c_str(), ".COSTPLAN." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_COSTPLAN;
	}
	else if( _stricmp( arg.c_str(), ".ESTIMATE." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_ESTIMATE;
	}
	else if( _stricmp( arg.c_str(), ".TENDER." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_TENDER;
	}
	else if( _stricmp( arg.c_str(), ".PRICEDBILLOFQUANTITIES." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_PRICEDBILLOFQUANTITIES;
	}
	else if( _stricmp( arg.c_str(), ".UNPRICEDBILLOFQUANTITIES." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_UNPRICEDBILLOFQUANTITIES;
	}
	else if( _stricmp( arg.c_str(), ".SCHEDULEOFRATES." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_SCHEDULEOFRATES;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcCostScheduleTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

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
#include "include/IfcRecurrenceTypeEnum.h"

// TYPE IfcRecurrenceTypeEnum 
IfcRecurrenceTypeEnum::IfcRecurrenceTypeEnum() {}
IfcRecurrenceTypeEnum::~IfcRecurrenceTypeEnum() {}
void IfcRecurrenceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCRECURRENCETYPEENUM("; }
	if( m_enum == ENUM_DAILY )
	{
		stream << ".DAILY.";
	}
	else if( m_enum == ENUM_WEEKLY )
	{
		stream << ".WEEKLY.";
	}
	else if( m_enum == ENUM_MONTHLY_BY_DAY_OF_MONTH )
	{
		stream << ".MONTHLY_BY_DAY_OF_MONTH.";
	}
	else if( m_enum == ENUM_MONTHLY_BY_POSITION )
	{
		stream << ".MONTHLY_BY_POSITION.";
	}
	else if( m_enum == ENUM_BY_DAY_COUNT )
	{
		stream << ".BY_DAY_COUNT.";
	}
	else if( m_enum == ENUM_BY_WEEKDAY_COUNT )
	{
		stream << ".BY_WEEKDAY_COUNT.";
	}
	else if( m_enum == ENUM_YEARLY_BY_DAY_OF_MONTH )
	{
		stream << ".YEARLY_BY_DAY_OF_MONTH.";
	}
	else if( m_enum == ENUM_YEARLY_BY_POSITION )
	{
		stream << ".YEARLY_BY_POSITION.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcRecurrenceTypeEnum> IfcRecurrenceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcRecurrenceTypeEnum>(); }
	shared_ptr<IfcRecurrenceTypeEnum> type_object( new IfcRecurrenceTypeEnum() );
	if( _stricmp( arg.c_str(), ".DAILY." ) == 0 )
	{
		type_object->m_enum = IfcRecurrenceTypeEnum::ENUM_DAILY;
	}
	else if( _stricmp( arg.c_str(), ".WEEKLY." ) == 0 )
	{
		type_object->m_enum = IfcRecurrenceTypeEnum::ENUM_WEEKLY;
	}
	else if( _stricmp( arg.c_str(), ".MONTHLY_BY_DAY_OF_MONTH." ) == 0 )
	{
		type_object->m_enum = IfcRecurrenceTypeEnum::ENUM_MONTHLY_BY_DAY_OF_MONTH;
	}
	else if( _stricmp( arg.c_str(), ".MONTHLY_BY_POSITION." ) == 0 )
	{
		type_object->m_enum = IfcRecurrenceTypeEnum::ENUM_MONTHLY_BY_POSITION;
	}
	else if( _stricmp( arg.c_str(), ".BY_DAY_COUNT." ) == 0 )
	{
		type_object->m_enum = IfcRecurrenceTypeEnum::ENUM_BY_DAY_COUNT;
	}
	else if( _stricmp( arg.c_str(), ".BY_WEEKDAY_COUNT." ) == 0 )
	{
		type_object->m_enum = IfcRecurrenceTypeEnum::ENUM_BY_WEEKDAY_COUNT;
	}
	else if( _stricmp( arg.c_str(), ".YEARLY_BY_DAY_OF_MONTH." ) == 0 )
	{
		type_object->m_enum = IfcRecurrenceTypeEnum::ENUM_YEARLY_BY_DAY_OF_MONTH;
	}
	else if( _stricmp( arg.c_str(), ".YEARLY_BY_POSITION." ) == 0 )
	{
		type_object->m_enum = IfcRecurrenceTypeEnum::ENUM_YEARLY_BY_POSITION;
	}
	return type_object;
}

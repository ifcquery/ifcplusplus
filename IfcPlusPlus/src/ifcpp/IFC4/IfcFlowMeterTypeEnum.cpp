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
#include "include/IfcFlowMeterTypeEnum.h"

// TYPE IfcFlowMeterTypeEnum 
IfcFlowMeterTypeEnum::IfcFlowMeterTypeEnum() {}
IfcFlowMeterTypeEnum::~IfcFlowMeterTypeEnum() {}
void IfcFlowMeterTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFLOWMETERTYPEENUM("; }
	if( m_enum == ENUM_ENERGYMETER )
	{
		stream << ".ENERGYMETER.";
	}
	else if( m_enum == ENUM_GASMETER )
	{
		stream << ".GASMETER.";
	}
	else if( m_enum == ENUM_OILMETER )
	{
		stream << ".OILMETER.";
	}
	else if( m_enum == ENUM_WATERMETER )
	{
		stream << ".WATERMETER.";
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
shared_ptr<IfcFlowMeterTypeEnum> IfcFlowMeterTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcFlowMeterTypeEnum>(); }
	shared_ptr<IfcFlowMeterTypeEnum> type_object( new IfcFlowMeterTypeEnum() );
	if( _stricmp( arg.c_str(), ".ENERGYMETER." ) == 0 )
	{
		type_object->m_enum = IfcFlowMeterTypeEnum::ENUM_ENERGYMETER;
	}
	else if( _stricmp( arg.c_str(), ".GASMETER." ) == 0 )
	{
		type_object->m_enum = IfcFlowMeterTypeEnum::ENUM_GASMETER;
	}
	else if( _stricmp( arg.c_str(), ".OILMETER." ) == 0 )
	{
		type_object->m_enum = IfcFlowMeterTypeEnum::ENUM_OILMETER;
	}
	else if( _stricmp( arg.c_str(), ".WATERMETER." ) == 0 )
	{
		type_object->m_enum = IfcFlowMeterTypeEnum::ENUM_WATERMETER;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcFlowMeterTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcFlowMeterTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

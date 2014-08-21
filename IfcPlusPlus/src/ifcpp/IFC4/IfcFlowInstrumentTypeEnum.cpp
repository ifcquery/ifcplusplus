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
#include "include/IfcFlowInstrumentTypeEnum.h"

// TYPE IfcFlowInstrumentTypeEnum = ENUMERATION OF	(PRESSUREGAUGE	,THERMOMETER	,AMMETER	,FREQUENCYMETER	,POWERFACTORMETER	,PHASEANGLEMETER	,VOLTMETER_PEAK	,VOLTMETER_RMS	,USERDEFINED	,NOTDEFINED);
IfcFlowInstrumentTypeEnum::IfcFlowInstrumentTypeEnum() {}
IfcFlowInstrumentTypeEnum::~IfcFlowInstrumentTypeEnum() {}
shared_ptr<IfcPPObject> IfcFlowInstrumentTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFlowInstrumentTypeEnum> copy_self( new IfcFlowInstrumentTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFlowInstrumentTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFLOWINSTRUMENTTYPEENUM("; }
	if( m_enum == ENUM_PRESSUREGAUGE )
	{
		stream << ".PRESSUREGAUGE.";
	}
	else if( m_enum == ENUM_THERMOMETER )
	{
		stream << ".THERMOMETER.";
	}
	else if( m_enum == ENUM_AMMETER )
	{
		stream << ".AMMETER.";
	}
	else if( m_enum == ENUM_FREQUENCYMETER )
	{
		stream << ".FREQUENCYMETER.";
	}
	else if( m_enum == ENUM_POWERFACTORMETER )
	{
		stream << ".POWERFACTORMETER.";
	}
	else if( m_enum == ENUM_PHASEANGLEMETER )
	{
		stream << ".PHASEANGLEMETER.";
	}
	else if( m_enum == ENUM_VOLTMETER_PEAK )
	{
		stream << ".VOLTMETER_PEAK.";
	}
	else if( m_enum == ENUM_VOLTMETER_RMS )
	{
		stream << ".VOLTMETER_RMS.";
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
shared_ptr<IfcFlowInstrumentTypeEnum> IfcFlowInstrumentTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFlowInstrumentTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFlowInstrumentTypeEnum>(); }
	shared_ptr<IfcFlowInstrumentTypeEnum> type_object( new IfcFlowInstrumentTypeEnum() );
	if( boost::iequals( arg, L".PRESSUREGAUGE." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_PRESSUREGAUGE;
	}
	else if( boost::iequals( arg, L".THERMOMETER." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_THERMOMETER;
	}
	else if( boost::iequals( arg, L".AMMETER." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_AMMETER;
	}
	else if( boost::iequals( arg, L".FREQUENCYMETER." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_FREQUENCYMETER;
	}
	else if( boost::iequals( arg, L".POWERFACTORMETER." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_POWERFACTORMETER;
	}
	else if( boost::iequals( arg, L".PHASEANGLEMETER." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_PHASEANGLEMETER;
	}
	else if( boost::iequals( arg, L".VOLTMETER_PEAK." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_VOLTMETER_PEAK;
	}
	else if( boost::iequals( arg, L".VOLTMETER_RMS." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_VOLTMETER_RMS;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFlowInstrumentTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}

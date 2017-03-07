/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
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

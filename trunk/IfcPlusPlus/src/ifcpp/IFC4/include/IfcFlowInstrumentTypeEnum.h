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

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"

// TYPE IfcFlowInstrumentTypeEnum = ENUMERATION OF	(PRESSUREGAUGE	,THERMOMETER	,AMMETER	,FREQUENCYMETER	,POWERFACTORMETER	,PHASEANGLEMETER	,VOLTMETER_PEAK	,VOLTMETER_RMS	,USERDEFINED	,NOTDEFINED);
class IfcFlowInstrumentTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcFlowInstrumentTypeEnumEnum
	{
		ENUM_PRESSUREGAUGE,
		ENUM_THERMOMETER,
		ENUM_AMMETER,
		ENUM_FREQUENCYMETER,
		ENUM_POWERFACTORMETER,
		ENUM_PHASEANGLEMETER,
		ENUM_VOLTMETER_PEAK,
		ENUM_VOLTMETER_RMS,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcFlowInstrumentTypeEnum();
	IfcFlowInstrumentTypeEnum( IfcFlowInstrumentTypeEnumEnum e ) { m_enum = e; }
	~IfcFlowInstrumentTypeEnum();
	virtual const char* classname() const { return "IfcFlowInstrumentTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcFlowInstrumentTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcFlowInstrumentTypeEnumEnum m_enum;
};


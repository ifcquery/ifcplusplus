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

// TYPE IfcFlowMeterTypeEnum = ENUMERATION OF	(ENERGYMETER	,GASMETER	,OILMETER	,WATERMETER	,USERDEFINED	,NOTDEFINED);
class IfcFlowMeterTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcFlowMeterTypeEnumEnum
	{
		ENUM_ENERGYMETER,
		ENUM_GASMETER,
		ENUM_OILMETER,
		ENUM_WATERMETER,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcFlowMeterTypeEnum();
	IfcFlowMeterTypeEnum( IfcFlowMeterTypeEnumEnum e ) { m_enum = e; }
	~IfcFlowMeterTypeEnum();
	virtual const char* classname() const { return "IfcFlowMeterTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcFlowMeterTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcFlowMeterTypeEnumEnum m_enum;
};


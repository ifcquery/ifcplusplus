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

// TYPE IfcActionSourceTypeEnum = ENUMERATION OF	(DEAD_LOAD_G	,COMPLETION_G1	,LIVE_LOAD_Q	,SNOW_S	,WIND_W	,PRESTRESSING_P	,SETTLEMENT_U	,TEMPERATURE_T	,EARTHQUAKE_E	,FIRE	,IMPULSE	,IMPACT	,TRANSPORT	,ERECTION	,PROPPING	,SYSTEM_IMPERFECTION	,SHRINKAGE	,CREEP	,LACK_OF_FIT	,BUOYANCY	,ICE	,CURRENT	,WAVE	,RAIN	,BRAKES	,USERDEFINED	,NOTDEFINED);
class IfcActionSourceTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcActionSourceTypeEnumEnum
	{
		ENUM_DEAD_LOAD_G,
		ENUM_COMPLETION_G1,
		ENUM_LIVE_LOAD_Q,
		ENUM_SNOW_S,
		ENUM_WIND_W,
		ENUM_PRESTRESSING_P,
		ENUM_SETTLEMENT_U,
		ENUM_TEMPERATURE_T,
		ENUM_EARTHQUAKE_E,
		ENUM_FIRE,
		ENUM_IMPULSE,
		ENUM_IMPACT,
		ENUM_TRANSPORT,
		ENUM_ERECTION,
		ENUM_PROPPING,
		ENUM_SYSTEM_IMPERFECTION,
		ENUM_SHRINKAGE,
		ENUM_CREEP,
		ENUM_LACK_OF_FIT,
		ENUM_BUOYANCY,
		ENUM_ICE,
		ENUM_CURRENT,
		ENUM_WAVE,
		ENUM_RAIN,
		ENUM_BRAKES,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcActionSourceTypeEnum();
	IfcActionSourceTypeEnum( IfcActionSourceTypeEnumEnum e ) { m_enum = e; }
	~IfcActionSourceTypeEnum();
	virtual const char* classname() const { return "IfcActionSourceTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcActionSourceTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcActionSourceTypeEnumEnum m_enum;
};


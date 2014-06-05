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

// TYPE IfcRampTypeEnum = ENUMERATION OF	(STRAIGHT_RUN_RAMP	,TWO_STRAIGHT_RUN_RAMP	,QUARTER_TURN_RAMP	,TWO_QUARTER_TURN_RAMP	,HALF_TURN_RAMP	,SPIRAL_RAMP	,USERDEFINED	,NOTDEFINED);
class IfcRampTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcRampTypeEnumEnum
	{
		ENUM_STRAIGHT_RUN_RAMP,
		ENUM_TWO_STRAIGHT_RUN_RAMP,
		ENUM_QUARTER_TURN_RAMP,
		ENUM_TWO_QUARTER_TURN_RAMP,
		ENUM_HALF_TURN_RAMP,
		ENUM_SPIRAL_RAMP,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcRampTypeEnum();
	IfcRampTypeEnum( IfcRampTypeEnumEnum e ) { m_enum = e; }
	~IfcRampTypeEnum();
	virtual const char* classname() const { return "IfcRampTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcRampTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcRampTypeEnumEnum m_enum;
};


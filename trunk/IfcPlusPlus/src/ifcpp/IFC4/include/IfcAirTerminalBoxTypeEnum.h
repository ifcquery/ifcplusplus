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

// TYPE IfcAirTerminalBoxTypeEnum = ENUMERATION OF	(CONSTANTFLOW	,VARIABLEFLOWPRESSUREDEPENDANT	,VARIABLEFLOWPRESSUREINDEPENDANT	,USERDEFINED	,NOTDEFINED);
class IfcAirTerminalBoxTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcAirTerminalBoxTypeEnumEnum
	{
		ENUM_CONSTANTFLOW,
		ENUM_VARIABLEFLOWPRESSUREDEPENDANT,
		ENUM_VARIABLEFLOWPRESSUREINDEPENDANT,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcAirTerminalBoxTypeEnum();
	IfcAirTerminalBoxTypeEnum( IfcAirTerminalBoxTypeEnumEnum e ) { m_enum = e; }
	~IfcAirTerminalBoxTypeEnum();
	virtual const char* classname() const { return "IfcAirTerminalBoxTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcAirTerminalBoxTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcAirTerminalBoxTypeEnumEnum m_enum;
};


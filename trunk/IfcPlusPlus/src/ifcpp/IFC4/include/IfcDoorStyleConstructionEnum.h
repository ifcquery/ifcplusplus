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

// TYPE IfcDoorStyleConstructionEnum = ENUMERATION OF	(ALUMINIUM	,HIGH_GRADE_STEEL	,STEEL	,WOOD	,ALUMINIUM_WOOD	,ALUMINIUM_PLASTIC	,PLASTIC	,USERDEFINED	,NOTDEFINED);
class IfcDoorStyleConstructionEnum : virtual public IfcPPObject
{
public:
	enum IfcDoorStyleConstructionEnumEnum
	{
		ENUM_ALUMINIUM,
		ENUM_HIGH_GRADE_STEEL,
		ENUM_STEEL,
		ENUM_WOOD,
		ENUM_ALUMINIUM_WOOD,
		ENUM_ALUMINIUM_PLASTIC,
		ENUM_PLASTIC,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDoorStyleConstructionEnum();
	IfcDoorStyleConstructionEnum( IfcDoorStyleConstructionEnumEnum e ) { m_enum = e; }
	~IfcDoorStyleConstructionEnum();
	virtual const char* classname() const { return "IfcDoorStyleConstructionEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcDoorStyleConstructionEnum> createObjectFromStepData( const std::string& arg );
	IfcDoorStyleConstructionEnumEnum m_enum;
};


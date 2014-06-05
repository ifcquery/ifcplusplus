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

// TYPE IfcWindowStyleConstructionEnum = ENUMERATION OF	(ALUMINIUM	,HIGH_GRADE_STEEL	,STEEL	,WOOD	,ALUMINIUM_WOOD	,PLASTIC	,OTHER_CONSTRUCTION	,NOTDEFINED);
class IfcWindowStyleConstructionEnum : virtual public IfcPPObject
{
public:
	enum IfcWindowStyleConstructionEnumEnum
	{
		ENUM_ALUMINIUM,
		ENUM_HIGH_GRADE_STEEL,
		ENUM_STEEL,
		ENUM_WOOD,
		ENUM_ALUMINIUM_WOOD,
		ENUM_PLASTIC,
		ENUM_OTHER_CONSTRUCTION,
		ENUM_NOTDEFINED
	};

	IfcWindowStyleConstructionEnum();
	IfcWindowStyleConstructionEnum( IfcWindowStyleConstructionEnumEnum e ) { m_enum = e; }
	~IfcWindowStyleConstructionEnum();
	virtual const char* classname() const { return "IfcWindowStyleConstructionEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcWindowStyleConstructionEnum> createObjectFromStepData( const std::string& arg );
	IfcWindowStyleConstructionEnumEnum m_enum;
};


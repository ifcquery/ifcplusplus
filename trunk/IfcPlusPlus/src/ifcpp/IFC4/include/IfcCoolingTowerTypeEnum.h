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

// TYPE IfcCoolingTowerTypeEnum = ENUMERATION OF	(NATURALDRAFT	,MECHANICALINDUCEDDRAFT	,MECHANICALFORCEDDRAFT	,USERDEFINED	,NOTDEFINED);
class IfcCoolingTowerTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcCoolingTowerTypeEnumEnum
	{
		ENUM_NATURALDRAFT,
		ENUM_MECHANICALINDUCEDDRAFT,
		ENUM_MECHANICALFORCEDDRAFT,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcCoolingTowerTypeEnum();
	IfcCoolingTowerTypeEnum( IfcCoolingTowerTypeEnumEnum e ) { m_enum = e; }
	~IfcCoolingTowerTypeEnum();
	virtual const char* classname() const { return "IfcCoolingTowerTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcCoolingTowerTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcCoolingTowerTypeEnumEnum m_enum;
};


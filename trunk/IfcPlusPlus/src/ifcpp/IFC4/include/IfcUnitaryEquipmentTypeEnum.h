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

// TYPE IfcUnitaryEquipmentTypeEnum = ENUMERATION OF	(AIRHANDLER	,AIRCONDITIONINGUNIT	,DEHUMIDIFIER	,SPLITSYSTEM	,ROOFTOPUNIT	,USERDEFINED	,NOTDEFINED);
class IfcUnitaryEquipmentTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcUnitaryEquipmentTypeEnumEnum
	{
		ENUM_AIRHANDLER,
		ENUM_AIRCONDITIONINGUNIT,
		ENUM_DEHUMIDIFIER,
		ENUM_SPLITSYSTEM,
		ENUM_ROOFTOPUNIT,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcUnitaryEquipmentTypeEnum();
	IfcUnitaryEquipmentTypeEnum( IfcUnitaryEquipmentTypeEnumEnum e ) { m_enum = e; }
	~IfcUnitaryEquipmentTypeEnum();
	virtual const char* classname() const { return "IfcUnitaryEquipmentTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcUnitaryEquipmentTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcUnitaryEquipmentTypeEnumEnum m_enum;
};


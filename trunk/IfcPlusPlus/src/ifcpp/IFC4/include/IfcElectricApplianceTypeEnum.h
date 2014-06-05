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

// TYPE IfcElectricApplianceTypeEnum = ENUMERATION OF	(DISHWASHER	,ELECTRICCOOKER	,FREESTANDINGELECTRICHEATER	,FREESTANDINGFAN	,FREESTANDINGWATERHEATER	,FREESTANDINGWATERCOOLER	,FREEZER	,FRIDGE_FREEZER	,HANDDRYER	,KITCHENMACHINE	,MICROWAVE	,PHOTOCOPIER	,REFRIGERATOR	,TUMBLEDRYER	,VENDINGMACHINE	,WASHINGMACHINE	,USERDEFINED	,NOTDEFINED);
class IfcElectricApplianceTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcElectricApplianceTypeEnumEnum
	{
		ENUM_DISHWASHER,
		ENUM_ELECTRICCOOKER,
		ENUM_FREESTANDINGELECTRICHEATER,
		ENUM_FREESTANDINGFAN,
		ENUM_FREESTANDINGWATERHEATER,
		ENUM_FREESTANDINGWATERCOOLER,
		ENUM_FREEZER,
		ENUM_FRIDGE_FREEZER,
		ENUM_HANDDRYER,
		ENUM_KITCHENMACHINE,
		ENUM_MICROWAVE,
		ENUM_PHOTOCOPIER,
		ENUM_REFRIGERATOR,
		ENUM_TUMBLEDRYER,
		ENUM_VENDINGMACHINE,
		ENUM_WASHINGMACHINE,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcElectricApplianceTypeEnum();
	IfcElectricApplianceTypeEnum( IfcElectricApplianceTypeEnumEnum e ) { m_enum = e; }
	~IfcElectricApplianceTypeEnum();
	virtual const char* classname() const { return "IfcElectricApplianceTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcElectricApplianceTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcElectricApplianceTypeEnumEnum m_enum;
};


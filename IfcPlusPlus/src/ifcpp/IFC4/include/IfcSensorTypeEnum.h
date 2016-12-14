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
#include "ifcpp/model/IfcPPGlobal.h"

// TYPE IfcSensorTypeEnum = ENUMERATION OF	(COSENSOR	,CO2SENSOR	,CONDUCTANCESENSOR	,CONTACTSENSOR	,FIRESENSOR	,FLOWSENSOR	,FROSTSENSOR	,GASSENSOR	,HEATSENSOR	,HUMIDITYSENSOR	,IDENTIFIERSENSOR	,IONCONCENTRATIONSENSOR	,LEVELSENSOR	,LIGHTSENSOR	,MOISTURESENSOR	,MOVEMENTSENSOR	,PHSENSOR	,PRESSURESENSOR	,RADIATIONSENSOR	,RADIOACTIVITYSENSOR	,SMOKESENSOR	,SOUNDSENSOR	,TEMPERATURESENSOR	,WINDSENSOR	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcSensorTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcSensorTypeEnumEnum
	{
		ENUM_COSENSOR,
		ENUM_CO2SENSOR,
		ENUM_CONDUCTANCESENSOR,
		ENUM_CONTACTSENSOR,
		ENUM_FIRESENSOR,
		ENUM_FLOWSENSOR,
		ENUM_FROSTSENSOR,
		ENUM_GASSENSOR,
		ENUM_HEATSENSOR,
		ENUM_HUMIDITYSENSOR,
		ENUM_IDENTIFIERSENSOR,
		ENUM_IONCONCENTRATIONSENSOR,
		ENUM_LEVELSENSOR,
		ENUM_LIGHTSENSOR,
		ENUM_MOISTURESENSOR,
		ENUM_MOVEMENTSENSOR,
		ENUM_PHSENSOR,
		ENUM_PRESSURESENSOR,
		ENUM_RADIATIONSENSOR,
		ENUM_RADIOACTIVITYSENSOR,
		ENUM_SMOKESENSOR,
		ENUM_SOUNDSENSOR,
		ENUM_TEMPERATURESENSOR,
		ENUM_WINDSENSOR,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcSensorTypeEnum();
	IfcSensorTypeEnum( IfcSensorTypeEnumEnum e ) { m_enum = e; }
	~IfcSensorTypeEnum();
	virtual const char* className() const { return "IfcSensorTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcSensorTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcSensorTypeEnumEnum m_enum;
};


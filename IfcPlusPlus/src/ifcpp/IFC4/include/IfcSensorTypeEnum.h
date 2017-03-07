/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/IfcPPBasicTypes.h"
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


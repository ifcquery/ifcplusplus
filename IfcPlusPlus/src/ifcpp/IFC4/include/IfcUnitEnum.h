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

// TYPE IfcUnitEnum = ENUMERATION OF	(ABSORBEDDOSEUNIT	,AMOUNTOFSUBSTANCEUNIT	,AREAUNIT	,DOSEEQUIVALENTUNIT	,ELECTRICCAPACITANCEUNIT	,ELECTRICCHARGEUNIT	,ELECTRICCONDUCTANCEUNIT	,ELECTRICCURRENTUNIT	,ELECTRICRESISTANCEUNIT	,ELECTRICVOLTAGEUNIT	,ENERGYUNIT	,FORCEUNIT	,FREQUENCYUNIT	,ILLUMINANCEUNIT	,INDUCTANCEUNIT	,LENGTHUNIT	,LUMINOUSFLUXUNIT	,LUMINOUSINTENSITYUNIT	,MAGNETICFLUXDENSITYUNIT	,MAGNETICFLUXUNIT	,MASSUNIT	,PLANEANGLEUNIT	,POWERUNIT	,PRESSUREUNIT	,RADIOACTIVITYUNIT	,SOLIDANGLEUNIT	,THERMODYNAMICTEMPERATUREUNIT	,TIMEUNIT	,VOLUMEUNIT	,USERDEFINED);
class IFCPP_EXPORT IfcUnitEnum : virtual public IfcPPObject
{
public:
	enum IfcUnitEnumEnum
	{
		ENUM_ABSORBEDDOSEUNIT,
		ENUM_AMOUNTOFSUBSTANCEUNIT,
		ENUM_AREAUNIT,
		ENUM_DOSEEQUIVALENTUNIT,
		ENUM_ELECTRICCAPACITANCEUNIT,
		ENUM_ELECTRICCHARGEUNIT,
		ENUM_ELECTRICCONDUCTANCEUNIT,
		ENUM_ELECTRICCURRENTUNIT,
		ENUM_ELECTRICRESISTANCEUNIT,
		ENUM_ELECTRICVOLTAGEUNIT,
		ENUM_ENERGYUNIT,
		ENUM_FORCEUNIT,
		ENUM_FREQUENCYUNIT,
		ENUM_ILLUMINANCEUNIT,
		ENUM_INDUCTANCEUNIT,
		ENUM_LENGTHUNIT,
		ENUM_LUMINOUSFLUXUNIT,
		ENUM_LUMINOUSINTENSITYUNIT,
		ENUM_MAGNETICFLUXDENSITYUNIT,
		ENUM_MAGNETICFLUXUNIT,
		ENUM_MASSUNIT,
		ENUM_PLANEANGLEUNIT,
		ENUM_POWERUNIT,
		ENUM_PRESSUREUNIT,
		ENUM_RADIOACTIVITYUNIT,
		ENUM_SOLIDANGLEUNIT,
		ENUM_THERMODYNAMICTEMPERATUREUNIT,
		ENUM_TIMEUNIT,
		ENUM_VOLUMEUNIT,
		ENUM_USERDEFINED
	};

	IfcUnitEnum();
	IfcUnitEnum( IfcUnitEnumEnum e ) { m_enum = e; }
	~IfcUnitEnum();
	virtual const char* className() const { return "IfcUnitEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcUnitEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcUnitEnumEnum m_enum;
};


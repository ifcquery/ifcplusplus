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

// TYPE IfcDistributionSystemEnum = ENUMERATION OF	(AIRCONDITIONING	,AUDIOVISUAL	,CHEMICAL	,CHILLEDWATER	,COMMUNICATION	,COMPRESSEDAIR	,CONDENSERWATER	,CONTROL	,CONVEYING	,DATA	,DISPOSAL	,DOMESTICCOLDWATER	,DOMESTICHOTWATER	,DRAINAGE	,EARTHING	,ELECTRICAL	,ELECTROACOUSTIC	,EXHAUST	,FIREPROTECTION	,FUEL	,GAS	,HAZARDOUS	,HEATING	,LIGHTING	,LIGHTNINGPROTECTION	,MUNICIPALSOLIDWASTE	,OIL	,OPERATIONAL	,POWERGENERATION	,RAINWATER	,REFRIGERATION	,SECURITY	,SEWAGE	,SIGNAL	,STORMWATER	,TELEPHONE	,TV	,VACUUM	,VENT	,VENTILATION	,WASTEWATER	,WATERSUPPLY	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcDistributionSystemEnum : virtual public IfcPPObject
{
public:
	enum IfcDistributionSystemEnumEnum
	{
		ENUM_AIRCONDITIONING,
		ENUM_AUDIOVISUAL,
		ENUM_CHEMICAL,
		ENUM_CHILLEDWATER,
		ENUM_COMMUNICATION,
		ENUM_COMPRESSEDAIR,
		ENUM_CONDENSERWATER,
		ENUM_CONTROL,
		ENUM_CONVEYING,
		ENUM_DATA,
		ENUM_DISPOSAL,
		ENUM_DOMESTICCOLDWATER,
		ENUM_DOMESTICHOTWATER,
		ENUM_DRAINAGE,
		ENUM_EARTHING,
		ENUM_ELECTRICAL,
		ENUM_ELECTROACOUSTIC,
		ENUM_EXHAUST,
		ENUM_FIREPROTECTION,
		ENUM_FUEL,
		ENUM_GAS,
		ENUM_HAZARDOUS,
		ENUM_HEATING,
		ENUM_LIGHTING,
		ENUM_LIGHTNINGPROTECTION,
		ENUM_MUNICIPALSOLIDWASTE,
		ENUM_OIL,
		ENUM_OPERATIONAL,
		ENUM_POWERGENERATION,
		ENUM_RAINWATER,
		ENUM_REFRIGERATION,
		ENUM_SECURITY,
		ENUM_SEWAGE,
		ENUM_SIGNAL,
		ENUM_STORMWATER,
		ENUM_TELEPHONE,
		ENUM_TV,
		ENUM_VACUUM,
		ENUM_VENT,
		ENUM_VENTILATION,
		ENUM_WASTEWATER,
		ENUM_WATERSUPPLY,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDistributionSystemEnum();
	IfcDistributionSystemEnum( IfcDistributionSystemEnumEnum e ) { m_enum = e; }
	~IfcDistributionSystemEnum();
	virtual const char* className() const { return "IfcDistributionSystemEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcDistributionSystemEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcDistributionSystemEnumEnum m_enum;
};


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

// TYPE IfcActionSourceTypeEnum = ENUMERATION OF	(DEAD_LOAD_G	,COMPLETION_G1	,LIVE_LOAD_Q	,SNOW_S	,WIND_W	,PRESTRESSING_P	,SETTLEMENT_U	,TEMPERATURE_T	,EARTHQUAKE_E	,FIRE	,IMPULSE	,IMPACT	,TRANSPORT	,ERECTION	,PROPPING	,SYSTEM_IMPERFECTION	,SHRINKAGE	,CREEP	,LACK_OF_FIT	,BUOYANCY	,ICE	,CURRENT	,WAVE	,RAIN	,BRAKES	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcActionSourceTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcActionSourceTypeEnumEnum
	{
		ENUM_DEAD_LOAD_G,
		ENUM_COMPLETION_G1,
		ENUM_LIVE_LOAD_Q,
		ENUM_SNOW_S,
		ENUM_WIND_W,
		ENUM_PRESTRESSING_P,
		ENUM_SETTLEMENT_U,
		ENUM_TEMPERATURE_T,
		ENUM_EARTHQUAKE_E,
		ENUM_FIRE,
		ENUM_IMPULSE,
		ENUM_IMPACT,
		ENUM_TRANSPORT,
		ENUM_ERECTION,
		ENUM_PROPPING,
		ENUM_SYSTEM_IMPERFECTION,
		ENUM_SHRINKAGE,
		ENUM_CREEP,
		ENUM_LACK_OF_FIT,
		ENUM_BUOYANCY,
		ENUM_ICE,
		ENUM_CURRENT,
		ENUM_WAVE,
		ENUM_RAIN,
		ENUM_BRAKES,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcActionSourceTypeEnum();
	IfcActionSourceTypeEnum( IfcActionSourceTypeEnumEnum e ) { m_enum = e; }
	~IfcActionSourceTypeEnum();
	virtual const char* className() const { return "IfcActionSourceTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcActionSourceTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcActionSourceTypeEnumEnum m_enum;
};


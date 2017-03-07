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

// TYPE IfcValveTypeEnum = ENUMERATION OF	(AIRRELEASE	,ANTIVACUUM	,CHANGEOVER	,CHECK	,COMMISSIONING	,DIVERTING	,DRAWOFFCOCK	,DOUBLECHECK	,DOUBLEREGULATING	,FAUCET	,FLUSHING	,GASCOCK	,GASTAP	,ISOLATING	,MIXING	,PRESSUREREDUCING	,PRESSURERELIEF	,REGULATING	,SAFETYCUTOFF	,STEAMTRAP	,STOPCOCK	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcValveTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcValveTypeEnumEnum
	{
		ENUM_AIRRELEASE,
		ENUM_ANTIVACUUM,
		ENUM_CHANGEOVER,
		ENUM_CHECK,
		ENUM_COMMISSIONING,
		ENUM_DIVERTING,
		ENUM_DRAWOFFCOCK,
		ENUM_DOUBLECHECK,
		ENUM_DOUBLEREGULATING,
		ENUM_FAUCET,
		ENUM_FLUSHING,
		ENUM_GASCOCK,
		ENUM_GASTAP,
		ENUM_ISOLATING,
		ENUM_MIXING,
		ENUM_PRESSUREREDUCING,
		ENUM_PRESSURERELIEF,
		ENUM_REGULATING,
		ENUM_SAFETYCUTOFF,
		ENUM_STEAMTRAP,
		ENUM_STOPCOCK,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcValveTypeEnum();
	IfcValveTypeEnum( IfcValveTypeEnumEnum e ) { m_enum = e; }
	~IfcValveTypeEnum();
	virtual const char* className() const { return "IfcValveTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcValveTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcValveTypeEnumEnum m_enum;
};


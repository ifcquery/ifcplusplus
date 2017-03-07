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

// TYPE IfcCoilTypeEnum = ENUMERATION OF	(DXCOOLINGCOIL	,ELECTRICHEATINGCOIL	,GASHEATINGCOIL	,HYDRONICCOIL	,STEAMHEATINGCOIL	,WATERCOOLINGCOIL	,WATERHEATINGCOIL	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcCoilTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcCoilTypeEnumEnum
	{
		ENUM_DXCOOLINGCOIL,
		ENUM_ELECTRICHEATINGCOIL,
		ENUM_GASHEATINGCOIL,
		ENUM_HYDRONICCOIL,
		ENUM_STEAMHEATINGCOIL,
		ENUM_WATERCOOLINGCOIL,
		ENUM_WATERHEATINGCOIL,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcCoilTypeEnum();
	IfcCoilTypeEnum( IfcCoilTypeEnumEnum e ) { m_enum = e; }
	~IfcCoilTypeEnum();
	virtual const char* className() const { return "IfcCoilTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcCoilTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcCoilTypeEnumEnum m_enum;
};


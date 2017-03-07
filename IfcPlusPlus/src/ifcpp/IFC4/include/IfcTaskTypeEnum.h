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

// TYPE IfcTaskTypeEnum = ENUMERATION OF	(ATTENDANCE	,CONSTRUCTION	,DEMOLITION	,DISMANTLE	,DISPOSAL	,INSTALLATION	,LOGISTIC	,MAINTENANCE	,MOVE	,OPERATION	,REMOVAL	,RENOVATION	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcTaskTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcTaskTypeEnumEnum
	{
		ENUM_ATTENDANCE,
		ENUM_CONSTRUCTION,
		ENUM_DEMOLITION,
		ENUM_DISMANTLE,
		ENUM_DISPOSAL,
		ENUM_INSTALLATION,
		ENUM_LOGISTIC,
		ENUM_MAINTENANCE,
		ENUM_MOVE,
		ENUM_OPERATION,
		ENUM_REMOVAL,
		ENUM_RENOVATION,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcTaskTypeEnum();
	IfcTaskTypeEnum( IfcTaskTypeEnumEnum e ) { m_enum = e; }
	~IfcTaskTypeEnum();
	virtual const char* className() const { return "IfcTaskTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcTaskTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcTaskTypeEnumEnum m_enum;
};


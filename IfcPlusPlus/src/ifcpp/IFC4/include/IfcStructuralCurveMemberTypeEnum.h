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

// TYPE IfcStructuralCurveMemberTypeEnum = ENUMERATION OF	(RIGID_JOINED_MEMBER	,PIN_JOINED_MEMBER	,CABLE	,TENSION_MEMBER	,COMPRESSION_MEMBER	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcStructuralCurveMemberTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcStructuralCurveMemberTypeEnumEnum
	{
		ENUM_RIGID_JOINED_MEMBER,
		ENUM_PIN_JOINED_MEMBER,
		ENUM_CABLE,
		ENUM_TENSION_MEMBER,
		ENUM_COMPRESSION_MEMBER,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcStructuralCurveMemberTypeEnum();
	IfcStructuralCurveMemberTypeEnum( IfcStructuralCurveMemberTypeEnumEnum e ) { m_enum = e; }
	~IfcStructuralCurveMemberTypeEnum();
	virtual const char* className() const { return "IfcStructuralCurveMemberTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcStructuralCurveMemberTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcStructuralCurveMemberTypeEnumEnum m_enum;
};


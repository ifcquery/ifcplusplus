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

// TYPE IfcStairTypeEnum = ENUMERATION OF	(STRAIGHT_RUN_STAIR	,TWO_STRAIGHT_RUN_STAIR	,QUARTER_WINDING_STAIR	,QUARTER_TURN_STAIR	,HALF_WINDING_STAIR	,HALF_TURN_STAIR	,TWO_QUARTER_WINDING_STAIR	,TWO_QUARTER_TURN_STAIR	,THREE_QUARTER_WINDING_STAIR	,THREE_QUARTER_TURN_STAIR	,SPIRAL_STAIR	,DOUBLE_RETURN_STAIR	,CURVED_RUN_STAIR	,TWO_CURVED_RUN_STAIR	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcStairTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcStairTypeEnumEnum
	{
		ENUM_STRAIGHT_RUN_STAIR,
		ENUM_TWO_STRAIGHT_RUN_STAIR,
		ENUM_QUARTER_WINDING_STAIR,
		ENUM_QUARTER_TURN_STAIR,
		ENUM_HALF_WINDING_STAIR,
		ENUM_HALF_TURN_STAIR,
		ENUM_TWO_QUARTER_WINDING_STAIR,
		ENUM_TWO_QUARTER_TURN_STAIR,
		ENUM_THREE_QUARTER_WINDING_STAIR,
		ENUM_THREE_QUARTER_TURN_STAIR,
		ENUM_SPIRAL_STAIR,
		ENUM_DOUBLE_RETURN_STAIR,
		ENUM_CURVED_RUN_STAIR,
		ENUM_TWO_CURVED_RUN_STAIR,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcStairTypeEnum();
	IfcStairTypeEnum( IfcStairTypeEnumEnum e ) { m_enum = e; }
	~IfcStairTypeEnum();
	virtual const char* className() const { return "IfcStairTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcStairTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcStairTypeEnumEnum m_enum;
};


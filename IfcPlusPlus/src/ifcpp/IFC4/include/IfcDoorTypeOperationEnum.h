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

// TYPE IfcDoorTypeOperationEnum = ENUMERATION OF	(SINGLE_SWING_LEFT	,SINGLE_SWING_RIGHT	,DOUBLE_DOOR_SINGLE_SWING	,DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT	,DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT	,DOUBLE_SWING_LEFT	,DOUBLE_SWING_RIGHT	,DOUBLE_DOOR_DOUBLE_SWING	,SLIDING_TO_LEFT	,SLIDING_TO_RIGHT	,DOUBLE_DOOR_SLIDING	,FOLDING_TO_LEFT	,FOLDING_TO_RIGHT	,DOUBLE_DOOR_FOLDING	,REVOLVING	,ROLLINGUP	,SWING_FIXED_LEFT	,SWING_FIXED_RIGHT	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcDoorTypeOperationEnum : virtual public IfcPPObject
{
public:
	enum IfcDoorTypeOperationEnumEnum
	{
		ENUM_SINGLE_SWING_LEFT,
		ENUM_SINGLE_SWING_RIGHT,
		ENUM_DOUBLE_DOOR_SINGLE_SWING,
		ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT,
		ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT,
		ENUM_DOUBLE_SWING_LEFT,
		ENUM_DOUBLE_SWING_RIGHT,
		ENUM_DOUBLE_DOOR_DOUBLE_SWING,
		ENUM_SLIDING_TO_LEFT,
		ENUM_SLIDING_TO_RIGHT,
		ENUM_DOUBLE_DOOR_SLIDING,
		ENUM_FOLDING_TO_LEFT,
		ENUM_FOLDING_TO_RIGHT,
		ENUM_DOUBLE_DOOR_FOLDING,
		ENUM_REVOLVING,
		ENUM_ROLLINGUP,
		ENUM_SWING_FIXED_LEFT,
		ENUM_SWING_FIXED_RIGHT,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDoorTypeOperationEnum();
	IfcDoorTypeOperationEnum( IfcDoorTypeOperationEnumEnum e ) { m_enum = e; }
	~IfcDoorTypeOperationEnum();
	virtual const char* className() const { return "IfcDoorTypeOperationEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcDoorTypeOperationEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcDoorTypeOperationEnumEnum m_enum;
};


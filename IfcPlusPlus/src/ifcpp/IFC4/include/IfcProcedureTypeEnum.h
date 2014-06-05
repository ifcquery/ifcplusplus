/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"

// TYPE IfcProcedureTypeEnum = ENUMERATION OF	(ADVICE_CAUTION	,ADVICE_NOTE	,ADVICE_WARNING	,CALIBRATION	,DIAGNOSTIC	,SHUTDOWN	,STARTUP	,USERDEFINED	,NOTDEFINED);
class IfcProcedureTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcProcedureTypeEnumEnum
	{
		ENUM_ADVICE_CAUTION,
		ENUM_ADVICE_NOTE,
		ENUM_ADVICE_WARNING,
		ENUM_CALIBRATION,
		ENUM_DIAGNOSTIC,
		ENUM_SHUTDOWN,
		ENUM_STARTUP,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcProcedureTypeEnum();
	IfcProcedureTypeEnum( IfcProcedureTypeEnumEnum e ) { m_enum = e; }
	~IfcProcedureTypeEnum();
	virtual const char* classname() const { return "IfcProcedureTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcProcedureTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcProcedureTypeEnumEnum m_enum;
};


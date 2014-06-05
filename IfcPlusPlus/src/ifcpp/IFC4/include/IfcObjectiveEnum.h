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

// TYPE IfcObjectiveEnum = ENUMERATION OF	(CODECOMPLIANCE	,CODEWAIVER	,DESIGNINTENT	,EXTERNAL	,HEALTHANDSAFETY	,MERGECONFLICT	,MODELVIEW	,PARAMETER	,REQUIREMENT	,SPECIFICATION	,TRIGGERCONDITION	,USERDEFINED	,NOTDEFINED);
class IfcObjectiveEnum : virtual public IfcPPObject
{
public:
	enum IfcObjectiveEnumEnum
	{
		ENUM_CODECOMPLIANCE,
		ENUM_CODEWAIVER,
		ENUM_DESIGNINTENT,
		ENUM_EXTERNAL,
		ENUM_HEALTHANDSAFETY,
		ENUM_MERGECONFLICT,
		ENUM_MODELVIEW,
		ENUM_PARAMETER,
		ENUM_REQUIREMENT,
		ENUM_SPECIFICATION,
		ENUM_TRIGGERCONDITION,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcObjectiveEnum();
	IfcObjectiveEnum( IfcObjectiveEnumEnum e ) { m_enum = e; }
	~IfcObjectiveEnum();
	virtual const char* classname() const { return "IfcObjectiveEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcObjectiveEnum> createObjectFromStepData( const std::string& arg );
	IfcObjectiveEnumEnum m_enum;
};


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

// TYPE IfcCoilTypeEnum = ENUMERATION OF	(DXCOOLINGCOIL	,ELECTRICHEATINGCOIL	,GASHEATINGCOIL	,HYDRONICCOIL	,STEAMHEATINGCOIL	,WATERCOOLINGCOIL	,WATERHEATINGCOIL	,USERDEFINED	,NOTDEFINED);
class IfcCoilTypeEnum : virtual public IfcPPObject
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
	virtual const char* classname() const { return "IfcCoilTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcCoilTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcCoilTypeEnumEnum m_enum;
};


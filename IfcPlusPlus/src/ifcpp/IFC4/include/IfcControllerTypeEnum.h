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

// TYPE IfcControllerTypeEnum = ENUMERATION OF	(FLOATING	,PROGRAMMABLE	,PROPORTIONAL	,MULTIPOSITION	,TWOPOSITION	,USERDEFINED	,NOTDEFINED);
class IfcControllerTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcControllerTypeEnumEnum
	{
		ENUM_FLOATING,
		ENUM_PROGRAMMABLE,
		ENUM_PROPORTIONAL,
		ENUM_MULTIPOSITION,
		ENUM_TWOPOSITION,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcControllerTypeEnum();
	IfcControllerTypeEnum( IfcControllerTypeEnumEnum e ) { m_enum = e; }
	~IfcControllerTypeEnum();
	virtual const char* classname() const { return "IfcControllerTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcControllerTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcControllerTypeEnumEnum m_enum;
};


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

// TYPE IfcMechanicalFastenerTypeEnum = ENUMERATION OF	(ANCHORBOLT	,BOLT	,DOWEL	,NAIL	,NAILPLATE	,RIVET	,SCREW	,SHEARCONNECTOR	,STAPLE	,STUDSHEARCONNECTOR	,USERDEFINED	,NOTDEFINED);
class IfcMechanicalFastenerTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcMechanicalFastenerTypeEnumEnum
	{
		ENUM_ANCHORBOLT,
		ENUM_BOLT,
		ENUM_DOWEL,
		ENUM_NAIL,
		ENUM_NAILPLATE,
		ENUM_RIVET,
		ENUM_SCREW,
		ENUM_SHEARCONNECTOR,
		ENUM_STAPLE,
		ENUM_STUDSHEARCONNECTOR,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcMechanicalFastenerTypeEnum();
	IfcMechanicalFastenerTypeEnum( IfcMechanicalFastenerTypeEnumEnum e ) { m_enum = e; }
	~IfcMechanicalFastenerTypeEnum();
	virtual const char* classname() const { return "IfcMechanicalFastenerTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcMechanicalFastenerTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcMechanicalFastenerTypeEnumEnum m_enum;
};


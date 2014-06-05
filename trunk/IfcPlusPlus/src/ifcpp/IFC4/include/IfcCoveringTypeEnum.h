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

// TYPE IfcCoveringTypeEnum = ENUMERATION OF	(CEILING	,FLOORING	,CLADDING	,ROOFING	,MOLDING	,SKIRTINGBOARD	,INSULATION	,MEMBRANE	,SLEEVING	,WRAPPING	,USERDEFINED	,NOTDEFINED);
class IfcCoveringTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcCoveringTypeEnumEnum
	{
		ENUM_CEILING,
		ENUM_FLOORING,
		ENUM_CLADDING,
		ENUM_ROOFING,
		ENUM_MOLDING,
		ENUM_SKIRTINGBOARD,
		ENUM_INSULATION,
		ENUM_MEMBRANE,
		ENUM_SLEEVING,
		ENUM_WRAPPING,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcCoveringTypeEnum();
	IfcCoveringTypeEnum( IfcCoveringTypeEnumEnum e ) { m_enum = e; }
	~IfcCoveringTypeEnum();
	virtual const char* classname() const { return "IfcCoveringTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcCoveringTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcCoveringTypeEnumEnum m_enum;
};


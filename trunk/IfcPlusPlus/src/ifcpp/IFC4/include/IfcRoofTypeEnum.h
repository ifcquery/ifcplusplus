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

// TYPE IfcRoofTypeEnum = ENUMERATION OF	(FLAT_ROOF	,SHED_ROOF	,GABLE_ROOF	,HIP_ROOF	,HIPPED_GABLE_ROOF	,GAMBREL_ROOF	,MANSARD_ROOF	,BARREL_ROOF	,RAINBOW_ROOF	,BUTTERFLY_ROOF	,PAVILION_ROOF	,DOME_ROOF	,FREEFORM	,USERDEFINED	,NOTDEFINED);
class IfcRoofTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcRoofTypeEnumEnum
	{
		ENUM_FLAT_ROOF,
		ENUM_SHED_ROOF,
		ENUM_GABLE_ROOF,
		ENUM_HIP_ROOF,
		ENUM_HIPPED_GABLE_ROOF,
		ENUM_GAMBREL_ROOF,
		ENUM_MANSARD_ROOF,
		ENUM_BARREL_ROOF,
		ENUM_RAINBOW_ROOF,
		ENUM_BUTTERFLY_ROOF,
		ENUM_PAVILION_ROOF,
		ENUM_DOME_ROOF,
		ENUM_FREEFORM,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcRoofTypeEnum();
	IfcRoofTypeEnum( IfcRoofTypeEnumEnum e ) { m_enum = e; }
	~IfcRoofTypeEnum();
	virtual const char* classname() const { return "IfcRoofTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcRoofTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcRoofTypeEnumEnum m_enum;
};


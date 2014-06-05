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

// TYPE IfcGeometricProjectionEnum = ENUMERATION OF	(GRAPH_VIEW	,SKETCH_VIEW	,MODEL_VIEW	,PLAN_VIEW	,REFLECTED_PLAN_VIEW	,SECTION_VIEW	,ELEVATION_VIEW	,USERDEFINED	,NOTDEFINED);
class IfcGeometricProjectionEnum : virtual public IfcPPObject
{
public:
	enum IfcGeometricProjectionEnumEnum
	{
		ENUM_GRAPH_VIEW,
		ENUM_SKETCH_VIEW,
		ENUM_MODEL_VIEW,
		ENUM_PLAN_VIEW,
		ENUM_REFLECTED_PLAN_VIEW,
		ENUM_SECTION_VIEW,
		ENUM_ELEVATION_VIEW,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcGeometricProjectionEnum();
	IfcGeometricProjectionEnum( IfcGeometricProjectionEnumEnum e ) { m_enum = e; }
	~IfcGeometricProjectionEnum();
	virtual const char* classname() const { return "IfcGeometricProjectionEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcGeometricProjectionEnum> createObjectFromStepData( const std::string& arg );
	IfcGeometricProjectionEnumEnum m_enum;
};


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

// TYPE IfcBSplineSurfaceForm = ENUMERATION OF	(PLANE_SURF	,CYLINDRICAL_SURF	,CONICAL_SURF	,SPHERICAL_SURF	,TOROIDAL_SURF	,SURF_OF_REVOLUTION	,RULED_SURF	,GENERALISED_CONE	,QUADRIC_SURF	,SURF_OF_LINEAR_EXTRUSION	,UNSPECIFIED);
class IfcBSplineSurfaceForm : virtual public IfcPPObject
{
public:
	enum IfcBSplineSurfaceFormEnum
	{
		ENUM_PLANE_SURF,
		ENUM_CYLINDRICAL_SURF,
		ENUM_CONICAL_SURF,
		ENUM_SPHERICAL_SURF,
		ENUM_TOROIDAL_SURF,
		ENUM_SURF_OF_REVOLUTION,
		ENUM_RULED_SURF,
		ENUM_GENERALISED_CONE,
		ENUM_QUADRIC_SURF,
		ENUM_SURF_OF_LINEAR_EXTRUSION,
		ENUM_UNSPECIFIED
	};

	IfcBSplineSurfaceForm();
	IfcBSplineSurfaceForm( IfcBSplineSurfaceFormEnum e ) { m_enum = e; }
	~IfcBSplineSurfaceForm();
	virtual const char* classname() const { return "IfcBSplineSurfaceForm"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcBSplineSurfaceForm> createObjectFromStepData( const std::string& arg );
	IfcBSplineSurfaceFormEnum m_enum;
};


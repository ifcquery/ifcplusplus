/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
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

// uncomment the following line, if you want to use Carve for boolean operations in geometric representations
#define GEOM_USE_CARVE

#ifdef GEOM_USE_CARVE
	#include <ifcpp/geometry/Carve/GeometryConverter.h>
	#include <ifcpp/geometry/Carve/GeomUtils.h>
	#include <ifcpp/geometry/Carve/SceneGraphUtils.h>
	#include <ifcpp/geometry/Carve/GeometrySettings.h>
#else
	// defaulting to OpenCasade
	#include <ifcpp/geometry/OCC/GeometryConverter.h>
	#include <ifcpp/geometry/OCC/GeomUtils.h>
	#include <ifcpp/geometry/OCC/SceneGraphUtils.h>
	#include <ifcpp/geometry/OCC/GeometrySettings.h>
#endif

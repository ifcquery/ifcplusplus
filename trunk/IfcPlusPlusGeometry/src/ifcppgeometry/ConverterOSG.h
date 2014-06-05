/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
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

#include <osg/Matrix>
#include <osg/Geode>
#include <ifcpp/model/shared_ptr.h>
#include "IncludeCarveHeaders.h"

class AppearanceData;

class ConverterOSG
{
public:
	static void drawFace(		const carve::mesh::Face<3>* face,					osg::Geode* geode, bool add_color_array = false );
	static void drawMesh(		const carve::mesh::Mesh<3>* mesh,					osg::Geode* geode, bool add_color_array = false );
	static void drawOpenMesh(	const carve::input::PolyhedronData* poly_data,		osg::Geode* geode, bool add_color_array = false );
	static void drawMeshSet(	const carve::mesh::MeshSet<3>* mesh_set,			osg::Geode* geode, bool add_color_array = false );
	static void drawPolyhedron( const carve::poly::Polyhedron* polyhedron,			osg::Geode* geode, bool add_color_array = false );
	static void drawPolyline(	const carve::input::PolylineSetData* polyline_data, osg::Geode* geode, bool add_color_array = false );
	static double computeSurfaceAreaOfGroup( const osg::Group* grp );
};

class AppearanceManagerOSG
{
public:
	//AppearanceManagerOSG();
	//~AppearanceManagerOSG();
	static void clearAppearanceCache();
	static osg::StateSet* convertToStateSet( shared_ptr<AppearanceData>& appearence );
	
};

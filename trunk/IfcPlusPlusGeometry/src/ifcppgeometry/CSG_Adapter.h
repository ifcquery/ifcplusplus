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

#include "IncludeCarveHeaders.h"
#include <ifcpp/model/shared_ptr.h>

typedef carve::mesh::Edge<3> edge_t;
typedef carve::mesh::Face<3> face_t;

class CSG_Adapter
{
public:
	CSG_Adapter();
	~CSG_Adapter();

	static void simplifyMesh( shared_ptr<carve::mesh::MeshSet<3> >& meshset );
	static bool computeCSG( shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation, 
		const int entity1, const int entity2, std::stringstream& err, shared_ptr<carve::mesh::MeshSet<3> >& result );
	//static bool checkMeshSetEmpty(	const carve::mesh::MeshSet<3>* meshset );
	static bool checkMeshSetValidAndClosed(	const carve::mesh::MeshSet<3>* meshset, std::stringstream& err_poly, int entity_id );
	static bool checkFaceIntegrity(	const carve::mesh::MeshSet<3>* meshset );
};

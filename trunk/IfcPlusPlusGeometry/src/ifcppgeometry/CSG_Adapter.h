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

	static void simplifyMesh( shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool triangulate );
	static void retriangulateMeshSet( shared_ptr<carve::mesh::MeshSet<3> >& meshset );
	static void computeCSG( shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation, 
		const int entity1, const int entity2, std::stringstream& err, shared_ptr<carve::mesh::MeshSet<3> >& result );
	static bool checkMeshSetValidAndClosed(	const carve::mesh::MeshSet<3>* meshset, std::stringstream& err_poly, int entity_id );
	static bool checkFaceIntegrity(	const carve::mesh::MeshSet<3>* meshset );

#ifdef _DEBUG
	static void dumpPolyhedronInput( carve::input::PolyhedronData& poly_input, carve::geom::vector<4>& color, bool append );
	static void dumpPolyhedron( carve::poly::Polyhedron* poly, carve::geom::vector<4>& color, bool append );
	static void dumpMeshset( carve::mesh::MeshSet<3>* meshset, carve::geom::vector<4>& color, bool append );
	static void dumpMeshsets( std::vector<carve::mesh::MeshSet<3>* >& meshset, std::vector<carve::geom::vector<4> >& vec_colors, bool append );
	static void dumpFaces( const carve::mesh::MeshSet<3>* meshset, std::vector<face_t* >& vec_faces );
	static void dumpEdges( const carve::mesh::MeshSet<3>* meshset, std::vector<edge_t* >& vec_edges );
	static void clearMeshsetDump();
#endif
};

class MeshOps
{
public:
	static void applyTranslate( carve::mesh::MeshSet<3>* meshset, const carve::geom::vector<3>& pos );
	static void roundVertices( carve::mesh::MeshSet<3>* meshset );
	static int getNumFaces( const carve::mesh::MeshSet<3>* meshset );
	static int getNumClosedEdges( const carve::mesh::MeshSet<3>* meshset );
	static double getVolume( const carve::mesh::MeshSet<3>* meshset );
};

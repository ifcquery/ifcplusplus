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

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/StatusCallback.h>
#include "IncludeCarveHeaders.h"

typedef carve::mesh::Edge<3> edge_t;
typedef carve::mesh::Face<3> face_t;
typedef carve::mesh::MeshSet<3> meshset_t;

class CSG_Adapter : public StatusCallback
{
public:
	CSG_Adapter();
	~CSG_Adapter();

	static void simplifyMesh( shared_ptr<meshset_t >& meshset, bool triangulate );
	static void retriangulateMeshSet( shared_ptr<meshset_t >& meshset );
	static void computeCSG( shared_ptr<meshset_t >& op1, shared_ptr<meshset_t >& op2, const carve::csg::CSG::OP operation, 
		const int entity1, const int entity2, shared_ptr<meshset_t >& result );
	static bool checkMeshSetValidAndClosed(	const shared_ptr<meshset_t> meshset, std::stringstream& err_poly, int entity_id );
	static bool checkFaceIntegrity(	const meshset_t* meshset );

	static void applyTranslate( meshset_t* meshset, const carve::geom::vector<3>& pos );
	static void roundVertices( meshset_t* meshset );
	static int getNumFaces( const meshset_t* meshset );
	static int getNumClosedEdges( const meshset_t* meshset );
	static double getVolume( const meshset_t* meshset );

#ifdef _DEBUG
	static void dumpPolyhedronInput( carve::input::PolyhedronData& poly_input, carve::geom::vector<4>& color, bool append );
	static void dumpPolyhedron( carve::poly::Polyhedron* poly, carve::geom::vector<4>& color, bool append );
	static void dumpMeshset( meshset_t* meshset, carve::geom::vector<4>& color, bool append );
	static void dumpMeshsets( std::vector<meshset_t* >& meshset, std::vector<carve::geom::vector<4> >& vec_colors, bool append );
	static void dumpFaces( const meshset_t* meshset, std::vector<face_t* >& vec_faces );
	static void dumpEdges( const meshset_t* meshset, std::vector<edge_t* >& vec_edges );
	static void dumpPolylineSet( carve::input::PolylineSetData* polyline, carve::geom::vector<4>& color, bool append );
	static void clearMeshsetDump();
#endif
};

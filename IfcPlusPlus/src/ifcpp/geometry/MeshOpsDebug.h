/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include "MeshUtils.h"

#ifdef _DEBUG
static size_t numM6 = 0;
static size_t dumpFaceCount = 0;
static int mergedFacesDumpCount = 0;
static size_t numPointsAtCorner = 0;

struct DumpSettingsStruct
{
	DumpSettingsStruct()
	{
		
	}
	glm::vec4 colorLabel = glm::vec4(0.3, 0.33, 0.33, 1.);
	glm::vec4 colorMesh = glm::vec4(0.4, 0.5, 0.4, 0.4);
	glm::vec4 colorOpenEdges = glm::vec4(0.99, 0.2, 0.2, 0.4);
	glm::vec4 colorValidMesh = glm::vec4(0.3, 0.8, 0.3, 0.4);
	bool triangulateBeforeDump = true;
};

static void getFaceIndexes(const carve::mesh::Face<3>* face, const std::vector<carve::mesh::Vertex<3> >& vec_vertices, std::vector<int>& result)
{
	carve::mesh::Edge<3>* edge = face->edge;
	for( size_t i3 = 0; i3 < face->n_edges; ++i3 )
	{
		const carve::mesh::Vertex<3>* v = edge->vert;
		//const vec3& v = edge->vert->v;
		//static int findVertexIndex(const std::vector<carve::mesh::Vertex<3> >&vec_vertices, const carve::mesh::Vertex<3>*v)
		int idx = GeomDebugDump::findVertexIndexInVector(vec_vertices, v);
		result.push_back(idx);
		edge = edge->next;
	}
}

static void getFaceIndexes(const carve::mesh::Face<3>* face, const std::vector<carve::mesh::Vertex<3> >& vec_vertices, std::set<int>& result)
{
	carve::mesh::Edge<3>* edge = face->edge;
	for( size_t i3 = 0; i3 < face->n_edges; ++i3 )
	{
		const carve::mesh::Vertex<3>* v = edge->vert;
		int idx = GeomDebugDump::findVertexIndexInVector(vec_vertices, v);
		result.insert(idx);
		edge = edge->next;
	}
}

static bool GetPolygonNormalAndArea( const std::vector<glm::dvec3>& polygon, glm::dvec3& normal, double& area )
{
	normal = glm::dvec3();
	area = 0.0;
	int n = polygon.size();
	if( n < 3 )
	{
		return false;
	}

	if( 3 == n )
	{
		glm::dvec3 a = polygon[0];
		glm::dvec3 b = polygon[1];
		glm::dvec3 c = polygon[2];
		glm::dvec3 v = b - a;
		normal = cross( v, ( c - a ) );
	}
	else if( 4 == n )
	{
		glm::dvec3 a = polygon[0];
		glm::dvec3 b = polygon[1];
		glm::dvec3 c = polygon[2];
		glm::dvec3 d = polygon[3];

		normal.x = ( c.y - a.y ) * ( d.z - b.z ) + ( c.z - a.z ) * ( b.y - d.y );
		normal.y = ( c.z - a.z ) * ( d.x - b.x ) + ( c.x - a.x ) * ( b.z - d.z );
		normal.z = ( c.x - a.x ) * ( d.y - b.y ) + ( c.y - a.y ) * ( b.x - d.x );

	}
	else if( 4 < n )
	{
		glm::dvec3 a;
		glm::dvec3 b = polygon[n - 2];
		glm::dvec3 c = polygon[n - 1];
		glm::dvec3 s;

		for( int i = 0; i < n; ++i ) {
			a = b;
			b = c;
			c = polygon[i];

			normal.x += b.y * (c.z - a.z);
			normal.y += b.z * (c.x - a.x);
			normal.z += b.x * (c.y - a.y);

			s += c;
		}
	}

	double length = glm::length( normal );
	if( std::abs(length) < EPS_M8 )
	{
		return false;
	}

	normal /= length;
	area = 0.5 * length;
	return true;
}

static int debug_poly_dump_count = 0;
namespace MeshOps
{
	static void retriangulateMeshSetSimple(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, double eps, size_t retryCount);
};

inline void dumpMeshset(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, const glm::vec4& color, bool move_offset = true)
{
	if( meshset->meshes.size() == 0 )
	{
		return;
	}

	bool checkForDegenerateEdges = false;
	MeshSetInfo info;
	MeshUtils::checkMeshSetPointers(meshset, checkForDegenerateEdges, info);
	if( !info.allPointersValid )
	{
		return;
	}
	
	vec3 offset = carve::geom::VECTOR(0, GeomDebugDump::dump_y_pos_geom, 0);
	shared_ptr<carve::mesh::MeshSet<3> > meshset_copy(meshset->clone());
	double eps = EPS_M8;
	MeshOps::retriangulateMeshSetSimple(meshset_copy, true, eps, 0);
	GeomDebugDump::dumpMeshset(meshset_copy.get(), offset, color);

	if( move_offset )
	{
		GeomDebugDump::dump_y_pos_geom += meshset->getAABB().extent.y * 2.2;
	}
}

static void dumpWithLabel(std::string labelStr, shared_ptr<carve::mesh::MeshSet<3> >& meshset, const DumpSettingsStruct& colorSettings, bool checkZeroAreaFaces, bool moveOffsetBefore, bool moveOffsetAfter)
{
	if( !meshset )
	{
		return;
	}

	auto aabb = meshset->getAABB();
	if( !aabb.isEmpty() )
	{
		double minX = aabb.pos.x - aabb.extent.x - 0.08;
		double maxX = aabb.pos.x + aabb.extent.x;
		double minZ = aabb.pos.z - aabb.extent.z;
		if( minX < GeomDebugDump::labelPos.x )
		{
			GeomDebugDump::labelPos = carve::geom::VECTOR(minX, 0, minZ);
			GeomDebugDump::countLabelPos = carve::geom::VECTOR(maxX + 0.3, 0, minZ);
		}
	}

	if( moveOffsetBefore )
	{
		GeomDebugDump::moveOffset(0.4);
	}
	
	shared_ptr<carve::mesh::MeshSet<3> > meshset_copy( meshset );
	if( colorSettings.triangulateBeforeDump )
	{
		//meshset_copy = shared_ptr<carve::mesh::MeshSet<3> > (meshset->clone());
		//MeshOps::retriangulateMeshSetSimple(meshset_copy, true, EPS_DEFAULT, 0);
	}

	MeshSetInfo infoMeshset;
	bool meshset_ok = MeshUtils::checkMeshSetValidAndClosed(meshset, infoMeshset, EPS_DEFAULT);

	glm::vec4 colorCurrentLabel = colorSettings.colorLabel;
	if( infoMeshset.meshSetValid )
	{
		colorCurrentLabel = colorSettings.colorValidMesh;
		labelStr += ", valid: yes";
	}
	else
	{
		colorCurrentLabel = colorSettings.colorOpenEdges;
		labelStr += ", valid: no, " + std::to_string(infoMeshset.numOpenEdges) + "_open_edges";

		if( infoMeshset.numOpenEdges == 4 )
		{
			int wait = 0;
		}
	}
	if( GeomDebugDump::dumpCount >= 7 )
	{
		int wait = 0;
	}
	if( GeomDebugDump::dumpCount % 10 == 0 )
	{
		//GeomDebugDump::clearMeshsetDump();
	}
	GeomDebugDump::dumpVertex(GeomDebugDump::labelPos, colorCurrentLabel, labelStr);
	GeomDebugDump::dumpCountLabel(GeomDebugDump::countLabelPos);
	GeomDebugDump::dumpMeshset(meshset_copy, colorSettings.colorMesh, false);

	bool moveOffset = false;
	if( moveOffsetAfter )
	{
		GeomDebugDump::moveOffset(0.2);
		moveOffset = true;
	}
	GeomDebugDump::dumpMeshsetOpenEdges(meshset_copy, colorSettings.colorOpenEdges, checkZeroAreaFaces, moveOffset);
}
#endif

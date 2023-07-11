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

#include <ifcpp/model/BasicTypes.h>
#include "GeomUtils.h"
#include <ifcpp/model/StatusCallback.h>
#include "GeometryInputData.h"

class CarveMeshNormalizer
{
private:
	double m_scale = 1.0;
	vec3 m_normalizeCenter;
	std::map<std::string, carve::mesh::MeshSet<3>* > m_normalizedMeshes;

public:
	bool m_disableNormalizeAll = false;
	bool m_normalizeCoordsInsteadOfEpsilon = false;

	CarveMeshNormalizer(bool normalizeCoordsInsteadOfEpsilon)
	{
		m_normalizeCoordsInsteadOfEpsilon = normalizeCoordsInsteadOfEpsilon;
	}

	CarveMeshNormalizer(const CarveMeshNormalizer& other)
	{
		this->m_disableNormalizeAll = other.m_disableNormalizeAll;
		this->m_scale = other.m_scale;
		this->m_normalizeCenter = other.m_normalizeCenter;
		//this->m_normalizedMeshes = other->m_normalizedMeshes;
		this->m_normalizeCoordsInsteadOfEpsilon = other.m_normalizeCoordsInsteadOfEpsilon;
	}

	CarveMeshNormalizer(const carve::geom::aabb<3>& bbox1, const carve::geom::aabb<3>& bbox2, bool normalizeCoordsInsteadOfEpsilon)
	{
		m_normalizeCoordsInsteadOfEpsilon = normalizeCoordsInsteadOfEpsilon;
		vec3 center1 = bbox1.pos;
		vec3 extents1 = bbox1.extent;

		if (bbox2.isEmpty())
		{
			return;
		}

		vec3 center2 = bbox2.pos;
		vec3 extents2 = bbox2.extent;

		double extent1 = std::max(extents1.x, std::max(extents1.y, extents1.z));
		double extent2 = std::max(extents2.x, std::max(extents2.y, extents2.z));
		double maxExtent = std::max(extent1, extent2);

		double scale = 1.0;
		if (maxExtent > EPS_M5)
		{
			scale = 1.0 / maxExtent;
		}
		m_scale = scale*0.8;  // conversative approach: 80%

		vec3 center = (center1 + center2) * 0.5;
		m_normalizeCenter = center;

		if (std::abs(m_scale) > 0.5 && std::abs(m_scale) < 2.0)
		{
			// if scale is close to 1, scaling does not make a big difference. 
			// Negative effects like small inaccuracies after multiplication and time to multiply overweigh
			m_scale = 1.0;
		}
	}

	double getScale() const { return m_scale; }
	void setToZero()
	{
		m_scale = 1.0;
		m_normalizeCenter.setZero();
	}

	void normalizeMesh(shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::string tag, double eps)
	{
		if (m_disableNormalizeAll)
		{
			return;
		}
		double centerLength2 = m_normalizeCenter.length2();
		if (m_scale == 1.0 && centerLength2 < 2.0)
		{
			return;
		}

		if (tag.size() > 0)
		{
			if (m_normalizedMeshes.find(tag) != m_normalizedMeshes.end())
			{
				// already normalized
				return;
			}
		}

		std::vector<carve::mesh::Vertex<3> >& vertex_storage = meshset->vertex_storage;
		const size_t num_vertices = vertex_storage.size();

		for (size_t i = 0; i < num_vertices; ++i)
		{
			carve::mesh::Vertex<3>& vertex = vertex_storage[i];
			vec3& point = vertex.v;

			if (m_scale != 1.0 && m_normalizeCoordsInsteadOfEpsilon)
			{
				point = (point - m_normalizeCenter) * m_scale;
			}
			else
			{
				point = (point - m_normalizeCenter);
			}
		}

		for (size_t kk = 0; kk < meshset->meshes.size(); ++kk)
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			mesh->recalc(eps);
		}

		m_normalizedMeshes.insert({ tag, meshset.get() });
	}

	void deNormalizeMesh(shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::string tag, double eps)
	{
		if (m_disableNormalizeAll)
		{
			return;
		}
		double centerLength2 = m_normalizeCenter.length2();
		if (m_scale == 1.0 && centerLength2 < 2.0)
		{
			return;
		}

		if (tag.size() > 0)
		{
			auto it = m_normalizedMeshes.find(tag);
			if (it == m_normalizedMeshes.end())
			{
				// already denormalized, or not normalized before
				return;
			}
			m_normalizedMeshes.erase(it);
		}

		double unScaleFactor = (1.0 / m_scale);

		std::vector<carve::mesh::Vertex<3> >& vertex_storage = meshset->vertex_storage;
		const size_t num_vertices = vertex_storage.size();
		for (size_t i = 0; i < num_vertices; ++i)
		{
			carve::mesh::Vertex<3>& vertex = vertex_storage[i];
			vec3& point = vertex.v;
			if (m_scale != 1.0 && m_normalizeCoordsInsteadOfEpsilon)
			{
				point = point * unScaleFactor + m_normalizeCenter;
			}
			else
			{
				point = point + m_normalizeCenter;
			}
		}
		for (size_t kk = 0; kk < meshset->meshes.size(); ++kk)
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			mesh->recalc(eps);
		}
	}
};

class MeshOps
{
public:
	static double computeFaceArea(const carve::mesh::Face<3>* face);
	static double computeFaceArea(const carve::mesh::Face<3>* face, double& longestEdge);
	static size_t getNumFaces(const carve::mesh::MeshSet<3>* meshset);
	static void recalcMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, double CARVE_EPSILON);
	static void checkFaceLoops(const carve::mesh::Face<3>* face);
	static size_t countFaces(carve::mesh::MeshSet<3>* mesh);
	static size_t countDegeneratedFaces(carve::mesh::MeshSet<3>* mesh);
					
	static void retriangulateMeshSetForBoolOp(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, const GeomProcessingParams& params, size_t retryCount);
	static void retriangulateMeshSetForBoolOp_carve(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, const GeomProcessingParams& params, size_t retryCount);
	static void retriangulateMeshSetForBoolOp_earcut(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, const GeomProcessingParams& params, size_t retryCount);
	
	/// \brief retriangulateMeshSetForExport: triangulate mesh, with priority of not skipping triangles, mesh does not need to be closed and valid for boolean operations
	static void retriangulateMeshSetForExport(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params);

	static void intersectOpenEdgesWithPoints(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params);

	static void intersectOpenEdgesWithEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params);

	static bool isBetterForBoolOp(const MeshSetInfo& infoNew, const MeshSetInfo& infoBefore, bool considerTriangulation);

	static bool checkMeshSetValidAndClosed(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, const GeomProcessingParams& params );

	static bool checkMeshSetNonNegativeAndClosed(const shared_ptr<carve::mesh::MeshSet<3>> mesh_set, const GeomProcessingParams& params);

	static void checkMeshPointers(const carve::mesh::Mesh<3>* mesh, bool checkForDegenerateEdges, const GeomProcessingParams& params, MeshSetInfo& info);

	static void checkMeshSetPointers(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool checkForDegenerateEdges, const GeomProcessingParams& params, MeshSetInfo& info);

	static void resolveOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, const GeomProcessingParams& params);

	static void flattenFacePlanes(shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const GeomProcessingParams& params);

	static void removeDegenerateMeshes(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& params, bool ensureValidMesh);
	static void removeDegenerateFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& params, bool ensureValidMesh);

	static vec3 computeFaceNormal(carve::mesh::Face<3>* face);

	static double computeMeshsetVolume(const carve::mesh::MeshSet<3>* meshset);
	static double computeMeshSetSurface(const shared_ptr<carve::mesh::MeshSet<3> >& meshset);

	static double computeShapeSurfaceArea(const shared_ptr<ProductShapeData>& shape_input_data);
	static double computeShapeSurfaceArea(const shared_ptr<ItemShapeData>& item_data);

	static std::shared_ptr<carve::mesh::MeshSet<3> > createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, double CARVE_EPSILON);
	static std::shared_ptr<carve::mesh::MeshSet<3> > createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, vec3& p3, double CARVE_EPSILON);
	static std::shared_ptr<carve::mesh::MeshSet<3> > createBoxMesh(vec3& pos, vec3& extent, carve::math::Matrix& transform, double CARVE_EPSILON);
	static void boundingBox2Mesh(carve::geom::aabb<3>& bbox, shared_ptr<carve::mesh::MeshSet<3> >& meshset, double CARVE_EPSILON);

/// \brief simplifyMeshSet merge coplanar faces and re-triangulate each set of merged faces
/// \param meshset				Carve meshset
/// \param report_callback		callback function for errors, warnings, notifications, progress
/// \param entity				IFC entity that is currently being processed
/// \param ignoreOpenEdgesInResult	If true, the result is kept even with open edges (good for visualization). If false, the result will be the input mesh in case open edges occur after triangulation (good for further boolean operations)
	static void simplifyMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params, bool triangulateResult, bool shouldBeClosedManifold);

};

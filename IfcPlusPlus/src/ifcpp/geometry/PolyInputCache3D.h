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

#include <vector>
#include <unordered_set>
#include "IncludeCarveHeaders.h"

// hash function for vec3
static uint32_t Vec3Hash(const vec3& v, double epsilon) {
	//  uint32_t is enough for hash. 
	uint32_t hx = (uint32_t)std::hash<double>{}(v.x / epsilon);
	uint32_t hy = (uint32_t)std::hash<double>{}(v.y / epsilon);
	uint32_t hz = (uint32_t)std::hash<double>{}(v.z / epsilon);
	return (uint32_t)hx ^ (hy << 1) ^ (hz << 2);
}


// equality function for vec3
static bool Vec3Equal(const vec3& lhs, const vec3& rhs, double epsilon) {
	return std::fabs(lhs.x - rhs.x) < epsilon &&
		std::fabs(lhs.y - rhs.y) < epsilon &&
		std::fabs(lhs.z - rhs.z) < epsilon;
}

class PolyInputCache3D {
public:

	// uint32 max value is 14757395217560929096, this should be enough for indexing points
	std::unordered_map<uint32_t, uint32_t> m_hashToIndexMap;
	shared_ptr<carve::input::PolyhedronData> m_poly_data;
	double epsilon;

	PolyInputCache3D(double eps = 1e-6) : epsilon(eps) {
		m_poly_data = shared_ptr<carve::input::PolyhedronData>(new carve::input::PolyhedronData());
	}

	// Adds a point to the cache. Returns the index of the existing or newly inserted point.
	uint32_t addPoint(const vec3& pt) {
		uint32_t hash = Vec3Hash(pt, epsilon);
		std::vector<vec3>& pointList = m_poly_data->points;

		auto it = m_hashToIndexMap.find(hash);
		if (it != m_hashToIndexMap.end()) {
			// If the hash exists, check if the point matches (in case of hash collision)
			uint32_t index = it->second;
			if (Vec3Equal(pointList[index], pt, epsilon)) {
				return index;
			}
		}

		// Otherwise, add the new point
		uint32_t newIndex = static_cast<uint32_t>(pointList.size());
		pointList.push_back(pt);
		m_hashToIndexMap[hash] = newIndex;

		return newIndex;
	}

	void clearPointCache()
	{
		m_hashToIndexMap.clear();
		m_poly_data->points.clear();
	}

	void copyOtherPolyData(shared_ptr<carve::input::PolyhedronData>& other)
	{
		shared_ptr<carve::mesh::MeshSet<3> > meshset(other->createMesh(carve::input::opts(), epsilon));

		for (size_t i = 0; i < meshset->meshes.size(); ++i)
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[i];
			for (size_t j = 0; j < mesh->faces.size(); ++j)
			{
				carve::mesh::Face<3>* face = mesh->faces[j];
				carve::mesh::Edge<3>* edge = face->edge;
				std::vector<int> faceIndices;
				for (size_t k = 0; k < face->nVertices(); ++k)
				{
					const vec3& vertex = edge->vert->v;
					int pointIndex = addPoint(vertex);
					faceIndices.push_back(pointIndex);
					edge = edge->next;
				}

				m_poly_data->addFace(faceIndices.begin(), faceIndices.end());
			}
		}
	}

	void addFaceCheckIndexes(uint32_t idxA, uint32_t idxB, uint32_t idxC, uint32_t idxD,
		const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3, double eps)
	{
		std::unordered_set<uint32_t> setIndices = { idxA, idxB, idxC, idxD };

		if (setIndices.size() == 3)
		{
			auto it = setIndices.begin();
			idxA = *it;
			++it;
			idxB = *it;
			++it;
			idxC = *it;
			m_poly_data->addFace(idxA, idxB, idxC);
			return;
		}

		addTriangleCheckDegenerate(idxA, idxB, idxC, v0, v1, v2, eps);
		addTriangleCheckDegenerate(idxA, idxC, idxD, v0, v2, v3, eps);
	}

	void addFaceCheckIndexes(const vec3& v0, const vec3& v1, const vec3& v2, double eps)
	{
		uint32_t idxA = addPoint(v0);
		uint32_t idxB = addPoint(v1);
		uint32_t idxC = addPoint(v2);
		addTriangleCheckDegenerate(idxA, idxB, idxC, v0, v1, v2, eps);
	}

	void addFaceCheckIndexes(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3, double eps)
	{
		uint32_t idxA = addPoint(v0);
		uint32_t idxB = addPoint(v1);
		uint32_t idxC = addPoint(v2);
		uint32_t idxD = addPoint(v3);

		addTriangleCheckDegenerate(idxA, idxB, idxC, v0, v1, v2, eps);
		addTriangleCheckDegenerate(idxA, idxC, idxD, v0, v2, v3, eps);
	}

	void addTriangleCheckDegenerate(uint32_t idxA, uint32_t idxB, uint32_t idxC,
		const vec3& pointA, const vec3& pointB, const vec3& pointC, double eps)
	{
		if (idxA == idxB || idxA == idxC || idxB == idxC)
		{
#ifdef _DEBUG
			std::cout << "skipping degenerate triangle: " << idxA << "/" << idxB << "/" << idxC << std::endl;
#endif
			return;
		}

		double lengthAB = (pointB - pointA).length2();
		if (lengthAB < eps * eps * 10)
		{
#ifdef _DEBUG
			std::cout << "skipping degenerate triangle: " << idxA << "/" << idxB << "/" << idxC << std::endl;
#endif
			return;
		}

		double lengthAC = (pointC - pointA).length2();
		if (lengthAC < eps * eps * 10)
		{
#ifdef _DEBUG
			std::cout << "skipping degenerate triangle: " << idxA << "/" << idxB << "/" << idxC << std::endl;
#endif
			return;
		}

		double lengthBC = (pointC - pointB).length2();
		if (lengthBC < eps * eps * 10)
		{
#ifdef _DEBUG
			std::cout << "skipping degenerate triangle: " << idxA << "/" << idxB << "/" << idxC << std::endl;
#endif
			return;
		}

#ifdef _CHECK_INDICES
		bool confirmDone = false;
		size_t numFaceIndices = m_poly_data->faceIndices.size();
		if (m_poly_data->points.size() >= 71805) {
			if (m_poly_data->faceIndices.size() > 574200) {

				std::cout << ": m_poly_data->points.size() == 71805 " << std::endl;
				std::cout << ": m_poly_data->faceIndices.size() = " << m_poly_data->faceIndices.size() << std::endl;
				if (m_poly_data->faceIndices.size() == 574252) {
					std::cout << "adding face indices: " << " A " << idxA << ": B " << idxB << ": C ... " << idxC;
					confirmDone = true;
				}
			}
		}
#endif

		m_poly_data->addFace(idxA, idxB, idxC);

#ifdef _CHECK_INDICES
		if (numFaceIndices + 4 != m_poly_data->faceIndices.size()) {
			std::cout << " addFace failed, incorrect faceIndices count" << std::endl;
		}

		if (m_poly_data->points.size() >= 71805) {

			if (confirmDone) {
				std::cout << " done" << std::endl;


			}

			if (m_poly_data->faceIndices.size() >= 574252) {
				std::cout << ": m_poly_data->faceIndices.size() = " << m_poly_data->faceIndices.size() << std::endl;
			}
		}
#endif
	}
};

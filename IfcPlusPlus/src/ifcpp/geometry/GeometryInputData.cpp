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

#include <vector>
#include <ifcpp/geometry/AppearanceData.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/GeomDebugDump.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/IFC4X3/include/IfcObjectPlacement.h>
#include <ifcpp/IFC4X3/include/IfcProduct.h>
#include <ifcpp/IFC4X3/include/IfcRepresentation.h>
#include <ifcpp/IFC4X3/include/IfcTextStyle.h>
#include "IncludeCarveHeaders.h"
#include "MeshOps.h"
#include "GeometryInputData.h"

using namespace IFC4X3;

bool checkPolyhedronData( const shared_ptr<carve::input::PolyhedronData>& poly_data, double minFaceArea, std::string& details )
{
	if( poly_data )
	{
		const std::vector<int>& faceIndices = poly_data->faceIndices;
		if( faceIndices.size() > 0 )
		{
			size_t iiFace = 0;
			for(  ; iiFace < faceIndices.size(); )
			{
				int numPoints = faceIndices[iiFace];
				int numPointsIdx = iiFace;

#ifdef _DEBUG
				std::vector<int> checkIndexes1;
				if( faceIndices.size() < 500 )
				{
					auto it = faceIndices.begin() + iiFace;
					std::copy(it, faceIndices.end(), std::back_inserter(checkIndexes1));
				}
#endif

				if( iiFace + numPoints >= faceIndices.size() )
				{
					return false;
				}

				if( numPoints < 3 )
				{
#ifdef _DEBUG
					//std::cout << "checkPolyhedronData: face with < 3 points" << std::endl;
#endif
					return false;
				}

				++iiFace;

				int idxFirst = faceIndices[iiFace];
				int idxLast = faceIndices[iiFace+numPoints-1];
				if( idxFirst == idxLast )
				{
#ifdef _DEBUG
					//std::cout << "checkPolyhedronData: closed polygon of " << numPoints << " points" << std::endl;
#endif
					return false;
				}

				std::vector<vec3> facePoints;
				for( size_t iiPoint = 0; iiPoint < numPoints; ++iiPoint )
				{
					int idx = faceIndices[iiFace + iiPoint];
					if( idx >= poly_data->points.size() )
					{
#ifdef _DEBUG
						//std::cout << "checkPolyhedronData: incorrect idx" << std::endl;
#endif
						return false;
					}

					if( iiPoint < numPoints - 1)
					{
						int idxNext = faceIndices[iiFace + iiPoint + 1];
						if( idx == idxNext )
						{
#ifdef _DEBUG
							//std::cout << "checkPolyhedronData: duplicate point" << std::endl;
#endif
 							return false;
						}
					}

					vec3& point = poly_data->points[idx];
					facePoints.push_back(point);
				}

				double area = GeomUtils::computePolygonArea(facePoints);
				if (std::abs(area) < minFaceArea)
				{
					details = "face area < eps";
				//TODO: if face is triangle
				//		if all points dx/dy/dz < eps, remove false
				//		else if 2 points are close, collapse 2 long edges
				//		else if there are 3 long edges and 1 point is on longest edge, split longest edge by point, and collapse all 4 edges

					return false;
				}

				iiFace = iiFace + numPoints;
			}

			if( iiFace != faceIndices.size() )
			{
				details = "iiFace != faceIndices.size()";
				return false;
			}
		}
	}
	return true;
}

bool fixPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data, double epsMergePoints )
{
	if( !poly_data )
	{
		return false;
	}

	std::vector<int>& faceIndices = poly_data->faceIndices;
	if( faceIndices.size() == 0 )
	{
		return true;
	}

	size_t numPointsAll = poly_data->points.size();
	if( numPointsAll < 2 )
	{
		return true;
	}
	bool inputCorrect = true;
	size_t maxPointIndex = numPointsAll - 1;

	std::vector<int> polyDataCorrected;
	int numFacesCorrected = 0;

	for( size_t iiFace = 0; iiFace < faceIndices.size(); )
	{
		int numPoints = faceIndices[iiFace];
		int numPointsIdx = iiFace;

		if( iiFace + numPoints >= faceIndices.size() )
		{
			// skip face
			break;
		}

		std::vector<int> pointIdxCurrentFace;
		for( size_t iiPoint = 1; iiPoint <= numPoints; ++iiPoint )
		{
			int idx = faceIndices[iiFace + iiPoint];
			if( idx > maxPointIndex )
			{
				// incorrect point index, skip current point
				continue;
			}

			if( pointIdxCurrentFace.size() > 0 )
			{
				if( idx == pointIdxCurrentFace.back() )
				{
					// duplicate index, skip
					continue;
				}
			}
			pointIdxCurrentFace.push_back(idx);
		}

		if( pointIdxCurrentFace.size() > 2 )
		{
			int firstPointIndex = pointIdxCurrentFace.front();
			int lastPointIndex = pointIdxCurrentFace.back();
			if( firstPointIndex == lastPointIndex )
			{
				// duplicate index, remove last point
				pointIdxCurrentFace.pop_back();
			}

			if( pointIdxCurrentFace.size() > 2 )
			{
				bool allPointsWithinEps = true;
				int idx = pointIdxCurrentFace[0];
				vec3 pointPrevious = poly_data->points[idx];

				for( size_t iiPoint = 1; iiPoint < pointIdxCurrentFace.size(); ++iiPoint )
				{
					int idx = pointIdxCurrentFace[iiPoint];
					const vec3& point = poly_data->points[idx];

					if (std::abs(point.x - pointPrevious.x) > epsMergePoints)
					{
						allPointsWithinEps = false;
						break;
					}

					if (std::abs(point.y - pointPrevious.y) > epsMergePoints)
					{
						allPointsWithinEps = false;
						break;
					}

					if (std::abs(point.z - pointPrevious.z) > epsMergePoints)
					{
						allPointsWithinEps = false;
						break;
					}
					pointPrevious = point;
				}

				// checking just the face area does not work, since it could be a long face with 0 width. Removing it, would result in open edges
				if(allPointsWithinEps)
				{
					//inputCorrect = false;
#ifdef _DEBUG
					int wait = 0;
#endif
				}
				else
				{
					// found correct face
					++numFacesCorrected;
					int numPointsInFace = pointIdxCurrentFace.size();
					polyDataCorrected.push_back(numPointsInFace);
					std::copy(pointIdxCurrentFace.begin(), pointIdxCurrentFace.end(), std::back_inserter(polyDataCorrected));
				}
			}
		}

		iiFace += numPoints + 1;

		if( iiFace > faceIndices.size() )
		{
			inputCorrect = false;
			break;
		}
		if( iiFace == faceIndices.size() )
		{
			break;
		}
	}

	poly_data->faceCount = numFacesCorrected;
	faceIndices = polyDataCorrected;

	return inputCorrect;
}

bool reverseFacesInPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data)
{
	if( !poly_data )
	{
		return false;
	}

	std::vector<int>& faceIndices = poly_data->faceIndices;
	if( faceIndices.size() == 0 )
	{
		return true;
	}

	size_t numPointsAll = poly_data->points.size();
	if( numPointsAll < 2 )
	{
		return true;
	}
	bool inputCorrect = true;
	size_t maxPointIndex = numPointsAll - 1;

	std::vector<int> polyDataReversed;
	int numFacesCorrected = 0;

	for( size_t iiFace = 0; iiFace < faceIndices.size(); )
	{
		int numPoints = faceIndices[iiFace];
		int numPointsIdx = iiFace;

		if( iiFace + numPoints >= faceIndices.size() )
		{
			// skip face
			break;
		}

		std::vector<int> pointIdxCurrentFace;
		for( size_t iiPoint = 1; iiPoint <= numPoints; ++iiPoint )
		{
			int idx = faceIndices[iiFace + iiPoint];
			pointIdxCurrentFace.push_back(idx);
		}

		polyDataReversed.push_back(numPoints);
		std::copy(pointIdxCurrentFace.rbegin(), pointIdxCurrentFace.rend(), std::back_inserter(polyDataReversed));

		iiFace += numPoints + 1;

		if( iiFace > faceIndices.size() )
		{
			inputCorrect = false;
			break;
		}
		if( iiFace == faceIndices.size() )
		{
			break;
		}
	}

	faceIndices = polyDataReversed;

	return inputCorrect;
}

void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, PolyInputCache3D& polyInput)
{
	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];

		for (size_t jj = 0; jj < mesh->faces.size(); ++jj)
		{
			carve::mesh::Face<3>* face = mesh->faces[jj];

			carve::mesh::Edge<3>* edge = face->edge;
			if (edge)
			{
				std::vector<int> vecPointIndexes;
				size_t maxNumEdges = 1000;
				for (size_t kk = 0; kk < face->n_edges; ++kk)
				{
					vec3& edgeEndPoint = edge->v2()->v;
					int idx = polyInput.addPoint(edgeEndPoint);
					vecPointIndexes.push_back(idx);

					edge = edge->next;
					if (edge == face->edge)
					{
						break;
					}
				}
				if (vecPointIndexes.size() < 3)
				{
#ifdef _DEBUG
					std::cout << "face with < 3 edges" << std::endl;
#endif
					continue;
				}
				polyInput.m_poly_data->addFace(vecPointIndexes.begin(), vecPointIndexes.end());
			}
		}
	}
}

void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, const std::set<carve::mesh::Face<3>* >& setSkipFaces, PolyInputCache3D& polyInput)
{
	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];

		for (size_t jj = 0; jj < mesh->faces.size(); ++jj)
		{
			carve::mesh::Face<3>* face = mesh->faces[jj];

			if (setSkipFaces.find(face) != setSkipFaces.end())
			{
				continue;
			}

			carve::mesh::Edge<3>* edge = face->edge;
			if (edge)
			{
				std::vector<int> vecPointIndexes;
				size_t maxNumEdges = 1000;
				for (size_t kk = 0; kk < face->n_edges; ++kk)
				{
					vec3& edgeEndPoint = edge->v2()->v;
					int idx = polyInput.addPoint(edgeEndPoint);
					vecPointIndexes.push_back(idx);

					edge = edge->next;
					if (edge == face->edge)
					{
						break;
					}
				}
				if (vecPointIndexes.size() < 3)
				{
#ifdef _DEBUG
					std::cout << "face with < 3 edges" << std::endl;
#endif
					continue;
				}

				// TODO: fix polyline, detect overlapping edges

				polyInput.m_poly_data->addFace(vecPointIndexes.begin(), vecPointIndexes.end());
			}
		}
	}
}

void polyhedronFromMesh(const carve::mesh::Mesh<3>* mesh, PolyInputCache3D& polyInput)
{
	for (size_t jj = 0; jj < mesh->faces.size(); ++jj)
	{
		carve::mesh::Face<3>* face = mesh->faces[jj];
		carve::mesh::Edge<3>* edge = face->edge;
		if (edge)
		{
			std::vector<int> vecPointIndexes;
			size_t maxNumEdges = 1000;
			for (size_t kk = 0; kk < face->n_edges; ++kk)
			{
				vec3& edgeEndPoint = edge->v2()->v;
				int idx = polyInput.addPoint(edgeEndPoint);
				vecPointIndexes.push_back(idx);

				edge = edge->next;
				if (edge == face->edge)
				{
					break;
				}
			}
			if (vecPointIndexes.size() < 3)
			{
#ifdef _DEBUG
				std::cout << "face with < 3 edges" << std::endl;
#endif
				continue;
			}
			polyInput.m_poly_data->addFace(vecPointIndexes.begin(), vecPointIndexes.end());
		}
	}
}

bool ItemShapeData::addClosedPolyhedron(const shared_ptr<carve::input::PolyhedronData>& poly_data, GeomProcessingParams& params, shared_ptr<GeometrySettings>& geom_settings)
{
	if (poly_data->getVertexCount() < 3)
	{
		return false;
	}

	double eps = params.epsMergePoints;
	double minFaceArea = params.minFaceArea;
	std::map<std::string, std::string> mesh_input_options;
	shared_ptr<carve::mesh::MeshSet<3> > meshsetUnchanged(poly_data->createMesh(mesh_input_options, eps));
	std::string details;
	bool correct = checkPolyhedronData(poly_data, minFaceArea, details);
	if (!correct)
	{
		fixPolyhedronData(poly_data, params.epsMergePoints);
		std::string details2;
		bool correct2 = checkPolyhedronData(poly_data, minFaceArea, details2);
		if (correct2)
		{
			shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(mesh_input_options, eps));

			MeshSetInfo infoTriangulated;
			bool validMesh = MeshOps::checkMeshSetValidAndClosed(meshset, infoTriangulated, params);
			if (validMesh)
			{
				m_meshsets.push_back(meshset);
				return true;
			}
		}
		else
		{
			shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(mesh_input_options, eps));

#ifdef _DEBUG
			//std::cout << "failed to correct polyhedron data\n";
			if (params.debugDump)
			{
				glm::vec4 color(0.3, 0.4, 0.5, 1.0);
				GeomDebugDump::dumpMeshsetOpenEdges(meshset, color, false, false);
				GeomDebugDump::dumpMeshset(meshset.get(), color, false);
			}
#endif

			bool triangulateOperands = false;
			bool shouldBeClosedManifold = true;
			MeshOps::simplifyMeshSet(meshset, geom_settings, params, triangulateOperands, shouldBeClosedManifold);
		}
	}

	if (meshsetUnchanged->isClosed())
	{
		m_meshsets.push_back(meshsetUnchanged);
		return true;
	}

#ifdef _DEBUG
	if (params.debugDump)
	{
		glm::vec4 color(0.3, 0.4, 0.5, 1.0);
		GeomDebugDump::dumpMeshsetOpenEdges(meshsetUnchanged, color, false, false);
		GeomDebugDump::dumpMeshset(meshsetUnchanged.get(), color, false);
	}
#endif

	if (poly_data->faceCount > 10000)
	{
		// TODO: buld spatial partition tree, and intersect all edges within a box with vertices and edges of that box
		m_meshsets_open.push_back(meshsetUnchanged); // still may be useful as open mesh
		return false;
	}

	// try to fix winding order
	reverseFacesInPolyhedronData(poly_data);

	meshsetUnchanged = shared_ptr<carve::mesh::MeshSet<3> >(poly_data->createMesh(mesh_input_options, eps));
	if (meshsetUnchanged->isClosed())
	{
		m_meshsets.push_back(meshsetUnchanged);
		return true;
	}

	MeshOps::intersectOpenEdgesWithPoints(meshsetUnchanged, params);

	for (size_t i = 0; i < meshsetUnchanged->meshes.size(); ++i)
	{
		meshsetUnchanged->meshes[i]->recalc(eps);
	}
	if (meshsetUnchanged->isClosed())
	{
		m_meshsets.push_back(meshsetUnchanged);
		return true;
	}

	MeshOps::intersectOpenEdgesWithEdges(meshsetUnchanged, params);

	if (meshsetUnchanged->isClosed())
	{
		m_meshsets.push_back(meshsetUnchanged);
		return true;
	}

	{
		MeshOps::resolveOpenEdges(meshsetUnchanged, params);

		if (meshsetUnchanged->isClosed())
		{
			m_meshsets.push_back(meshsetUnchanged);
			return true;
		}

		m_meshsets_open.push_back(meshsetUnchanged); // still may be useful as open mesh
	}
	// Meshset is not closed
	return false;
}

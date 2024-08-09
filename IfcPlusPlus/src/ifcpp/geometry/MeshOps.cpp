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

#include "IncludeCarveHeaders.h"
#include <ifcpp/geometry/FaceConverter.h>
#include <ifcpp/IFC4X3/include/IfcCartesianPoint.h>
#include "EdgeLoopFinder.h"
#include "MeshSimplifier.h"
#include "MeshOps.h"
using namespace IFC4X3;

size_t MeshOps::getNumFaces(const carve::mesh::MeshSet<3>* meshset)
{
	size_t num_faces = 0;
	for (size_t i = 0; i < meshset->meshes.size(); ++i)
	{
		num_faces += meshset->meshes[i]->faces.size();
	}
	return num_faces;
}

void MeshOps::recalcMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, double eps)
{
	for (auto mesh : meshset->meshes)
	{
		mesh->cacheEdges();
		mesh->recalc(eps);
	}
}

double MeshOps::computeFaceArea(const carve::mesh::Face<3>* face)
{
	if (face->edge == nullptr)
	{
		return 0;
	}

	double face_area = 0;
	if (face->nVertices() == 3)
	{
		carve::mesh::Edge<3>* edge = face->edge;
		if (edge)
		{
			carve::geom::vector<3>& v1 = edge->v1()->v;
			carve::geom::vector<3>& v2 = edge->v2()->v;
			if (edge->next)
			{
				carve::geom::vector<3>& v3 = edge->next->v2()->v;
				carve::geom::vector<3> side1 = v2 - v1;
				carve::geom::vector<3> side2 = v3 - v2;
				carve::geom::vector<3> c = cross(side1, side2);
				double len_square = c.length2();
				if (len_square > EPS_M14 * 0.001)
				{
					double area = sqrt(len_square) * 0.5;
					face_area += std::abs(area);
				}
			}
		}
	}
	else
	{
		std::vector<carve::mesh::Face<3>::vertex_t* > faceVertices;
		face->getVertices(faceVertices);
		if (faceVertices.size() > 2)
		{
			size_t n = faceVertices.size();

			carve::geom::vector<3> normal;
			carve::geom::vector<3> a;
			carve::geom::vector<3> b = faceVertices[n - 2]->v;
			carve::geom::vector<3> c = faceVertices[n - 1]->v;
			carve::geom::vector<3> s;

			for (int i = 0; i < n; ++i)
			{
				a = b;
				b = c;
				c = faceVertices[i]->v;

				normal.x += b.y * (c.z - a.z);
				normal.y += b.z * (c.x - a.x);
				normal.z += b.x * (c.y - a.y);

				s += c;
			}

			double length = normal.length();// glm::length(normal);
			if (std::abs(length) < EPS_M8)
			{
				return false;
			}

			normal /= length;
			double area = 0.5 * length;
			face_area += area;
		}
	}
	return face_area;
}

double MeshOps::computeFaceArea(const carve::mesh::Face<3>* face, double& longestEdge)
{
	if (face->edge == nullptr)
	{
		return 0;
	}

	longestEdge = 0;
	double face_area = 0;
	if (face->nVertices() == 3)
	{
		carve::mesh::Edge<3>* edge = face->edge;
		if (edge)
		{
			carve::geom::vector<3>& v1 = edge->v1()->v;
			carve::geom::vector<3>& v2 = edge->v2()->v;
			if (edge->next)
			{
				carve::geom::vector<3>& v3 = edge->next->v2()->v;
				carve::geom::vector<3> side1 = v2 - v1;
				carve::geom::vector<3> side2 = v3 - v2;
				carve::geom::vector<3> c = cross(side1, side2);
				double len_square = c.length2();
				//if (len_square > EPS_M14 * 0.001)
				{
					double length1 = side1.length();
					double length2 = side2.length();
					double length3 = (v3 - v1).length();
					double area = sqrt(len_square) * 0.5;
					if (length1 > longestEdge)
					{
						longestEdge = length1;
					}
					if (length2 > longestEdge)
					{
						longestEdge = length2;
					}
					if (length3 > longestEdge)
					{
						longestEdge = length3;
					}
					face_area += std::abs(area);
				}
			}
		}
	}
	else
	{
		std::vector<carve::mesh::Face<3>::vertex_t* > faceVertices;
		const carve::mesh::Edge<3>* e = face->edge;
		for (size_t ii = 0; ii < face->n_edges; ++ii)
		{
			faceVertices.push_back(e->vert);

			double length = e->length();
			if (length > longestEdge)
			{
				longestEdge = length;
			}

			e = e->next;
		}

		if (faceVertices.size() > 2)
		{
			size_t n = faceVertices.size();

			carve::geom::vector<3> normal;
			carve::geom::vector<3> a;
			carve::geom::vector<3> b = faceVertices[n - 2]->v;
			carve::geom::vector<3> c = faceVertices[n - 1]->v;
			carve::geom::vector<3> s;

			for (int i = 0; i < n; ++i)
			{
				a = b;
				b = c;
				c = faceVertices[i]->v;

				normal.x += b.y * (c.z - a.z);
				normal.y += b.z * (c.x - a.x);
				normal.z += b.x * (c.y - a.y);

				s += c;
			}

			double length = normal.length();
			if (std::abs(length) < EPS_M8)
			{
				return 0;
			}

			normal /= length;
			double area = 0.5 * length;
			face_area += area;
		}
	}
	return face_area;
}


void splitIntoSubLoops(const std::vector<carve::geom::vector<2> >& polygonMerged, std::vector<std::vector<carve::geom::vector<2> > >& polygonLoops, std::map<size_t, std::array<size_t, 2> >& mapInOut, double eps, bool dumpPolygon)
{
	// find inner loops and shift them such that there is no self-intersection
	std::vector<carve::geom::vector<2>> previousLoop;

	for (size_t ii = 0; ii < polygonMerged.size(); ++ii)
	{
		const carve::geom::vector<2>& point = polygonMerged[ii];


		for (size_t jj = 0; jj < previousLoop.size(); ++jj)
		{
			const carve::geom::vector<2>& previousPoint = previousLoop[jj];
			double dx = previousPoint[0] - point.x;
			if (std::abs(dx) < eps)
			{
				double dy = previousPoint[1] - point.y;
				if (std::abs(dy) < eps)
				{
					// a loop should have at least 3 points
					if (previousLoop.size() < 3)
					{
						previousLoop.clear();
						break;
					}
					else
					{
						if (jj == 1)
						{
							// connecting edge between outer and inner loop, erase
							if (previousLoop.size() > 1)
							{
								previousLoop.erase(previousLoop.begin());
							}
							polygonLoops.push_back(previousLoop);
							previousLoop.clear();
							break;
						}
						// back on point in previous loop. Split here and begin new loop
						polygonLoops.push_back(previousLoop);
						previousLoop.clear();
					}
					break;
				}
			}
		}

		size_t loopIndex = polygonLoops.size();
		size_t pointIndex = previousLoop.size();
		mapInOut[ii] = { loopIndex, pointIndex };
		previousLoop.push_back(point);
	}

	if (previousLoop.size() > 2)
	{
		polygonLoops.push_back(previousLoop);
	}

	std::map<double, std::vector<std::vector<carve::geom::vector<2> > > > mapAreaLoops;
	for (size_t ii = 0; ii < polygonLoops.size(); ++ii)
	{
		std::vector<carve::geom::vector<2> >& loop = polygonLoops[ii];
		double area = std::abs(GeomUtils::signedArea(loop));
		auto itFind = mapAreaLoops.find(area);
		if (itFind == mapAreaLoops.end())
		{
			std::vector<std::vector<carve::geom::vector<2> > >& vecOfLoops = itFind->second;
			vecOfLoops.push_back({ loop });
		}
		else
		{
			mapAreaLoops.insert({ area, { loop } });
		}
	}
	polygonLoops.clear();
	for (auto it = mapAreaLoops.rbegin(); it != mapAreaLoops.rend(); ++it)
	{
		std::vector<std::vector<carve::geom::vector<2> > >& vecOfLoops = it->second;
		for (std::vector<carve::geom::vector<2> >& loop : vecOfLoops)
		{
			polygonLoops.push_back(loop);
		}
	}
}

void shiftSubLoops(std::vector<carve::geom::vector<2> >& polygonMerged, std::map<size_t, size_t >& mapInOut, double eps, bool dumpPolygon)
{
	// find inner loops and shift them such that there is no self-intersection
	std::vector<carve::geom::vector<2>> previousLoop;
	std::vector<std::vector<carve::geom::vector<2> > > polygonLoops;
	for (size_t ii = 0; ii < polygonMerged.size(); ++ii)
	{
		const carve::geom::vector<2>& point = polygonMerged[ii];

		for (size_t jj = 0; jj < previousLoop.size(); ++jj)
		{
			const carve::geom::vector<2>& previousPoint = previousLoop[jj];
			double dx = previousPoint[0] - point.x;
			if (std::abs(dx) < eps)
			{
				double dy = previousPoint[1] - point.y;
				if (std::abs(dy) < eps)
				{
					// a loop should have at least 3 points
					if (previousLoop.size() < 3)
					{
						previousLoop.clear();
						break;
					}
					else
					{
						if (jj == 1)
						{
							// connecting edge between outer and inner loop, erase
							if (previousLoop.size() > 1)
							{
								previousLoop.erase(previousLoop.begin());
							}
							polygonLoops.push_back(previousLoop);
							previousLoop.clear();
							break;
						}
						// back on point in previous loop. Split here and begin new loop
						polygonLoops.push_back(previousLoop);
						previousLoop.clear();
					}
					break;
				}
			}
		}

		size_t loopIndex = polygonLoops.size();
		size_t pointIndex = previousLoop.size();
		//mapInOut[ii] = { loopIndex, pointIndex };
		previousLoop.push_back(point);
	}

	if (previousLoop.size() > 2)
	{
		//polygon_earcut_out.push_back(previousLoop);
	}

#ifdef _DEBUG
	if (polygonMerged.size() > 5 && dumpPolygon)
	{
		vec4 color(0.3, 0.33, 0.33, 1.);
		GeomDebugDump::dumpLocalCoordinateSystem();
		GeomDebugDump::moveOffset(0.1);
		GeomDebugDump::dumpPolyline(polygonMerged, color, 0, true, false);
		GeomDebugDump::moveOffset(0.1);
		GeomDebugDump::dumpPolyline(polygonLoops, color, true, false);
	}
#endif
}

inline double getFaceArea2D(const carve::mesh::Face<3>* face)
{
	std::vector<carve::geom::vector<2> > facePoints;
	std::vector<std::array<double, 2> > facePointsArray2D;
	face->getProjectedVertices(facePoints);
	GeomUtils::poly2VecToArray2(facePoints, facePointsArray2D);
	double area = GeomUtils::signedArea(facePointsArray2D);
	return area;
}

void getFacePoints(const carve::mesh::Face<3>* face, std::vector<vec3>& facePoints)
{
	facePoints.clear();
	std::vector<carve::mesh::Face<3>::vertex_t* > verts3d;
	face->getVertices(verts3d);

	for (size_t i3 = 0; i3 < verts3d.size(); ++i3)
	{
		const carve::geom::vector<3>& vertex_point = verts3d[i3]->v;
		facePoints.push_back(vertex_point);
	}
}

bool isBetterForExport(const MeshSetInfo& infoTriangulated, const MeshSetInfo& infoBefore)
{
	if (infoTriangulated.meshSetValid && infoTriangulated.maxNumberOfEdgesPerFace == 3)
	{
		return true;
	}

	if (infoTriangulated.maxNumberOfEdgesPerFace == 3 && infoBefore.maxNumberOfEdgesPerFace == 3)
	{
		// was already triangulated
		return false;
	}

	if (infoTriangulated.maxNumberOfEdgesPerFace == 3)
	{
		return true;
		//size_t numEdgesBefore = infoBefore.numClosedEdges + infoBefore.numOpenEdges();
		//size_t numEdgesTriangulated = infoTriangulated.numClosedEdges + infoTriangulated.numOpenEdges();
		//bool condition2 = infoTriangulated.numClosedEdges > numEdgesBefore * 0.7;
		//bool condition3 = infoTriangulated.numFaces > infoBefore.numFaces * 0.7;
		//if (infoTriangulated.meshSetValid || condition2 || condition3)
		//{
		//	return true;
		//}
	}

	// futher tests

	return false;
}

void MeshOps::checkAndFixMeshsetInverted( shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, const GeomProcessingParams& params)
{
	if( !meshset )
	{
		return;
	}

	if (meshset->meshes.size() > 100)
	{
		return;
	}
	if (meshset->vertex_storage.size() > 5000)
	{
		return;
	}

	// this approach is maybe too simple: get highest and lowest face in x/y/z, then check if normal vector points in the same direction
	// we can definitely not rely on meshset->is_negative
	carve::mesh::Face<3>* minXFace = nullptr;
	carve::mesh::Face<3>* maxXFace = nullptr;
	carve::mesh::Face<3>* minYFace = nullptr;
	carve::mesh::Face<3>* maxYFace = nullptr;
	carve::mesh::Face<3>* minZFace = nullptr;
	carve::mesh::Face<3>* maxZFace = nullptr;
	double minX = DBL_MAX;
	double maxX = -DBL_MAX;
	double minY = DBL_MAX;
	double maxY = -DBL_MAX;
	double minZ = DBL_MAX;
	double maxZ = -DBL_MAX;
	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
		size_t numOutwardNormals = 0;
		size_t numInwardNormals = 0;
		size_t numFaces = mesh->faces.size();

		for (size_t jj = 0; jj < numFaces; ++jj)
		{
			carve::mesh::Face<3>* face = mesh->faces[jj];
			if (face == nullptr)
			{
				continue;
			}
			vec3 faceCentroid = face->centroid();
			if (faceCentroid.x < minX)
			{
				minX = faceCentroid.x;
				minXFace = face;
			}

			if (faceCentroid.z > maxX)
			{
				maxX = faceCentroid.x;
				maxXFace = face;
			}

			if (faceCentroid.y < minY)
			{
				minY = faceCentroid.y;
				minYFace = face;
			}

			if (faceCentroid.y > maxY)
			{
				maxY = faceCentroid.y;
				maxYFace = face;
			}

			if (faceCentroid.z < minZ)
			{
				minZ = faceCentroid.z;
				minZFace = face;
			}

			if (faceCentroid.z > maxZ)
			{
				maxZ = faceCentroid.z;
				maxZFace = face;
			}
		}
	}

	size_t countWindingProbablyCorrect = 0;
	size_t countWindingProbablyInCorrect = 0;
	if (minXFace)
	{
		vec3 faceNormal = minXFace->computeNormal(params.epsMergePoints);
		if (faceNormal.x < 0) ++countWindingProbablyCorrect;
		else ++countWindingProbablyInCorrect;
	}

	if (maxXFace)
	{
		vec3 faceNormal = maxXFace->computeNormal(params.epsMergePoints);
		if (faceNormal.x > 0) ++countWindingProbablyCorrect;
		else ++countWindingProbablyInCorrect;
	}

	if (minYFace)
	{
		vec3 faceNormal = minYFace->computeNormal(params.epsMergePoints);
		if (faceNormal.y < 0) ++countWindingProbablyCorrect;
		else ++countWindingProbablyInCorrect;
	}

	if (maxYFace)
	{
		vec3 faceNormal = maxYFace->computeNormal(params.epsMergePoints);
		if (faceNormal.y > 0) ++countWindingProbablyCorrect;
		else ++countWindingProbablyInCorrect;
	}

	if (minZFace)
	{
		vec3 faceNormal = minZFace->computeNormal(params.epsMergePoints);
		if (faceNormal.z < 0) ++countWindingProbablyCorrect;
		else ++countWindingProbablyInCorrect;
	}

	if (maxZFace)
	{
		vec3 faceNormal = maxZFace->computeNormal(params.epsMergePoints);
		if (faceNormal.z > 0) ++countWindingProbablyCorrect;
		else ++countWindingProbablyInCorrect;
	}

	if (countWindingProbablyCorrect > countWindingProbablyInCorrect)
	{
		return;
	}

#ifdef _DEBUG
	vec4 color(0.2, 0.2, 0.2, 1.);
	if (params.debugDump)
	{
		GeomDebugDump::dumpMeshset(meshset.get(), color, false, true);
	}
#endif

	if (countWindingProbablyCorrect*3 < countWindingProbablyInCorrect*2)
	{
		// majority of faces have inward normals, so invert mesh
		std::unordered_set<const carve::mesh::Face<3>* > setSkipFaces;
		std::unordered_set<const carve::mesh::Face<3>* > setFlipFaces;
		PolyInputCache3D polyInput(params.epsMergePoints);
		polyhedronFromMeshSet(meshset, setSkipFaces, setFlipFaces, polyInput);

		std::map<std::string, std::string> mesh_input_options;
		std::string details;
		bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
		if (!correct)
		{
			fixPolyhedronData(polyInput.m_poly_data, params);
			std::string details2;
			correct = checkPolyhedronData(polyInput.m_poly_data, params, details2);
		}

		bool invertDone = false;
		if (correct)
		{
			reverseFacesInPolyhedronData(polyInput.m_poly_data);
			bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
			if (!correct)
			{
				fixPolyhedronData(polyInput.m_poly_data, params);
				std::string details2;
				correct = checkPolyhedronData(polyInput.m_poly_data, params, details2);
			}

			MeshSetInfo infoInput;
			MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, params);

			shared_ptr<carve::mesh::MeshSet<3> > meshsetFromPolyhedron(polyInput.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));
			MeshSetInfo infoFlippedFaces;
			MeshOps::checkMeshSetValidAndClosed(meshsetFromPolyhedron, infoFlippedFaces, params);

			if (MeshOps::isBetterForBoolOp(infoFlippedFaces, infoInput, true))
			{
				meshset = meshsetFromPolyhedron;
				invertDone = true;
			}
			else
			{
				// could be same and valid, so ok
				if (infoFlippedFaces.meshSetValid)
				{
					meshset = meshsetFromPolyhedron;
					invertDone = true;
				}
			}
		}

		if (!invertDone)
		{
			meshset->invert();
		}
	}
}

void MeshOps::classifyMeshesInside(std::vector<carve::mesh::Mesh<3>*>& meshes, shared_ptr<carve::mesh::MeshSet<3> >& result, GeomProcessingParams& params)
{
	// TODO: if this fails, sort meshes by volume. Biggest volume first -> probably outer mesh

	shared_ptr<carve::mesh::MeshSet<3> > outerMeshAsMeshset;
	for (size_t ii = 0; ii < meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* meshToMerge = meshes[ii];
		if (meshToMerge->is_negative)
		{
			meshToMerge->invert();
		}

		PolyInputCache3D polyInput1(params.epsMergePoints);
		polyhedronFromMesh(meshToMerge, polyInput1);

		std::map<std::string, std::string> mesh_input_options;
		shared_ptr<carve::mesh::MeshSet<3> > currentMeshAsMeshset(polyInput1.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));

		bool isClosed = currentMeshAsMeshset->isClosed();

		if (!outerMeshAsMeshset)
		{
			outerMeshAsMeshset = currentMeshAsMeshset;
			continue;
		}

		for (auto resultMesh : outerMeshAsMeshset->meshes)
		{
			if (resultMesh->is_negative)
			{
				resultMesh->invert();
			}
		}

#ifdef _DEBUG
		vec4 color(0.2, 0.2, 0.2, 1.);
		if (params.debugDump)
		{
			GeomDebugDump::dumpMeshset(outerMeshAsMeshset.get(), color, true, false);
			GeomDebugDump::dumpMeshset(currentMeshAsMeshset.get(), color, true, false);
		}
#endif

		carve::geom::aabb<3> bbox1 = outerMeshAsMeshset->getAABB();
		carve::geom::aabb<3> bbox2 = currentMeshAsMeshset->getAABB();

		bool intersectsWithEpsilon = bbox1.intersects(bbox2, params.epsMergePoints);
		if (!intersectsWithEpsilon)
		{
			// currentMeshAsMeshset is not an inner mesh, so merge it with  outerMeshAsMeshset
			carve::csg::CSG csg(params.epsMergePoints);
			shared_ptr<carve::mesh::MeshSet<3> > resultMerge(csg.compute(outerMeshAsMeshset.get(), currentMeshAsMeshset.get(), carve::csg::CSG::UNION, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
			if (resultMerge)
			{
				outerMeshAsMeshset = resultMerge;
				result = resultMerge;
			}
			continue;
		}


		size_t numOuterMeshes = 0;
		for (auto mesh : outerMeshAsMeshset->meshes)
		{
			if (!mesh->is_inner_mesh)
			{
				++numOuterMeshes;
			}
		}

		size_t resultNumOuterMeshes = 0;
		size_t resultNumInnerMeshes = 0;
		carve::csg::CSG csg(params.epsMergePoints);
		shared_ptr<carve::mesh::MeshSet<3> > resultSubtract(csg.compute(outerMeshAsMeshset.get(), currentMeshAsMeshset.get(), carve::csg::CSG::A_MINUS_B, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
		if (resultSubtract)
		{
			//if (resultSubtract->meshes.size() > 1)
			{
				for (auto mesh : resultSubtract->meshes)
				{
					if (mesh->is_inner_mesh)
					{
						++resultNumInnerMeshes;
					}
					else
					{
						++resultNumOuterMeshes;

						if (resultNumOuterMeshes > 1)
						{
#ifdef _DEBUG
							GeomDebugDump::moveOffset(1);
							GeomDebugDump::dumpMesh(mesh, color, true);
#endif
						}
					}
				}
			}
#ifdef _DEBUG
			if (params.debugDump)
			{
				GeomDebugDump::moveOffset(1);
				GeomDebugDump::dumpMeshset(resultSubtract.get(), color, true, false);
			}
#endif
			if (resultNumOuterMeshes == numOuterMeshes)
			{
				outerMeshAsMeshset = resultSubtract;
				result = resultSubtract;
				continue;
			}
		}

		carve::csg::CSG csg2(params.epsMergePoints);
		shared_ptr<carve::mesh::MeshSet<3> > resultMerge(csg2.compute(outerMeshAsMeshset.get(), currentMeshAsMeshset.get(), carve::csg::CSG::UNION, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
		if (resultMerge)
		{
			outerMeshAsMeshset = resultMerge;
			result = resultMerge;
		}
	}
}


void MeshOps::retriangulateMeshSetForExport( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, PolyInputCache3D& polyOut, const GeomProcessingParams& paramsInput)
{
	if (!meshset)
	{
		return;
	}

	MeshSetInfo infoInput;
	bool validInput = MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, paramsInput);
	MeshOps::checkMeshSetNonNegativeAndClosed(meshset, paramsInput);

	//if( infoInput.meshSetValid && infoInput.maxNumberOfEdgesPerFace == 3 )
	//{
	//	return;
	//}
	//
	//if ( infoInput.maxNumberOfEdgesPerFace == 3 )
	//{
	//	// TODO: check if this is sufficient
	//	return;
	//}

	GeomProcessingParams params(paramsInput);
	params.checkZeroAreaFaces = false;
	params.allowDegenerateEdges = true;
	params.allowFinEdges = true;
	params.mergeAlignedEdges = true;
	//PolyInputCache3D poly_cache(params.epsMergePoints);
		
	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		const carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
		const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;

		for (size_t i2 = 0; i2 < vec_faces.size(); ++i2)
		{
			const carve::mesh::Face<3>* face = vec_faces[i2];

			std::vector<vec3> faceBound;
			getFacePoints(face, faceBound);

			if (faceBound.size() < 3)
			{
				// ignore face
				continue;
			}
			
			if (faceBound.size() == 3 )
			{
				int vertex_index_a = polyOut.addPoint(faceBound[0]);
				int vertex_index_b = polyOut.addPoint(faceBound[1]);
				int vertex_index_c = polyOut.addPoint(faceBound[2]);

				if (vertex_index_a == vertex_index_b || vertex_index_a == vertex_index_c || vertex_index_b == vertex_index_c)
				{
					continue;
				}

				polyOut.m_poly_data->addFace(vertex_index_a, vertex_index_b, vertex_index_c);
				continue;
			}
			else if (faceBound.size() == 4)
			{
				vec3& pointA = faceBound[0];
				vec3& pointB = faceBound[1];
				vec3& pointC = faceBound[2];
				vec3& pointD = faceBound[3];
				polyOut.addFaceCheckIndexes(pointA, pointB, pointC, pointD, params.epsMergePoints);

				continue;
			}

			std::vector<std::vector<vec3> > inputBounds3D = { faceBound };
			FaceConverter::createTriangulated3DFace(inputBounds3D, polyOut, params, false);
		}
	}

	bool checkPolyData = false; // probably not necessary for openGL or other renderers
	if (checkPolyData)
	{
		std::string details = "";
		bool correct = checkPolyhedronData(polyOut.m_poly_data, params, details);
		if (!correct)
		{
			fixPolyhedronData(polyOut.m_poly_data, params);
			correct = checkPolyhedronData(polyOut.m_poly_data, params, details);

#ifdef _DEBUG
			if (!correct)
			{
				std::cout << "fixPolyhedronData  failed" << std::endl;
			}
#endif
		}
	}

#ifdef _DEBUG

	{
		GeomDebugDump::DumpSettingsStruct dumpSet;
		dumpSet.triangulateBeforeDump = false;
		GeomProcessingParams paramCopy(params);
		paramCopy.checkZeroAreaFaces = false;

		GeomDebugDump::dumpLocalCoordinateSystem();
		GeomDebugDump::moveOffset(0.3);
		shared_ptr<carve::mesh::MeshSet<3>> meshsetTriangulated = shared_ptr<carve::mesh::MeshSet<3> >(polyOut.m_poly_data->createMesh(carve::input::opts(), params.epsMergePoints));
		MeshSetInfo infoTriangulated;
		MeshOps::checkMeshSetValidAndClosed(meshsetTriangulated, infoTriangulated, params);
		GeomDebugDump::dumpWithLabel("triangulate:output ", meshsetTriangulated, dumpSet, paramCopy, true, true);
	}
#endif

	return;

//	shared_ptr<carve::mesh::MeshSet<3>> meshsetTriangulated = shared_ptr<carve::mesh::MeshSet<3> >(polyOut.m_poly_data->createMesh(carve::input::opts(), params.epsMergePoints));
//	MeshSetInfo infoTriangulated;
//	MeshOps::checkMeshSetValidAndClosed(meshsetTriangulated, infoTriangulated, params);
//
//	if( isBetterForExport( infoTriangulated, infoInput ))
//	{
//		meshset.reset();
//		meshset = meshsetTriangulated;
//	}
//
//#ifdef _DEBUG
//	bool dumpMesh = true;
//	if (validInput && dumpMesh && meshset->vertex_storage.size() > 60)
//	{
//		GeomDebugDump::DumpSettingsStruct dumpSet;
//		dumpSet.triangulateBeforeDump = false;
//		GeomProcessingParams paramCopy(params);
//		paramCopy.checkZeroAreaFaces = false;
//			
//		GeomDebugDump::dumpLocalCoordinateSystem();
//		GeomDebugDump::moveOffset(0.3);
//		GeomDebugDump::dumpWithLabel("triangulate:input ", meshset, dumpSet, paramCopy, true, true);
//		GeomDebugDump::moveOffset(0.3);
//		GeomDebugDump::dumpWithLabel("triangulate:result", meshsetTriangulated, dumpSet, paramCopy, true, true);
//	}
//#endif
//
//	checkAndFixMeshsetInverted(meshset, infoTriangulated, params);
}

void MeshOps::simplifyMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, MeshSetInfo& infoMeshOut, const GeomProcessingParams& params)
{
	if (!meshset)
	{
		infoMeshOut.meshSetValid = false;
		return;
	}

	bool useDefaultSimplifier = true;
	if (useDefaultSimplifier)
	{
		MeshSimplifier::simplifyMeshSet(meshset, params);
	}
	else
	{
		if (params.generalSettings)
		{
			if (params.generalSettings->m_callback_simplify_mesh)
			{
				params.generalSettings->m_callback_simplify_mesh(meshset, params);
			}
		}
	}

	MeshOps::checkMeshSetValidAndClosed(meshset, infoMeshOut, params);
}

void MeshOps::retriangulateMeshSetForBoolOp(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, const GeomProcessingParams& paramsInput, size_t retryCount)
{
	if (!meshset)
	{
		return;
	}

	MeshSetInfo info;
	bool validInput = MeshOps::checkMeshSetValidAndClosed(meshset, info, paramsInput);
	MeshOps::checkMeshSetNonNegativeAndClosed(meshset, paramsInput);

	if (info.maxNumberOfEdgesPerFace == 3 && validInput)
	{
		return;
	}
		
	PolyInputCache3D poly_cache(paramsInput.epsMergePoints);
	GeomProcessingParams params(paramsInput);
	params.mergeAlignedEdges = true;

	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		const carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
		const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;

		for (size_t i2 = 0; i2 < vec_faces.size(); ++i2)
		{
			const carve::mesh::Face<3>* face = vec_faces[i2];

			std::vector<vec3> faceBound;
			getFacePoints(face, faceBound);

			if (faceBound.size() < 3)
			{
				// ignore face
				continue;
			}

			if (faceBound.size() == 3 )
			{
				int vertex_index_a = poly_cache.addPoint(faceBound[0]);
				int vertex_index_b = poly_cache.addPoint(faceBound[1]);
				int vertex_index_c = poly_cache.addPoint(faceBound[2]);

				if (vertex_index_a == vertex_index_b || vertex_index_a == vertex_index_c || vertex_index_b == vertex_index_c)
				{
					continue;
				}

				poly_cache.m_poly_data->addFace(vertex_index_a, vertex_index_b, vertex_index_c);
				continue;
			}

			std::vector<std::vector<vec3> > inputBounds3D = { faceBound };
			FaceConverter::createTriangulated3DFace(inputBounds3D, poly_cache, params, false);
		}
	}

	shared_ptr<carve::mesh::MeshSet<3>> meshsetTrinangulated1 = shared_ptr<carve::mesh::MeshSet<3> >(poly_cache.m_poly_data->createMesh(carve::input::opts(), params.epsMergePoints));

	std::string details = "";
	bool correct = checkPolyhedronData(poly_cache.m_poly_data, params, details);
	if (!correct)
	{
		fixPolyhedronData(poly_cache.m_poly_data, params);
#ifdef _DEBUG
		bool correct2 = checkPolyhedronData(poly_cache.m_poly_data, params, details);
		if (!correct2)
		{
			std::cout << "fixPolyhedronData  failed" << std::endl;
		}
#endif
	}

	shared_ptr<carve::mesh::MeshSet<3>> meshsetTrinangulated = shared_ptr<carve::mesh::MeshSet<3> >(poly_cache.m_poly_data->createMesh(carve::input::opts(), params.epsMergePoints));
	MeshSetInfo infoTriangulated;
	bool validTriangulatedMesh = MeshOps::checkMeshSetValidAndClosed(meshsetTrinangulated, infoTriangulated, params);
	if (!validTriangulatedMesh)
	{
#ifdef _DEBUG
		bool dumpMesh = false;
		if (validInput && dumpMesh)
		{
			GeomDebugDump::DumpSettingsStruct dumpSet;
			dumpSet.triangulateBeforeDump = false;
			
			GeomProcessingParams paramCopy(params);
			paramCopy.checkZeroAreaFaces = false;

			GeomDebugDump::dumpLocalCoordinateSystem();
			GeomDebugDump::moveOffset(0.3);
			GeomDebugDump::dumpWithLabel("triangulate:input ", meshset, dumpSet, paramCopy, true, true);
			GeomDebugDump::moveOffset(0.3);
			GeomDebugDump::dumpWithLabel("triangulate:result", meshsetTrinangulated, dumpSet, paramCopy, true, true);
		}
#endif

		if (isBetterForBoolOp(infoTriangulated, info, true))
		{
			meshset.reset();
			meshset = meshsetTrinangulated1;
			return;
		}

		bool validTriangulatedMesh1 = MeshOps::checkMeshSetValidAndClosed(meshsetTrinangulated1, infoTriangulated, params);
		if (validTriangulatedMesh1)
		{
			meshset.reset();
			meshset = meshsetTrinangulated1;
			return;
		}

		if (!ignoreResultOpenEdges)
		{
			return;
		}
	}
	meshset.reset();
	meshset = meshsetTrinangulated;
}

bool MeshOps::isBetterForBoolOp(const MeshSetInfo& infoNew, const MeshSetInfo& infoBefore, bool considerTriangulation)
{
	if (infoNew.numOpenEdges() > infoBefore.numOpenEdges())
	{
		return false;
	}

	if (!infoNew.meshSetValid && infoBefore.meshSetValid)
	{
		return false;
	}

	// fewer open edges is better
	int numOpenEdgesRemoved = infoBefore.numOpenEdges() - infoNew.numOpenEdges();
	int numClosedEdgesRemoved = infoBefore.numClosedEdges - infoNew.numClosedEdges;
	
	int numDegenerateEdgesRemoved = infoBefore.degenerateEdges.size() - infoNew.degenerateEdges.size();
	int numFinEdgesRemoved = infoBefore.finEdges.size() - infoNew.finEdges.size();
	int numFinFacesRemoved = infoBefore.finFaces.size() - infoNew.finFaces.size();

	size_t numAllEdgesNew = infoNew.numClosedEdges + infoNew.numOpenEdges();
	bool fewerOpenEdges = numOpenEdgesRemoved > 0 && numAllEdgesNew > 0;
	
	if ( infoNew.meshSetValid )
	{
		if (fewerOpenEdges)
		{
			return true;
		}

		// fewer faces is better. But only when it is > 0
		bool fewerFaces = infoNew.numFaces < infoBefore.numFaces && infoNew.numFaces > 0;
		if (fewerFaces)
		{
			return true;
		}

		if (considerTriangulation)
		{
			if (infoNew.maxNumberOfEdgesPerFace == 3 && infoBefore.maxNumberOfEdgesPerFace > 3)
			{
				if (infoNew.degenerateEdges.size() <= infoBefore.degenerateEdges.size())
				{
					// mesh before was not triangulated, new is triangulated
					return true;
				}
			}
		}

		if (numDegenerateEdgesRemoved > 0 && numFinEdgesRemoved >= 0 && numFinFacesRemoved >= 0)
		{
			// fewer degenerate edges
			return true;
		}

		if (numDegenerateEdgesRemoved >= 0 && numFinEdgesRemoved > 0 && numFinFacesRemoved >= 0)
		{
			// fewer fin edges
			return true;
		}

		if (numDegenerateEdgesRemoved >= 0 && numFinEdgesRemoved >= 0 && numFinFacesRemoved > 0)
		{
			// fewer fin faces
			return true;
		}
	}

	if (!infoNew.meshSetValid && !infoBefore.meshSetValid)
	{
		if (fewerOpenEdges)
		{
			if ( infoNew.allPointersValid || !infoBefore.allPointersValid)
			{
				return true;
			}
		}
	}

#ifdef _DEBUG
	bool isEqual = infoNew.isEqual(infoBefore);
	if (!isEqual)
	{
		bool OpenEdgesRemove = numOpenEdgesRemoved > 0;
		bool ClosedEdgesRemove = numClosedEdgesRemoved > 0;
		int numFacesRemoved = infoBefore.numFaces - infoNew.numFaces;
		bool meshBeforeValid = infoBefore.meshSetValid;
		bool meshNewValid = infoNew.meshSetValid;
		int check = 0;
	}
#endif

	return false;
}

void findFinEdges(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::unordered_set<carve::mesh::Edge<3>* >& setFinEdges, const GeomProcessingParams& params)
{
	for (const carve::mesh::Mesh<3>*mesh : meshset->meshes)
	{
		if (mesh->closed_edges.size() > 1000)
		{
			continue;
		}
		for (carve::mesh::Edge<3>*edge : mesh->closed_edges)
		{
			if (!edge)
			{
				continue;
			}

			carve::mesh::Edge<3>* reverseEdge = edge->rev;
			if (!reverseEdge)
			{
				continue;
			}

			carve::mesh::Face<3>* face = edge->face;
			if (!face)
			{
				continue;
			}

			carve::mesh::Face<3>* adjacentFace = reverseEdge->face;
			if (!adjacentFace)
			{
				continue;
			}

			// re-compute face normal here
			face->computeNormal(params.epsMergePoints);
			adjacentFace->computeNormal(params.epsMergePoints);
			const vec3 faceNormal = face->plane.N;
			const vec3 face2Normal = adjacentFace->plane.N;

			// adjacent faces back-to-back have -1 as normal vector dot product
			double dotProduct = dot(faceNormal, face2Normal);
			if (std::abs(dotProduct + 1.0) < params.epsMergeAlignedEdgesAngle*10)
			{
				setFinEdges.insert(edge);
			}
		}
	}
}

void MeshOps::removeDegenerateMeshes(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& paramsInput, bool ensureValidMesh)
{
	if (!meshsetInput)
	{
		return;
	}

	GeomProcessingParams params(paramsInput);
	params.allowZeroAreaFaces = true;
	MeshSetInfo infoInput;
	bool meshInputOk = MeshOps::checkMeshSetValidAndClosed(meshsetInput, infoInput, params);
	size_t numRemovedMeshes = 0;
	
	for (auto it = meshsetInput->meshes.begin(); it != meshsetInput->meshes.end(); ++it )
	{
		carve::mesh::Mesh<3>* mesh = *it;
		double meshVolume = mesh->volume();
		if (meshVolume < params.epsMergePoints)
		{
			it = meshsetInput->meshes.erase(it);
			delete mesh;
			++numRemovedMeshes;
			it = meshsetInput->meshes.begin();
			if( it == meshsetInput->meshes.end() )
			{
				break;
			}
		}
	}

	if( numRemovedMeshes > 0 )
	{
		for( auto it = meshsetInput->meshes.begin(); it != meshsetInput->meshes.end(); ++it )
		{
			carve::mesh::Mesh<3>* mesh = *it;
			mesh->recalc(paramsInput.epsMergePoints);
		}

		
		MeshSetInfo info;
		bool meshOk = MeshOps::checkMeshSetValidAndClosed(meshsetInput, info, params);
#ifdef _DEBUG
		if (meshInputOk && !meshOk)
		{
			GeomDebugDump::DumpSettingsStruct dumpColorSettings;
			GeomDebugDump::dumpWithLabel("removeDegenerateMeshes--input", meshsetInput, dumpColorSettings, params, true, true);
			
		}
#endif
		if (ensureValidMesh)
		{
			if (isBetterForBoolOp(info, infoInput, true))
			{
				// TODO check if backup is necessary
			}
		}
	}
}

static void checkEdgeIntegrity(carve::mesh::Edge<3>* e, const carve::mesh::Face<3>* face, bool checkForDegenerateEdges, MeshSetInfo& info, double eps)
{
	if (!e)
	{
		info.allPointersValid = false;
		info.details = "edge is nullptr";
		info.degenerateFaces.insert(face);
		return;
	}

	if (!e->rev)
	{
		info.allPointersValid = false;
		info.details = "edge->rev is nullptr";
		info.openEdges.insert(e);
		info.degenerateFaces.insert(face);
		return;
	}

	if (!e->prev)
	{
		info.allPointersValid = false;
		info.details = "edge->prev is nullptr";
		info.degenerateFaces.insert(face);
		return;
	}

	if (!e->next)
	{
		info.allPointersValid = false;
		info.details = "edge->next is nullptr";
		info.degenerateFaces.insert(face);
		return;
	}

	if (!e->vert)
	{
		info.allPointersValid = false;
		info.details = "edge->vert is nullptr";
		info.degenerateFaces.insert(face);
		return;
	}

	if (!e->face)
	{
		info.allPointersValid = false;
		info.details = "edge->face is nullptr";
		info.degenerateFaces.insert(face);
		return;
	}

	if (checkForDegenerateEdges)
	{
		if (e->rev == nullptr || e->next == nullptr)
		{
			info.degenerateFaces.insert(face);
			info.degenerateEdges.insert(e);
			return;
		}

		if (e->rev->rev != e)
		{
			info.degenerateFaces.insert(face);
			info.degenerateEdges.insert(e);
			return;
		}

		if (e->next == e )
		{
#ifdef _DEBUG
			double length = e->length();
#endif
			info.details = "e->next == e";
			info.degenerateFaces.insert(face);
			info.degenerateEdges.insert(e);
			return;
		}

		if (e->next->vert == e->vert)
		{
			// this can actually happen, when an inner mesh touches the outer mesh with just one edge
			double length2 = e->length2();
			if (length2 < eps * eps)
			{
				info.details = "e->next->vert == e->vert";
				info.degenerateFaces.insert(face);
				info.degenerateEdges.insert(e);
				return;
			}
		}

		if (e->prev == e )
		{
			info.details = "e->prev == e";
			info.degenerateFaces.insert(face);
			info.degenerateEdges.insert(e);
			return;
		}

		if ( e->prev->vert == e->vert)
		{
#ifdef _DEBUG
			double lengthPreviousEdge = e->prev->length();
#endif
			info.details = "e->prev->vert == e->vert";
			info.degenerateFaces.insert(face);
			info.degenerateEdges.insert(e);
			return;
		}

		if (e->next->prev != e)
		{
			info.degenerateFaces.insert(face);
			info.degenerateEdges.insert(e);
			return;
		}

		if (e->prev->next != e)
		{
			info.degenerateFaces.insert(e->face);
			info.degenerateEdges.insert(e);
			return;
		}

		if (e->face != face )
		{
			info.degenerateFaces.insert(face);
			info.degenerateEdges.insert(e);
			return;
		}

		if (e->rev->rev != e)
		{
			info.degenerateFaces.insert(face);
			info.degenerateEdges.insert(e);
			info.details = "edge->rev->rev != edge";
		}

		if (e->prev == e->next)
		{
			info.degenerateEdges.insert(e);
			info.details = "e->prev == e->next";
		}

		double length2 = (e->v1()->v - e->v2()->v).length2();
		if (length2 < eps * eps)
		{
			info.degenerateEdges.insert(e);
			info.details = "e->length < eps";
		}

		if (e->prev->vert == e->next->vert)
		{
			double length2 = e->length2();
#ifdef _DEBUG
			double length2prev = e->prev->length2();
			double length2next = e->next->length2();

			carve::mesh::Face<3>* f1 = e->face;
			carve::mesh::Face<3>* f2 = e->prev->face;
			carve::mesh::Face<3>* f3 = e->next->face;
			double area1 = MeshOps::computeFaceArea(f1);
			double area2 = MeshOps::computeFaceArea(f2);
			double area3 = MeshOps::computeFaceArea(f3);

			//vec4 color(0.6, 0.6, 0.6, 1.);
			//GeomDebugDump::clearMeshsetDump();
			//GeomDebugDump::dumpFacePolygon(e->face, color, true);
#endif

			if (length2 < eps * eps)
			{
				info.degenerateEdges.insert(e);
				info.details = "e->prev->vert == e->next->vert";
			}
		}

		if (e->next == e->rev)
		{
			if (e->next->next == e)
			{
				// 2 edges referencing itself
				info.details = "e->next == e->rev";
				info.degenerateEdges.insert(e);
			}
		}
	}
}

void MeshOps::checkFaceIntegrity(const carve::mesh::Face<3>* face, bool checkForDegenerateEdges, MeshSetInfo& info, double eps)
{
	if (!face)
	{
		info.allPointersValid = false;
		return;
	}

	carve::mesh::Edge<3>* e = face->edge;
	if (!e)
	{
		info.allPointersValid = false;
		info.details = "face->edge is nullptr";
		return;
	}

	try
	{
		const size_t n_edges = face->n_edges;
		if (n_edges > 10000)
		{
			info.maxNumEdgesExceeded = true;
#ifdef _DEBUG
			std::cout << "n_edges > 10000" << std::endl;
			vec4 color(0.3, 0.3, 0.3, 1.);
			GeomDebugDump::dumpFacePolygon(face, color, false);
#endif
			return;
		}
		for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
		{
			checkEdgeIntegrity(e, face, checkForDegenerateEdges, info, eps);

			if (!info.allPointersValid)
			{
				return;
			}

			if (e->rev)
			{
				carve::mesh::Face<3>* reverseFace = e->rev->face;
				checkEdgeIntegrity(e->rev, reverseFace, checkForDegenerateEdges, info, eps);
			}

			if (!info.allPointersValid)
			{
				return;
			}

			// continue
			e = e->next;
		}

		if (e != face->edge)
		{
			info.details = "e != face->edge";
			info.degenerateFaces.insert(face);
			return;
		}
	}
	catch (std::exception& ex)
	{
#ifdef _DEBUG
		std::cout << ex.what() << std::endl;
#endif
		info.allPointersValid = false;
		return;
	}
	catch (std::exception* ex)
	{
#ifdef _DEBUG
		std::cout << ex->what() << std::endl;
#endif
		info.allPointersValid = false;
		return;
	}
	catch (carve::exception& ex)
	{
#ifdef _DEBUG
		std::cout << ex.str() << std::endl;
#endif
		info.allPointersValid = false;
		return;
	}
	catch (...)
	{
		info.allPointersValid = false;
		return;
	}
}

void MeshOps::checkMeshIntegrity(const carve::mesh::Mesh<3>* mesh, bool checkForDegenerateEdges, const GeomProcessingParams& params, MeshSetInfo& info)
{
	if (!mesh)
	{
		info.allPointersValid = false;
		return;
	}
	const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
	for (size_t j = 0; j < vec_faces.size(); ++j)
	{
		carve::mesh::Face<3>* face = vec_faces[j];
		checkFaceIntegrity(face, checkForDegenerateEdges, info, params.epsMergePoints);

		if (!info.allPointersValid)
		{
			return;
		}
	}
}

inline bool edgeToEdgeIntersect(const carve::mesh::Edge<3>* edge1, const carve::mesh::Edge<3>* edge2, double eps, vec3& intersectionPoint)
{
	const vec3 pointA = edge1->v1()->v;
	const vec3 pointB = edge1->v2()->v;
	const vec3 pointC = edge2->v1()->v;
	const vec3 pointD = edge2->v2()->v;

	vec3 AB = pointB - pointA;
	vec3 CD = pointD - pointC;
	vec3 CA = pointA - pointC;

	double a = dot(AB, AB);         // always >= 0
	double b = dot(AB, CD);
	double c = dot(CD, CD);         // always >= 0
	double d = dot(AB, CA);
	double e = dot(CD, CA);
	double sc, sN, sD = a * c - b * b;  // sc = sN / sD, sD >= 0
	double tc, tN, tD = a * c - b * b;  // tc = tN / tD, tD >= 0

	// compute the line parameters of the two closest points
	if (sD < eps)
	{
		// the lines are almost parallel
		sN = 0.0;              // force using point A on segment AB to prevent possible division by 0.0 later
		sD = 1.0;
		tN = e;
		tD = c;
	}
	else
	{
		// get the closest points on the infinite lines
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0)
		{
			// sc < 0 => the s=0 edge is visible
			sN = 0.0;          // compute shortest connection of A to segment CD
			tN = e;
			tD = c;
		}
		else if (sN > sD)  // sc > 1  => the s=1 edge is visible
		{
			sN = sD;       // compute shortest connection of B to segment CD
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0)
	{
		// tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)          // compute shortest connection of C to segment AB
		{
			sN = 0.0;
		}
		else if (-d > a)
		{
			sN = sD;
		}
		else
		{
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD)
	{
		// tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)  // compute shortest connection of D to segment AB
		{
			sN = 0;
		}
		else if ((-d + b) > a)
		{
			sN = sD;
		}
		else
		{
			sN = (-d + b);
			sD = a;
		}
	}

	sc = fabs(sN) < eps ? 0.0 : sN / sD;
	tc = fabs(tN) < eps ? 0.0 : tN / tD;

	vec3 P1 = pointA + (sc * AB);
	vec3 P2 = pointC + (tc * CD);
	vec3 delt = P2 - P1;
	if (delt.length2() < eps * eps)
	{
		// intersecting
		if (sc < eps)
		{
			// intersection is at edge1->v1() -> split edge2

			if (tc < eps || fabs(tc - 1.0) < eps)
			{
				// intersecting at point, not edge
				return false;
			}
		}
		else if (fabs(sc - 1.0) < eps)
		{
			if (tc < eps || fabs(tc - 1.0) < eps)
			{
				// intersecting at point, not edge
				return false;
			}

			// intersection is at edge1->v2() -> split edge2
		}

		// split edge1
		carve::mesh::Face<3>* face = edge1->face;

		if (edge1->rev)
		{
			carve::mesh::Face<3>* faceRev = edge1->rev->face;
		}

		intersectionPoint = P1;
		return true;
	}
	return false;
}

size_t flipFacesOnOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& infoInput, const GeomProcessingParams& params)
{
	size_t numEdgesAll = infoInput.numClosedEdges + infoInput.numOpenEdges();
	if (numEdgesAll > 5000)
	{
		return 0;
	}

	if (infoInput.numFaces > 5000)
	{
		return 0;
	}

	size_t numChanges = 0;
	std::unordered_set<carve::mesh::Face<3>* > setFacesDone;
	std::unordered_set<const carve::mesh::Face<3>* > setFlipFaces;

	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
		for (size_t jj = 0; jj < mesh->open_edges.size(); ++jj)
		{
			if (jj >= mesh->open_edges.size())
			{
				continue;
			}
			carve::mesh::Edge<3>* edge = mesh->open_edges[jj];
			if (edge)
			{
				if (edge->face)
				{
					if (setFacesDone.find(edge->face) == setFacesDone.end())
					{
						setFacesDone.insert(edge->face);
						setFlipFaces.insert(edge->face);
						++numChanges;
					}
				}
			}
		}
	}

	std::unordered_set<const carve::mesh::Face<3>* > setSkipFaces;
	PolyInputCache3D polyInput(params.epsMergePoints);
	MeshOps::polyhedronFromMeshSet(meshset, setSkipFaces, setFlipFaces, polyInput);

	std::map<std::string, std::string> mesh_input_options;
	std::string details;
	bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
	if (!correct)
	{
		fixPolyhedronData(polyInput.m_poly_data, params);
		std::string details2;
		correct = checkPolyhedronData(polyInput.m_poly_data, params, details2);
	}

	if (correct)
	{
		shared_ptr<carve::mesh::MeshSet<3> > meshsetFromPolyhedron(polyInput.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));
		MeshSetInfo infoFlippedFaces;
		MeshOps::checkMeshSetValidAndClosed(meshsetFromPolyhedron, infoFlippedFaces, params);

		if (MeshOps::isBetterForBoolOp(infoFlippedFaces, infoInput, true))
		{
			meshset = meshsetFromPolyhedron;
		}
		return numChanges;
	}
	return 0;
}

void MeshOps::resolveOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset,  const GeomProcessingParams& params)
{
	MeshSetInfo infoInput;
	MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, params);
	resolveOpenEdges(meshset, infoInput, params);
}

void MeshOps::resolveOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& infoInput, const GeomProcessingParams& params)
{
	if (!meshset)
	{
		return;
	}

	if (meshset->vertex_storage.size() > 2000)
	{
		return;
	}

	if (meshset->meshes.size() > 100) {
		return;
	}

	bool tryFlipFaces = true;
	if (tryFlipFaces)
	{
		size_t numChanges = flipFacesOnOpenEdges(meshset, infoInput, params);
		if (numChanges > 0)
		{
			MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, params);
		}
	}

	if (infoInput.meshSetValid)
	{
		return;
	}

	bool tryMergeShortOpenEdges = true;
	size_t numMeshesChanged = 0;
	shared_ptr<carve::mesh::MeshSet<3> > meshsetCopyUnChanged;

	for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
	{
		// check if all open edges are close together -> merge points to center
		EdgeLoopFinder loopFinder(params.epsMergePoints, params.epsMergePoints * 50);
		bool meshsetChanged = false;
		loopFinder.initFromMesh(meshset, mesh, params, tryMergeShortOpenEdges, meshsetChanged);
		if (meshsetChanged)
		{
			meshsetCopyUnChanged = loopFinder.m_meshsetCopyUnChanged;
			++numMeshesChanged;
		}
	}

	if (numMeshesChanged > 0)
	{
		shared_ptr<carve::mesh::MeshSet<3> > meshsetFromPolyhedron;
		int minNumFacesPerMesh = 4;
		MeshSet2Polyhedron2MeshSet(meshset, meshsetFromPolyhedron, params, minNumFacesPerMesh);

		MeshSetInfo infoChanged;
		MeshOps::checkMeshSetValidAndClosed(meshsetFromPolyhedron, infoChanged, params);

		if (MeshOps::isBetterForBoolOp(infoChanged, infoInput, true))
		{
			meshset = meshsetFromPolyhedron;
			infoInput.copyFromOther(infoChanged);
		}
		else
		{
			meshset = meshsetCopyUnChanged;
		}

		if (infoInput.meshSetValid)
		{
			return;
		}
	}

	size_t maxNumRepeats = meshset->meshes.size();
	if (maxNumRepeats > 1) maxNumRepeats = 1;
	for (size_t repeat = 0; repeat < maxNumRepeats; ++repeat)
	{
		if (meshset->meshes.size() > 20)
		{
			continue;
		}

		size_t numChanges = 0;
		for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
		{
			// check if all open edges are close together -> merge points to center
			EdgeLoopFinder loopFinder(params.epsMergePoints, params.epsMergePoints * 50);
			bool meshsetChanged = false;
			loopFinder.initFromMesh(meshset, mesh, params, tryMergeShortOpenEdges, meshsetChanged);

			float ratioOpenEdges = 0;
			if (infoInput.numClosedEdges > 0)
			{
				ratioOpenEdges = float(infoInput.numOpenEdges()) / float(infoInput.numClosedEdges);
			}

			if (loopFinder.m_mapVertexOpenEdges.size() > 0)
			{
				if (ratioOpenEdges < 0.2)
				{
#ifdef _DEBUG
					GeomDebugDump::clearMeshsetDump();
#endif
					loopFinder.findLoops();
				}

				if (loopFinder.m_closedEdgeLoopsOfOpenEdges.size() > 0)
				{
					PolyInputCache3D polyInput(params.epsMergePoints);

					polyhedronFromMeshSet(meshset, polyInput);

					for (std::vector<carve::mesh::Edge<3>* >&loop : loopFinder.m_closedEdgeLoopsOfOpenEdges)
					{
						std::vector<int> loopPointIndexes;
						for (carve::mesh::Edge<3>*edge : loop)
						{
							vec3& point = edge->vert->v;
							int idx = polyInput.addPoint(point);
							loopPointIndexes.push_back(idx);
						}
						polyInput.m_poly_data->addFace(loopPointIndexes.begin(), loopPointIndexes.end());
					}


					std::map<std::string, std::string> mesh_input_options;
					std::string details;
					bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
					if (!correct)
					{
						fixPolyhedronData(polyInput.m_poly_data, params);
						std::string details2;
						correct = checkPolyhedronData(polyInput.m_poly_data, params, details2);
					}

					if (correct)
					{
						shared_ptr<carve::mesh::MeshSet<3> > meshsetFromPolyhedron(polyInput.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));
						MeshSetInfo infoAddedPatches;
						MeshOps::checkMeshSetValidAndClosed(meshsetFromPolyhedron, infoAddedPatches, params);

						if (MeshOps::isBetterForBoolOp(infoAddedPatches, infoInput, true))
						{
							infoInput.copyFromOther(infoAddedPatches);
							meshset = meshsetFromPolyhedron;
							++numChanges;
							break;
						}
					}
				}
			}
		}
		if (numChanges == 0)
		{
			break;
		}
	}

	// check if all open edges are close together -> merge points to center
	bool openEdgesMergedToPoint = false;
		
	MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, params);
	if (infoInput.meshSetValid)
	{
		return;
	}

	// try to find loop of open edges -> close with a new face
	PolyInputCache3D polyInput(params.epsMergePoints);
	size_t numMeshesInput = meshset->meshes.size();
	size_t numFacesInput = 0;
	size_t numOpenEdgesInput = 0;
	size_t numClosedEdgesInput = 0;
	std::set<carve::mesh::Edge<3>* > allOpenEdges;
	std::set<carve::mesh::Edge<3>* > allEdges;
	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
		numOpenEdgesInput += mesh->open_edges.size();
		numClosedEdgesInput += mesh->closed_edges.size();
		numFacesInput += mesh->faces.size();

		for (auto edge : mesh->closed_edges)
		{
			if (edge)
			{
				allEdges.insert(edge);
			}
		}

		for (auto edge : mesh->open_edges)
		{
			if (edge)
			{
				allEdges.insert(edge);
			}
		}

		if (mesh->faces.size() >= 6)
		{
			for (auto edge : mesh->open_edges)
			{
				if (edge)
				{
					allOpenEdges.insert(edge);
				}
			}
		}
	}

	if (numOpenEdgesInput + numClosedEdgesInput > params.generalSettings->m_maxNumFaceEdges )
	{
		return;
	}

	if (numOpenEdgesInput == 0)
	{
		return;
	}

	if (numOpenEdgesInput > 100)
	{
		return;
	}

	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];

		for (auto f : mesh->faces)
		{
			std::vector<int> vecPointIndexes;
			std::vector<vec3> vecFaceLoop;
			carve::mesh::Edge<3>* edge = f->edge;

			for (size_t jj = 0; jj < f->n_edges; ++jj)
			{
				vec3& edgePoint2 = edge->v2()->v;

				auto itFind = allOpenEdges.find(edge);
				if (itFind != allOpenEdges.end())
				{
					// intersect with all points and edges of MeshSet
					vec3& edgePoint1 = edge->v1()->v;

					const vec3 edgeDelta = edgePoint2 - edgePoint1;
					double dotLineSegDelta = dot(edgeDelta, edgeDelta);

					std::map<double, vec3> mapIntersectionsOnEdge;

					// check if current edge needs to be split
					for (size_t iiVertex = 0; iiVertex < meshset->vertex_storage.size(); ++iiVertex)
					{
						const carve::mesh::Vertex<3>& vert = meshset->vertex_storage[iiVertex];
						vec3 vertexPoint = vert.v;


						double t = -1;
						bool onSegment = GeomUtils::isPointOnLineSegment(edgePoint1, edgeDelta, dotLineSegDelta, vertexPoint, t, params.epsMergePoints);

						if (onSegment)
						{
							mapIntersectionsOnEdge.insert({ t, vertexPoint });
						}
					}

					// intersect edge-edge
					for (carve::mesh::Edge<3>*edgeToIntersect : allEdges)
					{
						if (edgeToIntersect == edge)
						{
							continue;
						}

						vec3 intersectionPoint;
						bool intersect = edgeToEdgeIntersect(edgeToIntersect, edge, params.epsMergePoints, intersectionPoint);
						if (intersect)
						{
							double t = -1;
							bool onSegment = GeomUtils::isPointOnLineSegment(edgePoint1, edgeDelta, dotLineSegDelta, intersectionPoint, t, params.epsMergePoints);

							if (onSegment)
							{
								mapIntersectionsOnEdge.insert({ t, intersectionPoint });
							}
						}
					}


					for (auto it : mapIntersectionsOnEdge)
					{
						vec3& vertexPoint = it.second;
						int idx = polyInput.addPoint(vertexPoint);
						if (vecPointIndexes.size() > 0)
						{
							size_t lastIndex = vecPointIndexes.back();
							if (lastIndex == idx)
							{
								continue;
							}
						}

						vecPointIndexes.push_back(idx);
						vecFaceLoop.push_back(vertexPoint);
					}
				}

				int idx = polyInput.addPoint(edgePoint2);
				vecPointIndexes.push_back(idx);
				vecFaceLoop.push_back(edgePoint2);

				edge = edge->next;
				if (edge == f->edge)
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

			double area = GeomUtils::computePolygonArea(vecFaceLoop, params.epsMergePoints);
			if (area < params.minFaceArea)
			{
				continue;
			}
			polyInput.m_poly_data->addFace(vecPointIndexes.begin(), vecPointIndexes.end());
		}
	}

	std::string details = "";
	bool polyInputCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
	if (!polyInputCorrect)
	{
		fixPolyhedronData(polyInput.m_poly_data, params);
		polyInputCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
	}

	if (polyInputCorrect)
	{
		shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts(), params.epsMergePoints));

		size_t numOpenEdges = 0;
		size_t numClosedEdges = 0;
		for (size_t ii = 0; ii < meshsetNew->meshes.size(); ++ii)
		{
			carve::mesh::Mesh<3>* mesh = meshsetNew->meshes[ii];
			mesh->recalc(params.epsMergePoints);
			numOpenEdges += mesh->open_edges.size();
			numClosedEdges += mesh->closed_edges.size();
		}

#ifdef _DEBUG
		if (params.debugDump)
		{
			GeomDebugDump::moveOffset(0.4);
			GeomDebugDump::moveOffset(meshset->getAABB().extent.y * 1.1);
			vec4 color(0, 1, 1, 1);
			if (numOpenEdges > 0)
			{
				vec4 color(1, 0.5, 1, 1);
				//GeomDebugDump::dumpMeshsetOpenEdges(meshsetNew, color, false, false);
			}
			bool drawNormals = true;
			GeomDebugDump::dumpMeshset(meshsetNew, color, drawNormals, true);
		}
#endif

		MeshSetInfo infoNewMesh;
		MeshOps::checkMeshSetValidAndClosed(meshsetNew, infoNewMesh, params);

		if (isBetterForBoolOp(infoNewMesh, infoInput, false))
		{
			meshset = meshsetNew;
		}
	}
}

double MeshOps::computeMeshSetSurface(const shared_ptr<carve::mesh::MeshSet<3> >& meshset)
{
	double surface_area = 0;
	const std::vector<carve::mesh::Mesh<3>* >& vec_meshes = meshset->meshes;
	for (size_t kk = 0; kk < vec_meshes.size(); ++kk)
	{
		const carve::mesh::Mesh<3>* mesh = vec_meshes[kk];
		const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
		for (size_t mm = 0; mm < vec_faces.size(); ++mm)
		{
			const carve::mesh::Face<3>* face = vec_faces[mm];
			surface_area += computeFaceArea(face);
		}
	}
	return surface_area;
}

double MeshOps::computeShapeSurfaceArea(const shared_ptr<ItemShapeData>& geomItem)
{
	double surface_area = 0;
	double volume = 0;
	if (geomItem)
	{
		{
			for (size_t jj = 0; jj < geomItem->m_meshsets.size(); ++jj)
			{
				surface_area += computeMeshSetSurface(geomItem->m_meshsets[jj]);
			}

			for (size_t jj = 0; jj < geomItem->m_meshsets_open.size(); ++jj)
			{
				surface_area += computeMeshSetSurface(geomItem->m_meshsets_open[jj]);
			}

			for (const shared_ptr<ItemShapeData>& item_data : geomItem->m_child_items )
			{
				double childArea = computeShapeSurfaceArea(geomItem);
				surface_area += childArea;
			}
		}
	}
	return surface_area;
}

double MeshOps::computeShapeSurfaceArea(const shared_ptr<ProductShapeData>& shape_input_data)
{
	double surface_area = 0;
	double volume = 0;
	if (shape_input_data)
	{
		for (auto geomItem : shape_input_data->getGeometricItems())
		{
			for (size_t jj = 0; jj < geomItem->m_meshsets.size(); ++jj)
			{
				surface_area += computeMeshSetSurface(geomItem->m_meshsets[jj]);
			}

			for (size_t jj = 0; jj < geomItem->m_meshsets_open.size(); ++jj)
			{
				surface_area += computeMeshSetSurface(geomItem->m_meshsets_open[jj]);
			}
		}

		for (const shared_ptr<ProductShapeData>& item_data : shape_input_data->getChildElements() )
		{
			double childArea = computeShapeSurfaceArea(item_data);
			surface_area += childArea;
		}
	}
	return surface_area;
}

double MeshOps::computeMeshsetVolume(const carve::mesh::MeshSet<3>* meshset)
{
	if (!meshset)
	{
		return 0;
	}
	double volume = 0;
	for (size_t kk = 0; kk < meshset->meshes.size(); ++kk)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
		double meshVolume = mesh->volume();

		if (meshVolume < 0)
		{
			mesh->invert();
			meshVolume = -meshVolume;
		}
		volume += meshVolume;
	}
	return volume;
}

size_t MeshOps::countFaces(carve::mesh::MeshSet<3>* mesh)
{
	if (!mesh)
	{
		return 0;
	}

	size_t numFaces = 0;
	for (auto& mesh : mesh->meshes)
	{
		numFaces += mesh->faces.size();
	}
	return numFaces;
}

size_t MeshOps::countDegeneratedFaces(carve::mesh::MeshSet<3>* mesh)
{
	if (!mesh)
	{
		return 0;
	}

	size_t numDegenerateFaces = 0;
	for (auto& mesh : mesh->meshes)
	{
		for (auto& f : mesh->faces)
		{
			if (f->n_edges == 0)
			{
				++numDegenerateFaces;
			}
		}
	}
	return numDegenerateFaces;
}

bool MeshOps::checkMeshSetNonNegativeAndClosed(const shared_ptr<carve::mesh::MeshSet<3> > mesh_set, const GeomProcessingParams& params)
{
	bool meshes_closed = true;
	if (mesh_set)
	{
		for (size_t i = 0; i < mesh_set->meshes.size(); ++i)
		{
			carve::mesh::Mesh<3>* mesh_i = mesh_set->meshes[i];
			if (mesh_i)
			{
				if (mesh_i->isNegative())
				{
					mesh_i->invert();
					if (mesh_i->isNegative())
					{
						mesh_i->recalc(params.epsMergePoints);
						mesh_i->calcOrientation();
						if (mesh_i->isNegative())
						{
							std::cout << "could not invert mesh_set->meshes[" << i << "] " << std::endl;
						}
					}
				}

				if (!mesh_i->isClosed())
				{
					meshes_closed = false;
				}

				if (mesh_i->open_edges.size() > 0)
				{
					meshes_closed = false;
				}
			}
		}
	}
	return meshes_closed;
}

void MeshOps::checkMeshSetIntegrity(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool checkForDegenerateEdges, const GeomProcessingParams& params, MeshSetInfo& info)
{
	if (meshset)
	{
		for (size_t i = 0; i < meshset->meshes.size(); ++i)
		{
			carve::mesh::Mesh<3>* mesh_i = meshset->meshes[i];
			MeshOps::checkMeshIntegrity(mesh_i, checkForDegenerateEdges, params, info);
			if (!info.allPointersValid)
			{
				return;
			}
		}
	}
}

bool MeshOps::checkMeshSetValidAndClosed(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, const GeomProcessingParams& params)
{
	info.meshSetValid = false;
	if (!meshset)
	{
		return false;
	}
	
	info.meshset = meshset;
	bool allowFinEdges = params.allowFinEdges;
	info.resetInfoResult();
	if (!meshset)
	{
#ifdef _DEBUG
		if (info.entity)
		{
			std::cout << "MeshSet of entity #" << info.entity->m_tag << " not valid" << std::endl;
		}
#endif
		info.allPointersValid = false;
		info.meshSetValid = false;
		return false;
	}
	if (meshset->meshes.size() == 0)
	{
#ifdef _DEBUG
		if (info.entity)
		{
			//std::cout << "MeshSet of entity #" << info.entity->m_tag << " has no meshes" << std::endl;
		}
#endif
		info.meshSetValid = false;
		return info.meshSetValid;
	}

	// check open edges first, since it is the easiest and cheapest
	info.meshSetValid = true;
	info.maxNumberOfEdgesPerFace = 0;
	
	for (size_t kk = 0; kk < meshset->meshes.size(); ++kk)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];

		if (!mesh)
		{
			info.allPointersValid = false;
			continue;
		}
		for (auto e : mesh->open_edges)
		{
			info.openEdges.insert(e);
		}
		info.numClosedEdges += mesh->closed_edges.size();
		info.numFaces += mesh->faces.size();

		for (carve::mesh::Face<3>* inputFace : mesh->faces)
		{
			if (inputFace == nullptr)
			{
				info.allPointersValid = false;
				continue;
			}
			
			if (inputFace->n_edges > info.maxNumberOfEdgesPerFace)
			{
				info.maxNumberOfEdgesPerFace = inputFace->n_edges;
			}
			
			if (inputFace->n_edges < 3)
			{
				info.degenerateFaces.insert(inputFace);
			}

			double longestEdge = 0;
			double face_area = MeshOps::computeFaceArea(inputFace, longestEdge);
			if (std::abs(face_area) < params.minFaceArea  )
			{
				info.degenerateFaces.insert(inputFace);
				if (params.treatLongThinFaceAsDegenerate)
				{
					info.zeroAreaFaces.insert(inputFace);
				}
				else if (longestEdge < params.epsMergePoints )
				{
					info.zeroAreaFaces.insert(inputFace);
				}
			}
			info.surfaceArea += face_area;
		}
	}

	// check for valid pointers first
	bool checkForDegenerateEdges = true;
	checkMeshSetIntegrity(meshset, checkForDegenerateEdges, params, info);
	if (!info.allPointersValid)
	{
		info.meshSetValid = false;
		return false;
	}

	if (!params.allowDegenerateEdges)
	{
		if (info.degenerateEdges.size() > 0)
		{
			info.meshSetValid = false;
			return info.meshSetValid;
		}
	}

	// check for fin edges, where a face is back-to-back to face of reverse edge
	
	findFinEdges(meshset, info.finEdges, params);

	if (info.finEdges.size() > 0)
	{
		if (!allowFinEdges)
		{
			info.meshSetValid = false;
		}
	}

	if (!allowFinEdges)
	{
		for (size_t i = 0; i < meshset->meshes.size(); ++i)
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[i];

			if (!mesh)
			{
				continue;
			}
			const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
			for (size_t j = 0; j < vec_faces.size(); ++j)
			{
				carve::mesh::Face<3>* face = vec_faces[j];

				const size_t n_edges = face->n_edges;
				if (n_edges > 10000)
				{
#ifdef _DEBUG
					std::cout << "n_edges > 10000" << std::endl;
					vec4 color(0.3, 0.3, 0.3, 1.);
					GeomDebugDump::dumpFacePolygon(face, color, false);
#endif
					continue;
				}

				carve::mesh::Edge<3>* e = face->edge;
				for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
				{
					// check for adjacent fin faces
					if (e->face)
					{
						const carve::geom::vector<3>& normal = e->face->plane.N;
						if (e->face->edge)
						{
							if (e->face->edge->rev)
							{
								if (e->face->edge->rev->face)
								{
									const carve::geom::vector<3>& reverserFaceNormal = e->face->edge->rev->face->plane.N;

									double dotProduct = dot(reverserFaceNormal, normal);
									if (std::abs(dotProduct + 1.0) < params.epsMergePoints)
									{
										double face_area = MeshOps::computeFaceArea(face);
										if (std::abs(face_area) > params.minFaceArea * 10)
										{
#ifdef _DEBUG
											//std::cout << "opposite face is coplanar" << std::endl;
#endif
											info.finFaces.insert( face );
										}
									}
								}
							}
						}
					}

					// continue
					e = e->next;
				}
			}
		}

		//if (!checkMeshFins(meshset, params.epsMergePoints))
		if( info.finEdges.size() > 0)
		{
			info.meshSetValid = false;
			return info.meshSetValid;
		}
	}

	if (info.numOpenEdges() > 0)
	{
		info.meshSetValid = false;
		return info.meshSetValid;
	}

	if (info.zeroAreaFaces.size() > 0)
	{
		if (!params.allowZeroAreaFaces)
		{
			info.meshSetValid = false;
			return info.meshSetValid;
		}
	}

	std::stringstream err;
	bool meshes_closed = checkMeshSetNonNegativeAndClosed(meshset, params);
	if (meshes_closed)
	{
		// check volume
		double meshset_volume = 0;
		for (size_t kk = 0; kk < meshset->meshes.size(); ++kk)
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			double mesh_volume = mesh->volume();

			if (mesh_volume < 0)
			{
				mesh->invert();
				if (mesh->isNegative())
				{
					mesh->recalc(params.epsMergePoints);
					mesh->calcOrientation();
					if (mesh->isNegative())
					{
						std::cout << "could not invert negative mesh[" << kk << "] " << std::endl;
					}
				}
				mesh_volume = mesh->volume();
			}

			if (mesh_volume < 0)
			{
				err << "mesh_volume < 0" << std::endl;
			}

			meshset_volume += mesh_volume;
		}
	}
	else
	{
		err << "mesh_set not closed" << std::endl;
	}

	if (err.tellp() > 0)
	{
#ifdef _DEBUG
		if (info.report_callback)
		{
			info.report_callback->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, info.entity);
		}
#endif
		info.meshSetValid = false;
		return false;
	}

	return info.meshSetValid;
}

bool intersectRayTriangle(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection, const glm::dvec3& p0, const glm::dvec3& p1, const glm::dvec3& p2)
{
	double t = 0;
	glm::dvec2 baryPosition(1, 1);
	bool intersects = glm::intersectRayTriangle(rayOrigin, rayDirection, p0, p1, p2, baryPosition, t);
	bool edgeIntersected = false;
	if (intersects && t >= 0)
	{
		return true;
	}
	return false;
}

///\brief method intersectOpenEdges: Intersect open edges of MeshSet with closed edges, and split the open edges in case of intersection
///\param[in/out] meshset: MeshSet with open edges. If fix is found, a new MeshSet is assigned to the smart pointer
///\param[in] eps: tolerance to find edge-edge intersections
///\param[in] dumpMeshes: write meshes to dump file for debugging
void MeshOps::intersectOpenEdgesWithPoints(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& params)
{
	if (!meshsetInput)
	{
		return;
	}

	size_t maxNumFaces = 2000;
	size_t maxNumEdges = 2000;
	size_t maxNumOpenEdges = 100;
	double eps = params.epsMergePoints * 1.2;

#ifdef _DEBUG
	vec4 color(0.5, 0.6, 0.7, 1.0);
	if (params.debugDump)
	{
		GeomDebugDump::moveOffset(0.8);
		//GeomDebugDump::dumpMeshset(meshset, color, true);
		double dy = meshsetInput->getAABB().extent.y;
		GeomDebugDump::moveOffset(dy * 2.2);
		GeomDebugDump::dumpMeshsetOpenEdges(meshsetInput, color, false, false);
		GeomDebugDump::moveOffset(dy * 2.2);
	}
#endif

	for (size_t round = 0; round < 5; ++round)
	{
		size_t numClosedEdgesBefore = 0;
		std::vector<carve::mesh::Edge<3>* > allOpenEdges;
		std::vector<carve::mesh::Face<3>* > allFaces;

		for (size_t ii = 0; ii < meshsetInput->meshes.size(); ++ii)
		{
			const carve::mesh::Mesh<3>* mesh = meshsetInput->meshes[ii];
			numClosedEdgesBefore += mesh->closed_edges.size();
			std::copy(mesh->open_edges.begin(), mesh->open_edges.end(), std::back_inserter(allOpenEdges));
			std::copy(mesh->faces.begin(), mesh->faces.end(), std::back_inserter(allFaces));
		}

		std::set<carve::mesh::Face<3>* > setOpenEdgesAdjacentFaces;
		for (size_t ii = 0; ii < allOpenEdges.size(); ++ii)
		{
			carve::mesh::Edge<3>* openEdge = allOpenEdges[ii];
			carve::mesh::Face<3>* adjacentFace = openEdge->face;
			setOpenEdgesAdjacentFaces.insert(adjacentFace);
		}

		PolyInputCache3D polyInput(params.epsMergePoints);

		// intersect with closed edges
		size_t numSplitEdges = 0;
		for (size_t iiFace = 0; iiFace < allFaces.size(); ++iiFace)
		{
			if (iiFace > maxNumFaces)
			{
				return;
			}
			carve::mesh::Face<3>* face = allFaces[iiFace];

			const size_t n_edges = face->n_edges;
			if (n_edges > maxNumEdges)
			{
#ifdef _DEBUG
				if (params.debugDump)
				{
					std::cout << "n_edges=" << n_edges << " > max (" << maxNumEdges << ")" << std::endl;
					vec4 color(0.3, 0.3, 0.3, 1.);
					GeomDebugDump::dumpFacePolygon(face, color, false);
				}
#endif
				return;
			}

			std::vector<vec3> faceLoop;
			carve::mesh::Edge<3>* edge = face->edge;
			faceLoop.push_back(edge->v1()->v);

			bool addFaceDirectly = true;
			bool tryIntersect = false;
			auto itFind = setOpenEdgesAdjacentFaces.find(face);
			if (itFind != setOpenEdgesAdjacentFaces.end())
			{
				tryIntersect = true;
			}

			// face is adjacent to an open edge, so try to intersect with points
			for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
			{
				carve::mesh::Vertex<3>* vertex1 = edge->v1();
				carve::mesh::Vertex<3>* vertex2 = edge->v2();
				const vec3& edgePoint1 = vertex1->v;
				const vec3& edgePoint2 = vertex2->v;

				if (tryIntersect)
				{
					const vec3 edgeDelta = edgePoint2 - edgePoint1;
					double dotLineSegDelta = dot(edgeDelta, edgeDelta);

					std::map<double, vec3> mapIntersections;

					// check if current edge needs to be split
					for (size_t iiVertex = 0; iiVertex < meshsetInput->vertex_storage.size(); ++iiVertex)
					{
						const carve::mesh::Vertex<3>& vert = meshsetInput->vertex_storage[iiVertex];
						vec3 vertexPoint = vert.v;

						double t = -1;
						bool onSegment = GeomUtils::isPointOnLineSegment(edgePoint1, edgeDelta, dotLineSegDelta, vertexPoint, t, eps);

						if (onSegment)
						{
							mapIntersections.insert({ t, vertexPoint });
							++numSplitEdges;
						}
					}

					for (auto itIntersections = mapIntersections.begin(); itIntersections != mapIntersections.end(); ++itIntersections)
					{
						const vec3& vertexPoint = itIntersections->second;

#ifdef _DEBUG
						if (params.debugDump && false)
						{
							vec4 color(0.3, 0.3, 0.3, 1.);
							if (itIntersections == mapIntersections.begin())
							{
								GeomDebugDump::dumpFacePolygon(face, color, false);
							}
							GeomDebugDump::dumpPolyline(faceLoop, color, 0, false, false);
						}
#endif

						faceLoop.push_back(vertexPoint);
					}
				}
				faceLoop.push_back(edgePoint2);

				edge = edge->next;

				if (edge == face->edge)
				{
					break;
				}
			}

			if (addFaceDirectly)
			{
				std::vector<int> faceIndexes;
				for (size_t iiPoint = 0; iiPoint < faceLoop.size(); ++iiPoint)
				{
					const vec3& v0 = faceLoop[iiPoint];
					int idxA = polyInput.addPoint(v0);
					if (faceIndexes.size() > 0)
					{
						int previousIndex = faceIndexes.back();
						if (idxA == previousIndex)
						{
							continue;
						}
					}
					faceIndexes.push_back(idxA);
				}

				GeomUtils::removeLastIfEqualToFirst(faceIndexes);

				if (faceIndexes.size() > 2 && faceIndexes.size() < 500)
				{
					size_t n = std::distance(faceIndexes.begin(), faceIndexes.end());

					size_t numFaceIndices = polyInput.m_poly_data->faceIndices.size();
					if (numFaceIndices > 10000)
					{
#ifdef _DEBUG
						std::cout << "numFaceIndices > 10000" << std::endl;
#endif
					}
					polyInput.m_poly_data->addFace(faceIndexes.begin(), faceIndexes.end());
				}
			}
			else
			{
				std::vector<std::vector<vec3> > faceLoops = { faceLoop };
				bool mergeAlignedEdges = false;
				bool dumpPolygon = true;

				GeomProcessingParams paramsTriangulate(params);
				paramsTriangulate.epsMergeAlignedEdgesAngle = 0.0;
				FaceConverter::createTriangulated3DFace(faceLoops, polyInput, paramsTriangulate, false);
			}
		}

		if (numSplitEdges > 0)
		{
			std::string details = "";
			bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
			if (!correct)
			{
				bool correct2 = fixPolyhedronData(polyInput.m_poly_data, params);
#ifdef _DEBUG
				if (!correct2)
				{
					std::cout << "fixPolyhedronData  failed" << std::endl;
				}
#endif
				return;
			}

			shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts(), eps));
			MeshSetInfo infoNew;
			//checkMeshSetValidAndClosed(meshsetNew, infoNew, params);

			MeshSetInfo infoInput;
			checkMeshSetValidAndClosed(meshsetInput, infoInput, params);

			assignIfBetterForBoolOp(meshsetNew, meshsetInput, infoNew, infoInput, false, params, false);

#ifdef _DEBUG
			if (params.debugDump)
			{
				vec4 color(0.3, 0.3, 0.3, 1.);
				bool drawNormals = true;
				GeomDebugDump::dumpMeshset(meshsetNew, color, drawNormals, false);

				double dy = meshsetNew->getAABB().extent.y;
				GeomDebugDump::moveOffset(dy * 2.2);
				GeomDebugDump::dumpMeshsetOpenEdges(meshsetNew, color, false, false);
				GeomDebugDump::moveOffset(dy * 3.2);
			}
#endif
		}
	}
}

bool MeshOps::assignIfBetterForBoolOp(shared_ptr<carve::mesh::MeshSet<3> >& meshsetNew, shared_ptr<carve::mesh::MeshSet<3> >& meshsetBefore, MeshSetInfo& infoNew, MeshSetInfo& infoBefore, bool considerTriangulation, const GeomProcessingParams& params, bool deepCopyMesh)
{
	MeshOps::checkMeshSetValidAndClosed(meshsetNew, infoNew, params);
	if (MeshOps::isBetterForBoolOp(infoNew, infoBefore, considerTriangulation))
	{
#if defined(_DEBUG) || defined(_DEBUG_RELEASE)
		std::unordered_map<std::string, int> changes;
		changes["numClosedEdgesRemoved"] = infoBefore.numClosedEdges - infoNew.numClosedEdges;
		changes["numOpenEdgesRemoved"] = infoBefore.numOpenEdges() - infoNew.numOpenEdges();
		changes["numFacesRemoved"] = infoBefore.numFaces - infoNew.numFaces;
		changes["numZeroAreaFacesRemoved"] = infoBefore.zeroAreaFaces.size() - infoNew.zeroAreaFaces.size();
		changes["numFinEdgesRemoved"] = infoBefore.finEdges.size() - infoNew.finEdges.size();
		changes["numFinFacesRemoved"] = infoBefore.finFaces.size() - infoNew.finFaces.size();
		changes["numDegeneratedEdgesRemoved"] = infoBefore.degenerateEdges.size() - infoNew.degenerateEdges.size();

		int numChanges = 0;
		for (auto it : changes)
		{
			numChanges += it.second;
		}
		if (numChanges > 0)
		{
			//std::cout << "assignIfBetterForBoolOp: changes: " << numChanges << std::endl;
		}
#endif
		if (deepCopyMesh)
		{
			meshsetBefore = shared_ptr<carve::mesh::MeshSet<3>>(meshsetNew->clone());
		}
		else
		{
			meshsetBefore = meshsetNew;
		}
		infoBefore.copyFromOther(infoNew);
		return true;
	}

	return false;
}

///\brief method intersectOpenEdges: Intersect open edges of MeshSet with closed edges, and split the open edges in case of intersection
///\param[in/out] meshset: MeshSet with open edges. If fix is found, a new MeshSet is assigned to the smart pointer
///\param[in] eps: tolerance to find edge-edge intersections
///\param[in] dumpMeshes: write meshes to dump file for debugging
void MeshOps::intersectOpenEdgesWithEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params)
{
	if (!meshset)
	{
		return;
	}

	size_t maxNumFaces = 600;
	size_t maxNumEdges = 600;
	size_t maxNumOpenEdges = 100;
	double eps = params.epsMergePoints;

#ifdef _DEBUG
	vec4 color(0.5, 0.6, 0.7, 1.0);
	if (params.debugDump)
	{
		GeomDebugDump::moveOffset(0.8);
		//GeomDebugDump::dumpMeshset(meshset, color, true);
		double dy = meshset->getAABB().extent.y;
		GeomDebugDump::moveOffset(dy * 2.2);
		GeomDebugDump::dumpMeshsetOpenEdges(meshset, color, false, false);
		GeomDebugDump::moveOffset(1.2);
	}
#endif

	for (size_t round = 0; round < 5; ++round)
	{
		size_t numClosedEdgesBefore = 0;
		std::vector<carve::mesh::Edge<3>* > allOpenEdges;
		std::vector<carve::mesh::Face<3>* > allFaces;

		for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
			numClosedEdgesBefore += mesh->closed_edges.size();
			std::copy(mesh->open_edges.begin(), mesh->open_edges.end(), std::back_inserter(allOpenEdges));
			std::copy(mesh->faces.begin(), mesh->faces.end(), std::back_inserter(allFaces));
		}

		PolyInputCache3D polyInput(params.epsMergePoints);

		// intersect with closed edges
		size_t numSplitEdges = 0;
		for (size_t iiFace = 0; iiFace < allFaces.size(); ++iiFace)
		{
			if (iiFace > maxNumFaces)
			{
				return;
			}
			carve::mesh::Face<3>* face = allFaces[iiFace];

			const size_t n_edges = face->n_edges;
			if (n_edges > maxNumEdges)
			{
#ifdef _DEBUG
				if (params.debugDump)
				{
					std::cout << "n_edges=" << n_edges << " > max (" << maxNumEdges << ")" << std::endl;
					vec4 color(0.3, 0.3, 0.3, 1.);
					GeomDebugDump::dumpFacePolygon(face, color, false);
				}
#endif
				return;
			}

			std::vector<vec3> faceLoop;
			carve::mesh::Edge<3>* edge = face->edge;
			faceLoop.push_back(edge->v1()->v);

			for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
			{
				// check if current edge needs to be split
				for (size_t iiOpenEdge = 0; iiOpenEdge < allOpenEdges.size(); ++iiOpenEdge)
				{
					if (iiOpenEdge > maxNumOpenEdges)
					{
						return;
					}
					const carve::mesh::Edge<3>* openEdge = allOpenEdges[iiOpenEdge];
					vec3 openEdgeStart = openEdge->v1()->v;
					vec3 openEdgeEnd = openEdge->v2()->v;

					vec3 intersectionPoint;
					bool intersect = edgeToEdgeIntersect(openEdge, edge, params.epsMergePoints, intersectionPoint);

					if (intersect)
					{
						faceLoop.push_back(intersectionPoint);
						++numSplitEdges;

						// handle only first split. TODO: sort split points and add to faceLoop, sorted
						break;
					}
				}
				faceLoop.push_back(edge->v2()->v);

				edge = edge->next;

				if (edge == face->edge)
				{
					break;
				}
			}

			bool addFaceDirectly = true;
			if (addFaceDirectly)
			{
				std::vector<int> faceIndexes;
				for (size_t iiPoint = 0; iiPoint < faceLoop.size(); ++iiPoint)
				{
					const vec3& v0 = faceLoop[iiPoint];
					int idxA = polyInput.addPoint(v0);
					if (faceIndexes.size() > 0)
					{
						int previousIndex = faceIndexes.back();
						if (idxA == previousIndex)
						{
							continue;
						}
					}
					faceIndexes.push_back(idxA);
				}

				if (faceIndexes.size() > 1)
				{
					for (size_t iiPoint = 0; iiPoint < faceIndexes.size(); ++iiPoint)
					{
						size_t idxFirst = faceIndexes.front();
						size_t idxLast = faceIndexes.back();
						if (idxFirst == idxLast)
						{
							faceIndexes.pop_back();
							if (faceIndexes.size() < 2)
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}

				if (faceIndexes.size() > 2 && faceIndexes.size() < 500)
				{
					size_t n = std::distance(faceIndexes.begin(), faceIndexes.end());

					size_t numFaceIndices = polyInput.m_poly_data->faceIndices.size();
					if (numFaceIndices > 1000)
					{
						//std::cout << numFaceIndices << " ";
					}
					polyInput.m_poly_data->addFace(faceIndexes.begin(), faceIndexes.end());
				}
			}
			else
			{
				std::vector<std::vector<vec3> > faceLoops = { faceLoop };
				bool mergeAlignedEdges = false;
				bool dumpPolygon = true;
				FaceConverter::createTriangulated3DFace(faceLoops, polyInput, params, false);
			}
		}

		if (numSplitEdges > 0)
		{
			std::string details = "";
			bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
			if (!correct)
			{
				bool correct2 = fixPolyhedronData(polyInput.m_poly_data, params);
#ifdef _DEBUG
				if (!correct2)
				{
					std::cout << "fixPolyhedronData  failed" << std::endl;
				}
#endif
				return;
			}

			shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts(), eps));
			if (meshsetNew->isClosed())
			{
				meshset = meshsetNew;
				return;
			}
			else
			{
				size_t numOpenEdgesNew = 0;
				size_t numClosedEdgesNew = 0;
				for (size_t ii = 0; ii < meshsetNew->meshes.size(); ++ii)
				{
					carve::mesh::Mesh<3>* mesh = meshsetNew->meshes[ii];
					numOpenEdgesNew += mesh->open_edges.size();
					numClosedEdgesNew += mesh->closed_edges.size();
				}

				if (numOpenEdgesNew < allOpenEdges.size())
				{
					size_t numAllEdgesNew = numOpenEdgesNew + numClosedEdgesNew;
					size_t numAllEdgesBefore = numClosedEdgesBefore + allOpenEdges.size();
					if (numClosedEdgesNew >= numClosedEdgesBefore)
					{
						meshset = meshsetNew;
					}
				}

#ifdef _DEBUG
				if (params.debugDump)
				{
					vec4 color(0.3, 0.3, 0.3, 1.);
					bool drawNormals = true;
					GeomDebugDump::dumpMeshset(meshsetNew, color, drawNormals, false);

					double dy = meshsetNew->getAABB().extent.y;
					GeomDebugDump::moveOffset(dy * 2.2);
					GeomDebugDump::dumpMeshsetOpenEdges(meshsetNew, color, false, false);
					GeomDebugDump::moveOffset(dy * 3.2);
				}
#endif
			}
		}
	}
}

void MeshOps::boundingBox2Mesh(const carve::geom::aabb<3>& bbox, shared_ptr<carve::mesh::MeshSet<3> >& meshset, double eps)
{
	double minX = bbox.pos.x - bbox.extent.x;
	double maxX = bbox.pos.x + bbox.extent.x;
	double minY = bbox.pos.y - bbox.extent.y;
	double maxY = bbox.pos.y + bbox.extent.y;
	double minZ = bbox.pos.z - bbox.extent.z;
	double maxZ = bbox.pos.z + bbox.extent.z;

	shared_ptr<carve::input::PolyhedronData> polyhedron_data(new carve::input::PolyhedronData());
	polyhedron_data->addVertex(carve::geom::VECTOR(maxX, maxY, maxZ));
	polyhedron_data->addVertex(carve::geom::VECTOR(minX, maxY, maxZ));
	polyhedron_data->addVertex(carve::geom::VECTOR(minX, minY, maxZ));
	polyhedron_data->addVertex(carve::geom::VECTOR(maxX, minY, maxZ));

	polyhedron_data->addVertex(carve::geom::VECTOR(maxX, maxY, minZ));
	polyhedron_data->addVertex(carve::geom::VECTOR(minX, maxY, minZ));
	polyhedron_data->addVertex(carve::geom::VECTOR(minX, minY, minZ));
	polyhedron_data->addVertex(carve::geom::VECTOR(maxX, minY, minZ));

	polyhedron_data->addFace(0, 1, 2);
	polyhedron_data->addFace(2, 3, 0);

	polyhedron_data->addFace(7, 6, 5);
	polyhedron_data->addFace(5, 4, 7);

	polyhedron_data->addFace(0, 4, 5);
	polyhedron_data->addFace(5, 1, 0);
	polyhedron_data->addFace(1, 5, 6);
	polyhedron_data->addFace(6, 2, 1);
	polyhedron_data->addFace(2, 6, 7);
	polyhedron_data->addFace(7, 3, 2);
	polyhedron_data->addFace(3, 7, 4);
	polyhedron_data->addFace(4, 0, 3);

	meshset = shared_ptr<carve::mesh::MeshSet<3> >(polyhedron_data->createMesh(carve::input::opts(), eps));
}

std::shared_ptr<carve::mesh::MeshSet<3> > MeshOps::createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, double eps)
{
	carve::input::PolyhedronData polyhedron_data;
	polyhedron_data.addVertex(p0);
	polyhedron_data.addVertex(p1);
	polyhedron_data.addVertex(p2);
	polyhedron_data.addFace(0, 1, 2);
	std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), eps));
	return mesh;
}

std::shared_ptr<carve::mesh::MeshSet<3> > MeshOps::createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, vec3& p3, double eps)
{
	carve::input::PolyhedronData polyhedron_data;
	polyhedron_data.addVertex(p0);
	polyhedron_data.addVertex(p1);
	polyhedron_data.addVertex(p2);
	polyhedron_data.addVertex(p3);

	polyhedron_data.addFace(0, 1, 2, 3);
	//polyhedron_data.addFace(2, 3, 0);
	std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), eps));
	return mesh;
}

std::shared_ptr<carve::mesh::MeshSet<3> > MeshOps::createBoxMesh(vec3& pos, vec3& extent, carve::math::Matrix& transform, double eps)
{
	carve::input::PolyhedronData polyhedron_data;
	polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(extent.x, extent.y, -extent.z)));
	polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(extent.x, -extent.y, -extent.z)));
	polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(-extent.x, -extent.y, -extent.z)));
	polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(-extent.x, extent.y, -extent.z)));

	polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(extent.x, extent.y, extent.z)));
	polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(extent.x, -extent.y, extent.z)));
	polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(-extent.x, -extent.y, extent.z)));
	polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(-extent.x, extent.y, extent.z)));

	polyhedron_data.addFace(0, 1, 2);
	polyhedron_data.addFace(2, 3, 0);
	polyhedron_data.addFace(7, 6, 5);
	polyhedron_data.addFace(5, 4, 7);
	polyhedron_data.addFace(0, 4, 5);
	polyhedron_data.addFace(5, 1, 0);
	polyhedron_data.addFace(1, 5, 6);
	polyhedron_data.addFace(6, 2, 1);
	polyhedron_data.addFace(2, 6, 7);
	polyhedron_data.addFace(7, 3, 2);
	polyhedron_data.addFace(3, 7, 4);
	polyhedron_data.addFace(4, 0, 3);

	std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), eps));
	return mesh;
}

bool checkPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params, std::string& details)
{
	bool allowZeroAreaFaces = params.allowZeroAreaFaces;

	if (poly_data)
	{
		const std::vector<int>& faceIndices = poly_data->faceIndices;
		if (faceIndices.size() > 0)
		{
			size_t iiFace = 0;
			for (; iiFace < faceIndices.size(); )
			{
				int numPoints = faceIndices[iiFace];
				int numPointsIdx = iiFace;

#ifdef _DEBUG
				std::vector<int> checkIndexes1;
				if (faceIndices.size() < 500)
				{
					auto it = faceIndices.begin() + iiFace;
					std::copy(it, faceIndices.end(), std::back_inserter(checkIndexes1));
				}
#endif

				if (iiFace + numPoints >= faceIndices.size())
				{
					return false;
				}

				if (numPoints < 3)
				{
#ifdef _DEBUG
					//std::cout << "checkPolyhedronData: face with < 3 points" << std::endl;
#endif
					return false;
				}

				++iiFace;

				int idxFirst = faceIndices[iiFace];
				int idxLast = faceIndices[iiFace + numPoints - 1];
				if (idxFirst == idxLast)
				{
#ifdef _DEBUG
					//std::cout << "checkPolyhedronData: closed polygon of " << numPoints << " points" << std::endl;
#endif
					return false;
				}

				std::vector<vec3> facePoints;
				for (size_t iiPoint = 0; iiPoint < numPoints; ++iiPoint)
				{
					int idx = faceIndices[iiFace + iiPoint];
					if (idx >= poly_data->points.size())
					{
#ifdef _DEBUG
						//std::cout << "checkPolyhedronData: incorrect idx" << std::endl;
#endif
						return false;
					}

					if (iiPoint < numPoints - 1)
					{
						int idxNext = faceIndices[iiFace + iiPoint + 1];
						if (idx == idxNext)
						{
#ifdef _DEBUG
							//std::cout << "checkPolyhedronData: duplicate point" << std::endl;
#endif
							details += "duplicate point";
							return false;
						}
					}

					vec3& point = poly_data->points[idx];
					facePoints.push_back(point);
				}

				if (!allowZeroAreaFaces)
				{
					double area = GeomUtils::computePolygonArea(facePoints, params.epsMergePoints);
					if (std::abs(area) < params.minFaceArea)
					{
						details += "face area < eps";
						//TODO: if face is triangle
						//		if all points dx/dy/dz < eps, remove false
						//		else if 2 points are close, collapse 2 long edges
						//		else if there are 3 long edges and 1 point is on longest edge, split longest edge by point, and collapse all 4 edges

						return false;
					}
				}

				iiFace = iiFace + numPoints;
			}

			if (iiFace != faceIndices.size())
			{
				details = "iiFace != faceIndices.size()";
				return false;
			}
		}
	}
	return true;
}

bool allPointsWithinEpsilon(const std::vector<vec3>& points, double eps)
{
	if (points.size() < 1)
	{
		return true;
	}

	double minX = points[0].x;
	double minY = points[0].y;
	double minZ = points[0].z;

	double maxX = points[0].x;
	double maxY = points[0].y;
	double maxZ = points[0].z;

	//if (values.size() < 100000)
	{
		for (size_t ii = 1; ii < points.size(); ii += 3)
		{
			const vec3& pt = points[ii];
			const double& x = pt.x;
			const double& y = pt.y;
			const double& z = pt.z;

			if (x > maxX) maxX = x;
			else if (x < minX) minX = x;

			if (y > maxY) maxY = y;
			else if (y < minY) minY = y;

			if (z > maxZ) maxZ = z;
			else if (z < minZ) minZ = z;

			if (maxX - minX > eps)
			{
				return false;
			}
			if (maxY - minY > eps)
			{
				return false;
			}
			if (maxZ - minZ > eps)
			{
				return false;
			}
		}
	}
	return true;
}

bool fixPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params)
{
	if (!poly_data)
	{
		return false;
	}

	std::vector<int>& faceIndices = poly_data->faceIndices;
	if (faceIndices.size() == 0)
	{
		return true;
	}

	size_t numPointsAll = poly_data->points.size();
	if (numPointsAll < 2)
	{
		return true;
	}
	bool inputCorrect = true;
	size_t maxPointIndex = numPointsAll - 1;
	double epsMergePoints = params.epsMergePoints;
	double epsMinFaceArea = params.minFaceArea;
	std::vector<int> polyDataCorrected;
	int numFacesCorrected = 0;
	int numSkippedPoints = 0;

	for (size_t iiFace = 0; iiFace < faceIndices.size(); )
	{
		int numPoints = faceIndices[iiFace];
		int numPointsIdx = iiFace;

		if (iiFace + numPoints >= faceIndices.size())
		{
			// skip face
			break;
		}

		std::vector<int> pointIdxCurrentFace;
		for (size_t iiPoint = 1; iiPoint <= numPoints; ++iiPoint)
		{
			int idx = faceIndices[iiFace + iiPoint];
			if (idx > maxPointIndex)
			{
				// incorrect point index, skip current point
				++numSkippedPoints;
				continue;
			}

			if (pointIdxCurrentFace.size() > 0)
			{
				if (idx == pointIdxCurrentFace.back())
				{
					// duplicate index, skip
					++numSkippedPoints;
					continue;
				}
			}
			pointIdxCurrentFace.push_back(idx);
		}

		if (pointIdxCurrentFace.size() > 2)
		{
			int firstPointIndex = pointIdxCurrentFace.front();
			int lastPointIndex = pointIdxCurrentFace.back();
			if (firstPointIndex == lastPointIndex)
			{
				// duplicate index, remove last point
				pointIdxCurrentFace.pop_back();
				++numSkippedPoints;
			}

			if (pointIdxCurrentFace.size() > 2)
			{
				int idx = pointIdxCurrentFace[0];

				vec3 pointPrevious = poly_data->points[idx];
				std::vector<vec3> polygonPoints;
				bool duplicatePointFound = false;

				for (size_t iiPoint = 0; iiPoint < pointIdxCurrentFace.size(); ++iiPoint)
				{
					int idx = pointIdxCurrentFace[iiPoint];
					const vec3& point = poly_data->points[idx];
#ifdef _DEBUG
					int idxNext = pointIdxCurrentFace[(iiPoint + 1) % pointIdxCurrentFace.size()];
					const vec3& pointNext = poly_data->points[idxNext];

					if (std::abs(pointNext.x - point.x) < epsMergePoints)
					{
						if (std::abs(pointNext.y - point.y) < epsMergePoints)
						{
							if (std::abs(pointNext.z - point.z) < epsMergePoints)
							{
								// delete current point and start over
								//auto itErase = pointIdxCurrentFace.begin();
								//std::advance(itErase, iiPoint);
								//iiPoint = 0;
								//polygonPoints.clear();
								//continue;
								duplicatePointFound = true;
							}
						}
					}
#endif
					polygonPoints.push_back(point);
				}

				GeomUtils::removeDuplicates(polygonPoints, epsMergePoints);

				bool allowLongZeroAreaFaces = true;
				if (allowLongZeroAreaFaces)
				{
					// remove face only when all points are within epsMergePoints

					bool faceIsPoint = allPointsWithinEpsilon(polygonPoints, epsMergePoints);
					if (!faceIsPoint)
					{
						// found correct face
						++numFacesCorrected;
						int numPointsInFace = pointIdxCurrentFace.size();
						polyDataCorrected.push_back(numPointsInFace);
						std::copy(pointIdxCurrentFace.begin(), pointIdxCurrentFace.end(), std::back_inserter(polyDataCorrected));
					}
				}
				else
				{

					// checking just the face area does not work, since it could be a long face with 0 width. Removing it, would result in open edges
					double area = GeomUtils::computePolygonArea(polygonPoints, epsMergePoints);
					if (area > epsMinFaceArea)
					{
						// found correct face
						++numFacesCorrected;
						int numPointsInFace = pointIdxCurrentFace.size();
						polyDataCorrected.push_back(numPointsInFace);
						std::copy(pointIdxCurrentFace.begin(), pointIdxCurrentFace.end(), std::back_inserter(polyDataCorrected));
					}
					else
					{
#ifdef _DEBUG
						//std::cout << "fixPolyhedronData: removing face with area " << area << std::endl;
#endif
					}
				}
			}
		}

		iiFace += numPoints + 1;

		if (iiFace > faceIndices.size())
		{
			inputCorrect = false;
			break;
		}
		if (iiFace == faceIndices.size())
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
	if (!poly_data)
	{
		return false;
	}

	std::vector<int>& faceIndices = poly_data->faceIndices;
	if (faceIndices.size() == 0)
	{
		return true;
	}

	size_t numPointsAll = poly_data->points.size();
	if (numPointsAll < 2)
	{
		return true;
	}
	bool inputCorrect = true;
	size_t maxPointIndex = numPointsAll - 1;

	std::vector<int> polyDataReversed;
	int numFacesCorrected = 0;

	for (size_t iiFace = 0; iiFace < faceIndices.size(); )
	{
		int numPoints = faceIndices[iiFace];
		int numPointsIdx = iiFace;

		if (iiFace + numPoints >= faceIndices.size())
		{
			// skip face
			break;
		}

		std::vector<int> pointIdxCurrentFace;
		for (size_t iiPoint = 1; iiPoint <= numPoints; ++iiPoint)
		{
			int idx = faceIndices[iiFace + iiPoint];
			pointIdxCurrentFace.push_back(idx);
		}

		polyDataReversed.push_back(numPoints);
		std::copy(pointIdxCurrentFace.rbegin(), pointIdxCurrentFace.rend(), std::back_inserter(polyDataReversed));

		iiFace += numPoints + 1;

		if (iiFace > faceIndices.size())
		{
			inputCorrect = false;
			break;
		}
		if (iiFace == faceIndices.size())
		{
			break;
		}
	}

	faceIndices = polyDataReversed;

	return inputCorrect;
}

void MeshOps::polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, PolyInputCache3D& polyInput, int minNumFacesPerMesh)
{
	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];

		if (minNumFacesPerMesh > 0)
		{
			if (mesh->faces.size() < minNumFacesPerMesh)
			{
				continue;
			}
		}

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

void MeshOps::polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, const std::unordered_set<const carve::mesh::Face<3>* >& setSkipFaces,
	const std::unordered_set<const carve::mesh::Face<3>* >& setFlipFaces, PolyInputCache3D& polyInput)
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
				if (setFlipFaces.find(face) != setFlipFaces.end())
				{
					polyInput.m_poly_data->addFace(vecPointIndexes.rbegin(), vecPointIndexes.rend());
				}
				else
				{
					polyInput.m_poly_data->addFace(vecPointIndexes.begin(), vecPointIndexes.end());
				}
			}
		}
	}
}

void MeshOps::polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, const std::unordered_set<const carve::mesh::Face<3>* >& setSkipFaces, PolyInputCache3D& polyInput)
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
						if (kk != face->n_edges - 1)
						{
							std::cout << "kk != face->n_edges - 1" << std::endl;
						}
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

void MeshOps::polyhedronFromMesh(const carve::mesh::Mesh<3>* mesh, PolyInputCache3D& polyInput)
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

void MeshOps::MeshSet2Polyhedron2MeshSet(const shared_ptr<carve::mesh::MeshSet<3> >& meshsetIn,
	shared_ptr<carve::mesh::MeshSet<3> >& meshsetOut, const GeomProcessingParams& params, int minNumFacesPerMesh)
{
	PolyInputCache3D polyInput(params.epsMergePoints);
	MeshOps::polyhedronFromMeshSet(meshsetIn, polyInput, minNumFacesPerMesh);

	std::string details;
	bool polyCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
	if (!polyCorrect)
	{
		fixPolyhedronData(polyInput.m_poly_data, params);
		polyCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
	}

	std::map<std::string, std::string> mesh_input_options;
	meshsetOut = shared_ptr<carve::mesh::MeshSet<3> >(polyInput.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));
}

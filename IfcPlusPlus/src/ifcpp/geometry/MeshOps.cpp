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

void MeshOps::recalcMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, double CARVE_EPSILON)
{
	for (auto mesh : meshset->meshes)
	{
		mesh->cacheEdges();
		mesh->recalc(CARVE_EPSILON);
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

size_t countEdges(carve::mesh::Face<3>* face)
{
	if (!face)
	{
		return 0;
	}
	size_t numEdges = 0;
	carve::mesh::Edge<3>* edge = face->edge;
	for (size_t ii = 0; ii < 10000; ++ii)
	{
		++numEdges;
		edge = edge->next;
		if (edge == face->edge)
		{
			break;
		}
	}

	face->n_edges = numEdges;
	return numEdges;
}

size_t removeFaceFromMesh(carve::mesh::Face<3>* fx)
{
	carve::mesh::Mesh<3>* mesh = fx->mesh;
	for (auto it = mesh->faces.begin(); it != mesh->faces.end(); ++it)
	{
		carve::mesh::Face<3>* f = *it;
		if (f == fx)
		{
			mesh->faces.erase(it);
			return 1;
		}
	}
	return 0;
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

	std::multimap<double, std::vector<carve::geom::vector<2> > > mapAreaLoops;
	for (size_t ii = 0; ii < polygonLoops.size(); ++ii)
	{
		std::vector<carve::geom::vector<2> >& loop = polygonLoops[ii];
		double area = std::abs(GeomUtils::signedArea(loop));
		mapAreaLoops.insert({ area, loop });
	}
	polygonLoops.clear();
	for (auto it = mapAreaLoops.rbegin(); it != mapAreaLoops.rend(); ++it)
	{
		polygonLoops.push_back(it->second);
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
		glm::vec4 color(0.3, 0.33, 0.33, 1.);
		GeomDebugDump::dumpLocalCoordinateSystem();
		GeomDebugDump::moveOffset(0.1);
		GeomDebugDump::dumpPolyline(polygonMerged, color, true);
		GeomDebugDump::moveOffset(0.1);
		GeomDebugDump::dumpPolyline(polygonLoops, color, true);
	}
#endif
}

void MeshOps::checkFaceLoops(const carve::mesh::Face<3>* face)
{
	carve::mesh::Edge<3>* e = face->edge;
	if (!e)
	{
		return;
	}
	size_t closed_edges = 0;

	size_t numEdges = face->n_edges;
	if (numEdges == 0)
	{
		return;
	}

	size_t ii = 0;
	do {
		if (e->rev == nullptr)
		{
			//open_edges.push_back(e);
		}
		else if (e < e->rev)
		{
			//closed_edges.push_back(e);
			++closed_edges;
		}
		e = e->next;

		if (ii > numEdges * 3)
		{
			break;
		}
		if (ii > 10000)
		{
			break;
		}
		++ii;

	} while (e != face->edge);

	if (ii > numEdges)
	{
		std::cout << "invalid edge loop " << std::endl;

#ifdef _DEBUG
		bool dumpFaces = true;
		if (dumpFaces)
		{
			GeomDebugDump::moveOffset(0.4);
			std::vector<const carve::mesh::Face<3>* > vecFaces = { face };
			glm::vec4 color2(0.7, 0.2, 0.2, 0.8);
			GeomDebugDump::dumpFaces(vecFaces, color2, false);
		}
#endif
	}
}

void checkFaceLoops(carve::mesh::Mesh<3>* mesh)
{
	size_t numFaces = mesh->faces.size();
	if (numFaces > 1000)
	{
		numFaces = 1000;
	}
	for (size_t i = 0; i < numFaces; ++i)
	{
		carve::mesh::Face<3>* face = mesh->faces[i];
		MeshOps::checkFaceLoops(face);
	}
}

void checkFaceLoops(carve::mesh::MeshSet<3>* meshset)
{
	for (size_t kk = 0; kk < meshset->meshes.size(); ++kk)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
		checkFaceLoops(mesh);
	}
}

void checkEdgeIntegrity(carve::mesh::Edge<3>* e, bool checkForDegenerateEdges, MeshSetInfo& info)
{
	if (!e)
	{
		info.allPointersValid = false;
		info.details = "edge is nullptr";
		return;
	}

	if (!e->rev)
	{
		info.allPointersValid = false;
		info.details = "edge->rev is nullptr";
		return;
	}


	if (!e->prev)
	{
		info.allPointersValid = false;
		info.details = "edge->prev is nullptr";
		return;
	}
	if (!e->next)
	{
		info.allPointersValid = false;
		info.details = "edge->next is nullptr";
		return;
	}
	if (!e->vert)
	{
		info.allPointersValid = false;
		info.details = "edge->vert is nullptr";
		return;
	}
	if (!e->face)
	{
		info.allPointersValid = false;
		info.details = "edge->face is nullptr";
		return;
	}

	if (checkForDegenerateEdges)
	{
		if (e->rev->rev != e)
		{
			info.degenerateEdges.insert(e);
			info.details = "edge->rev->rev != edge";
		}

		if (e->prev == e->next)
		{
			info.degenerateEdges.insert(e);
			info.details = "e->prev == e->next";
		}

		if (e->prev->vert == e->next->vert)
		{
#ifdef _DEBUG
			double length2 = e->length2();
			double length2prev = e->prev->length2();
			double length2next = e->next->length2();

			carve::mesh::Face<3>* f1 = e->face;
			carve::mesh::Face<3>* f2 = e->prev->face;
			carve::mesh::Face<3>* f3 = e->next->face;
			double area1 = MeshOps::computeFaceArea(f1);
			double area2 = MeshOps::computeFaceArea(f2);
			double area3 = MeshOps::computeFaceArea(f3);

			//glm::vec4 color(0.6, 0.6, 0.6, 1.);
			//GeomDebugDump::clearMeshsetDump();
			//GeomDebugDump::dumpFacePolygon(e->face, color, true);
#endif
			info.degenerateEdges.insert(e);
			info.details = "e->prev->vert == e->next->vert";
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

void checkFaceIntegrity(const carve::mesh::Face<3>* face, bool checkForDegenerateEdges, MeshSetInfo& info)
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

	const size_t n_edges = face->n_edges;
	if (n_edges > 10000)
	{
		info.maxNumEdgesExceeded = true;
#ifdef _DEBUG
		std::cout << "n_edges > 10000" << std::endl;
		glm::vec4 color(0.3, 0.3, 0.3, 1.);
		GeomDebugDump::dumpFacePolygon(face, color, false);
#endif
		return;
	}
	for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
	{
		checkEdgeIntegrity(e, checkForDegenerateEdges, info);

		if (!info.allPointersValid)
		{
			return;
		}

		checkEdgeIntegrity(e->rev, checkForDegenerateEdges, info);

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
		return;
	}

	try
	{
		e->validateLoop();
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

bool isBetterForExport(MeshSetInfo infoTriangulated, MeshSetInfo infoBefore)
{
	if (infoTriangulated.meshSetValid && infoTriangulated.maxNumberOfEdgesPerFace == 3)
	{
		return true;
	}

	if (infoTriangulated.maxNumberOfEdgesPerFace == 3)
	{
		size_t numEdgesBefore = infoBefore.numClosedEdges + infoBefore.numOpenEdges;
		size_t numEdgesTriangulated = infoTriangulated.numClosedEdges + infoTriangulated.numOpenEdges;
		bool condition2 = infoTriangulated.numClosedEdges > numEdgesBefore * 0.7;
		bool condition3 = infoTriangulated.numFaces > infoBefore.numFaces * 0.7;
		if (infoTriangulated.meshSetValid || condition2 || condition3)
		{
			return true;
		}
	}

	if (infoTriangulated.maxNumberOfEdgesPerFace == 3 && infoBefore.maxNumberOfEdgesPerFace == 3 )
	{
		// was already triangulated
		return false;
	}

	// futher tests

	return false;
}

void MeshOps::retriangulateMeshSetForExport( shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& paramsInput)
{
	if (!meshset)
	{
		return;
	}

	MeshSetInfo infoInput;
	bool validInput = MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, paramsInput);
	MeshOps::checkMeshSetNonNegativeAndClosed(meshset, paramsInput);

	if( infoInput.meshSetValid && infoInput.maxNumberOfEdgesPerFace == 3 )
	{
		return;
	}
	
	if ( infoInput.maxNumberOfEdgesPerFace == 3 )
	{
		// TODO: check if this is sufficient
		return;
	}

	GeomProcessingParams params(paramsInput);
	params.checkZeroAreaFaces = false;
	params.allowDegenerateEdges = true;
	params.allowFinEdges = true;
	PolyInputCache3D poly_cache(params.epsMergePoints);
	//if (!ignoreResultOpenEdges)
	{
		params.mergeAlignedEdges = true;
	}

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
			
			if (faceBound.size() == 3 && false)
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
			FaceConverter::createTriangulated3DFace(inputBounds3D, poly_cache, params);
		}
	}

	std::string details = "";
	bool correct = checkPolyhedronData(poly_cache.m_poly_data, params, details);
	if (!correct)
	{
		fixPolyhedronData(poly_cache.m_poly_data, true, params);
		correct = checkPolyhedronData(poly_cache.m_poly_data, params, details);

#ifdef _DEBUG
		if (!correct)
		{
			std::cout << "fixPolyhedronData  failed" << std::endl;
		}
#endif
	}

	shared_ptr<carve::mesh::MeshSet<3>> meshsetTriangulated = shared_ptr<carve::mesh::MeshSet<3> >(poly_cache.m_poly_data->createMesh(carve::input::opts(), params.epsMergePoints));
	MeshSetInfo infoTriangulated;
	MeshOps::checkMeshSetValidAndClosed(meshsetTriangulated, infoTriangulated, params);

	if( isBetterForExport( infoTriangulated, infoInput ))
	{
		meshset.reset();
		meshset = meshsetTriangulated;
	}

	//if (!validTriangulatedMesh)
	{
#ifdef _DEBUG
		bool dumpMesh = true;
		if (validInput && dumpMesh && meshset->vertex_storage.size() > 60)
		{
			GeomDebugDump::DumpSettingsStruct dumpSet;
			dumpSet.triangulateBeforeDump = false;
			GeomProcessingParams paramCopy(params);
			paramCopy.checkZeroAreaFaces = false;
			
			GeomDebugDump::dumpLocalCoordinateSystem();
			GeomDebugDump::moveOffset(0.3);
			GeomDebugDump::dumpWithLabel("triangulate:input ", meshset, dumpSet, paramCopy, true, true);
			GeomDebugDump::moveOffset(0.3);
			GeomDebugDump::dumpWithLabel("triangulate:result", meshsetTriangulated, dumpSet, paramCopy, true, true);
		}
#endif
		MeshOps::checkMeshSetValidAndClosed(meshsetTriangulated, infoTriangulated, params);
		
		if (isBetterForExport(infoTriangulated, infoInput))
		{
			meshset.reset();
			meshset = meshsetTriangulated;
		}
	}
}

void MeshOps::retriangulateMeshSetForBoolOp(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, const GeomProcessingParams& params, size_t retryCount)
{
	retriangulateMeshSetForBoolOp_earcut(meshset, ignoreResultOpenEdges, params, retryCount);
	//retriangulateMeshSetForBoolOp_carve(meshset, ignoreResultOpenEdges, params, retryCount);
}

void MeshOps::retriangulateMeshSetForBoolOp_earcut(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, const GeomProcessingParams& paramsInput, size_t retryCount)
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

	
	PolyInputCache3D poly_cache(paramsInput.epsMergePoints);  // TODO: check to use double

	GeomProcessingParams params(paramsInput);
	
	//if (!ignoreResultOpenEdges)
	{
		params.mergeAlignedEdges = false;
	}

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
			FaceConverter::createTriangulated3DFace(inputBounds3D, poly_cache, params);
		}
	}

	shared_ptr<carve::mesh::MeshSet<3>> meshsetTrinangulated1 = shared_ptr<carve::mesh::MeshSet<3> >(poly_cache.m_poly_data->createMesh(carve::input::opts(), params.epsMergePoints));

	std::string details = "";
	bool correct = checkPolyhedronData(poly_cache.m_poly_data, params, details);
	if (!correct)
	{
		fixPolyhedronData(poly_cache.m_poly_data, true, params);
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

void MeshOps::retriangulateMeshSetForBoolOp_carve(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, const GeomProcessingParams& params, size_t retryCount)
{
	if (!meshset)
	{
		return;
	}

	MeshSetInfo info;
	bool validInput = MeshOps::checkMeshSetValidAndClosed(meshset, info, params);
	MeshOps::checkMeshSetNonNegativeAndClosed(meshset, params);

	if (info.maxNumberOfEdgesPerFace == 3 && validInput)
	{
		return;
	}


	PolyInputCache3D poly_cache(params.epsMergePoints);  // TODO: check to use double
	std::vector<size_t> map_merged_idx;
	map_merged_idx.resize(meshset->vertex_storage.size(), 0);
	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
		std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;

		for (size_t i2 = 0; i2 < vec_faces.size(); ++i2)
		{
			carve::mesh::Face<3>* face = vec_faces[i2];

#ifdef _DEBUG
			std::vector<int> face_idx;
			//getFaceIndexes(face, meshset->vertex_storage, face_idx);
#endif

			std::vector<vec2> verts2d;
			face->getProjectedVertices(verts2d);
			if (verts2d.size() < 3)
			{
				continue;
			}

			// now insert points to polygon, avoiding points with same coordinates
			int i_vert = 0;
			carve::mesh::Edge<3>* edge = face->edge;
			for (size_t i3 = 0; i3 < face->n_edges; ++i3)
			{
				const vec3& v = edge->vert->v;
				edge = edge->next;
				int vertex_index = poly_cache.addPoint(v);
				if (i_vert >= map_merged_idx.size())
				{
					map_merged_idx.resize(i_vert + 1, 0);
				}
				map_merged_idx[i_vert] = vertex_index;
				++i_vert;
				if (edge == face->edge)
				{
					break;
				}
			}

			if (retryCount > 0)
			{
#ifdef _DEBUG
				if (verts2d.size() > 7)
				{
					glm::vec4 color(0.3, 0.33, 0.33, 1.);
					GeomDebugDump::dumpLocalCoordinateSystem();
					GeomDebugDump::moveOffset(0.1);
					GeomDebugDump::dumpPolyline(verts2d, color, true);

					std::vector<std::vector<carve::geom::vector<2> > > polygon_earcut;
					std::map<size_t, std::array<size_t, 2> > mapFlat2NestedArray;
					splitIntoSubLoops(verts2d, polygon_earcut, mapFlat2NestedArray, params.epsMergePoints, false);

					GeomDebugDump::moveOffset(0.1);
					GeomDebugDump::dumpPolyline(polygon_earcut, color, true);
				}
#endif

				std::map<size_t, size_t> mapFlat2NestedArray;
				if (verts2d.size() > 7)
				{
					shiftSubLoops(verts2d, mapFlat2NestedArray, params.epsMergePoints, false);
				}

#ifdef _DEBUG
				if (verts2d.size() > 7)
				{
					glm::vec4 color(0.3, 0.33, 0.33, 1.);
					GeomDebugDump::dumpLocalCoordinateSystem();
					GeomDebugDump::moveOffset(0.1);
					GeomDebugDump::dumpPolyline(verts2d, color, true);

					//std::vector<std::vector<carve::geom::vector<2> > > polygon_earcut;
					//GeomDebugDump::moveOffset(0.1);
					//GeomDebugDump::dumpPolyline(polygon_earcut, color, true);
				}
#endif

			}

			std::vector<carve::triangulate::tri_idx> triangulated;
			if (verts2d.size() > 3)
			{
				try
				{
					carve::triangulate::triangulate(verts2d, triangulated, params.epsMergePoints);
					carve::triangulate::improve(verts2d, triangulated);
				}
				catch (...)
				{
#ifdef _DEBUG
					std::cout << __FUNC__ << " carve::triangulate failed " << std::endl;
#endif
					continue;
				}
			}
			else
			{
				triangulated.push_back(carve::triangulate::tri_idx(0, 1, 2));
			}

			for (size_t i4 = 0; i4 != triangulated.size(); ++i4)
			{
				const carve::triangulate::tri_idx& triangle = triangulated[i4];
				int a = triangle.a;
				int b = triangle.b;
				int c = triangle.c;

				int vertex_id_a = map_merged_idx[a];
				int vertex_id_b = map_merged_idx[b];
				int vertex_id_c = map_merged_idx[c];

				if (vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c)
				{
					continue;
				}

				const carve::poly::Vertex<3>& v_a = poly_cache.m_poly_data->getVertex(vertex_id_a);
				const carve::poly::Vertex<3>& v_b = poly_cache.m_poly_data->getVertex(vertex_id_b);

				double dx = v_a.v[0] - v_b.v[0];
				if (std::abs(dx) < 0.0000001)
				{
					double dy = v_a.v[1] - v_b.v[1];
					if (std::abs(dy) < 0.0000001)
					{
						double dz = v_a.v[2] - v_b.v[2];
						if (std::abs(dz) < 0.0000001)
						{
#ifdef _DEBUG
							std::cerr << "degenerated triangle: std::abs(dx) < 0.00001 && std::abs(dy) < 0.00001 && std::abs(dz) < 0.00001\n";
#endif	
							continue;
						}
					}
				}

				poly_cache.m_poly_data->addFace(vertex_id_a, vertex_id_b, vertex_id_c);
			}
		}
	}

	shared_ptr<carve::mesh::MeshSet<3>> meshsetTrinangulated1 = shared_ptr<carve::mesh::MeshSet<3> >(poly_cache.m_poly_data->createMesh(carve::input::opts(), params.epsMergePoints));

	std::string details = "";
	bool correct = checkPolyhedronData(poly_cache.m_poly_data, params, details);
	if (!correct)
	{
		fixPolyhedronData(poly_cache.m_poly_data, true, params);
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

void _link(carve::mesh::Edge<3>* a, carve::mesh::Edge<3>* b)
{
	a->next = b;
	b->prev = a;
}

void _setloopface(carve::mesh::Edge<3>* s, carve::mesh::Face<3>* f)
{
	carve::mesh::Edge<3>* e = s;
	do
	{
		e->face = f;
		e = e->next;
	} while (e != s);
}

carve::mesh::Edge<3>* checkMergeFaces(carve::mesh::Edge<3>* e, const GeomProcessingParams& params)
{
	if (e->rev == nullptr)
	{
		return nullptr;
	}


	carve::mesh::Face<3>* fwdface = e->face;
	carve::mesh::Face<3>* revface = e->rev->face;

	if (fwdface == revface)
	{
		return nullptr;
	}

	if (fwdface->n_edges > params.generalSettings->m_maxNumFaceEdges)
	{
		return nullptr;
	}
	if (revface->n_edges > params.generalSettings->m_maxNumFaceEdges)
	{
		return nullptr;
	}


#ifdef _DEBUG

	GeomDebugDump::ScopedDumpBuffering scoped_buffer;
	if (params.debugDump)
	{
		glm::vec4 color(0.3, 0.3, 0.3, 1.);
		std::vector<const carve::mesh::Face<3>* > vecFaces = { fwdface };
		GeomDebugDump::dumpFaces(vecFaces, color, false);
		GeomDebugDump::dumpFacePolygon(revface, color, false);
	}
#endif

	size_t n_removed = 0;

	carve::mesh::Edge<3>* splice_beg = e;
	for (size_t ii = 0; ii < fwdface->n_edges; ++ii)
	{
		splice_beg = splice_beg->prev;
		++n_removed;

		if (splice_beg == e) { break; }
		if (!splice_beg->rev) { break; }
		if (splice_beg->next->rev->prev != splice_beg->rev) { break; }
	}

	if (splice_beg == e)
	{
		// edge loops are completely matched.
		return nullptr;
	}

	carve::mesh::Edge<3>* splice_end = e;
	do {
		splice_end = splice_end->next;
		++n_removed;
	} while (splice_end->rev && splice_end->prev->rev->next == splice_end->rev);

	--n_removed;

	carve::mesh::Edge<3>* link1_p = splice_beg;
	carve::mesh::Edge<3>* link1_n = splice_beg->next->rev->next;

	carve::mesh::Edge<3>* link2_p = splice_end->prev->rev->prev;
	carve::mesh::Edge<3>* link2_n = splice_end;

	CARVE_ASSERT(link1_p->face == fwdface);
	CARVE_ASSERT(link1_n->face == revface);

	CARVE_ASSERT(link2_p->face == revface);
	CARVE_ASSERT(link2_n->face == fwdface);

	carve::mesh::Edge<3>* left_loop = link1_p->next;

	CARVE_ASSERT(left_loop->rev == link1_n->prev);

	_link(link2_n->prev, link1_p->next);
	_link(link1_n->prev, link2_p->next);

	_link(link1_p, link1_n);
	_link(link2_p, link2_n);

	fwdface->edge = link1_p;

	size_t edgeCount = 0;
	for (carve::mesh::Edge<3>*e = link1_n; e != link2_n; e = e->next)
	{
		CARVE_ASSERT(e->face == revface);
		e->face = fwdface;
		fwdface->n_edges++;
		++edgeCount;
		if (edgeCount > params.generalSettings->m_maxNumFaceEdges)
		{
			std::logic_error ex("edgeCount > m_maxNumFaceEdges");
			throw std::exception(ex);
			return nullptr;
		}
	}
	edgeCount = 0;
	for (carve::mesh::Edge<3>*e = link2_n; e != link1_n; e = e->next)
	{
		CARVE_ASSERT(e->face == fwdface);
		++edgeCount;
		if (edgeCount > params.generalSettings->m_maxNumFaceEdges)
		{
			std::logic_error ex("edgeCount > m_maxNumFaceEdges");
			throw std::exception(ex);
			return nullptr;
		}
	}

	fwdface->n_edges -= n_removed;

	revface->n_edges = 0;
	revface->edge = nullptr;

	_setloopface(left_loop, nullptr);
	_setloopface(left_loop->rev, nullptr);

#ifdef _DEBUG
	GeomDebugDump::clearBuffer();
#endif

	return left_loop;
}

bool MeshOps::isBetterForBoolOp(const MeshSetInfo& infoNew, const MeshSetInfo& infoBefore, bool considerTriangulation)
{
	if (infoNew.numOpenEdges > infoBefore.numOpenEdges)
	{
		return false;
	}

	if (!infoNew.meshSetValid && infoBefore.meshSetValid)
	{
		return false;
	}

	// fewer open edges is better
	int numOpenEdgesRemoved = infoBefore.numOpenEdges - infoNew.numOpenEdges;
	int numClosedEdgesRemoved = infoBefore.numClosedEdges - infoNew.numClosedEdges;
	
	int numDegenerateEdgesRemoved = infoBefore.degenerateEdges.size() - infoNew.degenerateEdges.size();
	int numFinEdgesRemoved = infoBefore.finEdges.size() - infoNew.finEdges.size();
	int numFinFacesRemoved = infoBefore.finFaces.size() - infoNew.finFaces.size();

	size_t numAllEdgesNew = infoNew.numClosedEdges + infoNew.numOpenEdges;
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

bool assignIfBetterForBoolOp( shared_ptr<carve::mesh::MeshSet<3> >& meshsetNew, shared_ptr<carve::mesh::MeshSet<3> >& meshsetBefore, const MeshSetInfo& infoNew, MeshSetInfo& infoBefore, bool considerTriangulation)
{
	if (MeshOps::isBetterForBoolOp(infoNew, infoBefore, considerTriangulation))
	{
		meshsetBefore = meshsetNew;
		infoBefore.copyFromOther( infoNew );
		return true;
	}

	return false;
}

inline void countReferencesToVertex(const carve::mesh::Edge<3>* edge, const carve::mesh::Vertex<3>* vertex, std::set<const carve::mesh::Edge<3>* >& setEdgesReferencing)
{
	if (!edge->face)
	{
		return;
	}

	for (size_t jj = 0; jj < edge->face->mesh->faces.size(); ++jj)
	{
		carve::mesh::Face<3>* face3 = edge->face->mesh->faces[jj];
		carve::mesh::Edge<3>* e3 = face3->edge;
		for (size_t kk = 0; kk < face3->n_edges; ++kk)
		{
			if (e3)
			{
				//if( e3 != edge )
				{
					if (e3->vert == vertex)
					{
						setEdgesReferencing.insert(e3);
					}
				}
			}
			e3 = e3->next;
		}
	}
}


inline void removeFinEdgesFromFace(const carve::mesh::Face<3>* face, size_t& numChanges, double eps)
{
	return;

	if (!face)
	{
		return;
	}

	carve::mesh::Mesh<3>* mesh = face->mesh;
	if (mesh->open_edges.size() == 0)
	{
		return;
	}

	carve::mesh::Edge<3>* e = face->edge;
	if (!e)
	{
		return;
	}

	const size_t n_edges = face->n_edges;
	std::set<carve::mesh::Edge<3>* > setEdgesToRemove;
	for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
	{
		carve::mesh::Edge<3>* degenerateEdge = nullptr;

		double edgeLength2 = e->length2();
		if (edgeLength2 < eps * eps)
		{
			degenerateEdge = e;
		}

		bool condition1 = e->prev->vert == e->next->vert;
		bool condition2 = e->next == e->rev;
		if (condition1 || condition2)
		{
			// check if the vertex is used by other edges
			degenerateEdge = e;
			std::set<const carve::mesh::Edge<3>* > setEdgesReferencing;
			carve::mesh::Vertex<3>* vertex = e->vert;
			countReferencesToVertex(e, vertex, setEdgesReferencing);
			size_t numReferences = setEdgesReferencing.size();

			if (numReferences > 1)
			{
				// other edges referencing current vertex
				degenerateEdge = nullptr;
				carve::mesh::Edge<3>* reverseEdge = e->rev;
				if (reverseEdge != nullptr)
				{
					carve::mesh::Vertex<3>* vertexReverseEdge = reverseEdge->vert;
					std::set<const carve::mesh::Edge<3>* > setEdgesReferencingReferseEdge;
					countReferencesToVertex(reverseEdge, vertexReverseEdge, setEdgesReferencingReferseEdge);
					size_t numReferencesReverse = setEdgesReferencingReferseEdge.size();

					if (numReferencesReverse == 1)
					{
						degenerateEdge = reverseEdge;
					}
				}
			}
		}

		if (degenerateEdge != nullptr)
		{
			carve::mesh::Edge<3>* degenerateEdgeReverse = degenerateEdge->rev;
			if (degenerateEdgeReverse != nullptr)
			{
				auto itFindEdge = setEdgesToRemove.find(degenerateEdgeReverse);
				if (itFindEdge == setEdgesToRemove.end())
				{
					setEdgesToRemove.insert(degenerateEdge);
				}
			}
			else
			{
				setEdgesToRemove.insert(degenerateEdge);
			}
		}
		e = e->next;
	}

	for (carve::mesh::Edge<3>*edgeRemove : setEdgesToRemove)
	{
		std::set<carve::mesh::Face<3>* > setFacesToReplaceEdgePointer;
		if (face->edge == edgeRemove)
		{
			carve::mesh::Face<3>* faceNonConst = (carve::mesh::Face<3>*)face;
			setFacesToReplaceEdgePointer.insert(faceNonConst);
		}

		for (auto faceInMesh : face->mesh->faces)
		{
			if (faceInMesh->edge == edgeRemove)
			{
				setFacesToReplaceEdgePointer.insert(faceInMesh);
			}
		}

		carve::mesh::Edge<3>* edgeRemainingNext = edgeRemove->removeEdge();
		for (auto faceReplaceEdgePointer : setFacesToReplaceEdgePointer)
		{
			if (faceReplaceEdgePointer->edge != edgeRemainingNext)
			{
				faceReplaceEdgePointer->edge = edgeRemainingNext;
			}
		}

		++numChanges;
	}
}

inline void removeFinEdges(carve::mesh::Mesh<3>* mesh, const GeomProcessingParams& params)
{
	if (!mesh)
	{
		return;
	}

	const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
	size_t numFaces = vec_faces.size();

#ifdef _DEBUG
	if (numFaces == 1 && false)
	{
		glm::vec4 color(0.4, 0.2, 0.2, 1.);
		std::vector<const carve::mesh::Face<3>* > vecFaces;
		vecFaces.push_back(vec_faces[0]);
		GeomDebugDump::moveOffset(0.3);
		GeomDebugDump::stopBuffering();
		GeomDebugDump::dumpFaces(vecFaces, color);
		GeomDebugDump::moveOffset(0.3);
		GeomDebugDump::dumpFacePolygon(vec_faces[0], color, true);
	}
#endif

	for (size_t ii = 0; ii < numFaces; ++ii)
	{
		size_t numChangesAll = 0;
		for (size_t jj = 0; jj < vec_faces.size(); ++jj)
		{
			size_t numChangesCurrentFace = 0;
			carve::mesh::Face<3>* face = vec_faces[jj];
			removeFinEdgesFromFace(face, numChangesCurrentFace, params.epsMergePoints);
			numChangesAll += numChangesCurrentFace;
		}

		// several fin-edges (where edge->next == edge->reverse) can be concatenated. Repeat until there are no changes
		if (numChangesAll > 0)
		{
			if (mesh->faces.size() < 2)
			{
				continue;
			}

			bool checkForDegenerateEdges = false;
			MeshSetInfo minf;
			MeshOps::checkMeshPointers(mesh, checkForDegenerateEdges, params, minf);

			if (!minf.allPointersValid)
			{
				continue;
			}

			mesh->cacheEdges();
			mesh->recalc(params.epsMergePoints);
		}

		if (numChangesAll == 0)
		{
			break;
		}
	}
}

void removeFinEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params)
{
	for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
	{
		removeFinEdges(mesh, params);
	}
}

void findFinEdges(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::set<carve::mesh::Edge<3>* >& setFinEdges, const GeomProcessingParams& params)
{
	for (const carve::mesh::Mesh<3>*mesh : meshset->meshes)
	{
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

			// compute face normal here. Don't rely on face->plane.N, since the winding order might be changed
			const vec3 faceNormal = MeshOps::computeFaceNormal(face);
			const vec3 face2Normal = MeshOps::computeFaceNormal(adjacentFace);

			// adjacent faces back-to-back have -1 as normal vector dot product
			double dotProduct = dot(faceNormal, face2Normal);
			if (std::abs(dotProduct + 1.0) < params.epsMergeAlignedEdgesAngle*10)
			{
				setFinEdges.insert(edge);
			}
		}
	}
}

void removeFinFaces(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params)
{
	//return;
	MeshSetInfo infoInput;
	MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, params);

	std::set<carve::mesh::Face<3>* > setFacesRemove;
	for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
	{
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

			// compute face normal here. Don't rely on face->plane.N, since the winding order might be changed
			const vec3 faceNormal = MeshOps::computeFaceNormal(face);
			const vec3 face2Normal = MeshOps::computeFaceNormal(adjacentFace);

			// adjacent faces back-to-back have -1 as normal vector dot product
			double dotProduct = dot(faceNormal, face2Normal);
			if (std::abs(dotProduct + 1.0) < params.epsMergeAlignedEdgesAngle)
			{
				setFacesRemove.insert(face);
				setFacesRemove.insert(adjacentFace);
			}
		}
	}

	if (setFacesRemove.size() > 0)
	{
		PolyInputCache3D polyInput(params.epsMergePoints);
		polyhedronFromMeshSet(meshset, setFacesRemove, polyInput);

		std::string details;
		bool polyCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
		if (!polyCorrect)
		{
			fixPolyhedronData(polyInput.m_poly_data, true, params);
		}

		std::map<std::string, std::string> mesh_input_options;
		shared_ptr<carve::mesh::MeshSet<3> > resultFromPolyhedron(polyInput.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));
		MeshSetInfo info;
		bool mesh_ok = MeshOps::checkMeshSetValidAndClosed(resultFromPolyhedron, info, params);

		assignIfBetterForBoolOp(resultFromPolyhedron, meshset, info, infoInput, false);

		if (mesh_ok)
		{
#ifdef _DEBUG
			if (params.debugDump)
			{
				glm::vec4 color(0.5, 0.5, 0.5, 1);
				GeomDebugDump::stopBuffering();
				GeomDebugDump::moveOffset(0.15);
				bool drawNormals = true;
				GeomDebugDump::dumpMeshset(meshset, color, drawNormals, true);
				GeomDebugDump::moveOffset(0.1);
				GeomDebugDump::dumpMeshset(resultFromPolyhedron, color, drawNormals, true);
			}
#endif
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
			GeomDebugDump::dumpWithLabel("removeDegenerateFacesInMeshSet--input", meshsetInput, dumpColorSettings, params, true, true);
			
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

void MeshOps::removeDegenerateFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& paramsInput, bool ensureValidMesh)
{
	if (!meshsetInput)
	{
		return;
	}

	GeomProcessingParams params(paramsInput);
	params.allowZeroAreaFaces = true;
	MeshSetInfo infoInput;
	bool meshInputOk = MeshOps::checkMeshSetValidAndClosed(meshsetInput, infoInput, params);

	std::set<carve::mesh::Face<3>* > setFacesRemove;
	std::map<carve::mesh::Face<3>*, std::set<carve::mesh::Edge<3>* > > degenerateEdges;  // TODO: handle degenerate edges
	for (carve::mesh::Mesh<3>*mesh : meshsetInput->meshes)
	{
		double meshVolume = mesh->volume();
		if (meshVolume < params.epsMergePoints)
		{
			// remove complete mesh
			for (carve::mesh::Face<3>*face : mesh->faces)
			{
				if (!face)
				{
					continue;
				}

				setFacesRemove.insert(face);
			}
			continue;
		}

		for (carve::mesh::Face<3>*face : mesh->faces)
		{
			if (!face)
			{
				continue;
			}

			if (face->n_edges < 3)
			{
				setFacesRemove.insert(face);
				continue;
			}

			double longestEdge = 0;
			double area = MeshOps::computeFaceArea(face, longestEdge);
			if (std::abs(area) < params.minFaceArea )
			{
				if (params.treatLongThinFaceAsDegenerate)
				{
					setFacesRemove.insert(face);
				}
				else if (longestEdge < params.epsMergePoints)
				{
					setFacesRemove.insert(face);
				}
			}

#ifdef _DEBUG
			const vec3 faceNormal = MeshOps::computeFaceNormal(face);
			vec3 posY = carve::geom::VECTOR(0, 1, 0);
			double dotProduct = dot(faceNormal, posY);
			bool onMainAxis = false;
			if (std::abs(dotProduct - 1.0) < 0.00001)
			{
				onMainAxis = true;
			}
			if (std::abs(dotProduct + 1.0) < 0.00001)
			{
				onMainAxis = true;
			}
			if (std::abs(dotProduct) < 0.00001)
			{
				// up/down or pos/neg x
				onMainAxis = true;
			}

			if (std::abs(area) > params.minFaceArea)
			{
				onMainAxis;
			}
#endif

			MeshSetInfo info;
			bool checkForDegenerateEdges = false;
			checkFaceIntegrity(face, checkForDegenerateEdges, info);

			if (!info.allPointersValid)
			{
				setFacesRemove.insert(face);
				continue;
			}

			if (info.degenerateEdges.size() > 0)
			{
				degenerateEdges[face] = info.degenerateEdges;
				continue;
			}
		}
	}

	if (setFacesRemove.size() > 0)
	{
		PolyInputCache3D polyInput(params.epsMergePoints);
		polyhedronFromMeshSet(meshsetInput, setFacesRemove, polyInput);

		std::string details;
		bool polyCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
		if (!polyCorrect)
		{
			fixPolyhedronData(polyInput.m_poly_data, true, params);
		}

		std::map<std::string, std::string> mesh_input_options;
		shared_ptr<carve::mesh::MeshSet<3> > resultFromPolyhedron(polyInput.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));
		MeshSetInfo info;
		bool mesh_ok = MeshOps::checkMeshSetValidAndClosed(resultFromPolyhedron, info, params);
#ifdef _DEBUG
		if (meshInputOk && !mesh_ok)
		{
			GeomDebugDump::DumpSettingsStruct dumpColorSettings;
			GeomDebugDump::dumpWithLabel("removeDegenerateFacesInMeshSet--input", meshsetInput, dumpColorSettings, params, true, true);
			GeomDebugDump::dumpWithLabel("removeDegenerateFacesInMeshSet--result", resultFromPolyhedron, dumpColorSettings, params, true, true);
		}
#endif

		if (ensureValidMesh)
		{
			if (isBetterForBoolOp(info, infoInput, true))
			{
				meshsetInput = resultFromPolyhedron;
			}
		}
	}
}

void MeshOps::checkMeshPointers(const carve::mesh::Mesh<3>* mesh, bool checkForDegenerateEdges, const GeomProcessingParams& params, MeshSetInfo& info)
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
		checkFaceIntegrity(face, checkForDegenerateEdges, info);

		if (!info.allPointersValid)
		{
			return;
		}
	}
}

inline bool edgeToEdgeIntersect(const carve::mesh::Edge<3>* edge1, const carve::mesh::Edge<3>* edge2, double eps, vec3& intersectionPoint)
{
	const vec3 A = edge1->v1()->v;
	const vec3 B = edge1->v2()->v;
	const vec3 C = edge2->v1()->v;
	const vec3 D = edge2->v2()->v;

	vec3 u = B - A;
	vec3 v = D - C;
	vec3 w = A - C;

	double    a = dot(u, u);         // always >= 0
	double    b = dot(u, v);
	double    c = dot(v, v);         // always >= 0
	double    d = dot(u, w);
	double    e = dot(v, w);
	double    sc, sN, sD = a * c - b * b;  // sc = sN / sD, sD >= 0
	double    tc, tN, tD = a * c - b * b;  // tc = tN / tD, tD >= 0

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

	vec3 P1 = A + (sc * u);
	vec3 P2 = C + (tc * v);
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

size_t removeEdgeAndMergeFaces(carve::mesh::Edge<3>* edgeIn, const GeomProcessingParams& params)
{
	double eps = params.epsMergePoints;
	carve::mesh::Face<3>* face = edgeIn->face;
	double faceArea = MeshOps::computeFaceArea(face);
	if (std::abs(faceArea) < eps * 10)
	{
		return 0;
	}

	vec3& facePosition_carve = edgeIn->v2()->v;
	size_t numChanges = 0;
	size_t numFacesDeleted = 0;

#ifdef _DEBUG
	if (params.debugDump)
	{
		GeomDebugDump::moveOffset(0.4);
		glm::vec4 color2(0.3, 0.2, 0.2, 0.8);
		//dumpAdjacentFaces(par.setAdjacentCoplanarFaces, color2);
	}
#endif

	carve::mesh::Face<3>* faceOnRverseEdge = edgeIn->rev->face;
	carve::mesh::Face<3>* faceOnEdge = edgeIn->face;
	if (faceOnEdge == nullptr)
	{
		return 0;
	}

	if (faceOnRverseEdge == nullptr)
	{
		return 0;
	}

	if (faceOnEdge == faceOnRverseEdge)
	{
		// remaining edge between inner opening of face and outer boundary. Leave it for triangulation
		return 0;
	}
	size_t numFacesBeforeMerge = faceOnRverseEdge->mesh->faces.size();

	carve::mesh::Edge<3>* edgeErase = edgeIn;
	size_t numEdgesFace = countEdges(faceOnEdge);
	size_t numEdgesFaceReverse = countEdges(faceOnRverseEdge);

	if (numEdgesFace >= params.generalSettings->m_maxNumFaceEdges)
	{
		std::logic_error ex("edgeCount > m_maxNumFaceEdges");
		throw std::exception(ex);
	}
	if (numEdgesFaceReverse >= params.generalSettings->m_maxNumFaceEdges)
	{
		std::logic_error ex("edgeCount > m_maxNumFaceEdges");
		throw std::exception(ex);
	}

#ifdef _DEBUG
	if (params.debugDump)
	{
		GeomDebugDump::moveOffset(0.05);
		glm::vec4 color2(0.3, 0.2, 0.2, 0.8);
		std::vector<carve::geom::vector<3> > vecEdgePoints = { edgeErase->v1()->v, edgeErase->v2()->v };
		GeomDebugDump::dumpPolyline(vecEdgePoints, color2, false);
	}
#endif

	MeshSetInfo info2;
	bool checkForDegenerateEdges = false;
	checkFaceIntegrity(edgeErase->face, checkForDegenerateEdges, info2);

	MeshSetInfo info3;
	checkFaceIntegrity(edgeErase->rev->face, checkForDegenerateEdges, info3);

	if (!info2.allPointersValid)
	{
		return 0;
	}
	if (!info3.allPointersValid)
	{
		return 0;
	}

	carve::mesh::Edge<3>* edgeMergeNext = checkMergeFaces(edgeErase, params);
	if (!edgeMergeNext)
	{
		return 0;
	}

	carve::mesh::Face<3>* faceRemain = nullptr;
	carve::mesh::Face<3>* faceRemove = nullptr;

	if (faceOnRverseEdge->edge)
	{
		faceRemain = faceOnRverseEdge;
		size_t numEdgesFaceRemove_afterMerge = countEdges(faceOnRverseEdge);
		if (numEdgesFaceRemove_afterMerge > params.generalSettings->m_maxNumFaceEdges)
		{
			std::logic_error ex("faceOnRverseEdge->edge count > maxNumFaceEdges");
			throw std::exception(ex);
		}
		if (numEdgesFaceReverse != numEdgesFaceRemove_afterMerge)
		{
			++numChanges;
		}
	}
	else
	{
		faceRemove = faceOnRverseEdge;
	}

	if (faceOnEdge->edge)
	{
		faceRemain = faceOnEdge;
		size_t numEdgesFaceRemain_afterMerge = countEdges(faceOnEdge);
		if (numEdgesFaceRemain_afterMerge > params.generalSettings->m_maxNumFaceEdges)
		{
			std::logic_error ex("faceOnEdge->edge count > maxNumFaceEdges");
			throw std::exception(ex);
		}
		if (numEdgesFace != numEdgesFaceRemain_afterMerge)
		{
			++numChanges;
		}
	}
	else
	{
		faceRemove = faceOnEdge;
	}

#ifdef _DEBUG
	if (params.debugDump)
	{
		GeomDebugDump::moveOffset(0.05);
		glm::vec4 color2(0.3, 0.2, 0.2, 0.8);
		GeomDebugDump::dumpFacePolygon({ faceRemain }, color2, false);
	}
#endif

	// replace pointers to faceRemove
	carve::mesh::Edge<3>* e = faceRemove->edge;
	for (size_t ii = 0; ii < faceRemove->n_edges; ++ii)
	{
		if (e == nullptr)
		{
			continue;
		}

		if (e->face == faceRemove)
		{
			e->face = faceRemain;
		}
		e = e->next;
	}

	numChanges += removeFaceFromMesh(faceRemove);
	delete faceRemove;
	++numFacesDeleted;

	if (!faceRemain)
	{
		return numChanges;
	}

	auto mesh = faceRemain->mesh;
	try
	{
		faceRemain->edge->validateLoop();
	}
	catch (carve::exception& e)
	{
		std::cout << "validateLoop failed: " << e.str();
	}

	checkFaceLoops(mesh);

	mesh->cacheEdges();
	//mesh->recalc(eps);
	++numChanges;

	// TODO: enforceMergedFacesToCommonPlane() : compute normal vector and centroid of merged face, then move all vertices precisely into that plane

	bool allVerticesInPlane = true;
	std::vector<carve::mesh::Vertex<3>* > faceVertices;
	faceRemain->getVertices(faceVertices);
	if (faceVertices.size() > 3)
	{
		carve::geom::vector<3> normalVector = GeomUtils::computePolygonNormal(faceVertices);
		carve::geom::vector<3> centroid = faceRemain->centroid();
		GeomUtils::Plane plane(glm::dvec3(centroid.x, centroid.y, centroid.z), glm::dvec3(normalVector.x, normalVector.y, normalVector.z));
		
		for (carve::mesh::Face<3>::vertex_t * vertex : faceVertices)
		{
			const carve::geom::vector<3>& facePoint_carve = vertex->v;
			glm::dvec3 facePoint(facePoint_carve.x, facePoint_carve.y, facePoint_carve.z);

			double distanceToPlane = plane.distancePointPlane(facePoint);

			if (std::abs(distanceToPlane) > params.epsMergePoints)
			{
				vec3 pointOnPlane = facePoint_carve + normalVector * distanceToPlane;

#ifdef _DEBUG
				glm::dvec3 pointOnPlane_glm(pointOnPlane.x, pointOnPlane.y, pointOnPlane.z);
				double distanceToPlaneCheck = plane.distancePointPlane(pointOnPlane_glm);

				double maxAllowedDistance = params.epsMergePoints * 5.0;
				if (std::abs(distanceToPlaneCheck) > maxAllowedDistance)
				{
					std::cout << "distanceToPlaneCheck too big: " << distanceToPlaneCheck << std::endl;
				}
#endif
				vertex->v = pointOnPlane;

				// TODO: average out current vertices between all faces that are connected
				// map<vertex, std::vector<face>>
			}
		}

		if (allVerticesInPlane)
		{
			return numChanges;
		}
	}

#ifdef _DEBUG
	size_t numFacesAfterMerge = mesh->faces.size();
	int numFacesMerged = numFacesBeforeMerge - numFacesAfterMerge;

	MeshSetInfo info6;
	MeshOps::checkMeshPointers(mesh, checkForDegenerateEdges, params, info6);
	if (!info6.allPointersValid || !allVerticesInPlane)
	{
		glm::vec4 color(0.2, 0.2, 0.2, 1.);
		GeomDebugDump::stopBuffering();
		GeomDebugDump::moveOffset(0.3);
		GeomDebugDump::dumpMesh(mesh, color, true);
	}
#endif

	return numChanges;
}

size_t mergeCoplanarFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& paramsInput, bool shouldBeClosedManifold)
{
	shared_ptr<carve::mesh::MeshSet<3> > meshset_copy(meshset->clone());

	GeomProcessingParams params(paramsInput);
	params.allowFinEdges = false;
	double epsAngleMergeEdges = params.epsMergeAlignedEdgesAngle * 10;

	MeshSetInfo infoInput;
	bool validMeshsetInput = MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, params);
	if (infoInput.finEdges.size() > 0)
	{
		// if input already has fin edges, allow it also for the result mesh
		params.allowFinEdges = true;
	}

	size_t numChanges = 0;
	double volume = MeshOps::computeMeshsetVolume(meshset.get());

	for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
	{
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

			if (adjacentFace == face)
			{
				// can happen with opening
				continue;
			}

			// compute face normal here. Don't rely on face->plane.N, since the winding order might be changed
			const vec3 faceNormal = MeshOps::computeFaceNormal(face);
			const vec3 face2Normal = MeshOps::computeFaceNormal(adjacentFace);

			// adjacent faces have 1 as normal vector dot product

			double dotProduct = dot(faceNormal, face2Normal);
			if (std::abs(dotProduct - 1.0) < epsAngleMergeEdges)
			{
				numChanges += removeEdgeAndMergeFaces(edge, params);
			}
		}
	} 

	if (numChanges > 0)
	{
		if (!shouldBeClosedManifold)
		{
			meshset = meshset_copy;
			return numChanges;
		}
		MeshSetInfo infoResult;
		bool validMeshsetResult = MeshOps::checkMeshSetValidAndClosed(meshset, infoResult, params);

		if (!validMeshsetResult)
		{
			MeshOps::retriangulateMeshSetForBoolOp(meshset, false, params, 0);
			validMeshsetResult = MeshOps::checkMeshSetValidAndClosed(meshset, infoResult, params);
		}

		double volumeResult = MeshOps::computeMeshsetVolume(meshset.get());
		if (volumeResult < 0.9 * volume)
		{
			validMeshsetResult = false;
		}

		if (!validMeshsetResult)
		{
			meshset = meshset_copy;
			numChanges = 0;
			return 0;
		}

		int numFacesRemoved = infoInput.numFaces - infoResult.numFaces;
		int numClosedEdgesRemoved = infoInput.numClosedEdges - infoResult.numClosedEdges;
	}
	return numChanges;
}

void flipFacesOnOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, const GeomProcessingParams& params)
{
	MeshSetInfo infoInput;
	MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, params);

	size_t numEdgesAll = infoInput.numClosedEdges + infoInput.numOpenEdges;
	if (numEdgesAll > 10000)
	{
		return;
	}

	std::set<carve::mesh::Face<3>* > setFacesDone;

	for (size_t ii = 0; ii < meshset->meshes.size(); ++ii)
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];

		std::set<carve::mesh::Face<3>* > setFlipFaces;
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
					}
				}
			}
		}

		std::set<carve::mesh::Face<3>* > setSkipFaces;
		PolyInputCache3D polyInput(params.epsMergePoints);
		polyhedronFromMeshSet(meshset, setSkipFaces, setFlipFaces, polyInput);

		std::map<std::string, std::string> mesh_input_options;
		std::string details;
		bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
		if (!correct)
		{
			fixPolyhedronData(polyInput.m_poly_data, true, params);
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
		}

		return;
	}
}

void MeshOps::resolveOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, const GeomProcessingParams& params)
{
	if (!meshset)
	{
		return;
	}

	flipFacesOnOpenEdges(meshset, params);

	MeshSetInfo info;
	MeshOps::checkMeshSetValidAndClosed(meshset, info, params);

	if (info.meshSetValid)
	{
		return;
	}

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

	if (numOpenEdgesInput + numClosedEdgesInput > 10000)
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

	PolyInputCache3D polyInput(params.epsMergePoints);
	size_t maxNumEdges = 1000;

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

			double area = GeomUtils::computePolygonArea(vecFaceLoop);
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
		fixPolyhedronData(polyInput.m_poly_data, true, params);
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
			glm::vec4 color(0, 1, 1, 1);
			if (numOpenEdges > 0)
			{
				glm::vec4 color(1, 0.5, 1, 1);
				//GeomDebugDump::dumpMeshsetOpenEdges(meshsetNew, color, false, false);
			}
			bool drawNormals = true;
			GeomDebugDump::dumpMeshset(meshsetNew, color, drawNormals, true);
		}
#endif

		size_t numMeshes = meshsetNew->meshes.size();
		if (numMeshes <= numMeshesInput)
		{
			if (numClosedEdges >= numClosedEdgesInput)
			{
				if (numOpenEdges <= numOpenEdgesInput)
				{
					meshset = meshsetNew;
				}
			}
		}
	}
}

void getPointerToEdge(carve::mesh::Mesh<3>* mesh, carve::mesh::Edge<3>* edgeFind, std::set<carve::mesh::Edge<3>* >& setEdges, std::set<carve::mesh::Face<3>* >& setFaces)
{
	size_t numEdges = 0;
	for (carve::mesh::Face<3>*face : mesh->faces)
	{
		if (!face)
		{
			continue;
		}

		carve::mesh::Edge<3>* edge = face->edge;
		if (edge == edgeFind)
		{
			setFaces.insert(face);
		}

		size_t numEdgesCurrentFace = face->n_edges;
		for (size_t ii = 0; ii < numEdgesCurrentFace; ++ii)
		{
			if (!edge)
			{
				continue;
			}
			if (edge->next == edgeFind)
			{
				setEdges.insert(edge);
			}
			if (edge->prev == edgeFind)
			{
				setEdges.insert(edge);
			}
			if (edge->rev == edgeFind)
			{
				setEdges.insert(edge);
			}
			edge = edge->next;
		}
	}
}

void getEdgesOnVertex(carve::mesh::Mesh<3>* mesh, carve::mesh::Vertex<3>* vertex, std::set<carve::mesh::Edge<3>* >& setEdges)
{
	size_t numEdges = 0;
	for (carve::mesh::Face<3>*face : mesh->faces)
	{
		if (!face)
		{
			continue;
		}

		carve::mesh::Edge<3>* edge = face->edge;
		size_t numEdgesCurrentFace = face->n_edges;
		for (size_t ii = 0; ii < numEdgesCurrentFace; ++ii)
		{
			if (!edge)
			{
				continue;
			}
			if (edge->vert == vertex)
			{
				setEdges.insert(edge);
			}
			edge = edge->next;
		}
	}
}

size_t removePointerToVertex(carve::mesh::Mesh<3>* mesh, carve::mesh::Vertex<3>* vertRemove, carve::mesh::Vertex<3>* vertReplace)
{
	size_t numChanges = 0;
	for (carve::mesh::Face<3>*face : mesh->faces)
	{
		if (!face)
		{
			continue;
		}

		carve::mesh::Edge<3>* edge = face->edge;
		size_t numEdgesCurrentFace = face->n_edges;
		for (size_t ii = 0; ii < numEdgesCurrentFace; ++ii)
		{
			if (!edge)
			{
				continue;
			}
			if (edge->vert == vertRemove)
			{
				edge->vert = vertReplace;
				++numChanges;
			}
			edge = edge->next;
		}
	}
	return numChanges;
}

size_t mergeAlignedEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, GeomProcessingParams& params )
{
#ifdef _DEBUG
	if (params.debugDump)
	{
		GeomDebugDump::moveOffset(0.3);
	}
#endif

	size_t numEdgesRemoved = 0;
	for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
	{
		for (carve::mesh::Face<3>*face : mesh->faces)
		{
			if (!face)
			{
				continue;
			}

			if (!face->edge)
			{
				continue;
			}

			bool faceDumped = false;
			carve::mesh::Edge<3>* edge = face->edge;

			// check how many edges are connected to end of edge
			size_t numEdges = face->n_edges;
			for (size_t ii = 0; ii < numEdges; ++ii)
			{
				if (!edge)
				{
					continue;
				}
				if (edge->next)
				{
					if (edge->next->rev)
					{
						if (edge->next->rev->next)
						{
							if (edge->next->rev->next->rev == edge)
							{
								// only one edge is connected, now check angle

								//      edge->rev->next         edge->rev            edge->next->rev        edge->next->next->rev
								//  <--------------------p1<------------------p2<-------------------------p3<-----------------------
								//   ------------------->   ------------------>   ----------------------->   ---------------------->
								//     edge->prev               edge                  edge->next             edge->next->next

								carve::mesh::Vertex<3>* vertex1 = edge->v1();
								carve::mesh::Vertex<3>* vertex2 = edge->v2();
								carve::mesh::Vertex<3>* vertex3 = edge->next->v2();

								std::set<carve::mesh::Edge<3>* > setEdges;
								getEdgesOnVertex(mesh, vertex2, setEdges);
								size_t numEdgesOnVertex = setEdges.size();

								const carve::geom::vector<3>& p1 = vertex1->v;
								const carve::geom::vector<3>& p2 = vertex2->v;
								const carve::geom::vector<3>& p3 = vertex3->v;

								carve::geom::vector<3> edgeVector = p2 - p1;
								carve::geom::vector<3> edgeNextVector = p3 - p2;
								edgeVector.normalize();
								edgeNextVector.normalize();

#ifdef _DEBUG
								glm::vec4 color1(0.4, 0.45, 0.45, 1.);
								if (params.debugDump)
								{
									for (auto edgeOnVertex : setEdges)
									{
										const carve::geom::vector<3>& p1 = edgeOnVertex->v1()->v;
										const carve::geom::vector<3>& p2 = edgeOnVertex->v2()->v;
										std::vector<carve::geom::vector<3> > vecLine = { p1, p2 };
										GeomDebugDump::dumpPolyline(vecLine, color1, false);
									}
								}

								if (face->plane.N.x > 0.9)
								{
									int wait = 0;
								}
#endif

								double dotProduct = dot(edgeVector, edgeNextVector);
								if (std::abs(dotProduct - 1.0) < params.epsMergeAlignedEdgesAngle * 1000)
								{
									carve::mesh::Edge<3>* edgeRemove = edge;// ->next;

#ifdef _DEBUG
									if (params.debugDump)
									{
										std::vector<const carve::mesh::Face<3>* > vecAdjacentFaces = { edge->face, edge->rev->face, edgeRemove->face, edgeRemove->rev->face, edge->prev->face, edge->prev->rev->face };
										GeomDebugDump::moveOffset(0.05);
										GeomDebugDump::dumpFaces(vecAdjacentFaces, color1, false);
										GeomDebugDump::dumpFacePolygons(vecAdjacentFaces, color1, false);
									}
#endif

									std::set<carve::mesh::Edge<3>* > setEdgePointersToRemovedEdge;
									std::set<carve::mesh::Face<3>* > setFacePointersToRemovedEdge;
									getPointerToEdge(mesh, edgeRemove, setEdgePointersToRemovedEdge, setFacePointersToRemovedEdge);
									size_t numVertexChanges = removePointerToVertex(mesh, vertex2, vertex1);
									edge = edgeRemove->removeEdge();  // returns ->next
									carve::geom::vector<3> distanceV1 = edge->v1()->v - p1;
									carve::geom::vector<3> distanceV3 = edge->v2()->v - p3;

									//double epsMinFaceArea = params.minFaceArea;// *0.001;
									//MeshOps::removeZeroAreaFacesInMesh(mesh, epsMinFaceArea, eps, dumpFaces);

									++numEdgesRemoved;
									mesh->cacheEdges();
									mesh->recalc(params.epsMergePoints);

									//      edge->rev->next         edge->rev                                    edge->next->rev  
									//  <--------------------v1<---------------------------------------------v2<------------------------
									//   ------------------->   -------------------------------------------->   ---------------------->
									//     edge->prev               edge                                           edge->next    

#ifdef _DEBUG
									if (params.debugDump)
									{
										glm::vec4 color(0.4, 0.45, 0.45, 1.);

										std::set<carve::mesh::Edge<3>* > setEdges1;
										getEdgesOnVertex(mesh, vertex1, setEdges1);

										std::set<carve::mesh::Edge<3>* > setEdges2;
										getEdgesOnVertex(mesh, vertex2, setEdges2);

										std::set<carve::mesh::Edge<3>* > setEdges3;
										getEdgesOnVertex(mesh, vertex3, setEdges3);


										for (auto edgeOnVertex : setEdges1)
										{
											const carve::geom::vector<3>& p1 = edgeOnVertex->v1()->v;
											const carve::geom::vector<3>& p2 = edgeOnVertex->v2()->v;
											std::vector<carve::geom::vector<3> > vecLine = { p1, p2 };
											GeomDebugDump::dumpPolyline(vecLine, color1, false);
										}


										for (auto edgeOnVertex : setEdges2)
										{
											const carve::geom::vector<3>& p1 = edgeOnVertex->v1()->v;
											const carve::geom::vector<3>& p2 = edgeOnVertex->v2()->v;
											std::vector<carve::geom::vector<3> > vecLine = { p1, p2 };
											GeomDebugDump::dumpPolyline(vecLine, color1, false);
										}

										for (auto edgeOnVertex : setEdges3)
										{
											const carve::geom::vector<3>& p1 = edgeOnVertex->v1()->v;
											const carve::geom::vector<3>& p2 = edgeOnVertex->v2()->v;
											std::vector<carve::geom::vector<3> > vecLine = { p1, p2 };
											GeomDebugDump::dumpPolyline(vecLine, color1, false);
										}

										GeomDebugDump::moveOffset(0.4);
										GeomDebugDump::dumpFacePolygons(mesh->faces, color1, false);

										GeomDebugDump::moveOffset(0.05);
										std::vector<carve::geom::vector<3> > edgePolygon = { p1,p2,p3 };
										GeomDebugDump::dumpPolyline(edgePolygon, color, false);

										std::vector<carve::geom::vector<3> > edgePolygon2 = { edge->v1()->v, edge->v2()->v };
										GeomDebugDump::moveOffset(0.001);
										GeomDebugDump::dumpPolyline(edgePolygon2, color, false);
									}

									MeshSetInfo infoMergedFaces;
									bool validMeshsetMergedFaces = MeshOps::checkMeshSetValidAndClosed(meshset, infoMergedFaces, params);

									if (params.debugDump)
									{
										GeomDebugDump::moveOffset(0.3);
										GeomDebugDump::DumpSettingsStruct dumpColorSettings;
										params.checkZeroAreaFaces = true;
										GeomDebugDump::dumpWithLabel("mesh-merged-faces", meshset, dumpColorSettings, params, true, true);
									}

									double dx = edgeVector.x - edgeNextVector.x;
									double dy = edgeVector.y - edgeNextVector.y;
									double dz = edgeVector.z - edgeNextVector.z;
									if (std::abs(dx) > EPS_M8)
									{
										std::cout << "align check" << std::endl;
									}
									if (std::abs(dy) > EPS_M8)
									{
										std::cout << "align check" << std::endl;
									}
									if (std::abs(dz) > EPS_M8)
									{
										std::cout << "align check" << std::endl;
									}
#endif

									continue;
								}
							}
						}
					}
					edge = edge->next;
				}
			}
#ifdef _DEBUG
			if (params.debugDump)
			{
				GeomDebugDump::moveOffset(0.002);
				glm::vec4 color(0.4, 0.45, 0.45, 1.);
				GeomDebugDump::dumpFacePolygon(face, color, false);
			}
#endif

		}
	}

	if (numEdgesRemoved > 0)
	{
#ifdef _DEBUG
		if (params.debugDump)
		{
			GeomDebugDump::moveOffset(0.2);
		}
#endif

		for (auto mesh : meshset->meshes)
		{
			mesh->cacheEdges();
			mesh->recalc(params.epsMergePoints);

#ifdef _DEBUG
			if (params.debugDump)
			{
				for (carve::mesh::Face<3>*face : mesh->faces)
				{
					glm::vec4 color(0.4, 0.45, 0.45, 1.);
					GeomDebugDump::dumpFacePolygon(face, color, false);
				}
			}
#endif
		}
	}

	return numEdgesRemoved;
}



	/// \brief simplifyMeshSet merge coplanar faces and re-triangulate each set of merged faces
	/// \param meshset				Carve meshset
	/// \param report_callback		callback function for errors, warnings, notifications, progress
	/// \param entity				IFC entity that is currently being processed
	/// \param ignoreOpenEdgesInResult	If true, the result is kept even with open edges (good for visualization). If false, the result will be the input mesh in case open edges occur after triangulation (good for further boolean operations)
void MeshOps::simplifyMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& paramsInput, bool triangulateResult, bool shouldBeClosedManifold)
{
	if (!meshsetInput)
	{
		return;
	}

	GeomProcessingParams params(paramsInput);
	StatusCallback* report_callback = params.callbackFunc;
	BuildingEntity* entity = params.ifc_entity;
	bool dumpPolygon = params.debugDump;
	double eps = params.epsMergePoints;
	MeshSetInfo infoInput(report_callback, entity);
	bool validMeshsetInput = MeshOps::checkMeshSetValidAndClosed(meshsetInput, infoInput, params);

	if (meshsetInput->vertex_storage.size() < 9 && infoInput.numOpenEdges == 0)
	{
		return;
	}

	if (infoInput.finEdges.size() > 0)
	{
		// if input mesh has fin edges, allow that also for the result
		params.allowFinEdges = true;
	}

	MeshSetInfo info(report_callback, entity);
	shared_ptr<carve::mesh::MeshSet<3> > meshset(meshsetInput->clone());

#ifdef _DEBUG
	GeomDebugDump::DumpSettingsStruct dumpColorSettings;

	if (dumpPolygon)
	{
		GeomProcessingParams par(params);
		par.checkZeroAreaFaces = false;
		GeomDebugDump::dumpWithLabel("simplify--input", meshset, dumpColorSettings, par, true, true);
	}

	if (dumpPolygon)
	{
		GeomDebugDump::moveOffset(0.2);

		glm::vec4 color1(0.7, 0.7, 0.7, 0.88);
		std::string labelStr = "simplify--merged-faces";
		GeomDebugDump::dumpVertex(GeomDebugDump::DumpData::instance().labelPos, color1, labelStr);
		GeomDebugDump::dumpCountLabel(GeomDebugDump::DumpData::instance().countLabelPos);
	}

	if (GeomDebugDump::DumpData::instance().dumpCount >= 16)
	{
		int wait = 0;
	}
	shared_ptr<carve::mesh::MeshSet<3> > meshset_copy_input(meshset->clone());
#endif

	if (infoInput.zeroAreaFaces.size() > 0)
	{
		if (infoInput.numOpenEdges == 0)
		{
			if (infoInput.allPointersValid)
			{
				removeDegenerateFacesInMeshSet(meshset, params, false);
				MeshOps::checkMeshSetValidAndClosed(meshset, info, params);
				assignIfBetterForBoolOp( meshset, meshsetInput, info, infoInput, true);
				//meshset = meshsetInput;
			}
		}
	}

	bool validMeshset = MeshOps::checkMeshSetValidAndClosed(meshset, info, params);
	if (!validMeshset)
	{
		resolveOpenEdges(meshset, params);
		validMeshset = MeshOps::checkMeshSetValidAndClosed(meshset, info, params);
		assignIfBetterForBoolOp( meshset, meshsetInput, info, infoInput, false);
		//meshset = meshsetInput;

		if (!validMeshset)
		{
			resolveOpenEdges(meshset, params);
			validMeshset = MeshOps::checkMeshSetValidAndClosed(meshset, info, params);
			assignIfBetterForBoolOp(meshset, meshsetInput, info, infoInput, false);
			//meshset = meshsetInput;
		}
	}

	dumpPolygon = false;

	try
	{
		removeFinEdges(meshset, params);
		removeFinFaces(meshset, params);
		removeDegenerateFacesInMeshSet(meshset, params, false);
		bool mergedFacesMeshShouldBeClosed = false;  // allow open edges, will try to sew them together later
		size_t numChanges = mergeCoplanarFacesInMeshSet(meshset, params, mergedFacesMeshShouldBeClosed);

		size_t numEdgesRemoved = mergeAlignedEdges(meshset, params);

		MeshOps::recalcMeshSet(meshset, eps);

		// TODO: find faces with biggest area, and trim all points to plane

		MeshSetInfo infoMergedFaces(report_callback, entity);
		bool validMeshsetMergedFaces = MeshOps::checkMeshSetValidAndClosed(meshset, infoMergedFaces, params);

		assignIfBetterForBoolOp(meshset, meshsetInput, infoMergedFaces, infoInput, false);
		//meshset = meshsetInput;

#ifdef _DEBUG
		shared_ptr<carve::mesh::MeshSet<3> > meshset_merged_faces(meshset->clone());
		GeomProcessingParams paramCopy(params);
		paramCopy.checkZeroAreaFaces = false;

		if (dumpPolygon)//|| numChanges > 0 )
		{
			GeomDebugDump::moveOffset(0.3);
			shared_ptr<carve::mesh::MeshSet<3> > meshset_dump = shared_ptr<carve::mesh::MeshSet<3> >(meshset->clone());
			dumpColorSettings.triangulateBeforeDump = false;

			dumpWithLabel("mesh-merged-faces", meshset_dump, dumpColorSettings, paramCopy, true, true);
			dumpColorSettings.triangulateBeforeDump = true;

			GeomDebugDump::moveOffset(0.3);
			glm::vec4 color1(0.7, 0.7, 0.7, 0.88);
			for (carve::mesh::Mesh<3>*mesh : meshset_dump->meshes)
			{
				GeomDebugDump::dumpFacePolygons(mesh->faces, color1, false);
			}
		}

#endif
		validMeshsetMergedFaces = MeshOps::checkMeshSetValidAndClosed(meshset, infoMergedFaces, params);
		if (!validMeshsetMergedFaces)
		{
			size_t retry_count = 0;
			MeshOps::retriangulateMeshSetForBoolOp(meshset, false, params, retry_count);

			MeshSetInfo infoRetriangulated(report_callback, entity);
			validMeshsetMergedFaces = MeshOps::checkMeshSetValidAndClosed(meshset, infoRetriangulated, params);
		}

		assignIfBetterForBoolOp( meshset, meshsetInput, infoMergedFaces, infoInput, false);
		//meshset = meshsetInput;

		MeshOps::recalcMeshSet(meshset, eps);
		validMeshsetMergedFaces = MeshOps::checkMeshSetValidAndClosed(meshset, info, params);

		size_t numEdgesRemoved2 = mergeAlignedEdges(meshset, params);
		if (numEdgesRemoved2 > 0)
		{
			MeshSetInfo infoMergedAlignedEdges(report_callback, entity);
			bool validMergedAlignedEdges = MeshOps::checkMeshSetValidAndClosed(meshset, infoMergedAlignedEdges, params);

#ifdef _DEBUG
			if (dumpPolygon)
			{
				GeomDebugDump::moveOffset(0.4);
				dumpWithLabel("mesh-simplify-input", meshset_copy_input, dumpColorSettings, paramCopy, true, true);

				GeomDebugDump::moveOffset(0.2);
				dumpWithLabel("mesh-merged-faces", meshset_merged_faces, dumpColorSettings, paramCopy, true, true);

				GeomDebugDump::moveOffset(0.2);
				dumpWithLabel("mesh-merged-aligned-edges", meshset, dumpColorSettings, paramCopy, true, true);

				GeomDebugDump::moveOffset(0.1);
				glm::vec4 color1(0.7, 0.7, 0.7, 0.88);
				for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
				{
					GeomDebugDump::dumpFacePolygons(mesh->faces, color1, false);
				}

				GeomDebugDump::moveOffset(0.3);
			}
#endif
			assignIfBetterForBoolOp(meshset, meshsetInput, infoMergedAlignedEdges, infoInput, false);

			if (validMergedAlignedEdges)
			{
				if (triangulateResult)
				{
					retriangulateMeshSetForBoolOp(meshset, false, params, 0);
					MeshSetInfo infoTriangulated(report_callback, entity);
					bool validTriangulated = MeshOps::checkMeshSetValidAndClosed(meshset, infoTriangulated, params);

					assignIfBetterForBoolOp(meshset, meshsetInput, infoTriangulated, infoInput, true);
				}
#ifdef _DEBUG
				GeomDebugDump::clearBuffer();
#endif
				return;
			}
		}

#ifdef _DEBUG
		GeomDebugDump::clearBuffer();
#endif

		assignIfBetterForBoolOp(meshset, meshsetInput, infoMergedFaces, infoInput, true);

#ifdef _DEBUG
		MeshSetInfo infoResult(report_callback, entity);
		bool validResult = MeshOps::checkMeshSetValidAndClosed(meshsetInput, infoResult, params);
		if (!validResult)
		{
			if (validMeshsetInput)
			{
				std::cout << "validMeshsetInput && !validResult: should not happen" << std::endl;
			}
		}

#endif

		return;
	}
	catch (std::exception& ex)
	{
#ifdef _DEBUG
		std::cout << ex.what() << std::endl;
#endif
	}
	catch (carve::exception& ex)
	{
		std::cout << ex.str() << std::endl;
	}
	catch (...)
	{

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

double MeshOps::computeShapeSurfaceArea(const shared_ptr<ItemShapeData>& item_data)
{
	double surface_area = 0;
	double volume = 0;
	if (item_data)
	{
		for (size_t jj = 0; jj < item_data->m_meshsets.size(); ++jj)
		{
			surface_area += computeMeshSetSurface(item_data->m_meshsets[jj]);
		}

		for (size_t jj = 0; jj < item_data->m_meshsets_open.size(); ++jj)
		{
			surface_area += computeMeshSetSurface(item_data->m_meshsets_open[jj]);
		}

		for (size_t ii = 0; ii < item_data->m_child_items.size(); ++ii)
		{
			const shared_ptr<ItemShapeData>& child_item_data = item_data->m_child_items[ii];
			double childArea = computeShapeSurfaceArea(child_item_data);
			surface_area += childArea;
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
		for (size_t ii = 0; ii < shape_input_data->m_geometric_items.size(); ++ii)
		{
			const shared_ptr<ItemShapeData>& item_data = shape_input_data->m_geometric_items[ii];
			double childArea = computeShapeSurfaceArea(item_data);
			surface_area += childArea;
		}

		for (size_t ii = 0; ii < shape_input_data->m_vec_children.size(); ++ii)
		{
			const shared_ptr<ProductShapeData>& item_data = shape_input_data->m_vec_children[ii];
			double childArea = computeShapeSurfaceArea(item_data);
			surface_area += childArea;
		}
	}
	return surface_area;
}

double MeshOps::computeMeshsetVolume(const carve::mesh::MeshSet<3>* meshset)
{
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

double triangleArea(const vec3& A, const vec3& B, const vec3& C)
{
	vec3 AB(B - A);
	vec3 AC(C - A);
	vec3 crossProduct = cross(AB, AC);
	return crossProduct.length() * 0.5;
}

vec3 MeshOps::computeFaceNormal(carve::mesh::Face<3>* face)
{
	vec3 polygon_normal(carve::geom::VECTOR(0, 0, 0));

	if (!face) { return polygon_normal; }
	carve::mesh::Edge<3>* edge = face->edge;

	//face->recalc()

	const size_t n_edges = face->n_edges;

	if (n_edges == 3)
	{
		const vec3& A = edge->vert->v;
		edge = edge->next;
		const vec3& B = edge->vert->v;
		edge = edge->next;
		const vec3& C = edge->vert->v;
		vec3 AB(B - A);
		vec3 AC(C - A);
		vec3 crossProduct = cross(AB, AC);
		crossProduct.normalize();
		return crossProduct;
	}

	if (n_edges < 2)
	{
		return polygon_normal;
	}

	// find triangle with largest area
	carve::mesh::Edge<3>* longestEdge = nullptr;
	double longestEdgeLength = 0;

	double largestArea = 0;
	for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
	{
		if (!edge)
		{
			continue;
		}

		double length2 = edge->length2();
		if (length2 > longestEdgeLength)
		{
			longestEdge = edge;
			longestEdgeLength = length2;
		}
		edge = edge->next;
	}

	if (longestEdge != nullptr)
	{
		carve::mesh::Edge<3>* edge1 = longestEdge->next;
		const vec3& A = longestEdge->v1()->v;  // vert
		const vec3& B = longestEdge->v2()->v;  // next->vert
		const vec3 AB(B - A);

		for (size_t i_edge = 0; i_edge < n_edges - 1; ++i_edge)
		{
			const vec3& C = edge1->v2()->v;
			const vec3 AC(C - A);
			vec3 crossProduct = cross(AB, AC);
			double area = crossProduct.length() * 0.5;

			//double area = triangleArea(A, B, C);
			if (std::abs(area) > largestArea)
			{
				largestArea = area;

				vec3 AB(B - A);
				vec3 AC(C - A);
				vec3 crossProduct = cross(AB, AC);
				crossProduct.normalize();
				polygon_normal = crossProduct;
			}

			edge1 = edge1->next;
		}
	}

#ifdef _DEBUG
	if (edge != face->edge)
	{
		std::cout << "edge != face->edge" << std::endl;
	}
#endif

	return polygon_normal;
}

size_t MeshOps::countFaces(carve::mesh::MeshSet<3>* mesh)
{
	size_t numFaces = 0;
	for (auto& mesh : mesh->meshes)
	{
		numFaces += mesh->faces.size();
	}
	return numFaces;
}

size_t MeshOps::countDegeneratedFaces(carve::mesh::MeshSet<3>* mesh)
{
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
	return meshes_closed;
}

void MeshOps::checkMeshSetPointers(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool checkForDegenerateEdges, const GeomProcessingParams& params, MeshSetInfo& info)
{
	for (size_t i = 0; i < meshset->meshes.size(); ++i)
	{
		carve::mesh::Mesh<3>* mesh_i = meshset->meshes[i];
		MeshOps::checkMeshPointers(mesh_i, checkForDegenerateEdges, params, info);
		if (!info.allPointersValid)
		{
			return;
		}
	}
}

bool MeshOps::checkMeshSetValidAndClosed(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, const GeomProcessingParams& params)
{
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
		return false;
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
		info.numOpenEdges += mesh->open_edges.size();
		info.numClosedEdges += mesh->closed_edges.size();
		info.numFaces += mesh->faces.size();

		for (size_t jj = 0; jj < mesh->faces.size(); ++jj)
		{
			carve::mesh::Face<3>* inputFace = mesh->faces[jj];
			if (inputFace == nullptr)
			{
				info.allPointersValid = false;
				continue;
			}
			if (inputFace->n_edges > info.maxNumberOfEdgesPerFace)
			{
				info.maxNumberOfEdgesPerFace = inputFace->n_edges;
			}
			double longestEdge = 0;
			double face_area = MeshOps::computeFaceArea(inputFace, longestEdge);
			if (std::abs(face_area) < params.minFaceArea  )
			{
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
	checkMeshSetPointers(meshset, checkForDegenerateEdges, params, info);
	if (!info.allPointersValid)
	{
		info.meshSetValid = false;
		return false;
	}

	if (!params.allowDegenerateEdges)
	{
		if (info.degenerateEdges.size() > 0)
		{
			return false;
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
					glm::vec4 color(0.3, 0.3, 0.3, 1.);
					GeomDebugDump::dumpFacePolygon(face, color, false);
#endif
					continue;
				}

				carve::mesh::Edge<3>* e = face->edge;
				for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
				{
					// check if is a flat part
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
			return false;
		}
	}

	if (info.numOpenEdges > 0)
	{
		info.meshSetValid = false;
		return false;
	}

	if (info.zeroAreaFaces.size() > 0)
	{
		if (!params.allowZeroAreaFaces)
		{
			info.meshSetValid = false;
			return false;
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

void MeshOps::flattenFacePlanes(shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const GeomProcessingParams& params)
{
	// project face points into coplanar face
	double epsAngle = params.epsMergeAlignedEdgesAngle * 100.0;
	double epsDistanceSinglePoints = params.epsMergePoints * 10.0;
	double epsDistanceFaceCentroids = params.epsMergePoints * 10.0;
	double epsMinDistanceMovePoints2 = params.epsMergePoints * 0.01 * params.epsMergePoints * 0.01;

	// TODO: integrate this into other function. First merge all coplanar faces, then do this:

	for (auto mesh1 : op1->meshes)
	{
		if (mesh1->faces.size() > 1000)
		{
			continue;
		}

		for (auto face1 : mesh1->faces)
		{
			vec3 face1Centroid = face1->centroid();
			const vec3 face1Normal = face1->plane.N;
			bool face1dumped = false;

			for (auto mesh2 : op2->meshes)
			{
				if (mesh2->faces.size() > 1000)
				{
					continue;
				}
				for (auto face2 : mesh2->faces)
				{
					const vec3 face2Normal = face2->plane.N;

					double dotProduct = dot(face1Normal, face2Normal);
					if (std::abs(dotProduct + 1.0) > epsAngle && std::abs(dotProduct - 1.0) > epsAngle)
					{
						continue;
					}

					// check if faces overlap
					carve::geom::aabb<3> bbox1 = face1->getAABB();
					carve::geom::aabb<3> bbox2 = face2->getAABB();
					if (bbox1.extent.x < epsDistanceSinglePoints) { bbox1.extent.x = epsDistanceSinglePoints * 2; bbox1.pos.x -= epsDistanceSinglePoints; }
					if (bbox1.extent.y < epsDistanceSinglePoints) { bbox1.extent.y = epsDistanceSinglePoints * 2; bbox1.pos.y -= epsDistanceSinglePoints; }
					if (bbox1.extent.z < epsDistanceSinglePoints) { bbox1.extent.z = epsDistanceSinglePoints * 2; bbox1.pos.z -= epsDistanceSinglePoints; }

					if (bbox2.extent.x < epsDistanceSinglePoints) { bbox2.extent.x = epsDistanceSinglePoints * 2; bbox2.pos.x -= epsDistanceSinglePoints; }
					if (bbox2.extent.y < epsDistanceSinglePoints) { bbox2.extent.y = epsDistanceSinglePoints * 2; bbox2.pos.y -= epsDistanceSinglePoints; }
					if (bbox2.extent.z < epsDistanceSinglePoints) { bbox2.extent.z = epsDistanceSinglePoints * 2; bbox2.pos.z -= epsDistanceSinglePoints; }

					bbox1.extent.x *= 2;
					bbox1.extent.y *= 2;
					bbox1.extent.z *= 2;

					bbox2.extent.x *= 2;
					bbox2.extent.y *= 2;
					bbox2.extent.z *= 2;

					if (!bbox1.intersects(bbox2))
					{
						continue;
					}

					// faces are parallel 
					vec3 face2Centroid = face2->centroid();
					double distance = GeomUtils::distancePointPlane(face2Centroid, face1Normal, face1Centroid);
					if (std::abs(distance) < epsDistanceFaceCentroids)
					{
#ifdef _DEBUG
						if (params.debugDump)
						{
							if (!face1dumped)
							{
								glm::vec4 color(0, 0.4, 0.3, 1);
								GeomDebugDump::dumpFacePolygon(face1, color, false);
								face1dumped = true;
							}

							glm::vec4 color(0, 1, 1, 1);
							GeomDebugDump::dumpFacePolygon(face2, color, false);
						}

						carve::geom::aabb<3> bbox1test;
						carve::geom::aabb<3> bbox2test;
						bbox1test.pos = carve::geom::VECTOR(0, 0, 0);
						bbox1test.extent = carve::geom::VECTOR(1, 1, 1);

						bbox2test.pos = carve::geom::VECTOR(2 + params.epsMergePoints, 2 + params.epsMergePoints, 2 + params.epsMergePoints);
						bbox2test.extent = carve::geom::VECTOR(1, 1, 1);
						bool intersects = bbox1test.intersects(bbox2test);
#endif

						// faces are in plane. Now check if all vertices have a similar distance
						bool allPointsInPlane = true;
						auto edge2 = face2->edge;
						for (size_t jjEdge = 0; jjEdge < face2->n_edges; ++jjEdge)
						{
							carve::mesh::Vertex<3>* vert = edge2->vert;

							double distance = GeomUtils::distancePointPlane(vert->v, face1Normal, face1Centroid);

							edge2 = edge2->next;

							if (std::abs(distance) > epsDistanceSinglePoints)
							{

								allPointsInPlane = false;
								break;
							}
							//mapDistances.insert({ distance, { face1, vert } });
						}


						if (allPointsInPlane)
						{
							//intersectRayTriangle()
							double area1 = MeshOps::computeFaceArea(face1);
							double area2 = MeshOps::computeFaceArea(face2);

							carve::mesh::Face<3>* smallerFace = face1;
							carve::mesh::Face<3>* biggerFace = face2;
							if (area1 > area2)
							{
								smallerFace = face2;
								biggerFace = face1;
							}

							std::vector<carve::mesh::Vertex<3>*> vertices;
							smallerFace->getVertices(vertices);
							for (auto vert : vertices)
							{
								vec3 v;
								double t;
								const carve::geom3d::Plane& plane = biggerFace->plane;
								const vec3& rayPoint1 = vert->v;
								vec3 rayPoint2 = rayPoint1 + plane.N;
								//                                        const Plane& p, const Vector& v1, const Vector& v2, Vector& v, double& t, double CARVE_EPSILON)
								carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection(plane, rayPoint1, rayPoint2, v, t, params.epsMergePoints);
								if (intersect > 0)
								{
									double dx = vert->v.x - v.x;
									double dy = vert->v.y - v.y;
									double dz = vert->v.z - v.z;

									double distance2 = dx * dx + dy * dy + dz * dz;
									//if (distance2 > epsMinDistanceMovePoints2 && distance2 < epsDistanceSinglePoints * epsDistanceSinglePoints)
									//{
									//	vert->v = v;
									//}

									if (distance2 > 0.0 && distance2 < epsDistanceSinglePoints * epsDistanceSinglePoints)
									{
										vert->v = v;
									}
								}
							}
						}
					}
				}
			}

#ifdef _DEBUG
			carve::geom::aabb<3> bbox1 = op1->getAABB();
			carve::geom::aabb<3> bbox2 = op2->getAABB();
			bbox1.unionAABB(bbox2);
			//GeomDebugDump::moveOffset(bbox1.extent.y + 0.2);
#endif
		}
	}
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
	glm::vec4 color(0.5, 0.6, 0.7, 1.0);
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
					glm::vec4 color(0.3, 0.3, 0.3, 1.);
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
							glm::vec4 color(0.3, 0.3, 0.3, 1.);
							if (itIntersections == mapIntersections.begin())
							{
								GeomDebugDump::dumpFacePolygon(face, color, false);
							}
							GeomDebugDump::dumpPolyline(faceLoop, color, false);
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
				FaceConverter::createTriangulated3DFace(faceLoops, polyInput, paramsTriangulate);
			}
		}

		if (numSplitEdges > 0)
		{
			std::string details = "";
			bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
			if (!correct)
			{
				bool correct2 = fixPolyhedronData(polyInput.m_poly_data, true, params);
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
			checkMeshSetValidAndClosed(meshsetNew, infoNew, params);

			MeshSetInfo infoInput;
			checkMeshSetValidAndClosed(meshsetInput, infoInput, params);

			assignIfBetterForBoolOp(meshsetNew, meshsetInput, infoNew, infoInput, false);

#ifdef _DEBUG
			if (params.debugDump)
			{
				glm::vec4 color(0.3, 0.3, 0.3, 1.);
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
	double CARVE_EPSILON = params.epsMergePoints;

#ifdef _DEBUG
	glm::vec4 color(0.5, 0.6, 0.7, 1.0);
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
					glm::vec4 color(0.3, 0.3, 0.3, 1.);
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
				FaceConverter::createTriangulated3DFace(faceLoops, polyInput, params);
			}
		}

		if (numSplitEdges > 0)
		{
			std::string details = "";
			bool correct = checkPolyhedronData(polyInput.m_poly_data, params, details);
			if (!correct)
			{
				bool correct2 = fixPolyhedronData(polyInput.m_poly_data, true, params);
#ifdef _DEBUG
				if (!correct2)
				{
					std::cout << "fixPolyhedronData  failed" << std::endl;
				}
#endif
				return;
			}

			shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts(), CARVE_EPSILON));
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
					glm::vec4 color(0.3, 0.3, 0.3, 1.);
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

void MeshOps::boundingBox2Mesh(carve::geom::aabb<3>& bbox, shared_ptr<carve::mesh::MeshSet<3> >& meshset, double CARVE_EPSILON)
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

	meshset = shared_ptr<carve::mesh::MeshSet<3> >(polyhedron_data->createMesh(carve::input::opts(), CARVE_EPSILON));
}

std::shared_ptr<carve::mesh::MeshSet<3> > MeshOps::createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, double CARVE_EPSILON)
{
	carve::input::PolyhedronData polyhedron_data;
	polyhedron_data.addVertex(p0);
	polyhedron_data.addVertex(p1);
	polyhedron_data.addVertex(p2);
	polyhedron_data.addFace(0, 1, 2);
	std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), CARVE_EPSILON));
	return mesh;
}

std::shared_ptr<carve::mesh::MeshSet<3> > MeshOps::createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, vec3& p3, double CARVE_EPSILON)
{
	carve::input::PolyhedronData polyhedron_data;
	polyhedron_data.addVertex(p0);
	polyhedron_data.addVertex(p1);
	polyhedron_data.addVertex(p2);
	polyhedron_data.addVertex(p3);

	polyhedron_data.addFace(0, 1, 2, 3);
	//polyhedron_data.addFace(2, 3, 0);
	std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), CARVE_EPSILON));
	return mesh;
}

std::shared_ptr<carve::mesh::MeshSet<3> > MeshOps::createBoxMesh(vec3& pos, vec3& extent, carve::math::Matrix& transform, double CARVE_EPSILON)
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

	std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), CARVE_EPSILON));
	return mesh;
}

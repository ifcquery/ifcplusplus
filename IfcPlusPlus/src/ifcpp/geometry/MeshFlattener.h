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
#pragma warning( disable: 4251 4702 )

#include <ifcpp/model/BasicTypes.h>
#include "GeomUtils.h"
#include <ifcpp/model/StatusCallback.h>
#include "GeometryInputData.h"
#include "MeshOps.h"

class MeshFlattener
{
public:
	size_t m_numCorrectedVertices = 0;
	struct SetOfFacesInPlane {
		carve::geom::plane<3> plane;
		std::set<carve::mesh::Face<3>* > setFaces;
	};
	std::multimap<double, std::vector<shared_ptr<SetOfFacesInPlane> > > m_mapInPlaneFaces;

	shared_ptr<SetOfFacesInPlane> findPlaneForDistance(const std::vector<shared_ptr<SetOfFacesInPlane> >& vecOfPlanes, const vec3& normal, double epsAngle)
	{
		for (const shared_ptr<SetOfFacesInPlane>& setOfFacesInPlane : vecOfPlanes)
		{
			vec3& setOfFacesNormal = setOfFacesInPlane->plane.N;
			double dotProduct = dot(setOfFacesNormal, normal);
			if (std::abs(dotProduct + 1.0) > epsAngle && std::abs(dotProduct - 1.0) > epsAngle)
			{
				continue;
			}
			return setOfFacesInPlane;
		}
		return shared_ptr<SetOfFacesInPlane>();
	}

	shared_ptr<SetOfFacesInPlane> findPlaneForDistance(double d, const vec3& normal, double epsMergePoints, double epsPlaneAngle)
	{
		auto it = m_mapInPlaneFaces.lower_bound(d);

		auto low = m_mapInPlaneFaces.lower_bound(d);
		if (low == m_mapInPlaneFaces.end())
		{
			if (m_mapInPlaneFaces.size() > 0)
			{
				double lastElement = m_mapInPlaneFaces.rbegin()->first;
				double delta = lastElement - d;
				if (std::abs(delta) <= epsMergePoints)
				{
					std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = m_mapInPlaneFaces.rbegin()->second;
					return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
				}
			}

			return shared_ptr<SetOfFacesInPlane>();
			//size_t vertex_index = m_poly_data->addVertex(pt);
			//m_mapInPlaneFaces.insert({ { vertex_z, vertex_index } });
			//return vertex_index;
		}
		else if (low == m_mapInPlaneFaces.begin())
		{
			double existingD = low->first;
			double delta = existingD - d;
			if (std::abs(delta) <= epsMergePoints)
			{
				if (low != m_mapInPlaneFaces.end())
				{
					// check if next element is closer
					auto itNext = low;
					++itNext;
					if (itNext != m_mapInPlaneFaces.end())
					{
						double nextD = itNext->first;
						double deltaNext = nextD - d;
						if (std::abs(deltaNext) <= std::abs(delta))
						{
							std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = itNext->second;
							return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
						}
					}
				}
				std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = low->second;
				return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
			}
			else
			{
				return shared_ptr<SetOfFacesInPlane>();
				//size_t vertex_index = m_poly_data->addVertex(pt);
				//m_mapInPlaneFaces.insert({ { vertex_z, vertex_index } });
				//return vertex_index;
			}
		}
		else
		{
			auto prev = std::prev(low);
			double dzPrev = d - prev->first;
			double dLow = low->first - d;
			if (std::abs(dzPrev) < std::abs(dLow))
			{
				if (std::abs(dzPrev) <= epsMergePoints)
				{
					std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = prev->second;
					return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
				}
				else
				{
					return shared_ptr<SetOfFacesInPlane>();
					//size_t vertex_index = m_poly_data->addVertex(pt);
					//m_mapInPlaneFaces.insert({ { vertex_z, vertex_index } });
					//return vertex_index;
				}
			}
			else
			{
				if (std::abs(dLow) <= epsMergePoints)
				{
					std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = low->second;
					return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
				}
				else
				{
					return shared_ptr<SetOfFacesInPlane>();
					//size_t vertex_index = m_poly_data->addVertex(pt);
					//m_mapInPlaneFaces.insert({ { vertex_z, vertex_index } });
					//return vertex_index;
				}
			}
		}

		return shared_ptr<SetOfFacesInPlane>();
	}

	void addMeshToPlaneCache(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params)
	{
		if (!meshset)
		{
			return;
		}

		double epsAngle = params.epsMergeAlignedEdgesAngle * 100.0;
		double epsDistanceSinglePoints = params.epsMergePoints * 10.0;
		double epsDistanceFaceCentroids = params.epsMergePoints * 10.0;

		for (auto mesh1 : meshset->meshes)
		{
			if (mesh1->faces.size() > 1000)
			{
				continue;
			}

			for (auto face1 : mesh1->faces)
			{
				carve::geom::plane<3>& facePlane = face1->plane;
				const vec3 face1Normal = facePlane.N;

				shared_ptr<SetOfFacesInPlane> existingPlane = findPlaneForDistance(facePlane.d, face1Normal, epsDistanceSinglePoints, epsAngle);
				if (!existingPlane)
				{
					existingPlane = make_shared<SetOfFacesInPlane>();
					existingPlane->setFaces.insert(face1);
					existingPlane->plane = facePlane;
					m_mapInPlaneFaces.insert({ facePlane.d, {existingPlane} });
					continue;
				}

#ifdef _DEBUG
				const vec3 setOfFacesPlaneNormal = existingPlane->plane.N;
				double dotProduct = dot(face1Normal, setOfFacesPlaneNormal);
				if (std::abs(dotProduct + 1.0) > epsAngle * 1.1 && std::abs(dotProduct - 1.0) > epsAngle * 1.1)
				{
					std::cerr << "check angle in plane failed" << std::endl;
				}

				double deltaD = facePlane.d - existingPlane->plane.d;
				if (std::abs(deltaD) > epsDistanceFaceCentroids * 1.1)
				{
					std::cerr << "check distance in plane failed" << std::endl;
				}
#endif

				existingPlane->plane.d = (existingPlane->plane.d + facePlane.d) * 0.5;
				existingPlane->plane.N = (existingPlane->plane.N + facePlane.N) * 0.5;  // could be also weighted by face area
				existingPlane->setFaces.insert(face1);

			}
		}
	}

	void flattenFacePlanes(shared_ptr<carve::mesh::MeshSet<3> >& meshset1, shared_ptr<carve::mesh::MeshSet<3> >& meshset2, const GeomProcessingParams& params)
	{
		if (!meshset1)
		{
			return;
		}


		// collect all faces in one plane with high epsilon

		addMeshToPlaneCache(meshset1, params);
		addMeshToPlaneCache(meshset2, params);

		// then re-compute the set of faces plane normals. Then check again which faces are in the plane
		compute(params);

	}

	void compute( const GeomProcessingParams& params)
	{
		// project face points into coplanar face
		double epsAngle = params.epsMergeAlignedEdgesAngle * 100.0;
		double epsDistanceSinglePoints = params.epsMergePoints * 10.0;
		double epsDistanceFaceCentroids = params.epsMergePoints * 10.0;
		double epsMinDistanceMovePoints2 = params.epsMergePoints * 0.01 * params.epsMergePoints * 0.01;

		for (auto it : m_mapInPlaneFaces)
		{
			std::vector<shared_ptr<SetOfFacesInPlane> >& vecOfSetOfFaces = it.second;

			for (shared_ptr<SetOfFacesInPlane>& setOfFaces : vecOfSetOfFaces)
			{
				std::vector<carve::mesh::Vertex<3>*> verticesAllFacesInPlane;
				for (carve::mesh::Face<3>*face : setOfFaces->setFaces)
				{
					const carve::mesh::Edge<3>* e = face->edge;
					for (size_t ii = 0; ii < face->n_edges; ++ii)
					{
						verticesAllFacesInPlane.push_back(e->vert);
						e = e->next;
					}
					face->getVertices(verticesAllFacesInPlane);
				}

				vec3 normalAllFaces = GeomUtils::computePolygonNormal(verticesAllFacesInPlane);

				double dotProduct = dot(setOfFaces->plane.N, normalAllFaces);
				if (std::abs(dotProduct + 1.0) > epsAngle * 100)
				{
					// opposite direction
					normalAllFaces = -normalAllFaces;
				}

				dotProduct = dot(setOfFaces->plane.N, normalAllFaces);

				if (std::abs(dotProduct - 1.0) > epsAngle)
				{
					// should not be so much off, but can happen
				}
				//setOfFaces->plane.N = normalAllFaces;

				for (carve::mesh::Face<3>*face : setOfFaces->setFaces)
				{
					face->plane.d = setOfFaces->plane.d;
					face->plane.N = setOfFaces->plane.N;
					std::vector<carve::mesh::Vertex<3>*> vertices;
					face->getVertices(vertices);
					for (auto vert : vertices)
					{
						vec3 v;
						double t;

						const vec3& rayPoint1 = vert->v;
						vec3 rayPoint2 = rayPoint1 + face->plane.N;
						//                                        const Plane& p, const Vector& v1, const Vector& v2, Vector& v, double& t, double eps)
						carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection(face->plane, rayPoint1, rayPoint2, v, t, params.epsMergePoints);
						if (intersect > 0)
						{
							double dx = vert->v.x - v.x;
							double dy = vert->v.y - v.y;
							double dz = vert->v.z - v.z;

							double distance2 = dx * dx + dy * dy + dz * dz;

							if (distance2 > 1e-20 && distance2 < epsDistanceSinglePoints * epsDistanceSinglePoints)
							{
								vert->v = v;
								++m_numCorrectedVertices;
								if (face->mesh)
								{
									face->mesh->m_volume = std::numeric_limits<double>::quiet_NaN();
								}
#ifdef _DEBUG
								if (distance2 > EPS_M9)
								{
									int wait = 0;
								}
#endif
							}
						}
					}
				}
			}
		}
	}



	static void flattenFacePlanes_old(shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const GeomProcessingParams& params)
	{
		// project face points into coplanar face
		double epsAngle = params.epsMergeAlignedEdgesAngle * 100.0;
		double epsDistanceSinglePoints = params.epsMergePoints * 10.0;
		double epsDistanceFaceCentroids = params.epsMergePoints * 10.0;
		double epsMinDistanceMovePoints2 = params.epsMergePoints * 0.01 * params.epsMergePoints * 0.01;

		// TODO: integrate this into other function. First merge all coplanar faces, then do this:

		// TODO: use caching from single mesh flattening, first for op1, then op1

		for (auto mesh1 : op1->meshes)
		{
			if (mesh1->faces.size() > 500)
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

						if (!bbox1.intersects(bbox2, params.epsMergePoints))
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
							bool intersects = bbox1test.intersects(bbox2test, params.epsMergePoints);
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
									//                                        const Plane& p, const Vector& v1, const Vector& v2, Vector& v, double& t, double eps)
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

										if (distance2 > 1e-18 && distance2 < epsDistanceSinglePoints * epsDistanceSinglePoints)
										{
											vert->v = v;
										}
									}
								}
							}
						}
					}
				}

				//#ifdef _DEBUG
				//			carve::geom::aabb<3> bbox1 = op1->getAABB();
				//			carve::geom::aabb<3> bbox2 = op2->getAABB();
				//			bbox1.unionAABB(bbox2);
				//			//GeomDebugDump::moveOffset(bbox1.extent.y + 0.2);
				//#endif
			}
		}
	}
};



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


#pragma once

#include <ifcpp/model/BasicTypes.h>
#include "GeomUtils.h"
#include <ifcpp/model/StatusCallback.h>
#include "GeometryInputData.h"
#include "MeshOps.h"

class MeshFlattener
{
public:
	size_t m_numCorrectedVertices = 0;
	struct SetOfFacesInPlane {
		carve::geom::plane<3> plane;
		std::set<carve::mesh::Face<3>* > setFaces;
	};
	std::multimap<double, std::vector<shared_ptr<SetOfFacesInPlane> > > m_mapInPlaneFaces;

	shared_ptr<SetOfFacesInPlane> findPlaneForDistance( const std::vector<shared_ptr<SetOfFacesInPlane> >& vecOfPlanes, const vec3& normal, double epsAngle)
	{
		for (const shared_ptr<SetOfFacesInPlane>& setOfFacesInPlane : vecOfPlanes)
		{
			vec3& setOfFacesNormal = setOfFacesInPlane->plane.N;
			double dotProduct = dot(setOfFacesNormal, normal);
			if (std::abs(dotProduct + 1.0) > epsAngle && std::abs(dotProduct - 1.0) > epsAngle)
			{
				continue;
			}
			return setOfFacesInPlane;
		}
		return shared_ptr<SetOfFacesInPlane>();
	}

	shared_ptr<SetOfFacesInPlane> findPlaneForDistance(double d, const vec3& normal, double epsMergePoints, double epsPlaneAngle)
	{
		auto it = m_mapInPlaneFaces.lower_bound(d);

		auto low = m_mapInPlaneFaces.lower_bound(d);
		if (low == m_mapInPlaneFaces.end())
		{
			if (m_mapInPlaneFaces.size() > 0)
			{
				double lastElement = m_mapInPlaneFaces.rbegin()->first;
				double delta = lastElement - d;
				if (std::abs(delta) <= epsMergePoints)
				{
					std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = m_mapInPlaneFaces.rbegin()->second;
					return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
				}
			}

			return shared_ptr<SetOfFacesInPlane>();
			//size_t vertex_index = m_poly_data->addVertex(pt);
			//m_mapInPlaneFaces.insert({ { vertex_z, vertex_index } });
			//return vertex_index;
		}
		else if (low == m_mapInPlaneFaces.begin())
		{
			double existingD = low->first;
			double delta = existingD - d;
			if (std::abs(delta) <= epsMergePoints)
			{
				if (low != m_mapInPlaneFaces.end())
				{
					// check if next element is closer
					auto itNext = low;
					++itNext;
					if (itNext != m_mapInPlaneFaces.end())
					{
						double nextD = itNext->first;
						double deltaNext = nextD - d;
						if (std::abs(deltaNext) <= std::abs(delta))
						{
							std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = itNext->second;
							return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
						}
					}
				}
				std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = low->second;
				return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
			}
			else
			{
				return shared_ptr<SetOfFacesInPlane>();
				//size_t vertex_index = m_poly_data->addVertex(pt);
				//m_mapInPlaneFaces.insert({ { vertex_z, vertex_index } });
				//return vertex_index;
			}
		}
		else
		{
			auto prev = std::prev(low);
			double dzPrev = d - prev->first;
			double dLow = low->first - d;
			if (std::abs(dzPrev) < std::abs(dLow))
			{
				if (std::abs(dzPrev) <= epsMergePoints)
				{
					std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = prev->second;
					return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
				}
				else
				{
					return shared_ptr<SetOfFacesInPlane>();
					//size_t vertex_index = m_poly_data->addVertex(pt);
					//m_mapInPlaneFaces.insert({ { vertex_z, vertex_index } });
					//return vertex_index;
				}
			}
			else
			{
				if (std::abs(dLow) <= epsMergePoints)
				{
					std::vector<shared_ptr<SetOfFacesInPlane> >& existingPlanes = low->second;
					return findPlaneForDistance(existingPlanes, normal, epsPlaneAngle);
				}
				else
				{
					return shared_ptr<SetOfFacesInPlane>();
					//size_t vertex_index = m_poly_data->addVertex(pt);
					//m_mapInPlaneFaces.insert({ { vertex_z, vertex_index } });
					//return vertex_index;
				}
			}
		}

		return shared_ptr<SetOfFacesInPlane>();
	}

	void flattenFacePlanes(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params)
	{
		if (!meshset)
		{
			return;
		}
		// project face points into coplanar face
		double epsAngle = params.epsMergeAlignedEdgesAngle * 100.0;
		double epsDistanceSinglePoints = params.epsMergePoints * 10.0;
		double epsDistanceFaceCentroids = params.epsMergePoints * 10.0;
		double epsMinDistanceMovePoints2 = params.epsMergePoints * 0.01 * params.epsMergePoints * 0.01;

		// collect all faces in one plane with high epsilon


		for (auto mesh1 : meshset->meshes)
		{
			if (mesh1->faces.size() > 1000)
			{
				continue;
			}

			for (auto face1 : mesh1->faces)
			{
				carve::geom::plane<3>& facePlane = face1->plane;
				const vec3 face1Normal = facePlane.N;

				shared_ptr<SetOfFacesInPlane> existingPlane = findPlaneForDistance(facePlane.d, face1Normal, epsDistanceSinglePoints, epsAngle);
				if (!existingPlane)
				{
					existingPlane = make_shared<SetOfFacesInPlane>();
					existingPlane->setFaces.insert(face1);
					existingPlane->plane = facePlane;
					m_mapInPlaneFaces.insert({ facePlane.d, {existingPlane} });
					continue;
				}

#ifdef _DEBUG
				const vec3 setOfFacesPlaneNormal = existingPlane->plane.N;
				double dotProduct = dot(face1Normal, setOfFacesPlaneNormal);
				if (std::abs(dotProduct + 1.0) > epsAngle * 1.1 && std::abs(dotProduct - 1.0) > epsAngle*1.1)
				{
					std::cerr << "check angle in plane failed" << std::endl;
				}

				double deltaD = facePlane.d - existingPlane->plane.d;
				if (std::abs(deltaD) > epsDistanceFaceCentroids*1.1)
				{
					std::cerr << "check distance in plane failed" << std::endl;
				}
#endif

				existingPlane->plane.d = (existingPlane->plane.d + facePlane.d) * 0.5;
				existingPlane->plane.N = (existingPlane->plane.N + facePlane.N) * 0.5;  // could be also weighted by face area
				existingPlane->setFaces.insert(face1);

			}

		}

		// then re-compute the set of faces plane normals. Then check again which faces are in the plane

		for (auto it : m_mapInPlaneFaces)
		{
			std::vector<shared_ptr<SetOfFacesInPlane> >& vecOfSetOfFaces = it.second;

			for (shared_ptr<SetOfFacesInPlane>& setOfFaces : vecOfSetOfFaces)
			{
				std::vector<carve::mesh::Vertex<3>*> verticesAllFacesInPlane;
				for (carve::mesh::Face<3>*face : setOfFaces->setFaces)
				{
					const carve::mesh::Edge<3>* e = face->edge;
					for (size_t ii = 0; ii < face->n_edges; ++ii)
					{
						verticesAllFacesInPlane.push_back(e->vert);
						e = e->next;
					}
					face->getVertices(verticesAllFacesInPlane);
				}

				vec3 normalAllFaces = GeomUtils::computePolygonNormal(verticesAllFacesInPlane);

				double dotProduct = dot(setOfFaces->plane.N, normalAllFaces);
				if (std::abs(dotProduct + 1.0) > epsAngle*100)
				{
					// opposite direction
					normalAllFaces = -normalAllFaces;
				}

				dotProduct = dot(setOfFaces->plane.N, normalAllFaces);

				if (std::abs(dotProduct - 1.0) > epsAngle)
				{
					// should not be so much off, but can happen
				}
				//setOfFaces->plane.N = normalAllFaces;

				for (carve::mesh::Face<3>*face : setOfFaces->setFaces)
				{
					face->plane.d = setOfFaces->plane.d;
					face->plane.N = setOfFaces->plane.N;
					std::vector<carve::mesh::Vertex<3>*> vertices;
					face->getVertices(vertices);
					for (auto vert : vertices)
					{
						vec3 v;
						double t;
						
						const vec3& rayPoint1 = vert->v;
						vec3 rayPoint2 = rayPoint1 + face->plane.N;
						//                                        const Plane& p, const Vector& v1, const Vector& v2, Vector& v, double& t, double eps)
						carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection(face->plane, rayPoint1, rayPoint2, v, t, params.epsMergePoints);
						if (intersect > 0)
						{
							double dx = vert->v.x - v.x;
							double dy = vert->v.y - v.y;
							double dz = vert->v.z - v.z;

							double distance2 = dx * dx + dy * dy + dz * dz;

							if (distance2 > 0.0 && distance2 < epsDistanceSinglePoints * epsDistanceSinglePoints)
							{
								vert->v = v;
								++m_numCorrectedVertices;
#ifdef _DEBUG
								if (distance2 > EPS_M9)
								{
									int wait = 0;
								}
#endif
							}
						}
					}
				}
			}
		}
	}



	static void flattenFacePlanes(shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const GeomProcessingParams& params)
	{
		// project face points into coplanar face
		double epsAngle = params.epsMergeAlignedEdgesAngle * 100.0;
		double epsDistanceSinglePoints = params.epsMergePoints * 10.0;
		double epsDistanceFaceCentroids = params.epsMergePoints * 10.0;
		double epsMinDistanceMovePoints2 = params.epsMergePoints * 0.01 * params.epsMergePoints * 0.01;

		// TODO: integrate this into other function. First merge all coplanar faces, then do this:

		// TODO: use caching from single mesh flattening, first for op1, then op1

		for (auto mesh1 : op1->meshes)
		{
			if (mesh1->faces.size() > 500)
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

						if (!bbox1.intersects(bbox2, params.epsMergePoints))
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
							bool intersects = bbox1test.intersects(bbox2test, params.epsMergePoints);
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
									//                                        const Plane& p, const Vector& v1, const Vector& v2, Vector& v, double& t, double eps)
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

				//#ifdef _DEBUG
				//			carve::geom::aabb<3> bbox1 = op1->getAABB();
				//			carve::geom::aabb<3> bbox2 = op2->getAABB();
				//			bbox1.unionAABB(bbox2);
				//			//GeomDebugDump::moveOffset(bbox1.extent.y + 0.2);
				//#endif
			}
		}
	}
};
*/
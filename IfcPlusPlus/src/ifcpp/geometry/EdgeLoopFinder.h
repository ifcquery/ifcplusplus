#pragma once

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/geometry/GeomUtils.h>
#include <ifcpp/geometry/GeometryInputData.h>
#include <ifcpp/geometry/MeshOps.h>

struct EdgeLoopElement
{
	EdgeLoopElement(carve::mesh::Edge<3>* _edge, bool _sameSense)
	{
		edge = _edge;
		sameSense = _sameSense;
	}
	carve::mesh::Edge<3>* edge = nullptr;
	bool sameSense = true;
};

class EdgeLoopFinder
{
public:
	double m_epsMergePoints = 1e-8;
	double m_epsMergeOpenEdgesToPoint = 50*1e-8;
	bool m_openEdgesMergedToPoint = false;
	std::set<carve::mesh::Edge<3>* > m_setOpenEdges;
	std::unordered_map<carve::mesh::Vertex<3>*, std::set<carve::mesh::Edge<3>* > > m_mapVertexOpenEdges;
	std::vector<std::vector<carve::mesh::Edge<3>*> > m_closedEdgeLoopsOfOpenEdges;
	std::vector<carve::mesh::Edge<3>* > m_createdEdgesToDelete;
	const carve::mesh::Mesh<3>* m_mesh = nullptr;
	shared_ptr<carve::mesh::MeshSet<3> > m_meshset;
	shared_ptr<carve::mesh::MeshSet<3> > m_meshsetCopyUnChanged;
	//typedef carve::geom::RTreeNode<3, carve::mesh::Vertex<3>*> vertex_rtree_t;
	typedef carve::geom::RTreeNode<3, carve::mesh::Face<3>*> face_rtree_t;

	EdgeLoopFinder(double epsMergePoints, double epsMergeOpenEdgesToPoint) 
		: m_epsMergePoints(epsMergePoints), m_epsMergeOpenEdgesToPoint(epsMergeOpenEdgesToPoint)
	{

	}

	~EdgeLoopFinder()
	{
		for (carve::mesh::Edge<3>*edge : m_createdEdgesToDelete)
		{
			delete edge;
		}
	}

	void createBackup()
	{
		if (!m_meshsetCopyUnChanged)
		{
			m_meshsetCopyUnChanged = shared_ptr<carve::mesh::MeshSet<3> >(m_meshset->clone());
		}
	}

	void initFromMesh(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, const carve::mesh::Mesh<3>* mesh, 
		const GeomProcessingParams& params, bool tryMergeShortOpenEdges, bool& meshsetChanged)
	{
		if (!mesh)
		{
			return;
		}
		meshsetChanged = false;
		m_meshset = meshset;
		m_mesh = mesh;

		if (mesh->open_edges.size() == 0)
		{
			return;
		}

		std::set<carve::mesh::Vertex<3>* > openEdgeVertices;
		std::set<vec3> openEdgeVertexPoints;
		for (auto openEdge : mesh->open_edges)
		{
			if (openEdge)
			{
				m_setOpenEdges.insert(openEdge);
				if (openEdge->vert)
				{
					openEdgeVertices.insert(openEdge->v1());
					openEdgeVertices.insert(openEdge->v2());
					openEdgeVertexPoints.insert(openEdge->v1()->v);
					openEdgeVertexPoints.insert(openEdge->v2()->v);

					auto itFindVertexMapV1 = m_mapVertexOpenEdges.find(openEdge->v1());
					if (itFindVertexMapV1 == m_mapVertexOpenEdges.end())
					{
						m_mapVertexOpenEdges.insert({ openEdge->v1(), {openEdge} });
					}
					else
					{
						itFindVertexMapV1->second.insert(openEdge);
					}

					auto itFindVertexMapV2 = m_mapVertexOpenEdges.find(openEdge->v2());
					if (itFindVertexMapV2 == m_mapVertexOpenEdges.end())
					{
						m_mapVertexOpenEdges.insert({ openEdge->v2(), {openEdge} });
					}
					else
					{
						itFindVertexMapV2->second.insert(openEdge);
					}
				}
			}
		}

		if (tryMergeShortOpenEdges)
		{
			carve::geom::aabb<3> bbox;
			bbox.fit(openEdgeVertexPoints.begin(), openEdgeVertexPoints.end());

			if (bbox.extent.x < m_epsMergeOpenEdgesToPoint)
			{
				if (bbox.extent.y < m_epsMergeOpenEdgesToPoint)
				{
					if (bbox.extent.z < m_epsMergeOpenEdgesToPoint)
					{
						createBackup();
						for (auto v : openEdgeVertices)
						{
							v->v = bbox.pos;
						}
						m_openEdgesMergedToPoint = true;
						meshsetChanged = true;
					}
				}
			}
		}

		size_t numPointsMoved = 0;
		mergeCloseVertices(numPointsMoved);

		if (numPointsMoved > 0 || m_openEdgesMergedToPoint)
		{
			meshsetChanged = true;
		}
	}

	bool findNextEdge(const carve::mesh::Vertex<3>* startVertex, carve::mesh::Vertex<3>* currentVertex, carve::mesh::Edge<3>* currentEdge, std::set<carve::mesh::Edge<3>*>& setCurrentPath, std::vector<shared_ptr<EdgeLoopElement> >& vecLoopEdges)
	{
		auto itFindVertexMapV1 = m_mapVertexOpenEdges.find(currentVertex);

		std::set<carve::mesh::Edge<3>* >& setEdgesFromStartVertex = itFindVertexMapV1->second;

		for (carve::mesh::Edge<3>*checkEdge : setEdgesFromStartVertex)
		{
			if (!checkEdge)
			{
				continue;
			}

			if (!checkEdge->vert)
			{
				continue;
			}

			if (currentEdge == checkEdge)
			{
				// we don't want to go back
				continue;
			}

			if (m_setOpenEdges.find(checkEdge) == m_setOpenEdges.end())
			{
				continue;
			}
			if (setCurrentPath.find(checkEdge) != setCurrentPath.end())
			{
				continue;
			}

			carve::mesh::Vertex<3>* oppositeVertex = nullptr;
			bool sameSenseCheckEdge = true;
			if (checkEdge->v1() == currentVertex)
			{
				oppositeVertex = checkEdge->v2();
			}
			else if (checkEdge->v2() == currentVertex)
			{
				oppositeVertex = checkEdge->v2();
				sameSenseCheckEdge = false;
			}
			else
			{
				// should not happen
				std::cout << "checkEdge->v1() != currentVertex && checkEdge->v2() == currentVertex" << std::endl;
				continue;
			}

			if (oppositeVertex == startVertex)
			{
				if (currentEdge == nullptr)
				{
					continue;
				}
				else
				{
					// found closed loop
					vecLoopEdges.push_back(make_shared<EdgeLoopElement>(checkEdge, sameSenseCheckEdge));
					return true;
				}
			}

			// check if v2 is also in mapVertexOpenEdges
			auto itFindV2 = m_mapVertexOpenEdges.find(oppositeVertex);
			if (itFindV2 == m_mapVertexOpenEdges.end())
			{
				// checkEdge leads somewhere else. It is not part of the set of open edges
				continue;
			}

			setCurrentPath.insert(checkEdge);
			bool found = findNextEdge(startVertex, oppositeVertex, checkEdge, setCurrentPath, vecLoopEdges);
			if (found)
			{
				vecLoopEdges.push_back(make_shared<EdgeLoopElement>(checkEdge, sameSenseCheckEdge));
				return true;
			}

			continue;

			std::set<carve::mesh::Edge<3>* >& setEdgesFromCurrentVertex = itFindVertexMapV1->second;
			for (auto it : setEdgesFromStartVertex)
			{
				carve::mesh::Edge<3>* edgeFromOppositeVertex = it;
				carve::mesh::Vertex<3>* vnext = nullptr;
				bool sameSense = true;
				if (edgeFromOppositeVertex->v1() == oppositeVertex)
				{
					carve::mesh::Vertex<3>* vnext = edgeFromOppositeVertex->v2();
				}
				else if (edgeFromOppositeVertex->v2() == oppositeVertex)
				{
					carve::mesh::Vertex<3>* vnext = edgeFromOppositeVertex->v1();
					sameSense = false;
				}

				if (vnext == startVertex)
				{
					if (currentEdge != nullptr)
					{
						vecLoopEdges.push_back(make_shared<EdgeLoopElement>(edgeFromOppositeVertex, sameSense));
						return true;
					}
				}

				if (vnext != nullptr)
				{
					bool found = findNextEdge(startVertex, oppositeVertex, edgeFromOppositeVertex, setCurrentPath, vecLoopEdges);
					if (found)
					{
						vecLoopEdges.push_back(make_shared<EdgeLoopElement>(edgeFromOppositeVertex, sameSense));
						//vecLoopEdges.push_back(edgeFromOppositeVertex);
					}
				}
			}
		}

		if (currentEdge == nullptr)
		{
			// start

		}
		return false;
	}

	bool findAndEliminateShortOpenEdges( double epsMinDistanceInEachAxisDirection)
	{
		bool changesDone = false;

		for (auto openEdge : m_mesh->open_edges)
		{
			vec3 deltaEdge1 = openEdge->v1()->v - openEdge->v2()->v;
			if (std::abs(deltaEdge1.x) < epsMinDistanceInEachAxisDirection)
			{
				if (std::abs(deltaEdge1.y) < epsMinDistanceInEachAxisDirection)
				{
					if (std::abs(deltaEdge1.z) < epsMinDistanceInEachAxisDirection)
					{
						// TODO: try to preserve the plane of faces with openings. 
						// If there are many faces in one plane, and current v1() or v2() is in that plane, move the other one

						// for now, simple solution:
						openEdge->v1()->v = openEdge->v2()->v;
						changesDone = true;
					}
				}
			}
		}
		return changesDone;
	}

	void traverseNode(face_rtree_t* node)
	{
		if (node)
		{
			for (face_rtree_t* childNode = node->child; node; node = node->sibling)
			{
				//childNode->findPartition()
				traverseNode(childNode);
			}
		}
	}

	bool findAndMoveOffsetVertices()
	{

#ifdef _DEBUG
		//typedef std::unordered_map<carve::mesh::Face<3>*, std::vector<carve::mesh::Face<3>*> > face_pairs_t;

		auto itFaceBegin = m_meshset->faceBegin();
		auto itFaceEnd = m_meshset->faceEnd();
		std::unique_ptr<face_rtree_t> vert_rtree(face_rtree_t::construct_STR(itFaceBegin, itFaceEnd, 4, 4));

		if (vert_rtree->child)
		{
			traverseNode(vert_rtree.get());

			//for (vertex_rtree_t* node = vert_rtree->child; node; node = node->sibling)
			{
				
				//generateIntersectionCandidates(a, node, b, b_node, face_pairs, false);
			}
		}
#endif

		

		bool changesDone = false;
		//typedef carve::geom::RTreeNode<3, carve::mesh::Face<3>*> face_rtree_t;
		//typedef std::unordered_map<carve::mesh::Face<3>*, std::vector<carve::mesh::Face<3>*> > face_pairs_t;

		//auto itFaceBegin = m_meshset->faceBegin();
		//auto itFaceEnd = m_meshset->faceEnd();
		//std::unique_ptr<face_rtree_t> a_rtree(face_rtree_t::construct_STR(itFaceBegin, itFaceEnd, 4, 4));

		//if (!a_rtree->bbox.intersects(b_node->bbox, m_epsilon))
		//{
		//	return;
		//}

		//if (a_rtree->child )// && (descend_a || !b_node->child))
		//{
		//	for (face_rtree_t* node = a_rtree->child; node; node = node->sibling)
		//	{
		//		generateIntersectionCandidates(a, node, b, b_node, face_pairs, false);
		//	}
		//}

		return changesDone;
	}

	bool findAndEliminateSimpleLoops1(const GeomProcessingParams& params)
	{
		bool changesDone = false;

		// case 1:
		//                      openEdge1
		//   startVertex  o -------------------->  simpleLoopEndVertex
		//                  <--------------------   
		//                      openEdge2

#ifdef _DEBUG
		GeomDebugDump::clearMeshsetDump();
		carve::geom::aabb<3> bbox;
		std::vector<vec3> points;
		glm::vec4 color(1, 0.5, 1, 1);
		GeomDebugDump::dumpMeshset(m_meshset, color, true, true);
		for (auto openEdge : m_mesh->open_edges)
		{
			vec3 deltaEdge1 = openEdge->v1()->v - openEdge->v2()->v;
			points.push_back(openEdge->v1()->v);
			points.push_back(openEdge->v2()->v);
			
			GeomDebugDump::dumpPolyline({ openEdge->v1()->v, openEdge->v2()->v }, color, 1, true, false);
			GeomDebugDump::moveOffset(0.01);
		}
		bbox.fit(points.begin(), points.end());
		GeomDebugDump::moveOffset(bbox.extent.y*2.2 + 0.1);
#endif

		for (auto itFindVertexMapV1 = m_mapVertexOpenEdges.begin(); itFindVertexMapV1 != m_mapVertexOpenEdges.end(); ++itFindVertexMapV1)
		{
			// try to find 2 matching edges
			carve::mesh::Vertex<3>* startVertex = itFindVertexMapV1->first;
			carve::mesh::Edge<3>* openEdge1 = nullptr;
			carve::mesh::Edge<3>* openEdge2 = nullptr;
			
			std::set<carve::mesh::Edge<3>* >& openEdgesFromAndToStartVertex = itFindVertexMapV1->second;

			for (carve::mesh::Edge<3>* openEdge1 : openEdgesFromAndToStartVertex)
			{
				vec3 deltaEdge1 = openEdge1->v1()->v - openEdge1->v2()->v;
				if (deltaEdge1.length2() < m_epsMergePoints * m_epsMergePoints * 10)
				{
					continue;
				}
				carve::mesh::Vertex<3>* simpleLoopEndVertex = nullptr;
				if (openEdge1->v1() == startVertex)
				{
					simpleLoopEndVertex = openEdge1->v2();
				}
				else if (openEdge1->v2() == startVertex)
				{
					simpleLoopEndVertex = openEdge1->v1();
				}
				else
				{
#ifdef _DEBUG
					std::cout << "edge in m_mapVertexOpenEdges not pointing to vertex" << std::endl;
#endif
				}

#ifdef _DEBUG
				glm::vec4 color(1, 0.5, 1, 1);
				
				GeomDebugDump::dumpPolyline({ openEdge1->v1()->v, openEdge1->v2()->v }, color, 1, true, false);
				GeomDebugDump::moveOffset(0.01);
#endif

				bool edgeIsReversed = false;
				std::set<std::pair<carve::mesh::Vertex<3>*, carve::mesh::Vertex<3>*>> closeButNotIdinticalVertices;
				openEdge2 = findEdgeByStartAndEndPoint(startVertex, simpleLoopEndVertex, openEdge1, edgeIsReversed, m_epsMergePoints, closeButNotIdinticalVertices);
				if (!openEdge2)
				{
					// try with bigger epsilon
					openEdge2 = findEdgeByStartAndEndPoint(startVertex, simpleLoopEndVertex, openEdge1, edgeIsReversed, m_epsMergePoints * 10, closeButNotIdinticalVertices);
				}

				if (openEdge2)
				{
					if (closeButNotIdinticalVertices.size() > 0)
					{
						createBackup();
						auto pair = closeButNotIdinticalVertices.begin();
						carve::mesh::Vertex<3>* v1 = pair->first;
						carve::mesh::Vertex<3>* v2 = pair->second;
						v1->v = v2->v;
						changesDone = true;
					}

	
					//       v1  o --------------------> o v2
					//       v2   <--------------------   v1
					// 
					//       v1  o --------------------> o v2
					//       v1   --------------------->   v2
					//                      openEdge2
					
					
					

					//bool connected1 = openEdge1->v1() == openEdge2->v2() && openEdge1->v2() == openEdge2->v1();
					//bool connected2 = openEdge1->v1() == openEdge2->v1() && openEdge1->v2() == openEdge2->v2();  // not a correct half-edge

					vec3 delta1 = openEdge1->v1()->v - openEdge2->v2()->v;
					bool connected1 = false;
					if (delta1.length2() < m_epsMergePoints * m_epsMergeOpenEdgesToPoint * 10)
					{
						vec3 delta2 = openEdge1->v2()->v - openEdge2->v1()->v;
						if (delta2.length2() < m_epsMergePoints * m_epsMergeOpenEdgesToPoint * 10)
						{
							if (delta2.length2() > EPS_M16)
							{
								openEdge1->v2()->v = openEdge2->v1()->v;
							}
							connected1 = true;
						}
					}

					if(connected1)
					{
						createBackup();
						bool check = openEdge2->vert == openEdge1->v2();
						openEdge2->rev = openEdge1;
						openEdge1->rev = openEdge2;
						changesDone = true;
					}
					else
					{
						// check if connected reversed
						delta1 = openEdge1->v1()->v - openEdge2->v1()->v;
						bool connected2 = false;
						if (delta1.length2() < m_epsMergePoints * m_epsMergeOpenEdgesToPoint * 10)
						{
							vec3 delta2 = openEdge1->v2()->v - openEdge2->v2()->v;
							if (delta2.length2() < m_epsMergePoints * m_epsMergeOpenEdgesToPoint * 10)
							{
								connected2 = true;
							}
						}

						if (connected2)
						{
							createBackup();

							//       v1  o --------------------> o v2
							//  ---> v1   --------------------->   v2    ------>
							//  prev                                      next

							if (openEdge2->next)
							{
								openEdge2->next->vert = openEdge1->v2();
							}
							openEdge2->vert = openEdge1->v1();
							openEdge2->rev = openEdge1;
							openEdge1->rev = openEdge2;
							changesDone = true;
						}
					}

					//if (openEdge1->vert == openEdge2->v2())
					//{
					//	createBackup();
					//	openEdge2->vert = openEdge1->v2();
					//	openEdge2->rev = openEdge1;
					//	openEdge1->rev = openEdge2;
					//	changesDone = true;
					//}

					//if (!changesDone)
					//{
					//	
					//	
					//	if (edgeIsReversed)
					//	{
					//		createBackup();
					//		//openEdge1->vert = openEdge1->v2();
					//		//openEdge2->vert = openEdge1->v1();
					//		openEdge2->rev = openEdge1;
					//		openEdge1->rev = openEdge2;
					//		changesDone = true;
					//	}
					//	else
					//	{
					//		createBackup();
					//		// TODO: check which one of the vertices is abandoned
					//		//openEdge1->vert = openEdge1->v1();
					//		//openEdge2->vert = openEdge1->v2();
					//		openEdge2->rev = openEdge1;
					//		openEdge1->rev = openEdge2;
					//		changesDone = true;
					//	}
					//}
				}
			}
		}

		if (m_meshsetCopyUnChanged)
		{
			MeshSetInfo infoChangedMesh;
			MeshOps::checkMeshSetValidAndClosed(m_meshset, infoChangedMesh, params);

			MeshSetInfo infoBackup;
			MeshOps::checkMeshSetValidAndClosed(m_meshsetCopyUnChanged, infoBackup, params);

			if (MeshOps::isBetterForBoolOp(infoBackup, infoChangedMesh, false))
			{
				m_meshset = m_meshsetCopyUnChanged;
			}
		}

		return changesDone;
	}


	bool findAndEliminateSimpleLoops2()
	{
		bool changesDone = false;

		// case 2:
		//                             openEdge1
		//   startVertex  o ---------------------------------------> o v2
		//                 <-------------------o<------------------   
		//                    openEdge3        v3     openEdge2
		return changesDone;
	}

	void mergeCloseVertices( size_t& numPointsMoved)
	{
		numPointsMoved = 0;
		if (m_mapVertexOpenEdges.size() > 10000)
		{
			return;
		}

		// find vertices that are close enough to merge
		double eps2 = m_epsMergePoints * m_epsMergePoints * 10.0;
		size_t numMapEntries = m_mapVertexOpenEdges.size();
		for (auto it1 = m_mapVertexOpenEdges.begin(); it1 != m_mapVertexOpenEdges.end(); ++it1)
		{
			carve::mesh::Vertex<3>* vertex1 = it1->first;

			for (auto it2 = m_mapVertexOpenEdges.begin(); it2 != m_mapVertexOpenEdges.end(); ++it2)
			{
				carve::mesh::Vertex<3>* vertex2 = it2->first;

				if (vertex1 == vertex2)
				{
					continue;
				}
				vec3 delt = vertex1->v - vertex2->v.v;
				double length2 = delt.length2();
				if (length2 > 0 && length2 < eps2)
				{
					vec3 middlePoint = (vertex1->v + vertex2->v.v)*0.5;
					createBackup();
					vertex1->v = middlePoint;
					vertex2->v = middlePoint;
					++numPointsMoved;
				}
			}
		}
	}

	void findLoops()
	{
		if (m_mapVertexOpenEdges.size() > 100)
		{
			return;
		}

		// find connected sets of vertices
		//std::unordered_map<carve::mesh::Vertex<3>*, std::set<carve::mesh::Edge<3>* > > mapVertexOpenEdges;
		std::vector<std::set<carve::mesh::Edge<3>*> > connectedEdges;
		std::set<carve::mesh::Vertex<3>*> setVerticesDone;
		std::set<carve::mesh::Edge<3>*> setEdgesDone;
		for (auto it = m_mapVertexOpenEdges.begin(); it != m_mapVertexOpenEdges.end(); ++it)
		{
			// get all connected vertices
			carve::mesh::Vertex<3>* startVertex = it->first;
			//if (setVerticesDone.find(startVertex) != setVerticesDone.end())
			//{
			//	continue;
			//}
			//setVerticesDone.insert(startVertex);

			std::set<carve::mesh::Edge<3>* > currentSetOfConnectedEdges;

			std::set<carve::mesh::Edge<3>* >& edgesConnectedToStartVertex = it->second;
			for (auto it2 = edgesConnectedToStartVertex.begin(); it2 != edgesConnectedToStartVertex.end(); ++it2)
			{
				carve::mesh::Edge<3>* edgeConnected = *it2;
				if (setEdgesDone.find(edgeConnected) != setEdgesDone.end())
				{
					continue;
				}
				setEdgesDone.insert(edgeConnected);

				if (m_setOpenEdges.find(edgeConnected) == m_setOpenEdges.end())
				{
					// it's not an open edge
					continue;
				}

				if (edgeConnected->v1() == startVertex)
				{
					currentSetOfConnectedEdges.insert(edgeConnected);
				}
				carve::mesh::Edge<3>* edgeConnectedNext = edgeConnected->next;
				if (edgeConnectedNext->face)
				{
					size_t numEdges = edgeConnectedNext->face->n_edges;
					for (size_t jj = 0; jj < numEdges; ++jj)
					{
						if (m_setOpenEdges.find(edgeConnected) != m_setOpenEdges.end())
						{
							// it's an open edge
							if (setEdgesDone.find(edgeConnectedNext) != setEdgesDone.end())
							{
								continue;
							}
							setEdgesDone.insert(edgeConnectedNext);
							currentSetOfConnectedEdges.insert(edgeConnected);
						}

						edgeConnectedNext = edgeConnectedNext->next;
					}
				}
			}
			
		}
		


		size_t numMapEntries = m_mapVertexOpenEdges.size();
		for (size_t ii = 0; ii < numMapEntries; ++ii)
		{
			// try to find closed loop of open edges
			std::vector<shared_ptr<EdgeLoopElement> > vecLoopEdges;
			auto itFindVertexMapV1 = m_mapVertexOpenEdges.begin();
			carve::mesh::Vertex<3>* startVertex = itFindVertexMapV1->first;
			carve::mesh::Edge<3>* currentEdge = nullptr;

			std::set<carve::mesh::Edge<3>*> setCurrentPath;
			bool foundLoop = findNextEdge(startVertex, startVertex, currentEdge, setCurrentPath, vecLoopEdges);

			itFindVertexMapV1 = m_mapVertexOpenEdges.erase(itFindVertexMapV1);

			if (foundLoop)
			{
				m_closedEdgeLoopsOfOpenEdges.push_back(std::vector<carve::mesh::Edge<3>*>());
				std::vector<carve::mesh::Edge<3>* >& edgeLoop = *m_closedEdgeLoopsOfOpenEdges.rbegin();
				for (auto it : vecLoopEdges)
				{
					shared_ptr<EdgeLoopElement>& loopElement = it;
					carve::mesh::Edge<3>* currentEdge = nullptr;
					if (loopElement->sameSense)
					{
						currentEdge = loopElement->edge;
					}
					else
					{
						if (loopElement->edge->rev != nullptr)
						{
							currentEdge = loopElement->edge->rev;
						}
						else
						{
							carve::mesh::Vertex<3>* v1 = loopElement->edge->v1();
							carve::mesh::Vertex<3>* v2 = loopElement->edge->v2();
							currentEdge = new carve::mesh::Edge<3>(v1, nullptr);
							m_createdEdgesToDelete.push_back(currentEdge);
						}
					}

					if (currentEdge != nullptr)
					{
						auto itFindErase = m_mapVertexOpenEdges.find(currentEdge->vert);
						if (itFindErase != m_mapVertexOpenEdges.end())
						{
							m_mapVertexOpenEdges.erase(itFindErase);
						}
						edgeLoop.push_back(currentEdge);
					}
				}

#ifdef _DEBUG
				GeomDebugDump::clearMeshsetDump();

				glm::vec4 color(1, 0.5, 1, 1);
				std::vector<vec3 > openEdgePoints;
				std::vector<double> lengthOfEdges;
				int edgeCount = 0;
				for (auto e : m_mesh->open_edges)
				{
					vec3 p1Scaled = e->vert->v * 500;
					vec3 p2Scaled = e->v2()->v * 500;
					openEdgePoints.push_back(p1Scaled);
					openEdgePoints.push_back(p2Scaled);
					vec3 delta = p1Scaled - p2Scaled;
					double lengthDelta = delta.length();
					lengthOfEdges.push_back(lengthDelta);
					glm::vec4 color(1, 0.5 + 0.1*edgeCount, 1, 1);
					GeomDebugDump::dumpPolyline({ p1Scaled, p2Scaled }, color, 1, false, false);
					++edgeCount;
					GeomDebugDump::moveOffset(0.001);
				}
				GeomDebugDump::moveOffset(0.2);
				GeomDebugDump::dumpPolyline(openEdgePoints, color, 1, true, false);
				GeomDebugDump::moveOffset(0.08);
				

				std::vector<vec3 > vecPointLoop;
				for (auto e : edgeLoop)
				{
					vec3 p1Scaled = e->vert->v*1000;
					vec3 p2Scaled = e->v2()->v * 1000;
					vecPointLoop.push_back(p1Scaled);
					vec3 delta = p1Scaled - p2Scaled;
					double lengthDelta = delta.length();
					GeomDebugDump::dumpPolyline({ p1Scaled, p2Scaled }, color, 1, false, false);
					GeomDebugDump::moveOffset(0.01);
				}

				GeomDebugDump::dumpPolyline(vecPointLoop, color, 1, true, false);
#endif
			}

			if (m_mapVertexOpenEdges.size() < 2)
			{
				break;
			}
		}
	}

	void findEdgesByStartPoint(carve::mesh::Vertex<3>* vertex, std::set<carve::mesh::Edge<3>* >& result,
		carve::mesh::Edge<3>* excludeEdge, double epsMergePoints)
	{
		for (auto it : m_mapVertexOpenEdges)
		{
			std::set<carve::mesh::Edge<3>* >& openEdgesFromAndVertex = it.second;

			for (carve::mesh::Edge<3>*edge : openEdgesFromAndVertex)
			{
				if (edge == excludeEdge)
				{
					continue;
				}

				carve::mesh::Vertex<3>* vert = edge->v1();
				vec3 delt = vert->v - vertex->v.v;
				if (std::abs(delt.x) < epsMergePoints)
				{
					if (std::abs(delt.y) < epsMergePoints)
					{
						if (std::abs(delt.z) < epsMergePoints)
						{
							result.insert(edge);
						}
					}
				}
			}
		}
	}

	//void findEdgesByEndPoint(carve::mesh::Vertex<3>* vertex, std::vector<carve::mesh::Edge<3>* >& result,
	//	carve::mesh::Edge<3>* excludeEdge, double epsMergePoints)
	//{
	//	for (auto it : m_mapVertexOpenEdges)
	//	{
	//		carve::mesh::Vertex<3>* vert = it.first;
	//		vec3 delt = vert->v2() - vertex->v.v;
	//		if (std::abs(delt.x) < epsMergePoints)
	//		{
	//			if (std::abs(delt.y) < epsMergePoints)
	//			{
	//				if (std::abs(delt.z) < epsMergePoints)
	//				{
	//					std::set<carve::mesh::Edge<3>* >& openEdgesFromAndVertex = it.second;

	//					for (carve::mesh::Edge<3>*edge : openEdgesFromAndVertex)
	//					{
	//						if (edge == excludeEdge)
	//						{
	//							continue;
	//						}

	//						result.push_back(edge);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	carve::mesh::Edge<3>* findEdgeByStartAndEndPoint(carve::mesh::Vertex<3>* startVertex, carve::mesh::Vertex<3>* endVertex,
		carve::mesh::Edge<3>* excludeEdge, bool& edgeIsReversed, double epsMergePoints, 
		std::set<std::pair<carve::mesh::Vertex<3>*, carve::mesh::Vertex<3>*>>& closeButNotIdinticalVertices)
	{
		std::set<carve::mesh::Edge<3>* > edgesFromStartVertex;
		findEdgesByStartPoint(startVertex, edgesFromStartVertex, excludeEdge, epsMergePoints);

		for (carve::mesh::Edge<3>*edge : edgesFromStartVertex)
		{
			// v1() of edge matches startVertex already
			vec3 deltEndPoint = edge->v2()->v - endVertex->v.v;
			if (std::abs(deltEndPoint.x) < epsMergePoints)
			{
				if (std::abs(deltEndPoint.y) < epsMergePoints)
				{
					if (std::abs(deltEndPoint.z) < epsMergePoints)
					{
						edgeIsReversed = false;

						if (edge->v2() != endVertex)
						{
							closeButNotIdinticalVertices.insert({ edge->v2() , endVertex });
						}
						return edge;
					}
				}
			}
		}

		std::set<carve::mesh::Edge<3>* > edgesFromEndVertex;
		findEdgesByStartPoint(endVertex, edgesFromEndVertex, excludeEdge, epsMergePoints);

		for (carve::mesh::Edge<3>*edge : edgesFromEndVertex)
		{
			// v1() of edge matches endVertex already
#ifdef _DEBUG
			vec3 deltV1_to_EndVertex = edge->v1()->v - endVertex->v.v;

			glm::vec4 color(1, 0.5, 1, 1);

			GeomDebugDump::dumpPolyline({ edge->v1()->v, edge->v2()->v }, color, 1, true, false);
			GeomDebugDump::moveOffset(0.01);

#endif
			// now check if edge->v2() matches startVertex
			vec3 deltEndPoint = edge->v2()->v - startVertex->v.v;
			if (std::abs(deltEndPoint.x) < epsMergePoints)
			{
				if (std::abs(deltEndPoint.y) < epsMergePoints)
				{
					if (std::abs(deltEndPoint.z) < epsMergePoints)
					{
						edgeIsReversed = true;
						if (edge->v2() != endVertex)
						{
							closeButNotIdinticalVertices.insert({ edge->v2() , startVertex });
						}
						return edge;
					}
				}
			}
		}

		//		for (auto it : m_mapVertexOpenEdges)
		//		{
		//			carve::mesh::Vertex<3>* vert = it.first;
		//			vec3 delt = vert->v - startVertex->v.v;
		//			if (std::abs(delt.x) < epsMergePoints)
		//			{
		//				if (std::abs(delt.y) < epsMergePoints)
		//				{
		//					if (std::abs(delt.z) < epsMergePoints)
		//					{
		//						std::set<carve::mesh::Edge<3>* >& openEdgesFromAndVertex = it.second;
		//
		//						for (carve::mesh::Edge<3>*edge : openEdgesFromAndVertex)
		//						{
		//							if (edge == excludeEdge)
		//							{
		//								continue;
		//							}
		//							carve::mesh::Vertex<3>* vertOpposite = nullptr;
		//							if (edge->v1() == vert)  // v1() is the start vertex of the edge
		//							{
		//								vertOpposite = edge->v2();
		//							}
		//							else if (edge->v2() == vert)
		//							{
		//								vertOpposite = edge->v1();
		//							}
		//							else
		//							{
		//#ifdef _DEBUG
		//								std::cout << "incorrect edge in m_mapVertexOpenEdges" << std::endl;
		//#endif
		//								continue;
		//							}
		//
		//							vec3 deltEndPoint = vertOpposite->v - endVertex->v.v;
		//							if (std::abs(deltEndPoint.x) < epsMergePoints)
		//							{
		//								if (std::abs(deltEndPoint.y) < epsMergePoints)
		//								{
		//									if (std::abs(deltEndPoint.z) < epsMergePoints)
		//									{
		//										return edge;
		//									}
		//								}
		//							}
		//						}
		//					}
		//				}
		//			}
		//		}
		return nullptr;
	}
};

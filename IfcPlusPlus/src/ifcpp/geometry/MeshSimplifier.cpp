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

#pragma warning (disable: 4702)
#include <ifcpp/geometry/GeomDebugDump.h>
#include <ifcpp/geometry/MeshOps.h>
#include "PolyInputCache3D.h"
#include "MeshSimplifier.h"

void MeshSimplifier::simplifyMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& paramsInput)
{
	if (!meshsetInput)
	{
		return;
	}

	printToDebugLog(__FUNC__, "");
	GeomProcessingParams params(paramsInput);
	StatusCallback* report_callback = params.callbackFunc;
	BuildingEntity* entity = params.ifc_entity;
	bool dumpPolygon = params.debugDump;
	double eps = params.epsMergePoints;
	MeshSetInfo infoInput(report_callback, entity);
	bool validMeshsetInput = MeshOps::checkMeshSetValidAndClosed(meshsetInput, infoInput, params);
	bool deepCopyMesh = true;

	if (meshsetInput->vertex_storage.size() < 9
		&& infoInput.openEdges.size() == 0
		&& infoInput.degenerateEdges.size() == 0
		&& infoInput.zeroAreaFaces.size() == 0
		&& infoInput.meshSetValid)
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
	MeshOps::checkMeshSetValidAndClosed(meshset, info, params);

	bool useCarveMeshSimplifier = false;
	if (useCarveMeshSimplifier)
	{
		try
		{
			size_t numChanges = 0;
			carve::mesh::MeshSimplifier carveSimplifier(params.epsMergePoints);
			//numChanges += carveSimplifier.removeFins(meshset.get());
			//numChanges += carveSimplifier.mergeCoplanarFaces(meshset.get(), params.epsMergeAlignedEdgesAngle);
			numChanges += carveSimplifier.improveMesh_conservative(meshset.get(), params.epsMergePoints);

			//MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, info, infoInput, false, params);

			double min_deltaV = params.epsMergeAlignedEdgesAngle;
			//numChanges += carveSimplifier.improveMesh(meshset.get(), params.epsMergeAlignedEdgesAngle, min_deltaV, params.epsMergeAlignedEdgesAngle, params.epsMergePoints );
			if (numChanges > 0)
			{
#if defined(_DEBUG) || defined(_DEBUG_RELEASE)
				MeshSetInfo infoChanges(report_callback, entity);
				MeshOps::checkMeshSetValidAndClosed(meshset, infoChanges, params);
				if (MeshOps::isBetterForBoolOp(infoChanges, info, false))
				{
					std::cout << "carve::mesh::MeshSimplifier: numChanges: " << numChanges << std::endl;
				}
#endif
			}
		}
		catch (carve::exception& e)
		{
#ifdef _DEBUG
			std::cout << e.str() << std::endl;
#endif
		}

		MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, info, infoInput, false, params, deepCopyMesh);
	}

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

		vec4 color1(0.7, 0.7, 0.7, 0.88);
		std::string labelStr = "simplify--merged-faces";
		GeomDebugDump::dumpVertex(GeomDebugDump::DumpData::instance().labelPos, color1, labelStr);
		GeomDebugDump::dumpCountLabel(GeomDebugDump::DumpData::instance().countLabelPos);
	}

	if (GeomDebugDump::getDumpCount() >= 16)
	{
		int wait = 0;
	}
	shared_ptr<carve::mesh::MeshSet<3> > meshset_copy_input(meshset->clone());
#endif

	if (infoInput.zeroAreaFaces.size() > 0)
	{
		if (infoInput.openEdges.size() == 0)
		{
			removeDegenerateFacesInMeshSet(meshset, params);
			MeshOps::checkMeshSetValidAndClosed(meshset, info, params);
			MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, info, infoInput, true, params, deepCopyMesh);
		}
	}

	if (!info.meshSetValid)
	{
		MeshOps::resolveOpenEdges(meshset, params);
		MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, info, infoInput, false, params, deepCopyMesh);

		if (!info.meshSetValid)
		{
			MeshOps::resolveOpenEdges(meshset, params);
			MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, info, infoInput, false, params, deepCopyMesh);
		}
	}

	dumpPolygon = false;

	try
	{
		removeDegenerateFacesInMeshSet(meshset, params);

		const clock_t begin_time = clock();
		bool mergedFacesMeshShouldBeClosed = false;  // allow open edges, will try to sew them together later
		size_t numChanges = mergeCoplanarFacesInMeshSet(meshset, params, mergedFacesMeshShouldBeClosed);
		int millisecs = (clock() - begin_time);
		if (millisecs > 5 && numChanges == 0)
		{
#if defined _DEBUG || defined _DEBUG_RELEASE
			std::cout << "no success in mergeCoplanarFacesInMeshSet, vertex count: " << meshset->vertex_storage.size() << std::endl;
#endif
		}

		size_t numEdgesRemoved = mergeAlignedEdges(meshset, params);

		MeshOps::recalcMeshSet(meshset, eps);

		// TODO: find faces with biggest area, and trim all points to plane

		MeshSetInfo infoMergedFaces(report_callback, entity);
		MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, infoMergedFaces, infoInput, false, params, deepCopyMesh);

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
			vec4 color1(0.7, 0.7, 0.7, 0.88);
			for (carve::mesh::Mesh<3>*mesh : meshset_dump->meshes)
			{
				GeomDebugDump::dumpFacePolygons(mesh->faces, color1, false);
			}
		}

#endif
		MeshOps::checkMeshSetValidAndClosed(meshset, infoMergedFaces, params);
		if (!infoMergedFaces.meshSetValid)
		{
			size_t retry_count = 0;
			MeshOps::retriangulateMeshSetForBoolOp(meshset, false, params, retry_count);

			MeshSetInfo infoRetriangulated(report_callback, entity);
			MeshOps::checkMeshSetValidAndClosed(meshset, infoRetriangulated, params);
		}

		MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, infoMergedFaces, infoInput, false, params, deepCopyMesh);

		MeshOps::recalcMeshSet(meshset, eps);
		MeshOps::checkMeshSetValidAndClosed(meshset, info, params);

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
				vec4 color1(0.7, 0.7, 0.7, 0.88);
				for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
				{
					GeomDebugDump::dumpFacePolygons(mesh->faces, color1, false);
				}

				GeomDebugDump::moveOffset(0.3);
			}
#endif
			MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, infoMergedAlignedEdges, infoInput, false, params, deepCopyMesh);

			if (validMergedAlignedEdges)
			{
				if (paramsInput.triangulateResult)
				{
					MeshOps::retriangulateMeshSetForBoolOp(meshset, false, params, 0);
					MeshSetInfo infoTriangulated(report_callback, entity);
					bool validTriangulated = MeshOps::checkMeshSetValidAndClosed(meshset, infoTriangulated, params);

					MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, infoTriangulated, infoInput, true, params, deepCopyMesh);
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

		MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, infoMergedFaces, infoInput, true, params, deepCopyMesh);

		MeshOps::checkMeshSetValidAndClosed(meshset, infoMergedFaces, params);
		MeshOps::checkAndFixMeshsetInverted(meshset, infoMergedFaces, params);

		MeshOps::assignIfBetterForBoolOp(meshset, meshsetInput, infoMergedFaces, infoInput, true, params, deepCopyMesh);

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

size_t MeshSimplifier::mergeCoplanarFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& paramsInput, bool shouldBeClosedManifold)
{
	if (!meshset)
	{
		return 0;
	}

	if (meshset->vertex_storage.size() > 5000)
	{
		return 0;
	}

	shared_ptr<carve::mesh::MeshSet<3> > meshset_copy(meshset->clone());
	GeomProcessingParams params(paramsInput);
	params.allowFinEdges = false;
	double epsAngleMergeEdges = params.epsMergeAlignedEdgesAngle * 10;
	double eps = params.epsMergePoints;

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

			// re-compute face normal here
			face->computeNormal(eps);
			adjacentFace->computeNormal(eps);
			const vec3 faceNormal = face->plane.N;
			const vec3 face2Normal = adjacentFace->plane.N;

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


size_t MeshSimplifier::mergeAlignedEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, GeomProcessingParams& params)
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

								std::unordered_set<carve::mesh::Edge<3>* > setEdges;
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
								vec4 color1(0.4, 0.45, 0.45, 1.);
								if (params.debugDump)
								{
									for (auto edgeOnVertex : setEdges)
									{
										const carve::geom::vector<3>& p1 = edgeOnVertex->v1()->v;
										const carve::geom::vector<3>& p2 = edgeOnVertex->v2()->v;
										std::vector<carve::geom::vector<3> > vecLine = { p1, p2 };
										GeomDebugDump::dumpPolyline(vecLine, color1, 0, false, false);
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
										if (edge->rev && edge->prev->rev)
										{
											std::vector<const carve::mesh::Face<3>* > vecAdjacentFaces = { edge->face, edge->rev->face, edgeRemove->face, edgeRemove->rev->face, edge->prev->face, edge->prev->rev->face };
											GeomDebugDump::moveOffset(0.05);
											GeomDebugDump::dumpFaces(vecAdjacentFaces, color1, false);
											GeomDebugDump::dumpFacePolygons(vecAdjacentFaces, color1, false);
										}
									}
#endif

									std::unordered_set<carve::mesh::Edge<3>* > setEdgePointersToRemovedEdge;
									std::unordered_set<carve::mesh::Face<3>* > setFacePointersToRemovedEdge;
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
										vec4 color(0.4, 0.45, 0.45, 1.);

										std::unordered_set<carve::mesh::Edge<3>* > setEdges1;
										getEdgesOnVertex(mesh, vertex1, setEdges1);

										std::unordered_set<carve::mesh::Edge<3>* > setEdges2;
										getEdgesOnVertex(mesh, vertex2, setEdges2);

										std::unordered_set<carve::mesh::Edge<3>* > setEdges3;
										getEdgesOnVertex(mesh, vertex3, setEdges3);


										for (auto edgeOnVertex : setEdges1)
										{
											const carve::geom::vector<3>& p1 = edgeOnVertex->v1()->v;
											const carve::geom::vector<3>& p2 = edgeOnVertex->v2()->v;
											std::vector<carve::geom::vector<3> > vecLine = { p1, p2 };
											GeomDebugDump::dumpPolyline(vecLine, color1, 0, false, false);
										}


										for (auto edgeOnVertex : setEdges2)
										{
											const carve::geom::vector<3>& p1 = edgeOnVertex->v1()->v;
											const carve::geom::vector<3>& p2 = edgeOnVertex->v2()->v;
											std::vector<carve::geom::vector<3> > vecLine = { p1, p2 };
											GeomDebugDump::dumpPolyline(vecLine, color1, 0, false, false);
										}

										for (auto edgeOnVertex : setEdges3)
										{
											const carve::geom::vector<3>& p1 = edgeOnVertex->v1()->v;
											const carve::geom::vector<3>& p2 = edgeOnVertex->v2()->v;
											std::vector<carve::geom::vector<3> > vecLine = { p1, p2 };
											GeomDebugDump::dumpPolyline(vecLine, color1, 0, false, false);
										}

										GeomDebugDump::moveOffset(0.4);
										GeomDebugDump::dumpFacePolygons(mesh->faces, color1, false);

										GeomDebugDump::moveOffset(0.05);
										std::vector<carve::geom::vector<3> > edgePolygon = { p1,p2,p3 };
										GeomDebugDump::dumpPolyline(edgePolygon, color, 0, false, false);

										std::vector<carve::geom::vector<3> > edgePolygon2 = { edge->v1()->v, edge->v2()->v };
										GeomDebugDump::moveOffset(0.001);
										GeomDebugDump::dumpPolyline(edgePolygon2, color, 0, false, false);
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
				vec4 color(0.4, 0.45, 0.45, 1.);
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
					vec4 color(0.4, 0.45, 0.45, 1.);
					GeomDebugDump::dumpFacePolygon(face, color, false);
				}
			}
#endif
		}
	}

	return numEdgesRemoved;
}

size_t MeshSimplifier::removeEdgeAndMergeFaces(carve::mesh::Edge<3>* edgeIn, const GeomProcessingParams& params)
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
		vec4 color2(0.3, 0.2, 0.2, 0.8);
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
		vec4 color2(0.3, 0.2, 0.2, 0.8);
		std::vector<carve::geom::vector<3> > vecEdgePoints = { edgeErase->v1()->v, edgeErase->v2()->v };
		GeomDebugDump::dumpPolyline(vecEdgePoints, color2, 0, false, false);
	}
#endif

	MeshSetInfo info2;
	bool checkForDegenerateEdges = false;
	MeshOps::checkFaceIntegrity(edgeErase->face, checkForDegenerateEdges, info2, params.epsMergePoints);

	MeshSetInfo info3;
	MeshOps::checkFaceIntegrity(edgeErase->rev->face, checkForDegenerateEdges, info3, params.epsMergePoints);

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
		vec4 color2(0.3, 0.2, 0.2, 0.8);
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
#ifdef _DEBUG
		std::cout << "validateLoop failed: " << e.str();
#endif
	}

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
					//std::cout << "distanceToPlaneCheck too big: " << distanceToPlaneCheck << std::endl;
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
	MeshOps::checkMeshIntegrity(mesh, checkForDegenerateEdges, params, info6);
	if (!info6.allPointersValid || !allVerticesInPlane)
	{
		vec4 color(0.2, 0.2, 0.2, 1.);
		GeomDebugDump::stopBuffering();
		GeomDebugDump::moveOffset(0.3);
		GeomDebugDump::dumpMesh(mesh, color, true);
	}
#endif

	return numChanges;
}

void MeshSimplifier::removeDegenerateFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& paramsInput)
{
	return;


	if (!meshsetInput)
	{
		return;
	}

	GeomProcessingParams params(paramsInput);
	params.allowZeroAreaFaces = true;
	MeshSetInfo infoInput;
	bool meshInputOk = MeshOps::checkMeshSetValidAndClosed(meshsetInput, infoInput, params);

	std::unordered_set<const carve::mesh::Face<3>* > setFacesRemove = infoInput.degenerateFaces;
	std::unordered_set<carve::mesh::Edge<3>* > setEdgesRemove = infoInput.degenerateEdges;

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
	}

	if (setFacesRemove.size() > 0 || setEdgesRemove.size() > 0)
	{
		PolyInputCache3D polyInput(params.epsMergePoints);
		MeshOps::polyhedronFromMeshSet(meshsetInput, setFacesRemove, polyInput);

		int numFacesInput = infoInput.numFaces;
		std::string details;
		bool polyCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
		if (!polyCorrect)
		{
			fixPolyhedronData(polyInput.m_poly_data, params);
			polyCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
		}

		std::map<std::string, std::string> mesh_input_options;
		shared_ptr<carve::mesh::MeshSet<3> > resultFromPolyhedron(polyInput.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));
		MeshSetInfo info;
		MeshOps::checkMeshSetValidAndClosed(resultFromPolyhedron, info, params);
		int numFacesPoly = info.numFaces;
#ifdef _DEBUG
		if (meshInputOk && !info.meshSetValid && false)
		{
			GeomDebugDump::DumpSettingsStruct dumpColorSettings;
			GeomDebugDump::dumpWithLabel("removeDegenerateFacesInMeshSet--input", meshsetInput, dumpColorSettings, params, true, true);
			GeomDebugDump::dumpMeshsetOpenEdges(resultFromPolyhedron, dumpColorSettings.colorMesh, false, true);
			GeomDebugDump::dumpWithLabel("removeDegenerateFacesInMeshSet--result", resultFromPolyhedron, dumpColorSettings, params, true, true);
		}
#endif

		if (info.numOpenEdges() > 0)
		{
			MeshOps::resolveOpenEdges(resultFromPolyhedron, params);
			MeshOps::checkMeshSetValidAndClosed(resultFromPolyhedron, info, params);
		}

		if (MeshOps::isBetterForBoolOp(info, infoInput, true))
		{
			meshsetInput = resultFromPolyhedron;
		}
	}
}

void MeshSimplifier::removeFinFaces(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params)
{
	return;



	MeshSetInfo infoInput;
	MeshOps::checkMeshSetValidAndClosed(meshset, infoInput, params);

	std::unordered_set<const carve::mesh::Face<3>* > setFacesRemove;
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

			// re-compute face normal here
			face->computeNormal(params.epsMergePoints);
			adjacentFace->computeNormal(params.epsMergePoints);
			const vec3 faceNormal = face->plane.N;
			const vec3 face2Normal = adjacentFace->plane.N;

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
		MeshOps::polyhedronFromMeshSet(meshset, setFacesRemove, polyInput);

		std::string details;
		bool polyCorrect = checkPolyhedronData(polyInput.m_poly_data, params, details);
		if (!polyCorrect)
		{
			fixPolyhedronData(polyInput.m_poly_data, params);
		}

		std::map<std::string, std::string> mesh_input_options;
		shared_ptr<carve::mesh::MeshSet<3> > resultFromPolyhedron(polyInput.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));
		MeshSetInfo info;
		MeshOps::assignIfBetterForBoolOp(resultFromPolyhedron, meshset, info, infoInput, false, params, false);

		if (info.meshSetValid)
		{
#ifdef _DEBUG
			if (params.debugDump)
			{
				vec4 color(0.5, 0.5, 0.5, 1);
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

void MeshSimplifier::removeFinEdges(carve::mesh::Mesh<3>* mesh, const GeomProcessingParams& params)
{
	return;  // degenerate edge = degenerate face, so remove degenerate faces instead

	if (!mesh)
	{
		return;
	}

	const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
	size_t numFaces = vec_faces.size();

#ifdef _DEBUG
	if (numFaces == 1 && false)
	{
		vec4 color(0.4, 0.2, 0.2, 1.);
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
			MeshOps::checkMeshIntegrity(mesh, checkForDegenerateEdges, params, minf);

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

carve::mesh::Edge<3>* MeshSimplifier::checkMergeFaces(carve::mesh::Edge<3>* e, const GeomProcessingParams& params)
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
		vec4 color(0.3, 0.3, 0.3, 1.);
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

	linkEdges(link2_n->prev, link1_p->next);
	linkEdges(link1_n->prev, link2_p->next);

	linkEdges(link1_p, link1_n);
	linkEdges(link2_p, link2_n);

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

	setFacePointerToEdgeLoop(left_loop, nullptr);
	setFacePointerToEdgeLoop(left_loop->rev, nullptr);

#ifdef _DEBUG
	GeomDebugDump::clearBuffer();
#endif

	return left_loop;
}

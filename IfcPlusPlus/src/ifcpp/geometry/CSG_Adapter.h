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

#include <ifcpp/geometry/GeometryException.h>
#include <ifcpp/geometry/GeomDebugDump.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/StatusCallback.h>

#include "IncludeCarveHeaders.h"
#include "MeshNormalizer.h"
#include "MeshOps.h"
#include "GeometryInputData.h"

#if defined(_DEBUG) || defined(_DEBUG_RELEASE)
static int csg_compute_count = 0;
#define CSG_DEBUG
#endif

class CSG_Adapter
{
public:
	static void mergeMeshesToMeshset(std::vector<carve::mesh::Mesh<3>*>& meshes, shared_ptr<carve::mesh::MeshSet<3> >& result, GeomProcessingParams& params)
	{
		for (size_t ii = 0; ii < meshes.size(); ++ii)
		{
			carve::mesh::Mesh<3>* meshToMerge = meshes[ii];
			if (meshToMerge->is_negative)
			{
				meshToMerge->invert();
			}

			PolyInputCache3D polyInput1(params.epsMergePoints);
			MeshOps::polyhedronFromMesh(meshToMerge, polyInput1);

			std::map<std::string, std::string> mesh_input_options;
			shared_ptr<carve::mesh::MeshSet<3> > currentMeshAsMeshset(polyInput1.m_poly_data->createMesh(mesh_input_options, params.epsMergePoints));

			bool isClosed = currentMeshAsMeshset->isClosed();

			if (!result)
			{
				result = currentMeshAsMeshset;
				continue;
			}

			GeomProcessingParams paramsCurrentMesh(params);
			paramsCurrentMesh.debugDump = false;
			MeshSetInfo infoResult(params.callbackFunc, params.ifc_entity);
			MeshOps::simplifyMeshSet(currentMeshAsMeshset, infoResult, paramsCurrentMesh);

			MeshOps::checkMeshSetNonNegativeAndClosed(result, params);

#ifdef CSG_DEBUG
			glm::vec4 color(0.2, 0.2, 0.2, 1.);
			if (params.debugDump)
			{
				GeomDebugDump::moveOffset(0.5);
				GeomDebugDump::dumpMeshset(result.get(), color, true, false);
				GeomDebugDump::dumpMeshset(currentMeshAsMeshset.get(), color, true, false);
			}
#endif

			carve::csg::CSG csg(params.epsMergePoints);
			shared_ptr<carve::mesh::MeshSet<3> > resultMerge(csg.compute(result.get(), currentMeshAsMeshset.get(), carve::csg::CSG::UNION, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
			if (resultMerge)
			{
#ifdef CSG_DEBUG
				if (params.debugDump)
				{
					GeomDebugDump::moveOffset(0.5);
					GeomDebugDump::dumpMeshset(resultMerge.get(), color, true, false);
				}
#endif
				GeomProcessingParams paramsResult(params);
				paramsResult.debugDump = false;
				MeshSetInfo infoResult(params.callbackFunc, params.ifc_entity);
				MeshOps::simplifyMeshSet(resultMerge, infoResult, paramsResult);
				
				if (infoResult.meshSetValid)
				{
					result = resultMerge;
				}
			}
		}
	}

	static void assignResultOnFail(const shared_ptr<carve::mesh::MeshSet<3> >& op1, const shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result)
	{
		if( operation == carve::csg::CSG::A_MINUS_B )
		{
			result = op1;
		}
		else if( operation == carve::csg::CSG::B_MINUS_A )
		{
			result = op2;
		}
		else if( operation == carve::csg::CSG::UNION )
		{
			result = op1;
		}
	}

	static bool checkBoundinbBoxIntersection(const carve::geom::aabb<3>& bbox1, const carve::geom::aabb<3>& bbox2, const carve::csg::CSG::OP operation, double eps)
	{
		if( operation == carve::csg::CSG::UNION )
		{
			// union operation needs to be done also when there is no intersection
			return true;
		}

		bool bbox_direct_intersects = bbox1.intersects(bbox2, eps);
		if( bbox_direct_intersects )
		{
			return true;
		}

		double deltBbox = bbox1.maxAxisSeparation(bbox2);
		if (deltBbox < 0)
		{
			return true;
		}

		if (deltBbox > eps)
		{
			return false;
		}
		
		return true;
	}

	static bool computeCSG_Carve(const shared_ptr<carve::mesh::MeshSet<3> >& inputA, const shared_ptr<carve::mesh::MeshSet<3> >& inputB, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result,  
		GeomProcessingParams& params, bool normalizeCoords, bool allowDegenerateEdges)
	{
		if( !inputA || !inputB )
		{
			assignResultOnFail(inputA, inputB, operation, result);
			return false;
		}

		if( inputA->vertex_storage.size() > 4000 )
		{
			assignResultOnFail(inputA, inputB, operation, result);
			return false;
		}
		
		if( inputB->vertex_storage.size() > 4000 )
		{
			assignResultOnFail(inputA, inputB, operation, result);
			return false;
		}

		carve::geom::aabb<3> bboxA = inputA->getAABB();
		carve::geom::aabb<3> bboxB = inputB->getAABB();
		CarveMeshNormalizer normMesh(bboxA, bboxB, normalizeCoords);
		normMesh.m_disableNormalizeAll = false;

		int tag = -1;
		if (params.ifc_entity)
		{
			tag = params.ifc_entity->m_tag;
		}

		GeomProcessingParams paramsUnscaled(params);
		shared_ptr<GeometrySettings> geomSettingsLocal(new GeometrySettings(params.generalSettings));
		double scale = normMesh.getScale();
		double epsDefault = geomSettingsLocal->getEpsilonMergePoints();
		if (!normalizeCoords)
		{
			scale = 1.0;

			// if scale < 1.0, then epsilon needs to be smaller too
			double epsMinFaceAreaDefault = geomSettingsLocal->getMinTriangleArea();
			double epsMinFaceArea = epsMinFaceAreaDefault * (scale * scale);
			geomSettingsLocal->setEpsilonMergePoints(epsDefault * scale);
			geomSettingsLocal->setMinTriangleArea(epsMinFaceArea);
		}
		else
		{
			epsDefault = 1.5 * EPS_M8;
			geomSettingsLocal->setEpsilonMergePoints(epsDefault);
			geomSettingsLocal->setMinTriangleArea(epsDefault*0.00001);
		}
		GeomProcessingParams paramsScaled(geomSettingsLocal, false);
		
		bool intersecting = checkBoundinbBoxIntersection(bboxA, bboxB, operation, epsDefault);
		if (!intersecting)
		{
			assignResultOnFail(inputA, inputB, operation, result);
			return true;
		}

		size_t numDegenerateFacesA = MeshOps::countDegeneratedFaces(inputA.get());
		size_t numDegenerateFacesB = MeshOps::countDegeneratedFaces(inputB.get());
		if (numDegenerateFacesA > 0)
		{
			assignResultOnFail(inputA, inputB, operation, result);
			return true;
		}
		if (numDegenerateFacesB > 0)
		{
			assignResultOnFail(inputA, inputB, operation, result);
			return true;
		}

		if( inputA == inputB)
		{
			assignResultOnFail(inputA, inputB, operation, result);
			return true;
		}

		MeshSetInfo infoResult(params.callbackFunc, params.ifc_entity);
		MeshSetInfo infoInputA(params.callbackFunc, params.ifc_entity);
		MeshSetInfo infoInputB(params.callbackFunc, params.ifc_entity);
		paramsScaled.allowFinEdges = false;
		paramsUnscaled.allowFinEdges = false;
		MeshOps::checkMeshSetValidAndClosed(inputA, infoInputA, paramsScaled);
		MeshOps::checkMeshSetValidAndClosed(inputB, infoInputB, paramsScaled);

		shared_ptr<carve::mesh::MeshSet<3> > op1(inputA->clone());
		shared_ptr<carve::mesh::MeshSet<3> > op2(inputB->clone());
	
		std::stringstream strs_err;
		try
		{
			// normalize first, so that EPS values match the size of different meshes
			normMesh.normalizeMesh(op1, "op1", epsDefault);
			normMesh.normalizeMesh(op2, "op2", epsDefault);

			MeshOps::flattenFacePlanes(op1, op2, paramsScaled);

			paramsScaled.triangulateResult = true;
			paramsScaled.shouldBeClosedManifold = true;
			paramsScaled.allowDegenerateEdges = allowDegenerateEdges;

			MeshSetInfo infoOp1(params.callbackFunc, params.ifc_entity);
			MeshSetInfo infoOp2(params.callbackFunc, params.ifc_entity);
			MeshOps::simplifyMeshSet(op1, infoOp1, paramsScaled);
			MeshOps::simplifyMeshSet(op2, infoOp2, paramsScaled);

			double volumeOp1 = MeshOps::computeMeshsetVolume(op1.get());
			double volumeOp2 = MeshOps::computeMeshsetVolume(op2.get());

			if (infoOp1.finEdges.size() > 0)
			{
				paramsScaled.allowFinEdges = true;
				paramsUnscaled.allowFinEdges = true;
			}

			if (infoOp2.finEdges.size() > 0)
			{
				paramsScaled.allowFinEdges = true;
				paramsUnscaled.allowFinEdges = true;
			}

			if (infoInputA.zeroAreaFaces.size() > 0)
			{
				paramsScaled.allowZeroAreaFaces = true;  // temp
				paramsUnscaled.allowZeroAreaFaces = true;  // temp
			}

#ifdef CSG_DEBUG
			++csg_compute_count;
			GeomDebugDump::DumpSettingsStruct dumpColorSettings;
			GeomDebugDump::DumpData::instance().maxDumpCount = 1000;
			bool operandA_dumped = false;
			bool operandB_dumped = false;
			bool dump_result_mesh = false;

			if (csg_compute_count > 0 )
			{
				if (14700322 == tag || !infoInputA.meshSetValid)
				{
					paramsScaled.debugDump = true;
					MeshSetInfo infoMeshOp1;
					MeshOps::checkMeshSetValidAndClosed(op1, infoMeshOp1, params);

					//MeshOps::simplifyMeshSet(op1, infoOp1, params);
					GeomDebugDump::dumpMeshset(op1, dumpColorSettings.colorMesh, false, true);
					GeomDebugDump::dumpMeshsetOpenEdges(op1, dumpColorSettings.colorMesh, false, false);
					std::vector<const carve::mesh::Face<3>* > vecDegenerateEdgeFaces;
					for (auto e : infoOp1.degenerateEdges)
					{
						size_t n_edges = e->face->n_edges;
						size_t n_edges_reverseFace = e->rev->face->n_edges;
						vecDegenerateEdgeFaces.push_back(e->face);
					}
					GeomDebugDump::dumpFaces(vecDegenerateEdgeFaces, dumpColorSettings.colorMesh, true);

					dump_result_mesh = true;
					//paramsScaled.debugDump = true;
					dumpOperands(op1, op2, result, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
				}

				if (infoInputA.finEdges.size() > 0 || infoInputA.finFaces.size() > 0)
				{
					for (auto mesh : inputA->meshes)
					{
						const std::vector<carve::mesh::Face<3>* >& vecFaces = mesh->faces;
						GeomDebugDump::dumpFacePolygons(vecFaces, dumpColorSettings.colorMesh, false);
					}
					std::vector<const carve::mesh::Face<3>* > vecFinFaces;
					std::vector<carve::mesh::Edge<3>* > vecFinEdges;
					std::copy(infoInputA.finFaces.begin(), infoInputA.finFaces.end(), std::back_inserter(vecFinFaces));
					std::copy(infoInputA.finEdges.begin(), infoInputA.finEdges.end(), std::back_inserter(vecFinEdges));

					GeomDebugDump::dumpEdges(vecFinEdges);
					GeomDebugDump::dumpFaces(vecFinFaces, dumpColorSettings.colorMesh, true);
					GeomDebugDump::moveOffset(1.5);
				}


				MeshSetInfo infoMesh1_debug(params.callbackFunc, params.ifc_entity);
				bool operand1Valid_debug = MeshOps::checkMeshSetValidAndClosed(op1, infoMesh1_debug, paramsScaled);
				if (infoMesh1_debug.finEdges.size() > 0 || infoMesh1_debug.finFaces.size() > 0)
				{
					for (auto mesh : op1->meshes)
					{
						const std::vector<carve::mesh::Face<3>* >& vecFaces = mesh->faces;
						GeomDebugDump::dumpFacePolygons(vecFaces, dumpColorSettings.colorMesh, false);
					}
					std::vector<const carve::mesh::Face<3>* > vecFinFaces;
					std::vector<carve::mesh::Edge<3>* > vecFinEdges;
					std::copy(infoMesh1_debug.finFaces.begin(), infoMesh1_debug.finFaces.end(), std::back_inserter(vecFinFaces));
					std::copy(infoMesh1_debug.finEdges.begin(), infoMesh1_debug.finEdges.end(), std::back_inserter(vecFinEdges));

					GeomDebugDump::dumpEdges(vecFinEdges);
					GeomDebugDump::dumpFaces(vecFinFaces, dumpColorSettings.colorMesh, true);
					GeomDebugDump::moveOffset(0.5);
					dump_result_mesh = true;
				}
			}

			dumpColorSettings.eps = epsDefault;
			CarveMeshNormalizer normMesh_scaleMeshDump(normMesh);
			if (!normalizeCoords)
			{
				normMesh_scaleMeshDump.m_normalizeCoordsInsteadOfEpsilon = true;
				paramsScaled.normalizer = &normMesh_scaleMeshDump;
				paramsUnscaled.normalizer = &normMesh_scaleMeshDump;
			}

			if (infoInputA.zeroAreaFaces.size() > 0)
			{
				dumpOperands(op1, op2, result, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
			}
#endif

			if (!infoOp1.meshSetValid)
			{
				op1 = shared_ptr<carve::mesh::MeshSet<3> >(inputA->clone());
				MeshSetInfo infoMesh1copy(params.callbackFunc, params.ifc_entity);
				MeshOps::checkMeshSetValidAndClosed(op1, infoMesh1copy, paramsUnscaled);

				normMesh.normalizeMesh(op1, "op1_copy", epsDefault);
				MeshOps::checkMeshSetValidAndClosed(op1, infoOp1, paramsScaled);
				if (!infoOp1.meshSetValid && infoInputA.meshSetValid)
				{
					// normalizing changed the validity, should not happen
#ifdef CSG_DEBUG
					//dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsScaled);
					double vol1 = MeshOps::computeMeshsetVolume(op1.get());
#endif
				}
			}

			if (!infoOp2.meshSetValid)
			{
				op2 = shared_ptr<carve::mesh::MeshSet<3> >(inputB->clone());
				MeshSetInfo infoMesh2copy(params.callbackFunc, params.ifc_entity);
				bool operand2copy_valid = MeshOps::checkMeshSetValidAndClosed(op2, infoMesh2copy, paramsUnscaled);

				normMesh.normalizeMesh(op2, "op2_copy", epsDefault);
				MeshOps::checkMeshSetValidAndClosed(op2, infoOp2, paramsScaled);
				if (!infoOp2.meshSetValid && infoInputB.meshSetValid)
				{
					// normalizing changed the validity, should not happen
#ifdef CSG_DEBUG

					dumpOperands(op1, op2, result, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
					double vol2 = MeshOps::computeMeshsetVolume(op2.get());
#endif
				}
			}

			if( !infoOp1.meshSetValid || !infoOp2.meshSetValid)
			{
				assignResultOnFail(inputA, inputB, operation, result);
				return false;
			}

			size_t numFacesOp1 = MeshOps::countFaces(op1.get());
			if (op1->vertex_storage.size() < 4 || numFacesOp1 < 4)
			{
				assignResultOnFail(inputA, inputB, operation, result);
				return false;
			}

			size_t numFacesOp2 = MeshOps::countFaces(op2.get());
			if (op2->vertex_storage.size() < 4 || numFacesOp2 < 4)
			{
				assignResultOnFail(inputA, inputB, operation, result);
				return false;
			}

			//paramsScaled.allowDegenerateEdges = true;
			if (infoOp1.degenerateEdges.size() > 0) { paramsScaled.allowDegenerateEdges = true; }
			if (infoOp2.degenerateEdges.size() > 0) { paramsScaled.allowDegenerateEdges = true; }

			////////////////////// compute carve csg operation   /////////////////////////////////////////////
			bool boolOpDone = false;
			if (op1->meshes.size() > 1 && operation == carve::csg::CSG::A_MINUS_B )
			{
				handleInnerOuterMeshesInOperands(op1, op2, result, paramsScaled, boolOpDone, epsDefault);
			}
			
			if( !boolOpDone )
			{
				carve::csg::CSG csg(epsDefault);
				result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
			}

			paramsScaled.allowFinEdges = true;
			MeshOps::checkMeshSetValidAndClosed(result, infoResult, paramsScaled);

			if (!infoResult.meshSetValid || (infoResult.meshSetValid && infoResult.degenerateEdges.size() > 0 ))
			{
#ifdef CSG_DEBUG
				//if ( paramsScaled.debugDump || csg_compute_count > 14)
				{
					dumpOperands(op1, op2, result, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
					double volume_result = MeshOps::computeMeshsetVolume(result.get());
				}
#endif

				MeshOps::simplifyMeshSet(result, infoResult, paramsScaled);
				MeshOps::removeDegenerateMeshes(result, paramsScaled, true);
				MeshOps::checkMeshSetValidAndClosed(result, infoResult, paramsScaled);
			}

#ifdef CSG_DEBUG
			if (!infoResult.meshSetValid|| dump_result_mesh)
			{
				if (paramsScaled.debugDump)
				{
					dumpOperands(op1, op2, result, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
				}
			}
#endif

			if (!infoResult.meshSetValid || (infoResult.meshSetValid && infoResult.degenerateEdges.size() > 0))
			{
				carve::csg::CSG csg(epsDefault);
				shared_ptr<carve::mesh::MeshSet<3> > resultClassifyNormal(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_NORMAL));

				MeshSetInfo infoResultClassifyNormal;
				MeshOps::checkMeshSetValidAndClosed(resultClassifyNormal, infoResultClassifyNormal, paramsScaled);

				if (!infoResultClassifyNormal.meshSetValid || (infoResultClassifyNormal.meshSetValid && infoResultClassifyNormal.degenerateEdges.size() > 0))
				{
#ifdef CSG_DEBUG
					if (paramsScaled.debugDump)
					{
						dumpOperands(op1, op2, resultClassifyNormal, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
					}
#endif

					MeshOps::simplifyMeshSet(resultClassifyNormal, infoResultClassifyNormal, paramsScaled);
					MeshOps::checkMeshSetValidAndClosed(resultClassifyNormal, infoResultClassifyNormal, paramsScaled);

					if (MeshOps::isBetterForBoolOp(infoResultClassifyNormal, infoResult, false))
					{
						result = resultClassifyNormal;
					}
				}
			}

			
			if (operation == carve::csg::CSG::A_MINUS_B)
			{
				if (op1->meshes.size() == 1)
				{
					// volume of result mesh:  volumeOp1-volumeOp2 >= volumeResult  >=  volumeOp1
					double volumeResult = MeshOps::computeMeshsetVolume(result.get());
					double expectedMinVolume = volumeOp1 - volumeOp2;
					double deltaExpectedVolume = volumeResult - expectedMinVolume*0.99;  // leave 1% margin for mesh inaccuracies
					if (deltaExpectedVolume < 0)
					{
#ifdef CSG_DEBUG
						if (volumeResult < 0.0002)
						{
							//std::cout << "empty?" << std::endl;
							dumpOperands(op1, op2, result, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
						}
#endif

						result = op1;
					}
				}
			}

			if( !infoResult.meshSetValid)
			{
				strs_err << "csg operation failed" << std::endl;

#ifdef CSG_DEBUG
				if (paramsScaled.debugDump)
				{
					dumpOperands(op1, op2, result, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
				}
#endif
			}
		}
		catch( carve::exception& ce )
		{
			strs_err << "csg operation failed" << ce.str().c_str();
		}
		catch( const std::out_of_range& oor )
		{
			strs_err << "csg operation failed" << oor.what();
		}
		catch( std::exception& e )
		{
			strs_err << "csg operation failed" << e.what();
		}
		catch( ... )
		{
			strs_err << "csg operation failed" << std::endl;
		}

		if( strs_err.tellp() > 0 )
		{
			if( !infoResult.meshSetValid)
			{
				assignResultOnFail(inputA, inputB, operation, result);
				return false;
			}
		}

#ifdef CSG_DEBUG
		MeshSetInfo infoMesh_beforeDeNormalize(params.callbackFunc, params.ifc_entity);
		bool result_meshset_ok_beforeDeNormalize = MeshOps::checkMeshSetValidAndClosed(result, infoMesh_beforeDeNormalize, paramsScaled);
#endif

		normMesh.deNormalizeMesh(result, "", epsDefault);

#ifdef CSG_DEBUG
		{
			// de-normalized:
			MeshSetInfo infoMesh1(params.callbackFunc, params.ifc_entity);
			bool result_valid_2 = MeshOps::checkMeshSetValidAndClosed(result, infoMesh1, paramsUnscaled);

			if (!result_valid_2)
			{
				std::cout << "!result" << std::endl;
			}
		}
#endif

		return infoResult.meshSetValid;
	}

	static void computeCSG(shared_ptr<carve::mesh::MeshSet<3> >& op1, const std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& operands2, const carve::csg::CSG::OP operation, GeomProcessingParams& params)
	{
		if( !op1 || operands2.size() == 0 )
		{
			return;
		}

		bool success = false;
		std::multimap<double, shared_ptr<carve::mesh::MeshSet<3> > > mapVolumeMeshes;
		for (const shared_ptr<carve::mesh::MeshSet<3> >&meshset2 : operands2)
		{
#ifdef _ORDER_CSG_BY_VOLUME
			double volume = MeshOps::computeMeshsetVolume(meshset2.get());
			mapVolumeMeshes.insert({ volume, meshset2 });
		}

		for (auto it : mapVolumeMeshes)
		{
			const shared_ptr<carve::mesh::MeshSet<3> >& meshset2 = it.second;
#endif
			bool normalizeCoords = true;
			bool allowDegenerateEdges = false;
			shared_ptr<carve::mesh::MeshSet<3> > result;
			success = computeCSG_Carve(op1, meshset2, operation, result, params, normalizeCoords, allowDegenerateEdges);

			if( success )
			{
				if( operation == carve::csg::CSG::A_MINUS_B || operation == carve::csg::CSG::UNION )
				{
					op1 = result;
				}

				continue;
			}

			allowDegenerateEdges = true;
			success = computeCSG_Carve(op1, meshset2, operation, result, params, normalizeCoords, allowDegenerateEdges);
			if( success )
			{
				if (operation == carve::csg::CSG::A_MINUS_B || operation == carve::csg::CSG::UNION)
				{
					op1 = result;
				}
				continue;
			}

			normalizeCoords = false;
			allowDegenerateEdges = false;
			success = computeCSG_Carve(op1, meshset2, operation, result, params, normalizeCoords, allowDegenerateEdges);
			if (success)
			{
				if (operation == carve::csg::CSG::A_MINUS_B || operation == carve::csg::CSG::UNION)
				{
#ifdef CSG_DEBUG
					std::cout << "csg success with !normalizeCoords" << std::endl;
#endif
					op1 = result;
				}
				continue;
			}

			allowDegenerateEdges = true;
			success = computeCSG_Carve(op1, meshset2, operation, result, params, normalizeCoords, allowDegenerateEdges);
			if (success)
			{
				if (operation == carve::csg::CSG::A_MINUS_B || operation == carve::csg::CSG::UNION)
				{
#ifdef CSG_DEBUG
					std::cout << "csg success with !normalizeCoords" << std::endl;
#endif
					op1 = result;
				}
				continue;
			}

		}
	}

	static void handleInnerOuterMeshesInOperands(shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, shared_ptr<carve::mesh::MeshSet<3> >& result, 
		GeomProcessingParams& params, bool& boolOpDone, double eps)
	{
		// if op1 consists of > 1 meshes, and one is completely inside the other, then all inner meshes need to be merged with op2, then op2 subtracted from the op1 outer mesh
		// if op1 consists of > 1 meshes, and they are apart from each other, then regular csg.compute works fine

		std::vector<carve::mesh::Mesh<3>*> innerMeshes;
		std::vector<carve::mesh::Mesh<3>*> outerMeshes;

		for (size_t iiMesh = 0; iiMesh < op1->meshes.size(); ++iiMesh)
		{
			carve::mesh::Mesh<3>* mesh1 = op1->meshes[iiMesh];

			if (mesh1->is_inner_mesh && iiMesh > 0)
			{
				innerMeshes.push_back(mesh1);
			}
			else
			{
				outerMeshes.push_back(mesh1);
			}
		}

		shared_ptr<carve::mesh::MeshSet<3> > resultClassified;
		if (outerMeshes.size() > 1)
		{
			// check if some outer meshes are in fact inner meshes
			if (innerMeshes.size() == 0)
			{
				MeshOps::classifyMeshesInside(outerMeshes, resultClassified, params);

				innerMeshes.clear();
				outerMeshes.clear();
				if (resultClassified)
				{
					for (size_t iiMesh = 0; iiMesh < resultClassified->meshes.size(); ++iiMesh)
					{
						carve::mesh::Mesh<3>* mesh1 = resultClassified->meshes[iiMesh];

						if (mesh1->is_inner_mesh && iiMesh > 0)
						{
							innerMeshes.push_back(mesh1);
						}
						else
						{
							outerMeshes.push_back(mesh1);
						}
					}
				}
			}
		}

		shared_ptr<carve::mesh::MeshSet<3> > op1OuterMeshset;
		mergeMeshesToMeshset(outerMeshes, op1OuterMeshset, params);

		shared_ptr<carve::mesh::MeshSet<3> > op2Meshset(op2->clone());
		mergeMeshesToMeshset(innerMeshes, op2Meshset, params);

#ifdef CSG_DEBUG
		if (csg_compute_count >= 9)
		{
			GeomDebugDump::moveOffset(0.4);
			//operandA_dumped = false;
			//operandB_dumped = false;
			//dumpOperands(op1OuterMeshset, op2Meshset, result, tag, operandA_dumped, operandB_dumped, dumpColorSettings, paramsScaled);
		}
		GeomDebugDump::DumpSettingsStruct dumpColorSettings;
		if (csg_compute_count == 12 && false)
		{
			GeomDebugDump::moveOffset(0.2);
			dumpWithLabel("computeCSG::op1", op1OuterMeshset, dumpColorSettings, params, true, false);
			dumpWithLabel("computeCSG::op2", op2, dumpColorSettings, params, false, false);
		}
#endif

		MeshSetInfo infoOuterMesh(params.callbackFunc, params.ifc_entity);
		MeshSetInfo infoInnerMesh(params.callbackFunc, params.ifc_entity);
		bool outerMesh_valid = MeshOps::checkMeshSetValidAndClosed(op1OuterMeshset, infoOuterMesh, params);
		bool innerMesh_valid = MeshOps::checkMeshSetValidAndClosed(op2Meshset, infoInnerMesh, params);

		if (outerMesh_valid && innerMesh_valid)
		{
			carve::csg::CSG csg(eps);
			shared_ptr<carve::mesh::MeshSet<3> > resultMerge(csg.compute(op1OuterMeshset.get(), op2Meshset.get(), carve::csg::CSG::A_MINUS_B, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
			if (resultMerge)
			{
				result = resultMerge;
				MeshSetInfo infoResult(params.callbackFunc, params.ifc_entity);
				params.allowDegenerateEdges = true;
				bool resultValid = MeshOps::checkMeshSetValidAndClosed(result, infoResult, params);
				if (resultValid)
				{
					boolOpDone = true;
				}
			}
		}
	}
};

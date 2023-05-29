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
#include "MeshOps.h"
#include "MeshOpsDebug.h"
#include "GeometryInputData.h"

//#define _USE_MANIFOLD_CSG
#ifdef _USE_MANIFOLD_CSG
#include "CSG_Manifold.h"
#endif

#ifdef _DEBUG
static int csg_compute_count = 0;
#endif
#define _USE_CLASSIFY_NORMAL

namespace CSG_Adapter
{
	inline void assignResultOnFail(const shared_ptr<carve::mesh::MeshSet<3> >& op1, const shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result)
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

	inline bool checkBoundinbBoxIntersection(const carve::geom::aabb<3>& bbox1, const carve::geom::aabb<3>& bbox2, const carve::csg::CSG::OP operation, double eps)
	{
		if( operation == carve::csg::CSG::UNION )
		{
			// union operation needs to be done also when there is no intersection
			return true;
		}

		double deltBbox = bbox1.maxAxisSeparation(bbox2);
		bool intersectsWithEpsilon = std::abs(deltBbox) > eps;
		bool intersects = intersectsWithEpsilon;
		return intersects;
	}

	inline bool computeCSG_Carve(const shared_ptr<carve::mesh::MeshSet<3> >& op1Orig, const shared_ptr<carve::mesh::MeshSet<3> >& op2Orig, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result,  
		shared_ptr<GeometrySettings>& geomSettingsDefault, StatusCallback* report_callback, const shared_ptr<BuildingEntity>& entity, bool normalizeCoords)
	{
		if( !op1Orig || !op2Orig )
		{
			assignResultOnFail(op1Orig, op2Orig, operation, result);
			return false;
		}

		if( op1Orig->vertex_storage.size() > 2000 )
		{
			assignResultOnFail(op1Orig, op2Orig, operation, result);
			return false;
		}
		
		if( op2Orig->vertex_storage.size() > 2000 )
		{
			assignResultOnFail(op1Orig, op2Orig, operation, result);
			return false;
		}
		
		
		carve::geom::aabb<3> bbox1 = op1Orig->getAABB();
		carve::geom::aabb<3> bbox2 = op2Orig->getAABB();
		bool result_meshset_ok = false;
		bool normalizeCoordsInsteadOfEpsilon = normalizeCoords;
		CarveMeshNormalizer normMesh(bbox1, bbox2, normalizeCoordsInsteadOfEpsilon);
		normMesh.m_disableNormalizeAll = false;

		GeomProcessingParams paramsUnscaled(geomSettingsDefault, false);
		shared_ptr<GeometrySettings> geomSettings(new GeometrySettings(geomSettingsDefault));
		double scale = normMesh.getScale();
		if (!normalizeCoords)
		{
			scale = 1.0;
		}
		double epsDefault = geomSettings->getEpsilonCoplanarDistance();
		double epsMinFaceAreaDefault = geomSettings->getMinTriangleArea();
		double CARVE_EPSILON = 10.0 * epsDefault * scale;
		double epsMinFaceArea = epsMinFaceAreaDefault * ( scale * scale );
		double epsCoplanarAngle = geomSettings->getEpsilonCoplanarAngle() * scale;
		double CARVE_EPSILON_restore = epsDefault;
		geomSettings->setEpsilonCoplanarDistance(CARVE_EPSILON);
		geomSettings->setEpsilonCoplanarAngle(epsCoplanarAngle);
		geomSettings->setMinTriangleArea(epsMinFaceArea);
		GeomProcessingParams paramsScaled(geomSettings, false);
		paramsScaled.ifc_entity = entity.get();
		paramsScaled.callbackFunc = report_callback;
		paramsUnscaled.ifc_entity = entity.get();
		paramsUnscaled.callbackFunc = report_callback;

		bool intersecting = checkBoundinbBoxIntersection(bbox1, bbox2, operation, CARVE_EPSILON);
		if (!intersecting)
		{
#ifdef _DEBUG
			shared_ptr<carve::mesh::MeshSet<3> > op1(op1Orig->clone());
			shared_ptr<carve::mesh::MeshSet<3> > op2(op2Orig->clone());
			int tag = 0;
			bool op1_dumped = false, op2_dumped = false;
			DumpSettingsStruct dumpColorSettings;
			dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsUnscaled);
#endif
			assignResultOnFail(op1Orig, op2Orig, operation, result);
			return true;
		}

		size_t numDegenerateFaces1 = MeshOps::countDegeneratedFaces(op1Orig.get());
		size_t numDegenerateFaces2 = MeshOps::countDegeneratedFaces(op2Orig.get());
		if (numDegenerateFaces1 > 0)
		{
			assignResultOnFail(op1Orig, op2Orig, operation, result);
			return true;
		}
		if (numDegenerateFaces2 > 0)
		{
			assignResultOnFail(op1Orig, op2Orig, operation, result);
			return true;
		}

		MeshSetInfo infoMesh1orig(report_callback, entity.get());
		MeshSetInfo infoMesh2orig(report_callback, entity.get());
		paramsScaled.allowFinEdges = false;
		paramsUnscaled.allowFinEdges = false;
		bool operand1origvalid = MeshOps::checkMeshSetValidAndClosed(op1Orig, infoMesh1orig, paramsScaled);
		bool operand2origvalid = MeshOps::checkMeshSetValidAndClosed(op2Orig, infoMesh2orig, paramsScaled);

		shared_ptr<carve::mesh::MeshSet<3> > op1(op1Orig->clone());
		shared_ptr<carve::mesh::MeshSet<3> > op2(op2Orig->clone());

	
		std::stringstream strs_err;
		try
		{
			// normalize first, so that EPS values match the size of different meshes
			normMesh.normalizeMesh(op1, "op1", CARVE_EPSILON);
			normMesh.normalizeMesh(op2, "op2", CARVE_EPSILON);

			bool op1_dumped = false;
			bool op2_dumped = false;
			int tag = entity->m_tag;
			MeshOps::flattenFacePlanes(op1, op2, paramsScaled);

#ifdef _DEBUG
			++csg_compute_count;
			DumpSettingsStruct dumpColorSettings;
			dumpColorSettings.eps = CARVE_EPSILON;
			if (!normalizeCoords)
			{
				dumpColorSettings.normalizer = &normMesh;
			}

			if (csg_compute_count == 24  || tag == 20896)
			{
				dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsScaled);
				//dumpMeshes = true;
			}
#endif

			bool triangulateOperands = true;
			bool shouldBeClosedManifold = true;
			MeshOps::simplifyMeshSet(op1, geomSettings, paramsScaled, triangulateOperands, shouldBeClosedManifold);
			MeshOps::simplifyMeshSet(op2, geomSettings, paramsScaled, triangulateOperands, shouldBeClosedManifold);

			MeshOps::retriangulateMeshSetSimple(op1, false, paramsScaled, 0);
			MeshOps::retriangulateMeshSetSimple(op2, false, paramsScaled, 0);

			MeshSetInfo infoMesh1(report_callback, entity.get());
			MeshSetInfo infoMesh2(report_callback, entity.get());
			bool operand1valid = MeshOps::checkMeshSetValidAndClosed(op1, infoMesh1, paramsScaled);
			bool operand2valid = MeshOps::checkMeshSetValidAndClosed(op2, infoMesh2, paramsScaled);

			if (infoMesh1.finEdges.size() > 0)
			{
				paramsScaled.allowFinEdges = true;
				paramsUnscaled.allowFinEdges = true;
			}
			if (infoMesh2.finEdges.size() > 0)
			{
				paramsScaled.allowFinEdges = true;
				paramsUnscaled.allowFinEdges = true;
			}

			if (!operand1valid && operand1origvalid)
			{
				op1 = shared_ptr<carve::mesh::MeshSet<3> >(op1Orig->clone());
				MeshSetInfo infoMesh1copy(report_callback, entity.get());
				bool operand1copy_valid = MeshOps::checkMeshSetValidAndClosed(op1, infoMesh1copy, paramsUnscaled);

				normMesh.normalizeMesh(op1, "op1orig", CARVE_EPSILON);
				operand1valid = MeshOps::checkMeshSetValidAndClosed(op1, infoMesh1, paramsScaled);
				if (!operand1valid && operand1origvalid)
				{
					// normalizing changed the validity, should not happen
#ifdef _DEBUG

					dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsScaled);
					double vol2 = MeshOps::computeMeshsetVolume(op2.get());
#endif
				}
			}

			if( !operand1valid || !operand2valid )
			{
				assignResultOnFail(op1Orig, op2Orig, operation, result);
				return false;
			}

			size_t numFacesOp1 = MeshOps::countFaces(op1.get());
			if (op1->vertex_storage.size() < 4 || numFacesOp1 < 4)
			{
				assignResultOnFail(op1Orig, op2Orig, operation, result);
				return false;
			}

			size_t numFacesOp2 = MeshOps::countFaces(op2.get());
			if (op2->vertex_storage.size() < 4 || numFacesOp2 < 4)
			{
				assignResultOnFail(op1Orig, op2Orig, operation, result);
				return false;
			}

			paramsScaled.allowDegenerateEdges = false;
			if (infoMesh1.degenerateEdges.size() > 0) { paramsScaled.allowDegenerateEdges = true; }
			if (infoMesh2.degenerateEdges.size() > 0) { paramsScaled.allowDegenerateEdges = true; }

			////////////////////// compute carve csg operation   /////////////////////////////////////////////
			carve::csg::CSG csg(CARVE_EPSILON);
			result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));

			MeshSetInfo infoResult( report_callback, entity.get() );
			result_meshset_ok = MeshOps::checkMeshSetValidAndClosed(result, infoResult, paramsScaled);

			if (!result_meshset_ok )
			{
#ifdef _DEBUG
				
				dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsScaled);
				double vol2 = MeshOps::computeMeshsetVolume(op2.get());
#endif
				MeshOps::fixMeshset(result, geomSettings, paramsScaled.debugDump);
				result_meshset_ok = MeshOps::checkMeshSetValidAndClosed(result, infoResult, paramsScaled);
			}

#ifdef _DEBUG
			if (csg_compute_count > 15)
			{
				double vol2 = MeshOps::computeMeshsetVolume(op2.get());
				dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsScaled);
			}
#endif

			// TODO: check for fail with closed mesh, but not fully sliced through.

#ifdef _USE_CLASSIFY_NORMAL
			if (!result_meshset_ok)
			{
				carve::csg::CSG csg(CARVE_EPSILON);
				result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_NORMAL));
				result_meshset_ok = MeshOps::checkMeshSetValidAndClosed(result, infoResult, paramsScaled);

				if (!result_meshset_ok)
				{
#ifdef _DEBUG
					dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsScaled);
#endif
					MeshOps::fixMeshset(result, geomSettings, paramsScaled.debugDump);
					result_meshset_ok = MeshOps::checkMeshSetValidAndClosed(result, infoResult, paramsScaled);
				}
			}
#endif

#ifdef _DEBUG
			double vol = MeshOps::computeMeshsetVolume(result.get());
			if (vol < 0.0002)
			{
				std::cout << "empty?" << std::endl;
				dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsScaled);
			}
#endif

			if( !result_meshset_ok )
			{
				//dumpMeshes = true;

#ifdef _USE_MANIFOLD_CSG
				computeCSG_Manifold(op1, op2, operation, result, CARVE_EPSILON, report_callback, entity);
				result_meshset_ok = MeshOps::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
#endif
			}

			if( !result_meshset_ok )
			{
				strs_err << "csg operation failed" << std::endl;

#ifdef _DEBUG
				if (paramsScaled.debugDump)
				{
					dumpOperands(op1, op2, result, tag, op1_dumped, op2_dumped, dumpColorSettings, paramsScaled);
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
			if( !result_meshset_ok )
			{
				assignResultOnFail(op1Orig, op2Orig, operation, result);
				return false;
			}
		}

#ifdef _DEBUG
		MeshSetInfo infoMesh_beforeDeNormalize(report_callback, entity.get());
		bool result_meshset_ok_beforeDeNormalize = MeshOps::checkMeshSetValidAndClosed(result, infoMesh_beforeDeNormalize, paramsScaled);
#endif

		normMesh.deNormalizeMesh(result, "", CARVE_EPSILON);


#ifdef _DEBUG
		{
			// de-normalized:
			MeshSetInfo infoMesh1(report_callback, entity.get());
			bool result_valid_2 = MeshOps::checkMeshSetValidAndClosed(result, infoMesh1, paramsUnscaled);

			if (!result_valid_2)
			{
				std::cout << "!result" << std::endl;
			}
		}
#endif

		return result_meshset_ok;
	}

	inline void computeCSG(shared_ptr<carve::mesh::MeshSet<3> >& op1, const std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& operands2, const carve::csg::CSG::OP operation,
		shared_ptr<GeometrySettings>& geomSettings, StatusCallback* report_callback, const shared_ptr<BuildingEntity>& entity)
	{
		if( !op1 || operands2.size() == 0 )
		{
			return;
		}

		// TODO: scale here, then do all the bool ops, then unscale
		bool success = false;
		std::multimap<double, shared_ptr<carve::mesh::MeshSet<3> > > mapVolumeMeshes;
		for (const shared_ptr<carve::mesh::MeshSet<3> >&meshset2 : operands2)
		{
			double volume = MeshOps::computeMeshsetVolume(meshset2.get());
			mapVolumeMeshes.insert({ volume, meshset2 });
		}

		size_t ii = 0;
		for( auto it = mapVolumeMeshes.rbegin(); it != mapVolumeMeshes.rend(); ++it, ++ii )
		{
			double volume = it->first;
			shared_ptr<carve::mesh::MeshSet<3> >& mesh2 = it->second;

#ifdef _DEBUG

			MeshSetInfo infoMesh1(report_callback, entity.get());
			bool allowFinEdges = false;
			GeomProcessingParams params(geomSettings, false);
			bool operand1valid = MeshOps::checkMeshSetValidAndClosed(op1, infoMesh1, params);

			if (!operand1valid)
			{
				std::cout << "!operand1valid" << std::endl;
			}
#endif

			bool normalizeCoords = true;
			shared_ptr<carve::mesh::MeshSet<3> > result;
			success = computeCSG_Carve(op1, mesh2, operation, result, geomSettings, report_callback, entity, normalizeCoords);

			if( success )
			{
				if( operation == carve::csg::CSG::A_MINUS_B || operation == carve::csg::CSG::UNION )
				{
					op1 = result;
				}

#ifdef _DEBUG

				MeshSetInfo infoMesh1(report_callback, entity.get());
				bool allowFinEdges = false;
				GeomProcessingParams params(geomSettings, false);
				bool operand1valid_2 = MeshOps::checkMeshSetValidAndClosed(op1, infoMesh1, params);

				if (!operand1valid_2)
				{
					std::cout << "!operand1valid" << std::endl;
				}
#endif
				continue;
			}
			normalizeCoords = false;
			success = computeCSG_Carve(op1, mesh2, operation, result, geomSettings, report_callback, entity, normalizeCoords);
			if( success )
			{
				if (operation == carve::csg::CSG::A_MINUS_B || operation == carve::csg::CSG::UNION)
				{
					op1 = result;
				}
			}

#ifdef _DEBUG
			{
				MeshSetInfo infoMesh1(report_callback, entity.get());
				bool allowFinEdges = false;
				GeomProcessingParams params(geomSettings, false);
				bool operand1valid_3 = MeshOps::checkMeshSetValidAndClosed(op1, infoMesh1, params);

				if (!operand1valid_3)
				{
					std::cout << "!operand1valid" << std::endl;
				}
			}
#endif
		}
	}
};

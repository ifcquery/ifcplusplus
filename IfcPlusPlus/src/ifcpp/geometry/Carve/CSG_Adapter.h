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
#include <ifcpp/geometry/Carve/GeomDebugDump.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/StatusCallback.h>

#include "IncludeCarveHeaders.h"
#include "MeshOps.h"
#include "MeshUtils.h"
#include "GeometryInputData.h"

#ifdef _USE_MANIFOLD_CSG
#include "CSG_Manifold.h"
#endif

#ifdef _DEBUG
static int csg_compute_count = 0;
#endif
#define _USE_CLASSIFY_NORMAL


namespace CSG_Adapter
{
	class ScopedEpsilonSetter
	{
	public:
		ScopedEpsilonSetter()
		{
			m_epsPrevious = carve::CARVE_EPSILON;
		}

		~ScopedEpsilonSetter()
		{
			if( m_epsPrevious != 1.0 )
			{
				carve::setEpsilon(m_epsPrevious);
			}
		}

		void setEpsilonValue( double epsilonToSet  )
		{
			carve::setEpsilon(epsilonToSet);
		}

		double m_epsPrevious = 1.0;
	};

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

	inline bool checkBoundinbBoxIntersection(const shared_ptr<carve::mesh::MeshSet<3> >& op1, const shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation)
	{
		if( operation == carve::csg::CSG::UNION )
		{
			// union operation needs to be done also when there is no intersection
			return true;
		}

		carve::geom::aabb<3> bbox1 = op1->getAABB();
		carve::geom::aabb<3> bbox2 = op2->getAABB();
		double xmin1 = bbox1.pos.x - bbox1.extent.x;
		double xmin2 = bbox2.pos.x - bbox2.extent.x;
		double xmax1 = bbox1.pos.x + bbox1.extent.x;
		double xmax2 = bbox2.pos.x + bbox2.extent.x;
		if( xmin1 >= xmax2 )
		{
			return false;
		}
		if( xmax1 <= xmin2 )
		{
			return false;
		}

		double ymin1 = bbox1.pos.y - bbox1.extent.y;
		double ymin2 = bbox2.pos.y - bbox2.extent.y;
		double ymax1 = bbox1.pos.y + bbox1.extent.y;
		double ymax2 = bbox2.pos.y + bbox2.extent.y;
		if( ymin1 >= ymax2 )
		{
			return false;
		}
		if( ymax1 <= ymin2 )
		{
			return false;
		}

		double zmin1 = bbox1.pos.z - bbox1.extent.z;
		double zmin2 = bbox2.pos.z - bbox2.extent.z;
		double zmax1 = bbox1.pos.z + bbox1.extent.z;
		double zmax2 = bbox2.pos.z + bbox2.extent.z;
		if( zmin1 >= zmax2 )
		{
			return false;
		}
		if( zmax1 <= zmin2 )
		{
			return false;
		}

		double volume1 = MeshUtils::getMeshVolume(op1.get());
		double volume2 = MeshUtils::getMeshVolume(op2.get());

		if( std::abs(volume1) < 0.005 )
		{
			double maxExtent = std::max(bbox1.extent.x, std::max(bbox1.extent.y, bbox1.extent.z));
			if( maxExtent < 0.01 )
			{
#ifdef _DEBUG
				glm::vec4 color(0.7, 0.7, 0.7, 1.0);
				GeomDebugDump::moveOffset(0.2);
				GeomDebugDump::dumpLocalCoordinateSystem();
				GeomDebugDump::moveOffset(op1);
				dumpMeshset(op1, color, false);
#endif

				return false;
			}
		}
		if( std::abs(volume2) < 0.005 )
		{
			double maxExtent = std::max(bbox2.extent.x, std::max(bbox2.extent.y, bbox2.extent.z));
			if( maxExtent < 0.01 )
			{
				return false;
			}
		}

		return true;
	}

	inline bool computeCSG_Carve(const shared_ptr<carve::mesh::MeshSet<3> >& op1Orig, const shared_ptr<carve::mesh::MeshSet<3> >& op2Orig, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result,  
		shared_ptr<GeometrySettings>& geom_settings, StatusCallback* report_callback, const shared_ptr<BuildingEntity>& entity, bool normalizeCoords)
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
		
		bool intersecting = checkBoundinbBoxIntersection(op1Orig, op2Orig, operation);
		if( !intersecting )
		{
			assignResultOnFail(op1Orig, op2Orig, operation, result);
			return true;
		}

		shared_ptr<carve::mesh::MeshSet<3> > op1(op1Orig->clone());
		shared_ptr<carve::mesh::MeshSet<3> > op2(op2Orig->clone());

		bool result_meshset_ok = false;
		bool dumpMeshes = false;
		bool normalizeCoordsInsteadOfEpsilon = normalizeCoords;
		MeshOps::CarveMeshNormalizer normMesh(op1, op2, normalizeCoordsInsteadOfEpsilon);
		normMesh.disableNormalizeAll = false;

		double scale = normMesh.getScale();
		double eps = 1.0 * EPS_M8 / scale;
		double epsCoplanarDistance = geom_settings->m_epsCoplanarDistance;

		ScopedEpsilonSetter epsilonSetter;
		if( !normalizeCoordsInsteadOfEpsilon )
		{
			epsilonSetter.setEpsilonValue(eps);
		}

		std::stringstream strs_err;
		try
		{
			// normalize first, so that EPS values match the size of different meshes
			normMesh.normalizeMesh(op1, "op1");
			normMesh.normalizeMesh(op2, "op2");

#ifdef _DEBUG
			if( csg_compute_count == 2 )
			{
				int wait = 0;
			}
			bool op1_dumped = false;
			bool op2_dumped = false;
			if( op1->vertex_storage.size() > 8 || op2->vertex_storage.size() >= 8 )
			{
				if( entity->m_tag == 20896 || entity->m_tag == 24704 || entity->m_tag == 36849 )
				{
					dumpMeshes = true;
				}
			}
#endif
			int tag = entity->m_tag;

			bool triangulateOperands = true;
			bool shouldBeClosedManifold = true;
			MeshOps::simplifyMeshSet(op1, geom_settings, report_callback, entity.get(), triangulateOperands, shouldBeClosedManifold, dumpMeshes);
			MeshOps::simplifyMeshSet(op2, geom_settings, report_callback, entity.get(), triangulateOperands, shouldBeClosedManifold, dumpMeshes);

			MeshOps::retriangulateMeshSetSimple(op1, false, epsCoplanarDistance, 0);
			MeshOps::retriangulateMeshSetSimple(op2, false, epsCoplanarDistance, 0);

			MeshSetInfo infoMesh1(report_callback, entity.get());
			MeshSetInfo infoMesh2(report_callback, entity.get());
			bool operand1valid = MeshUtils::checkMeshSetValidAndClosed(op1, infoMesh1);
			bool operand2valid = MeshUtils::checkMeshSetValidAndClosed(op2, infoMesh2);

			if( !operand1valid || !operand2valid )
			{
				assignResultOnFail(op1Orig, op2Orig, operation, result);
				return true;
			}

#ifdef _DEBUG
			struct FaceInfo
			{
				double area = 0;
				int numEdges = 0;
				std::set<carve::mesh::Edge<3>* > setEdges;
				std::set<carve::mesh::Vertex<3>* > setVertices;
			};
			std::multimap<double, FaceInfo> mapFaceInfos;
			for( auto mesh : op1->meshes)
			{
				double vol = mesh->volume();
				for( auto face : mesh->faces )
				{
					double faceArea = MeshUtils::computeFaceArea(face);

					FaceInfo fi;
					fi.area = faceArea;
					fi.numEdges = face->n_edges;
					auto edge = face->edge;
					for( size_t jjEdge = 0; jjEdge < face->n_edges; ++jjEdge )
					{
						fi.setEdges.insert(edge);
						carve::mesh::Vertex<3>* vert = edge->vert;
						fi.setVertices.insert(vert);
						edge = edge->next;
					}
					if( edge != face->edge )
					{
						std::cout << "edge != face->edge" << std::endl;
					}
					mapFaceInfos.insert({ faceArea, fi });
				}
			}

			DumpSettingsStruct dumpColorSettings;
			glm::vec4 color(0.3, 0.33, 0.33, 1.);
			if( tag == 1951294 || tag == 19026)
			{
				dumpMeshes = true;
				if( !operand1valid )
				{
					std::cout << "incorrect csg operand, csg_compute_count " << csg_compute_count << std::endl;
				}

				if( op1->vertex_storage.size() >=50 )
				{
					int wait = 0;
				}

				glm::vec4 color(0.7, 0.7, 0.7, 1.0);
				GeomDebugDump::moveOffset(0.2);
				GeomDebugDump::dumpLocalCoordinateSystem();
				GeomDebugDump::moveOffset(op1);
				GeomDebugDump::moveOffset(op2);
				op1_dumped = true;
				op2_dumped = true;

				std::string labelStr = "csg-operands-input";
				GeomDebugDump::dumpVertex( GeomDebugDump::labelPos, color, labelStr);

				dumpMeshset(op1, color, false);
				//GeomDebugDump::moveOffset(op1);
				//GeomDebugDump::moveOffset(0.3);

				GeomDebugDump::dumpFacePolygons(op2.get(), color, false);
				dumpMeshset(op2, color);

				//GeomDebugDump::dumpEntity(entity);
			}

			if( !operand2valid )
			{
				glm::vec4 color(0.7, 0.7, 0.7, 1.0);
				dumpMeshset(op2, color, true);
				op2_dumped = true;
			}
			++csg_compute_count;
#endif

			////////////////////// compute carve csg operation   /////////////////////////////////////////////
			carve::csg::CSG csg;
			result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));

			MeshSetInfo infoResult( report_callback, entity.get() );
			result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, eps, true, dumpMeshes );

			// TODO: check for fail with closed mesh, but not fully sliced through.

#ifdef _USE_CLASSIFY_NORMAL
			if( !result_meshset_ok )
			{
				bool shouldBeClosedManifold = true;
				MeshOps::mergeCoplanarFacesInMeshSet(op1, geom_settings, shouldBeClosedManifold, dumpMeshes);
				carve::csg::CSG csg;
				result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_NORMAL));
				result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, eps, true, dumpMeshes );

				if( result_meshset_ok )
				{
					shared_ptr<carve::mesh::MeshSet<3> > result_clone(result->clone());
					MeshOps::retriangulateMeshSetSimple(result_clone, false, eps, 0);
					result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult);
				}
			}
#endif

			if( !result_meshset_ok )
			{
				dumpMeshes = true;

#ifdef _DEBUG
				if( dumpMeshes )
				{
					bool checkZeroAreaFaces = false;
					dumpWithLabel("computeCSG::carve::result", result, dumpColorSettings, checkZeroAreaFaces, true, true);
				}
#endif

#ifdef _USE_MANIFOLD_CSG
				computeCSG_Manifold(op1, op2, operation, result, eps, report_callback, entity);
				result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
#endif
			}

#ifdef _DEBUG
			if( dumpMeshes )
			{
				bool checkZeroAreaFaces = false;

				if( (op2_dumped && !op1_dumped) )
				{
					GeomDebugDump::moveOffset(0.3);
				}

				if( !op1_dumped )
				{
					op1_dumped = true;
					dumpWithLabel("computeCSG::op1", op1, dumpColorSettings, checkZeroAreaFaces, true, false);
				}

				if( !op2_dumped )
				{
					dumpWithLabel("computeCSG::op2", op2, dumpColorSettings, checkZeroAreaFaces, false, true);
					op2_dumped = true;

				}

				GeomDebugDump::moveOffset(0.5);
				GeomDebugDump::moveOffset(op2);

				std::string labelStr = "computeCSG::results";
				GeomDebugDump::dumpVertex( GeomDebugDump::labelPos, color, labelStr);
				GeomDebugDump::dumpFacePolygons(op2.get(), color, false);
				GeomDebugDump::dumpMeshset(result, color, true);
				GeomDebugDump::moveOffset(0.5);

				if( !result_meshset_ok )
				{
					dumpWithLabel("computeCSG::results", result, dumpColorSettings, checkZeroAreaFaces, true, true);
				}
				//GeomDebugDump::dumpMeshsetOpenEdges(result, color, false, true);

				GeomDebugDump::moveOffset(op1);
				GeomDebugDump::moveOffset(op2);
				GeomDebugDump::moveOffset(0.3);
			}
#endif

			if( !result_meshset_ok )
			{
				strs_err << "csg operation failed" << std::endl;

#ifdef _DEBUG
				if( dumpMeshes )
				{
					MeshSetInfo infoOp1(report_callback, entity.get());
					MeshSetInfo infoOp2(report_callback, entity.get());
					bool op1_mesh_closed = MeshUtils::checkMeshSetValidAndClosed(op1, infoOp1);
					bool op2_mesh_closed = MeshUtils::checkMeshSetValidAndClosed(op2, infoOp2);
					if( !op1_mesh_closed || !op2_mesh_closed )
					{
						//normMesh.deNormalizeMesh(op1);
						//normMesh.deNormalizeMesh(op2);
						GeomDebugDump::moveOffset(0.5);
						glm::vec4 color1(0.7, 0.7, 0.7, 0.88);
						std::string labelStr = "csg--result--invalid--operands: ";
						GeomDebugDump::dumpVertex(GeomDebugDump::labelPos, color1, labelStr);

						glm::vec4 color(0.7, 0.7, 0.7, 0.05);
						dumpMeshset(op1, color, false);
						color = glm::vec4(0.5, 0.7, 0.5, 1.0);
						dumpMeshset(op2, color);
						GeomDebugDump::moveOffset(op1);
						GeomDebugDump::moveOffset(op2);
					}
				}
#endif
			}
		}
		catch( carve::exception& ce )
		{
			strs_err << ce.str().c_str();
		}
		catch( const std::out_of_range& oor )
		{
			strs_err << oor.what();
		}
		catch( std::exception& e )
		{
			strs_err << e.what();
		}
		catch( ... )
		{
			strs_err << "csg operation failed" << std::endl;
		}

		if( strs_err.tellp() > 0 )
		{

#ifdef _DEBUG
			if( dumpMeshes )
			{
				glm::vec4 color(0.7, 0.7, 0.7, 0.05);
				GeomDebugDump::moveOffset(0.4);
				dumpMeshset(op1, color, false);
				dumpMeshset(op2, color, true);

				if( result )
				{
					GeomDebugDump::moveOffset(op2);
					GeomDebugDump::moveOffset(0.3);
					dumpMeshset(result, color, true);
				}
			}
#endif
			if( !result_meshset_ok )
			{
				assignResultOnFail(op1, op2, operation, result);
				normMesh.deNormalizeMesh(result, "");
				return false;
			}
		}

		normMesh.deNormalizeMesh(result, "");
		return result_meshset_ok;
	}

	inline void computeCSG(shared_ptr<carve::mesh::MeshSet<3> >& op1, std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& operands2, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result,
		shared_ptr<GeometrySettings>& geom_settings, StatusCallback* report_callback, const shared_ptr<BuildingEntity>& entity)
	{
		if( !op1 || operands2.size() == 0 )
		{
			if( operation == carve::csg::CSG::A_MINUS_B )
			{
				result = op1;
				return;
			}

			return;
		}

		// TODO: scale here, then do all the bool ops, then unscale

		shared_ptr<carve::mesh::MeshSet<3> > resultCurrentMesh;
		bool success = false;
		std::multimap<double, shared_ptr<carve::mesh::MeshSet<3> > > mapVolumeMeshes;
		for( shared_ptr<carve::mesh::MeshSet<3> >&meshset2 : operands2 )
		{
			double volume = MeshUtils::getMeshVolume(meshset2.get());
			mapVolumeMeshes.insert({ volume, meshset2 });
		}

		for( auto it = mapVolumeMeshes.rbegin(); it != mapVolumeMeshes.rend(); ++it )
		{
			double volume = it->first;
			shared_ptr<carve::mesh::MeshSet<3> >& mesh2 = it->second;

			bool normalizeCoords = true;
			success = computeCSG_Carve(op1, mesh2, operation, resultCurrentMesh, geom_settings, report_callback, entity, normalizeCoords);
			if( success )
			{
				result = resultCurrentMesh;
				
				if( operation == carve::csg::CSG::A_MINUS_B )
				{
					op1 = resultCurrentMesh;	
				}
				continue;
			}
			normalizeCoords = !normalizeCoords;
			success = computeCSG_Carve(op1, mesh2, operation, resultCurrentMesh, geom_settings, report_callback, entity, normalizeCoords);
			if( success )
			{
				result = resultCurrentMesh;
				if( operation == carve::csg::CSG::A_MINUS_B )
				{
					op1 = resultCurrentMesh;
				}
				continue;
			}
		}
	}
};

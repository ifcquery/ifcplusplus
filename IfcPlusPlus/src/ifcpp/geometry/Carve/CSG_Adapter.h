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
#include "GeometryInputData.h"

#ifdef _DEBUG
static int csg_compute_count = 0;
#endif

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

	inline void computeCSG(shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result,  
		shared_ptr<GeometrySettings>& geom_settings, StatusCallback* report_callback, const shared_ptr<BuildingEntity>& entity)
	{
		if( !op1 || !op2 )
		{
			return;
		}

		bool dumpMeshes = false;
		bool normalizeCoordsInsteadOfEpsilon = false;
		MeshOps::CarveMeshNormalizer normMesh(op1, op2, normalizeCoordsInsteadOfEpsilon);

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
			if( op1->vertex_storage.size() > 8 || op2->vertex_storage.size() > 8 )
			{
				if( entity->m_tag == 5498 || entity->m_tag == 835571 || entity->m_tag == 779169 )
				{
					dumpMeshes = true;
					op1_dumped = true;
					op2_dumped = true;
				}
			}
#endif

			bool triangulateOperands = true;
			MeshOps::simplifyMeshSet(op1, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);
			MeshOps::simplifyMeshSet(op2, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);

			MeshOps::retriangulateMeshSetSimple(op1, false, epsCoplanarDistance, 0);
			MeshOps::retriangulateMeshSetSimple(op2, false, epsCoplanarDistance, 0);
			
			MeshSetInfo infoMesh1;
			MeshSetInfo infoMesh2;
			bool mesh1valid = MeshUtils::checkMeshSetValidAndClosed(op1, infoMesh1, report_callback, entity.get());
			bool mesh2valid = MeshUtils::checkMeshSetValidAndClosed(op2, infoMesh2, report_callback, entity.get());

#ifdef _DEBUG
			DumpSettingsStruct dumpColorSettings;
			glm::vec4 color(0.3, 0.33, 0.33, 1.);
			if( !mesh1valid )//|| entity->m_tag == 835571 )
			{
				dumpMeshes = true;
				if( !mesh1valid )
				{
					std::cout << "incorrect csg operand, csg_compute_count " << csg_compute_count << std::endl;
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
				dumpMeshset(op2, color);
				//GeomDebugDump::dumpEntity(entity);
			}

			if( !mesh2valid )
			{
				glm::vec4 color(0.7, 0.7, 0.7, 1.0);
				dumpMeshset(op2, color, true);
				op2_dumped = true;
			}
			++csg_compute_count;
#endif

			////////////////////// compute carve csg operation   /////////////////////////////////////////////
			if( mesh1valid && mesh2valid )
			{
				carve::csg::CSG csg;
				result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
			}

			bool result_meshset_ok = false;
			if( result )
			{
				if( result->meshes.size() == 0 )
				{
					if( mesh1valid && mesh2valid )
					{
						MeshOps::simplifyMeshCarve(op1, geom_settings, report_callback, entity.get(), dumpMeshes);
						MeshOps::simplifyMeshCarve(op2, geom_settings, report_callback, entity.get(), dumpMeshes);
						carve::csg::CSG csg;
						result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));

						double eps_restore = eps;
						//for( size_t ii = 0; ii < 3; ++ii )
						{
							if( result )
							{
								if( result->meshes.size() == 0 )
								{
									eps = eps * 5;
									epsilonSetter.setEpsilonValue(eps);
									carve::csg::CSG csg;
									result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
									if( result )
									{
										MeshSetInfo infoResult;
										result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
										if( result_meshset_ok )
										{
											//break;
										}
									}
								}
							}
						}
						epsilonSetter.setEpsilonValue(eps_restore);
						if( result )
						{
							if( result->meshes.size() == 0 )
							{
								normMesh.deNormalizeMesh(op1, "op1");
								normMesh.deNormalizeMesh(op2, "op2");
								carve::csg::CSG csg;
								result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
								normMesh.setToZero();
							}
						}
					}
				}
			}

			if( result )
			{
				if( result->meshes.size() == 0 )
				{
					result_meshset_ok = false;
				}
				else
				{
					MeshSetInfo infoResult;
					result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());

					if( !result_meshset_ok )
					{
						MeshOps::simplifyMeshSet(op1, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);
						MeshOps::simplifyMeshSet(op2, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);

						{
							// try again with bigger epsilon tolerance
							eps = eps * 10;
							ScopedEpsilonSetter epsilonSetter;
							epsilonSetter.setEpsilonValue(eps);

							carve::csg::CSG csg;
							result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
						}

						result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());

						if( !result_meshset_ok )
						{
							dumpMeshes = true;
							MeshOps::simplifyMeshSet(result, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);
							result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());

							if( operation == carve::csg::CSG::A_MINUS_B )
							{
								// try first to intersect, then A_MINUS_B
								carve::csg::CSG csg;
								shared_ptr<carve::mesh::MeshSet<3> > intersectionResult(csg.compute(op1.get(), op2.get(), carve::csg::CSG::INTERSECTION, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
								MeshOps::simplifyMeshSet(intersectionResult, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);

								MeshSetInfo infoIntersectionResult;
								bool intersection_ok = MeshUtils::checkMeshSetValidAndClosed(intersectionResult, infoIntersectionResult, report_callback, entity.get());

								if( intersection_ok )
								{
									carve::csg::CSG csg;
									result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), intersectionResult.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));

									result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
								}
								
#ifdef _DEBUG
								if( dumpMeshes )
								{
									bool checkZeroAreaFaces = false;
									GeomDebugDump::moveOffset(0.6);
									dumpWithLabel("computeCSG::intersection: ", intersectionResult, dumpColorSettings, checkZeroAreaFaces, false, true);
								}
#endif
							}
						}
					}
					

#ifdef _DEBUG
					if( dumpMeshes )
					{
						bool checkZeroAreaFaces = false;
						
						if( (op2_dumped && !op1_dumped) )//|| (!op2_dumped && op1_dumped) )
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


						GeomDebugDump::moveOffset(0.2);
						GeomDebugDump::moveOffset(op2);
						
						dumpWithLabel("computeCSG::results", result, dumpColorSettings, checkZeroAreaFaces, true, true);
						
						GeomDebugDump::dumpMeshsetOpenEdges(result, color, false, true);
						
						GeomDebugDump::moveOffset(op1);
						GeomDebugDump::moveOffset(op2);
						GeomDebugDump::moveOffset(0.3);
					}
#endif
					normMesh.deNormalizeMesh(result, "");
				}
			}

			if( !result_meshset_ok )
			{
				strs_err << "csg operation failed" << std::endl;
				
#ifdef _DEBUG
				if( dumpMeshes )
				{
					MeshSetInfo infoOp1, infoOp2;
					bool op1_mesh_closed = MeshUtils::checkMeshSetValidAndClosed(op1, infoOp1, report_callback, entity.get());
					bool op2_mesh_closed = MeshUtils::checkMeshSetValidAndClosed(op2, infoOp2, report_callback, entity.get());
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

			if( operation == carve::csg::CSG::A_MINUS_B )
			{
				normMesh.deNormalizeMesh(op1, "");
				result = op1;
			}
			else if( operation == carve::csg::CSG::B_MINUS_A )
			{
				normMesh.deNormalizeMesh(op2, "");
				result = op2;
			}
			else if( operation == carve::csg::CSG::UNION )
			{
				normMesh.deNormalizeMesh(op1, "");
				result = op1;
			}
			else
			{
				result.reset();
			}
		}
	}
};

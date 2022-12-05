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
				if( entity->m_tag == 5498 || entity->m_tag == 835571 || entity->m_tag == 38022 )
				{
					dumpMeshes = true;
					op1_dumped = true;
					op2_dumped = true;
				}
			}
			if( entity->m_tag == 38022 )
			{
				dumpMeshes = true;
			}
#endif

			bool triangulateOperands = true;
			MeshOps::simplifyMeshSet(op1, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);
			MeshOps::simplifyMeshSet(op2, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);

			MeshOps::retriangulateMeshSetSimple(op1, false, epsCoplanarDistance, 0);
			MeshOps::retriangulateMeshSetSimple(op2, false, epsCoplanarDistance, 0);
			
			MeshSetInfo infoMesh1;
			MeshSetInfo infoMesh2;
			bool operand1valid = MeshUtils::checkMeshSetValidAndClosed(op1, infoMesh1, report_callback, entity.get());
			bool operand2valid = MeshUtils::checkMeshSetValidAndClosed(op2, infoMesh2, report_callback, entity.get());

#ifdef _DEBUG
			DumpSettingsStruct dumpColorSettings;
			glm::vec4 color(0.3, 0.33, 0.33, 1.);
			if( !operand1valid )//|| entity->m_tag == 835571 )
			{
				dumpMeshes = true;
				if( !operand1valid )
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
				GeomDebugDump::moveOffset(op1);
				GeomDebugDump::moveOffset(0.3);
				
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
			if( operand1valid && operand2valid )
			{
				carve::csg::CSG csg;
				result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
			}

			MeshSetInfo infoResult;
			bool result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());

			if( !result_meshset_ok )
			{
				carve::csg::CSG csg;
				result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_NORMAL));
				result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
			}

			if( !result_meshset_ok )
			{
				if( operand1valid && operand2valid )
				{
					MeshOps::simplifyMeshCarve(op1, geom_settings, report_callback, entity.get(), dumpMeshes);
					MeshOps::simplifyMeshCarve(op2, geom_settings, report_callback, entity.get(), dumpMeshes);
					carve::csg::CSG csg;
					result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));

					double eps_restore = eps;
					for( size_t ii = 0; ii < 5; ++ii )
					{
						eps = eps * 5.5;
						epsilonSetter.setEpsilonValue(eps);
						carve::csg::CSG csg;
						result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
						result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
						
						if( !result_meshset_ok )
						{
							carve::csg::CSG csg;
							result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_NORMAL));
							result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
						}

						if( result_meshset_ok )
						{
#ifdef _DEBUG
							bool checkZeroAreaFaces = false;
							GeomDebugDump::moveOffset(1.0);
							dumpWithLabel("result_changed_eps: ", op1, dumpColorSettings, checkZeroAreaFaces, false, false);
							GeomDebugDump::moveOffset(1.0);
							dumpWithLabel("result_changed_eps: ", op2, dumpColorSettings, checkZeroAreaFaces, false, false);
							GeomDebugDump::moveOffset(1.0);
							dumpWithLabel("result_changed_eps: ", result, dumpColorSettings, checkZeroAreaFaces, false, false);
							std::cout << "successfully changed epsilon: " << eps << std::endl;
#endif
							break;
						}
					}
					epsilonSetter.setEpsilonValue(eps_restore);
				}
			}

			if( !result_meshset_ok )
			{
				dumpMeshes = true;

				if( operation == carve::csg::CSG::A_MINUS_B )
				{
					//// try first to intersect, then A_MINUS_B
					//carve::csg::CSG csg;
					//shared_ptr<carve::mesh::MeshSet<3> > unionResultMesh(csg.compute(op1.get(), op2.get(), carve::csg::CSG::UNION, nullptr, carve::csg::CSG::CLASSIFY_NORMAL));

					//shared_ptr<carve::mesh::MeshSet<3> > unionResult;
					//MeshUtils::getLargestClosedMesh(unionResultMesh, unionResult);
					//MeshOps::simplifyMeshSet(unionResult, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);

					//MeshSetInfo infoIntersectionResult;
					//bool intersection_ok = MeshUtils::checkMeshSetValidAndClosed(unionResult, infoIntersectionResult, report_callback, entity.get());

					//if( !intersection_ok )
					//{
					//	carve::csg::CSG csg;
					//	shared_ptr<carve::mesh::MeshSet<3> > unionResult2(csg.compute(op1.get(), op2.get(), carve::csg::CSG::UNION, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
					//	
					//	shared_ptr<carve::mesh::MeshSet<3> > unionResult;
					//	MeshUtils::getLargestClosedMesh(unionResult2, unionResult);
					//	MeshOps::simplifyMeshSet(unionResult, geom_settings, report_callback, entity.get(), triangulateOperands, dumpMeshes);
					//	intersection_ok = MeshUtils::checkMeshSetValidAndClosed(unionResult, infoIntersectionResult, report_callback, entity.get());

					//}
//
//					if( intersection_ok )
//					{
//						carve::csg::CSG csg;
//						result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(unionResult.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
//						result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
//
//						if( !result_meshset_ok )
//						{
//#ifdef _DEBUG
//							if( dumpMeshes )
//							{
//								bool checkZeroAreaFaces = false;
//								GeomDebugDump::dumpLocalCoordinateSystem();
//								GeomDebugDump::moveOffset(1.0);
//								dumpWithLabel("unionResult: ", unionResult, dumpColorSettings, checkZeroAreaFaces, false, false);
//								dumpWithLabel("result: ", result, dumpColorSettings, checkZeroAreaFaces, false, true);
//							}
//#endif
//
//							carve::csg::CSG csg;
//							result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(unionResult.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_NORMAL));
//							result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
//						}
//					}


					if( !result_meshset_ok )
					{
#ifdef OCC_FOUND
						TopoDS_Shape operand1_shape;
						TopoDS_Shape operand2_shape;
						MeshUtils::convertCarve2OpenCascade(op1.get(), operand1_shape);
						MeshUtils::convertCarve2OpenCascade(op2.get(), operand2_shape);

						TopoDS_Solid operand1_solid;
						TopoDS_Solid operand2_solid;
						MeshUtils::getLargestSolid(operand1_shape, operand1_solid);
						MeshUtils::getLargestSolid(operand2_shape, operand2_solid);

#ifdef _DEBUG
						if( dumpMeshes )
						{
							bool checkZeroAreaFaces = false;
							GeomDebugDump::dumpLocalCoordinateSystem();
							GeomDebugDump::moveOffset(1.0);
							dumpWithLabel("op1: ", op1, dumpColorSettings, checkZeroAreaFaces, false, false);
							dumpWithLabel("op2: ", op2, dumpColorSettings, checkZeroAreaFaces, false, true);
							GeomDebugDump::moveOffset(0.7);
							//dumpWithLabel("computeCSG::union: ", unionResult, dumpColorSettings, checkZeroAreaFaces, true, true);
							GeomDebugDump::moveOffset(0.7);
							dumpWithLabel("computeCSG:: result: ", result, dumpColorSettings, checkZeroAreaFaces, true, true);
							GeomDebugDump::moveOffset(0.5);
							GeomDebugDump::dumpLocalCoordinateSystem();
						}
						

						shared_ptr<carve::mesh::MeshSet<3> > shape1mesh;
						shared_ptr<carve::mesh::MeshSet<3> > shape2mesh;
						MeshUtils::TopoDS_Shape2Meshset(operand1_solid, shape1mesh);
						MeshUtils::TopoDS_Shape2Meshset(operand2_solid, shape2mesh);

						MeshSetInfo infoOp1;
						bool mesh_closed1 = MeshUtils::checkMeshSetValidAndClosed(shape1mesh, infoOp1, nullptr, nullptr);
						if( !mesh_closed1 )
						{
							bool triangulateOperands = false;
							bool dumpMeshes = false;
							MeshOps::simplifyMeshSet(shape1mesh, geom_settings, nullptr, nullptr, triangulateOperands, dumpMeshes);
						}
						MeshSetInfo infoOp2;
						bool mesh_closed2 = MeshUtils::checkMeshSetValidAndClosed(shape2mesh, infoOp1, nullptr, nullptr);
						if( !mesh_closed2 )
						{
							bool triangulateOperands = false;
							bool dumpMeshes = false;
							MeshOps::simplifyMeshSet(shape2mesh, geom_settings, nullptr, nullptr, triangulateOperands, dumpMeshes);
						}

						bool checkZeroAreaFaces = false;
						GeomDebugDump::moveOffset(0.6);
						dumpWithLabel("TopoDS_Shape1: ", shape1mesh, dumpColorSettings, checkZeroAreaFaces, false, true);

						GeomDebugDump::moveOffset(0.6);
						dumpWithLabel("TopoDS_Shape2: ", shape2mesh, dumpColorSettings, checkZeroAreaFaces, false, true);
#endif

						if( !operand1_solid.IsNull() && !operand2_solid.IsNull() )
						{
							double volume1 = MeshUtils::getVolume(operand1_solid);
							double volume2 = MeshUtils::getVolume(operand2_solid);
							if( volume1 < 0 ) { operand1_solid.Reverse(); }
							if( volume2 < 0 ) { operand2_solid.Reverse(); }

							ShapeFix_ShapeTolerance shapeTolerance;// = new ShapeFix_ShapeTolerance();
							double tolerance = 1e-7;
							shapeTolerance.SetTolerance(operand1_solid,tolerance);
							shapeTolerance.SetTolerance(operand2_solid,tolerance);

							BRepAlgoAPI_Cut mkCut(operand1_solid, operand2_solid);  // CSG::A_MINUS_B
							mkCut.SetFuzzyValue(tolerance);
							mkCut.SetNonDestructive(true);
							mkCut.SetUseOBB( true );

							mkCut.Build();

							if( mkCut.IsDone() )
							{
								TopoDS_Shape cutResultShape = mkCut.Shape();

								TopoDS_Solid cutResult;
								MeshUtils::getLargestSolid(cutResultShape, cutResult);
								

								MeshUtils::TopoDS_Shape2Meshset(cutResult, result);
								result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
#ifdef _DEBUG
								if( result_meshset_ok )
								{
									std::cout << "CSG no success with carve, success with OCC" << std::endl;
								}

								if( !result_meshset_ok )
								{

									StlAPI_Writer STLwriter;
									STLwriter.Write(operand1_solid, "D:\\temp\\op1_shape.stl");
									STLwriter.Write(operand2_solid, "D:\\temp\\op2_shape.stl");
									STLwriter.Write(cutResult, "D:\\temp\\cutResult.stl");


									bool checkZeroAreaFaces = false;
									GeomDebugDump::moveOffset(0.6);
									dumpWithLabel("BRepAlgoAPI_Cut: ", result, dumpColorSettings, checkZeroAreaFaces, false, true);
									GeomDebugDump::moveOffset(0.6);
									dumpWithLabel("op1: ", op1, dumpColorSettings, checkZeroAreaFaces, false, true);
									GeomDebugDump::moveOffset(0.6);
									dumpWithLabel("op2: ", op2, dumpColorSettings, checkZeroAreaFaces, false, true);
								}

								double volume = MeshUtils::getVolume(cutResult);
								if( volume == 0 )
								{
									std::cout << "volume is 0";
								}
#endif
							}
							else
							{
								std::cout << "BRepAlgoAPI_Cut failed";
							}

//							if( !result_meshset_ok )
//							{
//								BRepAlgoAPI_Fuse mkCommon(operand1_solid, operand2_solid);       // CSG::UNION
//								mkCommon.SetFuzzyValue(tolerance);
//								mkCommon.SetNonDestructive(true);
//								mkCommon.Build();
//
//								if( mkCommon.IsDone() )
//								{
//									TopoDS_Solid fusedSolid;
//									MeshUtils::getLargestSolid(mkCommon.Shape(), fusedSolid);
//
//									shared_ptr<carve::mesh::MeshSet<3> > meshCommon;
//									MeshUtils::TopoDS_Shape2Meshset(fusedSolid, meshCommon);
//
//#ifdef _DEBUG
//									GProp_GProps gprops;
//									BRepGProp::VolumeProperties(fusedSolid, gprops);
//									double volume = gprops.Mass();
//
//
//									bool checkZeroAreaFaces = false;
//									GeomDebugDump::moveOffset(0.6);
//									dumpWithLabel("meshCommon: ", meshCommon, dumpColorSettings, checkZeroAreaFaces, false, true);
//									//GeomDebugDump::dumpMeshset(meshCommon, color, true);
//#endif
//
//									TopoDS_Shape aResult = BRepAlgoAPI_Cut(fusedSolid, operand2_solid).Shape();
//									MeshUtils::TopoDS_Shape2Meshset(aResult, result);
//									result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
//
//									if( !result_meshset_ok )
//									{
//										shared_ptr<carve::mesh::MeshSet<3> > largestMesh;
//										MeshUtils::getLargestClosedMesh(result, largestMesh);
//
//										//carve::csg::CSG csg;
//										//result = shared_ptr<carve::mesh::MeshSet<3> >(csg.compute(op1.get(), meshCommon1.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE));
//
//										//result_meshset_ok = MeshUtils::checkMeshSetValidAndClosed(result, infoResult, report_callback, entity.get());
//
//
//#ifdef _DEBUG
//										bool checkZeroAreaFaces = false;
//										GeomDebugDump::moveOffset(0.6);
//										GeomDebugDump::dumpMesh(meshCommon->meshes[0], color, false);
//										GeomDebugDump::moveOffset(0.6);
//										GeomDebugDump::dumpMeshset(op1, color, false);
//										GeomDebugDump::dumpMeshset(op2, color, false);
//										GeomDebugDump::moveOffset(0.6);
//										dumpWithLabel("BRepAlgoAPI_Cut: ", result, dumpColorSettings, checkZeroAreaFaces, false, true);
//
//										// try also https://github.com/executionunit/csgjs-cpp
//#endif
//									}
//								}
//							}
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

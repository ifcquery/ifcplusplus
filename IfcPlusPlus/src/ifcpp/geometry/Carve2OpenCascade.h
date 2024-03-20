#pragma once

#ifdef CSG_OCC

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepOffsetAPI_Sewing.hxx>
#include <BRep_Builder.hxx>
#include <Poly_Triangulation.hxx>
#include <ShapeFix_Solid.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColgp_SequenceOfPnt.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>
#include <TopExp_Explorer.hxx>

class Carve2OpenCascade
{
public:
	static void convertOCC2Carve(const TopoDS_Shape& input, shared_ptr<carve::mesh::MeshSet<3> >& result)
	{
		if (input.IsNull())
		{
			return;
		}
		double eps = 1.1e-6;
		PolyInputCache3D poly(eps);

		TopAbs_ShapeEnum shape_type = input.ShapeType();

		if (shape_type == TopAbs_SOLID || shape_type == TopAbs_COMPOUND)
		{
			//shared_ptr<IfcPolyLoop> upperLoop = make_shared<IfcPolyLoop>();
			//shared_ptr<IfcPolyLoop> lowerLoop = make_shared<IfcPolyLoop>();


			//shared_ptr<IfcClosedShell> shell = make_shared<IfcClosedShell>();
			//ifcBrep->m_Outer = shell;

			//std::unordered_map<int, shared_ptr<IfcCartesianPoint> > mapIfcPoints;

			Standard_Real linear_tolerance = 0.01;// 0.06 * 0.001;  // for [m]
			Standard_Real angular_tolerance = 0.5;
			bool is_relative = false;
			BRepMesh_IncrementalMesh incremental_mesh(input, linear_tolerance, is_relative, angular_tolerance);

			TopExp_Explorer shape_explorer(input, TopAbs_FACE);
			for (; shape_explorer.More(); shape_explorer.Next())
			{
				const TopoDS_Face& face = TopoDS::Face(shape_explorer.Current());
				TopLoc_Location L = TopLoc_Location();
				const Handle(Poly_Triangulation)& poly_triangulation = BRep_Tool::Triangulation(face, L);

				if (poly_triangulation.IsNull())
				{
					continue;
				}

				const gp_Trsf& face_trsf = L.Transformation();


				//! Returns the number of nodes for this triangulation.
				Standard_Integer numNodes = poly_triangulation->NbNodes();

				//! Returns the number of triangles for this triangulation.
				Standard_Integer numTriangles = poly_triangulation->NbTriangles();

				//size_t offset_vertex_storage = vertices_tri.size();

				//! Returns a node at the given index.
				//! @param[in] theIndex node index within [1, NbNodes()] range
				//! @return 3D point coordinates
				for (size_t ii = 1; ii <= numTriangles; ++ii)
				{
					const Poly_Triangle& triangle = poly_triangulation->Triangle(ii);

					Standard_Integer theN1, theN2, theN3;
					triangle.Get(theN1, theN2, theN3);
					gp_Pnt point0 = poly_triangulation->Node(theN1);
					gp_Pnt point1 = poly_triangulation->Node(theN2);
					gp_Pnt point2 = poly_triangulation->Node(theN3);

					vec3 p0 = carve::geom::VECTOR(point0.X(), point0.Y(), point0.Z());
					vec3 p1 = carve::geom::VECTOR(point1.X(), point1.Y(), point1.Z());
					vec3 p2 = carve::geom::VECTOR(point2.X(), point2.Y(), point2.Z());

					int idx0 = poly.addPoint(p0);
					int idx1 = poly.addPoint(p1);
					int idx2 = poly.addPoint(p2);
					poly.m_poly_data->addFace(idx0, idx1, idx2);

					//shared_ptr<IfcCartesianPoint> ifcPoint0, ifcPoint1, ifcPoint2;

					//getOrCreateIfcPoint(mapIfcPoints, idx0, p0, ifcPoint0);
					//getOrCreateIfcPoint(mapIfcPoints, idx1, p1, ifcPoint1);
					//getOrCreateIfcPoint(mapIfcPoints, idx2, p2, ifcPoint2);

					//shared_ptr<IfcFaceOuterBound> faceBound = make_shared<IfcFaceOuterBound>();

					//shared_ptr<IfcPolyLoop> polyLoop = make_shared<IfcPolyLoop>();
					//polyLoop->m_Polygon.push_back(ifcPoint0);
					//polyLoop->m_Polygon.push_back(ifcPoint1);
					//polyLoop->m_Polygon.push_back(ifcPoint2);

					//faceBound->m_Bound = polyLoop;
					//faceBound->m_Orientation = make_shared<IfcBoolean>(true);

					//shared_ptr<IfcFace> ifcFace = make_shared<IfcFace>();

					//ifcFace->m_Bounds.push_back(faceBound);
					//shell->m_CfsFaces.push_back(ifcFace);
				}
			}

			//m_ifcGeometricItem = ifcBrep;
		}
		else if (shape_type == TopAbs_WIRE || shape_type == TopAbs_EDGE || shape_type == TopAbs_VERTEX)
		{
			TopExp_Explorer Ex;
			for (Ex.Init(input, TopAbs_EDGE); Ex.More(); Ex.Next())
			{
				TopoDS_Edge edge = TopoDS::Edge(Ex.Current());
				//getEdgePoints(edge, vertices_lines, render_options);
			}
		}


		if (input.ShapeType() == TopAbs_COMPOUND)
		{
			for (TopoDS_Iterator iter(input); iter.More(); iter.Next())
			{
			}


			//Standard_Real aDeflection = 0.1;
			//BRepMesh_IncrementalMesh(input, 1);
			//Standard_Integer aIndex = 1, nbNodes = 0;
			//TColgp_SequenceOfPnt aPoints, aPoints1;
			//for (TopExp_Explorer aExpFace(input, TopAbs_FACE); aExpFace.More(); aExpFace.Next())
			//{
			//	TopoDS_Face aFace = TopoDS::Face(aExpFace.Current());
			//	TopAbs_Orientation faceOrientation = aFace.Orientation();


			//	TopLoc_Location aLocation;
			//	Handle(Poly_Triangulation) aTr = BRep_Tool::Triangulation(aFace, aLocation);


			//	if (!aTr.IsNull())
			//	{
			//		const TColgp_Array1OfPnt& aNodes = aTr->Nodes();
			//		const Poly_Array1OfTriangle& triangles = aTr->Triangles();
			//		const TColgp_Array1OfPnt2d& uvNodes = aTr->UVNodes();


			//		TColgp_Array1OfPnt aPoints(1, aNodes.Length());
			//		for (Standard_Integer i = 1; i < aNodes.Length() + 1; i++)
			//			aPoints(i) = aNodes(i).Transformed(aLocation);


			//		Standard_Integer nnn = aTr->NbTriangles();
			//		Standard_Integer nt, n1, n2, n3;


			//		for (nt = 1; nt < nnn + 1; nt++)
			//		{
			//			triangles(nt).Get(n1, n2, n3);
			//			gp_Pnt aPnt1 = aPoints(n1);
			//			gp_Pnt aPnt2 = aPoints(n2);
			//			gp_Pnt aPnt3 = aPoints(n3);


			//			gp_Pnt2d uv1 = uvNodes(n1);
			//			gp_Pnt2d uv2 = uvNodes(n2);
			//			gp_Pnt2d uv3 = uvNodes(n3);


			//			vec3 p1, p2, p3;
			//			if (faceOrientation == TopAbs_Orientation::TopAbs_FORWARD)
			//			{
			//				p1 = carve::geom::VECTOR(aPnt1.X(), aPnt1.Y(), aPnt1.Z());
			//				p2 = carve::geom::VECTOR(aPnt2.X(), aPnt2.Y(), aPnt2.Z());
			//				p3 = carve::geom::VECTOR(aPnt3.X(), aPnt3.Y(), aPnt3.Z());
			//			}
			//			else
			//			{
			//				p1 = carve::geom::VECTOR(aPnt3.X(), aPnt3.Y(), aPnt3.Z());
			//				p2 = carve::geom::VECTOR(aPnt2.X(), aPnt2.Y(), aPnt2.Z());
			//				p3 = carve::geom::VECTOR(aPnt1.X(), aPnt1.Y(), aPnt1.Z());
			//			}



			//			vertices.push_back(p1);
			//			vertices.push_back(p2);
			//			vertices.push_back(p3);


			//			QVector3D dir1 = p2 - p1;
			//			QVector3D dir2 = p3 - p1;
			//			QVector3D normal = QVector3D::crossProduct(dir1, dir2);


			//			normals.push_back(normal);
			//			normals.push_back(normal);
			//			normals.push_back(normal);


			//			uvs2.push_back(QVector2D(uv1.X(), uv1.Y()));
			//			uvs2.push_back(QVector2D(uv2.X(), uv2.Y()));
			//			uvs2.push_back(QVector2D(uv3.X(), uv3.Y()));


			//			indices.push_back(idxCounter++);
			//			indices.push_back(idxCounter++);
			//			indices.push_back(idxCounter++);
			//		}
			//	}
			//}

			//	resultShapes.push_back(iter.Value());
		}

		std::map<std::string, std::string> mesh_input_options;
		shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(mesh_input_options, eps));

		result = meshset;
	}

	static void createSolid(const TopTools_ListOfShape& face_list, TopoDS_Shape& result, double epsilon, BuildingEntity* entity)
	{
		bool valid_shell = false;

		TopTools_ListIteratorOfListOfShape face_iterator;

		BRepOffsetAPI_Sewing builder;
		builder.SetTolerance(epsilon);
		builder.SetMaxTolerance(epsilon * 10);
		builder.SetMinTolerance(epsilon);
		for (face_iterator.Initialize(face_list); face_iterator.More(); face_iterator.Next())
		{
			builder.Add(face_iterator.Value());
		}

		try
		{
			builder.Perform();
			result = builder.SewedShape();
			valid_shell = BRepCheck_Analyzer(result).IsValid() != 0;
		}
		catch (Standard_Failure sf)
		{
		}
		catch (...)
		{
		}

		if (valid_shell)
		{
			TopAbs_ShapeEnum shapeType = result.ShapeType();
			if (shapeType == TopAbs_SOLID)
			{
				return;
			}

			for (TopExp_Explorer exp(result, TopAbs_SHELL); exp.More(); exp.Next())
			{
				TopoDS_Shape result_shape = exp.Current();
			}

			TopoDS_Shape complete_shape;
			for (TopExp_Explorer exp(result, TopAbs_SHELL); exp.More(); exp.Next())
			{
				TopoDS_Shape result_shape = exp.Current();

				try
				{
					ShapeFix_Solid solid;
					solid.LimitTolerance(epsilon);
					TopoDS_Solid solid_shape = solid.SolidFromShell(TopoDS::Shell(exp.Current()));
					if (!solid_shape.IsNull())
					{
						try
						{
							BRepClass3d_SolidClassifier classifier(solid_shape);
							result_shape = solid_shape;
							classifier.PerformInfinitePoint(epsilon);
							if (classifier.State() == TopAbs_IN)
							{
								result.Reverse();
							}
						}
						catch (Standard_Failure sf)
						{
							//messageCallback(sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, "", entity);
						}
						catch (...)
						{
						}
					}
				}
				catch (Standard_Failure sf)
				{
				}
				catch (...)
				{
				}

				if (complete_shape.IsNull())
				{
					complete_shape = result_shape;
				}
				else
				{
					BRep_Builder brep_builder;
					if (complete_shape.ShapeType() != TopAbs_COMPOUND)
					{
						TopoDS_Compound compound;
						brep_builder.MakeCompound(compound);
						brep_builder.Add(compound, complete_shape);
						complete_shape = compound;

						//messageCallback("Failed to connect faces", StatusCallback::MESSAGE_TYPE_MINOR_WARNING, "", entity);
					}
					brep_builder.Add(complete_shape, result_shape);
				}
			}

			if (!complete_shape.IsNull())
			{
				result = complete_shape;
			}
		}
		else
		{
			//messageCallback("Failed to connect faces", StatusCallback::MESSAGE_TYPE_MINOR_WARNING, "", entity);
		}
	}

	static void convertCarve2OCC(const shared_ptr<carve::mesh::MeshSet<3> >& input, TopoDS_Shape& result, double epsilon)
	{
		for (const carve::mesh::Mesh<3>*mesh : input->meshes)
		{
			bool closedShell = mesh->isClosed();
			BRepBuilderAPI_MakeShell makeOpenShell;

			TopTools_ListOfShape list_of_shapes;

			for (const carve::mesh::Face<3>*face : mesh->faces)
			{


				TopoDS_Vertex previousVertex;
				BRepBuilderAPI_MakeWire mk_wire;

				carve::mesh::Edge<3>* edge = face->edge;
				for (size_t ii = 0; ii < face->n_edges; ++ii)
				{
					vec3& point1 = edge->v1()->v;
					vec3& point2 = edge->v2()->v;
					if (previousVertex.IsNull())
					{
						previousVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(point1.x, point1.y, point1.z));
					}

					try
					{
						TopoDS_Vertex currentVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(point2.x, point2.y, point2.z));
						mk_wire.Add(BRepBuilderAPI_MakeEdge(previousVertex, currentVertex));

						previousVertex = currentVertex;
						edge = edge->next;
					}
					catch (...)
					{
					}
				}
				mk_wire.Build();
				TopoDS_Wire wire = mk_wire.Wire();

				TopoDS_Face faceOCC = BRepBuilderAPI_MakeFace(wire);
				if (!faceOCC.IsNull())
				{
					list_of_shapes.Append(faceOCC);
				}
			}

			if (closedShell)
			{
				//BRepBuilderAPI_MakeSolid makeSolid;

				TopoDS_Shape shape;
				createSolid(list_of_shapes, shape, epsilon, nullptr);

				if (!shape.IsNull())
				{
					result = shape;
					return;
				}
			}

			try
			{
				TopoDS_Shell shell;
				BRep_Builder builder;
				builder.MakeShell(shell);

				TopTools_ListIteratorOfListOfShape face_iterator;
				for (face_iterator.Initialize(list_of_shapes); face_iterator.More(); face_iterator.Next())
				{
					builder.Add(shell, face_iterator.Value());
				}
				if (!shell.IsNull())
				{
					result = shell;
				}
			}
			catch (Standard_Failure sf)
			{
				std::cout << sf.GetMessageString() << std::endl;
			}
			catch (...)
			{
				std::cout << __FUNC__ << " failed" << std::endl;
			}
		}
	}

	static bool boolOpOCC(const TopoDS_Shape& operandA, const TopoDS_Shape& operandB, carve::csg::CSG::OP op, TopoDS_Shape& resultShape)
	{
		//TopoDS_Solid solidPos, solidNeg;
		//TopoDS_Shape resultShape;

		//resultShapes.clear();
		//if (!Aux_MakeTwoHalfSpaceByFace(trimFaceTool, solidPos, solidNeg))
		//	return false;
		try
		{
			if (op == carve::csg::CSG::A_MINUS_B)
			{
				resultShape = BRepAlgoAPI_Cut(operandA, operandB);
				//resultShapes.push_back(resultShape);
				return true;
			}

			if (op == carve::csg::CSG::UNION)
			{
				resultShape = BRepAlgoAPI_Fuse(operandA, operandB);
				//resultShapes.push_back(resultShape);
				return true;
			}

			if (op == carve::csg::CSG::INTERSECTION)
			{
				resultShape = BRepAlgoAPI_Section(operandA, operandB);
				//resultShapes.push_back(resultShape);
				return true;
			}

			if (op == carve::csg::CSG::B_MINUS_A)
			{
				resultShape = BRepAlgoAPI_Cut(operandB, operandA);
				//resultShapes.push_back(resultShape);
				return true;
			}

			if (resultShape.ShapeType() == TopAbs_COMPOUND)
			{
				//for (TopoDS_Iterator iter(resultShape); iter.More(); iter.Next())
				//	resultShapes.push_back(iter.Value());
			}
			//else
			//	resultShapes.push_back(resultShape);
			//resultShape = BRepAlgoAPI_Cut(shellSolid, solidNeg);
			//if (resultShape.ShapeType() == TopAbs_COMPOUND)
			//{
			//	for (TopoDS_Iterator iter(resultShape); iter.More(); iter.Next())
			//		resultShapes.push_back(iter.Value());
			//}
			//else
			//	resultShapes.push_back(resultShape);
		}
		catch (...)
		{
			//resultShapes.clear();
			return false;
		}

		return false;
	}
};
#endif
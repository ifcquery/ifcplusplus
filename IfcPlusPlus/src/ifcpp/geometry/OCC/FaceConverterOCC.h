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

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <BRepOffsetAPI_Sewing.hxx>
#include <Geom_Plane.hxx>
#include <ShapeFix_Solid.hxx>

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/IFC4/include/IfcCurveBoundedPlane.h>
#include <ifcpp/IFC4/include/IfcCurveBoundedSurface.h>
#include <ifcpp/IFC4/include/IfcCylindricalSurface.h>
#include <ifcpp/IFC4/include/IfcFace.h>
#include <ifcpp/IFC4/include/IfcFaceBound.h>
#include <ifcpp/IFC4/include/IfcPlane.h>
#include <ifcpp/IFC4/include/IfcRationalBSplineSurfaceWithKnots.h>
#include <ifcpp/IFC4/include/IfcRectangularTrimmedSurface.h>
#include <ifcpp/IFC4/include/IfcSurfaceOfLinearExtrusion.h>
#include <ifcpp/IFC4/include/IfcSurfaceOfRevolution.h>
#include <ifcpp/IFC4/include/IfcSweptSurface.h>

#include "GeometryInputDataOCC.h"
#include "CurveConverterOCC.h"
#include "SplineConverterOCC.h"

class FaceConverterOCC : public StatusCallback
{
public:
	enum ShellType { SHELL_TYPE_UNKONWN, OPEN_SHELL, CLOSED_SHELL };

	shared_ptr<GeometrySettings>	m_geom_settings;
	shared_ptr<UnitConverter>		m_unit_converter;
	shared_ptr<CurveConverterOCC>	m_curve_converter;
	shared_ptr<SplineConverterOCC>	m_spline_converter;

	FaceConverterOCC( shared_ptr<GeometrySettings>& gs, shared_ptr<UnitConverter>& uc, shared_ptr<CurveConverterOCC>& cc, shared_ptr<SplineConverterOCC>& sc )
		: m_geom_settings( gs ), m_unit_converter( uc ), m_curve_converter( cc ), m_spline_converter( sc )
	{
	}

	virtual ~FaceConverterOCC(){}

	void convertIfcSurface( const shared_ptr<IfcSurface>& surface, Handle_Geom_Surface& target_surface, TopoDS_Face& target_face )
	{
		//ENTITY IfcSurface ABSTRACT SUPERTYPE OF(ONEOF(IfcBoundedSurface, IfcElementarySurface, IfcSweptSurface))

		double length_factor = m_unit_converter->getLengthInMeterFactor();
		shared_ptr<IfcBoundedSurface> bounded_surface = dynamic_pointer_cast<IfcBoundedSurface>( surface );
		if( bounded_surface )
		{
			// ENTITY IfcBoundedSurface ABSTRACT SUPERTYPE OF(ONEOF(IfcBSplineSurface, IfcCurveBoundedPlane, IfcCurveBoundedSurface, IfcRectangularTrimmedSurface))
			if( dynamic_pointer_cast<IfcBSplineSurface>( bounded_surface ) )
			{
				if( dynamic_pointer_cast<IfcRationalBSplineSurfaceWithKnots>( bounded_surface ) )
				{
					shared_ptr<IfcRationalBSplineSurfaceWithKnots> nurbs_surface = dynamic_pointer_cast<IfcRationalBSplineSurfaceWithKnots>( bounded_surface );
					if( nurbs_surface )
					{
						std::cout << "IfcRationalBSplineSurfaceWithKnots not implemented" << std::endl;
					}
				}
			}
			else if( dynamic_pointer_cast<IfcCurveBoundedPlane>( bounded_surface ) )
			{
				// ENTITY IfcCurveBoundedPlane SUBTYPE OF IfcBoundedSurface;
				shared_ptr<IfcCurveBoundedPlane> curve_bounded_plane = dynamic_pointer_cast<IfcCurveBoundedPlane>( bounded_surface );
				gp_Trsf curve_bounded_plane_matrix;
				shared_ptr<IfcPlane>& basis_surface = curve_bounded_plane->m_BasisSurface;
				if( basis_surface )
				{
					shared_ptr<IfcAxis2Placement3D>& basis_surface_placement = basis_surface->m_Position;

					if( basis_surface_placement )
					{
						PlacementConverterOCC::convertIfcAxis2Placement3D( basis_surface_placement, length_factor, curve_bounded_plane_matrix );
					}
				}

				// convert outer boundary
				shared_ptr<IfcCurve>& outer_boundary = curve_bounded_plane->m_OuterBoundary;
				std::vector<shared_ptr<IfcCurve> >& vec_inner_boundaries = curve_bounded_plane->m_InnerBoundaries;			//optional
				TopoDS_Face poly_face;
				createFace( outer_boundary, vec_inner_boundaries, poly_face );
				GeomUtilsOCC::applyMatrixToShape( poly_face, curve_bounded_plane_matrix );
				target_face = poly_face;			
			}
			else if( dynamic_pointer_cast<IfcCurveBoundedSurface>( bounded_surface ) )
			{
				shared_ptr<IfcCurveBoundedSurface> curve_bounded_surface = dynamic_pointer_cast<IfcCurveBoundedSurface>( bounded_surface );
				shared_ptr<IfcSurface>& basis_surface = curve_bounded_surface->m_BasisSurface;
				if( basis_surface )
				{
					convertIfcSurface( basis_surface, target_surface, target_face );
				}


				//std::vector<shared_ptr<IfcBoundaryCurve> >& vec_boundaries = curve_bounded_surface->m_Boundaries;
				//bool implicit_outer = curve_bounded_surface->m_ImplicitOuter;

				// TODO: implement
#ifdef _DEBUG
				std::cout << "IfcCurveBoundedSurface boundaries not implemented." << std::endl;
#endif
			}
			else if( dynamic_pointer_cast<IfcRectangularTrimmedSurface>( bounded_surface ) )
			{
				shared_ptr<IfcRectangularTrimmedSurface> rectengular_trimmed_surface = dynamic_pointer_cast<IfcRectangularTrimmedSurface>( bounded_surface );

				shared_ptr<IfcSurface>& basis_surface = rectengular_trimmed_surface->m_BasisSurface;
				if( basis_surface )
				{
					convertIfcSurface( basis_surface, target_surface, target_face );
				}

				shared_ptr<IfcParameterValue>& u1 = rectengular_trimmed_surface->m_U1;
				shared_ptr<IfcParameterValue>& v1 = rectengular_trimmed_surface->m_V1;
				shared_ptr<IfcParameterValue>& u2 = rectengular_trimmed_surface->m_U2;
				shared_ptr<IfcParameterValue>& v2 = rectengular_trimmed_surface->m_V2;
				//bool u_sense = rectengular_trimmed_surface->m_Usense;
				//bool v_sense = rectengular_trimmed_surface->m_Vsense;
				if( target_surface )
				{
					if( u1 && v1 && u2 && v2 )
					{
						Standard_Real u1_value = u1->m_value;
						Standard_Real v1_value = v1->m_value;
						Standard_Real u2_value = u2->m_value;
						Standard_Real v2_value = v2->m_value;
						double tolerance = 0.001;
						target_face = BRepBuilderAPI_MakeFace( target_surface, u1_value, v1_value, u2_value, v2_value, tolerance );
					}
				}
			}
			return;
		}

		shared_ptr<IfcElementarySurface> elementary_surface = dynamic_pointer_cast<IfcElementarySurface>( surface );
		if( elementary_surface )
		{
			//ENTITY IfcElementarySurface	ABSTRACT SUPERTYPE OF(ONEOF(IfcCylindricalSurface, IfcPlane))
			shared_ptr<IfcAxis2Placement3D>& elementary_surface_placement = elementary_surface->m_Position;

			gp_Pln elementary_surface_plane;
			gp_Trsf elementary_surface_matrix;
			if( elementary_surface_placement )
			{
				PlacementConverterOCC::convertIfcAxis2Placement3D( elementary_surface_placement, length_factor, elementary_surface_plane );
				PlacementConverterOCC::convertIfcAxis2Placement3D( elementary_surface_placement, length_factor, elementary_surface_matrix );
			}

			target_surface = new Geom_Plane( elementary_surface_plane );

			shared_ptr<IfcPlane> ifc_elementary_surface_plane = dynamic_pointer_cast<IfcPlane>( elementary_surface );
			if( ifc_elementary_surface_plane )
			{
				//  1----0     create big rectangular plane
				//  |    |     ^ y
				//  |    |     |
				//  2----3     ---> x
				{
					double plane_span = HALF_SPACE_BOX_SIZE;
					target_face = BRepBuilderAPI_MakeFace( elementary_surface_plane, -plane_span, plane_span, -plane_span, plane_span );
				}
				return;
			}

			shared_ptr<IfcCylindricalSurface> cylindrical_surface = dynamic_pointer_cast<IfcCylindricalSurface>( elementary_surface );
			if( cylindrical_surface )
			{
				shared_ptr<IfcPositiveLengthMeasure> cylindrical_surface_radius = cylindrical_surface->m_Radius;
				double circle_radius = cylindrical_surface_radius->m_value;

				gp_Dir dir( 0, 0, 1 );
				gp_Pnt point( 0, 0, 0 );
				point.Transform( elementary_surface_matrix );
				gp_Circ circle( gp_Ax2( point, dir ), circle_radius );
				BRepBuilderAPI_MakeWire mk_wire;
				mk_wire.Add( BRepBuilderAPI_MakeEdge( circle ) );
				TopoDS_Wire circle_wire = mk_wire.Wire();
				target_face = BRepBuilderAPI_MakeFace( circle_wire );
				return;
			}

			throw UnhandledRepresentationException( surface );
		}

		shared_ptr<IfcSweptSurface> swept_surface = dynamic_pointer_cast<IfcSweptSurface>( surface );
		if( dynamic_pointer_cast<IfcSweptSurface>( surface ) )
		{
			// ENTITY IfcSweptSurface	ABSTRACT SUPERTYPE OF(ONEOF(IfcSurfaceOfLinearExtrusion, IfcSurfaceOfRevolution))
			//shared_ptr<IfcProfileDef>& swept_surface_profile = swept_surface->m_SweptCurve;
			shared_ptr<IfcAxis2Placement3D>& swept_surface_placement = swept_surface->m_Position;

			gp_Trsf swept_surface_matrix;
			if( swept_surface_placement )
			{
				PlacementConverterOCC::convertIfcAxis2Placement3D( swept_surface_placement, length_factor, swept_surface_matrix );
				//swept_surface_matrix = pos*swept_surface_matrix;
			}

			shared_ptr<IfcSurfaceOfLinearExtrusion> linear_extrusion = dynamic_pointer_cast<IfcSurfaceOfLinearExtrusion>( swept_surface );
			if( linear_extrusion )
			{
				//shared_ptr<IfcDirection>& linear_extrusion_direction = linear_extrusion->m_ExtrudedDirection;
				//shared_ptr<IfcLengthMeasure>& linear_extrusion_depth = linear_extrusion->m_Depth;
				// TODO: implement
#ifdef _DEBUG
				std::cout << "IfcSurfaceOfLinearExtrusion not implemented." << std::endl;
#endif
				return;
			}

			shared_ptr<IfcSurfaceOfRevolution> suface_of_revolution = dynamic_pointer_cast<IfcSurfaceOfRevolution>( swept_surface );
			if( suface_of_revolution )
			{
				// TODO: implement
#ifdef _DEBUG
				std::cout << "IfcSurfaceOfRevolution not implemented." << std::endl;
#endif
				return;
			}

			throw UnhandledRepresentationException( surface );
		}
		throw UnhandledRepresentationException( surface );
	}

	void createSolid( const TopTools_ListOfShape& face_list, TopoDS_Shape& shape, BuildingEntity* entity )
	{
		bool valid_shell = false;

		TopTools_ListIteratorOfListOfShape face_iterator;

		BRepOffsetAPI_Sewing builder;
		builder.SetTolerance( GEOM_TOLERANCE );
		builder.SetMaxTolerance( GEOM_TOLERANCE );
		builder.SetMinTolerance( GEOM_TOLERANCE );
		for( face_iterator.Initialize( face_list ); face_iterator.More(); face_iterator.Next() )
		{
			builder.Add( face_iterator.Value() );
		}

		try
		{
			builder.Perform();
			shape = builder.SewedShape();
			valid_shell = BRepCheck_Analyzer( shape ).IsValid() != 0;
		}
		catch( Standard_Failure sf )
		{
		}
		catch( ... )
		{
		}

		if( valid_shell )
		{
			TopoDS_Shape complete_shape;
			for( TopExp_Explorer exp( shape, TopAbs_SHELL ); exp.More(); exp.Next() )
			{
				TopoDS_Shape result_shape = exp.Current();

				try
				{
					ShapeFix_Solid solid;
					solid.LimitTolerance( GEOM_TOLERANCE );
					TopoDS_Solid solid_shape = solid.SolidFromShell( TopoDS::Shell( exp.Current() ) );
					if( !solid_shape.IsNull() )
					{
						try
						{
							BRepClass3d_SolidClassifier classifier( solid_shape );
							result_shape = solid_shape;
							classifier.PerformInfinitePoint( GEOM_TOLERANCE );
							if( classifier.State() == TopAbs_IN )
							{
								shape.Reverse();
							}
						}
						catch( Standard_Failure sf )
						{
							messageCallback( sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, "", entity );
						}
						catch( ... )
						{
						}
					}
				}
				catch( Standard_Failure sf )
				{
				}
				catch( ... )
				{
				}

				if( complete_shape.IsNull() )
				{
					complete_shape = result_shape;
				}
				else
				{
					BRep_Builder brep_builder;
					if( complete_shape.ShapeType() != TopAbs_COMPOUND )
					{
						TopoDS_Compound compound;
						brep_builder.MakeCompound( compound );
						brep_builder.Add( compound, complete_shape );
						complete_shape = compound;

						messageCallback( "Failed to connect faces", StatusCallback::MESSAGE_TYPE_MINOR_WARNING, "", entity );
					}
					brep_builder.Add( complete_shape, result_shape );
				}
			}
			shape = complete_shape;
		}
		else
		{
			messageCallback( "Failed to connect faces", StatusCallback::MESSAGE_TYPE_MINOR_WARNING, "", entity );
		}
	}

	void createFace( const shared_ptr<IfcCurve>& outer_boundary, const std::vector<shared_ptr<IfcCurve> >& vec_inner_boundaries, TopoDS_Face& target_face )
	{
		try
		{
			TopoDS_Wire outer_boundary_wire;
			m_curve_converter->convertIfcCurve( outer_boundary, outer_boundary_wire );
			BRepBuilderAPI_MakeFace mk_face( outer_boundary_wire );

			// convert inner boundaries
			for( auto& inner_boundary : vec_inner_boundaries )
			{
				if( !inner_boundary )
				{
					continue;
				}

				TopoDS_Wire inner_boundary_wire;
				m_curve_converter->convertIfcCurve( inner_boundary, inner_boundary_wire );
				if( !inner_boundary_wire.IsNull() )
				{
					mk_face.Add( TopoDS::Wire( inner_boundary_wire.Reversed() ) );
				}
			}

			target_face = mk_face.Face();
		}
		catch( Standard_Failure& sf )
		{
			messageCallback( sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_ERROR, "", outer_boundary.get() );
		}
	}

	void convertIfcFaceList( const std::vector<shared_ptr<IfcFace> >& vec_faces, shared_ptr<ItemShapeDataOCC> item_data, ShellType st )
	{
		TopTools_ListOfShape list_of_shapes;
		BuildingEntity* report_entity = nullptr;

		for( const shared_ptr<IfcFace>& ifc_face : vec_faces )
		{
			if( !ifc_face )
			{
				continue;
			}
			const std::vector<shared_ptr<IfcFaceBound> >& vec_bounds = ifc_face->m_Bounds;
			report_entity = ifc_face.get();

			try
			{
				TopoDS_Face face;
				
				for( size_t ii_bounds = 0; ii_bounds < vec_bounds.size(); ++ii_bounds )
				{
					const shared_ptr<IfcFaceBound>& face_bound = vec_bounds[ii_bounds];

					if( !face_bound )
					{
						continue;
					}

					// ENTITY IfcLoop SUPERTYPE OF(ONEOF(IfcEdgeLoop, IfcPolyLoop, IfcVertexLoop))
					const shared_ptr<IfcLoop>& loop = face_bound->m_Bound;
					if( !loop )
					{
						if( ii_bounds == 0 )
						{
							break;
						}
						else
						{
							continue;
						}
					}

					TopoDS_Wire loop_wire;
					m_curve_converter->convertIfcLoop( loop, loop_wire );

					if( loop_wire.IsNull() )
					{
						if( ii_bounds == 0 )
						{
							// no valid outer boundary
							break;
						}
						else
						{
							// skip inner boundary
							continue;
						}
					}

					bool orientation = true;
					if( face_bound->m_Orientation )
					{
						orientation = face_bound->m_Orientation->m_value;
					}
					if( !orientation )
					{
						loop_wire.Reverse();
					}

					if( !loop_wire.IsNull() )
					{
						if( face.IsNull() )
						{
							face = BRepBuilderAPI_MakeFace( loop_wire );
						}
						else
						{
							BRepBuilderAPI_MakeFace mk_face( face );
							mk_face.Add( loop_wire );
							mk_face.Build();
							if( mk_face.IsDone() )
							{
								face = mk_face.Face();
							}
						}
					}
				}

				if( !face.IsNull() )
				{
					list_of_shapes.Append( face );
				}
			}
			catch( Standard_Failure sf )
			{
				std::cout << sf.GetMessageString() << std::endl;
			}
			catch( ... )
			{
				std::cout << __FUNC__ << " failed" << std::endl;
			}
		}

		// IfcFaceList can be a closed or open shell
		if( st == SHELL_TYPE_UNKONWN || st == OPEN_SHELL )
		{
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
					item_data->addShape(shell);
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
		else if( st == CLOSED_SHELL )
		{
			TopoDS_Shape shape;
			createSolid( list_of_shapes, shape, report_entity );

			if( !shape.IsNull() )
			{
				item_data->addShape( shape );
			}
		}
	}
};

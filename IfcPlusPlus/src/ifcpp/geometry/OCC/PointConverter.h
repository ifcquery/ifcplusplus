/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#pragma once

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepFilletAPI_MakeFillet2d.hxx>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcCurve.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLoop.h>
#include <ifcpp/IFC4/include/IfcPointOnCurve.h>
#include <ifcpp/IFC4/include/IfcPointOnSurface.h>
#include <ifcpp/IFC4/include/IfcPolyline.h>
#include <ifcpp/IFC4/include/IfcReal.h>
#include <ifcpp/IFC4/include/IfcTrimmingSelect.h>
#include <ifcpp/IFC4/include/IfcVertex.h>
#include <ifcpp/IFC4/include/IfcVertexPoint.h>

#include "GeomUtils.h"
#include "GeometrySettings.h"

//\brief class to convert IFC point representations to OCC
class PointConverter
{
public:
	static bool convertIfcCartesianPoint( const shared_ptr<IfcCartesianPoint>& ifc_point, gp_Vec& point, double length_factor )
	{
		if( !ifc_point )
		{
			return false;
		}
		std::vector<shared_ptr<IfcLengthMeasure> >& coords1 = ifc_point->m_Coordinates;
		if( coords1.size() > 2 )
		{
#ifdef ROUND_IFC_COORDINATES
			double x = round( coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
			double y = round( coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
			double z = round( coords1[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
			double x = coords1[0]->m_value*length_factor;
			double y = coords1[1]->m_value*length_factor;
			double z = coords1[2]->m_value*length_factor;
#endif
			point = gp_Vec( x, y, z );
			return true;
		}
		else if( coords1.size() > 1 )
		{
#ifdef ROUND_IFC_COORDINATES
			double x = round( coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
			double y = round( coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
			double x = coords1[0]->m_value*length_factor;
			double y = coords1[1]->m_value*length_factor;
#endif
			point = gp_Vec( x, y, 0.0 );
			return true;
		}
		return false;
	}

	static bool convertIfcDirection( const shared_ptr<IfcDirection>& ifc_direction, gp_Vec& direction_vector )
	{
		if( !ifc_direction )
		{
			return false;
		}

		const std::vector<shared_ptr<IfcReal> >& direction_ratios = ifc_direction->m_DirectionRatios;
		if( !GeomUtils::allPointersValid( direction_ratios ) )
		{
			return false;
		}
		if( direction_ratios.size() < 2 )
		{
			return false;
		}
		
		if( direction_ratios.size() > 2 )
		{
				direction_vector = gp_Vec( direction_ratios[0]->m_value, direction_ratios[1]->m_value, direction_ratios[2]->m_value );
		}
		else
		{
			direction_vector = gp_Vec( direction_ratios[0]->m_value, direction_ratios[1]->m_value, 0 );
		}
		direction_vector.Normalize();
		return true;
	}

	static void createWireFromPoints( const std::vector<vec2>& coordinates, TopoDS_Wire& wire, bool close_wire_with_first_point )
	{
		std::vector<TopoDS_Vertex> vertices;
		for( size_t ii = 0; ii < coordinates.size(); ++ii )
		{
			const vec2& vec = coordinates[ii];
			vertices.push_back( BRepBuilderAPI_MakeVertex( gp_Pnt( vec.X(), vec.Y(), 0 ) ) );
		}

		BRepBuilderAPI_MakeWire mk_wire;
		for( size_t ii = 0; ii < vertices.size(); ii++ )
		{
			size_t idx_next = (ii + 1) % vertices.size();
			TopoDS_Vertex& vertex_ii = vertices[ii];
			TopoDS_Vertex& vertex_next = vertices[idx_next];

			if( ii == vertices.size() - 1 )
			{
				if( !close_wire_with_first_point )
				{
					break;
				}
			}

			if( coordinates.size() == vertices.size() )
			{
				const vec2& vec = coordinates[ii];
				const vec2& vec_next = coordinates[idx_next];
				if( GeomUtils::equal( vec, vec_next ) )
				{
					// points are equal, length of edge would be zero
					continue;
				}
			}

			if( vertex_ii.IsNull() )
			{
				continue;
			}

			if( vertex_next.IsNull() )
			{
				continue;
			}

			BRepBuilderAPI_MakeEdge mk_edge( vertex_ii, vertex_next );
			if( !mk_edge.IsDone() )
			{
				continue;
			}
			mk_wire.Add( mk_edge );
		}
		wire = mk_wire.Wire();
	}

	static void createWireFromPoints( const std::vector<gp_Vec>& coordinates, TopoDS_Wire& wire, bool close_wire_with_first_point )
	{
		std::vector<TopoDS_Vertex> vertices;
		for( size_t ii = 0; ii < coordinates.size(); ++ii )
		{
			const gp_Vec& vec = coordinates[ii];
			vertices.push_back( BRepBuilderAPI_MakeVertex( gp_Pnt( vec.X(), vec.Y(), vec.Z() ) ) );
		}

		BRepBuilderAPI_MakeWire mk_wire;
		for( size_t ii = 0; ii < vertices.size(); ii++ )
		{
			size_t idx_next = (ii + 1) % vertices.size();
			TopoDS_Vertex& vertex_ii = vertices[ii];
			TopoDS_Vertex& vertex_next = vertices[idx_next];

			if( ii == vertices.size() - 1 )
			{
				if( !close_wire_with_first_point )
				{
					break;
				}
			}

			if( coordinates.size() == vertices.size() )
			{
				const gp_Vec& vec = coordinates[ii];
				const gp_Vec& vec_next = coordinates[idx_next];
				if( GeomUtils::equal( vec, vec_next ) )
				{
					// points are equal, length of edge would be zero
					continue;
				}
			}

			if( vertex_ii.IsNull() )
			{
				continue;
			}

			if( vertex_next.IsNull() )
			{
				continue;
			}

			BRepBuilderAPI_MakeEdge mk_edge( vertex_ii, vertex_next );
			if( !mk_edge.IsDone() )
			{
				continue;
			}
			mk_wire.Add( mk_edge );
		}
		wire = mk_wire.Wire();
	}
	static void createFaceFromPoints( const std::vector<vec2>& coordinates, TopoDS_Face& face )
	{
		std::vector<TopoDS_Vertex> vertices;
		for( size_t ii = 0; ii < coordinates.size(); ++ii )
		{
			const vec2& vec = coordinates[ii];
			vertices.push_back( BRepBuilderAPI_MakeVertex( gp_Pnt( vec.X(), vec.Y(), 0.0f ) ) );
		}

		BRepBuilderAPI_MakeWire mk_wire;
		for( size_t ii = 0; ii < vertices.size(); ii++ )
		{
			size_t idx_next = (ii + 1) % vertices.size();
			TopoDS_Vertex& vertex_ii = vertices[ii];
			TopoDS_Vertex& vertex_next = vertices[idx_next];

			if( coordinates.size() == vertices.size() )
			{
				const vec2& vec = coordinates[ii];
				const vec2& vec_next = coordinates[idx_next];
				if( GeomUtils::equal( vec, vec_next ) )
				{
					// points are equal, length of edge would be zero
					continue;
				}
			}

			if( vertex_ii.IsNull() )
			{
				continue;
			}
			if( vertex_next.IsNull() )
			{
				continue;
			}
			BRepBuilderAPI_MakeEdge mk_edge( vertex_ii, vertex_next );
			if( !mk_edge.IsDone() )
			{
				continue;
			}
			mk_wire.Add( mk_edge );
		}

		TopoDS_Wire wire = mk_wire.Wire();

		BRepBuilderAPI_MakeFace mk_face( wire, false );
		face = mk_face.Face();
	}

	static void createFaceFromPoints( const std::vector<TangentialPoint2D>& tangential_points, TopoDS_Face& face )
	{
		std::vector<TopoDS_Vertex> vertices;
		bool have_radius = false;
		for( size_t ii = 0; ii < tangential_points.size(); ++ii )
		{
			const TangentialPoint2D& tangent_point = tangential_points[ii];
			const vec2& vec = tangent_point.m_coords;
			const double radius = tangent_point.m_radius;
			if( abs( radius ) > 0.0001 )
			{
				have_radius = true;
			}
			vertices.push_back( BRepBuilderAPI_MakeVertex( gp_Pnt( vec.X(), vec.Y(), 0.0f ) ) );
		}

		TopoDS_Edge previous_edge;
		BRepBuilderAPI_MakeWire mk_wire;
		for( size_t ii = 0; ii < vertices.size(); ii++ )
		{
			size_t idx_next = (ii + 1) % vertices.size();
			TopoDS_Vertex& vertex_ii = vertices[ii];
			TopoDS_Vertex& vertex_next = vertices[idx_next];

			if( tangential_points.size() == vertices.size() )
			{
				const TangentialPoint2D& tangent_point = tangential_points[ii];
				const TangentialPoint2D& tangent_point_next = tangential_points[idx_next];
				const vec2& vec = tangent_point.m_coords;
				const vec2& vec_next = tangent_point_next.m_coords;
				if( GeomUtils::equal( vec, vec_next ) )
				{
					// points are equal, length of edge would be zero
					continue;
				}
			}

			if( vertex_ii.IsNull() )
			{
				continue;
			}
			if( vertex_next.IsNull() )
			{
				continue;
			}
			BRepBuilderAPI_MakeEdge mk_edge( vertex_ii, vertex_next );
			if( !mk_edge.IsDone() )
			{
				continue;
			}

			mk_wire.Add( mk_edge );
		}

		TopoDS_Wire wire = mk_wire.Wire();

		BRepBuilderAPI_MakeFace mk_face( wire, false );
		face = mk_face.Face();

		if( have_radius )
		{
			BRepFilletAPI_MakeFillet2d fillet( face );

			for( size_t ii = 0; ii < tangential_points.size(); ++ii )
			{
				const double radius = tangential_points[ii].m_radius;
				if( abs( radius ) > 0.0001 )
				{
					fillet.AddFillet( vertices[ii], abs( radius ) );
				}
			}
			fillet.Build();
			if( fillet.IsDone() )
			{
				face = TopoDS::Face( fillet.Shape() );
			}
			else
			{
				std::cout << "Failed to process profile fillets";
			}
		}
	}
	static void convertIfcCartesianPointVector( const std::vector<shared_ptr<IfcCartesianPoint> >& points, TopoDS_Wire& target_wire, double length_factor )
	{
		const size_t num_points = points.size();
		std::vector<vec3> vec_vertices;
		for( size_t i_point = 0; i_point < num_points; ++i_point )
		{
			const shared_ptr<IfcCartesianPoint>& ifc_cartesian_point = points[i_point];
			if( !ifc_cartesian_point )
			{
				continue;
			}

			const std::vector<shared_ptr<IfcLengthMeasure> >& coords = ifc_cartesian_point->m_Coordinates;
			if( coords.size() > 2 )
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double z = round( coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords[0]->m_value*length_factor;
				double y = coords[1]->m_value*length_factor;
				double z = coords[2]->m_value*length_factor;
#endif
				vec_vertices.push_back( vec3( x, y, z ) );
			}
			else if( coords.size() > 1 )
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords[0]->m_value*length_factor;
				double y = coords[1]->m_value*length_factor;
#endif
				vec_vertices.push_back( vec3( x, y, 0.0 ) );
			}
			else
			{
#ifdef _DEBUG
				std::cout << "convertIfcCartesianPointVector: ifc_pt->m_Coordinates.size() != 2" << std::endl;
#endif
			}
		}

		createWireFromPoints( vec_vertices, target_wire, false );
	}
	static void convertIfcCartesianPointVector( const std::vector<shared_ptr<IfcCartesianPoint> >& points, std::vector<gp_Vec>& loop, double length_factor )
	{
		const size_t num_points = points.size();
		for( size_t i_point = 0; i_point < num_points; ++i_point )
		{
			const shared_ptr<IfcCartesianPoint>& ifc_cartesian_point = points[i_point];
			if( !ifc_cartesian_point )
			{
				continue;
			}

			const std::vector<shared_ptr<IfcLengthMeasure> >& coords = ifc_cartesian_point->m_Coordinates;
			if( coords.size() > 2 )
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double z = round( coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords[0]->m_value*length_factor;
				double y = coords[1]->m_value*length_factor;
				double z = coords[2]->m_value*length_factor;
#endif
				loop.push_back( gp_Vec( x, y, z ) );
			}
			else if( coords.size() > 1 )
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords[0]->m_value*length_factor;
				double y = coords[1]->m_value*length_factor;
#endif
				loop.push_back( gp_Vec( x, y, 0.0 ) );
			}
			else
			{
#ifdef _DEBUG
				std::cout << "convertIfcCartesianPointVector: ifc_pt->m_Coordinates.size() != 2" << std::endl;
#endif
			}
		}
	}
	static void convertIfcCartesianPointVector2D( const std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& vec_points_in, std::vector<gp_Vec>& vertices, double length_factor )
	{
		for( size_t ii = 0; ii < vec_points_in.size(); ++ii )
		{
			const std::vector<shared_ptr<IfcCartesianPoint> >& points_inner = vec_points_in[ii];
			for( size_t jj = 0; jj < points_inner.size(); ++jj )
			{
				const shared_ptr<IfcCartesianPoint>& cartesian_point = points_inner[jj];

				if( !cartesian_point )
				{
					continue;
				}

				const std::vector<shared_ptr<IfcLengthMeasure> >& coords = cartesian_point->m_Coordinates;
				if( coords.size() > 2 )
				{
					vertices.push_back( gp_Vec( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, coords[2]->m_value*length_factor ) );
				}
				else if( coords.size() > 1 )
				{
					vertices.push_back( gp_Vec( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 ) );
				}
			}
		}
	}
	static void convertIfcCartesianPointVectorSkipDuplicates( const std::vector<shared_ptr<IfcCartesianPoint> >& vec_ifc_points, TopoDS_Wire& wire, bool close_wire_with_first_point, double length_factor )
	{
		std::vector<gp_Vec> vec_points;
		double previous_x = 0;
		double previous_y = 0;
		double previous_z = 0;
		for( size_t ii = 0; ii < vec_ifc_points.size(); ++ii )
		{
			const shared_ptr<IfcCartesianPoint>& cartesian_point = vec_ifc_points[ii];
			double x = 0.0, y = 0.0, z = 0.0;
			const std::vector<shared_ptr<IfcLengthMeasure> >& coords = cartesian_point->m_Coordinates;

			if( coords.size() > 2 )
			{
#ifdef ROUND_IFC_COORDINATES
				x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				z = round( coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				x = coords[0]->m_value*length_factor;
				y = coords[1]->m_value*length_factor;
				z = coords[2]->m_value*length_factor;
#endif
			}
			else if( coords.size() > 1 )
			{
#ifdef ROUND_IFC_COORDINATES
				x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				x = coords[0]->m_value*length_factor;
				y = coords[1]->m_value*length_factor;
#endif
			}

			// skip duplicate vertices
			if( ii > 0 )
			{
				if( std::abs( x - previous_x ) < 0.00000001 )
				{
					if( std::abs( y - previous_y ) < 0.00000001 )
					{
						if( std::abs( z - previous_z ) < 0.00000001 )
						{
							continue;
						}
					}
				}
			}
			vec_points.push_back( gp_Vec( x, y, z ) );
			previous_x = x;
			previous_y = y;
			previous_z = z;
		}
		createWireFromPoints( vec_points, wire, close_wire_with_first_point );
	}
	static bool convertIfcVertex( const shared_ptr<IfcVertex>& vertex, gp_Vec& point_result, const double length_factor )
	{
		shared_ptr<IfcVertexPoint> vertex_point = dynamic_pointer_cast<IfcVertexPoint>( vertex );
		if( vertex_point )
		{
			if( vertex_point->m_VertexGeometry )
			{
				const shared_ptr<IfcPoint>& vertex_point_geometry = vertex_point->m_VertexGeometry;
				// ENTITY IfcPoint ABSTRACT SUPERTYPE OF(ONEOF(IfcCartesianPoint, IfcPointOnCurve, IfcPointOnSurface))
				shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>( vertex_point_geometry );
				if( cartesian_point )
				{
					convertIfcCartesianPoint( cartesian_point, point_result, length_factor );
					return true;
				}

				shared_ptr<IfcPointOnCurve> point_on_curve = dynamic_pointer_cast<IfcPointOnCurve>(vertex_point_geometry);
				if( point_on_curve )
				{
					std::cout << "IfcVertexPoint as IfcPointOnCurve not implemented" << std::endl;
					return false;
				}

				shared_ptr<IfcPointOnSurface> point_on_surface = dynamic_pointer_cast<IfcPointOnSurface>(vertex_point_geometry);
				if( point_on_surface )
				{
					std::cout << "IfcVertexPoint as IfcPointOnSurface not implemented" << std::endl;
					return false;
				}
			}
		}
		return false;
	}
};

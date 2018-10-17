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

#include <vector>
#include <Bnd_Box.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepFilletAPI_MakeFillet2d.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <gp_Vec.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <ShapeBuild_Edge.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeFix_EdgeConnect.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Wire.hxx>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>

#define GEOM_EPSILON_LENGTH 1.4901161193847656e-08
#define GEOM_EPSILON_CSG_FUZZY 0.0001
#define GEOM_EPSILON_PROFILE_SIZE 0.00000001

struct TangentialPoint2D
{
	/**
	* \brief struct TangentialPoint: Tangential point to create wires with straight segments and fillets
	* @param in coords: coordinates of tangential point.
	* @param radius: radius at that point. If radius = 0, no fillet is created. If radius < 0, a left turn is created, if radius > 0, a right turn is created
	*/
	TangentialPoint2D( gp_Vec2d coords, double radius )
	{
		m_coords = coords;
		m_radius = radius;
	}
	gp_Vec2d m_coords;
	double m_radius;
};

namespace GeomUtilsOCC
{
	class Ray
	{
	public:
		void setRay( Ray* other )
		{
			origin = other->origin;
			direction = other->direction;
		}
		gp_Vec origin;
		gp_Vec direction;
	};
	class Plane
	{
	public:
		Plane() {}
		Plane( gp_Vec& position, gp_Vec& normal )
		{
			m_position = position;
			m_normal = normal;
		}
		bool intersectRay( const Ray* ray, bool intersect_in_both_directions, gp_Vec& intersect_point ) const
		{
			gp_Vec plane_pos( m_position );
			gp_Dir plane_normal( m_normal.X(), m_normal.Y(), m_normal.Z() );
			//gp_Pln plane = gp_Pln( plane_pos, plane_normal );

			gp_Vec ray_origin( ray->origin.X(), ray->origin.Y(), ray->origin.Z() );
			gp_Dir ray_direction( ray->direction.X(), ray->direction.Y(), ray->direction.Z() );

			double denom = plane_normal.Dot( ray_direction );
			double abs_denom = abs( denom );
			if( abs_denom < 0.0001 )
			{
				return false;
			}

			double t = (plane_pos - ray_origin).Dot( plane_normal ) / denom;
			if( !intersect_in_both_directions )
			{
				if( t < -0.0001 )
				{
					return false;
				}
			}
			intersect_point = ray_origin + t * ray_direction;
			return true;
		}

		/** distance point to plane */
		double distancePointPlane( const gp_Vec& point )
		{
			return m_normal*(point - m_position);
		}

		gp_Vec m_position;
		gp_Vec m_normal;
	};

	/** polygon operations */
	static inline bool equal( const gp_Vec2d& pt1, const gp_Vec2d& pt2, const double tolerance = GEOM_EPSILON_LENGTH )
	{
		// comparing square distance saves a call to sqrt
		double distance_square = (pt2-pt1).SquareMagnitude();
		if( distance_square <= tolerance*tolerance )
		{
			return true;
		}
		return false;
	}
	static inline bool equal( const gp_Vec& pt1, const gp_Vec& pt2, const double tolerance = GEOM_EPSILON_LENGTH )
	{
		// comparing square distance saves a call to sqrt
		double distance_square = (pt2-pt1).SquareMagnitude();
		if( distance_square <= tolerance*tolerance )
		{
			return true;
		}
		return false;
	}
	static inline bool isZero( const gp_Vec2d& vec, double epsilon = GEOM_EPSILON_LENGTH )
	{
		return (vec.X()*vec.X() + vec.Y()*vec.Y()) < epsilon * epsilon;
	}

	inline gp_Vec computePolygonCentroid( const std::vector<gp_Vec>& polygon )
	{
		gp_Vec polygon_centroid( 0, 0, 0 );
		for( std::vector<gp_Vec>::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
		{
			const gp_Vec& vertex_current = ( *it );
			polygon_centroid += vertex_current;
		}
		polygon_centroid /= (double)( polygon.size() );
		return polygon_centroid;
	}
	inline gp_Vec2d computePolygonCentroid( const std::vector<gp_Vec2d>& polygon )
	{
		gp_Vec2d polygon_centroid( 0, 0 );
		for( std::vector<gp_Vec2d>::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
		{
			const gp_Vec2d& vertex_current = ( *it );
			polygon_centroid += vertex_current;
		}
		polygon_centroid /= (double)( polygon.size() );
		return polygon_centroid;
	}
	inline gp_Vec computePolygonNormal( const std::vector<gp_Vec>& polygon )
	{
		gp_Vec polygon_normal( 0, 0, 0 );
		bool last_loop = false;
		for( std::vector<gp_Vec>::const_iterator it = polygon.begin();; )
		{
			const gp_Vec& vertex_current = ( *it );
			++it;
			if( it == polygon.end() )
			{
				it = polygon.begin();
				last_loop = true;
			}
			const gp_Vec& vertex_next = ( *it );
			polygon_normal.SetX( polygon_normal.X() + ( vertex_current.Y() - vertex_next.Y())*( vertex_current.Z() + vertex_next.Z()) );
			polygon_normal.SetY( polygon_normal.Y() + ( vertex_current.Z() - vertex_next.Z())*( vertex_current.X() + vertex_next.X()) );
			polygon_normal.SetZ( polygon_normal.Z() + ( vertex_current.X() - vertex_next.X())*( vertex_current.Y() + vertex_next.Y()) );
			if( last_loop )
			{
				break;
			}
		}
		polygon_normal.Normalize();
		return polygon_normal;
	}
	inline gp_Vec computePolygon2DNormal( const std::vector<gp_Vec2d>& polygon )
	{
		const int num_points = polygon.size();
		gp_Vec polygon_normal( 0, 0, 0 );
		for( int k = 0; k < num_points; ++k )
		{
			const gp_Vec2d& vertex_current = polygon[k];
			const gp_Vec2d& vertex_next = polygon[( k + 1 ) % num_points];
			polygon_normal.SetZ( polygon_normal.Z() + (vertex_current.X() - vertex_next.X())*(vertex_current.Y() + vertex_next.Y()) );// [2] += ( vertex_current.x - vertex_next.x )*( vertex_current.y + vertex_next.y );
		}
		polygon_normal.Normalize();
		return polygon_normal;
	}
	inline gp_Vec computeFaceNormal( const TopoDS_Face& face )
	{
		BRepAdaptor_Surface surface( face );
		Standard_Real u1, u2, v1, v2;
		u1 = surface.FirstUParameter();
		u2 = surface.LastUParameter();
		v1 = surface.FirstVParameter();
		v2 = surface.LastVParameter();

		gp_Pnt face_center_point;
		gp_Vec first_vec_on_face, second_vec_on_face, face_normal;

		surface.D1( (u1+u2)/2, (v1+v2)/2, face_center_point, first_vec_on_face, second_vec_on_face );

		face_normal = first_vec_on_face^second_vec_on_face;
		return face_normal;
	}
	inline bool checkOpenPolygonConvexity( const std::vector<gp_Vec2d>& polygon )
	{
		if( polygon.size() < 3 )
		{
			return true;
		}
		const int num_points = polygon.size();
		double zcrossproduct_previous = 0;
		for( int k = 0; k < num_points - 2; ++k )
		{
			const gp_Vec2d& vertex_current = polygon[k];
			const gp_Vec2d& vertex_next1 = polygon[k + 1];
			const gp_Vec2d& vertex_next2 = polygon[k + 2];

			double dx1 = vertex_next1.X() - vertex_current.X();
			double dy1 = vertex_next1.Y() - vertex_current.Y();

			double dx2 = vertex_next2.X() - vertex_next1.X();
			double dy2 = vertex_next2.Y() - vertex_next1.Y();

			double zcrossproduct = dx1*dy2 - dy1*dx2;

			if( k > 0 )
			{
				if( std::abs( zcrossproduct ) > 0.0001 )
				{
					if( std::abs( zcrossproduct_previous ) > 0.0001 )
					{
						if( zcrossproduct*zcrossproduct_previous < 0 )
						{
							// there is a change in direction -> not convex
							return false;
						}
					}
				}
			}
			zcrossproduct_previous = zcrossproduct;
		}
		return true;
	}
	inline void createVector2DWithRadius( const std::vector<gp_Vec2d>& vec_points_in, std::vector<TangentialPoint2D>& vec_out )
	{
		for( size_t ii = 0; ii < vec_points_in.size(); ++ii )
		{
			const gp_Vec2d& point = vec_points_in[ii];
			vec_out.push_back( TangentialPoint2D( point, 0.0 ) );
		}
	}
	inline void appendEdgeToWire( const TopoDS_Edge& edge_to_append, TopoDS_Wire& target_wire )
	{
		if( target_wire.IsNull() )
		{
			BRepBuilderAPI_MakeWire mk_wire;
			mk_wire.Add( edge_to_append );
			target_wire = mk_wire.Wire();
		}
		else
		{
			BRepBuilderAPI_MakeWire mk_wire( target_wire );
			mk_wire.Add( edge_to_append );
			target_wire = mk_wire.Wire();
		}
	}
	
	inline void appendAndFixWires(const TopoDS_Wire& wire_to_append, TopoDS_Wire& target_wire, bool fix_connected)
	{
		try
		{
			double eps = 0.001;
			ShapeFix_Wire fix_wire;
			fix_wire.SetPrecision(eps);
			Handle_ShapeExtend_WireData wire_data = new ShapeExtend_WireData();

			TopExp_Explorer explorer1( target_wire, TopAbs_EDGE );
			for( ; explorer1.More(); explorer1.Next() )
			{
				const TopoDS_Edge& edge = TopoDS::Edge( explorer1.Current() );
				wire_data->Add( edge );
			}

			TopExp_Explorer explorer2( wire_to_append, TopAbs_EDGE );
			for( ; explorer2.More(); explorer2.Next() )
			{
				const TopoDS_Edge& edge = TopoDS::Edge( explorer2.Current() );
				wire_data->Add( edge );
			}

			fix_wire.Load( wire_data );

			if( fix_connected )
			{
				fix_wire.FixConnected(eps);
			}
			bool fixed = fix_wire.Perform();

			target_wire = fix_wire.Wire();
		}
		catch( Standard_Failure sf )
		{
			std::cout << __FUNC__ << " ShapeFix_Wire failed: " << sf.GetMessageString() << std::endl;
			//GeomDebugDumpOCC::dumpShape( target_wire, vec4( 0.8, 0.9, 0.9, 1.0 ), true, true );
			//GeomDebugDumpOCC::dumpShape( wire_to_append, vec4( 0.8, 0.9, 0.9, 1.0 ), true, false );

			// check if there is a gap between the wires, try to fix it
			TopoDS_Edge last_edge_target_wire;
			TopExp_Explorer explorer(target_wire, TopAbs_EDGE);
			for( ; explorer.More(); explorer.Next() )
			{
				last_edge_target_wire = TopoDS::Edge(explorer.Current());
			}
			TopoDS_Edge first_edge_wire_to_append;
			TopExp_Explorer explorer2(wire_to_append, TopAbs_EDGE);
			first_edge_wire_to_append = TopoDS::Edge(explorer2.Current());

			try
			{
				BRepBuilderAPI_MakeWire mk_wire(target_wire);
				if( !last_edge_target_wire.IsNull() && !first_edge_wire_to_append.IsNull() )
				{
					const TopoDS_Vertex& current_end_vertex = TopExp::LastVertex(last_edge_target_wire);
					gp_Pnt current_point = BRep_Tool::Pnt(current_end_vertex);

					const TopoDS_Vertex& next_vertex = TopExp::FirstVertex(first_edge_wire_to_append);
					gp_Pnt next_point = BRep_Tool::Pnt(next_vertex);

					if( current_point.SquareDistance(next_point) > 0.001 )
					{
						TopoDS_Edge closing_edge = BRepBuilderAPI_MakeEdge(current_end_vertex, next_vertex);
						mk_wire.Add(closing_edge);

					}
					mk_wire.Add(wire_to_append);
					mk_wire.Build();
					if( mk_wire.IsDone() )
					{
						target_wire = mk_wire.Wire();
					}
				}
			}
			catch( Standard_Failure sf )
			{
				std::cout << __FUNC__ << " MakeWire failed: " << sf.GetMessageString() << std::endl;
#ifdef _DEBUG
				//GeomDebugUtils::dumpShape( target_wire, vec4( 0.8, 0.9, 0.9, 1.0 ), true, true );
				//GeomDebugUtils::dumpShape( wire_to_append, vec4( 0.8, 0.9, 0.9, 1.0 ), true, false );
#endif
			}
		}
	}

	inline void appendWireToWire( const TopoDS_Wire& wire_to_append, TopoDS_Wire& target_wire )
	{
		if( wire_to_append.IsNull() )
		{
			return;
		}

		if( target_wire.IsNull() )
		{
			target_wire = wire_to_append;
		}
		else
		{
			appendAndFixWires(wire_to_append, target_wire, true);
		}
	}
	static void createWireFromPoints(const std::vector<gp_Vec>& coordinates, TopoDS_Wire& wire, bool close_wire_with_first_point)
	{
		try
		{
			std::vector<TopoDS_Vertex> vertices;
			for( size_t ii = 0; ii < coordinates.size(); ++ii )
			{
				const gp_Vec& vec = coordinates[ii];
				vertices.push_back(BRepBuilderAPI_MakeVertex(gp_Pnt(vec.X(), vec.Y(), vec.Z())));
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
					if( GeomUtilsOCC::equal(vec, vec_next) )
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

				BRepBuilderAPI_MakeEdge mk_edge(vertex_ii, vertex_next);
				if( !mk_edge.IsDone() )
				{
					continue;
				}
				mk_wire.Add(mk_edge);
			}
			wire = mk_wire.Wire();
		}
		catch( Standard_Failure sf )
		{
			std::cout << __FUNC__ << " MakeWire failed: " << sf.GetMessageString() << std::endl;
#ifdef _DEBUG
			//GeomDebugUtils::dumpShape( target_wire, vec4( 0.8, 0.9, 0.9, 1.0 ), true, true );
			//GeomDebugUtils::dumpShape( wire_to_append, vec4( 0.8, 0.9, 0.9, 1.0 ), true, false );
#endif
		}
	}
	static void createFaceFromPoints(const std::vector<gp_Vec2d>& coordinates, TopoDS_Face& face)
	{
		try
		{
			std::vector<TopoDS_Vertex> vertices;
			for( size_t ii = 0; ii < coordinates.size(); ++ii )
			{
				const gp_Vec2d& vec = coordinates[ii];
				vertices.push_back(BRepBuilderAPI_MakeVertex(gp_Pnt(vec.X(), vec.Y(), 0.0f)));
			}

			BRepBuilderAPI_MakeWire mk_wire;
			for( size_t ii = 0; ii < vertices.size(); ii++ )
			{
				size_t idx_next = (ii + 1) % vertices.size();
				TopoDS_Vertex& vertex_ii = vertices[ii];
				TopoDS_Vertex& vertex_next = vertices[idx_next];

				if( coordinates.size() == vertices.size() )
				{
					const gp_Vec2d& vec = coordinates[ii];
					const gp_Vec2d& vec_next = coordinates[idx_next];
					if( GeomUtilsOCC::equal(vec, vec_next) )
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
				BRepBuilderAPI_MakeEdge mk_edge(vertex_ii, vertex_next);
				if( !mk_edge.IsDone() )
				{
					continue;
				}
				mk_wire.Add(mk_edge);
			}

			TopoDS_Wire wire = mk_wire.Wire();

			BRepBuilderAPI_MakeFace mk_face(wire, false);
			face = mk_face.Face();

		}
		catch( Standard_Failure sf )
		{
			std::cout << __FUNC__ << " MakeFace failed: " << sf.GetMessageString() << std::endl;
#ifdef _DEBUG
			//GeomDebugUtils::dumpShape( target_wire, vec4( 0.8, 0.9, 0.9, 1.0 ), true, true );
			//GeomDebugUtils::dumpShape( wire_to_append, vec4( 0.8, 0.9, 0.9, 1.0 ), true, false );
#endif
		}
	}

	static void createFaceFromPoints( const std::vector<TangentialPoint2D>& tangential_points, TopoDS_Face& face )
	{
		std::vector<TopoDS_Vertex> vertices;
		bool have_radius = false;
		for( size_t ii = 0; ii < tangential_points.size(); ++ii )
		{
			const TangentialPoint2D& tangent_point = tangential_points[ii];
			const gp_Vec2d& vec = tangent_point.m_coords;
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
				const gp_Vec2d& vec = tangent_point.m_coords;
				const gp_Vec2d& vec_next = tangent_point_next.m_coords;
				if( GeomUtilsOCC::equal( vec, vec_next ) )
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

	/* \brief closes gaps between edges in the given wire, without changing the order of the edges in the wire */
	inline void closeWire( TopoDS_Wire& wire )
	{
		if( wire.IsNull() )
		{
			return;
		}

		Standard_Boolean closed_before = wire.Closed();
		if( closed_before )
		{
			return;
		}

		// TODO: first try to re-order

		// close gaps with new edges
		std::vector<TopoDS_Edge> vec_edges;
		std::vector<TopoDS_Edge> vec_edges_closed;
		bool edges_to_insert = false;
		TopExp_Explorer explorer( wire, TopAbs_EDGE );
		for( ; explorer.More(); explorer.Next() )
		{
			TopoDS_Edge current_edge = TopoDS::Edge( explorer.Current() );
			vec_edges.push_back( current_edge );
		}

		if( vec_edges.size() > 1 )
		{
			try
			{
				for( size_t ii = 0; ii < vec_edges.size(); ++ii )
				{
					TopoDS_Edge current_edge = vec_edges[ii];
					TopoDS_Edge next_edge = vec_edges[(ii+1)%vec_edges.size()];

					if( !current_edge.IsNull() && !next_edge.IsNull() )
					{
						const TopoDS_Vertex& current_vertex = TopExp::LastVertex( current_edge );
						gp_Pnt current_point = BRep_Tool::Pnt( current_vertex );

						const TopoDS_Vertex& next_vertex = TopExp::FirstVertex( next_edge );
						gp_Pnt next_point = BRep_Tool::Pnt( next_vertex );

						if( current_point.SquareDistance( next_point ) > 0.001 )
						{
							TopoDS_Edge closing_edge = BRepBuilderAPI_MakeEdge( current_vertex, next_vertex );
							vec_edges_closed.push_back( closing_edge );
							edges_to_insert = true;
						}
						else
						{
							// same coordinates but different vertex obects
							ShapeBuild_Edge edge_builder;
							TopoDS_Vertex null_vertex;
							TopoDS_Edge closing_edge = edge_builder.CopyReplaceVertices( current_edge, null_vertex, next_vertex );
							vec_edges_closed.push_back( closing_edge );
							edges_to_insert = true;
						}
					}
				}
			}
			catch( Standard_Failure sf )
			{
				std::cout << __FUNCTION__ << sf.GetMessageString() << std::endl;
			}
		}

		try
		{
			ShapeFix_Wire fix_wire;
			Handle_ShapeExtend_WireData wire_data = new ShapeExtend_WireData();

			for( size_t ii = 0; ii < vec_edges_closed.size(); ++ii )
			{
				TopoDS_Edge edge = vec_edges_closed[ii];
				wire_data->Add( edge );
			}

			fix_wire.Load( wire_data );

			//if( close_wire )
			//{
			//fix_wire.ClosedWireMode() = Standard_True; //Enables closed wire mode
			//}
			fix_wire.FixConnected(); //Fix connection between wires
			fix_wire.FixClosed();
			//fix_wire.FixSelfIntersection(); //Fix Self Intersection
			fix_wire.Perform(); //Perform the fixes
			wire = fix_wire.Wire();
		}
		catch( Standard_Failure sf )
		{
			std::cout << __FUNCTION__ << sf.GetMessageString() << std::endl;
		}
		
#ifdef _DEBUG
		Standard_Boolean closed = wire.Closed();
		if( !closed )
		{
			std::cout << __FUNCTION__ << ": wire not closed" << std::endl;
		}
#endif
	}
	inline void removeDuplicates( std::vector<gp_Vec2d>& loop )
	{
		if( loop.size() > 1 )
		{
			auto it_loop = loop.begin();
			double previous_x = (*it_loop).X();
			double previous_y = (*it_loop).Y();
			++it_loop;

			while( it_loop != loop.end() )
			{
				const gp_Vec2d& current_point = *it_loop;
				if( std::abs( current_point.X() - previous_x ) < 0.00001 )
				{
					if( std::abs( current_point.Y() - previous_y ) < 0.00001 )
					{
						previous_x = current_point.X();
						previous_y = current_point.Y();
						it_loop = loop.erase( it_loop );
						continue;
					}
				}
				previous_x = current_point.X();
				previous_y = current_point.Y();
				++it_loop;
			}
		}
	}
	inline void removeDuplicates( std::vector<std::vector<gp_Vec2d> >&	paths )
	{
		for( size_t ii = 0; ii < paths.size(); ++ii )
		{
			std::vector<gp_Vec2d>& loop = paths[ii];
			removeDuplicates( loop );
		}
	}
	inline void removeDuplicates( std::vector<gp_Vec>& loop )
	{
		if( loop.size() > 1 )
		{
			auto it_loop = loop.begin();
			double previous_x = (*it_loop).X();
			double previous_y = (*it_loop).Y();
			double previous_z = (*it_loop).Z();
			++it_loop;

			while( it_loop != loop.end() )
			{
				gp_Vec& current_point = *it_loop;
				if( std::abs( current_point.X() - previous_x ) < 0.00001 )
				{
					if( std::abs( current_point.Y() - previous_y ) < 0.00001 )
					{
						if( std::abs( current_point.Z() - previous_z ) < 0.00001 )
						{
							previous_x = current_point.X();
							previous_y = current_point.Y();
							previous_z = current_point.Z();
							it_loop = loop.erase( it_loop );
							continue;
						}
					}
				}
				previous_x = current_point.X();
				previous_y = current_point.Y();
				previous_z = current_point.Z();
				++it_loop;
			}
		}
	}
	inline void getPointsOnEdge( const TopoDS_Edge& edge, std::vector<gp_Vec>& vec_points, const size_t num_points_on_circle = 40 )
	{
		Standard_Real first = 0;
		Standard_Real last = 1;
		Handle( Geom_Curve ) c = BRep_Tool::Curve( edge, first, last );
		if( c->DynamicType() == STANDARD_TYPE( Geom_Line ) )
		{
			// just straight line
			const TopoDS_Vertex& v1 = TopExp::FirstVertex( edge );
			const TopoDS_Vertex& v2 = TopExp::LastVertex( edge );
			gp_Pnt point1 = BRep_Tool::Pnt( v1 );
			gp_Pnt point2 = BRep_Tool::Pnt( v2 );
			vec_points.push_back( gp_Vec( point1.X(), point1.Y(), point1.Z() ) );
			vec_points.push_back( gp_Vec( point2.X(), point2.Y(), point2.Z() ) );
		}
		else
		{
			BRepAdaptor_Curve curve_adaptor( edge );
			double param_range = last - first;
			Standard_Real length_of_edge = GCPnts_AbscissaPoint::Length( curve_adaptor );
			double num_points = num_points_on_circle*param_range/(2.0*M_PI);
			double distance = length_of_edge/num_points;

			GCPnts_UniformAbscissa uniform_abscissa;
			uniform_abscissa.Initialize( curve_adaptor, distance );

			if( uniform_abscissa.IsDone() )
			{
				int nb_points = uniform_abscissa.NbPoints();
				for( int i = 0; i < nb_points; ++i )
				{
					Standard_Real parameter = uniform_abscissa.Parameter( i + 1 );
					gp_Pnt pnt = curve_adaptor.Value( parameter );
					vec_points.push_back( gp_Vec( pnt.X(), pnt.Y(), pnt.Z() ) );
					if( i > 0 && i < nb_points - 1 )
					{
						vec_points.push_back( gp_Vec( pnt.X(), pnt.Y(), pnt.Z() ) );
					}
				}
				if( vec_points.size()> 0 )
				{
					if( vec_points.size()%2 != 0 )
					{
						vec_points.push_back( vec_points.back() );
					}
				}
			}
		}
	}

	/*\brief method getPointsOnWire: get discretized points along the wire.
	* \param in wire: Input wire
	* \param out vec_points: Points along the given wire
	* \param in num_points_on_circle: In case of curved edges in the wire, create this many points on a full circle
	*/
	inline void getPointsOnWire( const TopoDS_Wire& wire, std::vector<gp_Vec>& vec_points, const size_t num_points_on_circle = 40 )
	{
		if( wire.IsNull() )
		{
			return;
		}

		TopExp_Explorer explorer( wire, TopAbs_EDGE );
		for( ; explorer.More(); explorer.Next() )
		{
			const TopoDS_Edge& edge = TopoDS::Edge( explorer.Current() );
			getPointsOnEdge( edge, vec_points, num_points_on_circle );
		}
		removeDuplicates( vec_points );
	}

	inline bool LineToLineIntersectionHelper( gp_Vec2d& v1, gp_Vec2d& v2, gp_Vec2d& v3, gp_Vec2d& v4, double & r, double & s )
	{
		// check if lines are parallel
		const gp_Vec2d vertex1to2 = v2 - v1;
		const gp_Vec2d vertex3to4 = v4 - v3;
		if( vertex1to2.Y() / vertex1to2.X() != vertex3to4.Y() / vertex3to4.X() )
		{
			const double d = vertex1to2.X()*vertex3to4.Y() - vertex1to2.Y()*vertex3to4.X();
			if( d != 0 )
			{
				const gp_Vec2d vertex3to1 = v1 - v3;
				r = ( vertex3to1.Y()*vertex3to4.X() - vertex3to1.X()*vertex3to4.Y()) / d;
				s = ( vertex3to1.Y()*vertex1to2.X() - vertex3to1.X()*vertex1to2.Y()) / d;
				return true;
			}
		}
		return false;

	}
	inline bool LineSegmentToLineIntersection( gp_Vec2d& v1, gp_Vec2d& v2, gp_Vec2d& v3, gp_Vec2d& v4, std::vector<gp_Vec2d>& result )
	{
		double r, s;
		if( LineToLineIntersectionHelper( v1, v2, v3, v4, r, s ) )
		{
			if( r >= 0 && r <= 1 )
			{
				result.push_back( v1 + ( v2 - v1 ) * r );
				return true;
			}
		}
		return false;
	}
	inline bool LineSegmentToLineSegmentIntersection( gp_Vec2d& v1, gp_Vec2d& v2, gp_Vec2d& v3, gp_Vec2d& v4, std::vector<gp_Vec2d>& result )
	{
		double r, s;
		if( LineToLineIntersectionHelper( v1, v2, v3, v4, r, s ) )
		{
			if( r >= 0 && r <= 1 )
			{
				if( s >= 0 && s <= 1 )
				{
					result.push_back( v1 + ( v2 - v1 ) * r );
					return true;
				}
			}
		}
		return false;
	}
	inline void closestPointOnLine( const gp_Vec& point, const gp_Vec& line_origin, const gp_Vec& line_direction, gp_Vec& closest )
	{
		const double denom = point.X()*line_direction.X() + point.Y()*line_direction.Y() + point.Z()*line_direction.Z() - line_direction.X()*line_origin.X() - line_direction.Y()*line_origin.Y() - line_direction.Z()*line_origin.Z();
		const double numer = line_direction.X()*line_direction.X() + line_direction.Y()*line_direction.Y() + line_direction.Z()*line_direction.Z();
		if( numer == 0.0 )
		{
			throw BuildingException( "Line is degenerated: the line's direction vector is a null vector!", __FUNC__ );
		}
		const double lambda = denom / numer;
		closest = gp_Vec( line_origin.X() + lambda*line_direction.X(), line_origin.Y() + lambda*line_direction.Y(), line_origin.Z() + lambda*line_direction.Z() );
	}
	inline void closestPointOnLine( const gp_Vec2d& point, const gp_Vec2d& line_origin, const gp_Vec2d& line_direction, gp_Vec2d& closest )
	{
		const double denom = point.X()*line_direction.X() + point.Y()*line_direction.Y() + -line_direction.X()*line_origin.X() - line_direction.Y()*line_origin.Y();
		const double numer = line_direction.X()*line_direction.X() + line_direction.Y()*line_direction.Y();
		if( numer == 0.0 )
		{
			throw BuildingException( "Line is degenerated: the line's direction vector is a null vector!", __FUNC__ );
		}
		const double lambda = denom / numer;
		closest = gp_Vec2d( line_origin.X() + lambda*line_direction.X(), line_origin.Y() + lambda*line_direction.Y() );
	}
	inline double distancePoint2Line( const gp_Vec& point, const gp_Vec& line_p0, const gp_Vec& line_p1 )
	{
		// d = |(point - line_p0)x(point - line_p1)| / |line_p1 - line_p0|
		return (point - line_p0).Crossed( (point - line_p1) ).Magnitude()/(line_p1 - line_p0).Magnitude();
	}
	inline double distancePoint2LineUnitDirection( const gp_Vec& point, const gp_Vec& line_pt, const gp_Vec& line_direction_normalized )
	{
		// d = |line_direction_normalized x ( point - line_pt )|
		return (point - line_pt).Crossed(line_direction_normalized).Magnitude();
	}
	
	inline bool isMatrixIdentity( const gp_Trsf& mat )
	{
		const gp_TrsfForm& form = mat.Form();
		if( form == gp_Identity )
		{
			return true;
		}
		if( form == gp_Translation )
		{
			double length_square = mat.TranslationPart().SquareModulus();
			if( length_square < GEOM_EPSILON_LENGTH )
			{
				return true;
			}
			return false;
		}
		return false;
	}
	inline void applyMatrixToShape( TopoDS_Shape& shape, const gp_Trsf& pos )
	{
		if( shape.IsNull() )
		{
			return;
		}

		if( isMatrixIdentity( pos ) )
		{
			return;
		}

		BRepBuilderAPI_Transform transform( shape, pos );
		TopoDS_Shape transformed_shape = transform.Shape();
		if( !transformed_shape.IsNull() )
		{
			shape = transformed_shape;
		}
	}
	inline void applyMatrixToShape( TopoDS_Shape& shape, const gp_GTrsf& pos )
	{
		if( shape.IsNull() )
		{
			return;
		}

		//if( isMatrixIdentity( pos ) )
		//{
		//	return;
		//}

		BRepBuilderAPI_GTransform transform( shape, pos );
		TopoDS_Shape transformed_shape = transform.Shape();
		if( !transformed_shape.IsNull() )
		{
			shape = transformed_shape;
		}
	}

	inline void applyTranslationToShape( TopoDS_Shape& shape, const gp_Vec& delta_pos )
	{
		if( shape.IsNull() )
		{
			return;
		}

		double length_square = delta_pos.SquareMagnitude();
		if( length_square < GEOM_EPSILON_LENGTH )
		{
			return;
		}

		gp_Trsf trsf;
		trsf.SetTranslation( delta_pos );

		BRepBuilderAPI_Transform transform( shape, trsf );
		TopoDS_Shape transformed_shape = transform.Shape();
		if( !transformed_shape.IsNull() )
		{
			shape = transformed_shape;
		}
	}
	inline void applyTranslate( std::vector<gp_Vec2d>& points, const gp_Vec2d& translate )
	{
		for( size_t ii = 0; ii < points.size(); ++ii )
		{
			gp_Vec2d& point = points[ii];
			point = point + translate;
		}
	}
	
	//\brief: finds the first occurrence of T in vector
	template<typename T, typename U>
	bool findFirstInVector( std::vector<shared_ptr<U> > vec, shared_ptr<T>& ptr )
	{
		typename std::vector<shared_ptr<U> >::iterator it_trim = vec.begin();
		for( ; it_trim != vec.end(); ++it_trim )
		{
			shared_ptr<U> item = *( it_trim );
			if( dynamic_pointer_cast<T>( item ) )
			{
				ptr = dynamic_pointer_cast<T>( item );
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool allPointersValid( const std::vector<shared_ptr<T> >& vec )
	{
		for( size_t ii = 0; ii < vec.size(); ++ii )
		{
			const shared_ptr<T>& ptr = vec[ii];
			if( !ptr )
			{
				return false;
			}
		}
		return true;
	}
}

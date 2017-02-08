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

#include <vector>
#include <unordered_set>

#include <Bnd_Box.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRep_Tool.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <gp_Vec.hxx>
#include <ShapeBuild_Edge.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeFix_EdgeConnect.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Wire.hxx>

#include <osg/Array>
#include <osg/CullFace>
#include <osg/Geode>
#include <osg/Group>
#include <osg/LineStipple>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Version>
#include <osgText/Text>
#include <osgViewer/Viewer>
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPException.h>
#include "GeometrySettings.h"

#define GEOM_LENGTH_EPSILON 1.4901161193847656e-08
#define GEOM_CSG_FUZZY_EPSILON 0.0001
#define GEOM_PROFILE_SIZE_EPSILON 0.00000001

typedef gp_Vec2d vec2;
typedef gp_Vec vec3;

struct TangentialPoint2D
{
	/**
	* \brief struct TangentialPoint: Tangential point to create wires with straight segments and fillets
	* @param in coords: coordinates of tangential point.
	* @param radius: radius at that point. If radius = 0, no fillet is created. If radius < 0, a left turn is created, if radius > 0, a right turn is created
	*/
	TangentialPoint2D( vec2 coords, double radius )
	{
		m_coords = coords;
		m_radius = radius;
	}
	vec2 m_coords;
	double m_radius;
};

class vec4
{
public:
	vec4() {}
	vec4( double r, double g, double b, double a ) : m_r( r ), m_g( g ), m_b( b ), m_a( a ) {}
	vec4( const vec4& other )
	{
		m_r = other.m_r;
		m_g = other.m_g;
		m_b = other.m_b;
		m_a = other.m_a;
	}
	double r() const { return m_r; }
	double g() const { return m_g; }
	double b() const { return m_b; }
	double a() const { return m_a; }

	double m_r = 0;
	double m_g = 0;
	double m_b = 0;
	double m_a = 0;
};

namespace GeomUtils
{
	/** polygon operations */
	static inline bool equal( const vec2& pt1, const vec2& pt2, const double tolerance = GEOM_LENGTH_EPSILON )
	{
		// comparing square distance saves a call to sqrt
		double distance_square = (pt2-pt1).SquareMagnitude();
		if( distance_square <= tolerance*tolerance )
		{
			return true;
		}
		return false;
	}
	static inline bool equal( const gp_Vec& pt1, const gp_Vec& pt2, const double tolerance = GEOM_LENGTH_EPSILON )
	{
		// comparing square distance saves a call to sqrt
		double distance_square = (pt2-pt1).SquareMagnitude();
		if( distance_square <= tolerance*tolerance )
		{
			return true;
		}
		return false;
	}
	static inline bool isZero( const gp_Vec2d& vec, double epsilon = GEOM_LENGTH_EPSILON )
	{
		return (vec.X()*vec.X() + vec.Y()*vec.Y()) < epsilon * epsilon;
	}

	inline vec3 computePolygonCentroid( const std::vector<vec3>& polygon )
	{
		vec3 polygon_centroid( 0, 0, 0 );
		for( std::vector<vec3>::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
		{
			const vec3& vertex_current = ( *it );
			polygon_centroid += vertex_current;
		}
		polygon_centroid /= (double)( polygon.size() );
		return polygon_centroid;
	}
	inline vec2 computePolygonCentroid( const std::vector<vec2>& polygon )
	{
		vec2 polygon_centroid( 0, 0 );
		for( std::vector<vec2>::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
		{
			const vec2& vertex_current = ( *it );
			polygon_centroid += vertex_current;
		}
		polygon_centroid /= (double)( polygon.size() );
		return polygon_centroid;
	}
	inline osg::Vec3d computePolygonNormal( const osg::Vec3dArray* polygon )
	{
		const int num_points = polygon->size();
		osg::Vec3d polygon_normal( 0, 0, 0 );
		for( int k = 0; k < num_points; ++k )
		{
			const osg::Vec3d& vertex_current = polygon->at( k );
			const osg::Vec3d& vertex_next = polygon->at( ( k + 1 ) % num_points );
			polygon_normal._v[0] += ( vertex_current.y() - vertex_next.y() )*( vertex_current.z() + vertex_next.z() );
			polygon_normal._v[1] += ( vertex_current.z() - vertex_next.z() )*( vertex_current.x() + vertex_next.x() );
			polygon_normal._v[2] += ( vertex_current.x() - vertex_next.x() )*( vertex_current.y() + vertex_next.y() );
		}
		polygon_normal.normalize();
		return polygon_normal;
	}
	inline osg::Vec3f computePolygonNormal( const osg::Vec3Array* polygon )
	{
		const int num_points = polygon->size();
		osg::Vec3f polygon_normal( 0, 0, 0 );
		for( int k = 0; k < num_points; ++k )
		{
			const osg::Vec3f& vertex_current = polygon->at( k );
			const osg::Vec3f& vertex_next = polygon->at( ( k + 1 ) % num_points );
			polygon_normal._v[0] += ( vertex_current.y() - vertex_next.y() )*( vertex_current.z() + vertex_next.z() );
			polygon_normal._v[1] += ( vertex_current.z() - vertex_next.z() )*( vertex_current.x() + vertex_next.x() );
			polygon_normal._v[2] += ( vertex_current.x() - vertex_next.x() )*( vertex_current.y() + vertex_next.y() );
		}
		polygon_normal.normalize();
		return polygon_normal;
	}
	inline vec3 computePolygonNormal( const std::vector<vec3>& polygon )
	{
		vec3 polygon_normal( 0, 0, 0 );
		bool last_loop = false;
		for( std::vector<vec3>::const_iterator it = polygon.begin();; )
		{
			const vec3& vertex_current = ( *it );
			++it;
			if( it == polygon.end() )
			{
				it = polygon.begin();
				last_loop = true;
			}
			const vec3& vertex_next = ( *it );
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
	inline vec3 computePolygon2DNormal( const std::vector<vec2>& polygon )
	{
		const int num_points = polygon.size();
		vec3 polygon_normal( 0, 0, 0 );
		for( int k = 0; k < num_points; ++k )
		{
			const vec2& vertex_current = polygon[k];
			const vec2& vertex_next = polygon[( k + 1 ) % num_points];
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
	inline bool checkOpenPolygonConvexity( const std::vector<vec2>& polygon )
	{
		if( polygon.size() < 3 )
		{
			return true;
		}
		const int num_points = polygon.size();
		double zcrossproduct_previous = 0;
		for( int k = 0; k < num_points - 2; ++k )
		{
			const vec2& vertex_current = polygon[k];
			const vec2& vertex_next1 = polygon[k + 1];
			const vec2& vertex_next2 = polygon[k + 2];

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
	inline void createVector2DWithRadius( const std::vector<vec2>& vec_points_in, std::vector<TangentialPoint2D>& vec_out )
	{
		for( size_t ii = 0; ii < vec_points_in.size(); ++ii )
		{
			const vec2& point = vec_points_in[ii];
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
	
	inline void appendAndFixWires( const TopoDS_Wire& wire_to_append, TopoDS_Wire& target_wire, bool fix_connected )
	{
		//try
		{
			ShapeFix_Wire fix_wire;
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
				fix_wire.FixConnected();
			}
			//fix_wire.FixSelfIntersection();
			fix_wire.Perform();

			target_wire = fix_wire.Wire();
		}
		//catch( Standard_Failure sf )
		//{
		//	std::cout << __FUNC__ << " ShapeFix_Wire failed: " << sf.GetMessageString() << std::endl;
		//	GeomDebugUtils::dumpShape( target_wire, vec4( 0.8, 0.9, 0.9, 1.0 ), true, true );
		//	GeomDebugUtils::dumpShape( wire_to_append, vec4( 0.8, 0.9, 0.9, 1.0 ), true, false );
		//}
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

			bool wire_successfully_appended = true;
			try
			{
				appendAndFixWires( wire_to_append, target_wire, false );
			}
			catch( Standard_Failure sf )
			{
				wire_successfully_appended = false;
			}

			if( !wire_successfully_appended )
			{
				// check if there is a gap between the wires, try to fix it
				TopoDS_Edge last_edge_target_wire;
				TopExp_Explorer explorer( target_wire, TopAbs_EDGE );
				for( ; explorer.More(); explorer.Next() )
				{
					last_edge_target_wire = TopoDS::Edge( explorer.Current() );
				}
				TopoDS_Edge first_edge_wire_to_append;
				TopExp_Explorer explorer2( wire_to_append, TopAbs_EDGE );
				first_edge_wire_to_append = TopoDS::Edge( explorer2.Current() );

				try
				{
					BRepBuilderAPI_MakeWire mk_wire( target_wire );
					if( !last_edge_target_wire.IsNull() && !first_edge_wire_to_append.IsNull() )
					{
						const TopoDS_Vertex& current_end_vertex = TopExp::LastVertex( last_edge_target_wire );
						gp_Pnt current_point = BRep_Tool::Pnt( current_end_vertex );

						const TopoDS_Vertex& next_vertex = TopExp::FirstVertex( first_edge_wire_to_append );
						gp_Pnt next_point = BRep_Tool::Pnt( next_vertex );

						if( current_point.SquareDistance( next_point ) > 0.001 )
						{
							TopoDS_Edge closing_edge = BRepBuilderAPI_MakeEdge( current_end_vertex, next_vertex );
							mk_wire.Add( closing_edge );

						}
						mk_wire.Add( wire_to_append );
						mk_wire.Build();
						if( mk_wire.IsDone() )
						{
							target_wire = mk_wire.Wire();
						}
						else
						{
							//BRepBuilderAPI_WireError err = mk_wire.Error();
							//if( err == BRepBuilderAPI_DisconnectedWire )
							{
								// apply ShapeFix_Wire only when simple methods failed, because it does not preserve order and orientation of the edges
								appendAndFixWires( wire_to_append, target_wire, true );
							}
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
	inline void removeDuplicates( std::vector<vec2>& loop )
	{
		if( loop.size() > 1 )
		{
			auto it_loop = loop.begin();
			double previous_x = (*it_loop).X();
			double previous_y = (*it_loop).Y();
			++it_loop;

			while( it_loop != loop.end() )
			{
				const vec2& current_point = *it_loop;
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
	inline void removeDuplicates( std::vector<std::vector<vec2> >&	paths )
	{
		for( size_t ii = 0; ii < paths.size(); ++ii )
		{
			std::vector<vec2>& loop = paths[ii];
			removeDuplicates( loop );
		}
	}
	inline void removeDuplicates( std::vector<vec3>& loop )
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
				vec3& current_point = *it_loop;
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
	inline void getPointsOnEdge( const TopoDS_Edge& edge, std::vector<vec3>& vec_points, const size_t num_points_on_circle = 40 )
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
			vec_points.push_back( vec3( point1.X(), point1.Y(), point1.Z() ) );
			vec_points.push_back( vec3( point2.X(), point2.Y(), point2.Z() ) );
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
					vec_points.push_back( vec3( pnt.X(), pnt.Y(), pnt.Z() ) );
					if( i > 0 && i < nb_points - 1 )
					{
						vec_points.push_back( vec3( pnt.X(), pnt.Y(), pnt.Z() ) );
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
	inline void getPointsOnWire( const TopoDS_Wire& wire, std::vector<vec3>& vec_points, const size_t num_points_on_circle = 40 )
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

	inline bool LineToLineIntersectionHelper( vec2& v1, vec2& v2, vec2& v3, vec2& v4, double & r, double & s )
	{
		// check if lines are parallel
		const vec2 vertex1to2 = v2 - v1;
		const vec2 vertex3to4 = v4 - v3;
		if( vertex1to2.Y() / vertex1to2.X() != vertex3to4.Y() / vertex3to4.X() )
		{
			const double d = vertex1to2.X()*vertex3to4.Y() - vertex1to2.Y()*vertex3to4.X();
			if( d != 0 )
			{
				const vec2 vertex3to1 = v1 - v3;
				r = ( vertex3to1.Y()*vertex3to4.X() - vertex3to1.X()*vertex3to4.Y()) / d;
				s = ( vertex3to1.Y()*vertex1to2.X() - vertex3to1.X()*vertex1to2.Y()) / d;
				return true;
			}
		}
		return false;

	}
	inline bool LineSegmentToLineIntersection( vec2& v1, vec2& v2, vec2& v3, vec2& v4, std::vector<vec2>& result )
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
	inline bool LineSegmentToLineSegmentIntersection( vec2& v1, vec2& v2, vec2& v3, vec2& v4, std::vector<vec2>& result )
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
			throw IfcPPException( "Line is degenerated: the line's direction vector is a null vector!", __FUNC__ );
		}
		const double lambda = denom / numer;
		closest = gp_Vec( line_origin.X() + lambda*line_direction.X(), line_origin.Y() + lambda*line_direction.Y(), line_origin.Z() + lambda*line_direction.Z() );
	}
	inline void closestPointOnLine( const vec2& point, const vec2& line_origin, const vec2& line_direction, vec2& closest )
	{
		const double denom = point.X()*line_direction.X() + point.Y()*line_direction.Y() + -line_direction.X()*line_origin.X() - line_direction.Y()*line_origin.Y();
		const double numer = line_direction.X()*line_direction.X() + line_direction.Y()*line_direction.Y();
		if( numer == 0.0 )
		{
			throw IfcPPException( "Line is degenerated: the line's direction vector is a null vector!", __FUNC__ );
		}
		const double lambda = denom / numer;
		closest = vec2( line_origin.X() + lambda*line_direction.X(), line_origin.Y() + lambda*line_direction.Y() );
	}
	inline double distancePoint2Line( const vec3& point, const vec3& line_p0, const vec3& line_p1 )
	{
		// d = |(point - line_p0)x(point - line_p1)| / |line_p1 - line_p0|
		return (point - line_p0).Crossed( (point - line_p1) ).Magnitude()/(line_p1 - line_p0).Magnitude();
	}
	inline double distancePoint2LineUnitDirection( const vec3& point, const vec3& line_pt, const vec3& line_direction_normalized )
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
			if( length_square < GEOM_LENGTH_EPSILON )
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

	inline void applyTranslationToShape( TopoDS_Shape& shape, const gp_Vec& delta_pos )
	{
		if( shape.IsNull() )
		{
			return;
		}

		double length_square = delta_pos.SquareMagnitude();
		if( length_square < GEOM_LENGTH_EPSILON )
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
	inline void applyTranslate( std::vector<vec2>& points, const vec2& translate )
	{
		for( size_t ii = 0; ii < points.size(); ++ii )
		{
			vec2& point = points[ii];
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

/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
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
#include <osg/Array>
#include <osg/Geode>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <ifcpp/model/shared_ptr.h>
#include "GeometrySettings.h"
#include "IncludeCarveHeaders.h"

enum ProjectionPlane { UNDEFINED, XY_PLANE, YZ_PLANE, XZ_PLANE };

class GeomUtils
{
public:
	class Ray : public osg::Referenced
	{
	public:
		void setRay( Ray* other )
		{
			origin = other->origin;
			direction = other->direction;
		}
		osg::Vec3d origin;
		osg::Vec3d direction;
	};
	class Plane : public osg::Referenced
	{
	public:
		Plane(){}
		Plane( osg::Vec3d& _position, osg::Vec3d& _normal )
		{
			position = _position;
			normal = _normal;
		}
		bool intersectRay( const GeomUtils::Ray* ray, osg::Vec3d& intersect_point ) const
		{
			carve::geom::vector<3>  plane_pos( carve::geom::VECTOR( position.x(), position.y(), position.z() ) );
			carve::geom::vector<3>  plane_normal( carve::geom::VECTOR( normal.x(), normal.y(), normal.z() ) );
			carve::geom::plane<3> plane( plane_normal, plane_pos );
			carve::geom::vector<3>  v1 = carve::geom::VECTOR( ray->origin.x(), ray->origin.y(), ray->origin.z() );
			carve::geom::vector<3>  v2 = v1 + carve::geom::VECTOR( ray->direction.x(), ray->direction.y(), ray->direction.z() );
			carve::geom::vector<3>  v_intersect;

			carve::geom::vector<3> Rd = v2 - v1;
			double Vd = dot( plane.N, Rd );
			double V0 = dot( plane.N, v1 ) + plane.d;

			if( carve::math::ZERO( Vd ) )
			{
				if( carve::math::ZERO( V0 ) )
				{
					return false; // bad intersection
				}
				else
				{
					return false; // no intersection
				}
			}

			double t = -V0 / Vd;
			v_intersect = v1 + t * Rd;
			intersect_point.set( v_intersect.x, v_intersect.y, v_intersect.z );
			return true;
		}
		
		/** distance point to plane */
		double distancePointPlane( const osg::Vec3d& point )
		{
			return normal*(point - position);
		}
	
		osg::Vec3d position;
		osg::Vec3d normal;
	};

	static osg::ref_ptr<osg::Geode> createCoordinateAxes( double length );
	static osg::ref_ptr<osg::Group> createCoordinateAxesArrows();
	static osg::ref_ptr<osg::Geode> createCoordinateGrid();
	static osg::ref_ptr<osg::Geode> createQuarterCircles();
	static void WireFrameModeOn( osg::StateSet* state );
	static void WireFrameModeOn( osg::Node* srisdNode );
	static void WireFrameModeOff( osg::StateSet* state );
	static void WireFrameModeOff( osg::Node* srisdNode );
	static void HiddenLineModeOn( osg::Group* node );
	static void HiddenLineModeOff( osg::Group* node );
	static void cullFrontBack( bool front, bool back, osg::StateSet* stateset );
	static void setMaterialAlpha( osg::Node* node, float alpha );
	
	/** polygon operations */
	static osg::Vec3d computePolygonNormal( const osg::Vec3dArray* polygon );
	static osg::Vec3f computePolygonNormal( const osg::Vec3Array* polygon );
	static carve::geom::vector<3> computePolygonCentroid( const std::vector<carve::geom::vector<3> >& polygon );
	static carve::geom::vector<3> computePolygonNormal( const std::vector<carve::geom::vector<3> >& polygon );
	static carve::geom::vector<3> computePolygon2DNormal( const std::vector<carve::geom::vector<2> >& polygon );
	static bool checkOpenPolygonConvexity( const std::vector<carve::geom::vector<2> >& polygon );
	static void appendPointsToCurve( const std::vector<carve::geom::vector<3> >& points_vec, std::vector<carve::geom::vector<3> >& target_vec );
	static void appendPointsToCurve( const std::vector<carve::geom::vector<2> >& points_vec, std::vector<carve::geom::vector<3> >& target_vec );
	
	/** distance point to line or linesegment */
	static bool LineSegmentToLineIntersection(carve::geom::vector<2>& v1, carve::geom::vector<2>& v2, carve::geom::vector<2>& v3, carve::geom::vector<2>& v4, std::vector<carve::geom::vector<2> >& result );
	static bool LineSegmentToLineSegmentIntersection(carve::geom::vector<2>& v1, carve::geom::vector<2>& v2, carve::geom::vector<2>& v3, carve::geom::vector<2>& v4, std::vector<carve::geom::vector<2> >& result );
	static void closestPointOnLine( const carve::geom::vector<3>& point, const carve::geom::vector<3>& line_origin, const carve::geom::vector<3>& line_direction, carve::geom::vector<3>& closest );
	static void closestPointOnLine( const carve::geom::vector<2>& point, const carve::geom::vector<2>& line_origin, const carve::geom::vector<2>& line_direction, carve::geom::vector<2>& closest );
	static double distancePoint2Line( const carve::geom::vector<3>& point, const carve::geom::vector<3>& line_p0, const carve::geom::vector<3>& line_p1 );
	static double distancePoint2LineUnitDirection( const carve::geom::vector<3>& point, const carve::geom::vector<3>& line_pt, const carve::geom::vector<3>& line_direction_normalized )
	{
		// d = |line_direction_normalized x ( point - line_pt )|
		return carve::geom::cross( ( point - line_pt ), ( line_direction_normalized ) ).length();
	}

	/** matrix operations */
	static void computeInverse( const carve::math::Matrix& matrix_a, carve::math::Matrix& inv );
	static void makeLookAt(const carve::geom::vector<3>& eye,const carve::geom::vector<3>& center,const carve::geom::vector<3>& up, carve::math::Matrix& m );
	static bool bisectingPlane( const carve::geom::vector<3>& v1, const carve::geom::vector<3>& v2, const carve::geom::vector<3>& v3, carve::geom::vector<3>& normal );
	static void convertPlane2Matrix( const carve::geom::vector<3>& plane_normal, const carve::geom::vector<3>& plane_position, 
							 const carve::geom::vector<3>& local_z, carve::math::Matrix& resulting_matrix );
	static void applyTranslate( osg::Group* grp, const osg::Vec3f& translate, std::unordered_set<osg::Geode*>& set_applied );
	static void applyPosition( shared_ptr<carve::input::PolyhedronData>& poly_data, carve::math::Matrix& matrix );
	static void applyPosition( shared_ptr<carve::mesh::MeshSet<3> >& meshset, carve::math::Matrix& matrix );
	static bool isMatrixIdentity( const carve::math::Matrix& mat )
	{
		if( std::abs( mat._11 - 1.0 ) > 0.00001 )  return false;
		if( std::abs( mat._22 - 1.0 ) > 0.00001 )  return false;
		if( std::abs( mat._33 - 1.0 ) > 0.00001 )  return false;
		if( std::abs( mat._44 - 1.0 ) > 0.00001 )  return false;

		if( std::abs( mat._12 ) > 0.00001 )  return false;
		if( std::abs( mat._13 ) > 0.00001 )  return false;
		if( std::abs( mat._14 ) > 0.00001 )  return false;

		if( std::abs( mat._21 ) > 0.00001 )  return false;
		if( std::abs( mat._23 ) > 0.00001 )  return false;
		if( std::abs( mat._24 ) > 0.00001 )  return false;

		if( std::abs( mat._31 ) > 0.00001 )  return false;
		if( std::abs( mat._32 ) > 0.00001 )  return false;
		if( std::abs( mat._34 ) > 0.00001 )  return false;

		if( std::abs( mat._41 ) > 0.00001 )  return false;
		if( std::abs( mat._42 ) > 0.00001 )  return false;
		if( std::abs( mat._43 ) > 0.00001 )  return false;
		return true;
	}

	static void computeMeshsetCreaseEdges( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::vector<carve::mesh::Edge<3>* >& vec_edges_out, const double crease_angle = M_PI*0.05 )
	{
		if( !meshset )
		{
			return;
		}

		for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
		{
			const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];
			std::vector<carve::mesh::Edge<3>* > vec_closed_edges = mesh->closed_edges;

			for( size_t i_edge = 0; i_edge < vec_closed_edges.size(); ++i_edge )
			{
				carve::mesh::Edge<3>* edge = vec_closed_edges[i_edge];

				if( !edge )
				{
					continue;
				}
				carve::mesh::Edge<3>* edge_reverse = edge->rev;
				if( !edge_reverse )
				{
					continue;
				}
				carve::mesh::Face<3>* face = edge->face;
				carve::mesh::Face<3>* face_reverse = edge_reverse->face;

				carve::geom::vector<3> f1_normal = face->plane.N;
				carve::geom::vector<3> f2_normal = face_reverse->plane.N;
				const double cos_angle = dot( f1_normal, f2_normal );
				if( cos_angle > 0 )
				{
					const double deviation = std::abs( cos_angle - 1.0 );
					if( deviation < crease_angle )
					{
						continue;
					}
				}
				vec_edges_out.push_back( edge );
			}
		}
	}

	static void renderMeshsetCreaseEdges( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, osg::Group* target_group, osg::Vec4f* color = nullptr, const double crease_angle = M_PI*0.05 )
	{
		if( !meshset )
		{
			return;
		}
		if( !target_group )
		{
			return;
		}
		std::vector<carve::mesh::Edge<3>* > vec_crease_edges;
		GeomUtils::computeMeshsetCreaseEdges( meshset, vec_crease_edges, crease_angle );

		if( vec_crease_edges.size() > 0 )
		{
			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
			for( size_t i_edge = 0; i_edge < vec_crease_edges.size(); ++i_edge )
			{
				carve::mesh::Edge<3>* edge = vec_crease_edges[i_edge];
				carve::geom::vector<3>& vertex1 = edge->v1()->v;
				carve::geom::vector<3>& vertex2 = edge->v2()->v;
				vertices->push_back( osg::Vec3d( vertex1.x, vertex1.y, vertex1.z ) );
				vertices->push_back( osg::Vec3d( vertex2.x, vertex2.y, vertex2.z ) );
			}

			osg::Geode* geode = new osg::Geode();
			osg::Geometry* geometry = new osg::Geometry();
			geometry->setVertexArray( vertices );

			if( color != nullptr )
			{
				osg::Vec4Array* colors = new osg::Vec4Array();
				colors->push_back( osg::Vec4f( color->r(), color->g(), color->b(), color->a() ) );
				colors->setBinding( osg::Array::BIND_OVERALL );
				geometry->setColorArray( colors );
			}

			geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vertices->size() ) );
			geode->addDrawable( geometry );
			target_group->addChild( geode );
		}
	}
};

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

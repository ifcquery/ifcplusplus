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
#pragma warning( disable: 4505 )

#include <vector>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>

#include "IncludeCarveHeaders.h"

namespace GeomUtils
{
	enum ProjectionPlane { UNDEFINED, XY_PLANE, YZ_PLANE, XZ_PLANE };

	class Ray
	{
	public:
		void setRay( Ray* other )
		{
			origin = other->origin;
			direction = other->direction;
		}
		void setRay( const Ray& other )
		{
			origin = other.origin;
			direction = other.direction;
		}
		vec3 origin;
		vec3 direction;
	};
	class Plane
	{
	public:
		Plane(){}
		Plane( vec3& _position, vec3& _normal )
		{
			m_position = _position;
			m_normal = _normal;
		}
		bool intersectRay( const GeomUtils::Ray* ray, vec3& intersect_point ) const
		{
			vec3  plane_pos( carve::geom::VECTOR( m_position.x, m_position.y, m_position.z ) );
			vec3  plane_normal( carve::geom::VECTOR( m_normal.x, m_normal.y, m_normal.z ) );
			carve::geom::plane<3> plane( plane_normal, plane_pos );
			vec3  v1 = carve::geom::VECTOR( ray->origin.x, ray->origin.y, ray->origin.z );
			vec3  v2 = v1 + carve::geom::VECTOR( ray->direction.x, ray->direction.y, ray->direction.z );
			vec3  v_intersect;

			vec3 Rd = v2 - v1;
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
			intersect_point = carve::geom::VECTOR( v_intersect.x, v_intersect.y, v_intersect.z );
			return true;
		}
		
		/** distance point to plane */
		double distancePointPlane( const vec3& point )
		{
			return dot( m_normal, point - m_position);
		}
	
		vec3 m_position;
		vec3 m_normal;
	};

	/** polygon operations */
	inline vec3 computePolygonCentroid( const std::vector<vec3>& polygon )
	{
		vec3 polygon_centroid( carve::geom::VECTOR( 0, 0, 0 ) );
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
		vec2 polygon_centroid( carve::geom::VECTOR( 0, 0 ) );
		for( std::vector<vec2>::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
		{
			const vec2& vertex_current = ( *it );
			polygon_centroid += vertex_current;
		}
		polygon_centroid /= (double)( polygon.size() );
		return polygon_centroid;
	}
	
	inline vec3 computePolygonNormal( const std::vector<vec3>& polygon )
	{
		vec3 polygon_normal( carve::geom::VECTOR( 0, 0, 0 ) );
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
			polygon_normal[0] += ( vertex_current.y - vertex_next.y )*( vertex_current.z + vertex_next.z );
			polygon_normal[1] += ( vertex_current.z - vertex_next.z )*( vertex_current.x + vertex_next.x );
			polygon_normal[2] += ( vertex_current.x - vertex_next.x )*( vertex_current.y + vertex_next.y );
			if( last_loop )
			{
				break;
			}
		}
		polygon_normal.normalize();
		return polygon_normal;
	}
	inline vec3 computePolygon2DNormal( const std::vector<vec2>& polygon )
	{
		const int num_points = polygon.size();
		vec3 polygon_normal( carve::geom::VECTOR( 0, 0, 0 ) );
		for( int k = 0; k < num_points; ++k )
		{
			const vec2& vertex_current = polygon[k];
			const vec2& vertex_next = polygon[( k + 1 ) % num_points];
			polygon_normal[2] += ( vertex_current.x - vertex_next.x )*( vertex_current.y + vertex_next.y );
		}
		polygon_normal.normalize();
		return polygon_normal;
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

			double dx1 = vertex_next1.x - vertex_current.x;
			double dy1 = vertex_next1.y - vertex_current.y;

			double dx2 = vertex_next2.x - vertex_next1.x;
			double dy2 = vertex_next2.y - vertex_next1.y;

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
	inline void appendPointsToCurve( const std::vector<vec2>& points_vec, std::vector<vec3>& target_vec )
	{
		bool omit_first = false;
		if( target_vec.size() > 0 )
		{
			const vec3& last_point = target_vec.back();
			const vec2& first_point_current_segment = points_vec.front();
			if( std::abs( last_point.x - first_point_current_segment.x ) < 0.000001 )
			{
				if( std::abs( last_point.y - first_point_current_segment.y ) < 0.000001 )
				{
					omit_first = true;
				}
			}
		}

		if( omit_first )
		{
			//target_vec.insert( target_vec.end(), points_vec.begin()+1, points_vec.end() );
			for( size_t i = 1; i < points_vec.size(); ++i )
			{
				const vec2& pt = points_vec[i];
				target_vec.push_back( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
			}
		}
		else
		{
			//target_vec.insert( target_vec.end(), points_vec.begin(), points_vec.end() );
			for( size_t i = 0; i < points_vec.size(); ++i )
			{
				const vec2& pt = points_vec[i];
				target_vec.push_back( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
			}
		}
		// TODO: handle all segments separately: std::vector<std::vector<vec3> >& target_vec
	}
	inline void appendPointsToCurve( const std::vector<vec3>& points_vec_src, std::vector<vec3>& target_vec )
	{
		// sometimes, sense agreement is not given correctly. try to correct sense of segment if necessary
		std::vector<vec3> points_vec( points_vec_src );
		if( target_vec.size() > 0 && points_vec.size() > 1 )
		{
			vec3 first_target_point = target_vec.front();
			vec3 last_target_point = target_vec.back();

			vec3 first_segment_point = points_vec.front();
			vec3 last_segment_point = points_vec.back();

			if( ( last_target_point - first_segment_point ).length2() < 0.000001 )
			{
				// segment order is as expected, nothing to do
			}
			else
			{
				if( ( last_target_point - last_segment_point ).length2() < 0.000001 )
				{
					// current segment seems to be in wrong order
					std::reverse( points_vec.begin(), points_vec.end() );
				}
				else
				{
					// maybe the current segment fits to the beginning of the target vector
					if( ( first_target_point - first_segment_point ).length2() < 0.000001 )
					{
						std::reverse( target_vec.begin(), target_vec.end() );
					}
					else
					{
						if( ( first_target_point - last_segment_point ).length2() < 0.000001 )
						{
							std::reverse( target_vec.begin(), target_vec.end() );
							std::reverse( points_vec.begin(), points_vec.end() );
						}
					}
				}
			}
		}

		bool omit_first = false;
		if( target_vec.size() > 0 )
		{
			vec3 last_point = target_vec.back();
			vec3 first_point_current_segment = points_vec.front();
			if( ( last_point - first_point_current_segment ).length() < 0.000001 )
			{
				omit_first = true;
			}
		}

		if( omit_first )
		{
			target_vec.insert( target_vec.end(), points_vec.begin() + 1, points_vec.end() );
		}
		else
		{
			target_vec.insert( target_vec.end(), points_vec.begin(), points_vec.end() );
		}
		// TODO: handle all segments separately: std::vector<std::vector<vec3> >& target_vec
	}
	inline void addArcWithEndPoint( std::vector<vec2>& coords, double radius, double start_angle, double opening_angle, double x_center, double y_center, int num_segments )
	{
		if( num_segments < 3 )
		{
			num_segments = 3;
		}

		if( num_segments > 100 )
		{
			num_segments = 100;
		}

		double angle = start_angle;
		double angle_delta = opening_angle / (double)( num_segments - 1 );
		for( int i = 0; i < num_segments; ++i )
		{
			coords.push_back( carve::geom::VECTOR( radius*cos( angle ) + x_center, radius*sin( angle ) + y_center ) );
			angle += angle_delta;
		}
	}

	inline bool LineToLineIntersectionHelper( vec2& v1, vec2& v2, vec2& v3, vec2& v4, double & r, double & s )
	{
		// check if lines are parallel
		const vec2 vertex1to2 = v2 - v1;
		const vec2 vertex3to4 = v4 - v3;
		if( vertex1to2.y / vertex1to2.x != vertex3to4.y / vertex3to4.x )
		{
			const double d = vertex1to2.x*vertex3to4.y - vertex1to2.y*vertex3to4.x;
			if( d != 0 )
			{
				const vec2 vertex3to1 = v1 - v3;
				r = ( vertex3to1.y*vertex3to4.x - vertex3to1.x*vertex3to4.y ) / d;
				s = ( vertex3to1.y*vertex1to2.x - vertex3to1.x*vertex1to2.y ) / d;
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
	inline void closestPointOnLine( const vec3& point, const vec3& line_origin, const vec3& line_direction, vec3& closest )
	{
		const double denom = point.x*line_direction.x + point.y*line_direction.y + point.z*line_direction.z - line_direction.x*line_origin.x - line_direction.y*line_origin.y - line_direction.z*line_origin.z;
		const double numer = line_direction.x*line_direction.x + line_direction.y*line_direction.y + line_direction.z*line_direction.z;
		if( numer == 0.0 )
		{
			throw BuildingException( "Line is degenerated: the line's direction vector is a null vector!", __FUNC__ );
		}
		const double lambda = denom / numer;
		closest = carve::geom::VECTOR( line_origin.x + lambda*line_direction.x, line_origin.y + lambda*line_direction.y, line_origin.z + lambda*line_direction.z );
	}
	inline void closestPointOnLine( const vec2& point, const vec2& line_origin, const vec2& line_direction, vec2& closest )
	{
		const double denom = point.x*line_direction.x + point.y*line_direction.y + -line_direction.x*line_origin.x - line_direction.y*line_origin.y;
		const double numer = line_direction.x*line_direction.x + line_direction.y*line_direction.y;
		if( numer == 0.0 )
		{
			throw BuildingException( "Line is degenerated: the line's direction vector is a null vector!", __FUNC__ );
		}
		const double lambda = denom / numer;
		closest = carve::geom::VECTOR( line_origin.x + lambda*line_direction.x, line_origin.y + lambda*line_direction.y );
	}
	inline double distancePoint2Line( const vec3& point, const vec3& line_p0, const vec3& line_p1 )
	{
		// d = |(point - line_p0)x(point - line_p1)| / |line_p1 - line_p0|
		return carve::geom::cross( ( point - line_p0 ), ( point - line_p1 ) ).length() / ( line_p1 - line_p0 ).length();
	}
	inline double distancePoint2LineUnitDirection( const vec3& point, const vec3& line_pt, const vec3& line_direction_normalized )
	{
		// d = |line_direction_normalized x ( point - line_pt )|
		return carve::geom::cross( ( point - line_pt ), ( line_direction_normalized ) ).length();
	}
	template<unsigned ndim>
	double Point2LineSegmentDistance2( const carve::geom::linesegment<ndim> &l, const carve::geom::vector<ndim> &v, carve::geom::vector<ndim> &closest_point )
	{
		carve::geom::vector<ndim> linesegment_delta = l.v2 - l.v1;
		double t = dot( v - l.v1, linesegment_delta ) / dot( linesegment_delta, linesegment_delta );
		if( t <= 0.0 )
		{
			t = 0.0;
		}
		if( t >= 1.0 )
		{
			t = 1.0;
		}
		closest_point = linesegment_delta*t + l.v1;
		return ( v - closest_point ).length2();
	}

	template<unsigned ndim>
	double Point2LineSegmentDistance( const carve::geom::linesegment<ndim> &l, const carve::geom::vector<ndim> &v, carve::geom::vector<ndim> &closest_point )
	{
		return sqrt( LineSegment2PointDistance2( l, v, closest_point ) );
	}

	inline bool isMatrixIdentity( const carve::math::Matrix& mat )
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
	inline bool isMatrixEqual(const carve::math::Matrix& mat1, const carve::math::Matrix& mat2, double maxDelta = 0.00001)
	{
		if (std::abs(mat1._11 - mat2._11) > maxDelta)  return false;
		if (std::abs(mat1._12 - mat2._12) > maxDelta)  return false;
		if (std::abs(mat1._13 - mat2._13) > maxDelta)  return false;
		if (std::abs(mat1._14 - mat2._14) > maxDelta)  return false;

		if (std::abs(mat1._21 - mat2._21) > maxDelta)  return false;
		if (std::abs(mat1._22 - mat2._22) > maxDelta)  return false;
		if (std::abs(mat1._23 - mat2._23) > maxDelta)  return false;
		if (std::abs(mat1._24 - mat2._24) > maxDelta)  return false;

		if (std::abs(mat1._31 - mat2._31) > maxDelta)  return false;
		if (std::abs(mat1._32 - mat2._32) > maxDelta)  return false;
		if (std::abs(mat1._33 - mat2._33) > maxDelta)  return false;
		if (std::abs(mat1._34 - mat2._34) > maxDelta)  return false;

		if (std::abs(mat1._41 - mat2._41) > maxDelta)  return false;
		if (std::abs(mat1._42 - mat2._42) > maxDelta)  return false;
		if (std::abs(mat1._43 - mat2._43) > maxDelta)  return false;
		if (std::abs(mat1._44 - mat2._44) > maxDelta)  return false;
		return true;
	}
	inline void makeLookAt(const vec3& eye,const vec3& center,const vec3& up, carve::math::Matrix& resulting_matrix )
	{
		vec3 zaxis = ( center - eye ).normalize();
		vec3 xaxis = cross( up, zaxis ).normalize();
		vec3 yaxis = cross( zaxis, xaxis );

		resulting_matrix = carve::math::Matrix(
			xaxis.x, yaxis.x, zaxis.x, 0,
			xaxis.y, yaxis.y, zaxis.y, 0,
			xaxis.z, yaxis.z, zaxis.z, 0,
			0, 0, 0, 1 );
	}
	inline bool bisectingPlane( const vec3& v1, const vec3& v2, const vec3& v3, vec3& normal )
	{
		bool valid = false;
		vec3 v21 = v2 - v1;
		vec3 v32 = v3 - v2;
		double len21_square = v21.length2();
		double len32_square = v32.length2();

		if( len21_square <= carve::CARVE_EPSILON * len32_square )
		{
			if( len32_square == 0.0 )
			{
				// all three points lie ontop of one-another
				normal = carve::geom::VECTOR( 0.0, 0.0, 0.0 );
				valid = false;
			}
			else
			{
				// return a normalized copy of v32 as bisector
				len32_square = 1.0 / len32_square;
				normal = v32*len32_square;
				normal.normalize();
				valid = true;
			}

		}
		else
		{
			valid = true;
			if( len32_square <= carve::CARVE_EPSILON * len21_square )
			{
				// return v21 as bisector
				v21.normalize();
				normal = v21;
			}
			else
			{
				v21.normalize();
				v32.normalize();

				double dot_product = dot( v32, v21 );
				double dot_product_abs = std::abs( dot_product );

				if( dot_product_abs > ( 1.0 + carve::CARVE_EPSILON ) || dot_product_abs < ( 1.0 - carve::CARVE_EPSILON ) )
				{
					normal = ( v32 + v21 )*dot_product - v32 - v21;
					normal.normalize();
				}
				else
				{
					// dot == 1 or -1, points are colinear
					normal = -v21;
				}
			}
		}
		return valid;
	}
	inline void convertPlane2Matrix( const vec3& plane_normal, const vec3& plane_position,
		const vec3& local_z, carve::math::Matrix& resulting_matrix )
	{
		vec3 local_normal( plane_normal );
		local_normal.normalize();
		vec3 local_z_new( local_z );
		//local_z_new.normalize();

		vec3 local_y = cross( local_normal, local_z_new );
		local_y.normalize();
		local_z_new = cross( local_y, local_normal );
		local_z_new.normalize();

		resulting_matrix = carve::math::Matrix(
			local_normal.x, local_y.x, local_z_new.x, plane_position.x,
			local_normal.y, local_y.y, local_z_new.y, plane_position.y,
			local_normal.z, local_y.z, local_z_new.z, plane_position.z,
			0, 0, 0, 1 );

	}
	
	/** MeshSet and Polyhedron operations */
	inline void applyTransform( shared_ptr<carve::input::PolyhedronData>& poly_data, const carve::math::Matrix& matrix )
	{
		for( size_t ii = 0; ii < poly_data->points.size(); ++ii )
		{
			vec3& point = poly_data->points[ii];
			point = matrix*point;
		}
	}
	inline void applyTranslate( shared_ptr<carve::mesh::MeshSet<3> >& meshset, const vec3& pos )
	{
		for( size_t i = 0; i < meshset->vertex_storage.size(); ++i )
		{
			vec3& point = meshset->vertex_storage[i].v;
			point = point + pos;
		}
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			meshset->meshes[i]->recalc();
		}
	}
	inline void applyTransform( shared_ptr<carve::mesh::MeshSet<3> >& meshset, const carve::math::Matrix& matrix )
	{
		for( size_t i = 0; i < meshset->vertex_storage.size(); ++i )
		{
			vec3& point = meshset->vertex_storage[i].v;
			point = matrix*point;
		}
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			meshset->meshes[i]->recalc();
		}
	}
	inline void applyTransform( carve::geom::aabb<3>& aabb, const carve::math::Matrix& matrix )
	{
		vec3& pos = aabb.pos;
		vec3& extent = aabb.extent;

		pos = matrix*pos;
		extent = matrix*extent;
	}

	/** matrix operations */
	inline bool computeInverse( const carve::math::Matrix& matrix_a, carve::math::Matrix& matrix_inv, const double eps = 0.01 )
	{
		double inv[16], det;
		double m[16] = {
			matrix_a._11, matrix_a._12, matrix_a._13, matrix_a._14,
			matrix_a._21, matrix_a._22, matrix_a._23, matrix_a._24,
			matrix_a._31, matrix_a._32, matrix_a._33, matrix_a._34,
			matrix_a._41, matrix_a._42, matrix_a._43, matrix_a._44 };

		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if( abs(det) < eps )
		{
			return false;
		}

		det = 1.0 / det;

		for( size_t i = 0; i < 16; i++ )
		{
			matrix_inv.v[i] = inv[i] * det;
		}

		return true;

	}
	inline bool checkMatricesIdentical( const carve::math::Matrix &A, const carve::math::Matrix &B, double tolerance = 0.000001 ) 
	{
		for( size_t i = 0; i < 16; ++i )
		{
			double delta = A.v[i] - B.v[i];
			if( abs(delta) > tolerance ) return false;
		}
		return true;
	}

	inline void removeDuplicates( std::vector<vec2>&	loop )
	{
		if( loop.size() > 1 )
		{
			auto it_loop = loop.begin();
			double previous_x = ( *it_loop ).x;
			double previous_y = ( *it_loop ).y;
			++it_loop;

			while( it_loop != loop.end() )
			{
				vec2& current_point = *it_loop;
				if( std::abs( current_point.x - previous_x ) < 0.00001 )
				{
					if( std::abs( current_point.y - previous_y ) < 0.00001 )
					{
						previous_x = current_point.x;
						previous_y = current_point.y;
						it_loop = loop.erase( it_loop );
						continue;
					}
				}
				previous_x = current_point.x;
				previous_y = current_point.y;
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
	inline void copyClosedLoopSkipDuplicates( const std::vector<vec2>& loop_in, std::vector<vec2>& loop_out )
	{
		loop_out.clear();
		if( loop_in.size() > 0 )
		{
			loop_out.reserve( loop_in.size() );
			vec2 previous_point = loop_in[0];
			loop_out.push_back( previous_point );
		
			if( loop_in.size() > 1 )
			{
				for( size_t ii = 1; ii < loop_in.size(); ++ii )
				{
					const vec2& current_point = loop_in[ii];
					if( std::abs( current_point.x - previous_point.x ) < 0.00001 )
					{
						if( std::abs( current_point.y - previous_point.y ) < 0.00001 )
						{
							continue;
						}
					}
					loop_out.push_back( current_point );
					previous_point.x = current_point.x;
					previous_point.y = current_point.y;
				}

				// delete last point if equal to first
				while( loop_out.size() > 2 )
				{
					vec2 & first = loop_out.front();
					vec2 & last = loop_out.back();

					if( std::abs( first.x - last.x ) < 0.00000001 )
					{
						if( std::abs( first.y - last.y ) < 0.00000001 )
						{
							loop_out.pop_back();
							continue;
						}
					}
					break;
				}
			}
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

	//\brief: collect connected edges and create face
	static carve::mesh::MeshSet<3>::face_t* createFaceFromEdgeLoop(carve::mesh::MeshSet<3>::edge_t* start)
	{
		carve::mesh::MeshSet<3>::edge_t* e = start;
		std::vector<carve::mesh::MeshSet<3>::edge_t*> loop_edges;
		do {
			if( e->rev != nullptr )
			{
				return nullptr;
			}
			loop_edges.push_back(e);
			e = e->perimNext();
		} while( e != start );

		const size_t N = loop_edges.size();
		for( size_t i = 0; i < N; ++i )
		{
			loop_edges[i]->rev = new carve::mesh::MeshSet<3>::edge_t(loop_edges[i]->v2(), nullptr);
		}

		for( size_t i = 0; i < N; ++i )
		{
			carve::mesh::MeshSet<3>::edge_t* openEdge = loop_edges[i];
			carve::mesh::MeshSet<3>::edge_t* openEdgeNext = loop_edges[(i + 1) % N];
			carve::mesh::MeshSet<3>::edge_t* e1 = openEdge->rev;
			carve::mesh::MeshSet<3>::edge_t* e2 = openEdgeNext->rev;
			e1->prev = e2;
			e2->next = e1;

			e1->rev = openEdge;
			e2->rev = openEdgeNext;
		}

		carve::mesh::MeshSet<3>::face_t* f = new carve::mesh::MeshSet<3>::face_t(start->rev);

		if( f->n_edges != N )
		{
			delete f;
			return nullptr;
		}

		return f;
	}

	//\brief: finds the first occurrence of T in vector
	static void closeMeshSet(carve::mesh::MeshSet<3>* meshset)
	{
		// try to fix open mesh
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			carve::mesh::MeshSet<3>::mesh_t *mesh = meshset->meshes[i];
			const size_t numOpenEdgesInitial = mesh->open_edges.size();
			if( numOpenEdgesInitial == 0 )
			{
				continue;
			}
			for( size_t kk = 0; kk < numOpenEdgesInitial; ++kk )
			{
				const size_t numOpenEdges = mesh->open_edges.size();
				if( numOpenEdges == 0 )
				{
					break;
				}

				mesh->faces.reserve(numOpenEdges + 1);

				carve::mesh::MeshSet<3>::edge_t *start = mesh->open_edges[0];

				carve::mesh::MeshSet<3>::edge_t *openEdge1 = nullptr;
				carve::mesh::MeshSet<3>::edge_t *openEdge2 = nullptr;
				std::vector<carve::mesh::MeshSet<3>::edge_t *> edges_to_close;
				edges_to_close.resize(numOpenEdges);
				carve::mesh::MeshSet<3>::edge_t *edge = start;
				size_t j = 0;
				size_t numOpenEdgesCurrentLoop = 0;
				do {
					edges_to_close[j++] = edge;

					carve::mesh::MeshSet<3>::edge_t *currentEdge = edge;
					carve::mesh::MeshSet<3>::edge_t *nextEdge = currentEdge->perimNext();
					++numOpenEdgesCurrentLoop;

					if( openEdge1 == nullptr )
					{
						// check if nextEdge is also an open edge
						for( size_t mm = 0; mm < mesh->open_edges.size(); ++mm )
						{
							carve::mesh::MeshSet<3>::edge_t* e = mesh->open_edges[mm];
							if( e == nextEdge )
							{
								openEdge1 = currentEdge;
								openEdge2 = nextEdge;
								break;
							}
						}
					}
					edge = nextEdge;
				} while( edge != start );

				if( numOpenEdgesCurrentLoop == 3 )
				{
					if( openEdge1 != nullptr )
					{
						// close with triangle
						carve::mesh::MeshSet<3>::face_t *closingTriangle = createFaceFromEdgeLoop(openEdge1);
						if( closingTriangle != nullptr )
						{
							closingTriangle->mesh = mesh;
							mesh->faces.push_back(closingTriangle);
						}
					}
				}
				else if( numOpenEdgesCurrentLoop > 3 )
				{
					if( openEdge1 != nullptr && openEdge2 != nullptr )
					{
						// add triangle with 2 open edges and a new edge
						carve::mesh::MeshSet<3>::face_t *triangle = new carve::mesh::MeshSet<3>::face_t(openEdge1->v2(), openEdge1->v1(), openEdge2->v2());
						triangle->mesh = mesh;
						openEdge1->rev = triangle->edge;
						triangle->edge->rev = openEdge1;
						mesh->faces.push_back(triangle);

						carve::mesh::MeshSet<3>::edge_t *e1 = openEdge1->rev;
						carve::mesh::MeshSet<3>::edge_t *e2 = e1->prev;
						openEdge2->rev = e2;
						e2->rev = openEdge2;
						//e1->validateLoop();
					}
				}

				meshset->collectVertices();
				mesh->cacheEdges();
				mesh->calcOrientation();
			}
		}
	}
}

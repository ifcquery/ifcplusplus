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
#include "GeometryInputData.h"
#include "IncludeCarveHeaders.h"

enum ProjectionPlane { UNDEFINED, XY_PLANE, YZ_PLANE, XZ_PLANE };

class GeomUtils
{
public:
	static osg::ref_ptr<osg::Geode> createCoordinateAxes();
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
	static void setMaterialTransparent( osg::Node* node, float transparency );

	static osg::Vec3d computePolygonNormal( const osg::Vec3dArray* polygon );
	static osg::Vec3f computePolygonNormal( const osg::Vec3Array* polygon );
	static carve::geom::vector<3> computePolygonCentroid( const std::vector<carve::geom::vector<3> >& polygon );
	static carve::geom::vector<3> computePolygonNormal( const std::vector<carve::geom::vector<3> >& polygon );
	static carve::geom::vector<3> computePolygon2DNormal( const std::vector<carve::geom::vector<2> >& polygon );
	static bool checkOpenPolygonConvexity( const std::vector<carve::geom::vector<2> >& polygon );

	static bool LineSegmentToLineIntersection(carve::geom::vector<2>& v1, carve::geom::vector<2>& v2, carve::geom::vector<2>& v3, carve::geom::vector<2>& v4, std::vector<carve::geom::vector<2> >& result );
	static bool LineSegmentToLineSegmentIntersection(carve::geom::vector<2>& v1, carve::geom::vector<2>& v2, carve::geom::vector<2>& v3, carve::geom::vector<2>& v4, std::vector<carve::geom::vector<2> >& result );

	static void appendPointsToCurve( const std::vector<carve::geom::vector<3> >& points_vec, std::vector<carve::geom::vector<3> >& target_vec );
	static void appendPointsToCurve( const std::vector<carve::geom::vector<2> >& points_vec, std::vector<carve::geom::vector<3> >& target_vec );
	static void computeInverse( const carve::math::Matrix& matrix_a, carve::math::Matrix& inv );
	static void closestPointOnLine( const carve::geom::vector<3>& point, const carve::geom::vector<3>& line_origin, const carve::geom::vector<3>& line_direction, carve::geom::vector<3>& closest );
	static void closestPointOnLine( const carve::geom::vector<2>& point, const carve::geom::vector<2>& line_origin, const carve::geom::vector<2>& line_direction, carve::geom::vector<2>& closest );

	/* creates a triangulated face. first input curve is outer curve, succeeding curves are inner curves **/
	static void createFace( const std::vector<std::vector<carve::geom::vector<3> > >& curves, PolyInputCache3D& poly_input, std::stringstream& err );
	static void extrude(	const std::vector<std::vector<carve::geom::vector<2> > >& paths, const carve::geom::vector<3> dir, shared_ptr<carve::input::PolyhedronData>& poly_data, std::stringstream& err );
	static void sweepDisk( std::vector<carve::geom::vector<3> >& curve_points, shared_ptr<ItemData>& item_data, const int nvc, const double radius, const double radius_inner = -1 );
	static void sweepArea( const std::vector<carve::geom::vector<3> >& curve_points, const std::vector<std::vector<carve::geom::vector<2> > >& profile_paths, shared_ptr<ItemData>& item_data_solid );
	static void makeLookAt(const carve::geom::vector<3>& eye,const carve::geom::vector<3>& center,const carve::geom::vector<3>& up, carve::math::Matrix& m );
	static bool bisectingPlane( const carve::geom::vector<3>& v1, const carve::geom::vector<3>& v2, const carve::geom::vector<3>& v3, carve::geom::vector<3>& normal );
	static void convertPlane2Matrix( const carve::geom::vector<3>& plane_normal, const carve::geom::vector<3>& plane_position, 
							 const carve::geom::vector<3>& local_z, carve::math::Matrix& resulting_matrix );
	static void applyTranslate( osg::Group* grp, const osg::Vec3f& translate );
	static void applyPosition( shared_ptr<carve::input::PolyhedronData>& poly_data, carve::math::Matrix& matrix );

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
		bool intersetRay( const Ray* ray, osg::Vec3d& intersect_point );
		osg::Vec3d position;
		osg::Vec3d normal;
	};
};

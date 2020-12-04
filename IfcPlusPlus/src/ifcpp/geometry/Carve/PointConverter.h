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

#define _USE_MATH_DEFINES
#include <math.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcCurve.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLoop.h>
#include <ifcpp/IFC4/include/IfcPolyline.h>
#include <ifcpp/IFC4/include/IfcTrimmingSelect.h>
#include <ifcpp/IFC4/include/IfcVertex.h>
#include <ifcpp/IFC4/include/IfcVertexPoint.h>

#include "IncludeCarveHeaders.h"

//\brief class to convert IFC point representations into carve input geometry
class PointConverter : public StatusCallback
{
protected:
	shared_ptr<UnitConverter>		m_unit_converter;

public:
	PointConverter( shared_ptr<UnitConverter>& uc ): m_unit_converter( uc )
	{
	}

	virtual ~PointConverter(){}

	const shared_ptr<UnitConverter>& getUnitConverter() { return m_unit_converter; }
	void setUnitConverter( shared_ptr<UnitConverter>& unit_converter )
	{
		m_unit_converter = unit_converter;
	}
	static bool convertIfcCartesianPoint( const shared_ptr<IfcCartesianPoint>& ifc_point, vec3& point, double length_factor )
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
			point = carve::geom::VECTOR( x, y, z );
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
			point = carve::geom::VECTOR( x, y, 0.0 );
			return true;
		}
		return false;
	}
	void convertIfcCartesianPointVector( const std::vector<shared_ptr<IfcCartesianPoint> >& points, std::vector<vec3>& loop ) const
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();
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
				loop.push_back( carve::geom::VECTOR( x, y, z ) );
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
				loop.push_back( carve::geom::VECTOR( x, y, 0.0 ) );
			}
			else
			{
#ifdef _DEBUG
				std::cout << "convertIfcCartesianPointVector: ifc_pt->m_Coordinates.size() != 2" << std::endl;
#endif
			}
		}
	}
	void convertIfcCartesianPointVector2D( const std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& vec_points_in, std::vector<vec3>& vertices )
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();
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
					vertices.push_back( carve::geom::VECTOR( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, coords[2]->m_value*length_factor ) );
				}
				else if( coords.size() > 1 )
				{
					vertices.push_back( carve::geom::VECTOR( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 ) );
				}
			}
		}
	}
	void convertIfcCartesianPointVectorSkipDuplicates( const std::vector<shared_ptr<IfcCartesianPoint> >& vec_ifc_points, std::vector<vec3>& loop ) const
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		vec3  vertex_previous;
		for( size_t ii = 0; ii < vec_ifc_points.size(); ++ii )
		{
			const shared_ptr<IfcCartesianPoint>& cartesian_point = vec_ifc_points[ii];
			double x = 0.0, y = 0.0, z = 0.0;
			if( cartesian_point )
			{
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
			}

			// skip duplicate vertices
			if( ii > 0 )
			{
				if( std::abs( x - vertex_previous.x ) < 0.00000001 )
				{
					if( std::abs( y - vertex_previous.y ) < 0.00000001 )
					{
						if( std::abs( z - vertex_previous.z ) < 0.00000001 )
						{
							continue;
						}
					}
				}
			}
			loop.push_back( carve::geom::VECTOR( x, y, z ) );
			vertex_previous.x = x;
			vertex_previous.y = y;
			vertex_previous.z = z;
		}
	}

	void convertPointList(const std::vector<std::vector<shared_ptr<IfcLengthMeasure> > >& pointList, std::vector<vec3>& loop)
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		for (size_t ii = 0; ii < pointList.size(); ++ii)
		{

			const std::vector<shared_ptr<IfcLengthMeasure> >& coords1 = pointList[ii];
			if (coords1.size() > 2)
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
				double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
				double z = round(coords1[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords1[0]->m_value*length_factor;
				double y = coords1[1]->m_value*length_factor;
				double z = coords1[2]->m_value*length_factor;
#endif
				loop.push_back(carve::geom::VECTOR(x, y, z));
			}
			else if (coords1.size() > 1)
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
				double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords1[0]->m_value*length_factor;
				double y = coords1[1]->m_value*length_factor;
#endif
				loop.push_back(carve::geom::VECTOR(x, y, 0.0));
			}
		}
	}

	static bool convertIfcVertex( const shared_ptr<IfcVertex>& vertex, vec3& point_result, const double length_factor )
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
					if( cartesian_point->m_Coordinates.size() > 2 )
					{
						point_result = carve::geom::VECTOR( cartesian_point->m_Coordinates[0]->m_value*length_factor, cartesian_point->m_Coordinates[1]->m_value*length_factor, cartesian_point->m_Coordinates[2]->m_value*length_factor );
						return true;
					}
					if( cartesian_point->m_Coordinates.size() > 1 )
					{
						point_result = carve::geom::VECTOR( cartesian_point->m_Coordinates[0]->m_value*length_factor, cartesian_point->m_Coordinates[1]->m_value*length_factor, 0.0 );
						return true;
					}
				}
			}
		}
		return false;
	}

	//\brief: returns the corresponding angle (radian, 0 is to the right) if the given point lies on the circle. If the point does not lie on the circle, -1 is returned.
	static double getAngleOnCircle( const vec3& circle_center, double circle_radius, const vec3& trim_point )
	{
		double result_angle = -1.0;
		vec3 center_trim_point = trim_point - circle_center;
		if( std::abs( center_trim_point.length() - circle_radius ) < 0.0001 )
		{
			vec3 center_trim_point_direction = center_trim_point;
			center_trim_point_direction.normalize();
			double cos_angle = carve::geom::dot( center_trim_point_direction, vec3( carve::geom::VECTOR( 1.0, 0, 0 ) ) );

			if( std::abs( cos_angle ) < 0.0001 )
			{
				if( center_trim_point.y >= 0 )
				{
					result_angle = M_PI_2;
				}
				else if( center_trim_point.y < 0 )
				{
					result_angle = M_PI*1.5;
				}
			}
			else
			{
				if( center_trim_point.y >= 0 )
				{
					result_angle = acos( cos_angle );
				}
				else if( center_trim_point.y < 0 )
				{
					result_angle = 2.0*M_PI - acos( cos_angle );
				}
			}
		}
		return result_angle;
	}
};

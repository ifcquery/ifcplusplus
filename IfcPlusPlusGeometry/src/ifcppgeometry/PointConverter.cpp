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


#define _USE_MATH_DEFINES 
#include <math.h>

#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/UnitConverter.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPUtil.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement2D.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement3D.h"
#include "ifcpp/IFC4/include/IfcCartesianPoint.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcPositiveLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcVector.h"
#include "ifcpp/IFC4/include/IfcVertexPoint.h"

#include "GeometrySettings.h"
#include "GeomUtils.h"
#include "GeometryException.h"
#include "PlacementConverter.h"

#include "PointConverter.h"

PointConverter::PointConverter( shared_ptr<GeometrySettings>& geom_settings, shared_ptr<UnitConverter>& uc )
	: m_geom_settings(geom_settings), m_unit_converter(uc)
{
	
}

PointConverter::~PointConverter()
{
}

void PointConverter::convertIfcCartesianPoint( const shared_ptr<IfcCartesianPoint>& ifc_point,	carve::geom::vector<3> & point ) const
{
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	std::vector<shared_ptr<IfcLengthMeasure> >& coords1 = ifc_point->m_Coordinates;
	if( coords1.size() > 2 )
	{
		// round to 0.1 mm
		// TODO: round only when digits are noise
#ifdef ROUND_IFC_COORDINATES
		double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double z = round(coords1[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		double x = coords1[0]->m_value*length_factor;
		double y = coords1[1]->m_value*length_factor;
		double z = coords1[2]->m_value*length_factor;
#endif
		point = carve::geom::VECTOR( x, y, z );
	}
	else if( coords1.size() > 1 )
	{
		// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
		double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		double x = coords1[0]->m_value*length_factor;
		double y = coords1[1]->m_value*length_factor;
#endif
		point = carve::geom::VECTOR( x, y, 0.0 );
	}
}

void PointConverter::convertIfcCartesianPoint( const shared_ptr<IfcCartesianPoint>& ifc_point,	carve::geom::vector<3> & point, double length_factor )
{
	std::vector<shared_ptr<IfcLengthMeasure> >& coords1 = ifc_point->m_Coordinates;
	if( coords1.size() > 2 )
	{
		// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
		double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double z = round(coords1[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		double x = coords1[0]->m_value*length_factor;
		double y = coords1[1]->m_value*length_factor;
		double z = coords1[2]->m_value*length_factor;
#endif
		point = carve::geom::VECTOR( x, y, z );
	}
	else if( coords1.size() > 1 )
	{
		// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
		double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		double x = coords1[0]->m_value*length_factor;
		double y = coords1[1]->m_value*length_factor;
#endif
		point = carve::geom::VECTOR( x, y, 0.0 );
	}
}

void PointConverter::convertIfcCartesianPointVector( const std::vector<shared_ptr<IfcCartesianPoint> >& points, std::vector<carve::geom::vector<3> >& loop ) const
{
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	const size_t num_points = points.size();
	for( size_t i_point=0; i_point < num_points; ++i_point )
	{
		const shared_ptr<IfcCartesianPoint>& ifc_cartesian_point = points[i_point];
		if( !ifc_cartesian_point )
		{
			continue;
		}

		const std::vector<shared_ptr<IfcLengthMeasure> >& coords = ifc_cartesian_point->m_Coordinates;
		if( coords.size() > 2  )
		{
			// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
			double x = round(coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			double y = round(coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			double z = round(coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
			double x = coords[0]->m_value*length_factor;
			double y = coords[1]->m_value*length_factor;
			double z = coords[2]->m_value*length_factor;
#endif
			loop.push_back( carve::geom::VECTOR( x, y, z ) );
		}
		else if( coords.size() > 1  )
		{
			// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
			double x = round(coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			double y = round(coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
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

void PointConverter::convertIfcCartesianPointVector2D( std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& points, std::vector<carve::geom::vector<3> >& vertices )
{
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >::iterator it_cp_outer;
	for( it_cp_outer=points.begin(); it_cp_outer!=points.end(); ++it_cp_outer )
	{
		std::vector<shared_ptr<IfcCartesianPoint> >& points_inner = (*it_cp_outer);
		std::vector<shared_ptr<IfcCartesianPoint> >::iterator it_cp;
		for( it_cp=points_inner.begin(); it_cp!=points_inner.end(); ++it_cp )
		{
			shared_ptr<IfcCartesianPoint> cp = (*it_cp);

			if( !cp )
			{
				continue;
			}

			std::vector<shared_ptr<IfcLengthMeasure> >& coords = cp->m_Coordinates;
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

void PointConverter::convertIfcCartesianPointVectorSkipDuplicates( const std::vector<shared_ptr<IfcCartesianPoint> >& ifc_points, std::vector<carve::geom::vector<3> >& loop ) const
{
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	std::vector<shared_ptr<IfcCartesianPoint> >::const_iterator it_cp;
	int i=0;
	carve::geom::vector<3>  vertex_previous;
	for( it_cp=ifc_points.begin(); it_cp!=ifc_points.end(); ++it_cp, ++i )
	{
		shared_ptr<IfcCartesianPoint> cp = (*it_cp);
		const int cp_id = cp->m_id;
		double x = 0.0, y = 0.0, z = 0.0;
		std::vector<shared_ptr<IfcLengthMeasure> >& coords = cp->m_Coordinates;


		if( coords.size() > 2 )
		{
#ifdef ROUND_IFC_COORDINATES
			x = round(coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			y = round(coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			z = round(coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
			x = coords[0]->m_value*length_factor;
			y = coords[1]->m_value*length_factor;
			z = coords[2]->m_value*length_factor;
#endif
		}
		else if( coords.size() > 1 )
		{
#ifdef ROUND_IFC_COORDINATES
			x = round(coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			y = round(coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
			x = coords[0]->m_value*length_factor;
			y = coords[1]->m_value*length_factor;
#endif
		}

		carve::geom::vector<3>  vertex( carve::geom::VECTOR( x, y, z ) );

		// skip duplicate vertices
		if( it_cp != ifc_points.begin() )
		{
			if( std::abs(vertex.x-vertex_previous.x) < 0.00000001 )
			{
				if( std::abs(vertex.y-vertex_previous.y) < 0.00000001 )
				{
					if( std::abs(vertex.z-vertex_previous.z) < 0.00000001 )
					{
						// TODO: is it better to report degenerated loops, or to just omit them?
						continue;
					}
				}
			}
		}
		loop.push_back( vertex );
		vertex_previous = vertex;
	}
}

// @brief: returns the corresponding angle (radian, 0 is to the right) if the given point lies on the circle. If the point does not lie on the circle, -1 is returned.
double PointConverter::getAngleOnCircle( const carve::geom::vector<3>& circle_center, double circle_radius, const carve::geom::vector<3>& trim_point )
{
	double result_angle = -1.0;
	carve::geom::vector<3> center_trim_point = trim_point-circle_center;
	if( std::abs(center_trim_point.length() - circle_radius) < 0.0001 )
	{
		carve::geom::vector<3> center_trim_point_direction = center_trim_point;
		center_trim_point_direction.normalize();
		double cos_angle = carve::geom::dot( center_trim_point_direction, carve::geom::vector<3> ( carve::geom::VECTOR( 1.0, 0, 0 ) ) );

		if( std::abs(cos_angle) < 0.0001 )
		{
			if( center_trim_point.y > 0 )
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
			if( center_trim_point.y > 0 )
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

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
#include <ifcpp/IFC4X3/EntityFactory.h>
#include <IfcCartesianPoint.h>
#include <IfcCurve.h>
#include <IfcLengthMeasure.h>
#include <IfcLoop.h>
#include <IfcPolyline.h>
#include <IfcTrimmingSelect.h>
#include <IfcVertex.h>
#include <IfcVertexPoint.h>
#include "GeomUtils.h"
#include "IncludeCarveHeaders.h"

using namespace IFC4X3;

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
		if( !std::isnan(ifc_point->m_Coordinates[2]) )
		{
			double x = ifc_point->m_Coordinates[0]*length_factor;
			double y = ifc_point->m_Coordinates[1]*length_factor;
			double z = ifc_point->m_Coordinates[2]*length_factor;
			point = carve::geom::VECTOR( x, y, z );
			return true;
		}
		else
		{
			double x = ifc_point->m_Coordinates[0]*length_factor;
			double y = ifc_point->m_Coordinates[1]*length_factor;
			point = carve::geom::VECTOR( x, y, 0.0 );
			return true;
		}
	}

	void convertIfcCartesianPointVector( const std::vector<shared_ptr<IfcCartesianPoint> >& points, std::vector<vec3>& loop ) const
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		const size_t num_points = points.size();
		for( size_t i_point = 0; i_point < num_points; ++i_point )
		{
			const shared_ptr<IfcCartesianPoint>& cartesian_point = points[i_point];
			if( !cartesian_point )
			{
				continue;
			}

			if( !std::isnan(cartesian_point->m_Coordinates[2]) )
			{
				double x = cartesian_point->m_Coordinates[0]*length_factor;
				double y = cartesian_point->m_Coordinates[1]*length_factor;
				double z = cartesian_point->m_Coordinates[2]*length_factor;
				loop.push_back( carve::geom::VECTOR( x, y, z ) );
			}
			else
			{
				double x = cartesian_point->m_Coordinates[0]*length_factor;
				double y = cartesian_point->m_Coordinates[1]*length_factor;
				loop.push_back( carve::geom::VECTOR( x, y, 0.0 ) );
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

				if( !std::isnan(cartesian_point->m_Coordinates[2]) )
				{
					vertices.push_back( carve::geom::VECTOR( cartesian_point->m_Coordinates[0]*length_factor, cartesian_point->m_Coordinates[1]*length_factor, cartesian_point->m_Coordinates[2]*length_factor ) );
				}
				else
				{
					vertices.push_back( carve::geom::VECTOR( cartesian_point->m_Coordinates[0]*length_factor, cartesian_point->m_Coordinates[1]*length_factor, 0.0 ) );
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
				if( !std::isnan(cartesian_point->m_Coordinates[2]) )
				{
					x = cartesian_point->m_Coordinates[0]*length_factor;
					y = cartesian_point->m_Coordinates[1]*length_factor;
					z = cartesian_point->m_Coordinates[2]*length_factor;
				}
				else
				{
					x = cartesian_point->m_Coordinates[0]*length_factor;
					y = cartesian_point->m_Coordinates[1]*length_factor;
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
				double x = coords1[0]->m_value*length_factor;
				double y = coords1[1]->m_value*length_factor;
				double z = coords1[2]->m_value*length_factor;
				loop.push_back(carve::geom::VECTOR(x, y, z));
			}
			else if (coords1.size() > 1)
			{
				double x = coords1[0]->m_value*length_factor;
				double y = coords1[1]->m_value*length_factor;
				loop.push_back(carve::geom::VECTOR(x, y, 0.0));
			}
		}
	}

	static bool convertIfcVertex(const shared_ptr<IfcVertex>& vertex, vec3& point_result, const double length_factor)
	{
		shared_ptr<IfcVertexPoint> vertex_point = dynamic_pointer_cast<IfcVertexPoint>(vertex);
		if( vertex_point )
		{
			if( vertex_point->m_VertexGeometry )
			{
				const shared_ptr<IfcPoint>& vertex_point_geometry = vertex_point->m_VertexGeometry;
				// ENTITY IfcPoint ABSTRACT SUPERTYPE OF(ONEOF(IfcCartesianPoint, IfcPointOnCurve, IfcPointOnSurface))
				shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>(vertex_point_geometry);
				if( cartesian_point )
				{
					if( !std::isnan(cartesian_point->m_Coordinates[2]) )
					{
						point_result = carve::geom::VECTOR(cartesian_point->m_Coordinates[0] * length_factor, cartesian_point->m_Coordinates[1] * length_factor, cartesian_point->m_Coordinates[2] * length_factor);
						return true;
					}
					else
					{
						point_result = carve::geom::VECTOR(cartesian_point->m_Coordinates[0] * length_factor, cartesian_point->m_Coordinates[1] * length_factor, 0.0);
						return true;
					}
				}
				else
				{
					std::cout << "IfcVertexPoint.VertexGeometry type = " << EntityFactory::getStringForClassID(vertex_point_geometry->classID()) << std::endl;
				}
			}
		}
		else
		{
			std::cout << "IfcVertex type = " << EntityFactory::getStringForClassID(vertex->classID()) << std::endl;
		}
		
		return false;
	}

	static double trimPointCircleDistance(double angle, double radius, const carve::math::Matrix& circlePosition, const vec3& trimPoint)
	{
		vec3 circlePoint = carve::geom::VECTOR(radius * cos(angle), radius * sin(angle), 0);
		circlePoint = circlePosition * circlePoint;
		double distance2 = (trimPoint - circlePoint).length2();
		return distance2;
	}

	static double regula( double x0,double x1, double fx0,double fx1)
	{
		return x0-((x1-x0)/(fx1-fx0))*fx0;
	}

	//\brief: returns the corresponding angle (radian, 0 is to the right) if the given point lies on the circle. If the point does not lie on the circle, -1 is returned.
	static double getAngleOnCircle( const vec3& circleCenter, double radius, vec3& trimPoint, const carve::math::Matrix& circlePosition, const carve::math::Matrix& circlePositionInverse )
	{
		double resultAngle = -1.0;
		vec3 center2trimPoint = trimPoint - circleCenter;
		vec3 center2trimPointDirection = center2trimPoint;
		center2trimPointDirection.normalize();

		bool checkDistanceToCircleCenter = false;
		if( checkDistanceToCircleCenter )
		{
			if( std::abs(center2trimPoint.length() - radius) > EPS_DEFAULT )
			{
				trimPoint = circleCenter + center2trimPointDirection * radius;
			}
		}
		
		vec3 circleCenter2D = circlePositionInverse * circleCenter;
		vec3 trimPoint2D = circlePositionInverse * trimPoint;
		vec3 trimPointRelative2D = trimPoint2D - circleCenter2D;
		if( std::abs(trimPointRelative2D.z) < EPS_M4 )
		{
			resultAngle = atan2(trimPointRelative2D.y - circleCenter2D.y, trimPointRelative2D.x - circleCenter2D.x);

			vec3 circleCenter3DCheck = circlePosition * carve::geom::VECTOR(0, 0, 0);
			vec3 circlePoint = carve::geom::VECTOR(radius * cos(resultAngle), radius * sin(resultAngle), 0);
			circlePoint = circlePosition * circlePoint;

			vec3 check = circlePoint - trimPoint;
			double dist = check.length();
			if( dist < EPS_M8 )
			{
				return resultAngle;
			}
		}
		
		{
			// try parabola
			double angle = resultAngle;
			double angleStep = 0.1;
			double x[4], y[4];
			x[1] = resultAngle - angleStep;
			x[2] = resultAngle;
			x[3] = resultAngle + angleStep;
			
			for( size_t ii = 0; ii < 40; ++ii )
			{
				y[1] = trimPointCircleDistance(x[1], radius, circlePosition, trimPoint);
				y[2] = trimPointCircleDistance(x[2], radius, circlePosition, trimPoint);
				y[3] = trimPointCircleDistance(x[3], radius, circlePosition, trimPoint);

				// zero of parabola
				x[0] = (x[2] * x[2] * (y[3] - y[1]) - x[1] * (y[3] - y[2]) - x[3] * (y[2] - y[1])) / (2 * (x[2] * (y[3] - y[1]) - x[1] * (y[3] - y[2]) - x[3] * (y[2] - y[1])));
				y[0] = trimPointCircleDistance(x[0], radius, circlePosition, trimPoint);
				bool improvementFound = false;
				
				for( size_t jj = 1; jj <= 3; ++jj )
				{
					if( y[jj] < EPS_DEFAULT )
					{
						resultAngle = x[jj];
						return resultAngle;
					}

					if( y[jj] < y[0] )
					{
						// improvement found
						angleStep *= 0.7;
						x[1] = x[jj] - angleStep;
						x[2] = x[jj];
						x[3] = x[jj] + angleStep;
						resultAngle = x[jj];
						x[0] = x[jj];
						y[0] = y[jj];
						improvementFound = true;
					}
				}

				if( !improvementFound )
				{
					double bestX = x[2];
					double bestY = y[2];

					if( y[1] < bestY )
					{
						bestX = x[1];
						bestY = y[1];
					}
					if( y[3] < bestY )
					{
						bestX = x[3];
						bestY = y[3];
					}

					if( angleStep < 3 )
					{
						angleStep += 0.03;
					}
					x[1] = bestX - angleStep;
					x[2] = bestX;
					x[3] = bestX + angleStep;
				}
			}
		}

		double angle = resultAngle;
		size_t smallestDistanceAngle = resultAngle;
		double angleStep = 0.1;
		double x0 = smallestDistanceAngle;
		double x1 = smallestDistanceAngle + angleStep;
		double x2 = x1;
		double x3 = x1;
		double f0 = trimPointCircleDistance(x0, radius, circlePosition, trimPoint);
		double f1 = trimPointCircleDistance(x1, radius, circlePosition, trimPoint);
		double f2 = f1;
		x2 = regula( x0, x1, f0, f1);

		for( size_t ii = 0; ii < 40; ++ii)
		{
			f0 = trimPointCircleDistance(x0, radius, circlePosition, trimPoint);
			f2 = trimPointCircleDistance(x2, radius, circlePosition, trimPoint);
			if(  f0 < f2 )
			{
				// no improvement found
				x1 = x2;
			}
			else
			{
				// improvement found
				x0 = x2;
				resultAngle = x2;
			}

			if( f0 < EPS_DEFAULT )
			{
				resultAngle = x0;
				break;
			}

			if( f2 < EPS_DEFAULT )
			{
				resultAngle = x2;
				break;
			}

			f0 = trimPointCircleDistance(x0, radius, circlePosition, trimPoint);
			f1 = trimPointCircleDistance(x1, radius, circlePosition, trimPoint);
			x3 = regula(x0,x1, f0, f1);
			if(fabs(x3-x2) < EPS_M9)
			{
				break;
			}

			if( f0 < EPS_DEFAULT )
			{
				resultAngle = x0;
				break;
			}
			if( f1 < EPS_DEFAULT )
			{
				resultAngle = x1;
				break;
			}
			x2=x3;
		}

		if( resultAngle < -2.0*M_PI )
		{
			resultAngle = -2.0 * M_PI;
		}
		
		if( resultAngle > 2.0*M_PI )
		{
			resultAngle = 2.0 * M_PI;
		}
		return resultAngle;
	}
};

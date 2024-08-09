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
#include "GeomDebugDump.h"
#include "IncludeCarveHeaders.h"

using namespace IFC4X3;

//\brief class to convert IFC point representations into carve input geometry
class PointConverter : public StatusCallback
{
protected:
	shared_ptr<UnitConverter>		m_unit_converter;

public:
	PointConverter(shared_ptr<UnitConverter>& uc) : m_unit_converter(uc)
	{
	}

	virtual ~PointConverter() {}

	const shared_ptr<UnitConverter>& getUnitConverter() { return m_unit_converter; }
	void setUnitConverter(shared_ptr<UnitConverter>& unit_converter)
	{
		m_unit_converter = unit_converter;
	}

	static bool convertIfcCartesianPoint(const shared_ptr<IfcCartesianPoint>& ifc_point, vec3& point, double length_factor)
	{
		if (!ifc_point)
		{
			return false;
		}
		if (!std::isnan(ifc_point->m_Coordinates[2]))
		{
			double x = ifc_point->m_Coordinates[0] * length_factor;
			double y = ifc_point->m_Coordinates[1] * length_factor;
			double z = ifc_point->m_Coordinates[2] * length_factor;
			point = carve::geom::VECTOR(x, y, z);
			return true;
		}
		else
		{
			double x = ifc_point->m_Coordinates[0] * length_factor;
			double y = ifc_point->m_Coordinates[1] * length_factor;
			point = carve::geom::VECTOR(x, y, 0.0);
			return true;
		}
	}

	void convertIfcCartesianPointVector(const std::vector<shared_ptr<IfcCartesianPoint> >& points, std::vector<vec3>& loop) const
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		const size_t num_points = points.size();
		for (size_t i_point = 0; i_point < num_points; ++i_point)
		{
			const shared_ptr<IfcCartesianPoint>& cartesian_point = points[i_point];
			if (!cartesian_point)
			{
				continue;
			}

			if (!std::isnan(cartesian_point->m_Coordinates[2]))
			{
				double x = cartesian_point->m_Coordinates[0] * length_factor;
				double y = cartesian_point->m_Coordinates[1] * length_factor;
				double z = cartesian_point->m_Coordinates[2] * length_factor;
				loop.push_back(carve::geom::VECTOR(x, y, z));
			}
			else
			{
				double x = cartesian_point->m_Coordinates[0] * length_factor;
				double y = cartesian_point->m_Coordinates[1] * length_factor;
				loop.push_back(carve::geom::VECTOR(x, y, 0.0));
			}
		}
	}
	void convertIfcCartesianPointVector2D(const std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& vec_points_in, std::vector<vec3>& vertices)
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		for (size_t ii = 0; ii < vec_points_in.size(); ++ii)
		{
			const std::vector<shared_ptr<IfcCartesianPoint> >& points_inner = vec_points_in[ii];
			for (size_t jj = 0; jj < points_inner.size(); ++jj)
			{
				const shared_ptr<IfcCartesianPoint>& cartesian_point = points_inner[jj];

				if (!cartesian_point)
				{
					continue;
				}

				if (!std::isnan(cartesian_point->m_Coordinates[2]))
				{
					vertices.push_back(carve::geom::VECTOR(cartesian_point->m_Coordinates[0] * length_factor, cartesian_point->m_Coordinates[1] * length_factor, cartesian_point->m_Coordinates[2] * length_factor));
				}
				else
				{
					vertices.push_back(carve::geom::VECTOR(cartesian_point->m_Coordinates[0] * length_factor, cartesian_point->m_Coordinates[1] * length_factor, 0.0));
				}
			}
		}
	}
	void convertIfcCartesianPointVectorSkipDuplicates(const std::vector<shared_ptr<IfcCartesianPoint> >& vec_ifc_points, std::vector<vec3>& loop, double eps) const
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		vec3  vertex_previous;
		for (size_t ii = 0; ii < vec_ifc_points.size(); ++ii)
		{
			const shared_ptr<IfcCartesianPoint>& cartesian_point = vec_ifc_points[ii];
			double x = 0.0, y = 0.0, z = 0.0;
			if (cartesian_point)
			{
				if (!std::isnan(cartesian_point->m_Coordinates[2]))
				{
					x = cartesian_point->m_Coordinates[0] * length_factor;
					y = cartesian_point->m_Coordinates[1] * length_factor;
					z = cartesian_point->m_Coordinates[2] * length_factor;
				}
				else
				{
					x = cartesian_point->m_Coordinates[0] * length_factor;
					y = cartesian_point->m_Coordinates[1] * length_factor;
				}
			}

			// skip duplicate vertices
			if (ii > 0)
			{
				if (std::abs(x - vertex_previous.x) < eps)
				{
					if (std::abs(y - vertex_previous.y) < eps)
					{
						if (std::abs(z - vertex_previous.z) < eps)
						{
							continue;
						}
					}
				}
			}
			loop.push_back(carve::geom::VECTOR(x, y, z));
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
				double x = coords1[0]->m_value * length_factor;
				double y = coords1[1]->m_value * length_factor;
				double z = coords1[2]->m_value * length_factor;
				loop.push_back(carve::geom::VECTOR(x, y, z));
			}
			else if (coords1.size() > 1)
			{
				double x = coords1[0]->m_value * length_factor;
				double y = coords1[1]->m_value * length_factor;
				loop.push_back(carve::geom::VECTOR(x, y, 0.0));
			}
		}
	}

	static bool convertIfcVertex(const shared_ptr<IfcVertex>& vertex, vec3& point_result, const double length_factor)
	{
		if (vertex)
		{
			shared_ptr<IfcVertexPoint> vertex_point = dynamic_pointer_cast<IfcVertexPoint>(vertex);
			if (vertex_point)
			{
				if (vertex_point->m_VertexGeometry)
				{
					const shared_ptr<IfcPoint>& vertex_point_geometry = vertex_point->m_VertexGeometry;
					// ENTITY IfcPoint ABSTRACT SUPERTYPE OF(ONEOF(IfcCartesianPoint, IfcPointOnCurve, IfcPointOnSurface))
					shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>(vertex_point_geometry);
					if (cartesian_point)
					{
						if (!std::isnan(cartesian_point->m_Coordinates[2]))
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
		}
		return false;
	}

	static void convertIfcVertex(const shared_ptr<IfcVertex>& vertex, shared_ptr<IfcCartesianPoint>& point_result, const double length_factor)
	{
		if (vertex)
		{
			shared_ptr<IfcVertexPoint> vertex_point = dynamic_pointer_cast<IfcVertexPoint>(vertex);
			if (vertex_point)
			{
				if (vertex_point->m_VertexGeometry)
				{
					const shared_ptr<IfcPoint>& vertex_point_geometry = vertex_point->m_VertexGeometry;
					// ENTITY IfcPoint ABSTRACT SUPERTYPE OF(ONEOF(IfcCartesianPoint, IfcPointOnCurve, IfcPointOnSurface))
					shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>(vertex_point_geometry);
					if (cartesian_point)
					{
						point_result = cartesian_point;
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
		}
	}

	

	static void convertPointToTrimmingSelect(const shared_ptr<IfcVertex> &vertex, double length_factor, std::vector<shared_ptr<IfcTrimmingSelect> >& trim_vec)
	{
		std::vector<shared_ptr<IfcTrimmingSelect> > curve_trim2_vec;
		shared_ptr<IfcCartesianPoint> trimPoint1;
		PointConverter::convertIfcVertex(vertex, trimPoint1, length_factor);
		if (trimPoint1)
		{
			trim_vec.push_back(trimPoint1);
		}
	}

	static double trimPointCircleDistance(double angle, double radius, const carve::math::Matrix& circlePosition, const vec3& trimPoint)
	{
		while (angle > 2.0 * M_PI) { angle -= 2.0 * M_PI; }
		while (angle < -2.0 * M_PI) { angle += 2.0 * M_PI; }
		vec3 circlePoint = carve::geom::VECTOR(radius * cos(angle), radius * sin(angle), 0);
		circlePoint = circlePosition * circlePoint;
		double distance = (trimPoint - circlePoint).length();
		return distance;
	}

	static void adjustTrimpointPositionOnConic(const vec3& circleCenter, double radius1, double radius2, vec3& trimPoint,
		const carve::math::Matrix& circlePosition, double eps)
	{
		if (std::abs(radius1 - radius2) < eps * 10)
		{
			double trimPointDistance2 = (circleCenter - trimPoint).length() - radius1;
			if (std::abs(trimPointDistance2) > eps * eps * 10)
			{
				// trim point could be given in local coordinates
				vec3 trimPointGlobal = circlePosition * trimPoint;

				double trimPointDistance3 = (circleCenter - trimPointGlobal).length() - radius1;

				// trim points might be given not super accurate, so don't check against epsilon. Check only if significant closer
				if (std::abs(trimPointDistance3) * 100 < std::abs(trimPointDistance2))
				{
					trimPoint = trimPointGlobal;
				}
			}
		}
	}

	//\brief: returns the corresponding angle in radian. angle 0 is on the positive x-axis.
	static double getAngleOnConic(const vec3& circleCenter, double radius1, double radius2, vec3& trimPoint, const carve::math::Matrix& circlePosition, const carve::math::Matrix& circlePositionInverse, double eps)
	{
#ifdef _DEBUG
		if (false)
		{
			std::vector<vec3> circlePoints;
			carve::math::Matrix m;
			GeomUtils::getCirclePoints(radius1, radius2, 0, M_PI * 0.5, 10, m, circlePoints);

			vec4 color(0.3, 0.4, 0.5, 1.0);
			GeomDebugDump::dumpPolyline(circlePoints, color, 1.0, false, false);
			std::string label = "p";
			GeomDebugDump::dumpVertex(trimPoint, color, label);

			vec3 trimPoint2 = circlePosition * trimPoint;
			std::string label2 = "p2";
			GeomDebugDump::dumpVertex(trimPoint2, color, label2);
		}
#endif

		adjustTrimpointPositionOnConic(circleCenter, radius1, radius2, trimPoint, circlePosition, eps);
		
		vec3 center2trimPoint = trimPoint - circleCenter;
		vec3 center2trimPointDirection = center2trimPoint;
		center2trimPointDirection.normalize();

		vec3 circleCenter2D = circlePositionInverse * circleCenter;
		vec3 trimPoint2D = circlePositionInverse * trimPoint;
		vec3 trimPointRelative2D = trimPoint2D - circleCenter2D;
		trimPointRelative2D.normalize();
		double resultAngle1 = atan2(trimPointRelative2D.y, trimPointRelative2D.x);
		//resultAngle1 = std::round(resultAngle1 * 100000000000000) * 0.00000000000001;
		while (resultAngle1 < -eps)
		{
			resultAngle1 += 2.0 * M_PI;
		}
		while (resultAngle1 >= 2.0 * M_PI)
		{
			resultAngle1 -= 2.0 * M_PI;
		}

#ifdef _DEBUG
		{
			double openingAngle1 = 0.1;  // doesn't really matter
			std::vector<vec3> points;
			GeomUtils::getCirclePoints(radius1, radius2, resultAngle1, openingAngle1, 5, circlePosition, points);
			vec3 check1 = points[0];

			vec3 circleCenter3DCheck = circlePosition * carve::geom::VECTOR(0, 0, 0);
			vec3 circlePointCheck = carve::geom::VECTOR(radius1 * cos(resultAngle1), radius2 * sin(resultAngle1), 0);
			circlePointCheck = circlePosition * circlePointCheck;
			vec3 check = circlePointCheck - trimPoint;
			double distance1FromTrimPoint = check.length();

			double maxAllowedTolerance = 0.005;
			if (distance1FromTrimPoint > maxAllowedTolerance)
			{
				std::cout << "getAngleOnCircle: point not on cicle. distance: " << distance1FromTrimPoint << std::endl;
			}
			double distanceCheck = (check1 - circlePointCheck).length();

			std::vector<vec3> circlePoints;
			carve::math::Matrix m;
			double openingAngle = M_PI * 0.5;
			GeomUtils::getCirclePoints(radius1, radius2, resultAngle1, openingAngle, 10, m, circlePoints);

			if (false) {
				vec4 color(0.3, 0.4, 0.5, 0.3);
				GeomDebugDump::dumpPolyline(circlePoints, color, 3.0, false, false);

				vec3 circlePointResult = carve::geom::VECTOR(radius1 * cos(resultAngle1), radius2 * sin(resultAngle1), 0);
				circlePointResult = circlePosition * circlePointResult;
				std::string label2 = "x";
				GeomDebugDump::dumpVertex(circlePointResult, color, label2);
			}
		}
#endif
		return resultAngle1;
	}
};

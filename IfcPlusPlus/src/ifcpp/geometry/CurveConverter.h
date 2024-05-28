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

#include <optional>
#include <ifcpp/geometry/GeometryException.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/UnitConverter.h>
#include <IfcArcIndex.h>
#include <IfcBoolean.h>
#include <IfcBSplineCurve.h>
#include <IfcCartesianPoint.h>
#include <IfcCartesianPointList2D.h>
#include <IfcCartesianPointList3D.h>
#include <IfcCircle.h>
#include <IfcConic.h>
#include <IfcCompositeCurve.h>
#include <IfcCompositeCurveSegment.h>
#include <IfcCurve.h>
#include <IfcCurveSegment.h>
#include <IfcDirection.h>
#include <IfcEdgeCurve.h>
#include <IfcEdgeLoop.h>
#include <IfcEllipse.h>
#include <IfcIndexedPolyCurve.h>
#include <IfcLengthMeasure.h>
#include <IfcLine.h>
#include <IfcLineIndex.h>
#include <IfcLoop.h>
#include <IfcOffsetCurve2D.h>
#include <IfcOffsetCurve3D.h>
#include <IfcOrientedEdge.h>
#include <IfcParameterValue.h>
#include <IfcPcurve.h>
#include <IfcPolyline.h>
#include <IfcPolyLoop.h>
#include <IfcPositiveLengthMeasure.h>
#include <IfcSubedge.h>
#include <IfcTrimmedCurve.h>
#include <IfcTrimmingSelect.h>
#include <IfcVector.h>
#include <IfcVertexPoint.h>

#include "SplineConverter.h"
#include "GeomUtils.h"
#include "GeomDebugDump.h"
#include "PlacementConverter.h"
#include "PointConverter.h"
#include "SplineConverter.h"
#include "IncludeCarveHeaders.h"


//\brief class to convert different types of IFC curve representations into carve input geometry
class CurveConverter : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings>	m_geom_settings;
	shared_ptr<PlacementConverter>	m_placement_converter;
	shared_ptr<PointConverter>		m_point_converter;
	shared_ptr<SplineConverter>		m_spline_converter;
	bool m_debugDumpGeometry = false;

public:
	CurveConverter(shared_ptr<GeometrySettings>& gs, shared_ptr<PlacementConverter>& placement_converter, shared_ptr<PointConverter>& pc, shared_ptr<SplineConverter>& sc);

	virtual ~CurveConverter();

	struct CurveSegment
	{
		void reverseOrientation()
		{
			std::reverse(m_points.begin(), m_points.end());
			if (m_type == CURVE_TYPE_ARC)
			{
				arcStartAngle += arcOpeningAngle;
				arcOpeningAngle = -arcOpeningAngle;
			}
		}
		enum CurveType
		{
			CURVE_TYPE_LINE, CURVE_TYPE_POLYLINE, CURVE_TYPE_ARC, CURVE_TYPE_CIRCLE, CURVE_TYPE_NURBS
		};
		CurveSegment(CurveType t) { m_type = t; }
		CurveType m_type;
		std::vector<vec3> m_points;
		vec3 arcOrCircleCenter, arcOrCircleAxis;  // only in case of arc and circle
		double arcStartAngle = 0, arcOpeningAngle = 0;
	};

	static void appendSegments(const std::vector<CurveSegment>& segmentsToAppend, std::vector<CurveSegment>& target_vec, double epsilonMergePoints);
	
	const shared_ptr<GeometrySettings>& getGeomSettings() { return m_geom_settings; }
	const shared_ptr<PlacementConverter>& getPlacementConverter() { return m_placement_converter; }
	const shared_ptr<PointConverter>& getPointConverter() { return m_point_converter; }
	const shared_ptr<SplineConverter>& getSplineConverter() { return m_spline_converter; }

	void convertIfcCurve2D(const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec2>& loops, std::vector<vec2>& segment_start_points, bool senseAgreement) const;

	void convertIfcCurve2D(const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec2>& target_vec, std::vector<vec2>& segment_start_points,
		std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool senseAgreement) const;

	void convertIfcCurve(const shared_ptr<IfcCurve>& ifc_curve, std::vector<CurveSegment>& segments,
		bool senseAgreement) const;

	void convertIfcCurve(const shared_ptr<IfcCurve>& ifc_curve, std::vector<CurveSegment>& resultSegments,
		std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool senseAgreement) const;
	
	void getTrimAngle(const std::vector<shared_ptr<IfcTrimmingSelect> >& trimSelect1, const vec3& circleCenter, double radius1, double radius2,
		double& trimAngle1, const carve::math::Matrix& circlePlacement, const carve::math::Matrix& circlePlacementInverse) const;

	void getTrimPoints(const std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, const std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec,
		const carve::math::Matrix& circlePosition, double circleRadius, double circleRadius2, bool senseAgreement, std::optional<vec3>& trimPoint1,
		std::optional<vec3>& trimPoint2) const;

	void getTrimPoints(const std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, const std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, std::optional<vec3>& trimPoint1, std::optional<vec3>& trimPoint2) const;

	void convertIfcPolyline(const shared_ptr<IfcPolyline>& polyLine, std::vector<vec3>& loop) const
	{
		m_point_converter->convertIfcCartesianPointVector(polyLine->m_Points, loop);
	}

	void convertIfcEdge(const shared_ptr<IfcEdge>& edge, std::vector<vec3>& resultPoints, double lengthFactor) const;

	void convertIfcLoop(const shared_ptr<IfcLoop>& loop, std::vector<vec3>& loopPoints) const;

	static void computeOpeningAngle(double startAngle, double endAngle, double eps, bool senseAgreement, double& openingAngle);
};

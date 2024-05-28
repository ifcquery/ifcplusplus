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
#include "CurveConverter.h"

CurveConverter::CurveConverter(shared_ptr<GeometrySettings>& gs, shared_ptr<PlacementConverter>& placement_converter, shared_ptr<PointConverter>& pc, shared_ptr<SplineConverter>& sc)
		: m_geom_settings(gs), m_placement_converter(placement_converter), m_point_converter(pc), m_spline_converter(sc)
{
}

CurveConverter:: ~CurveConverter() {}

void CurveConverter::appendSegments(const std::vector<CurveSegment>& segmentsToAppend, std::vector<CurveSegment>& target_vec, double epsilonMergePoints)
{
	if (segmentsToAppend.size() == 0)
	{
		return;
	}

	if (target_vec.size() == 0)
	{
		std::copy(segmentsToAppend.begin(), segmentsToAppend.end(), std::back_inserter(target_vec));
		return;
	}

	for (CurveSegment newSegment : segmentsToAppend)
	{
		// check if last point of target_vec is equal to first point of segments
		const vec3& lastPointExistingSegments = target_vec.back().m_points.back();
		const vec3& firstPointOfNewSegment = newSegment.m_points.front();
		if ((lastPointExistingSegments - firstPointOfNewSegment).length() < epsilonMergePoints)
		{
			// normal case, last point of target_vec is equal to first point of segments
			target_vec.push_back(newSegment);
			continue;
		}

		const vec3& lastPointOfNewSegment = newSegment.m_points.back();
		if ((lastPointExistingSegments - lastPointOfNewSegment).length() < epsilonMergePoints)
		{
#ifdef _DEBUG
			vec4 color1(0.2, 0.8, 0.2, 1.0);
			for (auto existing : target_vec) {
				GeomDebugDump::dumpPolyline(existing.m_points, color1, 1., false, false);
			}
			vec4 color2(0.2, 0.8, 0.9, 1.0);
			GeomDebugDump::dumpPolyline(newSegment.m_points, color2, 1., false, false);
#endif
			// new segment matches if reversed
			newSegment.reverseOrientation();
			target_vec.push_back(newSegment);
			continue;
		}

		const vec3& firstPointExistingSegments = target_vec.front().m_points.front();
		if ((firstPointExistingSegments - lastPointOfNewSegment).length() < epsilonMergePoints)
		{
			// new segment matches beginning of existing segments
			target_vec.insert(target_vec.begin(), newSegment);
			continue;
		}

		if ((firstPointExistingSegments - firstPointOfNewSegment).length() < epsilonMergePoints)
		{
			// new segment reversed matches beginning of existing segments
			newSegment.reverseOrientation();
			target_vec.insert(target_vec.begin(), newSegment);
			continue;
		}

		// assume that new segment matches end of existing segments
		target_vec.push_back(newSegment);
	}
}


void CurveConverter::convertIfcCurve2D(const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec2>& loops, std::vector<vec2>& segment_start_points, bool senseAgreement) const
{
	std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
	std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
	convertIfcCurve2D(ifc_curve, loops, segment_start_points, trim1_vec, trim2_vec, senseAgreement);
}

void CurveConverter::convertIfcCurve2D(const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec2>& target_vec, std::vector<vec2>& segment_start_points,
	std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool senseAgreement) const
{
	std::vector<CurveSegment> segments;
	convertIfcCurve(ifc_curve, segments, senseAgreement);

	for (size_t ii = 0; ii < segments.size(); ++ii)
	{
		CurveSegment& seg = segments[ii];
		for (size_t jj = 0; jj < seg.m_points.size(); ++jj)
		{
			vec3& point_3d = seg.m_points[jj];
			target_vec.push_back(carve::geom::VECTOR(point_3d.x, point_3d.y));
		}
	}
}

void CurveConverter::convertIfcCurve(const shared_ptr<IfcCurve>& ifc_curve, std::vector<CurveSegment>& segments,
	bool senseAgreement) const
{
	std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
	std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
	convertIfcCurve(ifc_curve, segments, trim1_vec, trim2_vec, senseAgreement);
}

bool trimpointWarning = true;
void CurveConverter::convertIfcCurve(const shared_ptr<IfcCurve>& ifc_curve, std::vector<CurveSegment>& resultSegments,
	std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool senseAgreement) const
{
	double lengthFactor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();
	double epsilonMergePoints = m_geom_settings->getEpsilonMergePoints();

	//	ENTITY IfcCurve ABSTRACT SUPERTYPE OF	(ONEOF(IfcBoundedCurve, IfcConic, IfcLine, IfcOffsetCurve2D, IfcOffsetCurve3D, IfcPCurve))
	shared_ptr<IfcBoundedCurve> bounded_curve = dynamic_pointer_cast<IfcBoundedCurve>(ifc_curve);
	if (bounded_curve)
	{
		shared_ptr<IfcCompositeCurve> composite_curve = dynamic_pointer_cast<IfcCompositeCurve>(bounded_curve);
		if (composite_curve)
		{
			// ENTITY IfcBoundedCurve ABSTRACT SUPERTYPE OF	(ONEOF(IfcCompositeCurve, IfcPolyline, IfcTrimmedCurve, IfcBSplineCurve))
			for (const shared_ptr<IfcSegment>& ifcSegment : composite_curve->m_Segments)
			{
				// ENTITY IfcSegment ABSTRACT SUPERTYPE OF (ONEOF (IfcCompositeCurveSegment ,IfcCurveSegment))
				shared_ptr<IfcCompositeCurveSegment> compositeCurveSegment = dynamic_pointer_cast<IfcCompositeCurveSegment>(ifcSegment);
				if (compositeCurveSegment)
				{
					shared_ptr<IfcCurve> segment_curve = compositeCurveSegment->m_ParentCurve;

					std::vector<CurveSegment> segment_vec;
					convertIfcCurve(segment_curve, segment_vec, senseAgreement);
					if (segment_vec.size() > 0)
					{
						appendSegments(segment_vec, resultSegments, epsilonMergePoints);
					}
					continue;
				}

				shared_ptr<IfcCurveSegment> curveSegment = dynamic_pointer_cast<IfcCurveSegment>(ifcSegment);
				if (curveSegment)
				{
					//shared_ptr<IfcPlacement>								m_Placement;
					//shared_ptr<IfcCurveMeasureSelect>						m_SegmentStart;
					//shared_ptr<IfcCurveMeasureSelect>						m_SegmentLength;
					//shared_ptr<IfcCurve>									m_ParentCurve;

					shared_ptr<IfcCurve> segment_curve = curveSegment->m_ParentCurve;

					std::vector<CurveSegment> segment_vec;
					convertIfcCurve(segment_curve, segment_vec, senseAgreement);
					if (segment_vec.size() > 0)
					{
						appendSegments(segment_vec, resultSegments, epsilonMergePoints);
					}
					continue;
				}
			}

#ifdef _DEBUG
			if (resultSegments.size() == 6)
			{
				vec4 color(0.4, 0.6, 0.6, 1.0);
				GeomDebugDump::dumpLocalCoordinateSystem();
				GeomDebugDump::dumpPolyline(resultSegments.front().m_points, color, 1.0, true, false);
			}
#endif
			return;
		}

		shared_ptr<IfcPolyline> polyLine = dynamic_pointer_cast<IfcPolyline>(ifc_curve);
		if (polyLine)
		{
			std::vector<shared_ptr<IfcCartesianPoint> >& points = polyLine->m_Points;
			if (points.size() > 0)
			{
				resultSegments.push_back(CurveSegment(CurveSegment::CURVE_TYPE_POLYLINE));
				CurveSegment& currentSegment = resultSegments.back();
				m_point_converter->convertIfcCartesianPointVector(points, currentSegment.m_points);
				if (!senseAgreement)
				{
					currentSegment.reverseOrientation();
				}
			}
			return;
		}

		shared_ptr<IfcTrimmedCurve> trimmed_curve = dynamic_pointer_cast<IfcTrimmedCurve>(bounded_curve);
		if (trimmed_curve)
		{
			shared_ptr<IfcCurve> basis_curve = trimmed_curve->m_BasisCurve;
			if (basis_curve)
			{
				std::vector<CurveSegment> basisCurveSegment;
				std::vector<shared_ptr<IfcTrimmingSelect> >& curveTrim1vec = trimmed_curve->m_Trim1;
				std::vector<shared_ptr<IfcTrimmingSelect> >& curveTrim2vec = trimmed_curve->m_Trim2;
				bool trimmed_senseAgreement = true;
				if (trimmed_curve->m_SenseAgreement) { trimmed_senseAgreement = trimmed_curve->m_SenseAgreement->m_value; }
				if (!senseAgreement)
				{
					trimmed_senseAgreement = !trimmed_senseAgreement;
				}

				convertIfcCurve(basis_curve, basisCurveSegment, curveTrim1vec, curveTrim2vec, trimmed_senseAgreement);
				appendSegments(basisCurveSegment, resultSegments, epsilonMergePoints);
			}
			return;
		}

		shared_ptr<IfcBSplineCurve> bspline_curve = dynamic_pointer_cast<IfcBSplineCurve>(bounded_curve);
		if (bspline_curve)
		{
			CurveSegment seg(CurveSegment::CURVE_TYPE_NURBS);
			std::vector<vec3> segmentStartPoints;
			m_spline_converter->convertBSplineCurve(bspline_curve, seg.m_points, segmentStartPoints);

			std::optional<vec3> trimPoint1;
			std::optional<vec3> trimPoint2;
			getTrimPoints(trim1_vec, trim2_vec, trimPoint1, trimPoint2);

			resultSegments.push_back(seg);
#ifdef _DEBUG
			//GeomDebugDump::clearMeshsetDump();
			//vec4 color(0.2, 0.2, 0.2, 0.8);
			//GeomDebugDump::dumpPolyline(target_vec, color, 1.0, false);

			//if (trimPoint1.has_value() && trimPoint2.has_value())
			//{
			//	std::vector<vec3> trimPoints = { trimPoint1.value(), trimPoint2.value() };
			//	GeomDebugDump::dumpPolyline(trimPoints, color, 1.0, false);
			//}
#endif

			// TODO: handle trim points
			return;
		}

		shared_ptr<IfcIndexedPolyCurve> indexed_poly_curve = dynamic_pointer_cast<IfcIndexedPolyCurve>(bounded_curve);
		if (indexed_poly_curve)
		{
			shared_ptr<IfcCartesianPointList>& pointList = indexed_poly_curve->m_Points;
			if (!pointList)
			{
				return;
			}

			// IfcIndexedPolyCurve -----------------------------------------------------------
			std::vector<vec3> pointVec;

			shared_ptr<IfcCartesianPointList2D> pointList2D = dynamic_pointer_cast<IfcCartesianPointList2D>(pointList);
			if (pointList2D)
			{
				m_point_converter->convertPointList(pointList2D->m_CoordList, pointVec);
			}
			else
			{
				shared_ptr<IfcCartesianPointList3D> pointList3D = dynamic_pointer_cast<IfcCartesianPointList3D>(pointList);
				if (pointList3D)
				{
					m_point_converter->convertPointList(pointList3D->m_CoordList, pointVec);
				}
			}

			int tag = indexed_poly_curve->m_tag;
			const std::vector<shared_ptr<IfcSegmentIndexSelect> >& segments = indexed_poly_curve->m_Segments;					//optional
			if (segments.size() > 0)
			{
				for (size_t ii = 0; ii < segments.size(); ++ii)
				{
					const shared_ptr<IfcSegmentIndexSelect>& segment = segments[ii];

					shared_ptr<IfcLineIndex> lineIdx = dynamic_pointer_cast<IfcLineIndex>(segment);
					if (lineIdx)
					{
						if (lineIdx->m_vec.size() > 1)
						{
							std::vector<vec3> line_points_3d;

							for (size_t jj = 0; jj < lineIdx->m_vec.size(); ++jj)
							{
								int idx0 = lineIdx->m_vec[jj]->m_value - 1;
								if (idx0 < pointVec.size())
								{
									const vec3& pt0 = pointVec[idx0];

									line_points_3d.push_back(pt0);
								}
							}

							if (line_points_3d.size() > 1)
							{
								CurveSegment seg(CurveSegment::CURVE_TYPE_LINE);
								GeomUtils::appendPointsToCurve(line_points_3d, seg.m_points, epsilonMergePoints);
								resultSegments.push_back(seg);
							}
						}
						continue;
					}

					shared_ptr<IfcArcIndex> arcIdx = dynamic_pointer_cast<IfcArcIndex>(segment);
					if (arcIdx)
					{
						if (arcIdx->m_vec.size() < 3)
						{
							continue;
						}

						if (arcIdx->m_vec.size() > 3)
						{
#ifdef _DEBUG
							std::cout << "IfcIndexedPolyCurve: arc index with > 3 points not implemented" << std::endl;
#endif
						}

						if (arcIdx->m_vec[0] && arcIdx->m_vec[1] && arcIdx->m_vec[2])
						{
							int idx0 = arcIdx->m_vec[0]->m_value - 1;
							int idx1 = arcIdx->m_vec[1]->m_value - 1;
							int idx2 = arcIdx->m_vec[2]->m_value - 1;
							if (idx0 >= 0 && idx1 >= 0 && idx2 >= 0)
							{
								if (idx0 < pointVec.size() && idx1 < pointVec.size() && idx2 < pointVec.size())
								{
									const vec3& p0 = pointVec[idx0];
									const vec3& p1 = pointVec[idx1];
									const vec3& p2 = pointVec[idx2];

									const vec3 t = p1 - p0;
									const vec3 u = p2 - p0;
									const vec3 v = p2 - p1;

									const vec3 w = carve::geom::cross(t, u);
									const double wsl = w.length2();
									if (wsl > 10e-14)
									{
										const double iwsl2 = 1.0 / (2.0 * wsl);
										const double tt = carve::geom::dot(t, t);
										const double uu = carve::geom::dot(u, u);

										vec3 circ_center = p0 + (u * tt * (carve::geom::dot(u, v)) - t * uu * (carve::geom::dot(t, v))) * iwsl2;
										vec3 circAxis = w / sqrt(wsl);
										vec3 center_p0 = p0 - circ_center;
										vec3 center_p1 = p1 - circ_center;
										vec3 center_p2 = p2 - circ_center;
										vec3 center_p0_normalized = center_p0.normalized();
										vec3 center_p2_normalized = center_p2.normalized();

										double dotProduct = carve::geom::dot(center_p0_normalized, center_p2_normalized);
										if (dotProduct < -1.0) { dotProduct = -1.0; }
										if (dotProduct > 1.0) { dotProduct = 1.0; }
										double openingAngle = std::acos(dotProduct);
										if (openingAngle < -M_PI * 2.0) { openingAngle = -M_PI * 2.0; }
										if (openingAngle > M_PI * 2.0) { openingAngle = M_PI * 2.0; }
										size_t n = m_geom_settings->getNumVerticesPerCircle() * openingAngle / (M_PI * 2.0);
										if (n < m_geom_settings->getMinNumVerticesPerArc())
										{
											n = m_geom_settings->getMinNumVerticesPerArc();
										}

										const double deltaAngle = openingAngle / (double)(n - 1);
										double angle = 0;

										CurveSegment seg(CurveSegment::CURVE_TYPE_ARC);
										seg.arcOrCircleCenter = circ_center;
										seg.arcOrCircleAxis = circAxis;
										seg.arcStartAngle = 0;
										seg.arcOpeningAngle = openingAngle;
										resultSegments.push_back(seg);
										for (size_t kk = 0; kk < n; ++kk)
										{
											carve::math::Matrix m = carve::math::Matrix::ROT(angle, circAxis, epsilonMergePoints);

											vec3 p_rotated = center_p0;
											p_rotated = m * p_rotated + circ_center;

											seg.m_points.push_back(p_rotated);
											angle += deltaAngle;
										}
									}
								}
							}
						}
					}
				}
			}
			else
			{
				// no segments, take all points from CoordList
				if (pointVec.size() > 0)
				{
					CurveSegment seg(CurveSegment::CURVE_TYPE_POLYLINE);
					std::copy(pointVec.begin(), pointVec.end(), std::back_inserter(seg.m_points));
					resultSegments.push_back(seg);
				}
			}

			return;
		}
		throw UnhandledRepresentationException(bounded_curve);
	}

	shared_ptr<IfcConic> conic = dynamic_pointer_cast<IfcConic>(ifc_curve);
	if (conic)
	{
		// ENTITY IfcConic ABSTRACT SUPERTYPE OF(ONEOF(IfcCircle, IfcEllipse))
		shared_ptr<TransformData> conicPositionMatrix(new TransformData());
		shared_ptr<IfcPlacement> conic_placement = dynamic_pointer_cast<IfcPlacement>(conic->m_Position);
		if (conic_placement)
		{
			m_placement_converter->convertIfcPlacement(conic_placement, conicPositionMatrix, false);
		}

		vec3 circleCenter = conicPositionMatrix->m_matrix * carve::geom::VECTOR(0, 0, 0);

		double circleRadius = -1;
		double circleRadius2 = -1;
		shared_ptr<IfcEllipse> ellipse = dynamic_pointer_cast<IfcEllipse>(conic);
		if (ellipse)
		{
			if (ellipse->m_SemiAxis1)
			{
				if (ellipse->m_SemiAxis2)
				{
					circleRadius = ellipse->m_SemiAxis1->m_value * lengthFactor;
					circleRadius2 = ellipse->m_SemiAxis2->m_value * lengthFactor;

				}
			}
		}

		shared_ptr<IfcCircle> circle = dynamic_pointer_cast<IfcCircle>(conic);
		if (circle)
		{
			circleRadius = 0.0;
			if (circle->m_Radius)
			{
				circleRadius = circle->m_Radius->m_value * lengthFactor;
			}
		}

		carve::math::Matrix circlePositionInverse;
		GeomUtils::computeInverse(conicPositionMatrix->m_matrix, circlePositionInverse);

		double startAngle = 0, endAngle = 0;
		double openingAngle = M_PI * 2.0;
		getTrimAngle(trim1_vec, circleCenter, circleRadius, circleRadius2, startAngle, conicPositionMatrix->m_matrix, circlePositionInverse);
		getTrimAngle(trim2_vec, circleCenter, circleRadius, circleRadius2, endAngle, conicPositionMatrix->m_matrix, circlePositionInverse);
		computeOpeningAngle(startAngle, endAngle, epsilonMergePoints, senseAgreement, openingAngle);

#if defined( _DEBUG)
		if( trimpointWarning )
		{
			trimpointWarning = false;
			// compute trim points from startAngle, endAngle, then re-compute startAngle, endAngle from these trim points (round trip)
			vec3 trimPoint1 = conicPositionMatrix->m_matrix * carve::geom::VECTOR(circleRadius * cos(startAngle), circleRadius2 * sin(startAngle), 0);
			vec3 trimPoint2 = conicPositionMatrix->m_matrix * carve::geom::VECTOR(circleRadius * cos(endAngle), circleRadius2 * sin(endAngle), 0);

			std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec_check, trim2_vec_check;
			shared_ptr<IfcCartesianPoint> t1(new IfcCartesianPoint());
			double lengthFactor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();
			t1->m_Coordinates[0] = trimPoint1.x / lengthFactor;
			t1->m_Coordinates[1] = trimPoint1.y / lengthFactor;
			t1->m_Coordinates[2] = trimPoint1.z / lengthFactor;
			trim1_vec_check.push_back(t1);

			shared_ptr<IfcCartesianPoint> t2(new IfcCartesianPoint());
			t2->m_Coordinates[0] = trimPoint2.x / lengthFactor;
			t2->m_Coordinates[1] = trimPoint2.y / lengthFactor;
			t2->m_Coordinates[2] = trimPoint2.z / lengthFactor;
			trim2_vec_check.push_back(t2);

			double startAngleCheck = 0, endAngleCheck = 0;
			double openingAngleCheck = M_PI * 2.0;
			getTrimAngle(trim1_vec_check, circleCenter, circleRadius, circleRadius2, startAngleCheck, conicPositionMatrix->m_matrix, circlePositionInverse);
			getTrimAngle(trim2_vec_check, circleCenter, circleRadius, circleRadius2, endAngleCheck, conicPositionMatrix->m_matrix, circlePositionInverse);
			double deltaStartAngle = startAngle - startAngleCheck;
			if (std::abs(deltaStartAngle) > epsilonMergePoints * 10000) {
				std::cout << "trim point 1 round trip check failed" << std::endl;

				size_t num_segments = 10;
				std::vector<vec3> points;
				GeomUtils::getCirclePoints(circleRadius, circleRadius2, startAngle, openingAngle, num_segments, conicPositionMatrix->m_matrix, points);
				vec4 color3(0.4, 0.6, 0.6, 1.0);
				GeomDebugDump::dumpPolyline(points, color3, 2.0, false, false);

				GeomDebugDump::dumpPolyline({ trimPoint1, circleCenter , trimPoint2 }, color3, 2.0, false, false);

			}
			double deltaEndAngle = endAngle - endAngleCheck;
			if (std::abs(deltaEndAngle) > epsilonMergePoints * 10000) {
				std::cout << "trim point 2 round trip check failed" << std::endl;
			}
		}
#endif
		CurveSegment seg(CurveSegment::CURVE_TYPE_CIRCLE);
			
		if (circleRadius > epsilonMergePoints*1000)
		{
			int num_segments = m_geom_settings->getNumVerticesPerCircleWithRadius(circleRadius) * (std::abs(openingAngle) / (2.0 * M_PI));
			if (num_segments < m_geom_settings->getMinNumVerticesPerArc()) num_segments = m_geom_settings->getMinNumVerticesPerArc();
			GeomUtils::getCirclePoints(circleRadius, circleRadius2, startAngle, openingAngle, num_segments,
				conicPositionMatrix->m_matrix, seg.m_points);
			seg.arcStartAngle = startAngle;
			seg.arcOpeningAngle = openingAngle;
			seg.arcOrCircleCenter = circleCenter;
			seg.arcOrCircleAxis = conicPositionMatrix->m_matrix * carve::geom::VECTOR(0, 0, 1);  // TODO: check if this is correct
		}
		else
		{
			seg.arcOrCircleCenter = circleCenter;
			seg.m_points.push_back(seg.arcOrCircleCenter);
		}
		resultSegments.push_back(seg);

#ifdef _DEBUG
		vec4 color(0.2, 0.2, 0.2, 0.8);
		//GeomDebugDump::dumpPolyline(circle_segment_points3D, color, 0, true, false);
#endif
		return;
	}

	shared_ptr<IfcLine> line = dynamic_pointer_cast<IfcLine>(ifc_curve);
	if (line)
	{
		shared_ptr<IfcCartesianPoint> ifc_line_point = line->m_Pnt;
		vec3 line_origin;
		vec3 line_end;
		if (PointConverter::convertIfcCartesianPoint(ifc_line_point, line_origin, lengthFactor))
		{
			// line: lambda(u) = line_point + u*line_direction
			shared_ptr<IfcVector> line_vec = line->m_Dir;
			if (!line_vec)
			{
				return;
			}
			shared_ptr<IfcDirection> ifc_line_direction = line_vec->m_Orientation;
			vec3 line_direction = carve::geom::VECTOR(1,0,0);
			if (ifc_line_direction)
			{
				std::vector<shared_ptr<IfcReal> >& direction_ratios = ifc_line_direction->m_DirectionRatios;

				if (direction_ratios.size() > 1)
				{
					if (direction_ratios.size() > 2)
					{
						line_direction = carve::geom::VECTOR(direction_ratios[0]->m_value, direction_ratios[1]->m_value, direction_ratios[2]->m_value);
					}
					else
					{
						line_direction = carve::geom::VECTOR(direction_ratios[0]->m_value, direction_ratios[1]->m_value, 0);
					}
				}
				line_direction.normalize();
			}

			line_end = line_origin + line_direction;  // will be overwritten by trimming points in most cases
			if (line_vec->m_Magnitude)
			{
				line_end = line_origin + line_direction * line_vec->m_Magnitude->m_value;
			}

			// check for trimming at beginning of line
			double start_parameter = 0.0;
			shared_ptr<IfcParameterValue> trim_par1;
			if (GeomUtils::findFirstInVector(trim1_vec, trim_par1))
			{
				start_parameter = trim_par1->m_value;
				line_origin = line_origin + line_direction * start_parameter;
			}
			else
			{
				shared_ptr<IfcCartesianPoint> ifc_trimPoint;
				if (GeomUtils::findFirstInVector(trim1_vec, ifc_trimPoint))
				{
					vec3 trimPoint;
					PointConverter::convertIfcCartesianPoint(ifc_trimPoint, trimPoint, lengthFactor);
					line_origin = trimPoint;

					vec3 closest_point_on_line;
					GeomUtils::closestPointOnLine(trimPoint, line_origin, line_direction, closest_point_on_line);

					if ((closest_point_on_line - trimPoint).length() < epsilonMergePoints * 10.0)
					{
						// trimming point is on the line
						line_origin = trimPoint;
					}
					else
					{
						line_origin = closest_point_on_line;
					}
				}
			}

			// check for trimming at end of line
			shared_ptr<IfcParameterValue> trim_par2;
			if (GeomUtils::findFirstInVector(trim2_vec, trim_par2))
			{
				line_end = line_origin + line_direction * trim_par2->m_value * lengthFactor;
			}
			else
			{
				shared_ptr<IfcCartesianPoint> ifc_trimPoint;
				if (GeomUtils::findFirstInVector(trim2_vec, ifc_trimPoint))
				{
					vec3 trimPoint;
					PointConverter::convertIfcCartesianPoint(ifc_trimPoint, trimPoint, lengthFactor);
					line_end = trimPoint;

					vec3 closest_point_on_line;
					GeomUtils::closestPointOnLine(trimPoint, line_origin, line_direction, closest_point_on_line);

					if ((closest_point_on_line - trimPoint).length() < 0.0001)
					{
						// trimming point is on the line
						line_end = trimPoint;
					}
					else
					{
						line_end = closest_point_on_line;
					}
				}
			}

			CurveSegment seg(CurveSegment::CURVE_TYPE_LINE);
			seg.m_points.push_back(line_origin);
			seg.m_points.push_back(line_end);
			resultSegments.push_back(seg);
		}
		return;
	}

	shared_ptr<IfcOffsetCurve2D> offset_curve_2d = dynamic_pointer_cast<IfcOffsetCurve2D>(ifc_curve);
	if (offset_curve_2d)
	{
		// TODO: implement
		return;
	}

	shared_ptr<IfcOffsetCurve3D> offset_curve_3d = dynamic_pointer_cast<IfcOffsetCurve3D>(ifc_curve);
	if (offset_curve_3d)
	{
		// TODO: implement
		return;
	}

	shared_ptr<IfcPcurve> pcurve = dynamic_pointer_cast<IfcPcurve>(ifc_curve);
	if (pcurve)
	{
		// TODO: implement
		return;
	}

	throw UnhandledRepresentationException(ifc_curve);
}

void CurveConverter::getTrimAngle(const std::vector<shared_ptr<IfcTrimmingSelect> >& trimSelect1, const vec3& circleCenter, double radius1, double radius2,
	double& trimAngle1, const carve::math::Matrix& circlePlacement, const carve::math::Matrix& circlePlacementInverse) const
{
	trimAngle1 = 0;
	double lengthFactor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();
	double eps = m_geom_settings->getEpsilonMergePoints();

	// check for trimming begin
	shared_ptr<IfcParameterValue> trim_par1;
	if (trimSelect1.size() > 0)
	{
		if (GeomUtils::findFirstInVector(trimSelect1, trim_par1))
		{
			double planeAngleFactor = m_point_converter->getUnitConverter()->getAngleInRadiantFactor();
			if (m_point_converter->getUnitConverter()->getAngularUnit() == UnitConverter::UNDEFINED)
			{
				// angular unit definition not found in model, default to radian
				planeAngleFactor = 1.0;

				if (trim_par1->m_value > 2.0 * M_PI)
				{
					// assume degree
					planeAngleFactor = M_PI / 180.0;
				}
			}

			trimAngle1 = trim_par1->m_value * planeAngleFactor;
		}
		else
		{
			shared_ptr<IfcCartesianPoint> trimPoint1;
			if (GeomUtils::findFirstInVector(trimSelect1, trimPoint1))
			{
				vec3 trimPoint;
				PointConverter::convertIfcCartesianPoint(trimPoint1, trimPoint, lengthFactor);
				trimAngle1 = m_point_converter->getAngleOnConic(circleCenter, radius1, radius2, trimPoint, circlePlacement, circlePlacementInverse, eps);
			}
		}
	}
}

void CurveConverter::getTrimPoints(const std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, const std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec,
	const carve::math::Matrix& circlePosition, double circleRadius, double circleRadius2, bool senseAgreement, std::optional<vec3>& trimPoint1, 
	std::optional<vec3>& trimPoint2) const
{
	double lengthFactor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

	// check for trimming begin
	shared_ptr<IfcCartesianPoint> ifc_trimPoint1;
	if (GeomUtils::findFirstInVector(trim1_vec, ifc_trimPoint1))
	{
		vec3 trimPoint;
		PointConverter::convertIfcCartesianPoint(ifc_trimPoint1, trimPoint, lengthFactor);
		trimPoint = circlePosition * trimPoint;
		trimPoint1 = std::make_optional<vec3>(trimPoint);
	}
	else
	{
		shared_ptr<IfcParameterValue> trim_par1;
		if (GeomUtils::findFirstInVector(trim1_vec, trim_par1))
		{
			double planeAngleFactor = m_point_converter->getUnitConverter()->getAngleInRadiantFactor();
			if (m_point_converter->getUnitConverter()->getAngularUnit() == UnitConverter::UNDEFINED)
			{
				// angular unit definition not found in model, default to radian
				planeAngleFactor = 1.0;

				if (trim_par1->m_value > 2.0 * M_PI)
				{
					// assume degree
					planeAngleFactor = M_PI / 180.0;
				}
			}

			vec3 trimPoint;
			double trimAngle1 = trim_par1->m_value * planeAngleFactor;
			trimPoint = circlePosition * carve::geom::VECTOR(circleRadius * cos(trimAngle1), circleRadius * sin(trimAngle1), 0);
			if (circleRadius2 > 0)
			{
				trimPoint = carve::geom::VECTOR(circleRadius * cos(trimAngle1), circleRadius2 * sin(trimAngle1), 0);
			}
			trimPoint1 = std::make_optional<vec3>(trimPoint);
		}
	}

	// check for trimming end
	shared_ptr<IfcCartesianPoint> ifc_trimPoint2;
	if (GeomUtils::findFirstInVector(trim2_vec, ifc_trimPoint2))
	{
		vec3 trimPoint;
		PointConverter::convertIfcCartesianPoint(ifc_trimPoint2, trimPoint, lengthFactor);
		trimPoint = circlePosition * trimPoint;
		trimPoint2 = std::make_optional<vec3>(trimPoint);
	}
	else
	{
		shared_ptr<IfcParameterValue> trim_par;
		if (GeomUtils::findFirstInVector(trim2_vec, trim_par))
		{
			double planeAngleFactor = m_point_converter->getUnitConverter()->getAngleInRadiantFactor();
			if (m_point_converter->getUnitConverter()->getAngularUnit() == UnitConverter::UNDEFINED)
			{
				// angular unit definition not found in model, default to radian
				planeAngleFactor = 1.0;

				if (trim_par->m_value > 2.0 * M_PI)
				{
					// assume degree
					planeAngleFactor = M_PI / 180.0;
				}
			}

			double trimAngle = trim_par->m_value * planeAngleFactor;
			vec3 trimPoint = circlePosition * carve::geom::VECTOR(circleRadius * cos(trimAngle), circleRadius * sin(trimAngle), 0);
			if (circleRadius2 > 0)
			{
				trimPoint = carve::geom::VECTOR(circleRadius * cos(trimAngle), circleRadius2 * sin(trimAngle), 0);
			}
			trimPoint2 = std::make_optional<vec3>(trimPoint);
		}
	}
}

void CurveConverter::getTrimPoints(const std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, const std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, std::optional<vec3>& trimPoint1, std::optional<vec3>& trimPoint2) const
{
	double lengthFactor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

	// check for trimming begin
	shared_ptr<IfcCartesianPoint> ifc_trimPoint1;
	if (GeomUtils::findFirstInVector(trim1_vec, ifc_trimPoint1))
	{
		vec3 trimPoint;
		PointConverter::convertIfcCartesianPoint(ifc_trimPoint1, trimPoint, lengthFactor);
		trimPoint1 = std::make_optional<vec3>(trimPoint);
	}

	// check for trimming end
	shared_ptr<IfcCartesianPoint> ifc_trimPoint2;
	if (GeomUtils::findFirstInVector(trim2_vec, ifc_trimPoint2))
	{
		vec3 trimPoint;
		PointConverter::convertIfcCartesianPoint(ifc_trimPoint2, trimPoint, lengthFactor);
		trimPoint2 = std::make_optional<vec3>(trimPoint);
	}
}

void CurveConverter::convertIfcEdge(const shared_ptr<IfcEdge>& edge, std::vector<vec3>& resultPoints, double lengthFactor) const
{
	if (!edge)
	{
		return;
	}

	//ENTITY IfcEdge SUPERTYPE OF	(ONEOF(IfcOrientedEdge, IfcEdgeCurve, IfcSubedge))

	const shared_ptr<IfcVertex>& edgeStart = edge->m_EdgeStart;
	const shared_ptr<IfcVertex>& edgeEnd = edge->m_EdgeEnd;

	vec3 p0, p1;
	bool hasEdgeStart = PointConverter::convertIfcVertex(edgeStart, p0, lengthFactor);
	bool hasEdgeEnd = PointConverter::convertIfcVertex(edgeEnd, p1, lengthFactor);

	const shared_ptr<IfcOrientedEdge> orientedEdge = dynamic_pointer_cast<IfcOrientedEdge>(edge);
	if (orientedEdge)
	{
		//shared_ptr<IfcEdge>										m_EdgeElement;
		//shared_ptr<IfcBoolean>									m_Orientation;
		bool orientedEdgeOrientation = orientedEdge->m_Orientation->m_value;
		std::vector<vec3> loopPointsEdgeElement;
		convertIfcEdge(orientedEdge->m_EdgeElement, loopPointsEdgeElement, lengthFactor);
		if (!orientedEdgeOrientation)
		{
			std::reverse(loopPointsEdgeElement.begin(), loopPointsEdgeElement.end());
		}
		resultPoints = loopPointsEdgeElement;
		return;
	}

	const shared_ptr<IfcSubedge> subEdge = dynamic_pointer_cast<IfcSubedge>(edge);
	if (subEdge)
	{
		if (subEdge->m_ParentEdge)
		{
			std::vector<vec3> loopPointsParentEdge;
			convertIfcEdge(subEdge->m_ParentEdge, loopPointsParentEdge, lengthFactor);
			resultPoints = loopPointsParentEdge;
			return;
		}
	}

	std::vector<vec3> curvePoints;
	const shared_ptr<IfcEdgeCurve> edgeCurve = dynamic_pointer_cast<IfcEdgeCurve>(edge);
	if (edgeCurve)
	{
		//shared_ptr<IfcCurve>					m_EdgeGeometry;
		//shared_ptr<IfcBoolean>				m_SameSense;
		bool edgeSameSense = true;
		if (edgeCurve->m_SameSense)
		{
			edgeSameSense = edgeCurve->m_SameSense->m_value;
		}

		if (!edgeSameSense)
		{
			std::swap(p0, p1);
		}

		std::vector<CurveSegment> curveSegments;
		const shared_ptr<IfcCurve> edgeCurveGeometry = edgeCurve->m_EdgeGeometry;
		bool senseAgreement = true;

		if (edgeCurveGeometry)
		{
			shared_ptr<IfcTrimmedCurve> trimmedCurve = dynamic_pointer_cast<IfcTrimmedCurve>(edgeCurveGeometry);
			if (trimmedCurve)
			{
				const shared_ptr<IfcCurve> basisCurve = trimmedCurve->m_BasisCurve;
				if (basisCurve)
				{
					carve::math::Matrix matrix;
					shared_ptr<IfcConic> conic = dynamic_pointer_cast<IfcConic>(basisCurve);
					if (conic)
					{
						// ENTITY IfcConic ABSTRACT SUPERTYPE OF(ONEOF(IfcCircle, IfcEllipse))
						shared_ptr<TransformData> conicPositionMatrix(new TransformData());
						shared_ptr<IfcPlacement> conic_placement = dynamic_pointer_cast<IfcPlacement>(conic->m_Position);
						if (conic_placement)
						{
							m_placement_converter->convertIfcPlacement(conic_placement, conicPositionMatrix, false);

							carve::math::Matrix circlePositionInverse;
							GeomUtils::computeInverse(conicPositionMatrix->m_matrix, circlePositionInverse);

							matrix = circlePositionInverse;
						}
					}
					vec3 p0inCirclePlacement = matrix * p0;
					vec3 p1inCirclePlacement = matrix * p1;

					// use IfcEdge.EdgeStart and IfcEdge.EdgeEnd as trimming points
					std::vector<shared_ptr<IfcTrimmingSelect> > curveTrim1vec;
					std::vector<shared_ptr<IfcTrimmingSelect> > curveTrim2vec;

					shared_ptr<IfcCartesianPoint> trim1(new IfcCartesianPoint());
					trim1->m_Coordinates[0] = p0inCirclePlacement.x;  // in convertIfcCurve, the trim point will be multiplied with lengthFactor
					trim1->m_Coordinates[1] = p0inCirclePlacement.y;
					trim1->m_Coordinates[2] = p0inCirclePlacement.z;
					//trim1->m_size = 3;
					curveTrim1vec.push_back(trim1);

					shared_ptr<IfcCartesianPoint> trim2(new IfcCartesianPoint());
					trim2->m_Coordinates[0] = p1inCirclePlacement.x;
					trim2->m_Coordinates[1] = p1inCirclePlacement.y;
					trim2->m_Coordinates[2] = p1inCirclePlacement.z;
					//trim2->m_size = 3;
					curveTrim2vec.push_back(trim2);
					convertIfcCurve(basisCurve, curveSegments, curveTrim1vec, curveTrim2vec, senseAgreement);
				}
			}
			else
			{
				std::vector<shared_ptr<IfcTrimmingSelect> > curveTrim1vec;
				PointConverter::convertPointToTrimmingSelect(edgeStart, lengthFactor, curveTrim1vec);
					
				std::vector<shared_ptr<IfcTrimmingSelect> > curveTrim2vec;
				PointConverter::convertPointToTrimmingSelect(edgeEnd, lengthFactor, curveTrim2vec);

				convertIfcCurve(edgeCurveGeometry, curveSegments, curveTrim1vec, curveTrim2vec, senseAgreement);
			}

			for (CurveSegment& seg : curveSegments)
			{
				std::copy(seg.m_points.begin(), seg.m_points.end(), std::back_inserter(curvePoints));
			}
		}
		else
		{
			curvePoints.push_back(p0);
			curvePoints.push_back(p1);
		}

		if (edgeSameSense)
		{
			std::copy(curvePoints.begin(), curvePoints.end(), std::back_inserter(resultPoints));
		}
		else
		{
			// edge is oriented reverse
			std::copy(curvePoints.rbegin(), curvePoints.rend(), std::back_inserter(resultPoints));
		}
		return;
	}

	std::cout << "IfcEdge: " << EntityFactory::getStringForClassID(edge->classID()) << " not implemented" << std::endl;
}

void CurveConverter::convertIfcLoop(const shared_ptr<IfcLoop>& loop, std::vector<vec3>& loopPoints) const
{
	double eps = m_geom_settings->getEpsilonMergePoints();
	const shared_ptr<IfcPolyLoop> poly_loop = dynamic_pointer_cast<IfcPolyLoop>(loop);
	if (poly_loop)
	{
		const std::vector<shared_ptr<IfcCartesianPoint> >& ifc_points = poly_loop->m_Polygon;
		m_point_converter->convertIfcCartesianPointVectorSkipDuplicates(ifc_points, loopPoints, eps);

		GeomUtils::unClosePolygon(loopPoints, eps );
		return;
	}

	shared_ptr<IfcEdgeLoop> edge_loop = dynamic_pointer_cast<IfcEdgeLoop>(loop);
	if (edge_loop)
	{
		const double lengthFactor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

		for (const shared_ptr<IfcOrientedEdge>& orientedEdge : edge_loop->m_EdgeList)
		{
			// IfcOrientedEdge ---------------------------------------
			//   shared_ptr<IfcEdge>					m_EdgeElement;
			//   shared_ptr<IfcBoolean>					m_Orientation;

			bool orientation = true;
			if (orientedEdge->m_Orientation)
			{
				orientation = orientedEdge->m_Orientation->m_value;
			}

			shared_ptr<IfcEdge> edge = orientedEdge->m_EdgeElement;
			std::vector<vec3> edgePoints;
			convertIfcEdge(edge, edgePoints, lengthFactor);

			if (orientation)
			{
				std::copy(edgePoints.begin(), edgePoints.end(), std::back_inserter(loopPoints));
			}
			else
			{
				std::copy(edgePoints.rbegin(), edgePoints.rend(), std::back_inserter(loopPoints));
			}
		}

		GeomUtils::removeDuplicates(loopPoints, eps);
		GeomUtils::closePolygon(loopPoints, eps);
		return;
	}

	std::cout << "IfcLoop: " << EntityFactory::getStringForClassID(loop->classID()) << " not implemented" << std::endl;
}

void CurveConverter::computeOpeningAngle(double startAngle, double endAngle, double eps, bool senseAgreement, double& openingAngle)
{
	openingAngle = endAngle - startAngle;

	if (senseAgreement)
	{
		if (startAngle > endAngle)
		{
			if (openingAngle < 0)
			{
				openingAngle += 2.0 * M_PI;
			}
			// circle passes 0 angle
			//if (std::abs(startAngle) > eps && endAngle >= 0 || std::abs(startAngle) < eps && endAngle <= 0)
			if (startAngle > eps && endAngle >= 0 || std::abs(startAngle) < eps && endAngle <= 0)
			{
				//openingAngle = endAngle - startAngle + 2.0 * M_PI;
			}
		}
	}
	else
	{
		if (endAngle > startAngle)
		{
			if (openingAngle > 0)
			{
				openingAngle -= 2.0 * M_PI;
			}

			if (std::abs(endAngle) > eps && startAngle >= 0 || std::abs(endAngle) < eps && startAngle <= 0)
			{
				// circle passes 0 angle
				//openingAngle = startAngle - endAngle - 2.0 * M_PI;
			}
		}
	}

	double diffToHalfCircle = openingAngle - M_PI;
	if (std::abs(diffToHalfCircle) < eps)
	{
		if (senseAgreement)
		{
			// in case of exact half circle, opening angle could be positive or negative without violating the trim points. Prefer positive openingAngle
			if (openingAngle < 0)
			{
				openingAngle = -openingAngle;
			}
		}
		else
		{
			if (openingAngle > 0)
			{
				openingAngle = -openingAngle;
			}
		}
	}

	while (openingAngle > 2.0 * M_PI)
	{
		openingAngle -= 2.0 * M_PI;
	}
	while (openingAngle < -2.0 * M_PI)
	{
		openingAngle += 2.0 * M_PI;
	}
}

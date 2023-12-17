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
	CurveConverter(shared_ptr<GeometrySettings>& gs, shared_ptr<PlacementConverter>& placement_converter, shared_ptr<PointConverter>& pc, shared_ptr<SplineConverter>& sc)
		: m_geom_settings(gs), m_placement_converter(placement_converter), m_point_converter(pc), m_spline_converter(sc)
	{
	}

	virtual ~CurveConverter() {}

	const shared_ptr<GeometrySettings>& getGeomSettings() { return m_geom_settings; }
	const shared_ptr<PlacementConverter>& getPlacementConverter() { return m_placement_converter; }
	const shared_ptr<PointConverter>& getPointConverter() { return m_point_converter; }
	const shared_ptr<SplineConverter>& getSplineConverter() { return m_spline_converter; }

	void convertIfcCurve2D(const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec2>& loops, std::vector<vec2>& segment_start_points, bool senseAgreement) const
	{
		std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
		std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
		convertIfcCurve2D(ifc_curve, loops, segment_start_points, trim1_vec, trim2_vec, senseAgreement);
	}

	void convertIfcCurve2D(const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec2>& target_vec, std::vector<vec2>& segment_start_points,
		std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool senseAgreement) const
	{
		std::vector<vec3> target_vec_3d;
		std::vector<vec3> segment_start_points_3d;
		convertIfcCurve(ifc_curve, target_vec_3d, segment_start_points_3d, senseAgreement);

		for (size_t i = 0; i < target_vec_3d.size(); ++i)
		{
			vec3& point_3d = target_vec_3d[i];
			target_vec.push_back(carve::geom::VECTOR(point_3d.x, point_3d.y));
		}
		for (size_t i = 0; i < segment_start_points_3d.size(); ++i)
		{
			vec3& point_3d = segment_start_points_3d[i];
			segment_start_points.push_back(carve::geom::VECTOR(point_3d.x, point_3d.y));
		}
	}

	void convertIfcCurve(const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec3>& loops, std::vector<vec3>& segment_start_points, bool senseAgreement) const
	{
		std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
		std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
		convertIfcCurve(ifc_curve, loops, segment_start_points, trim1_vec, trim2_vec, senseAgreement);
	}

	void convertIfcCurve(const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec3>& target_vec, std::vector<vec3>& segment_start_points,
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
				//std::vector<shared_ptr<IfcSegment> >					m_Segments;
				std::vector<shared_ptr<IfcSegment> >& vec_segements = composite_curve->m_Segments;
				for (size_t i_segments = 0; i_segments < vec_segements.size(); ++i_segments)
				{
					shared_ptr<IfcSegment> segement = vec_segements[i_segments];

					// ENTITY IfcSegment ABSTRACT SUPERTYPE OF (ONEOF (IfcCompositeCurveSegment ,IfcCurveSegment))
					shared_ptr<IfcCompositeCurveSegment> compositeCurveSegment = dynamic_pointer_cast<IfcCompositeCurveSegment>(segement);
					if (compositeCurveSegment)
					{
						shared_ptr<IfcCurve> segement_curve = compositeCurveSegment->m_ParentCurve;

						std::vector<vec3> segment_vec;
						convertIfcCurve(segement_curve, segment_vec, segment_start_points, senseAgreement);
						if (segment_vec.size() > 0)
						{
							GeomUtils::appendPointsToCurve(segment_vec, target_vec, epsilonMergePoints);
						}
						continue;
					}

					shared_ptr<IfcCurveSegment> curveSegment = dynamic_pointer_cast<IfcCurveSegment>(segement);
					if (curveSegment)
					{
						//shared_ptr<IfcPlacement>								m_Placement;
						//shared_ptr<IfcCurveMeasureSelect>						m_SegmentStart;
						//shared_ptr<IfcCurveMeasureSelect>						m_SegmentLength;
						//shared_ptr<IfcCurve>									m_ParentCurve;

						shared_ptr<IfcCurve> segement_curve = curveSegment->m_ParentCurve;

						std::vector<vec3> segment_vec;
						convertIfcCurve(segement_curve, segment_vec, segment_start_points, senseAgreement);
						if (segment_vec.size() > 0)
						{
							GeomUtils::appendPointsToCurve(segment_vec, target_vec, epsilonMergePoints);
						}
						continue;
					}
				}
				return;
			}

			shared_ptr<IfcPolyline> poly_line = dynamic_pointer_cast<IfcPolyline>(ifc_curve);
			if (poly_line)
			{
				std::vector<shared_ptr<IfcCartesianPoint> >& points = poly_line->m_Points;
				if (points.size() > 0)
				{
					m_point_converter->convertIfcCartesianPointVector(points, target_vec);
					shared_ptr<IfcCartesianPoint>& first_point = points[0];
					segment_start_points.push_back(carve::geom::VECTOR(first_point->m_Coordinates[0] * lengthFactor, first_point->m_Coordinates[1] * lengthFactor, 0));
				}
				return;
			}

			shared_ptr<IfcTrimmedCurve> trimmed_curve = dynamic_pointer_cast<IfcTrimmedCurve>(bounded_curve);
			if (trimmed_curve)
			{
				shared_ptr<IfcCurve> basis_curve = trimmed_curve->m_BasisCurve;
				if (basis_curve)
				{
					std::vector<vec3> basis_curve_points;
					std::vector<vec3> basis_curve_start_points;
					std::vector<shared_ptr<IfcTrimmingSelect> >& curve_trim1_vec = trimmed_curve->m_Trim1;
					std::vector<shared_ptr<IfcTrimmingSelect> >& curve_trim2_vec = trimmed_curve->m_Trim2;
					bool trimmed_senseAgreement = true;
					if (trimmed_curve->m_SenseAgreement) { trimmed_senseAgreement = trimmed_curve->m_SenseAgreement->m_value; }
					if (!senseAgreement)
					{
						trimmed_senseAgreement = !trimmed_senseAgreement;
					}

					convertIfcCurve(basis_curve, basis_curve_points, basis_curve_start_points, curve_trim1_vec, curve_trim2_vec, trimmed_senseAgreement);

					GeomUtils::appendPointsToCurve(basis_curve_points, target_vec, epsilonMergePoints);
					GeomUtils::appendPointsToCurve(basis_curve_start_points, segment_start_points, epsilonMergePoints);
				}
				return;
			}

			shared_ptr<IfcBSplineCurve> bspline_curve = dynamic_pointer_cast<IfcBSplineCurve>(bounded_curve);
			if (bspline_curve)
			{
				m_spline_converter->convertBSplineCurve(bspline_curve, target_vec, segment_start_points);

				std::optional<vec3> trimPoint1;
				std::optional<vec3> trimPoint2;
				getTrimPoints(trim1_vec, trim2_vec, trimPoint1, trimPoint2);

#ifdef _DEBUG
				//GeomDebugDump::clearMeshsetDump();
				//glm::vec4 color(0.2, 0.2, 0.2, 0.8);
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
									GeomUtils::appendPointsToCurve(line_points_3d, target_vec, epsilonMergePoints);
									segment_start_points.push_back(line_points_3d[0]);
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
											std::vector<vec3> circle_points_3d;
											for (size_t kk = 0; kk < n; ++kk)
											{
												carve::math::Matrix m = carve::math::Matrix::ROT(-angle, circAxis, epsilonMergePoints);

												vec3 p_rotated = center_p0;
												p_rotated = m * p_rotated + circ_center;

												circle_points_3d.push_back(p_rotated);
												angle += deltaAngle;
											}
											GeomUtils::appendPointsToCurve(circle_points_3d, target_vec, epsilonMergePoints);
											segment_start_points.push_back(circle_points_3d[0]);
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
						const vec3& pt0 = pointVec[0];
						GeomUtils::appendPointsToCurve(pointVec, target_vec, epsilonMergePoints);
						segment_start_points.push_back(pt0);
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
			shared_ptr<TransformData> conic_position_matrix(new TransformData());
			shared_ptr<IfcPlacement> conic_placement = dynamic_pointer_cast<IfcPlacement>(conic->m_Position);
			if (conic_placement)
			{
				m_placement_converter->convertIfcPlacement(conic_placement, conic_position_matrix, false);
			}

			vec3 circle_center = conic_position_matrix->m_matrix * carve::geom::VECTOR(0, 0, 0);

			double circle_radius = -1;
			double circle_radius2 = -1;
			shared_ptr<IfcEllipse> ellipse = dynamic_pointer_cast<IfcEllipse>(conic);
			if (ellipse)
			{
				if (ellipse->m_SemiAxis1)
				{
					if (ellipse->m_SemiAxis2)
					{
						circle_radius = ellipse->m_SemiAxis1->m_value * lengthFactor;
						circle_radius2 = ellipse->m_SemiAxis2->m_value * lengthFactor;

					}
				}
			}

			shared_ptr<IfcCircle> circle = dynamic_pointer_cast<IfcCircle>(conic);
			if (circle)
			{
				circle_radius = 0.0;
				if (circle->m_Radius)
				{
					circle_radius = circle->m_Radius->m_value * lengthFactor;
				}
			}

			carve::math::Matrix circlePositionInverse;
			GeomUtils::computeInverse(conic_position_matrix->m_matrix, circlePositionInverse);

			double maxRadius = std::max(circle_radius, circle_radius2);
			double startAngle = 0;
			double openingAngle = M_PI * 2.0;
			getTrimAngles(trim1_vec, trim2_vec, circle_center, maxRadius, senseAgreement, startAngle, openingAngle, conic_position_matrix->m_matrix, circlePositionInverse);

			std::optional<vec3> trimPoint1;
			std::optional<vec3> trimPoint2;
			getTrimPoints(trim1_vec, trim2_vec, conic_position_matrix->m_matrix, circle_radius, circle_radius2, senseAgreement, trimPoint1, trimPoint2);

#ifdef _DEBUG
			if (m_debugDumpGeometry && trimPoint1.has_value())
			{
				glm::vec4 color(0.4, 0.6, 0.6, 1.0);
				vec3 circleCenter = conic_position_matrix->m_matrix * carve::geom::VECTOR(0, 0, 0);
				std::vector<vec3> polyline = { trimPoint1.value() , circleCenter};
				GeomDebugDump::dumpPolyline(polyline, color, 3.0, false, false);
				GeomDebugDump::dumpCoordinateSystem(conic_position_matrix->m_matrix, circle_radius, false);

				vec3 circlePoint0 = conic_position_matrix->m_matrix * carve::geom::VECTOR(circle_radius * cos(startAngle), circle_radius * sin(startAngle), 0);
				vec3 circlePoint1 = conic_position_matrix->m_matrix * carve::geom::VECTOR(circle_radius * cos(startAngle + openingAngle*0.1), circle_radius * sin(startAngle + openingAngle*0.1), 0);
				glm::vec4 color3(0.3, 0.4, 0.94, 0.6);
				polyline = { circlePoint0 , circlePoint1 };
				GeomDebugDump::dumpPolyline(polyline, color3, 2.0, false, false);
			}
#endif

			if (trimPoint1.has_value())
			{
				double distanceTrim1 = PointConverter::trimPointCircleDistance(startAngle, circle_radius, conic_position_matrix->m_matrix, trimPoint1.value());
				if (distanceTrim1 > EPS_M6 )
				{
					startAngle = m_point_converter->getAngleOnCircle(circle_center, circle_radius, trimPoint1.value(), conic_position_matrix->m_matrix, circlePositionInverse, epsilonMergePoints);

#ifdef _DEBUG
					double distanceTrim12 = PointConverter::trimPointCircleDistance(startAngle, circle_radius, conic_position_matrix->m_matrix, trimPoint2.value());
					if (distanceTrim12 > EPS_M6)
					{
						std::cout << "distanceTrim1: " << distanceTrim12 << std::endl;
					}
#endif
				}
			}
			
			if (trimPoint2.has_value())
			{
				double endAngle = startAngle + openingAngle;
				double distanceTrim2 = PointConverter::trimPointCircleDistance(endAngle, circle_radius, conic_position_matrix->m_matrix, trimPoint2.value());
				if (distanceTrim2 > EPS_M6)
				{
					endAngle = m_point_converter->getAngleOnCircle(circle_center, circle_radius, trimPoint2.value(), conic_position_matrix->m_matrix, circlePositionInverse, epsilonMergePoints);

					computeOpeningAngle(startAngle, endAngle, epsilonMergePoints, senseAgreement, openingAngle);


#ifdef _DEBUG
					if (!senseAgreement)
					{
						openingAngle = -openingAngle;
					}
					double endAngle2 = startAngle + openingAngle;
					double distanceTrim22 = PointConverter::trimPointCircleDistance(endAngle2, circle_radius, conic_position_matrix->m_matrix, trimPoint2.value());
					if (distanceTrim22 > EPS_M5)
					{
						std::cout << "distanceTrim2: " << distanceTrim22 << std::endl;
					}

#endif
				}
			}


			int num_segments = m_geom_settings->getNumVerticesPerCircleWithRadius(circle_radius) * (std::abs(openingAngle) / (2.0 * M_PI));
			if (num_segments < m_geom_settings->getMinNumVerticesPerArc()) num_segments = m_geom_settings->getMinNumVerticesPerArc();
			const double circle_center_x = 0.0;
			const double circle_center_y = 0.0;
			std::vector<vec3> circle_segment_points3D;

			if (circle_radius > EPS_M8)
			{
				double angle = startAngle;
				double angle_delta = openingAngle / (double)(num_segments - 1);

				for (int i = 0; i < num_segments; ++i)
				{
					vec3 circlePoint = carve::geom::VECTOR(circle_radius * cos(angle), circle_radius * sin(angle), 0);
					if (circle_radius2 > 0)
					{
						circlePoint = carve::geom::VECTOR(circle_radius * cos(angle), circle_radius2 * sin(angle), 0);
					}

					// apply position
					circlePoint = conic_position_matrix->m_matrix * circlePoint;

					circle_segment_points3D.push_back(circlePoint);
					angle += angle_delta;
				}
			}
			else
			{
				circle_segment_points3D.push_back(carve::geom::VECTOR(circle_center_x, circle_center_y, 0));
			}

			GeomUtils::appendPointsToCurve(circle_segment_points3D, target_vec, epsilonMergePoints);
			segment_start_points.push_back(circle_segment_points3D[0]);

#ifdef _DEBUG
			//glm::vec4 color(0.2, 0.2, 0.2, 0.8);
			//GeomDebugDump::dumpPolyline(circle_segment_points3D, color, 0, true);
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

				std::vector<shared_ptr<IfcReal> >& direction_ratios = ifc_line_direction->m_DirectionRatios;
				vec3 line_direction;
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
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if (GeomUtils::findFirstInVector(trim1_vec, ifc_trim_point))
					{
						vec3 trim_point;
						PointConverter::convertIfcCartesianPoint(ifc_trim_point, trim_point, lengthFactor);
						line_origin = trim_point;

						vec3 closest_point_on_line;
						GeomUtils::closestPointOnLine(trim_point, line_origin, line_direction, closest_point_on_line);

						if ((closest_point_on_line - trim_point).length() < epsilonMergePoints *10.0)
						{
							// trimming point is on the line
							line_origin = trim_point;
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
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if (GeomUtils::findFirstInVector(trim2_vec, ifc_trim_point))
					{
						vec3 trim_point;
						PointConverter::convertIfcCartesianPoint(ifc_trim_point, trim_point, lengthFactor);
						line_end = trim_point;

						vec3 closest_point_on_line;
						GeomUtils::closestPointOnLine(trim_point, line_origin, line_direction, closest_point_on_line);

						if ((closest_point_on_line - trim_point).length() < 0.0001)
						{
							// trimming point is on the line
							line_end = trim_point;
						}
						else
						{
							line_end = closest_point_on_line;
						}
					}
				}

				std::vector<vec3> points_vec;
				points_vec.push_back(line_origin);
				points_vec.push_back(line_end);

				GeomUtils::appendPointsToCurve(points_vec, target_vec, epsilonMergePoints);
				segment_start_points.push_back(line_origin);
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

	void getTrimAngles( const std::vector<shared_ptr<IfcTrimmingSelect> >& trimSelect1,  const std::vector<shared_ptr<IfcTrimmingSelect> >& trimSelect2, 
		const vec3& circle_center, double radius,  bool senseAgreement, double& startAngle, double& openingAngle, 
		const carve::math::Matrix& circlePlacement, const carve::math::Matrix& circlePlacementInverse) const
	{
		double trimAngle1 = 0.0;
		double trimAngle2 = M_PI * 2.0;
		startAngle = 0;
		openingAngle = 0;
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

					if (trim_par1->m_value > M_PI)
					{
						// assume degree
						planeAngleFactor = M_PI / 180.0;
					}
				}

				trimAngle1 = trim_par1->m_value*planeAngleFactor;
			}
			else
			{
				shared_ptr<IfcCartesianPoint> trim_point1;
				if (GeomUtils::findFirstInVector(trimSelect1, trim_point1))
				{
					vec3 trim_point;
					PointConverter::convertIfcCartesianPoint(trim_point1, trim_point, lengthFactor);
					trimAngle1 = m_point_converter->getAngleOnCircle(circle_center, radius, trim_point, circlePlacement, circlePlacementInverse, eps);
				}
			}
		}

		if (trimSelect2.size() > 0)
		{
			// check for trimming end
			shared_ptr<IfcParameterValue> trim_par2;
			if (GeomUtils::findFirstInVector(trimSelect2, trim_par2))
			{
				double planeAngleFactor = m_point_converter->getUnitConverter()->getAngleInRadiantFactor();
				if (m_point_converter->getUnitConverter()->getAngularUnit() == UnitConverter::UNDEFINED)
				{
					// angular unit definition not found in model, default to radian
					planeAngleFactor = 1.0;

					if (trim_par2->m_value > M_PI)
					{
						// assume degree
						planeAngleFactor = M_PI / 180.0;
					}
				}
				trimAngle2 = trim_par2->m_value*planeAngleFactor;
			}
			else
			{
				shared_ptr<IfcCartesianPoint> ifc_trim_point;
				if (GeomUtils::findFirstInVector(trimSelect2, ifc_trim_point))
				{
					vec3 trim_point;
					PointConverter::convertIfcCartesianPoint(ifc_trim_point, trim_point, lengthFactor);
					trimAngle2 = m_point_converter->getAngleOnCircle(circle_center, radius, trim_point, circlePlacement, circlePlacementInverse, eps);
				}
			}
		}

		computeOpeningAngle(trimAngle1, trimAngle2, eps, senseAgreement, openingAngle);
	}

	void getTrimPoints(const std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, const std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, const carve::math::Matrix& circlePosition,
		double circleRadius, double circleRadius2, bool senseAgreement, std::optional<vec3>& trimPoint1, std::optional<vec3>& trimPoint2) const
	{
		double lengthFactor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

		// check for trimming begin
		shared_ptr<IfcCartesianPoint> ifc_trim_point1;
		if (GeomUtils::findFirstInVector(trim1_vec, ifc_trim_point1))
		{
			vec3 trimPoint;
			PointConverter::convertIfcCartesianPoint(ifc_trim_point1, trimPoint, lengthFactor);
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

					if (trim_par1->m_value > M_PI)
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
		shared_ptr<IfcCartesianPoint> ifc_trim_point2;
		if (GeomUtils::findFirstInVector(trim2_vec, ifc_trim_point2))
		{
			vec3 trimPoint;
			PointConverter::convertIfcCartesianPoint(ifc_trim_point2, trimPoint, lengthFactor);
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

					if (trim_par->m_value > M_PI)
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

	void getTrimPoints(const std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, const std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, std::optional<vec3>& trimPoint1, std::optional<vec3>& trimPoint2) const
	{
		double lengthFactor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

		// check for trimming begin
		shared_ptr<IfcCartesianPoint> ifc_trim_point1;
		if (GeomUtils::findFirstInVector(trim1_vec, ifc_trim_point1))
		{
			vec3 trimPoint;
			PointConverter::convertIfcCartesianPoint(ifc_trim_point1, trimPoint, lengthFactor);
			trimPoint1 = std::make_optional<vec3>(trimPoint);
		}

		// check for trimming end
		shared_ptr<IfcCartesianPoint> ifc_trim_point2;
		if (GeomUtils::findFirstInVector(trim2_vec, ifc_trim_point2))
		{
			vec3 trimPoint;
			PointConverter::convertIfcCartesianPoint(ifc_trim_point2, trimPoint, lengthFactor);
			trimPoint2 = std::make_optional<vec3>(trimPoint);
		}
	}

	void convertIfcPolyline(const shared_ptr<IfcPolyline>& poly_line, std::vector<vec3>& loop) const
	{
		m_point_converter->convertIfcCartesianPointVector(poly_line->m_Points, loop);
	}

	void convertIfcEdge(const shared_ptr<IfcEdge>& edge, std::vector<vec3>& loopPoints, double length_factor) const
	{
		if (!edge)
		{
			return;
		}

		//ENTITY IfcEdge SUPERTYPE OF	(ONEOF(IfcOrientedEdge, IfcEdgeCurve, IfcSubedge))

		const shared_ptr<IfcVertex>& edgeStart = edge->m_EdgeStart;
		const shared_ptr<IfcVertex>& edgeEnd = edge->m_EdgeEnd;

		vec3 p0, p1;
		bool hasEdgeStart = PointConverter::convertIfcVertex(edgeStart, p0, length_factor);
		bool hasEdgeEnd = PointConverter::convertIfcVertex(edgeEnd, p1, length_factor);

		const shared_ptr<IfcOrientedEdge> orientedEdge = dynamic_pointer_cast<IfcOrientedEdge>(edge);
		if (orientedEdge)
		{
			//shared_ptr<IfcEdge>										m_EdgeElement;
			//shared_ptr<IfcBoolean>									m_Orientation;
			bool orientedEdgeOrientation = orientedEdge->m_Orientation->m_value;
			std::vector<vec3> loopPointsEdgeElement;
			convertIfcEdge(orientedEdge->m_EdgeElement, loopPointsEdgeElement, length_factor);
			if (!orientedEdgeOrientation)
			{
				std::reverse(loopPointsEdgeElement.begin(), loopPointsEdgeElement.end());
			}
			loopPoints = loopPointsEdgeElement;
			return;
		}

		const shared_ptr<IfcSubedge> subEdge = dynamic_pointer_cast<IfcSubedge>(edge);
		if (subEdge)
		{
			if (subEdge->m_ParentEdge)
			{
				std::vector<vec3> loopPointsParentEdge;
				convertIfcEdge(subEdge->m_ParentEdge, loopPointsParentEdge, length_factor);
				loopPoints = loopPointsParentEdge;
				return;
			}
		}

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

			std::vector<vec3> curvePoints;
			std::vector<vec3> segmentStartPoints;
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
						// use IfcEdge.EdgeStart and IfcEdge.EdgeEnd as trimming points
						std::vector<shared_ptr<IfcTrimmingSelect> > curve_trim1_vec;
						std::vector<shared_ptr<IfcTrimmingSelect> > curve_trim2_vec;

						shared_ptr<IfcCartesianPoint> trim1(new IfcCartesianPoint());
						trim1->m_Coordinates[0] = p0.x / length_factor;  // in convertIfcCurve, the trim point will be multiplied with length_factor
						trim1->m_Coordinates[1] = p0.y / length_factor;
						trim1->m_Coordinates[2] = p0.z / length_factor;
						curve_trim1_vec.push_back(trim1);

						shared_ptr<IfcCartesianPoint> trim2(new IfcCartesianPoint());
						trim2->m_Coordinates[0] = p1.x / length_factor;
						trim2->m_Coordinates[1] = p1.y / length_factor;
						trim2->m_Coordinates[2] = p1.z / length_factor;
						curve_trim2_vec.push_back(trim2);
						convertIfcCurve(basisCurve, curvePoints, segmentStartPoints, curve_trim1_vec, curve_trim2_vec, senseAgreement);
					}
				}
				else
				{
					std::vector<shared_ptr<IfcTrimmingSelect> > curve_trim1_vec;
					PointConverter::convertPointToTrimmingSelect(edgeStart, length_factor, curve_trim1_vec);
					
					std::vector<shared_ptr<IfcTrimmingSelect> > curve_trim2_vec;
					PointConverter::convertPointToTrimmingSelect(edgeEnd, length_factor, curve_trim2_vec);

					convertIfcCurve(edgeCurveGeometry, curvePoints, segmentStartPoints, curve_trim1_vec, curve_trim2_vec, senseAgreement);
				}
			}
			else
			{
				curvePoints.push_back(p0);
				curvePoints.push_back(p1);
			}

			if (edgeSameSense)
			{
				std::copy(curvePoints.begin(), curvePoints.end(), std::back_inserter(loopPoints));
			}
			else
			{
				// edge is oriented reverse
				std::copy(curvePoints.rbegin(), curvePoints.rend(), std::back_inserter(loopPoints));
			}

			return;
		}

		std::cout << "IfcEdge: " << EntityFactory::getStringForClassID(edge->classID()) << " not implemented" << std::endl;
	}


	void convertIfcLoop(const shared_ptr<IfcLoop>& loop, std::vector<vec3>& loop_points) const
	{
		double eps = m_geom_settings->getEpsilonMergePoints();
		const shared_ptr<IfcPolyLoop> poly_loop = dynamic_pointer_cast<IfcPolyLoop>(loop);
		if (poly_loop)
		{
			const std::vector<shared_ptr<IfcCartesianPoint> >& ifc_points = poly_loop->m_Polygon;
			m_point_converter->convertIfcCartesianPointVectorSkipDuplicates(ifc_points, loop_points, eps);

			GeomUtils::unClosePolygon(loop_points, eps );
			return;
		}

		shared_ptr<IfcEdgeLoop> edge_loop = dynamic_pointer_cast<IfcEdgeLoop>(loop);
		if (edge_loop)
		{
			const double length_factor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

			for (const shared_ptr<IfcOrientedEdge>& oriented_edge : edge_loop->m_EdgeList)
			{
				// IfcOrientedEdge ---------------------------------------
				//   shared_ptr<IfcEdge>					m_EdgeElement;
				//   shared_ptr<IfcBoolean>					m_Orientation;

				bool orientation = true;
				if (oriented_edge->m_Orientation)
				{
					orientation = oriented_edge->m_Orientation->m_value;
				}

				shared_ptr<IfcEdge> edge = oriented_edge->m_EdgeElement;
				std::vector<vec3> edge_points;
				convertIfcEdge(edge, edge_points, length_factor);

				if (orientation)
				{
					std::copy(edge_points.begin(), edge_points.end(), std::back_inserter(loop_points));
				}
				else
				{
					std::copy(edge_points.rbegin(), edge_points.rend(), std::back_inserter(loop_points));
				}
			}

			GeomUtils::removeDuplicates(loop_points, eps);
			GeomUtils::closePolygon(loop_points, eps);
			return;
		}

		std::cout << "IfcLoop: " << EntityFactory::getStringForClassID(loop->classID()) << " not implemented" << std::endl;
	}

	static void computeOpeningAngle(double startAngle, double endAngle, double eps, bool senseAgreement, double& openingAngle)
	{
		openingAngle = endAngle - startAngle;

		if (senseAgreement)
		{
			
			if (startAngle > endAngle)
			{
				// circle passes 0 angle
				if (std::abs(startAngle) > eps && endAngle >= 0 || std::abs(startAngle) < eps && endAngle <= 0)
				{
					openingAngle = endAngle - startAngle + 2.0 * M_PI;
				}
			}
		}
		else
		{
			//openingAngle = startAngle - endAngle;
			if (endAngle < startAngle)
			{
				if (std::abs(endAngle) > eps && startAngle >= 0 || std::abs(endAngle) < eps && startAngle <= 0)
				{
					// circle passes 0 angle
					openingAngle = startAngle - endAngle - 2.0 * M_PI;
				}
			}
		}

		double diffToHalfCircle = openingAngle - M_PI;
		if (std::abs(diffToHalfCircle) < eps)
		{
			// in case of exact half circle, opening angle could be positive or negative without violating the trim points. Prefer positive openingAngle
			if (openingAngle < 0)
			{
				openingAngle = -openingAngle;
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
};

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

#include <ifcpp/model/shared_ptr.h>
#include "IncludeCarveHeaders.h"

class UnitConverter;
class GeometrySettings;
class PointConverter;
class SplineConverter;
class IfcPolyline;
class IfcLoop;
class IfcCurve;
class IfcCartesianPoint;
class IfcTrimmingSelect;

//\brief class to convert different kinds of IFC curve representations into carve input geometry

class CurveConverter
{
public:
	CurveConverter( shared_ptr<GeometrySettings>& geom_settings, shared_ptr<UnitConverter>& unit_converter, shared_ptr<PointConverter>& pc, shared_ptr<SplineConverter>& sc );
	~CurveConverter();

	void convertIfcCurve( const shared_ptr<IfcCurve>& ifc_curve, std::vector<carve::geom::vector<3> >& loops, std::vector<carve::geom::vector<3> >& segment_start_points ) const;
	void convertIfcCurve( const shared_ptr<IfcCurve>& ifc_curve, std::vector<carve::geom::vector<3> >& loops, std::vector<carve::geom::vector<3> >& segment_start_points,
		std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool sense_agreement ) const;

	void convertIfcCurve2D( const shared_ptr<IfcCurve>& ifc_curve, std::vector<carve::geom::vector<2> >& loops, std::vector<carve::geom::vector<2> >& segment_start_points ) const;
	void convertIfcCurve2D( const shared_ptr<IfcCurve>& ifc_curve, std::vector<carve::geom::vector<2> >& loops, std::vector<carve::geom::vector<2> >& segment_start_points,
		std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool sense_agreement ) const;

	void convertIfcPolyline( const shared_ptr<IfcPolyline>& poly_line,	std::vector<carve::geom::vector<3> >& loops ) const;
	void convertIfcLoop( const shared_ptr<IfcLoop>& loop,	std::vector<carve::geom::vector<3> >& loop_points ) const;

	shared_ptr<GeometrySettings>	m_geom_settings;
	shared_ptr<UnitConverter>		m_unit_converter;
	shared_ptr<SplineConverter>		m_spline_converter;
	shared_ptr<PointConverter>		m_point_converter;
};

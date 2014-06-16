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
class IfcPolyline;
class IfcLoop;
class IfcCurve;
class IfcCartesianPoint;
class IfcTrimmingSelect;

//\brief class to convert IFC point representations into carve input geometry

class PointConverter
{
public:
	PointConverter( shared_ptr<GeometrySettings>& geom_settings, shared_ptr<UnitConverter>& unit_converter );
	~PointConverter();

	void convertIfcCartesianPoint(			const shared_ptr<IfcCartesianPoint>& ifc_point,				carve::geom::vector<3>& point ) const;
	static void convertIfcCartesianPoint(	const shared_ptr<IfcCartesianPoint>& ifc_point,				carve::geom::vector<3>& point, double length_factor );
	void convertIfcCartesianPointVector(	const std::vector<shared_ptr<IfcCartesianPoint> >& points,	std::vector<carve::geom::vector<3> >& vertices ) const;
	void convertIfcCartesianPointVectorSkipDuplicates( const std::vector<shared_ptr<IfcCartesianPoint> >& ifc_points, std::vector<carve::geom::vector<3> >& loop ) const;
	void convertIfcCartesianPointVector2D( std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& points, std::vector<carve::geom::vector<3> >& vertices );
	static double getAngleOnCircle( const carve::geom::vector<3>& circle_center, double circle_radius, const carve::geom::vector<3>& trim_point );
	
	shared_ptr<GeometrySettings>	m_geom_settings;
	shared_ptr<UnitConverter>		m_unit_converter;
};

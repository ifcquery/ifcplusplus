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
#include <ifcpp/model/StatusCallback.h>
#include "IncludeCarveHeaders.h"

class PointConverter;
class IfcBSplineCurve;
class IfcBSplineSurface;

class SplineConverter : public StatusCallback
{
public:
	SplineConverter( shared_ptr<PointConverter>& point_converter );
	~SplineConverter();
	void convertBSplineCurve( const shared_ptr<IfcBSplineCurve>& ifc_curve, std::vector<carve::geom::vector<3> >& target_vec, std::vector<carve::geom::vector<3> >& segment_start_points ) const;
	void convertIfcBSplineSurface(	const shared_ptr<IfcBSplineSurface>& ifc_surface,	shared_ptr<carve::input::PolylineSetData>& polyline_data );

	shared_ptr<PointConverter> m_point_converter;
};

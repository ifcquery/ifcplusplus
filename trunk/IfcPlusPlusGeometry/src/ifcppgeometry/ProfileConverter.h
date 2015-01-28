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

#include "ifcpp/model/shared_ptr.h"
#include <ifcpp/model/StatusCallback.h>
#include "IncludeCarveHeaders.h"

class GeometrySettings;
class UnitConverter;
class PointConverter;
class CurveConverter;
class PlacementConverter;
class SplineConverter;
class IfcProfileDef;
class IfcArbitraryClosedProfileDef;
class IfcArbitraryOpenProfileDef;
class IfcCompositeProfileDef;
class IfcDerivedProfileDef;
class IfcParameterizedProfileDef;

typedef carve::geom::vector<2> vector2d_t;

class ProfileConverter : public StatusCallback
{
public:
	ProfileConverter( shared_ptr<CurveConverter>& cc, shared_ptr<SplineConverter>& sc );
	~ProfileConverter();

	void computeProfile( shared_ptr<IfcProfileDef> profile_def );
	void convertIfcArbitraryClosedProfileDef(				const shared_ptr<IfcArbitraryClosedProfileDef>& profile_def,	std::vector<std::vector<vector2d_t > >& paths );
	void convertIfcArbitraryOpenProfileDef(					const shared_ptr<IfcArbitraryOpenProfileDef>& profile_def,		std::vector<std::vector<vector2d_t > >& paths );
	void convertIfcCompositeProfileDef(						const shared_ptr<IfcCompositeProfileDef>& profile_def,			std::vector<std::vector<vector2d_t > >& paths );
	void convertIfcDerivedProfileDef(						const shared_ptr<IfcDerivedProfileDef>& profile_def,			std::vector<std::vector<vector2d_t > >& paths );
	void convertIfcParameterizedProfileDef(					const shared_ptr<IfcParameterizedProfileDef>& profile_def,		std::vector<std::vector<vector2d_t > >& paths );
	void convertIfcParameterizedProfileDefWithPosition(		const shared_ptr<IfcParameterizedProfileDef>& profile_def,		std::vector<std::vector<vector2d_t > >& paths );
	void addArc(				std::vector<vector2d_t >& coords, double radius, double start_angle, double opening_angle, double x_center, double y_center, int segments = -1 ) const;
	void addArcWithEndPoint(	std::vector<vector2d_t >& coords, double radius, double start_angle, double opening_angle, double x_center, double y_center ) const;
	void simplifyPaths();
	
	static void addArcWithEndPoint( std::vector<vector2d_t >& coords, double radius, double start_angle, double opening_angle, double x_center, double y_center, int segments );
	static void mirrorCopyPath(			std::vector<vector2d_t >& coords, bool mirror_on_y_axis, bool mirror_on_x_axis );
	static void mirrorCopyPathReverse(		std::vector<vector2d_t >& coords, bool mirror_on_y_axis, bool mirror_on_x_axis );
	static void addAvoidingDuplicates( const std::vector<vector2d_t >& polygon, std::vector<std::vector<vector2d_t > >& paths );
	static void deleteLastPointIfEqualToFirst( std::vector<vector2d_t >& polygon );
	static void simplifyPaths( std::vector<std::vector<vector2d_t > >& paths );
	static void simplifyPath( std::vector<vector2d_t >& paths );
	const std::vector<std::vector<vector2d_t > >& getCoordinates() { return m_paths; }
	void clearProfileConverter() { m_paths.clear(); }

	shared_ptr<CurveConverter>				m_curve_converter;
	shared_ptr<SplineConverter>				m_spline_converter;

protected:
	std::vector<std::vector<vector2d_t > >	m_paths;
};

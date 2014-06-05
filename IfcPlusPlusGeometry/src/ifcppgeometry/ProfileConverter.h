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
#include "IncludeCarveHeaders.h"

class GeometrySettings;
class UnitConverter;
class IfcProfileDef;
class IfcArbitraryClosedProfileDef;
class IfcArbitraryOpenProfileDef;
class IfcCompositeProfileDef;
class IfcDerivedProfileDef;
class IfcParameterizedProfileDef;

class ProfileConverter
{
public:
	ProfileConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> unit_converter );
	~ProfileConverter();

	void computeProfile( shared_ptr<IfcProfileDef> profile_def );
	void convertIfcArbitraryClosedProfileDef(				const shared_ptr<IfcArbitraryClosedProfileDef>& profile_def,	std::vector<std::vector<carve::geom::vector<2> > >& paths ) const;
	void convertIfcArbitraryOpenProfileDef(					const shared_ptr<IfcArbitraryOpenProfileDef>& profile_def,		std::vector<std::vector<carve::geom::vector<2> > >& paths ) const;
	void convertIfcCompositeProfileDef(						const shared_ptr<IfcCompositeProfileDef>& profile_def,			std::vector<std::vector<carve::geom::vector<2> > >& paths ) const;
	void convertIfcDerivedProfileDef(						const shared_ptr<IfcDerivedProfileDef>& profile_def,			std::vector<std::vector<carve::geom::vector<2> > >& paths ) const;
	void convertIfcParameterizedProfileDef(					const shared_ptr<IfcParameterizedProfileDef>& profile_def,		std::vector<std::vector<carve::geom::vector<2> > >& paths ) const;
	void convertIfcParameterizedProfileDefWithPosition(		const shared_ptr<IfcParameterizedProfileDef>& profile_def,		std::vector<std::vector<carve::geom::vector<2> > >& paths ) const;
	void addArc(					std::vector<carve::geom::vector<2> >& coords, double radius, double start_angle, double opening_angle, double xM, double yM, int segments = -1 ) const;
	void addArcWithEndPoint(		std::vector<carve::geom::vector<2> >& coords, double radius, double start_angle, double opening_angle, double xM, double yM ) const;
	void simplifyPaths();
	
	static void addArcWithEndPoint(	std::vector<carve::geom::vector<2> >& coords, double radius, double start_angle, double opening_angle, double xM, double yM, int segments );
	static void mirrorCopyPath(			std::vector<carve::geom::vector<2> >& coords, bool mirror_on_y_axis, bool mirror_on_x_axis );
	static void mirrorCopyPathReverse(		std::vector<carve::geom::vector<2> >& coords, bool mirror_on_y_axis, bool mirror_on_x_axis );
	static void addAvoidingDuplicates( const std::vector<carve::geom::vector<2> >& polygon, std::vector<std::vector<carve::geom::vector<2> > >& paths );
	static void deleteLastPointIfEqualToFirst( std::vector<carve::geom::vector<2> >& polygon );
	static void simplifyPaths( std::vector<std::vector<carve::geom::vector<2> > >& paths );
	static void simplifyPath( std::vector<carve::geom::vector<2> >& paths );
	const std::vector<std::vector<carve::geom::vector<2> > >& getCoordinates() { return m_paths; }

protected:
	shared_ptr<GeometrySettings>						m_geom_settings;
	shared_ptr<UnitConverter>							m_unit_converter;
	std::vector<std::vector<carve::geom::vector<2> > >	m_paths;
};

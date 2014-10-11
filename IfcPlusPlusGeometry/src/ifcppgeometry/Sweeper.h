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
#include "GeometryInputData.h"
#include "IncludeCarveHeaders.h"

class GeometrySettings;
class UnitConverter;
class PointConverter;
class ProfileCache;
class FaceConverter;
class CurveConverter;

class IfcExtrudedAreaSolid;
class IfcSolidModel;
class IfcBooleanResult;
class IfcBooleanOperand;
class IfcHalfSpaceSolid;
class IfcRevolvedAreaSolid;
class IfcCsgPrimitive3D;

class Sweeper : public StatusCallback
{
public:
	Sweeper( shared_ptr<GeometrySettings>& settings, shared_ptr<UnitConverter>& uc );
	~Sweeper();

	/*\brief Extrudes a set of cross sections along a direction
	  \param[in] paths Set of cross sections to extrude
	  \param[in] dir Extrusion vector
	  \param[out] item_data Container to add result polyhedron or polyline
	**/
	void extrude(	const std::vector<std::vector<carve::geom::vector<2> > >& paths, const carve::geom::vector<3> dir, shared_ptr<ItemData>& item_data );

	/*\brief Extrudes a circle cross section along a path. At turns, the points are placed in the bisecting plane
	  \param[in] curve_points Path along which the circle is swept
	  \param[out] item_data Container to add result polyhedron or polyline
	  \param[in] nvc Number of vertices per circle
	  \param[in] radius_inner If positive value is given, the swept disk becomes a pipe
	**/
	void sweepDisk(	const std::vector<carve::geom::vector<3> >& curve_points, shared_ptr<ItemData>& item_data, const int nvc, const double radius, const double radius_inner = -1 );

	/*\brief Extrudes a cross section along a path. At turns, the points are placed in the bisecting plane
	  \param[in] curve_points Path along which the cross section is swept
	  \param[in] profile_paths Set of cross sections to sweep
	  \param[out] item_data Container to add result polyhedron or polyline
	**/
	void sweepArea(	const std::vector<carve::geom::vector<3> >& curve_points, const std::vector<std::vector<carve::geom::vector<2> > >& profile_paths, shared_ptr<ItemData>& item_data );

	/*\brief Creates a triangulated face
	  \param[in] Curves as face boundaries. The first input curve is the outer boundary, succeeding curves are inner boundaries **/
	void createFace( const std::vector<std::vector<carve::geom::vector<3> > >& curves, PolyInputCache3D& poly_data );

protected:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;
};

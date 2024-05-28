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

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <IfcCsgPrimitive3D.h>
#include <IfcExtrudedAreaSolid.h>
#include <IfcHalfSpaceSolid.h>
#include <IfcIndexedPolygonalFace.h>
#include <IfcRevolvedAreaSolid.h>
#include "IncludeCarveHeaders.h"
class FaceConverter;
class PointConverter;
class CurveConverter;
class ProfileCache;
class StylesConverter;
class Sweeper;

class SolidModelConverter : public StatusCallback
{
public:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<PointConverter>			m_point_converter;
	shared_ptr<CurveConverter>			m_curve_converter;
	shared_ptr<FaceConverter>			m_face_converter;
	shared_ptr<ProfileCache>			m_profile_cache;
	shared_ptr<StylesConverter>			m_styles_converter;
	shared_ptr<Sweeper>					m_sweeper;
	std::mutex m_mutex1;
	std::mutex m_mutex2;

	SolidModelConverter(shared_ptr<GeometrySettings>& gs, shared_ptr<PointConverter>& pc, shared_ptr<CurveConverter>& cc,
		shared_ptr<FaceConverter>& fc, shared_ptr<ProfileCache>& pcache, shared_ptr<Sweeper>& sw, shared_ptr<StylesConverter>& styles_converter);

	virtual ~SolidModelConverter();

	// ENTITY IfcSolidModel ABSTRACT SUPERTYPE OF(ONEOF(IfcCsgSolid, IfcManifoldSolidBrep, IfcSweptAreaSolid, IfcSweptDiskSolid))
	void convertIfcSolidModel(const shared_ptr<IfcSolidModel>& solid_model, shared_ptr<ItemShapeData> item_data);

	void convertIfcExtrudedAreaSolid(const shared_ptr<IfcExtrudedAreaSolid>& extruded_area, shared_ptr<ItemShapeData> item_data);

	void convertRevolvedAreaSolid(const std::vector<std::vector<vec2> >& profile_coords_unchecked, const vec3& axis_location, const vec3& axis_direction, double revolution_angle, shared_ptr<ItemShapeData> item_data, BuildingEntity* entity_of_origin = nullptr);

	void convertIfcRevolvedAreaSolid(const shared_ptr<IfcRevolvedAreaSolid>& revolved_area, shared_ptr<ItemShapeData> item_data);

	void convertIfcBooleanResult(const shared_ptr<IfcBooleanResult>& bool_result, shared_ptr<ItemShapeData> item_data);

	void convertIfcCsgPrimitive3D(const shared_ptr<IfcCsgPrimitive3D>& csg_primitive, shared_ptr<ItemShapeData> item_data);

	void extrudeBox(const std::vector<vec3>& boundary_points, const vec3& extrusion_vector, shared_ptr<carve::input::PolyhedronData>& box_data);

	void convertIfcHalfSpaceSolid(const shared_ptr<IfcHalfSpaceSolid>& half_space_solid, shared_ptr<ItemShapeData>& item_data, const shared_ptr<ItemShapeData>& other_operand);

	void copyIndexedFaceLoop(const std::vector<shared_ptr<IfcPositiveInteger> >& vecIdx, const std::vector<vec3>& vecPointsIn, std::vector<vec3>& vecOut);

	void convertIndexedPolygonalFace(shared_ptr<IfcIndexedPolygonalFace>& polygonalFace, std::vector<vec3>& pointStorage, PolyInputCache3D& poly_cache);

	void convertTesselatedItem(const shared_ptr<IfcTessellatedItem>& tessellatedItem, shared_ptr<ItemShapeData>& itemData);

	void convertIfcBooleanOperand(const shared_ptr<IfcBooleanOperand>& operand_select, shared_ptr<ItemShapeData>& item_data, 
		const shared_ptr<ItemShapeData>& other_operand);

	void convertIfcSectionedSpine(const shared_ptr<IfcSectionedSpine>& spine, shared_ptr<ItemShapeData> item_data);

	void addFaceIndices(std::vector<std::vector<std::vector<size_t> > > pointIndicesDebug, PolyInputCache3D& poly, const shared_ptr<BuildingEntity>& entity);
};

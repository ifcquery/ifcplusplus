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

#include "IncludeCarveHeaders.h"
#include <ifcpp/model/shared_ptr.h>
#include "GeometryInputData.h"

class GeometrySettings;
class UnitConverter;
class ProfileCache;
class FaceConverter;
class CurveConverter;

class IfcExtrudedAreaSolid;
class IfcSolidModel;
class IfcBooleanResult;
class IfcBooleanOperand;
class IfcRevolvedAreaSolid;
class IfcCsgPrimitive3D;

class SolidModelConverter
{
public:
	SolidModelConverter( shared_ptr<GeometrySettings> settings, shared_ptr<UnitConverter> uc, shared_ptr<CurveConverter>	cc, shared_ptr<FaceConverter> fc, shared_ptr<ProfileCache>	pc );
	~SolidModelConverter();

	void convertIfcBooleanResult(		const shared_ptr<IfcBooleanResult>& operand,			shared_ptr<ItemData> item_data, std::stringstream& err );
	void convertIfcBooleanOperand(		const shared_ptr<IfcBooleanOperand>& operand,			shared_ptr<ItemData> item_data, const shared_ptr<ItemData>& other_operand, std::stringstream& err );
	void convertIfcSolidModel(			const shared_ptr<IfcSolidModel>& solid_model,			shared_ptr<ItemData> item_data, std::stringstream& err );
	void convertIfcExtrudedAreaSolid(	const shared_ptr<IfcExtrudedAreaSolid>& extruded_area,	shared_ptr<ItemData> item_data, std::stringstream& err );
	void convertIfcRevolvedAreaSolid(	const shared_ptr<IfcRevolvedAreaSolid>& revolved_area,	shared_ptr<ItemData> item_data, std::stringstream& err );
	void convertIfcCsgPrimitive3D(		const shared_ptr<IfcCsgPrimitive3D>& csg_primitive,		shared_ptr<ItemData> item_data, std::stringstream& err );

protected:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;
	shared_ptr<CurveConverter>			m_curve_converter;
	shared_ptr<FaceConverter>			m_face_converter;
	shared_ptr<ProfileCache>			m_profile_cache;
};

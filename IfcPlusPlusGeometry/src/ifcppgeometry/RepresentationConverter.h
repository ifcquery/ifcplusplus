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

#include <set>
#include <sstream>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/StatusCallback.h>
#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "GeometrySettings.h"

class UnitConverter;
class StylesConverter;
class PlacementConverter;
class Sweeper;
class SplineConverter;
class PointConverter;
class CurveConverter;
class SolidModelConverter;
class FaceConverter;
class ProfileCache;

class IfcElement;
class IfcRepresentation;
class IfcRepresentationItem;
class IfcGeometricRepresentationItem;
class IfcSectionedSpine;
class IfcReferencedSectionedSpine;
class IfcPropertySet;

class RepresentationConverter : public StatusCallback
{
public:
	RepresentationConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> unit_converter );
	~RepresentationConverter();

	void convertIfcRepresentation(				const shared_ptr<IfcRepresentation>& representation,	shared_ptr<ShapeInputData>& shape_data );
	void convertIfcGeometricRepresentationItem(	const shared_ptr<IfcGeometricRepresentationItem>& item,	shared_ptr<ItemData> item_data );
	void convertRepresentationStyle(			const shared_ptr<IfcRepresentationItem>& representation_item, std::vector<shared_ptr<AppearanceData> >& vec_appearance_data );
	void subtractOpenings(						const shared_ptr<IfcElement>& ifc_element, shared_ptr<ShapeInputData>& product_shape );

	bool handleLayerAssignments() { return m_handle_layer_assignments; }
	void setHandleLayerAssignments( bool handle ) { m_handle_layer_assignments = handle; }
	bool handleStyledItems() { return m_handle_styled_items; }
	void setHandleStyledItems( bool handle ) { m_handle_styled_items = handle; }
	void clearCache();
	void setUnitConverter( shared_ptr<UnitConverter>& unit_converter );
	shared_ptr<UnitConverter>&			getUnitConverter()	{ return m_unit_converter;	}
	shared_ptr<GeometrySettings>&		getGeomSettings()	{ return m_geom_settings;	}
	shared_ptr<SolidModelConverter>&	getSolidConverter() { return m_solid_converter;	}
	shared_ptr<ProfileCache>&			getProfileCache()	{ return m_profile_cache;	}
	shared_ptr<Sweeper>&				getSweeper()		{ return m_sweeper;	}
	shared_ptr<PlacementConverter>&		getPlacementConverter() { return m_placement_converter; }
	shared_ptr<StylesConverter>&		getStylesConverter()	{ return m_styles_converter; }

protected:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;
	shared_ptr<PlacementConverter>		m_placement_converter;
	shared_ptr<Sweeper>					m_sweeper;
	shared_ptr<StylesConverter>			m_styles_converter;
	shared_ptr<SplineConverter>			m_spline_converter;
	shared_ptr<PointConverter>			m_point_converter;
	shared_ptr<CurveConverter>			m_curve_converter;
	shared_ptr<SolidModelConverter>		m_solid_converter;
	shared_ptr<FaceConverter>			m_face_converter;
	shared_ptr<ProfileCache>			m_profile_cache;
	
	bool								m_handle_styled_items;
	bool								m_handle_layer_assignments;
};

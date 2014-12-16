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

#include <vector>
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>
#include <ifcpp/IFC4/include/IfcTextStyle.h>
#include "IncludeCarveHeaders.h"

#include <osg/ref_ptr>
#include <osg/Switch>

class TextItemData
{
public:
	std::wstring m_text;
	carve::math::Matrix m_text_position;
};

class AppearanceData
{
public:
	enum GeometryTypeEnum { UNDEFINED, TEXT, CURVE, SURFACE, VOLUME };
	AppearanceData( int step_style_id ) : m_step_stype_id( step_style_id )
	{
		m_set_transparent = false;
		m_shininess = 10.f;
		m_transparency = 1.f;
		m_specular_exponent = 0.f;
		m_specular_roughness = 0.f;
		m_apply_to_geometry_type = UNDEFINED;
	}
	carve::geom::vector<4> m_color_ambient;
	carve::geom::vector<4> m_color_diffuse;
	carve::geom::vector<4> m_color_specular;
	int m_step_stype_id;
	float m_shininess;
	float m_transparency;
	float m_specular_exponent;
	float m_specular_roughness;
	bool m_set_transparent;
	shared_ptr<IfcTextStyle> m_text_style;
	GeometryTypeEnum m_apply_to_geometry_type;
};

//\brief Class to hold input data of one IFC geometric representation item.
class ItemData
{
public:
	ItemData();
	~ItemData();
	
	std::vector<shared_ptr<carve::input::PolylineSetData> > m_polylines;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		m_meshsets;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		m_meshsets_open;
	std::vector<shared_ptr<AppearanceData> >				m_vec_item_appearances;
	std::vector<shared_ptr<TextItemData> >					m_vec_text_literals;

	bool isEmpty();
	void addOpenOrClosedPolyhedron( shared_ptr<carve::input::PolyhedronData>& poly_data );
	void addOpenPolyhedron( shared_ptr<carve::input::PolyhedronData>& poly_data );
	void addClosedPolyhedron( shared_ptr<carve::input::PolyhedronData>& poly_data );
	void applyPosition( const carve::math::Matrix& mat );
	shared_ptr<ItemData> getDeepCopy();
	
	/** copies the content of other instance and adds it to own content */
	void addItemData( shared_ptr<ItemData>& other );
};

class ShapeInputData
{
public:
	ShapeInputData() : m_added_to_node(false) {}
	virtual ~ShapeInputData() {}

	void addInputData( shared_ptr<ShapeInputData>& other );
	void deepCopyFrom( shared_ptr<ShapeInputData>& other );
	const std::vector<shared_ptr<AppearanceData> >& getAppearances() { return m_vec_appearances; }
	void addAppearance( shared_ptr<AppearanceData>& appearance );
	void clearAppearanceData();
	void clearAll();

	weak_ptr<IfcProduct> m_ifc_product;
	weak_ptr<IfcRepresentation> m_representation;
	weak_ptr<IfcObjectPlacement> m_object_placement;
	osg::ref_ptr<osg::Switch>	m_product_switch;
	osg::ref_ptr<osg::Switch>	m_product_switch_curves;
	std::vector<shared_ptr<ItemData> >		m_vec_item_data;
	bool m_added_to_node;
	std::wstring m_representation_identifier;
	std::wstring m_representation_type;

protected:
	std::vector<shared_ptr<AppearanceData> >	m_vec_appearances;
};

class PolyInputCache3D
{
public:
	PolyInputCache3D();
	int addPoint( const carve::geom::vector<3>& v );
	int addPointPrecise( const carve::geom::vector<3>& v );

	shared_ptr<carve::input::PolyhedronData> m_poly_data;
	std::map<double, std::map<double, std::map<double, int> > > m_existing_vertices_coords;
};

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
	std::string m_text;
	carve::math::Matrix m_text_position;
};

class AppearanceData
{
public:
	AppearanceData()
	{
		set_transparent = false;
		shininess = 10.f;
		transparency = 1.f;
		specular_exponent = 0.f;
		specular_roughness = 0.f;
	}
	carve::geom::vector<4> color_ambient;
	carve::geom::vector<4> color_diffuse;
	carve::geom::vector<4> color_specular;
	float shininess;
	float transparency;
	float specular_exponent;
	float specular_roughness;
	bool set_transparent;
	shared_ptr<IfcTextStyle> text_style;
};

//\brief Class to hold input data of one IFC geometric representation item.
class ItemData
{
public:
	ItemData();
	~ItemData();
	std::vector<shared_ptr<carve::input::PolyhedronData> >	closed_polyhedrons;
	std::vector<shared_ptr<carve::input::PolyhedronData> >	open_polyhedrons;
	std::vector<shared_ptr<carve::input::PolyhedronData> >	open_or_closed_polyhedrons;
	std::vector<shared_ptr<carve::input::PolylineSetData> > polylines;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		meshsets;
	std::vector<shared_ptr<AppearanceData> >				vec_item_appearances;
	std::vector<shared_ptr<TextItemData> >					vec_text_literals;
	bool													m_csg_computed;

	bool isEmpty();
	void createMeshSetsFromClosedPolyhedrons();
	void applyPosition( const carve::math::Matrix& mat );
	shared_ptr<ItemData> getDeepCopy();
	
	/** copies the content of other instance and adds it to own content */
	void addItemData( shared_ptr<ItemData>& other );
};

struct PlacementData
{
	std::set<int> placement_already_applied;
	carve::math::Matrix pos;
};

class ShapeInputData
{
public:
	ShapeInputData() { added_to_storey = false; }
	~ShapeInputData() {}

	void addInputData( shared_ptr<ShapeInputData>& other );
	void deepCopyFrom( shared_ptr<ShapeInputData>& other );

	shared_ptr<IfcProduct> ifc_product;
	shared_ptr<IfcRepresentation> representation;
	shared_ptr<IfcObjectPlacement> object_placement;
	osg::ref_ptr<osg::Switch> product_switch;
	osg::ref_ptr<osg::Switch> space_switch;
	//std::vector<shared_ptr<IfcProduct> > vec_openings;
	
	std::vector<shared_ptr<ItemData> >			vec_item_data;
	std::vector<shared_ptr<AppearanceData> >	vec_appearances;
	bool added_to_storey;
};

class PolyInputCache3D
{
public:
	PolyInputCache3D();
	int addPoint( const carve::geom::vector<3>& v );
	int addPointPrecise( const carve::geom::vector<3>& v );
	shared_ptr<carve::input::PolyhedronData> m_poly_data;
	std::map<double, std::map<double, std::map<double, int> > > existing_vertices_coords;
	std::map<double, int>::iterator it_find_z;
};

/* -*-c++-*- IFC++ www.ifcquery.com
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

#include <vector>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>

#include <ifcpp/geometry/AppearanceData.h>
#include <ifcpp/model/IfcPPBasicTypes.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>
#include <ifcpp/IFC4/include/IfcTextStyle.h>

#include "GeomUtils.h"

class TextItemData
{
public:
	std::wstring m_text;
	gp_Trsf m_text_position;
};

//\brief Class to hold input data of one IFC geometric representation item.
class ItemShapeData
{
public:
	ItemShapeData(){}
	~ItemShapeData(){}

	bool isEmpty()
	{
		if( m_vertex_points.size() > 0 )			{ return false; }
		if( m_polylines.size() > 0 )				{ return false; }
		if( m_shapes.size() > 0 )					{ return false; }
		if( m_vec_item_appearances.size() > 0 )		{ return false; }
		if( m_vec_text_literals.size() > 0 )		{ return false; }

		return true;
	}

	void addShape( TopoDS_Shape& shape )
	{
		if( !shape.IsNull() )
		{
			m_shapes.push_back( shape );
		}
	}

	void addPoint( const gp_Pnt& point )
	{
		m_vertex_points.push_back( BRepBuilderAPI_MakeVertex( point ) );
	}

	void addPoint( const vec3& point )
	{
		m_vertex_points.push_back( BRepBuilderAPI_MakeVertex( gp_Pnt( point.X(), point.Y(), point.Z() ) ) );
	}

	void applyTransformToItem( const gp_Trsf& mat, bool matrix_identity_checked = false )
	{
		if( !matrix_identity_checked )
		{
			if( GeomUtils::isMatrixIdentity( mat ) )
			{
				return;
			}
		}

		for( size_t ii = 0; ii < m_vertex_points.size(); ++ii )
		{
			TopoDS_Vertex& vertex_data = m_vertex_points[ii];

			GeomUtils::applyMatrixToShape( vertex_data, mat );
		}

		for( size_t polyline_i = 0; polyline_i < m_polylines.size(); ++polyline_i )
		{
			TopoDS_Wire& wire = m_polylines[polyline_i];
			GeomUtils::applyMatrixToShape( wire, mat );
		}

		for( size_t i_meshsets = 0; i_meshsets < m_shapes.size(); ++i_meshsets )
		{
			TopoDS_Shape& item_meshset = m_shapes[i_meshsets];
			GeomUtils::applyMatrixToShape( item_meshset, mat );
		}

		for( size_t text_i = 0; text_i < m_vec_text_literals.size(); ++text_i )
		{
			shared_ptr<TextItemData>& text_literals = m_vec_text_literals[text_i];
			text_literals->m_text_position = mat*text_literals->m_text_position;
		}
	}

	void applyTransformToItem( const gp_GTrsf& mat, bool matrix_identity_checked = false )
	{
		if( !matrix_identity_checked )
		{
			//if( GeomUtils::isMatrixIdentity( mat ) )
			//{
			//	return;
			//}
		}

		for( size_t ii = 0; ii < m_vertex_points.size(); ++ii )
		{
			TopoDS_Vertex& vertex_data = m_vertex_points[ii];
			GeomUtils::applyMatrixToShape( vertex_data, mat );
		}

		for( size_t polyline_i = 0; polyline_i < m_polylines.size(); ++polyline_i )
		{
			TopoDS_Wire& wire = m_polylines[polyline_i];
			GeomUtils::applyMatrixToShape( wire, mat );
		}

		for( size_t i_meshsets = 0; i_meshsets < m_shapes.size(); ++i_meshsets )
		{
			TopoDS_Shape& item_meshset = m_shapes[i_meshsets];
			GeomUtils::applyMatrixToShape( item_meshset, mat );
		}

		for( size_t text_i = 0; text_i < m_vec_text_literals.size(); ++text_i )
		{
			shared_ptr<TextItemData>& text_literals = m_vec_text_literals[text_i];
			text_literals->m_text_position = (mat*text_literals->m_text_position).Trsf();
		}
	}

	shared_ptr<ItemShapeData> getDeepCopy()
	{
		shared_ptr<ItemShapeData> copy_item( new ItemShapeData() );

		for( size_t ii = 0; ii < m_vertex_points.size(); ++ii )
		{
			TopoDS_Vertex& data = m_vertex_points[ii];
			copy_item->m_vertex_points.push_back( TopoDS_Vertex( data ) );
		}

		for( size_t ii = 0; ii < m_polylines.size(); ++ii )
		{
			TopoDS_Wire polyline_data = m_polylines[ii];
			copy_item->m_polylines.push_back( polyline_data );
		}

		for( auto it_meshsets = m_shapes.begin(); it_meshsets != m_shapes.end(); ++it_meshsets )
		{
			TopoDS_Shape item_meshset = (*it_meshsets);
			copy_item->m_shapes.push_back( TopoDS_Shape( item_meshset ) );
		}

		for( size_t ii = 0; ii < m_vec_text_literals.size(); ++ii )
		{
			shared_ptr<TextItemData>& text_data = m_vec_text_literals[ii];

			shared_ptr<TextItemData> text_data_copy( new TextItemData() );
			text_data_copy->m_text = text_data->m_text.c_str();
			text_data_copy->m_text_position = text_data->m_text_position;
			copy_item->m_vec_text_literals.push_back( text_data_copy );
		}

		std::copy( m_vec_item_appearances.begin(), m_vec_item_appearances.end(), std::back_inserter( copy_item->m_vec_item_appearances ) );

		return copy_item;
	}
	
	/** copies the content of other instance and adds it to own content */
	void addItemData( const shared_ptr<ItemShapeData>& other )
	{
		std::copy( other->m_vertex_points.begin(), other->m_vertex_points.end(), std::back_inserter( m_vertex_points ) );
		std::copy( other->m_polylines.begin(), other->m_polylines.end(), std::back_inserter( m_polylines ) );
		std::copy( other->m_shapes.begin(), other->m_shapes.end(), std::back_inserter( m_shapes ) );
		std::copy( other->m_vec_item_appearances.begin(), other->m_vec_item_appearances.end(), std::back_inserter( m_vec_item_appearances ) );
		std::copy( other->m_vec_text_literals.begin(), other->m_vec_text_literals.end(), std::back_inserter( m_vec_text_literals ) );
	}

	const std::vector<TopoDS_Vertex>& getVertexPoints() { return m_vertex_points; }
	const std::vector<TopoDS_Wire>& getPolylines() { return m_polylines; }
	const std::vector<TopoDS_Shape>& getShapes() { return m_shapes; }
	const std::vector<shared_ptr<AppearanceData> >& getAppearances() { return m_vec_item_appearances; }
	const std::vector<shared_ptr<TextItemData> >& getTextItems() { return m_vec_text_literals; }
	
protected:
	std::vector<TopoDS_Vertex>					m_vertex_points;
	std::vector<TopoDS_Wire>					m_polylines;
	std::vector<TopoDS_Shape>					m_shapes;
	std::vector<shared_ptr<AppearanceData> >	m_vec_item_appearances;
	std::vector<shared_ptr<TextItemData> >		m_vec_text_literals;

friend class SolidModelConverter;
friend class RepresentationConverter;
};

class RepresentationData
{
public:
	RepresentationData() {}
	~RepresentationData(){}

	weak_ptr<IfcRepresentation>						m_ifc_representation;
	weak_ptr<IfcRepresentationContext>				m_ifc_representation_context;
	std::vector<shared_ptr<ItemShapeData> >	m_vec_item_data;
	std::vector<shared_ptr<AppearanceData> >		m_vec_representation_appearances;
	std::wstring									m_representation_identifier;
	std::wstring									m_representation_type;

	shared_ptr<RepresentationData> getDeepCopy()
	{
		shared_ptr<RepresentationData> copy_representation( new RepresentationData() );
		copy_representation->m_ifc_representation = m_ifc_representation;
		copy_representation->m_ifc_representation_context = m_ifc_representation_context;
		for( size_t ii = 0; ii < m_vec_item_data.size(); ++ii )
		{
			shared_ptr<ItemShapeData>& item_data = m_vec_item_data[ii];
			copy_representation->m_vec_item_data.push_back( item_data->getDeepCopy() );
		}
		std::copy( m_vec_representation_appearances.begin(), m_vec_representation_appearances.end(), std::back_inserter( copy_representation->m_vec_representation_appearances ) );
		return copy_representation;
	}

	void addInputData( shared_ptr<RepresentationData>& other )
	{
		std::copy( other->m_vec_item_data.begin(), other->m_vec_item_data.end(), std::back_inserter( m_vec_item_data ) );
		std::copy( other->m_vec_representation_appearances.begin(), other->m_vec_representation_appearances.end(), std::back_inserter( m_vec_representation_appearances ) );
	}

	void deepCopyFrom( shared_ptr<RepresentationData>& other )
	{
		m_vec_item_data.clear();
		m_vec_representation_appearances.clear();

		if( other )
		{
			for( size_t item_i = 0; item_i < other->m_vec_item_data.size(); ++item_i )
			{
				shared_ptr<ItemShapeData>& item_data = other->m_vec_item_data[item_i];
				m_vec_item_data.push_back( shared_ptr<ItemShapeData>( item_data->getDeepCopy() ) );
			}
			std::copy( other->m_vec_representation_appearances.begin(), other->m_vec_representation_appearances.end(), std::back_inserter( m_vec_representation_appearances ) );
		}
	}

	void addAppearance( shared_ptr<AppearanceData>& appearance )
	{
		if( !appearance )
		{
			return;
		}
		int append_id = appearance->m_step_style_id;
		for( size_t ii = 0; ii < m_vec_representation_appearances.size(); ++ii )
		{
			shared_ptr<AppearanceData>& existing_appearance = m_vec_representation_appearances[ii];
			if( existing_appearance->m_step_style_id == append_id )
			{
				return;
			}
		}
		m_vec_representation_appearances.push_back( appearance );
	}

	void clearAppearanceData()
	{
		m_vec_representation_appearances.clear();
	}

	void clearAll()
	{
		m_vec_representation_appearances.clear();
		m_ifc_representation.reset();
		m_ifc_representation_context.reset();
		m_vec_item_data.clear();
		m_representation_identifier = L"";
		m_representation_type = L"";
	}

	void applyTransformToRepresentation( const gp_Trsf& matrix, bool matrix_identity_checked = false )
	{
		if( GeomUtils::isMatrixIdentity( matrix ) )
		{
			return;
		}
		for( size_t i_item = 0; i_item < m_vec_item_data.size(); ++i_item )
		{
			m_vec_item_data[i_item]->applyTransformToItem( matrix, matrix_identity_checked );
		}
	}
	void applyTransformToRepresentation( const gp_GTrsf& matrix, bool matrix_identity_checked = false )
	{
		//if( GeomUtils::isMatrixIdentity( matrix ) )
		//{
		//	return;
		//}
		for( size_t i_item = 0; i_item < m_vec_item_data.size(); ++i_item )
		{
			m_vec_item_data[i_item]->applyTransformToItem( matrix, matrix_identity_checked );
		}
	}
};

class ProductShapeData
{
public:
	ProductShapeData() {}
	ProductShapeData( int entity_id ) : m_entity_id( entity_id ) { }
	virtual ~ProductShapeData() {}

	void addInputData( shared_ptr<ProductShapeData>& other )
	{
		std::copy( other->m_vec_representations.begin(), other->m_vec_representations.end(), std::back_inserter( m_vec_representations ) );
		std::copy( other->m_vec_product_appearances.begin(), other->m_vec_product_appearances.end(), std::back_inserter( m_vec_product_appearances ) );
	}

	void deepCopyFrom( shared_ptr<ProductShapeData>& other )
	{
		m_vec_representations.clear();
		m_vec_product_appearances.clear();

		if( other )
		{
			for( size_t item_i = 0; item_i < other->m_vec_representations.size(); ++item_i )
			{
				shared_ptr<RepresentationData>& representation_data = other->m_vec_representations[item_i];
				m_vec_representations.push_back( shared_ptr<RepresentationData>( representation_data->getDeepCopy() ) );
			}
			std::copy( other->m_vec_product_appearances.begin(), other->m_vec_product_appearances.end(), std::back_inserter( m_vec_product_appearances ) );
		}
	}

	void addAppearance( shared_ptr<AppearanceData>& appearance )
	{
		if( !appearance )
		{
			return;
		}
		int append_id = appearance->m_step_style_id;
		for( size_t ii = 0; ii < m_vec_product_appearances.size(); ++ii )
		{
			shared_ptr<AppearanceData>& existing_appearance = m_vec_product_appearances[ii];
			if( existing_appearance->m_step_style_id == append_id )
			{
				return;
			}
		}
		m_vec_product_appearances.push_back( appearance );
	}

	void clearAppearanceData()
	{
		m_vec_product_appearances.clear();
	}

	void clearAll()
	{
		m_vec_product_appearances.clear();

		m_ifc_object_definition.reset();
		m_object_placement.reset();
		m_vec_representations.clear();
		m_added_to_spatial_structure = false;
	}

	void applyTransformToProduct( const gp_Trsf& matrix )
	{
		if( GeomUtils::isMatrixIdentity( matrix ) )
		{
			return;
		}
		for( size_t i_item = 0; i_item < m_vec_representations.size(); ++i_item )
		{
			m_vec_representations[i_item]->applyTransformToRepresentation( matrix, true );
		}
	}
	std::vector<shared_ptr<AppearanceData> >& getAppearances() { return m_vec_product_appearances; }
	const std::vector<shared_ptr<ProductShapeData> >& getChildren() { return m_vec_children; }

	bool isContainedInParentsList( shared_ptr<ProductShapeData>& product_data_check )
	{
		if( !m_parent.expired() )
		{
			shared_ptr<ProductShapeData> product_data_parent( m_parent );
			if( product_data_parent == product_data_check )
			{
				return true;
			}
			product_data_parent->isContainedInParentsList( product_data_check );
		}
		return false;
	}

	void addChildProduct( shared_ptr<ProductShapeData>& add_child, shared_ptr<ProductShapeData>& ptr_self )
	{
		if( ptr_self.get() != this )
		{
			std::cout << __FUNCTION__ << ": ptr_self.get() != this" << std::endl;
		}
		if( isContainedInParentsList( add_child ) )
		{
			std::cout << __FUNCTION__ << ": isContainedInParentsList" << std::endl;
			return;
		}
#ifdef _DEBUG
		for( size_t ii = 0; ii < m_vec_children.size(); ++ii )
		{
			const shared_ptr<ProductShapeData>& existing_child = m_vec_children[ii];
			if( existing_child == add_child )
			{
				if( existing_child->m_entity_id == add_child->m_entity_id )
				{
					std::cout << __FUNCTION__ << ": child already added, entity_id: " << add_child->m_entity_id << std::endl;
				}
				else
				{
					std::cout << __FUNCTION__ << ": entity_id mismatch: " << add_child->m_entity_id << " != " << existing_child->m_entity_id << std::endl;
				}
				return;
			}
		}
#endif
		m_vec_children.push_back( add_child );
		add_child->m_parent = ptr_self;
	}

public:
	int												m_entity_id = -1;
	bool											m_added_to_spatial_structure = false;
	weak_ptr<ProductShapeData>						m_parent;
	weak_ptr<IfcObjectDefinition>					m_ifc_object_definition;
	weak_ptr<IfcObjectPlacement>					m_object_placement;
	std::vector<shared_ptr<RepresentationData> >	m_vec_representations;

protected:
	std::vector<shared_ptr<ProductShapeData> >		m_vec_children;
	std::vector<shared_ptr<AppearanceData> >		m_vec_product_appearances;
};

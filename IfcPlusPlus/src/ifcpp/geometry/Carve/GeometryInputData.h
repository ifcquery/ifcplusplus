/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
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
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>
#include <ifcpp/IFC4/include/IfcTextStyle.h>
#include "IncludeCarveHeaders.h"
#include "GeomUtils.h"
#include "GeomDebugUtils.h"

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
	enum GeometryTypeEnum { UNDEFINED, TEXT, CURVE, SURFACE, VOLUME, ANY };
	AppearanceData( int step_style_id )
	{
		m_step_stype_id = step_style_id;
		m_complete = false;
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
	bool m_complete;
	shared_ptr<IfcTextStyle> m_text_style;
	GeometryTypeEnum m_apply_to_geometry_type;
};

//\brief Class to hold input data of one IFC geometric representation item.
class ItemShapeInputData
{
public:
	ItemShapeInputData(){}
	~ItemShapeInputData(){}

	std::vector<shared_ptr<carve::input::PolylineSetData> > m_polylines;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		m_meshsets;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		m_meshsets_open;
	std::vector<shared_ptr<AppearanceData> >				m_vec_item_appearances;
	std::vector<shared_ptr<TextItemData> >					m_vec_text_literals;

	bool isEmpty()
	{
		if( m_vertex_points.size() > 0 )			{ return false; }
		if( m_polylines.size() > 0 )				{ return false; }
		if( m_meshsets.size() > 0 )					{ return false; }
		if( m_meshsets_open.size() > 0 )			{ return false; }
		if( m_vec_item_appearances.size() > 0 )		{ return false; }
		if( m_vec_text_literals.size() > 0 )		{ return false; }

		return true;
	}

	void addOpenOrClosedPolyhedron( const shared_ptr<carve::input::PolyhedronData>& poly_data )
	{
		if( !poly_data )
		{
			return;
		}

		// check if it is open or closed
		if( poly_data->getVertexCount() < 3 )
		{
			return;
		}

		shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts() ) );
		if( meshset->isClosed() )
		{
			m_meshsets.push_back( meshset );
		}
		else
		{
			m_meshsets_open.push_back( meshset );
		}
	}

	void addOpenPolyhedron( const shared_ptr<carve::input::PolyhedronData>& poly_data )
	{
		if( poly_data->getVertexCount() < 3 )
		{
			return;
		}

		shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts() ) );
		m_meshsets_open.push_back( meshset );
	}

	void addClosedPolyhedron( const shared_ptr<carve::input::PolyhedronData>& poly_data )
	{
		if( poly_data->getVertexCount() < 3 )
		{
			return;
		}

		std::map<std::string, std::string> mesh_input_options;
		mesh_input_options["avoid_cavities"] = "true";
		shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( mesh_input_options ) );
		if( meshset->isClosed() )
		{
			m_meshsets.push_back( meshset );
		}
		else
		{
			m_meshsets_open.push_back( meshset ); // still may be useful as open mesh

#ifdef _DEBUG
			carve::geom::vector<4> color = carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 );
			GeomDebugUtils::dumpMeshset( meshset, color, true );
#endif
			throw IfcPPException( "Meshset is not closed", __FUNC__ );
		}
	}

	void addPoint( const vec3& point )
	{
		shared_ptr<carve::input::VertexData> vertex_data;
		if( m_vertex_points.size() > 0 )
		{
			if( !m_vertex_points[0] )
			{
				m_vertex_points[0] = shared_ptr<carve::input::VertexData>( new carve::input::VertexData() );
				if( !m_vertex_points[0] ){ throw IfcPPOutOfMemoryException( __FUNC__ ); }
			}
			vertex_data = m_vertex_points[0];
		}
		else
		{
			vertex_data = shared_ptr<carve::input::VertexData>( new carve::input::VertexData() );
			if( !vertex_data ){ throw IfcPPOutOfMemoryException( __FUNC__ ); }
			m_vertex_points.push_back( vertex_data );
		}

		vertex_data->points.push_back( point );
	}

	void applyPosition( const carve::math::Matrix& mat, bool matrix_identity_checked = false )
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
			shared_ptr<carve::input::VertexData>& vertex_data = m_vertex_points[ii];
			for( size_t j = 0; j<vertex_data->points.size(); ++j )
			{
				vec3& point = vertex_data->points[j];
				point = mat*point;
			}
		}

		for( size_t polyline_i = 0; polyline_i < m_polylines.size(); ++polyline_i )
		{
			shared_ptr<carve::input::PolylineSetData>& polyline_data = m_polylines[polyline_i];
			for( size_t j = 0; j<polyline_data->points.size(); ++j )
			{
				vec3& point = polyline_data->points[j];
				point = mat*point;
			}
		}

		for( size_t i_meshsets = 0; i_meshsets < m_meshsets_open.size(); ++i_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets_open[i_meshsets];

			for( size_t i = 0; i < item_meshset->vertex_storage.size(); ++i )
			{
				vec3& point = item_meshset->vertex_storage[i].v;
				point = mat*point;
			}
			for( size_t i = 0; i < item_meshset->meshes.size(); ++i )
			{
				item_meshset->meshes[i]->recalc();
			}
		}

		for( size_t i_meshsets = 0; i_meshsets < m_meshsets.size(); ++i_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets[i_meshsets];

			for( size_t i = 0; i < item_meshset->vertex_storage.size(); ++i )
			{
				vec3& point = item_meshset->vertex_storage[i].v;
				point = mat*point;
			}
			for( size_t i = 0; i < item_meshset->meshes.size(); ++i )
			{
				item_meshset->meshes[i]->recalc();
			}
		}

		for( size_t text_i = 0; text_i < m_vec_text_literals.size(); ++text_i )
		{
			shared_ptr<TextItemData>& text_literals = m_vec_text_literals[text_i];
			text_literals->m_text_position = mat*text_literals->m_text_position;
		}
	}

	shared_ptr<ItemShapeInputData> getDeepCopy()
	{
		shared_ptr<ItemShapeInputData> copy_item( new ItemShapeInputData() );

		for( size_t ii = 0; ii < m_vertex_points.size(); ++ii )
		{
			shared_ptr<carve::input::VertexData>& data = m_vertex_points[ii];
			copy_item->m_vertex_points.push_back( shared_ptr<carve::input::VertexData>( new carve::input::VertexData( *( data.get() ) ) ) );
		}

		for( size_t ii = 0; ii < m_polylines.size(); ++ii )
		{
			shared_ptr<carve::input::PolylineSetData>& polyline_data = m_polylines[ii];
			copy_item->m_polylines.push_back( shared_ptr<carve::input::PolylineSetData>( new carve::input::PolylineSetData( *( polyline_data.get() ) ) ) );
		}

		for( auto it_meshsets = m_meshsets_open.begin(); it_meshsets != m_meshsets_open.end(); ++it_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = ( *it_meshsets );
			copy_item->m_meshsets.push_back( shared_ptr<carve::mesh::MeshSet<3> >( item_meshset->clone() ) );
		}

		for( auto it_meshsets = m_meshsets.begin(); it_meshsets != m_meshsets.end(); ++it_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = ( *it_meshsets );
			copy_item->m_meshsets.push_back( shared_ptr<carve::mesh::MeshSet<3> >( item_meshset->clone() ) );
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
	void addItemData( const shared_ptr<ItemShapeInputData>& other )
	{
		std::copy( other->m_vertex_points.begin(), other->m_vertex_points.end(), std::back_inserter( m_vertex_points ) );
		std::copy( other->m_polylines.begin(), other->m_polylines.end(), std::back_inserter( m_polylines ) );
		std::copy( other->m_meshsets.begin(), other->m_meshsets.end(), std::back_inserter( m_meshsets ) );
		std::copy( other->m_meshsets_open.begin(), other->m_meshsets_open.end(), std::back_inserter( m_meshsets_open ) );
		std::copy( other->m_vec_item_appearances.begin(), other->m_vec_item_appearances.end(), std::back_inserter( m_vec_item_appearances ) );
		std::copy( other->m_vec_text_literals.begin(), other->m_vec_text_literals.end(), std::back_inserter( m_vec_text_literals ) );
	}

	const std::vector<shared_ptr<carve::input::VertexData> >& getVertexPoints() { return m_vertex_points; }

protected:
	std::vector<shared_ptr<carve::input::VertexData> >	m_vertex_points;
};

class ProductRepresentationData
{
public:
	ProductRepresentationData() {}
	~ProductRepresentationData(){}

	weak_ptr<IfcRepresentationContext>				m_ifc_representation_context;
	std::vector<shared_ptr<ItemShapeInputData> >	m_vec_item_data;
	std::vector<shared_ptr<AppearanceData> >		m_vec_representation_appearances;
	osg::ref_ptr<osg::Switch>						m_representation_switch;
	std::wstring									m_representation_identifier;
	std::wstring									m_representation_type;


	shared_ptr<ProductRepresentationData> getDeepCopy()
	{
		shared_ptr<ProductRepresentationData> copy_representation( new ProductRepresentationData() );
		copy_representation->m_ifc_representation_context = m_ifc_representation_context;
		for( size_t ii = 0; ii < m_vec_item_data.size(); ++ii )
		{
			shared_ptr<ItemShapeInputData>& item_data = m_vec_item_data[ii];
			copy_representation->m_vec_item_data.push_back( item_data->getDeepCopy() );
		}
		std::copy( m_vec_representation_appearances.begin(), m_vec_representation_appearances.end(), std::back_inserter( copy_representation->m_vec_representation_appearances ) );
		return copy_representation;
	}

	void addInputData( shared_ptr<ProductRepresentationData>& other )
	{
		std::copy( other->m_vec_item_data.begin(), other->m_vec_item_data.end(), std::back_inserter( m_vec_item_data ) );
		std::copy( other->m_vec_representation_appearances.begin(), other->m_vec_representation_appearances.end(), std::back_inserter( m_vec_representation_appearances ) );
	}

	void deepCopyFrom( shared_ptr<ProductRepresentationData>& other )
	{
		m_vec_item_data.clear();
		m_vec_representation_appearances.clear();

		if( other )
		{
			for( size_t item_i = 0; item_i < other->m_vec_item_data.size(); ++item_i )
			{
				shared_ptr<ItemShapeInputData>& item_data = other->m_vec_item_data[item_i];
				m_vec_item_data.push_back( shared_ptr<ItemShapeInputData>( item_data->getDeepCopy() ) );
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
		int append_id = appearance->m_step_stype_id;
		for( size_t ii = 0; ii < m_vec_representation_appearances.size(); ++ii )
		{
			shared_ptr<AppearanceData>& appearance = m_vec_representation_appearances[ii];
			if( appearance->m_step_stype_id == append_id )
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
		m_ifc_representation_context.reset();
		if( m_representation_switch )
		{
			GeomUtils::removeChildren( m_representation_switch );
		}
		m_vec_item_data.clear();
		
		m_representation_identifier = L"";
		m_representation_type = L"";
	}

	void applyPosition( const carve::math::Matrix& matrix, bool matrix_identity_checked = false )
	{
		if( GeomUtils::isMatrixIdentity( matrix ) )
		{
			return;
		}
		for( size_t i_item = 0; i_item < m_vec_item_data.size(); ++i_item )
		{
			m_vec_item_data[i_item]->applyPosition( matrix, matrix_identity_checked );
		}
	}
};

class ProductShapeInputData
{
public:
	ProductShapeInputData() : m_added_to_node(false) {}
	virtual ~ProductShapeInputData() {}

	void addInputData( shared_ptr<ProductShapeInputData>& other )
	{
		std::copy( other->m_vec_representations.begin(), other->m_vec_representations.end(), std::back_inserter( m_vec_representations ) );
		std::copy( other->m_vec_product_appearances.begin(), other->m_vec_product_appearances.end(), std::back_inserter( m_vec_product_appearances ) );
	}

	void deepCopyFrom( shared_ptr<ProductShapeInputData>& other )
	{
		m_vec_representations.clear();
		m_vec_product_appearances.clear();

		if( other )
		{
			for( size_t item_i = 0; item_i < other->m_vec_representations.size(); ++item_i )
			{
				shared_ptr<ProductRepresentationData>& representation_data = other->m_vec_representations[item_i];
				m_vec_representations.push_back( shared_ptr<ProductRepresentationData>( representation_data->getDeepCopy() ) );
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
		int append_id = appearance->m_step_stype_id;
		for( size_t ii = 0; ii < m_vec_product_appearances.size(); ++ii )
		{
			shared_ptr<AppearanceData>& appearance = m_vec_product_appearances[ii];
			if( appearance->m_step_stype_id == append_id )
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

		m_ifc_product.reset();
		m_object_placement.reset();
		if( m_product_switch )
		{
			GeomUtils::removeChildren( m_product_switch );
		}
		m_vec_representations.clear();
		m_added_to_node = false;
	}

	void applyPosition( const carve::math::Matrix& matrix )
	{
		if( GeomUtils::isMatrixIdentity( matrix ) )
		{
			return;
		}
		for( size_t i_item = 0; i_item < m_vec_representations.size(); ++i_item )
		{
			m_vec_representations[i_item]->applyPosition( matrix, true );
		}
	}
	std::vector<shared_ptr<AppearanceData> >& getAppearances() { return m_vec_product_appearances; }

	weak_ptr<IfcProduct>								m_ifc_product;
	weak_ptr<IfcObjectPlacement>						m_object_placement;
	osg::ref_ptr<osg::Switch>							m_product_switch;
	std::vector<shared_ptr<ProductRepresentationData> >	m_vec_representations;
	bool												m_added_to_node;
	
protected:
	std::vector<shared_ptr<AppearanceData> >			m_vec_product_appearances;
};

#define ROUND_POLY_COORDINATES_UP 1000000.0
#define ROUND_POLY_COORDINATES_DOWN 0.000001

class PolyInputCache3D
{
public:
	PolyInputCache3D()
	{
		m_poly_data = shared_ptr<carve::input::PolyhedronData>( new carve::input::PolyhedronData() );
	}

	size_t addPointPrecise( const vec3& v )
	{
		const double vertex_x = v.x;
		const double vertex_y = v.y;
		const double vertex_z = v.z;

		// insert: returns a pair, with its member pair::first set to an iterator pointing to either the newly inserted element or to the element with an equivalent key in the map
		std::map<double, std::map<double, size_t> >& map_y_index = m_existing_vertices_coords.insert( std::make_pair( vertex_x, std::map<double, std::map<double, size_t> >() ) ).first->second;
		std::map<double, size_t>& map_z_index = map_y_index.insert( std::make_pair( vertex_y, std::map<double, size_t>() ) ).first->second;

		std::map<double, size_t>::iterator it_find_z = map_z_index.find( vertex_z );
		if( it_find_z != map_z_index.end() )
		{
			// vertex already exists in polyhedron. return its index
			size_t vertex_index = it_find_z->second;
			return vertex_index;
		}
		else
		{
			// add point to polyhedron
			size_t vertex_index = m_poly_data->addVertex( v );
			map_z_index[vertex_z] = vertex_index;
			return vertex_index;
		}
	}
	
	size_t addPoint( const vec3& v )
	{
		const double vertex_x = round( v.x*ROUND_POLY_COORDINATES_UP )*ROUND_POLY_COORDINATES_DOWN;
		const double vertex_y = round( v.y*ROUND_POLY_COORDINATES_UP )*ROUND_POLY_COORDINATES_DOWN;
		const double vertex_z = round( v.z*ROUND_POLY_COORDINATES_UP )*ROUND_POLY_COORDINATES_DOWN;

		// insert: returns a pair, with its member pair::first set to an iterator pointing to either the newly inserted element or to the element with an equivalent key in the map
		std::map<double, std::map<double, size_t> >& map_y_index = m_existing_vertices_coords.insert( std::make_pair( vertex_x, std::map<double, std::map<double, size_t> >() ) ).first->second;
		std::map<double, size_t>& map_z_index = map_y_index.insert( std::make_pair( vertex_y, std::map<double, size_t>() ) ).first->second;

		std::map<double, size_t>::iterator it_find_z = map_z_index.find( vertex_z );
		if( it_find_z != map_z_index.end() )
		{
			// vertex already exists in polyhedron. return its index
			size_t vertex_index = it_find_z->second;
			return vertex_index;
		}
		else
		{
			// add point to polyhedron
			size_t vertex_index = m_poly_data->addVertex( v );
			map_z_index[vertex_z] = vertex_index;
			return vertex_index;
		}
	}

	bool checkFaceIndices()
	{
		const std::vector<carve::geom3d::Vector>& vec_points = m_poly_data->points;
		const std::vector<int>& face_indices = m_poly_data->faceIndices;
		int face_count = 0;
		for( size_t ii = 0; ii < face_indices.size(); ++ii )
		{
			int num_vertices = face_indices[ii];
			for( int jj = 0; jj < num_vertices; ++jj )
			{
				++ii;
				if( ii >= face_indices.size() )
				{
					return false;
				}
				int vertex_index = face_indices[ii];
				if( vertex_index >= (int)vec_points.size() )
				{
					return false;
				}
			}
			
			++face_count;
		}

		if( face_count != m_poly_data->faceCount )
		{
			return false;
		}
		return true;
	}

	shared_ptr<carve::input::PolyhedronData> m_poly_data;
	std::map<double, std::map<double, std::map<double, size_t> > > m_existing_vertices_coords;
};

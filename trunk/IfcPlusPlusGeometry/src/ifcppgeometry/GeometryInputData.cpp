#include "GeometryInputData.h"

ItemData::ItemData()
{
}
ItemData::~ItemData()
{
}

void ItemData::addOpenOrClosedPolyhedron( shared_ptr<carve::input::PolyhedronData>& poly_data )
{
	// check if it is open or closed
	if( poly_data->getVertexCount() < 3 )
	{
		return;
	}

	shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts() ) );
	if( meshset->isClosed() )
	{
		meshsets.push_back( meshset );
	}
	else
	{
		meshsets_open.push_back( meshset );
	}
}

void ItemData::addOpenPolyhedron( shared_ptr<carve::input::PolyhedronData>& poly_data )
{
	if( poly_data->getVertexCount() < 3 )
	{
		return;
	}

	shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts() ) );
	meshsets_open.push_back( meshset );

}


void ItemData::addItemData( shared_ptr<ItemData>& other )
{
	std::copy( other->meshsets_open.begin(),				other->meshsets_open.end(),					std::back_inserter( meshsets_open ) );
	std::copy( other->polylines.begin(),					other->polylines.end(),						std::back_inserter( polylines ) );
	std::copy( other->meshsets.begin(),						other->meshsets.end(),						std::back_inserter( meshsets ) );
	std::copy( other->vec_item_appearances.begin(),			other->vec_item_appearances.end(),			std::back_inserter( vec_item_appearances ) );
}

bool ItemData::isEmpty()
{
	if( meshsets_open.size() > 0 )				{ return false; }
	if( polylines.size() > 0 )					{ return false; }
	if( meshsets.size() > 0 )					{ return false; }
	if( vec_item_appearances.size() > 0 )		{ return false; }
	if( vec_text_literals.size() > 0 )			{ return false; }

	return true;
}

bool isIdentity( const carve::math::Matrix& mat )
{
	if( std::abs(mat._11 -1.0) > 0.00001 )  return false;
	if( std::abs(mat._22 -1.0) > 0.00001 )  return false;
	if( std::abs(mat._33 -1.0) > 0.00001 )  return false;
	if( std::abs(mat._44 -1.0) > 0.00001 )  return false;
	
	if( std::abs(mat._12) > 0.00001 )  return false;
	if( std::abs(mat._13) > 0.00001 )  return false;
	if( std::abs(mat._14) > 0.00001 )  return false;
	
	if( std::abs(mat._21) > 0.00001 )  return false;
	if( std::abs(mat._23) > 0.00001 )  return false;
	if( std::abs(mat._24) > 0.00001 )  return false;
	
	if( std::abs(mat._31) > 0.00001 )  return false;
	if( std::abs(mat._32) > 0.00001 )  return false;
	if( std::abs(mat._34) > 0.00001 )  return false;

	if( std::abs(mat._41) > 0.00001 )  return false;
	if( std::abs(mat._42) > 0.00001 )  return false;
	if( std::abs(mat._43) > 0.00001 )  return false;
	return true;
}


void ItemData::applyPosition( const carve::math::Matrix& mat )
{
	if( isIdentity( mat ) )
	{
		return;
	}

	for( auto it_meshsets = meshsets_open.begin(); it_meshsets != meshsets_open.end(); ++it_meshsets )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = (*it_meshsets);

		for (size_t i = 0; i < item_meshset->vertex_storage.size(); ++i )
		{
			carve::geom::vector<3>& point = item_meshset->vertex_storage[i].v;
			point = mat*point;
		}
		for (size_t i = 0; i < item_meshset->meshes.size(); ++i)
		{
			item_meshset->meshes[i]->recalc();
        }
	}

	for( auto it_meshsets = meshsets.begin(); it_meshsets != meshsets.end(); ++it_meshsets )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = (*it_meshsets);

		for (size_t i = 0; i < item_meshset->vertex_storage.size(); ++i )
		{
			carve::geom::vector<3>& point = item_meshset->vertex_storage[i].v;
			point = mat*point;
		}
		for (size_t i = 0; i < item_meshset->meshes.size(); ++i)
		{
			item_meshset->meshes[i]->recalc();
        }
	}

	for( int polyline_i = 0; polyline_i < polylines.size(); ++polyline_i )
	{
		shared_ptr<carve::input::PolylineSetData>& polyline_data = polylines[polyline_i];
		for( size_t j=0; j<polyline_data->points.size(); ++j )
		{
			carve::geom::vector<3>& point = polyline_data->points[j];
			point = mat*point;
		}
	}

	for( int text_i = 0; text_i < vec_text_literals.size(); ++text_i )
	{
		shared_ptr<TextItemData>& text_literals = vec_text_literals[text_i];
		text_literals->m_text_position = mat*text_literals->m_text_position;
	}
}

shared_ptr<ItemData> ItemData::getDeepCopy()
{
	shared_ptr<ItemData> copy_item( new ItemData() );

	for( auto it_meshsets = meshsets_open.begin(); it_meshsets != meshsets_open.end(); ++it_meshsets )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = (*it_meshsets);
		copy_item->meshsets.push_back( shared_ptr<carve::mesh::MeshSet<3> >( item_meshset->clone() ) );
	}

	for( auto it_meshsets = meshsets.begin(); it_meshsets != meshsets.end(); ++it_meshsets )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = (*it_meshsets);
		copy_item->meshsets.push_back( shared_ptr<carve::mesh::MeshSet<3> >( item_meshset->clone() ) );
	}

	for( size_t polyline_i = 0; polyline_i < polylines.size(); ++polyline_i )
	{
		shared_ptr<carve::input::PolylineSetData>& polyline_data = polylines[polyline_i];
		copy_item->polylines.push_back( shared_ptr<carve::input::PolylineSetData>( new carve::input::PolylineSetData( *(polyline_data.get()) ) ) );
	}

	std::copy( vec_item_appearances.begin(),	vec_item_appearances.end(),		std::back_inserter( copy_item->vec_item_appearances ) );
	std::copy( vec_text_literals.begin(),		vec_text_literals.end(),		std::back_inserter( copy_item->vec_text_literals ) );

	return copy_item;
}

void ShapeInputData::addInputData( shared_ptr<ShapeInputData>& other )
{
	std::copy( other->vec_item_data.begin(), other->vec_item_data.end(), std::back_inserter( vec_item_data ) );
	std::copy( other->getAppearances().begin(), other->getAppearances().end(), std::back_inserter( vec_appearances ) );
}

void ShapeInputData::deepCopyFrom( shared_ptr<ShapeInputData>& other )
{
	vec_item_data.clear();
	vec_appearances.clear();

	for( int item_i = 0; item_i < other->vec_item_data.size(); ++item_i )
	{
		shared_ptr<ItemData>& item_data = other->vec_item_data[item_i];
		vec_item_data.push_back( shared_ptr<ItemData>( item_data->getDeepCopy() ) );
	}
	std::copy( other->vec_appearances.begin(), other->vec_appearances.end(), std::back_inserter( vec_appearances ) );
}

void ShapeInputData::addAppearance( shared_ptr<AppearanceData>& appearance )
{
	if( !appearance )
	{
		return;
	}
	int append_id = appearance->m_step_stype_id;
	for( size_t ii = 0; ii < vec_appearances.size(); ++ii )
	{
		shared_ptr<AppearanceData>& appearance = vec_appearances[ii];
		if( appearance->m_step_stype_id == append_id )
		{
			return;
		}
	}
	vec_appearances.push_back( appearance );
}

void ShapeInputData::clearAppearanceData()
{
	vec_appearances.clear();
}

void ShapeInputData::clearAll()
{
	vec_appearances.clear();

	ifc_product.reset();
	representation.reset();
	object_placement.reset();
	if( product_switch )
	{
		product_switch->removeChildren( 0, product_switch->getNumChildren() );
	}
	if( product_switch_curves )
	{
		product_switch_curves->removeChildren( 0, product_switch_curves->getNumChildren() );
	}
	vec_item_data.clear();
	added_to_storey = false;
	representation_identifier = L"";
	representation_type = L"";
}

// PolyInputCache
PolyInputCache3D::PolyInputCache3D()
{
	m_poly_data = shared_ptr<carve::input::PolyhedronData>( new carve::input::PolyhedronData() );
}

int PolyInputCache3D::addPointPrecise( const carve::geom::vector<3>& v )
{
	const double vertex_x = v.x;
	const double vertex_y = v.y;
	const double vertex_z = v.z;

	//  return a pair, with its member pair::first set to an iterator pointing to either the newly inserted element or to the element with an equivalent key in the map
	std::map<double, std::map<double, int> >& map_y_index = existing_vertices_coords.insert( std::make_pair(vertex_x, std::map<double, std::map<double, int> >() ) ).first->second;
	std::map<double, int>& map_z_index = map_y_index.insert( std::make_pair( vertex_y, std::map<double, int>() ) ).first->second;

	it_find_z = map_z_index.find( vertex_z );
	if( it_find_z != map_z_index.end() )
	{
		// vertex already exists in polyhedron. return its index
		int vertex_index = it_find_z->second;
		return vertex_index;
	}
	else
	{
		// add point to polyhedron
		int vertex_index = m_poly_data->addVertex( v );
		map_z_index[vertex_z] = vertex_index;
		return vertex_index;
	}
}

#define ROUND_POLY_COORDINATES_UP 1000000.0
#define ROUND_POLY_COORDINATES_DOWN 0.000001
int PolyInputCache3D::addPoint( const carve::geom::vector<3>& v )
{
	const double vertex_x = round(v.x*ROUND_POLY_COORDINATES_UP)*ROUND_POLY_COORDINATES_DOWN;
	const double vertex_y = round(v.y*ROUND_POLY_COORDINATES_UP)*ROUND_POLY_COORDINATES_DOWN;
	const double vertex_z = round(v.z*ROUND_POLY_COORDINATES_UP)*ROUND_POLY_COORDINATES_DOWN;

	//  return a pair, with its member pair::first set to an iterator pointing to either the newly inserted element or to the element with an equivalent key in the map
	std::map<double, std::map<double, int> >& map_y_index = existing_vertices_coords.insert( std::make_pair(vertex_x, std::map<double, std::map<double, int> >() ) ).first->second;
	std::map<double, int>& map_z_index = map_y_index.insert( std::make_pair( vertex_y, std::map<double, int>() ) ).first->second;

	it_find_z = map_z_index.find( vertex_z );
	if( it_find_z != map_z_index.end() )
	{
		// vertex already exists in polyhedron. return its index
		int vertex_index = it_find_z->second;
		return vertex_index;
	}
	else
	{
		// add point to polyhedron
		int vertex_index = m_poly_data->addVertex( v );
		map_z_index[vertex_z] = vertex_index;
		return vertex_index;
	}
}

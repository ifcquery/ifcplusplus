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

#include <ifcpp/model/IfcPPException.h>
#include "GeomUtils.h"
#include "GeometrySettings.h"
#include "CSG_Adapter.h"
#include "Sweeper.h"

Sweeper::Sweeper(shared_ptr<GeometrySettings>& settings, shared_ptr<UnitConverter>& uc) : m_geom_settings(settings), m_unit_converter(uc)
{
}

Sweeper::~Sweeper()
{
}

void Sweeper::extrude( const std::vector<std::vector<carve::geom::vector<2> > >& face_loops_input, const carve::geom::vector<3> extrusion_vector, IfcPPEntity* ifc_entity, shared_ptr<ItemShapeInputData>& item_data )
{
	if( face_loops_input.size() == 0 )
	{
		messageCallback( "face_loops_input.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		return;
	}
	if( !item_data )
	{
		messageCallback( "!item_data", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		return;
	}

	// figure 1: loops and indexes
	//  3----------------------------2
	//  |                            |
	//  |   1-------------------2    |3---------2
	//  |   |                   |    |          |
	//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
	//  |   0---face_loops[1]---3    |0---------1
	//  |                            |
	//  0-------face_loops[0]--------1

	carve::geom::vector<3> normal_first_loop;
	std::vector<std::vector<carve::geom2d::P2> >	face_loops;
	bool warning_small_loop_detected = false;
	for( std::vector<std::vector<carve::geom::vector<2> > >::const_iterator it_face_loops = face_loops_input.begin(); it_face_loops != face_loops_input.end(); ++it_face_loops )
	{
		const std::vector<carve::geom::vector<2> >& loop = (*it_face_loops);

		if( loop.size() == 0 )
		{
			continue;
		}
		else if( loop.size() == 1 )
		{
			// Cross section is just a point. Create a line along the extrusion vector
			shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
			if( !polyline_data )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			polyline_data->beginPolyline();
			polyline_data->addVertex( carve::geom::vector<3>( carve::geom::VECTOR( 0, 0, 0 ) ) );
			polyline_data->addVertex( extrusion_vector );
			polyline_data->addPolylineIndex( 0 );
			polyline_data->addPolylineIndex( 1 );
			item_data->m_polylines.push_back( polyline_data );
		}
		else if( loop.size() == 2 )
		{
			// Cross section is just a line
			messageCallback( "Unhandled line as cross section", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		}

		// check winding order
		std::vector<carve::geom2d::P2> loop_2d( loop );
		carve::geom::vector<3>  normal_2d = GeomUtils::computePolygon2DNormal( loop_2d );
		if( it_face_loops == face_loops_input.begin() )
		{
			normal_first_loop = normal_2d;
			if( normal_2d.z < 0 )
			{
				// reverse loop:
				std::reverse( loop_2d.begin(), loop_2d.end() );
				normal_first_loop = -normal_first_loop;
			}
		}
		else
		{
			if( normal_2d.z > 0 )
			{
				// reverse loop:
				std::reverse( loop_2d.begin(), loop_2d.end() );
			}
		}
				
		if( loop_2d.size() < 3 )
		{
			messageCallback( "loop_2d.size() < 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		}
		
		// close loop, insert first point at end if not already there
		carve::geom::vector<2>& first = loop_2d.front();
		carve::geom::vector<2>& last = loop_2d.back();

		if( std::abs(first.x-last.x) > 0.00001 || std::abs(first.y-last.y) > 0.00001 )
		{
			loop_2d.push_back( first );
		}

		double signed_area = carve::geom2d::signedArea( loop_2d );
		if( std::abs( signed_area ) < 1.e-10 )// m_geom_settings->m_min_face_area )
		{
			warning_small_loop_detected = true;
			continue;
		}

		face_loops.push_back(loop_2d);
	}

	if( warning_small_loop_detected )
	{
		std::stringstream err;
		err << "abs( signed_area ) < 1.e-10";
		messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, ifc_entity );
	}
	
	if( face_loops.size() == 0 )
	{
		return;
	}

	bool flip_faces = false;
	double extrusion_dot_normal = dot( extrusion_vector, normal_first_loop );
	if( extrusion_dot_normal > 0 )
	{
		flip_faces = true;
	}

	// triangulate
	std::vector<carve::geom2d::P2> merged_path;
	std::vector<carve::triangulate::tri_idx> triangulated;
	std::vector<std::pair<size_t, size_t> > path_all_loops;
	try
	{
		path_all_loops = carve::triangulate::incorporateHolesIntoPolygon(face_loops);
		// figure 2: path wich incorporates holes, described by path_all_loops
		// (0/0) -> (1/3) -> (1/0) -> (1/1) -> (1/2) -> (1/3) -> (0/0) -> (0/1) -> (0/2) -> (0/3)
		//  0/3<-----------------------0/2
		//  |                            ^
		//  |   1/0-------------->1/1    |
		//  |   ^                   |    |
		//  |   |                   v    |
		//  |   1/3<--------------1/2    |
		//  v                            |
		//  0/0------------------------>0/1

		merged_path.reserve(path_all_loops.size());
		for( size_t i = 0; i < path_all_loops.size(); ++i )
		{
			size_t loop_number = path_all_loops[i].first;
			size_t index_in_loop = path_all_loops[i].second;
			
			if( loop_number >= face_loops.size() )
			{
				messageCallback( "loop_number >= face_loops.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				continue;
			}
			std::vector<carve::geom2d::P2>& loop = face_loops[loop_number];
			carve::geom2d::P2& point_in_loop = loop[index_in_loop];
			merged_path.push_back( point_in_loop );

		}
		// figure 3: merged path for triangulation
		//  9<---------------------------8
		//  |                            ^
		//  |   2------------------>3    |
		//  |   ^                   |    |
		//  |   |                   v    |
		//  |   1, 5<---------------4    |
		//  | /                          |
		//  0,6------------------------->7
		carve::triangulate::triangulate(merged_path, triangulated);
		carve::triangulate::improve(merged_path, triangulated);
		// triangles: (9,0,1)  (5,6,7)  (4,5,7)  (4,7,8)  (9,1,2)  (8,9,2)  (3,4,8)  (2,3,8)
	}
	catch(...)
	{
		messageCallback( "carve::triangulate::incorporateHolesIntoPolygon failed ", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
#ifdef _DEBUG

		carve::input::PolylineSetData poly_line;
		poly_line.beginPolyline();
		int pline_index = 0;
		for( size_t ii = 0; ii < face_loops.size(); ++ii )
		{
			std::vector<carve::geom2d::P2>& face_loop_i = face_loops[ii];

			for( size_t jj = 0; jj < face_loop_i.size(); ++jj )
			{
				carve::geom2d::P2& point = face_loop_i[jj];
				poly_line.addVertex( carve::geom::VECTOR( point.x, point.y, 0 ) );
				poly_line.addPolylineIndex( pline_index++ );
			}
		}
		CSG_Adapter::dumpPolylineSet( &poly_line, carve::geom::VECTOR( 0.2, 0.6, 0.2, 1.0 ), true );
#endif
		return;
	}

	shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData() );
	if( !poly_data )
	{
		throw IfcPPOutOfMemoryException( __FUNC__ );
	}

	// now insert points to polygon, avoiding points with same coordinates
	std::map<double, std::map<double, unsigned int> > existing_vertices_coords;
	std::map<double, unsigned int>::iterator it_find_y;

	std::map<unsigned int,unsigned int> map_merged_idx;
	for( size_t i = 0; i < merged_path.size(); ++i )
	{
		const carve::geom::vector<2>&  v = merged_path[i];
		
#ifdef ROUND_IFC_COORDINATES
		const double vertex_x = round(v.x*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		const double vertex_y = round(v.y*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		const double vertex_x = v.x;
		const double vertex_y = v.y;
#endif

		//  return a pair, with its member pair::first set to an iterator pointing to either the newly inserted element or to the element with an equivalent key in the map
		std::map<double, unsigned int>& map_y_index = existing_vertices_coords.insert( std::make_pair(vertex_x, std::map<double,unsigned int>() ) ).first->second;

		it_find_y = map_y_index.find( vertex_y );
		if( it_find_y != map_y_index.end() )
		{
			// vertex already exists in polygon. remember its index for triangles
			map_merged_idx[i] = it_find_y->second;
			continue;
		}

		carve::geom::vector<3>  vertex3D( carve::geom::VECTOR( v.x, v.y, 0 ) );
		size_t vertex_id = poly_data->addVertex(vertex3D);
		map_y_index[vertex_y] = vertex_id;  // TODO: it works for now, but check if we have to round here. maybe use checksum of rounded x and y as a single map key
		map_merged_idx[i] = vertex_id;
	}

	// figure 4: points in poly_data (merged path without duplicate vertices):
	//  7<---------------------------6
	//  |                            ^
	//  |   2------------------>3    |
	//  |   ^                   |    |     map_merged_idx: figure 3 -> figure 4
	//  |   |                   v    |
	//  |   1<------------------4    |
	//  | /                          |
	//  0--------------------------->5

	// copy points from base to top
	std::vector<carve::geom::vector<3> >& points = poly_data->points;
	const size_t num_points_base = points.size();
	for( size_t i=0; i<num_points_base; ++i )
	{
		carve::geom::vector<3>& pt = points[i];
		poly_data->addVertex( pt + extrusion_vector );
	}

	// collect vertex indexes of loops
	std::map<size_t, std::vector<size_t> > loop_vert_idx;
	for( size_t merged_idx = 0; merged_idx < path_all_loops.size(); ++merged_idx )
	{
		int loop_number = path_all_loops[merged_idx].first;
		int point_idx_merged = map_merged_idx[merged_idx];
	
		std::map<size_t, std::vector<size_t> >::iterator it_result_loops = loop_vert_idx.insert( std::make_pair( loop_number, std::vector<size_t>() ) ).first;
		std::vector<size_t>& result_loop_vec = it_result_loops->second;
		
		// check if point index is already in loop
		bool already_in_loop = false;
		for( size_t i2 = 0; i2 < result_loop_vec.size(); ++i2 )
		{
			if( point_idx_merged == result_loop_vec[i2] )
			{
				already_in_loop = true;
				break;
			}
		}
		if( !already_in_loop )
		{
			result_loop_vec.push_back( point_idx_merged );
		}
	}

	// add faces along outer and inner loops
	for( std::map<size_t, std::vector<size_t> >::iterator it_result_loop = loop_vert_idx.begin(); it_result_loop != loop_vert_idx.end(); ++it_result_loop )
	{
		const std::vector<size_t>& loop_idx = it_result_loop->second;
		const size_t num_points_in_loop = loop_idx.size();
		
		for( size_t i=0; i<num_points_in_loop; ++i )
		{
			size_t point_idx		= loop_idx[i];
			size_t point_idx_next	= loop_idx[(i+1)%num_points_in_loop];
			size_t point_idx_up = point_idx + num_points_base;
			size_t point_idx_next_up = point_idx_next + num_points_base;

			if( point_idx_next_up >= 2*num_points_base )
			{
				messageCallback( "point_idx_next_up >= 2*num_points_base", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				continue;
			}
			if( flip_faces )
			{
				poly_data->addFace( point_idx,			point_idx_next,		point_idx_next_up );
				poly_data->addFace( point_idx_next_up,	point_idx_up,		point_idx );
			}
			else
			{
				poly_data->addFace( point_idx,			point_idx_up,		point_idx_next_up );
				poly_data->addFace( point_idx_next_up,	point_idx_next,		point_idx );
			}
		}
	}

	// now the triangulated bottom and top cap
	for( size_t i = 0; i != triangulated.size(); ++i )
	{
		carve::triangulate::tri_idx triangle = triangulated[i];
		unsigned int a = triangle.a;
		unsigned int b = triangle.b;
		unsigned int c = triangle.c;

		unsigned int vertex_id_a = map_merged_idx[a];
		unsigned int vertex_id_b = map_merged_idx[b];
		unsigned int vertex_id_c = map_merged_idx[c];

		if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
		{
			continue;
		}

		int vertex_id_a_top = vertex_id_a + num_points_base;
		int vertex_id_b_top = vertex_id_b + num_points_base;
		int vertex_id_c_top = vertex_id_c + num_points_base;

#ifdef _DEBUG
		const carve::poly::Vertex<3>& v_a = poly_data->getVertex(vertex_id_a);
		const carve::poly::Vertex<3>& v_b = poly_data->getVertex(vertex_id_b);
		const carve::poly::Vertex<3>& v_c = poly_data->getVertex(vertex_id_c);

		carve::geom::vector<3> pa( carve::geom::VECTOR( v_a.v[0],	v_a.v[1],	v_a.v[2] ) );
		carve::geom::vector<3> pb( carve::geom::VECTOR( v_b.v[0],	v_b.v[1],	v_b.v[2] ) );
		carve::geom::vector<3> pc( carve::geom::VECTOR( v_c.v[0],	v_c.v[1],	v_c.v[2] ) );

		double A = 0.5*(cross( pa-pb, pa-pc ).length());
		if( std::abs(A) < 0.000000001 )
		{
			std::cout <<  __FUNC__ << ": area < 0.000000001\n" << std::endl;
		}
#endif
				
		if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
		{
			continue;
		}

		if( flip_faces )
		{
			poly_data->addFace( vertex_id_a,		vertex_id_c,		vertex_id_b );		// bottom cap
			poly_data->addFace( vertex_id_a_top,	vertex_id_b_top,	vertex_id_c_top );	// top cap, flipped outward
		}
		else
		{
			poly_data->addFace( vertex_id_a,		vertex_id_b,		vertex_id_c );		// bottom cap
			poly_data->addFace( vertex_id_a_top,	vertex_id_c_top,	vertex_id_b_top );	// top cap, flipped outward
		}
	}

	try
	{
		item_data->addClosedPolyhedron( poly_data );
	}
	catch( IfcPPException& exception )
	{
		messageCallback( exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity );  // calling function already in e.what()
	}


#ifdef _DEBUG
	shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh(carve::input::opts()) );
	std::stringstream strs_err_meshset;
	int entity_id = -1;
	if( ifc_entity )
	{
		entity_id = ifc_entity->m_id;
	}
	bool poly_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset, strs_err_meshset, entity_id );

	if( !poly_ok )
	{
		std::cout << strs_err_meshset.str().c_str() << std::endl;
		CSG_Adapter::dumpMeshset( meshset.get(), carve::geom::VECTOR( 0.3, 0.4, 0.5, 1.0 ), true );
	}
#endif
}

void Sweeper::sweepDisk( const std::vector<carve::geom::vector<3> >& curve_points, IfcPPEntity* ifc_entity, shared_ptr<ItemShapeInputData>& item_data, const size_t nvc, const double radius, const double radius_inner )
{
	carve::geom::vector<3> local_z( carve::geom::VECTOR( 0, 0, 1 ) );
	const size_t num_curve_points = curve_points.size();
	if( num_curve_points < 2 )
	{
		messageCallback( "num curve points < 2", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		return;
	}

	if( !item_data )
	{
		messageCallback( "!item_data", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		return;
	}

	if( radius < 0.001 )
	{
		// Cross section is just a point. Create a polyline
		shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		if( !polyline_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		polyline_data->beginPolyline();
		for( size_t i_polyline = 0; i_polyline < curve_points.size(); ++i_polyline )
		{
			const carve::geom::vector<3>& curve_pt = curve_points[i_polyline];
			polyline_data->addVertex( curve_pt );
			polyline_data->addPolylineIndex( 0 );
			polyline_data->addPolylineIndex( i_polyline );
		}
		item_data->m_polylines.push_back( polyline_data );
		return;
	}

	double use_radius_inner = radius_inner;
	if( radius_inner > radius )
	{
		messageCallback( "radius_inner > radius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		use_radius_inner = radius;
	}

	carve::geom::vector<3>  curve_point_first = curve_points[0];
	carve::geom::vector<3>  curve_point_second = curve_points[1];

	bool bend_found = false;
	if( num_curve_points > 3 )
	{
		// compute local z vector by dot product of the first bend of the reference line
		carve::geom::vector<3> vertex_back2 = curve_point_first;
		carve::geom::vector<3> vertex_back1 = curve_point_second;
		for( size_t i = 2; i<num_curve_points; ++i )
		{
			const carve::geom::vector<3>& vertex_current = curve_points[i];
			carve::geom::vector<3> section1 = vertex_back1 - vertex_back2;
			carve::geom::vector<3> section2 = vertex_current - vertex_back1;
			section1.normalize();
			section2.normalize();

			double dot_product = dot( section1, section2 );
			double dot_product_abs = std::abs(dot_product);

			// if dot == 1 or -1, then points are colinear
			if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
			{
				// bend found, compute cross product
				carve::geom::vector<3> lateral_vec = cross( section1, section2 );
				local_z = cross( lateral_vec, section1 );
				local_z.normalize();
				bend_found = true;
				break;
			}
		}
	}

	if( !bend_found )
	{
		// sweeping curve is linear. assume any local z vector
		carve::geom::vector<3> sweep_dir = curve_point_second - curve_point_first;
		sweep_dir.normalize();
		local_z = cross( carve::geom::VECTOR( 0, 0, 1 ), sweep_dir );
		if( local_z.length2() < 0.001 )
		{
			local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
			local_z.normalize();
		}
		else
		{
			local_z.normalize();
		}
		double dot_normal_local_z = dot( sweep_dir, local_z );
		if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
		{
			local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
			local_z.normalize();

			dot_normal_local_z = dot( sweep_dir, local_z );
			if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
			{
				local_z = cross( carve::geom::VECTOR( 1, 0, 0 ), sweep_dir );
				local_z.normalize();
			}
		}
	}

	// rotate disk into first direction
	carve::geom::vector<3>  section_local_y = local_z;
	carve::geom::vector<3>  section_local_z = curve_point_first - curve_point_second;
	carve::geom::vector<3>  section_local_x = carve::geom::cross( section_local_y, section_local_z );
	section_local_y = carve::geom::cross( section_local_x, section_local_z );
	std::vector<carve::geom::vector<3> > inner_shape_points;

	
	section_local_x.normalize();
	section_local_y.normalize();
	section_local_z.normalize();

	carve::math::Matrix matrix_first_direction = carve::math::Matrix(
		section_local_x.x,		section_local_y.x,		section_local_z.x,	0,
		section_local_x.y,		section_local_y.y,		section_local_z.y,	0,
		section_local_x.z,		section_local_y.z,		section_local_z.z,	0,
		0,				0,				0,			1 );

	double angle = 0;
	double delta_angle = 2.0*M_PI/double(nvc);	// TODO: adapt to model size and complexity
	std::vector<carve::geom::vector<3> > circle_points;
	std::vector<carve::geom::vector<3> > circle_points_inner;
	for( size_t i = 0; i < nvc; ++i )
	{
		// cross section (circle) is defined in XY plane
		double x = sin(angle);
		double y = cos(angle);
		carve::geom::vector<3> vertex( carve::geom::VECTOR( x*radius, y*radius, 0.0 ) );
		vertex = matrix_first_direction*vertex + curve_point_first;
		circle_points.push_back( vertex );

		if( use_radius_inner > 0 )
		{
			carve::geom::vector<3> vertex_inner( carve::geom::VECTOR( x*use_radius_inner, y*use_radius_inner, 0.0 ) );
			vertex_inner = matrix_first_direction*vertex_inner + curve_point_first;
			circle_points_inner.push_back( vertex_inner );
		}
		angle += delta_angle;
	}

	shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData() );
	if( !poly_data )
	{
		throw IfcPPOutOfMemoryException( __FUNC__ );
	}

	for( size_t ii = 0; ii<num_curve_points; ++ii )
	{
		const carve::geom::vector<3>& vertex_current = curve_points[ii];
		carve::geom::vector<3> vertex_next;
		carve::geom::vector<3> vertex_before;
		if( ii == 0 )
		{
			// first point
			vertex_next	= curve_points[ii+1];
			carve::geom::vector<3> delta_element = vertex_next - vertex_current;
			vertex_before = vertex_current - (delta_element);
		}
		else if( ii == num_curve_points-1 )
		{
			// last point
			vertex_before	= curve_points[ii-1];
			carve::geom::vector<3> delta_element = vertex_current - vertex_before;
			vertex_next = vertex_before + (delta_element);
		}
		else
		{
			// inner point
			vertex_next		= curve_points[ii+1];
			vertex_before	= curve_points[ii-1];
		}

		carve::geom::vector<3> bisecting_normal;
		GeomUtils::bisectingPlane( vertex_before, vertex_current, vertex_next, bisecting_normal );

		carve::geom::vector<3> section1 = vertex_current - vertex_before;
		carve::geom::vector<3> section2 = vertex_next - vertex_current;
		section1.normalize();
		section2.normalize();
		double dot_product = dot( section1, section2 );
		double dot_product_abs = std::abs(dot_product);

		if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
		{
			// bend found, compute next local z vector
			carve::geom::vector<3> lateral_vec = cross( section1, section2 );
			local_z = cross( lateral_vec, section1 );
			local_z.normalize();
		}
		if( ii == num_curve_points -1 )
		{
			bisecting_normal *= -1.0;
		}

		carve::geom::plane<3> bisecting_plane( bisecting_normal, vertex_current );
		for( size_t jj = 0; jj < nvc; ++jj )
		{
			carve::geom::vector<3>& vertex = circle_points[jj];

			carve::geom::vector<3> v;
			double t;
			carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t);
			if( intersect > 0 )
			{
				vertex = v;
			}
			else
			{
				messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			}

			poly_data->addVertex( vertex );
		}

		if( use_radius_inner > 0 )
		{
			for( size_t jj = 0; jj < nvc; ++jj )
			{
				carve::geom::vector<3>& vertex = circle_points_inner[jj];
					
				carve::geom::vector<3> v;
				double t;
				carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t);
				if( intersect > 0 )
				{
					vertex = v;
				}
				else
				{
					messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}

				inner_shape_points.push_back( vertex );
			}
		}
	}

	// outer shape
	size_t num_vertices_outer = poly_data->getVertexCount();
	for( size_t i=0; i<num_curve_points- 1; ++i )
	{
		size_t i_offset = i*nvc;
		size_t i_offset_next = ( i + 1 )*nvc;
		for( size_t jj = 0; jj < nvc; ++jj )
		{
			size_t current_loop_pt1 = jj + i_offset;
			size_t current_loop_pt2 = ( jj + 1 ) % nvc + i_offset;

			size_t next_loop_pt1 = jj + i_offset_next;
			size_t next_loop_pt2 = ( jj + 1 ) % nvc + i_offset_next;
			poly_data->addFace( current_loop_pt1,	next_loop_pt1,		next_loop_pt2 );
			poly_data->addFace( next_loop_pt2,		current_loop_pt2,	current_loop_pt1  );
		}
	}

	if( use_radius_inner > 0 )
	{
		if( inner_shape_points.size() != num_vertices_outer )
		{
			messageCallback( "inner_shape_points.size() != num_vertices_outer", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		}

		// add points for inner shape
		for( size_t i=0; i<inner_shape_points.size(); ++i )
		{
			poly_data->addVertex( inner_shape_points[i] );
		}

		// faces of inner shape
		for( size_t i=0; i<num_curve_points- 1; ++i )
		{
			size_t i_offset = i*nvc + num_vertices_outer;
			size_t i_offset_next = ( i + 1 )*nvc + num_vertices_outer;
			for( size_t jj = 0; jj < nvc; ++jj )
			{
				size_t current_loop_pt1 = jj + i_offset;
				size_t current_loop_pt2 = ( jj + 1 ) % nvc + i_offset;

				size_t next_loop_pt1 = jj + i_offset_next;
				size_t next_loop_pt2 = ( jj + 1 ) % nvc + i_offset_next;
				poly_data->addFace( current_loop_pt1,	current_loop_pt2,	next_loop_pt2 );
				poly_data->addFace( next_loop_pt2,		next_loop_pt1,		current_loop_pt1  );
			}
		}

		// front cap
		for( size_t jj = 0; jj < nvc; ++jj )
		{
			size_t outer_rim_next = ( jj + 1 ) % nvc;
			size_t inner_rim_next = outer_rim_next + num_vertices_outer;
			poly_data->addFace( jj,					outer_rim_next,		num_vertices_outer+jj );
			poly_data->addFace( outer_rim_next,		inner_rim_next,		num_vertices_outer+jj );
		}

		// back cap
		size_t back_offset = ( num_curve_points - 1 )*nvc;
		for( size_t jj = 0; jj < nvc; ++jj )
		{
			size_t outer_rim_next = ( jj + 1 ) % nvc + back_offset;
			size_t inner_rim_next = outer_rim_next + num_vertices_outer;
			poly_data->addFace( jj+back_offset,		num_vertices_outer+jj+back_offset,	outer_rim_next );
			poly_data->addFace( outer_rim_next,		num_vertices_outer+jj+back_offset,	inner_rim_next );
		}
	}
	else
	{
		// front cap, full pipe, create triangle fan
		for( size_t jj = 0; jj < nvc - 2; ++jj )
		{
			poly_data->addFace( 0, jj+1, jj+2 );
		}

		// back cap
		size_t back_offset = ( num_curve_points - 1 )*nvc;
		for( size_t jj = 0; jj < nvc - 2; ++jj )
		{
			poly_data->addFace( back_offset, back_offset+jj+2, back_offset+jj+1 );
		}
	}

	try
	{
		item_data->addClosedPolyhedron( poly_data );
	}
	catch( IfcPPException& exception )
	{
		messageCallback( exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity );  // calling function already in e.what()
	}

#ifdef _DEBUG
	std::stringstream strs_err;
	shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh(carve::input::opts()) );
	bool poly_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset, strs_err, -1 );

	if( !poly_ok )
	{
		std::cout << strs_err.str().c_str() << std::endl;
	}
#endif
}

void Sweeper::sweepArea( const std::vector<carve::geom::vector<3> >& curve_points, const std::vector<std::vector<carve::geom::vector<2> > >& profile_paths, IfcPPEntity* ifc_entity, shared_ptr<ItemShapeInputData>& item_data )
{
	// TODO: complete and test
	if( profile_paths.size() == 0 )
	{
		messageCallback( "profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		return;
	}

	// figure 1: loops and indexes
	//  3----------------------------2
	//  |                            |
	//  |   1-------------------2    |3---------2
	//  |   |                   |    |          |
	//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
	//  |   0---face_loops[1]---3    |0---------1
	//  |                            |
	//  0-------face_loops[0]--------1

	carve::geom::vector<3> normal_first_loop;
	std::vector<std::vector<carve::geom2d::P2> > face_loops;
	bool warning_small_loop_detected = false;
	bool polyline_created = false;
	for( size_t i_face_loops = 0; i_face_loops < profile_paths.size(); ++i_face_loops )
	{
		const std::vector<carve::geom::vector<2> >& loop = profile_paths[i_face_loops];

		if( loop.size() < 3 )
		{
			if( loop.size() == 1 )
			{
				// Cross section is just a point. Create a polyline
				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				if( !polyline_data )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}
				polyline_data->beginPolyline();
				for( size_t i_polyline = 0; i_polyline < curve_points.size(); ++i_polyline )
				{
					const carve::geom::vector<3>& curve_pt = curve_points[i_polyline];
					polyline_data->addVertex( curve_pt );
					polyline_data->addPolylineIndex( 0 );
					polyline_data->addPolylineIndex( i_polyline );
				}
				item_data->m_polylines.push_back( polyline_data );
				polyline_created = true;
			}
			continue;
		}

		// check winding order
		bool reverse_loop = false;
		std::vector<carve::geom2d::P2> loop_2d( loop );
		carve::geom::vector<3>  normal_2d = GeomUtils::computePolygon2DNormal( loop_2d );
		if( i_face_loops == 0 )
		{
			normal_first_loop = normal_2d;
			if( normal_2d.z < 0 )
			{
				reverse_loop = true;
				normal_first_loop = -normal_first_loop;
			}
		}
		else
		{
			if( normal_2d.z > 0 )
			{
				reverse_loop = true;
			}
		}
		if( reverse_loop )
		{
			std::reverse( loop_2d.begin(), loop_2d.end() );
		}
				
		if( loop_2d.size() < 3 )
		{
			messageCallback( "loop_2d.size() < 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		}
		
		// close loop, insert first point at end if not already there
//		while( loop_2d.size() > 2 )
		{
			carve::geom::vector<2> first = loop_2d.front();
			carve::geom::vector<2>& last = loop_2d.back();

			if( std::abs(first.x-last.x) > 0.00001 || std::abs(first.y-last.y) > 0.00001 )
			{
				loop_2d.push_back( first );
			}
		}

		double signed_area = carve::geom2d::signedArea( loop_2d );
		double min_loop_area = 1.e-10;//m_geom_settings->m_min_face_area
		if( std::abs( signed_area ) < min_loop_area )
		{
			warning_small_loop_detected = true;
			continue;
		}

		face_loops.push_back(loop_2d);
	}

	if( warning_small_loop_detected )
	{
		messageCallback( "abs( signed_area ) < 1.e-10", StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, ifc_entity );
	}

	if( face_loops.size() == 0 )
	{
		if( polyline_created )
		{
			// already handled as curve
			return;
		}
		messageCallback( "face_loops.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		return;
	}

	// triangulate
	std::vector<carve::geom2d::P2> merged_path;
	std::vector<carve::triangulate::tri_idx> triangulated;
	std::vector<std::pair<size_t, size_t> > path_incorporated_holes;
	try
	{
		path_incorporated_holes = carve::triangulate::incorporateHolesIntoPolygon(face_loops);
		// figure 2: path wich incorporates holes, described by path_all_loops
		// (0/0) -> (1/3) -> (1/0) -> (1/1) -> (1/2) -> (1/3) -> (0/0) -> (0/1) -> (0/2) -> (0/3)
		//  0/3<-----------------------0/2
		//  |                            ^
		//  |   1/1-------------->1/2    |
		//  |   ^                   |    |
		//  |   |                   v    |  path_incorporated_holes
		//  |   1/0<--------------1/3    |
		//  v                            |
		//  0/0------------------------>0/1

		merged_path.reserve(path_incorporated_holes.size());
		for( size_t i = 0; i < path_incorporated_holes.size(); ++i )
		{
			size_t loop_number = path_incorporated_holes[i].first;
			size_t index_in_loop = path_incorporated_holes[i].second;
			
			if( loop_number >= face_loops.size() )
			{
				messageCallback( "loop_number >= face_loops.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				continue;
			}
			std::vector<carve::geom2d::P2>& loop = face_loops[loop_number];

			if( index_in_loop >= loop.size() )
			{
				messageCallback( "index_in_loop >= loop.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				continue;
			}
			carve::geom2d::P2& point_in_loop = loop[index_in_loop];
			merged_path.push_back( point_in_loop );
		}
		// figure 3: merged path for triangulation
		//  9<--------------------------8
		//  |                           ^
		//  |  2------------------>3    |
		//  |  ^                   |    |
		//  |  |                   v    |
		//  |  1  5<---------------4    |
		//  | /  /                      |
		//  0  6----------------------->7
		carve::triangulate::triangulate(merged_path, triangulated);
		carve::triangulate::improve(merged_path, triangulated);
		// triangles: (9,0,1)  (5,6,7)  (4,5,7)  (4,7,8)  (9,1,2)  (8,9,2)  (3,4,8)  (2,3,8)
	}
	catch(...)
	{
		messageCallback( "carve::triangulate::incorporateHolesIntoPolygon failed", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		return;
	}

	carve::geom::vector<3> local_z( carve::geom::VECTOR( 0, 0, 1 ) );
	const size_t num_curve_points = curve_points.size();
	if( num_curve_points < 2 )
	{
		messageCallback( "num curve points < 2", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		return;
	}

	const carve::geom::vector<3>& curve_point_first = curve_points[0];
	const carve::geom::vector<3>& curve_point_second = curve_points[1];

	bool bend_found = false;
	if( num_curve_points > 3 )
	{
		// compute local z vector by dot product of the first bend of the reference line
		const carve::geom::vector<3>& vertex_back2 = curve_point_first;
		const carve::geom::vector<3>& vertex_back1 = curve_point_second;
		for( size_t i = 2; i<num_curve_points; ++i )
		{
			const carve::geom::vector<3>& vertex_current = curve_points[i];
			carve::geom::vector<3> section1 = vertex_back1 - vertex_back2;
			carve::geom::vector<3> section2 = vertex_current - vertex_back1;
			section1.normalize();
			section2.normalize();

			double dot_product = dot( section1, section2 );
			double dot_product_abs = std::abs(dot_product);

			// if dot == 1 or -1, then points are colinear
			if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
			{
				// bend found, compute cross product
				carve::geom::vector<3> lateral_vec = cross( section1, section2 );
				local_z = cross( lateral_vec, section1 );
				local_z.normalize();
				bend_found = true;
				break;
			}
		}
	}

	if( !bend_found )
	{
		// sweeping curve is linear. assume any local z vector
		carve::geom::vector<3> sweep_dir = curve_point_second - curve_point_first;
		sweep_dir.normalize();
		local_z = cross( carve::geom::VECTOR( 0, 0, 1 ), sweep_dir );
		if( local_z.length2() < 0.001 )
		{
			local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
			local_z.normalize();
		}
		else
		{
			local_z.normalize();
		}
		double dot_normal_local_z = dot( sweep_dir, local_z );
		if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
		{
			local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
			local_z.normalize();

			dot_normal_local_z = dot( sweep_dir, local_z );
			if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
			{
				local_z = cross( carve::geom::VECTOR( 1, 0, 0 ), sweep_dir );
				local_z.normalize();
			}
		}
	}

	// rotate disk into first direction
	carve::geom::vector<3>  section_local_y = local_z;
	carve::geom::vector<3>  section_local_z = curve_point_first - curve_point_second;
	carve::geom::vector<3>  section_local_x = carve::geom::cross( section_local_y, section_local_z );
	section_local_y = carve::geom::cross( section_local_x, section_local_z );
	std::vector<carve::geom::vector<3> > inner_shape_points;

	section_local_x.normalize();
	section_local_y.normalize();
	section_local_z.normalize();

	carve::math::Matrix matrix_first_direction = carve::math::Matrix(
		section_local_x.x,		section_local_y.x,		section_local_z.x,	0,
		section_local_x.y,		section_local_y.y,		section_local_z.y,	0,
		section_local_x.z,		section_local_y.z,		section_local_z.z,	0,
		0,				0,				0,			1 );


	std::vector<std::vector<carve::geom::vector<3> > > face_loops_3d;
	face_loops_3d.resize( face_loops.size() );

	//std::vector<carve::geom::vector<3> > merged_path_3d;
	for( size_t i = 0; i < face_loops.size(); ++i )
	{
		std::vector<carve::geom::vector<2> >&  loop = face_loops[i];
		std::vector<carve::geom::vector<3> >& loop_3d = face_loops_3d[i];
		for( size_t jj = 0; jj < loop.size(); ++jj )
		{
			carve::geom::vector<2>&  vec_2d = loop[jj];
			carve::geom::vector<3>  vec_3d( carve::geom::VECTOR( vec_2d.x, vec_2d.y, 0 ) );

			// cross section is defined in XY plane
			vec_3d = matrix_first_direction*vec_3d + curve_point_first;
			loop_3d.push_back( vec_3d );
		}
	}

	// now insert points into polygon, avoiding points with same coordinates
	PolyInputCache3D poly_cache;
	std::vector<std::vector<std::vector<size_t> > > vec_point_indices_all_loops_all_curve_points;
	vec_point_indices_all_loops_all_curve_points.resize( num_curve_points );

	for( size_t ii = 0; ii<num_curve_points; ++ii )
	{
		carve::geom::vector<3> curve_point_current = curve_points[ii];
		carve::geom::vector<3> curve_point_next;
		carve::geom::vector<3> curve_point_before;
		if( ii == 0 )
		{
			// first point
			curve_point_next	= curve_points[ii+1];
			carve::geom::vector<3> delta_element = curve_point_next - curve_point_current;
			curve_point_before = curve_point_current - (delta_element);
		}
		else if( ii == num_curve_points-1 )
		{
			// last point
			curve_point_before	= curve_points[ii-1];
			carve::geom::vector<3> delta_element = curve_point_current - curve_point_before;
			curve_point_next = curve_point_before + (delta_element);
		}
		else
		{
			// inner point
			curve_point_next	= curve_points[ii+1];
			curve_point_before	= curve_points[ii-1];
		}

		carve::geom::vector<3> bisecting_normal;
		GeomUtils::bisectingPlane( curve_point_before, curve_point_current, curve_point_next, bisecting_normal );

		carve::geom::vector<3> section1 = curve_point_current - curve_point_before;
		carve::geom::vector<3> section2 = curve_point_next - curve_point_current;
		section1.normalize();
		section2.normalize();
		double dot_product = dot( section1, section2 );
		double dot_product_abs = std::abs(dot_product);

		if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
		{
			// bend found, compute next local z vector
			// TODO: vector with local z vectors as parameter
			carve::geom::vector<3> lateral_vec = cross( section1, section2 );
			local_z = cross( lateral_vec, section1 );
			local_z.normalize();
		}
		if( ii == num_curve_points -1 )
		{
			bisecting_normal *= -1.0;
		}

		carve::geom::plane<3> bisecting_plane( bisecting_normal, curve_point_current );

		std::vector<std::vector<size_t> >& vec_point_indices_all_loops_current_curve_point = vec_point_indices_all_loops_all_curve_points[ii];
		for( size_t jj = 0; jj < face_loops.size(); ++jj )
		{
			std::vector<carve::geom::vector<3> >& loop = face_loops_3d[jj];
			std::vector<size_t>& vec_point_indices_current_loop = vec_point_indices_all_loops_current_curve_point[ii];

			for( size_t kk = 0; kk < loop.size(); ++kk )
			{
				carve::geom::vector<3>&  section_point = loop[jj];

				carve::geom::vector<3> v;
				double t;
				carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, section_point, section_point + section1, v, t);
				if( intersect > 0 )
				{
					section_point = v;
				}
				else
				{
					messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}

				int vertex_id = poly_cache.addPoint(section_point);
			
				vec_point_indices_current_loop.push_back( vertex_id );
			}
		}

		//for( size_t jj = 0; jj < merged_path.size(); ++jj )
		//{
		//	carve::geom::vector<3>&  section_point = merged_path_3d[jj];
		//	carve::geom::vector<3> v;
		//	double t;
		//	carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, section_point, section_point + section1, v, t);
		//	if( intersect > 0 )
		//	{
		//		section_point = v;
		//	}
		//	else
		//	{
		//		messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		//	}

		//	int vertex_id = poly_cache.addPoint(section_point);
		//	//map_merged_idx[jj] = vertex_id;
		//	vec_loops_on_curve_point.push_back( vertex_id );
		//}
	}

	/*
	std::vector<carve::geom::vector<3> >& points = poly_cache.m_poly_data->points;
	const size_t num_points_all = points.size();
	size_t num_points_in_loop = 0;

	// create faces
	for( size_t ii=0; ii<num_curve_points-1; ++ii )
	{
		// figure 4: points in poly_data (merged path without duplicate vertices):
		// [9,7]<---------------------------[8,6]
		//   |                                ^
		//   |   [2,2]------------->[3,3]     |
		//   |   ^                    |       |     map_merged_idx: figure 3 -> figure 4
		//   |   |                    v       |
		//   |  [1,1] [5,1]<---------[4,4]    |
		//   | /      /                       |
		// [0,0]  [6,0]-------------------->[7,5]
		
		bool flip_faces = true;
		std::map<size_t, size_t>& map_merged_idx = vec_merged_idx[ii];

		// collect vertex indexes of loops
		std::map<size_t, std::vector<size_t> > loop_vert_idx;
		for( size_t merged_idx = 0; merged_idx < path_incorporated_holes.size(); ++merged_idx )
		{
			int loop_number = path_incorporated_holes[merged_idx].first;
			int point_idx_merged = map_merged_idx[merged_idx];
			std::vector<size_t>& result_loop_vec = loop_vert_idx.insert( std::make_pair( loop_number, std::vector<size_t>() ) ).first->second;
		
			// check if point index is already in loop
			bool already_in_loop = false;
			for( size_t i2 = 0; i2 < result_loop_vec.size(); ++i2 )
			{
				if( point_idx_merged == result_loop_vec[i2] )
				{
					already_in_loop = true;
					break;
				}
			}
			if( !already_in_loop )
			{
				result_loop_vec.push_back( point_idx_merged );
			}
		}

		// add faces along outer and inner loops
		for( std::map<size_t, std::vector<size_t> >::iterator it_result_loop = loop_vert_idx.begin(); it_result_loop != loop_vert_idx.end(); ++it_result_loop )
		{
			const std::vector<size_t>& loop_idx = it_result_loop->second;
			if( num_points_in_loop == 0 )
			{
				num_points_in_loop = loop_idx.size();
			}
			else
			{
				if( num_points_in_loop != loop_idx.size() )
				{
					messageCallback( "num_points_in_loop != loop_idx.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}
			}
		
			for( size_t i_loop=0; i_loop<num_points_in_loop; ++i_loop )
			{
				size_t point_idx = loop_idx[i_loop] + ii*num_points_in_loop;
				size_t point_idx_next = loop_idx[( i_loop + 1 ) % num_points_in_loop] + ii*num_points_in_loop;
				size_t point_idx_up = point_idx + num_points_in_loop;
				size_t point_idx_next_up = point_idx_next + num_points_in_loop;

				if( point_idx_next_up >= num_points_all )
				{
					messageCallback( "point_idx_next_up >= num_points_all", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}

#ifdef _DEBUG
				if( point_idx > num_points_all || point_idx_up > num_points_all )
				{
					messageCallback( "point_idx out of range", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}
				if( point_idx_next > num_points_all || point_idx_next_up > num_points_all )
				{
					messageCallback( "point_idx_next out of range", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}

				const carve::poly::Vertex<3>& v_a = poly_cache.m_poly_data->getVertex(point_idx);
				const carve::poly::Vertex<3>& v_b = poly_cache.m_poly_data->getVertex(point_idx_next);
				const carve::poly::Vertex<3>& v_c = poly_cache.m_poly_data->getVertex(point_idx_next_up);

				carve::geom::vector<3> pa( carve::geom::VECTOR( v_a.v[0],	v_a.v[1],	v_a.v[2] ) );
				carve::geom::vector<3> pb( carve::geom::VECTOR( v_b.v[0],	v_b.v[1],	v_b.v[2] ) );
				carve::geom::vector<3> pc( carve::geom::VECTOR( v_c.v[0],	v_c.v[1],	v_c.v[2] ) );

				double A = 0.5*(cross( pa-pb, pa-pc ).length());
				if( std::abs(A) < 0.000000001 )
				{
					messageCallback( "area < 0.000000001", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}
#endif

				if( flip_faces )
				{
					poly_cache.m_poly_data->addFace( point_idx,			point_idx_next, point_idx_next_up );
					poly_cache.m_poly_data->addFace( point_idx_next_up, point_idx_up,	point_idx );
				}
				else
				{
					poly_cache.m_poly_data->addFace( point_idx,			point_idx_up,	point_idx_next_up );
					poly_cache.m_poly_data->addFace( point_idx_next_up, point_idx_next, point_idx );
				}
			}
		}
	}

	bool flip_faces = true;

	// now the triangulated bottom and top cap
	std::map<size_t, size_t>& map_merged_idx_bottom = vec_merged_idx[0];
	std::map<size_t, size_t>& map_merged_idx_top = vec_merged_idx[vec_merged_idx.size()-1];
	for( size_t ii = 0; ii != triangulated.size(); ++ii )
	{
		carve::triangulate::tri_idx triangle = triangulated[ii];
		size_t a = triangle.a;
		size_t b = triangle.b;
		size_t c = triangle.c;

		size_t vertex_id_a = map_merged_idx_bottom[a];
		size_t vertex_id_b = map_merged_idx_bottom[b];
		size_t vertex_id_c = map_merged_idx_bottom[c];

		if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
		{
			continue;
		}

		// TODO: fix

		size_t vertex_id_a_top = map_merged_idx_top[a];
		size_t vertex_id_b_top = map_merged_idx_top[b];
		size_t vertex_id_c_top = map_merged_idx_top[c];

#ifdef _DEBUG
		
		if( vertex_id_a > num_points_all || vertex_id_a_top > num_points_all )
		{
			messageCallback( "vertex_id_a out of range", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		}
		if( vertex_id_b > num_points_all || vertex_id_b_top > num_points_all )
		{
			messageCallback( "vertex_id_b out of range", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		}
		if( vertex_id_c > num_points_all || vertex_id_c_top > num_points_all )
		{
			messageCallback( "vertex_id_c out of range", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		}
		const carve::poly::Vertex<3>& v_a = poly_cache.m_poly_data->getVertex(vertex_id_a);
		const carve::poly::Vertex<3>& v_b = poly_cache.m_poly_data->getVertex(vertex_id_b);
		const carve::poly::Vertex<3>& v_c = poly_cache.m_poly_data->getVertex(vertex_id_c);

		carve::geom::vector<3> pa( carve::geom::VECTOR( v_a.v[0],	v_a.v[1],	v_a.v[2] ) );
		carve::geom::vector<3> pb( carve::geom::VECTOR( v_b.v[0],	v_b.v[1],	v_b.v[2] ) );
		carve::geom::vector<3> pc( carve::geom::VECTOR( v_c.v[0],	v_c.v[1],	v_c.v[2] ) );

		double A = 0.5*(cross( pa-pb, pa-pc ).length());
		if( std::abs(A) < 0.000000001 )
		{
			messageCallback( "area < 0.000000001", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
		}
#endif

		if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
		{
			continue;
		}

		if( flip_faces )
		{
			poly_cache.m_poly_data->addFace( vertex_id_a, vertex_id_c, vertex_id_b );		// bottom cap
			poly_cache.m_poly_data->addFace( vertex_id_a_top,	vertex_id_b_top,	vertex_id_c_top );	// top cap, flipped outward
		}
		else
		{
			poly_cache.m_poly_data->addFace( vertex_id_a,		vertex_id_b,		vertex_id_c );		// bottom cap
			poly_cache.m_poly_data->addFace( vertex_id_a_top,	vertex_id_c_top,	vertex_id_b_top );	// top cap, flipped outward
		}
	}
	*/

#ifdef _DEBUG
	bool correct = poly_cache.checkFaceIndices();
	if( !correct )
	{
		CSG_Adapter::dumpPolyhedronInput( *(poly_cache.m_poly_data.get()), carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
	}
#endif

	try
	{
		item_data->addClosedPolyhedron( poly_cache.m_poly_data );
	}
	catch( IfcPPException& exception )
	{
		messageCallback( exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity );  // calling function already in e.what()
	}

#ifdef _DEBUG
	std::stringstream strs_err;
	shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_cache.m_poly_data->createMesh(carve::input::opts()) );
	bool poly_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset, strs_err, -1 );

	if( !poly_ok )
	{
		std::cout << strs_err.str().c_str() << std::endl;
	}
#endif
}

void Sweeper::createFace( const std::vector<std::vector<carve::geom::vector<3> > >& vec_bounds, IfcPPEntity* ifc_entity, PolyInputCache3D& poly_cache )
{
	std::vector<std::vector<carve::geom2d::P2> > face_loops_2d;
	std::vector<std::vector<double> > face_loop_3rd_dim;
	std::map<size_t, size_t> map_merged_idx;
	bool face_loop_reversed = false;
	bool warning_small_loop_detected = false;
	ProjectionPlane face_plane = UNDEFINED;

	for( auto it_bounds=vec_bounds.begin(); it_bounds!=vec_bounds.end(); ++it_bounds )
	{
		const std::vector<carve::geom::vector<3> >& loop_points = *it_bounds;

		if( loop_points.size() < 3 )
		{
			if( it_bounds == vec_bounds.begin() )
			{
				break;
			}
			else
			{
				continue;
			}
		}

		if( loop_points.size() == 3 && vec_bounds.size() == 1 )
		{
			std::vector<size_t> triangle_indexes;
			for( size_t point_i = 0; point_i < 3; ++point_i )
			{
				carve::geom::vector<3> v = loop_points[point_i];
				size_t vertex_id = poly_cache.addPoint( v );
				triangle_indexes.push_back( vertex_id );
				map_merged_idx[point_i] = vertex_id;
			}

			if( triangle_indexes.size() != 3 )
			{
				messageCallback( "triangle_indexes.size() != 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				continue;
			}

			poly_cache.m_poly_data->addFace( triangle_indexes[0], triangle_indexes[1], triangle_indexes[2] );
			continue;
		}

		carve::geom3d::Vector normal = GeomUtils::computePolygonNormal( loop_points );
		if( it_bounds == vec_bounds.begin() )
		{
			double nx = std::abs(normal.x);
			double ny = std::abs(normal.y);
			double nz = std::abs(normal.z);
			if( nz > nx && nz >= ny )
			{
				face_plane = XY_PLANE;
			}
			else if( nx >= ny && nx >= nz )
			{
				face_plane = YZ_PLANE;
			}
			else if( ny > nx && ny >= nz )
			{
				face_plane = XZ_PLANE;
			}
			else
			{
				std::stringstream err;
				err << "unable to project to plane: nx" << nx << " ny " << ny << " nz " << nz << std::endl;
				messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				continue;
			}
		}

		// project face into 2d plane
		std::vector<carve::geom2d::P2> path_loop_2d;
		std::vector<double> path_loop_3rd_dim;

		for( size_t i=0; i<loop_points.size(); ++i )
		{
			const carve::geom::vector<3>& point = loop_points[i];

			if( face_plane == XY_PLANE )
			{
				path_loop_2d.push_back( carve::geom::VECTOR(point.x, point.y ));
				path_loop_3rd_dim.push_back(point.z);
			}
			else if( face_plane == YZ_PLANE )
			{
				path_loop_2d.push_back( carve::geom::VECTOR(point.y, point.z ));
				path_loop_3rd_dim.push_back(point.x);
			}
			else if( face_plane == XZ_PLANE )
			{
				path_loop_2d.push_back( carve::geom::VECTOR(point.x, point.z ));
				path_loop_3rd_dim.push_back(point.y);
			}
		}

		if( loop_points.size() == 4 && vec_bounds.size() == 1 )
		{
			if( carve::geom2d::quadIsConvex( path_loop_2d[0], path_loop_2d[1], path_loop_2d[2], path_loop_2d[3] ) )
			{
				// add 2 triangles for quad
				std::vector<size_t> triangle_indexes;
				for( size_t point_i = 0; point_i < 4; ++point_i )
				{
					carve::geom::vector<3> v = loop_points[point_i];
					size_t vertex_index = poly_cache.addPoint( v );
					map_merged_idx[point_i] = vertex_index;
					triangle_indexes.push_back( vertex_index );
				}

				poly_cache.m_poly_data->addFace( triangle_indexes[0], triangle_indexes[1], triangle_indexes[2] );
				poly_cache.m_poly_data->addFace( triangle_indexes[2], triangle_indexes[3], triangle_indexes[0] );
				continue;
			}
		}

		// check winding order
		carve::geom3d::Vector normal_2d = GeomUtils::computePolygon2DNormal( path_loop_2d );
		if( it_bounds == vec_bounds.begin() )
		{
			if( normal_2d.z < 0 )
			{
				std::reverse( path_loop_2d.begin(), path_loop_2d.end() );
				face_loop_reversed = true;
			}
		}
		else
		{
			if( normal_2d.z > 0 )
			{
				std::reverse( path_loop_2d.begin(), path_loop_2d.end() );
			}
		}

		if( path_loop_2d.size() < 3 )
		{
			//std::cout << __FUNC__ << ": #" << face_id <<  "=IfcFace: path_loop.size() < 3" << std::endl;
			continue;
		}

		double signed_area = carve::geom2d::signedArea( path_loop_2d );
		double min_loop_area = 1.e-10;//m_geom_settings->m_min_face_area
		if( std::abs( signed_area ) < min_loop_area )
		{
			warning_small_loop_detected = true;
			continue;
		}

		face_loops_2d.push_back(path_loop_2d);
		face_loop_3rd_dim.push_back(path_loop_3rd_dim);
	}

	if( warning_small_loop_detected )
	{
		std::stringstream err;
		err << "abs( signed_area ) < 1.e-10";
		messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, ifc_entity );
	}

	if( face_loops_2d.size() > 0 )
	{
		std::vector<std::pair<size_t, size_t> > result; // first is loop index, second is vertex index in loop
		std::vector<carve::geom2d::P2> merged;
		std::vector<carve::geom3d::Vector> merged_3d;
		std::vector<carve::triangulate::tri_idx> triangulated;

		try
		{
			result = carve::triangulate::incorporateHolesIntoPolygon(face_loops_2d);
			merged.reserve(result.size());
			for( size_t i = 0; i < result.size(); ++i )
			{
				size_t loop_number = result[i].first;
				size_t index_in_loop = result[i].second;
				carve::geom2d::P2& loop_point = face_loops_2d[loop_number][index_in_loop];
				merged.push_back( loop_point );

				// restore 3rd dimension
				if( face_loop_reversed )
				{
					index_in_loop = face_loops_2d[loop_number].size() - index_in_loop - 1;
				}

				carve::geom3d::Vector v;
				if( face_plane == XY_PLANE )
				{
					double z = face_loop_3rd_dim[loop_number][index_in_loop];
					v = carve::geom::VECTOR(        loop_point.x,   loop_point.y,   z);
				}
				else if( face_plane == YZ_PLANE )
				{
					double x = face_loop_3rd_dim[loop_number][index_in_loop];
					v = carve::geom::VECTOR(        x,      loop_point.x,   loop_point.y);
				}
				else if( face_plane == XZ_PLANE )
				{
					double y = face_loop_3rd_dim[loop_number][index_in_loop];
					v = carve::geom::VECTOR(        loop_point.x,   y,      loop_point.y);
				}
				merged_3d.push_back( v );
			}
			carve::triangulate::triangulate(merged, triangulated);
			carve::triangulate::improve(merged, triangulated);

		}
		catch(...)
		{
			messageCallback( "carve::triangulate::incorporateHolesIntoPolygon failed ", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		// now insert points to polygon, avoiding points with same coordinates
		for( size_t i = 0; i != merged.size(); ++i )
		{
			const carve::geom::vector<3>& v = merged_3d[i];
			size_t vertex_index = poly_cache.addPoint( v );
			map_merged_idx[i] = vertex_index;
		}
		for( size_t i = 0; i != triangulated.size(); ++i )
		{
			carve::triangulate::tri_idx triangle = triangulated[i];
			size_t a = triangle.a;
			size_t b = triangle.b;
			size_t c = triangle.c;

			size_t vertex_id_a = map_merged_idx[a];
			size_t vertex_id_b = map_merged_idx[b];
			size_t vertex_id_c = map_merged_idx[c];

			if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
			{
				continue;
			}

#ifdef _DEBUG
			const carve::poly::Vertex<3>& v_a = poly_cache.m_poly_data->getVertex(vertex_id_a);
			const carve::poly::Vertex<3>& v_b = poly_cache.m_poly_data->getVertex(vertex_id_b);

			double dx = v_a.v[0] - v_b.v[0];
			if( std::abs(dx) < 0.0000001 )
			{
				double dy = v_a.v[1] - v_b.v[1];
				if( std::abs(dy) < 0.0000001 )
				{
					double dz = v_a.v[2] - v_b.v[2];
					if( std::abs(dz) < 0.0000001 )
					{
						messageCallback( "abs(dx) < 0.00001 && abs(dy) < 0.00001 && abs(dz) < 0.00001", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					}
				}
			}
#endif

			if( face_loop_reversed )
			{
				poly_cache.m_poly_data->addFace( vertex_id_a, vertex_id_c, vertex_id_b );
			}
			else
			{
				poly_cache.m_poly_data->addFace( vertex_id_a, vertex_id_b, vertex_id_c );
			}
		}
	}
}

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

#ifdef GEOMETRY_DEBUG_CHECK

#include <vector>
#include <osgDB/fstream>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include "IncludeCarveHeaders.h"

namespace GeomDebugDump
{
	static double dump_y_pos_geom = 0;

	inline void clearMeshsetDump()
	{
		std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::trunc );
		dump_ofstream.close();
	}

	inline void Polyhedron2Stream( const carve::poly::Polyhedron* poly, const vec3& offset, const carve::geom::vector<4>& color, std::stringstream& strs_out )
	{
		strs_out << "Polyhedron{" << std::endl;
		strs_out << "color{" << color.x << ", " << color.y << ", " << color.z << ", " << color.w << "}" << std::endl;
		strs_out << "vertices{" << std::endl;
		const size_t num_vertices = poly->vertices.size();
		for( size_t i = 0; i < num_vertices; ++i )
		{
			carve::poly::Vertex<3> vertex = poly->vertices[i];
			if( i > 0 )
			{
				strs_out << ",";
			}
			strs_out << "{" << vertex.v.x + offset.x << ", " << vertex.v.y + offset.y << ", " << vertex.v.z + offset.z << "}";
		}
		strs_out << "}" << std::endl;

		strs_out << "faces{" << std::endl;
		for( size_t ii = 0; ii < poly->faces.size(); ++ii )
		{
			carve::poly::Face<3> f = poly->faces[ii];
			if( ii > 0 )
			{
				strs_out << ",";
			}
			strs_out << "{";

			std::vector<const carve::poly::Vertex<3> *>::const_iterator vertices_begin = f.vbegin();
			std::vector<const carve::poly::Vertex<3> *>::const_iterator vertices_end = f.vend();
			for( auto it_vert = vertices_begin; it_vert != vertices_end; ++it_vert )
			{
				const carve::poly::Vertex<3>* vert = *it_vert;

				if( it_vert != vertices_begin )
				{
					strs_out << ", ";
				}
				ptrdiff_t ptr_diff = poly->vertexToIndex( vert );
				strs_out << ptr_diff;

#ifdef _DEBUG
				int vert_index = -1;

				for( size_t i_find_vert = 0; i_find_vert < num_vertices; ++i_find_vert )
				{
					const carve::poly::Vertex<3>& vertex_ref = (poly->vertices[i_find_vert]);
					const carve::poly::Vertex<3>* vertex_ptr = &vertex_ref;
					if( vertex_ptr == vert )
					{
						vert_index = i_find_vert;
						break;
					}
				}

				if( vert_index != ptr_diff )
				{
					std::cout << "vert_index != ptr_diff" << std::endl;
				}
#endif
			}
			////const_vertex_iter_t vbegin() const { return vertices.begin(); }
			////const_vertex_iter_t vend() const { return vertices.end(); }

			////f.getVertexLoop() vertices;
			//for( size_t jj = 0; jj < f.nVertices(); ++jj )
			//{
			//	if( jj > 0 )
			//	{
			//		strs_out << ", ";
			//	}
			//	ptrdiff_t ptr_diff = poly->vertexToIndex( f.vertex( jj ) );
			//	strs_out << ptr_diff;
			//}
			strs_out << "}" << std::endl;
		}
		strs_out << std::endl << "}" << std::endl;  // faces
		strs_out << std::endl << "}" << std::endl;  // Polyhedron
	}

	inline void dumpMeshsets( const std::vector<carve::mesh::MeshSet<3>* >& vec_meshsets, const vec3& offset, const std::vector<carve::geom::vector<4> >& vec_colors, bool append )
	{
		std::stringstream strs_out;
		for( size_t i = 0; i < vec_meshsets.size(); ++i )
		{
			carve::mesh::MeshSet<3>* meshset = vec_meshsets[i];
			carve::geom::vector<4> color = carve::geom::VECTOR( 0.5, 0.5, 0.5, 1.0 );
			if( i < vec_colors.size() )
			{
				color = vec_colors[i];
			}
			shared_ptr<carve::poly::Polyhedron> poly( carve::polyhedronFromMesh( meshset, -1 ) );
			Polyhedron2Stream( poly.get(), offset, color, strs_out );
		}

		if( !append )
		{
			clearMeshsetDump();
		}
		osgDB::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
		dump_ofstream << strs_out.str().c_str();
		dump_ofstream.close();
	}

	inline void dumpPolyhedron( const carve::poly::Polyhedron* poly, const vec3& offset, const carve::geom::vector<4>& color, bool append )
	{
		std::stringstream strs_out;
		Polyhedron2Stream( poly, offset, color, strs_out );

		if( !append )
		{
			clearMeshsetDump();
		}

		std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
		dump_ofstream << strs_out.str().c_str();
		dump_ofstream.close();
	}
	
	inline void dumpPolyhedronInput( const carve::input::PolyhedronData& poly_input, const vec3& offset, const carve::geom::vector<4>& color, bool append )
	{
		dumpPolyhedron( poly_input.create( carve::input::opts() ), offset, color, append );
	}

	inline void dumpMeshset( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, const carve::geom::vector<4>& color, bool append, bool move_offset = true )
	{
		if( meshset->meshes.size() == 0 )
		{
			return;
		}
		if( !move_offset )
		{
			//dump_y_pos_geom = 0;
		}
		//GeomUtils::applyTranslate( meshset, carve::geom::VECTOR( 0, dump_y_pos_geom, 0 ) );
		vec3 offset = carve::geom::VECTOR( 0, dump_y_pos_geom, 0 );

		shared_ptr<carve::poly::Polyhedron> poly( carve::polyhedronFromMesh( meshset.get(), -1 ) );
		dumpPolyhedron( poly.get(), offset, color, append );

		if( move_offset )
		{
			dump_y_pos_geom += meshset->getAABB().extent.y*2.2;
		}
	}

	inline void dumpFaces( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, const std::vector<carve::mesh::Face<3>* >& vec_faces )
	{
		std::stringstream strs_out;
		strs_out << "Polyhedron{" << std::endl;
		strs_out << "vertices{" << std::endl;

		std::map<const carve::mesh::Vertex<3>*, int > map_vertex_idx;
		const size_t num_vertices = meshset->vertex_storage.size();
		for( size_t i = 0; i < num_vertices; ++i )
		{
			const carve::mesh::Vertex<3>& vertex = meshset->vertex_storage[i];
			//carve::poly::Vertex<3> vertex = meshset->vertex_storage[i].v;
			map_vertex_idx[&vertex] = i;
			if( i > 0 )
			{
				strs_out << ",";
			}
			strs_out << "{" << vertex.v.x << ", " << vertex.v.y + dump_y_pos_geom << ", " << vertex.v.z << "}";
		}
		strs_out << "}" << std::endl;  // vertices

		strs_out << "faces{" << std::endl;
		for( size_t i = 0; i < vec_faces.size(); ++i )
		{
			carve::mesh::Face<3>* f = vec_faces[i];
			if( i > 0 )
			{
				strs_out << ",";
			}
			strs_out << "{";
			carve::mesh::Edge<3>* edge = f->edge;
			for( size_t j = 0; j < f->nEdges(); ++j )
			{
				if( j > 0 )
				{
					strs_out << ", ";
				}
				carve::mesh::Vertex<3>* face_vertex = edge->vert;
				int idx = map_vertex_idx[face_vertex];
				strs_out << idx;// poly->vertexToIndex( f.vertex( j ) );
				edge = edge->next;
			}
			strs_out << "}";
		}
		strs_out << std::endl << "}"; // faces
		strs_out << std::endl << "}"; // Polyhedron

		std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
		dump_ofstream << strs_out.str().c_str();
		dump_ofstream.close();

		dump_y_pos_geom += meshset->getAABB().extent.y*2.2;
	}

	inline void dumpEdges( const shared_ptr<carve::mesh::MeshSet<3> > meshset, const std::vector<carve::mesh::Edge<3>* >& vec_edges )
	{
		std::stringstream strs_out;
		strs_out << "PolyLineSet{" << std::endl;
		strs_out << "vertices{" << std::endl;

		std::map<const carve::mesh::Vertex<3>*, int > map_vertex_idx;
		size_t vertex_idx = 0;
		for( size_t i = 0; i < vec_edges.size(); ++i )
		{
			carve::mesh::Edge<3>* edge = vec_edges[i];

			const carve::mesh::Vertex<3>* vertex1 = edge->v1();
			map_vertex_idx[vertex1] = vertex_idx;
			if( vertex_idx > 0 )
			{
				strs_out << ",";
			}
			strs_out << "{" << vertex1->v.x << ", " << vertex1->v.y << ", " << vertex1->v.z << "}";
			++vertex_idx;

			const carve::mesh::Vertex<3>* vertex2 = edge->v2();
			map_vertex_idx[vertex2] = vertex_idx;
			if( vertex_idx > 0 )
			{
				strs_out << ",";
			}
			strs_out << "{" << vertex2->v.x << ", " << vertex2->v.y << ", " << vertex2->v.z << "}";
			++vertex_idx;
		}
		strs_out << "}" << std::endl; // vertices

		strs_out << "lines{" << std::endl;
		for( size_t i = 0; i < vec_edges.size(); ++i )
		{
			carve::mesh::Edge<3>* edge = vec_edges[i];
			if( i > 0 )
			{
				strs_out << ",";
			}

			const carve::mesh::Vertex<3>* vertex1 = edge->v1();
			const carve::mesh::Vertex<3>* vertex2 = edge->v2();

			if( i > 0 )
			{
				strs_out << ", ";
			}

			int idx1 = map_vertex_idx[vertex1];
			int idx2 = map_vertex_idx[vertex2];
			strs_out << "{" << idx1 << "," << idx2 << "}";
		}
		strs_out << std::endl << "}"; // lines
		strs_out << std::endl << "}"; // PolyLineSet

		std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
		dump_ofstream << strs_out.str().c_str();
		dump_ofstream.close();
	}

	inline void dumpPolyline( const std::vector<vec2>& vec_polyline, const carve::geom::vector<4>& color, bool append, bool move_dump_position )
	{
		if( vec_polyline.size() < 1 )
		{
			return;
		}

		std::stringstream strs_out;
		

		if( vec_polyline.size() < 1 )
		{
			return;
		}

		strs_out << "Polyline{" << std::endl;
		strs_out << "color{" << color.x << ", " << color.y << ", " << color.z << ", " << color.w << "}" << std::endl;

		strs_out << "vertices{" << std::endl;

		double min_y = 0;
		double max_y = 0;
		const size_t num_vertices = vec_polyline.size();
		for( size_t i = 0; i < num_vertices; ++i )
		{
			const vec2& vertex = vec_polyline[i];
			if( i > 0 )
			{
				strs_out << ",";
				if( vertex.y < min_y ) min_y = vertex.y;
				else if( vertex.y > max_y ) max_y = vertex.y;
			}
			else
			{
				min_y = vertex.y;
				max_y = vertex.y;
			}
			strs_out << "{" << vertex.x << ", " << vertex.y + dump_y_pos_geom << "}";
		}
		strs_out << "}" << std::endl;  // vertices
		if( move_dump_position )
		{
			dump_y_pos_geom += (max_y - min_y)*1.5;
			if( max_y > dump_y_pos_geom )
			{
				dump_y_pos_geom = max_y*1.1;
			}
		}

		strs_out << std::endl << "}" << std::endl;  // Polyline

		if( !append )
		{
			clearMeshsetDump();
		}

		std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
		dump_ofstream << strs_out.str().c_str();
		dump_ofstream.close();
	}

	inline void dumpPolylineSet( const carve::input::PolylineSetData* polyline_data, const carve::geom::vector<4>& color, bool append )
	{
		carve::line::PolylineSet* polyline_set = polyline_data->create( carve::input::opts() );

		if( polyline_set->vertices.size() < 2 )
		{
#ifdef _DEBUG
			std::cout << __FUNC__ << ": polyline_set->vertices.size() < 2" << std::endl;
#endif
			return;
		}


		std::stringstream strs_out;
		strs_out << "Polyline{" << std::endl;
		strs_out << "color{" << color.x << ", " << color.y << ", " << color.z << ", " << color.w << "}" << std::endl;
		strs_out << "vertices{" << std::endl;

		int vertex_count_all = 0;
		for( auto it = polyline_set->lines.begin(); it != polyline_set->lines.end(); ++it )
		{
			carve::line::Polyline* pline = *it;
			size_t vertex_count_polyline = pline->vertexCount();

			for( size_t vertex_i = 0; vertex_i < vertex_count_polyline; ++vertex_i )
			{
				if( vertex_i >= polyline_set->vertices.size() )
				{
#ifdef _DEBUG
					std::cout << __FUNC__ << ": vertex_i >= polyline_set->vertices.size()" << std::endl;
#endif
					continue;
				}
				const carve::line::Vertex* vertex = pline->vertex( vertex_i );
				//vertices->push_back( osg::Vec3d( v->v[0], v->v[1], v->v[2] ) );

				//if( it != polyline_set->lines.begin() )
				if( vertex_count_all > 0 )
				{
					strs_out << "," << std::endl;
				}
				strs_out << "{" << vertex->v.x << ", " << vertex->v.y << ", " << vertex->v.z << "}";
				++vertex_count_all;

			}
		}
		strs_out << "}" << std::endl; // vertices
		strs_out << "}" << std::endl; // Polyline



		if( !append )
		{
			clearMeshsetDump();
		}

		std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
		dump_ofstream << strs_out.str().c_str();
		dump_ofstream.close();
	}
}
#endif

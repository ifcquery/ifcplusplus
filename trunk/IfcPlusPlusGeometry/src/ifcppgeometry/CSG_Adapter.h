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

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/StatusCallback.h>
#include "IncludeCarveHeaders.h"
#include "GeometryException.h"
#include "GeometryInputData.h"

typedef carve::mesh::Edge<3> edge_t;
typedef carve::mesh::Face<3> face_t;
typedef carve::mesh::MeshSet<3> meshset_t;

namespace CSG_Adapter
{
#ifdef _DEBUG
	static int simplify_count = 0;
	static double dump_y_pos = 0;
#endif

	inline size_t getNumFaces( const meshset_t* meshset )
	{
		size_t num_faces = 0;
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			num_faces += meshset->meshes[i]->faces.size();
		}
		return num_faces;
	}
	inline size_t getNumClosedEdges( const meshset_t* meshset )
	{
		size_t num_edges = 0;
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			num_edges += meshset->meshes[i]->closed_edges.size();
		}
		return num_edges;
	}
	inline double getVolume( const meshset_t* meshset )
	{
		double meshset_volume = 0;
		for( size_t kk = 0; kk < meshset->meshes.size(); ++kk )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			double mesh_volume = mesh->volume();
			meshset_volume += mesh_volume;
		}
		return meshset_volume;
	}

	template<typename T>
	size_t findInVector( T* find_object, std::vector<T*>& vec )
	{
		for( size_t ii = 0; ii < vec.size(); ++ii )
		{
			T* t_check = vec[ii];
			if( t_check == find_object )
			{
				return ii;
			}
		}
		return -1;
	}


	inline void roundVertices( meshset_t* meshset )
	{
		std::vector<carve::mesh::Vertex<3> >& vertex_storage = meshset->vertex_storage;
		const size_t num_vertices = vertex_storage.size();
		for( size_t i = 0; i < num_vertices; ++i )
		{
			carve::mesh::Vertex<3>& vertex = vertex_storage[i];
			vertex.v.x = round( vertex.v.x*1000000.0 ) * 0.000001;
			vertex.v.y = round( vertex.v.y*1000000.0 ) * 0.000001;
			vertex.v.z = round( vertex.v.z*1000000.0 ) * 0.000001;
		}
	}
	inline void mergeAlignedEdges( shared_ptr<meshset_t >& meshset, carve::mesh::MeshSimplifier& simplifier )
	{
		if( !meshset )
		{
			return;
		}
		bool meshset_dirty = false;

		std::map<face_t*, std::vector<edge_t*> > map_omit_face_edges;
		for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

			std::vector<edge_t*>& vec_closed_edges = mesh->closed_edges;
			for( size_t closed_edge_i = 0; closed_edge_i < vec_closed_edges.size(); ++closed_edge_i )
			{
				edge_t* edge_i = vec_closed_edges[closed_edge_i];
				edge_t* edge_next = edge_i->next;

#ifdef _DEBUG
				if( edge_i == reinterpret_cast<edge_t*>( 0xfeeefeeefeeefeee ) )
				{
					std::cout << __FUNC__ << ": !edge_i" << std::endl;
					continue;
				}
				if( edge_next == reinterpret_cast<edge_t*>( 0xfeeefeeefeeefeee ) )
				{
					std::cout << __FUNC__ << ": !edge_next" << std::endl;
					continue;
				}
				if( edge_next->rev == reinterpret_cast<edge_t*>( 0xfeeefeeefeeefeee ) )
				{
					std::cout << __FUNC__ << ": !edge_j->rev" << std::endl;
					continue;
				}
#endif

				if( !edge_i )
				{
#ifdef _DEBUG
					std::cout << __FUNC__ << ": !edge_i" << std::endl;
#endif
					continue;
				}

				if( !edge_next )
				{
#ifdef _DEBUG
					std::cout << __FUNC__ << ": !edge_next" << std::endl;
#endif
					continue;
				}

				if( !edge_next->rev )
				{
#ifdef _DEBUG
					std::cout << __FUNC__ << ": !edge_j->rev" << std::endl;
#endif
					continue;
				}

				if( edge_next->rev->next == edge_i->rev )
				{
					//   ----------->(v3)--->		     --->o--->
					//               |^                      |^
					// edge_next->rev||edge_next(remove)     ||
					//               v|                      || 
					//              (v2)                     ||
					//               |^                      ||
					//       (remove)||edge_i	             ||edge_i
					//               v|				         v|
					//   <----------(v1)<---             <---o<---

					const carve::mesh::Vertex<3>* v1 = edge_i->v1();
					const carve::mesh::Vertex<3>* v2 = edge_i->v2();
					const carve::mesh::Vertex<3>* v3 = edge_next->v2();

					const carve::geom::vector<3>& v1vec = v1->v;
					const carve::geom::vector<3>& v2vec = v2->v;
					const carve::geom::vector<3>& v3vec = v3->v;
					carve::geom::vector<3> sement12 = v2vec - v1vec;
					carve::geom::vector<3> sement23 = v3vec - v2vec;
					double sement12_length2 = sement12.length2();
					double sement23_length2 = sement23.length2();
#ifdef _DEBUG
					if( std::abs( edge_i->length2() - sement12_length2 ) > 0.00001 )
					{
						std::cout << __FUNC__ << ": abs( edge_i->length2() - sement12_length2 ) > 0.00001" << std::endl;
					}

					if( std::abs( edge_next->length2() - sement23_length2 ) > 0.00001 )
					{
						std::cout << __FUNC__ << ": abs( edge_next->length2() - sement23_length2 ) > 0.00001" << std::endl;
					}
#endif

					// check angle between edges
					sement12.normalize();
					sement23.normalize();
					double dot_angle = dot( sement12, sement23 );
					if( std::abs( std::abs( dot_angle ) - 1.0 ) < 0.001 )
					{
						// edges are in line
						if( v1 == v3 )
						{
#ifdef _DEBUG
							std::cout << __FUNC__ << ": edge loop with only 2 edges" << std::endl;
#endif
						}

						// this links previous and next edges and deletes edge_j and reverse of edge_i:
						edge_t* edge_next_rev = edge_next->rev;

						size_t edge_remove_idx = findInVector( edge_next, vec_closed_edges );
						if( edge_remove_idx >= 0 && edge_remove_idx < vec_closed_edges.size() )
						{
							vec_closed_edges.erase( vec_closed_edges.begin() + edge_remove_idx );
							if( edge_remove_idx <= closed_edge_i )
							{
								--closed_edge_i;
							}
						}
						edge_next->removeHalfEdge();

						edge_remove_idx = findInVector( edge_i->rev, vec_closed_edges );
						if( edge_remove_idx >= 0 && edge_remove_idx < vec_closed_edges.size() )
						{
							vec_closed_edges.erase( vec_closed_edges.begin() + edge_remove_idx );
							if( edge_remove_idx <= closed_edge_i )
							{
								--closed_edge_i;
							}
						}
						edge_i->rev->removeHalfEdge();

						edge_i->rev = edge_next_rev;
						edge_next_rev->rev = edge_i;
						meshset_dirty = true;

						// the vertex is kept in the vertex storage, no need to delete here
					}
					else
					{
						// edges are not in line, so faces should be in a plane
#ifdef _DEBUG
						const carve::geom::vector<3>& normal_face_i = edge_i->face->plane.N;
						const carve::geom::vector<3>& normal_face_i_rev = edge_i->rev->face->plane.N;


						double dot_face_angle = dot( normal_face_i, normal_face_i_rev );
						if( std::abs( dot_face_angle - 1.0 ) > 0.001 )
						{
							std::cout << __FUNC__ << ": abs( dot_face_angle - 1.0 ) > 0.001" << std::endl;
						}
#endif
					}
				}
			}
		}

		if( meshset_dirty )
		{
			for( size_t i = 0; i < meshset->meshes.size(); ++i )
			{
				simplifier.removeRemnantFaces( meshset->meshes[i] );
				simplifier.cleanFaceEdges( meshset->meshes[i] );
				meshset->meshes[i]->cacheEdges();
			}
		}
	}
	inline bool checkMeshSetNonNegativeAndClosed( const shared_ptr<meshset_t> mesh_set )
	{
		bool meshes_closed = true;
		for( size_t i = 0; i < mesh_set->meshes.size(); ++i )
		{
			carve::mesh::Mesh<3>* mesh_i = mesh_set->meshes[i];
			if( mesh_i )
			{
				if( mesh_i->isNegative() )
				{
					mesh_i->invert();
					if( mesh_i->isNegative() )
					{
						mesh_i->recalc();
						mesh_i->calcOrientation();
						if( mesh_i->isNegative() )
						{
							std::cout << "could not invert mesh_set->meshes[" << i << "] " << std::endl;
						}
					}
				}

				if( !mesh_i->isClosed() )
				{
					meshes_closed = false;
				}

				if( mesh_i->open_edges.size() > 0 )
				{
					meshes_closed = false;
				}
			}
		}
		return meshes_closed;
	}
	inline bool checkFaceIntegrity( const shared_ptr<meshset_t>& meshset )
	{
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			carve::mesh::Mesh<3>* mesh_i = meshset->meshes[i];
			if( mesh_i->open_edges.size() > 0 )
			{
				//return false;
			}
			std::vector<face_t* >& vec_faces = mesh_i->faces;
			for( size_t j = 0; j < vec_faces.size(); ++j )
			{
				face_t* face = vec_faces[j];
				edge_t* e = face->edge;
				const size_t n_edges = face->n_edges;
				for( size_t i_edge = 0; i_edge < n_edges; ++i_edge )
				{
					if( e == reinterpret_cast<edge_t*>( 0xfeeefeeefeeefeee ) )
					{
						return false;
					}
					if( !e->rev )
					{
#ifdef _DEBUG
						std::vector<face_t*> vec_faces;
						vec_faces.push_back( face );
						GeomDebugUtils::dumpFaces( meshset, vec_faces );

						std::vector<edge_t*> vec_edges;
						vec_edges.push_back( e );
						GeomDebugUtils::dumpEdges( meshset, vec_edges );
#endif
						return false;
					}
					if( e->rev->next == reinterpret_cast<edge_t*>( 0xfeeefeeefeeefeee ) )
					{
						return false;
					}
					if( !e->rev->next )
					{
						return false;
					}
					if( !e->rev->next->next )
					{
						return false;
					}
					if( !e->rev->next->next->vert )
					{
						return false;
					}

					if( !e->next )
					{
						return false;
					}
					if( e->next == reinterpret_cast<edge_t*>( 0xfeeefeeefeeefeee ) )
					{
						return false;
					}
					if( !e->prev )
					{
						return false;
					}
					if( !e->rev )
					{
						return false;
					}
					if( !e->vert )
					{
						return false;
					}
					if( !e->face )
					{
						return false;
					}

					e = e->next;
				}
				if( e != face->edge )
				{
					return false;
				}
			}
		}
		return true;
	}
	inline bool checkMeshSetValidAndClosed( const shared_ptr<meshset_t> mesh_set, StatusCallback* report_callback, IfcPPEntity* entity )
	{
		if( !mesh_set )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_id << " not valid" << std::endl;
			}
#endif
			return false;
		}
		if( mesh_set->meshes.size() == 0 )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_id << " has no meshes" << std::endl;
			}
#endif
			return false;
		}

		if( !checkFaceIntegrity( mesh_set ) )
		{
			return false;
		}
		std::stringstream err;
		bool meshes_closed = checkMeshSetNonNegativeAndClosed( mesh_set );
		if( meshes_closed )
		{
			// check volume
			double meshset_volume = 0;
			for( size_t kk = 0; kk < mesh_set->meshes.size(); ++kk )
			{
				carve::mesh::Mesh<3>* mesh = mesh_set->meshes[kk];
				double mesh_volume = mesh->volume();

				if( mesh_volume < 0 )
				{
					mesh->invert();
					if( mesh->isNegative() )
					{
						mesh->recalc();
						mesh->calcOrientation();
						if( mesh->isNegative() )
						{
							std::cout << "could not invert negative mesh[" << kk << "] " << std::endl;
						}
					}
					mesh_volume = mesh->volume();
				}

				if( mesh_volume < 0 )
				{
					err << "mesh_volume < 0" << std::endl;
				}

				meshset_volume += mesh_volume;
			}
		}
		else
		{
			err << "mesh_set not closed" << std::endl;
		}

		if( err.tellp() > 0 )
		{
#ifdef _DEBUG
			report_callback->messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity );
#endif
			return false;
		}
		return true;
	}

	inline bool checkMeshSetVolume( const shared_ptr<meshset_t> mesh_set, StatusCallback* report_callback, IfcPPEntity* entity )
	{
		if( !mesh_set )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_id << " not valid" << std::endl;
			}
#endif
			return false;
		}
		if( mesh_set->meshes.size() == 0 )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_id << " has no meshes" << std::endl;
			}
#endif
			return false;
		}

		std::stringstream err;

		// check volume
		double meshset_volume = 0;
		for( size_t kk = 0; kk < mesh_set->meshes.size(); ++kk )
		{
			carve::mesh::Mesh<3>* mesh = mesh_set->meshes[kk];
			double mesh_volume = mesh->volume();

			if( mesh_volume < 0 )
			{
				mesh->invert();
				if( mesh->isNegative() )
				{
					mesh->recalc();
					mesh->calcOrientation();
					if( mesh->isNegative() )
					{
						err << "mesh[" << kk << "]->isNegative() " << std::endl;
					}
				}
				mesh_volume = mesh->volume();
			}

			if( mesh_volume < 0 )
			{
				err << "mesh_volume < 0" << std::endl;
			}

			meshset_volume += mesh_volume;
		}


		if( err.tellp() > 0 )
		{
#ifdef _DEBUG
			report_callback->messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity );
#endif
			return false;
		}
		return true;
	}
	inline void removeFins( shared_ptr<meshset_t >& meshset )
	{
		return;  // TODO: improve and check

		if( !meshset )
		{
			return;
		}
		bool meshset_dirty = false;

		std::map<face_t*, std::vector<edge_t*> > map_omit_face_edges;
		for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

			// find edges that are in line
			std::vector<edge_t*>& vec_closed_edges = mesh->closed_edges;

			for( size_t closed_edge_i = 0; closed_edge_i < vec_closed_edges.size(); ++closed_edge_i )
			{
				edge_t* edge = vec_closed_edges[closed_edge_i];
				edge_t* edge_reverse = edge->rev;
				face_t* face = edge->face;
				face_t* face_reverse = edge_reverse->face;

				carve::geom::vector<3>& face_normal = face->plane.N;
				carve::geom::vector<3>& face_reverse_normal = face_reverse->plane.N;

				const double cos_angle = dot( face_normal, face_reverse_normal );
				if( std::abs( cos_angle + 1.0 ) > 0.000001 )
				{
					continue;
				}

				//                      e->rev->prev
				//              <----------------                             //
				//             | --------------->                            //
				//  e->rev [x] |^    e->next [x]             e->rev->prev   // e
				//             ||                                          //
				//             ||e 
				//             v|

				if( edge->next->rev == edge_reverse->prev )
				{
					edge_t* edge_next_rev = edge->next->rev;
					edge->next->removeHalfEdge();
					edge_reverse->removeHalfEdge();

					// TODO: erase from vec_closed_edges, check if closed_edge_i needs to be decremented

					edge->rev = edge_next_rev;
					edge_next_rev->rev = edge;
					meshset_dirty = true;
				}
			}
		}
		if( meshset_dirty )
		{
			for( size_t i = 0; i < meshset->meshes.size(); ++i )
			{
				meshset->meshes[i]->cacheEdges();
			}
		}
	}
	inline void retriangulateMeshSet( shared_ptr<meshset_t >& meshset )
	{
		if( !meshset )
		{
			return;
		}

		checkMeshSetNonNegativeAndClosed( meshset );

		bool already_triagulated = true;
		for( size_t ii = 0; ii < meshset->meshes.size(); ++ii )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
			std::vector<face_t* >& vec_faces = mesh->faces;
			for( size_t i2 = 0; i2 < vec_faces.size(); ++i2 )
			{
				face_t* face = vec_faces[i2];
				if( face->n_edges != 3 )
				{
					already_triagulated = false;
				}
			}
		}
		if( already_triagulated )
		{
			return;
		}

		PolyInputCache3D poly_cache;
		std::map<int, int> map_merged_idx;
		for( size_t ii = 0; ii < meshset->meshes.size(); ++ii )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
			std::vector<face_t* >& vec_faces = mesh->faces;

			for( size_t i2 = 0; i2 < vec_faces.size(); ++i2 )
			{
				face_t* face = vec_faces[i2];
				std::vector<int> face_idx;

				//carve::geom3d::Vector normal = face->plane.N;

				std::vector<carve::geom::vector<2> > verts2d;
				face->getProjectedVertices( verts2d );
				if( verts2d.size() < 3 )
				{
					continue;
				}

				// check winding order
				//carve::geom3d::Vector normal_2d = GeomUtils::computePolygon2DNormal( verts2d );
				//if( normal_2d.z < 0 )
				//{
				//      std::reverse( verts2d.begin(), verts2d.end() );
				//}

				std::vector<carve::triangulate::tri_idx> triangulated;
				if( verts2d.size() > 3 )
				{
					try
					{
						carve::triangulate::triangulate( verts2d, triangulated );
						carve::triangulate::improve( verts2d, triangulated );
					}
					catch( ... )
					{
#ifdef _DEBUG
						std::cout << __FUNC__ << " carve::triangulate failed " << std::endl;
#endif
						continue;
					}
				}
				else
				{
					triangulated.push_back( carve::triangulate::tri_idx( 0, 1, 2 ) );
				}

				// now insert points to polygon, avoiding points with same coordinates
				int i_vert = 0;
				edge_t* edge = face->edge;
				do
				{
					const carve::geom::vector<3>& v = edge->vert->v;
					edge = edge->next;
					int vertex_index = poly_cache.addPoint( v );
					map_merged_idx[i_vert] = vertex_index;
					++i_vert;
				} while( edge != face->edge );

				//std::vector<carve::mesh::Vertex<3>* > verts;
				//face->getVertices( verts );

				// TODO: merge coplanar faces and re-triangulate

				for( size_t i = 0; i != triangulated.size(); ++i )
				{
					carve::triangulate::tri_idx triangle = triangulated[i];
					int a = triangle.a;
					int b = triangle.b;
					int c = triangle.c;

					int vertex_id_a = map_merged_idx[a];
					int vertex_id_b = map_merged_idx[b];
					int vertex_id_c = map_merged_idx[c];

					if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
					{
						continue;
					}

#ifdef _DEBUG
					const carve::poly::Vertex<3>& v_a = poly_cache.m_poly_data->getVertex( vertex_id_a );
					const carve::poly::Vertex<3>& v_b = poly_cache.m_poly_data->getVertex( vertex_id_b );

					double dx = v_a.v[0] - v_b.v[0];
					if( std::abs( dx ) < 0.0000001 )
					{
						double dy = v_a.v[1] - v_b.v[1];
						if( std::abs( dy ) < 0.0000001 )
						{
							double dz = v_a.v[2] - v_b.v[2];
							if( std::abs( dz ) < 0.0000001 )
							{
								std::cerr << "abs(dx) < 0.00001 && abs(dy) < 0.00001 && abs(dz) < 0.00001\n";
							}
						}
					}
#endif
					poly_cache.m_poly_data->addFace( vertex_id_a, vertex_id_b, vertex_id_c );
				}
			}
		}

		meshset.reset();
		meshset = shared_ptr<meshset_t >( poly_cache.m_poly_data->createMesh( carve::input::opts() ) );
	}
	inline void simplifyMesh( shared_ptr<meshset_t >& meshset, bool triangulate, StatusCallback* report_callback, IfcPPEntity* entity )
	{
		carve::mesh::MeshSimplifier simplifier;
		//double min_colinearity = m_geom_settings->m_min_colinearity;
		//double min_delta_v = m_geom_settings->m_min_delta_v;
		//double min_normal_angle = m_geom_settings->m_min_normal_angle;
		//double min_length = 0.0001;//m_geom_settings->m_min_length;

		//try
		//{
		//      simplifier.removeFins(meshset.get());
		//      //simplifier.cleanFaceEdges( meshset.get() );
		//      //simplifier.removeRemnantFaces( meshset.get() );
		//      //simplifier.mergeCoplanarFaces( meshset.get(), 0.0 );
		//      //simplifier.eliminateShortEdges( meshset.get(), min_length );
		//      //simplifier.removeFins(meshset.get());
		//      simplifier.simplify( meshset.get(), min_colinearity, min_delta_v, min_normal_angle, min_length );
		//      simplifier.removeFins(meshset.get());
		//      //simplifier.removeLowVolumeManifolds(meshset, 0.01);
		//      simplifier.improveMesh( meshset.get(), m_geom_settings->m_min_colinearity, m_geom_settings->m_min_delta_v, m_geom_settings->m_min_normal_angle );
		//}
		//catch(...)
		//{
		//      std::cout << "simplifier.eliminateShortEdges failed." << std::endl;
		//}

		if( !meshset )
		{
			return;
		}

		if( meshset->vertex_storage.size() < 9 )
		{
			return;
		}

		std::stringstream err;
		bool meshset_ok = checkMeshSetValidAndClosed( meshset, report_callback, entity );
		if( !meshset_ok )
		{
#ifdef _DEBUG
			std::cout << err.str().c_str() << std::endl;
#endif
			return;
		}
		shared_ptr<meshset_t > meshset_copy( meshset->clone() );
		simplifier.removeLowVolumeManifolds( meshset.get(), 0.000000001 );

		if( meshset->meshes.size() < 1 )
		{
			return;
		}

		int num_faces = getNumFaces( meshset.get() );
		size_t modifications_coplanar = simplifier.mergeCoplanarFaces( meshset.get(), 0.00001 );
		if( modifications_coplanar > 0 )
		{
			int num_faces_post_merge = getNumFaces( meshset.get() );
			if( num_faces_post_merge + modifications_coplanar != num_faces )
			{
#ifdef _DEBUG
				std::cout << "num_faces_post_merge + modifications_coplanar != num_faces" << std::endl;
#endif
			}
		}

		bool faces_ok = checkFaceIntegrity( meshset );
		if( !faces_ok )
		{
			meshset = meshset_copy;
#ifdef _DEBUG
			std::cout << "Error in simplifier.mergeCoplanarFaces" << std::endl;
#endif
			return;
		}

		meshset_copy = shared_ptr<meshset_t >( meshset->clone() );
		mergeAlignedEdges( meshset, simplifier );

		faces_ok = checkFaceIntegrity( meshset );
		if( !faces_ok )
		{
			meshset = meshset_copy;
#ifdef _DEBUG
			std::cout << "Error in mergeAlignedEdges" << std::endl;
#endif
			return;
		}

		simplifier.removeRemnantFaces( meshset.get() );
		//simplifier.cleanFaceEdges(meshset.get());
#ifdef _DEBUG
		meshset_copy = shared_ptr<meshset_t >( meshset->clone() );
		bool meshset_ok_pre_triang = checkMeshSetValidAndClosed( meshset, report_callback, entity );
		if( !meshset_ok_pre_triang )
		{
			//meshset = meshset_copy;
			std::cout << "Error in removeRemnantFaces" << std::endl;
		}
#endif

		removeFins( meshset );
		simplifier.cleanFaceEdges( meshset.get() );

		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			meshset->meshes[i]->cacheEdges();
		}

		std::stringstream err_simplified;
		bool simplified_meshset_ok = checkMeshSetValidAndClosed( meshset, report_callback, entity );
		if( !simplified_meshset_ok )
		{
#ifdef _DEBUG
			std::cout << err_simplified.str().c_str() << std::endl;
			GeomDebugUtils::dumpMeshset( meshset, carve::geom::VECTOR( 0.3, 0.4, 0.5, 1.0 ), true );
#endif
			meshset = meshset_copy;
			return;
		}

		simplifier.removeLowVolumeManifolds( meshset.get(), 0.0 );

		if( triangulate )
		{
			retriangulateMeshSet( meshset );
		}

		std::stringstream err_retriangulated;
		bool retriangulated_meshset_ok = checkMeshSetValidAndClosed( meshset, report_callback, entity );
		if( !retriangulated_meshset_ok )
		{
#ifdef _DEBUG
			std::cout << err_retriangulated.str().c_str() << std::endl;

			shared_ptr<meshset_t > meshset_pre_triang( meshset_copy->clone() );
			//applyTranslate( meshset_pre_triang.get(), carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
			GeomDebugUtils::dumpMeshset( meshset_pre_triang, carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
			//dump_y_pos += meshset_pre_triang->getAABB().extent.y*2.2;

			shared_ptr<meshset_t > meshset_post_triang( meshset->clone() );
			//applyTranslate( meshset_post_triang.get(), carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
			GeomDebugUtils::dumpMeshset( meshset_post_triang, carve::geom::VECTOR( 0.3, 0.4, 0.5, 1.0 ), true );
			//dump_y_pos += meshset_post_triang->getAABB().extent.y*2.2;

#endif
			meshset = meshset_copy;
		}

#ifdef _DEBUG
		++simplify_count;
#endif
	}

	inline void computeCSG( shared_ptr<meshset_t >& op1, shared_ptr<meshset_t >& op2, const carve::csg::CSG::OP operation, shared_ptr<meshset_t >& result, StatusCallback* report_callback,
		IfcPPEntity* entity1, IfcPPEntity* entity2 )
	{
#ifdef ROUND_MESH_COORDS
		MeshOps::roundVertices( op1.get() );
		MeshOps::roundVertices( op1.get() );
#endif

		std::stringstream strs_err;
		try
		{
			if( !checkMeshSetValidAndClosed( op1, report_callback, entity1 ) )
			{
				if( operation == carve::csg::CSG::B_MINUS_A )
				{
					result = op2;
				}
				else if( operation == carve::csg::CSG::UNION )
				{
					result = op2;
				}
#ifdef _DEBUG
				GeomDebugUtils::dumpMeshset( op1, carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
#endif
				return;
			}

			if( !checkMeshSetValidAndClosed( op2, report_callback, entity2 ) )
			{
				if( operation == carve::csg::CSG::A_MINUS_B )
				{
					result = op1;
				}
				else if( operation == carve::csg::CSG::UNION )
				{
					result = op1;
				}
#ifdef _DEBUG
				GeomDebugUtils::dumpMeshset( op2, carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
#endif
				return;
			}

			simplifyMesh( op1, false, report_callback, entity1 );
			simplifyMesh( op2, false, report_callback, entity2 );
			// TODO: Subclass from carve::mesh::MeshSet and add attribute to remember which meshset has already been simplified. 

			// check if meshset aabb is far away from origin. if so, move to origin, compute, move back
			carve::geom::vector<3> translate_avoid_large_numbers;
			const carve::geom::aabb<3>& aabb_op1 = op1->getAABB();
			if( aabb_op1.pos.length2() > 10000 )
			{
				const carve::geom::aabb<3>& aabb_op2 = op2->getAABB();

				if( aabb_op2.pos.length2() > 10000 )
				{
					carve::geom::vector<3> aabb_op1_direction( aabb_op1.pos );
					aabb_op1_direction.normalize();

					carve::geom::vector<3> aabb_op2_direction( aabb_op2.pos );
					aabb_op2_direction.normalize();

					double cos_angle = dot( aabb_op1_direction, aabb_op2_direction );
					if( cos_angle > -0.5 )
					{
						// if close to -1, the bboxes are in opposite direction, not useful to translate
						// if close to 1, the bboxes are somewhere in the same direction, good to translate

						// check extent
						if( aabb_op1.extent.length2() < 1000 && aabb_op2.extent.length2() < 1000 )
						{
							if( aabb_op1.pos.length2() > aabb_op2.pos.length2() )
							{
								// TODO: take biggest |x|, biggest |y|, biggest |z|
								translate_avoid_large_numbers = carve::geom::VECTOR( aabb_op1.pos.x, aabb_op1.pos.y, aabb_op1.pos.z );
							}
							else
							{
								translate_avoid_large_numbers = carve::geom::VECTOR( aabb_op2.pos.x, aabb_op2.pos.y, aabb_op2.pos.z );
							}
						}
					}
				}
			}

			if( translate_avoid_large_numbers.length2() > 1000.0 )
			{
				GeomUtils::applyTranslate( op1, -translate_avoid_large_numbers );
				GeomUtils::applyTranslate( op2, -translate_avoid_large_numbers );
			}

			if( !checkMeshSetValidAndClosed( op1, report_callback, entity1 ) )
			{
				if( operation == carve::csg::CSG::B_MINUS_A )
				{
					result = op2;
				}
				else if( operation == carve::csg::CSG::UNION )
				{
					result = op2;
				}
#ifdef _DEBUG
				GeomDebugUtils::dumpMeshset( op1, carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
#endif
				return;
			}

			if( !checkMeshSetValidAndClosed( op2, report_callback, entity2 ) )
			{
				if( operation == carve::csg::CSG::A_MINUS_B )
				{
					result = op1;
				}
				else if( operation == carve::csg::CSG::UNION )
				{
					result = op1;
				}
#ifdef _DEBUG
				GeomDebugUtils::dumpMeshset( op2, carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
#endif
				return;
			}

			carve::csg::CSG csg;
			//csg.hooks.registerHook(new carve::csg::CarveTriangulator(), carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);
			//csg.hooks.registerHook(new carve::csg::CarveTriangulatorWithImprovement(), carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);
			//csg.hooks.registerHook(new carve::csg::CarveHoleResolver(), carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);

			result = shared_ptr<meshset_t >( csg.compute( op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE ) );

			bool result_meshset_ok = false;
			if( result )
			{
				if( result->meshes.size() == 0 )
				{
					if( operation == carve::csg::CSG::A_MINUS_B )
					{
						result = op1;
						result_meshset_ok = true;
					}
					else if( operation == carve::csg::CSG::B_MINUS_A )
					{
						result = op2;
						result_meshset_ok = true;
					}
					else
					{
						result.reset();
					}
				}
				else
				{
					result_meshset_ok = true;
					bool result_mesh_closed = checkMeshSetValidAndClosed( result, report_callback, entity1 );
					if( !result_mesh_closed )
					{
						result_meshset_ok = false;
					}
				}
			}

			if( result_meshset_ok )
			{
				simplifyMesh( result, true, report_callback, entity1 );
			}
			else
			{
				strs_err << "csg operation failed, Entity ID 1: " << entity1 << ", Entity ID 2:" << entity2 << std::endl;
			}

			if( translate_avoid_large_numbers.length2() > 1000.0 )
			{
				if( result )
				{
					GeomUtils::applyTranslate( result, translate_avoid_large_numbers );
				}
				GeomUtils::applyTranslate( op1, translate_avoid_large_numbers );
				GeomUtils::applyTranslate( op2, translate_avoid_large_numbers );
			}
		}
#ifdef _DEBUG
		catch( DebugBreakException& e )
		{
			throw e;
		}
#endif
		catch( carve::exception& ce )
		{
			strs_err << ce.str().c_str();
		}
		catch( const std::out_of_range& oor )
		{
			strs_err << oor.what();
		}
		catch( std::exception& e )
		{
			strs_err << e.what();
		}
		catch( ... )
		{
			strs_err << "csg operation failed, Entity ID 1: " << entity1 << ", Entity ID 2:" << entity2 << std::endl;
		}

		if( strs_err.tellp() > 0 )
		{
#ifdef _DEBUG

			shared_ptr<meshset_t > op1_copy( op1->clone() );
			GeomUtils::applyTranslate( op1_copy, carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
			GeomDebugUtils::dumpMeshset( op1_copy, carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
			dump_y_pos += op1_copy->getAABB().extent.y*2.2;

			shared_ptr<meshset_t > op2_copy( op2->clone() );
			GeomUtils::applyTranslate( op2_copy, carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
			GeomDebugUtils::dumpMeshset( op2_copy, carve::geom::VECTOR( 0.6, 0.2, 0.2, 1.0 ), true );
			dump_y_pos += op2_copy->getAABB().extent.y*2.2;

			if( result )
			{
				shared_ptr<meshset_t > result_copy( result->clone() );
				GeomUtils::applyTranslate( result_copy, carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
				GeomDebugUtils::dumpMeshset( result_copy, carve::geom::VECTOR( 0.4, 0.7, 0.4, 1.0 ), true );
				dump_y_pos += result_copy->getAABB().extent.y*2.2;
			}
#endif

			if( operation == carve::csg::CSG::A_MINUS_B )
			{
				result = op1;
			}
			else if( operation == carve::csg::CSG::B_MINUS_A )
			{
				result = op2;
			}
			else if( operation == carve::csg::CSG::UNION )
			{
				result = op1;
			}
			else
			{
				result.reset();
			}
		}
	}
};

#include <osgDB/fstream>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/IfcPPOpenMP.h>

#include "ConverterOSG.h"
#include "GeometryInputData.h"
#include "GeometryException.h"
#include "CSG_Adapter.h"

template<typename T>
int findInVector( T* find_object, std::vector<T*>& vec )
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

void mergeAlignedEdges( shared_ptr<carve::mesh::MeshSet<3> >& meshset, carve::mesh::MeshSimplifier& simplifier )
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
			if( edge_i == reinterpret_cast<carve::mesh::Edge<3>*>( 0xfeeefeeefeeefeee ) )
			{
				std::cout << __func__ << ": !edge_i" << std::endl;
				continue;
			}
			if( edge_next == reinterpret_cast<carve::mesh::Edge<3>*>( 0xfeeefeeefeeefeee ) )
			{
				std::cout << __func__ << ": !edge_next" << std::endl;
				continue;
			}
			if( edge_next->rev == reinterpret_cast<carve::mesh::Edge<3>*>( 0xfeeefeeefeeefeee ) )
			{
				std::cout << __func__ << ": !edge_j->rev" << std::endl;
				continue;
			}
#endif

			if( !edge_i )
			{
#ifdef _DEBUG
				std::cout << __func__ << ": !edge_i" << std::endl;
#endif
				continue;
			}

			if( !edge_next )
			{
#ifdef _DEBUG
				std::cout << __func__ << ": !edge_next" << std::endl;
#endif
				continue;
			}

			if( !edge_next->rev )
			{
#ifdef _DEBUG
				std::cout << __func__ << ": !edge_j->rev" << std::endl;
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
					std::cout << __func__ << ": abs( edge_i->length2() - sement12_length2 ) > 0.00001" << std::endl;
				}

				if( std::abs( edge_next->length2() - sement23_length2 ) > 0.00001 )
				{
					std::cout << __func__ << ": abs( edge_next->length2() - sement23_length2 ) > 0.00001" << std::endl;
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
						std::cout << __func__ << ": edge loop with only 2 edges" << std::endl;
#endif
					}

					// this links previous and next edges and deletes edge_j and reverse of edge_i:
					edge_t* edge_next_rev = edge_next->rev;

					int edge_remove_idx = findInVector( edge_next, vec_closed_edges );
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
						std::cout << __func__ << ": abs( dot_face_angle - 1.0 ) > 0.001" << std::endl;
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

void removeFins( shared_ptr<carve::mesh::MeshSet<3> >& meshset )
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


void CSG_Adapter::retriangulateMeshSet( shared_ptr<carve::mesh::MeshSet<3> >& meshset )
{
	if( !meshset )
	{
		return;
	}

	bool already_triagulated = true;
	for( size_t ii = 0; ii < meshset->meshes.size(); ++ii )
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
		std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
		for( size_t i2 = 0; i2 < vec_faces.size(); ++i2 )
		{
			carve::mesh::Face<3>* face = vec_faces[i2];
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
		std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;

		for( size_t i2 = 0; i2 < vec_faces.size(); ++i2 )
		{
			carve::mesh::Face<3>* face = vec_faces[i2];
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
			carve::mesh::Edge<3>* edge = face->edge;
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
	meshset = shared_ptr<carve::mesh::MeshSet<3> >( poly_cache.m_poly_data->createMesh( carve::input::opts() ) );
}

bool CSG_Adapter::checkFaceIntegrity( const carve::mesh::MeshSet<3>* meshset )
{
	for( size_t i = 0; i < meshset->meshes.size(); ++i )
	{
		carve::mesh::Mesh<3>* mesh_i = meshset->meshes[i];
		if( mesh_i->open_edges.size() > 0 )
		{
			//return false;
		}
		std::vector<carve::mesh::Face<3>* >& vec_faces = mesh_i->faces;
		for( size_t j = 0; j < vec_faces.size(); ++j )
		{
			carve::mesh::Face<3>* face = vec_faces[j];
			carve::mesh::Edge<3>* e = face->edge;
			const size_t n_edges = face->n_edges;
			for( size_t i_edge = 0; i_edge < n_edges; ++i_edge )
			{
				if( e == reinterpret_cast<carve::mesh::Edge<3>*>( 0xfeeefeeefeeefeee ) )
				{
					return false;
				}
				if( !e->rev )
				{
#ifdef _DEBUG
					std::vector<face_t*> vec_faces;
					vec_faces.push_back( face );
					dumpFaces( meshset, vec_faces );

					std::vector<edge_t*> vec_edges;
					vec_edges.push_back( e );
					dumpEdges( meshset, vec_edges );
#endif
					return false;
				}
				if( e->rev->next == reinterpret_cast<carve::mesh::Edge<3>*>( 0xfeeefeeefeeefeee ) )
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
				if( e->next == reinterpret_cast<carve::mesh::Edge<3>*>( 0xfeeefeeefeeefeee ) )
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

bool CSG_Adapter::checkMeshSetValidAndClosed( const carve::mesh::MeshSet<3>* mesh_set, std::stringstream& err_poly, int entity_id )
{
	if( !mesh_set )
	{
#ifdef _DEBUG
		std::cout << "MeshSet of entity #" << entity_id << " not valid" << std::endl;
#endif
		return false;
	}
	if( mesh_set->meshes.size() == 0 )
	{
#ifdef _DEBUG
		std::cout << "MeshSet of entity #" << entity_id << " has no meshes" << std::endl;
#endif
		return false;
	}

	if( !checkFaceIntegrity( mesh_set ) )
	{
		return false;
	}

	std::stringstream err;
	bool meshes_closed = true;
	for( size_t i = 0; i < mesh_set->meshes.size(); ++i )
	{
		carve::mesh::Mesh<3>* mesh_i = mesh_set->meshes[i];
		if( !mesh_i )
		{
			err << "meshes[i] is nullptr" << std::endl;
			continue;
		}

		if( mesh_i->isNegative() )
		{
			mesh_i->invert();
			if( mesh_i->isNegative() )
			{
				mesh_i->recalc();
				mesh_i->calcOrientation();
				if( mesh_i->isNegative() )
				{
					err << "mesh_set->meshes[" << i << "]->isNegative() " << std::endl;
				}
			}
		}

		if( !mesh_i->isClosed() )
		{
			meshes_closed = false;
			break;
		}

		if( mesh_i->open_edges.size() > 0 )
		{
			meshes_closed = false;
			break;
		}
	}

	if( !meshes_closed )
	{
		err << "mesh_set not closed" << std::endl;
	}

#ifdef _DEBUG
	if( meshes_closed )
	{
		// check volume
		double object_volume = CSG_Adapter::getVolume( mesh_set );
		if( object_volume <= 0 )
		{
			err << "object_volume <= 0" << std::endl;
		}
	}
#endif

	if( err.tellp() > 0 )
	{
#ifdef _DEBUG
		err_poly << "MeshSet of entity #" << entity_id << " has problems:" << std::endl;
		err_poly << err.str().c_str();
#endif
		return false;
	}
	return true;
}

#ifdef _DEBUG
int simplify_count = 0;
double dump_y_pos = 0;
#endif

void CSG_Adapter::simplifyMesh( shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool triangulate )
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
	bool meshset_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), err, -1 );
	if( !meshset_ok )
	{
#ifdef _DEBUG
		std::cout << err.str().c_str() << std::endl;
#endif
		return;
	}
	shared_ptr<carve::mesh::MeshSet<3> > meshset_copy( meshset->clone() );
	simplifier.removeLowVolumeManifolds( meshset.get(), 0.000001 );

	if( meshset->meshes.size() < 1 )
	{
		return;
	}

	int num_faces = CSG_Adapter::getNumFaces( meshset.get() );
	size_t modifications_coplanar = simplifier.mergeCoplanarFaces( meshset.get(), 0.00001 );
	if( modifications_coplanar > 0 )
	{
		int num_faces_post_merge = CSG_Adapter::getNumFaces( meshset.get() );
		if( num_faces_post_merge + modifications_coplanar != num_faces )
		{
#ifdef _DEBUG
			std::cout << "num_faces_post_merge + modifications_coplanar != num_faces" << std::endl;
#endif
		}
	}

	bool faces_ok = checkFaceIntegrity( meshset.get() );
	if( !faces_ok )
	{
		meshset = meshset_copy;
#ifdef _DEBUG
		std::cout << "Error in simplifier.mergeCoplanarFaces" << std::endl;
#endif
		return;
	}

	meshset_copy = shared_ptr<carve::mesh::MeshSet<3> >( meshset->clone() );
	mergeAlignedEdges( meshset, simplifier );

	faces_ok = checkFaceIntegrity( meshset.get() );
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
	meshset_copy = shared_ptr<carve::mesh::MeshSet<3> >( meshset->clone() );
	bool meshset_ok_pre_triang = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), err, -1 );
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
	bool simplified_meshset_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), err_simplified, -1 );
	if( !simplified_meshset_ok )
	{
#ifdef _DEBUG
		std::cout << err_simplified.str().c_str() << std::endl;
		CSG_Adapter::dumpMeshset( meshset.get(), carve::geom::VECTOR( 0.3, 0.4, 0.5, 1.0 ), true );
		//throw DebugBreakException( "!simplified_meshset_ok" );
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
	bool retriangulated_meshset_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), err_retriangulated, -1 );
	if( !retriangulated_meshset_ok )
	{
#ifdef _DEBUG
		std::cout << err_retriangulated.str().c_str() << std::endl;

		shared_ptr<carve::mesh::MeshSet<3> > meshset_pre_triang( meshset_copy->clone() );
		//CSG_Adapter::applyTranslate( meshset_pre_triang.get(), carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
		CSG_Adapter::dumpMeshset( meshset_pre_triang.get(), carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
		//dump_y_pos += meshset_pre_triang->getAABB().extent.y*2.2;

		shared_ptr<carve::mesh::MeshSet<3> > meshset_post_triang( meshset->clone() );
		//CSG_Adapter::applyTranslate( meshset_post_triang.get(), carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
		CSG_Adapter::dumpMeshset( meshset_post_triang.get(), carve::geom::VECTOR( 0.3, 0.4, 0.5, 1.0 ), true );
		//dump_y_pos += meshset_post_triang->getAABB().extent.y*2.2;

#endif
		meshset = meshset_copy;
	}

#ifdef _DEBUG
	++simplify_count;
#endif
}




void CSG_Adapter::computeCSG( shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation,
	const int entity1, const int entity2, std::stringstream& err, shared_ptr<carve::mesh::MeshSet<3> >& result )
{
#ifdef ROUND_MESH_COORDS
	MeshOps::roundVertices( op1.get() );
	MeshOps::roundVertices( op1.get() );
#endif

	if( !op1 )
	{
		return;
	}
	
	if( !op2 )
	{
		return;
	}

	bool csg_op_ok = true;
	try
	{
		bool meshset1_ok = CSG_Adapter::checkMeshSetValidAndClosed( op1.get(), err, entity1 );
		if( !meshset1_ok )
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
			CSG_Adapter::dumpMeshset( op1.get(), carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
#endif
			return;
		}

		bool meshset2_ok = CSG_Adapter::checkMeshSetValidAndClosed( op2.get(), err, entity2 );
		if( !meshset2_ok )
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
			CSG_Adapter::dumpMeshset( op2.get(), carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
#endif
			return;
		}

		simplifyMesh( op1, false );
		simplifyMesh( op2, false );
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
			CSG_Adapter::applyTranslate( op1.get(), -translate_avoid_large_numbers );
			CSG_Adapter::applyTranslate( op2.get(), -translate_avoid_large_numbers );
		}

		carve::csg::CSG csg;
		//csg.hooks.registerHook(new carve::csg::CarveTriangulator(), carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);
		//csg.hooks.registerHook(new carve::csg::CarveTriangulatorWithImprovement(), carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);
		//csg.hooks.registerHook(new carve::csg::CarveHoleResolver(), carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);

		result = shared_ptr<carve::mesh::MeshSet<3> >( csg.compute( op1.get(), op2.get(), operation, nullptr, carve::csg::CSG::CLASSIFY_EDGE ) );

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
				bool result_mesh_closed = CSG_Adapter::checkMeshSetValidAndClosed( result.get(), err, -1 );
				if( !result_mesh_closed )
				{
					result_meshset_ok = false;
				}
			}
		}

		if( result_meshset_ok )
		{
			simplifyMesh( result, true );
		}
		else
		{
			csg_op_ok = false;
#ifdef _DEBUG
			std::cout << "csg.compute result nok ok." << std::endl;
#endif
		}

		if( translate_avoid_large_numbers.length2() > 1000.0 )
		{
			if( result )
			{
				CSG_Adapter::applyTranslate( result.get(), translate_avoid_large_numbers );
			}
			CSG_Adapter::applyTranslate( op1.get(), translate_avoid_large_numbers );
			CSG_Adapter::applyTranslate( op2.get(), translate_avoid_large_numbers );
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
		err << "csg operation failed, Entity ID 1: " << entity1 << ", Entity ID 2:" << entity2 << ", ";
		err << ce.str() << std::endl;
		csg_op_ok = false;
	}
	catch( const std::out_of_range& oor )
	{
		err << "csg operation failed, Entity ID 1:" << entity1 << ", Entity ID 2:" << entity2 << ", ";
		err << oor.what() << std::endl;
		csg_op_ok = false;
	}
	catch( std::exception& e )
	{
		err << "csg operation failed, Entity ID 1:" << entity1 << ", Entity ID 2:" << entity2 << ", ";
		err << e.what() << std::endl;
		csg_op_ok = false;
	}
	catch( ... )
	{
		err << "csg operation failed, Entity ID 1:" << entity1 << ", Entity ID 2:" << entity2 << std::endl;
		csg_op_ok = false;
	}

#ifdef _DEBUG
	if( !csg_op_ok )
	{
		std::cout << "!csg_operation_ok. Entity ID 1:" << entity1 << ", Entity ID 2:" << entity2 << std::endl;

		shared_ptr<carve::mesh::MeshSet<3> > op1_copy( op1->clone() );
		//CSG_Adapter::applyTranslate( op1_copy.get(), carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
		CSG_Adapter::dumpMeshset( op1_copy.get(), carve::geom::VECTOR( 0.7, 0.7, 0.7, 1.0 ), true );
		//dump_y_pos += op1_copy->getAABB().extent.y*2.2;

		shared_ptr<carve::mesh::MeshSet<3> > op2_copy( op2->clone() );
		CSG_Adapter::applyTranslate( op2_copy.get(), carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
		CSG_Adapter::dumpMeshset( op2_copy.get(), carve::geom::VECTOR( 0.6, 0.2, 0.2, 1.0 ), true );
		dump_y_pos += op2_copy->getAABB().extent.y*2.2;

		if( result )
		{
			shared_ptr<carve::mesh::MeshSet<3> > result_copy( result->clone() );
			CSG_Adapter::applyTranslate( result_copy.get(), carve::geom::VECTOR( 0, dump_y_pos, 0 ) );
			CSG_Adapter::dumpMeshset( result_copy.get(), carve::geom::VECTOR( 0.4, 0.7, 0.4, 1.0 ), true );
			dump_y_pos += result_copy->getAABB().extent.y*2.2;
		}
	}
#endif

	if( !csg_op_ok )
	{
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


void CSG_Adapter::applyTranslate( carve::mesh::MeshSet<3>* meshset, const carve::geom::vector<3>& pos )
{
	for( size_t i = 0; i < meshset->vertex_storage.size(); ++i )
	{
		carve::geom::vector<3>& point = meshset->vertex_storage[i].v;
		point = point + pos;
	}
	for( size_t i = 0; i < meshset->meshes.size(); ++i )
	{
		meshset->meshes[i]->recalc();
	}
}
void CSG_Adapter::roundVertices( carve::mesh::MeshSet<3>* meshset )
{
	std::vector<carve::mesh::Vertex<3> >& vertex_storage = meshset->vertex_storage;
	const size_t num_vertices = vertex_storage.size();
	for( int i = 0; i < num_vertices; ++i )
	{
		carve::mesh::Vertex<3>& vertex = vertex_storage[i];
		vertex.v.x = round( vertex.v.x*1000000.0 ) * 0.000001;
		vertex.v.y = round( vertex.v.y*1000000.0 ) * 0.000001;
		vertex.v.z = round( vertex.v.z*1000000.0 ) * 0.000001;
	}
}
int CSG_Adapter::getNumFaces( const carve::mesh::MeshSet<3>* meshset )
{
	int num_faces = 0;
	for( int i = 0; i < meshset->meshes.size(); ++i )
	{
		num_faces += meshset->meshes[i]->faces.size();
	}
	return num_faces;
}
int CSG_Adapter::getNumClosedEdges( const carve::mesh::MeshSet<3>* meshset )
{
	int num_edges = 0;
	for( int i = 0; i < meshset->meshes.size(); ++i )
	{
		num_edges += meshset->meshes[i]->closed_edges.size();
	}
	return num_edges;
}
double CSG_Adapter::getVolume( const carve::mesh::MeshSet<3>* meshset )
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



#ifdef _DEBUG
void Polyhedron2Stream( carve::poly::Polyhedron* poly, carve::geom::vector<4>& color, std::stringstream& strs_out )
{
	strs_out << "Polyhedron{" << std::endl;
	strs_out << "color{" << color.x << ", " << color.y << ", " << color.z << ", " << color.w << "}" << std::endl;
	strs_out << "vertices{" << std::endl;
	const size_t num_vertices = poly->vertices.size();
	for( int i = 0; i < num_vertices; ++i )
	{
		carve::poly::Vertex<3> vertex = poly->vertices[i];
		if( i > 0 )
		{
			strs_out << ",";
		}
		strs_out << "{" << vertex.v.x << ", " << vertex.v.y << ", " << vertex.v.z << "}";
	}
	strs_out << "}" << std::endl;

	strs_out << "faces{" << std::endl;
	for( int i = 0; i < poly->faces.size(); ++i )
	{
		carve::poly::Face<3> f = poly->faces[i];
		if( i > 0 )
		{
			strs_out << ",";
		}
		strs_out << "{";
		for( int j = 0; j < f.nVertices(); ++j )
		{
			if( j > 0 )
			{
				strs_out << ", ";
			}
			strs_out << poly->vertexToIndex( f.vertex( j ) );
		}
		strs_out << "}" << std::endl;
	}
	strs_out << std::endl << "}" << std::endl;
}

void CSG_Adapter::dumpMeshset( carve::mesh::MeshSet<3>* meshset, carve::geom::vector<4>& color, bool append )
{
	if( meshset->meshes.size() == 0 )
	{
		return;
	}
	applyTranslate( meshset, carve::geom::VECTOR( 0, dump_y_pos, 0 ) );

	shared_ptr<carve::poly::Polyhedron> poly( carve::polyhedronFromMesh( meshset, -1 ) );
	dumpPolyhedron( poly.get(), color, append );

	dump_y_pos += meshset->getAABB().extent.y*2.2;
}

void CSG_Adapter::dumpMeshsets( std::vector<carve::mesh::MeshSet<3>* >& vec_meshsets, std::vector<carve::geom::vector<4> >& vec_colors, bool append )
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
		Polyhedron2Stream( poly.get(), color, strs_out );
	}

	if( !append )
	{
		clearMeshsetDump();
	}
	osgDB::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
	dump_ofstream << strs_out.str().c_str();
	dump_ofstream.close();
}

void CSG_Adapter::dumpPolyhedronInput( carve::input::PolyhedronData& poly_input, carve::geom::vector<4>& color, bool append )
{
	dumpPolyhedron( poly_input.create( carve::input::opts() ), color, append );
}

void CSG_Adapter::dumpPolyhedron( carve::poly::Polyhedron* poly, carve::geom::vector<4>& color, bool append )
{
	std::stringstream strs_out;
	Polyhedron2Stream( poly, color, strs_out );

	if( !append )
	{
		clearMeshsetDump();
	}

	std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
	dump_ofstream << strs_out.str().c_str();
	dump_ofstream.close();
}

void CSG_Adapter::dumpFaces( const carve::mesh::MeshSet<3>* meshset, std::vector<face_t* >& vec_faces )
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
		strs_out << "{" << vertex.v.x << ", " << vertex.v.y + dump_y_pos << ", " << vertex.v.z << "}";
	}
	strs_out << "}" << std::endl;

	strs_out << "faces{" << std::endl;
	for( size_t i = 0; i < vec_faces.size(); ++i )
	{
		carve::mesh::Face<3>* f = vec_faces[i];
		if( i > 0 )
		{
			strs_out << ",";
		}
		strs_out << "{";
		edge_t* edge = f->edge;
		for( int j = 0; j < f->nEdges(); ++j )
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
	strs_out << std::endl << "}";

	std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
	dump_ofstream << strs_out.str().c_str();
	dump_ofstream.close();

	dump_y_pos += meshset->getAABB().extent.y*2.2;
}

void CSG_Adapter::dumpEdges( const carve::mesh::MeshSet<3>* meshset, std::vector<edge_t* >& vec_edges )
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
	strs_out << "}" << std::endl;

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
	strs_out << std::endl << "}";

	std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
	dump_ofstream << strs_out.str().c_str();
	dump_ofstream.close();
}


void CSG_Adapter::clearMeshsetDump()
{
	std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::trunc );
	dump_ofstream.close();
}

#endif

#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/IfcPPOpenMP.h>

#include "DebugViewerCallback.h"
#include "ConverterOSG.h"
#include "GeometryInputData.h"
#include "GeometryException.h"
#include "CSG_Adapter.h"


void mergeAlignedEdges( shared_ptr<carve::mesh::MeshSet<3> >& meshset )
{
	if( !meshset )
	{
		return;
	}
	bool cache_dirty = false;

	std::map<face_t*, std::vector<edge_t*> > map_omit_face_edges;
	for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

		std::vector<edge_t*>& vec_closed_edges = mesh->closed_edges;
		for( size_t closed_edge_i = 0; closed_edge_i < vec_closed_edges.size(); ++closed_edge_i )
		{
			edge_t* edge_i = vec_closed_edges[closed_edge_i];

			// edge i and j are in line, check if connected directly
			edge_t* edge_next = edge_i->next;
			if( !edge_next )
			{
				std::cout << "!edge_next" << std::endl;
				continue;
			}

			if( edge_next == reinterpret_cast<carve::mesh::Edge<3>*>(0xfeeefeeefeeefeee) )
			{
				std::cout << "!edge_next" << std::endl;
				continue;
			}

			if( !edge_next->rev )
			{
				std::cout << "!edge_j->rev" << std::endl;
				continue;
			}
					
			if( edge_next->rev == reinterpret_cast<carve::mesh::Edge<3>*>(0xfeeefeeefeeefeee) )
			{
				std::cout << "!edge_j->rev" << std::endl;
				continue;
			}

					
			if( edge_next->rev->next == edge_i->rev )
			{
				// --->o--->			 --->o--->
				//     |^                    |^
				//     ||edge_next(x)        ||
				//     v|                    || 
				//     o                     ||
				//     |^                    ||
				//  (x)||edge_i	             ||edge_i
				//     v|				     v|
				// <---o<---			 <---o<---

				int remove_count = 0;
				for( size_t closed_edge_remove = 0; closed_edge_remove < vec_closed_edges.size(); ++closed_edge_remove )
				{
					//const carve::geom::vector<3>& linesegment_i = vec_edge_segments[closed_edge_i];
					edge_t* edge_check = vec_closed_edges[closed_edge_remove];

					if( edge_next == edge_check )
					{
						vec_closed_edges.erase( vec_closed_edges.begin() + closed_edge_remove );
						--closed_edge_remove;
						++remove_count;
						if( remove_count == 2 )
						{
							break;
						}
					}

					if( edge_i->rev == edge_check )
					{
						vec_closed_edges.erase( vec_closed_edges.begin() + closed_edge_remove );
						--closed_edge_remove;
						++remove_count;
						if( remove_count == 2 )
						{
							break;
						}
					}
				}

				// this links previous and next edges and deletes edge_j and reverse of edge_i:
				edge_t* edge_next_rev = edge_next->rev;
				edge_next->removeHalfEdge();
				edge_i->rev->removeHalfEdge();
				edge_i->rev = edge_next_rev;
				edge_next_rev->rev = edge_i;
				cache_dirty = true;
			}

			// the vertex is kept in the vertex storage, no need to delete
		}
	}
	if( cache_dirty )
	{
		double volume_check2 = 0;
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			meshset->meshes[i]->cacheEdges();
		}
	}
}


void splitFace(carve::mesh::MeshSet<3>::face_t *face,
			   const carve::csg::V2Set &edges,
			   std::list<std::vector<carve::mesh::MeshSet<3>::vertex_t *> > &face_loops,
			   std::list<std::vector<carve::mesh::MeshSet<3>::vertex_t *> > &hole_loops,
			   const carve::csg::VertexIntersections & /* vi */);

int intersection_test_count = 0;
#ifdef IFCPP_OPENMP
Mutex writelock_count;
#endif



void removeSmallAreaFaces( shared_ptr<carve::mesh::MeshSet<3> >& meshset, double min_area = 0.0001, double min_side_length = 0.001 )
{
	if( !meshset )
	{
		return;
	}
	bool cache_dirty = false;
	double min_side_length2 = min_side_length*min_side_length;

	std::map<face_t*, std::vector<edge_t*> > map_omit_face_edges;
	for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
	{
		carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

		// find edges that are in line
		std::vector<face_t*>& vec_faces = mesh->faces;
		
		for( size_t face_i = 0; face_i < vec_faces.size(); ++face_i )
		{
			face_t* face = vec_faces[face_i];
			double area = min_area;

			if( face->n_edges == 3 )
			{
				edge_t* edge = face->edge;
				edge_t* edge2 = edge->next;
				edge_t* edge3 = edge2->next;
				const carve::geom::vector<3>& v1 = edge->v1()->v;
				const carve::geom::vector<3>& v2 = edge2->v1()->v;
				const carve::geom::vector<3>& v3 = edge3->v1()->v;
				carve::geom::vector<3> sement12 = v2 - v1;
				carve::geom::vector<3> sement23 = v3 - v2;
				area = 0.5*(cross( sement12, sement23 ).length());
			}
			else
			{
				std::cout << __FUNC__ << ": I want triangles as input" << std::endl;
				continue;
			}

			if( area < min_area )
			{
				edge_t* edge = face->edge;
				edge_t* edge_max = nullptr;
				edge_t* edge_min = nullptr;
				double max_edge_length2 = 0;
				double min_edge_length2 = DBL_MAX;
				for( size_t i_edge = 0; i_edge < face->n_edges; ++i_edge )
				{
					double edge_length2 = edge->length2();
					if( edge_length2 > max_edge_length2 )
					{
						max_edge_length2 = edge_length2;
						edge_max = edge;
					}
					if( edge_length2 < min_edge_length2 )
					{
						min_edge_length2 = edge_length2;
						edge_min = edge;
					}
					edge = edge->next;
				}

				carve::geom::vector<3> centroid = face->centroid();

				if( max_edge_length2 < min_side_length2 )
				{
					//    1-------------3
					//     \    e3     /
					//      \         /
					//       \e1   e2/
					//        \     /
					//         \   /
					//          \ /
					//           2

					for( size_t i_edge = 0; i_edge < face->n_edges; ++i_edge )
					{
						edge->vert->v.x = centroid.x;
						edge->vert->v.y = centroid.y;
						edge->vert->v.z = centroid.z;

						edge = edge->next;
					}
					//face->remove();
				}
				else
				{
					// keep the longest edge

					// 3----2
					// |   |
					// |  |
					// | |
					// ||
					// 1

					carve::mesh::Vertex<3>* vert_min1 = edge_min->vert;
					carve::mesh::Vertex<3>* vert_min2 = edge_min->next->vert;

					vert_min2->v.x = vert_min1->v.x;
					vert_min2->v.y = vert_min1->v.y;
					vert_min2->v.z = vert_min1->v.z;
				}

				// if all three edges ar almost equal, move the vertices to the center and collapse the edges
			}
		}
	}
}

void removeFins( shared_ptr<carve::mesh::MeshSet<3> >& meshset )
{
	return;

	if( !meshset )
	{
		return;
	}
	bool cache_dirty = false;

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
			if( std::abs(cos_angle+1.0) > 0.000001 )
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
				edge->rev = edge_next_rev;
				edge_next_rev->rev = edge;
				cache_dirty = true;
			}
		}
	}
	if( cache_dirty )
	{
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			meshset->meshes[i]->cacheEdges();
		}
	}
}


void retriangulateMeshSet( shared_ptr<carve::mesh::MeshSet<3> >& meshset )
{
	if( !meshset )
	{
		return;
	}

	PolyInputCache3D poly_cache;
	std::map<int,int> map_merged_idx;
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
					carve::triangulate::triangulate(verts2d, triangulated);
					carve::triangulate::improve(verts2d, triangulated);
				}
				catch(...)
				{
					std::cout << __FUNC__ << " carve::triangulate failed " << std::endl;
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

				//if( a >= verts.size() || b >= verts.size() || c >= verts.size() )
				//{
				//	continue;
				//}

				//int vertex_id_a = poly_cache.addPoint( verts[a]->v );
				//int vertex_id_b = poly_cache.addPoint( verts[b]->v );
				//int vertex_id_c = poly_cache.addPoint( verts[c]->v );
				
				int vertex_id_a = map_merged_idx[a];
				int vertex_id_b = map_merged_idx[b];
				int vertex_id_c = map_merged_idx[c];

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
	meshset = shared_ptr<carve::mesh::MeshSet<3> >( poly_cache.m_poly_data->createMesh(carve::input::opts()) );
}

bool CSG_Adapter::checkFaceIntegrity(	const carve::mesh::MeshSet<3>* mesh_set )
{
	for( size_t i = 0; i < mesh_set->meshes.size(); ++i )
	{
		carve::mesh::Mesh<3>* mesh_i = mesh_set->meshes[i];
		if( mesh_i->open_edges.size() > 0 )
		{
			return false;
		}
		std::vector<carve::mesh::Face<3>* >& vec_faces = mesh_i->faces;
		for( size_t j=0; j<vec_faces.size(); ++j )
		{
			carve::mesh::Face<3>* face = vec_faces[j];
			carve::mesh::Edge<3>* e = face->edge;
			const size_t n_edges = face->n_edges;
			for( size_t i_edge = 0; i_edge < n_edges; ++i_edge )
			{
				if( e == reinterpret_cast<carve::mesh::Edge<3>*>(0xfeeefeeefeeefeee) )
				{
					return false;
				}
				if( !e->rev )
				{
					return false;
				}
				if( e->rev->next == reinterpret_cast<carve::mesh::Edge<3>*>(0xfeeefeeefeeefeee) )
				{
					return false;
				}
				if( ! e->rev->next )
				{
					return false;
				}
				if( ! e->rev->next->next )
				{
					return false;
				}
				if( ! e->rev->next->next->vert )
				{
					return false;
				}

				if( !e->next )
				{
					return false;
				}
				if( e->next == reinterpret_cast<carve::mesh::Edge<3>*>(0xfeeefeeefeeefeee) )
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

class MeshOps
{
public:
	//static void applyPosition( carve::mesh::MeshSet<3>* meshset, const carve::math::Matrix& pos )
	//{
	//	for (size_t i = 0; i < meshset->vertex_storage.size(); ++i )
	//	{
	//		carve::geom::vector<3>& point = meshset->vertex_storage[i].v;
	//		point = pos*point;
	//	}
	//	for (size_t i = 0; i < meshset->meshes.size(); ++i)
	//	{
	//		meshset->meshes[i]->recalc();
	//	}
	//}
		
	static void applyTranslate( carve::mesh::MeshSet<3>* meshset, const carve::geom::vector<3>& pos )
	{
		for (size_t i = 0; i < meshset->vertex_storage.size(); ++i )
		{
			carve::geom::vector<3>& point = meshset->vertex_storage[i].v;
			point = point + pos;
		}
		for (size_t i = 0; i < meshset->meshes.size(); ++i)
		{
			meshset->meshes[i]->recalc();
		}
	}
	static void roundVertices( carve::mesh::MeshSet<3>* meshset )
	{
		std::vector<carve::mesh::Vertex<3> >& vertex_storage = meshset->vertex_storage;
		const size_t num_vertices = vertex_storage.size();
		for( int i=0; i<num_vertices; ++i )
		{
			carve::mesh::Vertex<3>& vertex = vertex_storage[i];
			vertex.v.x = round( vertex.v.x*1000000.0 ) * 0.000001;
			vertex.v.y = round( vertex.v.y*1000000.0 ) * 0.000001;
			vertex.v.z = round( vertex.v.z*1000000.0 ) * 0.000001;
		}
	}
	static int getNumFaces( const carve::mesh::MeshSet<3>* meshset )
	{
		int num_faces = 0;
		for( int i=0; i<meshset->meshes.size(); ++i )
		{
			num_faces += meshset->meshes[i]->faces.size();
		}
		return num_faces;
	}
	static int getNumClosedEdges( const carve::mesh::MeshSet<3>* meshset )
	{
		int num_edges = 0;
		for( int i=0; i<meshset->meshes.size(); ++i )
		{
			num_edges += meshset->meshes[i]->closed_edges.size();
		}
		return num_edges;
	}
	static double getVolume( const carve::mesh::MeshSet<3>* meshset )
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
};

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
		double object_volume = MeshOps::getVolume( mesh_set );
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
double y_trans = 0;
#endif

void CSG_Adapter::simplifyMesh( shared_ptr<carve::mesh::MeshSet<3> >& meshset )
{

	carve::mesh::MeshSimplifier simplifier;
	//double min_colinearity = m_geom_settings->m_min_colinearity;
	//double min_delta_v = m_geom_settings->m_min_delta_v;
	//double min_normal_angle = m_geom_settings->m_min_normal_angle;
	//double min_length = 0.0001;//m_geom_settings->m_min_length;

	//try
	//{
	//	simplifier.removeFins(meshset.get());
	//	//simplifier.cleanFaceEdges( meshset.get() );
	//	//simplifier.removeRemnantFaces( meshset.get() );
	//	//simplifier.mergeCoplanarFaces( meshset.get(), 0.0 );
	//	//simplifier.eliminateShortEdges( meshset.get(), min_length );
	//	//simplifier.removeFins(meshset.get());
	//	simplifier.simplify( meshset.get(), min_colinearity, min_delta_v, min_normal_angle, min_length );
	//	simplifier.removeFins(meshset.get());
	//	//simplifier.removeLowVolumeManifolds(meshset, 0.01);
	//	simplifier.improveMesh( meshset.get(), m_geom_settings->m_min_colinearity, m_geom_settings->m_min_delta_v, m_geom_settings->m_min_normal_angle );
	//}
	//catch(...)
	//{
	//	std::cout << "simplifier.eliminateShortEdges failed." << std::endl;
	//}

	if( !meshset )
	{
		return;
	}

	if( meshset->vertex_storage.size() < 9 )
	{
		//std::cout << "meshset->vertex_storage.size() < 9" << std::endl;
		return;
	}

	std::stringstream err;
	bool meshset_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), err, -1 );
	if( !meshset_ok )
	{
		std::cout << err.str().c_str() << std::endl;
		return;
	}
	shared_ptr<carve::mesh::MeshSet<3> > meshset_copy( meshset->clone() );

	retriangulateMeshSet( meshset );
	
	//simplifier.simplify( meshset.get(), 0.00001,0.001, 0.000001, 0.001 );
	//retriangulateMeshSet( meshset );

	//return;
	simplifier.removeLowVolumeManifolds( meshset.get(), 0.000001 );

	if( meshset->meshes.size() < 1 )
	{
		return;
	}
	//size_t modifications_short_edges = simplifier.eliminateShortEdges(meshset.get(), 0.001 );
	//removeSmallAreaFaces( meshset );

	//if( modifications_short_edges > 0 )
	{
		//retriangulateMeshSet( meshset );
	}

	int num_faces = MeshOps::getNumFaces( meshset.get() );
	size_t modifications_coplanar = simplifier.mergeCoplanarFaces( meshset.get(), 0.00001 );
	if( modifications_coplanar > 0 )
	{
		int num_faces_post_merge = MeshOps::getNumFaces( meshset.get() );

		if( num_faces_post_merge + modifications_coplanar != num_faces )
		{
			std::cout << "num_faces_post_merge + modifications_coplanar != num_faces" << std::endl;
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
	mergeAlignedEdges( meshset );

	faces_ok = checkFaceIntegrity( meshset.get() );
	if( !faces_ok )
	{
		meshset = meshset_copy;
#ifdef _DEBUG
		std::cout << "Error in mergeAlignedEdges" << std::endl;
#endif
		return;
	}

	//if( meshset->vertex_storage.size() < 9 )
	//{
	//	return;
	//}

	simplifier.removeRemnantFaces(meshset.get());
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

	//retriangulateMeshSet( meshset );

#ifdef _DEBUG
	bool meshset_ok_post_triang = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), err, -1 );
	if( !meshset_ok_post_triang )
	{
		meshset = meshset_copy;
		std::cout << "Error in retriangulateMeshSet" << std::endl;
	}
#endif

	//simplifier.removeRemnantFaces(meshset.get());
	//size_t modifications_short_edges2 = simplifier.eliminateShortEdges(meshset.get(), 0.0001 );
	
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
		renderMeshsetInDebugViewer( meshset.get(), osg::Vec4f( 0.2f, 0.3f, 0.4f, 1.f ), false );
		//throw DebugBreakException( "!simplified_meshset_ok" );
#endif
		meshset = meshset_copy;
		return;
	}

	simplifier.removeLowVolumeManifolds(meshset.get(), 0.0);
	retriangulateMeshSet( meshset );
	
	std::stringstream err_retriangulated;
	bool retriangulated_meshset_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), err_retriangulated, -1 );
	if( !retriangulated_meshset_ok )
	{
#ifdef _DEBUG
		std::cout << err_retriangulated.str().c_str() << std::endl;
		renderMeshsetInDebugViewer( meshset.get(), osg::Vec4f( 0.2f, 0.3f, 0.4f, 1.f ), false );
		//throw DebugBreakException( "!simplified_meshset_ok" );
#endif
		meshset = meshset_copy;
	}

#ifdef _DEBUG
	++simplify_count;
#endif
}



//#define ROUND_MESH_COORDS
#define SIMPLIFY_MESH_PRE_CSG
#define SIMPLIFY_MESH_POST_CSG
#ifdef _DEBUG
double y_pos = 0;
#endif

bool CSG_Adapter::computeCSG( shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation, 
							 const int entity1, const int entity2, std::stringstream& err, shared_ptr<carve::mesh::MeshSet<3> >& result )
{
#ifdef ROUND_MESH_COORDS
	MeshOps::roundVertices( op1.get() );
	MeshOps::roundVertices( op1.get() );
#endif

	bool csg_operation_ok = false;
	try
	{
		bool meshset1_ok = CSG_Adapter::checkMeshSetValidAndClosed( op1.get(), err, entity1 );
		bool meshset2_ok = CSG_Adapter::checkMeshSetValidAndClosed( op2.get(), err, entity2 );

		if( meshset1_ok && meshset2_ok )
		{
		
#ifdef SIMPLIFY_MESH_PRE_CSG
			simplifyMesh( op1 );
			simplifyMesh( op2 );
			// TODO: Subclass from carve::mesh::MeshSet and add attribute to remember which meshset has already been simplified. 
#endif
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
				MeshOps::applyTranslate( op1.get(), -translate_avoid_large_numbers );
				MeshOps::applyTranslate( op2.get(), -translate_avoid_large_numbers );
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
					result_meshset_ok = CSG_Adapter::checkMeshSetValidAndClosed( result.get(), err, -1 );
				}
			}

			if( result_meshset_ok )
			{
				csg_operation_ok = true;

#ifdef SIMPLIFY_MESH_POST_CSG
				simplifyMesh( result );
#endif
			}
			else
			{
#ifdef _DEBUG
				std::cout << "csg.compute result nok ok." << std::endl;
#endif
			}
		
			if( translate_avoid_large_numbers.length2() > 1000.0 )
			{
				if( result )
				{
					MeshOps::applyTranslate( result.get(), translate_avoid_large_numbers );
				}
				MeshOps::applyTranslate( op1.get(), translate_avoid_large_numbers );
				MeshOps::applyTranslate( op2.get(), translate_avoid_large_numbers );
			}
		}
		else
		{
			csg_operation_ok = false;
		}
	}
#ifdef _DEBUG
	catch(DebugBreakException& e)
	{
		throw e;
	}
#endif
	catch( carve::exception& ce )
	{
		csg_operation_ok = false;
		err << "csg operation failed, id1=" << entity1 << ", id2=" << entity2 << ", ";
		err << ce.str() << std::endl;
	}
	catch (const std::out_of_range& oor)
	{
		csg_operation_ok = false;
		err << "csg operation failed, id1=" << entity1 << ", id2=" << entity2 << ", ";
		err << oor.what() << std::endl;
	}
	catch(std::exception& e)
	{
		csg_operation_ok = false;
		err << "csg operation failed, id1=" << entity1 << ", id2=" << entity2 << ", ";
		err << e.what() << std::endl;
	}
	catch(...)
	{
		csg_operation_ok = false;
		err << "csg operation failed, id1=" << entity1 << ", id2=" << entity2 << std::endl;
	}

	//csg_operation_ok = false;

#ifdef _DEBUG
	if( !csg_operation_ok )
	{
		std::cout << "!csg_operation_ok. id1=" << entity1 << ", id2=" << entity2 << std::endl;

		if( result )
		{
			carve::mesh::MeshSet<3>* result_copy = result->clone();//new carve::mesh::MeshSet<3>();
			MeshOps::applyTranslate( result_copy, carve::geom::VECTOR( 0, y_pos, 0 ) );
			renderMeshsetInDebugViewer( result_copy, osg::Vec4(0.0f, 0.5f, 0.0f, 1.0f), false );
		}

		carve::mesh::MeshSet<3>* op1_copy = op1->clone();
		MeshOps::applyTranslate( op1_copy, carve::geom::VECTOR( 0, y_pos, 0 ) );
		renderMeshsetInDebugViewer( op1_copy, osg::Vec4(0.0f, 0.8f, 0.0f, 1.0f), true );

		carve::mesh::MeshSet<3>* op2_copy = op2->clone();
		MeshOps::applyTranslate( op2_copy, carve::geom::VECTOR( 0, y_pos, 0 ) );
		renderMeshsetInDebugViewer( op2_copy, osg::Vec4(0.8f, 0.0f, 0.0f, 1.0f), true );

		y_pos += 1;

		dumpMeshsets( op1.get(), op2.get(), result.get(), entity1, entity2 );
	}
#endif
	return csg_operation_ok;
}

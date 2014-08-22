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

#include <fstream>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osgUtil/Tessellator>
#include <osgUtil/Optimizer>
#include <osgText/Text>

#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/IfcPPOpenMP.h>

#include "GeomUtils.h"
#include "ProfileConverter.h"
#include "RepresentationConverter.h"
#include "SolidModelConverter.h"
#include "ConverterOSG.h"

void ConverterOSG::drawFace( const carve::mesh::Face<3>* face, osg::Geode* geode, bool add_color_array )
{
#ifdef _DEBUG
	std::cout << "not triangulated" << std::endl;
#endif
	std::vector<carve::geom::vector<3> > face_vertices;
	face_vertices.resize(face->nVertices());
	carve::mesh::Edge<3> *e = face->edge;
	const size_t num_vertices = face->nVertices();
	for( size_t i = 0; i < num_vertices; ++i )
	{
		face_vertices[i] = e->v1()->v;
		e = e->next;
	}

	if( num_vertices < 4 )
	{
		std::cout << "drawFace is meant only for num vertices > 4" << std::endl;
	}

	carve::geom::vector<3> * vertex_vec;
	osg::Vec3Array* vertices = new osg::Vec3Array();
	osg::ref_ptr<osg::DrawElementsUInt> triangles = new osg::DrawElementsUInt( osg::PrimitiveSet::POLYGON, 0 );

	for( size_t i = 0; i < num_vertices; ++i )
	{
		vertex_vec = &face_vertices[num_vertices-i-1];
		vertices->push_back(osg::Vec3f(vertex_vec->x, vertex_vec->y, vertex_vec->z));
		triangles->push_back( i );
	}

	osg::Vec3f poly_normal = GeomUtils::computePolygonNormal( vertices );
	osg::Vec3Array* normals = new osg::Vec3Array();
	normals->resize( num_vertices, poly_normal );


	osg::Geometry* geometry = new osg::Geometry();
	geometry->setVertexArray( vertices );
	geometry->setNormalArray( normals );

	geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,vertices->size()));

	if( add_color_array )
	{
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		colors->resize( vertices->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );

		geometry->setColorArray( colors );
		geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	}

	if( num_vertices > 4 )
	{
		// TODO: check if polygon is convex with Gift wrapping algorithm

		osg::ref_ptr<osgUtil::Tessellator> tesselator = new osgUtil::Tessellator();
		tesselator->setTessellationType(osgUtil::Tessellator::TESS_TYPE_POLYGONS);
		//tesselator->setWindingType( osgUtil::Tessellator::TESS_WINDING_ODD );
		tesselator->retessellatePolygons(*geometry);
	}
	geode->addDrawable(geometry);


#ifdef DEBUG_DRAW_NORMALS
	osg::Vec3Array* vertices_normals = new osg::Vec3Array();
	for( size_t i = 0; i < num_vertices; ++i )
	{
		vertex_vec = &face_vertices[num_vertices-i-1];
		vertices_normals->push_back(osg::Vec3f(vertex_vec->x, vertex_vec->y, vertex_vec->z));
		vertices_normals->push_back(osg::Vec3f(vertex_vec->x, vertex_vec->y, vertex_vec->z) + poly_normal );
	}

	osg::Vec4Array* colors_normals = new osg::Vec4Array();
	colors_normals->resize( num_vertices*2, osg::Vec4f( 0.4f, 0.7f, 0.4f, 1.f ) );

	osg::Geometry* geometry_normals = new osg::Geometry();
	geometry_normals->setVertexArray( vertices_normals );
	geometry_normals->setColorArray( colors_normals );
	geometry_normals->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	geometry_normals->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	geometry_normals->setNormalBinding( osg::Geometry::BIND_OFF );
	geometry_normals->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,vertices_normals->size()));
	geode->addDrawable(geometry_normals);
#endif
}

//#define DEBUG_DRAW_NORMALS

void ConverterOSG::drawMesh( const carve::mesh::Mesh<3>* mesh, osg::Geode* geode, double intermediate_normal_angle, bool add_color_array )
{
	osg::ref_ptr<osg::Vec3Array> vertices_tri = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> normals_tri = new osg::Vec3Array();

	osg::ref_ptr<osg::Vec3Array> vertices_quad = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> normals_quad = new osg::Vec3Array();
	
	std::map<carve::mesh::Vertex<3>*, std::vector<carve::mesh::Face<3>* > > map_vertex_all_faces;
	std::map<carve::mesh::Vertex<3>*, std::vector<carve::mesh::Face<3>* > >::iterator it_vertex_normal;

	const size_t num_faces = mesh->faces.size();
	for( size_t i = 0; i != num_faces; ++i )
	{
		carve::mesh::Face<3>* face = mesh->faces[i];
		const int n_vertices = face->nVertices();
		if( n_vertices > 4 )
		{
			drawFace(face, geode );
			continue;
		}
		const carve::geom::vector<3> face_normal = face->plane.N;
		carve::geom::vector<3> intermediate_normal = face_normal;

		if( intermediate_normal_angle > 0 )
		{
			carve::mesh::Edge<3>* e = face->edge;
			for( size_t jj = 0; jj < n_vertices; ++jj )
			{
				carve::mesh::Vertex<3>* vertex = e->vert;
				const carve::geom::vector<3>& vertex_v = vertex->v;

				it_vertex_normal = map_vertex_all_faces.find( vertex );
				if( it_vertex_normal == map_vertex_all_faces.end() )
				{
					std::vector<carve::mesh::Face<3>*>& vec_faces = map_vertex_all_faces.insert( std::make_pair( vertex, std::vector<carve::mesh::Face<3>*>() ) ).first->second;
					vec_faces.push_back( face );
					// collect all faces at vertex
					//              | ^
					//              | |
					//  f1   e->rev | | e    face
					//              v |
					// <---e1-------   <---------------
					//------------->   --------------->
					//              |  ^
					//              |  |
					//              v  |


					if( e->rev )
					{
						if( e->rev->next )
						{

							carve::mesh::Edge<3>* e1 = e->rev->next;
							carve::mesh::Face<3>* f1 = e1->face;
							vec_faces.push_back( f1 );

							for( size_t i_edge = 0; i_edge < num_faces; ++i_edge )
							{
								if( !e1->rev )
								{
									// it's an open mesh
									break;
								}

								e1 = e1->rev->next;
								if( !e1 )
								{
									break;
								}
								f1 = e1->face;


#ifdef _DEBUG
								if( e1->vert != vertex )
								{
									std::cout << "e1->vert != vertex" << std::endl;
								}

								if( i_edge > 50 )
								{
									std::cout << "i_edge > 50" << std::endl;
								}
#endif
								if( f1 == face )
								{
									break;
								}
								vec_faces.push_back( f1 );
							}
						}
					}
					it_vertex_normal = map_vertex_all_faces.find( vertex );
				}
				else
				{
					int wait = 0;
				}

				std::vector<carve::mesh::Face<3>*>& vec_faces = it_vertex_normal->second;

				// check if angle between adjacent faces is smaller than intermediate_normal_angle
				std::sort( vec_faces.begin(), vec_faces.end() );
				for( size_t i_face = 0; i_face < vec_faces.size(); ++i_face )
				{
					carve::mesh::Face<3>* f = vec_faces[i_face];

					// TODO: if face f1 is much bigger than the adjacent face f2, use f1->plane.N as normals for all its vertices
					carve::geom::vector<3> f1_normal = f->plane.N;
					double cos_angle = dot( f1_normal, face_normal );
					if( std::abs( cos_angle - 1.0 ) < intermediate_normal_angle )
					{
						intermediate_normal = 0.5*f1_normal + 0.5*intermediate_normal;
						intermediate_normal.normalize();
					}
				}


#ifdef __GNUC__ // for some reason this is not working under linux...
				intermediate_normal = face_normal;
#endif

				if( face->n_edges == 3 )
				{
					vertices_tri->push_back( osg::Vec3( vertex_v.x, vertex_v.y, vertex_v.z ) );
					normals_tri->push_back( osg::Vec3( intermediate_normal.x, intermediate_normal.y, intermediate_normal.z ) );
				}
				else if( face->n_edges == 4 )
				{
					vertices_quad->push_back( osg::Vec3( vertex_v.x, vertex_v.y, vertex_v.z ) );
					normals_quad->push_back( osg::Vec3( intermediate_normal.x, intermediate_normal.y, intermediate_normal.z ) );
				}
				e = e->next;
			}
		}
	}

	if( vertices_tri->size() > 0 )
	{
		//drawTriangles( vertices_tri, normals_tri, add_color_array, geode );
		osg::Geometry* geometry = new osg::Geometry();
		geometry->setVertexArray( vertices_tri );

		geometry->setNormalArray( normals_tri );
		geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

		if( add_color_array )
		{
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->resize( vertices_tri->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );

			geometry->setColorArray( colors );
			geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
		}

		geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,vertices_tri->size()));
		geode->addDrawable( geometry );

#ifdef DEBUG_DRAW_NORMALS
	osg::Vec3Array* vertices_normals = new osg::Vec3Array();
	for( size_t i = 0; i < vertices_tri->size(); ++i )
	{
		osg::Vec3f& vertex_vec = vertices_tri->at( i );// [i];
		osg::Vec3f& normal_vec = normals_tri->at( i );
		vertices_normals->push_back(osg::Vec3f(vertex_vec.x(), vertex_vec.y(), vertex_vec.z()));
		vertices_normals->push_back(osg::Vec3f(vertex_vec.x(), vertex_vec.y(), vertex_vec.z()) + normal_vec );
	}

	osg::Vec4Array* colors_normals = new osg::Vec4Array();
	colors_normals->resize( vertices_normals->size(), osg::Vec4f( 0.4f, 0.7f, 0.4f, 1.f ) );

	osg::Geometry* geometry_normals = new osg::Geometry();
	geometry_normals->setVertexArray( vertices_normals );
	geometry_normals->setColorArray( colors_normals );
	geometry_normals->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	geometry_normals->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	geometry_normals->setNormalBinding( osg::Geometry::BIND_OFF );
	geometry_normals->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,vertices_normals->size()));
	geode->addDrawable(geometry_normals);
#endif
	}

	if( vertices_quad->size() > 0 )
	{
		//drawQuads( vertices_quad, normals_quad, add_color_array, geode );
		osg::Geometry* geometry = new osg::Geometry();
		geometry->setVertexArray( vertices_quad );

		geometry->setNormalArray( normals_quad );
		geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

		if( add_color_array )
		{
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->resize( vertices_quad->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );

			geometry->setColorArray( colors );
			geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
		}

		geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,vertices_quad->size()));
		geode->addDrawable( geometry );
	}
}

void ConverterOSG::drawMeshSet( const carve::mesh::MeshSet<3>* meshset, osg::Geode* geode, double intermediate_normal_angle, bool add_color_array )
{
	if( !meshset )
	{
		return;
	}

	for( size_t i = 0; i < meshset->meshes.size(); ++i )
	{
		drawMesh( meshset->meshes[i], geode, intermediate_normal_angle, add_color_array );
	}
}

void ConverterOSG::drawPolyline( const carve::input::PolylineSetData* polyline_data, osg::Geode* geode, bool add_color_array )
{
	osg::Vec3Array* vertices = new osg::Vec3Array();
	carve::line::PolylineSet* polyline_set = polyline_data->create(carve::input::opts());

	if( polyline_set->vertices.size() < 2 )
	{
#ifdef _DEBUG
		std::cout << __FUNC__ << ": polyline_set->vertices.size() < 2" << std::endl;
#endif
		return;
	}

	carve::line::PolylineSet::const_line_iter it;
	for( it = polyline_set->lines.begin(); it != polyline_set->lines.end(); ++it )
	{
		carve::line::Polyline* pline = *it;
		size_t vertex_count = pline->vertexCount();

		for( size_t vertex_i = 0; vertex_i < vertex_count; ++vertex_i )
		{
			if( vertex_i >= polyline_set->vertices.size() )
			{
#ifdef _DEBUG
				std::cout << __FUNC__ <<  ": vertex_i >= polyline_set->vertices.size()" << std::endl;
#endif
				continue;
			}
			const carve::line::Vertex* v = pline->vertex( vertex_i );
			vertices->push_back( osg::Vec3d( v->v[0], v->v[1], v->v[2] ) );

		}
	}

	osg::Geometry* geometry = new osg::Geometry();
	geometry->setVertexArray( vertices );
	geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices->size()) );

	if( add_color_array )
	{
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		colors->resize( vertices->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );

		geometry->setColorArray( colors );
		geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	}

	geode->addDrawable(geometry);
}

double ConverterOSG::computeSurfaceAreaOfGroup( const osg::Group* grp )
{
	if( !grp )
	{
		return 0.0;
	}

	double surface_area = 0.0;
	int num_children = grp->getNumChildren();
	for( int i=0; i<num_children; ++i )
	{
		const osg::Node* node = grp->getChild(i);
		const osg::Group* child_group = dynamic_cast<const osg::Group*>(node);
		if( child_group )
		{
			surface_area += computeSurfaceAreaOfGroup( child_group );
			continue;
		}
		const osg::Geode* child_geode = dynamic_cast<const osg::Geode*>(node);
		if( child_geode )
		{
			const osg::Geode::DrawableList& drawable_list = child_geode->getDrawableList();
			osg::Geode::DrawableList::const_iterator it_drawables;
			for( it_drawables=drawable_list.begin(); it_drawables!=drawable_list.end(); ++it_drawables )
			{
				osg::Drawable* drawable = (*it_drawables);
				const osg::Geometry* child_gemetry = dynamic_cast<const osg::Geometry*>(drawable);
				if( !child_gemetry )
				{
#ifdef _DEBUG
					std::cout <<  __FUNC__ << " !child_gemetry" << std::endl;
#endif
					return 0;
				}
				const osg::Array* vertices_array = child_gemetry->getVertexArray();
				const osg::Vec3Array* vertices_float = dynamic_cast<const osg::Vec3Array*>(vertices_array);

				if( !vertices_float )
				{
#ifdef _DEBUG
					std::cout <<  __FUNC__ << " !vertices_float" << std::endl;
#endif
					return 0;
				}

				const osg::Geometry::PrimitiveSetList& primitive_sets = child_gemetry->getPrimitiveSetList();
				osg::Geometry::PrimitiveSetList::const_iterator it_primitives;
				for( it_primitives=primitive_sets.begin(); it_primitives!=primitive_sets.end(); ++it_primitives )
				{
					const osg::PrimitiveSet* p_set = (*it_primitives);

					const int num_elements = p_set->getNumIndices();
					if( num_elements < 3 )
					{
#ifdef _DEBUG
						std::cout << "num_elements < 3" << std::endl;
#endif
						continue;
					}

					if( p_set->getMode() == osg::PrimitiveSet::QUADS )
					{
						for( int k=0; k<num_elements-3; k+=4 )
						{
							const osg::Vec3& v0 = vertices_float->at( p_set->index(k) );
							const osg::Vec3& v1 = vertices_float->at( p_set->index(k+1) );
							const osg::Vec3& v2 = vertices_float->at( p_set->index(k+2) );
							const osg::Vec3& v3 = vertices_float->at( p_set->index(k+3) );

#ifdef _DEBUG
							if( (v0 -v1).length2() < 0.00001 )
							{
								continue;
							}
							if( (v1 -v2).length2() < 0.00001 )
							{
								continue;
							}
							if( (v2 -v3).length2() < 0.00001 )
							{
								continue;
							}
							if( (v3 -v0).length2() < 0.00001 )
							{
								continue;
							}
#endif
							osg::Vec3d v0_v1 = v1 - v0;
							osg::Vec3d v0_v3 = v3 - v0;
							osg::Vec3d v2_v1 = v1 - v2;
							osg::Vec3d v2_v3 = v3 - v2;
							osg::Vec3d cross_vec = v0_v1 ^ v0_v3;
							surface_area += cross_vec.length()*0.5;

							cross_vec = v2_v1 ^ v2_v3;
							surface_area += cross_vec.length()*0.5;
						}
					}
					else if( p_set->getMode() == osg::PrimitiveSet::TRIANGLES )
					{
						for( int k=0; k<num_elements-2; k+=3 )
						{
							const osg::Vec3& v0 = vertices_float->at( p_set->index(k) );
							const osg::Vec3& v1 = vertices_float->at( p_set->index(k+1) );
							const osg::Vec3& v2 = vertices_float->at( p_set->index(k+2) );

							osg::Vec3d v0_v1 = v1 - v0;
							osg::Vec3d v0_v2 = v2 - v0;

							osg::Vec3d cross_vec = v0_v1 ^ v0_v2;
							surface_area += cross_vec.length()*0.5;
						}
					}
					else if( p_set->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP )
					{
						for( int k=0; k<num_elements-3; ++k )
						{
							const osg::Vec3& v0 = vertices_float->at( p_set->index(k) );
							const osg::Vec3& v1 = vertices_float->at( p_set->index(k+1) );
							const osg::Vec3& v2 = vertices_float->at( p_set->index(k+2) );

							osg::Vec3d v0_v1 = v1 - v0;
							osg::Vec3d v0_v2 = v2 - v0;

							osg::Vec3d cross_vec = v0_v1 ^ v0_v2;
							surface_area += cross_vec.length()*0.5;
						}
					}
					else if( p_set->getMode() == osg::PrimitiveSet::TRIANGLE_FAN )
					{
						const osg::Vec3& v0 = vertices_float->at( p_set->index(0) );
						for( int k=0; k<num_elements-2; ++k )
						{
							const osg::Vec3& v1 = vertices_float->at( p_set->index(k+1) );
							const osg::Vec3& v2 = vertices_float->at( p_set->index(k+2) );

							osg::Vec3d v0_v1 = v1 - v0;
							osg::Vec3d v0_v2 = v2 - v0;

							osg::Vec3d cross_vec = v0_v1 ^ v0_v2;
							surface_area += cross_vec.length()*0.5;
						}
					}
					else
					{
#ifdef _DEBUG
						std::cout << "other primitive set mode" << std::endl;
#endif
						return 0;
					}
				}
			}
		}
	}
	return surface_area;
}


//\brief AppearanceManagerOSG: StateSet caching and re-use
std::vector<osg::ref_ptr<osg::StateSet> > global_vec_existing_statesets;
#ifdef IFCPP_OPENMP
	Mutex writelock_appearance_cache;
#endif

void AppearanceManagerOSG::clearAppearanceCache()
{
#ifdef IFCPP_OPENMP
	ScopedLock lock( writelock_appearance_cache );
#endif
	global_vec_existing_statesets.clear();
}

osg::StateSet* AppearanceManagerOSG::convertToStateSet( const shared_ptr<AppearanceData>& appearence )
{
	if( !appearence )
	{
		return nullptr;
	}
	const float shininess = appearence->shininess;
	const float transparency = appearence->transparency;
	const bool set_transparent = appearence->set_transparent;

	const float color_ambient_r = appearence->color_ambient.x;
	const float color_ambient_g = appearence->color_ambient.y;
	const float color_ambient_b = appearence->color_ambient.z;
	const float color_ambient_a = appearence->color_ambient.w;

	const float color_diffuse_r = appearence->color_diffuse.x;
	const float color_diffuse_g = appearence->color_diffuse.y;
	const float color_diffuse_b = appearence->color_diffuse.z;
	const float color_diffuse_a = appearence->color_diffuse.w;

	const float color_specular_r = appearence->color_specular.x;
	const float color_specular_g = appearence->color_specular.y;
	const float color_specular_b = appearence->color_specular.z;
	const float color_specular_a = appearence->color_specular.w;

#ifdef IFCPP_OPENMP
	ScopedLock lock( writelock_appearance_cache );
#endif

	for( int i=0; i<global_vec_existing_statesets.size(); ++i )
	{
		const osg::ref_ptr<osg::StateSet> stateset_existing = global_vec_existing_statesets[i];

		if( !stateset_existing.valid() )
		{
			continue;
		}

		osg::ref_ptr<osg::Material> mat_existing = (osg::Material*)stateset_existing->getAttribute(osg::StateAttribute::MATERIAL);
		if( !mat_existing )
		{
			continue;
		}

		// compare
		osg::Vec4f color_ambient_existing = mat_existing->getAmbient( osg::Material::FRONT_AND_BACK );
		if( fabs(color_ambient_existing.r() - color_ambient_r ) > 0.03 ) break;
		if( fabs(color_ambient_existing.g() - color_ambient_g ) > 0.03 ) break;
		if( fabs(color_ambient_existing.b() - color_ambient_b ) > 0.03 ) break;
		if( fabs(color_ambient_existing.a() - color_ambient_a ) > 0.03 ) break;

		osg::Vec4f color_diffuse_existing = mat_existing->getDiffuse( osg::Material::FRONT_AND_BACK );
		if( fabs(color_diffuse_existing.r() - color_diffuse_r ) > 0.03 ) break;
		if( fabs(color_diffuse_existing.g() - color_diffuse_g ) > 0.03 ) break;
		if( fabs(color_diffuse_existing.b() - color_diffuse_b ) > 0.03 ) break;
		if( fabs(color_diffuse_existing.a() - color_diffuse_a ) > 0.03 ) break;

		osg::Vec4f color_specular_existing = mat_existing->getSpecular( osg::Material::FRONT_AND_BACK );
		if( fabs(color_specular_existing.r() - color_specular_r ) > 0.03 ) break;
		if( fabs(color_specular_existing.g() - color_specular_g ) > 0.03 ) break;
		if( fabs(color_specular_existing.b() - color_specular_b ) > 0.03 ) break;
		if( fabs(color_specular_existing.a() - color_specular_a ) > 0.03 ) break;

		float shininess_existing = mat_existing->getShininess( osg::Material::FRONT_AND_BACK );
		if( fabs(shininess_existing - shininess ) > 0.03 ) break;

		bool blend_on_existing = stateset_existing->getMode( GL_BLEND ) == osg::StateAttribute::ON;
		if( blend_on_existing != set_transparent ) break;

		bool transparent_bin = stateset_existing->getRenderingHint() == osg::StateSet::TRANSPARENT_BIN;
		if( transparent_bin != set_transparent ) break;

		// if we get here, appearance is same as existing state set
		return stateset_existing;
	}

	osg::Vec4f ambientColor(	color_ambient_r,	color_ambient_g,	color_ambient_b,	transparency );
	osg::Vec4f diffuseColor(	color_diffuse_r,	color_diffuse_g,	color_diffuse_b,	transparency  );
	osg::Vec4f specularColor(	color_specular_r,	color_specular_g,	color_specular_b,	transparency );

	osg::ref_ptr<osg::Material> mat = new osg::Material();
	mat->setAmbient( osg::Material::FRONT_AND_BACK, ambientColor );
	mat->setDiffuse( osg::Material::FRONT_AND_BACK, diffuseColor );
	mat->setSpecular( osg::Material::FRONT_AND_BACK, specularColor );
	mat->setShininess( osg::Material::FRONT_AND_BACK, shininess );
	mat->setColorMode( osg::Material::SPECULAR );

	osg::StateSet* stateset = new osg::StateSet();
	stateset->setAttribute( mat, osg::StateAttribute::ON );
	
	if( appearence->set_transparent )
	{
		mat->setTransparency( osg::Material::FRONT_AND_BACK, transparency );	
		stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
		stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	}

	if( appearence->specular_exponent != 0.f )
	{
		//osg::ref_ptr<osgFX::SpecularHighlights> spec_highlights = new osgFX::SpecularHighlights();
		//spec_highlights->setSpecularExponent( spec->m_value );
		// todo: add to scenegraph
	}

	global_vec_existing_statesets.push_back( stateset );
	return stateset;
}

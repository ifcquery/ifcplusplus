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
#include <osg/Point>
#include <osgUtil/Tessellator>
#include <osgText/Text>

#include <ifcpp/IFC4/include/IfcSpace.h>
#include <ifcpp/IFC4/include/IfcCurtainWall.h>
#include <ifcpp/IFC4/include/IfcSite.h>
#include <ifcpp/IFC4/include/IfcWindow.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/IfcPPOpenMP.h>

#include "GeomUtils.h"
#include "ProfileConverter.h"
#include "RepresentationConverter.h"
#include "SolidModelConverter.h"
#include "CSG_Adapter.h"
#include "ConverterOSG.h"

ConverterOSG::ConverterOSG( shared_ptr<GeometrySettings>& geom_settings ) : m_geom_settings( geom_settings )
{
	m_cull_back_off = new osg::CullFace( osg::CullFace::BACK );
	
	m_glass_stateset = new osg::StateSet();
	m_glass_stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
	m_glass_stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
}
ConverterOSG::~ConverterOSG() {}

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
	geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POLYGON, 0, vertices->size() ) );

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

void ConverterOSG::drawMesh( const carve::mesh::Mesh<3>* mesh, osg::Geode* geode, double crease_angle, bool add_color_array )
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

		if( crease_angle > 0 )
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
					const double cos_angle = dot( f1_normal, face_normal );
					const double deviation = std::abs( cos_angle - 1.0 );
					if( deviation > 0.00001 && deviation < crease_angle )
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

		geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vertices_tri->size() ) );
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

		geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, vertices_quad->size() ) );
		geode->addDrawable( geometry );
	}
}

void ConverterOSG::drawMeshSet( const carve::mesh::MeshSet<3>* meshset, osg::Geode* geode, double crease_angle, bool add_color_array )
{
	if( !meshset )
	{
		return;
	}

	for( size_t i = 0; i < meshset->meshes.size(); ++i )
	{
		drawMesh( meshset->meshes[i], geode, crease_angle, add_color_array );
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

	for( auto it = polyline_set->lines.begin(); it != polyline_set->lines.end(); ++it )
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
	geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, vertices->size() ) );

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
			for( auto it_drawables=drawable_list.begin(); it_drawables!=drawable_list.end(); ++it_drawables )
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
				for( auto it_primitives=primitive_sets.begin(); it_primitives!=primitive_sets.end(); ++it_primitives )
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


//\brief creates geometry objects from an IfcProduct object
// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
void ConverterOSG::convertToOSG( shared_ptr<ProductShapeInputData>& product_shape, const double length_factor )
{
	std::stringstream group_name;
	shared_ptr<IfcProduct> ifc_product;
	if( !product_shape->m_ifc_product.expired() )
	{
		ifc_product = shared_ptr<IfcProduct>( product_shape->m_ifc_product );
		const int product_id = ifc_product->m_id;
		group_name << "#" << product_id << "=" << ifc_product->className() << " group";
	}

	osg::ref_ptr<osg::Switch> product_switch = new osg::Switch();
	osg::ref_ptr<osg::Switch> product_switch_curves = new osg::Switch();
	if( !product_switch || !product_switch_curves )
	{
		throw IfcPPOutOfMemoryException( __FUNC__ );
	}
	
	product_switch->setName( group_name.str().c_str() );
	product_switch_curves->setName( "CurveRepresentation" );

	// create OSG objects
	std::vector<shared_ptr<ItemShapeInputData> >& product_items = product_shape->m_vec_item_data;
	for( size_t i_item = 0; i_item < product_items.size(); ++i_item )
	{
		shared_ptr<ItemShapeInputData> item_shape = product_items[i_item];
		osg::ref_ptr<osg::Group> item_group = new osg::Group();
		osg::ref_ptr<osg::Group> item_group_curves = new osg::Group();

		if( !item_group || !item_group_curves )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}

		// create shape for open shells
		for( size_t ii = 0; ii < item_shape->m_meshsets_open.size(); ++ii )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = item_shape->m_meshsets_open[ii];
			CSG_Adapter::retriangulateMeshSet( item_meshset );
			osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			if( !geode )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			drawMeshSet( item_meshset.get(), geode, m_geom_settings->m_intermediate_normal_angle );

			// disable back face culling for open meshes
			geode->getOrCreateStateSet()->setAttributeAndModes( m_cull_back_off.get(), osg::StateAttribute::OFF );
			item_group->addChild( geode );
		}

		// create shape for meshsets
		for( size_t ii = 0; ii < item_shape->m_meshsets.size(); ++ii )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = item_shape->m_meshsets[ii];
			CSG_Adapter::retriangulateMeshSet( item_meshset );
			osg::ref_ptr<osg::Geode> geode_result = new osg::Geode();
			if( !geode_result )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			drawMeshSet( item_meshset.get(), geode_result, m_geom_settings->m_intermediate_normal_angle );
			item_group->addChild( geode_result );
		}

		// create shape for points
		for( size_t ii = 0; ii < item_shape->m_vertex_points.size(); ++ii )
		{
			shared_ptr<carve::input::VertexData>& pointset_data = item_shape->m_vertex_points[ii];
			if( pointset_data )
			{
				osg::ref_ptr<osg::Geode> geode = new osg::Geode();
				if( !geode )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}

				osg::Vec3Array* vertices = new osg::Vec3Array();
				for( size_t i_pointset_point = 0; i_pointset_point < pointset_data->points.size(); ++i_pointset_point )
				{
					carve::geom::vector<3>& carve_point = pointset_data->points[i_pointset_point];
					vertices->push_back( osg::Vec3d( carve_point.x, carve_point.y, carve_point.z ) );
				}

				osg::Geometry* geometry = new osg::Geometry();
				geometry->setVertexArray( vertices );
				geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POINTS, 0, vertices->size() ) );
				geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
				geode->getOrCreateStateSet()->setAttribute( new osg::Point( 3.0f ), osg::StateAttribute::ON );
				geode->addDrawable( geometry );
				geode->setCullingActive( false );
				geode->addDrawable( geometry );
				item_group_curves->addChild( geode );
			}
		}

		// create shape for polylines
		for( size_t ii = 0; ii < item_shape->m_polylines.size(); ++ii )
		{
			shared_ptr<carve::input::PolylineSetData>& polyline_data = item_shape->m_polylines[ii];
			osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			if( !geode )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
			drawPolyline( polyline_data.get(), geode );
			item_group_curves->addChild( geode );
		}

		if( m_geom_settings->m_show_text_literals )
		{
			for( size_t ii = 0; ii < item_shape->m_vec_text_literals.size(); ++ii )
			{
				shared_ptr<TextItemData>& text_data = item_shape->m_vec_text_literals[ii];
				if( !text_data )
				{
					continue;
				}
				carve::math::Matrix& text_pos = text_data->m_text_position;
				// TODO: handle rotation

				std::string text_str;
				text_str.assign( text_data->m_text.begin(), text_data->m_text.end() );

				osg::Vec3 pos2( text_pos._41, text_pos._42, text_pos._43 );

				osgText::Text* txt = new osgText::Text();
				if( !txt )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}
				txt->setFont( "fonts/arial.ttf" );
				txt->setColor( osg::Vec4f( 0, 0, 0, 1 ) );
				txt->setCharacterSize( 0.1f );
				txt->setAutoRotateToScreen( true );
				txt->setPosition( pos2 );
				txt->setText( text_str.c_str() );
				txt->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

				osg::ref_ptr<osg::Geode> geode = new osg::Geode();
				if( !geode )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}
				geode->addDrawable( txt );
				item_group->addChild( geode );
			}
		}

		// apply statesets if there are any
		if( item_shape->m_vec_item_appearances.size() > 0 )
		{
			for( size_t ii = 0; ii < item_shape->m_vec_item_appearances.size(); ++ii )
			{
				shared_ptr<AppearanceData>& appearance = item_shape->m_vec_item_appearances[ii];
				if( appearance->m_apply_to_geometry_type == AppearanceData::SURFACE )
				{
					osg::StateSet* item_stateset = convertToOSGStateSet( appearance );
					if( item_stateset != nullptr )
					{
						osg::StateSet* existing_item_stateset = item_group->getStateSet();
						if( existing_item_stateset )
						{
							osg::StateSet* merged_product_stateset = new osg::StateSet( *existing_item_stateset );
							if( !merged_product_stateset )
							{
								throw IfcPPOutOfMemoryException( __FUNC__ );
							}
							merged_product_stateset->merge( *item_stateset );
							item_group->setStateSet( merged_product_stateset );
						}
						else
						{
							item_group->setStateSet( item_stateset );
						}
					}
				}
				else if( appearance->m_apply_to_geometry_type == AppearanceData::CURVE )
				{
					osg::StateSet* item_stateset = convertToOSGStateSet( appearance );
					if( item_stateset != nullptr )
					{
						osg::Material* material = dynamic_cast<osg::Material*>( item_stateset->getAttribute( osg::StateAttribute::MATERIAL ) );
						if( material )
						{
							material->setColorMode( osg::Material::AMBIENT );
						}

						osg::StateSet* existing_item_stateset = item_group_curves->getStateSet();
						if( existing_item_stateset )
						{
							osg::StateSet* merged_product_stateset = new osg::StateSet( *existing_item_stateset );
							if( !merged_product_stateset )
							{
								throw IfcPPOutOfMemoryException( __FUNC__ );
							}
							merged_product_stateset->merge( *item_stateset );
							item_group_curves->setStateSet( merged_product_stateset );
						}
						else
						{
							item_group_curves->setStateSet( item_stateset );
						}
					}
				}
			}
		}

		// If anything has been created, add it to the product group
		if( item_group->getNumChildren() > 0 )
		{
			product_switch->addChild( item_group );
		}

		if( item_group_curves->getNumChildren() > 0 )
		{
			product_switch_curves->addChild( item_group_curves );
		}
	}

	const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances = product_shape->getAppearances();
	for( size_t ii = 0; ii < vec_product_appearances.size(); ++ii )
	{
		const shared_ptr<AppearanceData>& appearance = vec_product_appearances[ii];
		if( !appearance )
		{
			continue;
		}

		if( appearance->m_apply_to_geometry_type == AppearanceData::SURFACE )
		{
			osg::StateSet* item_stateset = convertToOSGStateSet( appearance );
			if( item_stateset != nullptr )
			{
				osg::StateSet* existing_item_stateset = product_switch->getStateSet();
				if( existing_item_stateset )
				{
					osg::StateSet* merged_product_stateset = new osg::StateSet( *existing_item_stateset );
					if( !merged_product_stateset )
					{
						throw IfcPPOutOfMemoryException( __FUNC__ );
					}
					merged_product_stateset->merge( *item_stateset );
					product_switch->setStateSet( merged_product_stateset );
				}
				else
				{
					product_switch->setStateSet( item_stateset );
				}
			}
		}
		else if( appearance->m_apply_to_geometry_type == AppearanceData::CURVE )
		{
			osg::StateSet* item_stateset = convertToOSGStateSet( appearance );
			if( item_stateset != nullptr )
			{
				osg::Material* material = dynamic_cast<osg::Material*>( item_stateset->getAttribute( osg::StateAttribute::MATERIAL ) );
				if( material )
				{
					material->setColorMode( osg::Material::AMBIENT );
				}

				osg::StateSet* existing_item_stateset = product_switch_curves->getStateSet();
				if( existing_item_stateset )
				{
					osg::StateSet* merged_product_stateset = new osg::StateSet( *existing_item_stateset );
					if( !merged_product_stateset )
					{
						throw IfcPPOutOfMemoryException( __FUNC__ );
					}
					merged_product_stateset->merge( *item_stateset );
					product_switch_curves->setStateSet( merged_product_stateset );
				}
				else
				{
					product_switch_curves->setStateSet( item_stateset );
				}
			}
		}
	}

	// enable transparency for certain objects
	if( dynamic_pointer_cast<IfcSpace>( ifc_product ) )
	{
		product_switch->setStateSet( m_glass_stateset );
	}
	else if( dynamic_pointer_cast<IfcCurtainWall>( ifc_product ) || dynamic_pointer_cast<IfcWindow>( ifc_product ) )
	{
		// TODO: make only glass part of window transparent
		product_switch->setStateSet( m_glass_stateset );
		GeomUtils::setMaterialAlpha( product_switch, 0.6f );
	}

	// TODO: if no color or material is given, set color 231/219/169 for walls, 140/140/140 for slabs 

	if( product_switch->getNumChildren() > 0 )
	{
		product_shape->m_product_switch = product_switch;
	}

	if( product_switch_curves->getNumChildren() > 0 )
	{
		product_shape->m_product_switch_curves = product_switch_curves;
	}
}

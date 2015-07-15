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

#include <osg/Material>
#include <osg/Geode>
#include <osg/CullFace>
#include <osg/Point>
#include <osgUtil/Tessellator>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/IFC4/include/IfcCurtainWall.h>
#include <ifcpp/IFC4/include/IfcSpace.h>
#include <ifcpp/IFC4/include/IfcWindow.h>

#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "GeometrySettings.h"
#include "CSG_Adapter.h"

class ConverterOSG : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings>	m_geom_settings;
	osg::ref_ptr<osg::CullFace>		m_cull_back_off;
	osg::ref_ptr<osg::StateSet>		m_glass_stateset;
	//\brief StateSet caching and re-use
	std::vector<osg::ref_ptr<osg::StateSet> > m_vec_existing_statesets;
#ifdef IFCPP_OPENMP
	Mutex m_writelock_appearance_cache;
#endif

public:
	ConverterOSG( shared_ptr<GeometrySettings>& geom_settings ) : m_geom_settings( geom_settings )
	{
		m_cull_back_off = new osg::CullFace( osg::CullFace::BACK );

		m_glass_stateset = new osg::StateSet();
		m_glass_stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
		m_glass_stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	}
	virtual ~ConverterOSG() {}

	static void drawFace( const carve::mesh::Face<3>* face, osg::Geode* geode, bool add_color_array = false )
	{
#ifdef _DEBUG
		std::cout << "not triangulated" << std::endl;
#endif
		std::vector<carve::geom::vector<3> > face_vertices;
		face_vertices.resize( face->nVertices() );
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
		osg::Vec3Array* vertices = new osg::Vec3Array( num_vertices );
		if( !vertices ) { throw IfcPPOutOfMemoryException(); }
		osg::ref_ptr<osg::DrawElementsUInt> triangles = new osg::DrawElementsUInt( osg::PrimitiveSet::POLYGON, num_vertices );
		if( !triangles ) { throw IfcPPOutOfMemoryException(); }

		for( size_t i = 0; i < num_vertices; ++i )
		{
			vertex_vec = &face_vertices[num_vertices - i - 1];
			//vertices->push_back(osg::Vec3f(vertex_vec->x, vertex_vec->y, vertex_vec->z));
			( *vertices )[i].set( vertex_vec->x, vertex_vec->y, vertex_vec->z );
			( *triangles )[i] = i;// ->push_back( i );
		}

		osg::Vec3f poly_normal = GeomUtils::computePolygonNormal( vertices );
		osg::Vec3Array* normals = new osg::Vec3Array();
		normals->resize( num_vertices, poly_normal );


		osg::Geometry* geometry = new osg::Geometry();
		geometry->setVertexArray( vertices );
		geometry->setNormalArray( normals );
		normals->setBinding( osg::Array::BIND_PER_VERTEX );

		//geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
		geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POLYGON, 0, vertices->size() ) );

		if( add_color_array )
		{
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->resize( vertices->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );
			colors->setBinding( osg::Array::BIND_PER_VERTEX );

			geometry->setColorArray( colors );
			//geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
		}

		if( num_vertices > 4 )
		{
			// TODO: check if polygon is convex with Gift wrapping algorithm

			osg::ref_ptr<osgUtil::Tessellator> tesselator = new osgUtil::Tessellator();
			tesselator->setTessellationType( osgUtil::Tessellator::TESS_TYPE_POLYGONS );
			//tesselator->setWindingType( osgUtil::Tessellator::TESS_WINDING_ODD );
			tesselator->retessellatePolygons( *geometry );
		}
		geode->addDrawable( geometry );


#ifdef DEBUG_DRAW_NORMALS
		osg::Vec3Array* vertices_normals = new osg::Vec3Array();
		for( size_t i = 0; i < num_vertices; ++i )
		{
			vertex_vec = &face_vertices[num_vertices - i - 1];
			vertices_normals->push_back( osg::Vec3f( vertex_vec->x, vertex_vec->y, vertex_vec->z ) );
			vertices_normals->push_back( osg::Vec3f( vertex_vec->x, vertex_vec->y, vertex_vec->z ) + poly_normal );
		}

		osg::Vec4Array* colors_normals = new osg::Vec4Array();
		colors_normals->resize( num_vertices * 2, osg::Vec4f( 0.4f, 0.7f, 0.4f, 1.f ) );

		osg::Geometry* geometry_normals = new osg::Geometry();
		geometry_normals->setVertexArray( vertices_normals );
		geometry_normals->setColorArray( colors_normals );
		geometry_normals->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
		geometry_normals->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

		geometry_normals->setNormalBinding( osg::Geometry::BIND_OFF );
		geometry_normals->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vertices_normals->size() ) );
		geode->addDrawable( geometry_normals );
#endif
	}

	//#define DEBUG_DRAW_NORMALS

	static void drawMeshSet( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, osg::Geode* geode, double crease_angle = M_PI*0.05, bool add_color_array = false )
	{
		if( !meshset )
		{
			return;
		}

		osg::ref_ptr<osg::Vec3Array> vertices_tri = new osg::Vec3Array();
		if( !vertices_tri ) { throw IfcPPOutOfMemoryException(); }
		osg::ref_ptr<osg::Vec3Array> normals_tri = new osg::Vec3Array();
		if( !normals_tri ) { throw IfcPPOutOfMemoryException(); }

		osg::ref_ptr<osg::Vec3Array> vertices_quad;
		osg::ref_ptr<osg::Vec3Array> normals_quad;

		const size_t max_num_faces_per_vertex = 10000;
		std::map<carve::mesh::Face<3>*, double> map_face_area;
		std::map<carve::mesh::Face<3>*, double>::iterator it_face_area;

		if( crease_angle > 0 )
		{
			for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
			{
				const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];
				const size_t num_faces = mesh->faces.size();
				for( size_t i_face = 0; i_face != num_faces; ++i_face )
				{
					carve::mesh::Face<3>* face = mesh->faces[i_face];
					// compute area of projected face:
					std::vector<carve::geom::vector<2> > projected;
					face->getProjectedVertices( projected );
					double face_area = carve::geom2d::signedArea( projected );
					map_face_area[face] = abs( face_area );
				}
			}
		}

		for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
		{
			const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

			const size_t num_faces = mesh->faces.size();
			for( size_t i_face = 0; i_face != num_faces; ++i_face )
			{
				carve::mesh::Face<3>* face = mesh->faces[i_face];
				const size_t n_vertices = face->nVertices();
				if( n_vertices > 4 )
				{
					drawFace( face, geode );
					continue;
				}
				const carve::geom::vector<3> face_normal = face->plane.N;

				if( crease_angle > 0 )
				{
					carve::mesh::Edge<3>* e = face->edge;
					for( size_t jj = 0; jj < n_vertices; ++jj )
					{
						carve::mesh::Vertex<3>* vertex = e->vert;
						carve::geom::vector<3> intermediate_normal;

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
						carve::mesh::Edge<3>* e1 = e;// ->rev->next;
						carve::mesh::Face<3>* f1 = e1->face;
#ifdef _DEBUG
						if( f1 != face )
						{
							std::cout << "f1 != face" << std::endl;
						}
#endif
						for( size_t i3 = 0; i3 < max_num_faces_per_vertex; ++i3 )
						{
							if( !e1->rev )
							{
								break;
							}
							if( !e1->rev->next )
							{
								break;
							}

							carve::geom::vector<3> f1_normal = f1->plane.N;
							const double cos_angle = dot( f1_normal, face_normal );
							if( cos_angle > 0 )
							{
								const double deviation = std::abs( cos_angle - 1.0 );
								if( deviation < crease_angle )
								{
									double weight = 0.0;
									it_face_area = map_face_area.find( f1 );
									if( it_face_area != map_face_area.end() )
									{
										weight = it_face_area->second;
									}
									intermediate_normal += weight*f1_normal;
								}
							}

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
#endif
							if( f1 == face )
							{
								break;
							}
						}
						const double intermediate_normal_length = intermediate_normal.length();
						if( intermediate_normal_length < 0.0000000001 )
						{
							intermediate_normal = face_normal;
						}
						else
						{
							// normalize:
							intermediate_normal *= 1.0 / intermediate_normal_length;
						}

						const carve::geom::vector<3>& vertex_v = vertex->v;
						if( face->n_edges == 3 )
						{
							vertices_tri->push_back( osg::Vec3( vertex_v.x, vertex_v.y, vertex_v.z ) );
							normals_tri->push_back( osg::Vec3( intermediate_normal.x, intermediate_normal.y, intermediate_normal.z ) );
						}
						else if( face->n_edges == 4 )
						{
							if( !vertices_quad ) vertices_quad = new osg::Vec3Array();
							vertices_quad->push_back( osg::Vec3( vertex_v.x, vertex_v.y, vertex_v.z ) );
							if( !normals_quad ) normals_quad = new osg::Vec3Array();
							normals_quad->push_back( osg::Vec3( intermediate_normal.x, intermediate_normal.y, intermediate_normal.z ) );
						}
						e = e->next;
					}
				}
				else
				{
					carve::mesh::Edge<3>* e = face->edge;
					for( size_t jj = 0; jj < n_vertices; ++jj )
					{
						carve::mesh::Vertex<3>* vertex = e->vert;
						const carve::geom::vector<3>& vertex_v = vertex->v;

						if( face->n_edges == 3 )
						{
							vertices_tri->push_back( osg::Vec3( vertex_v.x, vertex_v.y, vertex_v.z ) );
							normals_tri->push_back( osg::Vec3( face_normal.x, face_normal.y, face_normal.z ) );
						}
						else if( face->n_edges == 4 )
						{
							if( !vertices_quad ) vertices_quad = new osg::Vec3Array();
							vertices_quad->push_back( osg::Vec3( vertex_v.x, vertex_v.y, vertex_v.z ) );
							if( !normals_quad ) normals_quad = new osg::Vec3Array();
							normals_quad->push_back( osg::Vec3( face_normal.x, face_normal.y, face_normal.z ) );
						}
						e = e->next;
					}
				}
			}
		}

		if( vertices_tri->size() > 0 )
		{
			osg::Geometry* geometry = new osg::Geometry();
			if( !geometry ) { throw IfcPPOutOfMemoryException(); }
			geometry->setVertexArray( vertices_tri );

			geometry->setNormalArray( normals_tri );
			normals_tri->setBinding( osg::Array::BIND_PER_VERTEX );
			//geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

			if( add_color_array )
			{
				osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
				if( !colors ) { throw IfcPPOutOfMemoryException(); }
				colors->resize( vertices_tri->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );
				colors->setBinding( osg::Array::BIND_PER_VERTEX );
				geometry->setColorArray( colors );
				//geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
			}

			geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vertices_tri->size() ) );
			if( !geometry ) { throw IfcPPOutOfMemoryException(); }
			geode->addDrawable( geometry );

#ifdef DEBUG_DRAW_NORMALS
			osg::Vec3Array* vertices_normals = new osg::Vec3Array();
			for( size_t i = 0; i < vertices_tri->size(); ++i )
			{
				osg::Vec3f& vertex_vec = vertices_tri->at( i );// [i];
				osg::Vec3f& normal_vec = normals_tri->at( i );
				vertices_normals->push_back( osg::Vec3f( vertex_vec.x(), vertex_vec.y(), vertex_vec.z() ) );
				vertices_normals->push_back( osg::Vec3f( vertex_vec.x(), vertex_vec.y(), vertex_vec.z() ) + normal_vec );
			}

			osg::Vec4Array* colors_normals = new osg::Vec4Array();
			colors_normals->resize( vertices_normals->size(), osg::Vec4f( 0.4f, 0.7f, 0.4f, 1.f ) );

			osg::Geometry* geometry_normals = new osg::Geometry();
			geometry_normals->setVertexArray( vertices_normals );
			geometry_normals->setColorArray( colors_normals );
			geometry_normals->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
			geometry_normals->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

			geometry_normals->setNormalBinding( osg::Geometry::BIND_OFF );
			geometry_normals->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vertices_normals->size() ) );
			geode->addDrawable( geometry_normals );
#endif
		}

		if( vertices_quad )
		{
			if( vertices_quad->size() > 0 )
			{
				osg::Geometry* geometry = new osg::Geometry();
				if( !geometry ) { throw IfcPPOutOfMemoryException(); }
				geometry->setVertexArray( vertices_quad );
				if( normals_quad )
				{
					normals_quad->setBinding( osg::Array::BIND_PER_VERTEX );
					geometry->setNormalArray( normals_quad );
				}

				if( add_color_array )
				{
					osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
					if( !colors ) { throw IfcPPOutOfMemoryException(); }
					colors->resize( vertices_quad->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );
					colors->setBinding( osg::Array::BIND_PER_VERTEX );
					geometry->setColorArray( colors );
				}

				geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, vertices_quad->size() ) );
				if( !geometry ) { throw IfcPPOutOfMemoryException(); }
				geode->addDrawable( geometry );
			}
		}
	}

	void drawPolyline( const carve::input::PolylineSetData* polyline_data, osg::Geode* geode, bool add_color_array = false )
	{
		osg::Vec3Array* vertices = new osg::Vec3Array();
		if( !vertices ) { throw IfcPPOutOfMemoryException(); }
		carve::line::PolylineSet* polyline_set = polyline_data->create( carve::input::opts() );

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
					std::cout << __FUNC__ << ": vertex_i >= polyline_set->vertices.size()" << std::endl;
#endif
					continue;
				}
				const carve::line::Vertex* v = pline->vertex( vertex_i );
				vertices->push_back( osg::Vec3d( v->v[0], v->v[1], v->v[2] ) );
			}
		}

		osg::Geometry* geometry = new osg::Geometry();
		if( !geometry ) { throw IfcPPOutOfMemoryException(); }
		geometry->setVertexArray( vertices );
		geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, vertices->size() ) );

		if( add_color_array )
		{
			osg::Vec4f color( 0.6f, 0.6f, 0.6f, 0.1f );
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array( vertices->size(), &color );
			if( !colors ) { throw IfcPPOutOfMemoryException(); }
			colors->setBinding( osg::Array::BIND_PER_VERTEX );
			geometry->setColorArray( colors );
			//geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
		}

		geode->addDrawable( geometry );
	}

	double computeSurfaceAreaOfGroup( const osg::Group* grp )
	{
		if( !grp )
		{
			return 0.0;
		}

		double surface_area = 0.0;
		unsigned int num_children = grp->getNumChildren();
		for( unsigned int i = 0; i < num_children; ++i )
		{
			const osg::Node* node = grp->getChild( i );
			const osg::Group* child_group = dynamic_cast<const osg::Group*>( node );
			if( child_group )
			{
				surface_area += computeSurfaceAreaOfGroup( child_group );
				continue;
			}
			const osg::Geode* child_geode = dynamic_cast<const osg::Geode*>( node );
			if( child_geode )
			{
				const osg::Geode::DrawableList& drawable_list = child_geode->getDrawableList();
				for( auto it_drawables = drawable_list.begin(); it_drawables != drawable_list.end(); ++it_drawables )
				{
					osg::Drawable* drawable = ( *it_drawables );
					const osg::Geometry* child_gemetry = dynamic_cast<const osg::Geometry*>( drawable );
					if( !child_gemetry )
					{
#ifdef _DEBUG
						std::cout << __FUNC__ << " !child_gemetry" << std::endl;
#endif
						return 0;
					}
					const osg::Array* vertices_array = child_gemetry->getVertexArray();
					const osg::Vec3Array* vertices_float = dynamic_cast<const osg::Vec3Array*>( vertices_array );

					if( !vertices_float )
					{
#ifdef _DEBUG
						std::cout << __FUNC__ << " !vertices_float" << std::endl;
#endif
						return 0;
					}

					const osg::Geometry::PrimitiveSetList& primitive_sets = child_gemetry->getPrimitiveSetList();
					for( auto it_primitives = primitive_sets.begin(); it_primitives != primitive_sets.end(); ++it_primitives )
					{
						const osg::PrimitiveSet* p_set = ( *it_primitives );

						const unsigned int num_elements = p_set->getNumIndices();
						if( num_elements < 3 )
						{
#ifdef _DEBUG
							std::cout << "num_elements < 3" << std::endl;
#endif
							continue;
						}

						if( p_set->getMode() == osg::PrimitiveSet::QUADS )
						{
							for( unsigned int k = 0; k < num_elements - 3; k += 4 )
							{
								const osg::Vec3& v0 = vertices_float->at( p_set->index( k ) );
								const osg::Vec3& v1 = vertices_float->at( p_set->index( k + 1 ) );
								const osg::Vec3& v2 = vertices_float->at( p_set->index( k + 2 ) );
								const osg::Vec3& v3 = vertices_float->at( p_set->index( k + 3 ) );

#ifdef _DEBUG
								if( ( v0 - v1 ).length2() < 0.00001 )
								{
									continue;
								}
								if( ( v1 - v2 ).length2() < 0.00001 )
								{
									continue;
								}
								if( ( v2 - v3 ).length2() < 0.00001 )
								{
									continue;
								}
								if( ( v3 - v0 ).length2() < 0.00001 )
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
							for( unsigned int k = 0; k < num_elements - 2; k += 3 )
							{
								const osg::Vec3& v0 = vertices_float->at( p_set->index( k ) );
								const osg::Vec3& v1 = vertices_float->at( p_set->index( k + 1 ) );
								const osg::Vec3& v2 = vertices_float->at( p_set->index( k + 2 ) );

								osg::Vec3d v0_v1 = v1 - v0;
								osg::Vec3d v0_v2 = v2 - v0;

								osg::Vec3d cross_vec = v0_v1 ^ v0_v2;
								surface_area += cross_vec.length()*0.5;
							}
						}
						else if( p_set->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP )
						{
							for( unsigned int k = 0; k < num_elements - 3; ++k )
							{
								const osg::Vec3& v0 = vertices_float->at( p_set->index( k ) );
								const osg::Vec3& v1 = vertices_float->at( p_set->index( k + 1 ) );
								const osg::Vec3& v2 = vertices_float->at( p_set->index( k + 2 ) );

								osg::Vec3d v0_v1 = v1 - v0;
								osg::Vec3d v0_v2 = v2 - v0;

								osg::Vec3d cross_vec = v0_v1 ^ v0_v2;
								surface_area += cross_vec.length()*0.5;
							}
						}
						else if( p_set->getMode() == osg::PrimitiveSet::TRIANGLE_FAN )
						{
							const osg::Vec3& v0 = vertices_float->at( p_set->index( 0 ) );
							for( unsigned int k = 0; k < num_elements - 2; ++k )
							{
								const osg::Vec3& v1 = vertices_float->at( p_set->index( k + 1 ) );
								const osg::Vec3& v2 = vertices_float->at( p_set->index( k + 2 ) );

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

	void applyAppearancesToGroup( const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances, osg::Group* grp )
	{
		for( size_t ii = 0; ii < vec_product_appearances.size(); ++ii )
		{
			const shared_ptr<AppearanceData>& appearance = vec_product_appearances[ii];
			if( !appearance )
			{
				continue;
			}

			if( appearance->m_apply_to_geometry_type == AppearanceData::SURFACE || appearance->m_apply_to_geometry_type == AppearanceData::ANY )
			{
				osg::StateSet* item_stateset = convertToOSGStateSet( appearance );
				if( item_stateset != nullptr )
				{
					osg::StateSet* existing_item_stateset = grp->getStateSet();
					if( existing_item_stateset )
					{
						if( existing_item_stateset != item_stateset )
						{
							existing_item_stateset->merge( *item_stateset );
						}
					}
					else
					{
						grp->setStateSet( item_stateset );
					}
				}
			}
			else if( appearance->m_apply_to_geometry_type == AppearanceData::CURVE )
			{

			}
		}
	}

	//\brief creates geometry objects from an IfcProduct object
	// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
	void convertToOSG( shared_ptr<ProductShapeInputData>& product_shape, const double length_factor )
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
		if( !product_switch )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}

		product_switch->setName( group_name.str().c_str() );

		// create OSG objects
		std::vector<shared_ptr<ProductRepresentationData> >& vec_product_representations = product_shape->m_vec_representations;
		for( size_t i_rep = 0; i_rep < vec_product_representations.size(); ++i_rep )
		{
			const shared_ptr<ProductRepresentationData>& product_representation = vec_product_representations[i_rep];
			if( !product_representation->m_representation_switch )
			{
				product_representation->m_representation_switch = new osg::Switch();
			}
			
			const std::vector<shared_ptr<ItemShapeInputData> >& product_items = product_representation->m_vec_item_data;
			for( size_t i_item = 0; i_item < product_items.size(); ++i_item )
			{
				const shared_ptr<ItemShapeInputData>& item_shape = product_items[i_item];
				osg::ref_ptr<osg::Group> item_group = new osg::Group();

				if( !item_group ) { throw IfcPPOutOfMemoryException( __FUNC__ ); }

				// create shape for open shells
				for( size_t ii = 0; ii < item_shape->m_meshsets_open.size(); ++ii )
				{
					shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = item_shape->m_meshsets_open[ii];
					CSG_Adapter::retriangulateMeshSet( item_meshset );
					osg::ref_ptr<osg::Geode> geode = new osg::Geode();
					if( !geode ) { throw IfcPPOutOfMemoryException( __FUNC__ ); }
					drawMeshSet( item_meshset, geode, m_geom_settings->getMinCreaseAngle() );

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
					if( !geode_result ) { throw IfcPPOutOfMemoryException( __FUNC__ ); }
					drawMeshSet( item_meshset, geode_result, m_geom_settings->getMinCreaseAngle() );
					item_group->addChild( geode_result );
				}

				// create shape for points
				for( size_t ii = 0; ii < item_shape->m_vertex_points.size(); ++ii )
				{
					shared_ptr<carve::input::VertexData>& pointset_data = item_shape->m_vertex_points[ii];
					if( pointset_data )
					{
						osg::ref_ptr<osg::Geode> geode = new osg::Geode();
						if( !geode ) { throw IfcPPOutOfMemoryException( __FUNC__ ); }

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
						item_group->addChild( geode );
					}
				}

				// create shape for polylines
				for( size_t ii = 0; ii < item_shape->m_polylines.size(); ++ii )
				{
					shared_ptr<carve::input::PolylineSetData>& polyline_data = item_shape->m_polylines[ii];
					osg::ref_ptr<osg::Geode> geode = new osg::Geode();
					if( !geode ) { throw IfcPPOutOfMemoryException( __FUNC__ ); }
					geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
					drawPolyline( polyline_data.get(), geode );
					item_group->addChild( geode );
				}

				if( m_geom_settings->isShowTextLiterals() )
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
					applyAppearancesToGroup( item_shape->m_vec_item_appearances, item_group );
				}

				// If anything has been created, add it to the representation group
				if( item_group->getNumChildren() > 0 )
				{
					product_representation->m_representation_switch->addChild( item_group );
				}
			}

			// apply statesets if there are any
			if( product_representation->m_vec_representation_appearances.size() > 0 )
			{
				applyAppearancesToGroup( product_representation->m_vec_representation_appearances, product_representation->m_representation_switch );
			}

			// If anything has been created, add it to the product group
			if( product_representation->m_representation_switch->getNumChildren() > 0 )
			{
				product_switch->addChild( product_representation->m_representation_switch );
			}
		}

		// apply statesets if there are any
		const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances = product_shape->getAppearances();
		if( vec_product_appearances.size() > 0 )
		{
			applyAppearancesToGroup( vec_product_appearances, product_switch );
		}

		// enable transparency for certain objects
		if( dynamic_pointer_cast<IfcSpace>( ifc_product ) )
		{
			product_switch->setStateSet( m_glass_stateset );
		}
		else if( dynamic_pointer_cast<IfcCurtainWall>( ifc_product ) || dynamic_pointer_cast<IfcWindow>( ifc_product ) )
		{
			for( auto& product_representation_data : vec_product_representations )
			{
				if( product_representation_data->m_representation_switch )
				{
					product_representation_data->m_representation_switch->setStateSet( m_glass_stateset );
					GeomUtils::setMaterialAlpha( product_representation_data->m_representation_switch, 0.6f );
				}
			}

			// TODO: make only glass part of window transparent
			product_switch->setStateSet( m_glass_stateset );
			GeomUtils::setMaterialAlpha( product_switch, 0.6f );
		}

		// TODO: if no color or material is given, set color 231/219/169 for walls, 140/140/140 for slabs 

		if( product_switch->getNumChildren() > 0 )
		{
			product_shape->m_product_switch = product_switch;
		}
	}

	static inline void convertCarveToOsgMatrix( const carve::math::Matrix& matrix_in, osg::Matrix& matrix_out )
	{
		matrix_out = osg::Matrix(
			matrix_in._11, matrix_in._12, matrix_in._13, matrix_in._14,
			matrix_in._21, matrix_in._22, matrix_in._23, matrix_in._24,
			matrix_in._31, matrix_in._32, matrix_in._33, matrix_in._34,
			matrix_in._41, matrix_in._42, matrix_in._43, matrix_in._44
			);
	}

	static inline void convertOsgToCarveMatrix( const osg::Matrix& matrix_in, carve::math::Matrix& matrix_out )
	{
		matrix_out = carve::math::Matrix(
			matrix_in( 0, 0 ), matrix_in( 1, 0 ), matrix_in( 2, 0 ), matrix_in( 3, 0 ),
			matrix_in( 0, 1 ), matrix_in( 1, 1 ), matrix_in( 2, 1 ), matrix_in( 3, 1 ),
			matrix_in( 0, 2 ), matrix_in( 1, 2 ), matrix_in( 2, 2 ), matrix_in( 3, 2 ),
			matrix_in( 0, 3 ), matrix_in( 1, 3 ), matrix_in( 2, 3 ), matrix_in( 3, 3 )
			);
	}

	void clearAppearanceCache()
	{
#ifdef IFCPP_OPENMP
		ScopedLock lock( m_writelock_appearance_cache );
#endif
		m_vec_existing_statesets.clear();
	}

	osg::StateSet* convertToOSGStateSet( const shared_ptr<AppearanceData>& appearence )
	{
		if( !appearence )
		{
			return nullptr;
		}
		const float shininess = appearence->m_shininess;
		const float transparency = appearence->m_transparency;
		const bool set_transparent = appearence->m_set_transparent;

		const float color_ambient_r = appearence->m_color_ambient.x;
		const float color_ambient_g = appearence->m_color_ambient.y;
		const float color_ambient_b = appearence->m_color_ambient.z;
		const float color_ambient_a = appearence->m_color_ambient.w;

		const float color_diffuse_r = appearence->m_color_diffuse.x;
		const float color_diffuse_g = appearence->m_color_diffuse.y;
		const float color_diffuse_b = appearence->m_color_diffuse.z;
		const float color_diffuse_a = appearence->m_color_diffuse.w;

		const float color_specular_r = appearence->m_color_specular.x;
		const float color_specular_g = appearence->m_color_specular.y;
		const float color_specular_b = appearence->m_color_specular.z;
		const float color_specular_a = appearence->m_color_specular.w;

	#ifdef IFCPP_OPENMP
		ScopedLock lock( m_writelock_appearance_cache );
	#endif

		for( size_t i=0; i<m_vec_existing_statesets.size(); ++i )
		{
			const osg::ref_ptr<osg::StateSet> stateset_existing = m_vec_existing_statesets[i];

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
			// TODO: block this re-used stateset for merging, or prevent merged statesets from being re-used
			return stateset_existing;
		}

		osg::Vec4f ambientColor(	color_ambient_r,	color_ambient_g,	color_ambient_b,	transparency );
		osg::Vec4f diffuseColor(	color_diffuse_r,	color_diffuse_g,	color_diffuse_b,	transparency  );
		osg::Vec4f specularColor(	color_specular_r,	color_specular_g,	color_specular_b,	transparency );

		// TODO: material caching and re-use
		osg::ref_ptr<osg::Material> mat = new osg::Material();
		if( !mat ){ throw IfcPPOutOfMemoryException(); }
		mat->setAmbient( osg::Material::FRONT_AND_BACK, ambientColor );
		mat->setDiffuse( osg::Material::FRONT_AND_BACK, diffuseColor );
		mat->setSpecular( osg::Material::FRONT_AND_BACK, specularColor );
		mat->setShininess( osg::Material::FRONT_AND_BACK, shininess );
		mat->setColorMode( osg::Material::SPECULAR );

		osg::StateSet* stateset = new osg::StateSet();
		if( !stateset ){ throw IfcPPOutOfMemoryException(); }
		stateset->setAttribute( mat, osg::StateAttribute::ON );
	
		if( appearence->m_set_transparent )
		{
			mat->setTransparency( osg::Material::FRONT_AND_BACK, transparency );	
			stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
			stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
		}

		if( appearence->m_specular_exponent != 0.f )
		{
			//osg::ref_ptr<osgFX::SpecularHighlights> spec_highlights = new osgFX::SpecularHighlights();
			//spec_highlights->setSpecularExponent( spec->m_value );
			// todo: add to scenegraph
		}

		m_vec_existing_statesets.push_back( stateset );
		return stateset;
	}
};

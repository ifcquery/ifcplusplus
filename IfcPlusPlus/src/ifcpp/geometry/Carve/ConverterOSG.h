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

#include <osg/CullFace>
#include <osg/Geode>
#include <osg/Hint>
#include <osg/LineWidth>
#include <osg/Material>
#include <osg/Point>
#include <osgUtil/Tessellator>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/IFC4/include/IfcCurtainWall.h>
#include <ifcpp/IFC4/include/IfcFeatureElementSubtraction.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcPropertySetDefinitionSet.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcSpace.h>
#include <ifcpp/IFC4/include/IfcWindow.h>

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/SceneGraphUtils.h>
#include <ifcpp/geometry/AppearanceData.h>
#include "GeometryInputData.h"
#include "IncludeCarveHeaders.h"
#include "CSG_Adapter.h"

class ConverterOSG : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings>				m_geom_settings;
	std::map<int, osg::ref_ptr<osg::Switch> >	m_map_entity_id_to_switch;
	std::map<std::wstring, osg::ref_ptr<osg::Switch> >	m_map_entity_guid_to_switch;
	std::map<int, osg::ref_ptr<osg::Switch> >	m_map_representation_id_to_switch;
	double										m_recent_progress;
	osg::ref_ptr<osg::CullFace>					m_cull_back_off;
	osg::ref_ptr<osg::StateSet>					m_glass_stateset;
	//\brief StateSet caching and re-use
	std::vector<osg::ref_ptr<osg::StateSet> >	m_vec_existing_statesets;
	bool										m_enable_stateset_caching = false;

#ifdef ENABLE_OPENMP
	Mutex m_writelock_appearance_cache;
#endif

public:
	ConverterOSG( shared_ptr<GeometrySettings>& geom_settings ) : m_geom_settings(geom_settings)
	{
		m_cull_back_off = new osg::CullFace( osg::CullFace::BACK );
		m_glass_stateset = new osg::StateSet();
		m_glass_stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
		m_glass_stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	}
	virtual ~ConverterOSG() {}

	// Map: IfcProduct ID -> scenegraph switch
	std::map<int, osg::ref_ptr<osg::Switch> >& getMapEntityIdToSwitch() { return m_map_entity_id_to_switch; }
	std::map<std::wstring, osg::ref_ptr<osg::Switch> >& getMapEntityGUIDToSwitch() { return m_map_entity_guid_to_switch; }
	// Map: Representation Identifier -> scenegraph switch
	std::map<int, osg::ref_ptr<osg::Switch> >& getMapRepresentationToSwitch() { return m_map_representation_id_to_switch; }

	void clearInputCache()
	{
		m_map_entity_id_to_switch.clear();
		m_map_entity_guid_to_switch.clear();
		m_map_representation_id_to_switch.clear();
		m_vec_existing_statesets.clear();
	}

	static void drawBoundingBox( const carve::geom::aabb<3>& aabb, osg::Geometry* geom )
	{
		osg::ref_ptr<osg::Vec3Array> vertices = dynamic_cast<osg::Vec3Array*>( geom->getVertexArray() );
		if( !vertices )
		{
			vertices = new osg::Vec3Array();
			geom->setVertexArray( vertices );
		}
		const carve::geom::vector<3>& aabb_pos = aabb.pos;
		const carve::geom::vector<3>& extent = aabb.extent;
		const double dex = extent.x;
		const double dey = extent.y;
		const double dez = extent.z;

		const int vert_id_offset = vertices->size();
		vertices->push_back( osg::Vec3f( aabb_pos.x - dex, aabb_pos.y - dey, aabb_pos.z - dez ) );
		vertices->push_back( osg::Vec3f( aabb_pos.x + dex, aabb_pos.y - dey, aabb_pos.z - dez ) );
		vertices->push_back( osg::Vec3f( aabb_pos.x + dex, aabb_pos.y + dey, aabb_pos.z - dez ) );
		vertices->push_back( osg::Vec3f( aabb_pos.x - dex, aabb_pos.y + dey, aabb_pos.z - dez ) );

		vertices->push_back( osg::Vec3f( aabb_pos.x - dex, aabb_pos.y - dey, aabb_pos.z + dez ) );
		vertices->push_back( osg::Vec3f( aabb_pos.x + dex, aabb_pos.y - dey, aabb_pos.z + dez ) );
		vertices->push_back( osg::Vec3f( aabb_pos.x + dex, aabb_pos.y + dey, aabb_pos.z + dez ) );
		vertices->push_back( osg::Vec3f( aabb_pos.x - dex, aabb_pos.y + dey, aabb_pos.z + dez ) );

		osg::ref_ptr<osg::DrawElementsUInt> box_lines = new osg::DrawElementsUInt( GL_LINE_STRIP, 0 );
		box_lines->push_back( vert_id_offset + 0 );
		box_lines->push_back( vert_id_offset + 1 );
		box_lines->push_back( vert_id_offset + 2 );
		box_lines->push_back( vert_id_offset + 3 );
		box_lines->push_back( vert_id_offset + 0 );
		box_lines->push_back( vert_id_offset + 4 );
		box_lines->push_back( vert_id_offset + 5 );
		box_lines->push_back( vert_id_offset + 1 );
		box_lines->push_back( vert_id_offset + 5 );
		box_lines->push_back( vert_id_offset + 6 );
		box_lines->push_back( vert_id_offset + 2 );
		box_lines->push_back( vert_id_offset + 6 );
		box_lines->push_back( vert_id_offset + 7 );
		box_lines->push_back( vert_id_offset + 3 );
		box_lines->push_back( vert_id_offset + 7 );
		box_lines->push_back( vert_id_offset + 4 );
		geom->addPrimitiveSet( box_lines );

		osg::ref_ptr<osg::Material> mat = new osg::Material();
		if( !mat ) { throw OutOfMemoryException(); }
		osg::Vec4f ambientColor( 1.f, 0.2f, 0.1f, 1.f );
		mat->setAmbient( osg::Material::FRONT_AND_BACK, ambientColor );
		mat->setDiffuse( osg::Material::FRONT_AND_BACK, ambientColor );
		mat->setSpecular( osg::Material::FRONT_AND_BACK, ambientColor );
		//mat->setShininess( osg::Material::FRONT_AND_BACK, shininess );
		//mat->setColorMode( osg::Material::SPECULAR );

		osg::StateSet* stateset = geom->getOrCreateStateSet();
		if( !stateset ) { throw OutOfMemoryException(); }
		stateset->setAttribute( mat, osg::StateAttribute::ON );
		stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	}

	static void drawFace( const carve::mesh::Face<3>* face, osg::Geode* geode, bool add_color_array = false )
	{
#ifdef _DEBUG
		std::cout << "not triangulated" << std::endl;
#endif
		std::vector<vec3> face_vertices;
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

		vec3* vertex_vec;
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array( num_vertices );
		if( !vertices ) { throw OutOfMemoryException(); }
		osg::ref_ptr<osg::DrawElementsUInt> triangles = new osg::DrawElementsUInt( osg::PrimitiveSet::POLYGON, num_vertices );
		if( !triangles ) { throw OutOfMemoryException(); }

		for( size_t i = 0; i < num_vertices; ++i )
		{
			vertex_vec = &face_vertices[num_vertices - i - 1];
			( *vertices )[i].set( vertex_vec->x, vertex_vec->y, vertex_vec->z );
			( *triangles )[i] = i;
		}

		osg::Vec3f poly_normal = SceneGraphUtils::computePolygonNormal( vertices );
		osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();
		normals->resize( num_vertices, poly_normal );


		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
		geometry->setVertexArray( vertices );
		geometry->setNormalArray( normals );
		normals->setBinding( osg::Array::BIND_PER_VERTEX );
		geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POLYGON, 0, vertices->size() ) );

		if( add_color_array )
		{
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->resize( vertices->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );
			colors->setBinding( osg::Array::BIND_PER_VERTEX );
			geometry->setColorArray( colors );
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
		osg::ref_ptr<osg::Vec3Array> vertices_normals = new osg::Vec3Array();
		for( size_t i = 0; i < num_vertices; ++i )
		{
			vertex_vec = &face_vertices[num_vertices - i - 1];
			vertices_normals->push_back( osg::Vec3f( vertex_vec->x, vertex_vec->y, vertex_vec->z ) );
			vertices_normals->push_back( osg::Vec3f( vertex_vec->x, vertex_vec->y, vertex_vec->z ) + poly_normal );
		}

		osg::ref_ptr<osg::Vec4Array> colors_normals = new osg::Vec4Array();
		colors_normals->resize( num_vertices * 2, osg::Vec4f( 0.4f, 0.7f, 0.4f, 1.f ) );

		osg::ref_ptr<osg::Geometry> geometry_normals = new osg::Geometry();
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
		if( !vertices_tri ) { throw OutOfMemoryException(); }
		osg::ref_ptr<osg::Vec3Array> normals_tri = new osg::Vec3Array();
		if( !normals_tri ) { throw OutOfMemoryException(); }

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
					std::vector<vec2> projected;
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
				const vec3 face_normal = face->plane.N;

				if( crease_angle > 0 )
				{
					carve::mesh::Edge<3>* e = face->edge;
					for( size_t jj = 0; jj < n_vertices; ++jj )
					{
						carve::mesh::Vertex<3>* vertex = e->vert;
						vec3 intermediate_normal;

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

							vec3 f1_normal = f1->plane.N;
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

						const vec3& vertex_v = vertex->v;
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
						const vec3& vertex_v = vertex->v;

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
			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			if( !geometry ) { throw OutOfMemoryException(); }
			geometry->setVertexArray( vertices_tri );

			geometry->setNormalArray( normals_tri );
			normals_tri->setBinding( osg::Array::BIND_PER_VERTEX );

			if( add_color_array )
			{
				osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
				if( !colors ) { throw OutOfMemoryException(); }
				colors->resize( vertices_tri->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );
				colors->setBinding( osg::Array::BIND_PER_VERTEX );
				geometry->setColorArray( colors );
			}

			geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vertices_tri->size() ) );
			if( !geometry ) { throw OutOfMemoryException(); }
			geode->addDrawable( geometry );

#ifdef DEBUG_DRAW_NORMALS
			osg::ref_ptr<osg::Vec3Array> vertices_normals = new osg::Vec3Array();
			for( size_t i = 0; i < vertices_tri->size(); ++i )
			{
				osg::Vec3f& vertex_vec = vertices_tri->at( i );// [i];
				osg::Vec3f& normal_vec = normals_tri->at( i );
				vertices_normals->push_back( osg::Vec3f( vertex_vec.x(), vertex_vec.y(), vertex_vec.z() ) );
				vertices_normals->push_back( osg::Vec3f( vertex_vec.x(), vertex_vec.y(), vertex_vec.z() ) + normal_vec );
			}

			osg::ref_ptr<osg::Vec4Array> colors_normals = new osg::Vec4Array();
			colors_normals->resize( vertices_normals->size(), osg::Vec4f( 0.4f, 0.7f, 0.4f, 1.f ) );

			osg::ref_ptr<osg::Geometry> geometry_normals = new osg::Geometry();
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
				osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
				if( !geometry ) { throw OutOfMemoryException(); }
				geometry->setVertexArray( vertices_quad );
				if( normals_quad )
				{
					normals_quad->setBinding( osg::Array::BIND_PER_VERTEX );
					geometry->setNormalArray( normals_quad );
				}

				if( add_color_array )
				{
					osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
					if( !colors ) { throw OutOfMemoryException(); }
					colors->resize( vertices_quad->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );
					colors->setBinding( osg::Array::BIND_PER_VERTEX );
					geometry->setColorArray( colors );
				}

				geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, vertices_quad->size() ) );
				if( !geometry ) { throw OutOfMemoryException(); }
				geode->addDrawable( geometry );
			}
		}
	}

	static void drawPolyline( const carve::input::PolylineSetData* polyline_data, osg::Geode* geode, bool add_color_array = false )
	{
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
		if( !vertices ) { throw OutOfMemoryException(); }
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
			const carve::line::Polyline* pline = *it;
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

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
		if( !geometry ) { throw OutOfMemoryException(); }
		geometry->setVertexArray( vertices );
		geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, vertices->size() ) );

		if( add_color_array )
		{
			osg::Vec4f color( 0.6f, 0.6f, 0.6f, 0.1f );
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array( vertices->size(), &color );
			if( !colors ) { throw OutOfMemoryException(); }
			colors->setBinding( osg::Array::BIND_PER_VERTEX );
			geometry->setColorArray( colors );
		}

		geode->addDrawable( geometry );
	}

	void computeCreaseEdgesFromMeshset( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::vector<carve::mesh::Edge<3>* >& vec_edges_out, const double crease_angle )
	{
		if( !meshset )
		{
			return;
		}

		for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
		{
			const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];
			const std::vector<carve::mesh::Edge<3>* >& vec_closed_edges = mesh->closed_edges;

			for( size_t i_edge = 0; i_edge < vec_closed_edges.size(); ++i_edge )
			{
				carve::mesh::Edge<3>* edge = vec_closed_edges[i_edge];

				if( !edge )
				{
					continue;
				}
				carve::mesh::Edge<3>* edge_reverse = edge->rev;
				if( !edge_reverse )
				{
					continue;
				}
				carve::mesh::Face<3>* face = edge->face;
				carve::mesh::Face<3>* face_reverse = edge_reverse->face;

				const carve::geom::vector<3>& f1_normal = face->plane.N;
				const carve::geom::vector<3>& f2_normal = face_reverse->plane.N;
				const double cos_angle = dot( f1_normal, f2_normal );
				if( cos_angle > 0 )
				{
					const double deviation = std::abs( cos_angle - 1.0 );
					if( deviation < crease_angle )
					{
						continue;
					}
				}
				// TODO: if area of face and face_reverse is equal, skip the crease edge. It could be the inside or outside of a cylinder. Check also if > 2 faces in a row have same normal angle differences
				vec_edges_out.push_back( edge );
			}
		}
	}

	void renderMeshsetCreaseEdges( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, osg::Geode* target_geode, const double crease_angle )
	{
		if( !meshset )
		{
			return;
		}
		if( !target_geode )
		{
			return;
		}
		std::vector<carve::mesh::Edge<3>* > vec_crease_edges;
		computeCreaseEdgesFromMeshset( meshset, vec_crease_edges, crease_angle );

		if( vec_crease_edges.size() > 0 )
		{
			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
			for( size_t i_edge = 0; i_edge < vec_crease_edges.size(); ++i_edge )
			{
				const carve::mesh::Edge<3>* edge = vec_crease_edges[i_edge];
				const carve::geom::vector<3>& vertex1 = edge->v1()->v;
				const carve::geom::vector<3>& vertex2 = edge->v2()->v;
				vertices->push_back( osg::Vec3d( vertex1.x, vertex1.y, vertex1.z ) );
				vertices->push_back( osg::Vec3d( vertex2.x, vertex2.y, vertex2.z ) );
			}

			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			geometry->setName("creaseEdges");
			geometry->setVertexArray( vertices );
			geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vertices->size() ) );
			geometry->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
			geometry->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
			geometry->getOrCreateStateSet()->setAttributeAndModes( new osg::LineWidth( 3.0f ), osg::StateAttribute::ON );
			geometry->getOrCreateStateSet()->setMode( GL_LINE_SMOOTH, osg::StateAttribute::ON );
			geometry->getOrCreateStateSet()->setAttributeAndModes( new osg::Hint( GL_LINE_SMOOTH_HINT, GL_NICEST ), osg::StateAttribute::ON );
			geometry->getOrCreateStateSet()->setRenderBinDetails( 10, "RenderBin");
			target_geode->addDrawable( geometry );
		}
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

			if( appearance->m_apply_to_geometry_type == AppearanceData::GEOM_TYPE_SURFACE || appearance->m_apply_to_geometry_type == AppearanceData::GEOM_TYPE_ANY )
			{
				osg::ref_ptr<osg::StateSet> item_stateset;
				convertToOSGStateSet( appearance, item_stateset );
				if( item_stateset )
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
			else if( appearance->m_apply_to_geometry_type == AppearanceData::GEOM_TYPE_CURVE )
			{

			}
		}
	}

	osg::Matrixd convertMatrixToOSG( const carve::math::Matrix& mat_in )
	{
		return osg::Matrixd( mat_in.m[0][0], mat_in.m[0][1], mat_in.m[0][2], mat_in.m[0][3],
			mat_in.m[1][0], mat_in.m[1][1], mat_in.m[1][2], mat_in.m[1][3],
			mat_in.m[2][0], mat_in.m[2][1], mat_in.m[2][2], mat_in.m[2][3],
			mat_in.m[3][0], mat_in.m[3][1], mat_in.m[3][2], mat_in.m[3][3] );
	}

	//\brief method convertProductShapeToOSG: creates geometry objects from an IfcProduct object
	// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
	void convertProductShapeToOSG( shared_ptr<ProductShapeData>& product_shape, std::map<int, osg::ref_ptr<osg::Switch> >& map_representation_switches )
	{
		if( product_shape->m_ifc_object_definition.expired() )
		{
			return;
		}
		
		shared_ptr<IfcObjectDefinition> ifc_object_def( product_shape->m_ifc_object_definition );
		shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>( ifc_object_def );
		if( !ifc_product )
		{
			return;
		}
		const int product_id = ifc_product->m_entity_id;
		std::stringstream strs_product_switch_name;
		strs_product_switch_name << "#" << product_id << "=" << ifc_product->className() << " group";
		bool draw_bounding_box = false;
		
		// create OSG objects
		std::vector<shared_ptr<RepresentationData> >& vec_product_representations = product_shape->m_vec_representations;
		for( size_t ii_representation = 0; ii_representation < vec_product_representations.size(); ++ii_representation )
		{
			const shared_ptr<RepresentationData>& product_representation_data = vec_product_representations[ii_representation];
			if( product_representation_data->m_ifc_representation.expired() )
			{
				continue;
			}
			shared_ptr<IfcRepresentation> ifc_representation( product_representation_data->m_ifc_representation );
			const int representation_id = ifc_representation->m_entity_id;
			osg::ref_ptr<osg::Switch> representation_switch = new osg::Switch();
			
#ifdef _DEBUG
			std::stringstream strs_representation_name;
			strs_representation_name << strs_product_switch_name.str().c_str() << ", representation " << ii_representation;
			representation_switch->setName( strs_representation_name.str().c_str() );
#endif

			const std::vector<shared_ptr<ItemShapeData> >& product_items = product_representation_data->m_vec_item_data;
			for( size_t i_item = 0; i_item < product_items.size(); ++i_item )
			{
				const shared_ptr<ItemShapeData>& item_shape = product_items[i_item];
				osg::ref_ptr<osg::MatrixTransform> item_group = new osg::MatrixTransform();
				if( !item_group ) { throw OutOfMemoryException( __FUNC__ ); }

#ifdef _DEBUG
				std::stringstream strs_item_name;
				strs_item_name << strs_representation_name.str().c_str() << ", item " << i_item;
				item_group->setName( strs_item_name.str().c_str() );
#endif

				// create shape for open shells
				for( size_t ii = 0; ii < item_shape->m_meshsets_open.size(); ++ii )
				{
					shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = item_shape->m_meshsets_open[ii];
					CSG_Adapter::retriangulateMeshSet( item_meshset );
					osg::ref_ptr<osg::Geode> geode = new osg::Geode();
					if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
					drawMeshSet( item_meshset, geode, m_geom_settings->getCoplanarFacesMaxDeltaAngle() );

					if( m_geom_settings->getRenderCreaseEdges() )
					{
						renderMeshsetCreaseEdges( item_meshset, geode, m_geom_settings->getCreaseEdgesMaxDeltaAngle() );
					}

					// disable back face culling for open meshes
					geode->getOrCreateStateSet()->setAttributeAndModes( m_cull_back_off.get(), osg::StateAttribute::OFF );
					item_group->addChild( geode );

					if( draw_bounding_box )
					{
						carve::geom::aabb<3> bbox = item_meshset->getAABB();
						osg::ref_ptr<osg::Geometry> bbox_geom = new osg::Geometry();
						drawBoundingBox( bbox, bbox_geom );
						geode->addDrawable( bbox_geom );
					}

#ifdef _DEBUG
					std::stringstream strs_item_meshset_name;
					strs_item_meshset_name << strs_item_name.str().c_str() << ", open meshset " << ii;
					geode->setName( strs_item_meshset_name.str().c_str() );
#endif
				}

				// create shape for meshsets
				for( size_t ii = 0; ii < item_shape->m_meshsets.size(); ++ii )
				{
					shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = item_shape->m_meshsets[ii];
					CSG_Adapter::retriangulateMeshSet( item_meshset );
					osg::ref_ptr<osg::Geode> geode_meshset = new osg::Geode();
					if( !geode_meshset ) { throw OutOfMemoryException( __FUNC__ ); }
					drawMeshSet( item_meshset, geode_meshset, m_geom_settings->getCoplanarFacesMaxDeltaAngle() );
					item_group->addChild( geode_meshset );

					if( m_geom_settings->getRenderCreaseEdges() )
					{
						renderMeshsetCreaseEdges( item_meshset, geode_meshset, m_geom_settings->getCreaseEdgesMaxDeltaAngle() );
					}

					if( draw_bounding_box )
					{
						carve::geom::aabb<3> bbox = item_meshset->getAABB();
						osg::ref_ptr<osg::Geometry> bbox_geom = new osg::Geometry();
						drawBoundingBox( bbox, bbox_geom );
						geode_meshset->addDrawable( bbox_geom );
					}

#ifdef _DEBUG
					std::stringstream strs_item_meshset_name;
					strs_item_meshset_name << strs_item_name.str().c_str() << ", meshset " << ii;
					geode_meshset->setName( strs_item_meshset_name.str().c_str() );
#endif
				}

				// create shape for points
				const std::vector<shared_ptr<carve::input::VertexData> >& vertex_points = item_shape->getVertexPoints();
				for( size_t ii = 0; ii < vertex_points.size(); ++ii )
				{
					const shared_ptr<carve::input::VertexData>& pointset_data = vertex_points[ii];
					if( pointset_data )
					{
						if( pointset_data->points.size() > 0 )
						{
							osg::ref_ptr<osg::Geode> geode = new osg::Geode();
							if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }

							osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
							for( size_t i_pointset_point = 0; i_pointset_point < pointset_data->points.size(); ++i_pointset_point )
							{
								vec3& carve_point = pointset_data->points[i_pointset_point];
								vertices->push_back( osg::Vec3d( carve_point.x, carve_point.y, carve_point.z ) );
							}

							osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
							geometry->setVertexArray( vertices );
							geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POINTS, 0, vertices->size() ) );
							geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
							geode->getOrCreateStateSet()->setAttribute( new osg::Point( 3.0f ), osg::StateAttribute::ON );
							geode->addDrawable( geometry );
							geode->setCullingActive( false );
							item_group->addChild( geode );

#ifdef _DEBUG
							std::stringstream strs_item_meshset_name;
							strs_item_meshset_name << strs_item_name.str().c_str() << ", vertex_point " << ii;
							geode->setName( strs_item_meshset_name.str().c_str() );
#endif
						}
					}
				}

				// create shape for polylines
				for( size_t ii = 0; ii < item_shape->m_polylines.size(); ++ii )
				{
					shared_ptr<carve::input::PolylineSetData>& polyline_data = item_shape->m_polylines[ii];
					osg::ref_ptr<osg::Geode> geode = new osg::Geode();
					if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
					geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
					drawPolyline( polyline_data.get(), geode );
					item_group->addChild( geode );

#ifdef _DEBUG
					std::stringstream strs_item_meshset_name;
					strs_item_meshset_name << strs_item_name.str().c_str() << ", polylines " << ii;
					geode->setName( strs_item_meshset_name.str().c_str() );
#endif
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

						osg::ref_ptr<osgText::Text> txt = new osgText::Text();
						if( !txt )
						{
							throw OutOfMemoryException( __FUNC__ );
						}
						txt->setFont( "fonts/arial.ttf" );
						txt->setColor( osg::Vec4f( 0, 0, 0, 1 ) );
						txt->setCharacterSize( 0.1f );
						txt->setAutoRotateToScreen( true );
						txt->setPosition( pos2 );
						txt->setText( text_str.c_str() );
						txt->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

						osg::ref_ptr<osg::Geode> geode = new osg::Geode();
						if( !geode ){ throw OutOfMemoryException( __FUNC__ ); }
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
#ifdef _DEBUG
					if( item_group->getNumParents() > 0 )
					{
						std::cout << __FUNC__ << ": item_group->getNumParents() > 0" << std::endl;
					}
#endif
					representation_switch->addChild( item_group );
				}
			}

			// apply statesets if there are any
			if( product_representation_data->m_vec_representation_appearances.size() > 0 )
			{
				applyAppearancesToGroup( product_representation_data->m_vec_representation_appearances, representation_switch );
			}

			// If anything has been created, add it to the product group
			if( representation_switch->getNumChildren() > 0 )
			{
#ifdef _DEBUG
				if( representation_switch->getNumParents() > 0 )
				{
					std::cout << __FUNC__ << ": product_representation_switch->getNumParents() > 0" << std::endl;
				}
#endif
				// enable transparency for certain objects
				if( dynamic_pointer_cast<IfcSpace>(ifc_product) )
				{
					representation_switch->setStateSet( m_glass_stateset );
				}
				else if( dynamic_pointer_cast<IfcCurtainWall>(ifc_product) || dynamic_pointer_cast<IfcWindow>(ifc_product) )
				{
					representation_switch->setStateSet( m_glass_stateset );
					SceneGraphUtils::setMaterialAlpha( representation_switch, 0.6f, true );
				}

				// check if parent building element is window
				if( ifc_product->m_Decomposes_inverse.size() > 0 )
				{
					for( size_t ii_decomposes = 0; ii_decomposes < ifc_product->m_Decomposes_inverse.size(); ++ii_decomposes )
					{
						const weak_ptr<IfcRelAggregates>& decomposes_weak = ifc_product->m_Decomposes_inverse[ii_decomposes];
						if( decomposes_weak.expired() )
						{
							continue;
						}
						shared_ptr<IfcRelAggregates> decomposes_ptr(decomposes_weak);
						shared_ptr<IfcObjectDefinition>& relating_object = decomposes_ptr->m_RelatingObject;
						if( relating_object )
						{
							if( dynamic_pointer_cast<IfcCurtainWall>(relating_object) || dynamic_pointer_cast<IfcWindow>(relating_object) )
							{
								representation_switch->setStateSet(m_glass_stateset);
								SceneGraphUtils::setMaterialAlpha(representation_switch, 0.6f, true);
							}
						}
					}
				}

				map_representation_switches.insert( std::make_pair( representation_id, representation_switch ) );
			}
		}

		// TODO: if no color or material is given, set color 231/219/169 for walls, 140/140/140 for slabs 
	}

	/*\brief method convertToOSG: Creates geometry for OpenSceneGraph from given ProductShapeData.
	\param[out] parent_group Group to append the geometry.
	**/
	void convertToOSG( const std::map<int, shared_ptr<ProductShapeData> >& map_shape_data, osg::ref_ptr<osg::Switch> parent_group )
	{
		progressTextCallback( L"Converting geometry to OpenGL format ..." );
		progressValueCallback( 0, "scenegraph" );
		m_map_entity_id_to_switch.clear();
		m_map_entity_guid_to_switch.clear();
		m_map_representation_id_to_switch.clear();
		m_vec_existing_statesets.clear();

		shared_ptr<ProductShapeData> ifc_project_data;
		std::vector<shared_ptr<ProductShapeData> > vec_products;

		for( auto it = map_shape_data.begin(); it != map_shape_data.end(); ++it )
		{
			shared_ptr<ProductShapeData> shape_data = it->second;
			if( shape_data )
			{
				vec_products.push_back( shape_data );
			}
		}

		// create geometry for for each IfcProduct independently, spatial structure will be resolved later
		std::map<int, osg::ref_ptr<osg::Switch> >* map_entity_id = &m_map_entity_id_to_switch;
		std::map<std::wstring, osg::ref_ptr<osg::Switch> >* map_entity_guid = &m_map_entity_guid_to_switch;
		std::map<int, osg::ref_ptr<osg::Switch> >* map_representations = &m_map_representation_id_to_switch;
		const int num_products = (int)vec_products.size();

#ifdef ENABLE_OPENMP
		Mutex writelock_map;
		Mutex writelock_message_callback;
		Mutex writelock_ifc_project;

#pragma omp parallel firstprivate(num_products) shared(map_entity_id, map_entity_guid, map_representations)
		{
			// time for one product may vary significantly, so schedule not so many
#pragma omp for schedule(dynamic,40)
#endif
			for( int i = 0; i < num_products; ++i )
			{
				shared_ptr<ProductShapeData>& shape_data = vec_products[i];
				
				weak_ptr<IfcObjectDefinition>& ifc_object_def_weak = shape_data->m_ifc_object_definition;
				if( ifc_object_def_weak.expired() )
				{
					continue;
				}
				shared_ptr<IfcObjectDefinition> ifc_object_def( ifc_object_def_weak );
				
				std::stringstream thread_err;
				if( dynamic_pointer_cast<IfcFeatureElementSubtraction>(ifc_object_def) )
				{
					// geometry will be created in method subtractOpenings
					continue;
				}
				else if( dynamic_pointer_cast<IfcProject>(ifc_object_def) )
				{
#ifdef ENABLE_OPENMP
					ScopedLock scoped_lock( writelock_ifc_project );
#endif
					ifc_project_data = shape_data;
				}

				shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(ifc_object_def);
				if( !ifc_product )
				{
					continue;
				}

				if( !ifc_product->m_Representation )
				{
					continue;
				}
				
				const int product_id = ifc_product->m_entity_id;
				std::wstring product_guid = L"";
				std::map<int, osg::ref_ptr<osg::Switch> > map_representation_switches;
				try
				{
					convertProductShapeToOSG( shape_data, map_representation_switches );
				}
				catch( OutOfMemoryException& e )
				{
					throw e;
				}
				catch( BuildingException& e )
				{
					thread_err << e.what();
				}
				catch( carve::exception& e )
				{
					thread_err << e.str();
				}
				catch( std::exception& e )
				{
					thread_err << e.what();
				}
				catch( ... )
				{
					thread_err << "undefined error, product id " << product_id;
				}

				if (ifc_object_def->m_GlobalId)
				{
					product_guid = ifc_object_def->m_GlobalId->m_value;
				}

				if( map_representation_switches.size() > 0 )
				{
					osg::ref_ptr<osg::Switch> product_switch = new osg::Switch();

					osg::ref_ptr<osg::MatrixTransform> product_transform = new osg::MatrixTransform();
					product_transform->setMatrix( convertMatrixToOSG( shape_data->getTransform() ) );
					product_switch->addChild( product_transform );

					std::stringstream strs_product_switch_name;
					strs_product_switch_name << "#" << product_id << "=" << ifc_product->className() << " group";
					product_switch->setName( strs_product_switch_name.str().c_str() );

					for( auto it_map = map_representation_switches.begin(); it_map != map_representation_switches.end(); ++it_map )
					{
						osg::ref_ptr<osg::Switch>& repres_switch = it_map->second;
						product_transform->addChild( repres_switch );
					}

					// apply statesets if there are any
					const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances = shape_data->getAppearances();
					if( vec_product_appearances.size() > 0 )
					{
						applyAppearancesToGroup( vec_product_appearances, product_switch );
					}

#ifdef ENABLE_OPENMP
					ScopedLock scoped_lock( writelock_map );
#endif
					map_entity_id->insert( std::make_pair( product_id, product_switch ) );
					map_entity_guid->insert(std::make_pair(product_guid, product_switch));
					map_representations->insert( map_representation_switches.begin(), map_representation_switches.end() );
				}

				if( thread_err.tellp() > 0 )
				{
#ifdef ENABLE_OPENMP
					ScopedLock scoped_lock( writelock_message_callback );
#endif
					messageCallback( thread_err.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
				}

				// progress callback
				double progress = (double)i / (double)num_products;
				if( progress - m_recent_progress > 0.02 )
				{
#ifdef ENABLE_OPENMP
					if( omp_get_thread_num() == 0 )
#endif
					{
						// leave 10% of progress to openscenegraph internals
						progressValueCallback( progress*0.9, "scenegraph" );
						m_recent_progress = progress;
					}
				}
			}
#ifdef ENABLE_OPENMP
		} // implicit barrier
#endif

		try
		{
			// now resolve spatial structure
			if( ifc_project_data )
			{
				resolveProjectStructure( ifc_project_data, parent_group );
			}
		}
		catch( OutOfMemoryException& e )
		{
			throw e;
		}
		catch( BuildingException& e )
		{
			messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
		}
		catch( std::exception& e )
		{
			messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
		}
		catch( ... )
		{
			messageCallback( "undefined error", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		}
		progressValueCallback( 0.9, "scenegraph" );
	}

	void addNodes( const std::map<int, shared_ptr<BuildingObject> >& map_shape_data, osg::ref_ptr<osg::Switch>& target_group )
	{
		// check if there are entities that are not in spatial structure
		if( !target_group )
		{
			target_group = new osg::Switch();
		}

		for( auto it_product_shapes = map_shape_data.begin(); it_product_shapes != map_shape_data.end(); ++it_product_shapes )
		{
			int product_id = it_product_shapes->first;
			auto it_find = m_map_entity_id_to_switch.find( product_id );

			if( it_find != m_map_entity_id_to_switch.end() )
			{
				osg::ref_ptr<osg::Switch>& sw = it_find->second;
				if( sw )
				{
					target_group->addChild( sw );
				}
			}
		}
	}

	void resolveProjectStructure( const shared_ptr<ProductShapeData>& product_data, osg::ref_ptr<osg::Switch> group )
	{
		if( !product_data )
		{
			return;
		}

		if( product_data->m_ifc_object_definition.expired() )
		{
			return;
		}

		shared_ptr<IfcObjectDefinition> object_def( product_data->m_ifc_object_definition );
		const int entity_id = object_def->m_entity_id;
		if( SceneGraphUtils::inParentList( entity_id, group ) )
		{
			messageCallback( "Cycle in project structure detected", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, object_def.get() );
			return;
		}

		const std::vector<shared_ptr<ProductShapeData> >& vec_children = product_data->getChildren();
		for( size_t ii = 0; ii < vec_children.size(); ++ii )
		{
			const shared_ptr<ProductShapeData>& child_product_data = vec_children[ii];
			if( !child_product_data )
			{
				continue;
			}

			osg::ref_ptr<osg::Switch> group_subparts = new osg::Switch();
			if( !child_product_data->m_ifc_object_definition.expired() )
			{
				shared_ptr<IfcObjectDefinition> child_obj_def( child_product_data->m_ifc_object_definition );
				std::stringstream group_subparts_name;
				group_subparts_name << "#" << child_obj_def->m_entity_id << "=";
				group_subparts_name << child_obj_def->className();
				group_subparts->setName( group_subparts_name.str().c_str() );
			}

			group->addChild( group_subparts );
			resolveProjectStructure( child_product_data, group_subparts );
		}

		auto it_product_map = m_map_entity_id_to_switch.find( entity_id );
		if( it_product_map != m_map_entity_id_to_switch.end() )
		{
			const osg::ref_ptr<osg::Switch>& product_switch = it_product_map->second;
			if( product_switch )
			{
				group->addChild( product_switch );
			}
		}
		else
		{
			if( group->getNumChildren() == 0 )
			{
				osg::ref_ptr<osg::Switch> product_switch = new osg::Switch();
				group->addChild( product_switch );

				std::stringstream switch_name;
				switch_name << "#" << entity_id << "=" << object_def->className();
				product_switch->setName( switch_name.str().c_str() );
			}
		}
	}

	void clearAppearanceCache()
	{
#ifdef ENABLE_OPENMP
		ScopedLock lock( m_writelock_appearance_cache );
#endif
		m_vec_existing_statesets.clear();
	}

	void convertToOSGStateSet( const shared_ptr<AppearanceData>& appearence, osg::ref_ptr<osg::StateSet>& target_stateset )
	{
		if( !appearence )
		{
			return;
		}
		const float shininess = appearence->m_shininess;
		const float transparency = appearence->m_transparency;
		const bool set_transparent = appearence->m_set_transparent;

		const float color_ambient_r = appearence->m_color_ambient.r();
		const float color_ambient_g = appearence->m_color_ambient.g();
		const float color_ambient_b = appearence->m_color_ambient.b();
		const float color_ambient_a = appearence->m_color_ambient.a();

		const float color_diffuse_r = appearence->m_color_diffuse.r();
		const float color_diffuse_g = appearence->m_color_diffuse.g();
		const float color_diffuse_b = appearence->m_color_diffuse.b();
		const float color_diffuse_a = appearence->m_color_diffuse.a();

		const float color_specular_r = appearence->m_color_specular.r();
		const float color_specular_g = appearence->m_color_specular.g();
		const float color_specular_b = appearence->m_color_specular.b();
		const float color_specular_a = appearence->m_color_specular.a();

		if( m_enable_stateset_caching )
		{

#ifdef ENABLE_OPENMP
			ScopedLock lock( m_writelock_appearance_cache );
#endif

			for( size_t i = 0; i<m_vec_existing_statesets.size(); ++i )
			{
				const osg::ref_ptr<osg::StateSet> stateset_existing = m_vec_existing_statesets[i];

				if( !stateset_existing.valid() )
				{
					continue;
				}

				osg::ref_ptr<osg::Material> mat_existing = (osg::Material*)stateset_existing->getAttribute( osg::StateAttribute::MATERIAL );
				if( !mat_existing )
				{
					continue;
				}

				// compare
				osg::Vec4f color_ambient_existing = mat_existing->getAmbient( osg::Material::FRONT_AND_BACK );
				if( fabs( color_ambient_existing.r() - color_ambient_r ) > 0.03 ) break;
				if( fabs( color_ambient_existing.g() - color_ambient_g ) > 0.03 ) break;
				if( fabs( color_ambient_existing.b() - color_ambient_b ) > 0.03 ) break;
				if( fabs( color_ambient_existing.a() - color_ambient_a ) > 0.03 ) break;

				osg::Vec4f color_diffuse_existing = mat_existing->getDiffuse( osg::Material::FRONT_AND_BACK );
				if( fabs( color_diffuse_existing.r() - color_diffuse_r ) > 0.03 ) break;
				if( fabs( color_diffuse_existing.g() - color_diffuse_g ) > 0.03 ) break;
				if( fabs( color_diffuse_existing.b() - color_diffuse_b ) > 0.03 ) break;
				if( fabs( color_diffuse_existing.a() - color_diffuse_a ) > 0.03 ) break;

				osg::Vec4f color_specular_existing = mat_existing->getSpecular( osg::Material::FRONT_AND_BACK );
				if( fabs( color_specular_existing.r() - color_specular_r ) > 0.03 ) break;
				if( fabs( color_specular_existing.g() - color_specular_g ) > 0.03 ) break;
				if( fabs( color_specular_existing.b() - color_specular_b ) > 0.03 ) break;
				if( fabs( color_specular_existing.a() - color_specular_a ) > 0.03 ) break;

				float shininess_existing = mat_existing->getShininess( osg::Material::FRONT_AND_BACK );
				if( fabs( shininess_existing - shininess ) > 0.03 ) break;

				bool blend_on_existing = stateset_existing->getMode( GL_BLEND ) == osg::StateAttribute::ON;
				if( blend_on_existing != set_transparent ) break;

				bool transparent_bin = stateset_existing->getRenderingHint() == osg::StateSet::TRANSPARENT_BIN;
				if( transparent_bin != set_transparent ) break;

				// if we get here, appearance is same as existing state set
				// TODO: block this re-used stateset for merging, or prevent merged statesets from being re-used
				target_stateset = stateset_existing;
				return;
			}
		}

		osg::Vec4f ambientColor(	color_ambient_r,	color_ambient_g,	color_ambient_b,	transparency );
		osg::Vec4f diffuseColor(	color_diffuse_r,	color_diffuse_g,	color_diffuse_b,	transparency  );
		osg::Vec4f specularColor(	color_specular_r,	color_specular_g,	color_specular_b,	transparency );

		// TODO: material caching and re-use
		osg::ref_ptr<osg::Material> mat = new osg::Material();
		if( !mat ){ throw OutOfMemoryException(); }
		mat->setAmbient( osg::Material::FRONT_AND_BACK, ambientColor );
		mat->setDiffuse( osg::Material::FRONT_AND_BACK, diffuseColor );
		mat->setSpecular( osg::Material::FRONT_AND_BACK, specularColor );
		mat->setShininess( osg::Material::FRONT_AND_BACK, shininess );
		mat->setColorMode( osg::Material::SPECULAR );

		target_stateset = new osg::StateSet();
		if( !target_stateset ){ throw OutOfMemoryException(); }
		target_stateset->setAttribute( mat, osg::StateAttribute::ON );
	
		if( appearence->m_set_transparent )
		{
			mat->setTransparency( osg::Material::FRONT_AND_BACK, transparency );	
			target_stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
			target_stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
		}

		if( appearence->m_specular_exponent != 0.f )
		{
			//osg::ref_ptr<osgFX::SpecularHighlights> spec_highlights = new osgFX::SpecularHighlights();
			//spec_highlights->setSpecularExponent( spec->m_value );
			// todo: add to scenegraph
		}

		if( m_enable_stateset_caching )
		{
			m_vec_existing_statesets.push_back( target_stateset );
		}
	}
};

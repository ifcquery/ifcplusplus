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
#include <ifcpp/IFC4X3/EntityFactory.h>
#include <ifcpp/IFC4X3/include/IfcCurtainWall.h>
#include <ifcpp/IFC4X3/include/IfcFeatureElementSubtraction.h>
#include <ifcpp/IFC4X3/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4X3/include/IfcOpeningElement.h>
#include <ifcpp/IFC4X3/include/IfcProject.h>
#include <ifcpp/IFC4X3/include/IfcPropertySetDefinitionSet.h>
#include <ifcpp/IFC4X3/include/IfcRelAggregates.h>
#include <ifcpp/IFC4X3/include/IfcSpace.h>
#include <ifcpp/IFC4X3/include/IfcWindow.h>

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/MeshOps.h>
#include <ifcpp/geometry/SceneGraphUtils.h>
#include "GeometryInputData.h"
#include "IncludeCarveHeaders.h"
#include "CSG_Adapter.h"

using namespace IFC4X3;
#define ProductNodeType osg::Group

class ConverterOSG : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings>						m_geom_settings;
	std::map<std::string, osg::ref_ptr<ProductNodeType> >	m_map_entity_guid_to_switch;
	std::map<int, osg::ref_ptr<ProductNodeType> >			m_map_representation_id_to_switch;
	std::vector<osg::ref_ptr<osg::Vec3Array> >			m_cache_vertices;
	bool												m_caching_enabled = false;
	double												m_recent_progress;
	osg::ref_ptr<osg::CullFace>							m_cull_back_off;
	double												m_faces_crease_angle = M_PI * 0.02;
	double												m_crease_edges_max_delta_angle = M_PI * 0.05;
	double												m_crease_edges_line_width = 1.5;
	float												m_epsilon = 1.5e-8f;
	bool												m_render_crease_edges = false;
	bool												m_draw_bounding_box = false;
	size_t m_numConvertedProducts = 0;
	size_t m_numProductsInModel = 0;
	size_t m_numBuffersReused = 0;

public:
	ConverterOSG(shared_ptr<GeometrySettings>& geom_settings) : m_geom_settings(geom_settings)
	{
		m_cull_back_off = new osg::CullFace(osg::CullFace::BACK);
	}
	virtual ~ConverterOSG() {}

	/**\brief Render crease edges */
	bool getRenderCreaseEdges() { return m_render_crease_edges; }
	void setRenderCreaseEdges(bool render_crease_edges) { m_render_crease_edges = render_crease_edges; }


	// Map: IfcProduct ID -> scenegraph switch
	std::map<std::string, osg::ref_ptr<ProductNodeType> >& getMapEntityGUIDToSwitch() { return m_map_entity_guid_to_switch; }
	// Map: Representation Identifier -> scenegraph switch
	std::map<int, osg::ref_ptr<ProductNodeType> >& getMapRepresentationToSwitch() { return m_map_representation_id_to_switch; }

	void clearInputCache()
	{
		m_map_entity_guid_to_switch.clear();
		m_map_representation_id_to_switch.clear();

		m_numConvertedProducts = 0;
		m_numProductsInModel = 0;
	}

	void findExistingVertexArray(const osg::ref_ptr<osg::Vec3Array>& vertices, osg::ref_ptr<osg::Vec3Array>& existingArray)
	{
		for (const osg::ref_ptr<osg::Vec3Array>& existingVertices : m_cache_vertices)
		{
			if (existingVertices->size() == vertices->size())
			{
				bool matchingArrayFound = true;
				auto itNewPoint = vertices->begin();
				for (auto itExistingPoint = existingVertices->begin(); itExistingPoint != existingVertices->end(); ++itExistingPoint, ++itNewPoint)
				{
					float dx = itNewPoint->x() - itExistingPoint->x();
					if (fabs(dx) > m_epsilon)
					{
						matchingArrayFound = false;
						break;
					}

					float dy = itNewPoint->y() - itExistingPoint->y();
					if (fabs(dy) > m_epsilon)
					{
						matchingArrayFound = false;
						break;
					}

					float dz = itNewPoint->z() - itExistingPoint->z();
					if (fabs(dz) > m_epsilon)
					{
						matchingArrayFound = false;
						break;
					}
				}

				if (matchingArrayFound)
				{
					++m_numBuffersReused;
					existingArray = existingVertices;
					return;
				}
			}
		}

		if (m_caching_enabled) {
			m_cache_vertices.push_back(vertices);
		}
		existingArray = vertices;
	}

	static void drawBoundingBox(const carve::geom::aabb<3>& aabb, osg::ref_ptr<osg::Geometry>& geom)
	{
		osg::ref_ptr<osg::Vec3Array> vertices = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		if (!vertices)
		{
			vertices = new osg::Vec3Array();
			geom->setVertexArray(vertices);
		}
		const carve::geom::vector<3>& aabb_pos = aabb.pos;
		const carve::geom::vector<3>& extent = aabb.extent;
		const double dex = extent.x;
		const double dey = extent.y;
		const double dez = extent.z;

		const int vert_id_offset = vertices->size();
		vertices->push_back(osg::Vec3f(aabb_pos.x - dex, aabb_pos.y - dey, aabb_pos.z - dez));
		vertices->push_back(osg::Vec3f(aabb_pos.x + dex, aabb_pos.y - dey, aabb_pos.z - dez));
		vertices->push_back(osg::Vec3f(aabb_pos.x + dex, aabb_pos.y + dey, aabb_pos.z - dez));
		vertices->push_back(osg::Vec3f(aabb_pos.x - dex, aabb_pos.y + dey, aabb_pos.z - dez));

		vertices->push_back(osg::Vec3f(aabb_pos.x - dex, aabb_pos.y - dey, aabb_pos.z + dez));
		vertices->push_back(osg::Vec3f(aabb_pos.x + dex, aabb_pos.y - dey, aabb_pos.z + dez));
		vertices->push_back(osg::Vec3f(aabb_pos.x + dex, aabb_pos.y + dey, aabb_pos.z + dez));
		vertices->push_back(osg::Vec3f(aabb_pos.x - dex, aabb_pos.y + dey, aabb_pos.z + dez));

		osg::ref_ptr<osg::DrawElementsUInt> box_lines = new osg::DrawElementsUInt(GL_LINE_STRIP, 0);
		box_lines->push_back(vert_id_offset + 0);
		box_lines->push_back(vert_id_offset + 1);
		box_lines->push_back(vert_id_offset + 2);
		box_lines->push_back(vert_id_offset + 3);
		box_lines->push_back(vert_id_offset + 0);
		box_lines->push_back(vert_id_offset + 4);
		box_lines->push_back(vert_id_offset + 5);
		box_lines->push_back(vert_id_offset + 1);
		box_lines->push_back(vert_id_offset + 5);
		box_lines->push_back(vert_id_offset + 6);
		box_lines->push_back(vert_id_offset + 2);
		box_lines->push_back(vert_id_offset + 6);
		box_lines->push_back(vert_id_offset + 7);
		box_lines->push_back(vert_id_offset + 3);
		box_lines->push_back(vert_id_offset + 7);
		box_lines->push_back(vert_id_offset + 4);
		geom->addPrimitiveSet(box_lines);

		osg::ref_ptr<osg::DrawElementsUInt> faces = new osg::DrawElementsUInt(GL_QUADS, 0);
		faces->push_back(vert_id_offset + 0);
		faces->push_back(vert_id_offset + 1);
		faces->push_back(vert_id_offset + 2);
		faces->push_back(vert_id_offset + 3);

		faces->push_back(vert_id_offset + 7);
		faces->push_back(vert_id_offset + 6);
		faces->push_back(vert_id_offset + 5);
		faces->push_back(vert_id_offset + 4);

		faces->push_back(vert_id_offset + 4);
		faces->push_back(vert_id_offset + 5);
		faces->push_back(vert_id_offset + 1);
		faces->push_back(vert_id_offset + 0);

		faces->push_back(vert_id_offset + 5);
		faces->push_back(vert_id_offset + 6);
		faces->push_back(vert_id_offset + 2);
		faces->push_back(vert_id_offset + 1);

		faces->push_back(vert_id_offset + 6);
		faces->push_back(vert_id_offset + 7);
		faces->push_back(vert_id_offset + 3);
		faces->push_back(vert_id_offset + 2);

		faces->push_back(vert_id_offset + 7);
		faces->push_back(vert_id_offset + 4);
		faces->push_back(vert_id_offset + 0);
		faces->push_back(vert_id_offset + 3);

		geom->addPrimitiveSet(faces);

		osg::ref_ptr<osg::Material> mat = new osg::Material();
		osg::Vec4f ambientColor(1.f, 0.2f, 0.1f, 1.f);
		mat->setAmbient(osg::Material::FRONT, ambientColor);
		mat->setDiffuse(osg::Material::FRONT, ambientColor);
		mat->setSpecular(osg::Material::FRONT, ambientColor);
		//mat->setShininess( osg::Material::FRONT, shininess );
		//mat->setColorMode( osg::Material::SPECULAR );

		osg::ref_ptr < osg::StateSet> stateset = geom->getOrCreateStateSet();
		stateset->setAttribute(mat, osg::StateAttribute::ON);
		//stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	void drawFace(const carve::mesh::Face<3>* face, osg::ref_ptr<osg::Geode>& geode, bool add_color_array = false)
	{
#ifdef _DEBUG
		std::cout << "not triangulated" << std::endl;
#endif
		std::vector<vec3> face_vertices;
		face_vertices.resize(face->nVertices());
		carve::mesh::Edge<3>* e = face->edge;
		const size_t num_vertices = face->nVertices();
		for (size_t i = 0; i < num_vertices; ++i)
		{
			face_vertices[i] = e->v1()->v;
			e = e->next;
		}

		if (num_vertices < 4)
		{
			std::cout << "drawFace is meant only for num vertices > 4" << std::endl;
		}

		vec3* vertex_vec;
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(num_vertices);
		osg::ref_ptr<osg::DrawElementsUInt> triangles = new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, num_vertices);

		for (size_t i = 0; i < num_vertices; ++i)
		{
			vertex_vec = &face_vertices[num_vertices - i - 1];
			(*vertices)[i].set(vertex_vec->x, vertex_vec->y, vertex_vec->z);
			(*triangles)[i] = i;
		}

		osg::Vec3f poly_normal = SceneGraphUtils::computePolygonNormal(vertices);
		osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();
		normals->resize(num_vertices, poly_normal);

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

		if (m_caching_enabled) {
			osg::ref_ptr<osg::Vec3Array> cachedVertexArray;
			findExistingVertexArray(vertices, cachedVertexArray);
			geometry->setVertexArray(cachedVertexArray);
		}
		else {
			geometry->setVertexArray(vertices);
		}
		geometry->setNormalArray(normals);
		normals->setBinding(osg::Array::BIND_PER_VERTEX);
		geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, vertices->size()));

		if (add_color_array)
		{
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->resize(vertices->size(), osg::Vec4f(0.6f, 0.6f, 0.6f, 0.1f));
			colors->setBinding(osg::Array::BIND_PER_VERTEX);
			geometry->setColorArray(colors);
		}

		if (num_vertices > 4)
		{
			// TODO: check if polygon is convex with Gift wrapping algorithm

			osg::ref_ptr<osgUtil::Tessellator> tesselator = new osgUtil::Tessellator();
			tesselator->setTessellationType(osgUtil::Tessellator::TESS_TYPE_POLYGONS);
			//tesselator->setWindingType( osgUtil::Tessellator::TESS_WINDING_ODD );
			tesselator->retessellatePolygons(*geometry);
		}
		geode->addDrawable(geometry);


#ifdef DEBUG_DRAW_NORMALS
		osg::ref_ptr<osg::Vec3Array> vertices_normals = new osg::Vec3Array();
		for (size_t i = 0; i < num_vertices; ++i)
		{
			vertex_vec = &face_vertices[num_vertices - i - 1];
			vertices_normals->push_back(osg::Vec3f(vertex_vec->x, vertex_vec->y, vertex_vec->z));
			vertices_normals->push_back(osg::Vec3f(vertex_vec->x, vertex_vec->y, vertex_vec->z) + poly_normal);
		}

		osg::ref_ptr<osg::Vec4Array> colors_normals = new osg::Vec4Array();
		colors_normals->resize(num_vertices * 2, osg::Vec4f(0.4f, 0.7f, 0.4f, 1.f));

		osg::ref_ptr<osg::Geometry> geometry_normals = new osg::Geometry();
		geometry_normals->setVertexArray(vertices_normals);
		geometry_normals->setColorArray(colors_normals);
		geometry_normals->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		geometry_normals->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		geometry_normals->setNormalBinding(osg::Geometry::BIND_OFF);
		geometry_normals->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices_normals->size()));
		geode->addDrawable(geometry_normals);
#endif
	}

	//#define DEBUG_DRAW_NORMALS

	void drawMeshSet(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, osg::ref_ptr<osg::Geode>& geode, double crease_angle, double min_triangle_area, bool add_color_array, bool disableBackFaceCulling)
	{
		if (!meshset)
		{
			return;
		}

		osg::ref_ptr<osg::Vec3Array> vertices_tri = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> normals_tri = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> vertices_quad;
		osg::ref_ptr<osg::Vec3Array> normals_quad;

		const size_t max_num_faces_per_vertex = 10000;
		std::map<carve::mesh::Face<3>*, double> map_face_area;
		std::map<carve::mesh::Face<3>*, double>::iterator it_face_area;

		if (crease_angle > 0)
		{
			for (size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh)
			{
				const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];
				const size_t num_faces = mesh->faces.size();
				for (size_t i_face = 0; i_face != num_faces; ++i_face)
				{
					carve::mesh::Face<3>* face = mesh->faces[i_face];
					double face_area = MeshOps::computeFaceArea(face);
					map_face_area[face] = std::abs(face_area);
				}
			}
		}

		for (size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh)
		{
			const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

			const size_t num_faces = mesh->faces.size();
			for (size_t i_face = 0; i_face != num_faces; ++i_face)
			{
				carve::mesh::Face<3>* face = mesh->faces[i_face];
				const size_t n_vertices = face->nVertices();
				if (n_vertices > 4)
				{
					drawFace(face, geode);
					continue;
				}
				const vec3 face_normal = face->plane.N;

				if (crease_angle > 0)
				{
					carve::mesh::Edge<3>* e = face->edge;
					for (size_t jj = 0; jj < n_vertices; ++jj)
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
						if (f1 != face)
						{
							std::cout << "f1 != face" << std::endl;
						}
#endif
						for (size_t i3 = 0; i3 < max_num_faces_per_vertex; ++i3)
						{
							if (!e1->rev)
							{
								break;
							}
							if (!e1->rev->next)
							{
								break;
							}

							vec3 f1_normal = f1->plane.N;
							const double cos_angle = dot(f1_normal, face_normal);
							if (cos_angle > 0)
							{
								const double deviation = std::abs(cos_angle - 1.0);
								if (deviation < crease_angle)
								{
									double weight = 0.0;
									it_face_area = map_face_area.find(f1);
									if (it_face_area != map_face_area.end())
									{
										weight = it_face_area->second;
									}
									intermediate_normal += weight * f1_normal;
								}
							}

							if (!e1->rev)
							{
								// it's an open mesh
								break;
							}

							e1 = e1->rev->next;
							if (!e1)
							{
								break;
							}
							f1 = e1->face;
#ifdef _DEBUG
							if (e1->vert != vertex)
							{
								std::cout << "e1->vert != vertex" << std::endl;
							}
#endif
							if (f1 == face)
							{
								break;
							}
						}
						const double intermediate_normal_length = intermediate_normal.length();
						if (intermediate_normal_length < 0.0000000001)
						{
							intermediate_normal = face_normal;
						}
						else
						{
							// normalize:
							intermediate_normal *= 1.0 / intermediate_normal_length;
						}

						const vec3& vertex_v = vertex->v;
						if (face->n_edges == 3)
						{
							const carve::mesh::Edge<3>* edge0 = face->edge;
							const carve::mesh::Edge<3>* edge1 = edge0->next;
							const carve::mesh::Edge<3>* edge2 = edge1->next;
							const carve::mesh::Vertex<3>* v0 = edge0->vert;
							const carve::mesh::Vertex<3>* v1 = edge1->vert;
							const carve::mesh::Vertex<3>* v2 = edge2->vert;

							vec3 vert0 = v0->v;
							vec3 vert1 = v1->v;
							vec3 vert2 = v2->v;

							vec3 v0v1 = vert1 - vert0;
							vec3 v0v2 = vert2 - vert0;
							double area = (carve::geom::cross(v0v1, v0v2).length()) * 0.5;
							if (std::abs(area) > min_triangle_area)   // skip degenerated triangle
							{

								vertices_tri->push_back(osg::Vec3f(vertex_v.x, vertex_v.y, vertex_v.z));
								normals_tri->push_back(osg::Vec3f(intermediate_normal.x, intermediate_normal.y, intermediate_normal.z));
							}
						}
						else if (face->n_edges == 4)
						{
							if (!vertices_quad) vertices_quad = new osg::Vec3Array();
							vertices_quad->push_back(osg::Vec3f(vertex_v.x, vertex_v.y, vertex_v.z));
							if (!normals_quad) normals_quad = new osg::Vec3Array();
							normals_quad->push_back(osg::Vec3f(intermediate_normal.x, intermediate_normal.y, intermediate_normal.z));
						}
						e = e->next;
					}
				}
				else
				{
					carve::mesh::Edge<3>* e = face->edge;
					for (size_t jj = 0; jj < n_vertices; ++jj)
					{
						carve::mesh::Vertex<3>* vertex = e->vert;
						const vec3& vertex_v = vertex->v;

						if (face->n_edges == 3)
						{
							const carve::mesh::Edge<3>* edge0 = face->edge;
							const carve::mesh::Edge<3>* edge1 = edge0->next;
							const carve::mesh::Edge<3>* edge2 = edge1->next;
							const carve::mesh::Vertex<3>* v0 = edge0->vert;
							const carve::mesh::Vertex<3>* v1 = edge1->vert;
							const carve::mesh::Vertex<3>* v2 = edge2->vert;

							vec3 vert0 = v0->v;
							vec3 vert1 = v1->v;
							vec3 vert2 = v2->v;

							vec3 v0v1 = vert1 - vert0;
							vec3 v0v2 = vert2 - vert0;
							double area = (carve::geom::cross(v0v1, v0v2).length()) * 0.5;
							if (std::abs(area) > min_triangle_area)   // skip degenerated triangle
							{
								vertices_tri->push_back(osg::Vec3f(vertex_v.x, vertex_v.y, vertex_v.z));
								normals_tri->push_back(osg::Vec3f(face_normal.x, face_normal.y, face_normal.z));
							}
						}
						else if (face->n_edges == 4)
						{
							if (!vertices_quad) vertices_quad = new osg::Vec3Array();
							vertices_quad->push_back(osg::Vec3f(vertex_v.x, vertex_v.y, vertex_v.z));
							if (!normals_quad) normals_quad = new osg::Vec3Array();
							normals_quad->push_back(osg::Vec3f(face_normal.x, face_normal.y, face_normal.z));
						}
						e = e->next;
					}
				}
			}
		}

		if (vertices_tri->size() > 0)
		{
			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			if (m_caching_enabled) {
				osg::ref_ptr<osg::Vec3Array> cachedVertexArray;
				findExistingVertexArray(vertices_tri, cachedVertexArray);
				geometry->setVertexArray(cachedVertexArray);
			}
			else {
				geometry->setVertexArray(vertices_tri);
			}

			geometry->setNormalArray(normals_tri);
			normals_tri->setBinding(osg::Array::BIND_PER_VERTEX);

			if (add_color_array)
			{
				osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
				colors->resize(vertices_tri->size(), osg::Vec4f(0.6f, 0.6f, 0.6f, 0.1f));
				colors->setBinding(osg::Array::BIND_PER_VERTEX);
				geometry->setColorArray(colors);
			}

			geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertices_tri->size()));
			geode->addDrawable(geometry);

			// disable back face culling for open meshes
			if (disableBackFaceCulling)
			{
				geometry->getOrCreateStateSet()->setAttributeAndModes(m_cull_back_off.get(), osg::StateAttribute::OFF);
			}

#ifdef DEBUG_DRAW_NORMALS
			osg::ref_ptr<osg::Vec3Array> vertices_normals = new osg::Vec3Array();
			for (size_t i = 0; i < vertices_tri->size(); ++i)
			{
				osg::Vec3f& vertex_vec = vertices_tri->at(i);// [i];
				osg::Vec3f& normal_vec = normals_tri->at(i);
				vertices_normals->push_back(osg::Vec3f(vertex_vec.x(), vertex_vec.y(), vertex_vec.z()));
				vertices_normals->push_back(osg::Vec3f(vertex_vec.x(), vertex_vec.y(), vertex_vec.z()) + normal_vec);
			}

			osg::ref_ptr<osg::Vec4Array> colors_normals = new osg::Vec4Array();
			colors_normals->resize(vertices_normals->size(), osg::Vec4f(0.4f, 0.7f, 0.4f, 1.f));

			osg::ref_ptr<osg::Geometry> geometry_normals = new osg::Geometry();
			geometry_normals->setVertexArray(vertices_normals);
			geometry_normals->setColorArray(colors_normals);
			geometry_normals->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
			geometry_normals->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

			geometry_normals->setNormalBinding(osg::Geometry::BIND_OFF);
			geometry_normals->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices_normals->size()));
			geode->addDrawable(geometry_normals);
#endif
		}

		if (vertices_quad)
		{
			if (vertices_quad->size() > 0)
			{
				osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
				if (m_caching_enabled) {
					osg::ref_ptr<osg::Vec3Array> cachedVertexArray;
					findExistingVertexArray(vertices_quad, cachedVertexArray);
					geometry->setVertexArray(cachedVertexArray);
				}
				else {
					geometry->setVertexArray(vertices_quad);
				}

				if (normals_quad)
				{
					normals_quad->setBinding(osg::Array::BIND_PER_VERTEX);
					geometry->setNormalArray(normals_quad);
				}

				if (add_color_array)
				{
					osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
					colors->resize(vertices_quad->size(), osg::Vec4f(0.6f, 0.6f, 0.6f, 0.1f));
					colors->setBinding(osg::Array::BIND_PER_VERTEX);
					geometry->setColorArray(colors);
				}

				geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices_quad->size()));
				geode->addDrawable(geometry);
			}
		}
	}

	void drawPolyline(const carve::input::PolylineSetData* polyline_data, osg::ref_ptr<osg::Geode>& geode, bool add_color_array = false)
	{
		if (!polyline_data)
		{
			return;
		}

		if (polyline_data->points.size() < 2)
		{
			return;
		}

		if (polyline_data->polylines.size() < 1)
		{
			return;
		}

		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
		shared_ptr<carve::line::PolylineSet> polyline_set( polyline_data->create(carve::input::opts()) );

		if (polyline_set->vertices.size() < 2)
		{
#ifdef _DEBUG
			std::cout << __FUNC__ << ": polyline_set->vertices.size() < 2" << std::endl;
#endif
			return;
		}

		for (auto it = polyline_set->lines.begin(); it != polyline_set->lines.end(); ++it)
		{
			const carve::line::Polyline* pline = *it;
			size_t vertex_count = pline->vertexCount();

			for (size_t vertex_i = 0; vertex_i < vertex_count; ++vertex_i)
			{
				if (vertex_i >= polyline_set->vertices.size())
				{
#ifdef _DEBUG
					std::cout << __FUNC__ << ": vertex_i >= polyline_set->vertices.size()" << std::endl;
#endif
					continue;
				}
				const carve::line::Vertex* v = pline->vertex(vertex_i);
				vertices->push_back(osg::Vec3f(v->v[0], v->v[1], v->v[2]));
			}
		}

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
		if (m_caching_enabled) {
			osg::ref_ptr<osg::Vec3Array> cachedVertexArray;
			findExistingVertexArray(vertices, cachedVertexArray);
			geometry->setVertexArray(cachedVertexArray);
		}
		else {
			geometry->setVertexArray(vertices);
		}
		geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices->size()));

		if (add_color_array)
		{
			osg::Vec4f color(0.6f, 0.6f, 0.6f, 0.1f);
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(vertices->size(), &color);
			colors->setBinding(osg::Array::BIND_PER_VERTEX);
			geometry->setColorArray(colors);
		}

		geode->addDrawable(geometry);
	}

	static void computeCreaseEdgesFromMeshset(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::vector<carve::mesh::Edge<3>* >& vec_edges_out, const double crease_angle)
	{
		if (!meshset)
		{
			return;
		}

		for (size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh)
		{
			const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];
			const std::vector<carve::mesh::Edge<3>* >& vec_closed_edges = mesh->closed_edges;

			for (size_t i_edge = 0; i_edge < vec_closed_edges.size(); ++i_edge)
			{
				carve::mesh::Edge<3>* edge = vec_closed_edges[i_edge];

				if (!edge)
				{
					continue;
				}
				carve::mesh::Edge<3>* edge_reverse = edge->rev;
				if (!edge_reverse)
				{
					continue;
				}
				carve::mesh::Face<3>* face = edge->face;
				carve::mesh::Face<3>* face_reverse = edge_reverse->face;

				const carve::geom::vector<3>& f1_normal = face->plane.N;
				const carve::geom::vector<3>& f2_normal = face_reverse->plane.N;
				const double cos_angle = dot(f1_normal, f2_normal);
				if (cos_angle > 0)
				{
					const double deviation = std::abs(cos_angle - 1.0);
					if (deviation < crease_angle)
					{
						continue;
					}
				}
				// TODO: if area of face and face_reverse is equal, skip the crease edge. It could be the inside or outside of a cylinder. Check also if > 2 faces in a row have same normal angle differences
				vec_edges_out.push_back(edge);
			}
		}
	}

	void renderMeshsetCreaseEdges(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, osg::ref_ptr<osg::Geode>& target_geode, const double crease_angle, const float line_width)
	{
		if (!meshset)
		{
			return;
		}
		if (!target_geode)
		{
			return;
		}
		std::vector<carve::mesh::Edge<3>* > vec_crease_edges;
		computeCreaseEdgesFromMeshset(meshset, vec_crease_edges, crease_angle);

		if (vec_crease_edges.size() > 0)
		{
			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
			for (size_t i_edge = 0; i_edge < vec_crease_edges.size(); ++i_edge)
			{
				const carve::mesh::Edge<3>* edge = vec_crease_edges[i_edge];
				const carve::geom::vector<3>& vertex1 = edge->v1()->v;
				const carve::geom::vector<3>& vertex2 = edge->v2()->v;
				vertices->push_back(osg::Vec3f(vertex1.x, vertex1.y, vertex1.z));
				vertices->push_back(osg::Vec3f(vertex2.x, vertex2.y, vertex2.z));
			}

			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			if (m_caching_enabled) {
				osg::ref_ptr<osg::Vec3Array> cachedVertexArray;
				findExistingVertexArray(vertices, cachedVertexArray);
				geometry->setVertexArray(cachedVertexArray);
			}
			else {
				geometry->setVertexArray(vertices);
			}
			geometry->setName("creaseEdges");
			geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices->size()));
			geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			geometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
			geometry->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(line_width), osg::StateAttribute::ON);
			osg::ref_ptr<osg::Material> mat = new osg::Material();
			mat->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.3f, 0.3f, 0.35f, 0.8f));
			geometry->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON);
			geometry->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
			geometry->getOrCreateStateSet()->setAttributeAndModes(new osg::Hint(GL_LINE_SMOOTH_HINT, GL_NICEST), osg::StateAttribute::ON);
			geometry->getOrCreateStateSet()->setRenderBinDetails(10, "RenderBin");
			target_geode->addDrawable(geometry);
		}
	}

	void applyStylesToGroup(const std::vector<shared_ptr<StyleData> >& vec_product_styles, osg::ref_ptr<osg::Group>& grp, float transparencyOverride)
	{
		for (size_t ii = 0; ii < vec_product_styles.size(); ++ii)
		{
			const shared_ptr<StyleData>& style = vec_product_styles[ii];
			if (!style)
			{
				continue;
			}

			if (style->m_apply_to_geometry_type == StyleData::GEOM_TYPE_SURFACE || style->m_apply_to_geometry_type == StyleData::GEOM_TYPE_ANY)
			{
				osg::ref_ptr<osg::StateSet> item_stateset = grp->getStateSet();
				bool stateSetAlreadySet = item_stateset.valid();
				convertToOSGStateSet(style, item_stateset, transparencyOverride);
				if (!stateSetAlreadySet)
				{
					grp->setStateSet(item_stateset);
				}
			}
			else if (style->m_apply_to_geometry_type == StyleData::GEOM_TYPE_CURVE)
			{

			}
		}
	}

	osg::Matrixd convertMatrixToOSG(const carve::math::Matrix& mat_in)
	{
		return osg::Matrixd(mat_in.m[0][0], mat_in.m[0][1], mat_in.m[0][2], mat_in.m[0][3],
			mat_in.m[1][0], mat_in.m[1][1], mat_in.m[1][2], mat_in.m[1][3],
			mat_in.m[2][0], mat_in.m[2][1], mat_in.m[2][2], mat_in.m[2][3],
			mat_in.m[3][0], mat_in.m[3][1], mat_in.m[3][2], mat_in.m[3][3]);
	}

	void convertMeshSets(std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vecMeshSets, osg::ref_ptr<osg::Geode>& geode, size_t ii_item, bool disableBackfaceCulling)
	{
		double min_triangle_area = m_geom_settings->getMinTriangleArea();
		double eps = m_geom_settings->getEpsilonMergePoints();
		double crease_angle = m_faces_crease_angle;
		for (size_t ii = 0; ii < vecMeshSets.size(); ++ii)
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = vecMeshSets[ii];

			double epsCoplanarFacesAngle = eps;
			double minFaceArea = eps;
			bool dumpMeshes = false;
			GeomProcessingParams params(m_geom_settings, dumpMeshes);
			PolyInputCache3D poly(0.001);
			MeshOps::retriangulateMeshSetForExport(item_meshset, poly, params);

			std::map<std::string, std::string> mesh_input_options;
			shared_ptr<carve::mesh::MeshSet<3> > meshsetTriangulated(poly.m_poly_data->createMesh(mesh_input_options, eps));

			drawMeshSet(meshsetTriangulated, geode, crease_angle, min_triangle_area, false, disableBackfaceCulling);

			if (m_render_crease_edges)
			{
				renderMeshsetCreaseEdges(meshsetTriangulated, geode, m_crease_edges_max_delta_angle, m_crease_edges_line_width);
			}

			if (m_draw_bounding_box)
			{
				carve::geom::aabb<3> bbox = meshsetTriangulated->getAABB();
				osg::ref_ptr<osg::Geometry> bbox_geom = new osg::Geometry();
				drawBoundingBox(bbox, bbox_geom);
				geode->addDrawable(bbox_geom);
			}

#ifdef _DEBUG
			//vec4 color(0.6f, 0.6f, 0.6f, 0.1f);
			//GeomDebugDump::moveOffset(1);
			//GeomDebugDump::dumpMeshset(meshsetTriangulated, color, false, true, true);
#endif
		}
	}

	void convertGeometricItem(const shared_ptr<ItemShapeData>& item_data, shared_ptr<IfcProduct>& ifc_product, size_t ii_representation, size_t ii_item, 
		osg::ref_ptr<osg::Group>& parentNode, float transparencyOverride)
	{
		bool includeChildProducts = false;
		bool includeGeometricChildItems = false;
		if (item_data->hasItemDataGeometricRepresentation(includeGeometricChildItems, true))
		{
			osg::ref_ptr<osg::Geode> item_geode = new osg::Geode();

			std::string product_guid;
			if (ifc_product->m_GlobalId)
			{
				product_guid = ifc_product->m_GlobalId->m_value;
			}

			std::string ifc_entity_type = EntityFactory::getStringForClassID(ifc_product->classID());
			std::stringstream strs_product_switch_name;
			strs_product_switch_name << product_guid << ":" << ifc_entity_type << " group";
			std::string product_switch_name = strs_product_switch_name.str();

#ifdef _DEBUG
			int tag = ifc_product->m_tag;
			if (product_guid.compare("3WMG3ehJnBiu4F_L5ltNmO") == 0 || ifc_product->classID() == IFC4X3::IFCWINDOW)
			{
				int wait = 0;
			}
#endif

			if (item_data->m_meshsets_open.size() > 0)
			{
				// disable back face culling for open meshes
				convertMeshSets(item_data->m_meshsets_open, item_geode, ii_item, true);
			}

			// create shape for closed meshes
			convertMeshSets(item_data->m_meshsets, item_geode, ii_item, false);

			// create shape for points
			const std::vector<shared_ptr<carve::input::VertexData> >& vertex_points = item_data->getVertexPoints();
			for (size_t ii = 0; ii < vertex_points.size(); ++ii)
			{
				const shared_ptr<carve::input::VertexData>& pointset_data = vertex_points[ii];
				if (pointset_data)
				{
					if (pointset_data->points.size() > 0)
					{
						osg::ref_ptr<osg::Geode> geode = new osg::Geode();

						osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
						for (size_t i_pointset_point = 0; i_pointset_point < pointset_data->points.size(); ++i_pointset_point)
						{
							vec3& carve_point = pointset_data->points[i_pointset_point];
							vertices->push_back(osg::Vec3f(carve_point.x, carve_point.y, carve_point.z));
						}

						osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
						geometry->setVertexArray(vertices);
						geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, vertices->size()));
						geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
						geode->getOrCreateStateSet()->setAttribute(new osg::Point(3.0f), osg::StateAttribute::ON);
						geode->addDrawable(geometry);
						geode->setCullingActive(false);
						item_geode->addChild(geode);

#ifdef _DEBUG
						std::stringstream strs_item_meshset_name;
						strs_item_meshset_name << " vertex_point " << ii;
						geode->setName(strs_item_meshset_name.str().c_str());
#endif
					}
				}
			}

			// create shape for polylines
			for (size_t ii = 0; ii < item_data->m_polylines.size(); ++ii)
			{
				shared_ptr<carve::input::PolylineSetData>& polyline_data = item_data->m_polylines[ii];
				osg::ref_ptr<osg::Geode> geode = new osg::Geode();
				geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
				drawPolyline(polyline_data.get(), geode);
				item_geode->addChild(geode);

#ifdef _DEBUG
				std::stringstream strs_item_meshset_name;
				strs_item_meshset_name << " polylines " << ii;
				geode->setName(strs_item_meshset_name.str().c_str());
#endif
			}

			if (m_geom_settings->isShowTextLiterals())
			{
				for (size_t ii = 0; ii < item_data->m_text_literals.size(); ++ii)
				{
					shared_ptr<TextItemData>& text_data = item_data->m_text_literals[ii];
					if (!text_data)
					{
						continue;
					}
					carve::math::Matrix& text_pos = text_data->m_text_position;
					// TODO: handle rotation

					std::string text_str;
					text_str.assign(text_data->m_text.begin(), text_data->m_text.end());

					osg::Vec3f pos2(text_pos._41, text_pos._42, text_pos._43);

					osg::ref_ptr<osgText::Text> txt = new osgText::Text();
					txt->setFont("fonts/arial.ttf");
					txt->setColor(osg::Vec4f(0, 0, 0, 1));
					txt->setCharacterSize(0.1f);
					txt->setAutoRotateToScreen(true);
					txt->setPosition(pos2);
					txt->setText(text_str.c_str());
					txt->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

					osg::ref_ptr<osg::Geode> geodeText = new osg::Geode();
					geodeText->addDrawable(txt);
					item_geode->addChild(geodeText);
				}
			}

			// If anything has been created, add it to the product group
			if (item_geode->getNumChildren() > 0 || item_geode->getNumDrawables() > 0)
			{
#ifdef _DEBUG
				if (item_geode->getNumParents() > 0)
				{
					std::cout << __FUNC__ << ": product_representation_switch->getNumParents() > 0" << std::endl;
				}
#endif

				// apply statesets if there are any
				if (item_data->getStyles().size() > 0)
				{
					osg::ref_ptr<osg::Group> geodeAsGroup = item_geode;
					applyStylesToGroup(item_data->getStyles(), geodeAsGroup, transparencyOverride);
				}

				if (transparencyOverride > 0)
				{
					bool hasTriangles = SceneGraphUtils::hasTrianglesWithMaterial(item_geode);
					bool createMaterialIfNotExisting = false;

					if (!hasTriangles)
					{
						createMaterialIfNotExisting = true;
					}
					SceneGraphUtils::setMaterialAlpha(item_geode, transparencyOverride, createMaterialIfNotExisting);
				}

				parentNode->addChild(item_geode);
			}
		}

		for (size_t i_item = 0; i_item < item_data->m_child_items.size(); ++i_item)
		{
			const shared_ptr<ItemShapeData>& child = item_data->m_child_items[i_item];
			convertGeometricItem(child, ifc_product, ii_representation, i_item, parentNode, transparencyOverride);
		}
	}

	//\brief method convertProductShapeToOSG: creates geometry objects from an IfcProduct object
	void convertProductShapeToOSG(const shared_ptr<ProductShapeData>& product_shape, osg::ref_ptr<osg::Group>& parentNode, std::stringstream& errorStream, float transparencyOverride)
	{
		std::string product_guid = "";
		try
		{
			// parentNode
			//	 |-----> product_switch
			//	             |-----> product_transform
			//				 |           |-----> geometric_items
			//	             |-----> child_products

			osg::ref_ptr<osg::MatrixTransform> product_transform = new osg::MatrixTransform();
			product_transform->setMatrix(convertMatrixToOSG(product_shape->getTransform()));

			osg::ref_ptr<ProductNodeType> product_switch = new ProductNodeType();
			product_switch->addChild(product_transform);
			parentNode->addChild(product_switch);

			product_guid = product_shape->m_entity_guid;
			auto it_find = m_map_entity_guid_to_switch.find(product_guid);
			if (it_find == m_map_entity_guid_to_switch.end())
			{
				m_map_entity_guid_to_switch[product_guid] = product_switch;
			}

			product_shape->m_added_to_spatial_structure = true;

			std::string entityType = "";

			// create OSG objects
			if (!product_shape->m_ifc_object_definition.expired())
			{
				shared_ptr<IfcObjectDefinition> ifc_object_def(product_shape->m_ifc_object_definition);
				entityType = EntityFactory::getStringForClassID(ifc_object_def->classID());

				shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(ifc_object_def);
				if (ifc_product)
				{
					// enable transparency for certain objects
					if (dynamic_pointer_cast<IfcSpace>(ifc_product))
					{
						transparencyOverride = 0.1f;
					}
					else if (dynamic_pointer_cast<IfcCurtainWall>(ifc_product) || dynamic_pointer_cast<IfcWindow>(ifc_product))
					{
						transparencyOverride = 0.2f;
					}

					// check if parent building element is window
					if (ifc_product->m_Decomposes_inverse.size() > 0)
					{
						for (size_t ii_decomposes = 0; ii_decomposes < ifc_product->m_Decomposes_inverse.size(); ++ii_decomposes)
						{
							const weak_ptr<IfcRelAggregates>& decomposes_weak = ifc_product->m_Decomposes_inverse[ii_decomposes];
							if (decomposes_weak.expired())
							{
								continue;
							}
							shared_ptr<IfcRelAggregates> decomposes_ptr(decomposes_weak);
							shared_ptr<IfcObjectDefinition>& relating_object = decomposes_ptr->m_RelatingObject;
							if (relating_object)
							{
								if (dynamic_pointer_cast<IfcCurtainWall>(relating_object) || dynamic_pointer_cast<IfcWindow>(relating_object))
								{
									transparencyOverride = 0.6f;
								}
							}
						}
					}

#ifdef _DEBUG
					if (product_guid.compare("0mmrfTIC96mu8Q5Pb3rKvn") == 0)
					{
						int wait = 0;
					}
#endif

					for (size_t ii_representation = 0; ii_representation < product_shape->getGeometricItems().size(); ++ii_representation)
					{
						const shared_ptr<ItemShapeData>& geom_item = product_shape->getGeometricItems()[ii_representation];
						osg::ref_ptr<osg::Group> grp = product_transform.get();
						convertGeometricItem(geom_item, ifc_product, ii_representation, 0, grp, transparencyOverride);
					}
				}
			}

			std::stringstream strsName;
			strsName << product_guid;
			product_switch->setName(strsName.str());
			float transparencyOverrideChildElements = 0.0;
			for (size_t i_item = 0; i_item < product_shape->getChildElements().size(); ++i_item)
			{
				const shared_ptr<ProductShapeData>& child = product_shape->getChildElements()[i_item];
				osg::ref_ptr<osg::Group> grp = product_switch.get();
				convertProductShapeToOSG(child, grp, errorStream, transparencyOverrideChildElements);
			}

			// TODO: if no color or material is given, set color 231/219/169 for walls, 140/140/140 for slabs 
			if (product_shape->getStyles().size() > 0)
			{
				osg::ref_ptr<osg::Group> transformAsGroup = product_transform;
				applyStylesToGroup(product_shape->getStyles(), transformAsGroup, transparencyOverrideChildElements);
			}

			++m_numConvertedProducts;
			// progress callback
			double progress = (double)m_numConvertedProducts / (double)m_numProductsInModel;
			if (progress - m_recent_progress > 0.02)
			{
				// leave 10% of progress to openscenegraph internals
				progressValueCallback(progress * 0.9, "scenegraph");
				m_recent_progress = progress;
			}
		}
		catch (BuildingException& e)
		{
			errorStream << e.what();
		}
		catch (carve::exception& e)
		{
			errorStream << e.str();
		}
		catch (std::exception& e)
		{
			errorStream << e.what();
		}
		catch (...)
		{
			errorStream << "undefined error, product id " << product_guid;
		}
	}


	/*\brief method convertToOSG: Creates geometry for OpenSceneGraph from given ProductShapeData.
	\param[out] parent_group Group to append the geometry.
	**/
	void convertToOSG(const std::unordered_map<std::string, shared_ptr<ProductShapeData> >& map_shape_data, osg::ref_ptr<ProductNodeType>& parent_group)
	{
		progressTextCallback("Converting geometry to OpenGL format ...");
		progressValueCallback(0, "scenegraph");

		clearInputCache();

		shared_ptr<ProductShapeData> ifc_project_data;
		for (auto it = map_shape_data.begin(); it != map_shape_data.end(); ++it)
		{
			shared_ptr<ProductShapeData> shape_data = it->second;
			if (!shape_data)
			{
				continue;
			}

			shape_data->m_added_to_spatial_structure = false;

			weak_ptr<IfcObjectDefinition>& ifc_object_def_weak = shape_data->m_ifc_object_definition;
			if (ifc_object_def_weak.expired())
			{
				continue;
			}

			shared_ptr<IfcObjectDefinition> ifc_object_def(shape_data->m_ifc_object_definition);

			shared_ptr<IfcProject> ifc_project = dynamic_pointer_cast<IfcProject>(ifc_object_def);
			if (ifc_project)
			{
				ifc_project_data = shape_data;
			}
		}
		m_numProductsInModel = map_shape_data.size();

		std::stringstream errorStream;
		if (ifc_project_data)
		{
			osg::ref_ptr<osg::Group> grp = parent_group.get();
			float transparencyOverride = -1.f;
			convertProductShapeToOSG(ifc_project_data, grp, errorStream, transparencyOverride);
		}

		osg::ref_ptr<ProductNodeType> sw_objects_outside_spatial_structure = new ProductNodeType();
		sw_objects_outside_spatial_structure->setName("IfcProduct objects outside spatial structure");

		// in case there are IFC entities that are not in the spatial structure
		for (auto it = map_shape_data.begin(); it != map_shape_data.end(); ++it)
		{
			shared_ptr<ProductShapeData> shape_data = it->second;
			if (!shape_data)
			{
				continue;
			}

			if (shape_data->m_added_to_spatial_structure)
			{
				continue;
			}

			weak_ptr<IfcObjectDefinition>& ifc_object_def_weak = shape_data->m_ifc_object_definition;
			if (ifc_object_def_weak.expired())
			{
				continue;
			}

			shared_ptr<IfcObjectDefinition> ifc_object_def(shape_data->m_ifc_object_definition);

			shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(ifc_object_def);
			if (!ifc_product)
			{
				continue;
			}

			if (dynamic_pointer_cast<IfcFeatureElementSubtraction>(ifc_product))
			{
				// geometry will be created in method subtractOpenings
				continue;
			}

			if (dynamic_pointer_cast<IfcOpeningElement>(ifc_product))
			{
				// geometry will be created in method subtractOpenings
				continue;
			}

			if (!ifc_product->m_Representation)
			{
				continue;
			}

			osg::ref_ptr<osg::Group> product_group = new osg::Group();
			float transparencyOverride = -1.f;
			convertProductShapeToOSG(shape_data, product_group, errorStream, transparencyOverride);

			if (product_group->getNumChildren() > 0)
			{
				sw_objects_outside_spatial_structure->addChild(product_group);
			}
		}

		if (sw_objects_outside_spatial_structure->getNumChildren() > 0)
		{
			parent_group->addChild(sw_objects_outside_spatial_structure);
		}

		std::cout << "num buffers: " << m_cache_vertices.size() << ", reused: " << m_numBuffersReused << std::endl;

		if (errorStream.tellp() > 0)
		{
			messageCallback(errorStream.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		}

		progressValueCallback(0.9, "scenegraph");
	}

	void convertToOSGStateSet(const shared_ptr<StyleData>& appearence, osg::ref_ptr<osg::StateSet>& target_stateset, float transparencyOverride)
	{
		if (!appearence)
		{
			return;
		}
		float shininess = appearence->m_shininess;
		float transparency = appearence->m_transparency;
		bool set_transparent = false;

		const float color_ambient_r = appearence->m_color_ambient.r;
		const float color_ambient_g = appearence->m_color_ambient.g;
		const float color_ambient_b = appearence->m_color_ambient.b;
		const float color_ambient_a = appearence->m_color_ambient.a;

		const float color_diffuse_r = appearence->m_color_diffuse.r;
		const float color_diffuse_g = appearence->m_color_diffuse.g;
		const float color_diffuse_b = appearence->m_color_diffuse.b;
		const float color_diffuse_a = appearence->m_color_diffuse.a;

		const float color_specular_r = appearence->m_color_specular.r;
		const float color_specular_g = appearence->m_color_specular.g;
		const float color_specular_b = appearence->m_color_specular.b;
		const float color_specular_a = appearence->m_color_specular.a;

		if (transparencyOverride > 0)
		{
			set_transparent = true;
			transparency = transparencyOverride;
		}

		float alpha = 1.f;
		if (transparency > 0.01)	// transparency: 0 = opaque, 1 = fully transparent
		{
			set_transparent = true;
			alpha = 1.0f - transparency;
		}

		osg::Vec4f ambientColor(color_ambient_r, color_ambient_g, color_ambient_b, alpha);
		osg::Vec4f diffuseColor(color_diffuse_r, color_diffuse_g, color_diffuse_b, alpha);
		osg::Vec4f specularColor(color_specular_r, color_specular_g, color_specular_b, alpha);

		// TODO: material caching and re-use
		osg::ref_ptr<osg::Material> mat = new osg::Material();
		mat->setAmbient(osg::Material::FRONT, ambientColor);
		mat->setDiffuse(osg::Material::FRONT, diffuseColor);
		mat->setSpecular(osg::Material::FRONT, specularColor);
		mat->setShininess(osg::Material::FRONT, shininess);
		mat->setColorMode(osg::Material::SPECULAR);

		if (!target_stateset.valid()) {
			target_stateset = new osg::StateSet();
		}
		target_stateset->setAttribute(mat, osg::StateAttribute::ON);

		if (set_transparent)
		{
			mat->setTransparency(osg::Material::FRONT, transparency);
			target_stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
			target_stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}

		if (appearence->m_specular_exponent != 0.f)
		{
			//osg::ref_ptr<osgFX::SpecularHighlights> spec_highlights = new osgFX::SpecularHighlights();
			//spec_highlights->setSpecularExponent( spec->m_value );
			// todo: add to scenegraph
		}
	}
};

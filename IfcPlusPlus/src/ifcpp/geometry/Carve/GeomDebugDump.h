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

#ifdef _DEBUG

#include <vector>
#include <fstream>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/BuildingGuid.h>
#include <ifcpp/writer/WriterStep.h>

#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcBuilding.h>
#include <ifcpp/IFC4/include/IfcBuildingElementProxy.h>
#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcDimensionCount.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationContext.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLocalPlacement.h>
#include <ifcpp/IFC4/include/IfcOrganization.h>
#include <ifcpp/IFC4/include/IfcOwnerHistory.h>
#include <ifcpp/IFC4/include/IfcPerson.h>
#include <ifcpp/IFC4/include/IfcPersonAndOrganization.h>
#include <ifcpp/IFC4/include/IfcProductDefinitionShape.h>
#include <ifcpp/IFC4/include/IfcReal.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcRelDefinesByProperties.h>
#include <ifcpp/IFC4/include/IfcRelVoidsElement.h>
#include <ifcpp/IFC4/include/IfcShapeRepresentation.h>
#include <ifcpp/IFC4/include/IfcSite.h>
#include <ifcpp/IFC4/include/IfcSIUnit.h>
#include <ifcpp/IFC4/include/IfcText.h>
#include <ifcpp/IFC4/include/IfcUnitAssignment.h>
#include <ifcpp/IFC4/include/IfcUnitEnum.h>
#include <ifcpp/IFC4/include/IfcWall.h>
#include "IncludeCarveHeaders.h"

inline void convertPlacement(double local_x[3], double local_z[3], double location[3], shared_ptr<IfcAxis2Placement3D>& axis2placement3d, std::vector<shared_ptr<BuildingEntity> >& vec_new_entities)
{
	if (!axis2placement3d)
	{
		axis2placement3d = shared_ptr<IfcAxis2Placement3D>(new IfcAxis2Placement3D());
		vec_new_entities.push_back(axis2placement3d);
	}

	if (!axis2placement3d->m_Location)
	{
		axis2placement3d->m_Location = shared_ptr<IfcCartesianPoint>(new IfcCartesianPoint());
		vec_new_entities.push_back(axis2placement3d->m_Location);
	}
	axis2placement3d->m_Location->m_Coordinates.clear();
	axis2placement3d->m_Location->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(location[0])));
	axis2placement3d->m_Location->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(location[1])));
	axis2placement3d->m_Location->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(location[2])));

	if (!axis2placement3d->m_Axis)
	{
		axis2placement3d->m_Axis = shared_ptr<IfcDirection>(new IfcDirection());
		vec_new_entities.push_back(axis2placement3d->m_Axis);
	}
	axis2placement3d->m_Axis->m_DirectionRatios.clear();
	axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[0])));
	axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[1])));
	axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[2])));

	if (!axis2placement3d->m_RefDirection)
	{
		axis2placement3d->m_RefDirection = shared_ptr<IfcDirection>(new IfcDirection());
		vec_new_entities.push_back(axis2placement3d->m_RefDirection);
	}

	axis2placement3d->m_RefDirection->m_DirectionRatios.clear();
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[0])));
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[1])));
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[2])));
}

static double dump_y_pos_geom = 0;
namespace GeomDebugDump
{
	inline void clearMeshsetDump()
	{
		std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::trunc );
		dump_ofstream.close();
		dump_y_pos_geom = 0;
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
			strs_out << "{" << vertex.v.x + offset.x << ", " << vertex.v.y + offset.y << ", " << vertex.v.z + offset.z << "}" << std::endl;
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
	
	inline int findVertexIndex(const std::vector<carve::mesh::Vertex<3> >& vec_vertices, const carve::mesh::Vertex<3>* v)
	{
		for( size_t iiv = 0; iiv < vec_vertices.size(); ++iiv )
		{
			const carve::mesh::Vertex<3>& vertex = vec_vertices[iiv];
			if( v == &vertex )
			{
				return iiv;
			}
		}
		return -1;
	}
	
	inline void MeshSet2Stream(const carve::mesh::MeshSet<3>* meshset, const vec3& offset, const carve::geom::vector<4>& color, std::stringstream& strs_out)
	{
		strs_out << "Polyhedron{" << std::endl;
		strs_out << "color{" << color.x << ", " << color.y << ", " << color.z << ", " << color.w << "}" << std::endl;

		std::stringstream strs_vertices;
		std::stringstream str_faces;
		double scale_length_factor = 1.0;
		if (meshset)
		{
			// vertices of the meshset:
			size_t vertex_count = 0;
			size_t face_count = 0;
			const std::vector<carve::mesh::Vertex<3> >& vec_vertices = meshset->vertex_storage;

			for (size_t iiv = 0; iiv < vec_vertices.size(); ++iiv)
			{
				const carve::mesh::Vertex<3>& vertex = vec_vertices[iiv];
				if (vertex_count > 0)
				{
					strs_vertices << ",   ";
				}
				double x = (vertex.v.x + offset.x)*scale_length_factor;
				double y = (vertex.v.y + offset.y)*scale_length_factor;
				double z = (vertex.v.z + offset.z)*scale_length_factor;

				strs_vertices << "{" << x << "," << y << "," << z << "}" << std::endl;
				++vertex_count;
			}

			const std::vector<carve::mesh::Mesh<3>* >& vec_meshes = meshset->meshes;
			for (size_t i_mesh = 0; i_mesh < vec_meshes.size(); ++i_mesh)
			{
				const carve::mesh::Mesh<3>* mesh = vec_meshes[i_mesh];

				if (mesh)
				{
					// faces:
					const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
					for (size_t i_face = 0; i_face < vec_faces.size(); ++i_face)
					{
						const carve::mesh::Face<3>* face = vec_faces[i_face];
						if (face)
						{
							int number_of_edges = face->n_edges;

							if (number_of_edges == 3)
							{
								if (face_count > 0)
								{
									str_faces << ",   ";
								}
								carve::mesh::Edge<3>* edge0 = face->edge;
								carve::mesh::Edge<3>* edge1 = edge0->next;
								carve::mesh::Edge<3>* edge2 = edge1->next;
								carve::mesh::Vertex<3>* v0 = edge0->vert;
								carve::mesh::Vertex<3>* v1 = edge1->vert;
								carve::mesh::Vertex<3>* v2 = edge2->vert;
								int v0index = findVertexIndex(vec_vertices, v0);
								int v1index = findVertexIndex(vec_vertices, v1);
								int v2index = findVertexIndex(vec_vertices, v2);

								if (v0index < 0 || v1index < 0 || v2index < 0)
								{
									std::cout << "vertex not found " << std::endl;
									continue;
								}
								str_faces << "{" << v0index << "," << v1index << "," << v2index << "}" << std::endl;
								++face_count;
							}
							else
							{
								std::cout << "exporting only triangles " << std::endl;
							}
						}
					}
				}
			}
		}

		strs_out << "vertices{" << strs_vertices.str().c_str() << "}" << std::endl;
		strs_out << "faces{" << str_faces.str().c_str() << "}" << std::endl;
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
			//shared_ptr<carve::poly::Polyhedron> poly( carve::polyhedronFromMesh( meshset, -1 ) );
			//Polyhedron2Stream( poly.get(), offset, color, strs_out );
			MeshSet2Stream(meshset, offset, color, strs_out);
		}

		if( !append )
		{
			clearMeshsetDump();
		}
		std::ofstream dump_ofstream( "dump_mesh_debug.txt", std::ofstream::app );
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
	inline void dumpMeshSet(const carve::mesh::MeshSet<3>* poly, const vec3& offset, const carve::geom::vector<4>& color, bool append)
	{
		std::stringstream strs_out;
		MeshSet2Stream(poly, offset, color, strs_out);

		if( !append )
		{
			clearMeshsetDump();
		}

		std::ofstream dump_ofstream("dump_mesh_debug.txt", std::ofstream::app);
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

		
		//shared_ptr<carve::poly::Polyhedron> poly( carve::polyhedronFromMesh( meshset.get(), -1 ) );
		dumpMeshSet( meshset.get(), offset, color, append );

		if( move_offset )
		{
			dump_y_pos_geom += meshset->getAABB().extent.y*2.2;
		}
	}

	inline void moveOffset(const shared_ptr<carve::mesh::MeshSet<3> >& meshset)
	{
		if (meshset->meshes.size() == 0)
		{
			return;
		}
		const auto aabb = meshset->getAABB();
		double dy = aabb.extent.y*2.0;
#ifdef _DEBUG
		double dx = aabb.extent.x*2.0;
		double dz = aabb.extent.z*2.0;
		dx = dx;
		dz = dz;
#endif
		dump_y_pos_geom += dy*1.1;
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

	inline void dumpEntity(const shared_ptr<BuildingEntity>& entity)
	{
		std::vector<shared_ptr<BuildingEntity> > vec_new_entities;
		shared_ptr<BuildingModel> ifc_model(new BuildingModel());
		double unit_length_factor = 1.0;
		ifc_model->clearIfcModel();
		ifc_model->getUnitConverter()->setLengthInMeterFactor(unit_length_factor);

		std::map<BuildingEntity*, shared_ptr<BuildingEntity> > map_entities;
		ifc_model->collectDependentEntities(entity, map_entities);

		for (auto it : map_entities)
		{
			vec_new_entities.push_back(it.second);
		}

		shared_ptr<IfcProduct> entity_product = dynamic_pointer_cast<IfcProduct>(entity);
		if (!entity_product)
		{
			shared_ptr<IfcGeometricRepresentationItem> geom_item = dynamic_pointer_cast<IfcGeometricRepresentationItem>(entity);
			if (geom_item)
			{
				entity_product = shared_ptr<IfcBuildingElementProxy>(new IfcBuildingElementProxy());
				vec_new_entities.push_back(entity_product);
				entity_product->m_Name = shared_ptr<IfcLabel>(new IfcLabel());
				entity_product->m_Name->m_value = L"proxy";
				
				// geometry
				shared_ptr<IfcShapeRepresentation> shape_representation(new IfcShapeRepresentation());
				vec_new_entities.push_back(shape_representation);
				shape_representation->m_RepresentationIdentifier = shared_ptr<IfcLabel>(new IfcLabel(L"Body"));
				shape_representation->m_RepresentationType = shared_ptr<IfcLabel>(new IfcLabel(L"SweptSolid"));

				shared_ptr<IfcProductDefinitionShape> product_def_shape(new IfcProductDefinitionShape());
				vec_new_entities.push_back(product_def_shape);

				entity_product->m_Representation = product_def_shape;
				product_def_shape->m_Representations.push_back(shape_representation);

				shape_representation->m_Items.push_back(geom_item);
			}
			else
			{
				return;
			}
		}

		// building structure
		shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
		if (!ifc_project)
		{
			ifc_project = shared_ptr<IfcProject>(new IfcProject());
			vec_new_entities.push_back(ifc_project);
			ifc_model->setIfcProject(ifc_project); 
		}

		shared_ptr<IfcSite> ifc_site(new IfcSite());
		vec_new_entities.push_back(ifc_site);

		shared_ptr<IfcBuilding> ifc_building(new IfcBuilding());
		vec_new_entities.push_back(ifc_building);

		shared_ptr<IfcBuildingStorey> ifc_building_storey(new IfcBuildingStorey());
		vec_new_entities.push_back(ifc_building_storey);

		// relations object to connect site to project
		shared_ptr<IfcRelAggregates> rel_aggregates_project_site(new IfcRelAggregates());
		rel_aggregates_project_site->m_RelatingObject = ifc_project;
		rel_aggregates_project_site->m_RelatedObjects.push_back(ifc_site);
		vec_new_entities.push_back(rel_aggregates_project_site);

		// relations object to connect building to site
		shared_ptr<IfcRelAggregates> rel_aggregates_site_building(new IfcRelAggregates());
		rel_aggregates_site_building->m_RelatingObject = ifc_site;
		rel_aggregates_site_building->m_RelatedObjects.push_back(ifc_building);
		vec_new_entities.push_back(rel_aggregates_site_building);

		// relations object to connect building storey to building
		shared_ptr<IfcRelAggregates> rel_aggregates_building_buildingstorey(new IfcRelAggregates());
		rel_aggregates_building_buildingstorey->m_RelatingObject = ifc_building;
		rel_aggregates_building_buildingstorey->m_RelatedObjects.push_back(ifc_building_storey);
		vec_new_entities.push_back(rel_aggregates_building_buildingstorey);

		// relations object to connect wall to building storey
		shared_ptr<IfcRelContainedInSpatialStructure> rel_contained_buildingstorey_wall(new IfcRelContainedInSpatialStructure());
		rel_contained_buildingstorey_wall->m_RelatingStructure = ifc_building_storey;


		rel_contained_buildingstorey_wall->m_RelatedElements.push_back(entity_product);
		vec_new_entities.push_back(rel_contained_buildingstorey_wall);


		// coordinate systems
		double local_x[3] = { 1,0,0 };
		double local_z[3] = { 0,0,1 };
		double location[3] = { 0,0,0 };

		// global placement
		shared_ptr<IfcAxis2Placement3D> axis_placement_origin;
		convertPlacement(local_x, local_z, location, axis_placement_origin, vec_new_entities);
		shared_ptr<IfcLocalPlacement> global_placement(new IfcLocalPlacement());
		vec_new_entities.push_back(global_placement);
		global_placement->m_RelativePlacement = axis_placement_origin;

		// local placement of building
		shared_ptr<IfcAxis2Placement3D> axis_placement_building;
		convertPlacement(local_x, local_z, location, axis_placement_building, vec_new_entities);
		shared_ptr<IfcLocalPlacement> building_placement(new IfcLocalPlacement());
		vec_new_entities.push_back(building_placement);
		building_placement->m_PlacementRelTo = global_placement;
		building_placement->m_RelativePlacement = axis_placement_building;
		ifc_building->m_ObjectPlacement = building_placement;

		// local placement of entity
		shared_ptr<IfcAxis2Placement3D> axis_placement_local;
		convertPlacement(local_x, local_z, location, axis_placement_local, vec_new_entities);
		shared_ptr<IfcLocalPlacement> wall_placement(new IfcLocalPlacement());
		vec_new_entities.push_back(wall_placement);
		wall_placement->m_PlacementRelTo = building_placement;
		wall_placement->m_RelativePlacement = axis_placement_local;
		entity_product->m_ObjectPlacement = wall_placement;


		// general objects
		shared_ptr<IfcPerson> person(new IfcPerson());
		person->m_Identification = shared_ptr<IfcIdentifier>(new IfcIdentifier(L"MyID"));
		person->m_FamilyName = shared_ptr<IfcLabel>(new IfcLabel(L"MyFamilyName"));
		person->m_GivenName = shared_ptr<IfcLabel>(new IfcLabel(L"MyGivenName"));
		vec_new_entities.push_back(person);

		shared_ptr<IfcOrganization> orga(new IfcOrganization());
		orga->m_Identification = shared_ptr<IfcIdentifier>(new IfcIdentifier(L"MyOrganization"));
		orga->m_Name = shared_ptr<IfcLabel>(new IfcLabel(L"My organizations name"));
		orga->m_Description = shared_ptr<IfcText>(new IfcText(L"My organizations description"));
		vec_new_entities.push_back(orga);

		shared_ptr<IfcPersonAndOrganization> person_and_organization(new IfcPersonAndOrganization());
		person_and_organization->m_ThePerson = person;
		person_and_organization->m_TheOrganization = orga;
		vec_new_entities.push_back(person_and_organization);

		shared_ptr<IfcOwnerHistory> owner_history(new IfcOwnerHistory());
		owner_history->m_OwningUser = person_and_organization;
		vec_new_entities.push_back(owner_history);

		// define units of the model
		shared_ptr<IfcSIUnit> unit1(new IfcSIUnit());
		unit1->m_UnitType = shared_ptr<IfcUnitEnum>(new IfcUnitEnum(IfcUnitEnum::ENUM_LENGTHUNIT));
		unit1->m_Name = shared_ptr<IfcSIUnitName>(new IfcSIUnitName(IfcSIUnitName::ENUM_METRE));
		vec_new_entities.push_back(unit1);

		shared_ptr<IfcUnitAssignment> unit_assignment(new IfcUnitAssignment());
		unit_assignment->m_Units.push_back(unit1);
		vec_new_entities.push_back(unit_assignment);

		// define representation context
		shared_ptr<IfcGeometricRepresentationContext> geom_context(new IfcGeometricRepresentationContext());
		geom_context->m_ContextType = shared_ptr<IfcLabel>(new IfcLabel(L"Model"));
		geom_context->m_CoordinateSpaceDimension = shared_ptr<IfcDimensionCount>(new IfcDimensionCount(3));
		geom_context->m_Precision = shared_ptr<IfcReal>(new IfcReal(1.000E-5));
		geom_context->m_WorldCoordinateSystem = axis_placement_origin;
		vec_new_entities.push_back(geom_context);

		ifc_project->m_UnitsInContext = unit_assignment;
		ifc_project->m_RepresentationContexts.push_back(geom_context);

		std::map<BuildingEntity*, shared_ptr<BuildingEntity> > map_new_entities;
		shared_ptr<BuildingEntity> projectAsEntity = ifc_project;

		// insert entities into IFC model
		for (auto entity : vec_new_entities)
		{
			shared_ptr<IfcRoot> ifc_root_object = dynamic_pointer_cast<IfcRoot>(entity);
			if (ifc_root_object)
			{
				// each object that is derived from IfcRoot should have a GUID
				if (!ifc_root_object->m_GlobalId)
				{
					ifc_root_object->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId(createBase64Uuid_wstr().data()));
				}

				ifc_root_object->m_OwnerHistory = owner_history;
			}

			shared_ptr<IfcRepresentation> ifc_representation = dynamic_pointer_cast<IfcRepresentation>(entity);
			if (ifc_representation)
			{
				ifc_representation->m_ContextOfItems = geom_context;
			}

			ifc_model->insertEntity(entity);
		}

		ifc_model->unsetInverseAttributes();
		ifc_model->resolveInverseAttributes();
		ifc_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"IFC4";


		// write IFC file in STEP format
		std::wstring file_path = L"dumpEntity.ifc";
		ifc_model->initFileHeader(file_path);
		std::stringstream stream;

		shared_ptr<WriterSTEP> step_writer(new WriterSTEP());
		step_writer->writeModelToStream(stream, ifc_model);
		ifc_model->clearIfcModel();

		std::ofstream ofs(file_path, std::ofstream::out);
		ofs << stream.str().c_str();
		ofs.close();


	}
}
#endif

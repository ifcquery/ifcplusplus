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

#include "IncludeCarveHeaders.h"
#include <ifcpp/model/StatusCallback.h>
#include <earcut/include/mapbox/earcut.hpp>

struct MeshSetInfo
{
	MeshSetInfo()
	{
	}
	MeshSetInfo( MeshSetInfo& other )
	{
		numClosedEdges = other.numClosedEdges;
		numOpenEdges = other.numOpenEdges;
		numZeroAreaFaces = other.numZeroAreaFaces;
		numFaces = other.numFaces;
		surfaceArea = other.surfaceArea;
		allPointersValid = other.allPointersValid;
		meshSetValid = other.meshSetValid;
	}

	size_t numClosedEdges = 0;
	size_t numOpenEdges = 0;
	size_t numZeroAreaFaces = 0;
	size_t numFaces = 0;
	double surfaceArea = 0;
	bool allPointersValid = true;
	bool meshSetValid = false;

	void resetInfo()
	{
		allPointersValid = true;
		numClosedEdges = 0;
		numOpenEdges = 0;
		numZeroAreaFaces = 0;
		numFaces = 0;
		surfaceArea = 0;
		meshSetValid = false;
	}
};

namespace MeshUtils
{
	inline size_t getNumFaces( const carve::mesh::MeshSet<3>* meshset )
	{
		size_t num_faces = 0;
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			num_faces += meshset->meshes[i]->faces.size();
		}
		return num_faces;
	}
	inline size_t getNumClosedEdges( const carve::mesh::MeshSet<3>* meshset )
	{
		size_t num_edges = 0;
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			num_edges += meshset->meshes[i]->closed_edges.size();
		}
		return num_edges;
	}
	inline double getVolume( const carve::mesh::MeshSet<3>* meshset )
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

	inline void roundVertices( carve::mesh::MeshSet<3>* meshset )
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

	inline void recalcMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset)
	{
		for( auto mesh : meshset->meshes )
		{
			mesh->cacheEdges();
			mesh->recalc();
		}
	}

	inline bool checkMeshSetVolume( const shared_ptr<carve::mesh::MeshSet<3> >& mesh_set, StatusCallback* report_callback, BuildingEntity* entity )
	{
		if( !mesh_set )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_tag << " not valid" << std::endl;
			}
#endif
			return false;
		}
		if( mesh_set->meshes.size() == 0 )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_tag << " has no meshes" << std::endl;
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
			if( report_callback )
			{
				report_callback->messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity );
			}
#endif
			return false;
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static double computeFaceArea(const carve::mesh::Face<3>* face)
	{
		double face_area = 0;
		if( face->nVertices() == 3 )
		{
			carve::mesh::Edge<3>* edge = face->edge;
			if( edge )
			{
				carve::geom::vector<3>& v1 = edge->v1()->v;
				carve::geom::vector<3>& v2 = edge->v2()->v;
				if( edge->next )
				{
					carve::geom::vector<3>& v3 = edge->next->v2()->v;
					carve::geom::vector<3> side1 = v2 - v1;
					carve::geom::vector<3> side2 = v3 - v2;
					carve::geom::vector<3> c = cross(side1, side2);
					double len_square = c.length2();
					if( len_square > EPS_M14*0.001 )
					{
						double area = sqrt(len_square) * 0.5;
						face_area += abs(area);
					}
				}
			}
		}
		else
		{
			std::vector<carve::mesh::Face<3>::vertex_t* > faceVertices;
			face->getVertices(faceVertices);
			if( faceVertices.size() > 2 )
			{
				size_t n = faceVertices.size();

				carve::geom::vector<3> normal;
				carve::geom::vector<3> a;
				carve::geom::vector<3> b = faceVertices[n - 2]->v;
				carve::geom::vector<3> c = faceVertices[n - 1]->v;
				carve::geom::vector<3> s;

				for( int i = 0; i < n; ++i )
				{
					a = b;
					b = c;
					c = faceVertices[i]->v;

					normal.x += b.y * (c.z - a.z);
					normal.y += b.z * (c.x - a.x);
					normal.z += b.x * (c.y - a.y);

					s += c;
				}

				double length = normal.length();// glm::length(normal);
				if( std::abs(length) < EPS_M8 )
				{
					return false;
				}

				normal /= length;
				double area = 0.5 * length;
				face_area += area;
			}
		}
		return face_area;
	}

	static double computeMeshSetSurface(const shared_ptr<carve::mesh::MeshSet<3> >& meshset)
	{
		double surface_area = 0;
		const std::vector<carve::mesh::Mesh<3>* >& vec_meshes = meshset->meshes;
		for( size_t kk = 0; kk < vec_meshes.size(); ++kk )
		{
			const carve::mesh::Mesh<3>* mesh = vec_meshes[kk];
			//volume += mesh->volume();

			const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
			for( size_t mm = 0; mm < vec_faces.size(); ++mm )
			{
				const carve::mesh::Face<3>* face = vec_faces[mm];
				surface_area += computeFaceArea(face);
			}
		}
		return surface_area;
	}

	static double computeShapeSurfaceArea(const shared_ptr<ProductShapeData>& shape_input_data)
	{
		double surface_area = 0;
		double volume = 0;
		if( shape_input_data )
		{
			std::vector<shared_ptr<RepresentationData> >& vec_representations = shape_input_data->m_vec_representations;
			for( auto& representation : vec_representations )
			{
				std::vector<shared_ptr<ItemShapeData> >& vec_items = representation->m_vec_item_data;
				for( size_t ii = 0; ii < vec_items.size(); ++ii )
				{
					shared_ptr<ItemShapeData>& item_data = vec_items[ii];
					std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vec_meshsets = item_data->m_meshsets;
					for( size_t jj = 0; jj < vec_meshsets.size(); ++jj )
					{
						surface_area += computeMeshSetSurface(vec_meshsets[jj]);
					}
				}
			}
		}
		return surface_area;
	}

	static bool checkMeshsetTriangulated(const shared_ptr<carve::mesh::MeshSet<3>>& meshset)
	{
		for( size_t ii = 0; ii < meshset->meshes.size(); ++ii )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
			std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
			for( size_t i2 = 0; i2 < vec_faces.size(); ++i2 )
			{
				carve::mesh::Face<3>* face = vec_faces[i2];
				if( face->n_edges != 3 )
				{
					return false;
				}
			}
		}
		return true;
	}

	inline bool checkEdgePointers(carve::mesh::Edge<3>* e )
	{
		if( !e )
		{
			return false;
		}
		
		if( !e->rev )
		{
			return false;
		}

		if( e->rev->rev != e )
		{
			return false;
		}

		if( !e->prev )
		{
			return false;
		}
		if( !e->next )
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
		return true;
	}

	inline bool checkMeshFins(const shared_ptr<carve::mesh::MeshSet<3> >& meshset)
	{
		if( !meshset )
		{
			return false;
		}

		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[i];

			if( !mesh )
			{
				return false;
			}
			const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
			for( size_t j = 0; j < vec_faces.size(); ++j )
			{
				carve::mesh::Face<3>* face = vec_faces[j];

				const size_t n_edges = face->n_edges;
				if( n_edges > 10000 )
				{
#ifdef _DEBUG
					std::cout << "n_edges > 10000" << std::endl;
					glm::vec4 color(0.3, 0.3, 0.3, 1.);
					GeomDebugDump::dumpFacePolygon(face, color, false);
#endif
					return false;
				}

				carve::mesh::Edge<3>* e = face->edge;
				for( size_t i_edge = 0; i_edge < n_edges; ++i_edge )
				{
					// check if is a flat part
					if( e->face )
					{
						const carve::geom::vector<3>& normal = e->face->plane.N;
						if( e->face->edge )
						{
							if( e->face->edge->rev )
							{
								if( e->face->edge->rev->face )
								{
									const carve::geom::vector<3>& reverserFaceNormal = e->face->edge->rev->face->plane.N;

									double dotProduct = dot(reverserFaceNormal, normal);
									if( std::abs(dotProduct + 1.0) < EPS_DEFAULT * 10 )
									{
										double face_area = MeshUtils::computeFaceArea(face);
										if( std::abs(face_area) > carve::CARVE_EPSILON * 10 )
										{
#ifdef _DEBUG
											std::cout << "opposite face is coplanar" << std::endl;
#endif
											return false;
										}
									}
								}
							}
						}
					}

					// continue
					e = e->next;
				}
			}
		}
		return true;
	}

	inline bool checkFaceIntegrity(const carve::mesh::Face<3>* face, MeshSetInfo& info)
	{
		if( !face )
		{
			info.allPointersValid = false;
			return false;
		}
		carve::mesh::Edge<3>* e = face->edge;
		if( !e )
		{
			info.allPointersValid = false;
			return false;
		}

		const size_t n_edges = face->n_edges;
		if( n_edges > 10000 )
		{
#ifdef _DEBUG
			std::cout << "n_edges > 10000" << std::endl;
			glm::vec4 color(0.3, 0.3, 0.3, 1.);
			GeomDebugDump::dumpFacePolygon(face, color, false);
#endif
			return false;
		}
		for( size_t i_edge = 0; i_edge < n_edges; ++i_edge )
		{
			if( !checkEdgePointers(e) )
			{
				return false;
			}
	
			if( !checkEdgePointers(e->rev) )
			{
				return false;
			}
	
			// continue
			e = e->next;
		}
		if( e != face->edge )
		{
			std::cout << "face edge loop invalid" << std::endl;
			return false;
		}

		try
		{
			e->validateLoop();
		}
		catch( std::exception& ex )
		{
#ifdef _DEBUG
			std::cout << ex.what() << std::endl;
#endif
			return false;
		}
		catch( std::exception* ex )
		{
#ifdef _DEBUG
			std::cout << ex->what() << std::endl;
#endif
			return false;
		}
		catch( carve::exception& ex )
		{
#ifdef _DEBUG
			std::cout << ex.str() << std::endl;
#endif
			return false;
		}
		catch( ... )
		{
			return false;
		}

		return true;
	}

	inline bool hasPointer(const carve::mesh::Face<3>* face, const carve::mesh::Edge<3>* edgePtr)
	{
		carve::mesh::Edge<3>* e = face->edge;
		if( e == nullptr )
		{
			return false;
		}

		const size_t n_edges = face->n_edges;
		for( size_t i_edge = 0; i_edge < n_edges; ++i_edge )
		{
			if( e == edgePtr ) return true;
			if( e->rev == edgePtr ) return true;
			if( e->prev == edgePtr ) return true;
			if( e->next == edgePtr ) return true;
			if( e->rev->prev == edgePtr ) return true;
			if( e->rev->next == edgePtr ) return true;

			e = e->next;
		}
		
		return false;
	}

	inline bool checkMeshPointers(const carve::mesh::Mesh<3>* mesh, MeshSetInfo& info)
	{
		if( !mesh )
		{
			return false;
		}
		const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
		for( size_t j = 0; j < vec_faces.size(); ++j )
		{
			carve::mesh::Face<3>* face = vec_faces[j];
			bool face_valid = checkFaceIntegrity(face, info);

			if( !face_valid )
			{
				return false;
			}
		}

		return true;
	}

	inline bool checkMeshSetPointers(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, MeshSetInfo& info)
	{
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			carve::mesh::Mesh<3>* mesh_i = meshset->meshes[i];
			bool mesh_correct = checkMeshPointers(mesh_i, info);
			if( !mesh_correct )
			{
				info.allPointersValid = false;
				return false;
			}
		}
		return true;
	}

	inline double getFaceArea2D(const carve::mesh::Face<3>* face )
	{
		std::vector<carve::geom::vector<2> > facePoints;
		std::vector<std::array<double, 2> > facePointsArray2D;
		face->getProjectedVertices(facePoints);
		GeomUtils::poly2VecToArray2(facePoints, facePointsArray2D);
		double area = GeomUtils::signedArea(facePointsArray2D);
		return area;
	}

	static std::vector<vec3> getFacePoints(const carve::mesh::Face<3>* face)
	{
		std::vector<vec3> lines;

		std::vector<carve::mesh::Face<3>::vertex_t* > verts3d;
		face->getVertices(verts3d);

		for( size_t i3 = 0; i3 < verts3d.size(); ++i3 )
		{
			const carve::geom::vector<3>& vertex_point = verts3d[i3]->v;
			lines.push_back(vertex_point);
		}
		return lines;
	}

	static void getMeshVertexPoints(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::vector<glm::dvec3>& vecAllPoints)
	{
		for( size_t ii = 0; ii < meshset->vertex_storage.size(); ++ii )
		{
			carve::geom::vector<3>& carvePoint3D = meshset->vertex_storage[ii].v;
			vecAllPoints.push_back(glm::dvec3(carvePoint3D.x, carvePoint3D.y, carvePoint3D.z));
		}
	}

	static void getFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, std::set<carve::mesh::Face<3>* >& setAllFaces)
	{
		setAllFaces.clear();

		for( carve::mesh::Mesh<3>*mesh : meshsetInput->meshes )
		{
			for( carve::mesh::Face<3>* face : mesh->faces )
			{
				setAllFaces.insert(face);
			}
		}
	}

	static bool checkMeshNonNegativeAndClosed( carve::mesh::Mesh<3>* mesh_i)
	{
		if( !mesh_i )
		{
			return false;
		}

		bool meshes_closed = true;

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
						std::cout << "could not invert mesh " << std::endl;
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

		return meshes_closed;
	}

	static bool checkMeshSetNonNegativeAndClosed(const shared_ptr<carve::mesh::MeshSet<3>> mesh_set)
	{
		bool meshes_closed = true;
		if( !mesh_set->isClosed() )
		{
			//GeomUtils::closeMeshSet(mesh_set.get());
		}

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

	inline void checkFaceLoops(const carve::mesh::Face<3>* face)
	{
		carve::mesh::Edge<3>* e = face->edge;
		if( !e )
		{
			return;
		}
		size_t closed_edges = 0;

		size_t numEdges = face->n_edges;
		if( numEdges == 0 )
		{
			return;
		}

		size_t ii = 0;
		do {
			if( e->rev == nullptr ) {
				//open_edges.push_back(e);
			}
			else if( e < e->rev ) {
				//closed_edges.push_back(e);
				++closed_edges;
			}
			e = e->next;

			if( ii > numEdges * 3 )
			{
				break;
			}
			if( ii > 10000 )
			{
				break;
			}
			++ii;

		} while( e != face->edge );

		if( ii > numEdges )
		{
			std::cout << "invalid edge loop " << std::endl;

#ifdef _DEBUG
			bool dumpFaces = true;
			if( dumpFaces )
			{
				GeomDebugDump::moveOffset(0.4);
				std::vector<carve::mesh::Face<3>* > vecFaces = { (carve::mesh::Face<3>*)face };
				glm::vec4 color2(0.7, 0.2, 0.2, 0.8);
				GeomDebugDump::dumpFaces(vecFaces, color2, false);
			}
#endif
		}
	}

	static void checkFaceLoops( carve::mesh::Mesh<3>* mesh)
	{
		for (size_t i = 0; i < mesh->faces.size(); ++i)
		{
			carve::mesh::Face<3>* face = mesh->faces[i];
			checkFaceLoops(face);
		}
	}

	static void checkFaceLoops(carve::mesh::MeshSet<3>* meshset)
	{
		for( size_t kk = 0; kk < meshset->meshes.size(); ++kk )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			checkFaceLoops(mesh);
		}
	}

	static bool checkMeshSetValidAndClosed(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, StatusCallback* report_callback, BuildingEntity* entity)
	{
		if( !meshset )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_tag << " not valid" << std::endl;
			}
#endif
			info.allPointersValid = false;
			info.meshSetValid = false;
			return false;
		}
		if( meshset->meshes.size() == 0 )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_tag << " has no meshes" << std::endl;
			}
#endif
			return false;
		}

#ifdef _DEBUG
		checkFaceLoops(meshset.get());
#endif

		// check open edges first, since it is the easiest and cheapest
		info.numOpenEdges = 0;
		info.numZeroAreaFaces = 0;
		for( size_t kk = 0; kk < meshset->meshes.size(); ++kk )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			info.numOpenEdges += mesh->open_edges.size();
			info.numClosedEdges += mesh->closed_edges.size();
			info.numFaces += mesh->faces.size();

			for( size_t jj = 0; jj < mesh->faces.size(); ++jj )
			{
				const carve::mesh::Face<3>* inputFace = mesh->faces[jj];
				double face_area = MeshUtils::computeFaceArea(inputFace);
				if( std::abs(face_area) < carve::CARVE_EPSILON )
				{
					++info.numZeroAreaFaces;
				}
				info.surfaceArea += face_area;
			}
		}

		// check for valid pointers first
		if( !checkMeshSetPointers(meshset, info) )
		{
			return false;
		}

		if( !checkMeshFins(meshset) )
		{
			info.meshSetValid = false;
			return false;
		}

		if( info.numOpenEdges > 0 )
		{
			info.meshSetValid = false;
			return false;
		}

		std::stringstream err;
		bool meshes_closed = checkMeshSetNonNegativeAndClosed(meshset);
		if( meshes_closed )
		{
			// check volume
			double meshset_volume = 0;
			for( size_t kk = 0; kk < meshset->meshes.size(); ++kk )
			{
				carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
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
			if( report_callback )
			{
				report_callback->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity);
			}
#endif
			info.meshSetValid = false;
			return false;
		}

		info.meshSetValid = true;
		return true;
	}

	inline bool checkMeshValidAndClosed( carve::mesh::Mesh<3>* mesh, MeshSetInfo& info, StatusCallback* report_callback, BuildingEntity* entity)
	{

		if( !mesh )
		{
#ifdef _DEBUG
			if( entity )
			{
				std::cout << "MeshSet of entity #" << entity->m_tag << " not valid" << std::endl;
			}
#endif
			return false;
		}

		info.numOpenEdges = mesh->open_edges.size();
		if( info.numOpenEdges > 0 )
		{
			return false;
		}

		if( !checkMeshPointers(mesh, info) )
		{
			return false;
		}
		info.numOpenEdges += mesh->open_edges.size();
		info.numClosedEdges += mesh->closed_edges.size();
		info.numFaces += mesh->faces.size();

		for( size_t jj = 0; jj < mesh->faces.size(); ++jj )
		{
			const carve::mesh::Face<3>* inputFace = mesh->faces[jj];
			double face_area = MeshUtils::computeFaceArea(inputFace);
			if( std::abs(face_area) < carve::CARVE_EPSILON )
			{
				++info.numZeroAreaFaces;
			}
			info.surfaceArea += face_area;
		}

		std::stringstream err;
		bool meshes_closed = checkMeshNonNegativeAndClosed(mesh);
		if( meshes_closed )
		{
			// check volume
			double meshset_volume = 0;
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
						std::cout << "could not invert negative mesh " << std::endl;
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
		else
		{
			err << "mesh_set not closed" << std::endl;
		}

		if( err.tellp() > 0 )
		{
#ifdef _DEBUG
			if( report_callback )
			{
				report_callback->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity);
			}
#endif
			return false;
		}
		return true;
	}

	inline bool edgeToEdgeIntersect(const carve::mesh::Edge<3>* edge1, const carve::mesh::Edge<3>* edge2, double eps, vec3& intersectionPoint)
	{
		const vec3 A = edge1->v1()->v;
		const vec3 B = edge1->v2()->v;
		const vec3 C = edge2->v1()->v;
		const vec3 D = edge2->v2()->v;
		
		vec3 u = B - A;
		vec3 v = D - C;
		vec3 w = A - C;

		double    a = dot(u,u);         // always >= 0
		double    b = dot(u,v);
		double    c = dot(v,v);         // always >= 0
		double    d = dot(u,w);
		double    e = dot(v,w);
		double    sc, sN, sD = a*c - b*b;  // sc = sN / sD, sD >= 0
		double    tc, tN, tD = a*c - b*b;  // tc = tN / tD, tD >= 0

		// compute the line parameters of the two closest points
		if (sD < eps)
		{
			// the lines are almost parallel
			sN = 0.0;              // force using point A on segment AB to prevent possible division by 0.0 later
			sD = 1.0;
			tN = e;
			tD = c;
		}
		else
		{
			// get the closest points on the infinite lines
			sN = (b*e - c*d);
			tN = (a*e - b*d);
			if (sN < 0.0)
			{
				// sc < 0 => the s=0 edge is visible
				sN = 0.0;          // compute shortest connection of A to segment CD
				tN = e;
				tD = c;
			}
			else if (sN > sD) {    // sc > 1  => the s=1 edge is visible
				sN = sD;           // compute shortest connection of B to segment CD
				tN = e + b;
				tD = c;
			}
		}

		if (tN < 0.0)
		{
			// tc < 0 => the t=0 edge is visible
			tN = 0.0;             
			// recompute sc for this edge
			if( -d < 0.0 )          // compute shortest connection of C to segment AB
			{
				sN = 0.0;
			}
			else if( -d > a )
			{
				sN = sD;
			}
			else
			{
				sN = -d;
				sD = a;
			}
		}
		else if (tN > tD)
		{
			// tc > 1  => the t=1 edge is visible
			tN = tD;
			// recompute sc for this edge
			if( (-d + b) < 0.0 )  // compute shortest connection of D to segment AB
			{
				sN = 0;
			}
			else if( (-d + b) > a )
			{
				sN = sD;
			}
			else
			{
				sN = (-d +  b);
				sD = a;
			}
		}

		sc = fabs(sN) < eps ? 0.0 : sN / sD;
		tc = fabs(tN) < eps ? 0.0 : tN / tD;

		vec3 P1 = A + (sc * u);
		vec3 P2 = C + (tc * v);  
		vec3 delt = P2 - P1;
		if( delt.length2() < eps*eps )
		{
			// intersecting
			if( sc < eps )
			{
				// intersection is at edge1->v1() -> split edge2

				if( tc < eps || fabs(tc - 1.0) < eps )
				{
					// intersecting at point, not edge
					return false;
				}
			}
			else if( fabs(sc-1.0) < eps )
			{
				if( tc < eps || fabs(tc - 1.0) < eps )
				{
					// intersecting at point, not edge
					return false;
				}


				// intersection is at edge1->v2() -> split edge2
			}

			// split edge1
			carve::mesh::Face<3>* face = edge1->face;

			if( edge1->rev )
			{
				carve::mesh::Face<3>* faceRev = edge1->rev->face;
			}
			//carve::mesh::Vertex<3>* vertexNew = new carve::mesh::Vertex<3>( P1 );
			//carve::mesh::Edge<3>* edgeNew = new carve::mesh::Edge<3>( vertexNew, face );

			intersectionPoint = P1;
			return true;
		}
		return false;
	}

	///\brief method createTriangulated3DFace: Creates a triangulated face
	///\param[in] inputBounds3D: Curves as face boundaries. The first input curve is the outer boundary, succeeding curves are inner boundaries
	///\param[in] ifc_entity: Ifc entity that the geometry belongs to, just for error messages. Pass a nullptr if no entity at hand.
	///\param[out] meshOut: Result mesh
	static void createTriangulated3DFace(const std::vector<std::vector<vec3> >& inputBounds3D, BuildingEntity* ifc_entity, PolyInputCache3D& meshOut, bool mergeAlignedEdges, bool dumpPolygon, StatusCallback* callbackFunc)
	{
		if( inputBounds3D.size() == 1 )
		{
			const std::vector<vec3>& outerLoop = inputBounds3D[0];
			if( outerLoop.size() < 3 )
			{
				return;
			}
			if( outerLoop.size() == 3 )
			{
				const vec3& v0 = outerLoop[0];
				const vec3& v1 = outerLoop[1];
				const vec3& v2 = outerLoop[2];
				int idxA = meshOut.addPoint(v0);
				int idxB = meshOut.addPoint(v1);
				int idxC = meshOut.addPoint(v2);
				meshOut.m_poly_data->addFace(idxA, idxB, idxC);

#ifdef _DEBUG
				if( dumpPolygon )
				{
					glm::vec4 color(0, 1, 1, 1);
					PolyInputCache3D poly( carve::CARVE_EPSILON );
					int idxA = poly.addPoint(v0);
					int idxB = poly.addPoint(v1);
					int idxC = poly.addPoint(v2);
					poly.m_poly_data->addFace(idxA, idxB, idxC);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(carve::input::opts()));
					GeomDebugDump::dumpMeshset(meshset, color, false);
				}
#endif

				return;
			}
			if( outerLoop.size() == 4 )
			{
				const vec3& v0 = outerLoop[0];
				const vec3& v1 = outerLoop[1];
				const vec3& v2 = outerLoop[2];
				const vec3& v3 = outerLoop[3];
				int idxA = meshOut.addPoint(v0);
				int idxB = meshOut.addPoint(v1);
				int idxC = meshOut.addPoint(v2);
				int idxD = meshOut.addPoint(v3);
				//meshOut.m_poly_data->addFace(idxA, idxB, idxC, idxD);
				meshOut.m_poly_data->addFace(idxA, idxB, idxC);
				meshOut.m_poly_data->addFace(idxA, idxC, idxD);

#ifdef _DEBUG
				if( dumpPolygon )
				{
					glm::vec4 color(0, 1, 1, 1);
					PolyInputCache3D poly( carve::CARVE_EPSILON );
					int idxA = poly.addPoint(v0);
					int idxB = poly.addPoint(v1);
					int idxC = poly.addPoint(v2);
					int idxD = poly.addPoint(v3);
					poly.m_poly_data->addFace(idxA, idxB, idxC, idxD);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(carve::input::opts()));
					GeomDebugDump::dumpMeshset(meshset, color, false);
				}
#endif

				return;
			}
		}

#ifdef _DEBUG
		PolyInputCache3D poly(carve::CARVE_EPSILON);
#endif

		std::vector<std::vector<std::array<double, 2> > > polygons2d;
		std::vector<std::vector<vec3> > polygons3d;
		std::vector<double> polygon3DArea;
		bool face_loop_reversed = false;
		bool warning_small_loop_detected = false;
		bool errorOccured = false;
		GeomUtils::ProjectionPlane face_plane = GeomUtils::ProjectionPlane::UNDEFINED;
		vec3 normal = carve::geom::VECTOR(0, 0, 1);
		vec3 normalOuterBound = carve::geom::VECTOR(0, 0, 1);

		for( auto it_bounds = inputBounds3D.begin(); it_bounds != inputBounds3D.end(); ++it_bounds )
		{
			std::vector<vec3> loopPoints3Dinput = *it_bounds;

			if( loopPoints3Dinput.size() < 3 )
			{
				if( it_bounds == inputBounds3D.begin() )
				{
					break;
				}
				else
				{
					continue;
				}
			}

			//bool mergeAlignedEdges = true;
			GeomUtils::simplifyPolygon(loopPoints3Dinput, mergeAlignedEdges);
			GeomUtils::unClosePolygon(loopPoints3Dinput);
			normal = GeomUtils::computePolygonNormal(loopPoints3Dinput);

			if( it_bounds == inputBounds3D.begin() )
			{
				normalOuterBound = normal;

				// figure out on which plane to project the 3D points
				double nx = std::abs(normal.x);
				double ny = std::abs(normal.y);
				double nz = std::abs(normal.z);
				if( nz > nx && nz >= ny )
				{
					face_plane = GeomUtils::XY_PLANE;
				}
				else if( nx >= ny && nx >= nz )
				{
					face_plane = GeomUtils::YZ_PLANE;
				}
				else if( ny > nx && ny >= nz )
				{
					face_plane = GeomUtils::XZ_PLANE;
				}
				else
				{
					std::stringstream err;
					err << "unable to project to plane: nx" << nx << " ny " << ny << " nz " << nz << std::endl;
					if( callbackFunc )
					{
						callbackFunc->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
					}
					continue;
				}
			}

			// project face into 2d plane
			std::vector<std::array<double, 2> > path_loop_2d;
			std::vector<vec3> path_loop_3d;

			for( size_t i = 0; i < loopPoints3Dinput.size(); ++i )
			{
				const vec3& point = loopPoints3Dinput[i];
				path_loop_3d.push_back(point);
				if( face_plane == GeomUtils::XY_PLANE )
				{
					path_loop_2d.push_back({ point.x, point.y });
				}
				else if( face_plane == GeomUtils::YZ_PLANE )
				{
					path_loop_2d.push_back({ point.y, point.z });
				}
				else if( face_plane == GeomUtils::XZ_PLANE )
				{
					path_loop_2d.push_back({ point.x, point.z });
				}
			}

			if( path_loop_2d.size() < 3 )
			{
				//std::cout << __FUNC__ << ": #" << face_id <<  "=IfcFace: path_loop.size() < 3" << std::endl;
				continue;
			}

			double loop_area = std::abs(GeomUtils::signedArea(path_loop_2d));
			double min_loop_area = EPS_DEFAULT;
			if( loop_area < min_loop_area )
			{
				warning_small_loop_detected = true;
				continue;
			}

			if( loopPoints3Dinput.size() == 4 && inputBounds3D.size() == 1 )
			{
				if( GeomUtils::isQuadConvex(path_loop_2d[0], path_loop_2d[1], path_loop_2d[2], path_loop_2d[3]) )
				{
					// add 2 triangles for quad
					vec3 v0 = loopPoints3Dinput[0];
					vec3 v1 = loopPoints3Dinput[1];
					vec3 v2 = loopPoints3Dinput[2];
					vec3 v3 = loopPoints3Dinput[3];
					uint32_t idx0 = meshOut.addPoint(v0);
					uint32_t idx1 = meshOut.addPoint(v1);
					uint32_t idx2 = meshOut.addPoint(v2);
					uint32_t idx3 = meshOut.addPoint(v3);

#ifdef _DEBUG
					uint32_t idx0_dbg = poly.addPoint(v0);
					uint32_t idx1_dbg = poly.addPoint(v1);
					uint32_t idx2_dbg = poly.addPoint(v2);
					uint32_t idx3_dbg = poly.addPoint(v3);
#endif

					vec3 normalTriangle0 = GeomUtils::computePolygonNormal({ v0, v1, v2 });
					vec3 normalTriangle1 = GeomUtils::computePolygonNormal({ v2, v3, v0 });

					if( dot(normalTriangle0, normalOuterBound) > 0 )
					{
						// normalTriangle0 and normalOuterBound should point in the same direction" << std::endl;
						meshOut.m_poly_data->addFace(idx0, idx1, idx2);
#ifdef _DEBUG
						poly.m_poly_data->addFace(idx0_dbg, idx1_dbg, idx2_dbg);
#endif
					}
					else
					{
						// normalTriangle0 and normalOuterBound should point in the same direction" << std::endl;
						meshOut.m_poly_data->addFace(idx0, idx2, idx1);
#ifdef _DEBUG
						poly.m_poly_data->addFace(idx0_dbg, idx2_dbg, idx1_dbg);
#endif
					}

					if( dot(normalTriangle1, normalOuterBound) > 0 )
					{
						//std::cout << "normalTriangle1 and normalOuterBound should point in the same direction" << std::endl;
						meshOut.m_poly_data->addFace(idx2, idx3, idx0);
#ifdef _DEBUG
						poly.m_poly_data->addFace(idx2_dbg, idx3_dbg, idx0_dbg);
#endif
					}
					else
					{
						meshOut.m_poly_data->addFace(idx2, idx0, idx3);
#ifdef _DEBUG
						poly.m_poly_data->addFace(idx2_dbg, idx0_dbg, idx3_dbg);
#endif
					}

					return;
				}
			}

			// outer loop (biggest area) needs to come first
			bool insertPositionFound = false;
			for( size_t iiArea = 0; iiArea < polygon3DArea.size(); ++iiArea )
			{
				double existingLoopArea = polygon3DArea[iiArea];

				// existingArea[i]  < loop_area < existingArea[i+1]
				if( loop_area > existingLoopArea )
				{
					polygons2d.insert(polygons2d.begin() + iiArea, path_loop_2d);
					polygons3d.insert(polygons3d.begin() + iiArea, path_loop_3d);
					polygon3DArea.insert(polygon3DArea.begin() + iiArea, loop_area);
					insertPositionFound = true;
					break;
				}
			}

			if( !insertPositionFound )
			{
				polygons2d.push_back(path_loop_2d);
				polygons3d.push_back(path_loop_3d);
				polygon3DArea.push_back(loop_area);
			}
		}

#ifdef _DEBUG
		// check descending order
		if( polygon3DArea.size() > 0 )
		{
			double previousLoopArea = polygon3DArea[0];
			if( polygon3DArea.size() > 1 )
			{
				for( size_t iiArea = 1; iiArea < polygon3DArea.size(); ++iiArea )
				{
					double loopArea = polygon3DArea[iiArea];
					if( loopArea > previousLoopArea )
					{
						std::cout << "polygon3DArea not descending" << std::endl;
					}
				}
			}
		}
#endif
		if( polygons3d.size() > 0 )
		{
			std::vector<vec3>& loopOuterBound = polygons3d[0];
			normalOuterBound = GeomUtils::computePolygonNormal(loopOuterBound);
		}

		// check winding order in 2D
		std::vector<std::array<double, 2> > polygons2dFlatVector;
		for( size_t ii = 0; ii < polygons2d.size(); ++ii )
		{
			std::vector<std::array<double, 2> >& loop2D = polygons2d[ii];
			std::vector<vec3>& loop3D = polygons3d[ii];

			glm::dvec3 normal_2d = GeomUtils::computePolygon2DNormal(loop2D);
			if( ii == 0 )
			{
				if( normal_2d.z < 0 )
				{
					std::reverse(loop2D.begin(), loop2D.end());
					std::reverse(loop3D.begin(), loop3D.end());
					face_loop_reversed = true;
				}
			}
			else
			{
				if( normal_2d.z > 0 )
				{
					std::reverse(loop2D.begin(), loop2D.end());
					std::reverse(loop3D.begin(), loop3D.end());
				}
			}
		}

		if( warning_small_loop_detected )
		{
			std::stringstream err;
			err << "std::abs( signed_area ) < 1.e-10";
			if( callbackFunc )
			{
				callbackFunc->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, ifc_entity);
			}
		}

		if( polygons2d.size() > 0 )
		{
			std::vector<uint32_t> triangulated = mapbox::earcut<uint32_t>(polygons2d);

			std::vector<vec3> polygons3dFlatVector;
			GeomUtils::polygons2flatVec(polygons3d, polygons3dFlatVector);

			for( int ii = 0; ii < triangulated.size(); ii += 3 )
			{
				size_t idxA = triangulated[ii + 0];
				size_t idxB = triangulated[ii + 1];
				size_t idxC = triangulated[ii + 2];

				const vec3& pointA = polygons3dFlatVector[idxA];
				const vec3& pointB = polygons3dFlatVector[idxB];
				const vec3& pointC = polygons3dFlatVector[idxC];

				idxA = meshOut.addPoint(pointA);
				idxB = meshOut.addPoint(pointB);
				idxC = meshOut.addPoint(pointC);

#ifdef _DEBUG
				size_t idxA_dbg = poly.addPoint(pointA);
				size_t idxB_dbg = poly.addPoint(pointB);
				size_t idxC_dbg = poly.addPoint(pointC);
#endif

				vec3 triangleNormal = GeomUtils::computePolygonNormal({ pointA, pointB, pointC });
				if( dot(triangleNormal, normalOuterBound) >= 0 )
				{

					meshOut.m_poly_data->addFace(idxA, idxB, idxC);
#ifdef _DEBUG
					poly.m_poly_data->addFace(idxA_dbg, idxB_dbg, idxC_dbg);
#endif
				}
				else
				{
					meshOut.m_poly_data->addFace(idxA, idxC, idxB);
#ifdef _DEBUG
					poly.m_poly_data->addFace(idxA_dbg, idxC_dbg, idxB_dbg);
#endif
				}
			}
#ifdef _DEBUG
			if( errorOccured )
			{
				//glm::vec4 color(0, 1, 1, 1);
				//GeomDebugDump::dumpPolyline(polygons2d, color, true);
				//shared_ptr<carve::mesh::MeshSet<3> > meshset(meshOut.m_poly_data->createMesh(carve::input::opts()));
				//GeomDebugDump::dumpMeshset(meshset, color, true, true);
			}
#endif
		}

#ifdef _DEBUG
		if( errorOccured || dumpPolygon)
		{
			glm::vec4 color(0, 1, 1, 1);
			shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(carve::input::opts()));
			GeomDebugDump::dumpMeshset(meshset, color, false);
		}
#endif
	}

	///\brief method intersectOpenEdges: Intersect open edges of MeshSet with closed edges, and split the open edges in case of intersection
	///\param[in/out] meshset: MeshSet with open edges. If fix is found, a new MeshSet is assigned to the smart pointer
	///\param[in] eps: tolerance to find edge-edge intersections
	///\param[in] dumpMeshes: write meshes to dump file for debugging
	inline void intersectOpenEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, double eps, bool dumpMeshes)
	{
		if( !meshset )
		{
			return;
		}

		size_t maxNumFaces = 600;
		size_t maxNumEdges = 600;
		size_t maxNumOpenEdges = 100;

#ifdef _DEBUG
		glm::vec4 color(0.5, 0.6, 0.7, 1.0);
		if( dumpMeshes )
		{
			GeomDebugDump::moveOffset(0.8);
			GeomDebugDump::dumpMeshset(meshset, color, true);
			double dy = meshset->getAABB().extent.y;
			GeomDebugDump::moveOffset(dy*2.2);
			GeomDebugDump::dumpMeshsetOpenEdges(meshset, color, false, false);
			GeomDebugDump::moveOffset(1.2);
		}
#endif

		for( size_t round = 0; round < 5; ++round )
		{
			size_t numClosedEdgesBefore = 0;
			std::vector<carve::mesh::Edge<3>* > allOpenEdges;
			std::vector<carve::mesh::Face<3>* > allFaces;

			for( size_t ii = 0; ii < meshset->meshes.size(); ++ii )
			{
				carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
				numClosedEdgesBefore += mesh->closed_edges.size();
				std::copy(mesh->open_edges.begin(), mesh->open_edges.end(), std::back_inserter(allOpenEdges));
				std::copy(mesh->faces.begin(), mesh->faces.end(), std::back_inserter(allFaces));
			}

			PolyInputCache3D polyInput(eps);

			// intersect with closed edges
			size_t numSplitEdges = 0;
			for( size_t iiFace = 0; iiFace < allFaces.size(); ++iiFace )
			{
				if( iiFace > maxNumFaces )
				{
					return;
				}
				carve::mesh::Face<3>* face = allFaces[iiFace];

				const size_t n_edges = face->n_edges;
				if( n_edges > maxNumEdges )
				{
#ifdef _DEBUG
					if( dumpMeshes )
					{
						std::cout << "n_edges > maxNumEdges" << std::endl;
						glm::vec4 color(0.3, 0.3, 0.3, 1.);
						GeomDebugDump::dumpFacePolygon(face, color, false);
					}
#endif
					return;
				}

				std::vector<vec3> faceLoop;
				carve::mesh::Edge<3>* edge = face->edge;
				faceLoop.push_back(edge->v1()->v);

				for( size_t i_edge = 0; i_edge < n_edges; ++i_edge )
				{
					// check if current edge needs to be split
					for( size_t iiOpenEdge = 0; iiOpenEdge < allOpenEdges.size(); ++iiOpenEdge )
					{
						if( iiOpenEdge > maxNumOpenEdges )
						{
							return;
						}
						const carve::mesh::Edge<3>* openEdge = allOpenEdges[iiOpenEdge];
						vec3 openEdgeStart = openEdge->v1()->v;
						vec3 openEdgeEnd = openEdge->v2()->v;

						vec3 intersectionPoint;
						bool intersect = edgeToEdgeIntersect(openEdge, edge, eps, intersectionPoint);

						if( intersect )
						{
							faceLoop.push_back(intersectionPoint);
							++numSplitEdges;

							// handle only first split. TODO: sort split points and add to faceLoop, sorted
							break;
						}
					}
					faceLoop.push_back(edge->v2()->v);

					edge = edge->next;

					if( edge == face->edge )
					{
						break;
					}
				}

				bool addFaceDirectly = true;
				if( addFaceDirectly )
				{
					std::vector<int> faceIndexes;
					for( size_t iiPoint = 0; iiPoint < faceLoop.size(); ++iiPoint )
					{
						const vec3& v0 = faceLoop[iiPoint];
						int idxA = polyInput.addPoint(v0);
						if( faceIndexes.size() > 0 )
						{
							int previousIndex = faceIndexes.back();
							if( idxA == previousIndex )
							{
								continue;
							}
						}
						faceIndexes.push_back(idxA);
					}

					if( faceIndexes.size() > 1 )
					{
						for( size_t iiPoint = 0; iiPoint < faceIndexes.size(); ++iiPoint )
						{
							size_t idxFirst = faceIndexes.front();
							size_t idxLast = faceIndexes.back();
							if( idxFirst == idxLast )
							{
								faceIndexes.pop_back();
								if( faceIndexes.size() < 2 )
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
					}

					if( faceIndexes.size() > 2 )
					{
						polyInput.m_poly_data->addFace(faceIndexes.begin(), faceIndexes.end());
					}
				}
				else
				{
					std::vector<std::vector<vec3> > faceLoops = { faceLoop };
					bool mergeAlignedEdges = false;
					bool dumpPolygon = true;
					createTriangulated3DFace(faceLoops, nullptr, polyInput, mergeAlignedEdges, dumpPolygon, nullptr);
				}
			}

			if( numSplitEdges > 0 )
			{
				bool correct = checkPolyhedronData(polyInput.m_poly_data);
				if( !correct )
				{
					bool correct = fixPolyhedronData(polyInput.m_poly_data);
#ifdef _DEBUG
					std::cout << "incorrect idx";
#endif
					return;
				}

				shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts()));
				if( meshsetNew->isClosed() )
				{
					meshset = meshsetNew;
					return;
				}
				else
				{
					size_t numOpenEdgesNew = 0;
					size_t numClosedEdgesNew = 0;
					for( size_t ii = 0; ii < meshsetNew->meshes.size(); ++ii )
					{
						carve::mesh::Mesh<3>* mesh = meshsetNew->meshes[ii];
						numOpenEdgesNew += mesh->open_edges.size();
						numClosedEdgesNew += mesh->closed_edges.size();
					}

					if( numOpenEdgesNew < allOpenEdges.size() )
					{
						size_t numAllEdgesNew = numOpenEdgesNew + numClosedEdgesNew;
						size_t numAllEdgesBefore = numClosedEdgesBefore + allOpenEdges.size();
						if( numClosedEdgesNew >= numClosedEdgesBefore )
						{
							meshset = meshsetNew;
						}
					}

#ifdef _DEBUG
					if( dumpMeshes )
					{
						glm::vec4 color(0.3, 0.3, 0.3, 1.);
						GeomDebugDump::dumpMeshset(meshsetNew, color, false);

						double dy = meshsetNew->getAABB().extent.y;
						GeomDebugDump::moveOffset(dy * 2.2);
						GeomDebugDump::dumpMeshsetOpenEdges(meshsetNew, color, false, false);
						GeomDebugDump::moveOffset(dy * 3.2);
					}
#endif
				}
			}
		}
	}
	
	static void triangulateLoops( const std::vector<std::vector<vec2> >& profilePathsInput, std::vector<std::vector<vec2> >& faceLoopsUsedForTriangulation, std::vector<uint32_t>& face_indexes_out, BuildingEntity* ifc_entity )
	{
		// TODO: complete and test
		if( profilePathsInput.size() == 0 )
		{
			//messageCallback( "profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}
		if( faceLoopsUsedForTriangulation.size() > 0 )
		{
			faceLoopsUsedForTriangulation.clear();
		}

		// figure 1: loops and indexes
		//  3----------------------------2
		//  |                            |
		//  |   1-------------------2    |3---------2
		//  |   |                   |    |          |
		//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
		//  |   0---face_loops[1]---3    |0---------1
		//  |                            |
		//  0-------face_loops[0]--------1

		vec3 normal_first_loop;
		bool warning_small_loop_detected = false;
		bool polyline_created = false;

		for( size_t i_face_loops = 0; i_face_loops < profilePathsInput.size(); ++i_face_loops )
		{
			const std::vector<vec2>& loop_input = profilePathsInput[i_face_loops];
			std::vector<vec2> loop_2d;
			GeomUtils::copyClosedLoopSkipDuplicates( loop_input, loop_2d );

			if( loop_2d.size() < 3 )
			{
				if( profilePathsInput.size() == 1 )
				{
					// Cross section is just a point or a line. Create a face with one index
					face_indexes_out.push_back( loop_2d.size() );  // num points
					for( size_t ii = 0; ii < loop_2d.size(); ++ii )
					{
						face_indexes_out.push_back( ii );  // point index
					}
					// Nothing to triangulate, so return
					return;
				}
				continue;
			}

			// check winding order
			bool reverse_loop = false;
			vec3  normal_2d = GeomUtils::computePolygon2DNormal( loop_2d );
			if( i_face_loops == 0 )
			{
				normal_first_loop = normal_2d;
				if( normal_2d.z < 0 )
				{
					reverse_loop = true;
					normal_first_loop = -normal_first_loop;
				}
			}
			else
			{
				if( normal_2d.z > 0 )
				{
					reverse_loop = true;
				}
			}
			if( reverse_loop )
			{
				std::reverse( loop_2d.begin(), loop_2d.end() );
			}

			if( loop_2d.size() < 3 )
			{
				//messageCallback( "loop_2d.size() < 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			}

			double signed_area = carve::geom2d::signedArea( loop_2d );
			double min_loop_area = 1.e-10;//m_geom_settings->m_min_face_area
			if( std::abs( signed_area ) < min_loop_area )
			{
				warning_small_loop_detected = true;
				continue;
			}

			faceLoopsUsedForTriangulation.push_back( loop_2d );
		}

		if( warning_small_loop_detected )
		{
			//messageCallback( "std::abs( signed_area ) < 1.e-10", StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, ifc_entity );
		}

		if( faceLoopsUsedForTriangulation.size() == 0 )
		{
			if( polyline_created )
			{
				// already handled as curve
				return;
			}
			//messageCallback( "face_loops.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		size_t num_vertices_in_loops = 0;
		for( size_t ii = 0; ii < faceLoopsUsedForTriangulation.size(); ++ii )
		{
			std::vector<vec2>&  loop = faceLoopsUsedForTriangulation[ii];
			num_vertices_in_loops += loop.size();
		}

		// triangulate
		std::vector<vec2> merged_path;
		std::vector<carve::triangulate::tri_idx> triangulated;
		std::vector<std::pair<size_t, size_t> > path_incorporated_holes;
		try
		{
#ifdef _DEBUG
			if( profilePathsInput.size() > 1 )
			{
				GeomDebugDump::moveOffset(0.3);

				for( size_t ii = 0; ii < profilePathsInput.size(); ++ii )
				{
					const std::vector<vec2>& loop_input = profilePathsInput[ii];
					glm::dvec4 color(0.9 - ii*0.1, 0.4, 0.1 + ii*0.2, 1.0);
					GeomDebugDump::dumpPolyline(loop_input, color, false);
				}
			}
#endif

			path_incorporated_holes = carve::triangulate::incorporateHolesIntoPolygon( faceLoopsUsedForTriangulation );
			// figure 2: path which incorporates holes, described by path_all_loops
			// (0/0) -> (1/3) -> (1/0) -> (1/1) -> (1/2) -> (1/3) -> (0/0) -> (0/1) -> (0/2) -> (0/3)

			//  0/3<----------------------------0/2
			//  |                                ^
			//  |  1/0------------------>1/1     |
			//  |   ^                    |       |
			//  |   |                    v       |  path_incorporated_holes
			//  |  1/3  1/3<-------------1/2     |
			//  v /    /                         |
			//  0/0  0/0----------------------->0/1

			merged_path.reserve( path_incorporated_holes.size() );
			for( size_t i = 0; i < path_incorporated_holes.size(); ++i )
			{
				size_t loop_number = path_incorporated_holes[i].first;
				size_t index_in_loop = path_incorporated_holes[i].second;

				if( loop_number >= faceLoopsUsedForTriangulation.size() )
				{
					//messageCallback( "loop_number >= face_loops_used_for_triangulation.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}
				std::vector<vec2>& loop = faceLoopsUsedForTriangulation[loop_number];

				if( index_in_loop >= loop.size() )
				{
					//messageCallback( "index_in_loop >= loop.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}
				vec2& point_in_loop = loop[index_in_loop];
				merged_path.push_back( point_in_loop );
			}
			// figure 3: merged path for triangulation
			//  9<--------------------------8
			//  |                           ^
			//  |  2------------------>3    |
			//  |  ^                   |    |
			//  |  |                   v    |  merged_path
			//  |  1  5<---------------4    |
			//  | /  /                      |
			//  0  6----------------------->7
			carve::triangulate::triangulate( merged_path, triangulated );
			//carve::triangulate::improve( merged_path, triangulated );
			// triangles: (3,8,9) (2,0,1) (4,6,7)  (4,5,6)  (9,0,2)  (9,2,3)  (7,8,3)  (3,4,7)

		}
		catch( ... )
		{
#ifdef _DEBUG
			//messageCallback("carve::triangulate failed", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			glm::dvec4 color(0.3, 0.4, 0.5, 1.0);
			GeomDebugDump::moveOffset(0.3);
			GeomDebugDump::dumpPolyline(faceLoopsUsedForTriangulation, color, true );

			GeomDebugDump::moveOffset(0.3);
			GeomDebugDump::dumpPolyline(profilePathsInput, color, true );
			
#endif
			return;
		}

		for( size_t i = 0; i != triangulated.size(); ++i )
		{
			const carve::triangulate::tri_idx& triangle = triangulated[i];
			size_t a = triangle.a;
			size_t b = triangle.b;
			size_t c = triangle.c;

			size_t loop_number_a = path_incorporated_holes[a].first;
			size_t index_in_loop_a = path_incorporated_holes[a].second;

			size_t vertex_id_a = index_in_loop_a;
			for( size_t jj = 0; jj < loop_number_a; ++jj )
			{
				if( faceLoopsUsedForTriangulation.size() > jj )
				{
					vertex_id_a += faceLoopsUsedForTriangulation[jj].size();
				}
			}

			size_t loop_number_b = path_incorporated_holes[b].first;
			size_t index_in_loop_b = path_incorporated_holes[b].second;
			size_t vertex_id_b = index_in_loop_b;
			for( size_t jj = 0; jj < loop_number_b; ++jj )
			{
				if( faceLoopsUsedForTriangulation.size() > jj )
				{
					vertex_id_b += faceLoopsUsedForTriangulation[jj].size();
				}
			}

			size_t loop_number_c = path_incorporated_holes[c].first;
			size_t index_in_loop_c = path_incorporated_holes[c].second;
			size_t vertex_id_c = index_in_loop_c;
			for( size_t jj = 0; jj < loop_number_c; ++jj )
			{
				if( faceLoopsUsedForTriangulation.size() > jj )
				{
					vertex_id_c += faceLoopsUsedForTriangulation[jj].size();
				}
			}

			if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
			{
				continue;
			}

			face_indexes_out.push_back( 3 );
			face_indexes_out.push_back( vertex_id_a );
			face_indexes_out.push_back( vertex_id_b );
			face_indexes_out.push_back( vertex_id_c );
		}

#ifdef _DEBUG
		if( profilePathsInput.size() > 1 )
		{
			std::vector<vec2> loopAll;
			for( std::vector<vec2>& loop : faceLoopsUsedForTriangulation )
			{
				std::copy(loop.begin(), loop.end(), std::back_inserter(loopAll));
			}

			PolyInputCache3D poly;
			for( size_t ii = 0; ii+3 < face_indexes_out.size(); ii+= 4 )
			{
				size_t idx0 = face_indexes_out[ii + 1];
				size_t idx1 = face_indexes_out[ii + 2];
				size_t idx2 = face_indexes_out[ii + 3];
				vec2& pt0 = loopAll[idx0];
				vec2& pt1 = loopAll[idx1];
				vec2& pt2 = loopAll[idx2];
				size_t idx0_poly = poly.addPoint(carve::geom::VECTOR(pt0.x, pt0.y, 0));
				size_t idx1_poly = poly.addPoint(carve::geom::VECTOR(pt1.x, pt1.y, 0));
				size_t idx2_poly = poly.addPoint(carve::geom::VECTOR(pt2.x, pt2.y, 0));
				poly.m_poly_data->addFace(idx0_poly, idx1_poly, idx2_poly);
			}
			
			glm::dvec4 colorCurrentLabel(0.3, 0.4, 0.5, 1.0);
			std::string labelStr = "carve_triangulation";
			GeomDebugDump::dumpVertex(GeomDebugDump::labelPos, colorCurrentLabel, labelStr);
			GeomDebugDump::dumpCountLabel(GeomDebugDump::countLabelPos);
			
			glm::dvec4 color(0.3, 0.4, 0.5, 1.0);
			shared_ptr<carve::mesh::MeshSet<3> > mesh_triangle(poly.m_poly_data->createMesh(carve::input::opts()));
			GeomDebugDump::dumpMeshset(mesh_triangle, color, true );
		}
#endif
	}

	static std::shared_ptr<carve::mesh::MeshSet<3> > createPlaneMesh(vec3& p0, vec3& p1, vec3& p2)
	{
		carve::input::PolyhedronData polyhedron_data;
		polyhedron_data.addVertex(p0);
		polyhedron_data.addVertex(p1);
		polyhedron_data.addVertex(p2);
		polyhedron_data.addFace(0, 1, 2);
		std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts()));
		return mesh;
	}

	static std::shared_ptr<carve::mesh::MeshSet<3> > createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, vec3& p3)
	{
		carve::input::PolyhedronData polyhedron_data;
		polyhedron_data.addVertex(p0);
		polyhedron_data.addVertex(p1);
		polyhedron_data.addVertex(p2);
		polyhedron_data.addVertex(p3);

		polyhedron_data.addFace(0, 1, 2, 3);
		//polyhedron_data.addFace(2, 3, 0);
		std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts()));
		return mesh;
	}

	static std::shared_ptr<carve::mesh::MeshSet<3> > createBoxMesh(vec3& pos, vec3& extent, carve::math::Matrix& transform)
	{
		carve::input::PolyhedronData polyhedron_data;
		polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(extent.x, extent.y, -extent.z)));
		polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(extent.x, -extent.y, -extent.z)));
		polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(-extent.x, -extent.y, -extent.z)));
		polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(-extent.x, extent.y, -extent.z)));

		polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(extent.x, extent.y, extent.z)));
		polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(extent.x, -extent.y, extent.z)));
		polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(-extent.x, -extent.y, extent.z)));
		polyhedron_data.addVertex(transform * (pos + carve::geom::VECTOR(-extent.x, extent.y, extent.z)));

		polyhedron_data.addFace(0, 1, 2);
		polyhedron_data.addFace(2, 3, 0);
		polyhedron_data.addFace(7, 6, 5);
		polyhedron_data.addFace(5, 4, 7);
		polyhedron_data.addFace(0, 4, 5);
		polyhedron_data.addFace(5, 1, 0);
		polyhedron_data.addFace(1, 5, 6);
		polyhedron_data.addFace(6, 2, 1);
		polyhedron_data.addFace(2, 6, 7);
		polyhedron_data.addFace(7, 3, 2);
		polyhedron_data.addFace(3, 7, 4);
		polyhedron_data.addFace(4, 0, 3);

		std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts()));
		return mesh;
	}

	static bool findEdgeLoop(carve::mesh::Edge<3>* startEdge, const std::set<carve::mesh::Edge<3>* >& vecEdgesInput, std::set<carve::mesh::Edge<3>* >& usedEdges, std::vector<carve::mesh::Vertex<3>* >& vecResultLoop, double eps, bool dumpPolygon)
	{
		if( !startEdge )
		{
			return false;
		}

		if( vecEdgesInput.size() > 1000 )
		{
			return false;
		}

		vec3 startPoint = startEdge->v1()->v;
		vec3 currentEndPoint = startEdge->v2()->v;
		vecResultLoop.push_back(startEdge->v1());
		vecResultLoop.push_back(startEdge->v2());
		usedEdges.clear();
		usedEdges.insert(startEdge);
		
#ifdef _DEBUG
		bool blue = true;
		glm::vec4 color1(1.0, 0.1, 1, 1);
		glm::vec4 colorBlue(0, 0.8, 0.5, 1);
		if( dumpPolygon )
		{
			GeomDebugDump::moveOffset(0.5);
			std::vector<vec3> poly = { startPoint, currentEndPoint };
			GeomDebugDump::dumpPolyline(poly, color1, false);
		}
#endif

		std::set<carve::mesh::Edge<3>* > vecEdges = vecEdgesInput;
		
		for( size_t ii = 0; ii < vecEdges.size(); ++ii )
		{
			bool connectionFound = false;
			for( carve::mesh::Edge<3>*edge : vecEdges )
			{
				if( edge == startEdge )
				{
					continue;
				}

				auto itFind = usedEdges.find(edge);
				if( itFind != usedEdges.end() )
				{
					continue;
				}

				vec3& edgeP0 = edge->v1()->v;
				
				vec3 delt0 = edgeP0 - currentEndPoint;
				if( delt0.length2() < eps * eps )
				{
					// edge start point is same as loop end point -> connected in forward order
					vecResultLoop.push_back(edge->v2());
					currentEndPoint = edge->v2()->v;
					usedEdges.insert(edge);
					connectionFound = true;

#ifdef _DEBUG
					if( dumpPolygon )
					{
						glm::vec4 color = blue ? colorBlue : color1;
						blue = !blue;
						std::vector<vec3> poly = { edge->v1()->v, edge->v2()->v };
						GeomDebugDump::dumpPolyline(  poly, color, false );
					}
#endif

					break;
				}

				vec3& edgeP1 = edge->v2()->v;

				vec3 delt1 = edgeP1 - currentEndPoint;
				if( delt1.length2() < eps * eps )
				{
					// connected in reverse order
					vecResultLoop.push_back(edge->v1());
					currentEndPoint = edge->v1()->v;
					usedEdges.insert(edge);
					connectionFound = true;

#ifdef _DEBUG
					if( dumpPolygon )
					{
						glm::vec4 color = blue ? colorBlue : color1;
						blue = !blue;
						std::vector<vec3> poly = { edge->v2()->v, edge->v1()->v };
						GeomDebugDump::dumpPolyline(  poly, color, false );
					}
#endif

					break;
				}
			}

			if( !connectionFound )
			{
				break;
			}

			// check if loop is closed yet
			if( vecResultLoop.size() == 2 )
			{
#ifdef _DEBUG
				std::cout << "loop of two open edges -> set one as reverse edge of other" << std::endl;
#endif
			}

			if( vecResultLoop.size() > 2 )
			{
				vec3 delt0 = startPoint - currentEndPoint;
				if( delt0.length2() < eps * eps )
				{
					// closed loop
					return true;
				}
			}

			if( usedEdges.size() == vecEdges.size() )
			{
				return false;
			}
		}

		return false;
	}

	inline void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, PolyInputCache3D& polyInput)
	{
		for( size_t ii = 0; ii < meshset->meshes.size(); ++ii )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];

			for( size_t jj = 0; jj < mesh->faces.size(); ++jj )
			{
				carve::mesh::Face<3>* face = mesh->faces[jj];

				carve::mesh::Edge<3>* edge = face->edge;
				if( edge )
				{
					std::vector<int> vecPointIndexes;
					size_t maxNumEdges = 1000;
					for( size_t kk = 0; kk < face->n_edges; ++kk )
					{
						vec3& edgeEndPoint = edge->v2()->v;
						int idx = polyInput.addPoint(edgeEndPoint);
						vecPointIndexes.push_back(idx);

						edge = edge->next;
						if( edge == face->edge )
						{
							break;
						}
					}
					if( vecPointIndexes.size() < 3 )
					{
#ifdef _DEBUG
						std::cout << "face with < 3 edges" << std::endl;
#endif
						continue;
					}
					polyInput.m_poly_data->addFace(vecPointIndexes.begin(), vecPointIndexes.end());
				}
			}
		}
	}

	inline void polyhedronFromMesh(const carve::mesh::Mesh<3>* mesh, PolyInputCache3D& polyInput)
	{
		for( size_t jj = 0; jj < mesh->faces.size(); ++jj )
		{
			carve::mesh::Face<3>* face = mesh->faces[jj];
			carve::mesh::Edge<3>* edge = face->edge;
			if( edge )
			{
				std::vector<int> vecPointIndexes;
				size_t maxNumEdges = 1000;
				for( size_t kk = 0; kk < face->n_edges; ++kk )
				{
					vec3& edgeEndPoint = edge->v2()->v;
					int idx = polyInput.addPoint(edgeEndPoint);
					vecPointIndexes.push_back(idx);

					edge = edge->next;
					if( edge == face->edge )
					{
						break;
					}
				}
				if( vecPointIndexes.size() < 3 )
				{
#ifdef _DEBUG
					std::cout << "face with < 3 edges" << std::endl;
#endif
					continue;
				}
				polyInput.m_poly_data->addFace(vecPointIndexes.begin(), vecPointIndexes.end());
			}
		}
	}

	inline bool addFacesReversed(const PolyInputCache3D& poly_cache_source, PolyInputCache3D& poly_cache_target)
	{
		shared_ptr<carve::input::PolyhedronData> poly_data_source = poly_cache_source.m_poly_data;

		std::vector<int>& faceIndices = poly_data_source->faceIndices;
		if( faceIndices.size() == 0 )
		{
			return true;
		}

		size_t numPointsAll = poly_data_source->points.size();
		if( numPointsAll < 2 )
		{
			return true;
		}
		bool inputCorrect = true;
		for( size_t iiFace = 0; iiFace < faceIndices.size(); )
		{
			int numPoints = faceIndices[iiFace];
			int numPointsIdx = iiFace;

			if( iiFace + numPoints >= faceIndices.size() )
			{
				// skip face
				break;
			}

			std::vector<int> pointIdxCurrentFace;
			for( size_t iiPoint = 1; iiPoint <= numPoints; ++iiPoint )
			{
				int idx = faceIndices[iiFace + iiPoint];

				carve::geom3d::Vector point = poly_data_source->points[idx];
				int idxTarget = poly_cache_target.addPoint(point);
				pointIdxCurrentFace.push_back(idxTarget);
			}

			poly_cache_target.m_poly_data->addFace(pointIdxCurrentFace.rbegin(), pointIdxCurrentFace.rend());

			iiFace += numPoints + 1;

			if( iiFace > faceIndices.size() )
			{
				inputCorrect = false;
				break;
			}
			if( iiFace == faceIndices.size() )
			{
				break;
			}
		}

		return inputCorrect;
	}

	static void resolveOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, double eps, bool dumpPolygons)
	{
		if( !meshset )
		{
			return;
		}
		size_t numMeshesInput = meshset->meshes.size();
		size_t numOpenEdgesInput = 0;
		size_t numClosedEdgesInput = 0;
		std::set<carve::mesh::Edge<3>* > allOpenEdges;
		for( size_t ii = 0; ii < meshset->meshes.size(); ++ii )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
			numOpenEdgesInput += mesh->open_edges.size();
			numClosedEdgesInput += mesh->closed_edges.size();

			if( mesh->faces.size() >= 6 )
			{
				for( auto edge : mesh->open_edges )
				{
					allOpenEdges.insert(edge);
				}
			}
		}

		if( numOpenEdgesInput == 0 )
		{
			return;
		}

		size_t maxNumOpenEdges = 1000;
		if( allOpenEdges.size() > maxNumOpenEdges )
		{
			return;
		}

		PolyInputCache3D polyInput( eps );
		polyhedronFromMeshSet(meshset, polyInput);
		bool meshsetChanged = false;
		

		for( size_t ii = 0; ii < allOpenEdges.size(); ++ii )
		{
			if( allOpenEdges.size() == 0 )
			{
				break;
			}

			if( ii > maxNumOpenEdges )
			{
				break;
			}

			auto itBegin = allOpenEdges.begin();
			carve::mesh::Edge<3>* openEdge = *(itBegin);

			std::set<carve::mesh::Edge<3>* > usedEdges;
			std::vector<carve::mesh::Vertex<3>* > vecResultVertexLoop;
			bool closedLoop = findEdgeLoop(openEdge, allOpenEdges, usedEdges, vecResultVertexLoop, eps, dumpPolygons );

			if( !closedLoop )
			{
				allOpenEdges.erase(itBegin);
				continue;
			}

			std::vector<carve::geom::vector<3> > vecResultPointLoop;
			for( size_t jj = 0; jj < vecResultVertexLoop.size(); ++jj )
			{
				vecResultPointLoop.push_back(vecResultVertexLoop[jj]->v);
			}

#ifdef _DEBUG
			if( dumpPolygons )
			{
				GeomDebugDump::moveOffset(meshset->getAABB().extent.y * 1.1);
				glm::vec4 color(0, 0.5, 1, 1);
				GeomDebugDump::dumpPolyline(vecResultPointLoop, color, true);
			}
#endif

			vec3 pos = GeomUtils::computePolygonCentroid(vecResultPointLoop);
			vec3 normal = GeomUtils::computePolygonNormal(vecResultPointLoop);
			if( std::abs(normal.length2() - 1.0) > 0.01 )
			{
				allOpenEdges.erase(itBegin);
				continue;
			}

			GeomUtils::Plane facePlane(glm::dvec3(pos.x, pos.y, pos.z), glm::dvec3(normal.x, normal.y, normal.z));
			std::vector<int> pointIndicesInPlane;
			std::vector<carve::mesh::Vertex<3>* > vecLoopInPlane;

			// reverse, because the new edges need to be the reverse edges of the open edges
			std::reverse(vecResultVertexLoop.begin(), vecResultVertexLoop.end());

			if( vecResultVertexLoop.size() > 2 )
			{
				vec3 first = vecResultVertexLoop.front()->v;
				vec3 last = vecResultVertexLoop.back()->v;
				vec3 delt = last - first;
				if( delt.length2() < eps * eps * 10.0 )
				{
					vecResultVertexLoop.pop_back();
				}
			}

			for( size_t jj = 0; jj < vecResultVertexLoop.size(); ++jj )
			{
				carve::mesh::Vertex<3>* vertex = vecResultVertexLoop[jj];
				vec3& edgeEndPoint = vertex->v;

				// distance point to plane
				double dist = facePlane.distancePointPlane(glm::dvec3(edgeEndPoint.x, edgeEndPoint.y, edgeEndPoint.z));
				if( dist < carve::CARVE_EPSILON * 10 )
				{
					int idx = polyInput.addPoint(edgeEndPoint);
					pointIndicesInPlane.push_back(idx);
					vecLoopInPlane.push_back(vertex);
				}
			}

			if( pointIndicesInPlane.size() == vecResultVertexLoop.size() )
			{
				polyInput.m_poly_data->addFace(pointIndicesInPlane.begin(), pointIndicesInPlane.end());
				meshsetChanged = true;

				for( auto edgeInLoop : usedEdges )
				{
					auto itFind = allOpenEdges.find(edgeInLoop);
					if( itFind != allOpenEdges.end() )
					{
						allOpenEdges.erase(itFind);
					}
				}
			}
		}

		if( meshsetChanged )
		{
			bool polyInputCorrect = checkPolyhedronData(polyInput.m_poly_data);
			if( !polyInputCorrect )
			{
				fixPolyhedronData(polyInput.m_poly_data);
				polyInputCorrect = checkPolyhedronData(polyInput.m_poly_data);
			}

			if( polyInputCorrect )
			{
				shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts()));

				size_t numOpenEdges = 0;
				size_t numClosedEdges = 0;
				for( size_t ii = 0; ii < meshsetNew->meshes.size(); ++ii )
				{
					carve::mesh::Mesh<3>* mesh = meshsetNew->meshes[ii];
					mesh->recalc();
					numOpenEdges += mesh->open_edges.size();
					numClosedEdges += mesh->closed_edges.size();
				}

#ifdef _DEBUG
				if( dumpPolygons )
				{
					GeomDebugDump::moveOffset(0.4);
					GeomDebugDump::moveOffset(meshset->getAABB().extent.y * 1.1);
					glm::vec4 color(0, 1, 1, 1);
					GeomDebugDump::dumpMeshset(meshsetNew, color, true);
				}
#endif

				size_t numMeshes = meshsetNew->meshes.size();
				if( numMeshes <= numMeshesInput )
				{
					if( numClosedEdges >= numClosedEdgesInput )
					{
						if( numOpenEdges < numOpenEdgesInput )
						{
							meshset = meshsetNew;
						}
					}
				}
			}
		}
	}
}


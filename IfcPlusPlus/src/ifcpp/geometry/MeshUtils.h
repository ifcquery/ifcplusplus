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
#include "GeomUtils.h"
#include "GeometryInputData.h"
#include "GeomDebugDump.h"
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <earcut/include/mapbox/earcut.hpp>
#include <manifold/src/manifold/include/manifold.h>

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
	MeshSetInfo(StatusCallback* _report_callback, BuildingEntity* _entity )
	{
		report_callback = _report_callback;
		entity = _entity;
	}
	
	size_t numClosedEdges = 0;
	size_t numOpenEdges = 0;
	size_t numZeroAreaFaces = 0;
	size_t numFaces = 0;
	double surfaceArea = 0;
	bool allPointersValid = true;
	bool meshSetValid = false;
	std::string details;
	StatusCallback* report_callback = nullptr;
	BuildingEntity* entity = nullptr;

	void resetInfoResult()
	{
		allPointersValid = true;
		numClosedEdges = 0;
		numOpenEdges = 0;
		numZeroAreaFaces = 0;
		numFaces = 0;
		surfaceArea = 0;
		meshSetValid = false;
		details = "";
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
	inline double getMeshVolume( const carve::mesh::MeshSet<3>* meshset )
	{
		double volume = 0;
		for( size_t kk = 0; kk < meshset->meshes.size(); ++kk )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			double meshVolume = mesh->volume();

			if( meshVolume < 0 )
			{
				mesh->invert();
				meshVolume = -meshVolume;
			}
			volume += meshVolume;
		}
		return volume;
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

	inline void recalcMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, double CARVE_EPSILON)
	{
		for( auto mesh : meshset->meshes )
		{
			mesh->cacheEdges();
			mesh->recalc(CARVE_EPSILON);
		}
	}

	inline bool checkMeshSetVolume( const shared_ptr<carve::mesh::MeshSet<3> >& mesh_set, StatusCallback* report_callback, BuildingEntity* entity, double CARVE_EPSILON )
	{
		if( !mesh_set )
		{
#ifdef _DEBUG
			if( entity )
			{
				//std::cout << "MeshSet of entity #" << entity->m_tag << " not valid" << std::endl;
			}
#endif
			return false;
		}
		if( mesh_set->meshes.size() == 0 )
		{
#ifdef _DEBUG
			if( entity )
			{
				//std::cout << "MeshSet of entity #" << entity->m_tag << " has no meshes" << std::endl;
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
					mesh->recalc(CARVE_EPSILON);
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
		if( face->edge == nullptr )
		{
			return 0;
		}

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
						face_area += std::abs(area);
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

	inline bool checkEdgePointers(carve::mesh::Edge<3>* e, bool checkForDegenerateEdges, MeshSetInfo& info)
	{
		if( !e )
		{
			info.details = "edge is nullptr";
			return false;
		}

		if( !e->rev )
		{
			info.details = "edge->rev is nullptr";
			return false;
		}


		if( !e->prev )
		{
			info.details = "edge->prev is nullptr";
			return false;
		}
		if( !e->next )
		{
			info.details = "edge->next is nullptr";
			return false;
		}
		if( !e->vert )
		{
			info.details = "edge->vert is nullptr";
			return false;
		}
		if( !e->face )
		{
			info.details = "edge->face is nullptr";
			return false;
		}

		if( checkForDegenerateEdges )
		{
			if( e->rev->rev != e )
			{
				info.details = "edge->rev->rev != edge";
				return false;
			}

			if( e->prev->vert == e->next->vert )
			{
				info.details = "e->prev->vert == e->next->vert";
				return false;
			}

			if( e->next == e->rev )
			{
				if( e->next->next == e )
				{
					// 2 edges referencing itself
					info.details = "e->next == e->rev";
					return false;
				}
				return false;
			}
		}

		return true;
	}

	inline bool checkMeshFins(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, double CARVE_EPSILON)
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
										if( std::abs(face_area) > CARVE_EPSILON * 10 )
										{
#ifdef _DEBUG
											//std::cout << "opposite face is coplanar" << std::endl;
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

	inline bool checkFaceIntegrity(const carve::mesh::Face<3>* face, bool checkForDegenerateEdges, MeshSetInfo& info)
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
			info.details = "face->edge is nullptr";
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
			if( !checkEdgePointers(e, checkForDegenerateEdges, info) )
			{
				info.details = "!checkEdgePointers(e)  " + info.details;
				return false;
			}

			if( !checkEdgePointers(e->rev, checkForDegenerateEdges, info) )
			{
				info.details = "!checkEdgePointers(e->rev)  " + info.details;
				return false;
			}

			// continue
			e = e->next;
		}
		if( e != face->edge )
		{
			info.details = "e != face->edge";
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

	inline void countReferencesToVertex(const carve::mesh::Edge<3>* edge, const carve::mesh::Vertex<3>* vertex, std::set<const carve::mesh::Edge<3>* >& setEdgesReferencing)
	{
		if( !edge->face )
		{
			return;
		}

		for( size_t jj = 0; jj < edge->face->mesh->faces.size(); ++jj )
		{
			carve::mesh::Face<3>* face3 = edge->face->mesh->faces[jj];
			carve::mesh::Edge<3>* e3 = face3->edge;
			for( size_t kk = 0; kk < face3->n_edges; ++kk )
			{
				if( e3 )
				{
					//if( e3 != edge )
					{
						if( e3->vert == vertex )
						{
							setEdgesReferencing.insert(e3);
						}
					}
				}
				e3 = e3->next;
			}
		}
	}

	inline bool checkMeshPointers(const carve::mesh::Mesh<3>* mesh,  bool checkForDegenerateEdges, MeshSetInfo& info)
	{
		if( !mesh )
		{
			return false;
		}
		const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
		for( size_t j = 0; j < vec_faces.size(); ++j )
		{
			carve::mesh::Face<3>* face = vec_faces[j];
			bool face_valid = checkFaceIntegrity(face, checkForDegenerateEdges, info);

			if( !face_valid )
			{
				return false;
			}
		}

		return true;
	}

	inline bool checkMeshSetPointers(const shared_ptr<carve::mesh::MeshSet<3> >& meshset,  bool checkForDegenerateEdges, MeshSetInfo& info)
	{
		for( size_t i = 0; i < meshset->meshes.size(); ++i )
		{
			carve::mesh::Mesh<3>* mesh_i = meshset->meshes[i];
			bool mesh_correct = checkMeshPointers(mesh_i, checkForDegenerateEdges, info);
			if( !mesh_correct )
			{
				info.allPointersValid = false;
				return false;
			}
		}
		return true;
	}

	inline void removeFinEdges(const carve::mesh::Face<3>* face, size_t& numChanges, double eps)
	{
		if( !face )
		{
			return;
		}

		carve::mesh::Mesh<3>* mesh = face->mesh;
		if( mesh->open_edges.size() == 0 )
		{
			return;
		}

		carve::mesh::Edge<3>* e = face->edge;
		if( !e )
		{
			return;
		}

		const size_t n_edges = face->n_edges;
		std::set<carve::mesh::Edge<3>* > setEdgesToRemove;
		for( size_t i_edge = 0; i_edge < n_edges; ++i_edge )
		{
			carve::mesh::Edge<3>* degenerateEdge = nullptr;

			double edgeLength2 = e->length2();
			if( edgeLength2 < eps*eps )
			{
				degenerateEdge = e;
			}

			bool condition1 = e->prev->vert == e->next->vert;
			bool condition2 = e->next == e->rev;
			if( condition1 || condition2 )
			{
				// check if the vertex is used by other edges
				degenerateEdge = e;
				std::set<const carve::mesh::Edge<3>* > setEdgesReferencing;
				carve::mesh::Vertex<3>* vertex = e->vert;
				countReferencesToVertex(e, vertex, setEdgesReferencing);
				size_t numReferences = setEdgesReferencing.size();

				if( numReferences > 1 )
				{
					// other edges referencing current vertex
					degenerateEdge = nullptr;
					carve::mesh::Edge<3>* reverseEdge = e->rev;
					if( reverseEdge != nullptr )
					{
						carve::mesh::Vertex<3>* vertexReverseEdge = reverseEdge->vert;
						std::set<const carve::mesh::Edge<3>* > setEdgesReferencingReferseEdge;
						countReferencesToVertex(reverseEdge, vertexReverseEdge, setEdgesReferencingReferseEdge);
						size_t numReferencesReverse = setEdgesReferencingReferseEdge.size();

						if( numReferencesReverse == 1 )
						{
							degenerateEdge = reverseEdge;
						}
					}
				}
			}
			
			if( degenerateEdge != nullptr )
			{
				carve::mesh::Edge<3>* degenerateEdgeReverse = degenerateEdge->rev;
				if( degenerateEdgeReverse != nullptr )
				{
					auto itFindEdge = setEdgesToRemove.find(degenerateEdgeReverse);
					if( itFindEdge == setEdgesToRemove.end() )
					{
						setEdgesToRemove.insert(degenerateEdge);
					}
				}
				else
				{
					setEdgesToRemove.insert(degenerateEdge);
				}
			}
			e = e->next;
		}

		for( carve::mesh::Edge<3>* edgeRemove : setEdgesToRemove )
		{
			std::set<carve::mesh::Face<3>* > setFacesToReplaceEdgePointer;
			if( face->edge == edgeRemove )
			{
				carve::mesh::Face<3>* faceNonConst = (carve::mesh::Face<3>*)face;
				setFacesToReplaceEdgePointer.insert(faceNonConst);
			}

			for( auto faceInMesh : face->mesh->faces )
			{
				if( faceInMesh->edge == edgeRemove )
				{
					setFacesToReplaceEdgePointer.insert(faceInMesh);
				}
			}

			carve::mesh::Edge<3>* edgeRemainingNext = edgeRemove->removeEdge();
			for( auto faceReplaceEdgePointer : setFacesToReplaceEdgePointer )
			{
				if( faceReplaceEdgePointer->edge != edgeRemainingNext )
				{
					faceReplaceEdgePointer->edge = edgeRemainingNext;
				}
			}
			
			++numChanges;
		}
	}
	
	inline void removeFinEdges(carve::mesh::Mesh<3>* mesh, double CARVE_EPSILON)
	{
		if( !mesh )
		{
			return;
		}

		if( mesh->open_edges.size() == 0 )
		{
			//return;
		}

		const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
		size_t numFaces = vec_faces.size();

#ifdef _DEBUG
		if( numFaces  == 1 )
		{
			glm::vec4 color(0.4, 0.2, 0.2, 1.);
			std::vector<const carve::mesh::Face<3>* > vecFaces;
			vecFaces.push_back(vec_faces[0]);
			GeomDebugDump::moveOffset(0.3);
			GeomDebugDump::stopBuffering();
			GeomDebugDump::dumpFaces(vecFaces, color);
			GeomDebugDump::moveOffset(0.3);
			GeomDebugDump::dumpFacePolygon(vec_faces[0], color, true);
		
			//GeomDebugDump::moveOffset(0.01);
			//std::vector<vec3> vecPolyline = { e->v1()->v, e->v2()->v };
			//GeomDebugDump::dumpPolyline(vecPolyline, color, true);
		}
#endif

		for( size_t ii = 0; ii < numFaces; ++ii )
		{
			size_t numChangesAll = 0;
			for( size_t jj = 0; jj < vec_faces.size(); ++jj )
			{
				size_t numChangesCurrentFace = 0;
				carve::mesh::Face<3>* face = vec_faces[jj];
				removeFinEdges(face, numChangesCurrentFace, CARVE_EPSILON);
				numChangesAll += numChangesCurrentFace;
			}

			// several fin-edges (where edge->next == edge->reverse) can be concatenated. Repeat until there are no changes
			if( numChangesAll > 0 )
			{
				if( mesh->faces.size() < 2 )
				{
					continue;
				}

				bool checkForDegenerateEdges = false;
				MeshSetInfo minf;
				bool valid = checkMeshPointers(mesh, checkForDegenerateEdges, minf );

				if( !valid )
				{
					continue;
				}
				
				mesh->cacheEdges();
				mesh->recalc(CARVE_EPSILON);
			}

			if( numChangesAll == 0 )
			{
				break;
			}
		}
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

	static void addFaceCheckIndexes(int idxA, int idxB, int idxC, PolyInputCache3D& meshOut)
	{
		if( idxA == idxB || idxA == idxC || idxB == idxC )
		{
			return;
		}
		meshOut.m_poly_data->addFace(idxA, idxB, idxC);
	}

	static void addTriangleCheckDegenerate(int idxA, int idxB, int idxC, PolyInputCache3D& meshOut, double CARVE_EPSILON)
	{
		if( idxA == idxB || idxA == idxC || idxB == idxC )
		{
#ifdef _DEBUG
			std::cout << "skipping degenerate triangle: " << idxA << "/" << idxB << "/" << idxC << std::endl;
#endif
			return;
		}

		const carve::geom::vector<3>& pointA = meshOut.m_poly_data->getVertex(idxA);
		const carve::geom::vector<3>& pointB = meshOut.m_poly_data->getVertex(idxB);
		const carve::geom::vector<3>& pointC = meshOut.m_poly_data->getVertex(idxC);
		double lengthAB = (pointB - pointA).length2();
		if( lengthAB < CARVE_EPSILON*CARVE_EPSILON * 10 )
		{
#ifdef _DEBUG
			std::cout << "skipping degenerate triangle: " << idxA << "/" << idxB << "/" << idxC << std::endl;
#endif
			return;
		}

		double lengthAC = (pointC - pointA).length2();
		if( lengthAC < CARVE_EPSILON*CARVE_EPSILON * 10 )
		{
#ifdef _DEBUG
			std::cout << "skipping degenerate triangle: " << idxA << "/" << idxB << "/" << idxC << std::endl;
#endif
			return;
		}

		double lengthBC = (pointC - pointB).length2();
		if( lengthBC < CARVE_EPSILON*CARVE_EPSILON * 10 )
		{
#ifdef _DEBUG
			std::cout << "skipping degenerate triangle: " << idxA << "/" << idxB << "/" << idxC << std::endl;
#endif
			return;
		}

		meshOut.m_poly_data->addFace(idxA, idxB, idxC);
	}

	static void addFaceCheckIndexes(int idxA, int idxB, int idxC, int idxD, PolyInputCache3D& meshOut, double CARVE_EPSILON)
	{
		std::set<int> setIndices = { idxA, idxB, idxC, idxD };

		if( setIndices.size() == 3 )
		{
			auto it = setIndices.begin();
			idxA = *it;
			++it;
			idxB = *it;
			++it;
			idxC = *it;
			meshOut.m_poly_data->addFace(idxA, idxB, idxC);
			return;
		}

		addTriangleCheckDegenerate(idxA, idxB, idxC, meshOut, CARVE_EPSILON);
		addTriangleCheckDegenerate(idxA, idxC, idxD, meshOut, CARVE_EPSILON);
	}

	static void addFaceCheckIndexes(const vec3& v0, const vec3& v1, const vec3& v2, PolyInputCache3D& meshOut, double CARVE_EPSILON)
	{
		int idxA = meshOut.addPoint(v0);
		int idxB = meshOut.addPoint(v1);
		int idxC = meshOut.addPoint(v2);
		addTriangleCheckDegenerate(idxA, idxB, idxC, meshOut, CARVE_EPSILON);
	}

	static void addFaceCheckIndexes(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3, PolyInputCache3D& meshOut, double CARVE_EPSILON)
	{
		int idxA = meshOut.addPoint(v0);
		int idxB = meshOut.addPoint(v1);
		int idxC = meshOut.addPoint(v2);
		int idxD = meshOut.addPoint(v3);

		addTriangleCheckDegenerate(idxA, idxB, idxC, meshOut, CARVE_EPSILON);
		addTriangleCheckDegenerate(idxA, idxC, idxD, meshOut, CARVE_EPSILON);
	}

	static void getMeshVertexPoints(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::vector<glm::dvec3>& vecAllPoints)
	{
		for( size_t ii = 0; ii < meshset->vertex_storage.size(); ++ii )
		{
			carve::geom::vector<3>& carvePoint3D = meshset->vertex_storage[ii].v;
			vecAllPoints.push_back(glm::dvec3(carvePoint3D.x, carvePoint3D.y, carvePoint3D.z));
		}
	}

	static void getFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, std::set<carve::mesh::Face<3>* >& mapAllFaces)
	{
		mapAllFaces.clear();

		for( carve::mesh::Mesh<3>*mesh : meshsetInput->meshes )
		{
			for( carve::mesh::Face<3>* face : mesh->faces )
			{
				mapAllFaces.insert( face );
			}
		}
	}

	static void getAreaAndFacesMapInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, std::multimap<double, carve::mesh::Face<3>* >& mapAllFaces)
	{
		mapAllFaces.clear();

		std::set<carve::mesh::Face<3>* > setAllFaces;
		getFacesInMeshSet(meshsetInput, setAllFaces);

		for( carve::mesh::Face<3>* face : setAllFaces )
		{
			double area = MeshUtils::computeFaceArea(face);
			mapAllFaces.insert({ area, face });
		}
	}

	static void removeZeroAreaFacesInMesh(carve::mesh::Mesh<3>* mesh, double epsMinFaceArea, double CARVE_EPSILON)
	{
		size_t numFacesRemoved = 0;
		for( auto it = mesh->faces.begin(); it != mesh->faces.end(); ++it )
		{
			carve::mesh::Face<3>* face = *it;
			double area = MeshUtils::computeFaceArea(face);
			if( area < epsMinFaceArea )
			{
				it = mesh->faces.erase(it);
				++numFacesRemoved;
				if( it == mesh->faces.end() )
				{
					break;
				}
			}
		}
		if( numFacesRemoved > 0 )
		{
			mesh->cacheEdges();
			mesh->recalc(CARVE_EPSILON);
		}
	}

	static void removeZeroAreaFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, double epsMinFaceArea, double CARVE_EPSILON)
	{
		for( carve::mesh::Mesh<3>*mesh : meshsetInput->meshes )
		{
			removeZeroAreaFacesInMesh(mesh, epsMinFaceArea, CARVE_EPSILON);
		}
	}

	static bool checkMeshNonNegativeAndClosed( carve::mesh::Mesh<3>* mesh_i, double CARVE_EPSILON)
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
					mesh_i->recalc(CARVE_EPSILON);
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

	static bool checkMeshSetNonNegativeAndClosed(const shared_ptr<carve::mesh::MeshSet<3>> mesh_set, double CARVE_EPSILON)
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
						mesh_i->recalc(CARVE_EPSILON);
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
				std::vector<const carve::mesh::Face<3>* > vecFaces = { face };
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

	static bool checkMeshSetValidAndClosed(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, double CARVE_EPSILON )
	{
		info.resetInfoResult();
		if( !meshset )
		{
#ifdef _DEBUG
			if( info.entity )
			{
				std::cout << "MeshSet of entity #" << info.entity->m_tag << " not valid" << std::endl;
			}
#endif
			info.allPointersValid = false;
			info.meshSetValid = false;
			return false;
		}
		if( meshset->meshes.size() == 0 )
		{
#ifdef _DEBUG
			if( info.entity )
			{
				//std::cout << "MeshSet of entity #" << info.entity->m_tag << " has no meshes" << std::endl;
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
				if( std::abs(face_area) < CARVE_EPSILON )
				{
					++info.numZeroAreaFaces;
				}
				info.surfaceArea += face_area;
			}
		}

		// check for valid pointers first
		bool checkForDegenerateEdges = true;
		if( !checkMeshSetPointers(meshset, checkForDegenerateEdges, info) )
		{
			return false;
		}

		if( !checkMeshFins(meshset, CARVE_EPSILON) )
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
		bool meshes_closed = checkMeshSetNonNegativeAndClosed(meshset, CARVE_EPSILON);
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
						mesh->recalc(CARVE_EPSILON);
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
			if( info.report_callback )
			{
				info.report_callback->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, info.entity);
			}
#endif
			info.meshSetValid = false;
			return false;
		}

		info.meshSetValid = true;
		return true;
	}

	inline void fixMeshset(carve::mesh::MeshSet<3>* meshset, double CARVE_EPSILON, bool shouldBeClosedManifold, bool dumpMeshes)
	{
		for( carve::mesh::Mesh<3>* mesh : meshset->meshes )
		{
			removeFinEdges(mesh, CARVE_EPSILON);
		}

		for( auto it = meshset->meshes.begin(); it != meshset->meshes.end(); ++it )
		{
			carve::mesh::Mesh<3>* mesh = *it;

			bool meshChanged = false;
			for( auto itFaces = mesh->faces.begin(); itFaces != mesh->faces.end();  )
			{
				carve::mesh::Face<3>* face = *itFaces;
				if( face == nullptr )
				{
					itFaces = mesh->faces.erase(itFaces);
					delete face;
					meshChanged = true;
					continue;
				}

				if( !face->edge )
				{
					itFaces = mesh->faces.erase(itFaces);
					delete face;
					meshChanged = true;
					continue;
				}

				++itFaces;
			}

			if( meshChanged )
			{
				mesh->cacheEdges();
				mesh->recalc(CARVE_EPSILON);
			}

			bool checkForDegenerateEdges = false;
			MeshSetInfo minf;
			bool valid = checkMeshPointers(mesh, checkForDegenerateEdges, minf );

			bool removeMesh = false;
			if( !valid )
			{
				removeMesh = true;
			}

			if( shouldBeClosedManifold )
			{
				if( mesh->faces.size() < 4 )
				{
					removeMesh = true;
				}

				if( !mesh->isClosed() )
				{
					removeMesh = true;
				}
			}

			if( removeMesh )
			{
				meshset->meshes.erase(it);
				delete mesh;

				it = meshset->meshes.begin();
				if( it == meshset->meshes.end() )
				{
					break;
				}
			}
		}
	}

	static bool checkMeshSetValidAndClosed(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, double eps, bool tryToFixIfNotValid, bool dumpMeshes)
	{
		bool mesh_ok = checkMeshSetValidAndClosed(meshset, info, eps);
		if( tryToFixIfNotValid )
		{
			MeshUtils::fixMeshset(meshset.get(), eps, true, dumpMeshes);
			mesh_ok = checkMeshSetValidAndClosed(meshset, info, eps);
		}
		return mesh_ok;
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
			else if (sN > sD)  // sc > 1  => the s=1 edge is visible
			{    
				sN = sD;       // compute shortest connection of B to segment CD
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

			intersectionPoint = P1;
			return true;
		}
		return false;
	}

	///\brief method createTriangulated3DFace: Creates a triangulated face
	///\param[in] inputBounds3D: Curves as face boundaries. The first input curve is the outer boundary, succeeding curves are inner boundaries
	///\param[in] ifc_entity: Ifc entity that the geometry belongs to, just for error messages. Pass a nullptr if no entity at hand.
	///\param[out] meshOut: Result mesh
	static void createTriangulated3DFace(const std::vector<std::vector<vec3> >& inputBounds3D, PolyInputCache3D& meshOut, GeomProcessingParams& params )
	{
		double CARVE_EPSILON = params.epsMergePoints;
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
				addFaceCheckIndexes(v0, v1, v2, meshOut, CARVE_EPSILON);

#ifdef _DEBUG
				if( params.debugDump )
				{
					glm::vec4 color(0, 1, 1, 1);
					PolyInputCache3D poly( CARVE_EPSILON );
					int idxA = poly.addPoint(v0);
					int idxB = poly.addPoint(v1);
					int idxC = poly.addPoint(v2);
					poly.m_poly_data->addFace(idxA, idxB, idxC);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(carve::input::opts(), CARVE_EPSILON));
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

				addFaceCheckIndexes(v0, v1, v2, v3, meshOut, CARVE_EPSILON);

				

#ifdef _DEBUG
				if( params.debugDump )
				{
					glm::vec4 color(0, 1, 1, 1);
					PolyInputCache3D poly( CARVE_EPSILON );
					int idxA = poly.addPoint(v0);
					int idxB = poly.addPoint(v1);
					int idxC = poly.addPoint(v2);
					int idxD = poly.addPoint(v3);
					poly.m_poly_data->addFace(idxA, idxB, idxC, idxD);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(carve::input::opts(), CARVE_EPSILON));
					GeomDebugDump::dumpMeshset(meshset, color, false);
				}
#endif

				return;
			}
		}

#ifdef _DEBUG
		PolyInputCache3D poly(CARVE_EPSILON);
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
			GeomUtils::simplifyPolygon(loopPoints3Dinput, params.epsMergePoints, params.epsMergeAlignedEdgesAngle);
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
					if( params.callbackFunc )
					{
						params.callbackFunc->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
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
			if( params.callbackFunc )
			{
				params.callbackFunc->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, params.ifc_entity);
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
			if( errorOccured /*|| ifc_entity->m_tag == 64*/ )
			{
				glm::vec4 color(0, 1, 1, 1);
				GeomDebugDump::dumpPolyline(polygons2d, color, true);
				//shared_ptr<carve::mesh::MeshSet<3> > meshset(meshOut.m_poly_data->createMesh(carve::input::opts()));
				//GeomDebugDump::dumpMeshset(meshset, color, true, true);
			}
#endif
		}

#ifdef _DEBUG
		if( errorOccured || params.debugDump)
		{
			glm::vec4 color(0, 1, 1, 1);
			shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(carve::input::opts(), CARVE_EPSILON));
			GeomDebugDump::dumpMeshset(meshset, color, false);
		}
#endif
	}

	///\brief method intersectOpenEdges: Intersect open edges of MeshSet with closed edges, and split the open edges in case of intersection
	///\param[in/out] meshset: MeshSet with open edges. If fix is found, a new MeshSet is assigned to the smart pointer
	///\param[in] eps: tolerance to find edge-edge intersections
	///\param[in] dumpMeshes: write meshes to dump file for debugging
	inline void intersectOpenEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, GeomProcessingParams& params)
	{
		if( !meshset )
		{
			return;
		}

		size_t maxNumFaces = 600;
		size_t maxNumEdges = 600;
		size_t maxNumOpenEdges = 100;
		double CARVE_EPSILON = params.epsMergePoints;

#ifdef _DEBUG
		glm::vec4 color(0.5, 0.6, 0.7, 1.0);
		if( params.debugDump )
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

			PolyInputCache3D polyInput(params.epsMergePoints);

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
					if( params.debugDump )
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
						bool intersect = edgeToEdgeIntersect(openEdge, edge, params.epsMergePoints, intersectionPoint);

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

					if( faceIndexes.size() > 2 && faceIndexes.size() < 500 )
					{
						size_t n = std::distance(faceIndexes.begin(), faceIndexes.end());
						
						size_t numFaceIndices = polyInput.m_poly_data->faceIndices.size();
						if( numFaceIndices > 1000 )
						{
							//std::cout << numFaceIndices << " ";
						}
						polyInput.m_poly_data->addFace(faceIndexes.begin(), faceIndexes.end());
					}
				}
				else
				{
					std::vector<std::vector<vec3> > faceLoops = { faceLoop };
					bool mergeAlignedEdges = false;
					bool dumpPolygon = true;
					createTriangulated3DFace(faceLoops, polyInput, params );
				}
			}

			if( numSplitEdges > 0 )
			{
				bool correct = checkPolyhedronData(polyInput.m_poly_data, params.minFaceArea);
				if( !correct )
				{
					bool correct = fixPolyhedronData(polyInput.m_poly_data, params.minFaceArea);
#ifdef _DEBUG
					std::cout << "incorrect idx";
#endif
					return;
				}

				shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts(), CARVE_EPSILON));
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
					if( params.debugDump )
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

	static std::shared_ptr<carve::mesh::MeshSet<3> > createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, double CARVE_EPSILON)
	{
		carve::input::PolyhedronData polyhedron_data;
		polyhedron_data.addVertex(p0);
		polyhedron_data.addVertex(p1);
		polyhedron_data.addVertex(p2);
		polyhedron_data.addFace(0, 1, 2);
		std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), CARVE_EPSILON));
		return mesh;
	}

	static std::shared_ptr<carve::mesh::MeshSet<3> > createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, vec3& p3, double CARVE_EPSILON)
	{
		carve::input::PolyhedronData polyhedron_data;
		polyhedron_data.addVertex(p0);
		polyhedron_data.addVertex(p1);
		polyhedron_data.addVertex(p2);
		polyhedron_data.addVertex(p3);

		polyhedron_data.addFace(0, 1, 2, 3);
		//polyhedron_data.addFace(2, 3, 0);
		std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), CARVE_EPSILON));
		return mesh;
	}

	static std::shared_ptr<carve::mesh::MeshSet<3> > createBoxMesh(vec3& pos, vec3& extent, carve::math::Matrix& transform, double CARVE_EPSILON)
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

		std::shared_ptr<carve::mesh::MeshSet<3> > mesh(polyhedron_data.createMesh(carve::input::opts(), CARVE_EPSILON));
		return mesh;
	}

	inline void mesh2MeshSet(const carve::mesh::Mesh<3>* mesh, shared_ptr<carve::mesh::MeshSet<3> >& meshsetResult, double CARVE_EPSILON)
	{
		if( !mesh )
		{
			return;
		}

		size_t maxNumFaces = 600;
		size_t maxNumEdges = 600;
		size_t maxNumOpenEdges = 100;

		size_t numClosedEdgesBefore = mesh->closed_edges.size();



		PolyInputCache3D polyInput(CARVE_EPSILON);

		// intersect with closed edges
		size_t numSplitEdges = 0;
		for( size_t iiFace = 0; iiFace < mesh->faces.size(); ++iiFace )
		{
			if( iiFace > maxNumFaces )
			{
				continue;
			}
			carve::mesh::Face<3>* face = mesh->faces[iiFace];

			std::vector<carve::mesh::Vertex<3>* > faceVertices;

			face->getVertices(faceVertices);

			std::vector<int> vecIndexes;
			size_t n = faceVertices.size();
			carve::mesh::Vertex<3>* vertexFirst = faceVertices[0];

			for( int i = 0; i < n; ++i )
			{
				carve::mesh::Vertex<3>* v0 = faceVertices[i];

				vec3 point = v0->v;
				int idx = polyInput.addPoint(point);
				vecIndexes.push_back(idx);
			}

			polyInput.m_poly_data->addFace(vecIndexes.begin(), vecIndexes.end());


		}
		shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts(), CARVE_EPSILON));
		meshsetResult = meshsetNew;
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

	static void resolveOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, double CARVE_EPSILON, bool dumpPolygons)
	{
		if( !meshset )
		{
			return;
		}
		size_t numMeshesInput = meshset->meshes.size();
		size_t numFacesInput = 0;
		size_t numOpenEdgesInput = 0;
		size_t numClosedEdgesInput = 0;
		std::set<carve::mesh::Edge<3>* > allOpenEdges;
		for( size_t ii = 0; ii < meshset->meshes.size(); ++ii )
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[ii];
			numOpenEdgesInput += mesh->open_edges.size();
			numClosedEdgesInput += mesh->closed_edges.size();
			numFacesInput += mesh->faces.size();
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

		size_t maxNumFaces = 5000;
		if( numFacesInput > maxNumFaces )
		{
			return;
		}

		size_t maxNumOpenEdges = 1000;
		if( allOpenEdges.size() > maxNumOpenEdges )
		{
			return;
		}

		PolyInputCache3D polyInput( CARVE_EPSILON );
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
			bool closedLoop = findEdgeLoop(openEdge, allOpenEdges, usedEdges, vecResultVertexLoop, CARVE_EPSILON, dumpPolygons );

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
				if( delt.length2() < CARVE_EPSILON * CARVE_EPSILON * 10.0 )
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
				if( dist < CARVE_EPSILON * 10 )
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
			GeomProcessingParams params(CARVE_EPSILON, CARVE_EPSILON*0.1, CARVE_EPSILON * 0.01);
			bool polyInputCorrect = checkPolyhedronData(polyInput.m_poly_data, params.minFaceArea);
			if( !polyInputCorrect )
			{
				fixPolyhedronData(polyInput.m_poly_data, params.minFaceArea);
				polyInputCorrect = checkPolyhedronData(polyInput.m_poly_data, params.minFaceArea);
			}

			if( polyInputCorrect )
			{
				shared_ptr<carve::mesh::MeshSet<3> > meshsetNew(polyInput.m_poly_data->createMesh(carve::input::opts(), CARVE_EPSILON));

				size_t numOpenEdges = 0;
				size_t numClosedEdges = 0;
				for( size_t ii = 0; ii < meshsetNew->meshes.size(); ++ii )
				{
					carve::mesh::Mesh<3>* mesh = meshsetNew->meshes[ii];
					mesh->recalc(CARVE_EPSILON);
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

	static void getLargestClosedMesh(const shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, shared_ptr<carve::mesh::MeshSet<3> >& meshsetResult, double CARVE_EPSILON )
	{
		std::map<int, carve::mesh::Mesh<3>* > mapClosedMeshes;
		if( meshsetInput )
		{
			for( auto mesh : meshsetInput->meshes )
			{
				if( mesh->isClosed() )
				{
					int numFaces = mesh->faces.size();
					mapClosedMeshes.insert({ numFaces, mesh });
				}

			}
		}
		if( mapClosedMeshes.size() > 0 )
		{
			carve::mesh::Mesh<3>* mesh = mapClosedMeshes.rbegin()->second;

			MeshUtils::mesh2MeshSet(mesh, meshsetResult, CARVE_EPSILON);
		}
	}

	static void carve2Manifold( shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, shared_ptr<manifold::Mesh>& manimesh, double eps)
	{
		PolyInputCache3D polyhedron(eps);
		size_t pointCount = 0;

		for( size_t ii = 0; ii < meshsetInput->meshes.size(); ++ii )
		{
			carve::mesh::Mesh<3>* mesh = meshsetInput->meshes[ii];
			const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
			for( size_t jj = 0; jj < vec_faces.size(); ++jj )
			{
				carve::mesh::Face<3>* face = vec_faces[jj];

				const size_t n_edges = face->n_edges;
				if( n_edges < 3 )
				{
					continue;
				}

				if( n_edges == 3 )
				{
					carve::mesh::Edge<3>* e = face->edge;
					vec3& v0 = e->vert->v;
					e = e->next;
					vec3& v1 = e->vert->v;
					e = e->next;
					vec3& v2 = e->vert->v;

					if( GeomUtils::length2(v1,v0) < eps )
					{
						// skip current face, and merge point0 and point 1
						v0 = v1;
						continue;
					}

					if( GeomUtils::length2(v1,v2) < eps )
					{
						// skip current face, and merge point0 and point 1
						v2 = v1;
						continue;
					}

					if( GeomUtils::length2(v2,v0) < eps )
					{
						// skip current face, and merge point0 and point 1
						v0 = v2;
						continue;
					}

					MeshUtils::addFaceCheckIndexes(v0, v1, v2, polyhedron, eps);
					continue;
				}

				if( n_edges >= 4 )
				{
					std::cout << "MeshSet not triangulated: " << n_edges << " edges" << std::endl;
				}
			}
		}

		double minFaceArea = eps * 0.001;
		bool polyhedron_ok = checkPolyhedronData(polyhedron.m_poly_data, minFaceArea);
		if( !polyhedron_ok )
		{
			fixPolyhedronData(polyhedron.m_poly_data, minFaceArea);
		}

		if( !manimesh )
		{
			manimesh = shared_ptr<manifold::Mesh>(new manifold::Mesh());
		}

		for( int ii = 0; ii < polyhedron.m_poly_data->points.size(); ++ii )
		{
			const vec3& v0 = polyhedron.m_poly_data->points[ii];
			manimesh->vertPos.emplace_back(v0.x, v0.y, v0.z);
		}

		for( int ii = 0; ii < polyhedron.m_poly_data->faceIndices.size(); ++ii )
		{
			size_t numPointsInFace = polyhedron.m_poly_data->faceIndices[ii];
			if( numPointsInFace == 3 )
			{
				if( polyhedron.m_poly_data->faceIndices.size() > ii + 3 )
				{
					size_t idx0 = polyhedron.m_poly_data->faceIndices[ii + 1];
					size_t idx1 = polyhedron.m_poly_data->faceIndices[ii + 2];
					size_t idx2 = polyhedron.m_poly_data->faceIndices[ii + 3];

					if( idx0 == idx1 )
					{
						continue;
					}
					if( idx1 == idx2 )
					{
						continue;
					}
					if( idx2 == idx0 )
					{
						continue;
					}
					manimesh->triVerts.emplace_back(idx0, idx1, idx2);
					ii += 3;
					continue;
				}
			}

#ifdef _DEBUG
			std::cout << "check polyhedron.m_poly_data[" << ii << "]" << std::endl;
#endif
		}
	}

	static void manifold2carve( const shared_ptr<manifold::Manifold>& manifoldInput, shared_ptr<carve::mesh::MeshSet<3> >& result, double CARVE_EPSILON)
	{
		if( !manifoldInput )
		{
			return;
		}
		PolyInputCache3D poly( CARVE_EPSILON );
		manifold::Mesh resultMesh = manifoldInput->GetMesh();

		for (int i = 0, n = resultMesh.triVerts.size(); i < n; i++)
		{
			int triIdx0 = resultMesh.triVerts[i][0];
			int triIdx1 = resultMesh.triVerts[i][1];
			int triIdx2 = resultMesh.triVerts[i][2];

			if( triIdx0 > resultMesh.vertPos.size() )
			{
				continue;
			}

			if( triIdx1 > resultMesh.vertPos.size() )
			{
				continue;
			}

			if( triIdx2 > resultMesh.vertPos.size() )
			{
				continue;
			}
			glm::vec3 p0 = resultMesh.vertPos[triIdx0];
			glm::vec3 p1 = resultMesh.vertPos[triIdx1];
			glm::vec3 p2 = resultMesh.vertPos[triIdx2];
			carve::geom::vector<3> v0 = carve::geom::VECTOR(p0.x, p0.y, p0.z);
			carve::geom::vector<3> v1 = carve::geom::VECTOR(p1.x, p1.y, p1.z);
			carve::geom::vector<3> v2 = carve::geom::VECTOR(p2.x, p2.y, p2.z);

			MeshUtils::addFaceCheckIndexes(v0, v1, v2, poly, CARVE_EPSILON);
		}
		result = shared_ptr<carve::mesh::MeshSet<3> >(poly.m_poly_data->createMesh(carve::input::opts(), CARVE_EPSILON));
	}

	static void boundingBox2Mesh(carve::geom::aabb<3>& bbox, shared_ptr<carve::mesh::MeshSet<3> >& meshset, double CARVE_EPSILON)
	{
		double minX = bbox.pos.x - bbox.extent.x;
		double maxX = bbox.pos.x + bbox.extent.x;
		double minY = bbox.pos.y - bbox.extent.y;
		double maxY = bbox.pos.y + bbox.extent.y;
		double minZ = bbox.pos.z - bbox.extent.z;
		double maxZ = bbox.pos.z + bbox.extent.z;

		shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );
		polyhedron_data->addVertex( carve::geom::VECTOR( maxX, maxY, maxZ ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( minX, maxY, maxZ ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( minX, minY, maxZ ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( maxX, minY, maxZ ) );

		polyhedron_data->addVertex( carve::geom::VECTOR( maxX, maxY, minZ ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( minX, maxY, minZ ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( minX, minY, minZ ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( maxX, minY, minZ ) );

		polyhedron_data->addFace( 0, 1, 2 );
		polyhedron_data->addFace( 2, 3, 0 );

		polyhedron_data->addFace( 7, 6, 5 );
		polyhedron_data->addFace( 5, 4, 7 );

		polyhedron_data->addFace( 0, 4, 5 );
		polyhedron_data->addFace( 5, 1, 0 );
		polyhedron_data->addFace( 1, 5, 6 );
		polyhedron_data->addFace( 6, 2, 1 );
		polyhedron_data->addFace( 2, 6, 7 );
		polyhedron_data->addFace( 7, 3, 2 );
		polyhedron_data->addFace( 3, 7, 4 );
		polyhedron_data->addFace( 4, 0, 3 );

		meshset = shared_ptr<carve::mesh::MeshSet<3> >(polyhedron_data->createMesh(carve::input::opts(), CARVE_EPSILON));
	}
}

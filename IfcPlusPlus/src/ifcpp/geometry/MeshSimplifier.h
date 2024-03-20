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

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/geometry/GeomUtils.h>
#include <ifcpp/geometry/GeometryInputData.h>


class MeshSimplifier
{
public:
	/**
	* @brief simplifyMeshSet merge coplanar faces and re-triangulate each set of merged faces
	* @param meshset				Carve meshset
	* @param report_callback		callback function for errors, warnings, notifications, progress
	* @param entity				IFC entity that is currently being processed
	* @param ignoreOpenEdgesInResult	If true, the result is kept even with open edges (good for visualization). If false, the result will be the input mesh in case open edges occur after triangulation (good for further boolean operations)
	*/
	static void simplifyMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& paramsInput);


	static size_t mergeAlignedEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, GeomProcessingParams& params);

	static size_t removePointerToVertex(carve::mesh::Mesh<3>* mesh, carve::mesh::Vertex<3>* vertRemove, carve::mesh::Vertex<3>* vertReplace)
	{
		size_t numChanges = 0;
		for (carve::mesh::Face<3>*face : mesh->faces)
		{
			if (!face)
			{
				continue;
			}

			carve::mesh::Edge<3>* edge = face->edge;
			size_t numEdgesCurrentFace = face->n_edges;
			for (size_t ii = 0; ii < numEdgesCurrentFace; ++ii)
			{
				if (!edge)
				{
					continue;
				}
				if (edge->vert == vertRemove)
				{
					edge->vert = vertReplace;
					++numChanges;
				}
				edge = edge->next;
			}
		}
		return numChanges;
	}

	static void getPointerToEdge(carve::mesh::Mesh<3>* mesh, carve::mesh::Edge<3>* edgeFind, std::set<carve::mesh::Edge<3>* >& setEdges, std::set<carve::mesh::Face<3>* >& setFaces)
	{
		size_t numEdges = 0;
		for (carve::mesh::Face<3>*face : mesh->faces)
		{
			if (!face)
			{
				continue;
			}

			carve::mesh::Edge<3>* edge = face->edge;
			if (edge == edgeFind)
			{
				setFaces.insert(face);
			}

			size_t numEdgesCurrentFace = face->n_edges;
			for (size_t ii = 0; ii < numEdgesCurrentFace; ++ii)
			{
				if (!edge)
				{
					continue;
				}
				if (edge->next == edgeFind)
				{
					setEdges.insert(edge);
				}
				if (edge->prev == edgeFind)
				{
					setEdges.insert(edge);
				}
				if (edge->rev == edgeFind)
				{
					setEdges.insert(edge);
				}
				edge = edge->next;
			}
		}
	}

	static void getEdgesOnVertex(carve::mesh::Mesh<3>* mesh, carve::mesh::Vertex<3>* vertex, std::set<carve::mesh::Edge<3>* >& setEdges)
	{
		size_t numEdges = 0;
		for (carve::mesh::Face<3>*face : mesh->faces)
		{
			if (!face)
			{
				continue;
			}

			carve::mesh::Edge<3>* edge = face->edge;
			size_t numEdgesCurrentFace = face->n_edges;
			for (size_t ii = 0; ii < numEdgesCurrentFace; ++ii)
			{
				if (!edge)
				{
					continue;
				}
				if (edge->vert == vertex)
				{
					setEdges.insert(edge);
				}
				edge = edge->next;
			}
		}
	}

	static size_t mergeCoplanarFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& paramsInput, bool shouldBeClosedManifold);

	static size_t removeEdgeAndMergeFaces(carve::mesh::Edge<3>* edgeIn, const GeomProcessingParams& params);


	static size_t removeFaceFromMesh(carve::mesh::Face<3>* fx)
	{
		carve::mesh::Mesh<3>* mesh = fx->mesh;
		for (auto it = mesh->faces.begin(); it != mesh->faces.end(); ++it)
		{
			carve::mesh::Face<3>* f = *it;
			if (f == fx)
			{
				mesh->faces.erase(it);
				return 1;
			}
		}
		return 0;
	}


	static size_t countEdges(carve::mesh::Face<3>* face)
	{
		if (!face)
		{
			return 0;
		}
		size_t numEdges = 0;
		carve::mesh::Edge<3>* edge = face->edge;
		for (size_t ii = 0; ii < MAX_NUM_EDGES; ++ii)
		{
			++numEdges;
			edge = edge->next;
			if (edge == face->edge)
			{
				break;
			}
		}

		face->n_edges = numEdges;
		return numEdges;
	}


	static carve::mesh::Edge<3>* checkMergeFaces(carve::mesh::Edge<3>* e, const GeomProcessingParams& params);

	static void linkEdges(carve::mesh::Edge<3>* a, carve::mesh::Edge<3>* b)
	{
		a->next = b;
		b->prev = a;
	}

	static void setFacePointerToEdgeLoop(carve::mesh::Edge<3>* inputEdge, carve::mesh::Face<3>* f)
	{
		carve::mesh::Edge<3>* currentEdge = inputEdge;
		for( size_t ii = 0; ii < MAX_NUM_EDGES; ++ii)
		{
			currentEdge->face = f;
			currentEdge = currentEdge->next;
			if (currentEdge == inputEdge)
			{
				break;
			}
		}
	}

	static void removeDegenerateFacesInMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& paramsInput);

	static void removeFinFaces(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params);

	static void removeFinEdges(carve::mesh::Mesh<3>* mesh, const GeomProcessingParams& params);

	static void removeFinEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params)
	{
		for (carve::mesh::Mesh<3>*mesh : meshset->meshes)
		{
			removeFinEdges(mesh, params);
		}
	}

	static void removeFinEdgesFromFace(const carve::mesh::Face<3>* face, size_t& numChanges, double eps)
	{
		return;  // degenerate edge = degenerate face, so remove degenerate faces instead

		if (!face)
		{
			return;
		}

		carve::mesh::Mesh<3>* mesh = face->mesh;
		if (mesh->open_edges.size() == 0)
		{
			//return;
		}

		carve::mesh::Edge<3>* e = face->edge;
		if (!e)
		{
			return;
		}

		const size_t n_edges = face->n_edges;
		std::set<carve::mesh::Edge<3>* > setEdgesToRemove;
		for (size_t i_edge = 0; i_edge < n_edges; ++i_edge)
		{
			carve::mesh::Edge<3>* degenerateEdge = nullptr;

			double edgeLength2 = e->length2();
			if (edgeLength2 < eps * eps)
			{
				degenerateEdge = e;
			}

			bool condition1 = e->prev->vert == e->next->vert;
			bool condition2 = e->next == e->rev;
			if (condition1 || condition2)
			{
				// check if the vertex is used by other edges
				degenerateEdge = e;
				std::set<const carve::mesh::Edge<3>* > setEdgesReferencing;
				carve::mesh::Vertex<3>* vertex = e->vert;
				countReferencesToVertex(e, vertex, setEdgesReferencing);
				size_t numReferences = setEdgesReferencing.size();

				if (numReferences > 1)
				{
					// other edges referencing current vertex
					degenerateEdge = nullptr;
					carve::mesh::Edge<3>* reverseEdge = e->rev;
					if (reverseEdge != nullptr)
					{
						carve::mesh::Vertex<3>* vertexReverseEdge = reverseEdge->vert;
						std::set<const carve::mesh::Edge<3>* > setEdgesReferencingReferseEdge;
						countReferencesToVertex(reverseEdge, vertexReverseEdge, setEdgesReferencingReferseEdge);
						size_t numReferencesReverse = setEdgesReferencingReferseEdge.size();

						if (numReferencesReverse == 1)
						{
							degenerateEdge = reverseEdge;
						}
					}
				}
			}

			if (degenerateEdge != nullptr)
			{
				carve::mesh::Edge<3>* degenerateEdgeReverse = degenerateEdge->rev;
				if (degenerateEdgeReverse != nullptr)
				{
					auto itFindEdge = setEdgesToRemove.find(degenerateEdgeReverse);
					if (itFindEdge == setEdgesToRemove.end())
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

		for (carve::mesh::Edge<3>*edgeRemove : setEdgesToRemove)
		{
			std::set<carve::mesh::Face<3>* > setFacesToReplaceEdgePointer;
			if (face->edge == edgeRemove)
			{
				carve::mesh::Face<3>* faceNonConst = (carve::mesh::Face<3>*)face;
				setFacesToReplaceEdgePointer.insert(faceNonConst);
			}

			for (auto faceInMesh : face->mesh->faces)
			{
				if (faceInMesh->edge == edgeRemove)
				{
					setFacesToReplaceEdgePointer.insert(faceInMesh);
				}
			}

			carve::mesh::Edge<3>* edgeRemainingNext = edgeRemove->removeEdge();
			for (auto faceReplaceEdgePointer : setFacesToReplaceEdgePointer)
			{
				if (faceReplaceEdgePointer->edge != edgeRemainingNext)
				{
					faceReplaceEdgePointer->edge = edgeRemainingNext;
				}
			}

			++numChanges;
		}
	}

	static void countReferencesToVertex(const carve::mesh::Edge<3>* edge, const carve::mesh::Vertex<3>* vertex, std::set<const carve::mesh::Edge<3>* >& setEdgesReferencing)
	{
		if (!edge->face)
		{
			return;
		}

		for (size_t jj = 0; jj < edge->face->mesh->faces.size(); ++jj)
		{
			carve::mesh::Face<3>* face3 = edge->face->mesh->faces[jj];
			carve::mesh::Edge<3>* e3 = face3->edge;
			for (size_t kk = 0; kk < face3->n_edges; ++kk)
			{
				if (e3)
				{
					//if( e3 != edge )
					{
						if (e3->vert == vertex)
						{
							setEdgesReferencing.insert(e3);
						}
					}
				}
				e3 = e3->next;
			}
		}
	}

};

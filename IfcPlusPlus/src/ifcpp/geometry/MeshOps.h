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
#include "GeomUtils.h"
#include <ifcpp/model/StatusCallback.h>
#include "GeometryInputData.h"

class IFCQUERY_EXPORT MeshOps
{
public:
	static double computeFaceArea(const carve::mesh::Face<3>* face);
	static double computeFaceArea(const carve::mesh::Face<3>* face, double& longestEdge);
	static double computeMeshsetVolume(const carve::mesh::MeshSet<3>* meshset);
	static double computeMeshSetSurface(const shared_ptr<carve::mesh::MeshSet<3> >& meshset);
	static double computeShapeSurfaceArea(const shared_ptr<ItemShapeData>& shape_input_data);
	static double computeShapeSurfaceArea(const shared_ptr<ProductShapeData>& shape_input_data);
	static size_t getNumFaces(const carve::mesh::MeshSet<3>* meshset);
	static void recalcMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, double eps);
	static void checkFaceIntegrity(const carve::mesh::Face<3>* face, bool checkForDegenerateEdges, MeshSetInfo& info, double eps);
	static size_t countFaces(carve::mesh::MeshSet<3>* mesh);
	static size_t countDegeneratedFaces(carve::mesh::MeshSet<3>* mesh);
	
	static void simplifyMeshSet(shared_ptr<carve::mesh::MeshSet<3> >& meshset, MeshSetInfo& infoMeshOut, const GeomProcessingParams& params);

	static void retriangulateMeshSetForBoolOp(shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool ignoreResultOpenEdges, const GeomProcessingParams& params, size_t retryCount);
	
	/// \brief retriangulateMeshSetForExport: triangulate mesh, with priority of not skipping triangles, mesh does not need to be closed and valid for boolean operations
	static void retriangulateMeshSetForExport(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, PolyInputCache3D& polyOut, const GeomProcessingParams& params);

	static void intersectOpenEdgesWithPoints(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params);

	static void intersectOpenEdgesWithEdges(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params);

	static bool isBetterForBoolOp(const MeshSetInfo& infoNew, const MeshSetInfo& infoBefore, bool considerTriangulation);

	static bool checkMeshSetValidAndClosed(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, const GeomProcessingParams& params );

	static bool checkMeshSetNonNegativeAndClosed(const shared_ptr<carve::mesh::MeshSet<3>> mesh_set, const GeomProcessingParams& params);

	static void checkMeshIntegrity(const carve::mesh::Mesh<3>* mesh, bool checkForDegenerateEdges, const GeomProcessingParams& params, MeshSetInfo& info);

	static void checkMeshSetIntegrity(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, bool checkForDegenerateEdges, const GeomProcessingParams& params, MeshSetInfo& info);

	static void resolveOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, const GeomProcessingParams& params);
	static void resolveOpenEdges(shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, const GeomProcessingParams& params);

	static void removeDegenerateMeshes(shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const GeomProcessingParams& params, bool ensureValidMesh);

	static bool assignIfBetterForBoolOp(shared_ptr<carve::mesh::MeshSet<3> >& meshsetNew, shared_ptr<carve::mesh::MeshSet<3> >& meshsetBefore, MeshSetInfo& infoNew, MeshSetInfo& infoBefore, bool considerTriangulation, const GeomProcessingParams& params, bool deepCopyMesh);

	static void checkAndFixMeshsetInverted( shared_ptr<carve::mesh::MeshSet<3>>& meshset, MeshSetInfo& info, const GeomProcessingParams& params);

	static void classifyMeshesInside(std::vector<carve::mesh::Mesh<3>*>& meshes, shared_ptr<carve::mesh::MeshSet<3> >& result, GeomProcessingParams& params);

	static std::shared_ptr<carve::mesh::MeshSet<3> > createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, double eps);
	static std::shared_ptr<carve::mesh::MeshSet<3> > createPlaneMesh(vec3& p0, vec3& p1, vec3& p2, vec3& p3, double eps);
	static std::shared_ptr<carve::mesh::MeshSet<3> > createBoxMesh(vec3& pos, vec3& extent, carve::math::Matrix& transform, double eps);
	static void boundingBox2Mesh(const carve::geom::aabb<3>& bbox, shared_ptr<carve::mesh::MeshSet<3> >& meshset, double eps);
	
	static void polyhedronFromMesh(const carve::mesh::Mesh<3>* mesh, PolyInputCache3D& polyInput);
	static void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, PolyInputCache3D& polyInput, int minNumFacesPerMesh = -1);
	static void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, const std::unordered_set<const carve::mesh::Face<3>* >& setSkipFaces, PolyInputCache3D& polyInput);
	static void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, const std::unordered_set<const carve::mesh::Face<3>* >& setSkipFaces, const std::unordered_set<const carve::mesh::Face<3>* >& setFlipFaces, PolyInputCache3D& polyInput);
	static void MeshSet2Polyhedron2MeshSet(const shared_ptr<carve::mesh::MeshSet<3> >& meshsetIn, shared_ptr<carve::mesh::MeshSet<3> >& meshsetOut, const GeomProcessingParams& params, int minNumFacesPerMesh);
};

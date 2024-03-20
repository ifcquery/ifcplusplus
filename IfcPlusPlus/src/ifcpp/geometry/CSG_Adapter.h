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

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>

#include "IncludeCarveHeaders.h"

class CSG_Adapter
{
public:
	static void computeCSG(shared_ptr<carve::mesh::MeshSet<3> >& op1, const std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& operands2,
		const carve::csg::CSG::OP operation, GeomProcessingParams& params);


	static void mergeMeshesToMeshset(std::vector<carve::mesh::Mesh<3>*>& meshes, shared_ptr<carve::mesh::MeshSet<3> >& result, GeomProcessingParams& params);

	static void assignResultOnFail(const shared_ptr<carve::mesh::MeshSet<3> >& op1, const shared_ptr<carve::mesh::MeshSet<3> >& op2, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result);

	static bool checkBoundingBoxIntersection(const carve::geom::aabb<3>& bbox1, const carve::geom::aabb<3>& bbox2, const carve::csg::CSG::OP operation, double eps);

	struct CsgOperationParams
	{
		double epsilonFactor = 1.0;
		bool normalizeCoords = true;
		bool allowDegenerateEdges = false;
		bool allowFinFacesInResult = false;
		bool allowFinEdgesInResult = false;
		bool flattenFacePlanes = false;
	};
	static bool computeCSG_Carve(const shared_ptr<carve::mesh::MeshSet<3> >& inputA, const shared_ptr<carve::mesh::MeshSet<3> >& inputB, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result,
		GeomProcessingParams& params, CsgOperationParams& csgParams);

	
	static bool computeCSG_OCC(const shared_ptr<carve::mesh::MeshSet<3> >& inputA, const shared_ptr<carve::mesh::MeshSet<3> >& inputB, const carve::csg::CSG::OP operation, shared_ptr<carve::mesh::MeshSet<3> >& result,
		GeomProcessingParams& params, CsgOperationParams& csgParams);

	static void handleInnerOuterMeshesInOperands(shared_ptr<carve::mesh::MeshSet<3> >& op1, shared_ptr<carve::mesh::MeshSet<3> >& op2, shared_ptr<carve::mesh::MeshSet<3> >& result, 
		GeomProcessingParams& params, bool& boolOpDone, double eps);
};

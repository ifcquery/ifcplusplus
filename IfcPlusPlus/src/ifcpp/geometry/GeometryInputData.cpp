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

#include <vector>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/GeomDebugDump.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/IFC4X3/include/IfcObjectPlacement.h>
#include <ifcpp/IFC4X3/include/IfcProduct.h>
#include <ifcpp/IFC4X3/include/IfcRepresentation.h>
#include <ifcpp/IFC4X3/include/IfcTextStyle.h>
#include "IncludeCarveHeaders.h"
#include "MeshOps.h"
#include "GeometryInputData.h"

using namespace IFC4X3;

bool ItemShapeData::addClosedPolyhedron(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params, shared_ptr<GeometrySettings>& geom_settings)
{
	if (poly_data->getVertexCount() < 3)
	{
		return false;
	}

	double eps = params.epsMergePoints;
	std::map<std::string, std::string> mesh_input_options;
	shared_ptr<carve::mesh::MeshSet<3> > meshsetUnchanged(poly_data->createMesh(mesh_input_options, eps));
	std::string details;
	bool correct = checkPolyhedronData(poly_data, params, details);
	if (!correct)
	{
		fixPolyhedronData(poly_data, params);
		std::string details2;
		bool correct2 = checkPolyhedronData(poly_data, params, details2);
		if (correct2)
		{
			shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(mesh_input_options, eps));

			MeshSetInfo infoTriangulated;
			bool validMesh = MeshOps::checkMeshSetValidAndClosed(meshset, infoTriangulated, params);
			if (validMesh)
			{
				m_meshsets.push_back(meshset);
				return true;
			}
		}
	}

	if (meshsetUnchanged->isClosed())
	{
		m_meshsets.push_back(meshsetUnchanged);
		return true;
	}

#ifdef _DEBUG
	if (params.debugDump)
	{
		glm::vec4 color(0.3, 0.4, 0.5, 1.0);
		GeomDebugDump::dumpMeshsetOpenEdges(meshsetUnchanged, color, false, false);
		GeomDebugDump::dumpMeshset(meshsetUnchanged.get(), color, true, false);
	}
#endif

	MeshSetInfo info;
	bool validMesh = MeshOps::checkMeshSetValidAndClosed(meshsetUnchanged, info, params);
	MeshOps::checkAndFixMeshsetInverted(meshsetUnchanged, info, params);

	if (poly_data->faceCount > 10000)
	{
		// TODO: buld spatial partition tree, and intersect all edges within a box with vertices and edges of that box
		m_meshsets_open.push_back(meshsetUnchanged); // still may be useful as open mesh
		return false;
	}

	// try to fix winding order
	reverseFacesInPolyhedronData(poly_data);

	shared_ptr<carve::mesh::MeshSet<3> > meshsetChanged(poly_data->createMesh(mesh_input_options, eps));
	if (meshsetChanged->isClosed())
	{
		m_meshsets.push_back(meshsetChanged);
		return true;
	}

	MeshOps::intersectOpenEdgesWithPoints(meshsetChanged, params);

	for (size_t i = 0; i < meshsetChanged->meshes.size(); ++i)
	{
		meshsetChanged->meshes[i]->recalc(eps);
	}
	if (meshsetChanged->isClosed())
	{
		m_meshsets.push_back(meshsetChanged);
		return true;
	}

	MeshOps::intersectOpenEdgesWithEdges(meshsetChanged, params);

	if (meshsetChanged->isClosed())
	{
		m_meshsets.push_back(meshsetChanged);
		return true;
	}

	MeshOps::resolveOpenEdges(meshsetChanged, params);

	if (meshsetChanged->isClosed())
	{
		m_meshsets.push_back(meshsetChanged);
		return true;
	}

	MeshSetInfo infoChanged;
	bool validMeshChanged = MeshOps::checkMeshSetValidAndClosed(meshsetChanged, infoChanged, params);
	if (MeshOps::isBetterForBoolOp(infoChanged, info, false))
	{
		m_meshsets_open.push_back(meshsetChanged);

#ifdef _DEBUG
		if (params.debugDump)
		{
			glm::vec4 color(0.3, 0.4, 0.5, 1.0);
			GeomDebugDump::dumpMeshsetOpenEdges(meshsetChanged, color, false, false);
			GeomDebugDump::dumpMeshset(meshsetChanged.get(), color, true, false);
		}
#endif
	}
	else
	{
		// still may be useful as open mesh
		m_meshsets_open.push_back(meshsetUnchanged);

#ifdef _DEBUG
		if (params.debugDump)
		{
			glm::vec4 color(0.3, 0.4, 0.5, 1.0);
			GeomDebugDump::dumpMeshsetOpenEdges(meshsetUnchanged, color, false, false);
			GeomDebugDump::dumpMeshset(meshsetUnchanged.get(), color, true, false);
		}
#endif
	}

	// Meshset is not closed
	return false;
}

void ItemShapeData::addOpenOrClosedPolyhedron(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params)
{
	if (!poly_data)
	{
		return;
	}

	// check if it is open or closed
	if (poly_data->getVertexCount() < 3)
	{
		return;
	}

	std::string details;
	bool correct = checkPolyhedronData(poly_data, params, details);
	if (!correct)
	{
		fixPolyhedronData(poly_data, params);
#ifdef _DEBUG
		std::string details2;
		bool correct2 = checkPolyhedronData(poly_data, params, details2);
		if (!correct2)
		{
			std::cout << "fixPolyhedronData failed: " << details2 << std::endl;
		}
#endif
	}

	shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts(), params.epsMergePoints));
	if (meshset->isClosed())
	{
		m_meshsets.push_back(meshset);
	}
	else
	{
		m_meshsets_open.push_back(meshset);

#ifdef _DEBUG_GEOM
		MeshSetInfo info;
		MeshOps::checkMeshSetValidAndClosed(meshset, info, params);

		shared_ptr<carve::mesh::MeshSet<3> > meshsetCopy(meshset->clone());

		MeshOps::retriangulateMeshSetForBoolOp(meshsetCopy, false, params, 0);

		MeshSetInfo info2;
		MeshOps::checkMeshSetValidAndClosed(meshsetCopy, info2, params);

		if (MeshOps::isBetterForBoolOp(info2, info, false))
		{
			if (params.debugDump)
			{
				glm::vec4 color(0.5, 0.5, 0.5, 1);
				GeomDebugDump::moveOffset(0.1);
				bool drawNormals = false;
				GeomDebugDump::dumpMeshset(meshsetCopy, color, drawNormals, true);
			}
		}

		GeomProcessingParams params2(params);
		params2.triangulateResult = false;
		params2.shouldBeClosedManifold = false;
		if (params.generalSettings)
		{
			if (params.generalSettings->m_callback_simplify_mesh)
			{
				params.generalSettings->m_callback_simplify_mesh(meshsetCopy, params);
			}
		}

		MeshSetInfo info3;
		MeshOps::checkMeshSetValidAndClosed(meshsetCopy, info3, params);

		if (MeshOps::isBetterForBoolOp(info3, info, false))
		{
			// simplified meshset is better
			if (params.debugDump)
			{
				glm::vec4 color(0.5, 0.5, 0.5, 1);
				GeomDebugDump::moveOffset(0.1);
				bool drawNormals = false;
				GeomDebugDump::dumpMeshset(meshsetCopy, color, drawNormals, true);
			}
		}
#endif
	}
}
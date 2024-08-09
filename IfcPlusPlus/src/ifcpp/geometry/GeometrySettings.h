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

#define _USE_MATH_DEFINES 
#include <cmath>
#include <functional>
#include <map>
#include <unordered_set>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingObject.h>
#include <ifcpp/IFC4X3/EntityFactory.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define EPS_M16 1e-16
#define EPS_M14 1e-14
#define EPS_M12 1e-12
#define EPS_M9 1e-9
#define EPS_M8 1e-8
#define EPS_M7 1e-7
#define EPS_M6 1e-6
#define EPS_M5 1e-5
#define EPS_M4 1e-4
#define EPS_RANDOM_FACTOR 1.51634527
#define EPS_DEFAULT EPS_RANDOM_FACTOR*EPS_M8
#define EPS_ALIGNED_EDGES 1e-8
#define EPS_ANGLE_COPLANAR_FACES 1e-9
#define EPS_MIN_FACE_AREA 1e-10
#define HALF_SPACE_BOX_SIZE 100
#define MAX_NUM_EDGES 100000

class StatusCallback;
class CarveMeshNormalizer;
struct GeomProcessingParams;
namespace carve { namespace mesh { template <unsigned int ndim>	class MeshSet; } }
using MeshSimplifyCallbackType = std::function<void(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const GeomProcessingParams& params)>;

//\brief Central class to hold settings that influence geometry processing.
class GeometrySettings
{
public:
	GeometrySettings()
	{
		m_excludeIfcTypes.insert(IFCFEATUREELEMENTSUBTRACTION);// 1287392070);  // IfcFeatureElementSubtraction
	}
	GeometrySettings(const shared_ptr<GeometrySettings>& other)
	{
		m_excludeIfcTypes = other->m_excludeIfcTypes;
		m_renderOnlyIfcTypes = other->m_excludeIfcTypes;
		m_maxNumFaceEdges = other->m_maxNumFaceEdges;
		m_num_vertices_per_circle = other->m_num_vertices_per_circle;
		m_num_vertices_per_circle_default = other->m_num_vertices_per_circle_default;
		m_min_num_vertices_per_arc = other->m_min_num_vertices_per_arc;
		m_num_vertices_per_control_point = other->m_num_vertices_per_control_point;
		m_num_vertices_per_control_point_default = other->m_num_vertices_per_control_point_default;
		m_show_text_literals = other->m_show_text_literals;
		m_ignore_profile_radius = other->m_ignore_profile_radius;
		m_handle_styled_items = other->m_handle_styled_items;
		m_handle_layer_assignments = other->m_handle_layer_assignments;
		m_render_bounding_box = other->m_render_bounding_box;
		m_min_triangle_area = other->m_min_triangle_area;
		m_epsilonMergePoints = other->m_epsilonMergePoints;
		m_epsCoplanarAngle = other->m_epsCoplanarAngle;
		m_mergeAlignedEdges = other->m_mergeAlignedEdges;
		m_callback_simplify_mesh = other->m_callback_simplify_mesh;
	}

	// Number of discretization points per circle
	int getNumVerticesPerCircleWithRadius(double radius) { return m_num_vertices_per_circle_given_radius(radius); }
	int getNumVerticesPerCircle() { return m_num_vertices_per_circle; }
	void setNumVerticesPerCircle(int num) { m_num_vertices_per_circle = num; }
	void resetNumVerticesPerCircle() { m_num_vertices_per_circle = m_num_vertices_per_circle_default; }
	void setNumVerticesPerCircleGivenRadius(std::function<int(double)> f) { m_num_vertices_per_circle_given_radius = f; }

	// Minimum number of discretization points per arc
	int getMinNumVerticesPerArc() { return m_min_num_vertices_per_arc; }
	void resetMinNumVerticesPerArc() { m_min_num_vertices_per_arc = 6; }
	void setMinNumVerticesPerArc(int num) { m_min_num_vertices_per_arc = num; }

	int getNumVerticesPerControlPoint() { return m_num_vertices_per_control_point; }
	void setNumVerticesPerControlPoint(int num) { m_num_vertices_per_control_point = num; }
	void resetNumVerticesPerControlPoint() { m_num_vertices_per_control_point = m_num_vertices_per_control_point_default; }

	void setHandleLayerAssignments(bool handle) { m_handle_layer_assignments = handle; }
	bool handleLayerAssignments() { return m_handle_layer_assignments; }

	void setHandleStyledItems(bool handle) { m_handle_styled_items = handle; }
	bool handleStyledItems() { return m_handle_styled_items; }

	bool isShowTextLiterals() { return m_show_text_literals; }
	bool isIgnoreProfileRadius() { return m_ignore_profile_radius; }
	void setIgnoreProfileRadius(bool ignore_radius) { m_ignore_profile_radius = ignore_radius; }

	/**\brief setMinTriangleArea: if a triangle is smaller than this value, it is still in the carve meshset, but skipped for rendering.
	That reduces the number of triangles on the GPU, not visible unless you zoom in to a very small area */
	void setMinTriangleArea(double min_area) { m_min_triangle_area = min_area; }
	double getMinTriangleArea() { return m_min_triangle_area; }

	/**\brief Render bounding box for each object */
	bool getRenderBoundingBoxes() { return m_render_bounding_box; }
	void setRenderBoundingBoxes(bool render_bbox) { m_render_bounding_box = render_bbox; }

	void setEpsilonMergePoints(double eps)
	{
		m_epsilonMergePoints = eps;
	}

	double getEpsilonMergePoints()
	{
		return m_epsilonMergePoints;
	}

	void setEpsilonCoplanarAngle(double eps)
	{
		m_epsCoplanarAngle = eps;
	}

	double getEpsilonCoplanarAngle()
	{
		return m_epsCoplanarAngle;
	}

	bool skipRenderObject(uint32_t classID)
	{
		if (m_excludeIfcTypes.find(classID) != m_excludeIfcTypes.end())
		{
			return true;
		}

		if (m_renderOnlyIfcTypes.size() > 0)
		{
			if (m_renderOnlyIfcTypes.find(classID) == m_renderOnlyIfcTypes.end())
			{
				// classID not in list of types to convert
				return true;
			}
		}
		return false;
	}

	std::unordered_set<uint32_t> m_excludeIfcTypes;		// if set, these types will not be converted
	std::unordered_set<uint32_t> m_renderOnlyIfcTypes;	// if set, only these types will be converted
	size_t m_maxNumFaceEdges = MAX_NUM_EDGES;
	bool m_mergeAlignedEdges = true;
	MeshSimplifyCallbackType m_callback_simplify_mesh;
	std::map<int, std::vector<int>, std::greater<int> > m_mapCsgTimeTag;
	
protected:
	int	m_num_vertices_per_circle = 14;
	int m_num_vertices_per_circle_default = 14;
	int m_min_num_vertices_per_arc = 5;
	int m_num_vertices_per_control_point = 1;
	int m_num_vertices_per_control_point_default = 1;
	bool m_show_text_literals = false;
	bool m_ignore_profile_radius = false;
	bool m_handle_styled_items = true;
	bool m_handle_layer_assignments = true;
	bool m_render_bounding_box = false;
	double m_min_triangle_area = EPS_MIN_FACE_AREA;
	double m_epsilonMergePoints = EPS_DEFAULT;
	double m_epsCoplanarAngle = EPS_ANGLE_COPLANAR_FACES;

	std::function<int(double)> m_num_vertices_per_circle_given_radius = [&](double radius)
	{
		if (radius > 0.5) return int(m_num_vertices_per_circle*1.5);
		return m_num_vertices_per_circle;
	};
};

struct GeomProcessingParams
{
	GeomProcessingParams( shared_ptr<GeometrySettings>& generalSettings )
	{
		epsMergePoints = generalSettings->getEpsilonMergePoints();
		epsMergeAlignedEdgesAngle = generalSettings->getEpsilonCoplanarAngle();
		minFaceArea = generalSettings->getMinTriangleArea();
		mergeAlignedEdges = generalSettings->m_mergeAlignedEdges;
		this->generalSettings = generalSettings;
	}
	GeomProcessingParams(shared_ptr<GeometrySettings>& generalSettings, bool dumpMeshes) : GeomProcessingParams(generalSettings)
	{
		this->debugDump = dumpMeshes;
	}
	GeomProcessingParams( shared_ptr<GeometrySettings>& generalSettings, BuildingEntity* ifc_entity, StatusCallback* callbackFunc) : GeomProcessingParams(generalSettings)
	{
		this->ifc_entity = ifc_entity;
		this->callbackFunc = callbackFunc;
	}
	GeomProcessingParams(const GeomProcessingParams& other)
	{
		this->generalSettings = other.generalSettings;
		this->ifc_entity = other.ifc_entity;
		this->callbackFunc = other.callbackFunc;
		this->epsMergePoints = other.epsMergePoints;
		this->epsMergeAlignedEdgesAngle = other.epsMergeAlignedEdgesAngle;
		this->minFaceArea = other.minFaceArea;
		this->mergeAlignedEdges = other.mergeAlignedEdges;
		this->allowFinEdges = other.allowFinEdges;
		this->allowDegenerateEdges = other.allowDegenerateEdges;
		this->checkZeroAreaFaces = other.checkZeroAreaFaces;
		this->allowZeroAreaFaces = other.allowZeroAreaFaces;
		this->triangulateResult = other.triangulateResult;
		this->shouldBeClosedManifold = other.shouldBeClosedManifold;
		this->treatLongThinFaceAsDegenerate = other.treatLongThinFaceAsDegenerate;
		this->debugDump = other.debugDump;
		this->normalizer = other.normalizer;
	}
	shared_ptr<GeometrySettings> generalSettings;
	BuildingEntity* ifc_entity = nullptr;
	StatusCallback* callbackFunc = nullptr;
	
	double epsMergePoints = EPS_DEFAULT;
	double epsMergeAlignedEdgesAngle = EPS_ALIGNED_EDGES;
	double minFaceArea = EPS_MIN_FACE_AREA;
	bool mergeAlignedEdges = true;
	bool allowFinEdges = false;
	bool allowDegenerateEdges = false;
	bool checkZeroAreaFaces = true;
	bool allowZeroAreaFaces = false;
	bool triangulateResult = false;
	bool shouldBeClosedManifold = true;
	bool treatLongThinFaceAsDegenerate = false;
	bool debugDump = false;
	CarveMeshNormalizer* normalizer = nullptr;
};

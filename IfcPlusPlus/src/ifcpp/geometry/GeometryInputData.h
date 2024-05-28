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

#include <vector>
#include <unordered_set>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/IFC4X3/include/IfcObjectPlacement.h>
#include <ifcpp/IFC4X3/include/IfcProduct.h>
#include <ifcpp/IFC4X3/include/IfcRepresentation.h>
#include <ifcpp/IFC4X3/include/IfcTextStyle.h>
#include "IncludeCarveHeaders.h"
#include "GeomUtils.h"

class PolyInputCache3D;
class ProductShapeData;

struct MeshSetInfo
{
	MeshSetInfo()
	{
	}
	MeshSetInfo(const MeshSetInfo& other)
	{
		copyFromOther(other);
	}

	void copyFromOther(const MeshSetInfo& other)
	{
		numClosedEdges = other.numClosedEdges;
		openEdges = other.openEdges;
		numFaces = other.numFaces;
		zeroAreaFaces = other.zeroAreaFaces;
		degenerateEdges = other.degenerateEdges;
		degenerateFaces = other.degenerateFaces;
		finEdges = other.finEdges;
		finFaces = other.finFaces;
		surfaceArea = other.surfaceArea;
		allPointersValid = other.allPointersValid;
		meshSetValid = other.meshSetValid;
		maxNumEdgesExceeded = other.maxNumEdgesExceeded;
		maxNumberOfEdgesPerFace = other.maxNumberOfEdgesPerFace;
		details = other.details;
		report_callback = other.report_callback;
		entity = other.entity;
	}
	MeshSetInfo(StatusCallback* _report_callback, BuildingEntity* _entity)
	{
		report_callback = _report_callback;
		entity = _entity;
	}

	bool isEqual(const MeshSetInfo& other, bool checkOnlyNumberOfEdgesAndFaces = true) const
	{
		if (numClosedEdges != other.numClosedEdges) return false;
		if (openEdges.size() != other.openEdges.size() ) return false;
		if (numFaces != other.numFaces) return false;
		if (checkOnlyNumberOfEdgesAndFaces)
		{
			if (zeroAreaFaces.size() != other.zeroAreaFaces.size() ) return false;
			if (degenerateEdges.size() != other.degenerateEdges.size()) return false;
			if (degenerateFaces.size() != other.degenerateFaces.size()) return false;
			if (finEdges.size() != other.finEdges.size()) return false;
			if (finFaces.size() != other.finFaces.size()) return false;
		}
		else
		{
			if (zeroAreaFaces != other.zeroAreaFaces) return false;
			if (degenerateEdges != other.degenerateEdges) return false;
			if (degenerateFaces != other.degenerateFaces) return false;
			if (finEdges != other.finEdges) return false;
			if (finFaces != other.finFaces) return false;
		}
		if (std::abs(surfaceArea - other.surfaceArea) > EPS_M12 ) return false;
		if (allPointersValid != other.allPointersValid) return false;
		if (meshSetValid != other.meshSetValid) return false;
		if (maxNumEdgesExceeded != other.maxNumEdgesExceeded) return false;
		if (maxNumberOfEdgesPerFace != other.maxNumberOfEdgesPerFace) return false;
		//if (details != other.details) return false;
		return true;
	}

	size_t numClosedEdges = 0;
	size_t numOpenEdges() const { return openEdges.size(); }
	size_t numFaces = 0;
	std::unordered_set<carve::mesh::Edge<3>* > openEdges;
	std::unordered_set<carve::mesh::Face<3>* > zeroAreaFaces;
	std::unordered_set<carve::mesh::Edge<3>* > degenerateEdges;
	std::unordered_set<const carve::mesh::Face<3>* > degenerateFaces;
	std::unordered_set<carve::mesh::Edge<3>* > finEdges;
	std::unordered_set<carve::mesh::Face<3>* > finFaces;
	double surfaceArea = 0;
	bool allPointersValid = true;
	bool meshSetValid = false;
	bool maxNumEdgesExceeded = false;
	size_t maxNumberOfEdgesPerFace = 0; // if it is 3, meshset is triangulated
	std::string details;
	StatusCallback* report_callback = nullptr;
	BuildingEntity* entity = nullptr;
	shared_ptr<carve::mesh::MeshSet<3> > meshset;

	void resetInfoResult()
	{
		numClosedEdges = 0;
		openEdges.clear();
		numFaces = 0;
		zeroAreaFaces.clear();
		degenerateEdges.clear();
		degenerateFaces.clear();
		finEdges.clear();
		finFaces.clear();
		surfaceArea = 0;
		allPointersValid = true;
		meshSetValid = false;
		maxNumEdgesExceeded = false;
		maxNumberOfEdgesPerFace = false;
		details = "";
	}
};

class TextItemData
{
public:
	std::string m_text;
	carve::math::Matrix m_text_position;
};

inline void premultMatrix( const carve::math::Matrix& matrix_to_append, carve::math::Matrix& target_matrix )
{
	target_matrix = matrix_to_append*target_matrix;
}
struct ScopedBoolFalse
{
	bool previousValue = false;
	bool* boolToFlip = nullptr;
	ScopedBoolFalse(bool* var)
	{
		boolToFlip = var;
		previousValue = *var;
		(*boolToFlip) = false;
	}
	~ScopedBoolFalse()
	{
		(*boolToFlip) = previousValue;
	}
};

struct ScopedBoolTrue
{
	bool previousValue = false;
	bool* boolToTrue = nullptr;
	ScopedBoolTrue(bool* var)
	{
		boolToTrue = var;
		previousValue = *var;
		(*boolToTrue) = true;
	}
	~ScopedBoolTrue()
	{
		(*boolToTrue) = previousValue;
	}
};

struct ScopedCallStackEntry
{
	std::vector<std::string>* m_callStack = nullptr;
	ScopedCallStackEntry(const std::string& funcName, std::vector<std::string>& callStack)
	{
		callStack.push_back(funcName);
		m_callStack = &callStack;
	}
	~ScopedCallStackEntry()
	{
		m_callStack->pop_back();
	}
};

struct ScopedTimeMeasure
{
	int timeStart = 0;
	int tag = 0;
	int numElements = 10;
	std::map<int, std::vector<int>, std::greater<int> > * mapTimeTag = nullptr;
	ScopedTimeMeasure(std::map<int, std::vector<int>, std::greater<int> >* _mapTimeTag, int _tag, int _numElements)
	{
		mapTimeTag = _mapTimeTag;
		tag = _tag;
		numElements = _numElements;
		timeStart = clock();
	}
	~ScopedTimeMeasure()
	{
		int timeEnd = clock();
		int duration = timeEnd - timeStart;
		if (mapTimeTag->size() < numElements)
		{
			mapTimeTag->insert({ duration, { tag} });
		}
		else
		{
			auto it10 = mapTimeTag->rbegin();
			if (it10 == mapTimeTag->rend())
			{
				return; // what?
			}
			int duration10 = it10->first;
			if (duration < duration10)
			{
				return;
			}

			auto itFindExisting = mapTimeTag->find(duration);
			if (itFindExisting != mapTimeTag->end())
			{
				std::vector<int>& vecTags = itFindExisting->second;
				vecTags.push_back(tag);
				return;
			}

			mapTimeTag->erase(std::prev(mapTimeTag->end()));
			mapTimeTag->insert({ duration, {tag} });
		}
	}
};

/**
*\brief Class TransformData: holds a matrix of a coordinate system and a pointer to the corresponding IFC placement entity
*/
class TransformData
{
public:
	TransformData(){}
	TransformData( const carve::math::Matrix& matrix, weak_ptr<BuildingEntity>& placement_entity, int placement_id ) : m_placement_tag( placement_id ), m_matrix(matrix), m_placement_entity(placement_entity)
	{
	}
	TransformData( const shared_ptr<TransformData>& other )
	{
		if( other )
		{
			m_matrix = other->m_matrix;
			m_placement_entity = other->m_placement_entity;
			m_placement_tag = other->m_placement_tag;
		}
	}
	bool isSameTransform( shared_ptr<TransformData>& transform_data )
	{
		if( transform_data )
		{
			if( !transform_data->m_placement_entity.expired() )
			{
				if( !m_placement_entity.expired() )
				{
					shared_ptr<BuildingEntity> ent1( m_placement_entity );
					shared_ptr<BuildingEntity> ent2( transform_data->m_placement_entity );
					return ent1 == ent2;
				}
			}
		}
		return false;
	}
	
	carve::math::Matrix			m_matrix;
	weak_ptr<BuildingEntity>	m_placement_entity;
	int							m_placement_tag = -1;
};

class StyleData
{
public:
	enum GeometryTypeEnum { GEOM_TYPE_UNDEFINED, GEOM_TYPE_TEXT, GEOM_TYPE_CURVE, GEOM_TYPE_SURFACE, GEOM_TYPE_VOLUME, GEOM_TYPE_ANY };
	StyleData(int step_style_id) : m_step_style_id(step_style_id)
	{
	}
	vec4 m_color_ambient;
	vec4 m_color_diffuse;
	vec4 m_color_specular;
	int m_step_style_id;
	double m_shininess = 10.0;
	double m_transparency = 0.0;   // 0 = opaque, 1 = fully transparent
	double m_specular_exponent = 0.0;
	double m_specular_roughness = 0.0;
	bool m_complete = false;
	shared_ptr<IFC4X3::IfcTextStyle> m_text_style;
	GeometryTypeEnum m_apply_to_geometry_type = GEOM_TYPE_UNDEFINED;
};

bool checkPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params, std::string& details);
bool fixPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params);
bool reverseFacesInPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data);

class ItemShapeData
{
public:
	weak_ptr<ProductShapeData>					m_product;		// IFC product to which the item belongs
	weak_ptr<ItemShapeData>						m_parentItem;	// in cas the current item is a child item of another item
	std::vector<shared_ptr<ItemShapeData> >		m_child_items;
	weak_ptr<IFC4X3::IfcRepresentation>			m_ifc_representation;

	std::vector<shared_ptr<carve::input::PolylineSetData> > m_polylines;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		m_meshsets;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		m_meshsets_open;
	std::vector<shared_ptr<TextItemData> >					m_text_literals;
	std::vector<shared_ptr<carve::input::VertexData> >		m_vertex_points;
	std::vector<shared_ptr<StyleData> >						m_styles;
	
	const std::vector<shared_ptr<StyleData> >& getStyles() { return m_styles; }
	bool isItemShapeEmpty()
	{
		if (m_vertex_points.size() > 0) { return false; }
		if (m_polylines.size() > 0) { return false; }
		if (m_meshsets.size() > 0) { return false; }
		if (m_meshsets_open.size() > 0) { return false; }
		if (m_text_literals.size() > 0) { return false; }

		return true;
	}

	void copyFrom(shared_ptr<ItemShapeData>& other)
	{
		m_product = other->m_product;
		m_parentItem = other->m_parentItem;
		m_child_items = other->m_child_items;
		m_ifc_representation = other->m_ifc_representation;
		m_polylines = other->m_polylines;
		for (shared_ptr<carve::mesh::MeshSet<3> >&meshset : other->m_meshsets)
		{
			shared_ptr<carve::mesh::MeshSet<3> > meshsetCopy(meshset->clone());
			m_meshsets.push_back(meshsetCopy);
		}
		for (shared_ptr<carve::mesh::MeshSet<3> >&meshset : other->m_meshsets_open)
		{
			shared_ptr<carve::mesh::MeshSet<3> > meshsetCopy(meshset->clone());
			m_meshsets_open.push_back(meshsetCopy);
		}
		
		m_text_literals = other->m_text_literals;
		m_vertex_points = other->m_vertex_points;
		m_styles = other->m_styles;
	}

	void addOpenOrClosedPolyhedron(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params);

	void addOpenPolyhedron(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params)
	{
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
		m_meshsets_open.push_back(meshset);
	}

	bool addClosedPolyhedron(const shared_ptr<carve::input::PolyhedronData>& poly_data, GeomProcessingParams& params);

	void addPoint(const vec3& point)
	{
		shared_ptr<carve::input::VertexData> vertex_data;
		if (m_vertex_points.size() > 0)
		{
			if (!m_vertex_points[0])
			{
				m_vertex_points[0] = shared_ptr<carve::input::VertexData>(new carve::input::VertexData());
			}
			vertex_data = m_vertex_points[0];
		}
		else
		{
			vertex_data = shared_ptr<carve::input::VertexData>(new carve::input::VertexData());
			m_vertex_points.push_back(vertex_data);
		}

		vertex_data->points.push_back(point);
	}

	void addStyle(shared_ptr<StyleData>& newStyle)
	{
		if (!newStyle)
		{
			return;
		}
		int append_id = newStyle->m_step_style_id;
		for (size_t ii = 0; ii < m_styles.size(); ++ii)
		{
			shared_ptr<StyleData>& style = m_styles[ii];
			if (style->m_step_style_id == append_id)
			{
				return;
			}
		}
		m_styles.push_back(newStyle);
	}

	/** copies the content of other instance and adds it to own content */
	void addItemData(const shared_ptr<ItemShapeData>& other)
	{
		std::copy(other->m_vertex_points.begin(), other->m_vertex_points.end(), std::back_inserter(m_vertex_points));
		std::copy(other->m_polylines.begin(), other->m_polylines.end(), std::back_inserter(m_polylines));
		std::copy(other->m_meshsets.begin(), other->m_meshsets.end(), std::back_inserter(m_meshsets));
		std::copy(other->m_meshsets_open.begin(), other->m_meshsets_open.end(), std::back_inserter(m_meshsets_open));
		std::copy(other->m_styles.begin(), other->m_styles.end(), std::back_inserter(m_styles));
		std::copy(other->m_text_literals.begin(), other->m_text_literals.end(), std::back_inserter(m_text_literals));
	}
	
	void clearItemMeshGeometry()
	{
		m_meshsets.clear();
		m_meshsets_open.clear();
		m_text_literals.clear();
		m_styles.clear();
		m_vertex_points.clear();
		m_polylines.clear();

		for (auto child : m_child_items)
		{
			child->clearItemMeshGeometry();
		}
	}

	void releaseIfcObjects()
	{
		// nothing to do...
		m_ifc_representation.reset();
	}

	void applyTransformToItem(const carve::math::Matrix& mat, double eps, bool matrix_identity_checked)
	{
		if (!matrix_identity_checked)
		{
			if (GeomUtils::isMatrixIdentity(mat))
			{
				return;
			}
		}

		for (size_t ii = 0; ii < m_vertex_points.size(); ++ii)
		{
			shared_ptr<carve::input::VertexData>& vertex_data = m_vertex_points[ii];
			for (size_t j = 0; j < vertex_data->points.size(); ++j)
			{
				vec3& point = vertex_data->points[j];
				point = mat * point;
			}
		}

		for (size_t polyline_i = 0; polyline_i < m_polylines.size(); ++polyline_i)
		{
			shared_ptr<carve::input::PolylineSetData>& polyline_data = m_polylines[polyline_i];
			for (size_t j = 0; j < polyline_data->points.size(); ++j)
			{
				vec3& point = polyline_data->points[j];
				point = mat * point;
			}
		}

		//is negative if coordinate system changes handedness (for example as result of mirroring)
		//in this case invert the meshes to not make them look inside out (only noticeable if using back face culling)
		bool const invert_meshes = 0 > carve::geom::dotcross(
			carve::geom::VECTOR(mat.m[0][0], mat.m[1][0], mat.m[2][0]),
			carve::geom::VECTOR(mat.m[0][1], mat.m[1][1], mat.m[2][1]),
			carve::geom::VECTOR(mat.m[0][2], mat.m[1][2], mat.m[2][2]));

		for (size_t i_meshsets = 0; i_meshsets < m_meshsets_open.size(); ++i_meshsets)
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets_open[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}

			for (size_t i = 0; i < item_meshset->vertex_storage.size(); ++i)
			{
				vec3& point = item_meshset->vertex_storage[i].v;
				point = mat * point;
			}
			for (size_t i = 0; i < item_meshset->meshes.size(); ++i)
			{
				item_meshset->meshes[i]->recalc(eps);
				if (invert_meshes)
				{
					item_meshset->meshes[i]->invert();
				}
			}
		}

		for (size_t i_meshsets = 0; i_meshsets < m_meshsets.size(); ++i_meshsets)
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}

			for (size_t i = 0; i < item_meshset->vertex_storage.size(); ++i)
			{
				vec3& point = item_meshset->vertex_storage[i].v;
				point = mat * point;
			}
			for (size_t i = 0; i < item_meshset->meshes.size(); ++i)
			{
				item_meshset->meshes[i]->recalc(eps);
				if (invert_meshes)
				{
					item_meshset->meshes[i]->invert();
					//calcOrientation resets isNegative flag (usually)
					item_meshset->meshes[i]->calcOrientation();
				}
			}
		}

		for (size_t text_i = 0; text_i < m_text_literals.size(); ++text_i)
		{
			shared_ptr<TextItemData>& text_literals = m_text_literals[text_i];
			text_literals->m_text_position = mat * text_literals->m_text_position;
		}

		for (auto child : m_child_items)
		{
			child->applyTransformToItem(mat, eps, matrix_identity_checked);
		}
	}

	void getAllMeshPoints(std::vector<vec3>& points) const
	{
		for (size_t ii = 0; ii < m_vertex_points.size(); ++ii)
		{
			const shared_ptr<carve::input::VertexData>& vertex_data = m_vertex_points[ii];
			for (size_t j = 0; j < vertex_data->points.size(); ++j)
			{
				points.push_back(vertex_data->points[j]);
			}
		}

		for (size_t polyline_i = 0; polyline_i < m_polylines.size(); ++polyline_i)
		{
			const shared_ptr<carve::input::PolylineSetData>& polyline_data = m_polylines[polyline_i];
			for (size_t j = 0; j < polyline_data->points.size(); ++j)
			{
				points.push_back(polyline_data->points[j]);
			}
		}

		for (size_t i_meshsets = 0; i_meshsets < m_meshsets_open.size(); ++i_meshsets)
		{
			const shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets_open[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}
			points.reserve(points.size() + item_meshset->vertex_storage.size());
			for (size_t i = 0; i < item_meshset->vertex_storage.size(); ++i)
			{
				points.push_back(item_meshset->vertex_storage[i].v);
			}
		}

		for (size_t i_meshsets = 0; i_meshsets < m_meshsets.size(); ++i_meshsets)
		{
			const shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}
			points.reserve(points.size() + item_meshset->vertex_storage.size());
			for (size_t i = 0; i < item_meshset->vertex_storage.size(); ++i)
			{
				points.push_back(item_meshset->vertex_storage[i].v);
			}
		}

		for (size_t text_i = 0; text_i < m_text_literals.size(); ++text_i)
		{
			const shared_ptr<TextItemData>& text_literals = m_text_literals[text_i];
			const carve::math::Matrix& mat = text_literals->m_text_position;
			//vec3 text_pos = carve::geom::VECTOR(mat._41, mat._42,)
		}

		for (auto child : m_child_items)
		{
			child->getAllMeshPoints(points);
		}
	}

	void computeItemBoundingBox(carve::geom::aabb<3>& bbox, std::unordered_set<ItemShapeData*>& setVisited) const
	{
		for (size_t ii = 0; ii < m_vertex_points.size(); ++ii)
		{
			const shared_ptr<carve::input::VertexData>& vertex_data = m_vertex_points[ii];
			for (size_t j = 0; j < vertex_data->points.size(); ++j)
			{
				const vec3& point = vertex_data->points[j];
				if (bbox.isEmpty())
				{
					bbox = carve::geom::aabb<3>(point, /*parentTransform**/carve::geom::VECTOR(0, 0, 0));
				}
				else
				{
					bbox.unionAABB(carve::geom::aabb<3>(point, /*parentTransform**/carve::geom::VECTOR(0, 0, 0)));
				}
			}
		}

		for (size_t polyline_i = 0; polyline_i < m_polylines.size(); ++polyline_i)
		{
			const shared_ptr<carve::input::PolylineSetData>& polyline_data = m_polylines[polyline_i];
			for (size_t j = 0; j < polyline_data->points.size(); ++j)
			{
				const vec3& point = polyline_data->points[j];
				if (bbox.isEmpty())
				{
					bbox = carve::geom::aabb<3>(point, /*parentTransform**/carve::geom::VECTOR(0, 0, 0));
				}
				else
				{
					bbox.unionAABB(carve::geom::aabb<3>(point, /*parentTransform**/carve::geom::VECTOR(0, 0, 0)));
				}
			}
		}

		for (size_t i_meshsets = 0; i_meshsets < m_meshsets_open.size(); ++i_meshsets)
		{
			const shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets_open[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}
			carve::geom::aabb<3> meshBBox = item_meshset->getAABB();
			GeomUtils::unionBBox(bbox, meshBBox);// , parentTransform);
		}

		for (size_t i_meshsets = 0; i_meshsets < m_meshsets.size(); ++i_meshsets)
		{
			const shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}
			carve::geom::aabb<3> meshBBox = item_meshset->getAABB();
			GeomUtils::unionBBox(bbox, meshBBox);// , parentTransform);
		}

		for (size_t text_i = 0; text_i < m_text_literals.size(); ++text_i)
		{
			const shared_ptr<TextItemData>& text_literals = m_text_literals[text_i];
			const carve::math::Matrix& mat = text_literals->m_text_position;
			vec3 text_pos = carve::geom::VECTOR(mat._41, mat._42, mat._43);
			if (bbox.isEmpty())
			{
				bbox = carve::geom::aabb<3>(text_pos, /*parentTransform**/carve::geom::VECTOR(0, 0, 0));
			}
			else
			{
				bbox.unionAABB(carve::geom::aabb<3>(text_pos, /*parentTransform**/carve::geom::VECTOR(0, 0, 0)));
			}
		}

		for (auto child : m_child_items)
		{
			if (setVisited.find(child.get()) != setVisited.end())
			{
				continue;
			}
			setVisited.insert(child.get());

			carve::geom::aabb<3> meshBBox;
			carve::math::Matrix ident;
			child->computeItemBoundingBox(meshBBox, /*ident,*/ setVisited);
			GeomUtils::unionBBox(bbox, meshBBox/*, parentTransform*/);
		}
	}

	const std::vector<shared_ptr<carve::input::VertexData> >& getVertexPoints() { return m_vertex_points; }

	bool hasItemDataGeometricRepresentation(bool includeChildren, bool includeLinesPointsAndText ) const
	{
		if (m_meshsets.size() > 0) return true;
		if (m_meshsets_open.size() > 0) return true;
		if (includeLinesPointsAndText)
		{
			if (m_text_literals.size() > 0) return true;
			if (m_vertex_points.size() > 0) return true;
			if (m_polylines.size() > 0) return true;
		}

		if (includeChildren)
		{
			for (auto child : m_child_items)
			{
				if (child->hasItemDataGeometricRepresentation(includeChildren, includeLinesPointsAndText))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool hasItemDataGeometricStyles(bool includeChildren) const
	{
		if (m_styles.size() > 0) return true;

		if (includeChildren)
		{
			for (auto child : m_child_items)
			{
				if (child->hasItemDataGeometricStyles(includeChildren))
				{
					return true;
				}
			}
		}
		return false;
	}

	void addGeometricChildItem(shared_ptr<ItemShapeData>& item_data, const shared_ptr<ItemShapeData>& ptr_self)
	{
		if (ptr_self.get() != this)
		{
			std::cout << __FUNCTION__ << "ptr_self != this" << std::endl;
		}
		m_child_items.push_back(item_data);
		item_data->m_parentItem = ptr_self;
	}
};

/**
*\brief Class ProductShapeData: holds input data of one IFC element, its representations, and the gometric items of that representation.
* Typical hierarchy of IFC geometric shapes, stored by ProductShapeData:
*        ...
*          |-> ProductShapeData [1...n]                 logical element like IfcBuildingStorey
*                   |-> ProductShapeData [1...n]              physical element like IfcWall
*                           |-> ProductShapeData [1...n]            representation
*                                     |-> ProductShapeData [1...n]       geometric item
*/
class ProductShapeData 
{
protected:
	std::vector<shared_ptr<ItemShapeData> >		m_geometric_items;		// Items of geometric representations
	std::vector<shared_ptr<ProductShapeData> >	m_child_products;	// IfcElementAssembly for example can have child products
	std::vector<shared_ptr<StyleData> >			m_styles;			// styles can be attached to Products, as well as to geometric items

public:
	std::string										m_entity_guid;
	bool											m_added_to_spatial_structure = false;
	weak_ptr<IFC4X3::IfcObjectDefinition>			m_ifc_object_definition;
	weak_ptr<IFC4X3::IfcObjectPlacement>			m_object_placement;
	
	weak_ptr<ProductShapeData>						m_parent;
	std::vector<shared_ptr<TransformData> >			m_transforms;

	ProductShapeData() {}
	ProductShapeData( std::string entity_guid ) : m_entity_guid(entity_guid) { }

	const std::vector<shared_ptr<ProductShapeData> >& getChildElements() { return m_child_products; }
	const std::vector<shared_ptr<ItemShapeData> >& getGeometricItems() { return m_geometric_items; }
	const std::vector<shared_ptr<StyleData> >& getStyles() { return m_styles; }
	void clearGeometricChildItems() { m_geometric_items.clear(); }

	void addGeometricItem(shared_ptr<ItemShapeData>& item, const shared_ptr<ProductShapeData>& ptr_self)
	{
		if (ptr_self.get() != this)
		{
			std::cout << __FUNCTION__ << ": ptr_self.get() != this" << std::endl;
		}
		m_geometric_items.push_back(item);
		item->m_product = ptr_self;
	}

	void addStyle(shared_ptr<StyleData>& newStyle)
	{
		if (!newStyle)
		{
			return;
		}
		int append_id = newStyle->m_step_style_id;
		for (size_t ii = 0; ii < m_styles.size(); ++ii)
		{
			shared_ptr<StyleData>& style = m_styles[ii];
			if (style->m_step_style_id == append_id)
			{
				return;
			}
		}
		m_styles.push_back(newStyle);
	}

	void clearStyleData()
	{
		m_styles.clear();
	}

	void clearMeshGeometry(bool clearChildElements)
	{
		m_styles.clear();
		m_object_placement.reset();
		
		for( size_t item_i = 0; item_i < m_geometric_items.size(); ++item_i )
		{
			shared_ptr<ItemShapeData>& item_data = m_geometric_items[item_i];
			item_data->clearItemMeshGeometry();
		}

		if (clearChildElements)
		{
			for (size_t item_i = 0; item_i < m_child_products.size(); ++item_i)
			{
				shared_ptr<ProductShapeData>& product_data = m_child_products[item_i];
				product_data->clearMeshGeometry(clearChildElements);
			}
		}
	}

	void clearAll()
	{
		clearMeshGeometry(true);
		m_styles.clear();
		m_ifc_object_definition.reset();
		m_object_placement.reset();
		m_child_products.clear();
		m_geometric_items.clear();
	}

	void releaseIfcObjects()
	{
		for (size_t ii = 0; ii < m_child_products.size(); ++ii)
		{
			const shared_ptr<ProductShapeData>& existing_child = m_child_products[ii];
			existing_child->releaseIfcObjects();
		}

		for (size_t ii = 0; ii < m_geometric_items.size(); ++ii)
		{
			const shared_ptr<ItemShapeData>& geomItem = m_geometric_items[ii];
			geomItem->releaseIfcObjects();
		}
	}
	
	bool isContainedInParentsList( shared_ptr<ProductShapeData>& product_data_check )
	{
		if( !m_parent.expired() )
		{
			shared_ptr<ProductShapeData> product_data_parent( m_parent );
			if( product_data_parent == product_data_check )
			{
				return true;
			}
			product_data_parent->isContainedInParentsList( product_data_check );
		}
		return false;
	}

	void addChildProduct( shared_ptr<ProductShapeData>& add_child, shared_ptr<ProductShapeData>& ptr_self )
	{
		if( ptr_self.get() != this )
		{
			std::cout << __FUNCTION__ << ": ptr_self.get() != this" << std::endl;
		}
		if( isContainedInParentsList( add_child ) )
		{
			std::cout << __FUNCTION__ << ": isContainedInParentsList" << std::endl;
			return;
		}

		for( size_t ii = 0; ii < m_child_products.size(); ++ii )
		{
			const shared_ptr<ProductShapeData>& existing_child = m_child_products[ii];
			if( existing_child == add_child )
			{
#ifdef _DEBUG
				if( existing_child->m_entity_guid.compare( add_child->m_entity_guid ) == 0 )
				{
					std::cout << __FUNCTION__ << ": child already added, tag: " << add_child->m_entity_guid << std::endl;
				}
				else
				{
					std::cout << __FUNCTION__ << ": tag mismatch: " << add_child->m_entity_guid << " != " << existing_child->m_entity_guid << std::endl;
				}
#endif
				return;
			}
		}

		m_child_products.push_back( add_child );
		add_child->m_parent = ptr_self;
	}

	/**
	* \brief method getTransform: Computes the transformation matrix, that puts the geometry of this product into global coordinates
	* All transformation matrices of all parent coordinate systems are multiplied.
	*/
	carve::math::Matrix getTransform() const
	{
		carve::math::Matrix transform_matrix;
		if( m_transforms.size() > 0 )
		{
			for( const shared_ptr<TransformData>& transform : m_transforms )
			{
				if( transform )
				{
					transform_matrix = transform->m_matrix*transform_matrix;
				}
			}
		}
		return transform_matrix;
	}

	carve::math::Matrix getRelativeTransform(const shared_ptr<ProductShapeData>& other)
	{
		carve::math::Matrix transform_matrix;
		if (!other)
		{
			return transform_matrix;
		}
		
		if (m_transforms.size() > 0)
		{
			std::vector<shared_ptr<TransformData> >	diff_transforms;
			auto it_self = m_transforms.rbegin();
			auto it_other = other->m_transforms.rbegin();
			bool sameSoFar = true;
			for (size_t ii_self = 0; ii_self < m_transforms.size(); ++ii_self)
			{
				if (it_self == m_transforms.rend())
				{
					break;
				}
				
				shared_ptr<TransformData>& transform_self = *it_self;

				if( sameSoFar )
				{
					if( it_other != other->m_transforms.rend() )
					{
						shared_ptr<TransformData>& transform_other = *it_other;

						if( transform_self->m_placement_tag >= 0 && transform_self->m_placement_tag == transform_other->m_placement_tag )
						{
							// skip matrices that are the same at both products, to avoid unnecessary multiplications and numerical inaccuracies
							++it_self;
							++it_other;
							continue;
						}
						sameSoFar = false;
					}
				}
				
				if (transform_self)
				{
					transform_matrix = transform_self->m_matrix*transform_matrix;
				}
				++it_self;
			}
		}
		return transform_matrix;
	}

	void addTransform( shared_ptr<TransformData>& transform_data )
	{
		if( !transform_data )
		{
			return;
		}

		for( auto existing_transform : m_transforms )
		{
			if( existing_transform )
			{
				if( transform_data->isSameTransform( existing_transform ) )
				{
					return;
				}
			}
		}
		m_transforms.insert( m_transforms.begin(), transform_data );
#ifdef _DEBUG
		carve::math::Matrix matrix = transform_data->m_matrix;
		if (matrix.v[0] == 0 && matrix.v[1] == 0 && matrix.v[2] == 0 && matrix.v[3] == 0)
		{
			std::cout << __FUNCTION__ << " check matrix " << std::endl;
		}
#endif
	}

	void applyTransformToProduct( const carve::math::Matrix& matrix, double eps, bool matrix_identity_checked, bool applyToChildren )
	{
		if( !matrix_identity_checked )
		{
			if( GeomUtils::isMatrixIdentity( matrix ) )
			{
				return;
			}
		}
		for( size_t i_item = 0; i_item < m_geometric_items.size(); ++i_item )
		{
			m_geometric_items[i_item]->applyTransformToItem( matrix, eps, true );
		}

		if( applyToChildren )
		{
			for( auto child_product_data : m_child_products)
			{
				child_product_data->applyTransformToProduct(matrix, eps, true, applyToChildren );
			}
		}
	}

	void getAllMeshPoints(std::vector<vec3>& points, bool includeChildren, bool globalCoords) const
	{
		std::vector<vec3> itemPoints;
		for (size_t i_item = 0; i_item < m_geometric_items.size(); ++i_item)
		{
			const shared_ptr<ItemShapeData>& item_data = m_geometric_items[i_item];
			item_data->getAllMeshPoints(itemPoints);
		}

		if (itemPoints.size() > 0)
		{
			if (globalCoords)
			{
				carve::math::Matrix transform = getTransform();
				for( vec3& point : itemPoints)
				{
					vec3 pointGlobal = transform * point;
					point = pointGlobal;
				}
			}
			std::copy(itemPoints.begin(), itemPoints.end(), std::back_inserter(points));
		}

		if (includeChildren)
		{
			for (size_t i_child = 0; i_child < m_child_products.size(); ++i_child)
			{
				const shared_ptr<ProductShapeData>& child_product_data = m_child_products[i_child];
				child_product_data->getAllMeshPoints(points, includeChildren, globalCoords);
			}
		}
	}

	bool isShapeDataEmpty( bool check_also_children ) const
	{
		if(m_geometric_items.size() > 0 )
		{
			return false;
		}

		//if( m_styles.size() > 0 )
		//{
		//	return false;
		//}

		if( check_also_children )
		{
			//if(m_styles.size() > 0 )
			{
				for( size_t ii = 0; ii < m_child_products.size(); ++ii )
				{
					const shared_ptr<ProductShapeData>& child = m_child_products[ii];
					bool child_empty = child->isShapeDataEmpty( check_also_children );
					if( !child_empty )
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	bool hasGeometricRepresentation( bool includeChildProducts, bool includeGeometricChildItems, std::unordered_set<uint32_t>& setIgnoreTypes, bool includeLinesPointsAndText) const
	{
		if(includeChildProducts)
		{
			for( size_t ii = 0; ii < m_child_products.size(); ++ii )
			{
				const shared_ptr<ProductShapeData>& child = m_child_products[ii];

				if (!child->m_ifc_object_definition.expired())
				{
					shared_ptr<IFC4X3::IfcObjectDefinition> ifcObjectDef(child->m_ifc_object_definition);

					if (ifcObjectDef)
					{
						uint32_t ifcType = ifcObjectDef->classID();
						if (setIgnoreTypes.find(ifcType) != setIgnoreTypes.end())
						{
							continue;
						}
					}
				}
				bool childHasGeom = child->hasGeometricRepresentation(includeChildProducts, includeGeometricChildItems, setIgnoreTypes, includeLinesPointsAndText);
				if( childHasGeom )
				{
					return true;
				}
			}
		}

		for (size_t ii = 0; ii < m_geometric_items.size(); ++ii)
		{
			const shared_ptr<ItemShapeData>& geomItem = m_geometric_items[ii];
			if (geomItem->hasItemDataGeometricRepresentation(includeGeometricChildItems, includeLinesPointsAndText))
			{
				return true;
			}
		}

		return false;
	}

	void addGeometricChildItem(shared_ptr<ItemShapeData>& item_data, const shared_ptr<ProductShapeData>& ptr_self)
	{
		if (ptr_self.get() != this)
		{
			std::cout << __FUNCTION__ << "ptr_self != this" << std::endl;
		}
		m_geometric_items.push_back(item_data);
		item_data->m_product = ptr_self;
	}

	void applyTransformToItem(const shared_ptr<TransformData>& transform, double eps, bool matrix_identity_checked )
	{
		if (!transform)
		{
			return;
		}
		if (!matrix_identity_checked)
		{
			if (GeomUtils::isMatrixIdentity(transform->m_matrix))
			{
				return;
			}
		}

		for (auto itemData : m_geometric_items)
		{
			itemData->applyTransformToItem(transform->m_matrix, eps, true);
		}
	}
};

inline bool isEqual(const shared_ptr<carve::mesh::MeshSet<3> >& existingMesh, const shared_ptr<carve::mesh::MeshSet<3> >& checkMesh)
{
	if( existingMesh->meshes.size() != checkMesh->meshes.size() ) { return false; }
	if( existingMesh->vertex_storage.size() != checkMesh->vertex_storage.size() ) { return false; }

	for( size_t ii = 0; ii < existingMesh->meshes.size(); ++ii )
	{
		const carve::mesh::Mesh<3>* mesh1 = existingMesh->meshes[ii];
		const carve::mesh::Mesh<3>* mesh2 = checkMesh->meshes[ii];
		if( mesh1->closed_edges.size() != mesh2->closed_edges.size() ) { return false; }
		if( mesh1->open_edges.size() != mesh2->open_edges.size() ) { return false; }
		if( mesh1->faces.size() != mesh2->faces.size() ) { return false; }

		for( size_t jj = 0; jj < mesh1->faces.size(); ++jj )
		{
			const carve::mesh::Face<3>* face1 = mesh1->faces[jj];
			const carve::mesh::Face<3>* face2 = mesh2->faces[jj];
			if( face1->n_edges != face2->n_edges ) { return false; }

			const carve::mesh::Edge<3>* edge1 = face1->edge;
			const carve::mesh::Edge<3>* edge2 = face2->edge;

			for( size_t kk = 0; kk < face1->n_edges; ++kk )
			{
				const carve::mesh::Vertex<3>* vertex1 = edge1->vert;
				const carve::mesh::Vertex<3>* vertex2 = edge2->vert;
				vec3 delt = vertex1->v - vertex2->v;
				if( delt.x > 0.0001 ) { return false; }
				if( delt.x < -0.0001 ) { return false; }
				if( delt.y > 0.0001 ) { return false; }
				if( delt.y < -0.0001 ) { return false; }
				if( delt.z > 0.0001 ) { return false; }
				if( delt.z < -0.0001 ) { return false; }
				edge1 = edge1->next;
				edge2 = edge2->next;
			}
		}
	}
	return true;
}

inline bool isEqual(const shared_ptr<ItemShapeData>& existingItem, const shared_ptr<ItemShapeData>& checkItem)
{
	if( existingItem->m_meshsets.size() != checkItem->m_meshsets.size() ) { return false; }
	if( existingItem->m_meshsets_open.size() != checkItem->m_meshsets_open.size() ) { return false; }

	for( size_t ii = 0; ii < existingItem->m_meshsets.size(); ++ii )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& meshset1 = existingItem->m_meshsets[ii];
		shared_ptr<carve::mesh::MeshSet<3> >& meshset2 = checkItem->m_meshsets[ii];
		if( !isEqual(meshset1, meshset2) )
		{
			return false;
		}
	}

	for( size_t ii = 0; ii < existingItem->m_meshsets_open.size(); ++ii )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& meshset1 = existingItem->m_meshsets_open[ii];
		shared_ptr<carve::mesh::MeshSet<3> >& meshset2 = checkItem->m_meshsets_open[ii];
		if( !isEqual(meshset1, meshset2) )
		{
			return false;
		}
	}
	return true;
}

static carve::geom::aabb<3> computeBoundingBox(const shared_ptr<ProductShapeData>& productData, bool includeChildren, bool applyTransformToGlobalCoordinates)
{
	carve::geom::aabb<3> bbox;
	if( !productData )
	{
		return bbox;
	}
	
	std::vector<vec3> points;
	productData->getAllMeshPoints(points, includeChildren, applyTransformToGlobalCoordinates);

	bbox = carve::geom::aabb<3>(points.begin(), points.end());
	return bbox;
}

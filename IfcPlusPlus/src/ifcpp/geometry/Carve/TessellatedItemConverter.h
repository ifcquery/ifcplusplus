/* -*-c++-*- IFC++ www.ifcquery.com

MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <ifcpp/IFC4/include/IfcCartesianPointList3D.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/IFC4/include/IfcIndexedPolygonalFace.h>
#include <ifcpp/IFC4/include/IfcIndexedPolygonalFaceWithVoids.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcPolygonalFaceSet.h>
#include <ifcpp/IFC4/include/IfcPositiveInteger.h>
#include <ifcpp/IFC4/include/IfcTessellatedFaceSet.h>
#include <ifcpp/IFC4/include/IfcTriangulatedFaceSet.h>

#include "GeometryInputData.h"

///@brief imports tessellated meshes as carve meshes
//Open tasks & TODOS:
//- check each face for the correct winding order (counter-clockwise for faces, clockwise for holes)
//- check if a face is degenerated (collapsed to a line or a point) and reject it if it is
//- check for crossing edges and reject faces if they contain some
//- check if voids overlap/cross each other and reject them if they do
//- improve run time of mergeHolesIntoPoly by using a spatial structure. Currently
//  uses a naive algorithm, which is probably good enough as most polys have few
//  vertices, and few holes with few vertices (each in the single digit range)
//- check closed parameter and use it to use a different addPolyhedron method
class TessellatedItemConverter : public StatusCallback {
public:
	TessellatedItemConverter(shared_ptr<UnitConverter> const unit_converter):
		m_unit_converter(unit_converter)
	{ }

	void convertTessellatedItem(shared_ptr<IfcTessellatedItem> const tessellated_item,
			shared_ptr<ItemShapeData> item_data)
	{
		//Can be either an IfcPolygonalFaceSet or an IfcTriangulatedFaceSet
		//the former one needs to be triangulated by the user, while the
		//latter one can usually directly be used for rendering
		auto carve_mesh_builder = make_shared<carve::input::PolyhedronData>();
		auto face_set = dynamic_pointer_cast<IfcTessellatedFaceSet>(tessellated_item);
		if(!face_set)
		{
			messageCallback( "Tessellated item is not a face set! Probably a bare polygonal face?",
					StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, tessellated_item.get());
			return;
		}
		auto const& vertices = face_set->m_Coordinates;
		if(!vertices)
		{
			messageCallback( "Tessellated item does not contain any vertices!",
					StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, face_set.get());
			return;
		}
		if(!copyVertices(vertices, carve_mesh_builder))
			return;
		auto const coordinate_count = face_set->m_Coordinates->m_CoordList.size();

		if(auto const poly_face_set = dynamic_pointer_cast<IfcPolygonalFaceSet>(tessellated_item))
			convertPolygonalFaceSet( poly_face_set, coordinate_count, carve_mesh_builder );

		if(auto const tri_face_set = dynamic_pointer_cast<IfcTriangulatedFaceSet>(tessellated_item))
			convertTriangulatedFaceSet( tri_face_set, coordinate_count, carve_mesh_builder );

		item_data->addOpenOrClosedPolyhedron( carve_mesh_builder );
	}

protected:
	bool copyVertices(shared_ptr<IfcCartesianPointList3D> const point_list,
			shared_ptr<carve::input::PolyhedronData> carve_mesh_builder)
	{
		double length_factor = m_unit_converter->getLengthInMeterFactor();
		for(auto const& coord : point_list->m_CoordList)
		{
			if(coord.size() != 3)
			{
				messageCallback( "Coordinates with other dimension than 3 found, skipping entity.",
						StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, point_list.get());
				return false;
			}
			carve::geom3d::Vector carve_point;
			carve_point.x = coord[0]->m_value * length_factor;
			carve_point.y = coord[1]->m_value * length_factor;
			carve_point.z = coord[2]->m_value * length_factor;
			carve_mesh_builder->addVertex(carve_point);
		}
		return true;
	}

	void convertPolygonalFaceSet(shared_ptr<IfcPolygonalFaceSet> const poly_face_set,
			size_t const coordinate_count,
			shared_ptr<carve::input::PolyhedronData> carve_mesh_builder)
	{
		//indexing changes if PnIndex is present:
		//no PnIndex -> CoordIndex of faces is 1-based index into Coordinates
		//PnIndex -> CoordIndex of faces is 1-based index into PnIndex.
		//Value in PnIndex is 1-based index into Coordinates
		std::vector<int> vertex_indices;
		std::vector<std::vector<int>> hole_vertex_indices;
		//using a lambda saves one nested loop
		size_t const pn_index_count = poly_face_set->m_PnIndex.size();
		auto check_and_add = [&vertex_indices,&coordinate_count](auto index)
		{
			if(!index)
				return true;
			if(1 > index->m_value || coordinate_count < index->m_value)
				return true;
			vertex_indices.push_back(index->m_value - 1);
			return false;
		};
		auto check_and_add_indirect = [&](auto pn_index)
		{
			if(!pn_index)
				return true;
			if(1 > pn_index->m_value || pn_index_count < pn_index->m_value)
				return true;
			return check_and_add(poly_face_set->m_PnIndex[pn_index->m_value - 1]);
		};
		for(auto const indexed_face : poly_face_set->m_Faces)
		{
			auto const& coord_index = indexed_face->m_CoordIndex;
			if(!indexed_face ||3 > coord_index.size())
				continue;
			vertex_indices.clear();
			hole_vertex_indices.clear();
			//skip face if any vertex index is invalid or out of range
			if((pn_index_count)
				? std::any_of(coord_index.cbegin(), coord_index.cend(), check_and_add_indirect)
				: std::any_of(coord_index.cbegin(), coord_index.cend(), check_and_add))
				continue;
			if(auto face_with_voids = dynamic_pointer_cast<IfcIndexedPolygonalFaceWithVoids>
					(indexed_face))
			{
				copyHoleIndices(hole_vertex_indices, face_with_voids->m_InnerCoordIndices,
						poly_face_set->m_PnIndex, coordinate_count);
				mergeHolesIntoPoly(vertex_indices, hole_vertex_indices, carve_mesh_builder);
			}
			carve_mesh_builder->addFace(vertex_indices.cbegin(), vertex_indices.cend());
		}
	}

	//also resolves indirect access via pn, giving direct indices into coord list
	void copyHoleIndices(std::vector<std::vector<int>>& hole_indices,
			std::vector<std::vector<shared_ptr<IfcPositiveInteger>>> const& coord_index,
			std::vector<shared_ptr<IfcPositiveInteger>> const& pn_indices,
			size_t const coordinate_count)
	{
		std::vector<int> index_buffer;
		size_t const pn_index_count = pn_indices.size();
		auto check_and_add = [&index_buffer,&coordinate_count](auto index)
		{
			if(!index)
				return true;
			if(1 > index->m_value || coordinate_count < index->m_value)
				return true;
			index_buffer.push_back(index->m_value - 1);
			return false;
		};
		auto check_and_add_indirect = [&](auto pn_index)
		{
			if(!pn_index)
				return true;
			if(1 > pn_index->m_value || pn_index_count < pn_index->m_value)
				return true;
			return check_and_add(pn_indices[pn_index->m_value - 1]);
		};
		for(auto const& hole : coord_index)
		{
			index_buffer.clear();
			if((pn_index_count)
				? std::any_of(hole.cbegin(), hole.cend(), check_and_add_indirect)
				: std::any_of(hole.cbegin(), hole.cend(), check_and_add))
				continue;
			hole_indices.push_back(index_buffer);
		}
	}

	///@brief Merge boundary and hole indices together
	///@details This works because the IFC spec defines the winding order of
	///the boundary to be counter-clock-wise and the winding order of holes
	///to be clock-wise - the same order is used by carve internally. By
	///concatenating the indices and adding a back-edge index pair we basically
	///"cut" the polygon open and define the hole in a way carve recognises it.
	///
	///To find a suitable cutting edge, a naive nearest neighbour search is
	///done. Ths can be improved by using a spatial tree like kd-tree or
	///R-tree.
	void mergeHolesIntoPoly(std::vector<int>& vertex_indices,
			std::vector<std::vector<int>> const& hole_vertex_indices,
			shared_ptr<carve::input::PolyhedronData> carve_mesh_builder)
	{
		//use naive nearest neighbour search to find the boundary vertex closest
		//to the hole vertex and insert an edge, cutting the poly open.
		//Add the hole vertices to the boundary and repeat until no holes remain
		//Can be improved by using a kd- or R-tree (carve ships with kd-trees)
		for(auto hole : hole_vertex_indices)
		{
			auto candidate = std::make_tuple(std::numeric_limits<double>::max(), size_t(0), size_t(0));
			for(size_t i = 0; i < vertex_indices.size(); ++i)
			{
				int v_index = vertex_indices[i];
				auto const& boundary_vertex = carve_mesh_builder->points[v_index];
				for(size_t j = 0; j < hole.size(); ++j)
				{
					int  h_index = hole[j];
					auto const& hole_vertex = carve_mesh_builder->points[h_index];
					double const distance = (hole_vertex - boundary_vertex).length2();
					if(distance < std::get<0>(candidate))
						candidate = std::make_tuple(distance, i, j);
				}
			}
			//now that a minimum distance pair is found, rotate the index vectors
			//so that the pair is right next to each other when concatenating
			//this way an edge is formed. Also add a back-edge after the hole
			std::rotate(vertex_indices.begin(), vertex_indices.begin() + std::get<1>(candidate),
					vertex_indices.end());
			std::rotate(hole.begin(), hole.begin() + std::get<2>(candidate), hole.end());
			hole.push_back(hole.front());
			hole.push_back(vertex_indices.back());
			vertex_indices.insert(vertex_indices.end(), hole.cbegin(), hole.cend());
		}
	}

	void convertTriangulatedFaceSet(shared_ptr<IfcTriangulatedFaceSet> const tri_face_set,
			size_t const coordinate_count,
			shared_ptr<carve::input::PolyhedronData> carve_mesh_builder)
	{
		//indexing changes if PnIndex is present:
		//no PnIndex -> CoordIndex is 1-based index into Coordinates
		//PnIndex -> CoordIndex is 1-based index into PnIndex.
		//Value in PnIndex is 1-based index into Coordinates
		std::vector<int> vertex_indices;
		size_t const pn_index_count = tri_face_set->m_PnIndex.size();
		//using a lambda saves one nested loop
		auto check_and_add = [&vertex_indices,&coordinate_count](auto index)
		{
			if(!index)
				return true;
			if(1 > index->m_value || coordinate_count < index->m_value)
				return true;
			vertex_indices.push_back(index->m_value - 1);
			return false;
		};
		auto check_and_add_indirect = [&](auto pn_index)
		{
			if(!pn_index)
				return true;
			if(1 > pn_index->m_value || pn_index_count < pn_index->m_value)
				return true;
			return check_and_add(tri_face_set->m_PnIndex[pn_index->m_value - 1]);
		};
		for(auto const& tri_index : tri_face_set->m_CoordIndex)
		{
			if(3 != tri_index.size())
				continue;
			vertex_indices.clear();
			//skip face if any vertex index is invalid or out of range
			if((pn_index_count)
				? std::any_of(tri_index.cbegin(), tri_index.cend(), check_and_add_indirect)
				: std::any_of(tri_index.cbegin(), tri_index.cend(), check_and_add))
				continue;
			carve_mesh_builder->addFace(vertex_indices.cbegin(), vertex_indices.cend());
		}
	}

private:
	shared_ptr<UnitConverter> const m_unit_converter;
};


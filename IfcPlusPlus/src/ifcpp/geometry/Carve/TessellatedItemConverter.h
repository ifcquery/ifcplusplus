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
//- add support for holes. It should be possible to incorporate them by cutting the closed face open
//  Idea: 1. find a pair (border_vertex, void_vertex) that has min distance
//        2. rotate border index vector and void index vector so that the pair indices follow
//           each other.
//        3. append the vectors and append the pair indices reversed (as back edge)
//        4. the resulting vector is the new border_vertex index-vector
//        5. repeat 1.-4. if there are more voids
//- check closed parameter and use it to use a different addPolyhedron method
class TessellatedItemConverter : public StatusCallback {
public:
	TessellatedItemConverter(shared_ptr<UnitConverter> const unit_converter):
		m_unit_converter(unit_converter),
		m_length_factor(unit_converter->getLengthInMeterFactor()),
		m_carve_mesh_builder(std::make_shared<carve::input::PolyhedronData>())
	{ }

	void convertTessellatedItem(shared_ptr<IfcTessellatedItem> const tessellated_item,
			shared_ptr<ItemShapeData> item_data)
	{
		//Can be either an IfcPolygonalFaceSet or an IfcTriangulatedFaceSet
		//the former one needs to be triangulated by the user, while the
		//latter one can usually directly be used for rendering
		m_carve_mesh_builder->clearFaces();
		m_carve_mesh_builder->points.clear();
		m_coordinate_count = 0;
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
		if(!copyVertices(vertices))
			return;
		m_coordinate_count = face_set->m_Coordinates->m_CoordList.size();

		if(auto const poly_face_set = dynamic_pointer_cast<IfcPolygonalFaceSet>(tessellated_item))
			convertPolygonalFaceSet( poly_face_set );

		if(auto const tri_face_set = dynamic_pointer_cast<IfcTriangulatedFaceSet>(tessellated_item))
			convertTriangulatedFaceSet( tri_face_set );

		item_data->addOpenOrClosedPolyhedron( m_carve_mesh_builder );
	}

protected:
	bool copyVertices(shared_ptr<IfcCartesianPointList3D> const point_list)
	{
		for(auto const& coord : point_list->m_CoordList)
		{
			if(coord.size() != 3)
			{
				messageCallback( "Coordinates with other dimension than 3 found, skipping entity.",
						StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, point_list.get());
				return false;
			}
			carve::geom3d::Vector carve_point;
			carve_point.x = coord[0]->m_value * m_length_factor;
			carve_point.y = coord[1]->m_value * m_length_factor;
			carve_point.z = coord[2]->m_value * m_length_factor;
			m_carve_mesh_builder->addVertex(carve_point);
		}
		return true;
	}

	void convertPolygonalFaceSet(shared_ptr<IfcPolygonalFaceSet> const poly_face_set)
	{
		//indexing changes if PnIndex is present:
		//no PnIndex -> CoordIndex of faces is 1-based index into Coordinates
		//PnIndex -> CoordIndex of faces is 1-based index into PnIndex.
		//Value in PnIndex is 1-based index into Coordinates
		std::vector<int> vertex_indices;
		//using a lambda saves one nested loop
		size_t const pn_index_count = poly_face_set->m_PnIndex.size();
		auto check_and_add = [&vertex_indices,this](auto index)
		{
			if(!index)
				return true;
			if(1 > index->m_value || m_coordinate_count < index->m_value)
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
			if(auto face_with_voids = dynamic_pointer_cast<IfcIndexedPolygonalFaceWithVoids>
					(indexed_face))
			{
				messageCallback( "Indexed faces with voids are currently not supported",
						StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, face_with_voids.get());
			}
			vertex_indices.clear();
			if((pn_index_count)
				? std::any_of(coord_index.cbegin(), coord_index.cend(), check_and_add_indirect)
				: std::any_of(coord_index.cbegin(), coord_index.cend(), check_and_add))
				continue;
			m_carve_mesh_builder->addFace(vertex_indices.cbegin(), vertex_indices.cend());
		}
	}

	void convertTriangulatedFaceSet(shared_ptr<IfcTriangulatedFaceSet> const tri_face_set)
	{
		//indexing changes if PnIndex is present:
		//no PnIndex -> CoordIndex is 1-based index into Coordinates
		//PnIndex -> CoordIndex is 1-based index into PnIndex.
		//Value in PnIndex is 1-based index into Coordinates
		std::vector<int> vertex_indices;
		if(tri_face_set->m_PnIndex.size())
		{
			size_t const index_count = tri_face_set->m_PnIndex.size();
			for(auto const& tri_index : tri_face_set->m_CoordIndex)
			{
				if(3 != tri_index.size())
					continue;
				vertex_indices.clear();
				//using a lambda saves one nested loop
				auto check_and_add = [&vertex_indices,&index_count,&tri_face_set,this](auto pn_index)
				{
					if(!pn_index)
						return true;
					if(1 > pn_index->m_value || index_count < pn_index->m_value)
						return true;
					auto const& index = tri_face_set->m_PnIndex[pn_index->m_value - 1];
					if(!index)
						return true;
					if(1 > index->m_value || m_coordinate_count < index->m_value)
						return true;
					vertex_indices.push_back(index->m_value - 1);
					return false;
				};
				if(std::any_of(tri_index.cbegin(), tri_index.cend(), check_and_add))
					continue;
				m_carve_mesh_builder->addFace(vertex_indices.cbegin(), vertex_indices.cend());
			}
		}
		else
		{
			for(auto const& tri_index : tri_face_set->m_CoordIndex)
			{
				if(3 != tri_index.size())
					continue;
				vertex_indices.clear();
				//using a lambda saves a nested for loop
				auto check_and_add = [&vertex_indices,this](auto index)
				{
					if(!index) return true;
					if(1 > index->m_value || m_coordinate_count < index->m_value)
						return true;
					vertex_indices.push_back(index->m_value - 1);
					return false;
				};
				if(std::any_of(tri_index.cbegin(), tri_index.cend(), check_and_add))
					continue;
				m_carve_mesh_builder->addFace(vertex_indices.cbegin(), vertex_indices.cend());
			}
		}
	}

private:
	shared_ptr<UnitConverter> const m_unit_converter;
	double const m_length_factor;
	shared_ptr<carve::input::PolyhedronData> m_carve_mesh_builder;
	size_t m_coordinate_count;
};


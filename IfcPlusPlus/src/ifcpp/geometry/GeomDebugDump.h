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

#if defined(_DEBUG) || defined(_DEBUG_RELEASE)

#include <vector>
#include <fstream>
#include <cfloat>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/BuildingGuid.h>
#include <ifcpp/writer/WriterSTEP.h>
#include <ifcpp/geometry/GeometryInputData.h>

#include <IfcAxis2Placement3D.h>
#include <IfcBuilding.h>
#include <IfcBuildingElementProxy.h>
#include <IfcBuildingStorey.h>
#include <IfcCartesianPoint.h>
#include <IfcDirection.h>
#include <IfcDimensionCount.h>
#include <IfcGloballyUniqueId.h>
#include <IfcGeometricRepresentationContext.h>
#include <IfcIdentifier.h>
#include <IfcLabel.h>
#include <IfcLengthMeasure.h>
#include <IfcLocalPlacement.h>
#include <IfcOrganization.h>
#include <IfcOwnerHistory.h>
#include <IfcPerson.h>
#include <IfcPersonAndOrganization.h>
#include <IfcProductDefinitionShape.h>
#include <IfcReal.h>
#include <IfcRelAggregates.h>
#include <IfcRelContainedInSpatialStructure.h>
#include <IfcRelDefinesByProperties.h>
#include <IfcRelVoidsElement.h>
#include <IfcShapeRepresentation.h>
#include <IfcSite.h>
#include <IfcSIUnit.h>
#include <IfcText.h>
#include <IfcUnitAssignment.h>
#include <IfcUnitEnum.h>
#include <IfcWall.h>
#include "IncludeCarveHeaders.h"
#include "PolyInputCache3D.h"
#include "MeshOps.h"
#include "MeshNormalizer.h"

using namespace IFC4X3;

namespace GeomDebugDump
{
	class DumpData
	{
	private:
		DumpData() {}

	public:
		static DumpData& instance()
		{
			static DumpData INSTANCE;
			return INSTANCE;
		}

		double eps_debug_dump = EPS_RANDOM_FACTOR * EPS_M8;
		double dump_y_pos_geom;
		int dumpCount = 0;
		int maxDumpCount = 1000;
		carve::geom::vector<3> labelPos = carve::geom::VECTOR(-0.5, 0, 0);
		carve::geom::vector<3> countLabelPos = carve::geom::VECTOR(0.5, 0, 0);
		std::stringstream strs_buffer;
		int dump_buffering = 0;
	};

	static size_t numM6 = 0;
	static size_t dumpFaceCount = 0;
	static int mergedFacesDumpCount = 0;
	static size_t numPointsAtCorner = 0;
	static bool disableAllDebugDump = false;

	struct DumpSettingsStruct
	{
		DumpSettingsStruct()
		{

		}
		vec4 colorLabel = vec4(0.3, 0.33, 0.33, 1.);
		vec4 colorMesh = vec4(0.4, 0.5, 0.7, 0.4);
		vec4 colorOpenEdges = vec4(0.99, 0.2, 0.2, 0.4);
		vec4 colorValidMesh = vec4(0.3, 0.8, 0.3, 0.4);
		bool triangulateBeforeDump = true;
		double eps = 1.0;
	};

	static void setDumpEnabled(bool enabled)
	{
		disableAllDebugDump = !enabled;
	}

	static size_t getDumpCount()
	{
		if (disableAllDebugDump)
		{
			return 0;
		}
		//if (disableAllDebugDump)
		//{
		//	exit(0);
		//}
		return DumpData::instance().dumpCount;
	}

	static int findVertexIndexInVector(const std::vector<carve::mesh::Vertex<3> >& vec_vertices, const carve::mesh::Vertex<3>* v)
	{
		for (size_t iiv = 0; iiv < vec_vertices.size(); ++iiv)
		{
			const carve::mesh::Vertex<3>& vertex = vec_vertices[iiv];
			if (v == &vertex)
			{
				return iiv;
			}
		}
		return -1;
	}

	static void convertPlacement(double local_x[3], double local_z[3], double location[3], shared_ptr<IfcAxis2Placement3D>& axis2placement3d, std::vector<shared_ptr<BuildingEntity> >& vec_new_entities)
	{
		if (!axis2placement3d)
		{
			axis2placement3d = shared_ptr<IfcAxis2Placement3D>(new IfcAxis2Placement3D());
			vec_new_entities.push_back(axis2placement3d);
		}

		shared_ptr<IfcCartesianPoint> cartesianPoint;
		if (axis2placement3d->m_Location)
		{
			cartesianPoint = dynamic_pointer_cast<IfcCartesianPoint>(axis2placement3d->m_Location);
		}

		if (!cartesianPoint)
		{
			cartesianPoint = shared_ptr<IfcCartesianPoint>(new IfcCartesianPoint());
			axis2placement3d->m_Location = cartesianPoint;
			vec_new_entities.push_back(axis2placement3d->m_Location);
		}
		cartesianPoint->m_Coordinates[0] = location[0];
		cartesianPoint->m_Coordinates[1] = location[1];
		cartesianPoint->m_Coordinates[2] = location[2];

		if (!axis2placement3d->m_Axis)
		{
			axis2placement3d->m_Axis = shared_ptr<IfcDirection>(new IfcDirection());
			vec_new_entities.push_back(axis2placement3d->m_Axis);
		}
		axis2placement3d->m_Axis->m_DirectionRatios.clear();
		axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[0])));
		axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[1])));
		axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[2])));

		if (!axis2placement3d->m_RefDirection)
		{
			axis2placement3d->m_RefDirection = shared_ptr<IfcDirection>(new IfcDirection());
			vec_new_entities.push_back(axis2placement3d->m_RefDirection);
		}

		axis2placement3d->m_RefDirection->m_DirectionRatios.clear();
		axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[0])));
		axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[1])));
		axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[2])));
	}

	static void startBuffering()
	{
		if (disableAllDebugDump)
		{
			return;
		}
		++DumpData::instance().dump_buffering;
		if (DumpData::instance().strs_buffer.str().size() > 0)
		{
			std::ofstream dump_ofstream("dump_mesh_debug.txt", std::ofstream::app);
			dump_ofstream << DumpData::instance().strs_buffer.str().c_str();
			dump_ofstream.close();
		}
	}

	static void clearBuffer()
	{
		if (disableAllDebugDump)
		{
			return;
		}
		DumpData::instance().strs_buffer.str(std::string());
	}

	static void stopBuffering()
	{
		if (disableAllDebugDump)
		{
			return;
		}
		--DumpData::instance().dump_buffering;
		if (DumpData::instance().dump_buffering < 0)
		{
			DumpData::instance().dump_buffering = 0;
		}
		if (DumpData::instance().strs_buffer.str().size() > 0)
		{
			std::ofstream dump_ofstream("dump_mesh_debug.txt", std::ofstream::app);
			dump_ofstream << DumpData::instance().strs_buffer.str().c_str();
			dump_ofstream.close();
			DumpData::instance().strs_buffer.clear();
		}
	}

	static void clearMeshsetDump()
	{
		std::ofstream dump_ofstream("dump_mesh_debug.txt", std::ofstream::trunc);
		dump_ofstream.close();
		DumpData::instance().dump_y_pos_geom = 0;
	}

	class ScopedDumpBuffering
	{
	public:
		ScopedDumpBuffering()
		{
			startBuffering();
		}
		~ScopedDumpBuffering()
		{
			stopBuffering();
		}
	};

	static void appendToOutput(const std::stringstream& strs_out)
	{
		if (disableAllDebugDump)
		{
			return;
		}

		if (DumpData::instance().maxDumpCount > 0)
		{
			if (DumpData::instance().dumpCount > DumpData::instance().maxDumpCount)
			{
				clearMeshsetDump();
				DumpData::instance().dumpCount = 0;
			}
		}
		++DumpData::instance().dumpCount;

		if (DumpData::instance().dump_buffering > 0)
		{
			DumpData::instance().strs_buffer << strs_out.str();
		}
		else
		{
			std::ofstream dump_ofstream("dump_mesh_debug.txt", std::ofstream::app);
			dump_ofstream << strs_out.str().c_str();
			dump_ofstream.close();
		}
	}

	static void dumpPolyline(const std::vector<vec3>& vec_polyline, const vec4& color, double lineThickness, bool move_dump_position, bool depthTestOff)
	{
		if (disableAllDebugDump)
		{
			return;
		}
		if (vec_polyline.size() < 1)
		{
			return;
		}

		std::stringstream strs_out;
		strs_out << "Polyline{" << std::endl;
		strs_out << "thickness: " << lineThickness << ", " << std::endl;
		strs_out << "color{" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "}," << std::endl;
		if (depthTestOff)
		{
			strs_out << "depthTest: \"off\"," << std::endl;
		}

		strs_out << "vertices{" << std::endl;

		double min_y = 0;
		double max_y = 0;
		const size_t num_vertices = vec_polyline.size();
		for (size_t i = 0; i < num_vertices; ++i)
		{
			const vec3& vertex = vec_polyline[i];
			if (i > 0)
			{
				strs_out << ",";
				if (vertex.y < min_y) min_y = vertex.y;
				else if (vertex.y > max_y) max_y = vertex.y;
			}
			else
			{
				min_y = vertex.y;
				max_y = vertex.y;
			}
			strs_out << "{" << vertex.x << ", " << vertex.y + DumpData::instance().dump_y_pos_geom << ", " << vertex.z << "}";
		}
		strs_out << "}" << std::endl;  // vertices
		if (move_dump_position)
		{
			double deltaDump = (max_y - min_y) * 1.5;
			if (deltaDump < 0)
			{
				std::cout << "if (deltaDump < 0)" << std::endl;
			}
			DumpData::instance().dump_y_pos_geom += deltaDump;
			if (max_y > DumpData::instance().dump_y_pos_geom)
			{
				DumpData::instance().dump_y_pos_geom = max_y * 1.1;
			}
		}

		strs_out << std::endl << "}" << std::endl;  // Polyline
		appendToOutput(strs_out);
	}

	static void dumpPolyline(const std::vector<std::vector<vec3> >& loops_3d_input, const vec4& color, double lineThickness, bool move_dump_position, bool depthTestOff)
	{
		for (const std::vector<vec3>& vec_polyline : loops_3d_input)
		{
			dumpPolyline(vec_polyline, color, lineThickness, false, depthTestOff);
		}

		if (move_dump_position)
		{
			vec3 min, max;
			GeomUtils::polygonBbox(loops_3d_input, min, max);
			DumpData::instance().dump_y_pos_geom += (max.y - min.y) * 1.5;
		}
	}

	static void dumpPolyline(const std::vector<glm::dvec3>& loop_3d_input, const vec4& color, double lineThickness, bool move_dump_position, bool depthTestOff)
	{
		std::vector<vec3> poly_3d_carve;
		for (const glm::dvec3& point : loop_3d_input)
		{
			poly_3d_carve.push_back(carve::geom::VECTOR(point.x, point.y, point.z));
		}
		dumpPolyline(poly_3d_carve, color, lineThickness, move_dump_position, depthTestOff);
	}

	static void dumpPolyline(const std::vector<vec2>& vec_polyline, const vec4& color, double lineThickness, bool move_dump_position, bool depthTestOff)
	{
		if (vec_polyline.size() < 1)
		{
			return;
		}

		std::vector<vec3> vec_polyline3d;
		for (size_t i = 0; i < vec_polyline.size(); ++i)
		{
			const vec2& vertex = vec_polyline[i];
			vec_polyline3d.push_back(carve::geom::VECTOR(vertex.x, vertex.y, 0));
		}

		dumpPolyline(vec_polyline3d, color, lineThickness, move_dump_position, depthTestOff);
	}

	static void dumpPolyline(const std::vector<std::vector<vec2> >& loops_2d, const vec4& color, bool move_dump_position, bool depthTestOff)
	{
		std::stringstream strs_out;
		double min_y = DBL_MAX;
		double max_y = -DBL_MAX;

		if (DumpData::instance().maxDumpCount > 0)
		{
			if (DumpData::instance().dumpCount > DumpData::instance().maxDumpCount)
			{
				return;
			}
		}

		for (size_t ii = 0; ii < loops_2d.size(); ++ii)
		{
			const std::vector<vec2>& vec_polyline = loops_2d[ii];

			if (vec_polyline.size() == 0)
			{
				continue;
			}

			strs_out << "Polyline{" << std::endl;
			strs_out << "color{" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "}," << std::endl;
			if (depthTestOff)
			{
				strs_out << "depthTest: \"off\"," << std::endl;
			}

			strs_out << "vertices{" << std::endl;

			const size_t num_vertices = vec_polyline.size();
			for (size_t i = 0; i < num_vertices; ++i)
			{
				const vec2& vertex = vec_polyline[i];
				if (i > 0)
				{
					strs_out << ",";
					if (vertex.y < min_y) min_y = vertex.y;
					else if (vertex.y > max_y) max_y = vertex.y;
				}
				else
				{
					min_y = vertex.y;
					max_y = vertex.y;
				}
				strs_out << "{" << vertex.x << ", " << vertex.y + DumpData::instance().dump_y_pos_geom << "}";
			}
			strs_out << "}" << std::endl;  // vertices
			strs_out << std::endl << "}" << std::endl;  // Polyline
		}

		if (move_dump_position)
		{
			DumpData::instance().dump_y_pos_geom += (max_y - min_y) * 1.5;
			if (max_y > DumpData::instance().dump_y_pos_geom)
			{
				DumpData::instance().dump_y_pos_geom = max_y * 1.1;
			}
		}

		appendToOutput(strs_out);
	}

	static void dumpPolyline(const std::vector<array2d>& loops_2d_input, const vec4& color, double lineThickness, bool move_dump_position, bool depthTestOff)
	{
		std::vector<vec2> vec2d;
		for (const array2d& vertex : loops_2d_input)
		{
			vec2d.push_back(carve::geom::VECTOR(vertex[0], vertex[1]));
		}
		dumpPolyline(vec2d, color, lineThickness, move_dump_position, depthTestOff);
	}

	static void dumpPolyline(const std::vector<std::vector<array2d> >& loops_2d_input, const vec4& color, bool move_dump_position, bool depthTestOff)
	{
		std::vector<std::vector<vec2> > loops_2d;
		for (const std::vector<array2d>& vec_polyline : loops_2d_input)
		{
			std::vector<vec2> vec2d;

			for (const array2d& vertex : vec_polyline)
			{
				vec2d.push_back(carve::geom::VECTOR(vertex[0], vertex[1]));
			}

			loops_2d.push_back(vec2d);
		}

		dumpPolyline(loops_2d, color, move_dump_position, depthTestOff);
	}

	static void dumpCoordinateSystem(carve::math::Matrix m, double size, bool move_dump_position)
	{
		vec3 center = m * carve::geom::VECTOR( 0, 0, 0);
		vec3 positiveX = m * carve::geom::VECTOR(size * 0.25, 0, 0);
		std::vector<vec3> polyline = { center , positiveX };
		vec4 colorX(0.94, 0.3, 0.3, 0.6);
		dumpPolyline(polyline, colorX, 2.0, false, false);

		vec3 positiveY = m * carve::geom::VECTOR(0, size * 0.35, 0);
		polyline = { center , positiveY };
		vec4 colorY(0.3, 0.93, 0.3, 0.6);
		dumpPolyline(polyline, colorY, 2.0, false, false);

		vec4 color2(0.3, 0.3, 0.94, 0.6);
		vec3 positiveZ = m * carve::geom::VECTOR(0, 0, size * 0.5);
		polyline = { center , positiveZ };
		dumpPolyline(polyline, color2, 3.0, false, false);
	}

	static void MeshSet2Stream(const carve::mesh::MeshSet<3>* meshsetInput, const vec3& offset, const vec4& color, std::stringstream& strs_out, bool dumpOpenEdges, bool trianglesAndQuadsOnly = false)
	{
		if (!meshsetInput)
		{
			return;
		}

		// vertices of the meshset:
		size_t vertex_count = 0;
		size_t face_count = 0;

		const carve::mesh::MeshSet<3>* meshset = meshsetInput;
		const std::vector<carve::mesh::Vertex<3> >& vec_vertices = meshset->vertex_storage;

		if (vec_vertices.size() > 10000)
		{
			return;
		}
		std::stringstream strs_open_edges;
		strs_out << "Polyhedron{" << std::endl;
		strs_out << "color{" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "}" << std::endl;

		std::stringstream strs_vertices;
		std::stringstream strs_labels;
		std::stringstream strs_faces;
		double scale_length_factor = 1.0;
		std::vector<std::vector<vec3> > vecOpenEdgePolylines;

		for (size_t iiv = 0; iiv < vec_vertices.size(); ++iiv)
		{
			const carve::mesh::Vertex<3>& vertex = vec_vertices[iiv];
			if (vertex_count > 0)
			{
				strs_vertices << ",   ";
				strs_labels << ",   ";
			}
			double x = (vertex.v.x + offset.x) * scale_length_factor;
			double y = (vertex.v.y + offset.y) * scale_length_factor;
			double z = (vertex.v.z + offset.z) * scale_length_factor;

			strs_vertices << "{" << x << "," << y << "," << z << "}" << std::endl;
			strs_labels << "{\"" << iiv << "\", " << x << "," << y << "," << z << "}" << std::endl;
			++vertex_count;
		}

		std::map<size_t, size_t> mapVertices;
		for (size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh)
		{
			const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

			if (!mesh)
			{
				continue;
			}

			for (auto openEdge : mesh->open_edges)
			{
				if (!openEdge->face)
				{
					continue;
				}

				vec3 v1 = openEdge->v1()->v;
				vec3 v2 = openEdge->v2()->v;
				vec3 v1n = v1 + openEdge->face->plane.N * 0.01;
				vec3 v2n = v2 + openEdge->face->plane.N * 0.01;

				vecOpenEdgePolylines.push_back({ v1, v1n, v2n, v2 });
			}

			// faces:
			const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
			for (size_t i_face = 0; i_face < vec_faces.size(); ++i_face)
			{
				const carve::mesh::Face<3>* face = vec_faces[i_face];
				if (face)
				{
					int number_of_edges = face->n_edges;

					if (number_of_edges == 3)
					{
						if (face_count > 0)
						{
							strs_faces << ",   ";
						}
						carve::mesh::Edge<3>* edge0 = face->edge;
						carve::mesh::Edge<3>* edge1 = edge0->next;
						carve::mesh::Edge<3>* edge2 = edge1->next;
						carve::mesh::Vertex<3>* v0 = edge0->vert;
						carve::mesh::Vertex<3>* v1 = edge1->vert;
						carve::mesh::Vertex<3>* v2 = edge2->vert;
						int v0index = findVertexIndexInVector(vec_vertices, v0);
						int v1index = findVertexIndexInVector(vec_vertices, v1);
						int v2index = findVertexIndexInVector(vec_vertices, v2);

						if (v0index < 0 || v1index < 0 || v2index < 0)
						{
							std::cout << "vertex not found " << std::endl;
							continue;
						}
						mapVertices[v0index] = v0index;
						mapVertices[v1index] = v1index;
						mapVertices[v2index] = v2index;
						strs_faces << "{" << v0index << "," << v1index << "," << v2index << "}" << std::endl;
						++face_count;
					}
					else if (number_of_edges == 4)
					{
						if (face_count > 0)
						{
							strs_faces << ",   ";
						}
						carve::mesh::Edge<3>* edge0 = face->edge;
						carve::mesh::Edge<3>* edge1 = edge0->next;
						carve::mesh::Edge<3>* edge2 = edge1->next;
						carve::mesh::Edge<3>* edge3 = edge2->next;
						carve::mesh::Vertex<3>* v0 = edge0->vert;
						carve::mesh::Vertex<3>* v1 = edge1->vert;
						carve::mesh::Vertex<3>* v2 = edge2->vert;
						carve::mesh::Vertex<3>* v3 = edge3->vert;
						int v0index = findVertexIndexInVector(vec_vertices, v0);
						int v1index = findVertexIndexInVector(vec_vertices, v1);
						int v2index = findVertexIndexInVector(vec_vertices, v2);
						int v3index = findVertexIndexInVector(vec_vertices, v3);

						if (v0index < 0 || v1index < 0 || v2index < 0 || v3index < 0)
						{
							std::cout << "vertex not found " << std::endl;
							continue;
						}
						mapVertices[v0index] = v0index;
						mapVertices[v1index] = v1index;
						mapVertices[v2index] = v2index;
						mapVertices[v3index] = v3index;
						strs_faces << "{" << v0index << "," << v1index << "," << v2index << "," << v3index << "}" << std::endl;
						++face_count;
					}
					else
					{
						if (!trianglesAndQuadsOnly)
						{
							if (face_count > 0)
							{
								strs_faces << ",   ";
							}
							carve::mesh::Edge<3>* edge0 = face->edge;
							strs_faces << "{";

							for (size_t edge_ii = 0; edge_ii < face->n_edges; ++edge_ii)
							{
								carve::mesh::Vertex<3>* v0 = edge0->vert;

								int v0index = findVertexIndexInVector(vec_vertices, v0);

								if (v0index < 0)
								{
									std::cout << "vertex not found " << std::endl;
									continue;
								}

								mapVertices[v0index] = v0index;

								if (edge_ii > 0)
								{
									strs_faces << ",";
								}
								strs_faces << v0index;

								edge0 = edge0->next;
							}

							strs_faces << "}" << std::endl;
							++face_count;
						}
					}
				}
			}
		}

		strs_out << "vertices{" << strs_vertices.str().c_str() << "}" << std::endl;
		strs_out << "faces{" << strs_faces.str().c_str() << "}" << std::endl;
		strs_out << "labels{" << strs_labels.str().c_str() << "}" << std::endl;
		strs_out << std::endl << "}" << std::endl;  // Polyhedron

		if (vecOpenEdgePolylines.size() > 0 && dumpOpenEdges)
		{
			vec4 colorRed(0.6, 0.2, 0.2, 1.0);
			dumpPolyline(vecOpenEdgePolylines, colorRed, 0, false, false);
		}
	}

	static void Mesh2Stream(const carve::mesh::Mesh<3>* mesh, const vec3& offset, const vec4& color, std::stringstream& strs_out, double eps, bool trianglesAndQuadsOnly = false)
	{
		if (!mesh)
		{
			return;
		}

		if (DumpData::instance().maxDumpCount > 0)
		{
			if (DumpData::instance().dumpCount > DumpData::instance().maxDumpCount)
			{
				return;
			}
		}
		PolyInputCache3D poly_data(eps);

		const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
		for (size_t i_face = 0; i_face < vec_faces.size(); ++i_face)
		{
			const carve::mesh::Face<3>* face = vec_faces[i_face];
			if (face)
			{
				int number_of_edges = face->n_edges;

				if (number_of_edges == 3)
				{
					carve::mesh::Edge<3>* edge0 = face->edge;
					carve::mesh::Edge<3>* edge1 = edge0->next;
					carve::mesh::Edge<3>* edge2 = edge1->next;
					carve::mesh::Vertex<3>* v0 = edge0->vert;
					carve::mesh::Vertex<3>* v1 = edge1->vert;
					carve::mesh::Vertex<3>* v2 = edge2->vert;

					size_t idx0 = poly_data.addPoint(v0->v);
					size_t idx1 = poly_data.addPoint(v1->v);
					size_t idx2 = poly_data.addPoint(v2->v);

					poly_data.m_poly_data->addFace(idx0, idx1, idx2);
				}
				else if (number_of_edges == 4)
				{
					carve::mesh::Edge<3>* edge0 = face->edge;
					carve::mesh::Edge<3>* edge1 = edge0->next;
					carve::mesh::Edge<3>* edge2 = edge1->next;
					carve::mesh::Edge<3>* edge3 = edge2->next;
					carve::mesh::Vertex<3>* v0 = edge0->vert;
					carve::mesh::Vertex<3>* v1 = edge1->vert;
					carve::mesh::Vertex<3>* v2 = edge2->vert;
					carve::mesh::Vertex<3>* v3 = edge3->vert;

					size_t idx0 = poly_data.addPoint(v0->v);
					size_t idx1 = poly_data.addPoint(v1->v);
					size_t idx2 = poly_data.addPoint(v2->v);
					size_t idx3 = poly_data.addPoint(v3->v);

					poly_data.m_poly_data->addFace(idx0, idx1, idx2);
					poly_data.m_poly_data->addFace(idx2, idx3, idx0);
				}
				else
				{
					if (!trianglesAndQuadsOnly)
					{
						carve::mesh::Edge<3>* edge0 = face->edge;
						std::vector<size_t> faceIndexes;

						for (size_t edge_ii = 0; edge_ii < face->n_edges; ++edge_ii)
						{
							carve::mesh::Vertex<3>* v0 = edge0->vert;
							size_t idx0 = poly_data.addPoint(v0->v);
							faceIndexes.push_back(idx0);

							edge0 = edge0->next;
						}

						//std::cout << "skip dump face with > 4 vertices" << std::endl;

						poly_data.m_poly_data->faceIndices.push_back(faceIndexes.size());
						std::copy(faceIndexes.begin(), faceIndexes.end(), std::back_inserter(poly_data.m_poly_data->faceIndices));
						poly_data.m_poly_data->faceCount = poly_data.m_poly_data->faceCount + 1;
					}
				}
			}
		}

		shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data.m_poly_data->createMesh(carve::input::opts(), eps));
		MeshSet2Stream(meshset.get(), offset, color, strs_out, trianglesAndQuadsOnly);
	}

	static void dumpMeshsets(const std::vector<carve::mesh::MeshSet<3>* >& vec_meshsets, const vec3& offset, const std::vector<vec4 >& vec_colors, bool dumpOpenEdges)
	{
		std::stringstream strs_out;
		for (size_t i = 0; i < vec_meshsets.size(); ++i)
		{
			carve::mesh::MeshSet<3>* meshset = vec_meshsets[i];
			vec4 color(0.5, 0.5, 0.5, 1.0);
			if (i < vec_colors.size())
			{
				color = vec_colors[i];
			}
			MeshSet2Stream(meshset, offset, color, strs_out, dumpOpenEdges);
		}

		appendToOutput(strs_out);
	}

	static void dumpMeshsetsNormalVectors(const carve::mesh::MeshSet<3>* meshset, const vec3& offset, const vec4& color)
	{
		std::vector< std::vector <vec3> > vecLines;
		for (size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh)
		{
			const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

			if (!mesh)
			{
				continue;
			}
			// faces:
			const std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
			for (size_t i_face = 0; i_face < vec_faces.size(); ++i_face)
			{
				const carve::mesh::Face<3>* face = vec_faces[i_face];
				if (face)
				{
					vec3 c = face->centroid();
					vec3 normal = face->plane.N;

					std::vector <vec3> line = { c + normal * 0.001, c + normal * 0.01 };
					vecLines.push_back(line);
				}
			}
		}

		//if (vec_colors.size() > 0)
		{
			dumpPolyline(vecLines, color, 0, false, false);
		}
	}

	static void dumpMeshset(const carve::mesh::MeshSet<3>* meshset, const vec3& offset, const vec4& color, bool dumpOpenEdges)
	{
		std::stringstream strs_out;
		MeshSet2Stream(meshset, offset, color, strs_out, dumpOpenEdges);

		appendToOutput(strs_out);
	}

	static void dumpTrianglesAndQuads(const carve::mesh::MeshSet<3>* meshset, const vec3& offset, const vec4& color)
	{
		std::stringstream strs_out;
		MeshSet2Stream(meshset, offset, color, strs_out, true);

		appendToOutput(strs_out);
	}

	static void dumpTrianglesAndQuads(const carve::mesh::Mesh<3>* mesh, const vec3& offset, const vec4& color)
	{
		std::stringstream strs_out;
		Mesh2Stream(mesh, offset, color, strs_out, true);

		appendToOutput(strs_out);
	}

	static void dumpBBox(carve::geom::aabb<3>& bbox, vec4& color1, bool moveOffset)
	{
		std::vector<carve::geom::vector<3> > vertice_points;

		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z + bbox.extent.z));
		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z + bbox.extent.z));
		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z + bbox.extent.z));
		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z + bbox.extent.z));
		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z + bbox.extent.z));

		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z - bbox.extent.z));
		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z - bbox.extent.z));

		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z + bbox.extent.z));
		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z - bbox.extent.z));

		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z - bbox.extent.z));

		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z + bbox.extent.z));
		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z - bbox.extent.z));

		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z - bbox.extent.z));

		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z + bbox.extent.z));
		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z - bbox.extent.z));

		vertice_points.push_back(carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z - bbox.extent.z));

		dumpPolyline(vertice_points, color1, 0, moveOffset, false);
	}

	static void moveOffset(double deltaY)
	{
		if (disableAllDebugDump)
		{
			return;
		}
		if (deltaY > 0.000001)
		{
			DumpData::instance().dump_y_pos_geom += deltaY;
		}
	}

	static void moveOffset(const carve::geom::aabb<3>& bbox)
	{
		double dy = bbox.extent.y * 2.2;
		moveOffset(dy);
	}

	static void moveOffset(const shared_ptr<carve::mesh::MeshSet<3> >& meshset)
	{
		carve::geom::aabb<3> bbox = meshset->getAABB();
		moveOffset(bbox);
	}

	static void moveOffset(const carve::mesh::MeshSet<3>* meshset)
	{
		carve::geom::aabb<3> bbox = meshset->getAABB();
		moveOffset(bbox);
	}

	static void moveOffset(const carve::mesh::Mesh<3>* mesh)
	{
		carve::geom::aabb<3> bbox = mesh->getAABB();
		moveOffset(bbox);
	}

	static void dumpMeshset(carve::mesh::MeshSet<3>* meshset, const vec4& color, bool dumpOpenEdges, bool move_offset)
	{
		vec3 offset = carve::geom::VECTOR(0, DumpData::instance().dump_y_pos_geom, 0);
		dumpMeshset(meshset, offset, color, dumpOpenEdges);

		if (move_offset)
		{
			moveOffset(meshset);
		}
	}

	static void dumpMeshset(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, const vec4& color, bool drawNormals, bool dumpOpenEdges, bool move_offset)
	{
		if (!meshset)
		{
			return;
		}
		if (meshset->meshes.size() == 0)
		{
			return;
		}

		carve::geom::aabb<3> bbox = meshset->getAABB();
		if (bbox.extent.length() > 5.0)
		{
			//std::cout << "extent.length: " << bbox.extent.length() << std::endl;
		}

		vec3 offset = carve::geom::VECTOR(0, DumpData::instance().dump_y_pos_geom, 0);
		dumpMeshset(meshset.get(), offset, color, dumpOpenEdges);

		if (drawNormals)
		{
			dumpMeshsetsNormalVectors(meshset.get(), offset, color);
		}

		if (move_offset)
		{
			moveOffset(meshset.get());
		}
	}

	static void dumpMeshsets(const std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vecMeshsets, const vec4& color, bool dumpOpenEdges, bool move_offset)
	{
		vec3 offset = carve::geom::VECTOR(0, DumpData::instance().dump_y_pos_geom, 0);
		carve::geom::aabb<3> bbox;
		for (auto meshset : vecMeshsets)
		{
			dumpMeshset(meshset.get(), offset, color, dumpOpenEdges);

			if (move_offset)
			{
				if (bbox.isEmpty())
				{
					bbox = meshset->getAABB();
				}
				else
				{
					bbox.unionAABB(meshset->getAABB());
				}
			}
		}
		if (move_offset)
		{
			moveOffset(bbox);
		}
	}

	static void dumpMesh(const carve::mesh::Mesh<3>* mesh, const vec4& color, bool move_offset = true)
	{
		if (!mesh)
		{
			return;
		}

		vec3 offset = carve::geom::VECTOR(0, DumpData::instance().dump_y_pos_geom, 0);
		std::stringstream strs_out;
		Mesh2Stream(mesh, offset, color, strs_out, true);

		appendToOutput(strs_out);

		if (move_offset)
		{
			moveOffset(mesh);
		}
	}

	static void dumpPoints(const std::vector<glm::dvec3>& points, const vec4& color, bool move_offset = true)
	{
		PolyInputCache3D poly_cache;
		for (size_t ii = 0; ii < points.size(); ++ii)
		{
			const glm::dvec3& point = points[ii];
			poly_cache.addPoint(carve::geom::VECTOR(point.x, point.y, point.z));
		}
		shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_cache.m_poly_data->createMesh(carve::input::opts(), DumpData::instance().eps_debug_dump));

		bool drawNormals = false;
		bool dumpOpenEdges = false;
		dumpMeshset(meshset, color, drawNormals, dumpOpenEdges, move_offset);
	}

	static void dumpTriangle(const vec3& point1, const vec3& point2, const vec3& point3, const vec4& color, bool move_offset )
	{
		PolyInputCache3D poly_cache;
		size_t idx1 = poly_cache.addPoint(point1);
		size_t idx2 = poly_cache.addPoint(point2);
		size_t idx3 = poly_cache.addPoint(point3);
		poly_cache.m_poly_data->addFace(idx1, idx2, idx3);

		shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_cache.m_poly_data->createMesh(carve::input::opts(), DumpData::instance().eps_debug_dump));
		bool drawNormals = false;
		bool dumpOpenEdges = false;
		dumpMeshset(meshset, color, drawNormals, dumpOpenEdges, move_offset);
	}

	inline void dumpMeshset(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, const vec4& color, bool dumpOpenEdges, bool move_offset)
	{
		if (meshset->meshes.size() == 0)
		{
			return;
		}
		if (disableAllDebugDump)
		{
			return;
		}

		vec3 offset = carve::geom::VECTOR(0, DumpData::instance().dump_y_pos_geom, 0);
		shared_ptr<carve::mesh::MeshSet<3> > meshset_copy(meshset->clone());
		dumpMeshset(meshset_copy.get(), offset, color, dumpOpenEdges);

		if (move_offset)
		{
			DumpData::instance().dump_y_pos_geom += meshset->getAABB().extent.y * 2.2;
		}
	}

	static void dumpVertex(const vec3& point, const vec4& color, std::string& label)
	{
		double y_pos = DumpData::instance().dump_y_pos_geom + point.y;

		std::stringstream strs_out;
		strs_out << "Vertex{" << std::endl;
		strs_out << "color{" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "}" << std::endl;
		strs_out << "coords{" << point.x << ", " << y_pos << ", " << point.z << "}" << std::endl;
		strs_out << "label{\"" << label << "\"}" << std::endl;
		strs_out << "}" << std::endl << std::endl;
		std::string strs_out_str = strs_out.str();
		appendToOutput(strs_out);
	}

	static void dumpCountLabel(const vec3& point)
	{
		std::string label = std::to_string(DumpData::instance().dumpCount);
		const vec4 color(0.4, 0.4, 0.15, 0.9);
		std::stringstream strs_out;
		strs_out << "Vertex{" << std::endl;
		strs_out << "color{" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "}" << std::endl;
		strs_out << "coords{" << point.x << ", " << point.y + DumpData::instance().dump_y_pos_geom << ", " << point.z << "}" << std::endl;
		strs_out << "label{\"" << label << "\"}" << std::endl;
		strs_out << "}" << std::endl << std::endl;

		appendToOutput(strs_out);
	}

	static void dumpLocalCoordinateSystem()
	{
		std::stringstream strs_out;
		strs_out << "Polyline{" << std::endl;
		strs_out << "color{1,0,0,1}," << std::endl;
		strs_out << R"(pointLabels: "no",)" << std::endl;
		strs_out << "vertices{ {0, " << DumpData::instance().dump_y_pos_geom << ", 0}, {5, " << DumpData::instance().dump_y_pos_geom << ", 0}}" << std::endl;
		strs_out << std::endl << "}" << std::endl;  // Polyline

		strs_out << "Polyline{" << std::endl;
		strs_out << "color{0,1,0,1}" << std::endl;
		strs_out << R"(pointLabels: "no",)" << std::endl;
		strs_out << "vertices{ {0, " << DumpData::instance().dump_y_pos_geom << ", 0}, {0, " << 5 + DumpData::instance().dump_y_pos_geom << ", 0}}" << std::endl;
		strs_out << std::endl << "}" << std::endl;  // Polyline

		strs_out << "Polyline{" << std::endl;
		strs_out << "color{0,0,1,1}" << std::endl;
		strs_out << R"(pointLabels: "no",)" << std::endl;
		strs_out << "vertices{ {0, " << DumpData::instance().dump_y_pos_geom << ", 0}, {0, " << DumpData::instance().dump_y_pos_geom << ", 5}}" << std::endl;
		strs_out << std::endl << "}" << std::endl;  // Polyline

		appendToOutput(strs_out);
	}

	static void dumpFaces(const std::vector<const carve::mesh::Face<3>* >& vecFaces, const vec4& color, bool move_offset = true)
	{
		PolyInputCache3D poly_cache;
		size_t ii = 0;
		for (const carve::mesh::Face<3>*face : vecFaces)
		{
			if (!face)
			{
				continue;
			}

			if (!face->edge)
			{
				continue;
			}

			if (face->n_edges > 100000)
			{
				std::cout << "face->n_edges > 100000" << std::endl;
			}
			
			const carve::mesh::Edge<3>* edge = face->edge;
			size_t countEdges = 0;
			do
			{
				++countEdges;
				edge = edge->next;
				if (face->edge == edge)
				{
					break;
				}
			} while (countEdges < 10000);

			if (countEdges > face->n_edges)
			{
				std::cout << "countEdges > face->n_edges" << std::endl;
			}

			if (countEdges > 100000)
			{
				std::cout << "face->n_edges > 100000" << std::endl;
			}

			std::vector<carve::mesh::Face<3>::vertex_t* > faceVertices;
			face->getVertices(faceVertices);

			std::vector<int> faceIndexes;
			for (const carve::mesh::Face<3>::vertex_t * vertex : faceVertices)
			{
				const carve::geom::vector<3>& vertexPoint = vertex->v;
				size_t idx = poly_cache.addPoint(vertexPoint);
				faceIndexes.push_back(idx);

			}

			poly_cache.m_poly_data->addFace(faceIndexes.begin(), faceIndexes.end());
			++ii;
		}

		if (poly_cache.m_poly_data->points.size() < 3)
		{
			return;
		}
		shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_cache.m_poly_data->createMesh(carve::input::opts(), DumpData::instance().eps_debug_dump));
		bool drawNormals = false;
		dumpMeshset(meshset, color, drawNormals, move_offset);
	}

	static void dumpEdge(carve::mesh::Edge<3>* e, const vec4& color, double lineThickness, bool checkZeroAreaFaces, bool move_offset, bool depthTestOff)
	{
		if (e->vert == nullptr)
		{
			return;
		}
		auto p1 = e->v1()->v;
		auto p2 = e->v2()->v;

		double edgeLength = (p2 - p1).length();

		std::vector<vec3> line = { carve::geom::VECTOR(p1.x, p1.y, p1.z), carve::geom::VECTOR(p2.x, p2.y, p2.z) };

		dumpPolyline(line, color, lineThickness, false, depthTestOff);

		if (e->face)
		{
			if (checkZeroAreaFaces)
			{
				double faceArea = MeshOps::computeFaceArea(e->face);
				if (std::abs(faceArea) < EPS_M9)
				{
					std::cout << "faceArea) < EPS_M9 )" << std::endl;
				}
			}
			std::vector<const carve::mesh::Face<3>* > vecFaces = { e->face };
			dumpFaces(vecFaces, color, false);
		}

		if (move_offset)
		{
			moveOffset(0.00001);
		}
	}

	static void dumpMeshsetOpenEdges(const shared_ptr<carve::mesh::MeshSet<3> >& meshset, const vec4& colorInput, bool checkZeroAreaFaces, bool move_offset)
	{
		vec4 color = colorInput;
		float red = color.r;
		size_t numOpenEdges = 0;
		double lineThickness = 5.0;

		for (auto mesh : meshset->meshes)
		{
			bool pointersOk = true;
			for (auto f : mesh->faces)
			{
				if (f == nullptr)
				{
					pointersOk = false;
					break;
				}
				if (f->edge == nullptr)
				{
					pointersOk = false;
					break;
				}
			}

			if (!pointersOk)
			{
				continue;
			}

			mesh->cacheEdges();

			std::vector<carve::mesh::Edge<3>* > openEdges = mesh->open_edges;
			numOpenEdges += openEdges.size();
			for (size_t ii = 0; ii < openEdges.size(); ++ii)
			{
				auto e = openEdges[ii];

				if (ii % 2 == 0)
				{
					if (red > 0.5)
					{
						color.r = red - 0.3;
					}
					else
					{
						color.r = red + 0.3;
					}
				}
				else
				{
					color.r = red;
				}

				double edgeLength = (e->v2()->v - e->v1()->v).length();
				bool depthTestOff = true;
				dumpEdge(e, color, lineThickness, move_offset, checkZeroAreaFaces, depthTestOff);

			}
		}
		if (numOpenEdges > 0)
		{
			if (move_offset)
			{
				moveOffset(meshset);
			}
		}
	}

	static void dumpWithLabel(std::string labelStr, const shared_ptr<carve::mesh::MeshSet<3> >& meshsetInput, const DumpSettingsStruct& colorSettings, const GeomProcessingParams& params, bool moveOffsetBefore, bool moveOffsetAfter)
	{
		if (!meshsetInput)
		{
			return;
		}
		if (disableAllDebugDump)
		{
			return;
		}

		MeshSetInfo infoMeshsetInput;
		bool meshset_input_ok = MeshOps::checkMeshSetValidAndClosed(meshsetInput, infoMeshsetInput, params);
		auto aabbInput = meshsetInput->getAABB();
		shared_ptr<carve::mesh::MeshSet<3> > meshset = meshsetInput;

		auto aabb = meshset->getAABB();
		if (!aabb.isEmpty())
		{
			double minX = aabb.pos.x - aabb.extent.x - 0.08;
			double maxX = aabb.pos.x + aabb.extent.x;
			double minZ = 0;// aabb.pos.z - aabb.extent.z;
			if (minX < DumpData::instance().labelPos.x)
			{
				vec3& labelPos = DumpData::instance().labelPos;
				labelPos = carve::geom::VECTOR(minX, labelPos.y, labelPos.z);

				vec3& countLabelPos = DumpData::instance().countLabelPos;
				countLabelPos = carve::geom::VECTOR(maxX + 0.3, countLabelPos.y, countLabelPos.z);
			}
		}

		if (moveOffsetBefore)
		{
			moveOffset(0.4);
		}

		vec4 colorCurrentLabel = colorSettings.colorLabel;
		if (infoMeshsetInput.meshSetValid)
		{
			colorCurrentLabel = colorSettings.colorValidMesh;
			labelStr += ", valid: yes";
		}
		else
		{
			if (meshset->meshes.size() > 0)
			{
				colorCurrentLabel = colorSettings.colorOpenEdges;
				labelStr += ", valid: no, ";
				if (infoMeshsetInput.numOpenEdges() > 0)
				{
					labelStr += std::to_string(infoMeshsetInput.numOpenEdges()) + "_open_edges";
				}

				if (infoMeshsetInput.degenerateEdges.size() > 0)
				{
					labelStr += std::to_string(infoMeshsetInput.degenerateEdges.size()) + "_degenerate_edges";
				}

				if (infoMeshsetInput.zeroAreaFaces.size() > 0)
				{
					labelStr += std::to_string(infoMeshsetInput.zeroAreaFaces.size()) + "_0area_faces";
				}
			}
			else
			{
				colorCurrentLabel = colorSettings.colorOpenEdges;
				labelStr += ", 0 meshes";
			}
		}

		if (DumpData::instance().dumpCount >= 7)
		{
			int wait = 0;
		}

		dumpVertex(DumpData::instance().labelPos, colorCurrentLabel, labelStr);
		dumpCountLabel(DumpData::instance().countLabelPos);
		bool drawNormals = !meshset_input_ok;
		dumpMeshset(meshset, colorSettings.colorMesh, drawNormals, false);
		dumpMeshsetOpenEdges(meshset, colorSettings.colorMesh, false, false);

		bool moveOffsetNow = false;
		if (moveOffsetAfter)
		{
			carve::geom::aabb<3> bbox = meshset->getAABB();
			double dy = bbox.extent.y;
			moveOffset(dy * 2.0 + 0.2);
			moveOffsetNow = true;
		}

		if (infoMeshsetInput.degenerateEdges.size() > 0 && false )
		{
			double lineThickness = 4.0;
			bool depthTestOff = true;
			for (auto e : infoMeshsetInput.degenerateEdges)
			{
				dumpEdge(e, colorSettings.colorOpenEdges, lineThickness, params.checkZeroAreaFaces, moveOffsetNow, depthTestOff);
			}
		}
	}

	static void dumpEdges(const std::vector<carve::mesh::Edge<3>* >& vec_edges)
	{
		vec4 color(0.98, 0.2, 0.2, 1.0);

		for (size_t i = 0; i < vec_edges.size(); ++i)
		{
			carve::mesh::Edge<3>* edge = vec_edges[i];
			const carve::mesh::Vertex<3>* vertex1 = edge->v1();

			std::vector<vec3> points = { edge->v1()->v, edge->v2()->v };
			dumpPolyline(points, color, 0, false, false);
		}
	}

	inline void dumpOperands(const shared_ptr<carve::mesh::MeshSet<3> >& op1, const shared_ptr<carve::mesh::MeshSet<3> >& op2, shared_ptr<carve::mesh::MeshSet<3> >& result,
		int tag, bool& op1_dumped, bool& op2_dumped, DumpSettingsStruct& dumpColorSettings, GeomProcessingParams& paramsPrime)
	{
		if (disableAllDebugDump)
		{
			return;
		}
		GeomProcessingParams params(paramsPrime);
		params.checkZeroAreaFaces = false;
		moveOffset(0.4);
		dumpLocalCoordinateSystem();

		carve::geom::aabb<3> bbox1 = op1->getAABB();
		carve::geom::aabb<3> bbox2 = op2->getAABB();
		double dy = std::max(bbox1.extent.y, bbox2.extent.y);
		if (!op1_dumped)
		{
			op1_dumped = true;
			dumpWithLabel("computeCSG::op1", op1, dumpColorSettings, params, true, false);

			shared_ptr<carve::mesh::MeshSet<3> > bbox1Mshset;
			MeshOps::boundingBox2Mesh(bbox1, bbox1Mshset, params.epsMergePoints);
			vec4 color(1.0, 0.2, 0.2, 1.0);
			std::vector<carve::mesh::Edge<3>* > vec_edges;
			for (auto mesh : bbox1Mshset->meshes)
			{
				std::copy(mesh->closed_edges.begin(), mesh->closed_edges.end(), std::back_inserter(vec_edges));
			}
			dumpEdges(vec_edges);
		}

		if (!op2_dumped)
		{
			op2_dumped = true;
			dumpWithLabel("computeCSG::op2", op2, dumpColorSettings, params, false, false);

			shared_ptr<carve::mesh::MeshSet<3> > bboxMshset;
			MeshOps::boundingBox2Mesh(bbox2, bboxMshset, params.epsMergePoints);
			vec4 color(1.0, 0.2, 0.2, 1.0);
			std::vector<carve::mesh::Edge<3>* > vec_edges;
			for (auto mesh : bboxMshset->meshes)
			{
				std::copy(mesh->closed_edges.begin(), mesh->closed_edges.end(), std::back_inserter(vec_edges));
			}
			dumpEdges(vec_edges);
		}

		moveOffset(dy * 2.0 + 0.2);

		dumpWithLabel("computeCSG::result", result, dumpColorSettings, params, true, true);
	}

	static void dumpFacePolygon(const carve::mesh::Face<3>* face, vec4& color1, bool moveOffset)
	{
		if (!face)
		{
			return;
		}
		if (!face->edge)
		{
			return;
		}
		std::vector<carve::mesh::Vertex<3>* > vertices;
		face->getVertices(vertices);

		std::vector<carve::geom::vector<3> > vertice_points;
		for (auto vert : vertices)
		{
			vertice_points.push_back(vert->v);
		}
		double eps = 0.000000015;
		GeomUtils::closePolygon(vertice_points, eps);

		dumpPolyline(vertice_points, color1, 0, moveOffset, false);
	}

	static void dumpFacePolygons(const std::vector<const carve::mesh::Face<3>* >& vecFaces, vec4& color1, bool moveDumpOffset)
	{
		carve::geom::aabb<3> bbox;

		for (auto face : vecFaces)
		{
			dumpFacePolygon(face, color1, false);
			const auto aabb = face->getAABB();
			bbox.unionAABB(aabb);
		}

		if (moveDumpOffset)
		{
			DumpData::instance().dump_y_pos_geom += bbox.extent.y * 1.1;
		}
	}
	static void dumpFacePolygons(const std::vector<carve::mesh::Face<3>* >& vecFacesIn, vec4& color1, bool moveDumpOffset)
	{
		std::vector<const carve::mesh::Face<3>* > vecFaces;
		std::copy(vecFacesIn.begin(), vecFacesIn.end(), std::back_inserter(vecFaces));
		dumpFacePolygons(vecFaces, color1, moveDumpOffset);
	}
	static void dumpFacePolygons(const carve::mesh::MeshSet<3>* meshset, vec4& color1, bool moveDumpOffset)
	{
		carve::geom::aabb<3> bbox = meshset->getAABB();

		for (auto mesh : meshset->meshes)
		{
			dumpFacePolygons(mesh->faces, color1, false);
		}

		if (moveDumpOffset)
		{
			DumpData::instance().dump_y_pos_geom += bbox.extent.y * 1.1;
		}
	}

	static void dumpItemShapeInputData(shared_ptr<ItemShapeData>& item_shape, const vec4& color)
	{
		bool drawNormals = true;
		for (auto meshset_open : item_shape->m_meshsets_open)
		{
			dumpMeshset(meshset_open, color, drawNormals, false);
		}

		drawNormals = false;
		for (auto meshset_closed : item_shape->m_meshsets)
		{
			dumpMeshset(meshset_closed, color, drawNormals, false);
		}

		for (auto item_data : item_shape->m_child_items)
		{
			dumpItemShapeInputData(item_data, color);
		}
	}

	static void dumpShapeInputData(const shared_ptr<ProductShapeData>& product_shape, const vec4& color)
	{
		for (auto geom_item : product_shape->getGeometricItems())
		{
			dumpItemShapeInputData(geom_item, color);
		}

		for (auto child : product_shape->getChildElements())
		{
			dumpShapeInputData(child, color);
		}
	}

	static void dumpEntityToIfcFile(const shared_ptr<BuildingEntity>& entity)
	{
		std::vector<shared_ptr<BuildingEntity> > vec_new_entities;
		shared_ptr<BuildingModel> ifc_model(new BuildingModel());
		double unit_length_factor = 1.0;
		ifc_model->clearIfcModel();
		ifc_model->getUnitConverter()->setLengthInMeterFactor(unit_length_factor);

		std::unordered_map<BuildingObject*, shared_ptr<BuildingObject> > map_entities;
		ifc_model->collectDependentEntities(entity, map_entities, false);

		for (auto it : map_entities)
		{
			shared_ptr<BuildingEntity> entity = dynamic_pointer_cast<BuildingEntity>(it.second);
			if (entity)
			{
				vec_new_entities.push_back(entity);
			}
		}

		shared_ptr<IfcProduct> entity_product = dynamic_pointer_cast<IfcProduct>(entity);
		if (!entity_product)
		{
			shared_ptr<IfcGeometricRepresentationItem> geom_item = dynamic_pointer_cast<IfcGeometricRepresentationItem>(entity);
			if (geom_item)
			{
				entity_product = shared_ptr<IfcBuildingElementProxy>(new IfcBuildingElementProxy());
				vec_new_entities.push_back(entity_product);
				entity_product->m_Name = shared_ptr<IfcLabel>(new IfcLabel());
				entity_product->m_Name->m_value = "proxy";

				// geometry
				shared_ptr<IfcShapeRepresentation> shape_representation(new IfcShapeRepresentation());
				vec_new_entities.push_back(shape_representation);
				shape_representation->m_RepresentationIdentifier = shared_ptr<IfcLabel>(new IfcLabel("Body"));
				shape_representation->m_RepresentationType = shared_ptr<IfcLabel>(new IfcLabel("SweptSolid"));

				shared_ptr<IfcProductDefinitionShape> product_def_shape(new IfcProductDefinitionShape());
				vec_new_entities.push_back(product_def_shape);

				entity_product->m_Representation = product_def_shape;
				product_def_shape->m_Representations.push_back(shape_representation);

				shape_representation->m_Items.push_back(geom_item);
			}
			else
			{
				return;
			}
		}

		// building structure
		shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
		if (!ifc_project)
		{
			ifc_project = shared_ptr<IfcProject>(new IfcProject());
			vec_new_entities.push_back(ifc_project);
			ifc_model->setIfcProject(ifc_project);
		}

		shared_ptr<IfcSite> ifc_site(new IfcSite());
		vec_new_entities.push_back(ifc_site);

		shared_ptr<IfcBuilding> ifc_building(new IfcBuilding());
		vec_new_entities.push_back(ifc_building);

		shared_ptr<IfcBuildingStorey> ifc_building_storey(new IfcBuildingStorey());
		vec_new_entities.push_back(ifc_building_storey);

		// relations object to connect site to project
		shared_ptr<IfcRelAggregates> rel_aggregates_project_site(new IfcRelAggregates());
		rel_aggregates_project_site->m_RelatingObject = ifc_project;
		rel_aggregates_project_site->m_RelatedObjects.push_back(ifc_site);
		vec_new_entities.push_back(rel_aggregates_project_site);

		// relations object to connect building to site
		shared_ptr<IfcRelAggregates> rel_aggregates_site_building(new IfcRelAggregates());
		rel_aggregates_site_building->m_RelatingObject = ifc_site;
		rel_aggregates_site_building->m_RelatedObjects.push_back(ifc_building);
		vec_new_entities.push_back(rel_aggregates_site_building);

		// relations object to connect building storey to building
		shared_ptr<IfcRelAggregates> rel_aggregates_building_buildingstorey(new IfcRelAggregates());
		rel_aggregates_building_buildingstorey->m_RelatingObject = ifc_building;
		rel_aggregates_building_buildingstorey->m_RelatedObjects.push_back(ifc_building_storey);
		vec_new_entities.push_back(rel_aggregates_building_buildingstorey);

		// relations object to connect wall to building storey
		shared_ptr<IfcRelContainedInSpatialStructure> rel_contained_buildingstorey_wall(new IfcRelContainedInSpatialStructure());
		rel_contained_buildingstorey_wall->m_RelatingStructure = ifc_building_storey;


		rel_contained_buildingstorey_wall->m_RelatedElements.push_back(entity_product);
		vec_new_entities.push_back(rel_contained_buildingstorey_wall);


		// coordinate systems
		double local_x[3] = { 1,0,0 };
		double local_z[3] = { 0,0,1 };
		double location[3] = { 0,0,0 };

		// global placement
		shared_ptr<IfcAxis2Placement3D> axis_placement_origin;
		convertPlacement(local_x, local_z, location, axis_placement_origin, vec_new_entities);
		shared_ptr<IfcLocalPlacement> global_placement(new IfcLocalPlacement());
		vec_new_entities.push_back(global_placement);
		global_placement->m_RelativePlacement = axis_placement_origin;

		// local placement of building
		shared_ptr<IfcAxis2Placement3D> axis_placement_building;
		convertPlacement(local_x, local_z, location, axis_placement_building, vec_new_entities);
		shared_ptr<IfcLocalPlacement> building_placement(new IfcLocalPlacement());
		vec_new_entities.push_back(building_placement);
		building_placement->m_PlacementRelTo = global_placement;
		building_placement->m_RelativePlacement = axis_placement_building;
		ifc_building->m_ObjectPlacement = building_placement;

		// local placement of entity
		shared_ptr<IfcAxis2Placement3D> axis_placement_local;
		convertPlacement(local_x, local_z, location, axis_placement_local, vec_new_entities);
		shared_ptr<IfcLocalPlacement> wall_placement(new IfcLocalPlacement());
		vec_new_entities.push_back(wall_placement);
		wall_placement->m_PlacementRelTo = building_placement;
		wall_placement->m_RelativePlacement = axis_placement_local;
		entity_product->m_ObjectPlacement = wall_placement;


		// general objects
		shared_ptr<IfcPerson> person(new IfcPerson());
		person->m_Identification = shared_ptr<IfcIdentifier>(new IfcIdentifier("MyID"));
		person->m_FamilyName = shared_ptr<IfcLabel>(new IfcLabel("MyFamilyName"));
		person->m_GivenName = shared_ptr<IfcLabel>(new IfcLabel("MyGivenName"));
		vec_new_entities.push_back(person);

		shared_ptr<IfcOrganization> orga(new IfcOrganization());
		orga->m_Identification = shared_ptr<IfcIdentifier>(new IfcIdentifier("MyOrganization"));
		orga->m_Name = shared_ptr<IfcLabel>(new IfcLabel("My organizations name"));
		orga->m_Description = shared_ptr<IfcText>(new IfcText("My organizations description"));
		vec_new_entities.push_back(orga);

		shared_ptr<IfcPersonAndOrganization> person_and_organization(new IfcPersonAndOrganization());
		person_and_organization->m_ThePerson = person;
		person_and_organization->m_TheOrganization = orga;
		vec_new_entities.push_back(person_and_organization);

		shared_ptr<IfcOwnerHistory> owner_history(new IfcOwnerHistory());
		owner_history->m_OwningUser = person_and_organization;
		vec_new_entities.push_back(owner_history);

		// define units of the model
		shared_ptr<IfcSIUnit> unit1(new IfcSIUnit());
		unit1->m_UnitType = shared_ptr<IfcUnitEnum>(new IfcUnitEnum(IfcUnitEnum::ENUM_LENGTHUNIT));
		unit1->m_Name = shared_ptr<IfcSIUnitName>(new IfcSIUnitName(IfcSIUnitName::ENUM_METRE));
		vec_new_entities.push_back(unit1);

		shared_ptr<IfcUnitAssignment> unit_assignment(new IfcUnitAssignment());
		unit_assignment->m_Units.push_back(unit1);
		vec_new_entities.push_back(unit_assignment);

		// define representation context
		shared_ptr<IfcGeometricRepresentationContext> geom_context(new IfcGeometricRepresentationContext());
		geom_context->m_ContextType = shared_ptr<IfcLabel>(new IfcLabel("Model"));
		geom_context->m_CoordinateSpaceDimension = shared_ptr<IfcDimensionCount>(new IfcDimensionCount(3));
		geom_context->m_Precision = shared_ptr<IfcReal>(new IfcReal(1.000E-5));
		geom_context->m_WorldCoordinateSystem = axis_placement_origin;
		vec_new_entities.push_back(geom_context);

		ifc_project->m_UnitsInContext = unit_assignment;
		ifc_project->m_RepresentationContexts.push_back(geom_context);

		std::map<BuildingEntity*, shared_ptr<BuildingEntity> > map_new_entities;
		shared_ptr<BuildingEntity> projectAsEntity = ifc_project;

		// insert entities into IFC model
		for (auto entity : vec_new_entities)
		{
			shared_ptr<IfcRoot> ifc_root_object = dynamic_pointer_cast<IfcRoot>(entity);
			if (ifc_root_object)
			{
				// each object that is derived from IfcRoot should have a GUID
				if (!ifc_root_object->m_GlobalId)
				{
					ifc_root_object->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId(createBase64Uuid().data()));
				}

				ifc_root_object->m_OwnerHistory = owner_history;
			}

			shared_ptr<IfcRepresentation> ifc_representation = dynamic_pointer_cast<IfcRepresentation>(entity);
			if (ifc_representation)
			{
				ifc_representation->m_ContextOfItems = geom_context;
			}

			ifc_model->insertEntity(entity);
		}

		ifc_model->unsetInverseAttributes();
		ifc_model->resolveInverseAttributes();


		// write IFC file in STEP format
		std::string file_path = "dumpEntity.ifc";
		ifc_model->initFileHeader(file_path, "IfcPlusPlus");
		std::stringstream stream;

		shared_ptr<WriterSTEP> step_writer(new WriterSTEP());
		step_writer->writeModelToStream(stream, ifc_model);
		ifc_model->clearIfcModel();

#ifdef _MSC_VER
		std::ofstream ofs(file_path, std::ofstream::out);
#else
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		std::string file_path8 = conv.to_bytes(file_path);
		std::ofstream ofs(file_path8, std::ofstream::out);
#endif
		ofs << stream.str().c_str();
		ofs.close();
	}
};

//#else
//namespace GeomDebugDump
//{
//	static void startBuffering()
//	{
//	}
//
//	static void clearBuffer()
//	{
//	}
#endif

/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#include <iostream>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/reader/IfcPPReaderSTEP.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcppgeometry/GeometryConverter.h>
#include <ifcppgeometry/GeometryInputData.h>

int main(int argc, char *argv[])
{
	std::string file_path( "ExampleModel.ifc" );

	if( argc > 1 )
	{
		std::string arg1 = argv[1];
		
		if( arg1.length() > 4 )
		{
			std::string file_type = arg1.substr(arg1.find_last_of(".") + 1);
			std::transform(file_type.begin(), file_type.end(), file_type.begin(), toupper);

			if( file_type.compare( "IFC" ) == 0 || file_type.compare( "STP" ) == 0  )
			{
				file_path = arg1.c_str();
			}
		}
	}

	shared_ptr<IfcPPModel> ifc_model( new IfcPPModel() );
	shared_ptr<IfcPPReaderSTEP> reader( new IfcPPReaderSTEP() );
	reader->loadModelFromFile( file_path, ifc_model );

	shared_ptr<GeometryConverter> geometry_converter(new GeometryConverter());
	osg::ref_ptr<osg::Switch> model_switch = new osg::Switch();
	geometry_converter->createGeometryOSG(model_switch);

	// contains the VEF graph for each IfcProduct:
	std::map<int, shared_ptr<ProductShapeInputData> >& map_vef_data = geometry_converter->getShapeInputData();

	for( auto it = map_vef_data.begin(); it != map_vef_data.end(); ++it )
	{
		// STEP entity id:
		int entity_id = it->first;

		// shape data
		shared_ptr<ProductShapeInputData>& shape_data = it->second;

		// IfcProduct:
		shared_ptr<IfcProduct> ifc_product( shape_data->m_ifc_product );

		// for each IfcProduct, there can be mulitple geometric representation items:
		std::vector<shared_ptr<ItemShapeInputData> >& vec_item_data = shape_data->m_vec_item_data;
		for( size_t i_item = 0; i_item < vec_item_data.size(); ++i_item )
		{
			shared_ptr<ItemShapeInputData>& item_data = vec_item_data[i_item];

			// every item can have several meshsets:
			std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vec_item_meshsets = item_data->m_meshsets;
			for( size_t i_meshset = 0; i_meshset < vec_item_meshsets.size(); ++i_meshset )
			{
				shared_ptr<carve::mesh::MeshSet<3> >& meshset = vec_item_meshsets[i_meshset];

				// vertices of the meshset:
				std::vector<carve::mesh::Vertex<3> >& vec_vertices = meshset->vertex_storage;

				std::vector<carve::mesh::Mesh<3>* >& vec_meshes = meshset->meshes;
				for( size_t i_mesh = 0; i_mesh < vec_meshes.size(); ++i_mesh )
				{
					carve::mesh::Mesh<3>* mesh = vec_meshes[i_meshset];
					
					// computes the volume of the mesh:
					double mesh_volume = mesh->volume();

					// closed edges of the vef graph:
					std::vector<carve::mesh::Edge<3>* >& vec_closed_edges = mesh->closed_edges;

					std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
					for( size_t i_face = 0; i_face < vec_faces.size(); ++i_face )
					{
						carve::mesh::Face<3>* face = vec_faces[i_face];
						int number_of_edges = face->n_edges;

						// computes the centroid:
						carve::geom::vector<3> face_centroid = face->centroid();

						// iterate through edges:
						carve::mesh::Edge<3>* edge = face->edge;
						do
						{
							carve::mesh::Vertex<3>* vertex_begin = edge->v1();
							carve::mesh::Vertex<3>* vertex_end = edge->v2();

							// coordinates of vertex:
							carve::geom::vector<3>& vertex_begin_coords = vertex_begin->v;
							double x = vertex_begin_coords.x;
							double y = vertex_begin_coords.y;
							double z = vertex_begin_coords.z;

							double length_of_edge = edge->length();

							// operations for edge:
							//edge->mergeFaces();
							//edge->removeEdge();
							//edge->removeHalfEdge();
							//edge->insertAfter( ... );

							edge = edge->next;
						} while( edge != face->edge );
					}
				}
			}
		}
	}

	// traverse project structure
	shared_ptr<IfcProject> project = ifc_model->getIfcProject();
	if( project )
	{
		for( size_t ii = 0; ii < project->m_Decomposes_inverse.size(); ++ii )
		{
			// add recursive function here
		}
	}

	return 0;
}

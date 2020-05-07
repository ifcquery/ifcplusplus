/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

The above copyright notice notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdafx.h"

#define _USE_MATH_DEFINES
//#include <math.h>

#include "ConverterCarve2Coin3D.h"
#include "SceneGraphUtils.h"

#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/AppearanceData.h>
#include <ifcpp/geometry/Carve/CSG_Adapter.h>
#include <ifcpp/geometry/Carve/GeometryInputData.h>
#include <ifcpp/geometry/Carve/IncludeCarveHeaders.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/IFC4/include/IfcCurtainWall.h>
#include <ifcpp/IFC4/include/IfcFeatureElementSubtraction.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcSpace.h>
#include <ifcpp/IFC4/include/IfcWindow.h>
#include <ifcpp/IFC4/include/IfcPropertySetDefinitionSet.h>


ConverterCarve2Coin3D::ConverterCarve2Coin3D( shared_ptr<GeometrySettings>& geom_settings ) : m_geom_settings( geom_settings )
{
}
ConverterCarve2Coin3D::~ConverterCarve2Coin3D() {}


void ConverterCarve2Coin3D::clearInputCache()
{
	m_map_entity_id_to_node.clear();
	m_map_representation_id_to_node.clear();
}

void ConverterCarve2Coin3D::drawFace( const carve::mesh::Face<3>* face, SoSeparator* separator, bool add_color_array )
{
#ifdef _DEBUG
	std::cout << "not triangulated" << std::endl;
#endif
	std::vector<vec3> face_vertices;
	face_vertices.resize( face->nVertices() );
	carve::mesh::Edge<3> *e = face->edge;
	const size_t num_vertices = face->nVertices();
	for( size_t i = 0; i < num_vertices; ++i )
	{
		face_vertices[i] = e->v1()->v;
		e = e->next;
	}

	if( num_vertices < 4 )
	{
		std::cout << "drawFace is meant only for num vertices > 4" << std::endl;
	}

	vec3* vertex_vec;
	std::vector<double> vertices;
	vertices.resize( num_vertices );
	SoCoordinate3 *coords = new SoCoordinate3();
	SoFaceSet *faceSet = new SoFaceSet();

	for( size_t i = 0; i < num_vertices; ++i )
	{
		vertex_vec = &face_vertices[num_vertices - i - 1];
		vertices[i*3] = vertex_vec->x;
		vertices[i*3+1] = vertex_vec->y;
		vertices[i*3+2] = vertex_vec->z;
		coords->point.set1Value( i, vertex_vec->x, vertex_vec->y, vertex_vec->z );
		//indexedFaceSet->coordIndex.set1Value( i, i );
	}

	//faceSet->coordIndex.setValues( 0, 72, indices );
	separator->addChild( coords );
	separator->addChild( faceSet );


	SbVec3f poly_normal = SceneGraphUtils::computePolygonNormal( coords );
	//osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();
	//normals->resize( num_vertices, poly_normal );

}

//#define DEBUG_DRAW_NORMALS

void ConverterCarve2Coin3D::drawMeshSet( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, SoSeparator* geode, double crease_angle, bool add_color_array )
{
	if( !meshset )
	{
		return;
	}

	SoBaseList ptr_list;
	SoCoordinate3 *coords_triangles = new SoCoordinate3();
	ptr_list.append( coords_triangles );
	SoCoordinate3 *coords_quad = nullptr;
	SoFaceSet* faceSet_triangles = new SoFaceSet();
	ptr_list.append( faceSet_triangles );
	SoFaceSet *faceSet_quad = nullptr;
	SoNormal *normals_triangles = new SoNormal();
	ptr_list.append( normals_triangles );
	SoNormal *normals_quad = nullptr;

	const size_t max_num_faces_per_vertex = 10000;
	std::map<carve::mesh::Face<3>*, double> map_face_area;
	std::map<carve::mesh::Face<3>*, double>::iterator it_face_area;

	if( crease_angle > 0 )
	{
		for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
		{
			const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];
			const size_t num_faces = mesh->faces.size();
			for( size_t i_face = 0; i_face != num_faces; ++i_face )
			{
				carve::mesh::Face<3>* face = mesh->faces[i_face];
				// compute area of projected face:
				std::vector<vec2> projected;
				face->getProjectedVertices( projected );
				double face_area = carve::geom2d::signedArea( projected );
				map_face_area[face] = abs( face_area );
			}
		}
	}

	size_t pointset_idx = 0;
	size_t vertex_triangle_idx = 0;
	size_t vertex_quad_idx = 0;
	size_t normal_triangle_idx = 0;
	size_t normal_quad_idx = 0;
	for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
	{
		const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];

		const size_t num_faces = mesh->faces.size();
		for( size_t i_face = 0; i_face != num_faces; ++i_face )
		{
			carve::mesh::Face<3>* face = mesh->faces[i_face];
			const size_t n_vertices = face->nVertices();
			if( n_vertices > 4 )
			{
				drawFace( face, geode );
				continue;
			}
			const vec3 face_normal = face->plane.N;

			if( crease_angle > 0 )
			{
				carve::mesh::Edge<3>* e = face->edge;
				for( size_t jj = 0; jj < n_vertices; ++jj )
				{
					carve::mesh::Vertex<3>* vertex = e->vert;
					vec3 intermediate_normal;

					// collect all faces at vertex
					//              | ^
					//              | |
					//  f1   e->rev | | e    face
					//              v |
					// <---e1-------   <---------------
					//------------->   --------------->
					//              |  ^
					//              |  |
					//              v  |
					carve::mesh::Edge<3>* e1 = e;// ->rev->next;
					carve::mesh::Face<3>* f1 = e1->face;
#ifdef _DEBUG
					if( f1 != face )
					{
						std::cout << "f1 != face" << std::endl;
					}
#endif
					for( size_t i3 = 0; i3 < max_num_faces_per_vertex; ++i3 )
					{
						if( !e1->rev )
						{
							break;
						}
						if( !e1->rev->next )
						{
							break;
						}

						vec3 f1_normal = f1->plane.N;
						const double cos_angle = dot( f1_normal, face_normal );
						if( cos_angle > 0 )
						{
							const double deviation = std::abs( cos_angle - 1.0 );
							if( deviation < crease_angle )
							{
								double weight = 0.0;
								it_face_area = map_face_area.find( f1 );
								if( it_face_area != map_face_area.end() )
								{
									weight = it_face_area->second;
								}
								intermediate_normal += weight*f1_normal;
							}
						}

						if( !e1->rev )
						{
							// it's an open mesh
							break;
						}

						e1 = e1->rev->next;
						if( !e1 )
						{
							break;
						}
						f1 = e1->face;
#ifdef _DEBUG
						if( e1->vert != vertex )
						{
							std::cout << "e1->vert != vertex" << std::endl;
						}
#endif
						if( f1 == face )
						{
							break;
						}
					}
					const double intermediate_normal_length = intermediate_normal.length();
					if( intermediate_normal_length < 0.0000000001 )
					{
						intermediate_normal = face_normal;
					}
					else
					{
						// normalize:
						intermediate_normal *= 1.0 / intermediate_normal_length;
					}

					const vec3& vertex_v = vertex->v;
					if( face->n_edges == 3 )
					{
						coords_triangles->point.set1Value( vertex_triangle_idx, vertex_v.x, vertex_v.y, vertex_v.z );
						++vertex_triangle_idx;
					
						normals_triangles->vector.set1Value( normal_triangle_idx, intermediate_normal.x, intermediate_normal.y, intermediate_normal.z );
						++normal_triangle_idx;
					}
					else if( face->n_edges == 4 )
					{
						if( !coords_quad )
						{
							coords_quad = new SoCoordinate3();
							ptr_list.append( coords_quad );
						}
						coords_quad->point.set1Value( vertex_quad_idx, vertex_v.x, vertex_v.y, vertex_v.z );
						++vertex_quad_idx;

						normals_quad->vector.set1Value( normal_quad_idx, intermediate_normal.x, intermediate_normal.y, intermediate_normal.z );
						++normal_quad_idx;
					}
					e = e->next;
				}
			}
			else
			{
				carve::mesh::Edge<3>* e = face->edge;
				for( size_t jj = 0; jj < n_vertices; ++jj )
				{
					carve::mesh::Vertex<3>* vertex = e->vert;
					const vec3& vertex_v = vertex->v;

					if( face->n_edges == 3 )
					{
						coords_triangles->point.set1Value( vertex_triangle_idx, vertex_v.x, vertex_v.y, vertex_v.z );
						++vertex_triangle_idx;
						
						normals_triangles->vector.set1Value( normal_triangle_idx, face_normal.x, face_normal.y, face_normal.z );
						++normal_triangle_idx;
					}
					else if( face->n_edges == 4 )
					{
						if( !coords_quad )
						{
							coords_quad = new SoCoordinate3();
							ptr_list.append( coords_quad );
						}
						coords_quad->point.set1Value( vertex_quad_idx, vertex_v.x, vertex_v.y, vertex_v.z );
						++vertex_quad_idx;

						if( !normals_quad )
						{
							normals_quad = new SoNormal();
							ptr_list.append( normals_quad );
						}
						normals_quad->vector.set1Value( normal_quad_idx, face_normal.x, face_normal.y, face_normal.z );
						++normal_quad_idx;
					}
					e = e->next;
				}
			}
		}
	}

	if( vertex_triangle_idx > 0 )
	{
		size_t num_faces = std::floor(vertex_triangle_idx/3.0);
		std::vector<int32_t> num_vertices( num_faces, 3 );
		int32_t* num_vertices_arr = &num_vertices[0];
		faceSet_triangles->numVertices.setValues( 0, num_faces, num_vertices_arr );

		geode->addChild( coords_triangles );
		geode->addChild( normals_triangles );
		geode->addChild( faceSet_triangles );
	}

	if( vertex_quad_idx > 0 )
	{
		if( coords_quad )
		{
			//osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			//if( !geometry ) { throw OutOfMemoryException(); }
			//geometry->setVertexArray( vertices_quad );
			//if( normals_quad )
			//{
			//	normals_quad->setBinding( osg::Array::BIND_PER_VERTEX );
			//	geometry->setNormalArray( normals_quad );
			//}

			//if( add_color_array )
			//{
			//	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			//	if( !colors ) { throw OutOfMemoryException(); }
			//	colors->resize( vertices_quad->size(), osg::Vec4f( 0.6f, 0.6f, 0.6f, 0.1f ) );
			//	colors->setBinding( osg::Array::BIND_PER_VERTEX );
			//	geometry->setColorArray( colors );
			//}

			//geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, vertices_quad->size() ) );
			//if( !geometry ) { throw OutOfMemoryException(); }
			//geode->addDrawable( geometry );
		}
	}
}

void ConverterCarve2Coin3D::drawPolyline( const carve::input::PolylineSetData* polyline_data, SoSeparator* geode, bool add_color_array )
{
	carve::line::PolylineSet* polyline_set = polyline_data->create( carve::input::opts() );

	if( polyline_set->vertices.size() < 2 )
	{
#ifdef _DEBUG
		std::cout << __FUNC__ << ": polyline_set->vertices.size() < 2" << std::endl;
#endif
		return;
	}

	for( auto it = polyline_set->lines.begin(); it != polyline_set->lines.end(); ++it )
	{
		const carve::line::Polyline* pline = *it;
		size_t vertex_count = pline->vertexCount();

		for( size_t vertex_i = 0; vertex_i < vertex_count; ++vertex_i )
		{
			if( vertex_i >= polyline_set->vertices.size() )
			{
#ifdef _DEBUG
				std::cout << __FUNC__ << ": vertex_i >= polyline_set->vertices.size()" << std::endl;
#endif
				continue;
			}
			const carve::line::Vertex* v = pline->vertex( vertex_i );
//			vertices->push_back( osg::Vec3d( v->v[0], v->v[1], v->v[2] ) );
		}
	}
//
//	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
//	if( !geometry ) { throw OutOfMemoryException(); }
//	geometry->setVertexArray( vertices );
//	geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, vertices->size() ) );
//
//	if( add_color_array )
//	{
//		osg::Vec4f color( 0.6f, 0.6f, 0.6f, 0.1f );
//		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array( vertices->size(), &color );
//		if( !colors ) { throw OutOfMemoryException(); }
//		colors->setBinding( osg::Array::BIND_PER_VERTEX );
//		geometry->setColorArray( colors );
//	}
//
//	geode->addDrawable( geometry );
}

void ConverterCarve2Coin3D::computeCreaseEdgesFromMeshset( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::vector<carve::mesh::Edge<3>* >& vec_edges_out, const double crease_angle )
{
	if( !meshset )
	{
		return;
	}

	for( size_t i_mesh = 0; i_mesh < meshset->meshes.size(); ++i_mesh )
	{
		const carve::mesh::Mesh<3>* mesh = meshset->meshes[i_mesh];
		const std::vector<carve::mesh::Edge<3>* >& vec_closed_edges = mesh->closed_edges;

		for( size_t i_edge = 0; i_edge < vec_closed_edges.size(); ++i_edge )
		{
			carve::mesh::Edge<3>* edge = vec_closed_edges[i_edge];

			if( !edge )
			{
				continue;
			}
			carve::mesh::Edge<3>* edge_reverse = edge->rev;
			if( !edge_reverse )
			{
				continue;
			}
			carve::mesh::Face<3>* face = edge->face;
			carve::mesh::Face<3>* face_reverse = edge_reverse->face;

			const carve::geom::vector<3>& f1_normal = face->plane.N;
			const carve::geom::vector<3>& f2_normal = face_reverse->plane.N;
			const double cos_angle = dot( f1_normal, f2_normal );
			if( cos_angle > 0 )
			{
				const double deviation = std::abs( cos_angle - 1.0 );
				if( deviation < crease_angle )
				{
					continue;
				}
			}
			// TODO: if area of face and face_reverse is equal, skip the crease edge. It could be the inside or outside of a cylinder. Check also if > 2 faces in a row have same normal angle differences
			vec_edges_out.push_back( edge );
		}
	}
}

void ConverterCarve2Coin3D::renderMeshsetCreaseEdges( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, SoSeparator* target_geode, const double crease_angle )
{
	if( !meshset )
	{
		return;
	}
	if( !target_geode )
	{
		return;
	}
	std::vector<carve::mesh::Edge<3>* > vec_crease_edges;
	computeCreaseEdgesFromMeshset( meshset, vec_crease_edges, crease_angle );

	if( vec_crease_edges.size() > 0 )
	{
		SoIndexedLineSet* lineSet = new SoIndexedLineSet();
		SoVertexProperty* vprop = new SoVertexProperty();
		int line_idx = 0;
		for( size_t i_edge = 0; i_edge < vec_crease_edges.size(); ++i_edge )
		{
			const carve::mesh::Edge<3>* edge = vec_crease_edges[i_edge];
			const carve::geom::vector<3>& vertex1 = edge->v1()->v;
			const carve::geom::vector<3>& vertex2 = edge->v2()->v;

			vprop->vertex.set1Value( i_edge*2, vertex1.x, vertex1.y, vertex1.z );
			vprop->vertex.set1Value( i_edge*2+1, vertex2.x, vertex2.y, vertex2.z );

			lineSet->coordIndex.set1Value( line_idx++, i_edge*2 );
			lineSet->coordIndex.set1Value( line_idx++, i_edge*2+1 );
			lineSet->coordIndex.set1Value( line_idx++, -1 );
		}

		lineSet->vertexProperty = vprop;
		SoDrawStyle* lineStyle = new SoDrawStyle();
		lineStyle->style = SoDrawStyle::LINES;
		lineStyle->lineWidth = 2.5f;

		SoPolygonOffset *polygonOffset = new SoPolygonOffset();
		polygonOffset->factor = 1.;
		polygonOffset->units = 1.;

		target_geode->addChild( polygonOffset );
		target_geode->addChild( lineStyle );
		target_geode->addChild( lineSet );
	}
}

void ConverterCarve2Coin3D::applyAppearancesToGroup( const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances, SoSeparator* grp )
{
	for( size_t ii = 0; ii < vec_product_appearances.size(); ++ii )
	{
		const shared_ptr<AppearanceData>& appearance = vec_product_appearances[ii];
		if( !appearance )
		{
			continue;
		}

		convertAppearanceToCoin3D( appearance, grp );

		//if( appearance->m_apply_to_geometry_type == AppearanceData::GEOM_TYPE_SURFACE || appearance->m_apply_to_geometry_type == AppearanceData::GEOM_TYPE_ANY )
		//{
		//	convertAppearanceToCoin3D( appearance, grp );
		//}
		//else if( appearance->m_apply_to_geometry_type == AppearanceData::GEOM_TYPE_CURVE )
		//{

		//}
	}
}

SoTransform* ConverterCarve2Coin3D::convertMatrixToSoTransform( const carve::math::Matrix& mat_in )
{
	SoTransform* trans = new SoTransform();
	trans->setMatrix( SbMatrix( mat_in.m[0][0], mat_in.m[0][1], mat_in.m[0][2], mat_in.m[0][3],
		mat_in.m[1][0], mat_in.m[1][1], mat_in.m[1][2], mat_in.m[1][3],
		mat_in.m[2][0], mat_in.m[2][1], mat_in.m[2][2], mat_in.m[2][3],
		mat_in.m[3][0], mat_in.m[3][1], mat_in.m[3][2], mat_in.m[3][3] ) );
	return trans;
}

//\brief method ConverterCarve2Coin3D: creates geometry objects from an IfcProduct object
// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
void ConverterCarve2Coin3D::convertProductShapeToCoin3D( shared_ptr<ProductShapeData>& product_shape, std::map<std::string, SoSeparator* >& map_representation_switches )
{
	if( product_shape->m_ifc_object_definition.expired() )
	{
		return;
	}

	shared_ptr<IfcObjectDefinition> ifc_object_def( product_shape->m_ifc_object_definition );
	shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>( ifc_object_def );
	if( !ifc_product )
	{
		return;
	}
	std::string product_guid;
	if (ifc_product->m_GlobalId)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
		product_guid = converterX.to_bytes(ifc_product->m_GlobalId->m_value);
	}

	std::stringstream strs_product_switch_name;
	strs_product_switch_name << ifc_object_def->m_entity_id << "_" << ifc_product->className() << "_group";
	
	bool draw_bounding_box = false;
	if( m_geom_settings )
	{
		draw_bounding_box = m_geom_settings->getRenderBoundingBoxes();
	}

	// create Coin3D objects
	std::vector<shared_ptr<RepresentationData> >& vec_product_representations = product_shape->m_vec_representations;
	for( size_t ii_representation = 0; ii_representation < vec_product_representations.size(); ++ii_representation )
	{
		const shared_ptr<RepresentationData>& product_representation_data = vec_product_representations[ii_representation];
		if( product_representation_data->m_ifc_representation.expired() )
		{
			continue;
		}
		shared_ptr<IfcRepresentation> ifc_representation( product_representation_data->m_ifc_representation );
		std::string representation_guid;
		SoSeparator* representation_switch = new SoSeparator();
		
		// apply statesets if there are any
		if( product_representation_data->m_vec_representation_appearances.size() > 0 )
		{
			applyAppearancesToGroup( product_representation_data->m_vec_representation_appearances, representation_switch );
		}

//#ifdef _DEBUG
		std::stringstream strs_representation_name;
		strs_representation_name << "_" << strs_product_switch_name.str().c_str() << "_representation_" << ii_representation;
		representation_switch->setName( strs_representation_name.str().c_str() );
//#endif

		const std::vector<shared_ptr<ItemShapeData> >& product_items = product_representation_data->m_vec_item_data;
		for( size_t i_item = 0; i_item < product_items.size(); ++i_item )
		{
			const shared_ptr<ItemShapeData>& item_shape = product_items[i_item];
			SoSeparator* item_group = new SoSeparator();
			if( !item_group ) { throw OutOfMemoryException( __FUNC__ ); }

//#ifdef _DEBUG
			std::stringstream strs_item_name;
			strs_item_name << strs_representation_name.str().c_str() << ",item_" << i_item;
			item_group->setName( strs_item_name.str().c_str() );
//#endif
			// apply statesets if there are any
			if( item_shape->m_vec_item_appearances.size() > 0 )
			{
				applyAppearancesToGroup( item_shape->m_vec_item_appearances, item_group );
			}

			if( dynamic_pointer_cast<IfcCurtainWall>(ifc_product) || dynamic_pointer_cast<IfcWindow>(ifc_product) )
			{
				SoMaterial* mat = nullptr;
				if( item_group->getNumChildren() > 0 )
				{
					SoNode* child = item_group->getChild( 0 );
					SoMaterial* child_as_mat = dynamic_cast<SoMaterial*>(child);
					if( child_as_mat )
					{
						mat = child_as_mat;
					}
				}
				if( !mat )
				{
					mat = new SoMaterial();
				}
				mat->transparency.setValue( 0.5 );
				item_group->addChild( mat );
			}

			// create shape for open shells
			for( size_t ii = 0; ii < item_shape->m_meshsets_open.size(); ++ii )
			{
				shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = item_shape->m_meshsets_open[ii];
				CSG_Adapter::retriangulateMeshSet( item_meshset );
				SoSeparator* geode = new SoSeparator();
				geode->setName( "geode" );
				if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
				drawMeshSet( item_meshset, geode, m_geom_settings->getCoplanarFacesMaxDeltaAngle() );

				if( m_geom_settings->getRenderCreaseEdges() )
				{
					renderMeshsetCreaseEdges( item_meshset, geode, m_geom_settings->getCreaseEdgesMaxDeltaAngle() );
				}

				// disable back face culling for open meshes
				//geode->getOrCreateStateSet()->setAttributeAndModes( m_cull_back_off.get(), osg::StateAttribute::OFF );
				item_group->addChild( geode );

				if( draw_bounding_box )
				{
					carve::geom::aabb<3> bbox = item_meshset->getAABB();
					SoSeparator* bbox_geom = new SoSeparator();
					drawBoundingBox( bbox, bbox_geom );
					geode->addChild( bbox_geom );
				}

//#ifdef _DEBUG
				std::stringstream strs_item_meshset_name;
				strs_item_meshset_name << "_" << strs_item_name.str().c_str() << ",open_meshset_" << ii;
				geode->setName( strs_item_meshset_name.str().c_str() );
//#endif
			}

			// create shape for meshsets
			for( size_t ii = 0; ii < item_shape->m_meshsets.size(); ++ii )
			{
				shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = item_shape->m_meshsets[ii];
				CSG_Adapter::retriangulateMeshSet( item_meshset );
				SoSeparator* meshset_node = new SoSeparator();
				if( !meshset_node ) { throw OutOfMemoryException( __FUNC__ ); }
				meshset_node->setName( "meshset_node" );
				drawMeshSet( item_meshset, meshset_node, m_geom_settings->getCoplanarFacesMaxDeltaAngle() );
				item_group->addChild( meshset_node );

				if( m_geom_settings->getRenderCreaseEdges() )
				{
					renderMeshsetCreaseEdges( item_meshset, meshset_node, m_geom_settings->getCreaseEdgesMaxDeltaAngle() );
				}

				if( draw_bounding_box )
				{
					carve::geom::aabb<3> bbox = item_meshset->getAABB();
					SoSeparator* bbox_geom = new SoSeparator();
					drawBoundingBox( bbox, bbox_geom );
					meshset_node->addChild( bbox_geom );
				}

//#ifdef _DEBUG
				std::stringstream strs_item_meshset_name;
				strs_item_meshset_name << "_" << strs_item_name.str().c_str() << ",meshset_" << ii;
				meshset_node->setName( strs_item_meshset_name.str().c_str() );
//#endif
			}

			// create shape for points
			const std::vector<shared_ptr<carve::input::VertexData> >& vertex_points = item_shape->getVertexPoints();
			for( size_t ii = 0; ii < vertex_points.size(); ++ii )
			{
				const shared_ptr<carve::input::VertexData>& pointset_data = vertex_points[ii];
				if( pointset_data )
				{
					if( pointset_data->points.size() > 0 )
					{
						SoSeparator* geode = new SoSeparator();
						if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
						geode->setName( "geode" );

						SoCoordinate3 *coords_points = new SoCoordinate3();
						for( size_t i_pointset_point = 0; i_pointset_point < pointset_data->points.size(); ++i_pointset_point )
						{
							vec3& carve_point = pointset_data->points[i_pointset_point];
							coords_points->point.set1Value( i_pointset_point, carve_point.x, carve_point.y, carve_point.z );
						}

						geode->addChild( coords_points );
						geode->addChild( new SoPointSet() );
						item_group->addChild( geode );

//#ifdef _DEBUG
						std::stringstream strs_item_meshset_name;
						strs_item_meshset_name << "_" << strs_item_name.str().c_str() << ",vertex_point_" << ii;
						geode->setName( strs_item_meshset_name.str().c_str() );
//#endif
					}
				}
			}

			// create shape for polylines
			for( size_t ii = 0; ii < item_shape->m_polylines.size(); ++ii )
			{
				shared_ptr<carve::input::PolylineSetData>& polyline_data = item_shape->m_polylines[ii];
				//osg::ref_ptr<SoSeparator> geode = new SoSeparator();
				//if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
				//geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
				//drawPolyline( polyline_data.get(), geode );
				//item_group->addChild( geode );

#ifdef _DEBUG
				std::stringstream strs_item_meshset_name;
				strs_item_meshset_name << strs_item_name.str().c_str() << ",polylines_" << ii;
				//geode->setName( strs_item_meshset_name.str().c_str() );
#endif
			}

			if( m_geom_settings->isShowTextLiterals() )
			{
				for( size_t ii = 0; ii < item_shape->m_vec_text_literals.size(); ++ii )
				{
					shared_ptr<TextItemData>& text_data = item_shape->m_vec_text_literals[ii];
					if( !text_data )
					{
						continue;
					}

					SoSeparator* text_sep = new SoSeparator();
					text_sep->setName( "text_sep" );
					carve::math::Matrix& text_pos = text_data->m_text_position;
					// TODO: handle rotation

					std::string text_str;
					text_str.assign( text_data->m_text.begin(), text_data->m_text.end() );

					SoTranslation * text_trans = new SoTranslation;
					text_trans->translation = SbVec3f( text_pos._41, text_pos._42, text_pos._43 );
					text_sep->addChild( text_trans );

					SoText2 * text_node = new SoText2;
					text_node->string.set1Value( 0, text_str.c_str() );
					//instructions->string.setValues( 0, sizeof( str ) / sizeof( char * ), str );
					text_node->justification = SoText2::LEFT;
					text_sep->addChild( text_node );


					//osg::Vec3 pos2( text_pos._41, text_pos._42, text_pos._43 );

					//osg::ref_ptr<osgText::Text> txt = new osgText::Text();
					//if( !txt )
					//{
					//	throw OutOfMemoryException( __FUNC__ );
					//}
					//txt->setFont( "fonts/arial.ttf" );
					//txt->setColor( osg::Vec4f( 0, 0, 0, 1 ) );
					//txt->setCharacterSize( 0.1f );
					//txt->setAutoRotateToScreen( true );
					//txt->setPosition( pos2 );
					//txt->setText( text_str.c_str() );
					//txt->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

					
					//if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
					//geode->addChild( txt );
					item_group->addChild( text_sep );
				}
			}

			// If anything has been created, add it to the representation group
			if( item_group->getNumChildren() > 0 )
			{
#ifdef _DEBUG
				//if( item_group->getNumParents() > 0 )
				{
					//std::cout << __FUNC__ << ": item_group->getNumParents() > 0" << std::endl;
				}
#endif

				//SoMaterial* myMaterial = new SoMaterial();
				//myMaterial->diffuseColor.setValue( 0.5, 1.0, 1.0 );
				//item_group->addChild( myMaterial );

				representation_switch->addChild( item_group );
			}
			else
			{
				//item_group->unref();
			}
		}

		// If anything has been created, add it to the product group
		if( representation_switch->getNumChildren() > 0 )
		{
#ifdef _DEBUG
			//if( representation_switch->getNumParents() > 0 )
			{
				//std::cout << __FUNC__ << ": product_representation_switch->getNumParents() > 0" << std::endl;
			}
#endif
			// enable transparency for certain objects
			if( dynamic_pointer_cast<IfcSpace>( ifc_product ) )
			{
				//representation_switch->setStateSet( m_glass_stateset );
			}
			else if( dynamic_pointer_cast<IfcCurtainWall>( ifc_product ) || dynamic_pointer_cast<IfcWindow>( ifc_product ) )
			{

				
				//representation_switch->setStateSet( m_glass_stateset );
				//SceneGraphUtils::setMaterialAlpha( representation_switch, 0.6f );
				
				SoMaterial* mat = new SoMaterial();
				mat->transparency.setValue( 0.5 );
				representation_switch->addChild( mat );
			}

			map_representation_switches.insert( std::make_pair( product_guid, representation_switch ) );
		}
	}

	// TODO: if no color or material is given, set color 231/219/169 for walls, 140/140/140 for slabs 
}

/*\brief method convertToOSG: Creates geometry for OpenSceneGraph from given ProductShapeData.
\param[out] parent_group Group to append the geometry.
**/
void ConverterCarve2Coin3D::convertToCoin3D( const std::map<std::string, shared_ptr<ProductShapeData> >& map_shape_data, SoSeparator* parent_group )
{
	progressTextCallback( L"Converting geometry to OpenGL format ..." );
	progressValueCallback( 0, "scenegraph" );
	m_map_entity_id_to_node.clear();
	m_map_representation_id_to_node.clear();
	//m_ptr_list.truncate( m_ptr_list.getLength() );
	//m_vec_existing_statesets.clear();

	shared_ptr<ProductShapeData> ifc_project_data;
	std::vector<shared_ptr<ProductShapeData> > vec_products;

	for( auto it = map_shape_data.begin(); it != map_shape_data.end(); ++it )
	{
		shared_ptr<ProductShapeData> shape_data = it->second;
		if( shape_data )
		{
			vec_products.push_back( shape_data );
		}
	}

	// create geometry for for each IfcProduct independently, spatial structure will be resolved later
	std::map<std::string, SoPtr<SoSeparator> >* map_entity_id = &m_map_entity_id_to_node;
	std::map<std::string, SoPtr<SoSeparator> >* map_representations = &m_map_representation_id_to_node;
	const int num_products = (int)vec_products.size();

#ifdef ENABLE_OPENMP
	Mutex writelock_map;
	Mutex writelock_message_callback;
	Mutex writelock_ifc_project;

#pragma omp parallel firstprivate(num_products) shared(map_entity_id, map_representations)
	{
		// time for one product may vary significantly, so schedule not so many
#pragma omp for schedule(dynamic,40)
#endif
		for( int i = 0; i < num_products; ++i )
		{
			shared_ptr<ProductShapeData>& shape_data = vec_products[i];

			weak_ptr<IfcObjectDefinition>& ifc_object_def_weak = shape_data->m_ifc_object_definition;
			if( ifc_object_def_weak.expired() )
			{
				continue;
			}
			shared_ptr<IfcObjectDefinition> ifc_object_def( ifc_object_def_weak );

			std::stringstream thread_err;
			if( dynamic_pointer_cast<IfcFeatureElementSubtraction>( ifc_object_def ) )
			{
				// geometry will be created in method subtractOpenings
				continue;
			}
			else if( dynamic_pointer_cast<IfcProject>( ifc_object_def ) )
			{
#ifdef ENABLE_OPENMP
				ScopedLock scoped_lock( writelock_ifc_project );
#endif
				ifc_project_data = shape_data;
			}

			shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>( ifc_object_def );
			if( !ifc_product )
			{
				continue;
			}

			if( !ifc_product->m_Representation )
			{
				continue;
			}

			const int product_id = ifc_product->m_entity_id;
			std::map<std::string, SoSeparator* > map_representation_switches;
			try
			{
				convertProductShapeToCoin3D( shape_data, map_representation_switches );
			}
			catch( OutOfMemoryException& e )
			{
				throw e;
			}
			catch( BuildingException& e )
			{
				thread_err << e.what();
			}
			catch( carve::exception& e )
			{
				thread_err << e.str();
			}
			catch( std::exception& e )
			{
				thread_err << e.what();
			}
			catch( ... )
			{
				thread_err << "undefined error, product id " << product_id;
			}

			if( map_representation_switches.size() > 0 )
			{
				SoSeparator* product_switch = new SoSeparator();

				SoTransform* product_transform = convertMatrixToSoTransform( shape_data->getTransform() );
				product_switch->addChild( product_transform );

				std::string product_guid = "";
				if (ifc_product->m_GlobalId)
				{
					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
					product_guid = converterX.to_bytes(ifc_product->m_GlobalId->m_value);
				}
				std::stringstream strs_product_switch_name;
				strs_product_switch_name << "_" << product_id << "_" << ifc_product->className() << "_group";

				product_switch->setName( strs_product_switch_name.str().c_str() );

				for( auto it_map = map_representation_switches.begin(); it_map != map_representation_switches.end(); ++it_map )
				{
					SoSeparator* repres_switch = it_map->second;
					//product_transform->addChild( repres_switch );
					product_switch->addChild( repres_switch );
				}

				// apply statesets if there are any
				const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances = shape_data->getAppearances();
				if( vec_product_appearances.size() > 0 )
				{
					applyAppearancesToGroup( vec_product_appearances, product_switch );
				}

#ifdef ENABLE_OPENMP
				ScopedLock scoped_lock( writelock_map );
#endif
				map_entity_id->insert( std::make_pair(product_guid, product_switch ) );
				map_representations->insert( map_representation_switches.begin(), map_representation_switches.end() );
				//for( auto rep_switch : map_representation_switches )
				//{
				//	m_ptr_list.append( rep_switch.second );
				//}
				//m_ptr_list.append( product_switch );
			}

			if( thread_err.tellp() > 0 )
			{
#ifdef ENABLE_OPENMP
				ScopedLock scoped_lock( writelock_message_callback );
#endif
				messageCallback( thread_err.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
			}

			// progress callback
			double progress = (double)i / (double)num_products;
			if( progress - m_recent_progress > 0.02 )
			{
#ifdef ENABLE_OPENMP
				if( omp_get_thread_num() == 0 )
#endif
				{
					// leave 10% of progress to openscenegraph internals
					progressValueCallback( progress*0.9, "scenegraph" );
					m_recent_progress = progress;
				}
			}
		}
#ifdef ENABLE_OPENMP
	} // implicit barrier
#endif

	try
	{
		// now resolve spatial structure
		if( ifc_project_data )
		{
			resolveProjectStructure( ifc_project_data, parent_group );
		}
	}
	catch( OutOfMemoryException& e )
	{
		throw e;
	}
	catch( BuildingException& e )
	{
		messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
	}
	catch( std::exception& e )
	{
		messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
	}
	catch( ... )
	{
		messageCallback( "undefined error", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
	}
	progressValueCallback( 0.9, "scenegraph" );
}

void ConverterCarve2Coin3D::addNodes( const std::map<std::string, shared_ptr<BuildingObject> >& map_shape_data, SoSeparator* target_group )
{
	// check if there are entities that are not in spatial structure
	if( !target_group )
	{
		target_group = new SoSeparator();
	}

	for( auto it_product_shapes = map_shape_data.begin(); it_product_shapes != map_shape_data.end(); ++it_product_shapes )
	{
		std::string product_guid = it_product_shapes->first;
		auto it_find = m_map_entity_id_to_node.find(product_guid);

		if( it_find != m_map_entity_id_to_node.end() )
		{
			SoPtr<SoSeparator>& sw = it_find->second;
			if( sw.valid() )
			{
				target_group->addChild( sw.get() );
			}
		}
	}
}

void ConverterCarve2Coin3D::resolveProjectStructure( const shared_ptr<ProductShapeData>& product_data, SoSeparator* group )
{
	if( !product_data )
	{
		return;
	}

	if( product_data->m_ifc_object_definition.expired() )
	{
		return;
	}

	shared_ptr<IfcObjectDefinition> object_def( product_data->m_ifc_object_definition );
	
	std::string product_guid;
	if (object_def->m_GlobalId)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
		product_guid = converterX.to_bytes(object_def->m_GlobalId->m_value);
	}


	const std::vector<shared_ptr<ProductShapeData> >& vec_children = product_data->getChildren();
	for( size_t ii = 0; ii < vec_children.size(); ++ii )
	{
		const shared_ptr<ProductShapeData>& child_product_data = vec_children[ii];
		if( !child_product_data )
		{
			continue;
		}

		SoSeparator* group_subparts = new SoSeparator();
		if( !child_product_data->m_ifc_object_definition.expired() )
		{
			shared_ptr<IfcObjectDefinition> child_obj_def( child_product_data->m_ifc_object_definition );

			std::stringstream group_subparts_name;
			group_subparts_name << "_" << object_def->m_entity_id << "_" << child_obj_def->className() << "_group";
			group_subparts->setName( group_subparts_name.str().c_str() );
		}

		group->addChild( group_subparts );
		resolveProjectStructure( child_product_data, group_subparts );
	}

	auto it_product_map = m_map_entity_id_to_node.find(product_guid);
	if( it_product_map != m_map_entity_id_to_node.end() )
	{
		SoPtr<SoSeparator>& product_switch = it_product_map->second;
		if( product_switch.valid() )
		{
			group->addChild( product_switch.get() );
		}
	}
	else
	{
		if( group->getNumChildren() == 0 )
		{
			SoSeparator* product_switch = new SoSeparator();
			group->addChild( product_switch );

			std::stringstream switch_name;
			switch_name << "_" << object_def->m_entity_id << "_" << object_def->className();
			product_switch->setName( switch_name.str().c_str() );
		}
	}
}

void ConverterCarve2Coin3D::convertAppearanceToCoin3D( const shared_ptr<AppearanceData>& appearence, SoSeparator* target_node )
{
	if( !appearence )
	{
		return;
	}
	const float shininess = appearence->m_shininess;
	const float transparency = appearence->m_transparency;
	const bool set_transparent = appearence->m_set_transparent;

	const float color_ambient_r = appearence->m_color_ambient.r();
	const float color_ambient_g = appearence->m_color_ambient.g();
	const float color_ambient_b = appearence->m_color_ambient.b();
	const float color_ambient_a = appearence->m_color_ambient.a();

	const float color_diffuse_r = appearence->m_color_diffuse.r();
	const float color_diffuse_g = appearence->m_color_diffuse.g();
	const float color_diffuse_b = appearence->m_color_diffuse.b();
	const float color_diffuse_a = appearence->m_color_diffuse.a();

	const float color_specular_r = appearence->m_color_specular.r();
	const float color_specular_g = appearence->m_color_specular.g();
	const float color_specular_b = appearence->m_color_specular.b();
	const float color_specular_a = appearence->m_color_specular.a();

	SoMaterial* mat = new SoMaterial();
	
	// Coin3d: Transparency values range from 0.0 ( completely opaque, which is the default ) 
	// to 1.0 ( completely transparent, i.e.invisible ).
	// Traditional Open Inventor uses the same override bit for both transparency and diffuse color.
	// To get around this problem if you need to override one without the other, set the 
	// environment variable "COIN_SEPARATE_DIFFUSE_TRANSPARENCY_OVERRIDE".
	// This is a Coin extension, and will not work on the other Open Inventor implementations.

	if( transparency == 1.0 )
	{
		mat->diffuseColor.setValue( color_diffuse_r, color_diffuse_g, color_diffuse_b );
	}
	else
	{
		//SoMFColor 	diffuseColor;
		//diffuseColor.set, color_ambient_g, color_ambient_b );

		mat->ambientColor.setValue( color_ambient_r, color_ambient_g, color_ambient_b );
		float transparency_coin3d = 0.8f - transparency*0.2;
		mat->transparency.set1Value( 0, transparency_coin3d );
	}
		
	mat->shininess = 0.2f;

	target_node->addChild( mat );

	//if( appearence->m_set_transparent )
	//{
	//	mat->setTransparency( osg::Material::FRONT_AND_BACK, transparency );
	//	target_stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
	//	target_stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	//}

	if( appearence->m_specular_exponent != 0.f )
	{
		//osg::ref_ptr<osgFX::SpecularHighlights> spec_highlights = new osgFX::SpecularHighlights();
		//spec_highlights->setSpecularExponent( spec->m_value );
		// todo: add to scenegraph
	}
}

void ConverterCarve2Coin3D::drawBoundingBox( const carve::geom::aabb<3>& box, SoSeparator* geom )
{
	double min_x = box.pos.x - box.extent.x;
	double max_x = box.pos.x + box.extent.x;
	double min_y = box.pos.y - box.extent.y;
	double max_y = box.pos.y + box.extent.y;
	double min_z = box.pos.z - box.extent.z;
	double max_z = box.pos.z + box.extent.z;

	const SbVec3f corners[] = {
		// bottom points
		SbVec3f( min_x, min_y, min_z ),
		SbVec3f( max_x, min_y, min_z ),
		SbVec3f( max_x, max_y, min_z ),
		SbVec3f( min_x, max_y, min_z ),

		// top points
		SbVec3f( min_x, min_y, max_z ),
		SbVec3f( max_x, min_y, max_z ),
		SbVec3f( max_x, max_y, max_z ),
		SbVec3f( min_x, max_y, max_z )
	};

	const int32_t indices[] = {
		0, 1, 2, 3, 0, -1, // bottom loop
		4, 5, 6, 7, 4, -1, // top loop
		0, 4, -1, 1, 5, -1, 2, 6, -1, 3, 7, -1 // bootom to top lines
	};

	SoVertexProperty* vprop = new SoVertexProperty();
	vprop->vertex.setValues( 0, sizeof( corners ) / sizeof( corners[0] ), corners );

	SoIndexedLineSet* ils = new SoIndexedLineSet();
	ils->vertexProperty = vprop;
	ils->coordIndex.setValues( 0, sizeof( indices ) / sizeof( indices[0] ), indices );
	geom->addChild( ils );
}

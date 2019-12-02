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

#include <unordered_set>
#include <osg/Material>
#include <osg/Geode>
#include <osg/CullFace>
#include <osg/Point>
#include <osg/Switch>
#include <osgText/Text>

#include <BRepAdaptor_Curve.hxx>
#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <Geom_Line.hxx>
#include <Poly.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/SceneGraphUtils.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/IFC4/include/IfcCurtainWall.h>
#include <ifcpp/IFC4/include/IfcFeatureElementSubtraction.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcPropertySetDefinitionSet.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcRelDefinesByProperties.h>
#include <ifcpp/IFC4/include/IfcSpace.h>
#include <ifcpp/IFC4/include/IfcWindow.h>

#include "GeometryInputDataOCC.h"

class ScenegraphConverterOCC : public StatusCallback
{
protected:
	std::map<int, osg::ref_ptr<osg::Switch> > m_map_entity_id_to_switch;		// Map: IfcProduct ID -> scenegraph switch
	std::map<int, osg::ref_ptr<osg::Switch> > m_map_representation_to_switch;	// Map: Representation identifier -> scenegraph switch
	shared_ptr<GeometrySettings>	m_geom_settings;
	double m_recent_progress = 0;
	osg::ref_ptr<osg::CullFace>		m_cull_back_off;
	osg::ref_ptr<osg::StateSet>		m_glass_stateset;
	//\brief StateSet caching and re-use
	std::vector<osg::ref_ptr<osg::StateSet> >	m_vec_existing_statesets;
	bool										m_enable_stateset_caching = false;
#ifdef ENABLE_OPENMP
	Mutex m_writelock_appearance_cache;
#endif

public:
	ScenegraphConverterOCC( shared_ptr<GeometrySettings>& geom_settings ) : m_geom_settings( geom_settings )
	{
		m_cull_back_off = new osg::CullFace( osg::CullFace::BACK );
		m_glass_stateset = new osg::StateSet();
		m_glass_stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
		m_glass_stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	}
	virtual ~ScenegraphConverterOCC() {}

	// after calling convertToOSG, the OSG Switches are in the map returned by this method
	const std::map<int, osg::ref_ptr<osg::Switch> >& getMapIdSwitch() { return m_map_entity_id_to_switch; }

	struct RenderOptions
	{
		RenderOptions(){}
		RenderOptions( osg::Vec4f color, double distance_between_points_in_mm = 0.5, bool create_points_along_straight_line = false )
		{
			m_color = color;
			m_color_set = true;
			m_distance_between_points_in_mm = distance_between_points_in_mm;
			m_create_points_along_straight_line = create_points_along_straight_line;
		}
		osg::Vec4f m_color;
		bool m_color_set = false;
		double m_distance_between_points_in_mm = 0.5;
		bool m_create_points_along_straight_line = false;
	};

	void clearInputCache()
	{
		m_map_entity_id_to_switch.clear();
		m_map_representation_to_switch.clear();
		m_vec_existing_statesets.clear();
	}

	static void getEdgePoints( const TopoDS_Edge& edge, osg::Vec3Array* vertices, const RenderOptions& render_options )
	{
		Standard_Real first = 0;
		Standard_Real last = 1;
		Handle( Geom_Curve ) c = BRep_Tool::Curve( edge, first, last );
		bool discretize_points_on_straight_line = render_options.m_create_points_along_straight_line;
		if( c->DynamicType() == STANDARD_TYPE( Geom_Line ) && !discretize_points_on_straight_line )
		{
			// just straight line
			const TopoDS_Vertex& v1 = TopExp::FirstVertex( edge );
			const TopoDS_Vertex& v2 = TopExp::LastVertex( edge );
			gp_Pnt point1 = BRep_Tool::Pnt( v1 );
			gp_Pnt point2 = BRep_Tool::Pnt( v2 );
			vertices->push_back( osg::Vec3d( point1.X(), point1.Y(), point1.Z() ) );
			vertices->push_back( osg::Vec3d( point2.X(), point2.Y(), point2.Z() ) );
		}
		else
		{
			double param_range = last - first;
			BRepAdaptor_Curve curve_adaptor(edge);
			//curve_adaptor.Initialize( edge );

#ifdef _DEBUG
			const TopoDS_Vertex& v1 = TopExp::FirstVertex( edge );
			const TopoDS_Vertex& v2 = TopExp::LastVertex( edge );
			gp_Pnt point1 = BRep_Tool::Pnt( v1 );
			gp_Pnt point2 = BRep_Tool::Pnt( v2 );
#endif

			Standard_Real length_of_edge = GCPnts_AbscissaPoint::Length( curve_adaptor );

			double distance = render_options.m_distance_between_points_in_mm;

			double num_points = 40*param_range/(2.0*M_PI);
			distance = length_of_edge/num_points;

			GCPnts_UniformAbscissa uniform_abscissa;
			uniform_abscissa.Initialize( curve_adaptor, distance );

			if( uniform_abscissa.IsDone() )
			{
				int nb_points = uniform_abscissa.NbPoints();
				for( int i = 0; i < nb_points; ++i )
				{
					Standard_Real parameter = uniform_abscissa.Parameter( i + 1 );
					gp_Pnt pnt = curve_adaptor.Value( parameter );
					vertices->push_back( osg::Vec3d( pnt.X(), pnt.Y(), pnt.Z() ) );
					if( i > 0 && i < nb_points - 1 )
					{
						vertices->push_back( osg::Vec3d( pnt.X(), pnt.Y(), pnt.Z() ) );
					}
				}
				if( vertices->size()> 0 )
				{
					if( vertices->size()%2 != 0 )
					{
						vertices->push_back( vertices->back() );
					}
				}
			}
		}
	}

	static void drawShape( const TopoDS_Shape& shape, osg::Geode* parent_geode, const RenderOptions& render_options )
	{
		if( shape.IsNull() )
		{
			return;
		}

		osg::ref_ptr<osg::Vec3Array> vertices_lines = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> vertices_tri_storage = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> vertices_tri = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> normals_tri = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> normals_tri_storage = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> vertices_quad;
		osg::ref_ptr<osg::Vec3Array> normals_quad;

#ifdef _DEBUG
		osg::ref_ptr<osg::Vec3Array> vertices_triangle_edges = new osg::Vec3Array();
#endif

		TopAbs_ShapeEnum shape_type = shape.ShapeType();
		if( shape_type == TopAbs_WIRE || shape_type == TopAbs_EDGE || shape_type == TopAbs_VERTEX )
		{
			TopExp_Explorer Ex;
			for( Ex.Init( shape, TopAbs_EDGE ); Ex.More(); Ex.Next() )
			{
				TopoDS_Edge edge = TopoDS::Edge( Ex.Current() );
				getEdgePoints( edge, vertices_lines, render_options );
			}
		}
		else
		{
			Standard_Real linear_tolerance = 0.06*0.001;  // for [m]
			Standard_Real angular_tolerance = 0.5;
			bool is_relative = false;
			BRepMesh_IncrementalMesh incremental_mesh( shape, linear_tolerance, is_relative, angular_tolerance );

			TopExp_Explorer shape_explorer( shape, TopAbs_FACE );
			for( ; shape_explorer.More(); shape_explorer.Next() )
			{
				const TopoDS_Face& face = TopoDS::Face( shape_explorer.Current() );
				TopLoc_Location L = TopLoc_Location();
				const Handle( Poly_Triangulation )& poly_triangulation = BRep_Tool::Triangulation( face, L );

				if( poly_triangulation.IsNull() )
				{
					continue;
				}

				const gp_Trsf & face_trsf = L.Transformation();


				Poly::ComputeNormals( poly_triangulation );
				const TColgp_Array1OfPnt&       triang_vertices = poly_triangulation->Nodes();
				const TShort_Array1OfShortReal& triang_normals = poly_triangulation->Normals();
				const Poly_Array1OfTriangle& triangles = poly_triangulation->Triangles();

				// Number of nodes in the triangulation
				int num_vertices = poly_triangulation->Nodes().Length();
				if( num_vertices*3 != triang_normals.Length() )
				{
					std::cout << "Different number of normals and vertices\n";
					return;
				}

				if( !vertices_tri_storage )
				{
					vertices_tri_storage = new osg::Vec3Array();
				}
				size_t offset_vertex_storage = vertices_tri_storage->size();
				if( !normals_tri_storage )
				{
					normals_tri_storage = new osg::Vec3Array();
				}
				//size_t offset_normals_storage = normals_tri_storage->size();

				// Get each vertex index, checking common vertices between shapes
				for( int i = 0; i < num_vertices; i++ )
				{
					gp_Pnt triang_point = triang_vertices.Value( i+1 );
					gp_Vec normal( triang_normals.Value( i*3 + 1 ), triang_normals.Value( i*3 + 2 ), triang_normals.Value( i*3 + 3 ) );
					if( face_trsf.Form() != gp_Identity )
					{
						triang_point.Transform( face_trsf );
						normal.Transform( face_trsf );
					}
					double x = std::round( triang_point.X()*10.0 )*0.1;
					double y = std::round( triang_point.Y()*10.0 )*0.1;
					double z = std::round( triang_point.Z()*10.0 )*0.1;

					vertices_tri_storage->push_back( osg::Vec3d( x, y, z ) );
					normals_tri_storage->push_back( osg::Vec3d( normal.X(), normal.Y(), normal.Z() ) );
				}

				if( !vertices_tri )
				{
					vertices_tri = new osg::Vec3Array();
				}
				if( !normals_tri )
				{
					normals_tri = new osg::Vec3Array();
				}

				int num_stored_vertices = vertices_tri_storage->size();
				for( auto it = triangles.begin(); it != triangles.end(); ++it )
				{
					const Poly_Triangle& triang = *it;
					int idx_tri1, idx_tri2, idx_tri3;
					triang.Get( idx_tri1, idx_tri2, idx_tri3 );

					int idx1 = offset_vertex_storage + idx_tri1 - 1;
					int idx2 = offset_vertex_storage + idx_tri2 - 1;
					int idx3 = offset_vertex_storage + idx_tri3 - 1;

					if( idx1 >= num_stored_vertices || idx2 >= num_stored_vertices || idx3 >= num_stored_vertices )
					{
						std::cout << "idx > num_stored_vertices" << std::endl;
						continue;
					}
					osg::Vec3 v1 = vertices_tri_storage->at( idx1 );
					osg::Vec3 v2 = vertices_tri_storage->at( idx2 );
					osg::Vec3 v3 = vertices_tri_storage->at( idx3 );
					vertices_tri->push_back( v1 );
					vertices_tri->push_back( v2 );
					vertices_tri->push_back( v3 );

					osg::Vec3 n1 = normals_tri_storage->at( idx1 );
					osg::Vec3 n2 = normals_tri_storage->at( idx2 );
					osg::Vec3 n3 = normals_tri_storage->at( idx3 );
					normals_tri->push_back( n1 );
					normals_tri->push_back( n2 );
					normals_tri->push_back( n3 );


				}
			}
		}


		if( vertices_tri->size() > 0 )
		{
			if( vertices_tri->size() == normals_tri->size() )
			{
				osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
				geometry->setVertexArray( vertices_tri );
				geometry->setNormalArray( normals_tri );
				normals_tri->setBinding( osg::Array::BIND_PER_VERTEX );

				if( render_options.m_color_set )
				{
					osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
					colors->push_back( render_options.m_color );
					colors->setBinding( osg::Array::BIND_OVERALL );
					geometry->setColorArray( colors );
				}

				geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vertices_tri->size() ) );
				parent_geode->addDrawable( geometry );

#ifdef DEBUG_DRAW_NORMALS
				osg::ref_ptr<osg::Vec3Array> vertices_normals = new osg::Vec3Array();
				for( size_t i = 0; i < vertices_tri->size(); ++i )
				{
					osg::Vec3f& vertex_vec = vertices_tri->at( i );// [i];
					osg::Vec3f& normal_vec = normals_tri->at( i );
					vertices_normals->push_back( osg::Vec3f( vertex_vec.x(), vertex_vec.y(), vertex_vec.z() ) );
					vertices_normals->push_back( osg::Vec3f( vertex_vec.x(), vertex_vec.y(), vertex_vec.z() ) + normal_vec );
				}

				osg::ref_ptr<osg::Vec4Array> colors_normals = new osg::Vec4Array();
				colors_normals->resize( vertices_normals->size(), osg::Vec4f( 0.4f, 0.7f, 0.4f, 1.f ) );

				osg::ref_ptr<osg::Geometry> geometry_normals = new osg::Geometry();
				geometry_normals->setVertexArray( vertices_normals );
				geometry_normals->setColorArray( colors_normals );
				colors_normals->setBinding( osg::Array::BIND_PER_VERTEX );
				geometry_normals->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
				geometry_normals->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vertices_normals->size() ) );
				parent_geode->addDrawable( geometry_normals );
#endif
			}
		}

		if( vertices_quad )
		{
			if( vertices_quad->size() > 0 )
			{
				osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
				geometry->setVertexArray( vertices_quad );
				if( normals_quad )
				{
					normals_quad->setBinding( osg::Array::BIND_PER_VERTEX );
					geometry->setNormalArray( normals_quad );
				}

				if( render_options.m_color_set )
				{
					osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
					colors->push_back( render_options.m_color );
					colors->setBinding( osg::Array::BIND_OVERALL );
					geometry->setColorArray( colors );
				}

				geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, vertices_quad->size() ) );
				parent_geode->addDrawable( geometry );
			}
		}

		if( vertices_lines->size() > 0 )
		{
			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			geometry->setVertexArray( vertices_lines );

			if( render_options.m_color_set )
			{
				osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
				colors->push_back( render_options.m_color );
				colors->setBinding( osg::Array::BIND_OVERALL );
				geometry->setColorArray( colors );
			}

			geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vertices_lines->size() ) );
			geometry->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
			parent_geode->addDrawable( geometry );
		}

#ifdef _DEBUG
		if( vertices_triangle_edges->size() > 0 && false )
		{
			{
				osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
				geometry->setVertexArray( vertices_triangle_edges );
				osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
				colors->resize( vertices_triangle_edges->size(), osg::Vec4f( 0.6f, 0.7f, 0.6f, 0.1f ) );
				colors->setBinding( osg::Array::BIND_PER_VERTEX );
				geometry->setColorArray( colors );
				geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vertices_triangle_edges->size() ) );
				parent_geode->addDrawable( geometry );
				geometry->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
			}
		}

#endif
	}

	void applyAppearancesToGroup( const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances, osg::Group* grp )
	{
		for( size_t ii = 0; ii < vec_product_appearances.size(); ++ii )
		{
			const shared_ptr<AppearanceData>& appearance = vec_product_appearances[ii];
			if( !appearance )
			{
				continue;
			}

			AppearanceData::GeometryTypeEnum geom_type = appearance->m_apply_to_geometry_type;
			if( geom_type == AppearanceData::GEOM_TYPE_SURFACE || geom_type == AppearanceData::GEOM_TYPE_ANY )
			{
				osg::StateSet* item_stateset = convertToOSGStateSet( appearance );
				if( item_stateset != nullptr )
				{
					osg::StateSet* existing_item_stateset = grp->getStateSet();
					if( existing_item_stateset )
					{
						if( existing_item_stateset != item_stateset )
						{
							existing_item_stateset->merge( *item_stateset );
						}
					}
					else
					{
						grp->setStateSet( item_stateset );
					}
				}
			}
			else if( geom_type == AppearanceData::GEOM_TYPE_CURVE )
			{
				//osg::Vec4f color_lines( appearance->m_color_ambient.m_r, appearance->m_color_ambient.m_g, appearance->m_color_ambient.m_b, appearance->m_color_ambient.m_a );
				//GeomUtils::setColorToLines( grp, color_lines );
			}
		}
	}

	//\brief method convertProductShapeToOSG: creates geometry objects from an IfcProduct object
	// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
	void convertProductShapeToOSG( shared_ptr<ProductShapeDataOCC>& product_shape, std::map<int, osg::ref_ptr<osg::Switch> >& map_representation_switches )
	{
		if( product_shape->m_ifc_object_definition.expired() )
		{
			return;
		}

		RenderOptions render_options;
		shared_ptr<IfcObjectDefinition> ifc_object_def( product_shape->m_ifc_object_definition );
		shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(ifc_object_def);
		if( !ifc_product )
		{
			return;
		}
		const int product_id = ifc_product->m_entity_id;
		std::stringstream strs_product_switch_name;
		strs_product_switch_name << "#" << product_id << "=" << ifc_product->className() << " group";

		// create OSG objects
		std::vector<shared_ptr<RepresentationDataOCC> >& vec_product_representations = product_shape->m_vec_representations;
		for( size_t ii_representation = 0; ii_representation < vec_product_representations.size(); ++ii_representation )
		{
			const shared_ptr<RepresentationDataOCC>& product_representation_data = vec_product_representations[ii_representation];
			if( product_representation_data->m_ifc_representation.expired() )
			{
				continue;
			}
			shared_ptr<IfcRepresentation> ifc_representation( product_representation_data->m_ifc_representation );
			const int representation_id = ifc_representation->m_entity_id;
			osg::ref_ptr<osg::Switch> representation_switch = new osg::Switch();
			
#ifdef _DEBUG
			std::stringstream strs_representation_name;
			strs_representation_name << strs_product_switch_name.str().c_str() << ", representation " << ii_representation;
			representation_switch->setName( strs_representation_name.str().c_str() );
#endif

			const std::vector<shared_ptr<ItemShapeDataOCC> >& product_items = product_representation_data->m_vec_item_data;
			for( size_t i_item = 0; i_item < product_items.size(); ++i_item )
			{
				const shared_ptr<ItemShapeDataOCC>& item_input_data = product_items[i_item];
				osg::ref_ptr<osg::Group> item_group = new osg::Group();
				if( !item_group ) { throw OutOfMemoryException( __FUNC__ ); }

#ifdef _DEBUG
				std::stringstream strs_item_name;
				strs_item_name << strs_representation_name.str().c_str() << ", item " << i_item;
				item_group->setName( strs_item_name.str().c_str() );
#endif

				// create shape for open shells
				for( size_t ii_shapes = 0; ii_shapes < item_input_data->getShapes().size(); ++ii_shapes )
				{
					const TopoDS_Shape& item_shape = item_input_data->getShapes()[ii_shapes];

					osg::ref_ptr<osg::Geode> geode = new osg::Geode();
					if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
					drawShape( item_shape, geode, render_options );

					// disable back face culling for open meshes
					geode->getOrCreateStateSet()->setAttributeAndModes( m_cull_back_off.get(), osg::StateAttribute::OFF );
					if( geode->getNumDrawables() > 0 )
					{
						item_group->addChild( geode );
#ifdef _DEBUG
						std::stringstream strs_item_shape_name;
						strs_item_shape_name << strs_item_name.str().c_str() << ", open shape " << ii_shapes;
						geode->setName( strs_item_shape_name.str().c_str() );
#endif
					}
				}

				// create shape for points
				const std::vector<TopoDS_Vertex>& vertex_points = item_input_data->getVertexPoints();
				if( vertex_points.size() > 0 )
				{
					osg::ref_ptr<osg::Geode> geode = new osg::Geode();
					if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }

					osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();

					for( size_t ii_vertex_point = 0; ii_vertex_point < vertex_points.size(); ++ii_vertex_point )
					{
						const TopoDS_Vertex& vertex_input = vertex_points[ii_vertex_point];
						if( !vertex_input.IsNull() )
						{
							gp_Pnt point1 = BRep_Tool::Pnt( vertex_input );
							vertices->push_back( osg::Vec3d( point1.X(), point1.Y(), point1.Z() ) );
						}
					}

					if( vertices->size() > 0 )
					{
						osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
						geometry->setVertexArray( vertices );
						geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POINTS, 0, vertices->size() ) );
						geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
						geode->getOrCreateStateSet()->setAttribute( new osg::Point( 3.0f ), osg::StateAttribute::ON );
						geode->addDrawable( geometry );
						geode->setCullingActive( false );
						item_group->addChild( geode );

#ifdef _DEBUG
						std::stringstream strs_item_shape_name;
						strs_item_shape_name << strs_item_name.str().c_str() << ", vertex_point ";
						geode->setName( strs_item_shape_name.str().c_str() );
#endif
					}
					else
					{
						std::cout << __FUNC__ << ": unexpected vertices->size() == 0" << std::endl;
					}
				}

				// create shape for polylines
				for( size_t ii_shapes = 0; ii_shapes < item_input_data->getPolylines().size(); ++ii_shapes )
				{
					const TopoDS_Wire& polyline_data = item_input_data->getPolylines()[ii_shapes];
					osg::ref_ptr<osg::Geode> geode = new osg::Geode();
					if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
					geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

					RenderOptions render_options_polyline;
					if( item_input_data->getAppearances().size() > 0 )
					{
						for( size_t ii_appearances = 0; ii_appearances < item_input_data->getAppearances().size(); ++ii_appearances )
						{
							const shared_ptr<AppearanceData>& appearance = item_input_data->getAppearances()[ii_appearances];
							if( !appearance )
							{
								continue;
							}
							if( appearance->m_apply_to_geometry_type == AppearanceData::GEOM_TYPE_CURVE || appearance->m_apply_to_geometry_type == AppearanceData::GEOM_TYPE_ANY )
							{
								osg::Vec4f color_lines( appearance->m_color_ambient.m_r, appearance->m_color_ambient.m_g, appearance->m_color_ambient.m_b, appearance->m_color_ambient.m_a );
								render_options_polyline.m_color = color_lines;
								render_options_polyline.m_color_set = true;
								break;
							}
						}
					}

					drawShape( polyline_data, geode, render_options_polyline );
					if( geode->getNumDrawables() > 0 )
					{
						item_group->addChild( geode );
#ifdef _DEBUG
						std::stringstream strs_item_shape_name;
						strs_item_shape_name << strs_item_name.str().c_str() << ", polylines " << ii_shapes;
						geode->setName( strs_item_shape_name.str().c_str() );
#endif
					}
				}

				if( m_geom_settings->isShowTextLiterals() )
				{
					for( size_t ii = 0; ii < item_input_data->getTextItems().size(); ++ii )
					{
						const shared_ptr<TextItemDataOCC>& text_data = item_input_data->getTextItems()[ii];
						if( !text_data )
						{
							continue;
						}
						gp_Trsf& text_pos = text_data->m_text_position;
						// TODO: handle rotation

						std::string text_str;
						text_str.assign( text_data->m_text.begin(), text_data->m_text.end() );

						gp_XYZ pos_translation = text_pos.TranslationPart();
						osg::Vec3 pos2( pos_translation.X(), pos_translation.Y(), pos_translation.Z() );// text_pos._41, text_pos._42, text_pos._43 );

						osg::ref_ptr<osgText::Text> txt = new osgText::Text();
						if( !txt )
						{
							throw OutOfMemoryException( __FUNC__ );
						}
						txt->setFont( "fonts/arial.ttf" );
						txt->setColor( osg::Vec4f( 0, 0, 0, 1 ) );
						txt->setCharacterSize( 0.1f );
						txt->setAutoRotateToScreen( true );
						txt->setPosition( pos2 );
						txt->setText( text_str.c_str() );
						txt->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

						osg::ref_ptr<osg::Geode> geode = new osg::Geode();
						if( !geode ) { throw OutOfMemoryException( __FUNC__ ); }
						geode->addDrawable( txt );
						item_group->addChild( geode );
					}
				}

				// apply statesets if there are any
				if( item_input_data->getAppearances().size() > 0 )
				{
					applyAppearancesToGroup( item_input_data->getAppearances(), item_group );
				}

				// If anything has been created, add it to the representation group
				if( item_group->getNumChildren() > 0 )
				{
#ifdef _DEBUG
					if( item_group->getNumParents() > 0 )
					{
						std::cout << __FUNC__ << ": item_group->getNumParents() > 0" << std::endl;
					}
#endif
					representation_switch->addChild( item_group );
				}
			}

			// apply statesets if there are any
			if( product_representation_data->m_vec_representation_appearances.size() > 0 )
			{
				applyAppearancesToGroup( product_representation_data->m_vec_representation_appearances, representation_switch );
			}

			// If anything has been created, add it to the product group
			if( representation_switch->getNumChildren() > 0 )
			{
#ifdef _DEBUG
				if( representation_switch->getNumParents() > 0 )
				{
					std::cout << __FUNC__ << ": product_representation_switch->getNumParents() > 0" << std::endl;
				}
#endif

				// enable transparency for certain objects
				if( dynamic_pointer_cast<IfcSpace>(ifc_product) )
				{
					representation_switch->setStateSet( m_glass_stateset );
				}
				else if( dynamic_pointer_cast<IfcCurtainWall>(ifc_product) || dynamic_pointer_cast<IfcWindow>(ifc_product) )
				{
					representation_switch->setStateSet( m_glass_stateset );
					SceneGraphUtils::setMaterialAlpha( representation_switch, 0.6f, true );
				}
			}

			map_representation_switches.insert( std::make_pair( representation_id, representation_switch ) );
		}

		// TODO: if no color or material is given, set color 231/219/169 for walls, 140/140/140 for slabs 
	}

	/*\brief method convertToOSG: Creates geometry for OpenSceneGraph from given ProductShapeData.
	\param[out] parent_group Group to append the geometry.
	**/
	void convertToOSG( std::map<int, shared_ptr<ProductShapeDataOCC> >& map_shape_data, osg::ref_ptr<osg::Switch> parent_group )
	{
		progressTextCallback( L"Converting geometry to OpenGL format ..." );
		progressValueCallback( 0, "scenegraph" );
		m_map_entity_id_to_switch.clear();
		m_map_representation_to_switch.clear();

		shared_ptr<ProductShapeDataOCC> ifc_project_data;
		std::vector<shared_ptr<ProductShapeDataOCC> > vec_products;

		for( auto it = map_shape_data.begin(); it != map_shape_data.end(); ++it )
		{
			shared_ptr<ProductShapeDataOCC> shape_data = it->second;
			if( shape_data )
			{
				vec_products.push_back( shape_data );
			}
		}

		// create geometry for for each IfcProduct independently, spatial structure will be resolved later
		std::map<int, osg::ref_ptr<osg::Switch> >* map_entity_id = &m_map_entity_id_to_switch;
		std::map<int, osg::ref_ptr<osg::Switch> >* map_representations = &m_map_representation_to_switch;
		const int num_products = (int)vec_products.size();

	#ifdef ENABLE_OPENMP
		Mutex writelock_map;
		Mutex writelock_ifc_project;

	#pragma omp parallel firstprivate(num_products) shared(map_entity_id, map_representations)
		{
			// time for one product may vary significantly, so schedule not so many
	#pragma omp for schedule(dynamic,10)
	#endif
			for( int i = 0; i < num_products; ++i )
			{
				shared_ptr<ProductShapeDataOCC>& shape_data = vec_products[i];

				weak_ptr<IfcObjectDefinition>& ifc_object_def_weak = shape_data->m_ifc_object_definition;
				if( ifc_object_def_weak.expired() )
				{
					continue;
				}
				shared_ptr<IfcObjectDefinition> ifc_object_def( ifc_object_def_weak );

				std::stringstream thread_err;
				if( dynamic_pointer_cast<IfcFeatureElementSubtraction>(ifc_object_def) )
				{
					// geometry will be created in method subtractOpenings
					continue;
				}
				else if( dynamic_pointer_cast<IfcProject>(ifc_object_def) )
				{
	#ifdef ENABLE_OPENMP
					ScopedLock scoped_lock( writelock_ifc_project );
	#endif
					ifc_project_data = shape_data;
				}

				shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(ifc_object_def);
				if( !ifc_product )
				{
					continue;
				}

				if( !ifc_product->m_Representation )
				{
					continue;
				}

				const int product_id = ifc_product->m_entity_id;
				
				std::map<int, osg::ref_ptr<osg::Switch> > map_representation_switches;
				try
				{
					convertProductShapeToOSG( shape_data, map_representation_switches );
				}
				catch( OutOfMemoryException& e )
				{
					throw e;
				}
				catch( BuildingException& e )
				{
					thread_err << e.what();
				}
				catch( Standard_Failure& sf )
				{
					thread_err << sf.GetMessageString();
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
					osg::ref_ptr<osg::Switch> product_switch = new osg::Switch();
					std::stringstream strs_product_switch_name;
					strs_product_switch_name << "#" << product_id << "=" << ifc_product->className() << " group";
					product_switch->setName( strs_product_switch_name.str().c_str() );

					for( auto it_map = map_representation_switches.begin(); it_map != map_representation_switches.end(); ++it_map )
					{
						osg::ref_ptr<osg::Switch>& repres_switch = it_map->second;
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
					map_entity_id->insert( std::make_pair( product_id, product_switch ) );
					map_representations->insert( map_representation_switches.begin(), map_representation_switches.end() );
				}

				if( thread_err.tellp() > 0 )
				{
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
	}

	void addNodes( const std::map<int, shared_ptr<BuildingObject> >& map_shape_data, osg::ref_ptr<osg::Switch>& target_group )
	{
		// check if there are entities that are not in spatial structure
		if( !target_group )
		{
			target_group = new osg::Switch();
		}

		for( auto it_product_shapes = map_shape_data.begin(); it_product_shapes != map_shape_data.end(); ++it_product_shapes )
		{
			int product_id = it_product_shapes->first;
			auto it_find = m_map_entity_id_to_switch.find( product_id );

			if( it_find != m_map_entity_id_to_switch.end() )
			{
				osg::ref_ptr<osg::Switch>& sw = it_find->second;
				if( sw )
				{
					target_group->addChild( sw );
				}
			}
		}
	}

	bool inParentList( const int entity_id, osg::Group* group )
	{
		if( !group )
		{
			return false;
		}

		const osg::Group::ParentList& vec_parents = group->getParents();
		for( size_t ii = 0; ii < vec_parents.size(); ++ii )
		{
			osg::Group* parent = vec_parents[ii];
			if( parent )
			{
				const std::string parent_name = parent->getName();
				if( parent_name.length() > 0 )
				{
					if( parent_name.at( 0 ) == '#' )
					{
						// extract entity id
						std::string parent_name_id = parent_name.substr( 1 );
						size_t last_index = parent_name_id.find_first_not_of( "0123456789" );
						std::string id_str = parent_name_id.substr( 0, last_index );
						const int id = std::stoi( id_str.c_str() );
						if( id == entity_id )
						{
							return true;
						}
						bool in_parent_list = inParentList( entity_id, parent );
						if( in_parent_list )
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	void resolveProjectStructure( const shared_ptr<ProductShapeDataOCC>& product_data, osg::ref_ptr<osg::Switch> group )
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
		const int entity_id = object_def->m_entity_id;
		if( SceneGraphUtils::inParentList( entity_id, group ) )
		{
			messageCallback( "Cycle in project structure detected", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, object_def.get() );
			return;
		}

		const std::vector<shared_ptr<ProductShapeDataOCC> >& vec_children = product_data->getChildren();
		for( size_t ii = 0; ii < vec_children.size(); ++ii )
		{
			const shared_ptr<ProductShapeDataOCC>& child_product_data = vec_children[ii];
			if( !child_product_data )
			{
				continue;
			}

			osg::ref_ptr<osg::Switch> group_subparts = new osg::Switch();
			resolveProjectStructure( child_product_data, group_subparts );
			if( group_subparts->getNumChildren() > 0 )
			{
				if( !child_product_data->m_ifc_object_definition.expired() )
				{
					shared_ptr<IfcObjectDefinition> child_obj_def( child_product_data->m_ifc_object_definition );
					std::stringstream group_subparts_name;
					group_subparts_name << "#" << child_obj_def->m_entity_id << "=";
					group_subparts_name << child_obj_def->className();
					group_subparts->setName( group_subparts_name.str().c_str() );
				}

				group->addChild( group_subparts );
			}
		}

		auto it_product_map = m_map_entity_id_to_switch.find( entity_id );
		if( it_product_map != m_map_entity_id_to_switch.end() )
		{
			const osg::ref_ptr<osg::Switch>& product_switch = it_product_map->second;
			if( product_switch )
			{
				group->addChild( product_switch );
			}
		}
		else
		{
			if( group->getNumChildren() == 0 )
			{
				osg::ref_ptr<osg::Switch> product_switch = new osg::Switch();
				group->addChild( product_switch );

				std::stringstream switch_name;
				switch_name << "#" << entity_id << "=" << object_def->className();
				product_switch->setName( switch_name.str().c_str() );
			}
		}
	}

	void clearAppearanceCache()
	{
#ifdef ENABLE_OPENMP
		ScopedLock lock( m_writelock_appearance_cache );
#endif
		m_vec_existing_statesets.clear();
	}

	osg::StateSet* convertToOSGStateSet( const shared_ptr<AppearanceData>& appearence )
	{
		if( !appearence )
		{
			return nullptr;
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

		if( m_enable_stateset_caching )
		{
#ifdef ENABLE_OPENMP
			ScopedLock lock( m_writelock_appearance_cache );
#endif

			for( size_t i = 0; i<m_vec_existing_statesets.size(); ++i )
			{
				const osg::ref_ptr<osg::StateSet> stateset_existing = m_vec_existing_statesets[i];

				if( !stateset_existing.valid() )
				{
					continue;
				}

				osg::ref_ptr<osg::Material> mat_existing = (osg::Material*)stateset_existing->getAttribute( osg::StateAttribute::MATERIAL );
				if( !mat_existing )
				{
					continue;
				}

				// compare
				osg::Vec4f color_ambient_existing = mat_existing->getAmbient( osg::Material::FRONT_AND_BACK );
				if( abs( color_ambient_existing.r() - color_ambient_r ) > 0.03 ) break;
				if( abs( color_ambient_existing.g() - color_ambient_g ) > 0.03 ) break;
				if( abs( color_ambient_existing.b() - color_ambient_b ) > 0.03 ) break;
				if( abs( color_ambient_existing.a() - color_ambient_a ) > 0.03 ) break;

				osg::Vec4f color_diffuse_existing = mat_existing->getDiffuse( osg::Material::FRONT_AND_BACK );
				if( abs( color_diffuse_existing.r() - color_diffuse_r ) > 0.03 ) break;
				if( abs( color_diffuse_existing.g() - color_diffuse_g ) > 0.03 ) break;
				if( abs( color_diffuse_existing.b() - color_diffuse_b ) > 0.03 ) break;
				if( abs( color_diffuse_existing.a() - color_diffuse_a ) > 0.03 ) break;

				osg::Vec4f color_specular_existing = mat_existing->getSpecular( osg::Material::FRONT_AND_BACK );
				if( abs( color_specular_existing.r() - color_specular_r ) > 0.03 ) break;
				if( abs( color_specular_existing.g() - color_specular_g ) > 0.03 ) break;
				if( abs( color_specular_existing.b() - color_specular_b ) > 0.03 ) break;
				if( abs( color_specular_existing.a() - color_specular_a ) > 0.03 ) break;

				float shininess_existing = mat_existing->getShininess( osg::Material::FRONT_AND_BACK );
				if( abs( shininess_existing - shininess ) > 0.03 ) break;

				bool blend_on_existing = stateset_existing->getMode( GL_BLEND ) == osg::StateAttribute::ON;
				if( blend_on_existing != set_transparent ) break;

				bool transparent_bin = stateset_existing->getRenderingHint() == osg::StateSet::TRANSPARENT_BIN;
				if( transparent_bin != set_transparent ) break;

				// if we get here, appearance is same as existing state set
				// TODO: block this re-used stateset for merging, or prevent merged statesets from being re-used
				return stateset_existing;
			}
		}

		osg::Vec4f ambientColor( color_ambient_r, color_ambient_g, color_ambient_b, transparency );
		osg::Vec4f diffuseColor( color_diffuse_r, color_diffuse_g, color_diffuse_b, transparency );
		osg::Vec4f specularColor( color_specular_r, color_specular_g, color_specular_b, transparency );

		// TODO: material caching and re-use
		osg::ref_ptr<osg::Material> mat = new osg::Material();
		if( !mat ) { throw OutOfMemoryException(); }
		mat->setAmbient( osg::Material::FRONT_AND_BACK, ambientColor );
		mat->setDiffuse( osg::Material::FRONT_AND_BACK, diffuseColor );
		mat->setSpecular( osg::Material::FRONT_AND_BACK, specularColor );
		mat->setShininess( osg::Material::FRONT_AND_BACK, shininess );
		mat->setColorMode( osg::Material::SPECULAR );

		osg::StateSet* stateset = new osg::StateSet();
		if( !stateset ) { throw OutOfMemoryException(); }
		stateset->setAttribute( mat, osg::StateAttribute::ON );

		if( appearence->m_set_transparent )
		{
			mat->setTransparency( osg::Material::FRONT_AND_BACK, transparency );
			stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
			stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
		}

		if( appearence->m_specular_exponent != 0.f )
		{
			//osg::ref_ptr<osgFX::SpecularHighlights> spec_highlights = new osgFX::SpecularHighlights();
			//spec_highlights->setSpecularExponent( spec->m_value );
			// todo: add to scenegraph
		}

		if( m_enable_stateset_caching )
		{
			m_vec_existing_statesets.push_back( stateset );
		}
		return stateset;
	}
};

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

#define _USE_MATH_DEFINES 
#include <math.h>
#include <iterator>
#include <osg/Group>

#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcProductRepresentation.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>
#include <ifcpp/IFC4/include/IfcRepresentationItem.h>
#include <ifcpp/IFC4/include/IfcRelVoidsElement.h>
#include <ifcpp/IFC4/include/IfcFeatureElementSubtraction.h>
#include <ifcpp/IFC4/include/IfcElement.h>
#include <ifcpp/IFC4/include/IfcMappedItem.h>
#include <ifcpp/IFC4/include/IfcRepresentationMap.h>
#include <ifcpp/IFC4/include/IfcCartesianTransformationOperator.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement2D.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcPlacement.h>

#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcStyledItem.h>
#include <ifcpp/IFC4/include/IfcProperty.h>
#include <ifcpp/IFC4/include/IfcPropertySet.h>
#include <ifcpp/IFC4/include/IfcComplexProperty.h>
#include <ifcpp/IFC4/include/IfcSimpleProperty.h>
#include <ifcpp/IFC4/include/IfcPropertySingleValue.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcTextLiteral.h>
#include <ifcpp/IFC4/include/IfcPresentableText.h>
#include <ifcpp/IFC4/include/IfcTextPath.h>

#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcPlaneAngleMeasure.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcPolyline.h>
#include <ifcpp/IFC4/include/IfcBoundingBox.h>
#include <ifcpp/IFC4/include/IfcEdge.h>
#include <ifcpp/IFC4/include/IfcClosedShell.h>
#include <ifcpp/IFC4/include/IfcFaceBasedSurfaceModel.h>
#include <ifcpp/IFC4/include/IfcConnectedFaceSet.h>
#include <ifcpp/IFC4/include/IfcShellBasedSurfaceModel.h>
#include <ifcpp/IFC4/include/IfcOpenShell.h>
#include <ifcpp/IFC4/include/IfcFace.h>
#include <ifcpp/IFC4/include/IfcVertexPoint.h>
#include <ifcpp/IFC4/include/IfcVertex.h>
#include <ifcpp/IFC4/include/IfcGeometricSet.h>
#include <ifcpp/IFC4/include/IfcGeometricCurveSet.h>
#include <ifcpp/IFC4/include/IfcCurve.h>
#include <ifcpp/IFC4/include/IfcSurface.h>
#include <ifcpp/IFC4/include/IfcAnnotationFillArea.h>

#include <ifcpp/IFC4/include/IfcSolidModel.h>
#include <ifcpp/IFC4/include/IfcSectionedSpine.h>
#include <ifcpp/IFC4/include/IfcBooleanResult.h>
#include <ifcpp/IFC4/include/IfcPresentationLayerWithStyle.h>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/IfcPPException.h>

#include "IncludeCarveHeaders.h"
#include "GeomUtils.h"
#include "ConverterOSG.h"
#include "GeometrySettings.h"
#include "PlacementConverter.h"
#include "ProfileConverter.h"
#include "ProfileCache.h"
#include "StylesConverter.h"
#include "GeometryException.h"
#include "CurveConverter.h"
#include "SolidModelConverter.h"
#include "FaceConverter.h"
#include "DebugViewerCallback.h"
#include "CSG_Adapter.h"
#include "RepresentationConverter.h"

RepresentationConverter::RepresentationConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> unit_converter ) 
	: m_geom_settings(geom_settings), m_unit_converter(unit_converter)
{
	m_handle_styled_items = true;
	m_handle_layer_assignments = true;

	m_styles_converter = shared_ptr<StylesConverter>( new StylesConverter() );
	m_profile_cache = shared_ptr<ProfileCache>( new ProfileCache( m_geom_settings, m_unit_converter ) );
	m_curve_converter = shared_ptr<CurveConverter>( new CurveConverter( m_geom_settings, m_unit_converter ) );
	m_face_converter = shared_ptr<FaceConverter>( new FaceConverter( m_geom_settings, m_unit_converter, m_curve_converter ) );
	m_solid_converter = shared_ptr<SolidModelConverter>( new SolidModelConverter( m_geom_settings, m_unit_converter, m_curve_converter, m_face_converter, m_profile_cache ) );
}

RepresentationConverter::~RepresentationConverter()
{
}

void RepresentationConverter::convertStyledItem( const shared_ptr<IfcRepresentationItem>& representation_item, shared_ptr<ItemData>& item_data )
{
	std::vector<weak_ptr<IfcStyledItem> >&	StyledByItem_inverse_vec = representation_item->m_StyledByItem_inverse;
	for( unsigned int i=0; i<StyledByItem_inverse_vec.size(); ++i )
	{
		weak_ptr<IfcStyledItem> styled_item_weak = StyledByItem_inverse_vec[i];
		shared_ptr<IfcStyledItem> styled_item = shared_ptr<IfcStyledItem>(styled_item_weak);

#ifdef IFCPP_OPENMP
		ScopedLock lock(m_writelock_styles_converter);
#endif
		std::vector<shared_ptr<AppearanceData> > vec_appearance_data;
		m_styles_converter->convertIfcStyledItem( styled_item, vec_appearance_data );
		std::copy( vec_appearance_data.begin(), vec_appearance_data.end(), std::back_inserter( item_data->vec_item_appearances ) );
		//item_data->appearances.push_back( appearance_data );
	}
}

void RepresentationConverter::convertIfcRepresentation(  const shared_ptr<IfcRepresentation>& representation, shared_ptr<ShapeInputData>& input_data, std::stringstream& strs_err )
{
	const double length_factor = m_unit_converter->getLengthInMeterFactor();

	std::vector<shared_ptr<IfcRepresentationItem> >::const_iterator it_representation_items;
	for( it_representation_items=representation->m_Items.begin(); it_representation_items!=representation->m_Items.end(); ++it_representation_items )
	{
		shared_ptr<IfcRepresentationItem> representation_item = (*it_representation_items);
		shared_ptr<ItemData> item_data( new ItemData() );
		input_data->vec_item_data.push_back( item_data );

		if( m_handle_styled_items )
		{
			convertStyledItem( representation_item, item_data );
		}

		//ENTITY IfcRepresentationItem  ABSTRACT SUPERTYPE OF(ONEOF(IfcGeometricRepresentationItem, IfcMappedItem, IfcStyledItem, IfcTopologicalRepresentationItem));
		shared_ptr<IfcGeometricRepresentationItem> geom_item = dynamic_pointer_cast<IfcGeometricRepresentationItem>(representation_item);
		if( geom_item )
		{
			convertIfcGeometricRepresentationItem( geom_item, item_data, strs_err );
			continue;
		}
		
		shared_ptr<IfcMappedItem> mapped_item = dynamic_pointer_cast<IfcMappedItem>(representation_item);
		if( mapped_item )
		{
			shared_ptr<IfcRepresentationMap> map_source = mapped_item->m_MappingSource;
			if( !map_source )
			{
				strs_err << "unhandled representation: #" << representation_item->getId() << " = " << representation_item->classname() << std::endl;
				continue;
			}
			shared_ptr<IfcRepresentation> mapped_representation = map_source->m_MappedRepresentation;
			if( !mapped_representation )
			{
				strs_err << "unhandled representation: #" << representation_item->getId() << " = " << representation_item->classname() << std::endl;
				continue;
			}
		
			carve::math::Matrix map_matrix_target( carve::math::Matrix::IDENT() );
			if( mapped_item->m_MappingTarget )
			{
				shared_ptr<IfcCartesianTransformationOperator> transform_operator = mapped_item->m_MappingTarget;
				PlacementConverter::convertTransformationOperator(	transform_operator, map_matrix_target, length_factor );
			}

			carve::math::Matrix map_matrix_origin( carve::math::Matrix::IDENT() );
			shared_ptr<IfcAxis2Placement> mapping_origin = map_source->m_MappingOrigin;

			shared_ptr<IfcPlacement> mapping_origin_placement = dynamic_pointer_cast<IfcPlacement>( mapping_origin );
			if( mapping_origin_placement )
			{
				PlacementConverter::convertIfcPlacement( mapping_origin_placement, map_matrix_origin, length_factor );
			}
			else
			{
				strs_err << "#" << mapping_origin_placement->getId() << " = IfcPlacement: !dynamic_pointer_cast<IfcPlacement>( mapping_origin ) )";
				continue;
			}

			shared_ptr<ShapeInputData> mapped_input_data( new ShapeInputData() );
			std::copy( item_data->vec_item_appearances.begin(), item_data->vec_item_appearances.end(), std::back_inserter( mapped_input_data->vec_appearances ) );
			convertIfcRepresentation( mapped_representation, mapped_input_data, strs_err );
			carve::math::Matrix mapped_pos( map_matrix_target*map_matrix_origin );
			for( size_t i_item = 0; i_item < mapped_input_data->vec_item_data.size(); ++i_item )
			{
				mapped_input_data->vec_item_data[i_item]->applyPosition( mapped_pos );
			}
			input_data->addInputData( mapped_input_data );

			continue;
		}
		
		shared_ptr<IfcStyledItem> styled_item = dynamic_pointer_cast<IfcStyledItem>(representation_item);
		if( styled_item )
		{
			continue;
		}

		shared_ptr<IfcTopologicalRepresentationItem> topo_item = dynamic_pointer_cast<IfcTopologicalRepresentationItem>(representation_item);
		if( topo_item )
		{
			//IfcTopologicalRepresentationItem 		ABSTRACT SUPERTYPE OF(ONEOF(IfcConnectedFaceSet, IfcEdge, IfcFace, IfcFaceBound, IfcLoop, IfcPath, IfcVertex))
			shared_ptr<IfcConnectedFaceSet> topo_connected_face_set = dynamic_pointer_cast<IfcConnectedFaceSet>(topo_item);
			if( topo_connected_face_set )
			{
				continue;
			}

			shared_ptr<IfcEdge> topo_edge = dynamic_pointer_cast<IfcEdge>(topo_item);
			if( topo_edge )
			{
				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				polyline_data->beginPolyline();
				shared_ptr<IfcVertex>& vertex_start = topo_edge->m_EdgeStart;

				shared_ptr<IfcVertexPoint> vertex_start_point = dynamic_pointer_cast<IfcVertexPoint>(vertex_start);
				if( vertex_start_point )
				{
					if( vertex_start_point->m_VertexGeometry )
					{
						shared_ptr<IfcPoint> edge_start_point_geometry = vertex_start_point->m_VertexGeometry;
						shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>(edge_start_point_geometry);
						if( ifc_point )
						{
							if( ifc_point->m_Coordinates.size() > 2 )
							{
								carve::geom::vector<3> point = carve::geom::VECTOR( ifc_point->m_Coordinates[0]->m_value*length_factor, ifc_point->m_Coordinates[1]->m_value*length_factor, ifc_point->m_Coordinates[2]->m_value*length_factor );
								polyline_data->addVertex( point );
								polyline_data->addPolylineIndex(0);
							}
						}
					}
				}

				shared_ptr<IfcVertex>& vertex_end = topo_edge->m_EdgeEnd;

				shared_ptr<IfcVertexPoint> vertex_end_point = dynamic_pointer_cast<IfcVertexPoint>(vertex_end);
				if( vertex_end_point )
				{
					if( vertex_end_point->m_VertexGeometry )
					{
						shared_ptr<IfcPoint> edge_point_geometry = vertex_end_point->m_VertexGeometry;
						shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>(edge_point_geometry);
						if( ifc_point )
						{
							if( ifc_point->m_Coordinates.size() > 2 )
							{
								carve::geom::vector<3> point = carve::geom::VECTOR( ifc_point->m_Coordinates[0]->m_value*length_factor, ifc_point->m_Coordinates[1]->m_value*length_factor, ifc_point->m_Coordinates[2]->m_value*length_factor );
								polyline_data->addVertex( point );
								polyline_data->addPolylineIndex(1);
							}
						}
					}
				}
				item_data->polylines.push_back( polyline_data );
				continue;
			}
		}

		strs_err << "unhandled representation: #" << representation_item->getId() << " = " << representation_item->classname() << std::endl;
	}

	if( m_handle_layer_assignments )
	{
		std::vector<weak_ptr<IfcPresentationLayerAssignment> >& LayerAssignments_inverse = representation->m_LayerAssignments_inverse;
		std::vector<weak_ptr<IfcPresentationLayerAssignment> >::iterator it_LayerAssignments_inverse;
		for( it_LayerAssignments_inverse = LayerAssignments_inverse.begin(); it_LayerAssignments_inverse != LayerAssignments_inverse.end(); ++it_LayerAssignments_inverse)
		{
			shared_ptr<IfcPresentationLayerAssignment> layer_assignment( (*it_LayerAssignments_inverse) );

			shared_ptr<IfcPresentationLayerWithStyle> layer_assignment_with_style = dynamic_pointer_cast<IfcPresentationLayerWithStyle>( layer_assignment );
			if( layer_assignment_with_style )
			{
				std::vector<shared_ptr<IfcPresentationStyle> >& vec_presentation_styles = layer_assignment_with_style->m_LayerStyles;
				for( int i=0; i<vec_presentation_styles.size(); ++i )
				{
					shared_ptr<IfcPresentationStyle>&  presentation_style = vec_presentation_styles[i];

#ifdef IFCPP_OPENMP
					ScopedLock lock( m_writelock_styles_converter );
#endif
					shared_ptr<AppearanceData> appearance_data;
					input_data->vec_appearances.push_back( appearance_data );

					m_styles_converter->convertIfcPresentationStyle( presentation_style, appearance_data );

					//osg::StateSet* stateset = m_styles_converter->convertIfcPresentationStyle( presentation_style, item_data );
					//if( stateset != nullptr )
					//{
					//	input_data->vec_statesets.push_back(stateset);
					//}
				}
			}
		}
	}
}

void RepresentationConverter::convertIfcGeometricRepresentationItem( const shared_ptr<IfcGeometricRepresentationItem>& geom_item, shared_ptr<ItemData> item_data, std::stringstream& strs_err )
{
	//ENTITY IfcGeometricRepresentationItem
	//ABSTRACT SUPERTYPE OF(ONEOF(
	//	IfcAnnotationFillArea, IfcBooleanResult, IfcBoundingBox, IfcCartesianTransformationOperator, IfcCompositeCurveSegment, IfcCsgPrimitive3D,
	//	IfcCurve, IfcDefinedSymbol, IfcDirection, IfcFaceBasedSurfaceModel, IfcFillAreaStyleHatching, IfcFillAreaStyleTiles, IfcFillAreaStyleTileSymbolWithStyle,
	//	IfcGeometricSet, IfcHalfSpaceSolid, IfcLightSource, IfcOneDirectionRepeatFactor, IfcPlacement, IfcPlanarExtent, IfcPoint, IfcSectionedSpine,
	//	IfcShellBasedSurfaceModel, IfcSolidModel, IfcSurface, IfcTextLiteral, IfcTextureCoordinate, IfcTextureVertex, IfcVector))

	shared_ptr<IfcBoundingBox> bbox = dynamic_pointer_cast<IfcBoundingBox>(geom_item);
	if( bbox )
	{
		shared_ptr<IfcCartesianPoint> corner = bbox->m_Corner;
		shared_ptr<IfcPositiveLengthMeasure> x_dim = bbox->m_XDim;
		shared_ptr<IfcPositiveLengthMeasure> y_dim = bbox->m_YDim;
		shared_ptr<IfcPositiveLengthMeasure> z_dim = bbox->m_ZDim;
		return;
	}

	shared_ptr<IfcFaceBasedSurfaceModel> surface_model = dynamic_pointer_cast<IfcFaceBasedSurfaceModel>(geom_item);
	if( surface_model )
	{
		std::vector<shared_ptr<IfcConnectedFaceSet> >& vec_face_sets = surface_model->m_FbsmFaces;
		std::vector<shared_ptr<IfcConnectedFaceSet> >::iterator it_face_sets;
		
		for( it_face_sets=vec_face_sets.begin(); it_face_sets!=vec_face_sets.end(); ++it_face_sets )
		{
			shared_ptr<IfcConnectedFaceSet> face_set = (*it_face_sets);
			std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = face_set->m_CfsFaces;

			shared_ptr<ItemData> input_data_face_set( new ItemData );
			m_face_converter->convertIfcFaceList( vec_ifc_faces, input_data_face_set, strs_err );
			std::copy( input_data_face_set->open_or_closed_polyhedrons.begin(), input_data_face_set->open_or_closed_polyhedrons.end(), std::back_inserter(item_data->open_polyhedrons) );
		}
		
		return;
	}

	shared_ptr<IfcBooleanResult> boolean_result = dynamic_pointer_cast<IfcBooleanResult>(geom_item);
	if( boolean_result )
	{
		m_solid_converter->convertIfcBooleanResult( boolean_result, item_data, strs_err );
		return;
	}

	shared_ptr<IfcSolidModel> solid_model = dynamic_pointer_cast<IfcSolidModel>(geom_item);
	if( solid_model )
	{
		m_solid_converter->convertIfcSolidModel( solid_model, item_data, strs_err );
		return;
	}

	shared_ptr<IfcCurve> curve = dynamic_pointer_cast<IfcCurve>(geom_item);
	if( curve )
	{
		std::vector<carve::geom::vector<3> > loops;
		std::vector<carve::geom::vector<3> > segment_start_points;
		m_curve_converter->convertIfcCurve( curve, loops, segment_start_points );

		shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		polyline_data->beginPolyline();
		for( int i=0; i<loops.size(); ++i )
		{
			carve::geom::vector<3> point = loops[i];
			polyline_data->addVertex( point );
			polyline_data->addPolylineIndex(i);
		}
		item_data->polylines.push_back( polyline_data );
		
		return;
	}

	shared_ptr<IfcShellBasedSurfaceModel> shell_based_surface_model = dynamic_pointer_cast<IfcShellBasedSurfaceModel>(geom_item);
	if( shell_based_surface_model )
	{
		std::vector<shared_ptr<IfcShell> >& vec_shells = shell_based_surface_model->m_SbsmBoundary;
		for( std::vector<shared_ptr<IfcShell> >::iterator it_shells=vec_shells.begin(); it_shells!=vec_shells.end(); ++it_shells )
		{
			shared_ptr<IfcShell> shell_select = (*it_shells);
			if( dynamic_pointer_cast<IfcClosedShell>( shell_select ) )
			{
				shared_ptr<IfcClosedShell> closed_shell = dynamic_pointer_cast<IfcClosedShell>( shell_select );
				std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = closed_shell->m_CfsFaces;

				shared_ptr<ItemData> input_data( new ItemData() );
				m_face_converter->convertIfcFaceList( vec_ifc_faces, input_data, strs_err );
				std::copy( input_data->open_or_closed_polyhedrons.begin(), input_data->open_or_closed_polyhedrons.end(), std::back_inserter(item_data->closed_polyhedrons) );
			}
			else if( dynamic_pointer_cast<IfcOpenShell>( shell_select ) )
			{
				shared_ptr<IfcOpenShell> open_shell = dynamic_pointer_cast<IfcOpenShell>( shell_select );
				std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = open_shell->m_CfsFaces;

				shared_ptr<ItemData> input_data( new ItemData() );
				m_face_converter->convertIfcFaceList( vec_ifc_faces, input_data, strs_err );

				std::copy( input_data->open_or_closed_polyhedrons.begin(), input_data->open_or_closed_polyhedrons.end(), std::back_inserter(item_data->open_polyhedrons) );
			}
		}
		return;
	}

	shared_ptr<IfcSurface> surface = dynamic_pointer_cast<IfcSurface>(geom_item);
	if( surface )
	{
		shared_ptr<carve::input::PolylineSetData> polyline( new carve::input::PolylineSetData() );
		m_face_converter->convertIfcSurface( surface, polyline );
		if( polyline->getVertexCount() > 1 )
		{
			item_data->polylines.push_back( polyline );
		}
		return;
	}
	
	shared_ptr<IfcPolyline> poly_line = dynamic_pointer_cast<IfcPolyline>(geom_item);
	if( poly_line )
	{
		std::vector<carve::geom::vector<3> > poly_vertices;
		m_curve_converter->convertIfcPolyline( poly_line, poly_vertices );

		const unsigned int num_points = poly_vertices.size();
		shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		
		polyline_data->beginPolyline();

		// apply position
		for( unsigned int i=0; i<num_points; ++i )
		{
			carve::geom::vector<3>& vertex = poly_vertices[i];
			//vertex = vertex;

			polyline_data->addVertex(vertex);
			polyline_data->addPolylineIndex(i);
		}
		item_data->polylines.push_back(polyline_data);

		return;
	}

	shared_ptr<IfcGeometricSet> geometric_set = dynamic_pointer_cast<IfcGeometricSet>(geom_item);
	if( geometric_set )
	{
		// ENTITY IfcGeometricSet SUPERTYPE OF(IfcGeometricCurveSet)
		std::vector<shared_ptr<IfcGeometricSetSelect> >& geom_set_elements = geometric_set->m_Elements;
		std::vector<shared_ptr<IfcGeometricSetSelect> >::iterator it_set_elements;
		for( it_set_elements=geom_set_elements.begin(); it_set_elements != geom_set_elements.end(); ++it_set_elements )
		{
			// TYPE IfcGeometricSetSelect = SELECT (IfcPoint, IfcCurve, IfcSurface);
			shared_ptr<IfcGeometricSetSelect>& geom_select = (*it_set_elements);

			shared_ptr<IfcPoint> select_point = dynamic_pointer_cast<IfcPoint>(geom_select);
			if( select_point )
			{

				continue;
			}

			shared_ptr<IfcCurve> select_curve = dynamic_pointer_cast<IfcCurve>(geom_select);
			if( select_curve )
			{
				
				std::vector<carve::geom::vector<3> > loops;
				std::vector<carve::geom::vector<3> > segment_start_points;
				m_curve_converter->convertIfcCurve( select_curve, loops, segment_start_points );

				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				polyline_data->beginPolyline();
				for( int i=0; i<loops.size(); ++i )
				{
					carve::geom::vector<3> point = loops[i];
					polyline_data->addVertex( point );
					polyline_data->addPolylineIndex(i);
				}
				item_data->polylines.push_back( polyline_data );

				continue;
			}

			shared_ptr<IfcSurface> select_surface = dynamic_pointer_cast<IfcSurface>(geom_select);
			if( select_surface )
			{
				shared_ptr<carve::input::PolylineSetData> polyline( new carve::input::PolylineSetData() );
				m_face_converter->convertIfcSurface( select_surface, polyline );
				if( polyline->getVertexCount() > 1 )
				{
					item_data->polylines.push_back( polyline );
				}

				continue;
			}
		}

		shared_ptr<IfcGeometricCurveSet> geometric_curve_set = dynamic_pointer_cast<IfcGeometricCurveSet>(geometric_set);
		if( geometric_curve_set )
		{
			// no additional attributes
			return;
		}
		return;
	}

	shared_ptr<IfcSectionedSpine> sectioned_spine = dynamic_pointer_cast<IfcSectionedSpine>(geom_item);
	if( sectioned_spine )
	{
		convertIfcSectionedSpine( sectioned_spine, item_data, strs_err );
		return;
	}

	shared_ptr<IfcTextLiteral> text_literal = dynamic_pointer_cast<IfcTextLiteral>(geom_item);
	if( text_literal )
	{
		// Literal		: 	IfcPresentableText;
		// Placement	: 	IfcAxis2Placement;
		// Path			: 	IfcTextPath;
		if( m_geom_settings->m_show_text_literals )
		{
			shared_ptr<IfcPresentableText>& ifc_literal = text_literal->m_Literal;
			std::string& literal_text = ifc_literal->m_value;
			shared_ptr<IfcAxis2Placement>& text_placement = text_literal->m_Placement;
			
			carve::math::Matrix text_position_matrix( carve::math::Matrix::IDENT() );
			double length_factor = m_unit_converter->getLengthInMeterFactor();
			shared_ptr<IfcAxis2Placement3D> placement_3d = dynamic_pointer_cast<IfcAxis2Placement3D>( text_placement );
			if( placement_3d )
			{
				PlacementConverter::convertIfcAxis2Placement3D( placement_3d, text_position_matrix, length_factor );
			}
			else
			{
				shared_ptr<IfcAxis2Placement2D> placement_2d = dynamic_pointer_cast<IfcAxis2Placement2D>( text_placement );
				if( placement_2d )
				{
					PlacementConverter::convertIfcAxis2Placement2D( placement_2d, text_position_matrix, length_factor );
				}
			}

			shared_ptr<IfcTextPath>& path = text_literal->m_Path;
			shared_ptr<TextItemData> text_item_data( new TextItemData() );
			text_item_data->m_text_position = text_position_matrix;
			text_item_data->m_text = literal_text;

			item_data->vec_text_literals.push_back( text_item_data );
		}
		return;
	}

	shared_ptr<IfcAnnotationFillArea> annotation_fill_area = dynamic_pointer_cast<IfcAnnotationFillArea>(geom_item);
	if( annotation_fill_area )
	{
		// convert outer boundary
		shared_ptr<IfcCurve>& outer_boundary = annotation_fill_area->m_OuterBoundary;
		std::vector<std::vector<carve::geom::vector<3> > > face_loops;
		face_loops.push_back( std::vector<carve::geom::vector<3> >() );
		std::vector<carve::geom::vector<3> >& outer_boundary_loop = face_loops.back();
		std::vector<carve::geom::vector<3> > segment_start_points;
		m_curve_converter->convertIfcCurve( outer_boundary, outer_boundary_loop, segment_start_points );

		// convert inner boundaries
		std::vector<shared_ptr<IfcCurve> >& vec_inner_boundaries = annotation_fill_area->m_InnerBoundaries;			//optional
		for( std::vector<shared_ptr<IfcCurve> >::iterator it = vec_inner_boundaries.begin(); it != vec_inner_boundaries.end(); ++it )
		{
			shared_ptr<IfcCurve>& inner_boundary = *it;
			face_loops.push_back( std::vector<carve::geom::vector<3> >() );
			std::vector<carve::geom::vector<3> >& inner_boundary_loop = face_loops.back();
			std::vector<carve::geom::vector<3> > segment_start_points;
			m_curve_converter->convertIfcCurve( inner_boundary, inner_boundary_loop, segment_start_points );
		}
		
		PolyInputCache3D poly_cache;
		GeomUtils::createFace( face_loops, poly_cache, strs_err );
		item_data->open_polyhedrons.push_back( poly_cache.m_poly_data );
		
		return;
	}
	
	strs_err << "Unhandled IFC Representation: #" << geom_item->getId() << "=" << geom_item->classname() << std::endl;
}

void RepresentationConverter::subtractOpenings( const shared_ptr<IfcElement>& ifc_element, shared_ptr<ShapeInputData>& product_shape, std::stringstream& strs_err )
{
	std::vector<shared_ptr<ShapeInputData> > vec_opening_data;
	std::vector<weak_ptr<IfcRelVoidsElement> > vec_rel_voids( ifc_element->m_HasOpenings_inverse );
	if( vec_rel_voids.size() == 0 )
	{
		return;
	}
	const int product_id = ifc_element->getId();
	const double length_factor = m_unit_converter->getLengthInMeterFactor();

	// convert opening representation
	for( int i_void=0; i_void<vec_rel_voids.size(); ++i_void )
	{
		weak_ptr<IfcRelVoidsElement>& rel_voids_weak = vec_rel_voids[i_void];
		if( rel_voids_weak.expired() )
		{
			continue;
		}
		shared_ptr<IfcRelVoidsElement> rel_voids( rel_voids_weak );
		shared_ptr<IfcFeatureElementSubtraction> opening = rel_voids->m_RelatedOpeningElement;
		if( !opening )
		{
			continue;
		}
		if( !opening->m_Representation )
		{
			continue;
		}

		const int opening_id = opening->getId();

		// opening can have its own relative placement
		shared_ptr<IfcObjectPlacement>	opening_placement = opening->m_ObjectPlacement;			//optional
		carve::math::Matrix opening_placement_matrix( carve::math::Matrix::IDENT() );
		if( opening_placement )
		{
			std::set<int> opening_placements_applied;
			PlacementConverter::convertIfcObjectPlacement( opening_placement, opening_placement_matrix, length_factor, opening_placements_applied );
		}

		std::vector<shared_ptr<IfcRepresentation> >& vec_opening_representations = opening->m_Representation->m_Representations;
		for( std::vector<shared_ptr<IfcRepresentation> >::iterator it_representations=vec_opening_representations.begin(); it_representations!=vec_opening_representations.end(); ++it_representations )
		{
			shared_ptr<IfcRepresentation> ifc_opening_representation = (*it_representations);
			shared_ptr<ShapeInputData> opening_representation_data( new ShapeInputData() );
			opening_representation_data->representation = ifc_opening_representation;
			
			// TODO: Representation caching, one element could be used for several openings
			convertIfcRepresentation( ifc_opening_representation, opening_representation_data, strs_err );

			for( size_t i_item = 0; i_item < opening_representation_data->vec_item_data.size(); ++i_item )
			{
				opening_representation_data->vec_item_data[i_item]->applyPosition( opening_placement_matrix );
			}
			vec_opening_data.push_back( opening_representation_data );
		}
	}

	shared_ptr<carve::mesh::MeshSet<3> > unified_opening_meshset;
	for( int i_opening=0; i_opening<vec_opening_data.size(); ++i_opening )
	{
		shared_ptr<ShapeInputData>& opening_representation_data = vec_opening_data[i_opening];
		int representation_id = -1;
		if( opening_representation_data->representation )
		{
			representation_id = opening_representation_data->representation->getId();
		}

		std::vector<shared_ptr<ItemData> >& vec_opening_items = opening_representation_data->vec_item_data;
		for( int i_item=0; i_item<vec_opening_items.size(); ++i_item )
		{
			shared_ptr<ItemData>& opening_item_data = vec_opening_items[i_item];
			opening_item_data->createMeshSetsFromClosedPolyhedrons();
				
			std::vector<shared_ptr<carve::mesh::MeshSet<3> > >::iterator it_opening_meshsets = opening_item_data->meshsets.begin();
			for( ; it_opening_meshsets != opening_item_data->meshsets.end(); ++it_opening_meshsets )
			{
				shared_ptr<carve::mesh::MeshSet<3> > opening_meshset = (*it_opening_meshsets);

				if( !unified_opening_meshset )
				{
					unified_opening_meshset = opening_meshset;
					continue;
				}

				// do the unification
				shared_ptr<carve::mesh::MeshSet<3> > result;
				bool csg_op_ok = CSG_Adapter::computeCSG( unified_opening_meshset, opening_meshset, carve::csg::CSG::UNION, product_id, representation_id, strs_err, result );
				//item_data->m_csg_computed = true;
				if( csg_op_ok )
				{
					unified_opening_meshset = result;
				}
			}
		}
	}

	std::vector<shared_ptr<ItemData> >& product_items = product_shape->vec_item_data;
	for( int i_item=0; i_item<product_items.size(); ++i_item )
	{
		shared_ptr<ItemData> item_data = product_items[i_item];

		// now go through all meshsets of the item
		for( int i_product_meshset=0; i_product_meshset<item_data->meshsets.size(); ++i_product_meshset )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& product_meshset = item_data->meshsets[i_product_meshset];
			std::stringstream strs_meshset_err;
			bool product_meshset_valid_for_csg = CSG_Adapter::checkMeshSetValidAndClosed( product_meshset.get(), strs_meshset_err, product_id );
			if( !product_meshset_valid_for_csg )
			{
				continue;
			}


			// do the subtraction
			shared_ptr<carve::mesh::MeshSet<3> > result;
			bool csg_op_ok = CSG_Adapter::computeCSG( product_meshset, unified_opening_meshset, carve::csg::CSG::A_MINUS_B, product_id, -1, strs_err, result );
			item_data->m_csg_computed = true;
			if( csg_op_ok )
			{
				product_meshset = result;
			}
		}
	}
}

void RepresentationConverter::convertIfcPropertySet( const shared_ptr<IfcPropertySet>& prop_set, osg::Group* group )
{
	std::vector<shared_ptr<IfcProperty> >& vec_hasProperties = prop_set->m_HasProperties;

	std::vector<shared_ptr<IfcProperty> >::iterator it;
	for( it=vec_hasProperties.begin(); it!=vec_hasProperties.end(); ++it )
	{
		shared_ptr<IfcProperty> prop = (*it);

		shared_ptr<IfcSimpleProperty> simple_property = dynamic_pointer_cast<IfcSimpleProperty>(prop);
		if( simple_property )
		{
			shared_ptr<IfcIdentifier> property_name = simple_property->m_Name;
			std::string name_str = property_name->m_value;
			if( name_str.compare( "LayerName" ) == 0 )
			{
				// TODO: implement layers
			}
			shared_ptr<IfcText> description = simple_property->m_Description;


			shared_ptr<IfcPropertySingleValue> property_single_value = dynamic_pointer_cast<IfcPropertySingleValue>(simple_property);
			if( property_single_value )
			{
				shared_ptr<IfcValue>& nominal_value = property_single_value->m_NominalValue;				//optional
				shared_ptr<IfcUnit>& unit = property_single_value->m_Unit;						//optional

			}

			//ENTITY IfcSimpleProperty
			//ABSTRACT SUPERTYPE OF(ONEOF(
			//IfcPropertyBoundedValue,
			//IfcPropertyEnumeratedValue,
			//IfcPropertyListValue,
			//IfcPropertyReferenceValue,
			//IfcPropertySingleValue,
			//IfcPropertyTableValue))
			continue;
		}
		
		shared_ptr<IfcComplexProperty> complex_property = dynamic_pointer_cast<IfcComplexProperty>(prop);
		if( complex_property )
		{
			std::vector<shared_ptr<IfcProperty> >& vec_HasProperties = complex_property->m_HasProperties;
			if( !complex_property->m_UsageName ) continue;
			if( complex_property->m_UsageName->m_value.compare( "Color" ) == 0 )
			{

#ifdef IFCPP_OPENMP
				ScopedLock lock( m_writelock_styles_converter );
#endif
				shared_ptr<AppearanceData> appearance_data;
				m_styles_converter->convertIfcComplexPropertyColor( complex_property, appearance_data );

				osg::ref_ptr<osg::StateSet> stateset = AppearanceManagerOSG::convertToStateSet( appearance_data );

				if( stateset.valid() )
				{
					group->setStateSet( stateset );
				}
			}
		}
	}
}

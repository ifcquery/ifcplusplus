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

#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcProductRepresentation.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>
#include <ifcpp/IFC4/include/IfcRepresentationItem.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationSubContext.h>
#include <ifcpp/IFC4/include/IfcRelVoidsElement.h>
#include <ifcpp/IFC4/include/IfcFeatureElementSubtraction.h>
#include <ifcpp/IFC4/include/IfcElement.h>
#include <ifcpp/IFC4/include/IfcMappedItem.h>
#include <ifcpp/IFC4/include/IfcRepresentationMap.h>
#include <ifcpp/IFC4/include/IfcCartesianTransformationOperator.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement2D.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcPlacement.h>

#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcStyledItem.h>
#include <ifcpp/IFC4/include/IfcTextLiteral.h>
#include <ifcpp/IFC4/include/IfcPresentableText.h>

#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
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
#include <ifcpp/IFC4/include/IfcPoint.h>
#include <ifcpp/IFC4/include/IfcVertexPoint.h>
#include <ifcpp/IFC4/include/IfcVertex.h>
#include <ifcpp/IFC4/include/IfcGeometricSetSelect.h>
#include <ifcpp/IFC4/include/IfcGeometricSet.h>
#include <ifcpp/IFC4/include/IfcGeometricCurveSet.h>
#include <ifcpp/IFC4/include/IfcCurve.h>
#include <ifcpp/IFC4/include/IfcLoop.h>
#include <ifcpp/IFC4/include/IfcSurface.h>
#include <ifcpp/IFC4/include/IfcAnnotationFillArea.h>

#include <ifcpp/IFC4/include/IfcSolidModel.h>
#include <ifcpp/IFC4/include/IfcSectionedSpine.h>
#include <ifcpp/IFC4/include/IfcBooleanResult.h>
#include <ifcpp/IFC4/include/IfcShell.h>
#include <ifcpp/IFC4/include/IfcPresentationLayerWithStyle.h>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/IfcPPException.h>

#include "IncludeCarveHeaders.h"
#include "GeomUtils.h"
#include "GeometrySettings.h"
#include "PlacementConverter.h"
#include "PointConverter.h"
#include "ProfileConverter.h"
#include "ProfileCache.h"
#include "StylesConverter.h"
#include "GeometryException.h"
#include "CurveConverter.h"
#include "SolidModelConverter.h"
#include "FaceConverter.h"
#include "SplineConverter.h"
#include "Sweeper.h"
#include "CSG_Adapter.h"
#include "RepresentationConverter.h"

RepresentationConverter::RepresentationConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> unit_converter )
	: m_geom_settings( geom_settings ), m_unit_converter( unit_converter )
{
	m_handle_styled_items = true;
	m_handle_layer_assignments = true;

	m_styles_converter = shared_ptr<StylesConverter>( new StylesConverter() );
	m_point_converter = shared_ptr<PointConverter>( new PointConverter( m_geom_settings, m_unit_converter ) );
	m_spline_converter = shared_ptr<SplineConverter>( new SplineConverter( m_point_converter ) );
	m_sweeper = shared_ptr<Sweeper>( new Sweeper( m_geom_settings, m_unit_converter ) );
	m_curve_converter = shared_ptr<CurveConverter>( new CurveConverter( m_geom_settings, m_unit_converter, m_point_converter, m_spline_converter ) );
	m_profile_cache = shared_ptr<ProfileCache>( new ProfileCache( m_curve_converter, m_spline_converter ) );
	m_face_converter = shared_ptr<FaceConverter>( new FaceConverter( m_geom_settings, m_unit_converter, m_curve_converter, m_spline_converter, m_sweeper ) );
	m_solid_converter = shared_ptr<SolidModelConverter>( new SolidModelConverter( m_geom_settings, m_unit_converter, m_point_converter, m_curve_converter, m_face_converter, m_profile_cache, m_sweeper ) );
	
	// this redirects the callback messages from all converters to RepresentationConverter's callback
	addCallbackChild( m_styles_converter.get() );
	addCallbackChild( m_point_converter.get() );
	addCallbackChild( m_spline_converter.get() );
	addCallbackChild( m_profile_cache.get() );
	addCallbackChild( m_curve_converter.get() );
	addCallbackChild( m_face_converter.get() );
	addCallbackChild( m_solid_converter.get() );
	addCallbackChild( m_sweeper.get() );
}

RepresentationConverter::~RepresentationConverter()
{
	clearCache();
}

void RepresentationConverter::clearCache()
{
	m_profile_cache->clearProfileCache();
	m_styles_converter->clearStylesCache();
}

void RepresentationConverter::setUnitConverter( shared_ptr<UnitConverter>& unit_converter )
{
	m_unit_converter = unit_converter;
	m_point_converter->m_unit_converter = unit_converter;
	m_sweeper->m_unit_converter = unit_converter;
	m_curve_converter->m_unit_converter = unit_converter;
	m_face_converter->m_unit_converter = unit_converter;
	m_solid_converter->m_unit_converter = unit_converter;
}

void RepresentationConverter::convertRepresentationStyle( const shared_ptr<IfcRepresentationItem>& representation_item, std::vector<shared_ptr<AppearanceData> >& vec_appearance_data )
{
	std::vector<weak_ptr<IfcStyledItem> >&	vec_StyledByItem_inverse = representation_item->m_StyledByItem_inverse;
	for( size_t i = 0; i < vec_StyledByItem_inverse.size(); ++i )
	{
		weak_ptr<IfcStyledItem> styled_item_weak = vec_StyledByItem_inverse[i];
		shared_ptr<IfcStyledItem> styled_item = shared_ptr<IfcStyledItem>( styled_item_weak );
		m_styles_converter->convertIfcStyledItem( styled_item, vec_appearance_data );
	}
}

void RepresentationConverter::convertIfcRepresentation( const shared_ptr<IfcRepresentation>& representation, shared_ptr<ProductShapeInputData>& input_data )
{
	if( representation->m_RepresentationIdentifier )
	{
		// http://www.buildingsmart-tech.org/ifc/IFC4/final/html/index.htm
		// Box	Bounding box as simplified 3D box geometry of an element
		// Annotation	2D annotations not representing elements
		// Axis	2D or 3D Axis, or single line, representation of an element
		// FootPrint	2D Foot print, or double line, representation of an element, projected to ground view
		// Profile	3D line representation of a profile being planar, e.g. used for door and window outlines
		// Surface	3D Surface representation, e.g. of an analytical surface, of an elementplane)
		// Body	3D Body representation, e.g. as wireframe, surface, or solid model, of an element
		// Body-FallBack	3D Body representation, e.g. as tessellation, or other surface, or boundary representation, added in addition to the solid model (potentially involving Boolean operations) of an element
		// Clearance	3D clearance volume of the element. Such clearance region indicates space that should not intersect with the 'Body' representation of other elements, though may intersect with the 'Clearance' representation of other elements.
		// Lighting	Representation of emitting light as a light source within a shape representation
		input_data->m_representation_identifier = representation->m_RepresentationIdentifier->m_value;
	}

	if( representation->m_RepresentationType )
	{
		// Point	2 or 3 dimensional point(s)
		// PointCloud	3 dimensional points prepresented by a point list
		// Curve	2 or 3 dimensional curve(s)
		// Curve2D	2 dimensional curve(s)
		// Curve3D	3 dimensional curve(s)
		// Surface	2 or 3 dimensional surface(s)
		// Surface2D	2 dimensional surface(s) (a region on ground view)
		// Surface3D	3 dimensional surface(s)
		// FillArea	2D region(s) represented as a filled area (hatching)
		// Text	text defined as text literals
		// AdvancedSurface	3 dimensional b-spline surface(s)
		// GeometricSet	points, curves, surfaces (2 or 3 dimensional)
		//		GeometricCurveSet	points, curves (2 or 3 dimensional)
		//		Annotation2D	points, curves (2 or 3 dimensional), hatches and text (2 dimensional)
		// SurfaceModel	face based and shell based surface model(s), or tessellated surface model(s)
		//		Tessellation	tessellated surface representation(s) only
		// SolidModel	including swept solid, Boolean results and Brep bodies; more specific types are:
		//		SweptSolid	swept area solids, by extrusion and revolution, excluding tapered sweeps
		//		AdvancedSweptSolid	swept area solids created by sweeping a profile along a directrix, and tapered sweeps
		//		Brep	faceted Brep's with and without voids
		//		AdvancedBrep	Brep's based on advanced faces, with b-spline surface geometry, with and without voids
		//		CSG	Boolean results of operations between solid models, half spaces and Boolean results
		//		Clipping	Boolean differences between swept area solids, half spaces and Boolean results
		input_data->m_representation_type = representation->m_RepresentationType->m_value;
	}

	shared_ptr<IfcRepresentationContext>& context = representation->m_ContextOfItems;
	if( context )
	{
		shared_ptr<IfcGeometricRepresentationContext> geom_context = dynamic_pointer_cast<IfcGeometricRepresentationContext>( context );
		if( geom_context )
		{
			// TODO: save in ProductShapeInputData
			shared_ptr<IfcGeometricRepresentationSubContext> geom_sub_context = dynamic_pointer_cast<IfcGeometricRepresentationSubContext>( context );
			if( geom_sub_context )
			{

			}
		}
	}

	const double length_factor = m_unit_converter->getLengthInMeterFactor();
	for( size_t i_representation_items = 0; i_representation_items < representation->m_Items.size(); ++i_representation_items )
	{
		shared_ptr<IfcRepresentationItem> representation_item = representation->m_Items[i_representation_items];

		//ENTITY IfcRepresentationItem  ABSTRACT SUPERTYPE OF(ONEOF(IfcGeometricRepresentationItem, IfcMappedItem, IfcStyledItem, IfcTopologicalRepresentationItem));
		shared_ptr<IfcGeometricRepresentationItem> geom_item = dynamic_pointer_cast<IfcGeometricRepresentationItem>( representation_item );
		if( geom_item )
		{
			shared_ptr<ItemShapeInputData> geom_item_data( new ItemShapeInputData() );
			if( !geom_item_data )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			input_data->m_vec_item_data.push_back( geom_item_data );

			try
			{
				convertIfcGeometricRepresentationItem( geom_item, geom_item_data );
			}
			catch( IfcPPOutOfMemoryException& e)
			{
				throw e;
			}
			catch( IfcPPException& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", representation_item.get() );
			}
			catch( std::exception& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, representation_item.get() );
			}
			continue;
		}

		shared_ptr<IfcMappedItem> mapped_item = dynamic_pointer_cast<IfcMappedItem>( representation_item );
		if( mapped_item )
		{
			shared_ptr<IfcRepresentationMap> map_source = mapped_item->m_MappingSource;
			if( !map_source )
			{
				messageCallback( "MappingSource not valid", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, representation_item.get() );
				continue;
			}
			shared_ptr<IfcRepresentation> mapped_representation = map_source->m_MappedRepresentation;
			if( !mapped_representation )
			{
				messageCallback( "MappingSource.MappedRepresentation not valid", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, representation_item.get() );
				continue;
			}

			carve::math::Matrix map_matrix_target( carve::math::Matrix::IDENT() );
			if( mapped_item->m_MappingTarget )
			{
				shared_ptr<IfcCartesianTransformationOperator> transform_operator = mapped_item->m_MappingTarget;
				PlacementConverter::convertTransformationOperator( transform_operator, length_factor, map_matrix_target, this );
			}

			carve::math::Matrix map_matrix_origin( carve::math::Matrix::IDENT() );
			shared_ptr<IfcAxis2Placement> mapping_origin_select = map_source->m_MappingOrigin;
			if( mapping_origin_select )
			{
				shared_ptr<IfcPlacement> mapping_origin_placement = dynamic_pointer_cast<IfcPlacement>( mapping_origin_select );
				if( mapping_origin_placement )
				{
					PlacementConverter::convertIfcPlacement( mapping_origin_placement, length_factor, map_matrix_origin, this );
				}
				else
				{
					messageCallback( "!dynamic_pointer_cast<IfcPlacement>( mapping_origin )", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, mapping_origin_placement.get() );
					continue;
				}
			}

			shared_ptr<ProductShapeInputData> mapped_input_data( new ProductShapeInputData() );
			if( !mapped_input_data )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}

			try
			{
				convertIfcRepresentation( mapped_representation, mapped_input_data );
			}
			catch( IfcPPOutOfMemoryException& e)
			{
				throw e;
			}
			catch( IfcPPException& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
			}
			catch( std::exception& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
			}

			if( m_handle_styled_items )
			{
				std::vector<shared_ptr<AppearanceData> > vec_appearance_data;
				convertRepresentationStyle( representation_item, vec_appearance_data );

				if( vec_appearance_data.size() > 0 )
				{
					// overwrite item appearances with parent appearance
					for( size_t i_mapped_item = 0; i_mapped_item < mapped_input_data->m_vec_item_data.size(); ++i_mapped_item )
					{
						shared_ptr<ItemShapeInputData>& mapped_item_data = mapped_input_data->m_vec_item_data[i_mapped_item];
						mapped_item_data->m_vec_item_appearances.clear();

						for( size_t jj_appearance = 0; jj_appearance < vec_appearance_data.size(); ++jj_appearance )
						{
							shared_ptr<AppearanceData>& data = vec_appearance_data[jj_appearance];
							if( data )
							{
								mapped_item_data->m_vec_item_appearances.push_back( data );
							}
						}
					}
				}
			}

			carve::math::Matrix mapped_pos( map_matrix_target*map_matrix_origin );
			mapped_input_data->applyPosition( mapped_pos );
			input_data->addInputData( mapped_input_data );

			continue;
		}

		shared_ptr<IfcStyledItem> styled_item = dynamic_pointer_cast<IfcStyledItem>( representation_item );
		if( styled_item )
		{
			continue;
		}

		shared_ptr<IfcTopologicalRepresentationItem> topo_item = dynamic_pointer_cast<IfcTopologicalRepresentationItem>( representation_item );
		if( topo_item )
		{
			shared_ptr<ItemShapeInputData> topo_item_data( new ItemShapeInputData() );
			if( !topo_item_data )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			input_data->m_vec_item_data.push_back( topo_item_data );

			//IfcTopologicalRepresentationItem 		ABSTRACT SUPERTYPE OF(ONEOF(IfcConnectedFaceSet, IfcEdge, IfcFace, IfcFaceBound, IfcLoop, IfcPath, IfcVertex))
			shared_ptr<IfcConnectedFaceSet> topo_connected_face_set = dynamic_pointer_cast<IfcConnectedFaceSet>( topo_item );
			if( topo_connected_face_set )
			{
				messageCallback( "IfcConnectedFaceSet not implemented", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, representation_item.get() );
				continue;
			}

			shared_ptr<IfcEdge> topo_edge = dynamic_pointer_cast<IfcEdge>( topo_item );
			if( topo_edge )
			{
				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				if( !polyline_data )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}
				topo_item_data->m_polylines.push_back( polyline_data );
				polyline_data->beginPolyline();
				shared_ptr<IfcVertex>& vertex_start = topo_edge->m_EdgeStart;

				shared_ptr<IfcVertexPoint> vertex_start_point = dynamic_pointer_cast<IfcVertexPoint>( vertex_start );
				if( vertex_start_point )
				{
					if( vertex_start_point->m_VertexGeometry )
					{
						shared_ptr<IfcPoint> edge_start_point_geometry = vertex_start_point->m_VertexGeometry;
						shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>( edge_start_point_geometry );
						if( ifc_point )
						{
							if( ifc_point->m_Coordinates.size() > 2 )
							{
								carve::geom::vector<3> point = carve::geom::VECTOR( ifc_point->m_Coordinates[0]->m_value*length_factor, ifc_point->m_Coordinates[1]->m_value*length_factor, ifc_point->m_Coordinates[2]->m_value*length_factor );
								polyline_data->addVertex( point );
								polyline_data->addPolylineIndex( 0 );
							}
						}
					}
				}

				shared_ptr<IfcVertex>& vertex_end = topo_edge->m_EdgeEnd;
				shared_ptr<IfcVertexPoint> vertex_end_point = dynamic_pointer_cast<IfcVertexPoint>( vertex_end );
				if( vertex_end_point )
				{
					if( vertex_end_point->m_VertexGeometry )
					{
						shared_ptr<IfcPoint> edge_point_geometry = vertex_end_point->m_VertexGeometry;
						shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>( edge_point_geometry );
						if( ifc_point )
						{
							if( ifc_point->m_Coordinates.size() > 2 )
							{
								carve::geom::vector<3> point = carve::geom::VECTOR( ifc_point->m_Coordinates[0]->m_value*length_factor, ifc_point->m_Coordinates[1]->m_value*length_factor, ifc_point->m_Coordinates[2]->m_value*length_factor );
								polyline_data->addVertex( point );
								polyline_data->addPolylineIndex( 1 );
							}
						}
					}
				}
				topo_item_data->m_polylines.push_back( polyline_data );
				continue;
			}

			shared_ptr<IfcLoop> topo_loop = dynamic_pointer_cast<IfcLoop>( topo_item );
			if( topo_loop )
			{
				std::vector<carve::geom::vector<3> > loop_points;
				m_curve_converter->convertIfcLoop( topo_loop, loop_points );

				if( loop_points.size() > 0 )
				{
					shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
					if( !polyline_data )
					{
						throw IfcPPOutOfMemoryException( __FUNC__ );
					}
					topo_item_data->m_polylines.push_back( polyline_data );
					polyline_data->beginPolyline();

					for( size_t ii_loop = 0; ii_loop < loop_points.size(); ++ii_loop )
					{
						polyline_data->addVertex( loop_points[ii_loop] );
						polyline_data->addPolylineIndex( 1 );
					}
				}
				continue;
			}

			shared_ptr<IfcVertex> topo_vertex = dynamic_pointer_cast<IfcVertex>( topo_item );
			if( topo_vertex )
			{
				shared_ptr<IfcVertexPoint> topo_vertex_point = dynamic_pointer_cast<IfcVertexPoint>( topo_vertex );
				if( topo_vertex_point )
				{
					shared_ptr<IfcPoint> topo_vertex_point_geometry = topo_vertex_point->m_VertexGeometry;
					if( topo_vertex_point_geometry )
					{
						shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>( topo_vertex_point_geometry );
						if( !ifc_point )
						{
							carve::geom::vector<3> carve_point;
							m_point_converter->convertIfcCartesianPoint( ifc_point, carve_point );

							shared_ptr<carve::input::PointSetData> pointset_data( new carve::input::PointSetData() );
							if( !pointset_data )
							{
								throw IfcPPOutOfMemoryException( __FUNC__ );
							}
							topo_item_data->m_vertex_points.push_back( pointset_data );
							pointset_data->addVertex( carve_point );
						}
					}
				}
			}
		}

		messageCallback( "unhandled representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, representation_item.get() );
	}

	if( m_handle_layer_assignments )
	{
		std::vector<weak_ptr<IfcPresentationLayerAssignment> >& vec_layer_assignments_inverse = representation->m_LayerAssignments_inverse;
		for( size_t ii = 0; ii < vec_layer_assignments_inverse.size(); ++ii )
		{
			shared_ptr<IfcPresentationLayerAssignment> layer_assignment( vec_layer_assignments_inverse[ii] );
			if( layer_assignment )
			{
				shared_ptr<IfcPresentationLayerWithStyle> layer_assignment_with_style = dynamic_pointer_cast<IfcPresentationLayerWithStyle>( layer_assignment );
				if( layer_assignment_with_style )
				{
					std::vector<shared_ptr<IfcPresentationStyle> >& vec_presentation_styles = layer_assignment_with_style->m_LayerStyles;
					for( size_t ii_style = 0; ii_style < vec_presentation_styles.size(); ++ii_style  )
					{
						shared_ptr<IfcPresentationStyle>&  presentation_style = vec_presentation_styles[ii_style];
						if( presentation_style )
						{
							shared_ptr<AppearanceData> appearance_data;
							m_styles_converter->convertIfcPresentationStyle( presentation_style, appearance_data );
							if( appearance_data )
							{
								input_data->addAppearance( appearance_data );
							}
						}
					}
				}
			}
		}
	}
}

void RepresentationConverter::convertIfcGeometricRepresentationItem( const shared_ptr<IfcGeometricRepresentationItem>& geom_item, shared_ptr<ItemShapeInputData> item_data )
{
	//ENTITY IfcGeometricRepresentationItem
	//ABSTRACT SUPERTYPE OF(ONEOF(
	//	IfcAnnotationFillArea, IfcBooleanResult, IfcBoundingBox, IfcCartesianTransformationOperator, IfcCompositeCurveSegment, IfcCsgPrimitive3D,
	//	IfcCurve, IfcDefinedSymbol, IfcDirection, IfcFaceBasedSurfaceModel, IfcFillAreaStyleHatching, IfcFillAreaStyleTiles, IfcFillAreaStyleTileSymbolWithStyle,
	//	IfcGeometricSet, IfcHalfSpaceSolid, IfcLightSource, IfcOneDirectionRepeatFactor, IfcPlacement, IfcPlanarExtent, IfcPoint, IfcSectionedSpine,
	//	IfcShellBasedSurfaceModel, IfcSolidModel, IfcSurface, IfcTextLiteral, IfcTextureCoordinate, IfcTextureVertex, IfcVector))
	if( m_handle_styled_items )
	{
		std::vector<shared_ptr<AppearanceData> > vec_appearance_data;
		convertRepresentationStyle( geom_item, vec_appearance_data );

		for( size_t jj = 0; jj < vec_appearance_data.size(); ++jj )
		{
			shared_ptr<AppearanceData>& data = vec_appearance_data[jj];
			if( data )
			{
				item_data->m_vec_item_appearances.push_back( data );
			}
		}
	}

	shared_ptr<IfcBoundingBox> bbox = dynamic_pointer_cast<IfcBoundingBox>( geom_item );
	if( bbox )
	{
		shared_ptr<IfcCartesianPoint> corner = bbox->m_Corner;
		shared_ptr<IfcPositiveLengthMeasure> x_dim = bbox->m_XDim;
		shared_ptr<IfcPositiveLengthMeasure> y_dim = bbox->m_YDim;
		shared_ptr<IfcPositiveLengthMeasure> z_dim = bbox->m_ZDim;
		return;
	}

	shared_ptr<IfcFaceBasedSurfaceModel> surface_model = dynamic_pointer_cast<IfcFaceBasedSurfaceModel>( geom_item );
	if( surface_model )
	{
		std::vector<shared_ptr<IfcConnectedFaceSet> >& vec_face_sets = surface_model->m_FbsmFaces;
		for( auto it_face_sets = vec_face_sets.begin(); it_face_sets != vec_face_sets.end(); ++it_face_sets )
		{
			shared_ptr<IfcConnectedFaceSet> face_set = ( *it_face_sets );
			std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = face_set->m_CfsFaces;
			m_face_converter->convertIfcFaceList( vec_ifc_faces, item_data, FaceConverter::SHELL_TYPE_UNKONWN );
		}

		return;
	}

	shared_ptr<IfcBooleanResult> boolean_result = dynamic_pointer_cast<IfcBooleanResult>( geom_item );
	if( boolean_result )
	{
		m_solid_converter->convertIfcBooleanResult( boolean_result, item_data );
		return;
	}

	shared_ptr<IfcSolidModel> solid_model = dynamic_pointer_cast<IfcSolidModel>( geom_item );
	if( solid_model )
	{
		m_solid_converter->convertIfcSolidModel( solid_model, item_data );
		return;
	}

	shared_ptr<IfcCurve> ifc_curve = dynamic_pointer_cast<IfcCurve>( geom_item );
	if( ifc_curve )
	{
		std::vector<carve::geom::vector<3> > loops;
		std::vector<carve::geom::vector<3> > segment_start_points;
		m_curve_converter->convertIfcCurve( ifc_curve, loops, segment_start_points );

		shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		if( !polyline_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		polyline_data->beginPolyline();
		for( size_t i = 0; i < loops.size(); ++i )
		{
			carve::geom::vector<3> point = loops[i];
			polyline_data->addVertex( point );
			polyline_data->addPolylineIndex( i );
		}
		item_data->m_polylines.push_back( polyline_data );

		return;
	}

	shared_ptr<IfcShellBasedSurfaceModel> shell_based_surface_model = dynamic_pointer_cast<IfcShellBasedSurfaceModel>( geom_item );
	if( shell_based_surface_model )
	{
		std::vector<shared_ptr<IfcShell> >& vec_shells = shell_based_surface_model->m_SbsmBoundary;
		for( std::vector<shared_ptr<IfcShell> >::iterator it_shells = vec_shells.begin(); it_shells != vec_shells.end(); ++it_shells )
		{
			// TYPE IfcShell = SELECT	(IfcClosedShell	,IfcOpenShell);
			shared_ptr<IfcShell> shell_select = ( *it_shells );
			shared_ptr<IfcClosedShell> closed_shell = dynamic_pointer_cast<IfcClosedShell>( shell_select );
			if( closed_shell )
			{
				std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = closed_shell->m_CfsFaces;
				m_face_converter->convertIfcFaceList( vec_ifc_faces, item_data, FaceConverter::CLOSED_SHELL );
				continue;
			}

			shared_ptr<IfcOpenShell> open_shell = dynamic_pointer_cast<IfcOpenShell>( shell_select );
			if( open_shell )
			{
				std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = open_shell->m_CfsFaces;
				m_face_converter->convertIfcFaceList( vec_ifc_faces, item_data, FaceConverter::OPEN_SHELL );
				continue;
			}
		}

		return;
	}

	shared_ptr<IfcSurface> ifc_surface = dynamic_pointer_cast<IfcSurface>( geom_item );
	if( ifc_surface )
	{
		shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		if( !polyline_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		shared_ptr<SurfaceProxy> surface_proxy;
		m_face_converter->convertIfcSurface( ifc_surface, polyline_data, surface_proxy );

		// TODO: create open mesh for surface

		if( polyline_data->getVertexCount() > 1 )
		{
			item_data->m_polylines.push_back( polyline_data );
		}
		return;
	}

	shared_ptr<IfcPolyline> poly_line = dynamic_pointer_cast<IfcPolyline>( geom_item );
	if( poly_line )
	{
		std::vector<carve::geom::vector<3> > poly_vertices;
		m_curve_converter->convertIfcPolyline( poly_line, poly_vertices );

		const size_t num_points = poly_vertices.size();
		shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		if( !polyline_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		polyline_data->beginPolyline();

		// apply position
		for( size_t i = 0; i < num_points; ++i )
		{
			carve::geom::vector<3>& vertex = poly_vertices[i];
			polyline_data->addVertex( vertex );
			polyline_data->addPolylineIndex( i );
		}
		item_data->m_polylines.push_back( polyline_data );

		return;
	}

	shared_ptr<IfcGeometricSet> geometric_set = dynamic_pointer_cast<IfcGeometricSet>( geom_item );
	if( geometric_set )
	{
		// ENTITY IfcGeometricSet SUPERTYPE OF(IfcGeometricCurveSet)
		std::vector<shared_ptr<IfcGeometricSetSelect> >& geom_set_elements = geometric_set->m_Elements;
		std::vector<shared_ptr<IfcGeometricSetSelect> >::iterator it_set_elements;
		for( it_set_elements = geom_set_elements.begin(); it_set_elements != geom_set_elements.end(); ++it_set_elements )
		{
			// TYPE IfcGeometricSetSelect = SELECT (IfcPoint, IfcCurve, IfcSurface);
			shared_ptr<IfcGeometricSetSelect>& geom_select = ( *it_set_elements );
	
			shared_ptr<IfcPoint> point = dynamic_pointer_cast<IfcPoint>( geom_select );
			if( point )
			{
				// TODO: implement
				continue;
			}

			shared_ptr<IfcCurve> select_curve = dynamic_pointer_cast<IfcCurve>( geom_select );
			if( select_curve )
			{
				std::vector<carve::geom::vector<3> > loops;
				std::vector<carve::geom::vector<3> > segment_start_points;
				m_curve_converter->convertIfcCurve( select_curve, loops, segment_start_points );

				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				if( !polyline_data )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}
				polyline_data->beginPolyline();
				for( size_t i = 0; i < loops.size(); ++i )
				{
					carve::geom::vector<3> point = loops[i];
					polyline_data->addVertex( point );
					polyline_data->addPolylineIndex( i );
				}
				item_data->m_polylines.push_back( polyline_data );

				continue;
			}

			shared_ptr<IfcSurface> select_surface = dynamic_pointer_cast<IfcSurface>( geom_select );
			if( select_surface )
			{
				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				if( !polyline_data )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}
				shared_ptr<SurfaceProxy> surface_proxy;
				m_face_converter->convertIfcSurface( select_surface, polyline_data, surface_proxy );

				// TODO: create open mesh for surface

				if( polyline_data->getVertexCount() > 1 )
				{
					item_data->m_polylines.push_back( polyline_data );
				}

				continue;
			}
		}

		shared_ptr<IfcGeometricCurveSet> geometric_curve_set = dynamic_pointer_cast<IfcGeometricCurveSet>( geometric_set );
		if( geometric_curve_set )
		{
			// no additional attributes
			return;
		}
		return;
	}

	shared_ptr<IfcSectionedSpine> sectioned_spine = dynamic_pointer_cast<IfcSectionedSpine>( geom_item );
	if( sectioned_spine )
	{
		m_solid_converter->convertIfcSectionedSpine( sectioned_spine, item_data );
		return;
	}

	shared_ptr<IfcTextLiteral> text_literal = dynamic_pointer_cast<IfcTextLiteral>( geom_item );
	if( text_literal )
	{
		// Literal		: 	IfcPresentableText;
		// Placement	: 	IfcAxis2Placement;
		// Path			: 	IfcTextPath;
		if( m_geom_settings->isShowTextLiterals() )
		{
			shared_ptr<IfcPresentableText>& ifc_literal = text_literal->m_Literal;
			std::wstring& literal_text = ifc_literal->m_value;

			// check if text has a local placemnt
			carve::math::Matrix text_position_matrix( carve::math::Matrix::IDENT() );
			shared_ptr<IfcAxis2Placement>& text_placement_select = text_literal->m_Placement;
			if( text_placement_select )
			{
				double length_factor = m_unit_converter->getLengthInMeterFactor();
				shared_ptr<IfcAxis2Placement3D> placement_3d = dynamic_pointer_cast<IfcAxis2Placement3D>( text_placement_select );
				if( placement_3d )
				{
					PlacementConverter::convertIfcAxis2Placement3D( placement_3d, length_factor, text_position_matrix );
				}
				else
				{
					shared_ptr<IfcAxis2Placement2D> placement_2d = dynamic_pointer_cast<IfcAxis2Placement2D>( text_placement_select );
					if( placement_2d )
					{
						PlacementConverter::convertIfcAxis2Placement2D( placement_2d, length_factor, text_position_matrix );
					}
				}
			}

			shared_ptr<IfcTextPath>& path = text_literal->m_Path;
			shared_ptr<TextItemData> text_item_data( new TextItemData() );
			if( !text_item_data )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			text_item_data->m_text_position = text_position_matrix;
			text_item_data->m_text = literal_text;

			item_data->m_vec_text_literals.push_back( text_item_data );
		}
		return;
	}

	shared_ptr<IfcAnnotationFillArea> annotation_fill_area = dynamic_pointer_cast<IfcAnnotationFillArea>( geom_item );
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
		m_sweeper->createFace( face_loops, outer_boundary.get(), poly_cache );
		item_data->addOpenPolyhedron( poly_cache.m_poly_data );

		return;
	}

	shared_ptr<IfcPoint> ifc_point = dynamic_pointer_cast<IfcPoint>( geom_item );
	if( ifc_point )
	{
		// ENTITY IfcPoint ABSTRACT SUPERTYPE OF( ONEOF ( IfcCartesianPoint, IfcPointOnCurve, IfcPointOnSurface ) )
		shared_ptr<IfcCartesianPoint> ifc_cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>( geom_item );
		if( ifc_cartesian_point )
		{
			carve::geom::vector<3> point;
			m_point_converter->convertIfcCartesianPoint( ifc_cartesian_point, point );
			shared_ptr<carve::input::VertexData> vertex_data;
			if( item_data->m_vertex_points.size() > 0 )
			{
				if( !item_data->m_vertex_points[0] )
				{
					item_data->m_vertex_points[0] = shared_ptr<carve::input::VertexData>( new carve::input::VertexData() );
				}
				vertex_data = item_data->m_vertex_points[0];
			}
			else
			{
				vertex_data = shared_ptr<carve::input::VertexData>( new carve::input::VertexData() );
				item_data->m_vertex_points.push_back( vertex_data );
			}
						
			vertex_data->points.push_back( point );
			return;
		}
	}

	messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, geom_item.get() );
}

void RepresentationConverter::subtractOpenings( const shared_ptr<IfcElement>& ifc_element, shared_ptr<ProductShapeInputData>& product_shape )
{
	std::vector<shared_ptr<ProductShapeInputData> > vec_opening_data;
	std::vector<weak_ptr<IfcRelVoidsElement> > vec_rel_voids( ifc_element->m_HasOpenings_inverse );
	if( vec_rel_voids.size() == 0 )
	{
		return;
	}
	const int product_id = ifc_element->m_id;
	const double length_factor = m_unit_converter->getLengthInMeterFactor();

	// convert opening representation
	for( size_t i_void = 0; i_void < vec_rel_voids.size(); ++i_void )
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

		const int opening_id = opening->m_id;

		// opening can have its own relative placement
		shared_ptr<IfcObjectPlacement>	opening_placement = opening->m_ObjectPlacement;			//optional
		carve::math::Matrix opening_placement_matrix( carve::math::Matrix::IDENT() );
		if( opening_placement )
		{
			std::unordered_set<IfcObjectPlacement*> opening_placements_applied;
			PlacementConverter::convertIfcObjectPlacement( opening_placement, length_factor, opening_placement_matrix, this, opening_placements_applied );
		}

		std::vector<shared_ptr<IfcRepresentation> >& vec_opening_representations = opening->m_Representation->m_Representations;
		for( size_t i_representations = 0; i_representations < vec_opening_representations.size(); ++i_representations )
		{
			shared_ptr<IfcRepresentation> ifc_opening_representation = vec_opening_representations[i_representations];
			shared_ptr<ProductShapeInputData> opening_representation_data( new ProductShapeInputData() );
			if( !opening_representation_data )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			opening_representation_data->m_representation = ifc_opening_representation;

			// TODO: Representation caching, one element could be used for several openings
			try
			{
				convertIfcRepresentation( ifc_opening_representation, opening_representation_data );
			}
			catch( IfcPPOutOfMemoryException& e )
			{
				throw e;
			}
			catch( IfcPPException& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", ifc_element.get() );
			}
			catch( std::exception& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", ifc_element.get() );
			}

			opening_representation_data->applyPosition( opening_placement_matrix );
			vec_opening_data.push_back( opening_representation_data );
		}
	}

	shared_ptr<carve::mesh::MeshSet<3> > unified_opening_meshset;
	for( size_t i_opening = 0; i_opening < vec_opening_data.size(); ++i_opening )
	{
		shared_ptr<ProductShapeInputData>& opening_representation_data = vec_opening_data[i_opening];
		int representation_id = -1;
		shared_ptr<IfcRepresentation> opening_representation( opening_representation_data->m_representation );
		if( opening_representation )
		{
			representation_id = opening_representation->m_id;
		}

		std::vector<shared_ptr<ItemShapeInputData> >& vec_opening_items = opening_representation_data->m_vec_item_data;
		for( size_t i_item = 0; i_item < vec_opening_items.size(); ++i_item )
		{
			shared_ptr<ItemShapeInputData>& opening_item_data = vec_opening_items[i_item];
			if( opening_item_data )
			{
				std::vector<shared_ptr<carve::mesh::MeshSet<3> > >&	opening_meshsets = opening_item_data->m_meshsets;
				for( size_t i_opening_meshset = 0; i_opening_meshset < opening_meshsets.size(); ++i_opening_meshset )
				{
					shared_ptr<carve::mesh::MeshSet<3> > opening_meshset = opening_meshsets[i_opening_meshset];

					if( !unified_opening_meshset )
					{
						unified_opening_meshset = opening_meshset;
						continue;
					}

					// do the unification
					shared_ptr<carve::mesh::MeshSet<3> > result;
					try
					{
						CSG_Adapter::computeCSG( unified_opening_meshset, opening_meshset, carve::csg::CSG::UNION, product_id, representation_id, result );
					}
					catch( IfcPPOutOfMemoryException& e )
					{
						throw e;
					}
					catch( IfcPPException& e )
					{
						messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", ifc_element.get() );
					}
					catch( std::exception& e )
					{
						messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, ifc_element.get() );
					}
					unified_opening_meshset = result;
				}
			}
		}
	}

	if( unified_opening_meshset )
	{
		std::vector<shared_ptr<ItemShapeInputData> >& product_items = product_shape->m_vec_item_data;
		for( size_t i_item = 0; i_item < product_items.size(); ++i_item )
		{
			shared_ptr<ItemShapeInputData> item_data = product_items[i_item];

			// now go through all meshsets of the item
			for( size_t i_product_meshset = 0; i_product_meshset < item_data->m_meshsets.size(); ++i_product_meshset )
			{
				shared_ptr<carve::mesh::MeshSet<3> >& product_meshset = item_data->m_meshsets[i_product_meshset];
				std::stringstream strs_meshset_err;
				bool product_meshset_valid_for_csg = CSG_Adapter::checkMeshSetValidAndClosed( product_meshset.get(), strs_meshset_err, product_id );
				if( !product_meshset_valid_for_csg )
				{
					continue;
				}

				// do the subtraction
				shared_ptr<carve::mesh::MeshSet<3> > result;
				try
				{
					CSG_Adapter::computeCSG( product_meshset, unified_opening_meshset, carve::csg::CSG::A_MINUS_B, product_id, -1, result );
				}
				catch( IfcPPOutOfMemoryException& e)
				{
					throw e;
				}
				catch( IfcPPException& e )
				{
					messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", ifc_element.get() );
				}
				catch( std::exception& e )
				{
					messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, ifc_element.get() );
				}
				product_meshset = result;
			}
		}
	}
}

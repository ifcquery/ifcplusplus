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
#include <ifcpp/geometry/StylesConverter.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/IFC4/include/IfcAnnotationFillArea.h>
#include <ifcpp/IFC4/include/IfcBooleanResult.h>
#include <ifcpp/IFC4/include/IfcBoundingBox.h>
#include <ifcpp/IFC4/include/IfcClosedShell.h>
#include <ifcpp/IFC4/include/IfcConnectedFaceSet.h>
#include <ifcpp/IFC4/include/IfcElement.h>
#include <ifcpp/IFC4/include/IfcFaceBasedSurfaceModel.h>
#include <ifcpp/IFC4/include/IfcFaceSurface.h>
#include <ifcpp/IFC4/include/IfcFeatureElementSubtraction.h>
#include <ifcpp/IFC4/include/IfcGeometricCurveSet.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationItem.h>
#include <ifcpp/IFC4/include/IfcGeometricSet.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcMappedItem.h>
#include <ifcpp/IFC4/include/IfcOpenShell.h>
#include <ifcpp/IFC4/include/IfcPath.h>
#include <ifcpp/IFC4/include/IfcPresentableText.h>
#include <ifcpp/IFC4/include/IfcPresentationLayerWithStyle.h>
#include <ifcpp/IFC4/include/IfcProductRepresentation.h>
#include <ifcpp/IFC4/include/IfcPropertySet.h>
#include <ifcpp/IFC4/include/IfcSectionedSpine.h>
#include <ifcpp/IFC4/include/IfcShellBasedSurfaceModel.h>
#include <ifcpp/IFC4/include/IfcSolidModel.h>
#include <ifcpp/IFC4/include/IfcStyledItem.h>
#include <ifcpp/IFC4/include/IfcRelVoidsElement.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>
#include <ifcpp/IFC4/include/IfcRepresentationItem.h>
#include <ifcpp/IFC4/include/IfcRepresentationMap.h>
#include <ifcpp/IFC4/include/IfcTessellatedItem.h>
#include <ifcpp/IFC4/include/IfcTextLiteral.h>

#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "Sweeper.h"
#include "SplineConverter.h"
#include "PointConverter.h"
#include "CurveConverter.h"
#include "SolidModelConverter.h"
#include "FaceConverter.h"
#include "ProfileCache.h"
#include "TessellatedItemConverter.h"

class RepresentationConverter : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;
	shared_ptr<StylesConverter>			m_styles_converter;
	shared_ptr<PointConverter>			m_point_converter;
	shared_ptr<SplineConverter>			m_spline_converter;
	shared_ptr<Sweeper>					m_sweeper;
	shared_ptr<PlacementConverter>		m_placement_converter;
	shared_ptr<CurveConverter>			m_curve_converter;
	shared_ptr<ProfileCache>			m_profile_cache;
	shared_ptr<FaceConverter>			m_face_converter;
	shared_ptr<SolidModelConverter>		m_solid_converter;
	shared_ptr<TessellatedItemConverter> m_tessel_converter;
	
public:
	RepresentationConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> unit_converter )
		: m_geom_settings( geom_settings ), m_unit_converter( unit_converter )
	{
		m_styles_converter = shared_ptr<StylesConverter>( new StylesConverter() );
		m_point_converter = shared_ptr<PointConverter>( new PointConverter( m_unit_converter ) );
		m_spline_converter = shared_ptr<SplineConverter>( new SplineConverter( m_geom_settings, m_point_converter ) );
		m_sweeper = shared_ptr<Sweeper>( new Sweeper( m_geom_settings, m_unit_converter ) );
		m_placement_converter = shared_ptr<PlacementConverter>( new PlacementConverter( m_unit_converter ) );
		m_curve_converter = shared_ptr<CurveConverter>( new CurveConverter( m_geom_settings, m_placement_converter, m_point_converter, m_spline_converter ) );
		m_profile_cache = shared_ptr<ProfileCache>( new ProfileCache( m_curve_converter, m_spline_converter ) );
		m_face_converter = shared_ptr<FaceConverter>( new FaceConverter( m_geom_settings, m_unit_converter, m_curve_converter, m_spline_converter, m_sweeper ) );
		m_solid_converter = shared_ptr<SolidModelConverter>( new SolidModelConverter( m_geom_settings, m_point_converter, m_curve_converter, m_face_converter, m_profile_cache, m_sweeper ) );
		m_tessel_converter = shared_ptr<TessellatedItemConverter>( new TessellatedItemConverter( m_unit_converter ) );
		

		// this redirects the callback messages from all converters to RepresentationConverter's callback
		m_styles_converter->setMessageTarget( this );
		m_point_converter->setMessageTarget( this );
		m_spline_converter->setMessageTarget( this );
		m_sweeper->setMessageTarget( this );
		m_placement_converter->setMessageTarget( this );
		m_curve_converter->setMessageTarget( this );
		m_profile_cache->setMessageTarget( this );
		m_face_converter->setMessageTarget( this );
		m_solid_converter->setMessageTarget( this );
		m_tessel_converter->setMessageTarget( this );
	}

	virtual ~RepresentationConverter()
	{
		clearCache();
	}

	void clearCache()
	{
		m_profile_cache->clearProfileCache();
		m_styles_converter->clearStylesCache();
	}
	shared_ptr<GeometrySettings>&		getGeomSettings()	{ return m_geom_settings; }
	shared_ptr<UnitConverter>&			getUnitConverter() { return m_unit_converter; }
	shared_ptr<StylesConverter>&		getStylesConverter() { return m_styles_converter; }
	shared_ptr<PointConverter>&			getPointConverter() { return m_point_converter; }
	shared_ptr<SplineConverter>&		getSplineConverter() { return m_spline_converter; }
	shared_ptr<Sweeper>&				getSweeper() { return m_sweeper; }
	shared_ptr<PlacementConverter>&		getPlacementConverter() { return m_placement_converter; }
	shared_ptr<CurveConverter>&			getCurveConverter() { return m_curve_converter; }
	shared_ptr<ProfileCache>&			getProfileCache()	{ return m_profile_cache; }
	shared_ptr<FaceConverter>&			getFaceConverter() { return m_face_converter; }
	shared_ptr<SolidModelConverter>&	getSolidConverter() { return m_solid_converter; }

	void setUnitConverter( shared_ptr<UnitConverter>& unit_converter )
	{
		m_unit_converter = unit_converter;
		m_point_converter->setUnitConverter( unit_converter );
		m_sweeper->m_unit_converter = unit_converter;
		m_placement_converter->m_unit_converter = unit_converter;
		m_face_converter->m_unit_converter = unit_converter;
	}

	void convertRepresentationStyle( const shared_ptr<IfcRepresentationItem>& representation_item, std::vector<shared_ptr<AppearanceData> >& vec_appearance_data )
	{
		std::vector<weak_ptr<IfcStyledItem> >&	vec_StyledByItem_inverse = representation_item->m_StyledByItem_inverse;
		for( size_t i = 0; i < vec_StyledByItem_inverse.size(); ++i )
		{
			weak_ptr<IfcStyledItem> styled_item_weak = vec_StyledByItem_inverse[i];
			shared_ptr<IfcStyledItem> styled_item = shared_ptr<IfcStyledItem>( styled_item_weak );
			m_styles_converter->convertIfcStyledItem( styled_item, vec_appearance_data );
		}
	}

	void convertIfcRepresentation( const shared_ptr<IfcRepresentation>& ifc_representation, shared_ptr<RepresentationData>& representation_data )
	{
		if( ifc_representation->m_RepresentationIdentifier )
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
			representation_data->m_representation_identifier = ifc_representation->m_RepresentationIdentifier->m_value;
		}

		if( ifc_representation->m_RepresentationType )
		{
			// Point	2 or 3 dimensional point(s)
			// PointCloud	3 dimensional points represented by a point list
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
			representation_data->m_representation_type = ifc_representation->m_RepresentationType->m_value;
		}

		representation_data->m_ifc_representation = ifc_representation;
		representation_data->m_ifc_representation_context = ifc_representation->m_ContextOfItems;

		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		for( size_t i_representation_items = 0; i_representation_items < ifc_representation->m_Items.size(); ++i_representation_items )
		{
			shared_ptr<IfcRepresentationItem> representation_item = ifc_representation->m_Items[i_representation_items];

			//ENTITY IfcRepresentationItem  ABSTRACT SUPERTYPE OF(ONEOF(IfcGeometricRepresentationItem, IfcMappedItem, IfcStyledItem, IfcTopologicalRepresentationItem));
			shared_ptr<IfcGeometricRepresentationItem> geom_item = dynamic_pointer_cast<IfcGeometricRepresentationItem>( representation_item );
			if( geom_item )
			{
				shared_ptr<ItemShapeData> geom_item_data( new ItemShapeData() );
				if( !geom_item_data )
				{
					throw OutOfMemoryException( __FUNC__ );
				}
				representation_data->m_vec_item_data.push_back( geom_item_data );
				geom_item_data->m_parent_representation = representation_data;

				try
				{
					convertIfcGeometricRepresentationItem( geom_item, geom_item_data );
				}
				catch( OutOfMemoryException& e )
				{
					throw e;
				}
				catch( BuildingException& e )
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

				shared_ptr<TransformData> map_matrix_target;
				if( mapped_item->m_MappingTarget )
				{
					shared_ptr<IfcCartesianTransformationOperator> transform_operator = mapped_item->m_MappingTarget;
					m_placement_converter->convertTransformationOperator( transform_operator, map_matrix_target );
				}

				shared_ptr<TransformData> map_matrix_origin;
				shared_ptr<IfcAxis2Placement> mapping_origin_select = map_source->m_MappingOrigin;
				if( mapping_origin_select )
				{
					shared_ptr<IfcPlacement> mapping_origin_placement = dynamic_pointer_cast<IfcPlacement>( mapping_origin_select );
					if( mapping_origin_placement )
					{
						m_placement_converter->convertIfcPlacement( mapping_origin_placement, map_matrix_origin );
					}
					else
					{
						messageCallback( "!dynamic_pointer_cast<IfcPlacement>( mapping_origin )", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, mapping_origin_placement.get() );
						continue;
					}
				}

				shared_ptr<RepresentationData> mapped_input_data( new RepresentationData() );
				if( !mapped_input_data )
				{
					throw OutOfMemoryException( __FUNC__ );
				}

				try
				{
					convertIfcRepresentation( mapped_representation, mapped_input_data );
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
					messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
				}

				if( m_geom_settings->handleStyledItems() )
				{
					std::vector<shared_ptr<AppearanceData> > vec_appearance_data;
					convertRepresentationStyle( representation_item, vec_appearance_data );

					if( vec_appearance_data.size() > 0 )
					{
						// overwrite item appearances with parent appearance
						for( size_t i_mapped_item = 0; i_mapped_item < mapped_input_data->m_vec_item_data.size(); ++i_mapped_item )
						{
							shared_ptr<ItemShapeData>& mapped_item_data = mapped_input_data->m_vec_item_data[i_mapped_item];
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

				if( map_matrix_origin && map_matrix_target )
				{
					carve::math::Matrix mapped_pos(map_matrix_target->m_matrix*map_matrix_origin->m_matrix);
					mapped_input_data->applyTransformToRepresentation(mapped_pos);
				}
				representation_data->appendRepresentationData( mapped_input_data, representation_data );
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
				shared_ptr<ItemShapeData> topological_item_data( new ItemShapeData() );
				if( !topological_item_data )
				{
					throw OutOfMemoryException( __FUNC__ );
				}
				representation_data->m_vec_item_data.push_back( topological_item_data );
				topological_item_data->m_parent_representation = representation_data;

				try
				{
					convertTopologicalRepresentationItem( topo_item, topological_item_data );
				}
				catch( OutOfMemoryException& e )
				{
					throw e;
				}
				catch( BuildingException& e )
				{
					messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", representation_item.get() );
				}
				catch( std::exception& e )
				{
					messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, representation_item.get() );
				}

				continue;
			}

			messageCallback( "unhandled representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, representation_item.get() );
		}

		if( m_geom_settings->handleLayerAssignments() )
		{
			std::vector<weak_ptr<IfcPresentationLayerAssignment> >& vec_layer_assignments_inverse = ifc_representation->m_LayerAssignments_inverse;
			for( size_t ii = 0; ii < vec_layer_assignments_inverse.size(); ++ii )
			{
				shared_ptr<IfcPresentationLayerAssignment> layer_assignment( vec_layer_assignments_inverse[ii] );
				if( layer_assignment )
				{
					shared_ptr<IfcPresentationLayerWithStyle> layer_assignment_with_style = dynamic_pointer_cast<IfcPresentationLayerWithStyle>( layer_assignment );
					if( layer_assignment_with_style )
					{
						std::vector<shared_ptr<IfcPresentationStyle> >& vec_presentation_styles = layer_assignment_with_style->m_LayerStyles;
						for( size_t ii_style = 0; ii_style < vec_presentation_styles.size(); ++ii_style )
						{
							shared_ptr<IfcPresentationStyle>&  presentation_style = vec_presentation_styles[ii_style];
							if( presentation_style )
							{
								shared_ptr<AppearanceData> appearance_data;
								m_styles_converter->convertIfcPresentationStyle( presentation_style, appearance_data );
								if( appearance_data )
								{
									representation_data->addAppearance( appearance_data );
								}
							}
						}
					}
				}
			}
		}
	}

	void convertIfcGeometricRepresentationItem( const shared_ptr<IfcGeometricRepresentationItem>& geom_item, shared_ptr<ItemShapeData> item_data )
	{
		//ENTITY IfcGeometricRepresentationItem
		//ABSTRACT SUPERTYPE OF(ONEOF(IfcAnnotationFillArea, IfcBooleanResult, IfcBoundingBox, IfcCartesianPointList, IfcCartesianTransformationOperator, 
		//IfcCompositeCurveSegment, IfcCsgPrimitive3D, IfcCurve, IfcDirection, IfcFaceBasedSurfaceModel, IfcFillAreaStyleHatching, IfcFillAreaStyleTiles, 
		//IfcGeometricSet, IfcHalfSpaceSolid, IfcLightSource, IfcPlacement, IfcPlanarExtent, IfcPoint, IfcSectionedSpine, IfcShellBasedSurfaceModel, 
		//IfcSolidModel, IfcSurface, IfcTessellatedItem, IfcTextLiteral, IfcVector))
		if( m_geom_settings->handleStyledItems() )
		{
			std::vector<shared_ptr<AppearanceData> > vec_appearance_data;
			convertRepresentationStyle( geom_item, vec_appearance_data );
			std::copy( vec_appearance_data.begin(), vec_appearance_data.end(), std::back_inserter( item_data->m_vec_item_appearances ) );
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
			std::vector<vec3> loops;
			std::vector<vec3> segment_start_points;
			m_curve_converter->convertIfcCurve( ifc_curve, loops, segment_start_points );

			shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
			if( !polyline_data )
			{
				throw OutOfMemoryException( __FUNC__ );
			}
			polyline_data->beginPolyline();
			for( size_t i = 0; i < loops.size(); ++i )
			{
				vec3 point = loops[i];
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
			shared_ptr<SurfaceProxy> surface_proxy;
			m_face_converter->convertIfcSurface( ifc_surface, item_data, surface_proxy );
			return;
		}

		shared_ptr<IfcPolyline> poly_line = dynamic_pointer_cast<IfcPolyline>( geom_item );
		if( poly_line )
		{
			std::vector<vec3> poly_vertices;
			m_curve_converter->convertIfcPolyline( poly_line, poly_vertices );

			const size_t num_points = poly_vertices.size();
			shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
			if( !polyline_data )
			{
				throw OutOfMemoryException( __FUNC__ );
			}
			polyline_data->beginPolyline();

			// apply position
			for( size_t i = 0; i < num_points; ++i )
			{
				vec3& vertex = poly_vertices[i];
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
			for( auto geom_select : geometric_set->m_Elements )
			{
				// TYPE IfcGeometricSetSelect = SELECT (IfcPoint, IfcCurve, IfcSurface);
				if( !geom_select )
				{
					continue;
				}

				shared_ptr<IfcPoint> point = dynamic_pointer_cast<IfcPoint>( geom_select );
				if( point )
				{
					// TODO: implement
					continue;
				}

				shared_ptr<IfcCurve> select_curve = dynamic_pointer_cast<IfcCurve>( geom_select );
				if( select_curve )
				{
					std::vector<vec3> loops;
					std::vector<vec3> segment_start_points;
					m_curve_converter->convertIfcCurve( select_curve, loops, segment_start_points );

					shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
					if( !polyline_data )
					{
						throw OutOfMemoryException( __FUNC__ );
					}
					polyline_data->beginPolyline();
					for( size_t i = 0; i < loops.size(); ++i )
					{
						vec3 loop_point = loops[i];
						polyline_data->addVertex(loop_point);
						polyline_data->addPolylineIndex( i );
					}
					item_data->m_polylines.push_back( polyline_data );

					continue;
				}

				shared_ptr<IfcSurface> select_surface = dynamic_pointer_cast<IfcSurface>( geom_select );
				if( select_surface )
				{
					shared_ptr<SurfaceProxy> surface_proxy;
					m_face_converter->convertIfcSurface( select_surface, item_data, surface_proxy );
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
				shared_ptr<TransformData> text_position_matrix;
				shared_ptr<IfcAxis2Placement>& text_placement_select = text_literal->m_Placement;
				if( text_placement_select )
				{
					shared_ptr<IfcAxis2Placement3D> placement_3d = dynamic_pointer_cast<IfcAxis2Placement3D>( text_placement_select );
					if( placement_3d )
					{
						m_placement_converter->convertIfcAxis2Placement3D( placement_3d, text_position_matrix );
					}
					else
					{
						shared_ptr<IfcAxis2Placement2D> placement_2d = dynamic_pointer_cast<IfcAxis2Placement2D>( text_placement_select );
						if( placement_2d )
						{
							m_placement_converter->convertIfcAxis2Placement2D( placement_2d, text_position_matrix );
						}
					}
				}

				//shared_ptr<IfcTextPath>& path = text_literal->m_Path;
				shared_ptr<TextItemData> text_item_data( new TextItemData() );
				if( !text_item_data )
				{
					throw OutOfMemoryException( __FUNC__ );
				}
				if( text_position_matrix )
				{
					text_item_data->m_text_position = text_position_matrix->m_matrix;
				}
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
			std::vector<std::vector<vec3> > face_loops;
			face_loops.push_back( std::vector<vec3>() );
			std::vector<vec3>& outer_boundary_loop = face_loops.back();
			std::vector<vec3> segment_start_points;
			m_curve_converter->convertIfcCurve( outer_boundary, outer_boundary_loop, segment_start_points );

			// convert inner boundaries
			std::vector<shared_ptr<IfcCurve> >& vec_inner_boundaries = annotation_fill_area->m_InnerBoundaries;			//optional
			for( auto& inner_boundary : vec_inner_boundaries )
			{
				if( !inner_boundary )
				{
					continue;
				}
				face_loops.push_back( std::vector<vec3>() );
				std::vector<vec3>& inner_boundary_loop = face_loops.back();
				std::vector<vec3> segment_start_points_inner_curve;
				m_curve_converter->convertIfcCurve( inner_boundary, inner_boundary_loop, segment_start_points_inner_curve);
			}

			PolyInputCache3D poly_cache;
			m_sweeper->createTriangulated3DFace( face_loops, outer_boundary.get(), poly_cache );
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
				vec3 point;
				const double length_factor = m_unit_converter->getLengthInMeterFactor();
				if( PointConverter::convertIfcCartesianPoint( ifc_cartesian_point, point, length_factor ) )
				{
					item_data->addPoint( point );
				}
				return;
			}
		}

		shared_ptr<IfcTessellatedItem> ifc_tessel_item = dynamic_pointer_cast<IfcTessellatedItem>( geom_item );
		if(ifc_tessel_item)
		{
			m_tessel_converter->convertTessellatedItem(ifc_tessel_item, item_data);
			return;
		}

		messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, geom_item.get() );
	}

	void convertTopologicalRepresentationItem( const shared_ptr<IfcTopologicalRepresentationItem>& topological_item, shared_ptr<ItemShapeData> topo_item_data )
	{
		//IfcTopologicalRepresentationItem 		ABSTRACT SUPERTYPE OF(ONEOF(IfcConnectedFaceSet, IfcEdge, IfcFace, IfcFaceBound, IfcLoop, IfcPath, IfcVertex))
		const shared_ptr<IfcConnectedFaceSet> topo_connected_face_set = dynamic_pointer_cast<IfcConnectedFaceSet>( topological_item );
		if( topo_connected_face_set )
		{
			std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = topo_connected_face_set->m_CfsFaces;
			m_face_converter->convertIfcFaceList( vec_ifc_faces, topo_item_data, FaceConverter::SHELL_TYPE_UNKONWN );
			return;
		}

		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		const shared_ptr<IfcEdge> topo_edge = dynamic_pointer_cast<IfcEdge>( topological_item );
		if( topo_edge )
		{
			const shared_ptr<IfcVertex>& vertex_start = topo_edge->m_EdgeStart;
			vec3 point_start;
			if( PointConverter::convertIfcVertex( vertex_start, point_start, length_factor ) )
			{
				const shared_ptr<IfcVertex>& vertex_end = topo_edge->m_EdgeEnd;
				vec3 point_end;
				if( PointConverter::convertIfcVertex( vertex_end, point_end, length_factor ) )
				{
					shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
					if( !polyline_data )
					{
						throw OutOfMemoryException( __FUNC__ );
					}
					topo_item_data->m_polylines.push_back( polyline_data );
					polyline_data->beginPolyline();
					polyline_data->addVertex( point_start );
					polyline_data->addVertex( point_end );
					polyline_data->addPolylineIndex( 0 );
					polyline_data->addPolylineIndex( 1 );
				}
			}
			return;
		}

		const shared_ptr<IfcFace> topo_face = dynamic_pointer_cast<IfcFace>( topological_item );
		if( topo_face )
		{
			// ENTITY IfcFace SUPERTYPE OF( IfcFaceSurface )

			PolyInputCache3D poly_cache_top_face;
			//shared_ptr<IfcFaceSurface> topo_face_surface = dynamic_pointer_cast<IfcFaceSurface>( topo_face );
			//if( topo_face_surface )
			//{
			//	//  std::vector<shared_ptr<IfcFaceBound> >					m_Bounds;
			//	//  shared_ptr<IfcSurface>									m_FaceSurface;
			//	//  bool													m_SameSense;

			//	const shared_ptr<IfcSurface>& face_surface = topo_face_surface->m_FaceSurface;
			//	if( face_surface )
			//	{
			//		shared_ptr<SurfaceProxy> surface_proxy;
			//		m_face_converter->convertIfcSurface( face_surface, topo_item_data, surface_proxy );
			//	}
			//	if( poly_cache_top_face.m_poly_data )
			//	{
			//		topo_item_data->addOpenOrClosedPolyhedron( poly_cache_top_face.m_poly_data );
			//	}
			//	return;
			//}
	
			const std::vector<shared_ptr<IfcFaceBound> >& vec_face_bounds = topo_face->m_Bounds;
			if( vec_face_bounds.size() > 0 )
			{
				for( auto it_bounds = vec_face_bounds.begin(); it_bounds != vec_face_bounds.end(); ++it_bounds )
				{
					const shared_ptr<IfcFaceBound>& face_bound = ( *it_bounds );
					
					// ENTITY IfcLoop SUPERTYPE OF(ONEOF(IfcEdgeLoop, IfcPolyLoop, IfcVertexLoop))
					const shared_ptr<IfcLoop>& loop = face_bound->m_Bound;
					if( !loop )
					{
						if( it_bounds == vec_face_bounds.begin() )
						{
							break;
						}
						else
						{
							continue;
						}
					}

					std::vector<std::vector<vec3> > face_loops;
					face_loops.push_back( std::vector<vec3>() );
					std::vector<vec3>& loop_points = face_loops.back();
					m_curve_converter->convertIfcLoop( loop, loop_points );

					if( loop_points.size() < 3 )
					{
						if( it_bounds == vec_face_bounds.begin() )
						{
							break;
						}
						else
						{
							continue;
						}
					}

					bool orientation = face_bound->m_Orientation->m_value;
					if( !orientation )
					{
						std::reverse( loop_points.begin(), loop_points.end() );
					}

					m_sweeper->createTriangulated3DFace( face_loops, topo_face.get(), poly_cache_top_face );
				}
			}
			if( poly_cache_top_face.m_poly_data )
			{
				topo_item_data->addOpenOrClosedPolyhedron( poly_cache_top_face.m_poly_data );
			}
			return;
		}

		shared_ptr<IfcFaceBound> topo_face_bound = dynamic_pointer_cast<IfcFaceBound>( topological_item );
		if( topo_face_bound )
		{
			// ENTITY IfcLoop SUPERTYPE OF(ONEOF(IfcEdgeLoop, IfcPolyLoop, IfcVertexLoop))
			const shared_ptr<IfcLoop>& loop = topo_face_bound->m_Bound;
			if( !loop )
			{
				std::cout << __FUNC__ << ": Bound invalid " << std::endl;
				return;
			}
			std::vector<std::vector<vec3> > face_loops;
			face_loops.push_back( std::vector<vec3>() );
			std::vector<vec3>& loop_points = face_loops.back();
			m_curve_converter->convertIfcLoop( loop, loop_points );

			if( loop_points.size() > 2 )
			{
				bool orientation = topo_face_bound->m_Orientation->m_value;
				if( !orientation )
				{
					std::reverse( loop_points.begin(), loop_points.end() );
				}

				PolyInputCache3D poly_cache_top_face;
				m_sweeper->createTriangulated3DFace( face_loops, topo_face.get(), poly_cache_top_face );

				if( poly_cache_top_face.m_poly_data )
				{
					topo_item_data->addOpenOrClosedPolyhedron( poly_cache_top_face.m_poly_data );
				}
			}
			return;
		}

		shared_ptr<IfcLoop> topo_loop = dynamic_pointer_cast<IfcLoop>( topological_item );
		if( topo_loop )
		{
			std::vector<vec3> loop_points;
			m_curve_converter->convertIfcLoop( topo_loop, loop_points );

			if( loop_points.size() > 0 )
			{
				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				if( !polyline_data )
				{
					throw OutOfMemoryException( __FUNC__ );
				}
				topo_item_data->m_polylines.push_back( polyline_data );
				polyline_data->beginPolyline();

				for( size_t ii_loop = 0; ii_loop < loop_points.size(); ++ii_loop )
				{
					polyline_data->addVertex( loop_points[ii_loop] );
					polyline_data->addPolylineIndex( 1 );
				}
			}
			return;
		}

		shared_ptr<IfcPath> topo_path = dynamic_pointer_cast<IfcPath>( topological_item );
		if( topo_path )
		{
			return;
		}

		shared_ptr<IfcVertex> topo_vertex = dynamic_pointer_cast<IfcVertex>( topological_item );
		if( topo_vertex )
		{
			vec3 topo_vertex_point;
			if( PointConverter::convertIfcVertex( topo_vertex, topo_vertex_point, length_factor ) )
			{
				topo_item_data->addPoint( topo_vertex_point );
				return;
			}
		}

		messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, topological_item.get() );
	}

	void subtractOpenings( const shared_ptr<IfcElement>& ifc_element, shared_ptr<ProductShapeData>& product_shape )
	{
		std::vector<shared_ptr<ProductShapeData> > vec_opening_data;
		std::vector<weak_ptr<IfcRelVoidsElement> > vec_rel_voids( ifc_element->m_HasOpenings_inverse );
		if( vec_rel_voids.size() == 0 )
		{
			return;
		}
		const double length_factor = m_unit_converter->getLengthInMeterFactor();

		// convert opening representation
		for( auto& rel_voids_weak : vec_rel_voids )
		{
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

			// opening can have its own relative placement
			shared_ptr<IfcObjectPlacement>	opening_placement = opening->m_ObjectPlacement;
			shared_ptr<ProductShapeData> product_shape_opening( new ProductShapeData( opening_placement->m_entity_id ) );
			if( opening_placement )
			{
				std::unordered_set<IfcObjectPlacement*> opening_placements_applied;
				m_placement_converter->convertIfcObjectPlacement( opening_placement, product_shape_opening, opening_placements_applied, false );
			}

			std::vector<shared_ptr<IfcRepresentation> >& vec_opening_representations = opening->m_Representation->m_Representations;
			for( size_t i_representations = 0; i_representations < vec_opening_representations.size(); ++i_representations )
			{
				shared_ptr<IfcRepresentation> ifc_opening_representation = vec_opening_representations[i_representations];

				shared_ptr<RepresentationData> opening_representation_data( new RepresentationData() );
				if( !opening_representation_data )
				{
					throw OutOfMemoryException( __FUNC__ );
				}

				// TODO: Representation caching, one element could be used for several openings
				try
				{
					convertIfcRepresentation( ifc_opening_representation, opening_representation_data );
				}
				catch( OutOfMemoryException& e )
				{
					throw e;
				}
				catch( BuildingException& e )
				{
					messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", ifc_element.get() );
				}
				catch( std::exception& e )
				{
					messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", ifc_element.get() );
				}

				product_shape_opening->m_vec_representations.push_back( opening_representation_data );
			}
			vec_opening_data.push_back( product_shape_opening );
		}

		// for all items of the product shape, subtract all items of all related openings
		for( auto& product_representation : product_shape->m_vec_representations )
		{
			if( !product_representation )
			{
				continue;
			}

			std::vector<shared_ptr<ItemShapeData> >& vec_product_items = product_representation->m_vec_item_data;
			for( auto& product_item_data : vec_product_items )
			{
				if( !product_item_data )
				{
					continue;
				}

				// now go through all meshsets of the item
				for( size_t i_product_meshset = 0; i_product_meshset < product_item_data->m_meshsets.size(); ++i_product_meshset )
				{
					shared_ptr<carve::mesh::MeshSet<3> >& product_meshset = product_item_data->m_meshsets[i_product_meshset];
					std::stringstream strs_meshset_err;
					bool product_meshset_valid_for_csg = CSG_Adapter::checkMeshSetValidAndClosed( product_meshset, this, ifc_element.get() );
					if( !product_meshset_valid_for_csg )
					{
						continue;
					}

					// now for current product (wall) meshset, subtract all opening representations and items
					for( auto& opening_product_data : vec_opening_data )
					{
						if( !opening_product_data )
						{
							continue;
						}

						carve::math::Matrix product_transform = product_shape->getRelaviteTransform(opening_product_data);
						carve::math::Matrix product_matrix_inverse;
						try
						{
							GeomUtils::computeInverse(product_transform, product_matrix_inverse, 0.01 / m_unit_converter->getCustomLengthFactor());
						}
						catch (std::exception& e)
						{
							messageCallback(e.what(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, ifc_element.get());
						}

						carve::math::Matrix opening_relative_to_product_matrix = opening_product_data->getRelaviteTransform(product_shape);
						carve::math::Matrix opening_relative_to_product = product_matrix_inverse * opening_relative_to_product_matrix;

						carve::math::Matrix opening_relative_to_product_inverse;
						GeomUtils::computeInverse( opening_relative_to_product, opening_relative_to_product_inverse, 0.01/m_unit_converter->getCustomLengthFactor() );

						for( auto& opening_representation_data : opening_product_data->m_vec_representations )
						{
							std::vector<shared_ptr<ItemShapeData> >& vec_opening_items = opening_representation_data->m_vec_item_data;
							for( auto& opening_item_data : vec_opening_items )
							{
								if( !opening_item_data )
								{
									continue;
								}
								if( opening_item_data->m_meshsets.size() == 0 )
								{
									continue;
								}

								// transform opening meshset relative to product
								opening_item_data->applyTransformToItem( opening_relative_to_product );

								std::vector<shared_ptr<carve::mesh::MeshSet<3> > >&	opening_meshsets = opening_item_data->m_meshsets;
								for( size_t i_opening_meshset = 0; i_opening_meshset < opening_meshsets.size(); ++i_opening_meshset )
								{
									shared_ptr<carve::mesh::MeshSet<3> > opening_meshset = opening_meshsets[i_opening_meshset];

									if( !opening_meshset )
									{
										continue;
									}
									if( opening_meshset->meshes.size() < 1 )
									{
										continue;
									}

									// do the subtraction
									shared_ptr<carve::mesh::MeshSet<3> > result;
									try
									{
										CSG_Adapter::computeCSG( product_meshset, opening_meshset, carve::csg::CSG::A_MINUS_B, result, this, ifc_element );
									}
									catch( OutOfMemoryException& e )
									{
										throw e;
									}
									catch( BuildingException& e )
									{
										messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "", ifc_element.get() );
									}
									catch( std::exception& e )
									{
										messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, ifc_element.get() );
									}
									product_meshset = result;
								}

								// transform meshset back into local coordinates
								opening_item_data->applyTransformToItem( opening_relative_to_product_inverse );
							}
						}
					}
				}
			}
		}
	}
};

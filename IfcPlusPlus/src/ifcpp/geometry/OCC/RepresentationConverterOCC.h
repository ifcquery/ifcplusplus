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
#include <BRepBuilderAPI_MakeFace.hxx>

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/StylesConverter.h>
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
#include <ifcpp/IFC4/include/IfcTextLiteral.h>

#include "GeometryInputDataOCC.h"
#include "SplineConverterOCC.h"
#include "PointConverterOCC.h"
#include "CurveConverterOCC.h"
#include "SolidModelConverterOCC.h"
#include "FaceConverterOCC.h"
#include "ProfileCacheOCC.h"

class RepresentationConverterOCC : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;
	shared_ptr<StylesConverter>			m_styles_converter;
	shared_ptr<SplineConverterOCC>		m_spline_converter;
	shared_ptr<CurveConverterOCC>		m_curve_converter;
	shared_ptr<SolidModelConverterOCC>	m_solid_converter;
	shared_ptr<FaceConverterOCC>		m_face_converter;
	shared_ptr<ProfileCacheOCC>			m_profile_cache;
	
public:
	RepresentationConverterOCC( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> unit_converter )
		: m_geom_settings( geom_settings ), m_unit_converter( unit_converter )
	{
		m_styles_converter = shared_ptr<StylesConverter>( new StylesConverter() );
		m_spline_converter = shared_ptr<SplineConverterOCC>( new SplineConverterOCC( m_geom_settings ) );
		m_curve_converter = shared_ptr<CurveConverterOCC>( new CurveConverterOCC( m_geom_settings, m_unit_converter, m_spline_converter ) );
		m_profile_cache = shared_ptr<ProfileCacheOCC>( new ProfileCacheOCC( m_curve_converter, m_spline_converter ) );
		m_face_converter = shared_ptr<FaceConverterOCC>( new FaceConverterOCC( m_geom_settings, m_unit_converter, m_curve_converter, m_spline_converter ) );
		m_solid_converter = shared_ptr<SolidModelConverterOCC>( new SolidModelConverterOCC( m_geom_settings, m_unit_converter, m_curve_converter, m_face_converter, m_profile_cache ) );

		// this redirects the callback messages from all converters to RepresentationConverter's callback
		m_styles_converter->setMessageTarget( this );
		m_spline_converter->setMessageTarget( this );
		m_curve_converter->setMessageTarget( this );
		m_profile_cache->setMessageTarget( this );
		m_face_converter->setMessageTarget( this );
		m_solid_converter->setMessageTarget( this );
	}

	virtual ~RepresentationConverterOCC()
	{
		clearCache();
	}

	void clearCache()
	{
		m_profile_cache->clearProfileCache();
		m_styles_converter->clearStylesCache();
	}
	shared_ptr<UnitConverter>&			getUnitConverter()	{ return m_unit_converter; }
	shared_ptr<GeometrySettings>&		getGeomSettings()	{ return m_geom_settings; }
	shared_ptr<SolidModelConverterOCC>&	getSolidConverter() { return m_solid_converter; }
	shared_ptr<FaceConverterOCC>&		getFaceConverter() { return m_face_converter; }
	shared_ptr<ProfileCacheOCC>&		getProfileCache()	{ return m_profile_cache; }
	shared_ptr<StylesConverter>&		getStylesConverter()	{ return m_styles_converter; }
	shared_ptr<CurveConverterOCC>&		getCurveConverter() { return m_curve_converter; }
	shared_ptr<SplineConverterOCC>&		getSplineConverter(){ return m_spline_converter; }

	void setUnitConverter( shared_ptr<UnitConverter>& unit_converter )
	{
		m_unit_converter = unit_converter;
		m_curve_converter->m_unit_converter = unit_converter;
		m_face_converter->m_unit_converter = unit_converter;
		m_solid_converter->m_unit_converter = unit_converter;
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

	void convertIfcRepresentation( const shared_ptr<IfcRepresentation>& ifc_representation, shared_ptr<RepresentationDataOCC>& input_data )
	{
		if( ifc_representation->m_RepresentationIdentifier )
		{
			input_data->m_representation_identifier = ifc_representation->m_RepresentationIdentifier->m_value;
		}

		if( ifc_representation->m_RepresentationType )
		{
			input_data->m_representation_type = ifc_representation->m_RepresentationType->m_value;
		}

		input_data->m_ifc_representation = ifc_representation;
		input_data->m_ifc_representation_context = ifc_representation->m_ContextOfItems;

		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		for( size_t i_representation_items = 0; i_representation_items < ifc_representation->m_Items.size(); ++i_representation_items )
		{
			shared_ptr<IfcRepresentationItem> representation_item = ifc_representation->m_Items[i_representation_items];

			//ENTITY IfcRepresentationItem  ABSTRACT SUPERTYPE OF(ONEOF(IfcGeometricRepresentationItem, IfcMappedItem, IfcStyledItem, IfcTopologicalRepresentationItem));
			shared_ptr<IfcGeometricRepresentationItem> geom_item = dynamic_pointer_cast<IfcGeometricRepresentationItem>( representation_item );
			if( geom_item )
			{
				shared_ptr<ItemShapeDataOCC> geom_item_data( new ItemShapeDataOCC() );
				if( !geom_item_data )
				{
					throw OutOfMemoryException( __FUNC__ );
				}
				input_data->m_vec_item_data.push_back( geom_item_data );

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

				gp_Trsf map_matrix_target;
				gp_GTrsf map_matrix_target_general;
				bool target_non_uniform = false;
				if( mapped_item->m_MappingTarget )
				{
					shared_ptr<IfcCartesianTransformationOperator> transform_operator = mapped_item->m_MappingTarget;
					PlacementConverterOCC::convertTransformationOperator( transform_operator, length_factor, map_matrix_target, map_matrix_target_general, target_non_uniform, this );
				}

				gp_Trsf map_matrix_origin;
				shared_ptr<IfcAxis2Placement> mapping_origin_select = map_source->m_MappingOrigin;
				if( mapping_origin_select )
				{
					shared_ptr<IfcPlacement> mapping_origin_placement = dynamic_pointer_cast<IfcPlacement>( mapping_origin_select );
					if( mapping_origin_placement )
					{
						PlacementConverterOCC::convertIfcPlacement( mapping_origin_placement, length_factor, map_matrix_origin, this );
					}
					else
					{
						messageCallback( "!dynamic_pointer_cast<IfcPlacement>( mapping_origin )", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, mapping_origin_placement.get() );
						continue;
					}
				}

				shared_ptr<RepresentationDataOCC> mapped_input_data( new RepresentationDataOCC() );
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
							shared_ptr<ItemShapeDataOCC>& mapped_item_data = mapped_input_data->m_vec_item_data[i_mapped_item];
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

				if( target_non_uniform )
				{
					gp_GTrsf mapped_pos( map_matrix_target_general*map_matrix_origin );
					mapped_input_data->applyTransformToRepresentation( mapped_pos );
				}
				else
				{
					gp_Trsf mapped_pos( map_matrix_target*map_matrix_origin );
					mapped_input_data->applyTransformToRepresentation( mapped_pos );
				}
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
				shared_ptr<ItemShapeDataOCC> topological_item_data( new ItemShapeDataOCC() );
				if( !topological_item_data )
				{
					throw OutOfMemoryException( __FUNC__ );
				}
				input_data->m_vec_item_data.push_back( topological_item_data );

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
									input_data->addAppearance( appearance_data );
								}
							}
						}
					}
				}
			}
		}
	}

	void convertIfcGeometricRepresentationItem( const shared_ptr<IfcGeometricRepresentationItem>& geom_item, shared_ptr<ItemShapeDataOCC> item_data )
	{
		//ENTITY IfcGeometricRepresentationItem
		//ABSTRACT SUPERTYPE OF(ONEOF(
		//	IfcAnnotationFillArea, IfcBooleanResult, IfcBoundingBox, IfcCartesianTransformationOperator, IfcCompositeCurveSegment, IfcCsgPrimitive3D,
		//	IfcCurve, IfcDefinedSymbol, IfcDirection, IfcFaceBasedSurfaceModel, IfcFillAreaStyleHatching, IfcFillAreaStyleTiles, IfcFillAreaStyleTileSymbolWithStyle,
		//	IfcGeometricSet, IfcHalfSpaceSolid, IfcLightSource, IfcOneDirectionRepeatFactor, IfcPlacement, IfcPlanarExtent, IfcPoint, IfcSectionedSpine,
		//	IfcShellBasedSurfaceModel, IfcSolidModel, IfcSurface, IfcTextLiteral, IfcTextureCoordinate, IfcTextureVertex, IfcVector))
		if( m_geom_settings->handleStyledItems() )
		{
			std::vector<shared_ptr<AppearanceData> > vec_appearance_data;
			convertRepresentationStyle( geom_item, vec_appearance_data );
			std::copy( vec_appearance_data.begin(), vec_appearance_data.end(), std::back_inserter( item_data->m_vec_item_appearances ) );
		}
		
		shared_ptr<IfcBoundingBox> bbox = dynamic_pointer_cast<IfcBoundingBox>( geom_item );
		if( bbox )
		{
			// nothing to do here
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
				m_face_converter->convertIfcFaceList( vec_ifc_faces, item_data, FaceConverterOCC::SHELL_TYPE_UNKONWN );
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
			TopoDS_Wire wire;
			m_curve_converter->convertIfcCurve( ifc_curve, wire );

			if( !wire.IsNull() )
			{
				item_data->m_polylines.push_back( wire );
			}
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
					m_face_converter->convertIfcFaceList( vec_ifc_faces, item_data, FaceConverterOCC::CLOSED_SHELL );
					continue;
				}

				shared_ptr<IfcOpenShell> open_shell = dynamic_pointer_cast<IfcOpenShell>( shell_select );
				if( open_shell )
				{
					std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = open_shell->m_CfsFaces;
					m_face_converter->convertIfcFaceList( vec_ifc_faces, item_data, FaceConverterOCC::OPEN_SHELL );
					continue;
				}
			}
			return;
		}

		shared_ptr<IfcSurface> ifc_surface = dynamic_pointer_cast<IfcSurface>( geom_item );
		if( ifc_surface )
		{
			Handle_Geom_Surface geom_surface;
			TopoDS_Face face;
			m_face_converter->convertIfcSurface( ifc_surface, geom_surface, face );
			if( !face.IsNull() )
			{
				item_data->addShape( face );
			}
			return;
		}

		shared_ptr<IfcPolyline> poly_line = dynamic_pointer_cast<IfcPolyline>( geom_item );
		if( poly_line )
		{
			std::vector<gp_Vec> poly_vertices;
			m_curve_converter->convertIfcPolyline( poly_line, poly_vertices );

			TopoDS_Wire wire;
			GeomUtilsOCC::createWireFromPoints( poly_vertices, wire, false );
			if( !wire.IsNull() )
			{
				item_data->m_polylines.push_back( wire );
			}
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
					TopoDS_Wire wire;
					m_curve_converter->convertIfcCurve( select_curve, wire );

					if( !wire.IsNull() )
					{
						item_data->m_polylines.push_back( wire );
					}
					continue;
				}

				shared_ptr<IfcSurface> select_surface = dynamic_pointer_cast<IfcSurface>( geom_select );
				if( select_surface )
				{
					Handle_Geom_Surface geom_surface;
					TopoDS_Face face;
					m_face_converter->convertIfcSurface( select_surface, geom_surface, face );
					if( !face.IsNull() )
					{
						item_data->addShape( face );
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
				const shared_ptr<IfcPresentableText>& ifc_literal = text_literal->m_Literal;
				std::wstring& literal_text = ifc_literal->m_value;

				// check if text has a local placemnt
				gp_Trsf text_position_matrix;
				const shared_ptr<IfcAxis2Placement>& text_placement_select = text_literal->m_Placement;
				if( text_placement_select )
				{
					double length_factor = m_unit_converter->getLengthInMeterFactor();
					shared_ptr<IfcAxis2Placement3D> placement_3d = dynamic_pointer_cast<IfcAxis2Placement3D>( text_placement_select );
					if( placement_3d )
					{
						PlacementConverterOCC::convertIfcAxis2Placement3D( placement_3d, length_factor, text_position_matrix );
					}
					else
					{
						shared_ptr<IfcAxis2Placement2D> placement_2d = dynamic_pointer_cast<IfcAxis2Placement2D>( text_placement_select );
						if( placement_2d )
						{
							PlacementConverterOCC::convertIfcAxis2Placement2D( placement_2d, length_factor, text_position_matrix );
						}
					}
				}

				//shared_ptr<IfcTextPath>& path = text_literal->m_Path;
				shared_ptr<TextItemDataOCC> text_item_data( new TextItemDataOCC() );
				if( !text_item_data )
				{
					throw OutOfMemoryException( __FUNC__ );
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
			const shared_ptr<IfcCurve>& outer_boundary = annotation_fill_area->m_OuterBoundary;
			const std::vector<shared_ptr<IfcCurve> >& vec_inner_boundaries = annotation_fill_area->m_InnerBoundaries;			//optional
			TopoDS_Face face;
			m_face_converter->createFace( outer_boundary, vec_inner_boundaries, face );
			item_data->addShape( face );
			return;
		}

		shared_ptr<IfcPoint> ifc_point = dynamic_pointer_cast<IfcPoint>( geom_item );
		if( ifc_point )
		{
			// ENTITY IfcPoint ABSTRACT SUPERTYPE OF( ONEOF ( IfcCartesianPoint, IfcPointOnCurve, IfcPointOnSurface ) )
			shared_ptr<IfcCartesianPoint> ifc_cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>( geom_item );
			if( ifc_cartesian_point )
			{
				gp_Vec point;
				const double length_factor = m_unit_converter->getLengthInMeterFactor();
				if( PointConverterOCC::convertIfcCartesianPoint( ifc_cartesian_point, point, length_factor ) )
				{
					item_data->addPoint( point );
				}
				return;
			}
		}

		messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, geom_item.get() );
	}

	void convertTopologicalRepresentationItem( const shared_ptr<IfcTopologicalRepresentationItem>& topological_item, shared_ptr<ItemShapeDataOCC> topo_item_data )
	{
		//IfcTopologicalRepresentationItem 		ABSTRACT SUPERTYPE OF(ONEOF(IfcConnectedFaceSet, IfcEdge, IfcFace, IfcFaceBound, IfcLoop, IfcPath, IfcVertex))
		const shared_ptr<IfcConnectedFaceSet> topo_connected_face_set = dynamic_pointer_cast<IfcConnectedFaceSet>( topological_item );
		if( topo_connected_face_set )
		{
			std::vector<shared_ptr<IfcFace> >& vec_ifc_faces = topo_connected_face_set->m_CfsFaces;
			m_face_converter->convertIfcFaceList( vec_ifc_faces, topo_item_data, FaceConverterOCC::SHELL_TYPE_UNKONWN );
			return;
		}

		const double length_factor = m_unit_converter->getLengthInMeterFactor();
		const shared_ptr<IfcEdge> topo_edge = dynamic_pointer_cast<IfcEdge>( topological_item );
		if( topo_edge )
		{
			const shared_ptr<IfcVertex>& vertex_start = topo_edge->m_EdgeStart;
			gp_Vec point_start;
			if( PointConverterOCC::convertIfcVertex( vertex_start, point_start, length_factor ) )
			{
				const shared_ptr<IfcVertex>& vertex_end = topo_edge->m_EdgeEnd;
				gp_Vec point_end;
				if( PointConverterOCC::convertIfcVertex( vertex_end, point_end, length_factor ) )
				{
					std::vector<gp_Vec> vec_edge_points;
					vec_edge_points.push_back( point_start );
					vec_edge_points.push_back( point_end );
					TopoDS_Wire wire;
					GeomUtilsOCC::createWireFromPoints( vec_edge_points, wire, false );
					if( !wire.IsNull() )
					{
						topo_item_data->m_polylines.push_back( wire );
					}
				}
			}
			return;
		}

		const shared_ptr<IfcFace> ifc_topo_face = dynamic_pointer_cast<IfcFace>( topological_item );
		if( ifc_topo_face )
		{
			// ENTITY IfcFace SUPERTYPE OF( IfcFaceSurface )

			std::vector<shared_ptr<IfcFace> > vec_ifc_faces;
			vec_ifc_faces.push_back( ifc_topo_face );
			m_face_converter->convertIfcFaceList( vec_ifc_faces, topo_item_data, FaceConverterOCC::OPEN_SHELL );
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

			TopoDS_Wire loop_wire;
			m_curve_converter->convertIfcLoop( loop, loop_wire );

			if( !loop_wire.IsNull() )
			{
				bool orientation = true;
				if( topo_face_bound->m_Orientation )
				{
					orientation = topo_face_bound->m_Orientation->m_value;
				}
				if( !orientation )
				{
					loop_wire.Reverse();
				}

				TopoDS_Face poly_cache_top_face = BRepBuilderAPI_MakeFace( loop_wire );
				if( !poly_cache_top_face.IsNull() )
				{
					topo_item_data->addShape( poly_cache_top_face );
				}
			}

			return;
		}

		shared_ptr<IfcLoop> topo_loop = dynamic_pointer_cast<IfcLoop>( topological_item );
		if( topo_loop )
		{
			TopoDS_Wire loop_wire;
			m_curve_converter->convertIfcLoop( topo_loop, loop_wire );

			if( !loop_wire.IsNull() )
			{
				TopoDS_Face topo_loop_face = BRepBuilderAPI_MakeFace( loop_wire );

				if( !topo_loop_face.IsNull() )
				{
					topo_item_data->addShape( topo_loop_face );
				}
			}
			return;
		}

		shared_ptr<IfcPath> topo_path = dynamic_pointer_cast<IfcPath>( topological_item );
		if( topo_path )
		{
			//const std::vector<shared_ptr<IfcOrientedEdge> >& edge_list = topo_path->m_EdgeList;
			std::cout << "IfcPath not implemented" << std::endl;
			return;
		}

		shared_ptr<IfcVertex> topo_vertex = dynamic_pointer_cast<IfcVertex>( topological_item );
		if( topo_vertex )
		{
			gp_Vec topo_vertex_point;
			if( PointConverterOCC::convertIfcVertex( topo_vertex, topo_vertex_point, length_factor ) )
			{
				topo_item_data->addPoint( topo_vertex_point );
				return;
			}
		}

		messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, topological_item.get() );
	}

	void subtractOpenings( const shared_ptr<IfcElement>& ifc_element, shared_ptr<ProductShapeDataOCC>& product_shape )
	{
		std::vector<shared_ptr<RepresentationDataOCC> > vec_opening_data;
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
			shared_ptr<IfcObjectPlacement>	opening_placement = opening->m_ObjectPlacement;			//optional
			gp_Trsf opening_placement_matrix;
			if( opening_placement )
			{
				std::unordered_set<IfcObjectPlacement*> opening_placements_applied;
				PlacementConverterOCC::convertIfcObjectPlacement( opening_placement, length_factor, opening_placement_matrix, this, opening_placements_applied );
			}

			std::vector<shared_ptr<IfcRepresentation> >& vec_opening_representations = opening->m_Representation->m_Representations;
			for( size_t i_representations = 0; i_representations < vec_opening_representations.size(); ++i_representations )
			{
				shared_ptr<IfcRepresentation> ifc_opening_representation = vec_opening_representations[i_representations];
				shared_ptr<RepresentationDataOCC> opening_representation_data( new RepresentationDataOCC() );
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

				opening_representation_data->applyTransformToRepresentation( opening_placement_matrix );
				vec_opening_data.push_back( opening_representation_data );
			}
		}

		TopoDS_Shape unified_opening_meshset;
		double fuzzy_value = 1.e-7*m_unit_converter->getLengthInMeterFactor();// GEOM_EPSILON_CSG_FUZZY;
		for( auto& opening_representation_data : vec_opening_data )
		{
			if( !opening_representation_data )
			{
				continue;
			}

			std::vector<shared_ptr<ItemShapeDataOCC> >& vec_opening_items = opening_representation_data->m_vec_item_data;
			for( auto& opening_item_data : vec_opening_items )
			{
				if( opening_item_data )
				{
					std::vector<TopoDS_Shape>&	opening_meshsets = opening_item_data->m_shapes;
					for( size_t i_opening_meshset = 0; i_opening_meshset < opening_meshsets.size(); ++i_opening_meshset )
					{
						TopoDS_Shape opening_meshset = opening_meshsets[i_opening_meshset];

						if( opening_meshset.IsNull() )
						{
							continue;
						}

						TopAbs_ShapeEnum shape_type = opening_meshset.ShapeType();
						if( shape_type == TopAbs_WIRE || shape_type == TopAbs_EDGE || shape_type == TopAbs_VERTEX )
						{
							continue;
						}
						
						if( unified_opening_meshset.IsNull() )
						{
							unified_opening_meshset = opening_meshset;
							continue;
						}

						// do the unification
						TopoDS_Shape result;
						CSG_AdapterOCC::computeCSG( unified_opening_meshset, opening_meshset, result, CSG_UNION, fuzzy_value, this, ifc_element.get() );
						unified_opening_meshset = result;
					}
				}
			}
		}

		if( !unified_opening_meshset.IsNull() )
		{
			for( auto& product_representation : product_shape->m_vec_representations )
			{
				std::vector<shared_ptr<ItemShapeDataOCC> >& vec_product_items = product_representation->m_vec_item_data;
				for( auto& item_data : vec_product_items )
				{
					if( item_data )
					{
						// now go through all shapes of the item
						for( size_t i_product_meshset = 0; i_product_meshset < item_data->m_shapes.size(); ++i_product_meshset )
						{
							TopoDS_Shape& product_meshset = item_data->m_shapes[i_product_meshset];
							std::stringstream strs_meshset_err;

							// do the subtraction
							TopoDS_Shape result;
							CSG_AdapterOCC::computeCSG( product_meshset, unified_opening_meshset, result, CSG_A_MINUS_B, fuzzy_value, this, ifc_element.get() );
							product_meshset = result;
						}
					}
				}
			}
		}
	}
};

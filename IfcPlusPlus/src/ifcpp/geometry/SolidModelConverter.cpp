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

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/UnitConverter.h>

#include <IfcAdvancedBrepWithVoids.h>
#include <IfcBlock.h>
#include <IfcBooleanResult.h>
#include <IfcBooleanOperand.h>
#include <IfcBooleanOperator.h>
#include <IfcBooleanClippingResult.h>
#include <IfcBoundingBox.h>
#include <IfcBoxedHalfSpace.h>
#include <IfcClosedShell.h>
#include <IfcCsgPrimitive3D.h>
#include <IfcCsgSolid.h>
#include <IfcExtrudedAreaSolid.h>
#include <IfcFacetedBrep.h>
#include <IfcFixedReferenceSweptAreaSolid.h>
#include <IfcHalfSpaceSolid.h>
#include <IfcIndexedColourMap.h>
#include <IfcIndexedPolygonalFaceWithVoids.h>
#include <IfcManifoldSolidBrep.h>
#include <IfcPolygonalBoundedHalfSpace.h>
#include <IfcPolygonalFaceSet.h>
#include <IfcRectangularPyramid.h>
#include <IfcRevolvedAreaSolid.h>
#include <IfcRightCircularCone.h>
#include <IfcRightCircularCylinder.h>
#include <IfcSectionedSpine.h>
#include <IfcSolidModel.h>
#include <IfcSphere.h>
#include <IfcSurfaceCurveSweptAreaSolid.h>
#include <IfcSweptDiskSolid.h>
#include <IfcTessellatedFaceSet.h>
#include <IfcTransitionCode.h>
#include <IfcTriangulatedFaceSet.h>

#include "GeomDebugDump.h"
#include "GeometryInputData.h"
#include "MeshOps.h"
#include "PointConverter.h"
#include "ProfileCache.h"
#include "FaceConverter.h"
#include "CurveConverter.h"
#include "StylesConverter.h"
#include "Sweeper.h"
#include "CSG_Adapter.h"
#include "IncludeCarveHeaders.h"
#include "SolidModelConverter.h"

SolidModelConverter::SolidModelConverter( shared_ptr<GeometrySettings>& gs, shared_ptr<PointConverter>&	pc, shared_ptr<CurveConverter>& cc,
	shared_ptr<FaceConverter>& fc, shared_ptr<ProfileCache>& pcache, shared_ptr<Sweeper>& sw, shared_ptr<StylesConverter>& styles_converter )
	: m_geom_settings( gs ), m_point_converter( pc ), m_curve_converter( cc ), m_face_converter( fc ), m_profile_cache( pcache ),
	m_sweeper( sw ), m_styles_converter(styles_converter)
{
}

SolidModelConverter::~SolidModelConverter()
{
}

// ENTITY IfcSolidModel ABSTRACT SUPERTYPE OF(ONEOF(IfcCsgSolid, IfcManifoldSolidBrep, IfcSweptAreaSolid, IfcSweptDiskSolid))
void SolidModelConverter::convertIfcSolidModel( const shared_ptr<IfcSolidModel>& solid_model, shared_ptr<ItemShapeData> item_data)
{
	double eps = m_geom_settings->getEpsilonMergePoints();
	shared_ptr<IfcSweptAreaSolid> swept_area_solid = dynamic_pointer_cast<IfcSweptAreaSolid>( solid_model );
	if( swept_area_solid )
	{
		//ENTITY IfcSweptAreaSolid
		//	ABSTRACT SUPERTYPE OF(ONEOF(IfcExtrudedAreaSolid, IfcFixedReferenceSweptAreaSolid, IfcRevolvedAreaSolid, IfcSurfaceCurveSweptAreaSolid))
		//	SUBTYPE OF IfcSolidModel;
		//	SweptArea	 :	IfcProfileDef;
		//	Position	 :	OPTIONAL IfcAxis2Placement3D;
		//	WHERE
		//	SweptAreaType	 :	SweptArea.ProfileType = IfcProfileTypeEnum.Area;
		//END_ENTITY;

		shared_ptr<IfcProfileDef>& swept_area = swept_area_solid->m_SweptArea;
		if( !swept_area )
		{
			messageCallback( "SweptArea not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, swept_area_solid.get() );
			return;
		}
			
		shared_ptr<ItemShapeData> item_data_solid( new ItemShapeData() );

		// check if local coordinate system is specified for extrusion
		shared_ptr<TransformData> swept_area_pos;
		if( swept_area_solid->m_Position )
		{
			shared_ptr<IfcAxis2Placement3D> swept_area_position = swept_area_solid->m_Position;
			m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D( swept_area_position, swept_area_pos );
		}

		shared_ptr<IfcExtrudedAreaSolid> extruded_area = dynamic_pointer_cast<IfcExtrudedAreaSolid>( swept_area_solid );
		if( extruded_area )
		{
			convertIfcExtrudedAreaSolid( extruded_area, item_data_solid );
			item_data->addItemData( item_data_solid );
			if (swept_area_pos)
			{
				item_data->applyTransformToItem(swept_area_pos->m_matrix, eps, false);
			}
			return;
		}

		shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter( swept_area, true );
		if (profile_converter)
		{
			profile_converter->simplifyPaths();
			const std::vector<std::vector<vec2> >& profile_paths = profile_converter->getCoordinates();

			shared_ptr<IfcFixedReferenceSweptAreaSolid> fixed_reference_swept_area_solid = dynamic_pointer_cast<IfcFixedReferenceSweptAreaSolid>(swept_area_solid);
			if (fixed_reference_swept_area_solid)
			{
				//Directrix	 : OPTIONAL IfcCurve;
				//StartParam	 : OPTIONAL IfcParameterValue;
				//EndParam	 : OPTIONAL IfcParameterValue;
				//FixedReference	 : IfcDirection;

				shared_ptr<IfcCurve>& ifc_directrix_curve = fixed_reference_swept_area_solid->m_Directrix;
				//shared_ptr<IfcParameterValue>& ifc_start_param = fixed_reference_swept_area_solid->m_StartParam;				//optional
				//shared_ptr<IfcParameterValue>& ifc_end_param = fixed_reference_swept_area_solid->m_EndParam;					//optional
				//shared_ptr<IfcDirection>& ifc_fixed_reference = fixed_reference_swept_area_solid->m_FixedReference;				// TODO: apply fixed reference
				messageCallback("IfcFixedReferenceSweptAreaSolid: Fixed reference not implemented", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, fixed_reference_swept_area_solid.get());

				std::vector<CurveConverter::CurveSegment> segments;
				m_curve_converter->convertIfcCurve(ifc_directrix_curve, segments, true);
				std::vector<vec3> basis_curve_points;
				for (auto& seg : segments)
				{
					std::copy(seg.m_points.begin(), seg.m_points.end(), std::back_inserter(basis_curve_points));
				}

				GeomProcessingParams params(m_geom_settings, fixed_reference_swept_area_solid.get(), this);
				m_sweeper->sweepArea(basis_curve_points, profile_paths, item_data_solid, params);
				item_data->addItemData(item_data_solid);
				item_data->applyTransformToItem(swept_area_pos->m_matrix, eps, false);

				return;
			}

			shared_ptr<IfcRevolvedAreaSolid> revolved_area_solid = dynamic_pointer_cast<IfcRevolvedAreaSolid>(swept_area_solid);
			if (revolved_area_solid)
			{
				convertIfcRevolvedAreaSolid(revolved_area_solid, item_data_solid);
				item_data->addItemData(item_data_solid);
				item_data->applyTransformToItem(swept_area_pos->m_matrix, eps, false );
				return;
			}

			shared_ptr<IfcSurfaceCurveSweptAreaSolid> surface_curve_swept_area_solid = dynamic_pointer_cast<IfcSurfaceCurveSweptAreaSolid>(swept_area_solid);
			if (surface_curve_swept_area_solid)
			{
				shared_ptr<IfcCurve>& ifc_directrix_curve = surface_curve_swept_area_solid->m_Directrix;
				//shared_ptr<IfcParameterValue>& ifc_start_param = surface_curve_swept_area_solid->m_StartParam;				//optional
				//shared_ptr<IfcParameterValue>& ifc_end_param = surface_curve_swept_area_solid->m_EndParam;					//optional
				shared_ptr<IfcSurface>& ifc_reference_surface = surface_curve_swept_area_solid->m_ReferenceSurface;			// TODO: apply start_param, end_param
				messageCallback("IfcSurfaceCurveSweptAreaSolid: StartParam and EndParam not implemented", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, surface_curve_swept_area_solid.get());

				std::vector<CurveConverter::CurveSegment> segments;
				m_curve_converter->convertIfcCurve(ifc_directrix_curve, segments, true);
				std::vector<vec3> directrix_curve_points;
				for (auto& seg : segments)
				{
					std::copy(seg.m_points.begin(), seg.m_points.end(), std::back_inserter(directrix_curve_points));
				}

				// apply reference curve
				//shared_ptr<carve::input::PolylineSetData> reference_surface_data( new carve::input::PolylineSetData() );
				shared_ptr<SurfaceProxy> surface_proxy;
				m_face_converter->convertIfcSurface(ifc_reference_surface, item_data_solid, surface_proxy);

				if (surface_proxy)
				{
					for (size_t ii = 0; ii < directrix_curve_points.size(); ++ii)
					{
						//vec3& point_3d = directrix_curve_points[ii];
						//vec2 point_2d( carve::geom::VECTOR( point_3d.x, point_3d.y ) );
						//surface_proxy->computePointOnSurface( point_3d, point_3d );
						// TODO: implement
					}
				}

				GeomProcessingParams params(m_geom_settings, surface_curve_swept_area_solid.get(), this);
				m_sweeper->sweepArea(directrix_curve_points, profile_paths, item_data_solid, params);
				item_data->addItemData(item_data_solid);
				item_data->applyTransformToItem(swept_area_pos->m_matrix, eps, false);

				return;
			}
		}

		messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, solid_model.get() );
	}

	shared_ptr<IfcManifoldSolidBrep> manifold_solid_brep = dynamic_pointer_cast<IfcManifoldSolidBrep>( solid_model );
	if( manifold_solid_brep )
	{
		//ENTITY IfcManifoldSolidBrep 
		//	ABSTRACT SUPERTYPE OF(ONEOF(IfcAdvancedBrep, IfcFacetedBrep))
		//	SUBTYPE OF IfcSolidModel;
		//		Outer	 :	IfcClosedShell;
		//END_ENTITY;

		shared_ptr<IfcClosedShell>& outer_shell = manifold_solid_brep->m_Outer;
		if( outer_shell )
		{
			// first convert outer shell
			std::vector<shared_ptr<IfcFace> >& vec_faces_outer_shell = outer_shell->m_CfsFaces;
			m_face_converter->convertIfcFaceList( vec_faces_outer_shell, item_data, FaceConverter::CLOSED_SHELL );
		}

		shared_ptr<IfcFacetedBrep> faceted_brep = dynamic_pointer_cast<IfcFacetedBrep>( manifold_solid_brep );
		if( faceted_brep )
		{
			// no additional attributes
			return;
		}

		shared_ptr<IfcAdvancedBrep> advanced_brep = dynamic_pointer_cast<IfcAdvancedBrep>( manifold_solid_brep );
		if( advanced_brep )
		{
			// ENTITY IfcAdvancedBrep	SUPERTYPE OF(IfcAdvancedBrepWithVoids)
			shared_ptr<IfcAdvancedBrepWithVoids> brep_with_voids = dynamic_pointer_cast<IfcAdvancedBrepWithVoids>( solid_model );
			if( brep_with_voids )
			{
					
				//std::vector<shared_ptr<IfcClosedShell> >& vec_voids = advanced_brep_with_voids->m_Voids;

				// TODO: subtract voids from outer shell
#ifdef _DEBUG
				std::cout << "IfcAdvancedBrep not implemented" << std::endl;
#endif
			}
			return;
		}

		messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, solid_model.get() );
	}

	shared_ptr<IfcCsgSolid> csg_solid = dynamic_pointer_cast<IfcCsgSolid>( solid_model );
	if( csg_solid )
	{
		shared_ptr<IfcCsgSelect> csg_select = csg_solid->m_TreeRootExpression;

		shared_ptr<IfcBooleanResult> csg_select_boolean_result = dynamic_pointer_cast<IfcBooleanResult>( csg_select );
		if( csg_select_boolean_result )
		{
			convertIfcBooleanResult( csg_select_boolean_result, item_data);
		}
		else
		{
			shared_ptr<IfcCsgPrimitive3D> csg_select_primitive_3d = dynamic_pointer_cast<IfcCsgPrimitive3D>( csg_select );
			if( csg_select_primitive_3d )
			{
				convertIfcCsgPrimitive3D( csg_select_primitive_3d, item_data );
			}
		}
		return;
	}

	shared_ptr<IfcSectionedSpine> spine = dynamic_pointer_cast<IfcSectionedSpine>(solid_model);
	if( spine )
	{
		convertIfcSectionedSpine( spine, item_data );
		return;
	}

	shared_ptr<IfcSweptDiskSolid> swept_disp_solid = dynamic_pointer_cast<IfcSweptDiskSolid>( solid_model );
	if( swept_disp_solid )
	{
		//ENTITY IfcSweptDiskSolid;
		//	ENTITY IfcRepresentationItem;
		//	INVERSE
		//		LayerAssignments	 : 	SET OF IfcPresentationLayerAssignment FOR AssignedItems;
		//		StyledByItem	 : 	SET [0:1] OF IfcStyledItem FOR Item;
		//	ENTITY IfcGeometricRepresentationItem;
		//	ENTITY IfcSolidModel;
		//		DERIVE
		//		Dim	 : 	IfcDimensionCount :=  3;
		//	ENTITY IfcSweptDiskSolid;
		//		Directrix	 : 	IfcCurve;
		//		Radius	 : 	IfcPositiveLengthMeasure;
		//		InnerRadius	 : 	OPTIONAL IfcPositiveLengthMeasure;
		//		StartParam	 : 	OPTIONAL IfcParameterValue;
		//		EndParam	 : 	OPTIONAL IfcParameterValue;
		//END_ENTITY;	

		shared_ptr<IfcCurve>& directrix_curve = swept_disp_solid->m_Directrix;
		double radius = 0.0;
		const double length_in_meter = m_curve_converter->getPointConverter()->getUnitConverter()->getLengthInMeterFactor();
		if( swept_disp_solid->m_Radius )
		{
			radius = swept_disp_solid->m_Radius->m_value*length_in_meter;
		}

		double radius_inner = -1.0;
		if( swept_disp_solid->m_InnerRadius )
		{
			radius_inner = swept_disp_solid->m_InnerRadius->m_value*length_in_meter;
		}

		// TODO: handle start param, end param

		std::vector<CurveConverter::CurveSegment> segments;
		m_curve_converter->convertIfcCurve( directrix_curve, segments, true );
		std::vector<vec3> basis_curve_points;
		for (auto& seg : segments)
		{
			std::copy(seg.m_points.begin(), seg.m_points.end(), std::back_inserter(basis_curve_points));
		}
		GeomUtils::removeDuplicates(basis_curve_points, eps);

		shared_ptr<ItemShapeData> item_data_solid( new ItemShapeData() );
		const int nvc = m_geom_settings->getNumVerticesPerCircleWithRadius(radius);
		int nvc_disk = nvc;
		if( radius < 0.1 )
		{
			nvc_disk = std::min(12, nvc);
			if( radius < 0.05 )
			{
				nvc_disk = std::min(8, nvc);
			}
		}
			
		GeomProcessingParams params( m_geom_settings, swept_disp_solid.get(),  this );
		m_sweeper->sweepDisk( basis_curve_points, item_data_solid, params, nvc_disk, radius, radius_inner );
		item_data->addItemData( item_data_solid );

		return;
	}

	messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, solid_model.get() );
}

void SolidModelConverter::convertIfcExtrudedAreaSolid( const shared_ptr<IfcExtrudedAreaSolid>& extruded_area, shared_ptr<ItemShapeData> item_data )
{
	if( !extruded_area->m_ExtrudedDirection )
	{
		messageCallback( "Invalid ExtrudedDirection", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, extruded_area.get() );
		return;
	}

	if( !extruded_area->m_Depth )
	{
		messageCallback( "Invalid Depth", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, extruded_area.get() );
		return;
	}
	double length_factor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

	// direction and length of extrusion
	const double depth = extruded_area->m_Depth->m_value*length_factor;
	vec3  extrusion_vector;
	std::vector<shared_ptr<IfcReal> >& vec_direction = extruded_area->m_ExtrudedDirection->m_DirectionRatios;
	if( GeomUtils::allPointersValid( vec_direction ) )
	{
		if( vec_direction.size() > 2 )
		{
			extrusion_vector = carve::geom::VECTOR( vec_direction[0]->m_value * depth, vec_direction[1]->m_value * depth, vec_direction[2]->m_value * depth );
		}
		else if( vec_direction.size() > 1 )
		{
			extrusion_vector = carve::geom::VECTOR( vec_direction[0]->m_value * depth, vec_direction[1]->m_value * depth, 0 );
		}
	}

	// swept area
	shared_ptr<IfcProfileDef>	swept_area = extruded_area->m_SweptArea;
	if( !swept_area )
	{
		messageCallback( "Invalid SweptArea", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, extruded_area.get() );
		return;
	}

	shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter( swept_area, true );
	if (profile_converter)
	{
		profile_converter->simplifyPaths();
		const std::vector<std::vector<vec2> >& paths = profile_converter->getCoordinates();

		if (paths.size() == 0)
		{
			return;
		}
		GeomProcessingParams params(m_geom_settings, extruded_area.get(), this);
		m_sweeper->extrude(paths, extrusion_vector, item_data, params);
	}
}

void SolidModelConverter::convertRevolvedAreaSolid( const std::vector<std::vector<vec2> >& profile_coords_unchecked, const vec3& axis_location, const vec3& axis_direction, double revolution_angle, shared_ptr<ItemShapeData> item_data, BuildingEntity* entity_of_origin )
{
	bool warning_small_loop_detected = false;
	std::vector<std::vector<vec2> > profile_coords;
	for( size_t ii = 0; ii < profile_coords_unchecked.size(); ++ii )
	{
		const std::vector<vec2>& profile_loop_unchecked = profile_coords_unchecked[ii];
		vec3 normal_2d = GeomUtils::computePolygon2DNormal( profile_loop_unchecked );
		bool reverse_loop = false;
		if( ii == 0 )
		{
			if( normal_2d.z < 0 )
			{
				reverse_loop = true;
			}
		}
		else
		{
			if( normal_2d.z > 0 )
			{
				reverse_loop = true;
			}
		}

		double signed_area = carve::geom2d::signedArea( profile_loop_unchecked );
		if( std::abs( signed_area ) < 0.000001 )
		{
			warning_small_loop_detected = true;
			continue;
		}

		profile_coords.push_back( std::vector<vec2>() );
		std::vector<vec2>& profile_loop = profile_coords.back();

		if( reverse_loop )
		{
			std::copy( profile_loop_unchecked.rbegin(), profile_loop_unchecked.rend(), std::back_inserter( profile_loop ) );
		}
		else
		{
			std::copy( profile_loop_unchecked.begin(), profile_loop_unchecked.end(), std::back_inserter( profile_loop ) );
		}
	}

	if( warning_small_loop_detected )
	{
		std::stringstream err;
		err << "std::abs( signed_area ) < 1.e-6";
		messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, entity_of_origin );
	}

	// triangulate
	double eps = m_geom_settings->getEpsilonMergePoints();
	std::vector<vec2> path_merged;
	std::vector<std::pair<size_t, size_t> > path_incorporated_holes;
	std::vector<carve::triangulate::tri_idx> triangulated;
	try
	{
		path_incorporated_holes = carve::triangulate::incorporateHolesIntoPolygon( profile_coords );	// first is loop index, second is vertex index in loop
		path_merged.reserve( path_incorporated_holes.size() );
		for( size_t i = 0; i < path_incorporated_holes.size(); ++i )
		{
			size_t loop_number = path_incorporated_holes[i].first;
			size_t index_in_loop = path_incorporated_holes[i].second;

			if( loop_number >= profile_coords.size() )
			{
				messageCallback( "loop_number >= profile_coords.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
				continue;
			}

			const std::vector<vec2>& loop_2d = profile_coords[loop_number];

			const vec2 & point_2d = loop_2d[index_in_loop];
			path_merged.push_back( point_2d );
		}
		carve::triangulate::triangulate( path_merged, triangulated, eps );
		carve::triangulate::improve( path_merged, triangulated );
	}
	catch( ... )
	{
#ifdef _DEBUG
		messageCallback( "carve::triangulate failed", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
#endif
		return;
	}

	if( profile_coords.size() == 0 )
	{
		messageCallback( "profile_coords.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
		return;
	}
	if( profile_coords[0].size() < 3 )
	{
		messageCallback( "profile_coords[0].size() < 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
		return;
	}
		

	if( revolution_angle > M_PI * 2 ) revolution_angle = M_PI * 2;
	if( revolution_angle < -M_PI * 2 ) revolution_angle = M_PI * 2;

	// TODO: calculate num segments according to length/width/height ratio and overall size of the object
	int num_segments = m_geom_settings->getNumVerticesPerCircle()*(std::abs( revolution_angle ) / (2.0*M_PI));
	if( num_segments < 6 )
	{
		num_segments = 6;
	}
	double angle = 0.0;
	double d_angle = revolution_angle / num_segments;

	// rotation base point is the one with the smallest distance from origin to the rotation axis
	vec3  origin;
	vec3  base_point;
	GeomUtils::closestPointOnLine( origin, axis_location, axis_direction, base_point );
	base_point *= -1.0;

	// check if we have to change the direction
	vec3  polygon_normal = GeomUtils::computePolygon2DNormal( profile_coords[0] );
	const vec2&  pt0_2d = profile_coords[0][0];
	vec3  pt0_3d( carve::geom::VECTOR( pt0_2d.x, pt0_2d.y, 0 ) );
	vec3  pt0 = carve::math::Matrix::ROT( d_angle, axis_direction, eps )*(pt0_3d + base_point);
	if( polygon_normal.z*pt0.z > 0 )
	{
		angle = revolution_angle;
		d_angle = -d_angle;
	}

	// TODO: use m_sweeper->sweepArea

	shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );

	// create vertices
	carve::math::Matrix m;
	for( int ii = 0; ii <= num_segments; ++ii )
	{
		m = carve::math::Matrix::ROT( angle, -axis_direction, eps );
		for( size_t jj = 0; jj < profile_coords.size(); ++jj )
		{
			const std::vector<vec2>& loop = profile_coords[jj];

			for( size_t kk = 0; kk < loop.size(); ++kk )
			{
				const vec2& point = loop[kk];
				vec3  vertex = m*(carve::geom::VECTOR( point.x, point.y, 0 ) + base_point) - base_point;
				polyhedron_data->addVertex( vertex );
			}
		}
		angle += d_angle;
	}

	int num_vertices_per_section = 0;
	for( size_t j = 0; j < profile_coords.size(); ++j )
	{
		const std::vector<vec2>& loop = profile_coords[j];
		num_vertices_per_section += loop.size();
	}

	if( num_vertices_per_section < 3 )
	{
		messageCallback( "num_vertices_per_section < 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
		return;
	}

	const size_t num_vertices_in_poly = polyhedron_data->getVertexCount();
	if( num_vertices_in_poly <= num_vertices_per_section )
	{
		messageCallback( "num_vertices_in_poly <= num_vertices_per_section", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
		return;
	}
	// compute normal of front cap
	const vec3& vertex0_section0 = polyhedron_data->getVertex( 0 );
	const vec3& vertex0_section1 = polyhedron_data->getVertex( num_vertices_per_section );
	vec3 normal_front_cap = (vertex0_section0 - vertex0_section1).normalize();

	//if( std::abs( fmod( revolution_angle, 2.0*M_PI ) ) > 0.0001 )
	{
		// in case of a full circle, there are no front and back caps necessary
		// front cap
		int back_cap_offset = num_vertices_per_section*(num_segments);
		bool flip_faces = false;
		for( size_t ii = 0; ii != triangulated.size(); ++ii )
		{
			const carve::triangulate::tri_idx& triangle = triangulated[ii];
			const size_t vertex_id_a_triangulated = triangle.a;
			const size_t vertex_id_b_triangulated = triangle.b;
			const size_t vertex_id_c_triangulated = triangle.c;

			if( vertex_id_a_triangulated >= path_incorporated_holes.size() || vertex_id_b_triangulated >= path_incorporated_holes.size() || vertex_id_c_triangulated >= path_incorporated_holes.size() )
			{
				messageCallback( "error in revolved area mesh", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
				continue;
			}

			std::pair<size_t, size_t>& vertex_a_pair = path_incorporated_holes[vertex_id_a_triangulated];
			size_t loop_idx_a = vertex_a_pair.first;
			size_t vertex_idx_a = vertex_a_pair.second;

			std::pair<size_t, size_t>& vertex_b_pair = path_incorporated_holes[vertex_id_b_triangulated];
			size_t loop_idx_b = vertex_b_pair.first;
			size_t vertex_idx_b = vertex_b_pair.second;

			std::pair<size_t, size_t>& vertex_c_pair = path_incorporated_holes[vertex_id_c_triangulated];
			size_t loop_idx_c = vertex_c_pair.first;
			size_t vertex_idx_c = vertex_c_pair.second;

			size_t loop_offset_a = 0;
			size_t loop_offset_b = 0;
			size_t loop_offset_c = 0;
			if( loop_idx_a > 0 )
			{
				if( loop_idx_a < profile_coords.size() )
				{
					loop_offset_a = profile_coords[loop_idx_a].size();
				}
			}
			if( loop_idx_b > 0 )
			{
				if( loop_idx_b < profile_coords.size() )
				{
					loop_offset_b = profile_coords[loop_idx_b].size();
				}
			}
			if( loop_idx_c > 0 )
			{
				if( loop_idx_c < profile_coords.size() )
				{
					loop_offset_c = profile_coords[loop_idx_c].size();
				}
			}

			const size_t vertex_id_a = loop_offset_a + vertex_idx_a;
			const size_t vertex_id_b = loop_offset_b + vertex_idx_b;
			const size_t vertex_id_c = loop_offset_c + vertex_idx_c;

			if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
			{
				messageCallback( "error in revolved area mesh", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
				continue;
			}

			const int vertex_id_a_top = vertex_id_a + back_cap_offset;
			const int vertex_id_b_top = vertex_id_b + back_cap_offset;
			const int vertex_id_c_top = vertex_id_c + back_cap_offset;

			if( vertex_id_a_top >= num_vertices_in_poly || vertex_id_b_top >= num_vertices_in_poly || vertex_id_c_top >= num_vertices_in_poly )
			{
				messageCallback( "error in revolved area mesh", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
				continue;
			}

			if( ii == 0 )
			{
				std::vector<vec3> vec_triangle;
				vec_triangle.push_back( polyhedron_data->getVertex( vertex_id_a ) );
				vec_triangle.push_back( polyhedron_data->getVertex( vertex_id_b ) );
				vec_triangle.push_back( polyhedron_data->getVertex( vertex_id_c ) );
				vec3 normal_first_triangle = GeomUtils::computePolygonNormal( vec_triangle, eps );

				if( dot( normal_first_triangle, normal_front_cap ) < 0 )
				{
					flip_faces = true;
				}
			}

#ifdef _DEBUG
			const carve::poly::Vertex<3>& v_a = polyhedron_data->getVertex( vertex_id_a );
			const carve::poly::Vertex<3>& v_b = polyhedron_data->getVertex( vertex_id_b );
			const carve::poly::Vertex<3>& v_c = polyhedron_data->getVertex( vertex_id_c );

			vec3 pa( carve::geom::VECTOR( v_a.v[0], v_a.v[1], v_a.v[2] ) );
			vec3 pb( carve::geom::VECTOR( v_b.v[0], v_b.v[1], v_b.v[2] ) );
			vec3 pc( carve::geom::VECTOR( v_c.v[0], v_c.v[1], v_c.v[2] ) );

			double A = 0.5*(cross( pa - pb, pa - pc ).length());
			if( std::abs( A ) < 0.000000001 )
			{
				messageCallback( "std::abs(A) < 0.000000001", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
			}
#endif

			if( flip_faces )
			{
				polyhedron_data->addFace( vertex_id_a, vertex_id_c, vertex_id_b );		// bottom cap
				polyhedron_data->addFace( vertex_id_a_top, vertex_id_b_top, vertex_id_c_top );	// top cap, flipped outward
			}
			else
			{
				polyhedron_data->addFace( vertex_id_a, vertex_id_b, vertex_id_c );		// bottom cap
				polyhedron_data->addFace( vertex_id_a_top, vertex_id_c_top, vertex_id_b_top );	// top cap, flipped outward
			}
		}
	}

	// faces of revolved shape
	size_t segment_offset = 0;
	for( int ii = 0; ii < num_segments; ++ii )
	{
		size_t loop_offset = segment_offset;
		for( size_t jj = 0; jj < profile_coords.size(); ++jj )
		{
			const std::vector<vec2>& loop = profile_coords[jj];
			const size_t num_points_in_loop = loop.size();

			for( size_t kk = 0; kk < num_points_in_loop; ++kk )
			{
				size_t triangle_idx = loop_offset + kk;
				size_t triangle_idx_up = triangle_idx + num_vertices_per_section;
				size_t triangle_idx_next = loop_offset + (kk + 1)%num_points_in_loop;
				size_t triangle_idx_next_up = triangle_idx_next + num_vertices_per_section;

				if( triangle_idx >= num_vertices_in_poly || triangle_idx_up >= num_vertices_in_poly
					|| triangle_idx_next >= num_vertices_in_poly || triangle_idx_next_up >= num_vertices_in_poly )
				{
					messageCallback( "error in revolved area mesh", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
					continue;
				}

				polyhedron_data->addFace( triangle_idx, triangle_idx_next, triangle_idx_next_up );
				polyhedron_data->addFace( triangle_idx_next_up, triangle_idx_up, triangle_idx );
			}
			loop_offset += num_points_in_loop;
		}
		segment_offset += num_vertices_per_section;
	}

	GeomProcessingParams params(m_geom_settings, false);
	item_data->addOpenOrClosedPolyhedron( polyhedron_data, params );
}

void SolidModelConverter::convertIfcRevolvedAreaSolid( const shared_ptr<IfcRevolvedAreaSolid>& revolved_area, shared_ptr<ItemShapeData> item_data )
{
	// TODO: use Sweeper::sweepArea
	if( !revolved_area )
	{
		messageCallback( "Invalid IfcRevolvedAreaSolid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, revolved_area.get() );
		return;
	}
	if( !revolved_area->m_Angle )
	{
		messageCallback( "Invalid SweptArea", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, revolved_area.get() );
		return;
	}
	shared_ptr<IfcProfileDef> swept_area_profile = revolved_area->m_SweptArea;
	if( !swept_area_profile )
	{
		messageCallback( "Invalid SweptArea", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, revolved_area.get() );
		return;
	}

	// revolution angle
	double angle_factor = m_point_converter->getUnitConverter()->getAngleInRadiantFactor();
	if( m_point_converter->getUnitConverter()->getAngularUnit() == UnitConverter::UNDEFINED )
	{
		// angular unit definition not found in model, default to radian
		angle_factor = 1.0;

		if( revolved_area->m_Angle->m_value > M_PI )
		{
			// assume degree
			angle_factor = M_PI / 180.0;
		}
	}
	double revolution_angle = revolved_area->m_Angle->m_value*angle_factor;
	const double length_factor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

	// revolution axis
	vec3  axis_location;
	vec3  axis_direction;
	if( revolved_area->m_Axis )
	{
		shared_ptr<IfcAxis1Placement> axis_placement = revolved_area->m_Axis;

		if( axis_placement->m_Location )
		{
			shared_ptr<IfcCartesianPoint> location_point = dynamic_pointer_cast<IfcCartesianPoint>(axis_placement->m_Location);
			if( location_point )
			{
				PointConverter::convertIfcCartesianPoint(location_point, axis_location, length_factor);
			}
		}

		if( axis_placement->m_Axis )
		{
			shared_ptr<IfcDirection> axis = axis_placement->m_Axis;
			axis_direction = carve::geom::VECTOR( axis->m_DirectionRatios[0]->m_value, axis->m_DirectionRatios[1]->m_value, axis->m_DirectionRatios[2]->m_value );
		}
	}

	// swept area
	shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter( swept_area_profile, true );
	if (profile_converter)
	{
		profile_converter->simplifyPaths();
		const std::vector<std::vector<vec2> >& profile_coords_unchecked = profile_converter->getCoordinates();
		convertRevolvedAreaSolid(profile_coords_unchecked, axis_location, axis_direction, revolution_angle, item_data);
	}
}

void SolidModelConverter::convertIfcBooleanResult( const shared_ptr<IfcBooleanResult>& bool_result, shared_ptr<ItemShapeData> item_data )
{
	shared_ptr<IfcBooleanOperator>& ifc_boolean_operator = bool_result->m_Operator;
	shared_ptr<IfcBooleanOperand> ifc_first_operand = bool_result->m_FirstOperand;
	shared_ptr<IfcBooleanOperand> ifc_second_operand = bool_result->m_SecondOperand;
	if( !ifc_boolean_operator || !ifc_first_operand || !ifc_second_operand )
	{
		messageCallback( "Invalid IfcBooleanOperator or IfcBooleanOperand", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, bool_result.get() );
		return;
	}
	carve::csg::CSG::OP csg_operation = carve::csg::CSG::A_MINUS_B;

	if (ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_DIFFERENCE)
	{
		csg_operation = carve::csg::CSG::A_MINUS_B;
	}
	else if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_UNION )
	{
		csg_operation = carve::csg::CSG::UNION;
	}
	else if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_INTERSECTION )
	{
		csg_operation = carve::csg::CSG::INTERSECTION;
	}
	else
	{
		messageCallback( "Invalid IfcBooleanOperator", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, bool_result.get() );
		return;
	}

	// convert the first operand
	shared_ptr<ItemShapeData> first_operand_data( new ItemShapeData() );
	shared_ptr<ItemShapeData> empty_operand;
	convertIfcBooleanOperand( ifc_first_operand, first_operand_data, empty_operand );

	// convert the second operand
	shared_ptr<ItemShapeData> second_operand_data( new ItemShapeData() );
	convertIfcBooleanOperand( ifc_second_operand, second_operand_data, first_operand_data );

	//vec4 color(0.5, 0.5, 0.5, 1.);
	//GeomDebugDump::dumpItemShapeInputData(first_operand_data, color);
	//GeomDebugDump::dumpItemShapeInputData(second_operand_data, color);
	//GeomDebugDump::moveOffset(1);

#if defined(_DEBUG) || defined(_DEBUG_RELEASE)
	int tag = -1;
	std::string className = IFC4X3::EntityFactory::getStringForClassID(ifc_second_operand->classID());
	if (dynamic_pointer_cast<IfcBooleanResult>(ifc_second_operand))
	{
		tag = dynamic_pointer_cast<IfcBooleanResult>(ifc_second_operand)->m_tag;
	}
	else if (dynamic_pointer_cast<IfcFacetedBrep>(ifc_second_operand))
	{
		tag = dynamic_pointer_cast<IfcFacetedBrep>(ifc_second_operand)->m_tag;
	}
	else if (dynamic_pointer_cast<IfcExtrudedAreaSolid>(ifc_second_operand))
	{
		tag = dynamic_pointer_cast<IfcExtrudedAreaSolid>(ifc_second_operand)->m_tag;
	}
#endif

	// for every first operand polyhedrons, apply all second operand polyhedrons
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vec_first_operand_meshsets = first_operand_data->m_meshsets;
	for( size_t i_meshset_first = 0; i_meshset_first < vec_first_operand_meshsets.size(); ++i_meshset_first )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& first_operand_meshset = vec_first_operand_meshsets[i_meshset_first];

		if (!first_operand_meshset)
		{
			continue;
		}
		std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vec_second_operand_meshsets = second_operand_data->m_meshsets;
		GeomProcessingParams params(m_geom_settings);
		params.callbackFunc = this;
		params.ifc_entity = bool_result.get();
		CSG_Adapter::computeCSG(first_operand_meshset, vec_second_operand_meshsets, csg_operation, params);
	}

	// now copy processed first operands to result input data
	std::copy( first_operand_data->m_meshsets.begin(), first_operand_data->m_meshsets.end(), std::back_inserter( item_data->m_meshsets ) );

	// copy also styles from operands, if any
	std::copy(first_operand_data->m_styles.begin(), first_operand_data->m_styles.end(), std::back_inserter(item_data->m_styles));

	//GeomDebugDump::dumpItemShapeInputData(first_operand_data, color);

	shared_ptr<IfcBooleanClippingResult> boolean_clipping_result = dynamic_pointer_cast<IfcBooleanClippingResult>( bool_result );
	if( boolean_clipping_result )
	{
		// no additional attributes, just the type of operands and operator is restricted:
		// WHERE
		// FirstOperand is IFCSWEPTAREASOLID or IFCSWEPTDISCSOLID or IFCBOOLEANCLIPPINGRESULT
		// SecondOperand is IFCHALFSPACESOLID
		// OperatorType	 :	Operator = DIFFERENCE;
	}
}

void SolidModelConverter::convertIfcCsgPrimitive3D( const shared_ptr<IfcCsgPrimitive3D>& csg_primitive, shared_ptr<ItemShapeData> item_data )
{
	shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );
	const double length_factor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();
	GeomProcessingParams params(m_geom_settings, false);

	// ENTITY IfcCsgPrimitive3D  ABSTRACT SUPERTYPE OF(ONEOF(IfcBlock, IfcRectangularPyramid, IfcRightCircularCone, IfcRightCircularCylinder, IfcSphere
	shared_ptr<TransformData> primitive_placement_transform;
	shared_ptr<IfcAxis2Placement3D>& primitive_placement = csg_primitive->m_Position;
	if( primitive_placement )
	{
		m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D( primitive_placement, primitive_placement_transform );
	}

	carve::math::Matrix primitive_placement_matrix;
	if( primitive_placement_transform )
	{
		primitive_placement_matrix = primitive_placement_transform->m_matrix;
	}

	if( !item_data )
	{
		messageCallback( "Invalid item_data", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
		return;
	}

	shared_ptr<IfcBlock> block = dynamic_pointer_cast<IfcBlock>( csg_primitive );
	if( block )
	{
		if( !block->m_XLength )
		{
			messageCallback( "Invalid XLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}
		if( !block->m_YLength )
		{
			messageCallback( "Invalid YLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}
		if( !block->m_ZLength )
		{
			messageCallback( "Invalid ZLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}

		const double x_length = block->m_XLength->m_value*0.5*length_factor;
		const double y_length = block->m_YLength->m_value*0.5*length_factor;
		const double z_length = block->m_ZLength->m_value*0.5*length_factor;

		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, y_length, z_length ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( -x_length, y_length, z_length ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( -x_length, -y_length, z_length ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, -y_length, z_length ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, y_length, -z_length ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( -x_length, y_length, -z_length ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( -x_length, -y_length, -z_length ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, -y_length, -z_length ) );

		polyhedron_data->addFace( 0, 1, 2 );
		polyhedron_data->addFace( 2, 3, 0 );

		polyhedron_data->addFace( 7, 6, 5 );
		polyhedron_data->addFace( 5, 4, 7 );

		polyhedron_data->addFace( 0, 4, 5 );
		polyhedron_data->addFace( 5, 1, 0 );

		polyhedron_data->addFace( 1, 5, 6 );
		polyhedron_data->addFace( 6, 2, 1 );

		polyhedron_data->addFace( 2, 6, 7 );
		polyhedron_data->addFace( 7, 3, 2 );

		polyhedron_data->addFace( 3, 7, 4 );
		polyhedron_data->addFace( 4, 0, 3 );

		item_data->addOpenOrClosedPolyhedron( polyhedron_data, params );
		return;
	}

	shared_ptr<IfcRectangularPyramid> rectangular_pyramid = dynamic_pointer_cast<IfcRectangularPyramid>( csg_primitive );
	if( rectangular_pyramid )
	{
		if( !rectangular_pyramid->m_XLength )
		{
			messageCallback( "Invalid XLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}
		if( !rectangular_pyramid->m_YLength )
		{
			messageCallback( "Invalid YLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}
		if( !rectangular_pyramid->m_Height )
		{
			messageCallback( "Invalid Height", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}

		const double x_length = rectangular_pyramid->m_XLength->m_value*0.5*length_factor;
		const double y_length = rectangular_pyramid->m_YLength->m_value*0.5*length_factor;
		const double height = rectangular_pyramid->m_Height->m_value*0.5*length_factor;

		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( 0, 0, height ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, -y_length, 0.0 ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( -x_length, -y_length, 0.0 ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( -x_length, y_length, 0.0 ) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, y_length, 0.0 ) );

		polyhedron_data->addFace( 1, 2, 3 );
		polyhedron_data->addFace( 3, 4, 1 );
		polyhedron_data->addFace( 0, 2, 1 );
		polyhedron_data->addFace( 0, 1, 4 );
		polyhedron_data->addFace( 0, 4, 3 );
		polyhedron_data->addFace( 0, 3, 2 );

		item_data->addOpenOrClosedPolyhedron( polyhedron_data, params );
		return;
	}

	shared_ptr<IfcRightCircularCone> right_circular_cone = dynamic_pointer_cast<IfcRightCircularCone>( csg_primitive );
	if( right_circular_cone )
	{
		if( !right_circular_cone->m_Height )
		{
			messageCallback( "Invalid Height", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}
		if( !right_circular_cone->m_BottomRadius )
		{
			messageCallback( "Invalid BottomRadius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}

		const double height = right_circular_cone->m_Height->m_value*length_factor;
		const double radius = right_circular_cone->m_BottomRadius->m_value*length_factor;

		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( 0.0, 0.0, height ) ); // top
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( 0.0, 0.0, 0.0 ) ); // bottom center

		double angle = 0;
		double d_angle = 2.0*M_PI / double(m_geom_settings->getNumVerticesPerCircleWithRadius(radius) );
		for( int i = 0; i < m_geom_settings->getNumVerticesPerCircleWithRadius(radius); ++i )
		{
			polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( sin( angle )*radius, cos( angle )*radius, 0.0 ) );
			angle += d_angle;
		}

		// outer shape
		for( int i = 0; i < m_geom_settings->getNumVerticesPerCircleWithRadius(radius) - 1; ++i )
		{
			polyhedron_data->addFace( 0, i + 3, i + 2 );
		}
		polyhedron_data->addFace( 0, 2, m_geom_settings->getNumVerticesPerCircleWithRadius(radius) + 1 );

		// bottom circle
		for( int i = 0; i < m_geom_settings->getNumVerticesPerCircleWithRadius(radius) - 1; ++i )
		{
			polyhedron_data->addFace( 1, i + 2, i + 3 );
		}
		polyhedron_data->addFace( 1, m_geom_settings->getNumVerticesPerCircleWithRadius(radius) + 1, 2 );

		item_data->addOpenOrClosedPolyhedron( polyhedron_data, params );
		return;
	}

	shared_ptr<IfcRightCircularCylinder> right_circular_cylinder = dynamic_pointer_cast<IfcRightCircularCylinder>( csg_primitive );
	if( right_circular_cylinder )
	{
		if( !right_circular_cylinder->m_Height )
		{
			messageCallback( "Invalid Height", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}

		if( !right_circular_cylinder->m_Radius )
		{
			messageCallback( "Invalid Radius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}

		double height = right_circular_cylinder->m_Height->m_value*length_factor;
		double radius = right_circular_cylinder->m_Radius->m_value*length_factor;

		double angle = 0;
		const size_t num_points = m_geom_settings->getNumVerticesPerCircleWithRadius(radius);
		const double d_angle = 2.0*M_PI / double( num_points );	// TODO: adapt to model size and complexity
		for( int i = 0; i < num_points; ++i )
		{
			double x = cos( angle )*radius;
			double y = sin( angle )*radius;
			polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x, y, height ) );
			polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x, y, 0.0 ) );
			angle += d_angle;
		}

		for( size_t i = 0; i < num_points; ++i )
		{
			size_t idx_next = ( i + 1 )%num_points;
			size_t idx_next_top = idx_next*2;
			size_t idx_next_bottom = idx_next_top + 1;
			polyhedron_data->addFace( i*2, i*2 + 1, idx_next_bottom );		// side
			polyhedron_data->addFace( idx_next_bottom, idx_next_top, i*2 );	// side
		}

		for( size_t i = 0; i < num_points-2; ++i )
		{
			polyhedron_data->addFace( 0, i*2 + 2, i*2 + 4 );	// top cap:		0-2-4	0-4-6		0-6-8
			polyhedron_data->addFace( 1, i*2 + 5, i*2 + 3 );	// bottom cap:	1-5-3	1-7-5		1-9-7
		}

		item_data->addOpenOrClosedPolyhedron( polyhedron_data, params );
		return;
	}

	shared_ptr<IfcSphere> sphere = dynamic_pointer_cast<IfcSphere>( csg_primitive );
	if( sphere )
	{
		if( !sphere->m_Radius )
		{
			messageCallback( "Invalid Radius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}

		double radius = sphere->m_Radius->m_value;

		//        \   |   /
		//         2- 1 -nvc
		//        / \ | / \
				//    ---3--- 0 ---7---
		//       \  / | \ /
		//         4- 5 -6
		//        /   |   \

		shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( 0.0, 0.0, radius ) ); // top

		const int nvc = m_geom_settings->getNumVerticesPerCircleWithRadius(radius);
		const int num_vertical_edges = nvc*0.5;
		double d_vertical_angle = M_PI / double( num_vertical_edges - 1 );	// TODO: adapt to model size and complexity
		double vertical_angle = d_vertical_angle;

		for( int vertical = 1; vertical < num_vertical_edges - 1; ++vertical )
		{
			// for each vertical angle, add one horizontal circle
			double vertical_level = cos( vertical_angle )*radius;
			double radius_at_level = sin( vertical_angle )*radius;
			double horizontal_angle = 0;
			double d_horizontal_angle = 2.0*M_PI / double( nvc );
			for( int i = 0; i < nvc; ++i )
			{
				polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( sin( horizontal_angle )*radius_at_level, cos( horizontal_angle )*radius_at_level, vertical_level ) );
				horizontal_angle += d_horizontal_angle;
			}
			vertical_angle += d_vertical_angle;
		}
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( 0.0, 0.0, -radius ) ); // bottom

		// upper triangle fan
		for( int i = 0; i < nvc - 1; ++i )
		{
			polyhedron_data->addFace( 0, i + 2, i + 1 );
		}
		polyhedron_data->addFace( 0, 1, nvc );

		for( int vertical = 1; vertical < num_vertical_edges - 2; ++vertical )
		{
			int offset_inner = nvc*( vertical - 1 ) + 1;
			int offset_outer = nvc*vertical + 1;
			for( int i = 0; i < nvc - 1; ++i )
			{
				polyhedron_data->addFace( offset_inner + i, offset_inner + 1 + i, offset_outer + 1 + i );
				polyhedron_data->addFace( offset_outer + 1 + i, offset_outer + i, offset_inner + i );
			}
			polyhedron_data->addFace( offset_inner + nvc - 1, offset_inner, offset_outer );
			polyhedron_data->addFace( offset_outer, offset_outer + nvc - 1, offset_inner + nvc - 1 );

		}

		// lower triangle fan
		int last_index = ( num_vertical_edges - 2 )*nvc + 1;
		for( int i = 0; i < nvc - 1; ++i )
		{
			polyhedron_data->addFace( last_index, last_index - ( i + 2 ), last_index - ( i + 1 ) );
		}
		polyhedron_data->addFace( last_index, last_index - 1, last_index - nvc );
		item_data->addOpenOrClosedPolyhedron( polyhedron_data, params );
		return;
	}
	messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
}

void SolidModelConverter::extrudeBox( const std::vector<vec3>& boundary_points, const vec3& extrusion_vector, shared_ptr<carve::input::PolyhedronData>& box_data )
{
	box_data->addVertex( boundary_points[0] );
	box_data->addVertex( boundary_points[1] );
	box_data->addVertex( boundary_points[2] );
	box_data->addVertex( boundary_points[3] );
	box_data->addVertex( boundary_points[0] + extrusion_vector );
	box_data->addVertex( boundary_points[1] + extrusion_vector );
	box_data->addVertex( boundary_points[2] + extrusion_vector );
	box_data->addVertex( boundary_points[3] + extrusion_vector );
	box_data->addFace( 0, 1, 2 );
	box_data->addFace( 2, 3, 0 );
	box_data->addFace( 1, 5, 6 );
	box_data->addFace( 6, 2, 1 );
	box_data->addFace( 5, 4, 7 );
	box_data->addFace( 7, 6, 5 );
	box_data->addFace( 0, 3, 7 );
	box_data->addFace( 7, 4, 0 );
	box_data->addFace( 0, 4, 5 );
	box_data->addFace( 5, 1, 0 );
	box_data->addFace( 2, 6, 7 );
	box_data->addFace( 7, 3, 2 );
}

void SolidModelConverter::convertIfcHalfSpaceSolid( const shared_ptr<IfcHalfSpaceSolid>& half_space_solid, shared_ptr<ItemShapeData>& item_data, const shared_ptr<ItemShapeData>& other_operand )
{
	//ENTITY IfcHalfSpaceSolid SUPERTYPE OF(ONEOF(IfcBoxedHalfSpace, IfcPolygonalBoundedHalfSpace))
	double length_factor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();
	shared_ptr<IfcSurface> base_surface = half_space_solid->m_BaseSurface;
	GeomProcessingParams params(m_geom_settings, false);
	double eps = params.epsMergePoints;

	// base surface
	shared_ptr<IfcElementarySurface> elem_base_surface = dynamic_pointer_cast<IfcElementarySurface>( base_surface );
	if( !elem_base_surface )
	{
		messageCallback( "The base surface shall be an unbounded surface (subtype of IfcElementarySurface)", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, half_space_solid.get() );
		return;
	}
	shared_ptr<IfcAxis2Placement3D>& base_surface_pos = elem_base_surface->m_Position;
	carve::geom::plane<3> base_surface_plane;
	vec3 base_surface_position;
	shared_ptr<TransformData> base_position_transform;
	shared_ptr<TransformData> base_position_rotate;
	if( base_surface_pos )
	{
		m_curve_converter->getPlacementConverter()->getPlane( base_surface_pos, base_surface_plane, base_surface_position );
		m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D( base_surface_pos, base_position_transform );
		m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D(base_surface_pos, base_position_rotate, true);
	}
	carve::math::Matrix base_position_matrix = carve::math::Matrix::IDENT();
	if( base_position_transform )
	{
		base_position_matrix = base_position_transform->m_matrix;
	}
	carve::math::Matrix base_rotate_matrix = carve::math::Matrix::IDENT();
	if (base_position_rotate)
	{
		base_rotate_matrix = base_position_rotate->m_matrix;
	}

	// If the agreement flag is TRUE, then the subset is the one the normal points away from
	bool agreement = half_space_solid->m_AgreementFlag->m_value;
	if( !agreement )
	{
		base_surface_plane.negate();
	}

	shared_ptr<IfcBoxedHalfSpace> boxed_half_space = dynamic_pointer_cast<IfcBoxedHalfSpace>( half_space_solid );
	if( boxed_half_space )
	{
		shared_ptr<IfcBoundingBox> bbox = boxed_half_space->m_Enclosure;
		if( !bbox )
		{
			messageCallback( "Enclosure not given", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, half_space_solid.get() );
			return;
		}

		if( !bbox->m_Corner || !bbox->m_XDim || !bbox->m_YDim || !bbox->m_ZDim )
		{
			messageCallback( "Enclosure not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, half_space_solid.get() );
			return;
		}
		shared_ptr<IfcCartesianPoint>&			bbox_corner = bbox->m_Corner;
		shared_ptr<IfcPositiveLengthMeasure>&	bbox_x_dim = bbox->m_XDim;
		shared_ptr<IfcPositiveLengthMeasure>&	bbox_y_dim = bbox->m_YDim;
		shared_ptr<IfcPositiveLengthMeasure>&	bbox_z_dim = bbox->m_ZDim;

		vec3 corner;
		PointConverter::convertIfcCartesianPoint( bbox_corner, corner, length_factor );
		carve::math::Matrix box_position_matrix = base_position_matrix*carve::math::Matrix::TRANS( corner );

		// else, its an unbounded half space solid, create simple box
		shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );
		polyhedron_data->addVertex( carve::geom::VECTOR( bbox_x_dim->m_value, bbox_y_dim->m_value, bbox_z_dim->m_value ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( -bbox_x_dim->m_value, bbox_y_dim->m_value, bbox_z_dim->m_value ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( -bbox_x_dim->m_value, -bbox_y_dim->m_value, bbox_z_dim->m_value ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( bbox_x_dim->m_value, -bbox_y_dim->m_value, bbox_z_dim->m_value ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( bbox_x_dim->m_value, bbox_y_dim->m_value, -bbox_z_dim->m_value ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( -bbox_x_dim->m_value, bbox_y_dim->m_value, -bbox_z_dim->m_value ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( -bbox_x_dim->m_value, -bbox_y_dim->m_value, -bbox_z_dim->m_value ) );
		polyhedron_data->addVertex( carve::geom::VECTOR( bbox_x_dim->m_value, -bbox_y_dim->m_value, -bbox_z_dim->m_value ) );

		polyhedron_data->addFace( 0, 1, 2 );
		polyhedron_data->addFace( 2, 3, 0 );
		polyhedron_data->addFace( 7, 6, 5 );
		polyhedron_data->addFace( 5, 4, 7 );
		polyhedron_data->addFace( 0, 4, 5 );
		polyhedron_data->addFace( 5, 1, 0 );
		polyhedron_data->addFace( 1, 5, 6 );
		polyhedron_data->addFace( 6, 2, 1 );
		polyhedron_data->addFace( 2, 6, 7 );
		polyhedron_data->addFace( 7, 3, 2 );
		polyhedron_data->addFace( 3, 7, 4 );
		polyhedron_data->addFace( 4, 0, 3 );

		// apply box coordinate system
		for( std::vector<vec3>::iterator it_points = polyhedron_data->points.begin(); it_points != polyhedron_data->points.end(); ++it_points )
		{
			vec3 & poly_point = ( *it_points );
			poly_point = box_position_matrix*poly_point;
		}

		item_data->addOpenOrClosedPolyhedron( polyhedron_data, params );
		return;
	}

	// check dimensions of other operand
	carve::geom::aabb<3> bbox_other_operand;
	double extrusion_depth = HALF_SPACE_BOX_SIZE*m_point_converter->getUnitConverter()->getCustomLengthFactor();
	vec3 extrusion_extent = carve::geom::VECTOR(0, 0, extrusion_depth);
	if( other_operand )
	{
		std::unordered_set<ItemShapeData*> setVisited;
		carve::math::Matrix matrixIdentity;
		other_operand->computeItemBoundingBox(bbox_other_operand, /*matrixIdentity,*/ setVisited);
		extrusion_extent = bbox_other_operand.extent;

		extrusion_depth = 2.1 * extrusion_extent.z;
	}

	shared_ptr<IfcPolygonalBoundedHalfSpace> polygonal_half_space = dynamic_pointer_cast<IfcPolygonalBoundedHalfSpace>( half_space_solid );
	if( polygonal_half_space )
	{
		// ENTITY IfcPolygonalBoundedHalfSpace 
		//	SUBTYPE OF IfcHalfSpaceSolid;
		//	Position	 :	IfcAxis2Placement3D;
		//	PolygonalBoundary	 :	IfcBoundedCurve;

		shared_ptr<TransformData> boundary_transform;
		carve::math::Matrix boundary_position_matrix( carve::math::Matrix::IDENT() );
		vec3 boundary_plane_normal( carve::geom::VECTOR( 0, 0, 1 ) );
		vec3 boundary_position;
		if( polygonal_half_space->m_Position )
		{
			m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D( polygonal_half_space->m_Position, boundary_transform );
			if( boundary_transform )
			{
				boundary_position_matrix = boundary_transform->m_matrix;
			}
			boundary_plane_normal = carve::geom::VECTOR( boundary_position_matrix._31, boundary_position_matrix._32, boundary_position_matrix._33 );
			boundary_position = carve::geom::VECTOR( boundary_position_matrix._41, boundary_position_matrix._42, boundary_position_matrix._43 );

			extrusion_extent = boundary_position_matrix * extrusion_extent;
			extrusion_depth = 2.1 * extrusion_extent.z;
		}

		// PolygonalBoundary is given in 2D
		std::vector<vec2> polygonal_boundary;
		std::vector<vec2> segment_start_points_2d;
		shared_ptr<IfcBoundedCurve> bounded_curve = polygonal_half_space->m_PolygonalBoundary;
		m_curve_converter->convertIfcCurve2D( bounded_curve, polygonal_boundary, segment_start_points_2d, true );
		GeomUtils::unClosePolygon( polygonal_boundary, m_geom_settings->getEpsilonMergePoints());
		ProfileConverter::simplifyPath( polygonal_boundary, m_geom_settings->getEpsilonMergePoints(), m_geom_settings->getEpsilonCoplanarAngle() );

		vec3 solid_extrusion_direction = boundary_plane_normal;
		double agreement_check = dot( base_surface_plane.N, boundary_plane_normal );
		if( agreement_check > 0 )
		{
			solid_extrusion_direction = -solid_extrusion_direction;
		}

		std::vector<std::vector<vec2> > paths;
		paths.push_back( polygonal_boundary );
		shared_ptr<ItemShapeData> polygonal_halfspace_item_data( new ItemShapeData );
		GeomProcessingParams params( m_geom_settings, polygonal_half_space.get(),  this );
		vec3 extrusionVector = carve::geom::VECTOR(0, 0, extrusion_depth);
		m_sweeper->extrude( paths, extrusionVector, polygonal_halfspace_item_data, params );

		if( polygonal_halfspace_item_data->m_meshsets.size() != 1 )
		{
#ifdef _DEBUG
			vec4 color(0.5, 0.5, 0.5, 1);
			GeomDebugDump::dumpPolyline(paths, color, true, false);
#endif
			messageCallback( "polygonal_halfspace_item_data->meshsets.size() != 1", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
			return;
		}

		// apply position of PolygonalBoundary
		polygonal_halfspace_item_data->applyTransformToItem( boundary_position_matrix, eps, false );

		shared_ptr<carve::mesh::MeshSet<3> > polygonal_halfspace_meshset = polygonal_halfspace_item_data->m_meshsets[0];
		if( !polygonal_halfspace_meshset )
		{
			return;
		}
		const size_t num_poly_points = polygonal_halfspace_meshset->vertex_storage.size();

		if( num_poly_points % 2 )
		{
			// num_poly_points is odd
			messageCallback( "num_poly_points should be an even number", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
			return;
		}

		const size_t num_poly_boundary_points = num_poly_points / 2;

		// project to base surface
		for( size_t i_base_point = 0; i_base_point < polygonal_halfspace_meshset->vertex_storage.size(); ++i_base_point )
		{
			carve::mesh::Vertex<3>& poly_vert = polygonal_halfspace_meshset->vertex_storage[i_base_point];
			vec3& poly_point = poly_vert.v;

			// points below the base surface are projected into plane
			vec3 v;
			double t;
			carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( base_surface_plane, poly_point, poly_point + boundary_plane_normal, v, t, eps );
			if( intersect > 0 )
			{
				if( agreement_check > 0 )
				{
					if( i_base_point < num_poly_boundary_points )
					{
						poly_point = v + solid_extrusion_direction*extrusion_depth;
					}
					else
					{
						poly_point = v;
					}
				}
				else
				{
					if( i_base_point < num_poly_boundary_points )
					{
						poly_point = v;
					}
					else
					{
						poly_point = v + solid_extrusion_direction*extrusion_depth;
					}
				}
			}
			else
			{
				messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, polygonal_half_space.get() );
			}
		}

		for( size_t i_mesh = 0; i_mesh < polygonal_halfspace_meshset->meshes.size(); ++i_mesh )
		{
			polygonal_halfspace_meshset->meshes[i_mesh]->recalc(eps);
		}

		item_data->m_meshsets.push_back( polygonal_halfspace_meshset );

#ifdef _DEBUG
		bool isClosed = polygonal_halfspace_meshset->isClosed();
		if( !isClosed || polygonal_half_space->m_tag == 754980 )
		{
			std::cout << "!polygonal_halfspace_meshset->isClosed()" << std::endl;
			bool drawNormals = true;
			GeomDebugDump::dumpMeshset(polygonal_halfspace_meshset, vec4(0.7, 0.7, 0.7, 1), drawNormals, true);
		}
#endif
	}
	else
	{
		// else, its an unbounded half space solid, create simple box

		if (!bbox_other_operand.isEmpty())
		{
			double max_extent = std::max(bbox_other_operand.extent.x, std::max(bbox_other_operand.extent.y, bbox_other_operand.extent.z));
			if (std::abs(max_extent) > eps)
			{
				extrusion_depth = 2.1 * max_extent;
			}
		}

		{
			// project center of other operand onto plane, create box there
			vec3 v;
			vec3 poly_point = bbox_other_operand.pos;
			const vec3& base_surface_normal = base_surface_plane.N;
			double t;
			carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection(base_surface_plane, poly_point, poly_point + base_surface_normal, v, t, eps);
			if (intersect > 0)
			{
				vec3 localZ = carve::geom::VECTOR(0, 0, 1);
				localZ = base_rotate_matrix * localZ;
				double dotProduct = dot(localZ, base_surface_normal);
										
				vec3 posX = carve::geom::VECTOR(extrusion_depth, 0, 0);
				posX = base_rotate_matrix * posX;
				vec3 posY = carve::geom::VECTOR(0, extrusion_depth, 0);
				posY = base_rotate_matrix * posY;

				std::vector<vec3> base_surface_points = { v + posX + posY, v - posX + posY, v - posX - posY, v + posX - posY };
				if (dotProduct < 0)
				{
					std::reverse(base_surface_points.begin(), base_surface_points.end());
				}
				vec3  half_space_extrusion_direction = -base_surface_normal;
				vec3  half_space_extrusion_vector = half_space_extrusion_direction * extrusion_depth;
				shared_ptr<carve::input::PolyhedronData> half_space_box_data(new carve::input::PolyhedronData());
				extrudeBox(base_surface_points, half_space_extrusion_vector, half_space_box_data);
				item_data->addOpenOrClosedPolyhedron(half_space_box_data, params);
			}
		}
			
		return;
	}
}

void SolidModelConverter::copyIndexedFaceLoop(const std::vector<shared_ptr<IfcPositiveInteger> >& vecIdx, const std::vector<vec3>& vecPointsIn, std::vector<vec3>& vecOut)
{
	for( size_t ii = 0; ii < vecIdx.size(); ++ii )
	{
		const shared_ptr<IfcPositiveInteger>& positiveInt = vecIdx[ii];
		if( !positiveInt )
		{
			continue;
		}
		size_t idx = (size_t)positiveInt->m_value - 1;  // 1 based index in IfcIndexedPolygonalFace
		if( idx >= vecPointsIn.size() )
		{
			std::cout << "copyIndexedFaceLoop: invalid index" << std::endl;
			continue;
		}
		vecOut.push_back(vecPointsIn[idx]);
	}
}

void SolidModelConverter::convertIndexedPolygonalFace(shared_ptr<IfcIndexedPolygonalFace>& polygonalFace, std::vector<vec3>& pointStorage, PolyInputCache3D& poly_cache)
{
	// IfcIndexedPolygonalFace -----------------------------------------------------------
	// std::vector<shared_ptr<IfcPositiveInteger> >			m_CoordIndex;

	std::vector<vec3> faceOuterBound;
	copyIndexedFaceLoop(polygonalFace->m_CoordIndex, pointStorage, faceOuterBound);

	std::vector<std::vector<vec3> > face_loops;
	face_loops.push_back(faceOuterBound);

	shared_ptr<IfcIndexedPolygonalFaceWithVoids> faceWithVoids = dynamic_pointer_cast<IfcIndexedPolygonalFaceWithVoids>(polygonalFace);
	if( faceWithVoids )
	{
		for( auto innerLoop : faceWithVoids->m_InnerCoordIndices )
		{
			std::vector<vec3> faceInnerLoop;
			copyIndexedFaceLoop(innerLoop, pointStorage, faceInnerLoop);
			if( faceInnerLoop.size() > 2 )
			{
				face_loops.push_back(faceInnerLoop);
			}
		}
	}

	bool mergeAlignedEdges = true;
	GeomProcessingParams params( m_geom_settings, polygonalFace.get(),  this );
	FaceConverter::createTriangulated3DFace(face_loops, poly_cache, params, false);

#ifdef _DEBUG
	//vec4 color(0.3, 0.4, 0.5, 1.0);
	//GeomDebugDump::dumpPolyline(face_loops, color, true);
#endif
}

void SolidModelConverter::convertTesselatedItem( const shared_ptr<IfcTessellatedItem>& tessellatedItem, shared_ptr<ItemShapeData>& item_data)
{
	if( !tessellatedItem )
	{
		return;
	}

	//ENTITY IfcTessellatedItem   ABSTRACT SUPERTYPE OF(ONEOF(IfcIndexedPolygonalFace, IfcTessellatedFaceSet))
	double eps = m_geom_settings->getEpsilonMergePoints();
	GeomProcessingParams params(m_geom_settings, tessellatedItem.get(), this);
	PolyInputCache3D polyCache(eps);

	shared_ptr<IfcIndexedPolygonalFace> polygonalFace = dynamic_pointer_cast<IfcIndexedPolygonalFace>(tessellatedItem);
	if( polygonalFace )
	{
		//ENTITY IfcIndexedPolygonalFace SUPERTYPE OF(IfcIndexedPolygonalFaceWithVoids)
		messageCallback("Single IfcIndexedPolygonalFace, no coordinates", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, tessellatedItem.get());
		return;
	}

	shared_ptr<IfcTessellatedFaceSet> abstractFaceSet = dynamic_pointer_cast<IfcTessellatedFaceSet>(tessellatedItem);
	if( abstractFaceSet )
	{
		// ENTITY IfcTessellatedFaceSet ABSTRACT SUPERTYPE OF(ONEOF(IfcPolygonalFaceSet, IfcTriangulatedFaceSet))

		shared_ptr<IfcCartesianPointList3D> pointList = abstractFaceSet->m_Coordinates;
		if( !abstractFaceSet->m_Coordinates )
		{
			messageCallback("IfcTessellatedFaceSet without coordinates", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, abstractFaceSet.get());
			return;
		}

		std::vector<vec3> pointVec;
		m_point_converter->convertPointList(pointList->m_CoordList, pointVec);

		shared_ptr<IfcPolygonalFaceSet> polygonalFaceSet = dynamic_pointer_cast<IfcPolygonalFaceSet>(abstractFaceSet);
		if( polygonalFaceSet )
		{
			std::vector<shared_ptr<IfcIndexedPolygonalFace> >& vecFaces = polygonalFaceSet->m_Faces;
			for( auto face : vecFaces )
			{
				convertIndexedPolygonalFace(face, pointVec, polyCache);
			}
			std::vector<shared_ptr<IfcPositiveInteger> >& PnIndex = polygonalFaceSet->m_PnIndex;					//optional

			if( polygonalFaceSet->m_Closed )
			{
				bool success = false;
				if( polygonalFaceSet->m_Closed->m_value == true )
				{
					success = item_data->addClosedPolyhedron(polyCache.m_poly_data, params);
				}
				else
				{
					item_data->addOpenPolyhedron(polyCache.m_poly_data, params);
				}

#ifdef _DEBUG
				if (!success)
				{
					vec4 color(0.3, 0.4, 0.5, 1.0);
					GeomDebugDump::moveOffset(1);
					GeomDebugDump::dumpLocalCoordinateSystem();
					for( auto face : vecFaces )
					{
						PolyInputCache3D polyCacheCurrentFace( eps );
						convertIndexedPolygonalFace(face, pointVec, polyCacheCurrentFace);

						shared_ptr<carve::mesh::MeshSet<3> > meshset(polyCacheCurrentFace.m_poly_data->createMesh(carve::input::opts(), eps));
						GeomDebugDump::dumpMeshset(meshset.get(), color, false, false);
					}
				}
#endif
			}
			else
			{
				item_data->addOpenOrClosedPolyhedron(polyCache.m_poly_data, params);
			}

			// IfcTessellatedFaceSet --  attributes:
			//  shared_ptr<IfcCartesianPointList3D>						m_Coordinates;
			// inverse attributes:
			//  std::vector<weak_ptr<IfcIndexedColourMap> >				m_HasColours_inverse;
			//  std::vector<weak_ptr<IfcIndexedTextureMap> >			m_HasTextures_inverse;
			// IfcPolygonalFaceSet -- attributes:
			//shared_ptr<IfcBoolean>									m_Closed;					//optional
			//std::vector<shared_ptr<IfcIndexedPolygonalFace> >		m_Faces;
			//std::vector<shared_ptr<IfcPositiveInteger> >			m_PnIndex;					//optional
			int refCount1 = polygonalFaceSet.use_count();
			for (auto colorMapWeak : polygonalFaceSet->m_HasColours_inverse) {
				if (colorMapWeak.expired()) {
					continue;
				}
				std::lock_guard<std::mutex> lock(m_mutex1);
				shared_ptr<IfcIndexedColourMap> colorMap(colorMapWeak);
				colorMap->m_MappedTo.reset();  // temporary, to release memory
			}
			
#ifdef _DEBUG
			int refCount2 = polygonalFaceSet.use_count();
			int tag = polygonalFaceSet->m_tag;
#endif
			
			//std::lock_guard<std::mutex> lock(m_mutex2);
			//polygonalFaceSet->m_PnIndex.clear(); // clear memory, temporary solution
			//polygonalFaceSet->m_Faces.clear(); // clear memory, temporary solution
			//polygonalFaceSet->m_Coordinates.reset(); // clear memory, temporary solution
			return;
		}

		shared_ptr<IfcTriangulatedFaceSet> triangulatedFaceSet = dynamic_pointer_cast<IfcTriangulatedFaceSet>(abstractFaceSet);
		if( triangulatedFaceSet )
		{
			// IfcTriangulatedFaceSet -----------------------------------------------------------
			//std::vector<std::vector<shared_ptr<IfcParameterValue> > >	m_Normals;					//optional
			//shared_ptr<IfcBoolean>									m_Closed;					//optional
			//std::vector<std::vector<shared_ptr<IfcPositiveInteger> > >	m_CoordIndex;
			//std::vector<shared_ptr<IfcPositiveInteger> >			m_PnIndex;					//optional

			std::vector<vec3> faceNormals;
			for( const std::vector<shared_ptr<IfcParameterValue> >& vecNormalParam : triangulatedFaceSet->m_Normals )
			{
				if( vecNormalParam.size() == 3 )
				{
					const shared_ptr<IfcParameterValue>& normalParamX = vecNormalParam[0];
					const shared_ptr<IfcParameterValue>& normalParamY = vecNormalParam[1];
					const shared_ptr<IfcParameterValue>& normalParamZ = vecNormalParam[2];

					if( normalParamX && normalParamY && normalParamZ )
					{
						vec3 normal = carve::geom::VECTOR(normalParamX->m_value, normalParamY->m_value, normalParamZ->m_value);
						GeomUtils::safeNormalize(normal, eps);
						faceNormals.push_back(normal);
						continue;
					}
				}
				// insert default vector, to maintain the relation to faces
				faceNormals.push_back(carve::geom::VECTOR(0,0,1));
			}

			for( size_t ii = 0; ii < triangulatedFaceSet->m_CoordIndex.size(); ++ii )
			{
				const std::vector<shared_ptr<IfcPositiveInteger> >& vecFaceLoop = triangulatedFaceSet->m_CoordIndex[ii];
				std::vector<vec3> faceOuterBound;
				for( const shared_ptr<IfcPositiveInteger>& positiveInt : vecFaceLoop )
				{
					if( !positiveInt )
					{
						continue;
					}
					size_t idx = (size_t)positiveInt->m_value - 1;  // 1 based index in IfcIndexedPolygonalFace
					if( idx >= pointVec.size() )
					{
						std::cout << "copyIndexedFaceLoop: invalid index" << std::endl;
						continue;
					}
					faceOuterBound.push_back(pointVec[idx]);
				}
				std::vector<std::vector<vec3> > face_loops;
				face_loops.push_back(faceOuterBound);

				if( ii < faceNormals.size() )
				{
					vec3 computedNormal = GeomUtils::computePolygonNormal(faceOuterBound, eps);
					vec3& normal = faceNormals[ii];
					if( dot(normal, computedNormal) < 0 )
					{
						std::reverse(faceOuterBound.begin(), faceOuterBound.end());
					}
				}

				FaceConverter::createTriangulated3DFace(face_loops, polyCache, params, false);
			}

			if( triangulatedFaceSet->m_Closed )
			{
				if( triangulatedFaceSet->m_Closed->m_value == true )
				{
					item_data->addClosedPolyhedron(polyCache.m_poly_data, params);
				}
				else
				{
					item_data->addOpenPolyhedron(polyCache.m_poly_data, params);
				}
			}
			else
			{
				item_data->addOpenOrClosedPolyhedron(polyCache.m_poly_data, params);
			}
			return;
		}
	}

	messageCallback("IfcTessellatedItem not as expected", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, tessellatedItem.get());
}

void SolidModelConverter::convertIfcBooleanOperand( const shared_ptr<IfcBooleanOperand>& operand_select, shared_ptr<ItemShapeData>& item_data, const shared_ptr<ItemShapeData>& other_operand )
{
	if (m_geom_settings->handleStyledItems())
	{
		shared_ptr<IfcRepresentationItem> representationItem = dynamic_pointer_cast<IfcRepresentationItem>(operand_select);
		if (representationItem)
		{
			std::vector<shared_ptr<StyleData> > vec_style_data;
			m_styles_converter->convertRepresentationStyle(representationItem, vec_style_data);
			for (auto& style : vec_style_data)
			{
				item_data->addStyle(style);
			}
		}
	}

	// TYPE IfcBooleanOperand = SELECT	(IfcBooleanResult	,IfcCsgPrimitive3D	,IfcHalfSpaceSolid	,IfcSolidModel	,IfcTessellatedFaceSet);
	shared_ptr<IfcSolidModel> solid_model = dynamic_pointer_cast<IfcSolidModel>( operand_select );
	if( solid_model )
	{
		convertIfcSolidModel( solid_model, item_data );  // not necessary to catch GeometryWarningException here
		return;
	}

	shared_ptr<IfcHalfSpaceSolid> half_space_solid = dynamic_pointer_cast<IfcHalfSpaceSolid>( operand_select );
	if( half_space_solid )
	{
		convertIfcHalfSpaceSolid( half_space_solid, item_data, other_operand );
		return;
	}

	shared_ptr<IfcBooleanResult> boolean_result = dynamic_pointer_cast<IfcBooleanResult>( operand_select );
	if( boolean_result )
	{
		convertIfcBooleanResult( boolean_result, item_data );
		return;
	}

	shared_ptr<IfcCsgPrimitive3D> csg_primitive3D = dynamic_pointer_cast<IfcCsgPrimitive3D>( operand_select );
	if( csg_primitive3D )
	{
		convertIfcCsgPrimitive3D( csg_primitive3D, item_data );
		return;
	}

	shared_ptr<IfcTessellatedFaceSet> tess = dynamic_pointer_cast<IfcTessellatedFaceSet>( operand_select );
	if( tess )
	{
		convertTesselatedItem(tess, item_data);
		return;
	}

	messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, operand_select.get() );
}

void interpolateSection(const std::vector<std::vector<vec2> >& profileCoordsStart, const std::vector<std::vector<vec2> >& profileCoordsEnd, double t,
	std::vector<std::vector<vec2> >& profileCoordsResult) {

	size_t numLoops = profileCoordsStart.size();
	if (numLoops != profileCoordsEnd.size())
	{
		profileCoordsResult = profileCoordsStart;
		return;
	}

	if (t > 1.0){
		t = 1.0;
	}
	else if (t < 0.0){
		t = 0.0;
	}

	for( size_t ii = 0; ii < numLoops; ++ii )
	{
		const std::vector<vec2>& loopStart = profileCoordsStart[ii];
		const std::vector<vec2>& loopEnd = profileCoordsEnd[ii];
		if( loopStart.size() != loopEnd.size() )
		{
			profileCoordsResult = profileCoordsStart;
			return;
		}
		// interpolate current loop
		std::vector<vec2> loopResult;
		for( size_t jj = 0; jj < loopStart.size(); ++jj )
		{
			const vec2& pointStart = loopStart[jj];
			const vec2& pointEnd = loopEnd[jj];
			vec2 pointResult = pointStart + t*(pointEnd - pointStart);
			loopResult.push_back(pointResult);
		}
		profileCoordsResult.push_back(loopResult);
	}
}

void rotateSectionTo3D(const std::vector<std::vector<vec2> >& profileCoords2D, const carve::math::Matrix& currentMatrix,
	std::vector<std::vector<vec3> >& profileCoords3D) {
		
	for (auto& loop2D : profileCoords2D) {
		profileCoords3D.push_back(std::vector<vec3>());
		std::vector<vec3>& loop3D = profileCoords3D.back();
		for (auto& point2D : loop2D) {
			vec3 point3D = carve::geom::VECTOR(point2D.x, point2D.y, 0);
			point3D = currentMatrix * point3D;
			loop3D.push_back(point3D);
		}
	}
}

void applyMatrix(const carve::math::Matrix& currentMatrix, std::vector<vec3>& profileCoords3D) {
	for (auto& point3D : profileCoords3D) {
		point3D = currentMatrix * point3D;
	}
}
void applyMatrix( const carve::math::Matrix& currentMatrix, std::vector<std::vector<vec3> >& profileCoords3D) {
	for (auto& loop3D : profileCoords3D) {
		for (auto& point3D : loop3D) {
			point3D = currentMatrix * point3D;
		}
	}
}
void applyTranslate(const vec3& offset, std::vector<std::vector<vec3> >& profileCoords3D) {
	for (auto& loop3D : profileCoords3D) {
		for (auto& point3D : loop3D) {
			point3D = point3D + offset;
		}
	}
}

void addPointIndices(std::vector<std::vector<std::vector<size_t> > >& pointIndices, const std::vector<std::vector<vec3> >& profileCoords3D, 
	PolyInputCache3D& polyhedron) {
	pointIndices.push_back(std::vector<std::vector<size_t> >());
	std::vector<std::vector<size_t> >& pointIndicesInCurrentSection = pointIndices.back();
	for (auto& loop3D : profileCoords3D) {
		pointIndicesInCurrentSection.push_back(std::vector<size_t>());
		std::vector<size_t>& pointIndicesInCurrentSectionLoop = pointIndicesInCurrentSection.back();
		for (auto& point3D : loop3D) {
			// add point to polyhedron. Faces will be added later from pointIndices
			int pointIndex = polyhedron.addPoint(point3D);
			pointIndicesInCurrentSectionLoop.push_back(pointIndex);
		}
	}
}

void addCurrentSection2D(const carve::math::Matrix& currentMatrix, const std::vector<std::vector<vec2> >& profileCoords2D, GeomProcessingParams& params, 
	PolyInputCache3D& polyhedron, std::vector<std::vector<std::vector<size_t> > >& pointIndices, 
	std::vector<std::vector<std::vector<vec3> > >& profileCoordsInSections,
	shared_ptr<FaceConverter>& face_converter) {
	std::vector<std::vector<vec3> > profileCoords3D;
	rotateSectionTo3D(profileCoords2D, currentMatrix, profileCoords3D);

	addPointIndices(pointIndices, profileCoords3D, polyhedron);
	profileCoordsInSections.push_back(profileCoords3D);
}

vec3 getFirstNormalVector(const std::vector<std::vector<vec2> >& profileCoords2D) {
	vec3 normal = carve::geom::VECTOR(0, 0, 1);
	if (profileCoords2D.size() > 0) {
		normal = GeomUtils::computePolygon2DNormal(profileCoords2D[0]);
	}
	return normal;
}

void SolidModelConverter::convertIfcSectionedSpine(const shared_ptr<IfcSectionedSpine>& spine, shared_ptr<ItemShapeData> item_data)
{
	const shared_ptr<IfcCompositeCurve> spineCurve = spine->m_SpineCurve;
	if (!spineCurve) {
		messageCallback("invalid IfcSectionedSpine.BaseSurface", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, spine.get());
		return;
	}
	const std::vector<shared_ptr<IfcProfileDef> >& crossSections = spine->m_CrossSections;
	const std::vector<shared_ptr<IfcAxis2Placement3D> >& crossSectionPositions = spine->m_CrossSectionPositions;

	int entityTag = spine->m_tag;
	double eps = m_geom_settings->getEpsilonMergePoints();
	const double length_in_meter = m_curve_converter->getPointConverter()->getUnitConverter()->getLengthInMeterFactor();
	std::vector<vec3> curve_polygon;
	std::vector<CurveConverter::CurveSegment> segments;
	m_curve_converter->convertIfcCurve(spineCurve, segments, true);
	GeomProcessingParams params(m_geom_settings, spine.get(), this);
		
	if (crossSections.size() == 0) {
		messageCallback("invalid IfcSectionedSpine.CrossSections", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, spine.get());
		return;
	}
	
	std::vector<std::vector<std::vector<size_t> > > pointIndices;
	std::vector<std::vector<std::vector<vec3> > > profileCoordsInSections;
	PolyInputCache3D poly(eps);

	for (size_t ii = 0; ii < segments.size(); ++ii){
		CurveConverter::CurveSegment& curveSegment = segments[ii];
		shared_ptr<ProfileConverter> profileSectionBegin;
		carve::math::Matrix sectionBeginMatrix, sectionEndMatrix;

		if (crossSections.size() > ii){
			const shared_ptr<IfcProfileDef>& cross_section = crossSections[ii];
			profileSectionBegin = m_profile_cache->getProfileConverter(cross_section, false);
		}

		if (!profileSectionBegin) {
			messageCallback("invalid IfcSectionedSpine.CrossSection[0]", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, spine.get());
			return;
		}

		if (crossSectionPositions.size() > ii) {
			const shared_ptr<IfcAxis2Placement3D>& section_position = crossSectionPositions[ii];
			shared_ptr<TransformData> sectionTransform;
			m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D(section_position, sectionTransform);
			sectionBeginMatrix = sectionTransform->m_matrix;
		}
		if (crossSectionPositions.size() > ii+1) {
			const shared_ptr<IfcAxis2Placement3D>& section_position = crossSectionPositions[ii+1];
			shared_ptr<TransformData> sectionTransform;
			m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D(section_position, sectionTransform);
			sectionEndMatrix = sectionTransform->m_matrix;
		}

		shared_ptr<ProfileConverter> profileSectionEnd = profileSectionBegin;
		if (crossSections.size() > ii+1) {
			const shared_ptr<IfcProfileDef>& cross_section = crossSections[ii + 1];
			profileSectionEnd = m_profile_cache->getProfileConverter(cross_section, false);
		}

		const std::vector<std::vector<vec2> >& profileCoords2DSectionBegin = profileSectionBegin->getCoordinates();
		const std::vector<std::vector<vec2> >& profileCoords2DSectionEnd = profileSectionEnd->getCoordinates();
		bool isFirstSection = ii == 0;
		bool isLastSection = ii == segments.size() - 1;

		if (curveSegment.m_type == CurveConverter::CurveSegment::CURVE_TYPE_ARC || curveSegment.m_type == CurveConverter::CurveSegment::CURVE_TYPE_CIRCLE) {
			if (curveSegment.m_points.size() > 0) {
				const vec3& circleCenter = curveSegment.arcOrCircleCenter;
				vec3 rotationAxis = sectionBeginMatrix * carve::geom::VECTOR(0, 1, 0);
				size_t nunSubSectionsInSegment = curveSegment.m_points.size();
				double angle = curveSegment.arcStartAngle;
				double deltaAngle = curveSegment.arcOpeningAngle / double(nunSubSectionsInSegment);
#ifdef _DEBUG
				vec3 circleAxis = curveSegment.arcOrCircleAxis;
				vec3 center_p0 = curveSegment.m_points[0] - circleCenter;
				vec3 origin = carve::geom::VECTOR(0, 0, 0);
				vec4 color(0.5, 0.5, 0.5, 1);
				vec4 color_yellow(0.99, 0.95, 0.01, 1);
				vec4 color_blue(0.1, 0.1, 0.95, 1);
				vec4 color_orange_transparent(0.9, 0.7, 0.1, 0.5);
				GeomDebugDump::dumpCoordinateSystem(sectionBeginMatrix, 1, false);
				GeomDebugDump::dumpPolyline({ curveSegment.m_points.front(), circleCenter, curveSegment.m_points.back() }, color_orange_transparent, 2.5f, false, false);
				GeomDebugDump::dumpPolyline(curveSegment.m_points, color, 1.f, false, false);
				GeomDebugDump::dumpPolyline({ origin, circleCenter }, color_yellow, 1.f, false, false);
				GeomDebugDump::dumpPolyline({ circleCenter, circleCenter + rotationAxis * 0.2 }, color_yellow, 3.f, false, false);

				for (size_t jj = 0; jj < nunSubSectionsInSegment; ++jj) {
					carve::math::Matrix m1 = carve::math::Matrix::ROT(-angle, rotationAxis, eps);
					vec3 arcPoint = -circleCenter;
					arcPoint = m1 * arcPoint;
					arcPoint += circleCenter;
					GeomDebugDump::dumpPolyline({ circleCenter, arcPoint }, color_yellow, 1.f, false, false);
					angle += deltaAngle;
				}
#endif

				angle = curveSegment.arcStartAngle;
				size_t nunSubSectionsToAdd = nunSubSectionsInSegment;
				if (isLastSection) {
					// if not last section, the last point of the curve is omitted
					nunSubSectionsToAdd = nunSubSectionsInSegment + 1;
				}

				for (size_t jj = 0; jj < nunSubSectionsToAdd; ++jj) {
					// cross section is given in x/y plane, rotate around y axis
					carve::math::Matrix mRotate = carve::math::Matrix::ROT(-angle, rotationAxis, eps);

					std::vector<std::vector<vec2> > profileCoords2D;
					double t = double(jj) / double(nunSubSectionsInSegment);
					interpolateSection(profileCoords2DSectionBegin, profileCoords2DSectionEnd, t, profileCoords2D);

					std::vector<std::vector<vec3> > profileCoords3D;
					carve::math::Matrix sectionPlacementMatrix = sectionBeginMatrix;
					rotateSectionTo3D(profileCoords2D, sectionPlacementMatrix, profileCoords3D);
					applyTranslate(-circleCenter, profileCoords3D);
					applyMatrix(mRotate, profileCoords3D);
					applyTranslate(circleCenter, profileCoords3D);

					addPointIndices(pointIndices, profileCoords3D, poly);
					profileCoordsInSections.push_back(profileCoords3D);

#ifdef _DEBUG
					PolyInputCache3D polyDebug(eps);
					std::vector<std::vector<std::vector<size_t> > > pointIndicesDebug;
					addPointIndices(pointIndicesDebug, profileCoords3D, polyDebug);
					addFaceIndices(pointIndicesDebug, polyDebug, spine);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(polyDebug.m_poly_data->createMesh(carve::input::opts(), eps));
					GeomDebugDump::dumpMeshset(meshset, color, false, false);
					GeomDebugDump::dumpPolyline(profileCoords3D, color, 1.f, false, false);

					if (curveSegment.m_points.size() > jj) {
						vec3 p_rotated = mRotate * p_rotated + circleCenter;
						vec3 deltaToCurvePoint = curveSegment.m_points[jj] - p_rotated;
						double distance = deltaToCurvePoint.length();
						if (distance > 0.1) {
							std::cout << "distance to curve point: " << distance << std::endl;
						}
					}
#endif
					angle += deltaAngle;
				}
				continue;
			}
		}

#ifdef _DEBUG
		PolyInputCache3D polyDebug(eps);
		std::vector<std::vector<vec3> > profileCoords3D_debug;
		rotateSectionTo3D(profileCoords2DSectionBegin, sectionBeginMatrix, profileCoords3D_debug);
		std::vector<std::vector<std::vector<size_t> > > pointIndicesDebug;
		addPointIndices(pointIndicesDebug, profileCoords3D_debug, polyDebug);

		shared_ptr<carve::mesh::MeshSet<3> > meshset(polyDebug.m_poly_data->createMesh(carve::input::opts(), eps));
		vec4 color(0.5, 0.5, 0.5, 1);
		GeomDebugDump::dumpMeshset(meshset, color, false, false);
		GeomDebugDump::dumpPolyline(profileCoords3D_debug, color, 1.f, false, false);
#endif

		addCurrentSection2D(sectionBeginMatrix, profileCoords2DSectionBegin, params, poly, pointIndices, profileCoordsInSections, m_face_converter);

		if (isLastSection) {
			// last section
			std::vector<std::vector<vec3> > profileCoords3D;
			rotateSectionTo3D(profileCoords2DSectionEnd, sectionEndMatrix, profileCoords3D);
			addPointIndices(pointIndices, profileCoords3D, poly);
			profileCoordsInSections.push_back(profileCoords3D);
#ifdef _DEBUG
			PolyInputCache3D polyDebug(eps);
			std::vector<std::vector<std::vector<size_t> > > pointIndicesDebug;
			addPointIndices(pointIndicesDebug, profileCoords3D, polyDebug);
			addFaceIndices(pointIndicesDebug, polyDebug, spine);
			shared_ptr<carve::mesh::MeshSet<3> > meshset(polyDebug.m_poly_data->createMesh(carve::input::opts(), eps));
			vec4 color(0.5, 0.5, 0.5, 1);
			GeomDebugDump::dumpMeshset(meshset, color, false, false);
			GeomDebugDump::dumpPolyline(profileCoords3D, color, 1.f, false, false);
#endif
			continue;
		}
	}

	// add faces between the sections
	addFaceIndices( pointIndices,  poly, spine);

	// add front and back cap after all sections are added
	if(profileCoordsInSections.size() > 0) {
		std::vector<std::vector<vec3> >& firstSectionProfileCoords = profileCoordsInSections.front();
		if (firstSectionProfileCoords.size() > 0) {
			std::vector<vec3>& firstLoopPoints = firstSectionProfileCoords.front();

			vec3 centroidFirstLoop = GeomUtils::computePolygonCentroid(firstLoopPoints);
			vec3 normalFirstLoop = GeomUtils::computePolygonNormal(firstLoopPoints, eps);
			bool flipFirstFace = dot(normalFirstLoop, carve::geom::VECTOR(0, 0, 1)) > 0;

			if (profileCoordsInSections.size() > 1) {
				std::vector<std::vector<vec3> >& secondSectionProfileCoords = profileCoordsInSections[1];
				if (secondSectionProfileCoords.size() > 0) {
					std::vector<vec3>& secondLoopPoints = secondSectionProfileCoords.front();

					vec3 centroidSecondLoop = GeomUtils::computePolygonCentroid(secondLoopPoints);
					vec3 extrusionDirection = centroidSecondLoop - centroidFirstLoop;
					if (extrusionDirection.length2() > eps * eps) {
						extrusionDirection.normalize();
						flipFirstFace = dot(normalFirstLoop, extrusionDirection) > 0;
					}
				}

				// add faces for last loop
				std::vector<std::vector<vec3> >& lastSectionProfileCoords = profileCoordsInSections.back();
				PolyInputCache3D polyhedronCurrentFace(eps);
				bool flipLastFace = !flipFirstFace;
				m_face_converter->createTriangulated3DFace(lastSectionProfileCoords, polyhedronCurrentFace, params, flipLastFace);
				poly.copyOtherPolyData(polyhedronCurrentFace.m_poly_data);

#ifdef _DEBUG
				if (spine->m_tag == 291617) {
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(carve::input::opts(), 1e-8));
					vec4 color(0.5, 0.5, 0.5, 1);
					GeomDebugDump::dumpMeshset(meshset, color, false, false);
				}
#endif
			}

			// add faces for first loop
			PolyInputCache3D polyhedronCurrentFace(eps);
			m_face_converter->createTriangulated3DFace(firstSectionProfileCoords, polyhedronCurrentFace, params, flipFirstFace);
			poly.copyOtherPolyData(polyhedronCurrentFace.m_poly_data);

#ifdef _DEBUG
			if (spine->m_tag == 291617) {
				shared_ptr<carve::mesh::MeshSet<3> > meshset(poly.m_poly_data->createMesh(carve::input::opts(), 1e-8));
				vec4 color(0.5, 0.5, 0.5, 1);
				GeomDebugDump::dumpMeshset(meshset, color, false, false);
			}
#endif
		}
	}

	shared_ptr<ItemShapeData> solidItem(new ItemShapeData());
	solidItem->addClosedPolyhedron(poly.m_poly_data, params );
	item_data->addGeometricChildItem(solidItem, item_data);

#ifdef _DEBUG
	vec4 color(0.5, 0.5, 0.5, 1);
	GeomDebugDump::dumpItemShapeInputData(item_data, color);
	GeomDebugDump::moveOffset(1);
#endif
}

void SolidModelConverter::addFaceIndices(std::vector<std::vector<std::vector<size_t> > > pointIndices, PolyInputCache3D& poly, const shared_ptr<BuildingEntity>& entity) {
	if (pointIndices.size() > 1) {
		for (size_t ii = 1; ii < pointIndices.size(); ++ii) {
			const std::vector<std::vector<size_t> >& pointIndicesInPreviousSection = pointIndices[ii - 1];
			const std::vector<std::vector<size_t> >& pointIndicesInCurrentSection = pointIndices[ii];
			if (pointIndicesInPreviousSection.size() != pointIndicesInCurrentSection.size()) {
				messageCallback("IfcSectionedSpine: pointIndicesInPreviousSection.size() != pointIndicesInCurrentSection.size()", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, entity.get());
				continue;
			}

			for (size_t jj = 0; jj < pointIndicesInPreviousSection.size(); ++jj) {
				const std::vector<size_t>& pointIndicesInPreviousLoop = pointIndicesInPreviousSection[jj];
				const std::vector<size_t>& pointIndicesInCurrentLoop = pointIndicesInCurrentSection[jj];
				if (pointIndicesInPreviousLoop.size() != pointIndicesInCurrentLoop.size()) {
					std::cout << entity->m_tag << std::endl;
					messageCallback("IfcSectionedSpine: pointIndicesInPreviousLoop.size() != pointIndicesInCurrentLoop.size()", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, entity.get());
					continue;
				}
//#ifdef _DEBUG
//				PolyInputCache3D polyDebug(1e-8);
//				std::vector<std::vector<std::vector<size_t> > > pointIndicesDebug;
//				std::vector<vec3> loop1;
//				std::vector<vec3> loop2;
//#endif

				for (size_t kk = 0; kk < pointIndicesInPreviousLoop.size(); ++kk) {
					size_t indexNextInLoop = (kk + 1) % pointIndicesInPreviousLoop.size();
					const size_t& pointIndexPreviousLoop = pointIndicesInPreviousLoop[kk];
					const size_t& pointIndexCurrentLoop = pointIndicesInCurrentLoop[kk];
					const size_t& pointIndexCurrentLoopNext = pointIndicesInCurrentLoop[indexNextInLoop];
					const size_t& pointIndexPreviousLoopNext = pointIndicesInPreviousLoop[indexNextInLoop];
					if (jj == 0) {
						poly.m_poly_data->addFace(pointIndexCurrentLoop, pointIndexPreviousLoop, pointIndexPreviousLoopNext, pointIndexCurrentLoopNext);
					}
					else {
						poly.m_poly_data->addFace(pointIndexCurrentLoop, pointIndexCurrentLoopNext , pointIndexPreviousLoopNext, pointIndexPreviousLoop);
					}

//#ifdef _DEBUG
//					vec3 p0p = poly.m_poly_data->getVertex(pointIndexPreviousLoop);
//					vec3 p1c = poly.m_poly_data->getVertex(pointIndexCurrentLoop);
//					vec3 p2c = poly.m_poly_data->getVertex(pointIndexCurrentLoopNext);
//					vec3 p3p = poly.m_poly_data->getVertex(pointIndexPreviousLoopNext);
//					int idx0p = polyDebug.addPoint(p0p);
//					int idx1c = polyDebug.addPoint(p1c);
//					int idx2c = polyDebug.addPoint(p2c);
//					int idx3p = polyDebug.addPoint(p3p);
//					polyDebug.m_poly_data->addFace(idx1c, idx0p, idx3p, idx2c);
//					loop1.push_back(p0p);
//					loop1.push_back(p3p);
//					loop2.push_back(p1c);
//					loop2.push_back(p2c);
//#endif
				}
//#ifdef _DEBUG
//				shared_ptr<carve::mesh::MeshSet<3> > meshset(polyDebug.m_poly_data->createMesh(carve::input::opts(), 1e-8));
//				vec4 color(0.5, 0.5, 0.5, 1);
//				GeomDebugDump::dumpMeshset(meshset, color, false, false);
//				GeomDebugDump::dumpPolyline(loop1, color, 1.f, false, false);
//				GeomDebugDump::dumpPolyline(loop2, color, 1.f, false, false);
//#endif
			}
		}
	}
}

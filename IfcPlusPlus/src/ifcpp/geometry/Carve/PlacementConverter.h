/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
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

#pragma once

#include <set>
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/IFC4/include/IfcAxis1Placement.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement2D.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcCartesianTransformationOperator.h>
#include <ifcpp/IFC4/include/IfcCartesianTransformationOperator2DnonUniform.h>
#include <ifcpp/IFC4/include/IfcCartesianTransformationOperator3DnonUniform.h>
#include <ifcpp/IFC4/include/IfcDimensionCount.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationContext.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationSubContext.h>
#include <ifcpp/IFC4/include/IfcGridAxis.h>
#include <ifcpp/IFC4/include/IfcGridPlacement.h>
#include <ifcpp/IFC4/include/IfcLocalPlacement.h>
#include <ifcpp/IFC4/include/IfcObjectPlacement.h>
#include <ifcpp/IFC4/include/IfcPlacement.h>
#include <ifcpp/IFC4/include/IfcRepresentationContext.h>
#include <ifcpp/IFC4/include/IfcReal.h>
#include <ifcpp/IFC4/include/IfcVirtualGridIntersection.h>
#include "GeomUtils.h"
#include "IncludeCarveHeaders.h"

namespace PlacementConverter
{
	inline void convertIfcAxis2Placement2D( const shared_ptr<IfcAxis2Placement2D>& axis2placement2d, const double length_factor, carve::math::Matrix& resulting_matrix, bool only_rotation = false )
	{
		vec3  translate( carve::geom::VECTOR( 0.0, 0.0, 0.0 ) );
		vec3  local_x( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );
		vec3  local_y( carve::geom::VECTOR( 0.0, 1.0, 0.0 ) );
		vec3  local_z( carve::geom::VECTOR( 0.0, 0.0, 1.0 ) );
		vec3  ref_direction( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );

		if( !only_rotation )
		{
			if( axis2placement2d->m_Location )
			{
				std::vector<shared_ptr<IfcLengthMeasure> >& coords = axis2placement2d->m_Location->m_Coordinates;
				if( coords.size() > 1 )
				{
					translate = carve::geom::VECTOR( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 );
				}
			}
		}

		if( axis2placement2d->m_RefDirection )
		{
			if( axis2placement2d->m_RefDirection->m_DirectionRatios.size() > 1 )
			{
				if( axis2placement2d->m_RefDirection->m_DirectionRatios[0] )
				{
					ref_direction.x = axis2placement2d->m_RefDirection->m_DirectionRatios[0]->m_value;
				}
				if( axis2placement2d->m_RefDirection->m_DirectionRatios[1] )
				{
					ref_direction.y = axis2placement2d->m_RefDirection->m_DirectionRatios[1]->m_value;
				}
				ref_direction.z = 0;
			}
		}

		local_x = ref_direction;
		vec3  z_axis( carve::geom::VECTOR( 0.0, 0.0, 1.0 ) );
		local_y = carve::geom::cross( z_axis, local_x );
		// ref_direction can be just in the x-z-plane, not perpendicular to y and z. so re-compute local x
		local_x = carve::geom::cross( local_y, local_z );

		local_x.normalize();
		local_y.normalize();
		local_z.normalize();

		resulting_matrix = carve::math::Matrix(
			local_x.x, local_y.x, local_z.x, translate.x,
			local_x.y, local_y.y, local_z.y, translate.y,
			local_x.z, local_y.z, local_z.z, translate.z,
			0, 0, 0, 1 );
	}

	inline void convertIfcAxis2Placement3D( const shared_ptr<IfcAxis2Placement3D>& axis2placement3d, const double length_factor, carve::math::Matrix& resulting_matrix, bool only_rotation = false )
	{
		vec3  translate( carve::geom::VECTOR( 0.0, 0.0, 0.0 ) );
		vec3  local_x( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );
		vec3  local_y( carve::geom::VECTOR( 0.0, 1.0, 0.0 ) );
		vec3  local_z( carve::geom::VECTOR( 0.0, 0.0, 1.0 ) );
		vec3  ref_direction( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );

		if( !only_rotation )
		{
			if( axis2placement3d->m_Location )
			{
				std::vector<shared_ptr<IfcLengthMeasure> >& coords = axis2placement3d->m_Location->m_Coordinates;
				if( coords.size() > 2 )
				{
					translate = carve::geom::VECTOR( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, coords[2]->m_value*length_factor );
				}
				else if( coords.size() > 1 )
				{
					translate = carve::geom::VECTOR( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 );
				}
			}
		}

		if( axis2placement3d->m_Axis )
		{
			// local z-axis
			std::vector<shared_ptr<IfcReal> >& axis = axis2placement3d->m_Axis->m_DirectionRatios;
			if( axis.size() > 2 )
			{
				local_z = carve::geom::VECTOR( axis[0]->m_value, axis[1]->m_value, axis[2]->m_value );
			}
		}

		if( axis2placement3d->m_RefDirection )
		{
			if( axis2placement3d->m_RefDirection->m_DirectionRatios.size() > 2 )
			{
				ref_direction.x = axis2placement3d->m_RefDirection->m_DirectionRatios[0]->m_value;
				ref_direction.y = axis2placement3d->m_RefDirection->m_DirectionRatios[1]->m_value;
				ref_direction.z = axis2placement3d->m_RefDirection->m_DirectionRatios[2]->m_value;
			}
		}

		local_x = ref_direction;
		local_y = carve::geom::cross( local_z, local_x );
		// ref_direction can be just in the x-z-plane, not perpendicular to y and z. so re-compute local x
		local_x = carve::geom::cross( local_y, local_z );

		local_x.normalize();
		local_y.normalize();
		local_z.normalize();

		resulting_matrix = carve::math::Matrix(
			local_x.x, local_y.x, local_z.x, translate.x,
			local_x.y, local_y.y, local_z.y, translate.y,
			local_x.z, local_y.z, local_z.z, translate.z,
			0, 0, 0, 1 );
	}

	inline void getPlane( const shared_ptr<IfcAxis2Placement3D>& axis2placement3d, const double length_factor, carve::geom::plane<3>& plane, vec3& translate )
	{
		vec3  location( carve::geom::VECTOR( 0.0, 0.0, 0.0 ) );
		vec3  local_x( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );
		vec3  local_y( carve::geom::VECTOR( 0.0, 1.0, 0.0 ) );
		vec3  local_z( carve::geom::VECTOR( 0.0, 0.0, 1.0 ) );
		vec3  ref_direction( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );

		if( axis2placement3d->m_Location )
		{
			std::vector<shared_ptr<IfcLengthMeasure> >& coords = axis2placement3d->m_Location->m_Coordinates;
			if( coords.size() > 2 )
			{
				location = carve::geom::VECTOR( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, coords[2]->m_value*length_factor );
			}
			else if( coords.size() > 1 )
			{
				location = carve::geom::VECTOR( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 );
			}
		}

		if( axis2placement3d->m_Axis )
		{
			// local z-axis
			std::vector<shared_ptr<IfcReal> >& axis = axis2placement3d->m_Axis->m_DirectionRatios;
			if( axis.size() > 2 )
			{
				local_z = carve::geom::VECTOR( axis[0]->m_value, axis[1]->m_value, axis[2]->m_value );
			}
		}
		local_z.normalize();

		carve::geom::plane<3> p( local_z, location );
		plane.d = p.d;
		plane.N = local_z;
		translate = location;
	}

	inline void convertMatrix( const carve::math::Matrix& matrix, const double length_factor, shared_ptr<IfcAxis2Placement3D>& axis2placement3d, int& entity_id, std::vector<shared_ptr<IfcPPEntity> >& vec_new_entities )
	{
		if( !axis2placement3d )
		{
			axis2placement3d = shared_ptr<IfcAxis2Placement3D>( new IfcAxis2Placement3D() );
			if( entity_id > 0 )
			{
				axis2placement3d->m_id = entity_id++;
			}
			vec_new_entities.push_back( axis2placement3d );
		}

		vec3  local_x( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );
		vec3  local_y( carve::geom::VECTOR( 0.0, 1.0, 0.0 ) );
		vec3  local_z( carve::geom::VECTOR( 0.0, 0.0, 1.0 ) );
		vec3  translate( carve::geom::VECTOR( 0.0, 0.0, 0.0 ) );

		local_x.x = matrix._11;//(0,0);
		local_x.y = matrix._12;//(0,1);
		local_x.z = matrix._13;//(0,2);

		local_y.x = matrix._21;//(1,0);
		local_y.y = matrix._22;//(1,1);
		local_y.z = matrix._23;//(1,2);

		local_z.x = matrix._31;//(2,0);
		local_z.y = matrix._32;//(2,1);
		local_z.z = matrix._33;//(2,2);

		translate.x = matrix._41;//(3,0);
		translate.y = matrix._42;//(3,1);
		translate.z = matrix._43;//(3,2);

		local_x.normalize();
		local_y.normalize();
		local_z.normalize();

		if( !axis2placement3d->m_Location )
		{
			axis2placement3d->m_Location = shared_ptr<IfcCartesianPoint>( new IfcCartesianPoint() );
			if( entity_id > 0 )
			{
				axis2placement3d->m_Location->m_id = entity_id++;
			}
			vec_new_entities.push_back( axis2placement3d->m_Location );
		}
		axis2placement3d->m_Location->m_Coordinates.clear();
		axis2placement3d->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( translate.x / length_factor ) ) );
		axis2placement3d->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( translate.y / length_factor ) ) );
		axis2placement3d->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( translate.z / length_factor ) ) );

		if( !axis2placement3d->m_Axis )
		{
			axis2placement3d->m_Axis = shared_ptr<IfcDirection>( new IfcDirection() );
			if( entity_id > 0 )
			{
				axis2placement3d->m_Axis->m_id = entity_id++;
			}
			vec_new_entities.push_back( axis2placement3d->m_Axis );
		}
		axis2placement3d->m_Axis->m_DirectionRatios.clear();
		axis2placement3d->m_Axis->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( local_z.x ) ) );
		axis2placement3d->m_Axis->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( local_z.y ) ) );
		axis2placement3d->m_Axis->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( local_z.z ) ) );

		if( !axis2placement3d->m_RefDirection )
		{
			axis2placement3d->m_RefDirection = shared_ptr<IfcDirection>( new IfcDirection() );
			if( entity_id > 0 )
			{
				axis2placement3d->m_RefDirection->m_id = entity_id++;
			}
			vec_new_entities.push_back( axis2placement3d->m_RefDirection );
		}

		axis2placement3d->m_RefDirection->m_DirectionRatios.clear();
		axis2placement3d->m_RefDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( local_x.x ) ) );
		axis2placement3d->m_RefDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( local_x.y ) ) );
		axis2placement3d->m_RefDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( local_x.z ) ) );
	}

	inline void convertIfcPlacement( const shared_ptr<IfcPlacement>& placement, const double length_factor, carve::math::Matrix& resulting_matrix, StatusCallback* sc, bool only_rotation = false )
	{
		if( dynamic_pointer_cast<IfcAxis1Placement>( placement ) )
		{
			sc->messageCallback( "IfcAxis1Placement not implemented", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, placement.get() );
			return;
		}
		else if( dynamic_pointer_cast<IfcAxis2Placement2D>( placement ) )
		{
			shared_ptr<IfcAxis2Placement2D> axis2placement2d = dynamic_pointer_cast<IfcAxis2Placement2D>( placement );
			convertIfcAxis2Placement2D( axis2placement2d, length_factor, resulting_matrix, only_rotation );
		}
		else if( dynamic_pointer_cast<IfcAxis2Placement3D>( placement ) )
		{
			shared_ptr<IfcAxis2Placement3D> axis2placement3d = dynamic_pointer_cast<IfcAxis2Placement3D>( placement );
			convertIfcAxis2Placement3D( axis2placement3d, length_factor, resulting_matrix, only_rotation );
		}
		else
		{
			sc->messageCallback( "IfcPlacement is not IfcAxis2Placement2D or IfcAxis2Placement3D", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, placement.get() );
		}
	}

	inline void getWorldCoordinateSystem( const shared_ptr<IfcRepresentationContext>& context, const double length_factor,
		carve::math::Matrix& resulting_matrix, std::unordered_set<IfcRepresentationContext*>& already_applied )
	{
		if( !context )
		{
			return;
		}

		shared_ptr<IfcGeometricRepresentationContext> geom_context = dynamic_pointer_cast<IfcGeometricRepresentationContext>( context );
		if( !geom_context )
		{
			return;
		}

		// prevent cyclic relative placement
		IfcRepresentationContext* context_ptr = context.get();
		if( already_applied.find( context_ptr ) != already_applied.end() )
		{
			return;
		}
		already_applied.insert( context_ptr );

		//shared_ptr<IfcDimensionCount>& dim_count = geom_context->m_CoordinateSpaceDimension;
		//boost::optional<double>&		precision = geom_context->m_Precision;				//optional
		shared_ptr<IfcAxis2Placement>& world_coords_select = geom_context->m_WorldCoordinateSystem;
		//shared_ptr<IfcDirection>& true_north = geom_context->m_TrueNorth;				//optional
		// inverse attributes: std::vector<weak_ptr<IfcGeometricRepresentationSubContext> >	m_HasSubContexts_inverse;

		carve::math::Matrix world_coords_matrix( carve::math::Matrix::IDENT() );
		shared_ptr<IfcAxis2Placement3D> world_coords_3d = dynamic_pointer_cast<IfcAxis2Placement3D>( world_coords_select );
		if( world_coords_3d )
		{
			convertIfcAxis2Placement3D( world_coords_3d, length_factor, world_coords_matrix );
		}

		resulting_matrix = resulting_matrix*world_coords_matrix;

		shared_ptr<IfcGeometricRepresentationSubContext> geom_sub_context = dynamic_pointer_cast<IfcGeometricRepresentationSubContext>( geom_context );
		if( geom_sub_context )
		{
			shared_ptr<IfcGeometricRepresentationContext>& parent_context = geom_sub_context->m_ParentContext;
			//shared_ptr<IfcPositiveRatioMeasure>& target_scale = geom_sub_context->m_TargetScale;				//optional
			//shared_ptr<IfcGeometricProjectionEnum>& target_view = geom_sub_context->m_TargetView;
			//shared_ptr<IfcLabel>& user_target_view = geom_sub_context->m_UserDefinedTargetView;	//optional

			if( parent_context )
			{
				getWorldCoordinateSystem( parent_context, length_factor, resulting_matrix, already_applied );
			}
		}
	}

	//\brief translates an IfcObjectPlacement (or subtype) to carve Matrix
	inline void convertIfcObjectPlacement( const shared_ptr<IfcObjectPlacement>& ifc_object_placement,
		const double length_factor,
		carve::math::Matrix& resulting_matrix,
		StatusCallback* sc,
		std::unordered_set<IfcObjectPlacement*>& placement_already_applied,
		bool only_rotation = false )
	{
		if( !ifc_object_placement )
		{
			return;
		}
		// prevent cyclic relative placement
		IfcObjectPlacement* placement_ptr = ifc_object_placement.get();
		if( placement_already_applied.find( placement_ptr ) != placement_already_applied.end() )
		{
			return;
		}
		placement_already_applied.insert( placement_ptr );

		resulting_matrix = carve::math::Matrix::IDENT();
		shared_ptr<IfcLocalPlacement> local_placement = dynamic_pointer_cast<IfcLocalPlacement>( ifc_object_placement );
		if( local_placement )
		{
			shared_ptr<IfcAxis2Placement> relative_axis2placement_select = local_placement->m_RelativePlacement;
			if( relative_axis2placement_select )
			{
				// IfcAxis2Placement = SELECT(IfcAxis2Placement2D,IfcAxis2Placement3D)
				shared_ptr<IfcPlacement> relative_placement = dynamic_pointer_cast<IfcPlacement>( relative_axis2placement_select );
				if( relative_placement )
				{
					carve::math::Matrix relative_placement_matrix( carve::math::Matrix::IDENT() );
					convertIfcPlacement( relative_placement, length_factor, relative_placement_matrix, sc, only_rotation );
					resulting_matrix = relative_placement_matrix;
				}
				else
				{
					sc->messageCallback( "unhandled placement", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, local_placement.get() );
				}
			}

			if( local_placement->m_PlacementRelTo )
			{
				// placement is relative to other placement
				shared_ptr<IfcObjectPlacement> rel_to_placement = local_placement->m_PlacementRelTo;
				carve::math::Matrix rel_to_placement_matrix( carve::math::Matrix::IDENT() );
				convertIfcObjectPlacement( rel_to_placement, length_factor, rel_to_placement_matrix, sc, placement_already_applied, only_rotation );
				resulting_matrix = rel_to_placement_matrix*resulting_matrix;
			}
			else
			{
				// If the PlacementRelTo is not given, then the IfcProduct is placed absolutely within the world coordinate system
				//carve::math::Matrix context_matrix( carve::math::Matrix::IDENT() );
				//applyContext( context, context_matrix, length_factor, placement_already_applied );
				//object_placement = context_matrix*object_placement;
			}
		}
		else if( dynamic_pointer_cast<IfcGridPlacement>( ifc_object_placement ) )
		{
			shared_ptr<IfcGridPlacement> grid_placement = dynamic_pointer_cast<IfcGridPlacement>( ifc_object_placement );

			shared_ptr<IfcVirtualGridIntersection> grid_intersection = grid_placement->m_PlacementLocation;
			if( grid_intersection )
			{
				//std::vector<shared_ptr<IfcGridAxis> >& vec_grid_axis = grid_intersection->m_IntersectingAxes;
				//std::vector<shared_ptr<IfcLengthMeasure> >& vec_offsets = grid_intersection->m_OffsetDistances;
				// todo: implement

			}
			sc->messageCallback( "IfcGridPlacement not implemented", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, grid_placement.get() );

			//IfcGridPlacementDirectionSelect* ref_direction = grid_placement->m_PlacementRefDirection.get();	//optional
		}
	}

	inline void convertTransformationOperator( const shared_ptr<IfcCartesianTransformationOperator>& transform_operator, const double length_factor, carve::math::Matrix& resulting_matrix, StatusCallback* sc )
	{
		// ENTITY IfcCartesianTransformationOperator  ABSTRACT SUPERTYPE OF(ONEOF(IfcCartesianTransformationOperator2D, IfcCartesianTransformationOperator3D))
		vec3  translate( carve::geom::VECTOR( 0.0, 0.0, 0.0 ) );
		vec3  local_x( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );
		vec3  local_y( carve::geom::VECTOR( 0.0, 1.0, 0.0 ) );
		vec3  local_z( carve::geom::VECTOR( 0.0, 0.0, 1.0 ) );

		double scale = 1.0;
		double scale_y = 1.0;
		double scale_z = 1.0;

		shared_ptr<IfcCartesianTransformationOperator2D> trans_operator_2d = dynamic_pointer_cast<IfcCartesianTransformationOperator2D>( transform_operator );
		if( trans_operator_2d )
		{
			// ENTITY IfcCartesianTransformationOperator2D SUPERTYPE OF(IfcCartesianTransformationOperator2DnonUniform)
			if( !trans_operator_2d->m_LocalOrigin )
			{
				sc->messageCallback( "LocalOrigin not given", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_2d.get() );
				return;
			}
			if( trans_operator_2d->m_LocalOrigin->m_Coordinates.size() < 2 )
			{
				sc->messageCallback( "LocalOrigin is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_2d.get() );
				return;
			}
			double x = trans_operator_2d->m_LocalOrigin->m_Coordinates[0]->m_value*length_factor;
			double y = trans_operator_2d->m_LocalOrigin->m_Coordinates[1]->m_value*length_factor;
			translate = carve::geom::VECTOR( x, y, 0.0 );

			if( trans_operator_2d->m_Scale )
			{
				scale = trans_operator_2d->m_Scale->m_value;
			}
			scale_y = scale;
			scale_z = scale;
			if( trans_operator_2d->m_Axis1 && trans_operator_2d->m_Axis2 )
			{
				if( trans_operator_2d->m_Axis1->m_DirectionRatios.size() < 2 )
				{
					sc->messageCallback( "Axis1 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_2d.get() );
					return;
				}
				if( trans_operator_2d->m_Axis2->m_DirectionRatios.size() < 2 )
				{
					sc->messageCallback( "Axis2 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_2d.get() );
					return;
				}

				local_x.x = trans_operator_2d->m_Axis1->m_DirectionRatios[0]->m_value;
				local_x.y = trans_operator_2d->m_Axis1->m_DirectionRatios[1]->m_value;

				local_y.x = trans_operator_2d->m_Axis2->m_DirectionRatios[0]->m_value;
				local_y.y = trans_operator_2d->m_Axis2->m_DirectionRatios[1]->m_value;
			}

			shared_ptr<IfcCartesianTransformationOperator2DnonUniform> non_uniform = dynamic_pointer_cast<IfcCartesianTransformationOperator2DnonUniform>( transform_operator );
			if( non_uniform )
			{
				if( non_uniform->m_Scale2 )
				{
					scale_y = non_uniform->m_Scale2->m_value;
				}
			}
		}
		else
		{
			// ENTITY IfcCartesianTransformationOperator3D SUPERTYPE OF(IfcCartesianTransformationOperator3DnonUniform)
			shared_ptr<IfcCartesianTransformationOperator3D> trans_operator_3d = dynamic_pointer_cast<IfcCartesianTransformationOperator3D>( transform_operator );
			if( !trans_operator_3d )
			{
				sc->messageCallback( "IfcCartesianTransformationOperator is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
				return;
			}
			if( !trans_operator_3d->m_LocalOrigin )
			{
				sc->messageCallback( "LocalOrigin not given", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
				return;
			}
			if( trans_operator_3d->m_LocalOrigin->m_Coordinates.size() < 3 )
			{
				sc->messageCallback( "LocalOrigin is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
				return;
			}
			if( GeomUtils::allPointersValid( trans_operator_3d->m_LocalOrigin->m_Coordinates ) )
			{
				translate.x = trans_operator_3d->m_LocalOrigin->m_Coordinates[0]->m_value*length_factor;
				translate.y = trans_operator_3d->m_LocalOrigin->m_Coordinates[1]->m_value*length_factor;
				translate.z = trans_operator_3d->m_LocalOrigin->m_Coordinates[2]->m_value*length_factor;
			}
			if( trans_operator_3d->m_Scale )
			{
				scale = trans_operator_3d->m_Scale->m_value;
			}
			scale_y = scale;
			scale_z = scale;
			if( trans_operator_3d->m_Axis1 && trans_operator_3d->m_Axis2 && trans_operator_3d->m_Axis3 )
			{
				shared_ptr<IfcDirection> axis1 = trans_operator_3d->m_Axis1;
				shared_ptr<IfcDirection> axis2 = trans_operator_3d->m_Axis2;
				shared_ptr<IfcDirection> axis3 = trans_operator_3d->m_Axis3;
				if( axis1->m_DirectionRatios.size() < 2 )
				{
					sc->messageCallback( "Axis1 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
					return;
				}
				if( axis2->m_DirectionRatios.size() < 2 )
				{
					sc->messageCallback( "Axis2 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
					return;
				}
				if( axis3->m_DirectionRatios.size() < 2 )
				{
					sc->messageCallback( "Axis3 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
					return;
				}
				local_x.x = axis1->m_DirectionRatios[0]->m_value;
				local_x.y = axis1->m_DirectionRatios[1]->m_value;
				local_x.z = axis1->m_DirectionRatios[2]->m_value;

				local_y.x = axis2->m_DirectionRatios[0]->m_value;
				local_y.y = axis2->m_DirectionRatios[1]->m_value;
				local_y.z = axis2->m_DirectionRatios[2]->m_value;

				local_z.x = axis3->m_DirectionRatios[0]->m_value;
				local_z.y = axis3->m_DirectionRatios[1]->m_value;
				local_z.z = axis3->m_DirectionRatios[2]->m_value;
			}

			shared_ptr<IfcCartesianTransformationOperator3DnonUniform> non_uniform = dynamic_pointer_cast<IfcCartesianTransformationOperator3DnonUniform>( transform_operator );
			if( non_uniform )
			{
				if( non_uniform->m_Scale2 )
				{
					scale_y = non_uniform->m_Scale2->m_value;
				}
				if( non_uniform->m_Scale3 )
				{
					scale_z = non_uniform->m_Scale3->m_value;
				}
			}
		}
		local_x.normalize();
		local_y.normalize();
		local_z.normalize();

		carve::math::Matrix rotate_translate(
			local_x.x, local_y.x, local_z.x, translate.x,
			local_x.y, local_y.y, local_z.y, translate.y,
			local_x.z, local_y.z, local_z.z, translate.z,
			0, 0, 0, 1 );
		resulting_matrix = rotate_translate*carve::math::Matrix::SCALE( scale, scale_y, scale_z ); // scale is applied first, rotate second
	}
};

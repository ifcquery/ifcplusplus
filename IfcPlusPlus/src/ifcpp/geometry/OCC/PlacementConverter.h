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
#include <gp_Pln.hxx>

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
#include "PointConverter.h"
#include "GeomUtils.h"

namespace PlacementConverter
{
	inline void convertIfcAxis2Placement2D( const shared_ptr<IfcAxis2Placement2D>& axis2placement2d, const double length_factor, gp_Trsf& resulting_matrix, bool only_rotation = false )
	{
		gp_Vec  translate( 0.0, 0.0, 0.0 );
		gp_Vec  local_x( 1.0, 0.0, 0.0 );
		gp_Vec  local_y( 0.0, 1.0, 0.0 );
		gp_Vec  local_z( 0.0, 0.0, 1.0 );
		gp_Vec  ref_direction( 1.0, 0.0, 0.0 );

		if( !only_rotation )
		{
			if( axis2placement2d->m_Location )
			{
				std::vector<shared_ptr<IfcLengthMeasure> >& coords = axis2placement2d->m_Location->m_Coordinates;
				if( coords.size() > 1 )
				{
					translate = gp_Vec( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 );
				}
			}
		}

		if( axis2placement2d->m_RefDirection )
		{
			if( axis2placement2d->m_RefDirection->m_DirectionRatios.size() > 1 )
			{
				if( axis2placement2d->m_RefDirection->m_DirectionRatios[0] )
				{
					ref_direction.SetX( axis2placement2d->m_RefDirection->m_DirectionRatios[0]->m_value );
				}
				if( axis2placement2d->m_RefDirection->m_DirectionRatios[1] )
				{
					ref_direction.SetY( axis2placement2d->m_RefDirection->m_DirectionRatios[1]->m_value );
				}
				ref_direction.SetZ( 0 );
			}
		}

		local_x = ref_direction;
		gp_Vec  z_axis( 0.0, 0.0, 1.0 );
		local_y = z_axis.Crossed( local_x );
		// ref_direction can be just in the x-z-plane, not perpendicular to y and z. so re-compute local x
		local_x = local_y.Crossed( local_z );

		local_x.Normalize();
		local_y.Normalize();
		local_z.Normalize();

		if( translate.SquareMagnitude() < GEOM_LENGTH_EPSILON )
		{
			if( (local_x - vec3( 1.0, 0, 0 )).SquareMagnitude() < GEOM_LENGTH_EPSILON )
			{
				if( (local_y - vec3( 0, 1.0, 0 )).SquareMagnitude() < GEOM_LENGTH_EPSILON )
				{
					if( (local_z - vec3( 0, 0, 1.0 )).SquareMagnitude() < GEOM_LENGTH_EPSILON )
					{
						// set to identity
						resulting_matrix = gp_Trsf();
						return;
					}
				}
			}
		}

		resulting_matrix.SetValues(
			local_x.X(), local_y.X(), local_z.X(), translate.X(),
			local_x.Y(), local_y.Y(), local_z.Y(), translate.Y(),
			local_x.Z(), local_y.Z(), local_z.Z(), translate.Z() );
	}

	inline void convertIfcAxis2Placement3D( const shared_ptr<IfcAxis2Placement3D>& axis2placement3d, const double length_factor, 
		gp_Vec& translate, gp_Vec& local_x, gp_Vec& local_y, gp_Vec& local_z, bool only_rotation = false )
	{
		local_z = gp_Vec( 0.0, 0.0, 1.0 );
		gp_Vec  ref_direction( 1.0, 0.0, 0.0 );

		if( !only_rotation )
		{
			if( axis2placement3d->m_Location )
			{
				std::vector<shared_ptr<IfcLengthMeasure> >& coords = axis2placement3d->m_Location->m_Coordinates;
				if( coords.size() > 2 )
				{
					translate = gp_Vec( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, coords[2]->m_value*length_factor );
				}
				else if( coords.size() > 1 )
				{
					translate = gp_Vec( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 );
				}
			}
		}

		if( axis2placement3d->m_Axis )
		{
			// local z-axis
			std::vector<shared_ptr<IfcReal> >& axis = axis2placement3d->m_Axis->m_DirectionRatios;
			if( axis.size() > 2 )
			{
				local_z = gp_Vec( axis[0]->m_value, axis[1]->m_value, axis[2]->m_value );
			}
		}

		if( axis2placement3d->m_RefDirection )
		{
			if( axis2placement3d->m_RefDirection->m_DirectionRatios.size() > 2 )
			{
				ref_direction.SetX( axis2placement3d->m_RefDirection->m_DirectionRatios[0]->m_value );
				ref_direction.SetY( axis2placement3d->m_RefDirection->m_DirectionRatios[1]->m_value );
				ref_direction.SetZ( axis2placement3d->m_RefDirection->m_DirectionRatios[2]->m_value );
			}
		}

		local_x = ref_direction;
		local_y = local_z.Crossed( local_x );
		// ref_direction can be just in the x-z-plane, not perpendicular to y and z. so re-compute local x
		local_x = local_y.Crossed( local_z );

		local_x.Normalize();
		local_y.Normalize();
		local_z.Normalize();
	}
	inline void convertIfcAxis2Placement3D( const shared_ptr<IfcAxis2Placement3D>& axis2placement3d, const double length_factor, gp_Trsf& resulting_matrix, bool only_rotation = false )
	{
		gp_Vec  translate( 0.0, 0.0, 0.0 );
		gp_Vec  local_x( 1.0, 0.0, 0.0 );
		gp_Vec  local_y( 0.0, 1.0, 0.0 );
		gp_Vec  local_z( 0.0, 0.0, 1.0 );
		gp_Vec  ref_direction( 1.0, 0.0, 0.0 );

		convertIfcAxis2Placement3D( axis2placement3d, length_factor, translate, local_x, local_y, local_z, only_rotation );

		if( translate.SquareMagnitude() < GEOM_LENGTH_EPSILON )
		{
			if( (local_x - vec3( 1.0, 0, 0 )).SquareMagnitude() < GEOM_LENGTH_EPSILON )
			{
				if( (local_y - vec3( 0, 1.0, 0 )).SquareMagnitude() < GEOM_LENGTH_EPSILON )
				{
					if( (local_z - vec3( 0, 0, 1.0 )).SquareMagnitude() < GEOM_LENGTH_EPSILON )
					{
						// set to identity
						resulting_matrix = gp_Trsf();
						return;
					}
				}
			}
		}

		resulting_matrix.SetValues(
			local_x.X(), local_y.X(), local_z.X(), translate.X(),
			local_x.Y(), local_y.Y(), local_z.Y(), translate.Y(),
			local_x.Z(), local_y.Z(), local_z.Z(), translate.Z() );

	}

	inline void convertIfcAxis2Placement3D( const shared_ptr<IfcAxis2Placement3D>& axis2placement3d, const double length_factor, gp_Pln& plane )
	{
		gp_Pnt  location( 0.0, 0.0, 0.0 );
		vec3  local_z( 0.0, 0.0, 1.0 );
		vec3  ref_direction( 1.0, 0.0, 0.0 );

		if( axis2placement3d->m_Location )
		{
			std::vector<shared_ptr<IfcLengthMeasure> >& coords = axis2placement3d->m_Location->m_Coordinates;
			if( coords.size() > 2 )
			{
				location = gp_Pnt( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, coords[2]->m_value*length_factor );
			}
			else if( coords.size() > 1 )
			{
				location = gp_Pnt( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 );
			}
		}

		if( axis2placement3d->m_Axis )
		{
			// local z-axis
			std::vector<shared_ptr<IfcReal> >& axis = axis2placement3d->m_Axis->m_DirectionRatios;
			if( axis.size() > 2 )
			{
				local_z = vec3( axis[0]->m_value, axis[1]->m_value, axis[2]->m_value );
			}
		}
		local_z.Normalize();
		plane = gp_Pln( location, local_z );
	}

	inline void convertIfcPlacement( const shared_ptr<IfcPlacement>& placement, const double length_factor, gp_Trsf& resulting_matrix, StatusCallback* sc, bool only_rotation = false )
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
		gp_Trsf& resulting_matrix, std::unordered_set<IfcRepresentationContext*>& already_applied )
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

		gp_Trsf world_coords_matrix;
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

	//\brief translates an IfcObjectPlacement (or subtype) to gp_Trsf
	inline void convertIfcObjectPlacement( const shared_ptr<IfcObjectPlacement>& ifc_object_placement, const double length_factor, gp_Trsf& resulting_matrix, StatusCallback* sc,
		std::unordered_set<IfcObjectPlacement*>& placement_already_applied, bool only_rotation = false )
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

		resulting_matrix = gp_Trsf();
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
					gp_Trsf relative_placement_matrix;
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
				gp_Trsf rel_to_placement_matrix;
				convertIfcObjectPlacement( rel_to_placement, length_factor, rel_to_placement_matrix, sc, placement_already_applied, only_rotation );
				resulting_matrix = rel_to_placement_matrix*resulting_matrix;
			}
			else
			{
				// If the PlacementRelTo is not given, then the IfcProduct is placed absolutely within the world coordinate system
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

	inline void convertTransformationOperator( const shared_ptr<IfcCartesianTransformationOperator>& transform_operator, const double length_factor, gp_Trsf& resulting_matrix, StatusCallback* sc )
	{
		// ENTITY IfcCartesianTransformationOperator  ABSTRACT SUPERTYPE OF(ONEOF(IfcCartesianTransformationOperator2D, IfcCartesianTransformationOperator3D))
		vec3  translate( 0.0, 0.0, 0.0 );
		vec3  local_x( 1.0, 0.0, 0.0 );
		vec3  local_y( 0.0, 1.0, 0.0 );
		vec3  local_z( 0.0, 0.0, 1.0 );

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
			translate = vec3( x, y, 0.0 );

			if( trans_operator_2d->m_Scale )
			{
				scale = trans_operator_2d->m_Scale->m_value;
			}
			scale_y = scale;
			scale_z = scale;
			if( trans_operator_2d->m_Axis1 && trans_operator_2d->m_Axis2 )
			{
				const shared_ptr<IfcDirection>& axis1 = trans_operator_2d->m_Axis1;
				const shared_ptr<IfcDirection>& axis2 = trans_operator_2d->m_Axis2;
				if( !PointConverter::convertIfcDirection( axis1, local_x ) )
				{
					sc->messageCallback( "Axis1 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_2d.get() );
					return;
				}
				if( !PointConverter::convertIfcDirection( axis2, local_y ) )
				{
					sc->messageCallback( "Axis2 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_2d.get() );
					return;
				}
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
				translate.SetX( trans_operator_3d->m_LocalOrigin->m_Coordinates[0]->m_value*length_factor );
				translate.SetY( trans_operator_3d->m_LocalOrigin->m_Coordinates[1]->m_value*length_factor );
				translate.SetZ( trans_operator_3d->m_LocalOrigin->m_Coordinates[2]->m_value*length_factor );
			}
			if( trans_operator_3d->m_Scale )
			{
				scale = trans_operator_3d->m_Scale->m_value;
			}
			scale_y = scale;
			scale_z = scale;
			if( trans_operator_3d->m_Axis1 && trans_operator_3d->m_Axis2 && trans_operator_3d->m_Axis3 )
			{
				const shared_ptr<IfcDirection>& axis1 = trans_operator_3d->m_Axis1;
				const shared_ptr<IfcDirection>& axis2 = trans_operator_3d->m_Axis2;
				const shared_ptr<IfcDirection>& axis3 = trans_operator_3d->m_Axis3;
				if( !PointConverter::convertIfcDirection( axis1, local_x ) )
				{
					sc->messageCallback( "Axis1 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
					return;
				}
				if( !PointConverter::convertIfcDirection( axis2, local_y ) )
				{
					sc->messageCallback( "Axis2 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
					return;
				}
				if( !PointConverter::convertIfcDirection( axis3, local_z ) )
				{
					sc->messageCallback( "Axis3 is not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, trans_operator_3d.get() );
					return;
				}
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

		gp_Trsf t1, t2;
		t1.SetScale( gp_Pnt( 0, 0, 0 ), scale );
		t2.SetValues(
			local_x.X(), local_y.X(), local_z.X(), translate.X(),
			local_x.Y(), local_y.Y(), local_z.Y(), translate.Y(),
			local_x.Z(), local_y.Z(), local_z.Z(), translate.Z() );

		t2.Multiply( t1 ); // scale is applied first, rotate and translate second
		
		resulting_matrix = t2;
	}
};

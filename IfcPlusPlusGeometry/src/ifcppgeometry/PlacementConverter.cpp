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

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Matrix>

#include "ifcpp/IFC4/include/IfcPlacement.h"
#include "ifcpp/IFC4/include/IfcAxis1Placement.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement2D.h"
#include "ifcpp/IFC4/include/IfcDirection.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement3D.h"
#include "ifcpp/IFC4/include/IfcLocalPlacement.h"
#include "ifcpp/IFC4/include/IfcGridPlacement.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcCartesianPoint.h"
#include "ifcpp/IFC4/include/IfcGridAxis.h"
#include "ifcpp/IFC4/include/IfcVirtualGridIntersection.h"
#include "ifcpp/IFC4/include/IfcCartesianTransformationOperator.h"
#include "ifcpp/IFC4/include/IfcCartesianTransformationOperator2D.h"
#include "ifcpp/IFC4/include/IfcCartesianTransformationOperator2DnonUniform.h"
#include "ifcpp/IFC4/include/IfcCartesianTransformationOperator3D.h"
#include "ifcpp/IFC4/include/IfcCartesianTransformationOperator3DnonUniform.h"
#include "ifcpp/IFC4/include/IfcGeometricRepresentationContext.h"
#include "ifcpp/IFC4/include/IfcGeometricRepresentationSubContext.h"

#include "ifcpp/model/UnitConverter.h"
#include "ifcpp/model/IfcPPException.h"
#include "PlacementConverter.h"

void PlacementConverter::convertIfcAxis2Placement2D( const shared_ptr<IfcAxis2Placement2D> axis2placement2d, carve::math::Matrix& resulting_matrix, double length_factor )
{
	carve::geom::vector<3>  translate(		carve::geom::VECTOR(0.0, 0.0, 0.0) );
	carve::geom::vector<3>  local_x(		carve::geom::VECTOR(1.0, 0.0, 0.0) );
	carve::geom::vector<3>  local_y(		carve::geom::VECTOR(0.0, 1.0, 0.0) );
	carve::geom::vector<3>  local_z(		carve::geom::VECTOR(0.0, 0.0, 1.0) );
	carve::geom::vector<3>  ref_direction(	carve::geom::VECTOR(1.0, 0.0, 0.0) );

	if( axis2placement2d->m_Location )
	{
		std::vector<shared_ptr<IfcLengthMeasure> >& coords = axis2placement2d->m_Location->m_Coordinates;
		if( coords.size() > 1 )
		{
			translate = carve::geom::VECTOR( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 );
		}
	}

	if( axis2placement2d->m_RefDirection )
	{
		if( axis2placement2d->m_RefDirection->m_DirectionRatios.size() > 1 )
		{
			ref_direction.x = axis2placement2d->m_RefDirection->m_DirectionRatios[0];
			ref_direction.y = axis2placement2d->m_RefDirection->m_DirectionRatios[1];
			ref_direction.z = 0;
		}
	}

	//if( axis2placement2d->m_RefDirection )
	{
		//shared_ptr<IfcDirection>& ifc_placement_direction = axis2placement2d->m_RefDirection;
		//std::vector<double>& direction_ratios = ifc_placement_direction->m_DirectionRatios;

		//if( direction_ratios.size() > 1 )
		{
			local_x = ref_direction;//carve::geom::VECTOR(direction_ratios[0], direction_ratios[1], 0 );
			carve::geom::vector<3>  z_axis( carve::geom::VECTOR(0.0, 0.0, 1.0) );
			local_y = carve::geom::cross( z_axis, local_x );
			// ref_direction can be just in the x-z-plane, not perpendicular to y and z. so re-compute local x
			local_x = carve::geom::cross( local_y, local_z );
		}
	}
	local_x.normalize();
	local_y.normalize();
	local_z.normalize();

	resulting_matrix = carve::math::Matrix(
		local_x.x,		local_y.x,		local_z.x,	translate.x,
		local_x.y,		local_y.y,		local_z.y,	translate.y,
		local_x.z,		local_y.z,		local_z.z,	translate.z,
		0,				0,				0,			1 );
}

void PlacementConverter::convertIfcAxis2Placement3D( const shared_ptr<IfcAxis2Placement3D> axis2placement3d, carve::math::Matrix& resulting_matrix, double length_factor )
{
	carve::geom::vector<3>  translate(		carve::geom::VECTOR(0.0, 0.0, 0.0) );
	carve::geom::vector<3>  local_x(		carve::geom::VECTOR(1.0, 0.0, 0.0) );
	carve::geom::vector<3>  local_y(		carve::geom::VECTOR(0.0, 1.0, 0.0) );
	carve::geom::vector<3>  local_z(		carve::geom::VECTOR(0.0, 0.0, 1.0) );
	carve::geom::vector<3>  ref_direction(	carve::geom::VECTOR(1.0, 0.0, 0.0) );

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

	if( axis2placement3d->m_Axis )
	{
		// local z-axis
		std::vector<double>& axis = axis2placement3d->m_Axis->m_DirectionRatios;
		if( axis.size() > 2 )
		{
			local_z = carve::geom::VECTOR( axis[0], axis[1], axis[2] );
		}
	}

	if( axis2placement3d->m_RefDirection )
	{
		if( axis2placement3d->m_RefDirection->m_DirectionRatios.size() > 2 )
		{
			ref_direction.x = axis2placement3d->m_RefDirection->m_DirectionRatios[0];
			ref_direction.y = axis2placement3d->m_RefDirection->m_DirectionRatios[1];
			ref_direction.z = axis2placement3d->m_RefDirection->m_DirectionRatios[2];
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
		local_x.x,		local_y.x,		local_z.x,	translate.x,
		local_x.y,		local_y.y,		local_z.y,	translate.y,
		local_x.z,		local_y.z,		local_z.z,	translate.z,
		0,				0,				0,			1 );
}

void PlacementConverter::getPlane( const shared_ptr<IfcAxis2Placement3D> axis2placement3d, carve::geom::plane<3>& plane, carve::geom::vector<3>& translate, double length_factor )
{
	carve::geom::vector<3>  location(		carve::geom::VECTOR(0.0, 0.0, 0.0) );
	carve::geom::vector<3>  local_x(		carve::geom::VECTOR(1.0, 0.0, 0.0) );
	carve::geom::vector<3>  local_y(		carve::geom::VECTOR(0.0, 1.0, 0.0) );
	carve::geom::vector<3>  local_z(		carve::geom::VECTOR(0.0, 0.0, 1.0) );
	carve::geom::vector<3>  ref_direction(	carve::geom::VECTOR(1.0, 0.0, 0.0) );

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
		std::vector<double>& axis = axis2placement3d->m_Axis->m_DirectionRatios;
		if( axis.size() > 2 )
		{
			local_z = carve::geom::VECTOR( axis[0], axis[1], axis[2] );
		}
	}
	local_z.normalize();

	carve::geom::plane<3> p( local_z, location );
	plane.d = p.d;
	plane.N = local_z;
	translate = location;
}

void PlacementConverter::convertMatrix( const carve::math::Matrix& matrix, shared_ptr<IfcAxis2Placement3D>& axis2placement3d, double length_factor,
	int& entity_id, std::vector<shared_ptr<IfcPPEntity> >& vec_entities )
{
	throw std::exception();


	if( !axis2placement3d )
	{
		axis2placement3d = shared_ptr<IfcAxis2Placement3D>( new IfcAxis2Placement3D( entity_id++ ) );
		vec_entities.push_back( axis2placement3d );
	}

	carve::geom::vector<3>  local_x( carve::geom::VECTOR( 1.0, 0.0, 0.0 ) );
	carve::geom::vector<3>  local_y( carve::geom::VECTOR( 0.0, 1.0, 0.0 ) );
	carve::geom::vector<3>  local_z( carve::geom::VECTOR( 0.0, 0.0, 1.0 ) );
	carve::geom::vector<3>  translate( carve::geom::VECTOR( 0.0, 0.0, 0.0 ) );

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

	axis2placement3d->m_Location = shared_ptr<IfcCartesianPoint>( new IfcCartesianPoint( entity_id++ ) );
	vec_entities.push_back( axis2placement3d->m_Location );
	axis2placement3d->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( translate.x/length_factor ) ) );
	axis2placement3d->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( translate.y/length_factor ) ) );
	axis2placement3d->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( translate.z/length_factor ) ) );

	axis2placement3d->m_Axis = shared_ptr<IfcDirection>( new IfcDirection( entity_id++ ) );
	vec_entities.push_back( axis2placement3d->m_Axis );
	axis2placement3d->m_Axis->m_DirectionRatios.push_back( local_z.x );
	axis2placement3d->m_Axis->m_DirectionRatios.push_back( local_z.y );
	axis2placement3d->m_Axis->m_DirectionRatios.push_back( local_z.z );

	axis2placement3d->m_RefDirection = shared_ptr<IfcDirection>( new IfcDirection( entity_id++ ) );
	vec_entities.push_back( axis2placement3d->m_RefDirection );
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back( local_x.x );
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back( local_x.y );
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back( local_x.z );
}

void PlacementConverter::convertIfcPlacement( const shared_ptr<IfcPlacement> placement, carve::math::Matrix& resulting_matrix, double length_factor )
{
	if( dynamic_pointer_cast<IfcAxis1Placement>( placement ) )
	{
		throw IfcPPException( "IfcAxis1Placement not implemented", __func__ );
	}
	else if( dynamic_pointer_cast<IfcAxis2Placement2D>( placement ) )
	{
		shared_ptr<IfcAxis2Placement2D> axis2placement2d = dynamic_pointer_cast<IfcAxis2Placement2D>( placement );
		convertIfcAxis2Placement2D( axis2placement2d, resulting_matrix, length_factor );
	}
	else if( dynamic_pointer_cast<IfcAxis2Placement3D>( placement ) )
	{
		shared_ptr<IfcAxis2Placement3D> axis2placement3d = dynamic_pointer_cast<IfcAxis2Placement3D>( placement );
		convertIfcAxis2Placement3D( axis2placement3d, resulting_matrix, length_factor );
	}
	else
	{
		throw IfcPPException("IfcPlacement is not IfcAxis2Placement2D or IfcAxis2Placement3D", __func__);
	}
}

void PlacementConverter::getWorldCoordinateSystem( const shared_ptr<IfcRepresentationContext>& context, carve::math::Matrix& resulting_matrix,  double length_factor, std::set<int>& already_applied )
//void applyContext( const shared_ptr<IfcRepresentationContext>& context, carve::math::Matrix& resulting_matrix,  double length_factor, std::set<int>& placement_already_applied )
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
	const int placement_id = context->getId();
	if( placement_id > 0 )
	{
		if( already_applied.find(placement_id) != already_applied.end() )
		{
			return;
		}
		already_applied.insert(placement_id);
	}

	shared_ptr<IfcDimensionCount>& dim_count	= geom_context->m_CoordinateSpaceDimension;
	double							precision	= geom_context->m_Precision;				//optional
	shared_ptr<IfcAxis2Placement>& world_coords	= geom_context->m_WorldCoordinateSystem;
	shared_ptr<IfcDirection>& true_north		= geom_context->m_TrueNorth;				//optional
	// inverse attributes: std::vector<weak_ptr<IfcGeometricRepresentationSubContext> >	m_HasSubContexts_inverse;

	carve::math::Matrix world_coords_matrix( carve::math::Matrix::IDENT() );
	shared_ptr<IfcAxis2Placement3D> world_coords_3d = dynamic_pointer_cast<IfcAxis2Placement3D>( world_coords );
	if( world_coords_3d )
	{
		PlacementConverter::convertIfcAxis2Placement3D( world_coords_3d, world_coords_matrix, length_factor );
	}

	resulting_matrix = resulting_matrix*world_coords_matrix;
		
	shared_ptr<IfcGeometricRepresentationSubContext> geom_sub_context = dynamic_pointer_cast<IfcGeometricRepresentationSubContext>( geom_context );
	if( geom_sub_context )
	{
		shared_ptr<IfcGeometricRepresentationContext>& parent_context	= geom_sub_context->m_ParentContext;
		shared_ptr<IfcPositiveRatioMeasure>& target_scale				= geom_sub_context->m_TargetScale;				//optional
		shared_ptr<IfcGeometricProjectionEnum>& target_view				= geom_sub_context->m_TargetView;
		shared_ptr<IfcLabel>& user_target_view							= geom_sub_context->m_UserDefinedTargetView;	//optional

		if( parent_context )
		{
			getWorldCoordinateSystem( parent_context, resulting_matrix, length_factor, already_applied );
		}
	}
}

// @brief translates an IfcObjectPlacement (or subtype) to OpenSceneGraph Matrix
void PlacementConverter::convertIfcObjectPlacement( const shared_ptr<IfcObjectPlacement> ifc_object_placement, carve::math::Matrix& resulting_matrix,  double length_factor, std::set<int>& placement_already_applied )
{
	// prevent cyclic relative placement
	const int placement_id = ifc_object_placement->getId();
	if( placement_id > 0 )
	{
		if( placement_already_applied.find(placement_id) != placement_already_applied.end() )
		{
			return;
			//throw IfcPPException( "PlacementConverter::convertIfcObjectPlacement: detected placement cycle", __func__ );
		}
		placement_already_applied.insert(placement_id);
	}

	carve::math::Matrix object_placement( carve::math::Matrix::IDENT() );
	shared_ptr<IfcLocalPlacement> local_placement = dynamic_pointer_cast<IfcLocalPlacement>( ifc_object_placement );
	if( local_placement )
	{
		if( local_placement->m_RelativePlacement )
		{
			shared_ptr<IfcAxis2Placement> axis2placement = local_placement->m_RelativePlacement;
			// IfcAxis2Placement = SELECT(IfcAxis2Placement2D,IfcAxis2Placement3D)
			if( dynamic_pointer_cast<IfcPlacement>( axis2placement ) )
			{
				shared_ptr<IfcPlacement> placement = dynamic_pointer_cast<IfcPlacement>( axis2placement );
				carve::math::Matrix relative_placement( carve::math::Matrix::IDENT() );
				convertIfcPlacement( placement, relative_placement, length_factor );
				object_placement = relative_placement;
			}
		}

		if( local_placement->m_PlacementRelTo )
		{
			shared_ptr<IfcObjectPlacement> local_object_placement = local_placement->m_PlacementRelTo;
			// placement is relative to other placement
			carve::math::Matrix relative_placement( carve::math::Matrix::IDENT() );
			convertIfcObjectPlacement( local_object_placement, relative_placement, length_factor, placement_already_applied );
			object_placement = relative_placement*object_placement;
		}
		else
		{
			// If the PlacementRelTo is not given, then the IfcProduct is placed absolutely within the world coordinate system
			//carve::math::Matrix context_matrix( carve::math::Matrix::IDENT() );
			//applyContext( context, context_matrix, length_factor, placement_already_applied );
			//object_placement = context_matrix*object_placement;
		}
	}
	else if(  dynamic_pointer_cast<IfcGridPlacement>( ifc_object_placement ) )
	{
		shared_ptr<IfcGridPlacement> grid_placement = dynamic_pointer_cast<IfcGridPlacement>( ifc_object_placement );

		shared_ptr<IfcVirtualGridIntersection> grid_intersection = grid_placement->m_PlacementLocation;
		if( grid_intersection )
		{
			std::vector<shared_ptr<IfcGridAxis> >& vec_grid_axis = grid_intersection->m_IntersectingAxes;
			std::vector<shared_ptr<IfcLengthMeasure> >& vec_offsets = grid_intersection->m_OffsetDistances;
			// todo: implement

		}
		std::cout << "IfcGridPlacement not implemented" << std::endl;
		//IfcGridPlacementDirectionSelect* ref_direction = grid_placement->m_PlacementRefDirection.get();	//optional
	}

	resulting_matrix = object_placement;
}

void PlacementConverter::convertTransformationOperator( const shared_ptr<IfcCartesianTransformationOperator> transform_operator, carve::math::Matrix& resulting_matrix, double length_factor )
{
	// ENTITY IfcCartesianTransformationOperator  ABSTRACT SUPERTYPE OF(ONEOF(IfcCartesianTransformationOperator2D, IfcCartesianTransformationOperator3D))
	carve::geom::vector<3>  translate( carve::geom::VECTOR(0.0, 0.0, 0.0) );
	carve::geom::vector<3>  local_x( carve::geom::VECTOR(1.0, 0.0, 0.0) );
	carve::geom::vector<3>  local_y( carve::geom::VECTOR(0.0, 1.0, 0.0) );
	carve::geom::vector<3>  local_z( carve::geom::VECTOR(0.0, 0.0, 1.0) );

	double scale = 1.0;
	double scale_y = 1.0;
	double scale_z = 1.0;

	if( dynamic_pointer_cast<IfcCartesianTransformationOperator2D>(transform_operator))
	{
		// ENTITY IfcCartesianTransformationOperator2D SUPERTYPE OF(IfcCartesianTransformationOperator2DnonUniform)
		shared_ptr<IfcCartesianTransformationOperator2D> trans_operator_2d = dynamic_pointer_cast<IfcCartesianTransformationOperator2D>(transform_operator);
		if( !trans_operator_2d->m_LocalOrigin )
		{
			throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
		}
		if( trans_operator_2d->m_LocalOrigin->m_Coordinates.size() < 2 )
		{
			throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
		}
		double x = trans_operator_2d->m_LocalOrigin->m_Coordinates[0]->m_value*length_factor;
		double y = trans_operator_2d->m_LocalOrigin->m_Coordinates[1]->m_value*length_factor;
		translate = carve::geom::VECTOR( x, y, 0.0 );

		if( trans_operator_2d->m_Scale == trans_operator_2d->m_Scale )
		{
			// transOperator2D->m_Scale is not NAN
			scale = trans_operator_2d->m_Scale;
		}
		scale_y = scale;
		scale_z = scale;
		if( trans_operator_2d->m_Axis1 && trans_operator_2d->m_Axis2 )
		{
			if( trans_operator_2d->m_Axis1->m_DirectionRatios.size() < 2 )
			{
				throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
			}
			if( trans_operator_2d->m_Axis2->m_DirectionRatios.size() < 2 )
			{
				throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
			}

			local_x.x =  trans_operator_2d->m_Axis1->m_DirectionRatios[0];
			local_x.y = trans_operator_2d->m_Axis1->m_DirectionRatios[1];
			
			local_y.x = trans_operator_2d->m_Axis2->m_DirectionRatios[0];
			local_y.y = trans_operator_2d->m_Axis2->m_DirectionRatios[1];
		}

		shared_ptr<IfcCartesianTransformationOperator2DnonUniform> non_uniform = dynamic_pointer_cast<IfcCartesianTransformationOperator2DnonUniform>(transform_operator);
		if( non_uniform )
		{
			if( non_uniform->m_Scale2 == non_uniform->m_Scale2 )
			{
				// m_Scale2 is not NAN
				scale_y = non_uniform->m_Scale2;
			}
		}
	}
	else
	{
		// ENTITY IfcCartesianTransformationOperator3D SUPERTYPE OF(IfcCartesianTransformationOperator3DnonUniform)
		shared_ptr<IfcCartesianTransformationOperator3D> trans_operator_3d = dynamic_pointer_cast<IfcCartesianTransformationOperator3D>(transform_operator);
		if( !trans_operator_3d )
		{
			throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
		}
		if( !trans_operator_3d->m_LocalOrigin )
		{
			throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
		}
		if( trans_operator_3d->m_LocalOrigin->m_Coordinates.size() < 3 )
		{
			throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
		}
		translate.x = trans_operator_3d->m_LocalOrigin->m_Coordinates[0]->m_value*length_factor;
		translate.y = trans_operator_3d->m_LocalOrigin->m_Coordinates[1]->m_value*length_factor;
		translate.z = trans_operator_3d->m_LocalOrigin->m_Coordinates[2]->m_value*length_factor;
		if( trans_operator_3d->m_Scale == trans_operator_3d->m_Scale )
		{
			// m_Scale is not NAN
			scale = trans_operator_3d->m_Scale;
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
				throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
			}
			if( axis2->m_DirectionRatios.size() < 2 )
			{
				throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
			}
			if( axis3->m_DirectionRatios.size() < 2 )
			{
				throw IfcPPException( "IfcCartesianTransformationOperator is not valid", __func__ );
			}
			local_x.x = axis1->m_DirectionRatios[0];
			local_x.y = axis1->m_DirectionRatios[1];
			local_x.z = axis1->m_DirectionRatios[2];

			local_y.x = axis2->m_DirectionRatios[0];
			local_y.y = axis2->m_DirectionRatios[1];
			local_y.z = axis2->m_DirectionRatios[2];

			local_z.x = axis3->m_DirectionRatios[0];
			local_z.y = axis3->m_DirectionRatios[1];
			local_z.z = axis3->m_DirectionRatios[2];
		}

		shared_ptr<IfcCartesianTransformationOperator3DnonUniform> non_uniform = dynamic_pointer_cast<IfcCartesianTransformationOperator3DnonUniform>(transform_operator);
		if( non_uniform )
		{
			if(non_uniform->m_Scale2 == non_uniform->m_Scale2 )
			{
				scale_y = non_uniform->m_Scale2;
			}
			if( non_uniform->m_Scale3 == non_uniform->m_Scale3 )
			{
				scale_z = non_uniform->m_Scale3;
			}
		}
	}
	local_x.normalize();
	local_y.normalize();
	local_z.normalize();

	carve::math::Matrix rotate_translate(
		local_x.x,		local_y.x,		local_z.x,	translate.x,
		local_x.y,		local_y.y,		local_z.y,	translate.y,
		local_x.z,		local_y.z,		local_z.z,	translate.z,
		0,				0,				0,			1 );
	resulting_matrix = rotate_translate*carve::math::Matrix::SCALE( scale, scale_y, scale_z ); // scale is applied first, rotate second
}

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

#include <vector>
#include <unordered_set>

#include <Bnd_Box.hxx>

#include <osg/Array>
#include <osg/CullFace>
#include <osg/Geode>
#include <osg/Group>
#include <osg/LineStipple>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Version>
#include <osgText/Text>
#include <osgViewer/Viewer>
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPException.h>
#include "GeometrySettings.h"
#include "GeomUtils.h"


namespace SceneGraphUtils
{
	enum ProjectionPlane { UNDEFINED, XY_PLANE, YZ_PLANE, XZ_PLANE };

	class Ray : public osg::Referenced
	{
	public:
		void setRay( Ray* other )
		{
			origin = other->origin;
			direction = other->direction;
		}
		osg::Vec3d origin;
		osg::Vec3d direction;
	};
	class Plane : public osg::Referenced
	{
	public:
		Plane(){}
		Plane( osg::Vec3d& _position, osg::Vec3d& _normal )
		{
			position = _position;
			normal = _normal;
		}
		bool intersectRay( const Ray* ray, bool intersect_in_both_directions, vec3& intersect_point ) const
		{
			gp_Vec plane_pos( position.x(), position.y(), position.z() );
			gp_Dir plane_normal( normal.x(), normal.y(), normal.z() );
			//gp_Pln plane = gp_Pln( plane_pos, plane_normal );

			gp_Vec ray_origin( ray->origin.x(), ray->origin.y(), ray->origin.z() );
			gp_Dir ray_direction( ray->direction.x(), ray->direction.y(), ray->direction.z() );

			double denom = plane_normal.Dot( ray_direction );
			double abs_denom = abs( denom );
			if( abs_denom < 0.0001 )
			{
				return false;
			}

			double t = (plane_pos - ray_origin).Dot( plane_normal ) / denom;
			if( !intersect_in_both_directions )
			{
				if( t < -0.0001 )
				{
					return false;
				}
			}
			intersect_point = ray_origin + t * ray_direction;
			return true;
		}
		
		/** distance point to plane */
		double distancePointPlane( const osg::Vec3d& point )
		{
			return normal*(point - position);
		}
	
		osg::Vec3d position;
		osg::Vec3d normal;
	};

	inline osg::ref_ptr<osg::Geode> createCoordinateAxes( double length )
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
		stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		float alpha = 0.5f;

		// positive axes
		{
			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
			geode->addDrawable( geom );

			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( length, 0.0, 0.0 ) );

			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 0.0, length, 0.0 ) );

			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 0.0, 0.0, length ) );

#ifndef COORDINATE_AXES_NO_COLORS
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.8f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.8f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
			colors->setBinding( osg::Array::BIND_PER_VERTEX );

			geom->setColorArray( colors );
			//geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
#endif
			geom->setVertexArray( vertices );
			geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, 6 ) );
		}

		// positive axes
		{
			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
			geode->addDrawable( geom );

			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( -length, 0.0, 0.0 ) );

			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 0.0, -length, 0.0 ) );

			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 0.0, 0.0, -length ) );

#ifndef COORDINATE_AXES_NO_COLORS
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 1.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 1.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
			colors->setBinding( osg::Array::BIND_PER_VERTEX );

			geom->setColorArray( colors );
			//geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
#endif
			geom->setVertexArray( vertices );
			geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, 6 ) );

			// make negative axed dotted
			osg::ref_ptr<osg::StateSet> stateset_negative = geom->getOrCreateStateSet();
			osg::ref_ptr<osg::LineStipple> linestipple = new osg::LineStipple();
			linestipple->setFactor( 2 );
			linestipple->setPattern( 0xAAAA );
			stateset_negative->setAttributeAndModes( linestipple, osg::StateAttribute::ON );
		}

		// x axis label
		bool add_x_label = false;
		if( add_x_label )
		{
			osg::ref_ptr<osgText::Text> label_x = new  osgText::Text();
			label_x->setFont( "ARIAL.TTF" );
			label_x->setAlignment( osgText::Text::RIGHT_TOP );
			label_x->setAxisAlignment( osgText::Text::SCREEN );
			label_x->setColor( osg::Vec4( 0.8, 0.0, 0.0, 1.0 ) );
			label_x->setCharacterSize( 0.5f );
			label_x->setText( "x" );
			label_x->setPosition( osg::Vec3( 1, 0, 0 ) );
			label_x->setEnableDepthWrites( false );
			geode->addDrawable( label_x );
		}

		return geode;
	}
	inline osg::ref_ptr<osg::Group> createCoordinateAxesArrows()
	{
		float cone_tip = 1.2f;
		float cone_base = 1.f;
		float cone_radius = 0.06f;
		float cone_height = cone_tip - cone_base;
		osg::ref_ptr<osg::Group> group = new osg::Group();

		{
			// x
			osg::ref_ptr<osg::ShapeDrawable> cone_drawable = new osg::ShapeDrawable( new osg::Cone( osg::Vec3f( 0.f, 0.f, 0.f ), cone_radius, cone_height ) );
			osg::ref_ptr<osg::ShapeDrawable> cyl_drawable = new osg::ShapeDrawable( new osg::Cylinder( osg::Vec3f( 0.f, 0.f, -cone_base*0.5 ), cone_radius*0.2, cone_base ) );
			cone_drawable->setColor( osg::Vec4f( 0.8f, 0.0f, 0.0f, 0.7f ) );
			cyl_drawable->setColor( osg::Vec4f( 0.8f, 0.0f, 0.0f, 0.7f ) );
			osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform( osg::Matrix::rotate( M_PI_2, osg::Vec3d( 0, 1, 0 ) )*osg::Matrix::translate( 1, 0, 0 ) );
			osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			geode->addDrawable( cone_drawable );
			geode->addDrawable( cyl_drawable );
			mt1->addChild( geode );
			group->addChild( mt1 );

			osg::Material* material = new osg::Material();
			material->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.7f, 0.f, 0.f, 0.7f ) );
			material->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.7f, 0.f, 0.f, 0.7f ) );
			material->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 1.f, 0.4f, 0.4f, 0.7f ) );
			material->setShininess( osg::Material::FRONT_AND_BACK, 30.0 );
			cone_drawable->getOrCreateStateSet()->setAttribute( material, osg::StateAttribute::ON );
			cyl_drawable->getOrCreateStateSet()->setAttribute( material, osg::StateAttribute::ON );

		}

		{
			// y
			osg::ref_ptr<osg::Cone> cone = new osg::Cone( osg::Vec3f( 0.f, 0.f, 0.f ), cone_radius, cone_height );

			osg::ref_ptr<osg::ShapeDrawable> cone_drawable = new osg::ShapeDrawable( new osg::Cone( osg::Vec3f( 0.f, 0.f, 0.f ), cone_radius, cone_height ) );
			osg::ref_ptr<osg::ShapeDrawable> cyl_drawable = new osg::ShapeDrawable( new osg::Cylinder( osg::Vec3f( 0.f, 0.f, cone_base*0.5 ), cone_radius*0.2, cone_base ) );
			cone_drawable->setColor( osg::Vec4f( 0.f, 0.7f, 0.f, 0.7f ) );
			cyl_drawable->setColor( osg::Vec4f( 0.f, 0.7f, 0.f, 0.7f ) );


			osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform( osg::Matrix::rotate( -M_PI_2, osg::Vec3d( 1, 0, 0 ) )*osg::Matrix::translate( 0, 1, 0 ) );
			osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform( osg::Matrix::rotate( -M_PI_2, osg::Vec3d( 1, 0, 0 ) )*osg::Matrix::translate( 0, 1 + cone_height, 0 ) );
			osg::ref_ptr<osg::MatrixTransform> mt_cyl = new osg::MatrixTransform( osg::Matrix::rotate( -M_PI_2, osg::Vec3d( 1, 0, 0 ) ) );
			osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			geode->addDrawable( cone_drawable );

			osg::ref_ptr<osg::Geode> geode_cyl = new osg::Geode();
			geode_cyl->addDrawable( cyl_drawable );

			mt1->addChild( geode );
			mt2->addChild( geode );
			mt_cyl->addChild( geode_cyl );
			group->addChild( mt1 );
			group->addChild( mt2 );
			group->addChild( mt_cyl );

			osg::Material* material = new osg::Material();
			material->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.0f, 0.7f, 0.f, 0.7f ) );
			material->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.0f, 0.7f, 0.f, 0.7f ) );
			material->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.4f, 1.f, 0.4f, 0.7f ) );
			material->setShininess( osg::Material::FRONT_AND_BACK, 30.0 );
			cone_drawable->getOrCreateStateSet()->setAttribute( material, osg::StateAttribute::ON );
			cyl_drawable->getOrCreateStateSet()->setAttribute( material, osg::StateAttribute::ON );
		}

		{
			// z
			osg::ref_ptr<osg::ShapeDrawable> cone_drawable = new osg::ShapeDrawable( new osg::Cone( osg::Vec3f( 0.f, 0.f, 0.f ), cone_radius, cone_height ) );
			osg::ref_ptr<osg::ShapeDrawable> cyl_drawable = new osg::ShapeDrawable( new osg::Cylinder( osg::Vec3f( 0.f, 0.f, cone_base*0.5 ), cone_radius*0.2, cone_base ) );
			cone_drawable->setColor( osg::Vec4f( 0.0f, 0.0f, 0.8f, 0.7f ) );
			cyl_drawable->setColor( osg::Vec4f( 0.0f, 0.0f, 0.8f, 0.7f ) );

			osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform( osg::Matrix::translate( 0, 0, 1 ) );
			osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform( osg::Matrix::translate( 0, 0, 1 + cone_height ) );
			osg::ref_ptr<osg::MatrixTransform> mt3 = new osg::MatrixTransform( osg::Matrix::translate( 0, 0, 1 + cone_height * 2 ) );

			osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			geode->addDrawable( cone_drawable );
			osg::ref_ptr<osg::Geode> geode_cyl = new osg::Geode();
			geode_cyl->addDrawable( cyl_drawable );
			mt1->addChild( geode );
			mt2->addChild( geode );
			mt3->addChild( geode );
			group->addChild( mt1 );
			group->addChild( mt2 );
			group->addChild( mt3 );
			group->addChild( geode_cyl );

			osg::Material* material = new osg::Material();
			material->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.f, 0.f, 0.8f, 0.7f ) );
			material->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.f, 0.f, 0.8f, 0.7f ) );
			material->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.4f, 0.4f, 1.f, 0.7f ) );
			material->setShininess( osg::Material::FRONT_AND_BACK, 30.0 );
			cone_drawable->getOrCreateStateSet()->setAttribute( material, osg::StateAttribute::ON );
			cyl_drawable->getOrCreateStateSet()->setAttribute( material, osg::StateAttribute::ON );
		}
		return group;
	}
	inline osg::ref_ptr<osg::Geode> createCoordinateGrid()
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
		stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

		{
			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
			geode->addDrawable( geom );

			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();

			for( int i = 0; i <= 20; ++i )
			{
				vertices->push_back( osg::Vec3f( -10, -10 + i, 0.0 ) );
				vertices->push_back( osg::Vec3f( 10, -10 + i, 0.0 ) );

				vertices->push_back( osg::Vec3f( -10 + i, -10, 0.0 ) );
				vertices->push_back( osg::Vec3f( -10 + i, 10, 0.0 ) );
			}

			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->push_back( osg::Vec4f( 0.7f, 0.7f, 0.7f, 0.5f ) );
			geom->setColorArray( colors );
			colors->setBinding( osg::Array::BIND_OVERALL );
			//geom->setColorBinding( osg::Geometry::BIND_OVERALL );

			geom->setVertexArray( vertices );
			geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vertices->size() ) );
		}

		return geode;
	}
	inline void WireFrameModeOn( osg::StateSet* state )
	{
		osg::ref_ptr<osg::PolygonMode> polygon_mode = dynamic_cast<osg::PolygonMode*>( state->getAttribute( osg::StateAttribute::POLYGONMODE ) );
		if( !polygon_mode )
		{
			polygon_mode = new osg::PolygonMode();
			state->setAttribute( polygon_mode );
		}
		polygon_mode->setMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
	}
	inline void disableWireFrameOnText( osg::Node* node )
	{
		if( node == nullptr )
		{
			return;
		}

		osg::Group* grp = dynamic_cast<osg::Group*>( node );
		if( grp )
		{
			for( size_t i = 0; i < grp->getNumChildren(); ++i )
			{
				osg::Node* child = grp->getChild( i );
				disableWireFrameOnText( child );
			}
		}
		else
		{
			osg::Geode* geode = dynamic_cast<osg::Geode*>( node );
			if( geode )
			{
				for( size_t i = 0; i < geode->getNumDrawables(); ++i )
				{
					osg::Drawable* child = geode->getDrawable( i );

					osgText::Text* txt = dynamic_cast<osgText::Text*>( child );
					if( txt )
					{
						osg::StateSet* state = txt->getOrCreateStateSet();

						osg::PolygonMode* polygon_mode = dynamic_cast<osg::PolygonMode*>( state->getAttribute( osg::StateAttribute::POLYGONMODE ) );

						if( !polygon_mode )
						{
							polygon_mode = new osg::PolygonMode();
							state->setAttribute( polygon_mode );
						}
						polygon_mode->setMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL );
					}
				}
			}
		}
	}
	inline void WireFrameModeOn( osg::Node* node )
	{
		if( node == nullptr )
		{
			return;
		}

		osg::StateSet* state = node->getOrCreateStateSet();
		WireFrameModeOn( state );
		disableWireFrameOnText( node );
	}
	inline void WireFrameModeOff( osg::StateSet* state )
	{
		osg::PolygonMode *polygon_mode = dynamic_cast<osg::PolygonMode*>( state->getAttribute( osg::StateAttribute::POLYGONMODE ) );

		if( !polygon_mode )
		{
			polygon_mode = new osg::PolygonMode();
			state->setAttribute( polygon_mode );
		}
		polygon_mode->setMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL );
	}
	inline void WireFrameModeOff( osg::Node *node )
	{
		if( node == nullptr )
		{
			return;
		}

		osg::StateSet *state = node->getOrCreateStateSet();
		WireFrameModeOff( state );
	}
	inline void HiddenLineModeOn( osg::Group* node )
	{
		return;
		osg::ref_ptr<osg::StateSet> ss = node->getOrCreateStateSet();
		ss->setAttributeAndModes( new osg::ColorMask( false, false, false, false ), osg::StateAttribute::ON );
		ss->setBinName( "RenderBin" );
		ss->setBinNumber( 1 );
		ss->setRenderBinDetails( 1, "RenderBin" );

		osg::ref_ptr<osg::Group> node_lines = new osg::Group();
		// TODO: create lines

		ss = node_lines->getOrCreateStateSet();
		ss->setBinName( "RenderBin" );
		ss->setBinNumber( 2 );
		ss->setRenderBinDetails( 2, "RenderBin" );
		ss->setMode( GL_POLYGON_OFFSET_FILL, osg::StateAttribute::ON );
		osg::ref_ptr<osg::PolygonOffset> po = new osg::PolygonOffset( 10.0f, 10.0f );
		ss->setAttributeAndModes( po, osg::StateAttribute::ON );
	}
	inline void cullFrontBack( bool front, bool back, osg::StateSet* stateset )
	{
		if( front )
		{
			if( back )
			{
				// cull back and front
				osg::ref_ptr<osg::CullFace> cull = new osg::CullFace( osg::CullFace::FRONT_AND_BACK );
				stateset->setAttributeAndModes( cull.get(), osg::StateAttribute::ON );
			}
			else
			{
				// cull back face off
				osg::ref_ptr<osg::CullFace> cull_back_off = new osg::CullFace( osg::CullFace::BACK );
				stateset->setAttributeAndModes( cull_back_off.get(), osg::StateAttribute::OFF );

				// cull front face on
				osg::ref_ptr<osg::CullFace> cull_front_on = new osg::CullFace( osg::CullFace::FRONT );
				stateset->setAttributeAndModes( cull_front_on.get(), osg::StateAttribute::ON );
			}
		}
		else
		{
			if( back )
			{
				// cull front face off
				osg::ref_ptr<osg::CullFace> cull_front_off = new osg::CullFace( osg::CullFace::FRONT );
				stateset->setAttributeAndModes( cull_front_off.get(), osg::StateAttribute::OFF );

				// cull back face on
				osg::ref_ptr<osg::CullFace> cull_back_on = new osg::CullFace( osg::CullFace::BACK );
				stateset->setAttributeAndModes( cull_back_on.get(), osg::StateAttribute::ON );

			}
			else
			{
				// cull back and front off
				osg::ref_ptr<osg::CullFace> cull = new osg::CullFace( osg::CullFace::FRONT_AND_BACK );
				stateset->setAttributeAndModes( cull.get(), osg::StateAttribute::OFF );
			}
		}
	}
	inline void setMaterialAlpha( osg::Node* node, float alpha )
	{
		osg::StateSet* stateset = node->getStateSet();
		if( stateset )
		{
			osg::ref_ptr<osg::Material> mat = dynamic_cast<osg::Material*>( stateset->getAttribute( osg::StateAttribute::MATERIAL ) );
			if( mat )
			{
				//float transparency_restore = mat->getTransparency( osg::Material::FRONT_AND_BACK );
				mat->setAlpha( osg::Material::FRONT_AND_BACK, alpha );

				//const osg::Vec4& ambient = mat->getAmbient( osg::Material::FRONT_AND_BACK );
				//mat->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4( ambient.r(), ambient.g(), ambient.b(), transparency ) );

				//const osg::Vec4& diffuse = mat->getDiffuse( osg::Material::FRONT_AND_BACK );
				//mat->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4( diffuse.r(), diffuse.g(), diffuse.b(), transparency ) );

				//const osg::Vec4& specular = mat->getSpecular( osg::Material::FRONT_AND_BACK );
				//mat->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4( specular.r(), specular.g(), specular.b(), transparency ) );

				//mat->setShininess( osg::Material::FRONT_AND_BACK, 64.f );
				//mat->setColorMode( osg::Material::SPECULAR );
			}
		}
		osg::Group* group = dynamic_cast<osg::Group*>( node );
		if( group )
		{
			for( unsigned int ii = 0; ii < group->getNumChildren(); ++ii )
			{
				osg::Node* child_node = group->getChild( ii );
				setMaterialAlpha( child_node, alpha );
			}
		}
	}
	inline void removeChildren( osg::Group* group )
	{
		if( group )
		{
			group->removeChildren( 0, group->getNumChildren() );
		}
	}
	inline void removeDrawables( osg::Geode* geode )
	{
		if( geode )
		{
			geode->removeDrawables( 0, geode->getNumDrawables() );
		}
	}
	inline void drawBoundingBoxLines( const Bnd_Box& aabb, osg::Vec3Array* vertices, osg::Geometry* geom )
	{
		if( aabb.IsVoid() )
		{
			return;
		}
		double xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
		aabb.Get( xmin, ymin, zmin, xmax, ymax, zmax );
		double pos_x = (xmin + xmax)*0.5;
		double pos_y = (ymin + ymax)*0.5;
		double pos_z = (zmin + zmax)*0.5;
		double extent_x = (xmax - xmin)*0.5;
		double extent_y = (ymax - ymin)*0.5;
		double extent_z = (zmax - zmin)*0.5;

		const double dex = extent_x;
		const double dey = extent_y;
		const double dez = extent_z;

		const int vert_id_offset = vertices->size();
		vertices->push_back( osg::Vec3f( pos_x - dex, pos_y - dey, pos_z - dez ) );
		vertices->push_back( osg::Vec3f( pos_x + dex, pos_y - dey, pos_z - dez ) );
		vertices->push_back( osg::Vec3f( pos_x + dex, pos_y + dey, pos_z - dez ) );
		vertices->push_back( osg::Vec3f( pos_x - dex, pos_y + dey, pos_z - dez ) );

		vertices->push_back( osg::Vec3f( pos_x - dex, pos_y - dey, pos_z + dez ) );
		vertices->push_back( osg::Vec3f( pos_x + dex, pos_y - dey, pos_z + dez ) );
		vertices->push_back( osg::Vec3f( pos_x + dex, pos_y + dey, pos_z + dez ) );
		vertices->push_back( osg::Vec3f( pos_x - dex, pos_y + dey, pos_z + dez ) );

		osg::DrawElementsUInt* box_lines = new osg::DrawElementsUInt( GL_LINE_STRIP, 0 );
		box_lines->push_back( vert_id_offset + 0 );
		box_lines->push_back( vert_id_offset + 1 );
		box_lines->push_back( vert_id_offset + 2 );
		box_lines->push_back( vert_id_offset + 3 );
		box_lines->push_back( vert_id_offset + 0 );
		box_lines->push_back( vert_id_offset + 4 );
		box_lines->push_back( vert_id_offset + 5 );
		box_lines->push_back( vert_id_offset + 1 );
		box_lines->push_back( vert_id_offset + 5 );
		box_lines->push_back( vert_id_offset + 6 );
		box_lines->push_back( vert_id_offset + 2 );
		box_lines->push_back( vert_id_offset + 6 );
		box_lines->push_back( vert_id_offset + 7 );
		box_lines->push_back( vert_id_offset + 3 );
		box_lines->push_back( vert_id_offset + 7 );
		box_lines->push_back( vert_id_offset + 4 );
		geom->addPrimitiveSet( box_lines );
	}
	inline void drawBoundingBoxLines( const Bnd_Box& aabb, osg::Geode* geode )
	{
		if( aabb.IsVoid() )
		{
			return;
		}
		osg::Geometry* geom_bbox = new osg::Geometry();
		geode->addDrawable( geom_bbox );
		osg::Vec3Array* vertices_bbox = new osg::Vec3Array();
		geom_bbox->setVertexArray( vertices_bbox );
		drawBoundingBoxLines( aabb, vertices_bbox, geom_bbox );
	}
	inline void translateGroup( osg::Group* grp, const osg::Vec3f& trans, std::unordered_set<osg::Geode*>& set_applied )
	{
		int num_children = grp->getNumChildren();
		for( int i = 0; i < num_children; ++i )
		{
			osg::Node* node = grp->getChild( i );
			osg::Group* child_group = dynamic_cast<osg::Group*>( node );
			if( child_group )
			{
				translateGroup( child_group, trans, set_applied );
				continue;
			}

			osg::Geode* child_geode = dynamic_cast<osg::Geode*>( node );
			if( child_geode )
			{
				if( set_applied.find( child_geode ) != set_applied.end() )
				{
					continue;
				}
				set_applied.insert( child_geode );
				for( size_t ii_drawables = 0; ii_drawables < child_geode->getNumDrawables(); ++ii_drawables )
				{
					osg::Drawable* drawable = child_geode->getDrawable(ii_drawables);
				
					osg::Geometry* child_geometry = dynamic_cast<osg::Geometry*>( drawable );
					if( !child_geometry )
					{
#ifdef _DEBUG
						std::cout << "!child_geometry" << std::endl;
#endif
						return;
					}
					osg::Array* vertices_array = child_geometry->getVertexArray();
					osg::Vec3Array* vertices_float = dynamic_cast<osg::Vec3Array*>( vertices_array );

					if( !vertices_float )
					{
#ifdef _DEBUG
						std::cout << "!vertices_float" << std::endl;
#endif
						return;
					}

					for( osg::Vec3Array::iterator it_array = vertices_float->begin(); it_array != vertices_float->end(); ++it_array )
					{
						osg::Vec3f& vertex = ( *it_array );
						vertex = vertex + trans;
					}

					vertices_float->dirty();
					child_geometry->dirtyBound();
					child_geometry->dirtyDisplayList();
					child_geode->dirtyBound();
					grp->dirtyBound();
				}

				continue;
			}

#ifdef _DEBUG
			std::cout << __FUNC__ << ": unhandled node: " << node->className() << std::endl;
#endif
		}
	}
}

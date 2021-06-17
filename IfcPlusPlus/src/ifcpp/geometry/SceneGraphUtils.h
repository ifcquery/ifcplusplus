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

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <unordered_set>
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
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

namespace SceneGraphUtils
{
	inline bool inParentList(const std::string guid, const osg::Group* group)
	{
		if (!group)
		{
			return false;
		}

		const osg::Group::ParentList& vec_parents = group->getParents();
		for (size_t ii = 0; ii < vec_parents.size(); ++ii)
		{
			const osg::Group* parent = vec_parents[ii];
			if (parent)
			{
				const std::string parent_name = parent->getName();
				if (parent_name.length() >= 22)
				{
					std::string parent_name_id = parent_name.substr(22);
					if (parent_name_id == guid)
					{
						return true;
					}
					bool in_parent_list = inParentList(guid, parent);
					if (in_parent_list)
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	inline bool inParentList( const int entity_id, const osg::Group* group )
	{
		if( !group )
		{
			return false;
		}

		const osg::Group::ParentList& vec_parents = group->getParents();
		for( size_t ii = 0; ii < vec_parents.size(); ++ii )
		{
			const osg::Group* parent = vec_parents[ii];
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
	inline osg::Vec3d computePolygonNormal( const osg::Vec3dArray* polygon )
	{
		const int num_points = polygon->size();
		osg::Vec3d polygon_normal( 0, 0, 0 );
		for( int k = 0; k < num_points; ++k )
		{
			const osg::Vec3d& vertex_current = polygon->at( k );
			const osg::Vec3d& vertex_next = polygon->at( (k + 1) % num_points );
			polygon_normal._v[0] += (vertex_current.y() - vertex_next.y())*(vertex_current.z() + vertex_next.z());
			polygon_normal._v[1] += (vertex_current.z() - vertex_next.z())*(vertex_current.x() + vertex_next.x());
			polygon_normal._v[2] += (vertex_current.x() - vertex_next.x())*(vertex_current.y() + vertex_next.y());
		}
		polygon_normal.normalize();
		return polygon_normal;
	}
	inline osg::Vec3f computePolygonNormal( const osg::Vec3Array* polygon )
	{
		const int num_points = polygon->size();
		osg::Vec3f polygon_normal( 0, 0, 0 );
		for( int k = 0; k < num_points; ++k )
		{
			const osg::Vec3f& vertex_current = polygon->at( k );
			const osg::Vec3f& vertex_next = polygon->at( (k + 1) % num_points );
			polygon_normal._v[0] += (vertex_current.y() - vertex_next.y())*(vertex_current.z() + vertex_next.z());
			polygon_normal._v[1] += (vertex_current.z() - vertex_next.z())*(vertex_current.x() + vertex_next.x());
			polygon_normal._v[2] += (vertex_current.x() - vertex_next.x())*(vertex_current.y() + vertex_next.y());
		}
		polygon_normal.normalize();
		return polygon_normal;
	}
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
			label_x->setColor( osg::Vec4( 0.8f, 0.0f, 0.0f, 1.0f ) );
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

			osg::ref_ptr<osg::Material> material = new osg::Material();
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

			osg::ref_ptr<osg::Material> material = new osg::Material();
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
		osg::ref_ptr<osg::PolygonMode> polygon_mode = dynamic_cast<osg::PolygonMode*>( state->getAttribute( osg::StateAttribute::POLYGONMODE ) );

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
	inline void setMaterialAlpha( osg::Node* node, float alpha, bool create_material_if_not_existing )
	{
		osg::StateSet* stateset = node->getOrCreateStateSet();
		if( stateset )
		{
			osg::ref_ptr<osg::Material> mat = dynamic_cast<osg::Material*>( stateset->getAttribute( osg::StateAttribute::MATERIAL ) );
			if( mat )
			{
				osg::Vec4f ambient = mat->getAmbient(osg::Material::FRONT_AND_BACK);
				if (ambient.a() > alpha)
				{
					mat->setAlpha(osg::Material::FRONT_AND_BACK, alpha);
				}
			}
			else if( create_material_if_not_existing )
			{
				osg::ref_ptr<osg::Material> mat = new osg::Material();
				mat->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.8f, 0.83f, 0.84f, alpha));
				mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.8f, 0.83f, 0.84f, alpha));
				stateset->setAttribute(mat, osg::StateAttribute::ON);
				stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
				stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
			}
		}
		osg::Group* group = dynamic_cast<osg::Group*>( node );
		if( group )
		{
			for( unsigned int ii = 0; ii < group->getNumChildren(); ++ii )
			{
				osg::Node* child_node = group->getChild( ii );
				setMaterialAlpha( child_node, alpha, false );
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

	inline void clearAllChildNodes( osg::Group* group )
	{
		int num_children = group->getNumChildren();
		for( int i = 0; i < num_children; ++i )
		{
			osg::Node* node = group->getChild( i );
			osg::Group* child_group = dynamic_cast<osg::Group*>( node );
			if( child_group )
			{
				osg::Geode* geode = dynamic_cast<osg::Geode*>( child_group );
				if( geode )
				{
					geode->removeDrawables( 0, geode->getNumDrawables() );
					
				}
				clearAllChildNodes( child_group );
			}
			node->releaseGLObjects();
			if( node->getStateSet() )
			{
				node->setStateSet( nullptr );
			}
		}
		group->removeChildren( 0, group->getNumChildren() );
	}
	inline void removeDrawables( osg::Geode* geode )
	{
		if( geode )
		{
			geode->removeDrawables( 0, geode->getNumDrawables() );
		}
	}

	inline void translateGroup( osg::Group* grp, const osg::Vec3d& trans, std::unordered_set<osg::Node*>& set_applied, double minTranslateLength )
	{
		if (trans.length2() < minTranslateLength*minTranslateLength )
		{
			return;
		}

		osg::Vec3d currentTranslate = trans;
		int num_children = grp->getNumChildren();
		for (int i = 0; i < num_children; ++i)
		{
			osg::Node* node = grp->getChild(i);
			if (!node)
			{
				continue;
			}

			osg::MatrixTransform* child_transform = dynamic_cast<osg::MatrixTransform*>(node);
			if (child_transform)
			{
				osg::Matrix matrix = child_transform->getMatrix();
				osg::Vec3d delta = trans - matrix.getTrans();

				matrix.preMult(osg::Matrix::translate(trans));
				child_transform->setMatrix(matrix);

				osg::Vec3d matrix_translate2 = child_transform->getMatrix().getTrans();
				currentTranslate = osg::Vec3d();
			}

			osg::Group* child_group = dynamic_cast<osg::Group*>(node);
			if (child_group)
			{
				translateGroup(child_group, currentTranslate, set_applied, minTranslateLength);
				continue;
			}

			osg::Geode* child_geode = dynamic_cast<osg::Geode*>(node);
			if (child_geode)
			{
				if (set_applied.find(child_geode) != set_applied.end())
				{
					continue;
				}
				set_applied.insert(child_geode);
				for (size_t ii_drawables = 0; ii_drawables < child_geode->getNumDrawables(); ++ii_drawables)
				{
					osg::Drawable* drawable = child_geode->getDrawable(ii_drawables);

					osg::Geometry* child_geometry = dynamic_cast<osg::Geometry*>(drawable);
					if (!child_geometry)
					{
#ifdef _DEBUG
						std::cout << "!child_geometry" << std::endl;
#endif
						continue;
					}
					osg::Array* vertices_array = child_geometry->getVertexArray();

					osg::Vec3Array* vertices_float = dynamic_cast<osg::Vec3Array*>(vertices_array);
					if (vertices_float)
					{
						for (osg::Vec3Array::iterator it_array = vertices_float->begin(); it_array != vertices_float->end(); ++it_array)
						{
							osg::Vec3f& vertex = (*it_array);
							vertex = vertex + trans;
						}

						vertices_float->dirty();
						child_geometry->dirtyBound();
						child_geometry->dirtyDisplayList();
						child_geode->dirtyBound();
						grp->dirtyBound();
						continue;
					}

					osg::Vec3dArray* vertices_double = dynamic_cast<osg::Vec3dArray*>(vertices_array);
					if (vertices_double)
					{
						for (osg::Vec3dArray::iterator it_array = vertices_double->begin(); it_array != vertices_double->end(); ++it_array)
						{
							osg::Vec3d& vertex = (*it_array);
							vertex = vertex + trans;
						}

						vertices_double->dirty();
						child_geometry->dirtyBound();
						child_geometry->dirtyDisplayList();
						child_geode->dirtyBound();
						grp->dirtyBound();
						continue;
					}

#ifdef _DEBUG
					std::cout << "vertices type not implemented" << std::endl;
#endif



				}

				continue;
			}

			osg::Geometry* child_geometry = dynamic_cast<osg::Geometry*>(node);
			if (child_geometry)
			{
				if (set_applied.find(child_geometry) != set_applied.end())
				{
					continue;
				}
				set_applied.insert(child_geometry);

				osg::Array* vertices_array = child_geometry->getVertexArray();
				
				osg::Vec3Array* vertices_float = dynamic_cast<osg::Vec3Array*>(vertices_array);
				if (vertices_float)
				{
					for (osg::Vec3Array::iterator it_array = vertices_float->begin(); it_array != vertices_float->end(); ++it_array)
					{
						osg::Vec3f& vertex = (*it_array);
						vertex = vertex + trans;
					}

					vertices_float->dirty();
					child_geometry->dirtyBound();
					child_geometry->dirtyDisplayList();
					continue;
				}

				osg::Vec3dArray* vertices_double = dynamic_cast<osg::Vec3dArray*>(vertices_array);
				if (vertices_double)
				{
					for (osg::Vec3dArray::iterator it_array = vertices_double->begin(); it_array != vertices_double->end(); ++it_array)
					{
						osg::Vec3d& vertex = (*it_array);
						vertex = vertex + trans;
					}

					vertices_double->dirty();
					child_geometry->dirtyBound();
					child_geometry->dirtyDisplayList();
					continue;
				}

				std::cout << "vertices type not implemented" << std::endl;
				continue;
			}

			osgText::Text* child_text = dynamic_cast<osgText::Text*>(node);
			if (child_text)
			{
				osg::Vec3d pos = child_text->getPosition() + trans;
				child_text->setPosition(pos);
				continue;
			}

#ifdef _DEBUG
			std::cout << __FUNC__ << ": unhandled node: " << node->className() << std::endl;
#endif
		}
	}
}

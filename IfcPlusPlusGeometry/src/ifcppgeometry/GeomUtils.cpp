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

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Group>
#include <osgText/Text>
#include <osg/CullFace>
#include <osg/Material>
#include <osg/Depth>
#include <osg/LineStipple>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <utility>
#include <sstream>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/IFC4/include/IfcFace.h>

#include "IncludeCarveHeaders.h"
#include "GeometrySettings.h"
#include "ProfileConverter.h"
#include "CSG_Adapter.h"
#include "RepresentationConverter.h"
#include "GeomUtils.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GeomUtils::WireFrameModeOn( osg::StateSet* state )
{
	osg::ref_ptr<osg::PolygonMode> polygon_mode = dynamic_cast<osg::PolygonMode*>( state->getAttribute( osg::StateAttribute::POLYGONMODE ));
	if( !polygon_mode )
	{
		polygon_mode = new osg::PolygonMode();
		state->setAttribute( polygon_mode );	
	}
	polygon_mode->setMode(  osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
}

void disableWireFrameOnText( osg::Node* node )
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

					osg::PolygonMode* polygon_mode = dynamic_cast<osg::PolygonMode*>( state->getAttribute( osg::StateAttribute::POLYGONMODE ));

					if( !polygon_mode )
					{
						polygon_mode = new osg::PolygonMode();
						state->setAttribute( polygon_mode );	
					}
					polygon_mode->setMode(  osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL );
				}
			}
		}
	}
}

void GeomUtils::WireFrameModeOn( osg::Node* node )
{
	if( node == nullptr )
	{
		return;
	}

	osg::StateSet* state = node->getOrCreateStateSet();
	WireFrameModeOn( state );

	disableWireFrameOnText( node );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GeomUtils::WireFrameModeOff( osg::StateSet* state )
{
	osg::PolygonMode *polygon_mode = dynamic_cast<osg::PolygonMode*>( state->getAttribute( osg::StateAttribute::POLYGONMODE ));

	if(  !polygon_mode )
	{
		polygon_mode = new osg::PolygonMode();
		state->setAttribute( polygon_mode );	
	}
	polygon_mode->setMode(  osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL );
}
void GeomUtils::WireFrameModeOff( osg::Node *node )
{
	if( node == nullptr )
	{
		return;
	}

	osg::StateSet *state = node->getOrCreateStateSet();
	WireFrameModeOff( state );
}

void GeomUtils::HiddenLineModeOn( osg::Group* node )
{
	return;
	osg::ref_ptr<osg::StateSet> ss = node->getOrCreateStateSet(); 
	ss->setAttributeAndModes( new osg::ColorMask(false,false,false,false),osg::StateAttribute::ON); 
	ss->setBinName("RenderBin"); 
	ss->setBinNumber(1); 
	ss->setRenderBinDetails(1,"RenderBin");

	osg::ref_ptr<osg::Group> node_lines = new osg::Group();
	// TODO: create lines
	
	ss =node_lines->getOrCreateStateSet(); 
	ss->setBinName("RenderBin"); 
	ss->setBinNumber(2); 
	ss->setRenderBinDetails(2,"RenderBin"); 
	ss->setMode(GL_POLYGON_OFFSET_FILL,osg::StateAttribute::ON); 
	osg::ref_ptr<osg::PolygonOffset> po = new osg::PolygonOffset(10.0f,10.0f); 
	ss->setAttributeAndModes(po,osg::StateAttribute::ON); 

}
void GeomUtils::HiddenLineModeOff( osg::Group* node )
{

}

void GeomUtils::cullFrontBack( bool front, bool back, osg::StateSet* stateset )
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

void GeomUtils::setMaterialAlpha( osg::Node* node, float alpha )
{
	osg::StateSet* stateset = node->getStateSet();
	if( stateset )
	{
		osg::ref_ptr<osg::Material> mat = dynamic_cast<osg::Material*>( stateset->getAttribute( osg::StateAttribute::MATERIAL ));
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
		for( unsigned int ii=0; ii<group->getNumChildren(); ++ii )
		{
			osg::Node* child_node = group->getChild( ii );
			setMaterialAlpha( child_node, alpha );
		}
	}
}

//#define COORDINATE_AXES_NO_COLORS

osg::ref_ptr<osg::Geode> GeomUtils::createCoordinateAxes( double length )
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	float alpha = 0.5f;

	// positive axes
	{
		osg::ref_ptr<osg::Geometry> geom	= new osg::Geometry();
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
		colors->push_back( osg::Vec4f( 1.f,		0.f,	0.f, alpha ) );
		colors->push_back( osg::Vec4f( 1.f,		0.f,	0.f, alpha ) );
		colors->push_back( osg::Vec4f( 0.f,		0.8f,	0.f, alpha ) );
		colors->push_back( osg::Vec4f( 0.f,		0.8f,	0.f, alpha ) );
		colors->push_back( osg::Vec4f( 0.f,		0.f,	1.f, alpha ) );
		colors->push_back( osg::Vec4f( 0.f,		0.f,	1.f, alpha ) );

		geom->setColorArray( colors );
		geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
#endif
		geom->setVertexArray( vertices );
		geom->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 6 ) );
	}

	// positive axes
	{
		osg::ref_ptr<osg::Geometry> geom	= new osg::Geometry();
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
		colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha) );
		colors->push_back( osg::Vec4f( 0.f, 1.f, 0.f, alpha ) );
		colors->push_back( osg::Vec4f( 0.f, 1.f, 0.f, alpha ) );
		colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
		colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );

		geom->setColorArray( colors );
		geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
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
		label_x->setEnableDepthWrites(false);
		geode->addDrawable( label_x );
	}
	
	return geode;
}

osg::ref_ptr<osg::Geode> GeomUtils::createCoordinateGrid()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	{
		osg::ref_ptr<osg::Geometry> geom	= new osg::Geometry();
		geode->addDrawable( geom );

		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();

		for( int i=0; i<=20; ++i )
		{
			vertices->push_back( osg::Vec3f( -10, -10+i, 0.0 ) );
			vertices->push_back( osg::Vec3f(  10, -10+i, 0.0 ) );

			vertices->push_back( osg::Vec3f( -10+i, -10, 0.0 ) );
			vertices->push_back( osg::Vec3f( -10+i, 10, 0.0 ) );
		}
		
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		colors->push_back( osg::Vec4f( 0.7f,	0.7f,	0.7f, 0.5f ) );
		geom->setColorArray( colors );
		geom->setColorBinding( osg::Geometry::BIND_OVERALL );

		geom->setVertexArray( vertices );
		geom->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices->size() ) );
	}

	return geode;
}

osg::ref_ptr<osg::Group> GeomUtils::createCoordinateAxesArrows()
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
		cone_drawable->setColor( osg::Vec4f(0.8f, 0.0f, 0.0f, 0.7f) );
		cyl_drawable->setColor( osg::Vec4f(0.8f, 0.0f, 0.0f, 0.7f) );
		osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform( osg::Matrix::rotate( M_PI_2, osg::Vec3d(0,1,0) )*osg::Matrix::translate(1, 0, 0) );
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		geode->addDrawable(cone_drawable);
		geode->addDrawable(cyl_drawable);
		mt1->addChild(geode);
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
		cone_drawable->setColor( osg::Vec4f(0.f, 0.7f, 0.f, 0.7f) );
		cyl_drawable->setColor( osg::Vec4f(0.f, 0.7f, 0.f, 0.7f) );

		
		osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform( osg::Matrix::rotate( -M_PI_2, osg::Vec3d(1,0,0) )*osg::Matrix::translate(0, 1, 0) );
		osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform( osg::Matrix::rotate( -M_PI_2, osg::Vec3d(1,0,0) )*osg::Matrix::translate(0, 1+cone_height, 0) );
		osg::ref_ptr<osg::MatrixTransform> mt_cyl = new osg::MatrixTransform( osg::Matrix::rotate( -M_PI_2, osg::Vec3d(1,0,0) ) );
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		geode->addDrawable(cone_drawable);
		
		osg::ref_ptr<osg::Geode> geode_cyl = new osg::Geode();
		geode_cyl->addDrawable(cyl_drawable);
				
		mt1->addChild(geode);
		mt2->addChild(geode);
		mt_cyl->addChild(geode_cyl);
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
		cone_drawable->setColor( osg::Vec4f(0.0f, 0.0f, 0.8f, 0.7f) );
		cyl_drawable->setColor( osg::Vec4f(0.0f, 0.0f, 0.8f, 0.7f) );

		osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform( osg::Matrix::translate(0, 0, 1) );
		osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform( osg::Matrix::translate(0, 0, 1+cone_height) );
		osg::ref_ptr<osg::MatrixTransform> mt3 = new osg::MatrixTransform( osg::Matrix::translate(0, 0, 1+cone_height*2) );

		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		geode->addDrawable(cone_drawable);
		osg::ref_ptr<osg::Geode> geode_cyl = new osg::Geode();
		geode_cyl->addDrawable(cyl_drawable);
		mt1->addChild(geode);
		mt2->addChild(geode);
		mt3->addChild(geode);
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

	//group->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::ON );	
	return group;
}

osg::ref_ptr<osg::Geode> GeomUtils::createQuarterCircles()
{
	osg::ref_ptr<osg::DrawElementsUInt> circle_x = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_FAN, 13 );
	osg::ref_ptr<osg::DrawElementsUInt> circle_y = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_FAN, 13 );
	osg::ref_ptr<osg::DrawElementsUInt> circle_z = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_FAN, 13 );
	osg::ref_ptr<osg::Vec3Array> vertices_circle = new osg::Vec3Array(39);
	osg::ref_ptr<osg::Vec3Array> normals_circle = new osg::Vec3Array(39);

	normals_circle->at(0).set(0.f,1.f,0.f);
	normals_circle->at(13).set(1.f,0.f,0.f);
	normals_circle->at(26).set(0.f,0.f,1.f);
	float angle = 0.f, angle_delta = 3.14156f/(2.f*11.f);
	circle_x->at(0)=0;
	circle_y->at(0)=13;
	circle_z->at(0)=26;
	for( int i=1; i<=12; ++i )
	{
		vertices_circle->at(i).set(		sinf(angle),	0.f,			cosf(angle));
		vertices_circle->at(i+13).set(	0.f,			cosf(angle),	sinf(angle));
		vertices_circle->at(i+26).set(	cosf(angle),	sinf(angle),	0.f);
		normals_circle->at(i).set(		0.f,			1.f,			0.f);
		normals_circle->at(i+13).set(	1.f,			0.f,			0.f);
		normals_circle->at(i+26).set(	0.f,			0.f,			1.f);
		circle_x->at(i)=i;
		circle_y->at(i)=i+13;
		circle_z->at(i)=i+26;
		angle += angle_delta;
	}

	osg::ref_ptr<osg::Geometry> geom_circle = new osg::Geometry();
	geom_circle->setVertexArray(vertices_circle);
	geom_circle->setNormalArray(normals_circle);
	geom_circle->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::ref_ptr<osg::Vec4Array> colors_circle = new osg::Vec4Array(1);
	colors_circle->at(0).set(0.5, 0.5, 0.5, 0.3);
	geom_circle->setColorArray(colors_circle);
	geom_circle->setColorBinding( osg::Geometry::BIND_OVERALL );

	geom_circle->addPrimitiveSet(circle_x);
	geom_circle->addPrimitiveSet(circle_y);
	geom_circle->addPrimitiveSet(circle_z);
	osg::ref_ptr<osg::Geode> geode_circle = new osg::Geode();
	osg::ref_ptr<osg::StateSet> stateset_circles = geode_circle->getOrCreateStateSet();
	stateset_circles->setMode( GL_LIGHTING, osg::StateAttribute::ON );
	stateset_circles->setMode( GL_BLEND, osg::StateAttribute::ON );
	stateset_circles->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	stateset_circles->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
	osg::ref_ptr<osg::Depth> depth = new osg::Depth();
	depth->setWriteMask( false );
	stateset_circles->setAttributeAndModes( depth, osg::StateAttribute::ON );

	geode_circle->addDrawable(geom_circle);
	return geode_circle;
}

carve::geom::vector<3> GeomUtils::computePolygonCentroid( const std::vector<carve::geom::vector<3> >& polygon )
{
	carve::geom::vector<3> polygon_centroid( carve::geom::VECTOR(0, 0, 0) );
	for( std::vector<carve::geom::vector<3> >::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
	{
		const carve::geom::vector<3>& vertex_current = (*it);
		polygon_centroid += vertex_current;
	}
	polygon_centroid /= (double)(polygon.size());
	return polygon_centroid;
}

osg::Vec3d GeomUtils::computePolygonNormal( const osg::Vec3dArray* polygon )
{
	const int num_points = polygon->size();
	osg::Vec3d polygon_normal(0, 0, 0);
	for( int k=0; k<num_points; ++k )
	{
		const osg::Vec3d& vertex_current = polygon->at(k);
		const osg::Vec3d& vertex_next = polygon->at((k+1)%num_points);
		polygon_normal._v[0] += (vertex_current.y()-vertex_next.y() )*(vertex_current.z()+vertex_next.z() );
		polygon_normal._v[1] += (vertex_current.z()-vertex_next.z() )*(vertex_current.x()+vertex_next.x() );
		polygon_normal._v[2] += (vertex_current.x()-vertex_next.x() )*(vertex_current.y()+vertex_next.y() );
	}
	polygon_normal.normalize();
	return polygon_normal;
}

osg::Vec3f GeomUtils::computePolygonNormal( const osg::Vec3Array* polygon )
{
	const int num_points = polygon->size();
	osg::Vec3f polygon_normal(0, 0, 0);
	for( int k=0; k<num_points; ++k )
	{
		const osg::Vec3f& vertex_current = polygon->at(k);
		const osg::Vec3f& vertex_next = polygon->at((k+1)%num_points);
		polygon_normal._v[0] += (vertex_current.y()-vertex_next.y() )*(vertex_current.z()+vertex_next.z() );
		polygon_normal._v[1] += (vertex_current.z()-vertex_next.z() )*(vertex_current.x()+vertex_next.x() );
		polygon_normal._v[2] += (vertex_current.x()-vertex_next.x() )*(vertex_current.y()+vertex_next.y() );
	}
	polygon_normal.normalize();
	return polygon_normal;
}

carve::geom::vector<3> GeomUtils::computePolygonNormal( const std::vector<carve::geom::vector<3> >& polygon )
{
	carve::geom::vector<3> polygon_normal( carve::geom::VECTOR(0, 0, 0) );
	bool last_loop = false;
	for( std::vector<carve::geom::vector<3> >::const_iterator it = polygon.begin(); ;  )
	{
		const carve::geom::vector<3>& vertex_current = (*it);
		++it;
		if( it == polygon.end() )
		{
			it = polygon.begin();
			last_loop = true;
		}
		const carve::geom::vector<3>& vertex_next = (*it);
		polygon_normal[0] += (vertex_current.y-vertex_next.y )*(vertex_current.z+vertex_next.z );
		polygon_normal[1] += (vertex_current.z-vertex_next.z )*(vertex_current.x+vertex_next.x );
		polygon_normal[2] += (vertex_current.x-vertex_next.x )*(vertex_current.y+vertex_next.y );
		if( last_loop )
		{
			break;
		}
	}
	polygon_normal.normalize();
	return polygon_normal;
}

carve::geom::vector<3> GeomUtils::computePolygon2DNormal( const std::vector<carve::geom::vector<2> >& polygon )
{
	const int num_points = polygon.size();
	carve::geom::vector<3> polygon_normal( carve::geom::VECTOR(0, 0, 0) );
	for( int k=0; k<num_points; ++k )
	{
		const carve::geom::vector<2>& vertex_current = polygon[k];
		const carve::geom::vector<2>& vertex_next = polygon[(k+1)%num_points];
		polygon_normal[2] += (vertex_current.x-vertex_next.x )*(vertex_current.y+vertex_next.y );
	}
	polygon_normal.normalize();
	return polygon_normal;
}

bool GeomUtils::checkOpenPolygonConvexity( const std::vector<carve::geom::vector<2> >& polygon )
{
	if( polygon.size() < 3 )
	{
		return true;
	}
	const int num_points = polygon.size();
	double zcrossproduct_previous = 0;
	for( int k=0; k<num_points-2; ++k )
	{
		const carve::geom::vector<2>& vertex_current = polygon[k];
		const carve::geom::vector<2>& vertex_next1 = polygon[k+1];
		const carve::geom::vector<2>& vertex_next2 = polygon[k+2];

		double dx1 = vertex_next1.x - vertex_current.x;
		double dy1 = vertex_next1.y - vertex_current.y;

		double dx2 = vertex_next2.x - vertex_next1.x;
		double dy2 = vertex_next2.y - vertex_next1.y;

		double zcrossproduct = dx1*dy2 - dy1*dx2;

		if( k > 0 )
		{
			if( std::abs(zcrossproduct) > 0.0001 )
			{
				if( std::abs(zcrossproduct_previous) > 0.0001 )
				{
					if( zcrossproduct*zcrossproduct_previous < 0 )
					{
						// there is a change in direction -> not convex
						return false;
					}
				}
			}
		}
		zcrossproduct_previous = zcrossproduct;
	}
	return true;
}

void GeomUtils::computeInverse( const carve::math::Matrix& matrix_a, carve::math::Matrix& inv ) 
{
	int i, j;	// col, row
	int s;		// step
	int prow;	// pivot
	int err_flag = 0;
	double factor;
	const double eps = 0.01;
	double max;
	int pivot = 1;
	double a[4][8];

	a[0][0] = matrix_a._11;
	a[0][1] = matrix_a._12;
	a[0][2] = matrix_a._13;
	a[0][3] = matrix_a._14;

	a[1][0] = matrix_a._21;
	a[1][1] = matrix_a._22;
	a[1][2] = matrix_a._23;
	a[1][3] = matrix_a._24;

	a[2][0] = matrix_a._31;
	a[2][1] = matrix_a._32;
	a[2][2] = matrix_a._33;
	a[2][3] = matrix_a._34;

	a[3][0] = matrix_a._41;
	a[3][1] = matrix_a._42;
	a[3][2] = matrix_a._43;
	a[3][3] = matrix_a._44;

	// append identity at the right
	for( i = 0; i < 4; ++i )
	{
		for( j = 0; j < 4; ++j )
		{
			a[i][4+j] = 0.0;
			if( i == j )
			{
				a[i][4+j] = 1.0;
			}
		}
	}

	s = 0;
	do
	{
		max = std::abs(a[s][s]);
		if( pivot )
		{
			prow = s;
			for( i = s+1; i < 4; ++i )
			{
				if( std::abs(a[i][s]) > max)
				{
					max = std::abs(a[i][s]);
					prow = i;
				}
			}
		}
		err_flag = max < eps;

		if( err_flag ) break;

		if( pivot )
		{
			if( prow != s )
			{
				// change rows
				double temp;
				for( j = s ; j < 2*4; ++j )
				{
					temp = a[s][j];
					a[s][j] = a[prow][j];
					a[prow][j]= temp;
				}
			}
		}

		// elimination: divide by pivot coefficient f = a[s][s]
		factor = a[s][s];
		for( j = s; j < 2*4; ++j )
		{
			a[s][j] = a[s][j] / factor;
		}

		for( i = 0; i < 4; ++i )
		{
			if( i != s )
			{
				factor = -a[i][s];
				for( j = s; j < 2*4 ; ++j )
				{
					a[i][j] += factor*a[s][j];
				}
			}
		}
		++s;
	}
	while( s < 4 ) ;

	if( err_flag )
	{
		throw IfcPPException("cannot compute inverse of matrix", __FUNC__);
	}
	
	inv._11 = a[0][4];
	inv._12 = a[0][5];
	inv._13 = a[0][6];
	inv._14 = a[0][7];
	
	inv._21 = a[1][4];
	inv._22 = a[1][5];
	inv._23 = a[1][6];
	inv._24 = a[1][7];

	inv._31 = a[2][4];
	inv._32 = a[2][5];
	inv._33 = a[2][6];
	inv._34 = a[2][7];

	inv._41 = a[3][4];
	inv._42 = a[3][5];
	inv._43 = a[3][6];
	inv._44 = a[3][7];
}

void GeomUtils::closestPointOnLine( const carve::geom::vector<3>& point, const carve::geom::vector<3>& line_origin, const carve::geom::vector<3>& line_direction, carve::geom::vector<3>& closest )
{
	const double denom = point.x*line_direction.x + point.y*line_direction.y + point.z*line_direction.z - line_direction.x*line_origin.x - line_direction.y*line_origin.y - line_direction.z*line_origin.z;
	const double numer = line_direction.x*line_direction.x + line_direction.y*line_direction.y + line_direction.z*line_direction.z;
	if(numer == 0.0)
	{
		throw IfcPPException("Line is degenerated: the line's direction vector is a null vector!", __FUNC__);
	}
	const double lambda = denom/numer;
	closest = carve::geom::VECTOR( line_origin.x + lambda*line_direction.x, line_origin.y + lambda*line_direction.y, line_origin.z + lambda*line_direction.z );
}

void GeomUtils::closestPointOnLine( const carve::geom::vector<2>& point, const carve::geom::vector<2>& line_origin, const carve::geom::vector<2>& line_direction, carve::geom::vector<2>& closest )
{
	const double denom = point.x*line_direction.x + point.y*line_direction.y + - line_direction.x*line_origin.x - line_direction.y*line_origin.y;
	const double numer = line_direction.x*line_direction.x + line_direction.y*line_direction.y;
	if(numer == 0.0)
	{
		throw IfcPPException("Line is degenerated: the line's direction vector is a null vector!", __FUNC__);
	}
	const double lambda = denom/numer;
	closest = carve::geom::VECTOR( line_origin.x + lambda*line_direction.x, line_origin.y + lambda*line_direction.y );
}

bool LineToLineIntersectionHelper(carve::geom::vector<2>& v1, carve::geom::vector<2>& v2, carve::geom::vector<2>& v3, carve::geom::vector<2>& v4, double & r, double & s)
{
	// check if lines are parallel
	const carve::geom::vector<2> vertex1to2 = v2 - v1;
	const carve::geom::vector<2> vertex3to4 = v4 - v3;
	if( vertex1to2.y / vertex1to2.x != vertex3to4.y / vertex3to4.x )
	{
		const double d = vertex1to2.x*vertex3to4.y - vertex1to2.y*vertex3to4.x;
		if( d != 0 )
		{
			const carve::geom::vector<2> vertex3to1 = v1 - v3;
			r = (vertex3to1.y*vertex3to4.x - vertex3to1.x*vertex3to4.y) / d;
			s = (vertex3to1.y*vertex1to2.x - vertex3to1.x*vertex1to2.y) / d;
			return true;
		}
	}
	return false;

}

bool GeomUtils::LineSegmentToLineIntersection(carve::geom::vector<2>& v1, carve::geom::vector<2>& v2, carve::geom::vector<2>& v3, carve::geom::vector<2>& v4, std::vector<carve::geom::vector<2> >& result )
{
	double r, s;
	if( LineToLineIntersectionHelper(v1, v2, v3, v4, r, s) )
	{
		if (r >= 0 && r <= 1)
		{
			result.push_back(v1 + (v2 - v1) * r);
			return true;
		}
	}
	return false;
}

bool GeomUtils::LineSegmentToLineSegmentIntersection(carve::geom::vector<2>& v1, carve::geom::vector<2>& v2, carve::geom::vector<2>& v3, carve::geom::vector<2>& v4, std::vector<carve::geom::vector<2> >& result )
{
	double r, s;
	if( LineToLineIntersectionHelper(v1, v2, v3, v4, r, s) )
	{
		if (r >= 0 && r <= 1)
		{
			if (s >= 0 && s <= 1)
			{
				result.push_back(v1 + (v2 - v1) * r);
				return true;
			}
		}
	}
	return false;
}

double GeomUtils::distancePoint2Line( const carve::geom::vector<3>& point, const carve::geom::vector<3>& line_p0, const carve::geom::vector<3>& line_p1 )
{
	// d = |(point - line_p0)x(point - line_p1)| / |line_p1 - line_p0|
	return carve::geom::cross( ( point - line_p0 ), ( point - line_p1 ) ).length() / ( line_p1 - line_p0 ).length();
}

void GeomUtils::appendPointsToCurve( const std::vector<carve::geom::vector<2> >& points_vec, std::vector<carve::geom::vector<3> >& target_vec )
{
	// sometimes, sense agreement is not given correctly. try to correct sense of segment if necessary
	//if( target_vec.size() > 0 && points_vec.size() > 1 )
	//{
	//	carve::geom::vector<3> first_target_point = target_vec.front();
	//	carve::geom::vector<3> last_target_point = target_vec.back();

	//	carve::geom::vector<2> first_segment_point = points_vec.front();
	//	carve::geom::vector<2> last_segment_point = points_vec.back();

	//	if( (last_target_point.x - first_segment_point).length2() < 0.000001 )
	//	{
	//		// segment order is as expected, nothing to do
	//	}
	//	else
	//	{
	//		if( (last_target_point-last_segment_point).length() < 0.000001 )
	//		{
	//			// current segment seems to be in wrong order
	//			std::reverse( points_vec.begin(), points_vec.end() );
	//		}
	//		else
	//		{
	//			// maybe the current segment fits to the beginning of the target vector
	//			if( (first_target_point-first_segment_point).length() < 0.000001 )
	//			{
	//				std::reverse( target_vec.begin(), target_vec.end() );
	//			}
	//			else
	//			{
	//				if( (first_target_point-last_segment_point).length() < 0.000001 )
	//				{
	//					std::reverse( target_vec.begin(), target_vec.end() );
	//					std::reverse( points_vec.begin(), points_vec.end() );
	//				}
	//			}
	//		}
	//	}
	//}

	bool omit_first = false;
	if( target_vec.size() > 0 )
	{
		const carve::geom::vector<3>& last_point = target_vec.back();
		const carve::geom::vector<2>& first_point_current_segment = points_vec.front();
		if( std::abs(last_point.x - first_point_current_segment.x) < 0.000001 )
		{
			if( std::abs(last_point.y - first_point_current_segment.y) < 0.000001 )
			{
				omit_first = true;
			}
		}
	}

	if( omit_first )
	{
		//target_vec.insert( target_vec.end(), points_vec.begin()+1, points_vec.end() );
		for( size_t i = 1; i<points_vec.size(); ++i )
		{
			const carve::geom::vector<2>& pt = points_vec[i];
			target_vec.push_back( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
		}
	}
	else
	{
		//target_vec.insert( target_vec.end(), points_vec.begin(), points_vec.end() );
		for( size_t i = 0; i<points_vec.size(); ++i )
		{
			const carve::geom::vector<2>& pt = points_vec[i];
			target_vec.push_back( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
		}
	}
	// TODO: handle all segments separately: std::vector<std::vector<carve::geom::vector<3> > >& target_vec
}

void GeomUtils::appendPointsToCurve( const std::vector<carve::geom::vector<3> >& points_vec_src, std::vector<carve::geom::vector<3> >& target_vec )
{
	// sometimes, sense agreement is not given correctly. try to correct sense of segment if necessary
	std::vector<carve::geom::vector<3> > points_vec( points_vec_src );
	if( target_vec.size() > 0 && points_vec.size() > 1 )
	{
		carve::geom::vector<3> first_target_point = target_vec.front();
		carve::geom::vector<3> last_target_point = target_vec.back();

		carve::geom::vector<3> first_segment_point = points_vec.front();
		carve::geom::vector<3> last_segment_point = points_vec.back();

		if( (last_target_point-first_segment_point).length2() < 0.000001 )
		{
			// segment order is as expected, nothing to do
		}
		else
		{
			if( (last_target_point-last_segment_point).length2() < 0.000001 )
			{
				// current segment seems to be in wrong order
				std::reverse( points_vec.begin(), points_vec.end() );
			}
			else
			{
				// maybe the current segment fits to the beginning of the target vector
				if( (first_target_point-first_segment_point).length2() < 0.000001 )
				{
					std::reverse( target_vec.begin(), target_vec.end() );
				}
				else
				{
					if( (first_target_point-last_segment_point).length2() < 0.000001 )
					{
						std::reverse( target_vec.begin(), target_vec.end() );
						std::reverse( points_vec.begin(), points_vec.end() );
					}
				}
			}
		}
	}

	bool omit_first = false;
	if( target_vec.size() > 0 )
	{
		carve::geom::vector<3> last_point = target_vec.back();
		carve::geom::vector<3> first_point_current_segment = points_vec.front();
		if( (last_point-first_point_current_segment).length() < 0.000001 )
		{
			omit_first = true;
		}
	}

	if( omit_first )
	{
		target_vec.insert( target_vec.end(), points_vec.begin()+1, points_vec.end() );
	}
	else
	{
		target_vec.insert( target_vec.end(), points_vec.begin(), points_vec.end() );
	}
	// TODO: handle all segments separately: std::vector<std::vector<carve::geom::vector<3> > >& target_vec
}


void GeomUtils::makeLookAt(const carve::geom::vector<3>& eye,const carve::geom::vector<3>& center,const carve::geom::vector<3>& up, carve::math::Matrix& resulting_matrix )
{
	carve::geom::vector<3> zaxis = (center - eye).normalize();
	carve::geom::vector<3> xaxis = cross(up, zaxis).normalize();
	carve::geom::vector<3> yaxis = cross(zaxis, xaxis);

	resulting_matrix = carve::math::Matrix(
		xaxis.x,		yaxis.x,		zaxis.x,	0,//-dot(xaxis, eye), // translate.x,
		xaxis.y,		yaxis.y,		zaxis.y,	0,//-dot(yaxis, eye), //translate.y,
		xaxis.z,		yaxis.z,		zaxis.z,	0,//-dot(zaxis, eye), //translate.z,
		0,				0,				0,			1 );
}

bool GeomUtils::bisectingPlane( const carve::geom::vector<3>& v1, const carve::geom::vector<3>& v2, const carve::geom::vector<3>& v3, carve::geom::vector<3>& normal )
{
	bool valid = false;
	carve::geom::vector<3> v21 = v2 - v1;
	carve::geom::vector<3> v32 = v3 - v2;
	double len21_square = v21.length2();
	double len32_square = v32.length2();

	if( len21_square <= carve::EPSILON * len32_square)
	{
		if( len32_square == 0.0)
		{
			// all three points lie ontop of one-another
			normal = carve::geom::VECTOR( 0.0, 0.0, 0.0 );
			valid = false;
		}
		else
		{
			// return a normalized copy of v32 as bisector
			len32_square = 1.0 / len32_square;
			normal = v32*len32_square;
			normal.normalize();
			valid = true;
		}

	}
	else
	{
		valid = true;
		if( len32_square <= carve::EPSILON * len21_square)
		{
			// return v21 as bisector
			v21.normalize();
			normal = v21;
		}
		else
		{
			v21.normalize();
			v32.normalize();

			double dot_product = dot( v32, v21 );
			double dot_product_abs = std::abs( dot_product );

			if( dot_product_abs > (1.0+carve::EPSILON) || dot_product_abs < (1.0-carve::EPSILON) )
			{
				normal = (v32 + v21)*dot_product - v32 - v21;
				normal.normalize();
			}
			else
			{
				// dot == 1 or -1, points are colinear
				normal = -v21;
			}
		}
	}
	return valid;
}

void GeomUtils::convertPlane2Matrix( const carve::geom::vector<3>& plane_normal, const carve::geom::vector<3>& plane_position, 
						 const carve::geom::vector<3>& local_z, carve::math::Matrix& resulting_matrix )
{
	carve::geom::vector<3> local_normal( plane_normal );
	local_normal.normalize();
	carve::geom::vector<3> local_z_new( local_z );
	//local_z_new.normalize();

	carve::geom::vector<3> local_y = cross( local_normal, local_z_new );
	local_y.normalize();
	local_z_new = cross( local_y, local_normal );
	local_z_new.normalize();

	resulting_matrix = carve::math::Matrix(
		local_normal.x,		local_y.x,		local_z_new.x,	plane_position.x,
		local_normal.y,		local_y.y,		local_z_new.y,	plane_position.y,
		local_normal.z,		local_y.z,		local_z_new.z,	plane_position.z,
		0,					0,				0,				1 );

}

void GeomUtils::applyTranslate( osg::Group* grp, const osg::Vec3f& trans, std::unordered_set<osg::Geode*>& set_applied )
{
	int num_children = grp->getNumChildren();
	for( int i=0; i<num_children; ++i )
	{
		osg::Node* node = grp->getChild(i);
		osg::Group* child_group = dynamic_cast<osg::Group*>(node);
		if( child_group )
		{
			applyTranslate( child_group, trans, set_applied );
			continue;
		}
		
		
		osg::Geode* child_geode = dynamic_cast<osg::Geode*>(node);
		if( child_geode )
		{
			if( set_applied.find(child_geode) != set_applied.end() )
			{
				continue;
			}
			set_applied.insert( child_geode );
			const osg::Geode::DrawableList& drawable_list = child_geode->getDrawableList();
			for( osg::Geode::DrawableList::const_iterator it_drawables=drawable_list.begin(); it_drawables!=drawable_list.end(); ++it_drawables )
			{
				osg::Drawable* drawable = (*it_drawables);
				osg::Geometry* child_geometry = dynamic_cast<osg::Geometry*>(drawable);
				if( !child_geometry )
				{
#ifdef _DEBUG
					std::cout << "!child_geometry" << std::endl;
#endif
					return;
				}
				osg::Array* vertices_array = child_geometry->getVertexArray();
				osg::Vec3Array* vertices_float = dynamic_cast<osg::Vec3Array*>(vertices_array);

				if( !vertices_float )
				{
#ifdef _DEBUG
					std::cout << "!vertices_float" << std::endl; 
#endif
					return;
				}

				for( osg::Vec3Array::iterator it_array = vertices_float->begin(); it_array != vertices_float->end(); ++it_array )
				{
					osg::Vec3f& vertex = (*it_array);
					vertex = vertex + trans;
					int wait = 0;
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

void GeomUtils::applyPosition( shared_ptr<carve::input::PolyhedronData>& poly_data, carve::math::Matrix& matrix )
{
	for( std::vector<carve::geom::vector<3> >::iterator it_points = poly_data->points.begin(); it_points != poly_data->points.end(); ++it_points )
	{
		carve::geom::vector<3>& vertex = (*it_points);
		vertex = matrix*vertex;
	}
}


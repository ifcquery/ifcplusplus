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
#include "DebugViewerCallback.h"
#include "GeomUtils.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GeomUtils::WireFrameModeOn( osg::StateSet* state )
{
	osg::ref_ptr<osg::PolygonMode> polygon_mode = dynamic_cast<osg::PolygonMode*>( state->getAttribute( osg::StateAttribute::POLYGONMODE ));
	if(  !polygon_mode )
	{
		polygon_mode = new osg::PolygonMode();
		state->setAttribute( polygon_mode );	
	}
	polygon_mode->setMode(  osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
}

void GeomUtils::WireFrameModeOn( osg::Node* node )
{
	if( node == nullptr )
		return;

	osg::StateSet* state = node->getOrCreateStateSet();
	WireFrameModeOn( state );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GeomUtils::WireFrameModeOff( osg::StateSet* state )
{
	osg::PolygonMode *polygon_mode = dynamic_cast< osg::PolygonMode* >( state->getAttribute( osg::StateAttribute::POLYGONMODE ));

	if(  !polygon_mode )
	{
		polygon_mode = new osg::PolygonMode();
		state->setAttribute( polygon_mode );	
	}
	polygon_mode->setMode(  osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL );
}
void GeomUtils::WireFrameModeOff( osg::Node *srisdNode )
{
	if( srisdNode == nullptr )
		return;

	osg::StateSet *state = srisdNode->getOrCreateStateSet();
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

void GeomUtils::setMaterialTransparent( osg::Node* node, float transparency )
{
	osg::StateSet* stateset = node->getStateSet();
	if( stateset )
	{
		osg::ref_ptr<osg::Material> mat = dynamic_cast<osg::Material*>( stateset->getAttribute( osg::StateAttribute::MATERIAL ));
		if( mat )
		{
			mat->setTransparency( osg::Material::FRONT_AND_BACK, transparency );
			
			const osg::Vec4& ambient = mat->getAmbient( osg::Material::FRONT );
			mat->setAmbient( osg::Material::FRONT, osg::Vec4( ambient.r(), ambient.g(), ambient.b(), transparency ) );

			const osg::Vec4& diffuse = mat->getDiffuse( osg::Material::FRONT );
			mat->setDiffuse( osg::Material::FRONT, osg::Vec4( diffuse.r(), diffuse.g(), diffuse.b(), transparency ) );

			//const osg::Vec4& specular = mat->getSpecular( osg::Material::FRONT );
			//mat->setSpecular( osg::Material::FRONT, osg::Vec4( specular.r(), specular.g(), specular.b(), transparency ) );

			//mat->setShininess( osg::Material::FRONT, 64.f );
			//mat->setColorMode( osg::Material::SPECULAR );
		}
	}
	osg::Group* group = dynamic_cast<osg::Group*>( node );
	if( group )
	{
		for( unsigned int ii=0; ii<group->getNumChildren(); ++ii )
		{
			osg::Node* child_node = group->getChild( ii );
			setMaterialTransparent( child_node, transparency );
		}
	}
}


//#define COORDINATE_AXES_NO_COLORS

osg::ref_ptr<osg::Geode> GeomUtils::createCoordinateAxes()
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
		vertices->push_back( osg::Vec3f( 500.0, 0.0, 0.0 ) );

		vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
		vertices->push_back( osg::Vec3f( 0.0, 500.0, 0.0 ) );

		vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
		vertices->push_back( osg::Vec3f( 0.0, 0.0, 500.0 ) );

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
		vertices->push_back( osg::Vec3f( -500.0, 0.0, 0.0 ) );

		vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
		vertices->push_back( osg::Vec3f( 0.0, -500.0, 0.0 ) );

		vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
		vertices->push_back( osg::Vec3f( 0.0, 0.0, -500.0 ) );

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
		material->setAmbient( osg::Material::FRONT, osg::Vec4f( 0.7f, 0.f, 0.f, 0.7f ) );
		material->setDiffuse( osg::Material::FRONT, osg::Vec4f( 0.7f, 0.f, 0.f, 0.7f ) );
		material->setSpecular( osg::Material::FRONT, osg::Vec4f( 1.f, 0.4f, 0.4f, 0.7f ) );
		material->setShininess( osg::Material::FRONT, 30.0 );
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
		material->setAmbient( osg::Material::FRONT, osg::Vec4f( 0.0f, 0.7f, 0.f, 0.7f ) );
		material->setDiffuse( osg::Material::FRONT, osg::Vec4f( 0.0f, 0.7f, 0.f, 0.7f ) );
		material->setSpecular( osg::Material::FRONT, osg::Vec4f( 0.4f, 1.f, 0.4f, 0.7f ) );
		material->setShininess( osg::Material::FRONT, 30.0 );
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
		material->setAmbient( osg::Material::FRONT, osg::Vec4f( 0.f, 0.f, 0.8f, 0.7f ) );
		material->setDiffuse( osg::Material::FRONT, osg::Vec4f( 0.f, 0.f, 0.8f, 0.7f ) );
		material->setSpecular( osg::Material::FRONT, osg::Vec4f( 0.4f, 0.4f, 1.f, 0.7f ) );
		material->setShininess( osg::Material::FRONT, 30.0 );
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

void GeomUtils::createFace( const std::vector<std::vector<carve::geom::vector<3> > >& vec_bounds, PolyInputCache3D& poly_cache, std::stringstream& err )
{
	std::vector<std::vector<carve::geom2d::P2> > face_loops_2d;
	std::vector<std::vector<double> > face_loop_3rd_dim;
	std::map<int,int> map_merged_idx;
	bool face_loop_reversed = false;
	ProjectionPlane face_plane = UNDEFINED;

	for( auto it_bounds=vec_bounds.begin(); it_bounds!=vec_bounds.end(); ++it_bounds )
	{
		const std::vector<carve::geom::vector<3> >& loop_points = *it_bounds;

		if( loop_points.size() < 3 )
		{
			if( it_bounds == vec_bounds.begin() )
			{
				break;
			}
			else
			{
				continue;
			}
		}

		if( loop_points.size() == 3 && vec_bounds.size() == 1 )
		{
			std::vector<int> triangle_indexes;
			for( int point_i = 0; point_i < 3; ++point_i )
			{
				carve::geom::vector<3> v = loop_points[point_i];
				int vertex_id = poly_cache.addPoint( v );
				triangle_indexes.push_back( vertex_id );
				map_merged_idx[point_i] = vertex_id;
			}

			if( triangle_indexes.size() != 3 )
			{
				std::cout << __FUNC__ << ": triangle_indexes.size() != 3" << std::endl;
				continue;
			}

			poly_cache.m_poly_data->addFace( triangle_indexes[0], triangle_indexes[1], triangle_indexes[2] );
			continue;
		}

		carve::geom3d::Vector normal = GeomUtils::computePolygonNormal( loop_points );
		if( it_bounds == vec_bounds.begin() )
		{
			double nx = std::abs(normal.x);
			double ny = std::abs(normal.y);
			double nz = std::abs(normal.z);
			if( nz > nx && nz >= ny )
			{
				face_plane = XY_PLANE;
			}
			else if( nx >= ny && nx >= nz )
			{
				face_plane = YZ_PLANE;
			}
			else if( ny > nx && ny >= nz )
			{
				face_plane = XZ_PLANE;
			}
			else
			{
				err << __FUNC__ << ": unable to project to plane: nx" << nx << " ny " << ny << " nz " << nz << std::endl;
				continue;
			}
		}

		// project face into 2d plane
		std::vector<carve::geom2d::P2> path_loop_2d;
		std::vector<double> path_loop_3rd_dim;

		for( int i=0; i<loop_points.size(); ++i )
		{
			const carve::geom::vector<3>& point = loop_points[i];

			if( face_plane == XY_PLANE )
			{
				path_loop_2d.push_back( carve::geom::VECTOR(point.x, point.y ));
				path_loop_3rd_dim.push_back(point.z);
			}
			else if( face_plane == YZ_PLANE )
			{
				path_loop_2d.push_back( carve::geom::VECTOR(point.y, point.z ));
				path_loop_3rd_dim.push_back(point.x);
			}
			else if( face_plane == XZ_PLANE )
			{
				path_loop_2d.push_back( carve::geom::VECTOR(point.x, point.z ));
				path_loop_3rd_dim.push_back(point.y);
			}
		}

		if( loop_points.size() == 4 && vec_bounds.size() == 1 )
		{
			if( carve::geom2d::quadIsConvex( path_loop_2d[0], path_loop_2d[1], path_loop_2d[2], path_loop_2d[3] ) )
			{
				// add 2 triangles for quad
				std::vector<int> triangle_indexes;
				for( int point_i = 0; point_i < 4; ++point_i )
				{
					carve::geom::vector<3> v = loop_points[point_i];

					int vertex_index = poly_cache.addPoint( v );
					map_merged_idx[point_i] = vertex_index;
					triangle_indexes.push_back( vertex_index );
				}

				if( triangle_indexes.size() != 4 )
				{
					std::cout << __FUNC__ << ": triangle_indexes.size() != 4" << std::endl;
					continue;
				}

				poly_cache.m_poly_data->addFace( triangle_indexes[0], triangle_indexes[1], triangle_indexes[2] );
				poly_cache.m_poly_data->addFace( triangle_indexes[2], triangle_indexes[3], triangle_indexes[0] );
				continue;
			}
		}

		// check winding order
		carve::geom3d::Vector normal_2d = GeomUtils::computePolygon2DNormal( path_loop_2d );
		if( it_bounds == vec_bounds.begin() )
		{
			if( normal_2d.z < 0 )
			{
				std::reverse( path_loop_2d.begin(), path_loop_2d.end() );
				face_loop_reversed = true;
			}
		}
		else
		{
			if( normal_2d.z > 0 )
			{
				std::reverse( path_loop_2d.begin(), path_loop_2d.end() );
			}
		}

		if( path_loop_2d.size() < 3 )
		{
			//std::cout << __FUNC__ << ": #" << face_id <<  "=IfcFace: path_loop.size() < 3" << std::endl;
			continue;
		}

		face_loops_2d.push_back(path_loop_2d);
		face_loop_3rd_dim.push_back(path_loop_3rd_dim);
	}

	if( face_loops_2d.size() > 0 )
	{
		std::vector<std::pair<size_t, size_t> > result; // first is loop index, second is vertex index in loop
		std::vector<carve::geom2d::P2> merged;
		std::vector<carve::geom3d::Vector> merged_3d;
		std::vector<carve::triangulate::tri_idx> triangulated;

		try
		{
			result = carve::triangulate::incorporateHolesIntoPolygon(face_loops_2d);
			merged.reserve(result.size());
			for( size_t i = 0; i < result.size(); ++i )
			{
				int loop_number = result[i].first;
				int index_in_loop = result[i].second;
				carve::geom2d::P2& loop_point = face_loops_2d[loop_number][index_in_loop];
				merged.push_back( loop_point );

				// restore 3rd dimension
				if( face_loop_reversed )
				{
					index_in_loop = face_loops_2d[loop_number].size() - index_in_loop - 1;
				}

				carve::geom3d::Vector v;
				if( face_plane == XY_PLANE )
				{
					double z = face_loop_3rd_dim[loop_number][index_in_loop];
					v = carve::geom::VECTOR(        loop_point.x,   loop_point.y,   z);
				}
				else if( face_plane == YZ_PLANE )
				{
					double x = face_loop_3rd_dim[loop_number][index_in_loop];
					v = carve::geom::VECTOR(        x,      loop_point.x,   loop_point.y);
				}
				else if( face_plane == XZ_PLANE )
				{
					double y = face_loop_3rd_dim[loop_number][index_in_loop];
					v = carve::geom::VECTOR(        loop_point.x,   y,      loop_point.y);
				}
				merged_3d.push_back( v );
			}
			carve::triangulate::triangulate(merged, triangulated);
			carve::triangulate::improve(merged, triangulated);

		}
		catch(...)
		{
			err << __FUNC__ << ": carve::triangulate::incorporateHolesIntoPolygon failed " << std::endl;
			return;
		}

		// now insert points to polygon, avoiding points with same coordinates
		for( size_t i = 0; i != merged.size(); ++i )
		{
			const carve::geom::vector<3>& v = merged_3d[i];
			
			int vertex_index = poly_cache.addPoint( v );
			map_merged_idx[i] = vertex_index;
		}
		for( size_t i = 0; i != triangulated.size(); ++i )
		{
			carve::triangulate::tri_idx triangle = triangulated[i];
			int a = triangle.a;
			int b = triangle.b;
			int c = triangle.c;

			int vertex_id_a = map_merged_idx[a];
			int vertex_id_b = map_merged_idx[b];
			int vertex_id_c = map_merged_idx[c];

			if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
			{
				continue;
			}

#ifdef _DEBUG
			const carve::poly::Vertex<3>& v_a = poly_cache.m_poly_data->getVertex(vertex_id_a);
			const carve::poly::Vertex<3>& v_b = poly_cache.m_poly_data->getVertex(vertex_id_b);

			double dx = v_a.v[0] - v_b.v[0];
			if( std::abs(dx) < 0.0000001 )
			{
				double dy = v_a.v[1] - v_b.v[1];
				if( std::abs(dy) < 0.0000001 )
				{
					double dz = v_a.v[2] - v_b.v[2];
					if( std::abs(dz) < 0.0000001 )
					{
						std::cerr << "abs(dx) < 0.00001 && abs(dy) < 0.00001 && abs(dz) < 0.00001\n";
					}
				}
			}
#endif

			if( face_loop_reversed )
			{
				poly_cache.m_poly_data->addFace( vertex_id_a, vertex_id_c, vertex_id_b );
			}
			else
			{
				poly_cache.m_poly_data->addFace( vertex_id_a, vertex_id_b, vertex_id_c );
			}
		}
	}
}

void GeomUtils::extrude( const std::vector<std::vector<carve::geom::vector<2> > >& face_loops_input, const carve::geom::vector<3> extrusion_vector, shared_ptr<carve::input::PolyhedronData>& poly_data, std::stringstream& err )
{
	if( face_loops_input.size() == 0 )
	{
		std::cout << "extrude: face_loops_input.size() == 0" << std::endl;
		return;
	}
	if( poly_data->points.size() > 0 )
	{
		std::cout << "extrude: points vec should be empty" << std::endl;
		return;
	}
	if( poly_data->getFaceCount() > 0 )
	{
		std::cout << "extrude: PolyhedronData::faceCount should be 0" << std::endl;
		return;
	}

	// figure 1: loops and indexes
	//  3----------------------------2
	//  |                            |
	//  |   1-------------------2    |3---------2
	//  |   |                   |    |          |
	//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
	//  |   0---face_loops[1]---3    |0---------1
	//  |                            |
	//  0-------face_loops[0]--------1

	carve::geom::vector<3> normal_first_loop;
	std::vector<std::vector<carve::geom2d::P2> >	face_loops;
	for( std::vector<std::vector<carve::geom::vector<2> > >::const_iterator it_face_loops = face_loops_input.begin(); it_face_loops != face_loops_input.end(); ++it_face_loops )
	{
		const std::vector<carve::geom::vector<2> >& loop = (*it_face_loops);

		if( loop.size() < 3 )
		{
			err << "loop.size() < 3" << std::endl;
			if( it_face_loops == face_loops_input.begin() )
			{
				break;
			}
			else
			{
				continue;
			}
		}

		// check winding order
		bool reverse_loop = false;
		std::vector<carve::geom2d::P2> loop_2d( loop );
		carve::geom::vector<3>  normal_2d = computePolygon2DNormal( loop_2d );
		if( it_face_loops == face_loops_input.begin() )
		{
			normal_first_loop = normal_2d;
			if( normal_2d.z < 0 )
			{
				reverse_loop = true;
				normal_first_loop = -normal_first_loop;
			}
		}
		else
		{
			if( normal_2d.z > 0 )
			{
				reverse_loop = true;
			}
		}
		if( reverse_loop )
		{
			std::reverse( loop_2d.begin(), loop_2d.end() );
		}
				
		if( loop_2d.size() < 3 )
		{
			err << "extrude: loop_2d.size() < 3" << std::endl;
		}
		
		// close loop, insert first point at end if not already there
//		while( loop_2d.size() > 2 )
		{
			carve::geom::vector<2> first = loop_2d.front();
			carve::geom::vector<2>& last = loop_2d.back();

			if( std::abs(first.x-last.x) > 0.00001 || std::abs(first.y-last.y) > 0.00001 )
			{
				loop_2d.push_back( first );
			}
		}

		face_loops.push_back(loop_2d);
	}

	bool flip_faces = false;
	double extrusion_dot_normal = dot( extrusion_vector, normal_first_loop );
	if( extrusion_dot_normal > 0 )
	{
		flip_faces = true;
	}

	// triangulate
	std::vector<carve::geom2d::P2> merged_path;
	std::vector<carve::triangulate::tri_idx> triangulated;
	std::vector<std::pair<size_t, size_t> > path_all_loops;
	try
	{
		path_all_loops = carve::triangulate::incorporateHolesIntoPolygon(face_loops);
		// figure 2: path wich incorporates holes, described by path_all_loops
		// (0/0) -> (1/3) -> (1/0) -> (1/1) -> (1/2) -> (1/3) -> (0/0) -> (0/1) -> (0/2) -> (0/3)
		//  0/3<-----------------------0/2
		//  |                            ^
		//  |   1/0-------------->1/1    |
		//  |   ^                   |    |
		//  |   |                   v    |
		//  |   1/3<--------------1/2    |
		//  v                            |
		//  0/0------------------------>0/1

		merged_path.reserve(path_all_loops.size());
		for( size_t i = 0; i < path_all_loops.size(); ++i )
		{
			int loop_number = path_all_loops[i].first;
			int index_in_loop = path_all_loops[i].second;
			
			if( loop_number >= face_loops.size() )
			{
				err << __FUNCTION__ << ": loop_number >= face_loops.size()" << std::endl;
				continue;
			}
			std::vector<carve::geom2d::P2>& loop = face_loops[loop_number];
			carve::geom2d::P2& point_in_loop = loop[index_in_loop];
			merged_path.push_back( point_in_loop );

		}
		// figure 3: merged path for triangulation
		//  9<---------------------------8
		//  |                            ^
		//  |   2------------------>3    |
		//  |   ^                   |    |
		//  |   |                   v    |
		//  |   1, 5<---------------4    |
		//  | /                          |
		//  0,6------------------------->7
		carve::triangulate::triangulate(merged_path, triangulated);
		carve::triangulate::improve(merged_path, triangulated);
		// triangles: (9,0,1)  (5,6,7)  (4,5,7)  (4,7,8)  (9,1,2)  (8,9,2)  (3,4,8)  (2,3,8)
	}
	catch(...)
	{
		err << "carve::triangulate::incorporateHolesIntoPolygon failed " << std::endl;
		return;
	}

	// now insert points to polygon, avoiding points with same coordinates
	std::map<double, std::map<double, int> > existing_vertices_coords;
	std::map<double, int>::iterator it_find_y;

	std::map<int,int> map_merged_idx;
	for( size_t i = 0; i != merged_path.size(); ++i )
	{
		const carve::geom::vector<2>&  v = merged_path[i];
		
#ifdef ROUND_IFC_COORDINATES
		const double vertex_x = round(v.x*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		const double vertex_y = round(v.y*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		const double vertex_x = v.x;
		const double vertex_y = v.y;
#endif

		//  return a pair, with its member pair::first set to an iterator pointing to either the newly inserted element or to the element with an equivalent key in the map
		std::map<double, int>& map_y_index = existing_vertices_coords.insert( std::make_pair(vertex_x, std::map<double,int>() ) ).first->second;

		it_find_y = map_y_index.find( vertex_y );
		if( it_find_y != map_y_index.end() )
		{
			// vertex already exists in polygon. remember its index for triangles
			map_merged_idx[i] = it_find_y->second;
			continue;
		}

		carve::geom::vector<3>  vertex3D( carve::geom::VECTOR( v.x, v.y, 0 ) );
		int vertex_id = poly_data->addVertex(vertex3D);
		map_y_index[vertex_y] = vertex_id;  // TODO: it works for now, but check if we have to round here. maybe use checksum of rounded x and y as a single map key
		map_merged_idx[i] = vertex_id;
	}

	// figure 4: points in poly_data (merged path without duplicate vertices):
	//  7<---------------------------6
	//  |                            ^
	//  |   2------------------>3    |
	//  |   ^                   |    |     map_merged_idx: figure 3 -> figure 4
	//  |   |                   v    |
	//  |   1<------------------4    |
	//  | /                          |
	//  0--------------------------->5

	// copy points from base to top
	std::vector<carve::geom::vector<3> >& points = poly_data->points;
	const int num_points_base = points.size();
	for( int i=0; i<num_points_base; ++i )
	{
		carve::geom::vector<3>& pt = points[i];
		poly_data->addVertex( pt + extrusion_vector );
	}

	// collect vertex indexes of loops
	std::map<int, std::vector<int> > loop_vert_idx;
	for( size_t merged_idx = 0; merged_idx < path_all_loops.size(); ++merged_idx )
	{
		int loop_number = path_all_loops[merged_idx].first;
		int point_idx_merged = map_merged_idx[merged_idx];
	
		std::map<int, std::vector<int> >::iterator it_result_loops = loop_vert_idx.insert( std::make_pair( loop_number, std::vector<int>() ) ).first;
		std::vector<int>& result_loop_vec = it_result_loops->second;
		
		// check if point index is already in loop
		bool already_in_loop = false;
		for( int i2 = 0; i2 < result_loop_vec.size(); ++i2 )
		{
			if( point_idx_merged == result_loop_vec[i2] )
			{
				already_in_loop = true;
				break;
			}
		}
		if( !already_in_loop )
		{
			result_loop_vec.push_back( point_idx_merged );
		}
	}

	// add faces along outer and inner loops
	for( std::map<int, std::vector<int> >::iterator it_result_loop = loop_vert_idx.begin(); it_result_loop != loop_vert_idx.end(); ++it_result_loop )
	{
		const std::vector<int>& loop_idx = it_result_loop->second;
		const int num_points_in_loop = loop_idx.size();
		
		for( int i=0; i<num_points_in_loop; ++i )
		{
			int point_idx		= loop_idx[i];
			int point_idx_next	= loop_idx[(i+1)%num_points_in_loop];
			int point_idx_up = point_idx + num_points_base;
			int point_idx_next_up = point_idx_next + num_points_base;

			if( point_idx_next_up >= 2*num_points_base )
			{
				std::cout << "point_idx_next_up >= 2*num_points_base" << std::endl;
				continue;
			}
			if( flip_faces )
			{
				poly_data->addFace( point_idx,			point_idx_next,		point_idx_next_up );
				poly_data->addFace( point_idx_next_up,	point_idx_up,		point_idx );
			}
			else
			{
				poly_data->addFace( point_idx,			point_idx_up,		point_idx_next_up );
				poly_data->addFace( point_idx_next_up,	point_idx_next,		point_idx );
			}
		}
	}

	// now the triangulated bottom and top cap
	for( size_t i = 0; i != triangulated.size(); ++i )
	{
		carve::triangulate::tri_idx triangle = triangulated[i];
		int a = triangle.a;
		int b = triangle.b;
		int c = triangle.c;

		int vertex_id_a = map_merged_idx[a];
		int vertex_id_b = map_merged_idx[b];
		int vertex_id_c = map_merged_idx[c];

		if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
		{
			continue;
		}

		int vertex_id_a_top = vertex_id_a + num_points_base;
		int vertex_id_b_top = vertex_id_b + num_points_base;
		int vertex_id_c_top = vertex_id_c + num_points_base;

#ifdef _DEBUG
		const carve::poly::Vertex<3>& v_a = poly_data->getVertex(vertex_id_a);
		const carve::poly::Vertex<3>& v_b = poly_data->getVertex(vertex_id_b);
		const carve::poly::Vertex<3>& v_c = poly_data->getVertex(vertex_id_c);

		carve::geom::vector<3> pa( carve::geom::VECTOR( v_a.v[0],	v_a.v[1],	v_a.v[2] ) );
		carve::geom::vector<3> pb( carve::geom::VECTOR( v_b.v[0],	v_b.v[1],	v_b.v[2] ) );
		carve::geom::vector<3> pc( carve::geom::VECTOR( v_c.v[0],	v_c.v[1],	v_c.v[2] ) );

		double A = 0.5*(cross( pa-pb, pa-pc ).length());
		if( std::abs(A) < 0.000000001 )
		{
			std::cout << "area < 0.000000001\n" << std::endl;
		}
#endif

		if( flip_faces )
		{
			poly_data->addFace( vertex_id_a,		vertex_id_c,		vertex_id_b );		// bottom cap
			poly_data->addFace( vertex_id_a_top,	vertex_id_b_top,	vertex_id_c_top );	// top cap, flipped outward
		}
		else
		{
			poly_data->addFace( vertex_id_a,		vertex_id_b,		vertex_id_c );		// bottom cap
			poly_data->addFace( vertex_id_a_top,	vertex_id_c_top,	vertex_id_b_top );	// top cap, flipped outward
		}
	}
}

void GeomUtils::sweepDisk( std::vector<carve::geom::vector<3> >& basis_curve_points, shared_ptr<ItemData>& item_data_solid, const int nvc, const double radius, const double radius_inner )
{
	carve::geom::vector<3> local_z( carve::geom::VECTOR( 0, 0, 1 ) );
	const int num_curve_points = basis_curve_points.size();
	if( num_curve_points < 2 )
	{
		std::cout << "IfcSweptDiskSolid: num curve points < 2" << std::endl;
		return;
	}

	shared_ptr<carve::input::PolyhedronData> pipe_data( new carve::input::PolyhedronData() );
	item_data_solid->closed_polyhedrons.push_back( pipe_data );
	carve::geom::vector<3>  curve_point_first = basis_curve_points[0];
	carve::geom::vector<3>  curve_point_second = basis_curve_points[1];

	bool bend_found = false;
	if( num_curve_points > 3 )
	{
		// compute local z vector by dot product of the first bend of the reference line
		carve::geom::vector<3> vertex_back2 = curve_point_first;
		carve::geom::vector<3> vertex_back1 = curve_point_second;
		for( int i=2; i<num_curve_points; ++i )
		{
			carve::geom::vector<3>& vertex_current = basis_curve_points[i];
			carve::geom::vector<3> section1 = vertex_back1 - vertex_back2;
			carve::geom::vector<3> section2 = vertex_current - vertex_back1;
			section1.normalize();
			section2.normalize();

			double dot_product = dot( section1, section2 );
			double dot_product_abs = std::abs(dot_product);

			// if dot == 1 or -1, then points are colinear
			if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
			{
				// bend found, compute cross product
				carve::geom::vector<3> lateral_vec = cross( section1, section2 );
				local_z = cross( lateral_vec, section1 );
				local_z.normalize();
				bend_found = true;
				break;
			}
		}
	}

	if( !bend_found )
	{
		// sweeping curve is linear. assume any local z vector
		carve::geom::vector<3> sweep_dir = curve_point_second - curve_point_first;
		sweep_dir.normalize();
		local_z = cross( carve::geom::VECTOR( 0, 0, 1 ), sweep_dir );
		if( local_z.length2() < 0.001 )
		{
			local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
			local_z.normalize();
		}
		else
		{
			local_z.normalize();
		}
		double dot_normal_local_z = dot( sweep_dir, local_z );
		if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
		{
			local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
			local_z.normalize();

			dot_normal_local_z = dot( sweep_dir, local_z );
			if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
			{
				local_z = cross( carve::geom::VECTOR( 1, 0, 0 ), sweep_dir );
				local_z.normalize();
			}
		}
	}

	// rotate disc into first direction
	carve::geom::vector<3>  section_local_y = local_z;
	carve::geom::vector<3>  section_local_z = curve_point_first - curve_point_second;
	carve::geom::vector<3>  section_local_x = carve::geom::cross( section_local_y, section_local_z );
	section_local_y = carve::geom::cross( section_local_x, section_local_z );
	std::vector<carve::geom::vector<3> > inner_shape_points;

	
	section_local_x.normalize();
	section_local_y.normalize();
	section_local_z.normalize();

	carve::math::Matrix matrix_first_direction = carve::math::Matrix(
		section_local_x.x,		section_local_y.x,		section_local_z.x,	0,
		section_local_x.y,		section_local_y.y,		section_local_z.y,	0,
		section_local_x.z,		section_local_y.z,		section_local_z.z,	0,
		0,				0,				0,			1 );

	double angle = 0;
	double delta_angle = 2.0*M_PI/double(nvc);	// TODO: adapt to model size and complexity
	std::vector<carve::geom::vector<3> > circle_points;
	std::vector<carve::geom::vector<3> > circle_points_inner;
	for( int i = 0; i < nvc; ++i )
	{
		// cross section (circle) is defined in XY plane
		double x = sin(angle);
		double y = cos(angle);
		carve::geom::vector<3> vertex( carve::geom::VECTOR( x*radius, y*radius, 0.0 ) );
		vertex = matrix_first_direction*vertex + curve_point_first;
		circle_points.push_back( vertex );

		if( radius_inner > 0 )
		{
			carve::geom::vector<3> vertex_inner( carve::geom::VECTOR( x*radius_inner, y*radius_inner, 0.0 ) );
			vertex_inner = matrix_first_direction*vertex_inner + curve_point_first;
			circle_points_inner.push_back( vertex_inner );
		}
		angle += delta_angle;
	}

	for( int ii=0; ii<num_curve_points; ++ii )
	{
		carve::geom::vector<3> vertex_current = basis_curve_points[ii];
		carve::geom::vector<3> vertex_next;
		carve::geom::vector<3> vertex_before;
		if( ii == 0 )
		{
			// first point
			vertex_next	= basis_curve_points[ii+1];
			carve::geom::vector<3> delta_element = vertex_next - vertex_current;
			vertex_before = vertex_current - (delta_element);
		}
		else if( ii == num_curve_points-1 )
		{
			// last point
			vertex_before	= basis_curve_points[ii-1];
			carve::geom::vector<3> delta_element = vertex_current - vertex_before;
			vertex_next = vertex_before + (delta_element);
		}
		else
		{
			// inner point
			vertex_next		= basis_curve_points[ii+1];
			vertex_before	= basis_curve_points[ii-1];
		}

		carve::geom::vector<3> bisecting_normal;
		GeomUtils::bisectingPlane( vertex_before, vertex_current, vertex_next, bisecting_normal );

		carve::geom::vector<3> section1 = vertex_current - vertex_before;
		carve::geom::vector<3> section2 = vertex_next - vertex_current;
		section1.normalize();
		section2.normalize();
		double dot_product = dot( section1, section2 );
		double dot_product_abs = std::abs(dot_product);

		if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
		{
			// bend found, compute next local z vector
			carve::geom::vector<3> lateral_vec = cross( section1, section2 );
			local_z = cross( lateral_vec, section1 );
			local_z.normalize();
		}
		if( ii == num_curve_points -1 )
		{
			bisecting_normal *= -1.0;
		}

		carve::geom::plane<3> bisecting_plane( bisecting_normal, vertex_current );
		for( int jj = 0; jj < nvc; ++jj )
		{
			carve::geom::vector<3>& vertex = circle_points[jj];

			carve::geom::vector<3> v;
			double t;
			carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t);
			if( intersect > 0 )
			{
				vertex = v;
			}
			else
			{
				std::cout << "no intersection found" << std::endl;
			}

			pipe_data->addVertex( vertex );
		}

		if( radius_inner > 0 )
		{
			for( int jj = 0; jj < nvc; ++jj )
			{
				carve::geom::vector<3>& vertex = circle_points_inner[jj];
					
				carve::geom::vector<3> v;
				double t;
				carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t);
				if( intersect > 0 )
				{
					vertex = v;
				}
				else
				{
					std::cout << "no intersection found" << std::endl;
				}

				inner_shape_points.push_back( vertex );
			}
		}
	}

	// outer shape
	size_t num_vertices_outer = pipe_data->getVertexCount();
	for( size_t i=0; i<num_curve_points- 1; ++i )
	{
		int i_offset = i*nvc;
		int i_offset_next = (i+1)*nvc;
		for( int jj = 0; jj < nvc; ++jj )
		{
			int current_loop_pt1 = jj + i_offset;
			int current_loop_pt2 = (jj + 1)%nvc + i_offset;

			int next_loop_pt1 = jj + i_offset_next;
			int next_loop_pt2 = (jj + 1)%nvc + i_offset_next;
			//pipe_data->addFace( current_loop_pt1, next_loop_pt1, next_loop_pt2, current_loop_pt2 );
			pipe_data->addFace( current_loop_pt1,	next_loop_pt1,		next_loop_pt2 );
			pipe_data->addFace( next_loop_pt2,		current_loop_pt2,	current_loop_pt1  );
		}
	}

	if( radius_inner > 0 )
	{
		if( inner_shape_points.size() != num_vertices_outer )
		{
			std::cout << "IfcSweptDiskSolid: inner_shape_points.size() != num_vertices_outer" << std::endl;
		}

		// add points for inner shape
		for( size_t i=0; i<inner_shape_points.size(); ++i )
		{
			pipe_data->addVertex( inner_shape_points[i] );
		}

		// faces of inner shape
		for( size_t i=0; i<num_curve_points- 1; ++i )
		{
			int i_offset = i*nvc + num_vertices_outer;
			int i_offset_next = (i+1)*nvc + num_vertices_outer;
			for( int jj = 0; jj < nvc; ++jj )
			{
				int current_loop_pt1 = jj + i_offset;
				int current_loop_pt2 = (jj + 1)%nvc + i_offset;

				int next_loop_pt1 = jj + i_offset_next;
				int next_loop_pt2 = (jj + 1)%nvc + i_offset_next;
				//pipe_data->addFace( current_loop_pt1, current_loop_pt2, next_loop_pt2, next_loop_pt1 );  
				pipe_data->addFace( current_loop_pt1,	current_loop_pt2,	next_loop_pt2 );
				pipe_data->addFace( next_loop_pt2,		next_loop_pt1,		current_loop_pt1  );
			}
		}

		// front cap
		for( int jj = 0; jj < nvc; ++jj )
		{
			int outer_rim_next = (jj+1)%nvc;
			int inner_rim_next = outer_rim_next + num_vertices_outer;
			pipe_data->addFace( jj,					outer_rim_next,		num_vertices_outer+jj );
			pipe_data->addFace( outer_rim_next,		inner_rim_next,		num_vertices_outer+jj );
		}

		// back cap
		int back_offset = (num_curve_points - 1)*nvc;
		for( int jj = 0; jj < nvc; ++jj )
		{
			int outer_rim_next = (jj+1)%nvc + back_offset;
			int inner_rim_next = outer_rim_next + num_vertices_outer;
			pipe_data->addFace( jj+back_offset,		num_vertices_outer+jj+back_offset,	outer_rim_next );
			pipe_data->addFace( outer_rim_next,		num_vertices_outer+jj+back_offset,	inner_rim_next );
		}
	}
	else
	{
		// front cap, full pipe, create triangle fan
		for( int jj = 0; jj < nvc - 2; ++jj )
		{
			pipe_data->addFace( 0, jj+1, jj+2 );
		}

		// back cap
		int back_offset = (num_curve_points - 1)*nvc;
		for( int jj = 0; jj < nvc - 2; ++jj )
		{
			pipe_data->addFace( back_offset, back_offset+jj+2, back_offset+jj+1 );
		}
	}

#ifdef _DEBUG
	std::stringstream strs_err;
	shared_ptr<carve::mesh::MeshSet<3> > meshset( pipe_data->createMesh(carve::input::opts()) );
	bool poly_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), strs_err, -1 );

	if( !poly_ok )
	{
		std::cout << strs_err.str().c_str() << std::endl;
	}
#endif
}

void GeomUtils::sweepArea( const std::vector<carve::geom::vector<3> >& curve_points, const std::vector<std::vector<carve::geom::vector<2> > >& profile_paths, shared_ptr<ItemData>& item_data_solid )
{
	// TODO: complete and test
	std::stringstream err;
	if( profile_paths.size() == 0 )
	{
		std::cout << __FUNC__ << ": profile_paths.size() == 0" << std::endl;
		return;
	}
	if( !item_data_solid->isEmpty() )
	{
		std::cout << __FUNC__ << ": ItemData should be empty" << std::endl;
		return;
	}

	// figure 1: loops and indexes
	//  3----------------------------2
	//  |                            |
	//  |   1-------------------2    |3---------2
	//  |   |                   |    |          |
	//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
	//  |   0---face_loops[1]---3    |0---------1
	//  |                            |
	//  0-------face_loops[0]--------1

	carve::geom::vector<3> normal_first_loop;
	std::vector<std::vector<carve::geom2d::P2> >	face_loops;
	for( std::vector<std::vector<carve::geom::vector<2> > >::const_iterator it_face_loops = profile_paths.begin(); it_face_loops != profile_paths.end(); ++it_face_loops )
	{
		const std::vector<carve::geom::vector<2> >& loop = (*it_face_loops);

		if( loop.size() < 3 )
		{
			err << "loop.size() < 3" << std::endl;
			if( it_face_loops == profile_paths.begin() )
			{
				break;
			}
			else
			{
				continue;
			}
		}

		// check winding order
		bool reverse_loop = false;
		std::vector<carve::geom2d::P2> loop_2d( loop );
		carve::geom::vector<3>  normal_2d = computePolygon2DNormal( loop_2d );
		if( it_face_loops == profile_paths.begin() )
		{
			normal_first_loop = normal_2d;
			if( normal_2d.z < 0 )
			{
				reverse_loop = true;
				normal_first_loop = -normal_first_loop;
			}
		}
		else
		{
			if( normal_2d.z > 0 )
			{
				reverse_loop = true;
			}
		}
		if( reverse_loop )
		{
			std::reverse( loop_2d.begin(), loop_2d.end() );
		}
				
		if( loop_2d.size() < 3 )
		{
			err << "extrude: loop_2d.size() < 3" << std::endl;
		}
		
		// close loop, insert first point at end if not already there
//		while( loop_2d.size() > 2 )
		{
			carve::geom::vector<2> first = loop_2d.front();
			carve::geom::vector<2>& last = loop_2d.back();

			if( std::abs(first.x-last.x) > 0.00001 || std::abs(first.y-last.y) > 0.00001 )
			{
				loop_2d.push_back( first );
			}
		}

		face_loops.push_back(loop_2d);
	}

	// triangulate
	std::vector<carve::geom2d::P2> merged_path;
	std::vector<carve::triangulate::tri_idx> triangulated;
	std::vector<std::pair<size_t, size_t> > path_all_loops;
	try
	{
		path_all_loops = carve::triangulate::incorporateHolesIntoPolygon(face_loops);
		// figure 2: path wich incorporates holes, described by path_all_loops
		// (0/0) -> (1/3) -> (1/0) -> (1/1) -> (1/2) -> (1/3) -> (0/0) -> (0/1) -> (0/2) -> (0/3)
		//  0/3<-----------------------0/2
		//  |                            ^
		//  |   1/0-------------->1/1    |
		//  |   ^                   |    |
		//  |   |                   v    |
		//  |   1/3<--------------1/2    |
		//  v                            |
		//  0/0------------------------>0/1

		merged_path.reserve(path_all_loops.size());
		for( size_t i = 0; i < path_all_loops.size(); ++i )
		{
			int loop_number = path_all_loops[i].first;
			int index_in_loop = path_all_loops[i].second;
			
			if( loop_number >= face_loops.size() )
			{
				err << __FUNC__ << ": loop_number >= face_loops.size()" << std::endl;
				continue;
			}
			std::vector<carve::geom2d::P2>& loop = face_loops[loop_number];
			carve::geom2d::P2& point_in_loop = loop[index_in_loop];
			merged_path.push_back( point_in_loop );

		}
		// figure 3: merged path for triangulation
		//  9<---------------------------8
		//  |                            ^
		//  |   2------------------>3    |
		//  |   ^                   |    |
		//  |   |                   v    |
		//  |   1, 5<---------------4    |
		//  | /                          |
		//  0,6------------------------->7
		carve::triangulate::triangulate(merged_path, triangulated);
		carve::triangulate::improve(merged_path, triangulated);
		// triangles: (9,0,1)  (5,6,7)  (4,5,7)  (4,7,8)  (9,1,2)  (8,9,2)  (3,4,8)  (2,3,8)
	}
	catch(...)
	{
		err << "carve::triangulate::incorporateHolesIntoPolygon failed " << std::endl;
		return;
	}

	// now insert points to polygon, avoiding points with same coordinates

	PolyInputCache3D poly_cache;


//	std::map<double, std::map<double, int> > existing_vertices_coords;
//	std::map<double, int>::iterator it_find_y;
//
	std::map<int,int> map_merged_idx;
//	for( size_t i = 0; i != merged_path.size(); ++i )
//	{
//		const carve::geom::vector<2>&  v = merged_path[i];
//		
//#ifdef ROUND_IFC_COORDINATES
//		const double vertex_x = round(v.x*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
//		const double vertex_y = round(v.y*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
//#else
//		const double vertex_x = v.x;
//		const double vertex_y = v.y;
//#endif
//
//		//  return a pair, with its member pair::first set to an iterator pointing to either the newly inserted element or to the element with an equivalent key in the map
//		std::map<double, int>& map_y_index = existing_vertices_coords.insert( std::make_pair(vertex_x, std::map<double,int>() ) ).first->second;
//
//		it_find_y = map_y_index.find( vertex_y );
//		if( it_find_y != map_y_index.end() )
//		{
//			// vertex already exists in polygon. remember its index for triangles
//			map_merged_idx[i] = it_find_y->second;
//			continue;
//		}
//
//		carve::geom::vector<3>  vertex3D( carve::geom::VECTOR( v.x, v.y, 0 ) );
//		int vertex_id = poly_data->addVertex(vertex3D);
//		map_y_index[vertex_y] = vertex_id;  // TODO: it works for now, but check if we have to round here. maybe use checksum of rounded x and y as a single map key
//		map_merged_idx[i] = vertex_id;
//	}


	



	carve::geom::vector<3> local_z( carve::geom::VECTOR( 0, 0, 1 ) );
	const int num_curve_points = curve_points.size();
	if( num_curve_points < 2 )
	{
		std::cout << "IfcSweptDiskSolid: num curve points < 2" << std::endl;
		return;
	}

#ifdef _DEBUG
	if( num_curve_points == 2 )
	{
		std::cout << "IfcSweptDiskSolid: 2" << std::endl;
		;
	}
#endif

	const carve::geom::vector<3>& curve_point_first = curve_points[0];
	const carve::geom::vector<3>& curve_point_second = curve_points[1];

	bool bend_found = false;
	if( num_curve_points > 3 )
	{
		// compute local z vector by dot product of the first bend of the reference line
		const carve::geom::vector<3>& vertex_back2 = curve_point_first;
		const carve::geom::vector<3>& vertex_back1 = curve_point_second;
		for( int i=2; i<num_curve_points; ++i )
		{
			const carve::geom::vector<3>& vertex_current = curve_points[i];
			carve::geom::vector<3> section1 = vertex_back1 - vertex_back2;
			carve::geom::vector<3> section2 = vertex_current - vertex_back1;
			section1.normalize();
			section2.normalize();

			double dot_product = dot( section1, section2 );
			double dot_product_abs = std::abs(dot_product);

			// if dot == 1 or -1, then points are colinear
			if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
			{
				// bend found, compute cross product
				carve::geom::vector<3> lateral_vec = cross( section1, section2 );
				local_z = cross( lateral_vec, section1 );
				local_z.normalize();
				bend_found = true;
				break;
			}
		}
	}

	if( !bend_found )
	{
		// sweeping curve is linear. assume any local z vector
		carve::geom::vector<3> sweep_dir = curve_point_second - curve_point_first;
		sweep_dir.normalize();
		local_z = cross( carve::geom::VECTOR( 0, 0, 1 ), sweep_dir );
		if( local_z.length2() < 0.001 )
		{
			local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
			local_z.normalize();
		}
		else
		{
			local_z.normalize();
		}
		double dot_normal_local_z = dot( sweep_dir, local_z );
		if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
		{
			local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
			local_z.normalize();

			dot_normal_local_z = dot( sweep_dir, local_z );
			if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
			{
				local_z = cross( carve::geom::VECTOR( 1, 0, 0 ), sweep_dir );
				local_z.normalize();
			}
		}
	}

	// rotate disc into first direction
	carve::geom::vector<3>  section_local_y = local_z;
	carve::geom::vector<3>  section_local_z = curve_point_first - curve_point_second;
	carve::geom::vector<3>  section_local_x = carve::geom::cross( section_local_y, section_local_z );
	section_local_y = carve::geom::cross( section_local_x, section_local_z );
	std::vector<carve::geom::vector<3> > inner_shape_points;

	
	section_local_x.normalize();
	section_local_y.normalize();
	section_local_z.normalize();

	carve::math::Matrix matrix_first_direction = carve::math::Matrix(
		section_local_x.x,		section_local_y.x,		section_local_z.x,	0,
		section_local_x.y,		section_local_y.y,		section_local_z.y,	0,
		section_local_x.z,		section_local_y.z,		section_local_z.z,	0,
		0,				0,				0,			1 );

	//double angle = 0;
	//double delta_angle = 2.0*M_PI/double(nvc);	// TODO: adapt to model size and complexity
	//std::vector<carve::geom::vector<3> > circle_points;
	//std::vector<carve::geom::vector<3> > circle_points_inner;
	//for( int i = 0; i < nvc; ++i )
	//{
	//	// cross section (circle) is defined in XY plane
	//	double x = sin(angle);
	//	double y = cos(angle);
	//	carve::geom::vector<3> vertex( carve::geom::VECTOR( x*radius, y*radius, 0.0 ) );
	//	vertex = matrix_first_direction*vertex + curve_point_first;
	//	circle_points.push_back( vertex );

	//	if( radius_inner > 0 )
	//	{
	//		carve::geom::vector<3> vertex_inner( carve::geom::VECTOR( x*radius_inner, y*radius_inner, 0.0 ) );
	//		vertex_inner = matrix_first_direction*vertex_inner + curve_point_first;
	//		circle_points_inner.push_back( vertex_inner );
	//	}
	//	angle += delta_angle;
	//}

	for( int ii=0; ii<num_curve_points; ++ii )
	{
		carve::geom::vector<3> vertex_current = curve_points[ii];
		carve::geom::vector<3> vertex_next;
		carve::geom::vector<3> vertex_before;
		if( ii == 0 )
		{
			// first point
			vertex_next	= curve_points[ii+1];
			carve::geom::vector<3> delta_element = vertex_next - vertex_current;
			vertex_before = vertex_current - (delta_element);
		}
		else if( ii == num_curve_points-1 )
		{
			// last point
			vertex_before	= curve_points[ii-1];
			carve::geom::vector<3> delta_element = vertex_current - vertex_before;
			vertex_next = vertex_before + (delta_element);
		}
		else
		{
			// inner point
			vertex_next		= curve_points[ii+1];
			vertex_before	= curve_points[ii-1];
		}

		carve::geom::vector<3> bisecting_normal;
		GeomUtils::bisectingPlane( vertex_before, vertex_current, vertex_next, bisecting_normal );

		carve::geom::vector<3> section1 = vertex_current - vertex_before;
		carve::geom::vector<3> section2 = vertex_next - vertex_current;
		section1.normalize();
		section2.normalize();
		double dot_product = dot( section1, section2 );
		double dot_product_abs = std::abs(dot_product);

		if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
		{
			// bend found, compute next local z vector
			carve::geom::vector<3> lateral_vec = cross( section1, section2 );
			local_z = cross( lateral_vec, section1 );
			local_z.normalize();
		}
		if( ii == num_curve_points -1 )
		{
			bisecting_normal *= -1.0;
		}

		carve::geom::plane<3> bisecting_plane( bisecting_normal, vertex_current );

		for( size_t i = 0; i != merged_path.size(); ++i )
		{
			carve::geom::vector<2>&  vertex2d = merged_path[i];
			carve::geom::vector<3>  vertex( carve::geom::VECTOR( vertex2d.x, vertex2d.y, 0 ) );
		//for( int jj = 0; jj < nvc; ++jj )
		//{
			//carve::geom::vector<3>& vertex = circle_points[jj];

			carve::geom::vector<3> v;
			double t;
			carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t);
			if( intersect > 0 )
			{
				vertex = v;
			}
			else
			{
				std::cout << "no intersection found" << std::endl;
			}

			int vertex_id = poly_cache.addPoint(vertex);
			
			if( ii == 0 )
			{
				map_merged_idx[i] = vertex_id;
			}
			//pipe_data->addVertex( vertex );
		}

		//if( radius_inner > 0 )
		//{
		//	for( int jj = 0; jj < nvc; ++jj )
		//	{
		//		carve::geom::vector<3>& vertex = circle_points_inner[jj];
		//			
		//		carve::geom::vector<3> v;
		//		double t;
		//		carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t);
		//		if( intersect > 0 )
		//		{
		//			vertex = v;
		//		}
		//		else
		//		{
		//			std::cout << "no intersection found" << std::endl;
		//		}

		//		inner_shape_points.push_back( vertex );
		//	}
		//}
	}

	std::vector<carve::geom::vector<3> >& points = poly_cache.m_poly_data->points;
	const int num_points_base = points.size();

	// outer shape
	size_t num_vertices_outer = poly_cache.m_poly_data->getVertexCount();
	for( size_t i=0; i<num_curve_points- 1; ++i )
	{
		//int i_offset = i*nvc;
		//int i_offset_next = (i+1)*nvc;
		//for( int jj = 0; jj < nvc; ++jj )
		//{
		//	int current_loop_pt = jj + i_offset;
		//	int current_loop_pt_next = (jj + 1)%nvc + i_offset;

		//	int next_loop_pt = jj + i_offset_next;
		//	int next_loop_pt_next = (jj + 1)%nvc + i_offset_next;
		//	pipe_data->addFace( current_loop_pt, next_loop_pt, next_loop_pt_next, current_loop_pt_next );  
		//}

		
		// figure 4: points in poly_data (merged path without duplicate vertices):
		//  7<---------------------------6
		//  |                            ^
		//  |   2------------------>3    |
		//  |   ^                   |    |     map_merged_idx: figure 3 -> figure 4
		//  |   |                   v    |
		//  |   1<------------------4    |
		//  | /                          |
		//  0--------------------------->5

		// copy points from base to top

		bool flip_faces = false;
		//double extrusion_dot_normal = dot( extrusion_vector, normal_first_loop );
		//if( extrusion_dot_normal > 0 )
		//{
		//	flip_faces = true;
		//}

		
		//for( int i=0; i<num_points_base; ++i )
		//{
		//	carve::geom::vector<3>& pt = points[i];
		//	poly_data->addVertex( pt + extrusion_vector );
		//}

		// collect vertex indexes of loops
		std::map<int, std::vector<int> > loop_vert_idx;
		for( size_t merged_idx = 0; merged_idx < path_all_loops.size(); ++merged_idx )
		{
			int loop_number = path_all_loops[merged_idx].first;
			int point_idx_merged = map_merged_idx[merged_idx];
	
			std::map<int, std::vector<int> >::iterator it_result_loops = loop_vert_idx.insert( std::make_pair( loop_number, std::vector<int>() ) ).first;
			std::vector<int>& result_loop_vec = it_result_loops->second;
		
			// check if point index is already in loop
			bool already_in_loop = false;
			for( int i2 = 0; i2 < result_loop_vec.size(); ++i2 )
			{
				if( point_idx_merged == result_loop_vec[i2] )
				{
					already_in_loop = true;
					break;
				}
			}
			if( !already_in_loop )
			{
				result_loop_vec.push_back( point_idx_merged );
			}
		}

		// add faces along outer and inner loops
		for( std::map<int, std::vector<int> >::iterator it_result_loop = loop_vert_idx.begin(); it_result_loop != loop_vert_idx.end(); ++it_result_loop )
		{
			const std::vector<int>& loop_idx = it_result_loop->second;
			const int num_points_in_loop = loop_idx.size();
		
			for( int i=0; i<num_points_in_loop; ++i )
			{
				int point_idx		= loop_idx[i];
				int point_idx_next	= loop_idx[(i+1)%num_points_in_loop];
				int point_idx_up = point_idx + num_points_base;
				int point_idx_next_up = point_idx_next + num_points_base;

				if( point_idx_next_up >= 2*num_points_base )
				{
					std::cout << "point_idx_next_up >= 2*num_points_base" << std::endl;
					continue;
				}
				if( flip_faces )
				{
					poly_cache.m_poly_data->addFace( point_idx,			point_idx_next, point_idx_next_up );
					poly_cache.m_poly_data->addFace( point_idx_next_up, point_idx_up,	point_idx );
				}
				else
				{
					poly_cache.m_poly_data->addFace( point_idx,			point_idx_up,	point_idx_next_up );
					poly_cache.m_poly_data->addFace( point_idx_next_up, point_idx_next, point_idx );
				}
			}
		}
	}

	//if( radius_inner > 0 )
	//{
	//	if( inner_shape_points.size() != num_vertices_outer )
	//	{
	//		std::cout << "IfcSweptDiskSolid: inner_shape_points.size() != num_vertices_outer" << std::endl;
	//	}

	//	// add points for inner shape
	//	for( size_t i=0; i<inner_shape_points.size(); ++i )
	//	{
	//		pipe_data->addVertex( inner_shape_points[i] );
	//	}

	//	// faces of inner shape
	//	for( size_t i=0; i<num_curve_points- 1; ++i )
	//	{
	//		int i_offset = i*nvc + num_vertices_outer;
	//		int i_offset_next = (i+1)*nvc + num_vertices_outer;
	//		for( int jj = 0; jj < nvc; ++jj )
	//		{
	//			int current_loop_pt = jj + i_offset;
	//			int current_loop_pt_next = (jj + 1)%nvc + i_offset;

	//			int next_loop_pt = jj + i_offset_next;
	//			int next_loop_pt_next = (jj + 1)%nvc + i_offset_next;
	//			//pipe_data->addFace( current_loop_pt, next_loop_pt, next_loop_pt_next, current_loop_pt_next );  
	//			pipe_data->addFace( current_loop_pt, current_loop_pt_next, next_loop_pt_next, next_loop_pt );  
	//		}
	//	}

	//	// front cap
	//	for( int jj = 0; jj < nvc; ++jj )
	//	{
	//		int outer_rim_next = (jj+1)%nvc;
	//		int inner_rim_next = outer_rim_next + num_vertices_outer;
	//		pipe_data->addFace( jj, outer_rim_next, num_vertices_outer + jj );
	//		pipe_data->addFace( outer_rim_next, inner_rim_next, num_vertices_outer + jj );
	//	}

	//	// back cap
	//	int back_offset = (num_curve_points - 1)*nvc;
	//	for( int jj = 0; jj < nvc; ++jj )
	//	{
	//		int outer_rim_next = (jj+1)%nvc + back_offset;
	//		int inner_rim_next = outer_rim_next + num_vertices_outer;
	//		pipe_data->addFace( jj + back_offset, num_vertices_outer + jj + back_offset, outer_rim_next );
	//		pipe_data->addFace( outer_rim_next, num_vertices_outer + jj + back_offset, inner_rim_next );
	//	}
	//}
	//else
	//{
	//	// front cap, full pipe, create triangle fan
	//	for( int jj = 0; jj < nvc - 2; ++jj )
	//	{
	//		pipe_data->addFace( 0, jj+1, jj+2 );
	//	}

	//	// back cap
	//	int back_offset = (num_curve_points - 1)*nvc;
	//	for( int jj = 0; jj < nvc - 2; ++jj )
	//	{
	//		pipe_data->addFace( back_offset, back_offset + jj+2, back_offset + jj+1 );
	//	}
	//}

	bool flip_faces = false;

	// now the triangulated bottom and top cap
	for( size_t i = 0; i != triangulated.size(); ++i )
	{
		carve::triangulate::tri_idx triangle = triangulated[i];
		int a = triangle.a;
		int b = triangle.b;
		int c = triangle.c;

		int vertex_id_a = map_merged_idx[a];
		int vertex_id_b = map_merged_idx[b];
		int vertex_id_c = map_merged_idx[c];

		if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
		{
			continue;
		}

		int vertex_id_a_top = vertex_id_a + num_points_base;
		int vertex_id_b_top = vertex_id_b + num_points_base;
		int vertex_id_c_top = vertex_id_c + num_points_base;

#ifdef _DEBUG
		const carve::poly::Vertex<3>& v_a = poly_cache.m_poly_data->getVertex(vertex_id_a);
		const carve::poly::Vertex<3>& v_b = poly_cache.m_poly_data->getVertex(vertex_id_b);
		const carve::poly::Vertex<3>& v_c = poly_cache.m_poly_data->getVertex(vertex_id_c);

		carve::geom::vector<3> pa( carve::geom::VECTOR( v_a.v[0],	v_a.v[1],	v_a.v[2] ) );
		carve::geom::vector<3> pb( carve::geom::VECTOR( v_b.v[0],	v_b.v[1],	v_b.v[2] ) );
		carve::geom::vector<3> pc( carve::geom::VECTOR( v_c.v[0],	v_c.v[1],	v_c.v[2] ) );

		double A = 0.5*(cross( pa-pb, pa-pc ).length());
		if( std::abs(A) < 0.000000001 )
		{
			std::cout << "area < 0.000000001\n" << std::endl;
		}
#endif

		if( flip_faces )
		{
			poly_cache.m_poly_data->addFace( vertex_id_a,		vertex_id_c,		vertex_id_b );		// bottom cap
			poly_cache.m_poly_data->addFace( vertex_id_a_top,	vertex_id_b_top,	vertex_id_c_top );	// top cap, flipped outward
		}
		else
		{
			poly_cache.m_poly_data->addFace( vertex_id_a,		vertex_id_b,		vertex_id_c );		// bottom cap
			poly_cache.m_poly_data->addFace( vertex_id_a_top,	vertex_id_c_top,	vertex_id_b_top );	// top cap, flipped outward
		}
	}

#ifdef _DEBUG
	std::stringstream strs_err;
	shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_cache.m_poly_data->createMesh(carve::input::opts()) );
	bool poly_ok = CSG_Adapter::checkMeshSetValidAndClosed( meshset.get(), strs_err, -1 );

	if( !poly_ok )
	{
		std::cout << strs_err.str().c_str() << std::endl;
	}
#endif

	item_data_solid->closed_polyhedrons.push_back( poly_cache.m_poly_data );
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
		max = fabs(a[s][s]);
		if( pivot )
		{
			prow = s;
			for( i = s+1; i < 4; ++i )
			{
				if( fabs(a[i][s]) > max)
				{
					max = fabs(a[i][s]);
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
		throw IfcPPException("cannot compute inverse of matrix", __func__);
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
	double denom = point.x*line_direction.x + point.y*line_direction.y + point.z*line_direction.z - line_direction.x*line_origin.x - line_direction.y*line_origin.y - line_direction.z*line_origin.z;
	double numer = line_direction.x*line_direction.x + line_direction.y*line_direction.y + line_direction.z*line_direction.z;
	if(numer == 0)
	{
		throw IfcPPException("Line is degenerated: the line's direction vector is a null vector!", __func__);
	}
	double lambda = denom/numer;
	closest = carve::geom::VECTOR(line_origin.x+lambda*line_direction.x, line_origin.y+lambda*line_direction.y, line_origin.z+lambda*line_direction.z);
}

void GeomUtils::closestPointOnLine( const carve::geom::vector<2>& point, const carve::geom::vector<2>& line_origin, const carve::geom::vector<2>& line_direction, carve::geom::vector<2>& closest )
{
	double denom = point.x*line_direction.x + point.y*line_direction.y + - line_direction.x*line_origin.x - line_direction.y*line_origin.y;
	double numer = line_direction.x*line_direction.x + line_direction.y*line_direction.y;
	if(numer == 0)
	{
		throw IfcPPException("Line is degenerated: the line's direction vector is a null vector!", __func__);
	}
	double lambda = denom/numer;
	closest = carve::geom::VECTOR(line_origin.x+lambda*line_direction.x, line_origin.y+lambda*line_direction.y);
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
		for( int i=1; i<points_vec.size(); ++i )
		{
			const carve::geom::vector<2>& pt = points_vec[i];
			target_vec.push_back( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
		}
	}
	else
	{
		//target_vec.insert( target_vec.end(), points_vec.begin(), points_vec.end() );
		for( int i=0; i<points_vec.size(); ++i )
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
	double len21 = v21.length();
	double len32 = v32.length();

	if( len21 <= GEOM_TOLERANCE * len32)
	{
		if( len32 == 0.0)
		{
			// all three points lie ontop of one-another
			normal = carve::geom::VECTOR( 0.0, 0.0, 0.0 );
			valid = false;
		}
		else
		{
			// return a normalized copy of v32 as bisector
			len32 = 1.0 / len32;
			normal = v32*len32;
			normal.normalize();
			valid = true;
		}

	}
	else
	{
		valid = true;
		if( len32 <= GEOM_TOLERANCE * len21)
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

			if( dot_product_abs > (1.0+GEOM_TOLERANCE) || dot_product_abs < (1.0-GEOM_TOLERANCE) )
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

void GeomUtils::applyTranslate( osg::Group* grp, const osg::Vec3f& trans )
{
	int num_children = grp->getNumChildren();
	for( int i=0; i<num_children; ++i )
	{
		osg::Node* node = grp->getChild(i);
		osg::Group* child_group = dynamic_cast<osg::Group*>(node);
		if( child_group )
		{
			applyTranslate( child_group, trans );
			continue;
		}
		osg::Geode* child_geode = dynamic_cast<osg::Geode*>(node);
		if( child_geode )
		{
			const osg::Geode::DrawableList& drawable_list = child_geode->getDrawableList();
			for( osg::Geode::DrawableList::const_iterator it_drawables=drawable_list.begin(); it_drawables!=drawable_list.end(); ++it_drawables )
			{
				osg::Drawable* drawable = (*it_drawables);
				osg::Geometry* child_gemetry = dynamic_cast<osg::Geometry*>(drawable);
				if( !child_gemetry )
				{
					std::cout << "!child_gemetry" << std::endl;
					return;
				}
				osg::Array* vertices_array = child_gemetry->getVertexArray();
				osg::Vec3Array* vertices_float = dynamic_cast<osg::Vec3Array*>(vertices_array);

				if( !vertices_float )
				{
					std::cout << "!vertices_float" << std::endl; 
					return;
				}

				for( osg::Vec3Array::iterator it_array = vertices_float->begin(); it_array != vertices_float->end(); ++it_array )
				{
					osg::Vec3f& vertex = (*it_array);
					vertex = vertex + trans;
				}
				child_gemetry->dirtyBound();
				child_gemetry->dirtyDisplayList();
				child_geode->dirtyBound();
			}
		}
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

bool GeomUtils::Plane::intersetRay( const GeomUtils::Ray* ray, osg::Vec3d& intersect_point )
{
	carve::geom::vector<3>  location(		carve::geom::VECTOR( position.x(), position.y(), position.z()) );
	carve::geom::vector<3>  local_z(		carve::geom::VECTOR( normal.x(), normal.y(), normal.z() ) );
	//carve::geom::vector<3>  local_y(		carve::geom::VECTOR(0.0, 1.0, 0.0) );
	//carve::geom::vector<3>  local_z(		carve::geom::VECTOR(0.0, 0.0, 1.0) );
	//carve::geom::vector<3>  ref_direction(	carve::geom::VECTOR(1.0, 0.0, 0.0) );


	
	//local_z = carve::geom::VECTOR( axis[0], axis[1], axis[2] );
	//local_z.normalize();

	carve::geom::plane<3> plane( local_z, location );
	//plane.d = p.d;
	//plane.N = local_z;
	//translate = location;

	carve::geom::vector<3>  v1 = carve::geom::VECTOR( ray->origin.x(), ray->origin.y(), ray->origin.z() );
	carve::geom::vector<3>  v2 = v1 + carve::geom::VECTOR( ray->direction.x(), ray->direction.y(), ray->direction.z() );
	carve::geom::vector<3>  v_intersect;
	double t;

	carve::IntersectionClass intersect_result = carve::geom3d::rayPlaneIntersection( plane, v1, v2, v_intersect, t );
 //                                          Vector &v,
 //                                          double &t) 

	if( intersect_result == carve::INTERSECT_BAD || intersect_result == carve::INTERSECT_NONE )
	{
		return false;
	}

	intersect_point.set( v_intersect.x, v_intersect.y, v_intersect.z );
	return true;

    //  Vector Rd = v2 - v1;
    //  double Vd = dot(p.N, Rd);
    //  double V0 = dot(p.N, v1) + p.d;

    //  if (carve::math::ZERO(Vd)) {
    //    if (carve::math::ZERO(V0)) {
    //      return INTERSECT_BAD;
    //    } else {
    //      return INTERSECT_NONE;
    //    }
    //  }

    //  t = -V0 / Vd;
    //  v = v1 + t * Rd;
    //  return INTERSECT_PLANE;
    //}
}

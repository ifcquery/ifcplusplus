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

#include <osg/Switch>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Depth>
#include <osg/LightSource>
#include <osg/LightModel>

#include "ifcppgeometry/GeomUtils.h"
#include "IfcPlusPlusSystem.h"
#include "ViewController.h"

ViewController::ViewController()
{
	m_rootnode	= new osg::Group();
	m_rootnode->setName("m_rootnode");
	m_sw_model = new osg::Switch();
	m_transform_model = new osg::MatrixTransform();
	m_transform_model->setName("m_transform_model");
	m_transform_model->addChild( m_sw_model.get() );
	m_rootnode->addChild( m_transform_model.get() );
	//m_rootnode->setDataVariance( osg::Object::DYNAMIC );

	// background camera
	//osg::Camera* sky = new osg::Camera();
	//sky->setName( "Sky_Camera" );
	//sky->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	//sky->setViewMatrix( osg::Matrix::identity() );
	//sky->setRenderOrder( osg::Camera::PRE_RENDER );
	//sky->setClearColor( osg::Vec4f( 0.96f,	0.96f,	0.96f,	1.0f ) );
	//m_rootnode->addChild( sky );
	// TODO: fancy horizon

	m_sw_bound = new osg::Switch();
	m_sw_bound->setName("m_sw_bound");
	m_rootnode->addChild( m_sw_bound );
	m_transparent_model = false;
	m_viewer_mode = VIEWER_MODE_SHADED;

	m_shinyness = 35.f;
	m_material_default = new osg::Material();
	m_material_default->setAmbient( osg::Material::FRONT, osg::Vec4f( 0.2f, 0.25f, 0.3f, 0.3f ) );
	m_material_default->setDiffuse( osg::Material::FRONT, osg::Vec4( 0.8, 0.82, 0.84, 0.3f ) );
	m_material_default->setSpecular( osg::Material::FRONT, osg::Vec4f( 0.02f, 0.025f, 0.03f, 0.03f ) );
	m_material_default->setShininess( osg::Material::FRONT, m_shinyness );
	m_material_default->setColorMode( osg::Material::SPECULAR );
	//m_material_default->setEmission( osg::Material::FRONT, osg::Vec4f( 0.05f, 0.08f, 0.1f, 0.1f ) );
	//m_material_default->setEmission( osg::Material::BACK, osg::Vec4f( 0.095f, 0.098f, 0.091f, 0.091f ) );
	m_material_default->setTransparency( osg::Material::FRONT_AND_BACK, 0.7f );
	
	osg::LightModel* light_model = new osg::LightModel();  
	light_model->setAmbientIntensity( osg::Vec4f( 0.2f, 0.25f, 0.3f, 0.3f ) );
	light_model->setTwoSided(true);
	m_rootnode->getOrCreateStateSet()->setAttribute( light_model );

	m_stateset_default = m_transform_model->getOrCreateStateSet();
	m_stateset_default->setAttribute( m_material_default, osg::StateAttribute::ON );

	m_stateset_transparent = new osg::StateSet();
	m_stateset_transparent->setAttribute( m_material_default, osg::StateAttribute::ON );
	m_stateset_transparent->setMode(GL_BLEND, osg::StateAttribute::ON);
	m_stateset_transparent->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	m_stateset_transparent->setRenderBinDetails(11, "DepthSortedBin"); 

	osg::Material* material_selected = new osg::Material();
	//m_material_selected->setAmbient( osg::Material::FRONT, osg::Vec4f( 0.02f, 0.025f, 0.03f, 0.03f ) );
	material_selected->setDiffuse( osg::Material::FRONT, osg::Vec4f( 0.4f, 0.92f, 0.92f, 0.5f ) );
	material_selected->setSpecular( osg::Material::FRONT, osg::Vec4f( 0.3f, 0.35f, 0.3f, 1.0f ) );
	material_selected->setShininess( osg::Material::FRONT, m_shinyness );
	material_selected->setColorMode( osg::Material::SPECULAR );
	//m_material_selected->setEmission( osg::Material::FRONT, osg::Vec4f( 0.1f, 0.15f, 0.2f, 0.2f ) );
	//material_selected->setAlpha(osg::Material::FRONT, 0.8f);
	m_stateset_selected = new osg::StateSet();
	m_stateset_selected->setAttribute( material_selected, osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON );

	// create coordinate axes
	m_sw_coord_axes = new osg::Switch();
	m_sw_coord_axes->setName("m_sw_coord_axes");
	m_sw_coord_axes->addChild( GeomUtils::createCoordinateAxes() );
	m_sw_coord_axes->addChild( GeomUtils::createCoordinateAxesArrows() );
	//m_sw_coord_axes->addChild( createQuarterCircles() );
	m_rootnode->addChild( m_sw_coord_axes.get() );

	toggleSceneLight();
}

ViewController::~ViewController()
{
}

void ViewController::toggleModelTransparency()
{
	m_transparent_model = !m_transparent_model;
	if( m_transparent_model )
	{
		m_transform_model->setStateSet( m_stateset_transparent );
	}
	else
	{
		m_transform_model->setStateSet( m_stateset_default );
	}
}

void ViewController::setViewerMode( ViewerMode mode )
{
	if( mode != m_viewer_mode )
	{
		// first disable previous mode
		if( m_viewer_mode == VIEWER_MODE_WIREFRAME )
		{
			GeomUtils::WireFrameModeOff( m_transform_model.get() );
		}
		else if( m_viewer_mode == VIEWER_MODE_HIDDEN_LINE )
		{
			GeomUtils::HiddenLineModeOff( m_transform_model.get() );
		}

		m_viewer_mode = mode;
		if( m_viewer_mode == VIEWER_MODE_WIREFRAME )
		{
			GeomUtils::WireFrameModeOn( m_transform_model.get() );
		}
		else if( m_viewer_mode == VIEWER_MODE_HIDDEN_LINE )
		{
			GeomUtils::HiddenLineModeOn( m_transform_model.get() );
		}
	}
}

void ViewController::toggleBoundingSphere()
{
	if( m_sw_bound->getNumChildren() == 0 )
	{
		const osg::BoundingSphere& bs = m_sw_model->getBound();
		osg::Sphere* sphere = new osg::Sphere( bs.center(), bs.radius() );
		osg::ShapeDrawable* drawable = new osg::ShapeDrawable( sphere );
		drawable->setColor( osg::Vec4( 0.7, 0.7, 0.7, 0.1 ) );
		osg::Geode* geode = new osg::Geode();
		geode->addDrawable( drawable );
		m_sw_bound->addChild( geode );
		geode->setStateSet( m_stateset_transparent );
	}
	else
	{
		m_sw_bound->removeChildren( 0, m_sw_bound->getNumChildren() );
	}
}

void ViewController::toggleSceneLight()
{
	osg::StateSet* stateset_root = m_rootnode->getOrCreateStateSet();

	if( !m_transform_light.valid() )
	{
		osg::Group* light_group = new osg::Group();
		light_group->setName("light_group");
		double model_size = 100; // TODO: adjust when model is loaded

		osg::Light* light6 = new osg::Light();
		light6->setLightNum(6);
		light6->setPosition(osg::Vec4(0.0,0.0,0.0,1.0f));
		light6->setAmbient(osg::Vec4(0.5f,0.53f,0.57f,0.4f));
		light6->setDiffuse(osg::Vec4(0.5f,0.53f,0.57f,0.4f));
		light6->setConstantAttenuation(1.0f);
		light6->setLinearAttenuation(2.0f/model_size);
		light6->setQuadraticAttenuation(2.0f/(model_size*model_size));

		osg::LightSource* light_source6 = new osg::LightSource();
		light_source6->setLight(light6);
		light_source6->setLocalStateSetModes(osg::StateAttribute::ON);
		light_source6->setStateSetModes(*stateset_root,osg::StateAttribute::ON);
		m_transform_light = new osg::MatrixTransform( osg::Matrix::translate( 5, 5, 50 ) );
		m_transform_light->addChild(light_source6);

		light_group->addChild(m_transform_light);
		m_rootnode->addChild( light_group );

		m_light_on = false;
	}

	m_light_on = !m_light_on;
	if( m_light_on )
	{
		stateset_root->setMode( GL_LIGHT6, osg::StateAttribute::ON );
	}
	else
	{
		stateset_root->setMode( GL_LIGHT6, osg::StateAttribute::OFF );
	}
}

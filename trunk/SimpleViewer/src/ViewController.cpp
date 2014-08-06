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
	m_rootnode->addChild( m_sw_model.get() );
	
	m_sw_bound = new osg::Switch();
	m_sw_bound->setName("m_sw_bound");
	m_rootnode->addChild( m_sw_bound );
	m_transparent_model = false;
	m_viewer_mode = VIEWER_MODE_SHADED;
	m_show_curve_representation = true;

	m_shinyness = 35.f;
	m_material_default = new osg::Material();
	m_material_default->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.2f, 0.25f, 0.3f, 0.3f ) );
	m_material_default->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4( 0.8, 0.82, 0.84, 0.3f ) );
	m_material_default->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.02f, 0.025f, 0.03f, 0.03f ) );
	m_material_default->setShininess( osg::Material::FRONT_AND_BACK, m_shinyness );
	m_material_default->setColorMode( osg::Material::SPECULAR );
	m_material_default->setTransparency( osg::Material::FRONT_AND_BACK, 0.7f );
	
	osg::LightModel* light_model = new osg::LightModel();  
	light_model->setAmbientIntensity( osg::Vec4f( 0.2f, 0.25f, 0.3f, 0.3f ) );
	light_model->setTwoSided(true);
	m_rootnode->getOrCreateStateSet()->setAttribute( light_model );

	m_stateset_default = m_sw_model->getOrCreateStateSet();
	m_stateset_default->setAttribute( m_material_default, osg::StateAttribute::ON );

	m_stateset_transparent = new osg::StateSet();
	m_stateset_transparent->setAttribute( m_material_default, osg::StateAttribute::ON );
	m_stateset_transparent->setMode(GL_BLEND, osg::StateAttribute::ON);
	m_stateset_transparent->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	m_stateset_transparent->setRenderBinDetails(11, "DepthSortedBin"); 

	osg::Material* material_selected = new osg::Material();
	material_selected->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.4f, 0.92f, 0.92f, 0.5f ) );
	material_selected->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.3f, 0.35f, 0.3f, 1.0f ) );
	material_selected->setShininess( osg::Material::FRONT_AND_BACK, m_shinyness );
	material_selected->setColorMode( osg::Material::SPECULAR );
	m_stateset_selected = new osg::StateSet();
	m_stateset_selected->setAttribute( material_selected, osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON );

	// create coordinate axes
	m_sw_coord_axes = new osg::Switch();
	m_sw_coord_axes->setName("m_sw_coord_axes");
	m_sw_coord_axes->addChild( GeomUtils::createCoordinateAxes() );
	m_sw_coord_axes->addChild( GeomUtils::createCoordinateAxesArrows() );
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
		m_sw_model->setStateSet( m_stateset_transparent );
	}
	else
	{
		m_sw_model->setStateSet( m_stateset_default );
	}
}

void ViewController::setViewerMode( ViewerMode mode )
{
	if( mode != m_viewer_mode )
	{
		// first disable previous mode
		if( m_viewer_mode == VIEWER_MODE_WIREFRAME )
		{
			GeomUtils::WireFrameModeOff( m_sw_model.get() );
		}
		else if( m_viewer_mode == VIEWER_MODE_HIDDEN_LINE )
		{
			GeomUtils::HiddenLineModeOff( m_sw_model.get() );
		}

		m_viewer_mode = mode;
		if( m_viewer_mode == VIEWER_MODE_WIREFRAME )
		{
			GeomUtils::WireFrameModeOn( m_sw_model.get() );
		}
		else if( m_viewer_mode == VIEWER_MODE_HIDDEN_LINE )
		{
			GeomUtils::HiddenLineModeOn( m_sw_model.get() );
		}
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

void ViewController::switchCurveRepresentation( osg::Group* grp, bool on )
{
	m_show_curve_representation = on;
	osg::Switch* grp_switch = dynamic_cast<osg::Switch*>( grp );
	if( grp_switch )
	{
		if( grp_switch->getName().compare( "CurveRepresentation" ) == 0 )
		{
			if( on )
			{
				grp_switch->setAllChildrenOn();
			}
			else
			{
				grp_switch->setAllChildrenOff();
			}
		}
	}

	unsigned int num_children = grp->getNumChildren();
	for( unsigned int i=0; i<num_children; ++i)
	{
		osg::Node* child_node = grp->getChild(i);
		osg::Group* child_grp = dynamic_cast<osg::Group*>( child_node );
		if( child_grp )
		{
			switchCurveRepresentation( child_grp, on );
		}
	}
}

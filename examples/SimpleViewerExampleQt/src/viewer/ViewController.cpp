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

#include <QSettings>
#include <osg/Switch>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Hint>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Depth>
#include <osg/LightSource>
#include <osg/LightModel>

#include <ifcpp/geometry/GeomUtils.h>
#include <ifcpp/geometry/SceneGraphUtils.h>
#include "viewer/Orbit3DManipulator.h"
#include "IfcPlusPlusSystem.h"
#include "GraphicsWindowQt.h"
#include "ViewController.h"

ViewController::ViewController( Orbit3DManipulator* camera_manip )
{
	m_camera_manip = camera_manip;
	m_rootnode	= new osg::Group();
	m_rootnode->setName("m_rootnode");
	m_model_node = new osg::Group();
	m_rootnode->addChild(m_model_node.get() );

	m_temp_node = new osg::Group();
	m_temp_node->setName( "m_temp_node" );
	m_rootnode->addChild( m_temp_node );

	m_snap_node = new osg::Group();
	m_snap_node->setName( "m_snap_node" );
	m_rootnode->addChild( m_snap_node );
	
	m_material_default = new osg::Material();
	m_material_default->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.2f, 0.25f, 0.3f, 1.f ) );
	m_material_default->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4( 0.8, 0.82, 0.84, 1.f ) );
	m_material_default->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.02f, 0.025f, 0.03f, 1.f ) );
	m_material_default->setShininess( osg::Material::FRONT_AND_BACK, m_shinyness );
	m_material_default->setColorMode( osg::Material::SPECULAR );
	m_material_default->setTransparency( osg::Material::FRONT_AND_BACK, 1.f );
	
	osg::LightModel* light_model = new osg::LightModel();  
	light_model->setAmbientIntensity( osg::Vec4f( 0.2f, 0.25f, 0.3f, 0.1f ) );
	light_model->setTwoSided(true); 
	m_rootnode->getOrCreateStateSet()->setAttribute( light_model );

	m_stateset_default = m_model_node->getOrCreateStateSet();
	m_stateset_default->setAttribute( m_material_default, osg::StateAttribute::ON );

	m_stateset_transparent = new osg::StateSet();
	m_stateset_transparent->setAttribute( m_material_default, osg::StateAttribute::ON );
	m_stateset_transparent->setMode(GL_BLEND, osg::StateAttribute::ON);
	m_stateset_transparent->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	m_stateset_transparent->setRenderBinDetails(11, "DepthSortedBin"); 

	m_material_selected = new osg::Material();
	m_material_selected->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.98f, 0.98f, 0.22f, 0.9f ) );
	m_material_selected->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.3f, 0.35f, 0.3f, 1.0f ) );
	m_material_selected->setShininess( osg::Material::FRONT_AND_BACK, m_shinyness );
	m_material_selected->setColorMode( osg::Material::SPECULAR );
	m_material_selected->setAlpha(osg::Material::FRONT_AND_BACK, 0.8f);

	// create coordinate axes
	m_coordinate_axes_switch = new osg::Switch();
	m_coordinate_axes_switch->setName("m_coordinate_axes_switch");
	m_coordinate_axes_switch->addChild( SceneGraphUtils::createCoordinateAxes(500) );
	m_coordinate_axes_switch->addChild( SceneGraphUtils::createCoordinateAxesArrows() );
	
	m_rootnode->addChild( m_coordinate_axes_switch.get() );
	m_rootnode->getOrCreateStateSet()->setAttributeAndModes( new osg::Hint( GL_LINE_SMOOTH_HINT, GL_NICEST ), osg::StateAttribute::ON );
	m_rootnode->getOrCreateStateSet()->setAttributeAndModes( new osg::Hint( GL_POLYGON_SMOOTH_HINT, GL_NICEST ), osg::StateAttribute::ON );
	m_rootnode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );

	m_main_view = new osgViewer::View();
	m_main_view->setSceneData( m_rootnode );
	m_main_view->setCameraManipulator( m_camera_manip );

	if( m_hud_camera )
	{
		m_rootnode->addChild( m_hud_camera );
	}

	//m_current_pointer_ray = new GeomUtils::Ray();
	m_pointer_push_drag = false;
	m_show_curve_representation = false;
}

ViewController::~ViewController(){}

void ViewController::setGLWidget(QtOSGWidget* glw)
{
	m_gl_widget = glw;
}

QtOSGWidget* ViewController::getGLWidget()
{
#ifdef _DEBUG
	if (m_gl_widget == nullptr)
	{
		std::cout << "m_gl_widget not initialized" << std::endl;
	}
#endif
	return m_gl_widget;
}

void ViewController::setPointerRay( glm::dvec3& origin, glm::dvec3& direction )
{
	m_current_pointer_ray.origin = origin;
	m_current_pointer_ray.direction = direction;
}

void ViewController::setModelTransparent( bool transparent )
{
	m_transparent_model = transparent;
	if( transparent )
	{
		m_model_node->setStateSet( m_stateset_transparent );
	}
	else
	{
		m_model_node->setStateSet( m_stateset_default );
	}
}

void ViewController::toggleModelTransparency()
{
	setModelTransparent( !m_transparent_model );
}

void ViewController::setViewerMode( ViewerMode mode )
{
	if( mode != m_viewer_mode )
	{
		// first disable previous mode
		if( m_viewer_mode == VIEWER_MODE_WIREFRAME )
		{
			SceneGraphUtils::WireFrameModeOff(m_model_node.get() );
		}
		else if( m_viewer_mode == VIEWER_MODE_HIDDEN_LINE )
		{
			//SceneGraphUtils::HiddenLineModeOff( m_model_switch.get() );
		}

		m_viewer_mode = mode;
		if( m_viewer_mode == VIEWER_MODE_WIREFRAME )
		{
			SceneGraphUtils::WireFrameModeOn(m_model_node.get() );
		}
		else if( m_viewer_mode == VIEWER_MODE_HIDDEN_LINE )
		{
			//GeomUtils::HiddenLineModeOn( m_model_switch.get() );
		}
	}
}

void ViewController::setProjection( ViewerProjection p, int width, int height )
{
	m_projection = p;
	double ratio = double( height )/double( width );

	osg::Camera* cam = getMainView()->getCamera();
	cam->setViewport( new osg::Viewport( 0, 0, width, height ) );

	if( m_projection == PROJECTION_PARALLEL )
	{
		// for some reason, osg auto computed near/far planes cause unintended clipping...
		//cam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
		cam->setProjectionMatrixAsOrtho( -100, 100, -100.0*ratio, 100.0*ratio, m_near_plane-1000, m_near_plane/0.0005+100 );
	}
	else if( m_projection == PROJECTION_PERSPECTIVE )
	{
		//cam->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
		cam->setProjectionMatrixAsPerspective( 90.0, 1.0/ratio, m_near_plane, m_far_plane );
		cam->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );
	}

	if( m_hud_camera )
	{
		m_hud_camera->setProjectionMatrixAsOrtho2D( 0, width, 0, height );
	}
}

void ViewController::setSunLightOn( bool light_on )
{
	m_light_on = light_on;
	osg::StateSet* stateset_root = m_rootnode->getOrCreateStateSet();
	if( light_on )
	{
		if( !m_transform_light )
		{
			m_transform_light = new osg::MatrixTransform();// osg::Matrix::translate( 5, 5, 50 ) );
			osg::Group* light_group = new osg::Group();
			light_group->setName("light_group");
			light_group->addChild(m_transform_light);
			m_rootnode->addChild(light_group);

			m_sun_light = new osg::Light();
			m_sun_light->setLightNum(6);
			m_sun_light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0f));
			m_sun_light->setAmbient(osg::Vec4(0.5f, 0.53f, 0.57f, 0.1f));
			m_sun_light->setDiffuse(osg::Vec4(0.5f, 0.53f, 0.57f, 0.1f));
			m_sun_light->setSpecular(osg::Vec4(0.5f, 0.53f, 0.57f, 0.4f));
			double model_size = 100;
			m_sun_light->setConstantAttenuation(1.0f);
			m_sun_light->setLinearAttenuation(2.0f / model_size);
			m_sun_light->setQuadraticAttenuation(1.0f / (model_size * model_size));

			osg::LightSource* light_source6 = new osg::LightSource();
			//light_source6->setLightingMode( HEAD_LIGHT );
			light_source6->setReferenceFrame(osg::LightSource::ABSOLUTE_RF);
			light_source6->setLight(m_sun_light);
			light_source6->setLocalStateSetModes(osg::StateAttribute::ON);
			light_source6->setStateSetModes(*(m_rootnode->getOrCreateStateSet()), osg::StateAttribute::ON);
			m_transform_light->addChild(light_source6);
		}

		stateset_root->setMode( GL_LIGHT6, osg::StateAttribute::ON );
	}
	else
	{
		stateset_root->setMode( GL_LIGHT6, osg::StateAttribute::OFF );
	}
}

void ViewController::toggleSunLight()
{
	m_light_on = !m_light_on;
	setSunLightOn( m_light_on );
}

void ViewController::setCursor( const QCursor& cursor )
{
	if( m_gl_widget )
	{
		m_gl_widget->setCursor( cursor );
	}
}

void ViewController::restoreCursor()
{
	if( m_gl_widget )
	{
		m_gl_widget->setCursor( Qt::ArrowCursor );
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

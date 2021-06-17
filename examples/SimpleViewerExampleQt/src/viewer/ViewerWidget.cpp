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

#include <stdlib.h>
#include <sstream>
#include <osgGA/GUIEventAdapter>
#include <osgGA/OrbitManipulator>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Renderer>
#include <osg/AnimationPath>
#include <osg/LightModel>
#include <osg/LineStipple>
#include <osg/MatrixTransform>
#include <osgViewer/CompositeViewer>
#include <QtCore/qglobal.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QKeyEvent>

#include "IfcPlusPlusSystem.h"
#include "OrbitCameraManipulator.h"
#include "GraphicsWindowQt.h"
#include "ViewerWidget.h"

ViewerWidget::ViewerWidget( IfcPlusPlusSystem* sys, QWidget* parent ) : QWidget( parent )
{
	m_system = sys;
	m_system->setViewerWidget(this);
	m_parent = parent;
	m_shinyness = 35.0;
	m_material_default = new osg::Material();
	m_material_default->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.2f, 0.25f, 0.3f, 1.0f ) );
	m_material_default->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4( 0.8, 0.82, 0.84, 1.0f ) );
	m_material_default->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.2f, 0.25f, 0.3f, 1.0f ) );
	m_material_default->setShininess( osg::Material::FRONT_AND_BACK, m_shinyness );
	m_material_default->setColorMode( osg::Material::SPECULAR );

	osg::ref_ptr<osg::LightModel> light_model = new osg::LightModel();
	light_model->setTwoSided( true );
	light_model->setAmbientIntensity( osg::Vec4f( 0.2f, 0.25f, 0.3f, 0.3f ) );
	sys->getRootNode()->getOrCreateStateSet()->setAttribute( light_model );

	m_stateset_default = sys->getModelNode()->getOrCreateStateSet();
	m_stateset_default->setAttribute( m_material_default, osg::StateAttribute::ON );

	m_stateset_transparent = new osg::StateSet();
	m_stateset_transparent->setAttribute( m_material_default, osg::StateAttribute::ON );
	m_stateset_transparent->setMode( GL_BLEND, osg::StateAttribute::ON );
	m_stateset_transparent->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	m_stateset_transparent->setRenderBinDetails( 11, "DepthSortedBin" );

	osg::ref_ptr<osg::Material> material_selected = new osg::Material();
	material_selected->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.4f, 0.92f, 0.92f, 0.5f ) );
	material_selected->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.3f, 0.35f, 0.3f, 1.0f ) );
	material_selected->setShininess( osg::Material::FRONT_AND_BACK, m_shinyness );
	material_selected->setColorMode( osg::Material::SPECULAR );
	m_stateset_selected = new osg::StateSet();
	m_stateset_selected->setAttribute( material_selected, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );

	sys->toggleSceneLight();

	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		geode->setName( "createCoordinateAxes::geode" );
		osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
		stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		float alpha = 0.5f;
		m_system->getCoordinateAxesNode()->addChild( geode );

		// positive axes
		{
			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
			geode->addDrawable( geom );

			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 100, 0.0, 0.0 ) );

			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 0.0, 100, 0.0 ) );

			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 0.0, 0.0, 100 ) );

			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.8f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.8f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
			colors->setBinding( osg::Array::BIND_PER_VERTEX );
			geom->setColorArray( colors );

			geom->setVertexArray( vertices );
			geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, 6 ) );
		}

		// positive axes
		{
			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
			geode->addDrawable( geom );

			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( -100, 0.0, 0.0 ) );

			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 0.0, -100, 0.0 ) );

			vertices->push_back( osg::Vec3f( 0.0, 0.0, 0.0 ) );
			vertices->push_back( osg::Vec3f( 0.0, 0.0, -100 ) );

			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 1.f, 0.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 1.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 1.f, 0.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
			colors->push_back( osg::Vec4f( 0.f, 0.f, 1.f, alpha ) );
			colors->setBinding( osg::Array::BIND_PER_VERTEX );
			geom->setColorArray( colors );

			geom->setVertexArray( vertices );
			geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, 6 ) );

			// make negative axed dotted
			osg::ref_ptr<osg::StateSet> stateset_negative = geom->getOrCreateStateSet();
			stateset_negative->setAttributeAndModes( new osg::LineStipple( 2, 0xAAAA ), osg::StateAttribute::ON );
		}

		// x axis label
		bool add_x_label = false;
		if( add_x_label )
		{
			osg::ref_ptr<osgText::Text> label_x = new  osgText::Text();
			//label_x->setFont( "ARIAL.TTF" );
			label_x->setFont( "fonts/calibri.ttf" );
			label_x->setAlignment( osgText::Text::RIGHT_TOP );
			label_x->setAxisAlignment( osgText::Text::SCREEN );
			label_x->setColor( osg::Vec4( 0.8, 0.0, 0.0, 1.0 ) );
			label_x->setCharacterSize( 0.5f );
			label_x->setText( "x" );
			label_x->setPosition( osg::Vec3( 1, 0, 0 ) );
			label_x->setEnableDepthWrites( false );
			geode->addDrawable( label_x );
		}
	}

	m_camera_manipulator = new OrbitCameraManipulator( sys );

	QtOSGWidget* opengl_widget = getOpenGLWidget();
	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->setContentsMargins( 0, 0, 0, 1 );
	vbox->addWidget( opengl_widget );
	setLayout( vbox );

	connect( &m_timer, SIGNAL( timeout() ), this, SLOT( slotAnimationFrame() ) );
}
ViewerWidget::~ViewerWidget() {}

void ViewerWidget::initGLWidgetAndViewer()
{
	osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::SingleThreaded;
	m_system->getRootNode()->setCullingActive( false );
	m_graphics_window = new GraphicsWindowQt( this );
	QtOSGWidget* opengl_widget = m_graphics_window->getOpenGLWidget();
	opengl_widget->setMinimumSize( QSize( 150, 150 ) );
	m_main_view = opengl_widget->getView();
	m_main_view->setSceneData( m_system->getRootNode() );
	m_composite_viewer = opengl_widget->getViewer();
	m_composite_viewer->setThreadingModel( threadingModel );
	m_composite_viewer->setKeyEventSetsDone( 0 );		// disable the default setting of viewer.done() by pressing Escape.
	m_composite_viewer->addView( m_main_view );

	// set up the camera
	osg::ref_ptr<osg::Camera> camera = m_main_view->getCamera();
	camera->setGraphicsContext( m_graphics_window );
	camera->setClearColor( osg::Vec4f( 0.92, 0.93, 0.94, 1.0 ) );

	if( m_main_view )
	{
		m_main_view->setCameraManipulator( m_camera_manipulator );
		m_main_view->setSceneData( m_system->getRootNode() );
	}
}

QtOSGWidget* ViewerWidget::getOpenGLWidget()
{
	if( !m_graphics_window )
	{
		initGLWidgetAndViewer();
	}
	if( m_graphics_window )
	{
		QtOSGWidget* opengl_widget = m_graphics_window->getOpenGLWidget();
		return opengl_widget;
	}

	return nullptr;
}
GraphicsWindowQt* ViewerWidget::getGraphicsWindowQt()
{
	if( !m_graphics_window )
	{
		initGLWidgetAndViewer();
	}
	return m_graphics_window;
}
osgViewer::View* ViewerWidget::getMainView()
{
	if( !m_main_view )
	{
		initGLWidgetAndViewer();
	}
	return m_main_view;
}
osgViewer::CompositeViewer* ViewerWidget::getCompositeViewer()
{
	if( !m_composite_viewer )
	{
		initGLWidgetAndViewer();
	}
	return m_composite_viewer;
}

void ViewerWidget::slotAnimationFrame()
{
	update();
}

void ViewerWidget::paintEvent( QPaintEvent* )
{
	QtOSGWidget* gl_widget = getOpenGLWidget();
	if( gl_widget )
	{
		osgViewer::CompositeViewer* composite_viewer = getCompositeViewer();
		if( composite_viewer )
		{
			composite_viewer->frame();
		}
	}
}

void ViewerWidget::updateCamera()
{
	int w = width();
	int h = height();
	double ratio = double(h)/double(w);

	osg::Camera* cam = m_main_view->getCamera();
	cam->setViewport( new osg::Viewport(0, 0, w, h) );
	cam->setProjectionMatrixAsOrtho(-100, 100, -100.0*ratio, 100.0*ratio, m_near_plane-1000, m_near_plane/0.0005+100 );

	if( m_hud_camera )
	{
		m_hud_camera->setProjectionMatrixAsOrtho2D(0, w, 0, h);
	}
}

void ViewerWidget::setRootNode( osg::Group* root )
{
	m_main_view->setSceneData( root );
	if( m_hud_camera )
	{
		root->addChild( m_hud_camera );
	}
	m_system->setRootNode( root );
}

void ViewerWidget::stopTimer()
{
	m_timer.stop();
}

void ViewerWidget::startTimer()
{
	m_timer.start(10);
}

void ViewerWidget::resizeEvent( QResizeEvent * ev )
{
	QSize s = ev->size();
	int width = s.width();
	int height = s.height();
	m_main_view->getEventQueue()->windowResize(0, 0, width, height );
}

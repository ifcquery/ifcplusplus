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


#include <stdlib.h>
#include <osgGA/GUIEventAdapter>
#include <osgGA/OrbitManipulator>

#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Renderer>
#include <osg/AnimationPath>
#include <osg/LightModel>
#include <osgViewer/CompositeViewer>
#include <osgQt/GraphicsWindowQt>
#include <QtCore/qglobal.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QtGui/QKeyEvent>
#if QT_VERSION >= 0x04FF00
#include <QWindow>
#endif

#include "ViewerWidget.h"

ViewerWidget::ViewerWidget( QWidget* parent) : QWidget(parent)
{
	//osgViewer::ViewerBase::ThreadingModel threadingModel = m_viewer.suggestBestThreadingModel();
	osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::SingleThreaded;

    m_viewer.setThreadingModel(threadingModel);
	m_near_plane = 0.01;
	m_far_plane = 10000.0;
	m_projection = PROJECTION_PERSPECTIVE;
	m_viewer.setKeyEventSetsDone(0);

	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = "MainViewer";
    traits->windowDecoration = false;
    traits->x = 0;
    traits->y = 0;
    traits->width = width();
    traits->height = height();
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();
	osg::ref_ptr<osgQt::GraphicsWindowQt> gw = new osgQt::GraphicsWindowQt( traits.get() );

    m_main_view = new osgViewer::View();
	m_viewer.addView( m_main_view );

	osg::Camera* camera = m_main_view->getCamera();
	camera->setGraphicsContext( gw );
	camera->setClearColor( osg::Vec4(0.96f,	0.96f,  0.96f,  1.0f) );
	setProjection( ViewerProjection::PROJECTION_PERSPECTIVE );

	m_main_view->addEventHandler( new osgViewer::StatsHandler );

	m_gl_widget = gw->getGLWidget();

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->setContentsMargins( 0, 0, 0, 1 );
	vbox->addWidget( m_gl_widget );
    setLayout( vbox );
	
   connect( &m_timer, SIGNAL(timeout()), this, SLOT(update()) );
}

ViewerWidget::~ViewerWidget()
{
}

void ViewerWidget::paintEvent( QPaintEvent* event )
{
#if QT_VERSION >= 0x04FF00
	QWindow* w_gl = m_gl_widget->windowHandle();
	QWindow* w = windowHandle();
	if( w->isExposed() && w_gl->isExposed() )
	{
		m_viewer.frame();
	}
#else
	m_viewer.frame();
#endif
}

void ViewerWidget::setProjection( ViewerProjection p )
{
	m_projection = p;
	int w = width();
	int h = height();
	double ratio = double(h)/double(w);

	osg::Camera* cam = m_main_view->getCamera();
	cam->setViewport( new osg::Viewport(0, 0, w, h) );
	//cam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	
	if( m_projection == PROJECTION_PARALLEL )
	{
		cam->setProjectionMatrixAsOrtho(-100, 100, -100.0*ratio, 100.0*ratio, m_near_plane-1000, m_near_plane/0.0005+100 );
	}
	else if( m_projection == PROJECTION_PERSPECTIVE )
	{
		cam->setProjectionMatrixAsPerspective( 45.0, 1.0/ratio, m_near_plane, m_far_plane );
	}
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
	m_rootnode = root;
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

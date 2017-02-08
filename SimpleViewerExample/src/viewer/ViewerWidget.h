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

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgGA/StandardManipulator>
#include <osg/Material>
#include <QtCore/qglobal.h>
#include <QtCore/QTimer>
#include <qwidget.h>

#include "GraphicsWindowQt.h"
class IfcPlusPlusSystem;

//\brief class to combine OSG Viewer and Qt widget
class ViewerWidget : public QWidget
{
	Q_OBJECT

public:
    ViewerWidget( IfcPlusPlusSystem* sys, QWidget* parent = nullptr );
	~ViewerWidget();

	QtOSGWidget*				getOpenGLWidget();
	GraphicsWindowQt*			getGraphicsWindowQt();
	osgViewer::View*			getMainView();
	osgViewer::CompositeViewer* getCompositeViewer();
	osg::Camera*				getHeadUpCamera() { return m_hud_camera.get(); }
	osgGA::StandardManipulator*	getCameraManipulator() { return m_camera_manipulator.get(); }
	void updateCamera();
	void setRootNode( osg::Group* node );
	void stopTimer();
	void startTimer();
		
	virtual QSize minimumSizeHint() const { return QSize( 100, 100 ); }
	virtual QSize sizeHint() const { return QSize( 800, 600 ); }
	virtual void paintEvent( QPaintEvent* event );
	virtual void resizeEvent( QResizeEvent * );
	
public slots:
	void slotAnimationFrame();

protected:
	IfcPlusPlusSystem*							m_system;
	QWidget*									m_parent = nullptr;
	std::string									m_window_name;
	QTimer										m_timer;
	double										m_shinyness = 35;
	double										m_near_plane = 0.001;
	double										m_far_plane = 1000.0;
	osg::ref_ptr<osgViewer::View>				m_main_view;
	osg::ref_ptr<osgViewer::CompositeViewer>	m_composite_viewer;
	osg::ref_ptr<osg::Camera>					m_hud_camera;
	osg::ref_ptr<GraphicsWindowQt>				m_graphics_window;
	osg::ref_ptr<osgGA::StandardManipulator>	m_camera_manipulator;
	osg::ref_ptr<osg::Material>					m_material_default;
	osg::ref_ptr<osg::StateSet>					m_stateset_selected;
	osg::ref_ptr<osg::StateSet>					m_stateset_default;
	osg::ref_ptr<osg::StateSet>					m_stateset_transparent;

	void initGLWidgetAndViewer( int width, int height, QWidget* parent );
};

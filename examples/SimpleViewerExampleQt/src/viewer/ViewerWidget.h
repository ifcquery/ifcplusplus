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

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgGA/StandardManipulator>
#include <osg/Material>
#include <QtCore/qglobal.h>
#include <QtCore/QTimer>
#include <QtWidgets/QWidget>

#include "GraphicsWindowQt.h"
class IfcPlusPlusSystem;

//\brief class to combine OSG Viewer and Qt widget
class ViewerWidget : public QWidget
{
	Q_OBJECT

public:
    ViewerWidget(IfcPlusPlusSystem* sys, QWidget* parent = nullptr );
	~ViewerWidget();

	QtOSGWidget*				getOpenGLWidget();
	GraphicsWindowQt*			getGraphicsWindowQt();
	osgViewer::View*			getMainView();
	osgViewer::CompositeViewer* getCompositeViewer();
	osg::Camera*				getHeadUpCamera() { return m_hud_camera.get(); }
	osgGA::StandardManipulator*	getCameraManipulator() { return m_camera_manipulator.get(); }
	void updateCamera();
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

	void initGLWidgetAndViewer();
};

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

#pragma once

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgQt/GraphicsWindowQt>

#include <QtCore/qglobal.h>
#include <QtCore/QTimer>
#include <qwidget.h>

//\brief class to combine OSG Viewer and Qt widget

class ViewerWidget : public QWidget
{
public:
	enum ViewerProjection
	{
		PROJECTION_PARALLEL,
		PROJECTION_PERSPECTIVE
	};

    ViewerWidget( QWidget* parent = nullptr );
	~ViewerWidget();
		
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	virtual void paintEvent( QPaintEvent* event );
	
	void setProjection( ViewerProjection p );
	void setRootNode( osg::Group* node );
	void stopTimer();
	void startTimer();
	osgViewer::CompositeViewer& getViewer()	{ return m_viewer; }
	osgViewer::View* getMainView() {  return m_main_view.get(); }

protected:
	osgViewer::CompositeViewer		m_viewer;
	osg::ref_ptr<osgViewer::View>	m_main_view;

	osgQt::GLWidget*				m_gl_widget;
	ViewerProjection				m_projection;

    QTimer							m_timer;
	double							m_near_plane;
	double							m_far_plane;
	osg::ref_ptr<osg::Group>		m_rootnode;
	osg::ref_ptr<osg::Group>		m_model_node;
	osg::ref_ptr<osg::Camera>		m_hud_camera;
	virtual void resizeEvent(QResizeEvent *);
};

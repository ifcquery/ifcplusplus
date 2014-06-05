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

#ifdef _DEBUG

#include <QtCore/qglobal.h>
#include <QMainWindow>
#include <QLabel>

#include <osg/Group>
#include <osg/Vec4>

#include "ifcppgeometry/IncludeCarveHeaders.h"

class ViewerWidget;
class ViewController;

class DebugViewer : public QMainWindow
{
	Q_OBJECT

public:
	DebugViewer();
	~DebugViewer();

	static void renderPolyhedronWrapper(void* obj_ptr, const carve::input::PolyhedronData* poly, const osg::Vec4f& color, const bool wireframe);
	static void renderMeshsetWrapper(void* obj_ptr, const carve::mesh::MeshSet<3>* meshset, const osg::Vec4f& color, const bool wireframe);
	static void renderPolylineWrapper(void* obj_ptr, const carve::input::PolylineSetData* poly_line, const osg::Vec4f& color);
	static void renderPathsWrapper(void* obj_ptr, const std::vector<std::vector<carve::geom::vector<2> > >& paths );
	
	void renderPolyhedron( const carve::input::PolyhedronData* poly, const osg::Vec4f& color, const bool wireframe );
	void renderMeshset( const carve::mesh::MeshSet<3>* meshset, const osg::Vec4f& color, const bool wireframe );
	void renderPolyline( const carve::input::PolylineSetData* poly_line, const osg::Vec4f& color );
	void renderPaths( const std::vector<std::vector<carve::geom::vector<2> > >& paths );

	ViewerWidget*			m_viewer_widget;
	ViewController*			m_view_controller;
	bool m_cull_front;
	bool m_cull_back;

protected:
	void closeEvent( QCloseEvent *event );

public slots:
	void slotBtnZoomBoundingsClicked();
	void slotBtnWireframeClicked();
	void slotCullFrontFaces( int state );
	void slotCullBackFaces( int state );
};
#endif

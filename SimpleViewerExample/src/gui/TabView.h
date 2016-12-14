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

#include <qglobal.h>
#include <QWidget>
#include <QSpinBox>
#include <QCheckBox>

class IfcPlusPlusSystem;
class ViewerWidget;

class TabView : public QWidget
{
	Q_OBJECT
public:
	TabView( IfcPlusPlusSystem* s, ViewerWidget* vw );

private:
	IfcPlusPlusSystem*	m_system;
	ViewerWidget*	m_vw;
	QSpinBox*		m_spinbox_circle_vertices;
	QCheckBox*		m_check_show_curve_representations;
	bool			m_cull_front;
	bool			m_cull_back;
	
private slots:
	void slotToggleSceneLight();
	void slotCullFrontFaces( int state );
	void slotCullBackFaces( int state );
	void slotSetNumVertices(int);
	void slotShowCurves( int );
};

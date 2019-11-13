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
	void slotIgnoreProfileRadius(int);
};

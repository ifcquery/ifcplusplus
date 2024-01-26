/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2024 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>

class IfcPlusPlusSystem;
class ViewerWidget;

class SettingsWidget : public QWidget
{
	Q_OBJECT
public:
	SettingsWidget( IfcPlusPlusSystem* s, ViewerWidget* vw, bool autoHideFileWidget);

private:
	IfcPlusPlusSystem*	m_system;
	ViewerWidget*	m_vw;
	QSpinBox*		m_spinboxCircleVertices;
	QCheckBox*		m_checkShowCurveRepresentations;
	QCheckBox*		m_checkAutoHideFileWidget;
	bool			m_cullFront;
	bool			m_cullBack;

	void paintEvent(QPaintEvent* /*event*/);
	virtual QSize	sizeHint() const { return QSize(200, 120); }
	
signals:
	void signalAutoHideFileWidget(bool);

private slots:
	void slotAutoHideFileWidget(int state);
	void slotCullFrontFaces( int state );
	void slotCullBackFaces( int state );
	void slotSetNumVertices(int);
	void slotShowCurves( int );
	void slotIgnoreProfileRadius(int);
};

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

#include <QtCore/qglobal.h>
#include <QMainWindow>

class QLabel;
class QSplitter;
class QToolBar;
class IfcPlusPlusSystem;
class TabReadWrite;
class ViewerWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow( IfcPlusPlusSystem* sys, ViewerWidget* vw, QWidget *parent = 0 );
	~MainWindow();

	TabReadWrite* getTabReadWrite() { return m_tab_read_write; }

protected:
	void closeEvent( QCloseEvent *event );

private:
	IfcPlusPlusSystem* m_system;
	ViewerWidget*	m_viewer_widget;
	QTabWidget*		m_tabwidget;
	QLabel*			m_label_status_cursor;
	TabReadWrite*	m_tab_read_write;
	QSplitter*		m_splitter;

	QToolBar*		m_file_toolbar;
	QToolBar*		m_edit_toolbar;

	void createTabWidget();

signals:
	void signalMainWindowClosed();

private slots:
	void slotBtnZoomBoundingsClicked();
	void slotBtnWireframeClicked();
	void slotBtnRemoveSelectedObjectsClicked();
};

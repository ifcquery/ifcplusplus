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

#include <QtCore/qglobal.h>

#include <qaction.h>
#include <qtoolbar.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <QDockWidget>
#include <QtCore/qfile.h>
#include <QtCore/qsettings.h>

#include "IfcPlusPlusSystem.h"
#include "ViewController.h"
#include "viewer/ViewerWidget.h"
#include "viewer/Orbit3DManipulator.h"
#include "cmd/CmdRemoveSelectedObjects.h"
#include "cmd/CommandManager.h"
#include "ifcppgeometry/GeomUtils.h"
#include "TabReadWrite.h"
#include "TabView.h"
#include "IfcTreeWidget.h"
#include "MainWindow.h"

MainWindow::MainWindow( IfcPlusPlusSystem* sys, ViewerWidget* vw, QWidget *parent) : m_system(sys), m_viewer_widget(vw), QMainWindow(parent)
{
	m_system = sys;
	setWindowTitle("IFC++ Viewer");
	setWindowIcon( QIcon( ":img/IfcPlusPlusViewerWindowIcon.png" ) );
	
	// global style sheet definitions
	QFile file( ":styles.css" );
	file.open( QFile::ReadOnly );
	QString styleSheet = QLatin1String( file.readAll() );
	setStyleSheet( styleSheet );
	createTabWidget();

	//QDockWidget *dock = new QDockWidget( "View options", this);
	//dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea);
	//addDockWidget(Qt::TopDockWidgetArea, dock);

	QAction* zoom_bounds_btn = new QAction(QIcon(":img/zoomBoundings.png"), "&Zoom to boundings", this );
	zoom_bounds_btn->setShortcut(tr("Ctrl+Z"));
	zoom_bounds_btn->setStatusTip("Zoom to boundings");
	connect(zoom_bounds_btn, SIGNAL(triggered()), this, SLOT(slotBtnZoomBoundingsClicked()));

	QAction* wireframe = new QAction(QIcon(":img/TabViewWireframe.png"), "&Wireframe [w]", this );
	wireframe->setCheckable( true );
	//wireframe->setShortcut(tr("w"));
	wireframe->setStatusTip("Wireframe [w]");
	connect(wireframe, SIGNAL(triggered()), this, SLOT(slotBtnWireframeClicked()));

	QAction* remove_selected_objects = new QAction(QIcon(":img/RemoveSelectedObjects.png"), "&Remove selected objects [del]", this );
	remove_selected_objects->setStatusTip("Remove selected objects [del]");
	connect(remove_selected_objects, SIGNAL(triggered()), this, SLOT(slotBtnRemoveSelectedObjectsClicked()));


	// TODO: exploded view or storey shift
	// TODO: transparency for selected objects

	m_file_toolbar = new QToolBar();
	m_file_toolbar->setObjectName("FileToolbar");
	m_file_toolbar->addAction(zoom_bounds_btn);
	m_file_toolbar->addAction(wireframe);
	m_file_toolbar->addAction(remove_selected_objects);
	addToolBar( Qt::LeftToolBarArea, m_file_toolbar );

	// building structure widget
	QDockWidget *dock = new QDockWidget(tr("Project structure"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	
	IfcTreeWidget* ifc_tree_widget = new IfcTreeWidget( m_system );
	//connect( m_ifc_tree_widget, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( slotTreewidgetSelectionChanged(QTreeWidgetItem*, QTreeWidgetItem*) ) );
	dock->setWidget( ifc_tree_widget );

	m_splitter = new QSplitter( Qt::Vertical );
	m_splitter->setContentsMargins( 0, 0, 0, 0 );
	m_splitter->addWidget( m_viewer_widget );
	m_splitter->addWidget( m_tabwidget );
	m_splitter->setStretchFactor( 0, 2 );
	m_splitter->setStretchFactor( 1, 0 );

	QList<int> splitter_sizes;
	splitter_sizes << 400 << 100;
	m_splitter->setSizes( splitter_sizes );

	// status bar
	QStatusBar* status = new QStatusBar();
	m_label_status_cursor = new QLabel( "0.000, 0.000, 0.000" );
	status->addWidget( m_label_status_cursor, 0 );
	status->setSizeGripEnabled( true );
	setStatusBar( status );

	// central widget
	setCentralWidget( m_splitter );

	// restore geometry
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	QStringList keys = settings.allKeys();
	if( keys.contains( "MainWindowGeometry" ) )
	{
		restoreGeometry(settings.value("MainWindowGeometry").toByteArray());
	}
	else
	{
		showMaximized();
	}
	if( keys.contains( "mainWindowState" ) )
	{
		restoreState(settings.value("mainWindowState").toByteArray());
	}
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent( QCloseEvent *event )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("MainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
	
	// for some reason, closeEvent or other close methods are not called on widgets, so do it manually here...
	m_tab_read_write->closeEvent( event );
	QMainWindow::closeEvent( event );

	emit( signalMainWindowClosed() );
}

void MainWindow::createTabWidget()
{
	m_tabwidget = new QTabWidget();
	m_tabwidget->setIconSize( QSize( 19, 19 ) );

	m_tab_read_write	= new TabReadWrite( m_system, m_viewer_widget, this );
	QWidget* tab_view	= new TabView( m_system, m_viewer_widget );

	m_tabwidget->addTab( m_tab_read_write,	/*QIcon( ":img/material.png" ),*/	"Read/Write IFC" );
	m_tabwidget->addTab( tab_view,			/*QIcon( ":img/material.png" ),*/	"View" );
}

void MainWindow::slotBtnZoomBoundingsClicked()
{
	osg::BoundingSphere bs = m_system->getViewController()->getModelNode()->computeBound();
	
	osgViewer::View* main_view = m_viewer_widget->getMainView();
	if( main_view )
	{
		osgGA::CameraManipulator* camera_manip = main_view->getCameraManipulator();
		Orbit3DManipulator* orbit_manip = dynamic_cast<Orbit3DManipulator*>( camera_manip );
		if( orbit_manip )
		{
			orbit_manip->zoomToBoundingSphere( bs );
		}
	}

	//zoomToBoundingSphere( m_viewer_widget, bs );
	//m_viewer_widget->frame();
	//zoomToBoundingSphere( m_viewer_widget, bs );
	// TODO: fancy animation path, views from x/y/z and isometry
}

void MainWindow::slotBtnWireframeClicked()
{
	QAction* toggle_btn = (QAction*)sender();

	if( toggle_btn->isChecked() )
	{
		m_system->getViewController()->setViewerMode( ViewController::VIEWER_MODE_WIREFRAME );
		//m_viewer_widget->setViewerMode( ViewerWidget::VIEWER_MODE_WIREFRAME );
	}
	else
	{
		m_system->getViewController()->setViewerMode( ViewController::VIEWER_MODE_SHADED );
		//m_viewer_widget->setViewerMode( ViewerWidget::VIEWER_MODE_SHADED );
	}
}

void MainWindow::slotBtnRemoveSelectedObjectsClicked()
{
	shared_ptr<CmdRemoveSelectedObjects> cmd_remove( new CmdRemoveSelectedObjects( m_system ) );
	m_system->getCommandManager()->executeCommand( cmd_remove );
}

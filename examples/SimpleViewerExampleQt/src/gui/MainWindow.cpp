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


#include <QtWidgets/QAction>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QDockWidget>
#include <QtCore/QFile>
#include <QtWidgets/QStatusBar>
#include <QtCore/QSettings>
#include <QtWidgets/QToolButton>

#include "IncludeGeometryHeaders.h"
#include "EntityAttributeWidget.h"
#include "IfcPlusPlusSystem.h"
#include "viewer/ViewerWidget.h"
#include "viewer/OrbitCameraManipulator.h"
#include "cmd/CmdRemoveSelectedObjects.h"
#include "cmd/CommandManager.h"
#include "TabReadWrite.h"
#include "TabView.h"
#include "IfcTreeWidget.h"
#include "MainWindow.h"

MainWindow::MainWindow( IfcPlusPlusSystem* sys, QWidget *parent) : m_system(sys), QMainWindow(parent)
{
	setWindowTitle("IFC++ example application");
	setWindowIcon(QIcon(":img/IfcPlusPlusViewerWindowIcon.svg"));
	
	// global style sheet definitions
	QFile file( ":styles.css" );
	file.open( QFile::ReadOnly );
	QString styleSheet = QLatin1String( file.readAll() );
	setStyleSheet( styleSheet );
	
	m_viewer_widget = new ViewerWidget(sys);
	OrbitCameraManipulator* camera_manip = new OrbitCameraManipulator(sys);
	m_viewer_widget->getMainView()->setCameraManipulator(camera_manip);
	m_viewer_widget->setRootNode(sys->getRootNode());
	createTabWidget();

	QToolButton* zoom_bounds_btn = new QToolButton(this );
	zoom_bounds_btn->setIcon(QIcon(":img/zoomToBounds.svg"));
	zoom_bounds_btn->setShortcut(tr("Ctrl+Z"));
	zoom_bounds_btn->setStatusTip("Zoom to boundings");
	zoom_bounds_btn->setFixedSize(35, 35);
	zoom_bounds_btn->setIconSize(QSize(35, 35));
	zoom_bounds_btn->setStyleSheet("QToolButton{border:0px;background:none;}");
	connect(zoom_bounds_btn, &QToolButton::clicked, this, &MainWindow::slotBtnZoomBoundingsClicked);

	QToolButton* remove_selected_objects = new QToolButton( this );
	remove_selected_objects->setIcon(QIcon(":img/RemoveSelectedObjects.svg"));
	remove_selected_objects->setStatusTip("Remove selected objects [del]");
	remove_selected_objects->setFixedSize(35, 35);
	remove_selected_objects->setIconSize(QSize(35, 35));
	remove_selected_objects->setStyleSheet("QToolButton{border:0px;background:none;}");
	connect(remove_selected_objects, &QToolButton::clicked, this, &MainWindow::slotBtnRemoveSelectedObjectsClicked);

	m_file_toolbar = new QToolBar();
	m_file_toolbar->setObjectName("FileToolbar");
	m_file_toolbar->addWidget(zoom_bounds_btn);
	m_file_toolbar->addWidget(remove_selected_objects);
	addToolBar( Qt::LeftToolBarArea, m_file_toolbar );

	
	// building structure widget
	QDockWidget *dock1 = new QDockWidget(tr("Project structure"), this);
	dock1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock1->setObjectName("dockWidgetProjectStructure");
	addDockWidget(Qt::RightDockWidgetArea, dock1);
	
	IfcTreeWidget* ifc_tree_widget = new IfcTreeWidget( m_system );
	dock1->setWidget( ifc_tree_widget );


	QDockWidget* dock2 = new QDockWidget(tr("Attributes"), this);
	dock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock2->setObjectName("dockWidgetAttributes");
	addDockWidget(Qt::RightDockWidgetArea, dock2);

	EntityAttributeWidget* attribute_widget = new EntityAttributeWidget(m_system, this);
	dock2->setWidget(attribute_widget);

	m_splitter = new QSplitter( Qt::Vertical );
	m_splitter->setContentsMargins( 0, 0, 0, 0 );
	m_splitter->addWidget( m_viewer_widget );
	m_splitter->addWidget( m_tabwidget );
	m_splitter->setStretchFactor( 0, 2 );
	m_splitter->setStretchFactor( 1, 0 );

	QList<int> splitter_sizes;
	splitter_sizes << 400 << 100;
	m_splitter->setSizes( splitter_sizes );

	// progress bar
	m_progress_bar = new QProgressBar();
	m_progress_bar->setRange(0, 1000);

	// status bar
	QStatusBar* status_bar = new QStatusBar();
	m_label_status_cursor = new QLabel( "0.000, 0.000, 0.000" );
	status_bar->setStyleSheet("QStatusBar{ background-color: #83a7b7; }");
	status_bar->addWidget( m_label_status_cursor, 0 );
	status_bar->addWidget(m_progress_bar, 1);
	status_bar->addWidget(new QLabel(" "), 1);
	status_bar->setSizeGripEnabled( true );
	setStatusBar(status_bar);

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
		QRect rec = QApplication::desktop()->screenGeometry();
		setGeometry(rec.width()*0.2, rec.height()*0.2, rec.width()*0.6, rec.height()*0.7);
	}
	if( keys.contains( "mainWindowState" ) )
	{
		restoreState(settings.value("mainWindowState").toByteArray());
	}

	m_viewer_widget->setFocus();
	m_viewer_widget->startTimer();
	m_viewer_widget->getMainView()->addEventHandler(sys);
}

MainWindow::~MainWindow(){}

void MainWindow::closeEvent( QCloseEvent *event )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("MainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
	
	m_viewer_widget->getCompositeViewer()->setDone(true);
	m_viewer_widget->stopTimer();
	m_tab_read_write->closeEvent( event );
	QMainWindow::closeEvent( event );

	emit( signalMainWindowClosed() );
}

void MainWindow::createTabWidget()
{
	m_tabwidget = new QTabWidget();
	m_tabwidget->setIconSize( QSize( 19, 19 ) );

	m_tab_read_write	= new TabReadWrite( m_system, m_viewer_widget, this );
	connect(m_tab_read_write, &TabReadWrite::signalProgressValue, this, &MainWindow::slotProgressValue);
	QWidget* tab_view	= new TabView( m_system, m_viewer_widget );

	
	QString about = "SimpleViewerExample based on IFC++, OpenSceneGraph (OSG) and Qt.<br>\
For customized features like import, export to other file formats, please refer to <a href=\"http://www.ifcquery.com\">www.ifcquery.com</a><br>\
IFC++ can also be used in server applications without dependencies like Qt and OSG";
	
	QLabel* label_about = new QLabel(about);
	label_about->setTextFormat(Qt::RichText);
	label_about->setTextInteractionFlags(Qt::TextBrowserInteraction);
	label_about->setOpenExternalLinks(true);

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addWidget(label_about);
	QWidget* tab_about = new QWidget();
	tab_about->setContentsMargins(8, 8, 8, 8);
	tab_about->setLayout(vbox);

	m_tabwidget->addTab( m_tab_read_write,	"Read/Write IFC" );
	m_tabwidget->addTab( tab_view,			"View" );
	m_tabwidget->addTab( tab_about,			"About");
}

void MainWindow::slotBtnZoomBoundingsClicked()
{
	osg::BoundingSphere bs = m_system->getModelNode()->computeBound();
	
	osgViewer::View* main_view = m_viewer_widget->getMainView();
	if( main_view )
	{
		osgGA::CameraManipulator* camera_manip = main_view->getCameraManipulator();
		OrbitCameraManipulator* orbit_manip = dynamic_cast<OrbitCameraManipulator*>( camera_manip );
		if( orbit_manip )
		{
			orbit_manip->zoomToBoundingSphere( bs );
		}
	}
}

void MainWindow::slotBtnRemoveSelectedObjectsClicked()
{
	shared_ptr<CmdRemoveSelectedObjects> cmd_remove( new CmdRemoveSelectedObjects( m_system ) );
	m_system->getCommandManager()->executeCommand( cmd_remove );
}

void MainWindow::slotProgressValue(double progress_value_in, const std::string& progress_type)
{
	double progress_value = progress_value_in;
	if (progress_value >= 0.0)
	{
		if (progress_type.compare("parse") == 0)
		{
			progress_value = progress_value*0.3;
		}
		else if (progress_type.compare("geometry") == 0)
		{
			progress_value = 0.3 + progress_value*0.6;
		}
		else if (progress_type.compare("scenegraph") == 0)
		{
			progress_value = 0.9 + progress_value*0.1;
		}

		if (abs(m_current_progress_value - progress_value) > 0.015 || progress_value == 0.0 || progress_value == 1.0)
		{
			m_progress_bar->setValue((int)(progress_value * 1000));
			QApplication::processEvents();
			m_current_progress_value = progress_value;
		}
	}
}

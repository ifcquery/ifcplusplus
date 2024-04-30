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


#include <QAction>
#include <QFile>
#include <QKeyEvent>
#include <QStatusBar>
#include <QSettings>
#include <QToolButton>

#include "IncludeGeometryHeaders.h"
#include "EntityAttributeWidget.h"
#include "IfcPlusPlusSystem.h"
#include "viewer/ViewController.h"
#include "viewer/ViewerUtil.h"
#include "viewer/ViewerWidget.h"
#include "viewer/Orbit3DManipulator.h"
#include "cmd/CmdRemoveSelectedObjects.h"
#include "cmd/CommandManager.h"
#include "OpenFileWidget.h"
#include "SettingsWidget.h"
#include "IfcTreeWidget.h"
#include "MainWindow.h"

MainWindow::MainWindow( IfcPlusPlusSystem* sys, QWidget *parent) : m_system(sys), QMainWindow(parent)
{
	setWindowTitle("IFC++ example application");
	setWindowIcon(QIcon(":img/IfcPlusPlusViewerWindowIcon.png"));
	
	// global style sheet definitions
	QFile file( ":styles.css" );
	file.open( QFile::ReadOnly );
	QString styleSheet = QLatin1String( file.readAll() );
	setStyleSheet( styleSheet );

	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	QStringList keys = settings.allKeys();
	if (keys.contains("AutoHideFileWidget"))
	{
		m_autoHideFileWidget= settings.value("AutoHideFileWidget").toBool();
	}
		
	m_viewerWidget = new ViewerWidget(sys);
	sys->getViewController()->setGLWidget(m_viewerWidget->getOpenGLWidget());
	
	// create OpenFileWidget
	m_openFileWidget = new OpenFileWidget(m_system, this);
	m_openFileWidget->setAutoFillBackground(true);
	m_openFileWidget->setObjectName("OpenFileWidget");
	m_openFileWidget->setParent(m_viewerWidget);
	connect(m_openFileWidget, &OpenFileWidget::signalProgressValue, this, &MainWindow::slotProgressValue);
	connect(m_openFileWidget, &OpenFileWidget::signalClearSignalQueue, this, &MainWindow::slotClearSignalQueue);

	m_buttonToggleOpenFileWidget = new QPushButton("<<");
	m_buttonToggleOpenFileWidget->setMinimumHeight(80);
	m_buttonToggleOpenFileWidget->setFixedWidth(30);
	m_buttonToggleOpenFileWidget->setMaximumWidth(30);
	m_buttonToggleOpenFileWidget->setContentsMargins(0,0,0,0);
	connect(m_buttonToggleOpenFileWidget, &QPushButton::clicked, this, [this]() {
		slotToggleOpenFileWidget();
	});
	m_buttonToggleOpenFileWidget->setParent(m_viewerWidget);

	QToolButton* zoom_bounds_btn = new QToolButton(m_viewerWidget);
	zoom_bounds_btn->setObjectName("ZoomToBounds");
	zoom_bounds_btn->setIcon(QIcon(":img/bounds32.png"));
	zoom_bounds_btn->setIconSize(QSize(24, 24));
	zoom_bounds_btn->setShortcut(tr("Ctrl+Z"));
	zoom_bounds_btn->setStatusTip("Zoom to boundings");
	zoom_bounds_btn->setFixedSize(24, 24);
	zoom_bounds_btn->setGeometry(3, 6, 24, 24);
	connect(zoom_bounds_btn, &QToolButton::clicked, this, &MainWindow::slotBtnZoomBoundingsClicked);

		
	m_settingsWidget = new SettingsWidget(m_system, m_viewerWidget, m_autoHideFileWidget);
	connect(m_settingsWidget, &SettingsWidget::signalAutoHideFileWidget, this, [this](bool autoHide) {
		QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
		settings.setValue("AutoHideFileWidget", autoHide);
		m_autoHideFileWidget = autoHide;
	});
	

	QToolButton* settings_btn = new QToolButton(m_viewerWidget);
	settings_btn->setObjectName("SettingsButton");
	settings_btn->setIcon(QIcon(":img/settings32.png"));
	settings_btn->setIconSize(QSize(24, 24));
	settings_btn->setStatusTip("Settings");
	settings_btn->setFixedSize(24, 24);
	settings_btn->setGeometry(3, 42, 24, 24);
	connect(settings_btn, &QToolButton::clicked, this, &MainWindow::slotSettingsClicked);

	QString about = "SimpleViewerExample based on IFC++, OpenSceneGraph (OSG) and Qt.<br>\
Source code is available on <a href=\"http://www.github.com/ifcquery/ifcplusplus\">http://www.github.com/ifcquery/ifcplusplus</a><br>\
For customized features like import, export to other file formats, please refer to <a href=\"http://www.ifcquery.com\">www.ifcquery.com</a><br>\
IFC++ can also be used in server applications without dependencies like Qt and OSG";

	m_labelAbout = new QLabel(about);
	m_labelAbout->setTextFormat(Qt::RichText);
	m_labelAbout->setTextInteractionFlags(Qt::TextBrowserInteraction);
	m_labelAbout->setOpenExternalLinks(true);

	QToolButton* buttonAbout = new QToolButton(m_viewerWidget);
	buttonAbout->setObjectName("buttonAbout");
	buttonAbout->setIcon(QIcon(":img/IfcPlusPlus.png"));
	buttonAbout->setIconSize(QSize(24, 24));
	buttonAbout->setStatusTip("Settings");
	buttonAbout->setFixedSize(24, 24);
	buttonAbout->setGeometry(3, 75, 24, 24);
	connect(buttonAbout, &QToolButton::clicked, this, [this]() {
		if (m_labelAbout->isVisible()) { m_labelAbout->hide(); return; }
		if (m_settingsWidget->isVisible()) { m_settingsWidget->hide(); }
		m_labelAbout->setParent(m_viewerWidget);
		int w = m_viewerWidget->width() - 40;
		m_labelAbout->setGeometry(40, 10, w, 50);
		m_labelAbout->setVisible(true);
	 });

	// building structure widget
	IfcTreeWidget* ifc_tree_widget = new IfcTreeWidget( m_system );
	connect(ifc_tree_widget, &IfcTreeWidget::signalZoomToObject, this, &MainWindow::slotZoomToObject);

	EntityAttributeWidget* attribute_widget = new EntityAttributeWidget(m_system, this);

	QList<int> sizesTreeViews;
	sizesTreeViews.append(300);
	sizesTreeViews.append(100);

	QSplitter* attributesSplitter = new QSplitter(Qt::Vertical);
	attributesSplitter->addWidget(ifc_tree_widget);
	attributesSplitter->addWidget(attribute_widget);
	attributesSplitter->setSizes(sizesTreeViews);


	QSplitter* mainSplitter = new QSplitter(Qt::Horizontal);
	mainSplitter->addWidget(m_viewerWidget);
	mainSplitter->addWidget(attributesSplitter);
	mainSplitter->setStretchFactor(0, 4);
	mainSplitter->setStretchFactor(1, 1);
	QList<int> sizesMainSplitter;
	sizesMainSplitter.append(400);
	sizesMainSplitter.append(100);
	mainSplitter->setSizes(sizesMainSplitter);

	// progress bar
	m_progressBar = new QProgressBar();
	m_progressBar->setRange(0, 1000);

	// status bar
	QStatusBar* status_bar = new QStatusBar();
	m_labelStatusCursor = new QLabel( "0.000, 0.000, 0.000" );
	status_bar->setStyleSheet("QStatusBar{ background-color: #83a7b7; }");
	status_bar->addWidget( m_labelStatusCursor, 0 );
	status_bar->addWidget(m_progressBar, 1);
	status_bar->addWidget(new QLabel(" "), 1);
	status_bar->setSizeGripEnabled( true );
	setStatusBar(status_bar);

	// central widget
	setCentralWidget(mainSplitter);

	// restore geometry
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

	m_viewerWidget->setFocus();
	m_viewerWidget->startTimer();
	m_viewerWidget->getMainView()->addEventHandler(sys);
	updateOpenFileWidget();

	connect(m_system, &IfcPlusPlusSystem::signalModelLoadingDone, this, &MainWindow::slotFileLoadingDone);
	connect(m_system, &IfcPlusPlusSystem::signalCursorCoordinates, this, &MainWindow::slotCursorCoordinates);
}

MainWindow::~MainWindow(){}

void MainWindow::closeEvent( QCloseEvent *event )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("MainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
	
	m_viewerWidget->getCompositeViewer()->setDone(true);
	m_viewerWidget->stopTimer();
	m_openFileWidget->closeEvent( event );
	QMainWindow::closeEvent( event );

	emit( signalMainWindowClosed() );
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	updateOpenFileWidget();
}

void MainWindow::showEvent(QShowEvent* e)
{
	updateOpenFileWidget();
}

void MainWindow::slotFileLoadingDone()
{
	m_viewerWidget->update();

	osgViewer::View* main_view = m_viewerWidget->getMainView();
	if (main_view)
	{
		osgGA::CameraManipulator* camera_manip = main_view->getCameraManipulator();
		Orbit3DManipulator* orbit_manip = dynamic_cast<Orbit3DManipulator*>(camera_manip);
		if (orbit_manip)
		{
			osg::BoundingSphere bs = m_system->getViewController()->getModelNode()->computeBound();
			orbit_manip->zoomToBoundingSphere(bs);
		}
	}

	if (m_autoHideFileWidget)
	{
		slotToggleOpenFileWidget();
	}

	m_progressBar->setValue(1000);
	QCoreApplication::removePostedEvents(this);
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Control)
	{
		m_system->setCtrlKeyDown(true);
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Control)
	{
		m_system->setCtrlKeyDown(false);
	}
}

void MainWindow::updateOpenFileWidget()
{
	int w = m_viewerWidget->width();
	int h = m_viewerWidget->height();
	int buttonToggleWidth = m_buttonToggleOpenFileWidget->width();

	if (m_openFileWidget->isVisible())
	{
		int openFileWidgetHeight = m_openFileWidget->height();
		if (openFileWidgetHeight < 120)
		{
			openFileWidgetHeight = 120;
		}
		m_openFileWidget->setParent(m_viewerWidget);
		m_openFileWidget->setGeometry(buttonToggleWidth, h - openFileWidgetHeight, w- buttonToggleWidth, openFileWidgetHeight);
	}

	int openFileWidgetHeight = m_openFileWidget->height();
	m_buttonToggleOpenFileWidget->setParent(m_viewerWidget);
	m_buttonToggleOpenFileWidget->setGeometry(0, h - openFileWidgetHeight, 30, openFileWidgetHeight);
}

void MainWindow::slotToggleOpenFileWidget()
{
	if (m_openFileWidget->isVisible())
	{
		m_openFileWidget->hide();
		m_buttonToggleOpenFileWidget->setText(">>");
	}
	else
	{
		m_openFileWidget->show();
		m_buttonToggleOpenFileWidget->setText("<<");
	}
}

void MainWindow::slotBtnZoomBoundingsClicked()
{
	osg::BoundingSphere bs = m_system->getViewController()->getModelNode()->computeBound();
	
	osgViewer::View* main_view = m_viewerWidget->getMainView();
	if( main_view )
	{
		osgGA::CameraManipulator* camera_manip = main_view->getCameraManipulator();
		Orbit3DManipulator* orbit_manip = dynamic_cast<Orbit3DManipulator*>( camera_manip );
		if( orbit_manip )
		{
			orbit_manip->zoomToBoundingSphere( bs );
		}
	}
}

void MainWindow::slotSettingsClicked()
{
	if (m_settingsWidget->isVisible())
	{
		m_settingsWidget->hide();
		return;
	}
	if (m_labelAbout->isVisible()) { m_labelAbout->hide(); }

	int w = m_viewerWidget->width() - 40;
	int h = 250;
	m_settingsWidget->setParent(m_viewerWidget);
	m_settingsWidget->setGeometry(40, 0, w, h);
	m_settingsWidget->setVisible(true);
}

void MainWindow::slotBtnRemoveSelectedObjectsClicked()
{
	shared_ptr<CmdRemoveSelectedObjects> cmd_remove( new CmdRemoveSelectedObjects( m_system ) );
	m_system->getCommandManager()->executeCommand( cmd_remove );
}

void MainWindow::slotClearSignalQueue()
{
	QCoreApplication::removePostedEvents(this);
}

void MainWindow::slotProgressValue(double progress_value_in, const std::string& progress_type)
{
	if (!sender())
	{
		return;
	}

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

		if (abs(m_currentProgressValue - progress_value) > 0.015 || progress_value == 0.0 || progress_value == 1.0)
		{
			m_progressBar->setValue((int)(progress_value * 1000));
			m_currentProgressValue = progress_value;
		}
	}
}

osg::Group* findNodeByIfcId(osg::Group* group, std::string ifc_guid);

void MainWindow::slotZoomToObject(shared_ptr<BuildingEntity> ifc_object, osg::Group* grp)
{
	if (!m_viewerWidget)
	{
		return;
	}

	const std::string guid = getGUID(ifc_object);
	if (!grp)
	{
		grp = findNodeByIfcId( m_system->getViewController()->getModelNode(), guid);
		if (!grp)
		{
			return;
		}
	}

	osgViewer::View* main_view = m_viewerWidget->getMainView();
	if (!main_view)
	{
		return;
	}

	Orbit3DManipulator* orbit_manip = dynamic_cast<Orbit3DManipulator*>(main_view->getCameraManipulator());
	if (!orbit_manip)
	{
		return;
	}

	orbit_manip->zoomToBoundingSphere(grp->getBound());
}

void MainWindow::slotCursorCoordinates(double x, double y, double z)
{
	m_labelStatusCursor->setText(QString::number(x, 'f', 4) + ", " + QString::number(y, 'f', 4) + "," + QString::number(z, 'f', 4));
}

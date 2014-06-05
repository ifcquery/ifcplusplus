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

#include <iostream>

#include <QtCore/qglobal.h>
#include <QApplication>
#include <QSplashScreen>
#include <osgViewer/CompositeViewer>

#include <ifcpp/model/IfcPPException.h>
#include <ifcppgeometry/GeomUtils.h>
#include <ifcppgeometry/ReaderWriterIFC.h>
#include <ifcppgeometry/RepresentationConverter.h>
#include <ifcppgeometry/ConverterOSG.h>
#include <ifcppgeometry/DebugViewerCallback.h>
#include <ifcppgeometry/CSG_Adapter.h>

#include "gui/TabReadWrite.h"
#include "gui/MainWindow.h"
#include "viewer/ViewerWidget.h"
#include "viewer/Orbit3DManipulator.h"
#include "gui/DebugViewer.h"
#include "ViewController.h"
#include "IfcPlusPlusSystem.h"

class IfcPlusPlusApplication : public QApplication
{
public:
	IfcPlusPlusApplication( int &argc, char **argv ) : QApplication( argc, argv )
	{
	}
	~IfcPlusPlusApplication()
	{
	}
protected:
	virtual bool notify( QObject* receiver, QEvent* e )
	{
		bool errRet = false;
		bool tmp;
		try
		{
			tmp = QApplication::notify (receiver, e);
		}
		catch( std::exception& e )
		{
			std::cout << " * ApplicationEx::notify :" << std::endl;
			std::cout << "IfcPlusPlusApplication: exception occurred : " << e.what() << std::endl;
			// TODO: write file with error report
		}
		catch( std::exception* e )
		{
			std::cout << " * ApplicationEx::notify :" << std::endl;
			std::cout << "IfcPlusPlusApplication: exception occurred : " << e->what() << std::endl;
			// TODO: write file with error report
		}
		catch(...)
		{
			std::cout << "IfcPlusPlusApplication: exception occurred. " << std::endl;
		}
		return errRet;
	}
};

int main(int argc, char *argv[])
{
	IfcPlusPlusApplication app(argc, argv);
	QPixmap pixmap( ":img/IfcPlusPlusViewerSplash.png" );
	QSplashScreen splash( pixmap );

#ifndef _DEBUG
	splash.show();
	app.processEvents();
	QTimer::singleShot( 1500, &splash, SLOT(close()));
#endif

	IfcPlusPlusSystem* sys = new IfcPlusPlusSystem();

#ifdef _DEBUG
	DebugViewer* debug_viewer = new DebugViewer();
	debug_viewer->m_viewer_widget->startTimer();
	debug_viewer->show();
	//createTest( sys->getViewController()->getModelNode(), sys->getViewController()->getRootNode() );
#endif

	ViewerWidget* viewer_widget = new ViewerWidget();
	Orbit3DManipulator* camera_manip = new Orbit3DManipulator( sys );
	viewer_widget->getMainView()->setCameraManipulator( camera_manip );
	viewer_widget->setRootNode( sys->getViewController()->getRootNode() );

	MainWindow* window = new MainWindow( sys, viewer_widget );
	app.connect( window,	SIGNAL(signalMainWindowClosed()),	&app,	SLOT(quit()) );
	
	window->show();
	viewer_widget->setFocus();
	viewer_widget->startTimer();
	viewer_widget->getMainView()->addEventHandler( sys );

	if( argc > 1 )
	{
		std::string arg1 = argv[1];
		
		if( arg1.length() > 4 )
		{
			std::string file_type = arg1.substr(arg1.find_last_of(".") + 1);
			std::transform(file_type.begin(), file_type.end(), file_type.begin(), toupper);

			if( file_type.compare( "IFC" ) == 0 || file_type.compare( "STP" ) == 0  )
			{
				QString path(arg1.c_str());
				window->getTabReadWrite()->slotLoadIfcFile( path );
			}
		}
	}

	int re=0;
	try
	{
		re = app.exec();
	}
	catch( IfcPPException& e )
	{
		std::cout << "IfcPPException in app.exec(): " << e.what();
	}
	catch( std::exception& e )
	{
		std::cout << "std::exception in app.exec(): " << e.what();
	}
	catch( std::exception* e )
	{
		std::cout << "std::exception in app.exec(): " << e->what();
	}

	viewer_widget->getViewer().setDone(true);
	viewer_widget->stopTimer();

	return re;
}

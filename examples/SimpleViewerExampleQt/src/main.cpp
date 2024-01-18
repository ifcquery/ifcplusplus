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

#include <iostream>
#include <string>

#include <QtCore/qglobal.h>
#include <QApplication>
#include <osgViewer/CompositeViewer>

#include <ifcpp/model/BuildingException.h>
#include "gui/MainWindow.h"
#include "gui/OpenFileWidget.h"
#include "IfcPlusPlusSystem.h"
#include "IncludeGeometryHeaders.h"

class IfcPlusPlusApplication : public QApplication
{
public:
	IfcPlusPlusApplication( int &argc, char **argv ) : QApplication( argc, argv )
	{
	}
	~IfcPlusPlusApplication(){}
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
			std::cout << "IfcPlusPlusApplication: exception occurred : " << e.what() << std::endl;
		}
		catch( std::exception* e )
		{
			std::cout << "IfcPlusPlusApplication: exception occurred : " << e->what() << std::endl;
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
	IfcPlusPlusSystem* sys = new IfcPlusPlusSystem();
	MainWindow* window = new MainWindow( sys );
	app.connect( window,	SIGNAL(signalMainWindowClosed()),	&app,	SLOT(quit()) );
	window->show();

#ifdef _DEBUG
	GeomDebugDump::clearMeshsetDump();
#endif

	if( argc > 1 )
	{
		std::string arg1 = argv[1];
		
		if( arg1.length() > 4 )
		{
			std::string file_type = arg1.substr(arg1.find_last_of(".") + 1);
			std::transform( file_type.begin(), file_type.end(), file_type.begin(), []( unsigned char c ) -> unsigned char { return std::toupper( c ); } );

			if( file_type.compare( "IFC" ) == 0 || file_type.compare( "STP" ) == 0  )
			{
				QString path(arg1.c_str());
				window->getOpenFileWidget()->loadIfcFile( path );
			}
		}
	}

	int re=0;
	try
	{
		re = app.exec();
	}
	catch( BuildingException& e )
	{
		std::cout << "BuildingException in app.exec(): " << e.what();
	}
	catch( std::exception& e )
	{
		std::cout << "std::exception in app.exec(): " << e.what();
	}
	catch( std::exception* e )
	{
		std::cout << "std::exception in app.exec(): " << e->what();
	}

	return re;
}

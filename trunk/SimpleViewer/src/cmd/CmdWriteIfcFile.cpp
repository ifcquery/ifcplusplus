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

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <osgDB/Registry>
#include <osg/Switch>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/writer/IfcPPWriterSTEP.h>
#include <ifcppgeometry/ReaderWriterIFC.h>

#include "IfcPlusPlusSystem.h"
#include "ViewController.h"
#include "CmdWriteIfcFile.h"

CmdWriteIfcFile::CmdWriteIfcFile( IfcPlusPlusSystem* system ): Command(system)
{
	if( osgDB::Registry::instance() )
	{
		osgDB::ReaderWriter* rw = osgDB::Registry::instance()->getReaderWriterForExtension( "ifc" );
		ReaderWriterIFC* rw_ifc = dynamic_cast<ReaderWriterIFC*>(rw);
		if( rw_ifc )
		{
			m_reader_writer = rw_ifc;
		}
	}

	if( !m_reader_writer )
	{
		m_reader_writer = new ReaderWriterIFC();
	}
}

CmdWriteIfcFile::~CmdWriteIfcFile()
{

}

void CmdWriteIfcFile::setFilePath( std::string& path_in )
{
	m_file_path = path_in;

}

bool CmdWriteIfcFile::doCmd()
{
	if( m_file_path.length() == 0 )
	{
		return false;
	}

	m_system->getIfcModel()->initFileHeader( m_file_path );
	std::stringstream stream;
	m_reader_writer->getIfcPPWriter()->writeStream( stream, m_system->getIfcModel() );

	QFile file_out( m_file_path.c_str() );
	if( !file_out.open(QIODevice::WriteOnly | QIODevice::Text) )
	{
		return false;
	}

	QTextStream file_out_stream( &file_out );
	file_out_stream << stream.str().c_str();
	file_out.close();

	return true;
}

bool CmdWriteIfcFile::undo()
{
	return true;
}

bool CmdWriteIfcFile::redo()
{
	return true;
}

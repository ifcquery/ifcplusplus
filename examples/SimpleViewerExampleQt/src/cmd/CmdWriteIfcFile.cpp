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

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingObject.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/writer/WriterSTEP.h>

#include "IncludeGeometryHeaders.h"
#include "IfcPlusPlusSystem.h"
#include "CmdWriteIfcFile.h"

CmdWriteIfcFile::CmdWriteIfcFile( IfcPlusPlusSystem* system ): Command(system)
{
}

CmdWriteIfcFile::~CmdWriteIfcFile(){}

void CmdWriteIfcFile::setFilePath( std::wstring& path_in )
{
	m_file_path = path_in;
}

bool CmdWriteIfcFile::doCmd()
{
	if( m_file_path.length() == 0 )
	{
		return false;
	}

	shared_ptr<GeometryConverter> geom_converter = m_system->getGeometryConverter();
	shared_ptr<BuildingModel>& model = geom_converter->getBuildingModel();
	model->initFileHeader( m_file_path );
	std::stringstream stream;

	m_system->getModelWriter()->writeModelToStream( stream, model );

	QFile file_out( QString::fromStdWString( m_file_path.c_str() ) );
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

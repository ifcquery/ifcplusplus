/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
 *
 * This library is open source and may be redistributed and/or modified under the terms of the
 * OpenSceneGraph Public License (OSGPL) version 0.0 or (at your option) any later version.
 * The full license is in LICENSE file included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the OpenSceneGraph Public License for more details.
*/

#include <osgDB/Registry>
#include <osg/Switch>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcppgeometry/ReaderWriterIFC.h>
#include <ifcppgeometry/GeomUtils.h>

#include "Command.h"
#include "IfcPlusPlusSystem.h"
#include "ViewController.h"
#include "CmdLoadIfcFile.h"

CmdLoadIfcFile::CmdLoadIfcFile( IfcPlusPlusSystem* system ): Command(system)
{
	
}

CmdLoadIfcFile::~CmdLoadIfcFile()
{

}

void CmdLoadIfcFile::setFilePath( std::string& path_in )
{
	m_file_path = path_in;

}

bool CmdLoadIfcFile::doCmd()
{
	if( m_file_path.length() == 0 )
	{
		return false;
	}

	osg::Group* model_group = m_system->getViewController()->getModelNode();
	model_group->removeChildren( 0, model_group->getNumChildren() );
	m_system->clearSelection();

	osg::ref_ptr<ReaderWriterIFC> reader_writer = m_system->getReaderWriterIFC();
	reader_writer->resetModel();
	osg::ref_ptr<osgDB::ReaderWriter::Options> options = new osgDB::ReaderWriter::Options();
	std::stringstream err;
	osgDB::ReaderWriter::ReadResult res = osgDB::ReaderWriter::ReadResult::FILE_NOT_HANDLED;
	try
	{
		res = reader_writer->readNode( m_file_path, options.get() );
	}
	catch( IfcPPException& e)
	{
		err << e.what();
	}
	catch( std::exception& e )
	{
		err << e.what();
	}
	catch( ... )
	{
		err << "ReaderWriterIFC::readNode failed" << std::endl;
	}
	err << reader_writer->getErrors().str();

	try
	{
		osg::Object* obj = res.getObject();
		osg::Group* group = dynamic_cast<osg::Group*>(obj);
		if( group )
		{
			bool optimize = true;

			if( optimize )
			{
				osgUtil::Optimizer opt;
				opt.optimize(group);
				//opt.optimize( group, osgUtil::Optimizer::INDEX_MESH );
			}

			// if model bounding sphere is far from origin, move to origin
			const osg::BoundingSphere& bsphere = group->getBound();
			if( bsphere.center().length() > 10000 )
			{
				if( bsphere.center().length()/bsphere.radius() > 100 )
				{
					GeomUtils::applyTranslate( group, -bsphere.center() );
				}
			}

			model_group->addChild( group );


			// TODO: handle spaces, terrain, storeys separately. add buttons in gui to show/hide spaces or terrain and to shift storeys
		}
		else
		{
			if( res.status() == osgDB::ReaderWriter::ReadResult::FILE_NOT_HANDLED )
			{
				err << "FILE_NOT_HANDLED" << std::endl;
			}
			else if( res.status() == osgDB::ReaderWriter::ReadResult::FILE_NOT_FOUND )
			{
				err << "FILE_NOT_FOUND" << std::endl;
			}
			else if( res.status() == osgDB::ReaderWriter::ReadResult::ERROR_IN_READING_FILE )
			{
				err << "ERROR_IN_READING_FILE" << std::endl;
			}
			else
			{
				err << "ReaderWriterIFC returned no valid osg::Group" << std::endl;
			}
		}
	}
	catch( IfcPPException& e)
	{
		err << e.what();
		err << reader_writer->getErrors().str();
	}
	catch(std::exception& e)
	{
		err << e.what();
		err << reader_writer->getErrors().str();
	}

	shared_ptr<IfcPPModel> ifc_model = reader_writer->getIfcPPModel();
	m_system->setIfcModel( ifc_model );
	reader_writer->deleteInputCache();

	if( err.tellp() > 0 )
	{
		throw IfcPPException( err.str().c_str(), __func__ );
	}

	return true;
}

bool CmdLoadIfcFile::undo()
{
	return true;
}

bool CmdLoadIfcFile::redo()
{
	return true;
}

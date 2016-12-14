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

#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/reader/IfcPPReaderSTEP.h>
#include <ifcpp/geometry/GeometryConverter.h>
#include <ifcpp/geometry/GeomUtils.h>

#include "Command.h"
#include "IfcPlusPlusSystem.h"
#include "LoadIfcFileCommand.h"

LoadIfcFileCommand::LoadIfcFileCommand( IfcPlusPlusSystem* system ): Command(system)
{
	
}

LoadIfcFileCommand::~LoadIfcFileCommand(){}

void LoadIfcFileCommand::setFilePath( std::wstring& path_in )
{
	m_file_path = path_in;
}

bool LoadIfcFileCommand::doCmd()
{
	if( m_file_path.length() == 0 )
	{
		return false;
	}

	// first remove previously loaded geometry from scenegraph
	
	osg::ref_ptr<osg::Switch> model_switch = m_system->getModelNode();
	GeomUtils::removeChildren( model_switch );
	m_system->clearSelection();

	// reset the IFC model
	shared_ptr<GeometryConverter> geometry_converter = m_system->getGeometryConverter();
	geometry_converter->resetModel();
	carve::setEpsilon( carve::EPSILON*1.5 );
	std::stringstream err;

	// load file to model and create geometry
	try
	{
		m_system->getIfcPPReader()->loadModelFromFile( m_file_path, geometry_converter->getIfcPPModel() );
		geometry_converter->createGeometryOSG( model_switch );
	}
	catch( IfcPPOutOfMemoryException& e)
	{
		throw e;
	}
	catch( IfcPPException& e )
	{
		err << e.what();
	}
	catch( std::exception& e )
	{
		err << e.what();
	}
	catch( ... )
	{
		err << "loadModelFromFile, createGeometryOSG failed" << std::endl;
	}
	
	try
	{
		if( model_switch )
		{
			bool optimize = true;

			if( optimize )
			{
				osgUtil::Optimizer opt;
				opt.optimize(model_switch);
			}

			// if model bounding sphere is far from origin, move to origin
			const osg::BoundingSphere& bsphere = model_switch->getBound();
			if( bsphere.center().length() > 10000 )
			{
				if( bsphere.center().length()/bsphere.radius() > 100 )
				{
					std::unordered_set<osg::Geode*> set_applied;
					GeomUtils::applyTranslate( model_switch, -bsphere.center(), set_applied );
				}
			}
		}
	}
	catch(std::exception& e)
	{
		err << e.what();
	}

	geometry_converter->clearInputCache();
	
	if( err.tellp() > 0 )
	{
		throw IfcPPException( err.str().c_str() );
	}

	return true;
}

bool LoadIfcFileCommand::undo()
{
	return true;
}

bool LoadIfcFileCommand::redo()
{
	return true;
}

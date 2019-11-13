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

#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

#include <ifcpp/geometry/Carve/IncludeCarveHeaders.h>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingObject.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/BuildingGuid.h>
#include <ifcpp/reader/ReaderSTEP.h>

#include "IncludeGeometryHeaders.h"
#include "Command.h"
#include "IfcPlusPlusSystem.h"
#include "LoadIfcFileCommand.h"

LoadIfcFileCommand::LoadIfcFileCommand( IfcPlusPlusSystem* system ) : Command( system )
{
}

LoadIfcFileCommand::~LoadIfcFileCommand() {}

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
	SceneGraphUtils::clearAllChildNodes( model_switch );
	m_system->clearSelection();

	// reset the IFC model
	shared_ptr<GeometryConverter> geometry_converter = m_system->getGeometryConverter();
	geometry_converter->clearMessagesCallback();
	geometry_converter->resetModel();
	std::stringstream err;

	try
	{
		// load file to IFC model
		m_system->getModelReader()->loadModelFromFile( m_file_path, geometry_converter->getBuildingModel() );

		// convert IFC geometric representations into Carve geometry
		const double length_in_meter = geometry_converter->getBuildingModel()->getUnitConverter()->getLengthInMeterFactor();
		geometry_converter->setCsgEps(carve::CARVE_EPSILON*length_in_meter);
		geometry_converter->convertGeometry();

		// convert Carve geometry to OSG
		shared_ptr<ConverterOSG> converter_osg( new ConverterOSG( geometry_converter->getGeomSettings() ) );
		converter_osg->setMessageTarget( geometry_converter.get() );
		converter_osg->convertToOSG( geometry_converter->getShapeInputData(), model_switch );

		// in case there are IFC entities that are not in the spatial structure
		const std::map<int, shared_ptr<BuildingObject> >& objects_outside_spatial_structure = geometry_converter->getObjectsOutsideSpatialStructure();
		if( objects_outside_spatial_structure.size() > 0 )
		{
			osg::ref_ptr<osg::Switch> sw_objects_outside_spatial_structure = new osg::Switch();
			sw_objects_outside_spatial_structure->setName( "IfcProduct objects outside spatial structure" );

			converter_osg->addNodes( objects_outside_spatial_structure, sw_objects_outside_spatial_structure );
			if( sw_objects_outside_spatial_structure->getNumChildren() > 0 )
			{
				model_switch->addChild( sw_objects_outside_spatial_structure );
			}
		}
	}
	catch( OutOfMemoryException& e)
	{
		throw e;
	}
	catch( BuildingException& e )
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
					SceneGraphUtils::translateGroup( model_switch, -bsphere.center(), set_applied );
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
		throw BuildingException( err.str().c_str() );
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

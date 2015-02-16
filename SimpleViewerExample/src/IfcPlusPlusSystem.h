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

#pragma once

#include <QtCore/QObject>
#include <osgGA/GUIEventHandler>
#include <boost/unordered_map.hpp>
#include "ifcpp/model/shared_ptr.h"

class IfcPPModel;
class IfcPPEntity;
class IfcPPReaderSTEP;
class IfcPPWriterSTEP;
class GeometryConverter;
class CommandManager;
class ViewController;

struct selectedEntity 
{
	shared_ptr<IfcPPEntity> entity;
	osg::ref_ptr<osg::Group> osg_group;
};

class IfcPlusPlusSystem : public QObject, public osgGA::GUIEventHandler
{
	Q_OBJECT

public:
	IfcPlusPlusSystem();
	~IfcPlusPlusSystem();

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	bool intersectModel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, bool click, bool doubleclick );
	
	shared_ptr<GeometryConverter>	getGeometryConverter()	{ return m_geometry_converter; }
	shared_ptr<IfcPPReaderSTEP>&	getIfcPPReader()		{ return m_step_reader; }
	shared_ptr<IfcPPWriterSTEP>&	getIfcPPWriter()		{ return m_step_writer; }
	shared_ptr<CommandManager>		getCommandManager()		{ return m_command_manager; }
	shared_ptr<ViewController>		getViewController()		{ return m_view_controller; }

	void setObjectSelected( shared_ptr<IfcPPEntity> object, bool selected, osg::Group* node = 0 );
	const std::map<int, shared_ptr<selectedEntity> >& getSelectedObjects() { return m_map_selected; }
	void clearSelection();
	void notifyModelCleared();
	void notifyModelLoadingStart();
	void notifyModelLoadingDone();

private:
	shared_ptr<GeometryConverter>		m_geometry_converter;
	shared_ptr<IfcPPReaderSTEP>			m_step_reader;
	shared_ptr<IfcPPWriterSTEP>			m_step_writer;
	shared_ptr<CommandManager>			m_command_manager;
	shared_ptr<ViewController>			m_view_controller;
	std::map<int, shared_ptr<selectedEntity> > m_map_selected;

signals:
	void signalObjectsSelected( boost::unordered_map<int, shared_ptr<IfcPPEntity> >& map_objects );
	void signalObjectsUnselected( boost::unordered_map<int, shared_ptr<IfcPPEntity> >& map_objects );
	void signalModelCleared();
	void signalModelLoadingStart();
	void signalModelLoadingDone();
};

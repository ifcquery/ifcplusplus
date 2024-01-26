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

#pragma once

#include <QObject>
#include <osg/Material>
#include <osgGA/GUIEventHandler>
#include "ifcpp/model/BasicTypes.h"

class BuildingModel;
class BuildingEntity;
class GeometryConverter;
class GeometrySettings;
class CommandManager;
class ViewController;
struct SelectedEntity;

class IfcPlusPlusSystem : public QObject, public osgGA::GUIEventHandler
{
	Q_OBJECT

public:
	IfcPlusPlusSystem();
	~IfcPlusPlusSystem();

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	shared_ptr<BuildingModel>&		getIfcModel()			{ return m_ifc_model; }
	void setIfcModel( shared_ptr<BuildingModel>& model );
	shared_ptr<GeometrySettings>&	getGeometrySettings() { return m_geom_settings; }
	shared_ptr<GeometryConverter>&	getGeometryConverter() { return m_geometry_converter; }
	shared_ptr<CommandManager>&		getCommandManager()		{ return m_command_manager; }
	shared_ptr<ViewController>& getViewController() { return m_view_controller; }
	void setCtrlKeyDown(bool ctrl_key_down);
	bool isCtrlKeyDown() { return m_control_key_down; }
	
	void setObjectSelected( shared_ptr<BuildingEntity> object, bool selected, osg::Group* node = 0 );
	const std::unordered_map<std::string, shared_ptr<SelectedEntity> >& getSelectedObjects() { return m_map_selected; }
	void clearSelection();
	void notifyModelCleared();
	void notifyModelLoadingStart();
	void notifyModelLoadingDone();
	void notifyCursorCoordinates(double, double, double);

	shared_ptr<GeometrySettings>				m_geom_settings;
	shared_ptr<GeometryConverter>				m_geometry_converter;
	shared_ptr<CommandManager>					m_command_manager;
	shared_ptr<ViewController>					m_view_controller;
	std::unordered_map<std::string, shared_ptr<SelectedEntity> >	m_map_selected;
	shared_ptr<BuildingModel>					m_ifc_model;
	bool										m_control_key_down = false;

signals:
	void signalObjectsSelected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map_objects );
	void signalObjectsUnselected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map_objects );
	void signalModelCleared();
	void signalModelLoadingStart();
	void signalModelLoadingDone();
	void signalCursorCoordinates(double, double, double);
};

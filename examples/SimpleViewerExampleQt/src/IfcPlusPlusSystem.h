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
class CommandManager;
class ViewerWidget;

struct SelectedEntity 
{
	shared_ptr<BuildingEntity>	m_entity;
	osg::ref_ptr<osg::Group>	m_osg_group;
	osg::ref_ptr<osg::Material> m_material_previous;
	osg::ref_ptr<osg::Material> m_material_selected;
};
std::string getGUID(const shared_ptr<BuildingEntity>& ent);

class IfcPlusPlusSystem : public QObject, public osgGA::GUIEventHandler
{
	Q_OBJECT

public:
	IfcPlusPlusSystem();
	~IfcPlusPlusSystem();

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	shared_ptr<GeometryConverter>	getGeometryConverter()	{ return m_geometry_converter; }
	shared_ptr<BuildingModel>&		getIfcModel()			{ return m_ifc_model; }
	void setIfcModel( shared_ptr<BuildingModel>& model );
	shared_ptr<CommandManager>		getCommandManager()		{ return m_command_manager; }
	osg::Group*						getRootNode() { return m_rootnode; }
	osg::Switch*					getModelNode() { return m_sw_model; }
	osg::Switch*					getCoordinateAxesNode() { return m_sw_coord_axes; }
	void setRootNode( osg::Group* root );
	void setViewerWidget( ViewerWidget* widget );
	void toggleSceneLight();
	void switchCurveRepresentation( osg::Group* grp, bool on_off );
	
	void setObjectSelected( shared_ptr<BuildingEntity> object, bool selected, osg::Group* node = 0 );
	void zoomToObject( shared_ptr<BuildingEntity> object, osg::Group* node = 0 );
	const std::unordered_map<std::string, shared_ptr<SelectedEntity> >& getSelectedObjects() { return m_map_selected; }
	void clearSelection();
	void notifyModelCleared();
	void notifyModelLoadingStart();
	void notifyModelLoadingDone();

	ViewerWidget*								m_viewer_widget;
	shared_ptr<GeometryConverter>				m_geometry_converter;
	shared_ptr<CommandManager>					m_command_manager;
	std::unordered_map<std::string, shared_ptr<SelectedEntity> >	m_map_selected;
	shared_ptr<BuildingModel>					m_ifc_model;
	osg::ref_ptr<osg::Group>					m_rootnode;
	osg::ref_ptr<osg::Switch>					m_sw_coord_axes;
	osg::ref_ptr<osg::Switch>					m_sw_model;
	osg::ref_ptr<osg::MatrixTransform>			m_transform_light;
	osg::ref_ptr<osg::Material>					m_material_selected;
	bool										m_light_on;
	bool										m_show_curve_representation;

signals:
	void signalObjectsSelected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map_objects );
	void signalObjectsUnselected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map_objects );
	void signalModelCleared();
	void signalModelLoadingStart();
	void signalModelLoadingDone();
};

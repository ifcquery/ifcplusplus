/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2024 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <regex>
#include <osg/Group>
#include <osg/Material>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIActionAdapter>

#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/BuildingGuid.h>
#include <ifcpp/reader/ReaderUtil.h>
#include <IfcProduct.h>
#include <IfcSite.h>
#include <IfcLengthMeasure.h>
#include <IfcOpeningElement.h>
#include <IfcOwnerHistory.h>
#include <IfcGloballyUniqueId.h>

#include "cmd/CmdRemoveSelectedObjects.h"
#include "cmd/CommandManager.h"
#include "IncludeGeometryHeaders.h"
#include "IfcPlusPlusSystem.h"
#include "viewer/IntersectionHandler.h"
#include "viewer/ViewerWidget.h"
#include "viewer/ViewController.h"
#include "viewer/Orbit3DManipulator.h"
#include "viewer/ViewerUtil.h"

IfcPlusPlusSystem::IfcPlusPlusSystem()
{
	m_geom_settings = make_shared<GeometrySettings>();
	m_command_manager = shared_ptr<CommandManager>( new CommandManager() );
	IntersectionHandler* ih = new IntersectionHandler(this);
	Orbit3DManipulator* camera_manip = new Orbit3DManipulator(this, ih);
	m_view_controller = shared_ptr<ViewController>(new ViewController(camera_manip));
	m_view_controller->getRootNode()->addChild(ih->m_group_selected);
}

IfcPlusPlusSystem::~IfcPlusPlusSystem(){}

void IfcPlusPlusSystem::setIfcModel( shared_ptr<BuildingModel>& model )
{
	m_ifc_model = model;
}

bool IfcPlusPlusSystem::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& /*aa*/)
{
	bool handled=false;
	try
	{
		switch( ea.getEventType() )
		{
			case osgGA::GUIEventAdapter::FRAME : 
			{
				break;
			}
			case osgGA::GUIEventAdapter::MOVE :
			{
				//handled = handleMouseMoved( ea, aa );
				break;
			}
			case  osgGA::GUIEventAdapter::KEYDOWN :
			{
				const int key = ea.getKey();
				if( key == osgGA::GUIEventAdapter::KEY_Escape )
				{
				}
				else if( key == osgGA::GUIEventAdapter::KEY_Return )
				{
				}
				else if( key == osgGA::GUIEventAdapter::KEY_Delete )
				{
					shared_ptr<CmdRemoveSelectedObjects> cmd_remove( new CmdRemoveSelectedObjects( this ) );
					m_command_manager->executeCommand( cmd_remove );
				}
				break;
			}
			default:
				break;
		}
	}
	catch( BuildingException& e )
	{
#ifdef _DEBUG
		std::cout << e.what();
#endif
	}
#ifndef _DEBUG
	catch( std::exception& e )
	{
		std::cout << e.what();
	}
#endif
	return handled;
}

void IfcPlusPlusSystem::setCtrlKeyDown(bool ctrl_key_down)
{
	m_control_key_down = ctrl_key_down;
}

#ifdef _DEBUG
#define _DEBUG_GEOMETRY
#endif

osg::Group* findNodeByIfcId(osg::Group* group, std::string ifc_guid)
{
	int num_children = group->getNumChildren();
	for (int i = 0; i < num_children; ++i)
	{
		osg::Node* child_node = group->getChild(i);
		osg::Group* child = dynamic_cast<osg::Group*>(child_node);
		if (!child)
		{
			continue;
		}

		std::string child_name = child->getName();

		if (child_name.length() >= 22)
		{
			child_name = child_name.substr(0, 22);
			std::string guid;
			std::regex re("[a-zA-Z0-9_$]{22}");
			std::smatch match;
			if (std::regex_search(child_name, match, re))
			{
				guid = match.str(0);

				if (guid.compare(ifc_guid) == 0)
				{
					return child;
				}
			}
		}
		osg::Group* child_of_child = findNodeByIfcId(child, ifc_guid);
		if (child_of_child != 0)
		{
			return child_of_child;
		}
	}
	return 0;
}

void IfcPlusPlusSystem::setObjectSelected( shared_ptr<BuildingEntity> ifc_object, bool selected, osg::Group* grp )
{
	const std::string guid = getGUID(ifc_object);
	if( !grp )
	{
		grp = findNodeByIfcId( m_view_controller->getModelNode(), guid);
	}

	if( selected )
	{
		if( grp )
		{
			// insert into set of selected objects
			shared_ptr<SelectedEntity> selected_entity( new SelectedEntity() );
			selected_entity->m_entity = ifc_object;
			selected_entity->m_osg_group = grp;
			m_map_selected[guid] = selected_entity;

			bool site_selected = false;
			if( grp->getName().find( "IfcSite" ) != std::string::npos )
			{
				site_selected = true;
			}
			bool select_child = true;
			if( site_selected )
			{
				if( grp->getName().find( "IfcBuilding" ) != std::string::npos )
				{
					// do not select building with site
					select_child = false;
				}
			}

			if( select_child )
			{
				selected_entity->setSelected(m_view_controller->getMaterialSelected());
			}

			std::unordered_map<std::string, shared_ptr<BuildingEntity> > map_objects;
			map_objects[guid] = ifc_object;
			emit( signalObjectsSelected( map_objects ) );
		}
	}
	else
	{
		// deselect
		if( grp )
		{
 			auto it_selected = m_map_selected.find( guid );
			if( it_selected != m_map_selected.end() )
			{
				shared_ptr<SelectedEntity> selected_entity = it_selected->second;
				selected_entity->setUnselected();
				m_map_selected.erase( it_selected );
			}
		}
		std::unordered_map<std::string, shared_ptr<BuildingEntity> > map_objects;
		map_objects[guid] = ifc_object;
		emit( signalObjectsUnselected( map_objects ) );
	}
}

void IfcPlusPlusSystem::clearSelection()
{
	for( auto it = m_map_selected.begin(); it != m_map_selected.end(); ++it )
	{
		shared_ptr<SelectedEntity>& selected_entity = (*it).second;
		shared_ptr<BuildingEntity> entity = selected_entity->m_entity;
		selected_entity->setUnselected();
	}
	m_map_selected.clear();
}

void IfcPlusPlusSystem::notifyModelCleared()
{
	emit( signalModelCleared() );
}

void IfcPlusPlusSystem::notifyModelLoadingStart()
{
	emit( signalModelLoadingStart() );
}

void IfcPlusPlusSystem::notifyModelLoadingDone()
{
	emit( signalModelLoadingDone() );
}

void IfcPlusPlusSystem::notifyCursorCoordinates(double x, double y, double z)
{
	emit(signalCursorCoordinates(x, y, z));
}

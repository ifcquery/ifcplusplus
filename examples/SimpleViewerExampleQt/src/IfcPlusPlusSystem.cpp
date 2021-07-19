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

#include <regex>
#include <osg/Group>
#include <osg/Material>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIActionAdapter>

#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/BuildingGuid.h>
#include <ifcpp/reader/ReaderUtil.h>
#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcSite.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcOpeningElement.h>
#include <ifcpp/IFC4/include/IfcOwnerHistory.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>

#include "cmd/CmdRemoveSelectedObjects.h"
#include "cmd/CommandManager.h"
#include "IncludeGeometryHeaders.h"
#include "IfcPlusPlusSystem.h"
#include "viewer/ViewerWidget.h"
#include "viewer/OrbitCameraManipulator.h"

std::string getGUID(const shared_ptr<BuildingEntity>& ent)
{
	std::string guid;
	shared_ptr<IfcRoot> ifc_root = dynamic_pointer_cast<IfcRoot>(ent);
	if (ifc_root)
	{
		if (ifc_root->m_GlobalId)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> StringConverter;
			guid = StringConverter.to_bytes(ifc_root->m_GlobalId->m_value);
		}
	}
	return guid;
}

IfcPlusPlusSystem::IfcPlusPlusSystem()
	: m_viewer_widget(nullptr)
{
	m_command_manager = shared_ptr<CommandManager>( new CommandManager() );
	m_ifc_model = shared_ptr<BuildingModel>( new BuildingModel() );
	m_geometry_converter = shared_ptr<GeometryConverter>( new GeometryConverter( m_ifc_model ) );
	
	m_rootnode = new osg::Group();
	m_rootnode->setName( "m_rootnode" );
	
	m_sw_model = new osg::Switch();
	m_sw_model->setName( "m_sw_model" );
	m_rootnode->addChild( m_sw_model.get() );

	m_sw_coord_axes = new osg::Switch();
	m_sw_coord_axes->setName( "m_sw_coord_axes" );
	m_rootnode->addChild( m_sw_coord_axes.get() );

	m_show_curve_representation = true;
	m_light_on = false;

	m_material_selected = new osg::Material();
	m_material_selected->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.2f, 0.98f, 0.2f, 0.5f ) );
	m_material_selected->setSpecular( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.2f, 0.85f, 0.2f, 1.0f ) );
	m_material_selected->setShininess( osg::Material::FRONT_AND_BACK, 35.0 );
	m_material_selected->setColorMode( osg::Material::SPECULAR );
}

IfcPlusPlusSystem::~IfcPlusPlusSystem(){}

void IfcPlusPlusSystem::setIfcModel( shared_ptr<BuildingModel>& model )
{
	m_ifc_model = model;
}

void IfcPlusPlusSystem::setRootNode( osg::Group* root )
{
	m_rootnode = root;
}

void IfcPlusPlusSystem::setViewerWidget( ViewerWidget* viewer_widget )
{
	m_viewer_widget = viewer_widget;
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
	catch( OutOfMemoryException& e)
	{
		throw e;
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
		grp = findNodeByIfcId( m_sw_model, guid );
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
				osg::ref_ptr<osg::StateSet> stateset = grp->getOrCreateStateSet();
				osg::Material* material_previous = (osg::Material*)stateset->getAttribute( osg::StateAttribute::MATERIAL );
				if( material_previous )
				{
					selected_entity->m_material_previous = material_previous;
				}

				//stateset->setAttribute( m_material_selected, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

				osg::ref_ptr<osg::StateSet> statesetSelected = new osg::StateSet();
				statesetSelected->setAttribute(m_material_selected, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
				grp->setStateSet(statesetSelected);
				selected_entity->m_material_selected = m_material_selected;
			}

			std::map<std::string, shared_ptr<BuildingEntity> > map_objects;
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

				if( selected_entity->m_osg_group )
				{
					osg::ref_ptr<osg::StateSet> stateset_selected_node = selected_entity->m_osg_group->getOrCreateStateSet();
					if( selected_entity->m_material_previous )
					{
						stateset_selected_node->setAttribute( selected_entity->m_material_previous, osg::StateAttribute::ON );
					}
					else if( selected_entity->m_material_selected )
					{
						stateset_selected_node->removeAttribute( selected_entity->m_material_selected );
					}
				}

				m_map_selected.erase( it_selected );
			}
		}
		std::map<std::string, shared_ptr<BuildingEntity> > map_objects;
		map_objects[guid] = ifc_object;
		emit( signalObjectsUnselected( map_objects ) );
	}
}

void IfcPlusPlusSystem::zoomToObject( shared_ptr<BuildingEntity> ifc_object, osg::Group* grp )
{
	if( !m_viewer_widget )
	{
		return;
	}

	const std::string guid = getGUID(ifc_object);
	if( !grp )
	{
		grp = findNodeByIfcId( m_sw_model, guid );
		if( !grp )
			return;
	}

	osgViewer::View* main_view = m_viewer_widget->getMainView();
	if( !main_view )
	{
		return;
	}

	OrbitCameraManipulator* orbit_manip = dynamic_cast<OrbitCameraManipulator*>(main_view->getCameraManipulator());
	if( !orbit_manip )
	{
		return;
	}

	orbit_manip->zoomToBoundingSphere(grp->getBound());
}

void IfcPlusPlusSystem::clearSelection()
{
	for( auto it = m_map_selected.begin(); it != m_map_selected.end(); ++it )
	{
		shared_ptr<SelectedEntity>& selected_entity = (*it).second;
		shared_ptr<BuildingEntity> entity = selected_entity->m_entity;

		if( selected_entity->m_osg_group )
		{
			osg::ref_ptr<osg::StateSet> stateset_selected_node = selected_entity->m_osg_group->getOrCreateStateSet();
			if( selected_entity->m_material_previous )
			{
				stateset_selected_node->setAttribute( selected_entity->m_material_previous, osg::StateAttribute::ON );
			}
			else if( selected_entity->m_material_selected )
			{
				stateset_selected_node->removeAttribute( selected_entity->m_material_selected );
			}
		}
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

void IfcPlusPlusSystem::toggleSceneLight()
{
	osg::StateSet* stateset_root = m_rootnode->getOrCreateStateSet();

	if( !m_transform_light.valid() )
	{
		osg::ref_ptr<osg::Group> light_group = new osg::Group();
		light_group->setName( "light_group" );
		double model_size = 100; // TODO: adjust when model is loaded

		osg::ref_ptr<osg::Light> light6 = new osg::Light();
		light6->setLightNum( 6 );
		light6->setPosition( osg::Vec4( 0.0, 0.0, 0.0, 1.0f ) );
		light6->setAmbient( osg::Vec4( 0.5f, 0.53f, 0.57f, 0.4f ) );
		light6->setDiffuse( osg::Vec4( 0.5f, 0.53f, 0.57f, 0.4f ) );
		light6->setConstantAttenuation( 1.0f );
		light6->setLinearAttenuation( 2.0f/model_size );
		light6->setQuadraticAttenuation( 2.0f/(model_size*model_size) );

		osg::ref_ptr<osg::LightSource> light_source6 = new osg::LightSource();
		light_source6->setLight( light6 );
		light_source6->setLocalStateSetModes( osg::StateAttribute::ON );
		light_source6->setStateSetModes( *stateset_root, osg::StateAttribute::ON );
		m_transform_light = new osg::MatrixTransform( osg::Matrix::translate( 5, 5, 50 ) );
		m_transform_light->addChild( light_source6 );

		light_group->addChild( m_transform_light );
		m_rootnode->addChild( light_group );

		m_light_on = false;
	}

	m_light_on = !m_light_on;
	if( m_light_on )
	{
		stateset_root->setMode( GL_LIGHT6, osg::StateAttribute::ON );
	}
	else
	{
		stateset_root->setMode( GL_LIGHT6, osg::StateAttribute::OFF );
	}
}

void IfcPlusPlusSystem::switchCurveRepresentation( osg::Group* grp, bool on_off )
{
	m_show_curve_representation = on_off;
	osg::Switch* grp_switch = dynamic_cast<osg::Switch*>( grp );
	if( grp_switch )
	{
		if( grp_switch->getName().compare( "CurveRepresentation" ) == 0 )
		{
			if( on_off )
			{
				grp_switch->setAllChildrenOn();
			}
			else
			{
				grp_switch->setAllChildrenOff();
			}
		}
	}

	unsigned int num_children = grp->getNumChildren();
	for( unsigned int i=0; i<num_children; ++i)
	{
		osg::Node* child_node = grp->getChild(i);
		osg::Group* child_grp = dynamic_cast<osg::Group*>( child_node );
		if( child_grp )
		{
			switchCurveRepresentation( child_grp, on_off );
		}
	}
}

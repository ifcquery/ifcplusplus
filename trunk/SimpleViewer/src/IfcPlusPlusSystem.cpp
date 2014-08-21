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

#include <osg/Group>
#include <osg/Material>
#include <osgFX/Scribe>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIActionAdapter>
#include <osgDB/Registry>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcppgeometry/ReaderWriterIFC.h>
#include <ifcppgeometry/GeomUtils.h>

#include "cmd/CmdRemoveSelectedObjects.h"
#include "cmd/CommandManager.h"

#include "ViewController.h"
#include "IfcPlusPlusSystem.h"

IfcPlusPlusSystem::IfcPlusPlusSystem()
{
	m_view_controller = shared_ptr<ViewController>( new ViewController() );
	m_command_manager = shared_ptr<CommandManager>( new CommandManager() );
	m_ifc_model		= shared_ptr<IfcPPModel>( new IfcPPModel() );

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
	m_reader_writer->setModel( m_ifc_model );
}

IfcPlusPlusSystem::~IfcPlusPlusSystem()
{
}

void IfcPlusPlusSystem::setIfcModel( shared_ptr<IfcPPModel>& model )
{
	m_ifc_model = model;
}

bool IfcPlusPlusSystem::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
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
				else if( key == 't' )
				{
					// TODO: fix or disable conflicting window transparency statesets
					m_view_controller->toggleModelTransparency();
				}
				break;
			}
			default:
				break;
		}
	}
	catch( IfcPPException& e )
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
//	m_ga_t0 = &ea;
	return handled;
}

#ifdef _DEBUG
#define _DEBUG_GEOMETRY
#endif


osg::Group* findNodeByIfcId( osg::Group* group, int ifc_id )
{
	int num_children = group->getNumChildren();
	for( int i=0; i<num_children; ++i )
	{
		osg::Node* child_node = group->getChild( i );
		osg::Group* child = dynamic_cast<osg::Group*>(child_node);
		if( !child )
		{
			continue;
		}

		const std::string child_name = child->getName();

		if( child_name.length() > 0 )
		{
			if( child_name.substr(0,1).compare("#") == 0 )
			{
				std::string id_str = child_name.substr(1,child_name.length()-1);
				int id = atoi( id_str.c_str() );

				if( id == ifc_id )
				{
					return child;
				}
			}
		}
		osg::Group* child_of_child = findNodeByIfcId( child, ifc_id );
		if( child_of_child != 0 )
		{
			return child_of_child;
		}
	}
	return 0;
}


void IfcPlusPlusSystem::setObjectSelected( shared_ptr<IfcPPEntity> ifc_object, bool selected, osg::Group* grp )
{
	const int id = ifc_object->m_id;
	if( grp == 0 )
	{
		osg::Group* model_group = m_view_controller->m_sw_model;
		grp = findNodeByIfcId( model_group, id );
	}

	if( selected )
	{
		if( grp )
		{
			// insert into set of selected objects
			shared_ptr<selectedEntity> selected_entity( new selectedEntity() );
			selected_entity->entity = ifc_object;
			selected_entity->osg_group = grp;
			m_map_selected[id] = selected_entity;

			for( int child_ii = 0; child_ii < grp->getNumChildren(); ++child_ii )
			{
				osg::Node* child_node = grp->getChild( child_ii );

				osgFX::Scribe* child_as_scribe = dynamic_cast<osgFX::Scribe*>(child_node);
				if( !child_as_scribe )
				{
					// highlight object with an osgFX::Scribe
					osgFX::Scribe* scribe = new osgFX::Scribe();
					scribe->addChild(child_node);
					scribe->setWireframeColor( osg::Vec4f( 0.98f, 0.98f, 0.22f, 0.9f ) );
					grp->replaceChild(child_node,scribe);
				}
			}
		}

#ifdef _DEBUG
		shared_ptr<IfcProduct> product = dynamic_pointer_cast<IfcProduct>( ifc_object );
		if( product )
		{
			const int product_id = product->m_id;
			shared_ptr<ShapeInputData> product_shape( new ShapeInputData() );
			product_shape->ifc_product = product;

			if( product->m_Representation )
			{
				m_reader_writer->convertIfcProduct( product, product_shape );
			}
			//GeomUtils::dumpMeshset( product_shape );
		}
#endif

		std::map<int, shared_ptr<IfcPPEntity> > map_objects;
		map_objects[id] = ifc_object;
		emit( signalObjectsSelected( map_objects ) );
	}
	else
	{
		// deselect
		if( grp )
		{
 			std::map<int, shared_ptr<selectedEntity> >::iterator it_selected = m_map_selected.find( id );
			if( it_selected != m_map_selected.end() )
			{
				shared_ptr<selectedEntity> selected_entity = it_selected->second;
				for( int child_ii = 0; child_ii < grp->getNumChildren(); ++child_ii )
				{
					osg::Node* child_node = grp->getChild( child_ii );
					osg::Group* child_as_group = dynamic_cast<osg::Group*>(child_node);
					if( child_as_group )
					{
						osgFX::Scribe* child_as_scribe = dynamic_cast<osgFX::Scribe*>(child_as_group);
						if( child_as_scribe )
						{
							if( child_as_scribe->getNumChildren() > 0 )
							{
								osg::Node* original_child = child_as_scribe->getChild(0);
								grp->replaceChild( child_node, original_child );
							}
						}
					}
				}
				m_map_selected.erase( it_selected );
			}
			
		}
		std::map<int, shared_ptr<IfcPPEntity> > map_objects;
		map_objects[id] = ifc_object;
		emit( signalObjectsUnselected( map_objects ) );
	}
}


void IfcPlusPlusSystem::clearSelection()
{
	for( std::map<int, shared_ptr<selectedEntity> >::iterator it = m_map_selected.begin(); it != m_map_selected.end(); ++it )
	{
		shared_ptr<selectedEntity>& selected_entity = (*it).second;
		shared_ptr<IfcPPEntity> entity = selected_entity->entity;
		osg::Group* grp = selected_entity->osg_group;

		for( int child_ii = 0; child_ii < grp->getNumChildren(); ++child_ii )
		{
			osg::Node* child_node = grp->getChild( child_ii );
			osg::Group* child_as_group = dynamic_cast<osg::Group*>(child_node);
			if( child_as_group )
			{
				osgFX::Scribe* child_as_scribe = dynamic_cast<osgFX::Scribe*>(child_as_group);
				if( child_as_scribe )
				{
					if( child_as_scribe->getNumChildren() > 0 )
					{
						osg::Node* original_child = child_as_scribe->getChild(0);
						grp->replaceChild( child_node, original_child );
					}
				}
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

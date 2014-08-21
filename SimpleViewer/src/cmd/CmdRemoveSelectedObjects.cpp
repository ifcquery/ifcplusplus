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

#include <osg/Vec3f>
#include <osg/Group>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/IFC4/include/IfcSite.h>

#include "Command.h"
#include "IfcPlusPlusSystem.h"
#include "CmdRemoveSelectedObjects.h"

CmdRemoveSelectedObjects::CmdRemoveSelectedObjects( IfcPlusPlusSystem* system ): Command(system)
{
}
CmdRemoveSelectedObjects::~CmdRemoveSelectedObjects()
{

}

bool CmdRemoveSelectedObjects::doCmd()
{
	const std::map<int, shared_ptr<selectedEntity> >& selected_objects = m_system->getSelectedObjects();
	std::map<int, shared_ptr<selectedEntity> >::const_iterator it_selected;
	for( it_selected = selected_objects.begin(); it_selected != selected_objects.end(); ++it_selected )
{
		shared_ptr<selectedEntity> selected_entity = it_selected->second;
		shared_ptr<IfcPPEntity> entity = selected_entity->entity;
		osg::Group* grp = selected_entity->osg_group.get();
		
		grp->removeChildren(0, grp->getNumChildren() );
		// TODO: remove also from ifc model

		m_removed_objects[entity->m_id] = entity;
	}
	m_system->clearSelection();

	return true;
}

bool CmdRemoveSelectedObjects::undo()
{
	// TODO: implement
	return true;
}

bool CmdRemoveSelectedObjects::redo()
{
	// TODO: implement
	return true;
}

/* -*-c++-*- IFC++ www.ifcquery.com
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

#include <osg/Vec3f>
#include <osg/Group>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingObject.h>

#include "IncludeGeometryHeaders.h"
#include "Command.h"
#include "IfcPlusPlusSystem.h"
#include "CmdRemoveSelectedObjects.h"

CmdRemoveSelectedObjects::CmdRemoveSelectedObjects( IfcPlusPlusSystem* system ): Command(system)
{
}
CmdRemoveSelectedObjects::~CmdRemoveSelectedObjects(){}

bool CmdRemoveSelectedObjects::doCmd()
{
	const std::unordered_map<std::string, shared_ptr<SelectedEntity> >& selected_objects = m_system->getSelectedObjects();
	for( auto it_selected : selected_objects )
	{
		const shared_ptr<SelectedEntity>& selected_entity = it_selected.second;
		shared_ptr<BuildingEntity> entity = selected_entity->m_entity;
		osg::Group* grp = selected_entity->m_osg_group.get();
	
		SceneGraphUtils::removeChildren( grp );

		// TODO: remove also from ifc model
		m_removed_objects[entity->m_tag] = entity;
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

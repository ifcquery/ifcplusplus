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

#include <ifcpp/model/shared_ptr.h>
#include <osg/Vec3f>
#include <osg/Group>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include "Command.h"

class IfcPlusPlusSystem;
class IfcPPEntity;

class CmdRemoveSelectedObjects : public Command
{

protected:
	std::map<int, shared_ptr<IfcPPEntity> > m_removed_objects;


public:
	CmdRemoveSelectedObjects( IfcPlusPlusSystem* system );
	virtual ~CmdRemoveSelectedObjects();
	virtual shared_ptr<Command> copy() { return shared_ptr<CmdRemoveSelectedObjects>( new CmdRemoveSelectedObjects(m_system) ); }
	virtual const char* classname() { return "CmdRemoveSelectedObjects"; }

	virtual bool doCmd();
	virtual bool undo();
	virtual bool redo();

	virtual bool storeInUndoList()	{ return true; }
	virtual bool isUndoable()		{ return true; }
	virtual bool isRepeatable()		{ return false; }

};

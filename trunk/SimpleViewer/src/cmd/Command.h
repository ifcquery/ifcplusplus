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

class IfcPlusPlusSystem;
class IfcPPEntity;

class Command
{

protected:
	bool						m_unDone;
	IfcPlusPlusSystem*				m_system;
	osg::ref_ptr<osg::Group>	m_group;
	shared_ptr<Command>			m_interrupted_cmd;

public:
	Command( IfcPlusPlusSystem* system ): m_system(system)
	{
		m_group			= new osg::Group();
		m_unDone		= false;
	}
	virtual ~Command(){}
	virtual shared_ptr<Command> copy() = 0;
	virtual const char* classname() { return "Command"; }

	virtual bool mouseMoved( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool mouseClicked( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool mouseDragged( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool keyPressed( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual void itemSnapped( shared_ptr<IfcPPEntity> item, bool snapped );
	virtual void itemSelected( shared_ptr<IfcPPEntity> item, bool selected );

	virtual bool doCmd()	{ return true; }
	virtual bool undo()		{ return true; }
	virtual bool redo()		{ return true; }
	virtual void cancelCmd(){}

	virtual bool storeInUndoList()						{ return true; }
	virtual bool isUndoable()							{ return false; }
	virtual bool isRepeatable()							{ return false; }
	osg::Group*	getOsgGroup()							{ return m_group.get(); }
	void setInterruptedCmd( shared_ptr<Command> c )		{	m_interrupted_cmd = c; }
	shared_ptr<Command> getInterruptedCmd()				{	return m_interrupted_cmd; }

};

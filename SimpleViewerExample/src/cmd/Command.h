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

#include <ifcpp/model/BasicTypes.h>
#include <osg/Vec3f>
#include <osg/Group>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

class IfcPlusPlusSystem;
class BuildingEntity;

class Command
{
protected:
	IfcPlusPlusSystem*			m_system;
	osg::ref_ptr<osg::Group>	m_group;
	shared_ptr<Command>			m_interrupted_cmd;

public:
	Command( IfcPlusPlusSystem* system ): m_system(system)
	{
		m_group		= new osg::Group();
	}
	virtual ~Command(){}
	virtual shared_ptr<Command> copy() = 0;
	virtual const char* className() const { return "Command"; }

	virtual bool mouseMoved( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool mouseClicked( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool mouseDragged( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool keyPressed( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual void itemSnapped( shared_ptr<BuildingEntity> item, bool snapped );
	virtual void itemSelected( shared_ptr<BuildingEntity> item, bool selected );

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

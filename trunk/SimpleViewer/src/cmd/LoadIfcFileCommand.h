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

#include "ifcpp/model/shared_ptr.h"
#include "Command.h"

class IfcPlusPlusSystem;

class LoadIfcFileCommand : public Command
{
public:
	LoadIfcFileCommand( IfcPlusPlusSystem* system );
	virtual ~LoadIfcFileCommand();
	virtual shared_ptr<Command> copy() { return shared_ptr<LoadIfcFileCommand>( new LoadIfcFileCommand(m_system) ); }
	virtual const char* classname() { return "LoadIfcFileCommand"; }

	virtual bool doCmd();
	virtual bool undo();
	virtual bool redo();

	virtual bool storeInUndoList()	{ return true; }
	virtual bool isUndoable()		{ return true; }
	virtual bool isRepeatable()		{ return false; }

	void setFilePath( std::string& path );

protected:
	std::string m_file_path;
	
};

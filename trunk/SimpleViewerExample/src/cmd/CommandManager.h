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
class Command;

class CommandManager
{
public:
	CommandManager();
	~CommandManager();

	void executeCommand( shared_ptr<Command> c );
	shared_ptr<Command> getCurrentCommand();
	bool isCmdActive();
	void undoCmd();
	void redoCmd();
	void cmdCancelled();
	int getNumUndoCmds();
	int getNumRedoCmds();
	void clearUndoHistory();
	void slotCmdFinished();

private:
	shared_ptr<Command>	m_current_command;
	shared_ptr<Command>	m_previous_command;

	/** undo vector and current position in undo vector */
	std::vector<shared_ptr<Command> >	m_undo_vec;
	int									m_undo_pos;
};

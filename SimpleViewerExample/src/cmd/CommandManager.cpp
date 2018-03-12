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

#include <ifcpp/model/BasicTypes.h>
#include "Command.h"
#include "CommandManager.h"

CommandManager::CommandManager()
{
	m_undo_pos = -1;
}
CommandManager::~CommandManager(){}

void CommandManager::executeCommand( shared_ptr<Command> c )
{
	// keep a pointer to the new and to the current command, to prevent them from being deleted
	shared_ptr<Command> tmp_new_cmd = c;
	shared_ptr<Command> tmp_prev_cmd;

	if( m_current_command )
	{
		// remember interrupted command in new command
		c->setInterruptedCmd( m_current_command );
	}

	m_current_command = c;

	// put the commands widget into the gui
	shared_ptr<Command> emit_arg = m_current_command;

	// execute the command
	m_current_command->doCmd();
}

void CommandManager::slotCmdFinished()
{
	if( !m_current_command )
	{
		return;
	}

	if( m_current_command->storeInUndoList() )
	{
		if( m_current_command->isUndoable() )
		{
			int maxindex = m_undo_vec.size()-1;
			if( m_undo_pos != maxindex )
			{
				// todo: clear undo vec from current position upwards
				int num_remove_cmds = maxindex - m_undo_pos;
				for( ; num_remove_cmds > 0; --num_remove_cmds )
				{
					m_undo_vec.pop_back();
				}
			}
			// put current cmd into undo stack
			m_undo_vec.push_back( m_current_command );
			m_undo_pos = m_undo_vec.size()-1;
		}
		else
		{
			clearUndoHistory();
		}

		if( m_current_command->isRepeatable() )
		{
			m_previous_command = m_current_command;
		}
		else
		{
			m_previous_command.reset();
		}
	}
	m_current_command.reset();
}

void CommandManager::cmdCancelled()
{
	if( !m_current_command )
	{
		return;
	}
	m_previous_command = m_current_command;
	m_current_command.reset();
}

void CommandManager::undoCmd()
{
	int maxindex = m_undo_vec.size()-1;

	// check if undo vector has commands
	if( maxindex < 0 )
	{
		return;
	}

	// check upper bound of undo position
	if( m_undo_pos > maxindex )
	{
		return;
	}

	// check lower bound of undo position
	if( m_undo_pos < 0 )
	{
		return;
	}

	if( int( m_undo_vec.size() ) < m_undo_pos+1 )
	{
		return;
	}

	// get unco command from current position
	if( m_undo_vec[m_undo_pos]->undo() )
	{
		// decrement undo position
		--m_undo_pos;
	}

	// todo: return true/false here
}

void CommandManager::redoCmd()
{
	int maxindex = m_undo_vec.size()-1;

	// check if undo vector has commands
	if( maxindex < 0 )
	{
		return;
	}

	// check upper bound of undo position
	if( m_undo_pos+1 > maxindex )
	{
		return;
	}

	// check lower bound of undo position
	if( m_undo_pos < -1 )
	{
		return;
	}

	if( int( m_undo_vec.size() ) < m_undo_pos+2 )
	{
		return;
	}

	// get unco command from current position
	shared_ptr<Command> c = m_undo_vec[m_undo_pos+1];
	if( c->redo() )
	{
		// increment undo position
		++m_undo_pos;
	}
	// todo: return true/false here
}

void CommandManager::clearUndoHistory()
{
	while( m_undo_vec.size() > 0 )
	{
		m_undo_vec.pop_back();
	}
	m_undo_vec.clear();
	m_undo_pos = -1;
}

int CommandManager::getNumUndoCmds()
{
	return m_undo_pos+1;
}
int CommandManager::getNumRedoCmds()
{
	return m_undo_vec.size()-1-m_undo_pos;
}

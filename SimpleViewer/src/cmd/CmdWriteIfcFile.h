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
#include "Command.h"

class IfcPlusPlusSystem;
class IfcPPEntity;
class ReaderWriterIFC;

class CmdWriteIfcFile : public Command
{

protected:
	std::string m_file_path;
	osg::ref_ptr<ReaderWriterIFC>			m_reader_writer;  // osg::Registry uses ref_ptr, so use it here too

public:
	CmdWriteIfcFile( IfcPlusPlusSystem* system );
	virtual ~CmdWriteIfcFile();
	virtual shared_ptr<Command> copy() { return shared_ptr<CmdWriteIfcFile>( new CmdWriteIfcFile(m_system) ); }
	virtual const char* classname() { return "CmdWriteIfcFile"; }

	virtual bool doCmd();
	virtual bool undo();
	virtual bool redo();

	virtual bool storeInUndoList()	{ return true; }
	virtual bool isUndoable()		{ return true; }
	virtual bool isRepeatable()		{ return false; }

	void setFilePath( std::string& path );

};

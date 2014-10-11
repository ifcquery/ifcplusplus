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

#include <sstream>
#include <vector>
#include "IfcPPOpenMP.h"

class IfcPPEntity;

class StatusCallback
{
public:
	enum SeverityLevel { STATUS_SEVERITY_UNKNOWN, STATUS_SEVERITY_MESSAGE, STATUS_SEVERITY_WARNING, STATUS_SEVERITY_ERROR };

	StatusCallback();
	~StatusCallback();	
	virtual const char* className() { return "StatusCallback"; }

	//\brief progress callback mechanism to update progress bar or similar
	virtual void setProgressCallBack( void* obj_ptr, void (*func)(void*, double, const std::string& progress_type) );
	virtual void unsetProgressCallBack();

	//\brief this callback can be connected to set a message in the status bar
	virtual void setProgressTextCallBack( void* obj_ptr, void (*func)(void*, const std::wstring& message) );
	virtual void unsetProgressTextCallBack();

	//\brief error callback mechanism to show messages in gui
	virtual void setMessageCallBack( void* obj_ptr, void (*func)(void*, const std::wstring& message, SeverityLevel level) );
	virtual void unsetMessageCallBack();

protected:
	//\brief trigger the callback to set a progress, for example in a progress bar
	void progressCallback(double progress, const std::string& progress_type);

	//\brief trigger the callback to set a text to the progress, for example in a progress bar
	void progressTextCallback( const std::wstring& message );

	//\brief trigger the callback to pass a message, warning, or error, for example to store in a logfile
	virtual void messageCallback( const std::string& message, SeverityLevel level, const char* reporting_function, IfcPPEntity* entity = nullptr );
	virtual void messageCallback( const std::wstring& message, SeverityLevel level, const char* reporting_function, IfcPPEntity* entity = nullptr );

	//\brief callbacks are set to children as well
	void addCallbackChild( StatusCallback* child );
	void removeCallbackChild( StatusCallback* child );
	void clearCallbackChildren();

	void* m_obj_call_on_progress;
	void (*m_func_call_on_progress)(void*, double, const std::string& progress_type);

	void* m_obj_call_on_progress_text;
	void (*m_func_call_on_progress_text)(void*, const std::wstring& message);

	void* m_obj_call_on_message;
	void (*m_func_call_on_message)(void*, const std::wstring& message, SeverityLevel level);

	std::vector<StatusCallback*> m_callback_children;
#ifdef IFCPP_OPENMP
	Mutex m_writelock;
#endif
};

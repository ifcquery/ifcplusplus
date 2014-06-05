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

class StatusObservable
{
public:
	StatusObservable();
	~StatusObservable();	
	virtual const char* className() { return "StatusObservable"; }

	//! @brief progress callback mechanism to update progress bar or similar
	void setProgressCallBack( void* obj_ptr, void (*func)(void*, double, const std::string& progress_type) );
	void unsetProgressCallBack();

	//! @brief this callback can be connected to set a message in the status bar
	void setProgressTextCallBack( void* obj_ptr, void (*func)(void*, const std::string& message) );
	void unsetProgressTextCallBack();

	//! @brief message callback mechanism to show messages in gui
	void setMessageCallBack( void* obj_ptr, void (*func)(void*, const std::string& message) );
	void unsetMessageCallBack();

	//! @brief error callback mechanism to show errors in gui
	void setErrorCallBack( void* obj_ptr, void (*func)(void*, const std::string& message) );
	void unsetErrorCallBack();

protected:
	// progress callback
	void* m_obj_call_on_progress;
	void (*m_func_call_on_progress)(void*, double, const std::string& progress_type);
	void progressCallback(double progress, const std::string& progress_type);

	void* m_obj_call_on_progress_text;
	void (*m_func_call_on_progress_text)(void*, const std::string& message);
	void progressTextCallback( const std::string& message );

	void* m_obj_call_on_message;
	void (*m_func_call_on_message)(void*, const std::string& message);
	void messageCallback( const std::string& message );

	void* m_obj_call_on_error;
	void (*m_func_call_on_error)(void*, const std::string& err);
	void errorCallback( const std::string& err );
};

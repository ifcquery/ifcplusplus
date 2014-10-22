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
#include <iostream>
#include <vector>
#include "shared_ptr.h"
#include "IfcPPOpenMP.h"

class IfcPPEntity;

class StatusCallback
{
public:
	enum SeverityLevel { STATUS_SEVERITY_UNKNOWN, STATUS_SEVERITY_MESSAGE, STATUS_SEVERITY_WARNING, STATUS_SEVERITY_MINOR_WARNING, STATUS_SEVERITY_ERROR };
	/*\class Ticket
	  \brief Combines all information about a problem message
	*/
	class Ticket
	{
	public:
		StatusCallback::Ticket::Ticket(){}
		StatusCallback::Ticket::Ticket( const std::string& message, StatusCallback::SeverityLevel level, const char* reporting_function, IfcPPEntity* entity = nullptr )
			: m_level(level), m_reporting_function(reporting_function), m_entity(entity)
		{
			m_message.assign( message.begin(), message.end() );
		}
		StatusCallback::Ticket::Ticket( const std::wstring& message, StatusCallback::SeverityLevel level, const char* reporting_function, IfcPPEntity* entity = nullptr )
			: m_message(message), m_level(level), m_reporting_function(reporting_function), m_entity(entity)
		{
		}

		std::wstring m_message;
		SeverityLevel m_level;
		const char* m_reporting_function;
		IfcPPEntity* m_entity;
	};

	StatusCallback()
	{
		unsetProgressCallBack();
		unsetMessageCallBack();
	}
	~StatusCallback()
	{
	}
	virtual const char* className() { return "StatusCallback"; }

	//\brief progress callback mechanism to update progress bar or similar
	virtual void setProgressCallBack( void* obj_ptr, void (*func)(void*, double value, const std::string& progress_type, const std::wstring& progress_text) )
	{
		m_obj_call_on_progress = obj_ptr;
		m_func_call_on_progress = func;

		// pass to children
		for( auto it = m_callback_children.begin(); it != m_callback_children.end(); ++it )
		{
			StatusCallback* child = *it;
			if( child )
			{
				child->setProgressCallBack( obj_ptr, func );
			}
		}
	}

	virtual void unsetProgressCallBack()
	{
		m_obj_call_on_progress = nullptr;
		m_func_call_on_progress = nullptr;

		// pass to children
		for( auto it = m_callback_children.begin(); it != m_callback_children.end(); ++it )
		{
			StatusCallback* child = *it;
			if( child )
			{
				child->unsetProgressCallBack();
			}
		}
	}

	//\brief error callback mechanism to show messages in gui
	virtual void setMessageCallBack( void* obj_ptr, void (*func)(void*, shared_ptr<Ticket> t) )
	{
		m_obj_call_on_message = obj_ptr;
		m_func_call_on_message = func;

		// pass to children
		for( auto it = m_callback_children.begin(); it != m_callback_children.end(); ++it )
		{
			StatusCallback* child = *it;
			if( child )
			{
				child->setMessageCallBack( obj_ptr, func );
			}
		}
	}

	virtual void unsetMessageCallBack()
	{
		m_obj_call_on_message = nullptr;
		m_func_call_on_message = nullptr;

		// pass to children
		for( auto it = m_callback_children.begin(); it != m_callback_children.end(); ++it )
		{
			StatusCallback* child = *it;
			if( child )
			{
				child->unsetMessageCallBack();
			}
		}
	}

protected:
	/*\brief Trigger the callback to set a progress, for example in a progress bar.
	  \param[in] progress Value of progress [0...1]. If negative value is given, the progress itself is ignored, for example when only the progress text is updated.
	  \param[in] progress_type Type of progress, for example "parse", "geometry"
	  \param[in] progress_text Text that describes the current actions. It can be used for example to set a text on the progress bar.
	**/
	void progressCallback(double progress_value, const std::string& progress_type, const std::wstring& progress_text = L"" )
	{
#ifdef _DEBUG
		if( !m_func_call_on_progress )
		{
			std::cout << "progressCallback not set." << std::endl;
		}
		if( !m_obj_call_on_progress )
		{
			std::cout << "progressCallback not set." << std::endl;
		}
#endif

		if( m_func_call_on_progress )
		{
			if( m_obj_call_on_progress )
			{
				m_func_call_on_progress( m_obj_call_on_progress, progress_value, progress_type, progress_text );
			}
		}
	}

	//\brief trigger the callback to pass a message, warning, or error, for example to store in a logfile
	virtual void messageCallback( shared_ptr<Ticket> t )
	{
#ifdef _DEBUG
		if( !m_func_call_on_message )
		{
			std::cout << "messageCallback: !m_func_call_on_message. Lost message: " << t->m_message.c_str() << std::endl;
		}
		if( !m_obj_call_on_message )
		{
			std::cout << "messageCallback: !m_obj_call_on_message. Lost message: " << t->m_message.c_str() << std::endl;
		}
#endif

		if( m_func_call_on_message )
		{
			if( m_obj_call_on_message )
			{
#ifdef IFCPP_OPENMP
				ScopedLock lock( m_writelock );
#endif
				m_func_call_on_message( m_obj_call_on_message, t );
			}
		}
	}

	virtual void messageCallback( const std::string& message, SeverityLevel level, const char* reporting_function, IfcPPEntity* entity = nullptr )
	{
		messageCallback( shared_ptr<Ticket>( new Ticket( message, level, reporting_function, entity ) ) );
	}
	virtual void messageCallback( const std::wstring& message, SeverityLevel level, const char* reporting_function, IfcPPEntity* entity = nullptr )
	{
		messageCallback( shared_ptr<Ticket>( new Ticket( message, level, reporting_function, entity ) ) );
	}

	//\brief callbacks are set to children as well
	void addCallbackChild( StatusCallback* child )
	{
		if( child == this )
		{
#ifdef _DEBUG
			std::cout << "addCallbackChild: trying to add 'this' to children." << std::endl;
#endif
			return;
		}
		m_callback_children.push_back( child );

		if( m_obj_call_on_message )
		{
			if( m_func_call_on_message )
			{
				child->setMessageCallBack( m_obj_call_on_message, m_func_call_on_message );
			}
		}

		if( m_obj_call_on_progress )
		{
			if( m_func_call_on_progress )
			{
				child->setProgressCallBack( m_obj_call_on_progress, m_func_call_on_progress );
			}
		}
	}
	void removeCallbackChild( StatusCallback* remove_child )
	{
		for( auto it = m_callback_children.begin(); it != m_callback_children.end(); )
		{
			StatusCallback* child = *it;
			if( child == remove_child )
			{
				child->unsetMessageCallBack();
				child->unsetProgressCallBack();
				it = m_callback_children.erase( it );
			}
			else
			{
				++it;
			}
		}
	}
	void clearCallbackChildren()
	{
		for( auto it = m_callback_children.begin(); it != m_callback_children.end(); )
		{
			StatusCallback* child = *it;
			if( child )
			{
				child->unsetMessageCallBack();
				child->unsetProgressCallBack();
			}
		}
		m_callback_children.clear();
	}

	//\brief pointer to the object on which the progress callback function is called
	void* m_obj_call_on_progress;

	//\brief pointer to the callback function for progress
	void (*m_func_call_on_progress)(void*, double progress, const std::string& progress_type, const std::wstring& progress_text);

	//\brief pointer to the object on which the message callback function is called
	void* m_obj_call_on_message;

	//\brief pointer to the callback function for messages
	void (*m_func_call_on_message)(void*, shared_ptr<Ticket> t);

	std::vector<StatusCallback*> m_callback_children;
#ifdef IFCPP_OPENMP
	Mutex m_writelock;
#endif
};

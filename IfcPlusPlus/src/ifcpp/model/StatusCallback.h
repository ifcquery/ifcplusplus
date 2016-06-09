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
#include "IfcPPGlobal.h"
#include "IfcPPOpenMP.h"

class IfcPPEntity;

class IFCPP_EXPORT StatusCallback
{
public:
	enum MessageType
	{
		MESSAGE_TYPE_UNKNOWN,
		MESSAGE_TYPE_GENERAL_MESSAGE,
		MESSAGE_TYPE_PROGRESS_VALUE,	//\brief Progress mechanism to update progress bar or similar.
		MESSAGE_TYPE_PROGRESS_TEXT,		//\brief Progress mechanism to update text in progress bar or similar.
		MESSAGE_TYPE_MINOR_WARNING,
		MESSAGE_TYPE_WARNING,
		MESSAGE_TYPE_ERROR
	};
	/*\class Message
	  \brief Combines all information about a status message, being it a general message, a warning, error, or a notification about a progress (for example during reading of a file).
	*/
	class Message
	{
	public:
		//\brief Default constructor.
		Message()
		{
			m_message_type = MessageType::MESSAGE_TYPE_UNKNOWN;
			m_reporting_function = "";
			m_entity = nullptr;
			m_progress_value = -1;
		}

		std::wstring m_message_text;		// Message text.
		MessageType m_message_type;			// Type of message (warning, error etc.).
		const char* m_reporting_function;	// Function name where the message is sent from. You can use the __FUNC__ macro from IfcPPException.h.
		IfcPPEntity* m_entity;				// IFC entity in case the message applies to a certain entity.

		double m_progress_value;			// Value of progress [0...1]. If negative value is given, the progress itself is ignored, for example when only the progress text is updated.
		std::string m_progress_type;		// Type of progress, for example "parse", "geometry".
		std::wstring m_progress_text;		// A text that describes the current actions. It can be used for example to set a text on the progress bar.
	};

	StatusCallback()
	{
		unsetMessageCallBack();
	}
	virtual ~StatusCallback()
	{
	}

	//\brief error callback mechanism to show messages in gui
	virtual void setMessageCallBack( void* obj_ptr, void (*func)(void*, shared_ptr<Message> t) )
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

	//\brief trigger the callback to pass a message, warning, or error, for example to store in a logfile
	virtual void messageCallback( shared_ptr<Message> m )
	{
#ifdef _DEBUG
		if( !m_func_call_on_message )
		{
			if( m )
			{
				switch (m->m_message_type)
				{
					case MESSAGE_TYPE_UNKNOWN:
					case MESSAGE_TYPE_GENERAL_MESSAGE:
					case MESSAGE_TYPE_MINOR_WARNING:
					case MESSAGE_TYPE_WARNING:
					case MESSAGE_TYPE_ERROR:
						std::wcout << L"messageCallback: !m_func_call_on_message. Lost message: " << m->m_message_text.c_str() << std::endl;
						break;
					case MESSAGE_TYPE_PROGRESS_VALUE:
						std::wcout << L"messageCallback: !m_func_call_on_message. Lost progress value: " << m->m_progress_value << std::endl;
						break;
					case MESSAGE_TYPE_PROGRESS_TEXT:
						std::wcout << L"messageCallback: !m_func_call_on_message. Lost progress text: " << m->m_progress_text.c_str() << std::endl;
						break;
				}
				if( !m_obj_call_on_message )
				{
					std::wcout << L"messageCallback: !m_obj_call_on_message. Lost message: " << m->m_message_text.c_str() << std::endl;
				}
			}
		}
#endif

		if( m_func_call_on_message )
		{
			if( m_obj_call_on_message )
			{
#ifdef IFCPP_OPENMP
				// Note: this lock protects accesses only for this instance. If several StatusCallback (or derived) objects are bound to the same callback function, a lock is necessary there.
				ScopedLock lock( m_writelock );
#endif
				m_func_call_on_message( m_obj_call_on_message, m );
			}
		}
	}

	virtual void messageCallback( const std::string& message_text, MessageType type, const char* reporting_function, IfcPPEntity* entity = nullptr )
	{
		shared_ptr<Message> message( new Message() );
		message->m_message_text.assign( message_text.begin(), message_text.end() );
		message->m_message_type = type;
		message->m_reporting_function = reporting_function;
		message->m_entity = entity;
		messageCallback( message );
	}
	virtual void messageCallback( const std::wstring& message_text, MessageType type, const char* reporting_function, IfcPPEntity* entity = nullptr )
	{
		shared_ptr<Message> message( new Message() );
		message->m_message_text.assign( message_text.c_str() );
		message->m_message_type = type;
		message->m_reporting_function = reporting_function;
		message->m_entity = entity;
		messageCallback( message );
	}
	virtual void progressValueCallback( double progress_value, const std::string& progress_type )
	{
		shared_ptr<Message> progress_message( new Message() );
		progress_message->m_message_type = MessageType::MESSAGE_TYPE_PROGRESS_VALUE;
		progress_message->m_progress_value = progress_value;
		progress_message->m_progress_type.assign( progress_type.c_str() );
		messageCallback( progress_message );
	}
	virtual void progressTextCallback( const std::wstring& progress_text )
	{
		shared_ptr<Message> progress_message( new Message() );
		progress_message->m_message_type = MessageType::MESSAGE_TYPE_PROGRESS_TEXT;
		progress_message->m_progress_value = -1;
		progress_message->m_progress_text.assign( progress_text.c_str() );
		messageCallback( progress_message );
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
	}
	void removeCallbackChild( StatusCallback* remove_child )
	{
		for( auto it = m_callback_children.begin(); it != m_callback_children.end(); )
		{
			StatusCallback* child = *it;
			if( child == remove_child )
			{
				child->unsetMessageCallBack();
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
			}
		}
		m_callback_children.clear();
	}

protected:
	//\brief Pointer to the object on which the message callback function is called.
	void* m_obj_call_on_message;

	//\brief Pointer to the callback function for messages.
	void (*m_func_call_on_message)(void*, shared_ptr<Message> t);

	std::vector<StatusCallback*> m_callback_children;
#ifdef IFCPP_OPENMP
	Mutex m_writelock;
#endif
};

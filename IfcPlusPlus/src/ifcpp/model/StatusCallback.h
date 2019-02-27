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

#include <sstream>
#include <iostream>
#include <vector>
#include "BasicTypes.h"
#include "GlobalDefines.h"
#include "OpenMPIncludes.h"

class BuildingEntity;

class IFCQUERY_EXPORT StatusCallback
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
		MESSAGE_TYPE_ERROR,
		MESSAGE_TYPE_CLEAR_MESSAGES,
		MESSAGE_TYPE_CANCELED
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
		const char* m_reporting_function;	// Function name where the message is sent from. You can use the __FUNC__ macro from BuildingException.h.
		BuildingEntity* m_entity;			// IFC entity in case the message applies to a certain entity.

		double m_progress_value;			// Value of progress [0...1]. If negative value is given, the progress itself is ignored, for example when only the progress text is updated.
		std::string m_progress_type;		// Type of progress, for example "parse", "geometry".
		std::wstring m_progress_text;		// A text that describes the current actions. It can be used for example to set a text on the progress bar.
	};

	StatusCallback() = default;
	virtual ~StatusCallback()= default;

	//\brief error callback mechanism to show messages in gui
	virtual void setMessageCallBack( void* obj_ptr, void (*func)(void*, shared_ptr<Message> t) )
	{
		m_obj_call_on_message = obj_ptr;
		m_func_call_on_message = func;
	}

	virtual void setMessageTarget( StatusCallback* other )
	{
		m_redirect_target = other;
	}

	virtual void unsetMessageCallBack()
	{
		m_obj_call_on_message = nullptr;
		m_func_call_on_message = nullptr;
	}

	virtual void setCancelCheck( void* obj_ptr, bool (*func)(void*) )
	{
		m_obj_call_check_cancel = obj_ptr;
		m_func_check_cancel = func;
	}

	virtual void unsetCancelCheck()
	{
		m_obj_call_check_cancel = nullptr;
		m_func_check_cancel = nullptr;
	}

	//\brief trigger the callback to pass a message, warning, or error, for example to store in a logfile
	virtual void messageCallback( shared_ptr<Message> m )
	{
		if( m_redirect_target )
		{
			m_redirect_target->messageCallback( m );
			return;
		}

#ifdef _DEBUG
		if( !m_func_call_on_message || !m_obj_call_on_message )
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
						std::wcout << L"messageCallback not set. Lost message: " << m->m_message_text.c_str() << std::endl;
						break;
				}
			}
		}

		if( m->m_message_type == MESSAGE_TYPE_ERROR )
		{
			std::wcout << L"error: " << m->m_message_text.c_str() << std::endl;
		}
#endif

		if( m_func_call_on_message )
		{
			if( m_obj_call_on_message )
			{
#ifdef ENABLE_OPENMP
				// Note: this lock protects accesses only for this instance. If several StatusCallback (or derived) objects are bound to the same callback function, a lock is necessary there.
				ScopedLock lock( m_writelock );
#endif
				m_func_call_on_message( m_obj_call_on_message, m );
			}
		}
	}

	//\brief check if cancellation has been requested.
	virtual bool isCanceled()
	{
		if( m_redirect_target )
		{
			return m_redirect_target->isCanceled();
		}

		if( m_func_check_cancel )
		{
			if( m_obj_call_check_cancel )
			{
#ifdef ENABLE_OPENMP
				// Note: this lock protects accesses only for this instance. If several StatusCallback (or derived) objects are bound to the same callback function, a lock is necessary there.
				ScopedLock lock( m_writelock );
#endif
				return m_func_check_cancel( m_obj_call_check_cancel );
			}
		}

		return false;
	}

	virtual void messageCallback( const std::string& message_text, MessageType type, const char* reporting_function, BuildingEntity* entity = nullptr )
	{
		shared_ptr<Message> message( new Message() );
		message->m_message_text.assign( message_text.begin(), message_text.end() );
		message->m_message_type = type;
		message->m_reporting_function = reporting_function;
		message->m_entity = entity;
		messageCallback( message );
	}
	virtual void messageCallback( const std::wstring& message_text, MessageType type, const char* reporting_function, BuildingEntity* entity = nullptr )
	{
		shared_ptr<Message> message( new Message() );
		message->m_message_text.assign( message_text );
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
		progress_message->m_progress_type.assign( progress_type );
		messageCallback( progress_message );
	}
	virtual void progressTextCallback( const std::wstring& progress_text )
	{
		shared_ptr<Message> progress_message( new Message() );
		progress_message->m_message_type = MessageType::MESSAGE_TYPE_PROGRESS_TEXT;
		progress_message->m_progress_value = -1;
		progress_message->m_progress_text.assign( progress_text );
		messageCallback( progress_message );
	}
	virtual void clearMessagesCallback()
	{
		shared_ptr<Message> progress_message( new Message() );
		progress_message->m_message_type = MessageType::MESSAGE_TYPE_CLEAR_MESSAGES;
		messageCallback( progress_message );
	}
	virtual void canceledCallback()
	{
		shared_ptr<Message> canceled_message( new Message() );
		canceled_message->m_message_type = MessageType::MESSAGE_TYPE_CANCELED;
		messageCallback( canceled_message );
	}

protected:
	//\brief Pointer to the object on which the message callback function is called.
	void* m_obj_call_on_message = nullptr;

	//\brief Pointer to the object on which the cancel check function is called.
	void* m_obj_call_check_cancel = nullptr;

	//\brief Pointer to the callback function for messages.
	void (*m_func_call_on_message)(void*, shared_ptr<Message> t) = nullptr;

	//\brief Pointer to the predicate that determines whether an operation should be canceled.
	bool  (*m_func_check_cancel)(void*) = nullptr;

	StatusCallback* m_redirect_target = nullptr;

#ifdef ENABLE_OPENMP
	Mutex m_writelock;
#endif
};

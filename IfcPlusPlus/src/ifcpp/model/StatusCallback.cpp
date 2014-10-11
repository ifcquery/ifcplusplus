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

#include "IfcPPObject.h"
#include "IfcPPException.h"
#include "StatusCallback.h"

StatusCallback::StatusCallback()
{
	unsetProgressCallBack();
	unsetProgressTextCallBack();
	unsetMessageCallBack();
}

StatusCallback::~StatusCallback()
{
}

void StatusCallback::setProgressCallBack( void* obj_ptr, void (*func)(void*, double, const std::string& progress_type) )
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
void StatusCallback::unsetProgressCallBack()
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

void StatusCallback::progressCallback( double progress_value, const std::string& progress_type )
{
	if( m_func_call_on_progress )
	{
		if( m_obj_call_on_progress )
		{
			m_func_call_on_progress( m_obj_call_on_progress, progress_value, progress_type );
		}
	}
}

void StatusCallback::progressTextCallback( const std::wstring& str )
{
	if( m_func_call_on_progress_text )
	{
		if( m_obj_call_on_progress_text )
		{
			m_func_call_on_progress_text( m_obj_call_on_progress_text, str );
		}
	}
}

void StatusCallback::setProgressTextCallBack( void* obj_ptr, void (*func)(void*, const std::wstring& message) )
{
	m_obj_call_on_progress_text = obj_ptr;
	m_func_call_on_progress_text = func;

	// pass to children
	for( auto it = m_callback_children.begin(); it != m_callback_children.end(); ++it )
	{
		StatusCallback* child = *it;
		if( child )
		{
			child->setProgressTextCallBack( obj_ptr, func );
		}
	}
}

void StatusCallback::unsetProgressTextCallBack()
{
	m_obj_call_on_progress_text = nullptr;
	m_func_call_on_progress_text = nullptr;
	
	// pass to children
	for( auto it = m_callback_children.begin(); it != m_callback_children.end(); ++it )
	{
		StatusCallback* child = *it;
		if( child )
		{
			child->unsetProgressTextCallBack();
		}
	}
}

// message callback
void StatusCallback::setMessageCallBack( void* obj_ptr, void (*func)(void*, const std::wstring&, SeverityLevel) )
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

void StatusCallback::unsetMessageCallBack()
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

void StatusCallback::messageCallback( const std::string& message, SeverityLevel level, const char* reporting_function, IfcPPEntity* entity )
{
#ifdef _DEBUG
	if( !m_func_call_on_message )
	{
		std::cout << __FUNC__ << " !m_func_call_on_message. Lost message: " << message.c_str() << std::endl;
	}
	if( !m_obj_call_on_message )
	{
		std::cout << __FUNC__ << " !m_obj_call_on_message. Lost message: " << message.c_str() << std::endl;
	}
#endif

	if( m_func_call_on_message )
	{
		if( m_obj_call_on_message )
		{
			std::string reporting_function_str( reporting_function );
			std::wstringstream strs_report;
			if( reporting_function_str.size() > 0 )
			{
				strs_report << reporting_function << ", ";
			}
			strs_report << message.c_str();

			if( entity )
			{
				strs_report << ", IFC entity: #" << entity->m_id << "=" << entity->classname();
			}

#ifdef IFCPP_OPENMP
			ScopedLock lock( m_writelock );
#endif
			m_func_call_on_message( m_obj_call_on_message, strs_report.str().c_str(), level );
		}
	}
}

void StatusCallback::messageCallback( const std::wstring& message, SeverityLevel level, const char* reporting_function, IfcPPEntity* entity )
{
#ifdef _DEBUG
	if( !m_func_call_on_message )
	{
		std::cout << __FUNC__ << " !m_func_call_on_message. Lost message: " << message.c_str() << std::endl;
	}
	if( !m_obj_call_on_message )
	{
		std::cout << __FUNC__ << " !m_obj_call_on_message. Lost message: " << message.c_str() << std::endl;
	}
#endif

	if( m_func_call_on_message )
	{
		if( m_obj_call_on_message )
		{
			std::string reporting_function_str( reporting_function );
			std::wstringstream strs_report;
			if( reporting_function_str.size() > 0 )
			{
				strs_report << reporting_function << ", ";
			}
			strs_report << message.c_str();

			if( entity )
			{
				strs_report << ", IFC entity: #" << entity->m_id << "=" << entity->classname();
			}

#ifdef IFCPP_OPENMP
			ScopedLock lock( m_writelock );
#endif
			m_func_call_on_message( m_obj_call_on_message, strs_report.str().c_str(), level );
		}
	}
}

void StatusCallback::addCallbackChild( StatusCallback* child )
{
	if( child == this )
	{
#ifdef _DEBUG
		std::cout << __FUNC__ ": trying to add 'this' to children." << std::endl;
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

	if( m_obj_call_on_progress_text )
	{
		if( m_func_call_on_progress_text )
		{
			child->setProgressTextCallBack( m_obj_call_on_progress_text, m_func_call_on_progress_text );
		}
	}
}

void StatusCallback::removeCallbackChild( StatusCallback* remove_child )
{
	for( auto it = m_callback_children.begin(); it != m_callback_children.end(); )
	{
		StatusCallback* child = *it;
		if( child == remove_child )
		{
			it = m_callback_children.erase( it );
		}
		else
		{
			++it;
		}
	}
}

void StatusCallback::clearCallbackChildren()
{
	m_callback_children.clear();
}

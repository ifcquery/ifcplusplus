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

#include "StatusObservable.h"

StatusObservable::StatusObservable()
{
	unsetProgressCallBack();
	unsetProgressTextCallBack();
	unsetMessageCallBack();
	unsetErrorCallBack();
}

StatusObservable::~StatusObservable()
{
}

//void StatusObservable::slotProgressValueWrapper( void* ptr, double progress_value )
//{
//	StatusObservable* myself = (StatusObservable*)ptr;
//	myself->progressCallback( progress_value );
//}
//
//void StatusObservable::slotMessageWrapper( void* ptr, const std::string& str )
//{
//	StatusObservable* myself = (StatusObservable*)ptr;
//	if( myself )
//	{
//		myself->messageCallback( str );
//	}
//}
//
//void StatusObservable::slotErrorWrapper( void* ptr, const std::string& str )
//{
//	StatusObservable* myself = (StatusObservable*)ptr;
//	if( myself )
//	{
//		myself->errorCallback( str );
//	}
//}

void StatusObservable::setProgressCallBack( void* obj_ptr, void (*func)(void*, double, const std::string& progress_type) )
{
	m_obj_call_on_progress = obj_ptr;
	m_func_call_on_progress = func;
}
void StatusObservable::unsetProgressCallBack()
{
	m_obj_call_on_progress = 0;
	m_func_call_on_progress = 0;
}

void StatusObservable::progressCallback( double progress_value, const std::string& progress_type )
{
	if( m_func_call_on_progress )
	{
		if( m_obj_call_on_progress )
		{
			m_func_call_on_progress( m_obj_call_on_progress, progress_value, progress_type );
		}
	}
}

void StatusObservable::progressTextCallback( const std::string& str )
{
	if( m_func_call_on_progress_text )
	{
		if( m_obj_call_on_progress_text )
		{
			m_func_call_on_progress_text( m_obj_call_on_progress_text, str );
		}
	}
}

void StatusObservable::setProgressTextCallBack( void* obj_ptr, void (*func)(void*, const std::string& message) )
{
	m_obj_call_on_progress_text = obj_ptr;
	m_func_call_on_progress_text = func;
}

void StatusObservable::unsetProgressTextCallBack()
{
	m_obj_call_on_progress_text = 0;
	m_func_call_on_progress_text = 0;
}

// message callback
void StatusObservable::setMessageCallBack( void* obj_ptr, void (*func)(void*, const std::string&) )
{
	m_obj_call_on_message = obj_ptr;
	m_func_call_on_message = func;
}

void StatusObservable::unsetMessageCallBack()
{
	m_obj_call_on_message = 0;
	m_func_call_on_message = 0;
}

void StatusObservable::messageCallback( const std::string& str )
{
	if( m_func_call_on_message )
	{
		if( m_obj_call_on_message )
		{
			m_func_call_on_message( m_obj_call_on_message, str );
		}
	}
}

// error callback
void StatusObservable::setErrorCallBack( void* obj_ptr, void (*func)(void*, const std::string&) )
{
	m_obj_call_on_error = obj_ptr;
	m_func_call_on_error = func;
}

void StatusObservable::unsetErrorCallBack()
{
	m_obj_call_on_error = 0;
	m_func_call_on_error = 0;
}

void StatusObservable::errorCallback( const std::string& str )
{
	if( m_func_call_on_error )
	{
		if( m_obj_call_on_error )
		{
			m_func_call_on_error( m_obj_call_on_error, str );
		}
	}
}

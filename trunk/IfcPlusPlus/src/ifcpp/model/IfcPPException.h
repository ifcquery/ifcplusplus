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

#include <exception>
#include <string>
#include "IfcPPGlobal.h"

#define __FUNC__ __FUNCTION__
#define _func_ __FUNCTION__
#define __func__ __FUNC__

class IFCPP_EXPORT IfcPPException : public std::exception
{
public:
	IfcPPException( std::string reason )
	{
		m_reason_str = reason;
	}

	IfcPPException( std::string reason, const char* function_name )
	{
		m_reason_str.append(function_name);
		if( reason.size() > 0 )
		{
			m_reason_str.append( ": " );
			m_reason_str.append( reason );
		}
	}

	IfcPPException(std::wstring reason)
	{
		m_reason_str.assign(reason.begin(), reason.end());
	}

	IfcPPException(std::wstring reason, const char* function_name)
	{
		m_reason_str.append(function_name);
		if( reason.size() > 0 )
		{
			m_reason_str.append( ": " );
			std::string reason_str;
			reason_str.assign( reason.begin(), reason.end() );
			m_reason_str.append( reason_str );
		}
	}

	~IfcPPException() throw()
	{

	}

	virtual const char* what() const throw()
	{
		return m_reason_str.c_str();
	}

	std::string m_reason_str;
};

class IfcPPOutOfMemoryException : public std::exception
{
public:
	IfcPPOutOfMemoryException()
	{
		m_reason_str = "Out of memory";
	}

	IfcPPOutOfMemoryException( const char* function_name )
	{
		m_reason_str = "Out of memory in function: ";
		m_reason_str.append( function_name );
	}

	~IfcPPOutOfMemoryException() throw()
	{
	}

	virtual const char* what() const throw()
	{
		return m_reason_str.c_str();
	}

	std::string m_reason_str;
};

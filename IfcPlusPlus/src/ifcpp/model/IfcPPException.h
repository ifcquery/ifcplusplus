/* -*-c++-*- IFC++ www.ifcquery.com
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

#include <exception>
#include <string>
#include "IfcPPGlobal.h"

#define __FUNC__ __FUNCTION__
#define _func_ __FUNCTION__

class IfcPPException : public std::exception
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

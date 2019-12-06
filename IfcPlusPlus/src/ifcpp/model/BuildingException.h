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

#include <codecvt>
#include <exception>
#include <string>
#include "GlobalDefines.h"

#define __FUNC__ __FUNCTION__
#define _func_ __FUNCTION__

class BuildingException : public std::exception
{
public:
	BuildingException( std::string reason )
	{
		m_reason_str = reason;
	}

	BuildingException( std::string reason, const char* function_name )
	{
		m_reason_str.append(function_name);
		if( !reason.empty() )
		{
			m_reason_str.append( ": " );
			m_reason_str.append( reason.c_str() );
		}
	}

	BuildingException(std::wstring reason)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> StringConverter;
		std::string reason_str = StringConverter.to_bytes(reason);
		m_reason_str.assign(reason_str.begin(), reason_str.end());
	}

	BuildingException(std::wstring reason, const char* function_name)
	{
		m_reason_str.append(function_name);
		if( !reason.empty() )
		{
			m_reason_str.append( ": " );

			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> StringConverter;
			std::string reason_str = StringConverter.to_bytes(reason);

			m_reason_str.append( reason_str );
		}
	}

	~BuildingException() noexcept override = default;

	const char* what() const noexcept override
	{
		return m_reason_str.c_str();
	}

	std::string m_reason_str;
};

class OutOfMemoryException : public std::exception
{
public:
	OutOfMemoryException()
	{
		m_reason_str = "Out of memory";
	}

	OutOfMemoryException( const char* function_name )
	{
		m_reason_str = "Out of memory in function: ";
		m_reason_str.append( function_name );
	}

	~OutOfMemoryException() noexcept override = default;

	const char* what() const noexcept override
	{
		return m_reason_str.c_str();
	}

	std::string m_reason_str;
};

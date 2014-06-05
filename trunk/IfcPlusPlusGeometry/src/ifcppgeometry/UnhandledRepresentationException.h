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
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>

class UnhandledRepresentationException : public std::exception
{
public:
	UnhandledRepresentationException()
	{
	}
	UnhandledRepresentationException( shared_ptr<IfcRepresentationItem> item )
	{
		m_item = item;
	}

	~UnhandledRepresentationException() throw()
	{
	}

	const char* what() const throw()
	{
		return "Unhandled IFC Representation";
	}

	shared_ptr<IfcRepresentationItem> m_item;
	//shared_ptr<IfcPPAbstractSelect> m_select;
};

#ifdef _DEBUG

class DebugBreakException : public std::exception
{
public:
	DebugBreakException( std::string reason ) { m_reason = reason; }
	~DebugBreakException() throw() {}
	virtual const char* what() const throw() { return m_reason.c_str(); }
	std::string m_reason;
};

#endif

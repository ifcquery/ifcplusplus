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

#include "ifcpp/model/IfcPPException.h"

IfcPPException::IfcPPException( std::string reason )
{
	m_reason = reason;
}

IfcPPException::IfcPPException( std::string reason, const char* function_name )
{
	m_reason.append( function_name );
	m_reason.append( ": " );
	m_reason.append( reason );
}

IfcPPException::~IfcPPException() throw()
{

}

const char* IfcPPException::what() const throw()
{
	return m_reason.c_str();
}

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

#include <string>
#include <iostream>
#include "IfcPPException.h"
#include "ifcpp/writer/WriterUtil.h"
#include "IfcPPObject.h"

IfcPPBool::IfcPPBool(){}
IfcPPBool::IfcPPBool( bool value ) : m_value(value) {}
IfcPPBool::~IfcPPBool(){}
void IfcPPBool::readArgument( const std::wstring& attribute_value )
{
	if( boost::iequals( attribute_value, L".F." ) )
	{
		m_value = false;
	}
	else if( boost::iequals( attribute_value, L".T." ) )
	{
		m_value = true;;
	}
}

IfcPPLogical::IfcPPLogical(){}
IfcPPLogical::IfcPPLogical( LogicalEnum value ) : m_value(value) {}
IfcPPLogical::~IfcPPLogical(){}
void IfcPPLogical::readArgument( const std::wstring& attribute_value )
{
	if( boost::iequals(attribute_value, L".F." ) )
	{
		m_value = LOGICAL_FALSE;
	}
	else if( boost::iequals( attribute_value, L".T." ) )
	{
		m_value = LOGICAL_TRUE;
	}
	else if( boost::iequals( attribute_value, L".U." ) )
	{
		m_value = LOGICAL_UNKNOWN;;
	}
}

IfcPPInt::IfcPPInt(){}
IfcPPInt::IfcPPInt( int value ) : m_value(value) {}
IfcPPInt::~IfcPPInt(){}
void IfcPPInt::readArgument( const std::wstring& attribute_value )
{
	m_value = std::stoi( attribute_value );
}


IfcPPReal::IfcPPReal(){}
IfcPPReal::IfcPPReal( double value ) : m_value(value) {}
IfcPPReal::~IfcPPReal(){}
void IfcPPReal::readArgument( const std::wstring& attribute_value )
{
	m_value = std::stod( attribute_value.c_str() );
}

IfcPPString::IfcPPString(){}
IfcPPString::IfcPPString( std::wstring& value ) : m_value(value) {}
IfcPPString::~IfcPPString(){}
void IfcPPString::readArgument( const std::wstring& attribute_value )
{
	if( attribute_value.size() < 2 )
	{
		m_value = attribute_value.c_str();
		return;
	}
	if( attribute_value[0] == '\'' && attribute_value[attribute_value.size()-1] == '\'' )
	{
		m_value = attribute_value.substr( 1, attribute_value.size()-2 ).c_str();
	}
}





// ENTITY
IfcPPEntity::IfcPPEntity() : m_id(-1)
{
}

IfcPPEntity::IfcPPEntity( int id ) : m_id(id)
{
}

IfcPPEntity::~IfcPPEntity()
{
}

void IfcPPEntity::setId( int id )
{
	m_id = id;
}

void IfcPPEntity::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	throw IfcPPException("IfcPPEntity::readStepArguments(), this method should be overwritten");
}
void IfcPPEntity::getStepLine( std::stringstream& stream ) const
{
	throw IfcPPException("IfcPPEntity::getStepLine(), this method should be overwritten");
}

void IfcPPEntity::getStepParameter( std::stringstream& stream, bool ) const
{
	throw IfcPPException("IfcPPEntity::getStepParameter(), this method should be overwritten");
}

void IfcPPEntity::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self )
{
	throw IfcPPException("IfcPPEntity::setInverseCounterparts(), this method should be overwritten");
}

void IfcPPEntity::unlinkSelf()
{
	throw IfcPPException("IfcPPEntity::unlinkSelf(), this method should be overwritten");
}

void IfcPPEntity::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}

void IfcPPEntity::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}

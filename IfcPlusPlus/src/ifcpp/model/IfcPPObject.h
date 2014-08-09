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
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/IfcPPEntityEnums.h"

enum LogicalEnum { LOGICAL_TRUE, LOGICAL_FALSE, LOGICAL_UNKNOWN };

class IfcPPObject
{
public:
	IfcPPObject() {}
	~IfcPPObject() {}
	virtual const char* classname() const { return "IfcPPObject"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPObject() ); }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const {}
};

class IfcPPBool : virtual public IfcPPObject
{
public:
	IfcPPBool();
	IfcPPBool( bool value );
	~IfcPPBool();
	virtual const char* classname() const { return "IfcPPBool"; }
	void readArgument( const std::wstring& attribute_value );
	bool m_value;
};

class IfcPPLogical : virtual public IfcPPObject
{
public:
	IfcPPLogical();
	IfcPPLogical( LogicalEnum value );
	~IfcPPLogical();
	virtual const char* classname() const { return "IfcPPLogical"; }
	void readArgument( const std::wstring& attribute_value );
	LogicalEnum m_value;
};

class IfcPPInt : virtual public IfcPPObject
{
public:
	IfcPPInt();
	IfcPPInt( int value );
	~IfcPPInt();
	virtual const char* classname() const { return "IfcPPInt"; }
	void readArgument( const std::wstring& attribute_value );
	int m_value;
};

class IfcPPReal : virtual public IfcPPObject
{
public:
	IfcPPReal();
	IfcPPReal( double value );
	~IfcPPReal();
	//virtual const char* classname() const = 0;
	void readArgument( const std::wstring& attribute_value );
	double m_value;
};

class IfcPPString : virtual public IfcPPObject
{
public:
	IfcPPString();
	IfcPPString( std::wstring& value );
	~IfcPPString();
	virtual const char* classname() const { return "IfcPPString"; }
	void readArgument( const std::wstring& attribute_value );
	std::wstring m_value;
};

class IfcPPBinary : virtual public IfcPPObject
{
public:
	IfcPPBinary();
	IfcPPBinary( const char* value );
	~IfcPPBinary();
	virtual const char* classname() const { return "IfcPPBinary"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy();
	void readArgument( const char* attribute_value );
	const char* m_value;
};

// ENTITY
class IfcPPEntity : virtual public IfcPPObject
{
protected:
	int m_id;

public:
	IfcPPEntity();
	IfcPPEntity( int id );
	virtual ~IfcPPEntity();
	virtual const char* classname() const { return "IfcPPEntity"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() = 0;
	virtual void getStepLine( std::stringstream& stream ) const = 0;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map ) = 0;
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes ) = 0;
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& map_attributes ) = 0;
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self ) = 0;
	virtual void unlinkSelf() = 0;
	virtual const int getId() const { return m_id; }
	void setId( int id );
	std::string m_entity_argument_str;
	IfcPPEntityEnum m_entity_enum;
};

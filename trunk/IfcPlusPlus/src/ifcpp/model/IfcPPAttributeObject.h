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
#include <string>
#include <vector>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"

class IfcPPAttributeObjectVector : public IfcPPObject
{
public:
	IfcPPAttributeObjectVector();
	IfcPPAttributeObjectVector( std::vector<shared_ptr<IfcPPObject> >& vec );
	~IfcPPAttributeObjectVector();
	virtual const char* classname() const { return "IfcPPAttributeObjectVector"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() const { return shared_ptr<IfcPPObject>( new IfcPPAttributeObjectVector() );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const {}
	std::vector<shared_ptr<IfcPPObject> > m_vec;
};

//template<typename T>
//class IfcPPEnumAttribute : public IfcPPObject
//{
//public:
//	IfcPPEnumAttribute(T en) : m_enum(en)
//	{
//	}
//	~IfcPPEnumAttribute(){}
//	T m_enum;
//};

class IfcPPBoolAttribute : public IfcPPObject
{
public:
	IfcPPBoolAttribute(){}
	IfcPPBoolAttribute( bool value ) : m_value( value ){}
	~IfcPPBoolAttribute(){}
	virtual const char* classname() const { return "IfcPPBoolAttribute"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPBoolAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	bool m_value;
};

class IfcPPLogicalAttribute : public IfcPPObject
{
public:
	IfcPPLogicalAttribute(){}
	IfcPPLogicalAttribute( LogicalEnum value ) : m_value( value ){}
	~IfcPPLogicalAttribute(){}
	virtual const char* classname() const { return "IfcPPLogicalAttribute"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPLogicalAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	LogicalEnum m_value;
};

class IfcPPIntAttribute : public IfcPPObject
{
public:
	IfcPPIntAttribute(){}
	IfcPPIntAttribute( int value ) : m_value( value ){}
	~IfcPPIntAttribute(){}
	virtual const char* classname() const { return "IfcPPIntAttribute"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPIntAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	int m_value;
};

class IfcPPRealAttribute : public IfcPPObject
{
public:
	IfcPPRealAttribute(){}
	IfcPPRealAttribute( double value ) : m_value( value ){}
	~IfcPPRealAttribute(){}
	virtual const char* classname() const { return "IfcPPReal"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPRealAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	double m_value;
};

class IfcPPStringAttribute : public IfcPPObject
{
public:
	IfcPPStringAttribute(){}
	IfcPPStringAttribute( std::wstring& value ) : m_value( value ){}
	~IfcPPStringAttribute(){}
	virtual const char* classname() const { return "IfcPPStringAttribute"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPStringAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	std::wstring m_value;
};

class IfcPPBinaryAttribute : public IfcPPObject
{
public:
	IfcPPBinaryAttribute(){}
	IfcPPBinaryAttribute( const char* value ) : m_value( value ){}
	~IfcPPBinaryAttribute(){}
	virtual const char* classname() const { return "IfcPPBinaryAttribute"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPBinaryAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	const char* m_value;
};

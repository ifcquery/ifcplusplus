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
#include <string>
#include <vector>
#include "GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

class AttributeObjectVector : public BuildingObject
{
public:
	AttributeObjectVector() {}
	AttributeObjectVector( std::vector<shared_ptr<BuildingObject> >& vec ){ m_vec = vec; }
	virtual ~AttributeObjectVector(){}
	virtual const char* className() const { return "AttributeObjectVector"; }
	virtual const std::wstring toString() const
	{
		std::wstring result_str;
		for( size_t ii = 0; ii < m_vec.size(); ++ii )
		{
			if( ii > 0 )
			{
				result_str.append( L", " );
			}
			result_str.append( m_vec[ii]->toString() );
		}
		return result_str;
	}
	virtual shared_ptr<BuildingObject> getDeepCopy() const { return shared_ptr<BuildingObject>( new AttributeObjectVector() );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const {}
	std::vector<shared_ptr<BuildingObject> > m_vec;
};

class BoolAttribute : public BuildingObject
{
public:
	BoolAttribute(){}
	BoolAttribute( bool value ) : m_value( value ){}
	virtual ~BoolAttribute(){}
	virtual const char* className() const { return "BoolAttribute"; }
	virtual const std::wstring toString() const
	{
		if( m_value ) { return L"true"; }
		else { return L"false"; }
	}
	virtual shared_ptr<BuildingObject> getDeepCopy() { return shared_ptr<BuildingObject>( new BoolAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	bool m_value;
};

class LogicalAttribute : public BuildingObject
{
public:
	LogicalAttribute(){}
	LogicalAttribute( LogicalEnum value ) : m_value( value ){}
	virtual ~LogicalAttribute(){}
	virtual const char* className() const { return "LogicalAttribute"; }
	virtual const std::wstring toString() const
	{
		if( m_value == LOGICAL_TRUE ) { return L"true"; }
		else if( m_value == LOGICAL_FALSE ) { return L"false"; }
		else { return L"unknown"; }
	}
	virtual shared_ptr<BuildingObject> getDeepCopy() { return shared_ptr<BuildingObject>( new LogicalAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	LogicalEnum m_value;
};

class IntegerAttribute : public BuildingObject
{
public:
	IntegerAttribute(){}
	IntegerAttribute( int value ) : m_value( value ){}
	virtual ~IntegerAttribute(){}
	virtual const char* className() const { return "IntegerAttribute"; }
	virtual const std::wstring toString() const
	{
		std::wstringstream strs;
		strs << m_value;
		return strs.str();
	}
	virtual shared_ptr<BuildingObject> getDeepCopy() { return shared_ptr<BuildingObject>( new IntegerAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	int m_value;
};

class RealAttribute : public BuildingObject
{
public:
	RealAttribute(){}
	RealAttribute( double value ) : m_value( value ){}
	virtual ~RealAttribute(){}
	virtual const char* className() const { return "RealAttribute"; }
	virtual const std::wstring toString() const
	{
		std::wstringstream strs;
		strs << m_value;
		return strs.str();
	}
	virtual shared_ptr<BuildingObject> getDeepCopy() { return shared_ptr<BuildingObject>( new RealAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	double m_value;
};

class StringAttribute : public BuildingObject
{
public:
	StringAttribute(){}
	StringAttribute( std::wstring& value ) : m_value( value ){}
	virtual ~StringAttribute(){}
	virtual const char* className() const { return "StringAttribute"; }
	virtual const std::wstring toString() const
	{
		return m_value;
	}
	virtual shared_ptr<BuildingObject> getDeepCopy() { return shared_ptr<BuildingObject>( new StringAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	std::wstring m_value;
};

class BinaryAttribute : public BuildingObject
{
public:
	BinaryAttribute(){}
	BinaryAttribute( const std::wstring& value ) { m_value = value.c_str(); }
	BinaryAttribute( const wchar_t* value ) : m_value( value ) {}
	virtual ~BinaryAttribute(){}
	virtual const char* className() const { return "BinaryAttribute"; }
	virtual const std::wstring toString() const
	{
		return m_value;
	}
	virtual shared_ptr<BuildingObject> getDeepCopy() { return shared_ptr<BuildingObject>( new BinaryAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	const wchar_t* m_value;
};

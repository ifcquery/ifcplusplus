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
#include <string>
#include <vector>
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPObject.h"
#include "IfcPPGlobal.h"

class IfcPPAttributeObjectVector : public IfcPPObject
{
public:
	IfcPPAttributeObjectVector() {}
	IfcPPAttributeObjectVector( std::vector<shared_ptr<IfcPPObject> >& vec ){ m_vec = vec; }
	virtual ~IfcPPAttributeObjectVector(){}
	virtual const char* className() const { return "IfcPPAttributeObjectVector"; }
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
	virtual shared_ptr<IfcPPObject> getDeepCopy() const { return shared_ptr<IfcPPObject>( new IfcPPAttributeObjectVector() );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const {}
	std::vector<shared_ptr<IfcPPObject> > m_vec;
};

class IfcPPBoolAttribute : public IfcPPObject
{
public:
	IfcPPBoolAttribute(){}
	IfcPPBoolAttribute( bool value ) : m_value( value ){}
	virtual ~IfcPPBoolAttribute(){}
	virtual const char* className() const { return "IfcPPBoolAttribute"; }
	virtual const std::wstring toString() const
	{
		if( m_value ) { return L"true"; }
		else { return L"false"; }
	}
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPBoolAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	bool m_value;
};

class IfcPPLogicalAttribute : public IfcPPObject
{
public:
	IfcPPLogicalAttribute(){}
	IfcPPLogicalAttribute( LogicalEnum value ) : m_value( value ){}
	virtual ~IfcPPLogicalAttribute(){}
	virtual const char* className() const { return "IfcPPLogicalAttribute"; }
	virtual const std::wstring toString() const
	{
		if( m_value == LOGICAL_TRUE ) { return L"true"; }
		else if( m_value == LOGICAL_FALSE ) { return L"false"; }
		else { return L"unknown"; }
	}
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPLogicalAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	LogicalEnum m_value;
};

class IfcPPIntAttribute : public IfcPPObject
{
public:
	IfcPPIntAttribute(){}
	IfcPPIntAttribute( int value ) : m_value( value ){}
	virtual ~IfcPPIntAttribute(){}
	virtual const char* className() const { return "IfcPPIntAttribute"; }
	virtual const std::wstring toString() const
	{
		std::wstringstream strs;
		strs << m_value;
		return strs.str();
	}
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPIntAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	int m_value;
};

class IfcPPRealAttribute : public IfcPPObject
{
public:
	IfcPPRealAttribute(){}
	IfcPPRealAttribute( double value ) : m_value( value ){}
	virtual ~IfcPPRealAttribute(){}
	virtual const char* className() const { return "IfcPPRealAttribute"; }
	virtual const std::wstring toString() const
	{
		std::wstringstream strs;
		strs << m_value;
		return strs.str();
	}
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPRealAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	double m_value;
};

class IfcPPStringAttribute : public IfcPPObject
{
public:
	IfcPPStringAttribute(){}
	IfcPPStringAttribute( std::wstring& value ) : m_value( value ){}
	virtual ~IfcPPStringAttribute(){}
	virtual const char* className() const { return "IfcPPStringAttribute"; }
	virtual const std::wstring toString() const
	{
		return m_value;
	}
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPStringAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	std::wstring m_value;
};

class IfcPPBinaryAttribute : public IfcPPObject
{
public:
	IfcPPBinaryAttribute(){}
	IfcPPBinaryAttribute( const std::wstring& value ) { m_value = value.c_str(); }
	IfcPPBinaryAttribute( const wchar_t* value ) : m_value( value ) {}
	virtual ~IfcPPBinaryAttribute(){}
	virtual const char* className() const { return "IfcPPBinaryAttribute"; }
	virtual const std::wstring toString() const
	{
		return m_value;
	}
	virtual shared_ptr<IfcPPObject> getDeepCopy() { return shared_ptr<IfcPPObject>( new IfcPPBinaryAttribute( m_value ) );  }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const{}
	const wchar_t* m_value;
};

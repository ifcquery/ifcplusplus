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
	AttributeObjectVector() = default;
	AttributeObjectVector( std::vector<shared_ptr<BuildingObject> >& vec ){ m_vec = vec; }
	virtual ~AttributeObjectVector()= default;
	uint32_t classID() const override { return 3667068888; }
	void getStepParameter( std::stringstream& /*stream*/, bool /*is_select_type = false*/, size_t /*precision*/ ) const override {}
	std::vector<shared_ptr<BuildingObject> > m_vec;
};

class BoolAttribute : public BuildingObject
{
public:
	BoolAttribute()= default;
	BoolAttribute( bool value ) : m_value( value ){}
	virtual ~BoolAttribute()= default;
	uint32_t classID() const override { return 164996446; }
	void getStepParameter( std::stringstream& /*stream*/, bool /*is_select_type = false*/, size_t /*precision*/ ) const override{}
	bool m_value;
};

class LogicalAttribute : public BuildingObject
{
public:
	LogicalAttribute()= default;
	LogicalAttribute( LogicalEnum value ) : m_value( value ){}
	virtual ~LogicalAttribute()= default;
	uint32_t classID() const override { return 1777643854; }
	void getStepParameter( std::stringstream& /*stream*/, bool /*is_select_type = false*/, size_t /*precision*/ ) const override{}
	LogicalEnum m_value;
};

class IntegerAttribute : public BuildingObject
{
public:
	IntegerAttribute()= default;
	IntegerAttribute( int value ) : m_value( value ){}
	virtual ~IntegerAttribute()= default;
	uint32_t classID() const override { return 2693262938; }
	void getStepParameter( std::stringstream& /*stream*/, bool /*is_select_type = false*/, size_t /*precision*/ ) const override{}
	int m_value;
};

class RealAttribute : public BuildingObject
{
public:
	RealAttribute()= default;
	RealAttribute( double value ) : m_value( value ){}
	virtual ~RealAttribute()= default;
	uint32_t classID() const override { return 93569251; }
	void getStepParameter( std::stringstream& /*stream*/, bool /*is_select_type = false*/, size_t /*precision*/ ) const override{}
	double m_value;
};

class StringAttribute : public BuildingObject
{
public:
	StringAttribute()= default;
	StringAttribute( std::string& value ) : m_value( value ){}
	virtual ~StringAttribute()= default;
	uint32_t classID() const override { return 308612603; }
	void getStepParameter( std::stringstream& /*stream*/, bool /*is_select_type = false*/, size_t /*precision*/ ) const override{}
	std::string m_value;
};

class BinaryAttribute : public BuildingObject
{
public:
	BinaryAttribute()= default;
	BinaryAttribute( const std::string& value ) { m_value = value.c_str(); }
	BinaryAttribute( const char* value ) : m_value( value ) {}
	virtual ~BinaryAttribute()= default;
	uint32_t classID() const override { return 2341374947; }
	void getStepParameter( std::stringstream& /*stream*/, bool /*is_select_type = false*/, size_t /*precision*/ ) const override{}
	const char* m_value;
};

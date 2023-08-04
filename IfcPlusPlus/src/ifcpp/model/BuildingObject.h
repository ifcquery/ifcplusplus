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

#include "GlobalDefines.h"
#include <iterator>
#include <sstream>
#include <vector>
#include <limits>
#include <map>
#include "ifcpp/model/BasicTypes.h"

enum BuildingEntityEnum {};
enum LogicalEnum { LOGICAL_TRUE, LOGICAL_FALSE, LOGICAL_UNKNOWN };

class IFCQUERY_EXPORT BuildingObject
{
public:
	virtual uint32_t classID() const = 0;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const = 0;
};

// ENTITY
class IFCQUERY_EXPORT BuildingEntity : virtual public BuildingObject
{
public:
	BuildingEntity() : m_tag(-1)
	{
	}

	BuildingEntity( int tag ) : m_tag(tag)
	{
	}

	virtual ~BuildingEntity()
	{
	}
	virtual uint32_t classID()  const override = 0;

	/** \brief Appends a line in STEP format to stream, including all attributes. */
	virtual void getStepLine(std::stringstream& stream, size_t precision) const = 0;

	/** \brief Reads all attributes from args. References to other entities are taken from map_entities. */
	virtual void readStepArguments(const std::vector<std::string>& args, const std::map<int, shared_ptr<BuildingEntity> >& map_entities, std::stringstream& errorStream) = 0;

	/** \brief Number of attributes, including inherited attributes, without inverse attributes */
	virtual uint8_t getNumAttributes() const = 0;

	/** \brief Adds all attributes (including inherited attributes) with name and value to vec_attributes. Single attributes can be accessed directly, without this method.*/
	virtual void getAttributes(std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes) const = 0;

	/** \brief Same as getAttributes, but for inverse attributes.*/
	virtual void getAttributesInverse(std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& map_attributes) const = 0;

	/** \brief If there is a reference from object a to object b, and b has an inverse reference to a, the inverse reference is established here.*/
	virtual void setInverseCounterparts(shared_ptr<BuildingEntity> ptr_self) = 0;

	/** \brief Removes the inverse reference established in setInverseCounterparts.*/
	virtual void unlinkFromInverseCounterparts() {}

	/// Entity ID (same as STEP ID)
	int m_tag;
};

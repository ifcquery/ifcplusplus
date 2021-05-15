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
#include <map>
#include "ifcpp/model/BasicTypes.h"

enum LogicalEnum { LOGICAL_TRUE, LOGICAL_FALSE, LOGICAL_UNKNOWN };

struct BuildingCopyOptions
{
public:
	bool shallow_copy_IfcOwnerHistory = true;			// If set to true, references to the existing IfcOwnerHistory are set, instead of creating a deep copy
	bool shallow_copy_IfcRepresentationContext = true;	// If set to true, references to the existing IfcRepresentationContext (or derived) are set, instead of creating a deep copy
	bool shallow_copy_IfcProfileDef = true;				// If set to true, references to the existing IfcProfileDef (or derived) are set, instead of creating a deep copy
	bool shallow_copy_IfcLocalPlacement_PlacementRelTo = true; // If set to true, not the complete coordinate system hierarchy is copied, create shallow copy instead
	bool create_new_IfcGloballyUniqueId = true;			// If set to true, all copies of type IfcGloballyUniqueId get a new GUID.
};

class IFCQUERY_EXPORT BuildingObject
{
public:
	virtual const char* className() const = 0;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const = 0;
	
	/** \brief Returns a self-description of the Type or Entity 
	*  Enum types return their value as they appear in the STEP file
	*/
	virtual const std::wstring toString() const = 0;
};

// ENTITY
class IFCQUERY_EXPORT BuildingEntity : virtual public BuildingObject
{
public:
	BuildingEntity() : m_entity_id(-1)
	{
	}

	BuildingEntity( int id ) : m_entity_id(id)
	{
	}

	virtual ~BuildingEntity()
	{
	}
	virtual const char* className() const = 0;

	/** \brief Creates a deep copy of the object, recursively creating deep copies of attributes.
	 *  Usually it makes sense to create only a shallow copy (not a new object) for entities like IfcOwnerHistory, IfcRepresentationContext and others.
	 *  The exact copying behaviour can be set with BuildingCopyOptions.
	 *	Inverse attributes are not copied.
	*/
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options ) = 0;
	
	/** \brief Appends a line in STEP format to stream, including all attributes. */
	virtual void getStepLine( std::stringstream& stream ) const = 0;

	/** \brief Reads all attributes from args. References to other entities are taken from map_entities. */
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map_entities ) = 0;

	/** \brief Number of attributes, including inherited attributes, without inverse attributes */
	virtual size_t getNumAttributes() = 0;

	/** \brief Adds all attributes (including inherited attributes) with name and value to vec_attributes. Single attributes can be accessed directly, without this method.*/
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const = 0;

	/** \brief Same as getAttributes, but for inverse attributes.*/
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& map_attributes ) const = 0;

	/** \brief If there is a reference from object a to object b, and b has an inverse reference to a, the inverse reference is established here.*/
	virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self ) = 0;

	/** \brief Removes the inverse reference established in setInverseCounterparts.*/
	virtual void unlinkFromInverseCounterparts() = 0;

	/// Entity ID (same as STEP ID)
	int m_entity_id;
};

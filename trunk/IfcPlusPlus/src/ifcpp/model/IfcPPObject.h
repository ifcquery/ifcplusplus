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

struct IfcPPCopyOptions
{
public:
	bool shallow_copy_IfcOwnerHistory = true;			// If set to true, references to the existing IfcOwnerHistory are set, instead of creating a deep copy
	bool shallow_copy_IfcRepresentationContext = true;	// If set to true, references to the existing IfcRepresentationContext (or derived) are set, instead of creating a deep copy
	bool shallow_copy_IfcProfileDef = true;				// If set to true, references to the existing IfcProfileDef (or derived) are set, instead of creating a deep copy
	bool shallow_copy_IfcLocalPlacement_PlacementRelTo = true; // If set to true, not the complete coordinate system hierarchy is copied, create shallow copy instead
	bool create_new_IfcGloballyUniqueId = true;			// If set to true, all copies of type IfcGloballyUniqueId get a new GUID.
};

class IfcPPObject
{
public:
	virtual const char* classname() const = 0;
};

// ENTITY
class IfcPPEntity : virtual public IfcPPObject
{
public:
	IfcPPEntity();
	IfcPPEntity( int id );
	virtual ~IfcPPEntity();
	virtual const char* classname() const = 0;

	/** \brief Creates a deep copy of the object, recursively creating deep copies of attributes.
	 *  Usually it makes sense to create only a shallow copy (not a new object) for entities like IfcOwnerHistory, IfcRepresentationContext and others.
	 *  The exact copying behaviour can be set with IfcPPCopyOptions.
	 *	Inverse attributes are not copied.
	*/
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options ) = 0;
	
	/** \brief Appends a line in STEP format to stream, including all attributes. */
	virtual void getStepLine( std::stringstream& stream ) const = 0;

	/** \brief Reads all attributes from args. References to other entities are taken from map_entities. */
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities ) = 0;

	/** \brief Adds all attributes (including inherited attributes) with name and value to vec_attributes. Single attributes can be accessed directly, without this method.*/
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes ) = 0;

	/** \brief Same as getAttributes, but for inverse attributes.*/
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& map_attributes ) = 0;

	/** \brief If there is a reference from object a to object b, and b has an inverse reference to a, the inverse reference is established here.*/
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self ) = 0;

	/** \brief Removes the inverse reference established in setInverseCounterparts.*/
	virtual void unlinkSelf() = 0;

	/// Entity ID (same as STEP ID)
	int m_id;
	
	/// Enum reqresenting the entity type
	IfcPPEntityEnum m_entity_enum;
};

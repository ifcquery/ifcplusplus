/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
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
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcPreDefinedProperties.h"

// ENTITY IfcPreDefinedProperties 
IfcPreDefinedProperties::IfcPreDefinedProperties() {}
IfcPreDefinedProperties::IfcPreDefinedProperties( int id ) { m_id = id; }
IfcPreDefinedProperties::~IfcPreDefinedProperties() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPreDefinedProperties::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPreDefinedProperties> other = dynamic_pointer_cast<IfcPreDefinedProperties>(other_entity);
	if( !other) { return; }
}
void IfcPreDefinedProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPREDEFINEDPROPERTIES" << "(";
	stream << ");";
}
void IfcPreDefinedProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPreDefinedProperties::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcPreDefinedProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyAbstraction::getAttributes( vec_attributes );
}
void IfcPreDefinedProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPreDefinedProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyAbstraction::setInverseCounterparts( ptr_self_entity );
}
void IfcPreDefinedProperties::unlinkSelf()
{
	IfcPropertyAbstraction::unlinkSelf();
}

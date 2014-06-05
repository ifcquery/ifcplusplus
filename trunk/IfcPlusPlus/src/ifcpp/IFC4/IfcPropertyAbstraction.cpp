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
#include "include/IfcPropertyAbstraction.h"

// ENTITY IfcPropertyAbstraction 
IfcPropertyAbstraction::IfcPropertyAbstraction() {}
IfcPropertyAbstraction::IfcPropertyAbstraction( int id ) { m_id = id; }
IfcPropertyAbstraction::~IfcPropertyAbstraction() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertyAbstraction::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertyAbstraction> other = dynamic_pointer_cast<IfcPropertyAbstraction>(other_entity);
	if( !other) { return; }
}
void IfcPropertyAbstraction::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYABSTRACTION" << "(";
	stream << ");";
}
void IfcPropertyAbstraction::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyAbstraction::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcPropertyAbstraction::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}
void IfcPropertyAbstraction::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasExternalReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasExternalReferences_inverse.size(); ++i ) { HasExternalReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReferences_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasExternalReferences_inverse", HasExternalReferences_inverse_vec_obj ) );
}
void IfcPropertyAbstraction::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcPropertyAbstraction::unlinkSelf()
{
}

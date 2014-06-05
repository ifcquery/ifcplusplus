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
#include "include/IfcDefinitionSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertyDefinition.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssociates 
IfcRelAssociates::IfcRelAssociates() {}
IfcRelAssociates::IfcRelAssociates( int id ) { m_id = id; }
IfcRelAssociates::~IfcRelAssociates() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssociates::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssociates> other = dynamic_pointer_cast<IfcRelAssociates>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
}
void IfcRelAssociates::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSOCIATES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_RelatedObjects, true );
	stream << ");";
}
void IfcRelAssociates::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociates::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssociates, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcRelAssociates, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readSelectList( args[4], m_RelatedObjects, map );
}
void IfcRelAssociates::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelationship::getAttributes( vec_attributes );
	shared_ptr<IfcPPAttributeObjectVector> RelatedObjects_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_RelatedObjects.begin(), m_RelatedObjects.end(), std::back_inserter( RelatedObjects_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "RelatedObjects", RelatedObjects_vec_object ) );
}
void IfcRelAssociates::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelAssociates::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssociates> ptr_self = dynamic_pointer_cast<IfcRelAssociates>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssociates::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcObjectDefinition>  RelatedObjects_IfcObjectDefinition = dynamic_pointer_cast<IfcObjectDefinition>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObjectDefinition )
		{
			RelatedObjects_IfcObjectDefinition->m_HasAssociations_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcPropertyDefinition>  RelatedObjects_IfcPropertyDefinition = dynamic_pointer_cast<IfcPropertyDefinition>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcPropertyDefinition )
		{
			RelatedObjects_IfcPropertyDefinition->m_HasAssociations_inverse.push_back( ptr_self );
		}
	}
}
void IfcRelAssociates::unlinkSelf()
{
	IfcRelationship::unlinkSelf();
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcObjectDefinition>  RelatedObjects_IfcObjectDefinition = dynamic_pointer_cast<IfcObjectDefinition>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObjectDefinition )
		{
			std::vector<weak_ptr<IfcRelAssociates> >& HasAssociations_inverse = RelatedObjects_IfcObjectDefinition->m_HasAssociations_inverse;
			std::vector<weak_ptr<IfcRelAssociates> >::iterator it_HasAssociations_inverse;
			for( it_HasAssociations_inverse = HasAssociations_inverse.begin(); it_HasAssociations_inverse != HasAssociations_inverse.end(); ++it_HasAssociations_inverse)
			{
				shared_ptr<IfcRelAssociates> self_candidate( *it_HasAssociations_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					HasAssociations_inverse.erase( it_HasAssociations_inverse );
					break;
				}
			}
		}
		shared_ptr<IfcPropertyDefinition>  RelatedObjects_IfcPropertyDefinition = dynamic_pointer_cast<IfcPropertyDefinition>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcPropertyDefinition )
		{
			std::vector<weak_ptr<IfcRelAssociates> >& HasAssociations_inverse = RelatedObjects_IfcPropertyDefinition->m_HasAssociations_inverse;
			std::vector<weak_ptr<IfcRelAssociates> >::iterator it_HasAssociations_inverse;
			for( it_HasAssociations_inverse = HasAssociations_inverse.begin(); it_HasAssociations_inverse != HasAssociations_inverse.end(); ++it_HasAssociations_inverse)
			{
				shared_ptr<IfcRelAssociates> self_candidate( *it_HasAssociations_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					HasAssociations_inverse.erase( it_HasAssociations_inverse );
					break;
				}
			}
		}
	}
}

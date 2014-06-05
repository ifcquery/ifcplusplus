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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"

// ENTITY IfcTypeObject 
IfcTypeObject::IfcTypeObject() {}
IfcTypeObject::IfcTypeObject( int id ) { m_id = id; }
IfcTypeObject::~IfcTypeObject() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTypeObject::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTypeObject> other = dynamic_pointer_cast<IfcTypeObject>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ApplicableOccurrence = other->m_ApplicableOccurrence;
	m_HasPropertySets = other->m_HasPropertySets;
}
void IfcTypeObject::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTYPEOBJECT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ApplicableOccurrence ) { m_ApplicableOccurrence->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertySets );
	stream << ");";
}
void IfcTypeObject::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTypeObject::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTypeObject, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcTypeObject, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromStepData( args[4] );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
}
void IfcTypeObject::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObjectDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ApplicableOccurrence", m_ApplicableOccurrence ) );
}
void IfcTypeObject::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> Types_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_Types_inverse.size(); ++i ) { Types_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByType>( m_Types_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "Types_inverse", Types_inverse_vec_obj ) );
}
void IfcTypeObject::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObjectDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcTypeObject> ptr_self = dynamic_pointer_cast<IfcTypeObject>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcTypeObject::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_HasPropertySets.size(); ++i )
	{
		if( m_HasPropertySets[i] )
		{
			m_HasPropertySets[i]->m_DefinesType_inverse.push_back( ptr_self );
		}
	}
}
void IfcTypeObject::unlinkSelf()
{
	IfcObjectDefinition::unlinkSelf();
	for( int i=0; i<m_HasPropertySets.size(); ++i )
	{
		if( m_HasPropertySets[i] )
		{
			std::vector<weak_ptr<IfcTypeObject> >& DefinesType_inverse = m_HasPropertySets[i]->m_DefinesType_inverse;
			std::vector<weak_ptr<IfcTypeObject> >::iterator it_DefinesType_inverse;
			for( it_DefinesType_inverse = DefinesType_inverse.begin(); it_DefinesType_inverse != DefinesType_inverse.end(); ++it_DefinesType_inverse)
			{
				shared_ptr<IfcTypeObject> self_candidate( *it_DefinesType_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					DefinesType_inverse.erase( it_DefinesType_inverse );
					break;
				}
			}
		}
	}
}

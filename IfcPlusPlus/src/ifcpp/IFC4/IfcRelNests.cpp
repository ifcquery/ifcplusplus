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
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcRelNests 
IfcRelNests::IfcRelNests() {}
IfcRelNests::IfcRelNests( int id ) { m_id = id; }
IfcRelNests::~IfcRelNests() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelNests::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelNests> other = dynamic_pointer_cast<IfcRelNests>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingObject = other->m_RelatingObject;
	m_RelatedObjects = other->m_RelatedObjects;
}
void IfcRelNests::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELNESTS" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingObject ) { stream << "#" << m_RelatingObject->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedObjects );
	stream << ");";
}
void IfcRelNests::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelNests::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelNests, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelNests, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_RelatingObject, map );
	readEntityReferenceList( args[5], m_RelatedObjects, map );
}
void IfcRelNests::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelDecomposes::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingObject", m_RelatingObject ) );
}
void IfcRelNests::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelNests::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelDecomposes::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelNests> ptr_self = dynamic_pointer_cast<IfcRelNests>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelNests::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		if( m_RelatedObjects[i] )
		{
			m_RelatedObjects[i]->m_Nests_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingObject )
	{
		m_RelatingObject->m_IsNestedBy_inverse.push_back( ptr_self );
	}
}
void IfcRelNests::unlinkSelf()
{
	IfcRelDecomposes::unlinkSelf();
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		if( m_RelatedObjects[i] )
		{
			std::vector<weak_ptr<IfcRelNests> >& Nests_inverse = m_RelatedObjects[i]->m_Nests_inverse;
			std::vector<weak_ptr<IfcRelNests> >::iterator it_Nests_inverse;
			for( it_Nests_inverse = Nests_inverse.begin(); it_Nests_inverse != Nests_inverse.end(); ++it_Nests_inverse)
			{
				shared_ptr<IfcRelNests> self_candidate( *it_Nests_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					Nests_inverse.erase( it_Nests_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingObject )
	{
		std::vector<weak_ptr<IfcRelNests> >& IsNestedBy_inverse = m_RelatingObject->m_IsNestedBy_inverse;
		std::vector<weak_ptr<IfcRelNests> >::iterator it_IsNestedBy_inverse;
		for( it_IsNestedBy_inverse = IsNestedBy_inverse.begin(); it_IsNestedBy_inverse != IsNestedBy_inverse.end(); ++it_IsNestedBy_inverse)
		{
			shared_ptr<IfcRelNests> self_candidate( *it_IsNestedBy_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				IsNestedBy_inverse.erase( it_IsNestedBy_inverse );
				break;
			}
		}
	}
}

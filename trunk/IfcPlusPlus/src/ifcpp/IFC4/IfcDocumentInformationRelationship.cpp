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
#include "include/IfcDocumentInformation.h"
#include "include/IfcDocumentInformationRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcText.h"

// ENTITY IfcDocumentInformationRelationship 
IfcDocumentInformationRelationship::IfcDocumentInformationRelationship() {}
IfcDocumentInformationRelationship::IfcDocumentInformationRelationship( int id ) { m_id = id; }
IfcDocumentInformationRelationship::~IfcDocumentInformationRelationship() {}

// method setEntity takes over all attributes from another instance of the class
void IfcDocumentInformationRelationship::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcDocumentInformationRelationship> other = dynamic_pointer_cast<IfcDocumentInformationRelationship>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingDocument = other->m_RelatingDocument;
	m_RelatedDocuments = other->m_RelatedDocuments;
	m_RelationshipType = other->m_RelationshipType;
}
void IfcDocumentInformationRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCDOCUMENTINFORMATIONRELATIONSHIP" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingDocument ) { stream << "#" << m_RelatingDocument->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedDocuments );
	stream << ",";
	if( m_RelationshipType ) { m_RelationshipType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDocumentInformationRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDocumentInformationRelationship::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDocumentInformationRelationship, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcDocumentInformationRelationship, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_RelatingDocument, map );
	readEntityReferenceList( args[3], m_RelatedDocuments, map );
	m_RelationshipType = IfcLabel::createObjectFromStepData( args[4] );
}
void IfcDocumentInformationRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcResourceLevelRelationship::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingDocument", m_RelatingDocument ) );
	vec_attributes.push_back( std::make_pair( "RelationshipType", m_RelationshipType ) );
}
void IfcDocumentInformationRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcDocumentInformationRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcResourceLevelRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcDocumentInformationRelationship> ptr_self = dynamic_pointer_cast<IfcDocumentInformationRelationship>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcDocumentInformationRelationship::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedDocuments.size(); ++i )
	{
		if( m_RelatedDocuments[i] )
		{
			m_RelatedDocuments[i]->m_IsPointedTo_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingDocument )
	{
		m_RelatingDocument->m_IsPointer_inverse.push_back( ptr_self );
	}
}
void IfcDocumentInformationRelationship::unlinkSelf()
{
	IfcResourceLevelRelationship::unlinkSelf();
	for( int i=0; i<m_RelatedDocuments.size(); ++i )
	{
		if( m_RelatedDocuments[i] )
		{
			std::vector<weak_ptr<IfcDocumentInformationRelationship> >& IsPointedTo_inverse = m_RelatedDocuments[i]->m_IsPointedTo_inverse;
			std::vector<weak_ptr<IfcDocumentInformationRelationship> >::iterator it_IsPointedTo_inverse;
			for( it_IsPointedTo_inverse = IsPointedTo_inverse.begin(); it_IsPointedTo_inverse != IsPointedTo_inverse.end(); ++it_IsPointedTo_inverse)
			{
				shared_ptr<IfcDocumentInformationRelationship> self_candidate( *it_IsPointedTo_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					IsPointedTo_inverse.erase( it_IsPointedTo_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingDocument )
	{
		std::vector<weak_ptr<IfcDocumentInformationRelationship> >& IsPointer_inverse = m_RelatingDocument->m_IsPointer_inverse;
		std::vector<weak_ptr<IfcDocumentInformationRelationship> >::iterator it_IsPointer_inverse;
		for( it_IsPointer_inverse = IsPointer_inverse.begin(); it_IsPointer_inverse != IsPointer_inverse.end(); ++it_IsPointer_inverse)
		{
			shared_ptr<IfcDocumentInformationRelationship> self_candidate( *it_IsPointer_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				IsPointer_inverse.erase( it_IsPointer_inverse );
				break;
			}
		}
	}
}

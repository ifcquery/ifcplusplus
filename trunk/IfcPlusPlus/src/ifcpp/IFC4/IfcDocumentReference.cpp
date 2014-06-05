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
#include "include/IfcDocumentReference.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcRelAssociatesDocument.h"
#include "include/IfcText.h"
#include "include/IfcURIReference.h"

// ENTITY IfcDocumentReference 
IfcDocumentReference::IfcDocumentReference() {}
IfcDocumentReference::IfcDocumentReference( int id ) { m_id = id; }
IfcDocumentReference::~IfcDocumentReference() {}

// method setEntity takes over all attributes from another instance of the class
void IfcDocumentReference::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcDocumentReference> other = dynamic_pointer_cast<IfcDocumentReference>(other_entity);
	if( !other) { return; }
	m_Location = other->m_Location;
	m_Identification = other->m_Identification;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ReferencedDocument = other->m_ReferencedDocument;
}
void IfcDocumentReference::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCDOCUMENTREFERENCE" << "(";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReferencedDocument ) { stream << "#" << m_ReferencedDocument->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcDocumentReference::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDocumentReference::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDocumentReference, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcDocumentReference, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Location = IfcURIReference::createObjectFromStepData( args[0] );
	m_Identification = IfcIdentifier::createObjectFromStepData( args[1] );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_ReferencedDocument, map );
}
void IfcDocumentReference::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExternalReference::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "ReferencedDocument", m_ReferencedDocument ) );
}
void IfcDocumentReference::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> DocumentRefForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_DocumentRefForObjects_inverse.size(); ++i ) { DocumentRefForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesDocument>( m_DocumentRefForObjects_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "DocumentRefForObjects_inverse", DocumentRefForObjects_inverse_vec_obj ) );
}
void IfcDocumentReference::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalReference::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcDocumentReference> ptr_self = dynamic_pointer_cast<IfcDocumentReference>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcDocumentReference::setInverseCounterparts: type mismatch" ); }
	if( m_ReferencedDocument )
	{
		m_ReferencedDocument->m_HasDocumentReferences_inverse.push_back( ptr_self );
	}
}
void IfcDocumentReference::unlinkSelf()
{
	IfcExternalReference::unlinkSelf();
	if( m_ReferencedDocument )
	{
		std::vector<weak_ptr<IfcDocumentReference> >& HasDocumentReferences_inverse = m_ReferencedDocument->m_HasDocumentReferences_inverse;
		std::vector<weak_ptr<IfcDocumentReference> >::iterator it_HasDocumentReferences_inverse;
		for( it_HasDocumentReferences_inverse = HasDocumentReferences_inverse.begin(); it_HasDocumentReferences_inverse != HasDocumentReferences_inverse.end(); ++it_HasDocumentReferences_inverse)
		{
			shared_ptr<IfcDocumentReference> self_candidate( *it_HasDocumentReferences_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasDocumentReferences_inverse.erase( it_HasDocumentReferences_inverse );
				break;
			}
		}
	}
}

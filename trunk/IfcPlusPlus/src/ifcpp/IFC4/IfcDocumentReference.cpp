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
#include "ifcpp/model/IfcPPGuid.h"
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
shared_ptr<IfcPPObject> IfcDocumentReference::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDocumentReference> copy_self( new IfcDocumentReference() );
	if( m_Location ) { copy_self->m_Location = dynamic_pointer_cast<IfcURIReference>( m_Location->getDeepCopy(options) ); }
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ReferencedDocument ) { copy_self->m_ReferencedDocument = dynamic_pointer_cast<IfcDocumentInformation>( m_ReferencedDocument->getDeepCopy(options) ); }
	return copy_self;
}
void IfcDocumentReference::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDOCUMENTREFERENCE" << "(";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReferencedDocument ) { stream << "#" << m_ReferencedDocument->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcDocumentReference::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDocumentReference::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDocumentReference, expecting 5, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Location = IfcURIReference::createObjectFromSTEP( args[0] );
	m_Identification = IfcIdentifier::createObjectFromSTEP( args[1] );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
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
	IfcExternalReference::getAttributesInverse( vec_attributes_inverse );
	if( m_DocumentRefForObjects_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DocumentRefForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_DocumentRefForObjects_inverse.size(); ++i )
		{
			if( !m_DocumentRefForObjects_inverse[i].expired() )
			{
				DocumentRefForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesDocument>( m_DocumentRefForObjects_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "DocumentRefForObjects_inverse", DocumentRefForObjects_inverse_vec_obj ) );
	}
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
		for( auto it_HasDocumentReferences_inverse = HasDocumentReferences_inverse.begin(); it_HasDocumentReferences_inverse != HasDocumentReferences_inverse.end(); ++it_HasDocumentReferences_inverse)
		{
			shared_ptr<IfcDocumentReference> self_candidate( *it_HasDocumentReferences_inverse );
			if( self_candidate.get() == this )
			{
				HasDocumentReferences_inverse.erase( it_HasDocumentReferences_inverse );
				break;
			}
		}
	}
}

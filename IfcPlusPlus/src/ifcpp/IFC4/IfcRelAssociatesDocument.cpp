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
#include "include/IfcDefinitionSelect.h"
#include "include/IfcDocumentInformation.h"
#include "include/IfcDocumentReference.h"
#include "include/IfcDocumentSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssociatesDocument.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssociatesDocument 
IfcRelAssociatesDocument::IfcRelAssociatesDocument() { m_entity_enum = IFCRELASSOCIATESDOCUMENT; }
IfcRelAssociatesDocument::IfcRelAssociatesDocument( int id ) { m_id = id; m_entity_enum = IFCRELASSOCIATESDOCUMENT; }
IfcRelAssociatesDocument::~IfcRelAssociatesDocument() {}
shared_ptr<IfcPPObject> IfcRelAssociatesDocument::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelAssociatesDocument> copy_self( new IfcRelAssociatesDocument() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedObjects.push_back( dynamic_pointer_cast<IfcDefinitionSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatingDocument ) { copy_self->m_RelatingDocument = dynamic_pointer_cast<IfcDocumentSelect>( m_RelatingDocument->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelAssociatesDocument::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELASSOCIATESDOCUMENT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_RelatedObjects.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcDefinitionSelect>& type_object = m_RelatedObjects[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_RelatingDocument ) { m_RelatingDocument->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcRelAssociatesDocument::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociatesDocument::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRelAssociatesDocument, expecting 6, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readSelectList( args[4], m_RelatedObjects, map );
	m_RelatingDocument = IfcDocumentSelect::createObjectFromSTEP( args[5], map );
}
void IfcRelAssociatesDocument::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssociates::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingDocument", m_RelatingDocument ) );
}
void IfcRelAssociatesDocument::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelAssociates::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelAssociatesDocument::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssociates::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssociatesDocument> ptr_self = dynamic_pointer_cast<IfcRelAssociatesDocument>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssociatesDocument::setInverseCounterparts: type mismatch" ); }
	shared_ptr<IfcDocumentInformation>  RelatingDocument_IfcDocumentInformation = dynamic_pointer_cast<IfcDocumentInformation>( m_RelatingDocument );
	if( RelatingDocument_IfcDocumentInformation )
	{
		RelatingDocument_IfcDocumentInformation->m_DocumentInfoForObjects_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcDocumentReference>  RelatingDocument_IfcDocumentReference = dynamic_pointer_cast<IfcDocumentReference>( m_RelatingDocument );
	if( RelatingDocument_IfcDocumentReference )
	{
		RelatingDocument_IfcDocumentReference->m_DocumentRefForObjects_inverse.push_back( ptr_self );
	}
}
void IfcRelAssociatesDocument::unlinkFromInverseCounterparts()
{
	IfcRelAssociates::unlinkFromInverseCounterparts();
	shared_ptr<IfcDocumentInformation>  RelatingDocument_IfcDocumentInformation = dynamic_pointer_cast<IfcDocumentInformation>( m_RelatingDocument );
	if( RelatingDocument_IfcDocumentInformation )
	{
		std::vector<weak_ptr<IfcRelAssociatesDocument> >& DocumentInfoForObjects_inverse = RelatingDocument_IfcDocumentInformation->m_DocumentInfoForObjects_inverse;
		for( auto it_DocumentInfoForObjects_inverse = DocumentInfoForObjects_inverse.begin(); it_DocumentInfoForObjects_inverse != DocumentInfoForObjects_inverse.end(); )
		{
			shared_ptr<IfcRelAssociatesDocument> self_candidate( *it_DocumentInfoForObjects_inverse );
			if( self_candidate.get() == this )
			{
				it_DocumentInfoForObjects_inverse= DocumentInfoForObjects_inverse.erase( it_DocumentInfoForObjects_inverse );
			}
			else
			{
				++it_DocumentInfoForObjects_inverse;
			}
		}
	}
	shared_ptr<IfcDocumentReference>  RelatingDocument_IfcDocumentReference = dynamic_pointer_cast<IfcDocumentReference>( m_RelatingDocument );
	if( RelatingDocument_IfcDocumentReference )
	{
		std::vector<weak_ptr<IfcRelAssociatesDocument> >& DocumentRefForObjects_inverse = RelatingDocument_IfcDocumentReference->m_DocumentRefForObjects_inverse;
		for( auto it_DocumentRefForObjects_inverse = DocumentRefForObjects_inverse.begin(); it_DocumentRefForObjects_inverse != DocumentRefForObjects_inverse.end(); )
		{
			shared_ptr<IfcRelAssociatesDocument> self_candidate( *it_DocumentRefForObjects_inverse );
			if( self_candidate.get() == this )
			{
				it_DocumentRefForObjects_inverse= DocumentRefForObjects_inverse.erase( it_DocumentRefForObjects_inverse );
			}
			else
			{
				++it_DocumentRefForObjects_inverse;
			}
		}
	}
}

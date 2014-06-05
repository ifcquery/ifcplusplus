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
#include "include/IfcDocumentInformation.h"
#include "include/IfcDocumentReference.h"
#include "include/IfcDocumentSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssociatesDocument.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssociatesDocument 
IfcRelAssociatesDocument::IfcRelAssociatesDocument() {}
IfcRelAssociatesDocument::IfcRelAssociatesDocument( int id ) { m_id = id; }
IfcRelAssociatesDocument::~IfcRelAssociatesDocument() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssociatesDocument::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssociatesDocument> other = dynamic_pointer_cast<IfcRelAssociatesDocument>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatingDocument = other->m_RelatingDocument;
}
void IfcRelAssociatesDocument::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSOCIATESDOCUMENT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_RelatedObjects, true );
	stream << ",";
	if( m_RelatingDocument ) { m_RelatingDocument->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssociatesDocument::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociatesDocument::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssociatesDocument, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelAssociatesDocument, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readSelectList( args[4], m_RelatedObjects, map );
	m_RelatingDocument = IfcDocumentSelect::createObjectFromStepData( args[5], map );
}
void IfcRelAssociatesDocument::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssociates::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingDocument", m_RelatingDocument ) );
}
void IfcRelAssociatesDocument::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
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
void IfcRelAssociatesDocument::unlinkSelf()
{
	IfcRelAssociates::unlinkSelf();
	shared_ptr<IfcDocumentInformation>  RelatingDocument_IfcDocumentInformation = dynamic_pointer_cast<IfcDocumentInformation>( m_RelatingDocument );
	if( RelatingDocument_IfcDocumentInformation )
	{
		std::vector<weak_ptr<IfcRelAssociatesDocument> >& DocumentInfoForObjects_inverse = RelatingDocument_IfcDocumentInformation->m_DocumentInfoForObjects_inverse;
		std::vector<weak_ptr<IfcRelAssociatesDocument> >::iterator it_DocumentInfoForObjects_inverse;
		for( it_DocumentInfoForObjects_inverse = DocumentInfoForObjects_inverse.begin(); it_DocumentInfoForObjects_inverse != DocumentInfoForObjects_inverse.end(); ++it_DocumentInfoForObjects_inverse)
		{
			shared_ptr<IfcRelAssociatesDocument> self_candidate( *it_DocumentInfoForObjects_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				DocumentInfoForObjects_inverse.erase( it_DocumentInfoForObjects_inverse );
				break;
			}
		}
	}
	shared_ptr<IfcDocumentReference>  RelatingDocument_IfcDocumentReference = dynamic_pointer_cast<IfcDocumentReference>( m_RelatingDocument );
	if( RelatingDocument_IfcDocumentReference )
	{
		std::vector<weak_ptr<IfcRelAssociatesDocument> >& DocumentRefForObjects_inverse = RelatingDocument_IfcDocumentReference->m_DocumentRefForObjects_inverse;
		std::vector<weak_ptr<IfcRelAssociatesDocument> >::iterator it_DocumentRefForObjects_inverse;
		for( it_DocumentRefForObjects_inverse = DocumentRefForObjects_inverse.begin(); it_DocumentRefForObjects_inverse != DocumentRefForObjects_inverse.end(); ++it_DocumentRefForObjects_inverse)
		{
			shared_ptr<IfcRelAssociatesDocument> self_candidate( *it_DocumentRefForObjects_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				DocumentRefForObjects_inverse.erase( it_DocumentRefForObjects_inverse );
				break;
			}
		}
	}
}

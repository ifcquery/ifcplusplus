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
#include "include/IfcActorSelect.h"
#include "include/IfcDate.h"
#include "include/IfcDateTime.h"
#include "include/IfcDocumentConfidentialityEnum.h"
#include "include/IfcDocumentInformation.h"
#include "include/IfcDocumentInformationRelationship.h"
#include "include/IfcDocumentReference.h"
#include "include/IfcDocumentStatusEnum.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcRelAssociatesDocument.h"
#include "include/IfcText.h"
#include "include/IfcURIReference.h"

// ENTITY IfcDocumentInformation 
IfcDocumentInformation::IfcDocumentInformation() {}
IfcDocumentInformation::IfcDocumentInformation( int id ) { m_id = id; }
IfcDocumentInformation::~IfcDocumentInformation() {}

// method setEntity takes over all attributes from another instance of the class
void IfcDocumentInformation::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcDocumentInformation> other = dynamic_pointer_cast<IfcDocumentInformation>(other_entity);
	if( !other) { return; }
	m_Identification = other->m_Identification;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Location = other->m_Location;
	m_Purpose = other->m_Purpose;
	m_IntendedUse = other->m_IntendedUse;
	m_Scope = other->m_Scope;
	m_Revision = other->m_Revision;
	m_DocumentOwner = other->m_DocumentOwner;
	m_Editors = other->m_Editors;
	m_CreationTime = other->m_CreationTime;
	m_LastRevisionTime = other->m_LastRevisionTime;
	m_ElectronicFormat = other->m_ElectronicFormat;
	m_ValidFrom = other->m_ValidFrom;
	m_ValidUntil = other->m_ValidUntil;
	m_Confidentiality = other->m_Confidentiality;
	m_Status = other->m_Status;
}
void IfcDocumentInformation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCDOCUMENTINFORMATION" << "(";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Purpose ) { m_Purpose->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_IntendedUse ) { m_IntendedUse->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Scope ) { m_Scope->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Revision ) { m_Revision->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DocumentOwner ) { m_DocumentOwner->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_Editors, true );
	stream << ",";
	if( m_CreationTime ) { m_CreationTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LastRevisionTime ) { m_LastRevisionTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ElectronicFormat ) { m_ElectronicFormat->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ValidFrom ) { m_ValidFrom->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ValidUntil ) { m_ValidUntil->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Confidentiality ) { m_Confidentiality->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Status ) { m_Status->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDocumentInformation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDocumentInformation::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<17 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDocumentInformation, expecting 17, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>17 ){ std::cout << "Wrong parameter count for entity IfcDocumentInformation, expecting 17, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Identification = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Name = IfcLabel::createObjectFromStepData( args[1] );
	m_Description = IfcText::createObjectFromStepData( args[2] );
	m_Location = IfcURIReference::createObjectFromStepData( args[3] );
	m_Purpose = IfcText::createObjectFromStepData( args[4] );
	m_IntendedUse = IfcText::createObjectFromStepData( args[5] );
	m_Scope = IfcText::createObjectFromStepData( args[6] );
	m_Revision = IfcLabel::createObjectFromStepData( args[7] );
	m_DocumentOwner = IfcActorSelect::createObjectFromStepData( args[8], map );
	readSelectList( args[9], m_Editors, map );
	m_CreationTime = IfcDateTime::createObjectFromStepData( args[10] );
	m_LastRevisionTime = IfcDateTime::createObjectFromStepData( args[11] );
	m_ElectronicFormat = IfcIdentifier::createObjectFromStepData( args[12] );
	m_ValidFrom = IfcDate::createObjectFromStepData( args[13] );
	m_ValidUntil = IfcDate::createObjectFromStepData( args[14] );
	m_Confidentiality = IfcDocumentConfidentialityEnum::createObjectFromStepData( args[15] );
	m_Status = IfcDocumentStatusEnum::createObjectFromStepData( args[16] );
}
void IfcDocumentInformation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExternalInformation::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Identification", m_Identification ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "Location", m_Location ) );
	vec_attributes.push_back( std::make_pair( "Purpose", m_Purpose ) );
	vec_attributes.push_back( std::make_pair( "IntendedUse", m_IntendedUse ) );
	vec_attributes.push_back( std::make_pair( "Scope", m_Scope ) );
	vec_attributes.push_back( std::make_pair( "Revision", m_Revision ) );
	vec_attributes.push_back( std::make_pair( "DocumentOwner", m_DocumentOwner ) );
	shared_ptr<IfcPPAttributeObjectVector> Editors_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Editors.begin(), m_Editors.end(), std::back_inserter( Editors_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Editors", Editors_vec_object ) );
	vec_attributes.push_back( std::make_pair( "CreationTime", m_CreationTime ) );
	vec_attributes.push_back( std::make_pair( "LastRevisionTime", m_LastRevisionTime ) );
	vec_attributes.push_back( std::make_pair( "ElectronicFormat", m_ElectronicFormat ) );
	vec_attributes.push_back( std::make_pair( "ValidFrom", m_ValidFrom ) );
	vec_attributes.push_back( std::make_pair( "ValidUntil", m_ValidUntil ) );
	vec_attributes.push_back( std::make_pair( "Confidentiality", m_Confidentiality ) );
	vec_attributes.push_back( std::make_pair( "Status", m_Status ) );
}
void IfcDocumentInformation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> DocumentInfoForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_DocumentInfoForObjects_inverse.size(); ++i ) { DocumentInfoForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesDocument>( m_DocumentInfoForObjects_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "DocumentInfoForObjects_inverse", DocumentInfoForObjects_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasDocumentReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasDocumentReferences_inverse.size(); ++i ) { HasDocumentReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcDocumentReference>( m_HasDocumentReferences_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasDocumentReferences_inverse", HasDocumentReferences_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsPointedTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsPointedTo_inverse.size(); ++i ) { IsPointedTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcDocumentInformationRelationship>( m_IsPointedTo_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsPointedTo_inverse", IsPointedTo_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsPointer_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsPointer_inverse.size(); ++i ) { IsPointer_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcDocumentInformationRelationship>( m_IsPointer_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsPointer_inverse", IsPointer_inverse_vec_obj ) );
}
void IfcDocumentInformation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalInformation::setInverseCounterparts( ptr_self_entity );
}
void IfcDocumentInformation::unlinkSelf()
{
	IfcExternalInformation::unlinkSelf();
}

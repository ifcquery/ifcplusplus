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
shared_ptr<IfcPPObject> IfcDocumentInformation::getDeepCopy()
{
	shared_ptr<IfcDocumentInformation> copy_self( new IfcDocumentInformation() );
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_Location ) { copy_self->m_Location = dynamic_pointer_cast<IfcURIReference>( m_Location->getDeepCopy() ); }
	if( m_Purpose ) { copy_self->m_Purpose = dynamic_pointer_cast<IfcText>( m_Purpose->getDeepCopy() ); }
	if( m_IntendedUse ) { copy_self->m_IntendedUse = dynamic_pointer_cast<IfcText>( m_IntendedUse->getDeepCopy() ); }
	if( m_Scope ) { copy_self->m_Scope = dynamic_pointer_cast<IfcText>( m_Scope->getDeepCopy() ); }
	if( m_Revision ) { copy_self->m_Revision = dynamic_pointer_cast<IfcLabel>( m_Revision->getDeepCopy() ); }
	if( m_DocumentOwner ) { copy_self->m_DocumentOwner = dynamic_pointer_cast<IfcActorSelect>( m_DocumentOwner->getDeepCopy() ); }
	for( size_t ii=0; ii<m_Editors.size(); ++ii )
	{
		auto item_ii = m_Editors[ii];
		if( item_ii )
		{
			copy_self->m_Editors.push_back( dynamic_pointer_cast<IfcActorSelect>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_CreationTime ) { copy_self->m_CreationTime = dynamic_pointer_cast<IfcDateTime>( m_CreationTime->getDeepCopy() ); }
	if( m_LastRevisionTime ) { copy_self->m_LastRevisionTime = dynamic_pointer_cast<IfcDateTime>( m_LastRevisionTime->getDeepCopy() ); }
	if( m_ElectronicFormat ) { copy_self->m_ElectronicFormat = dynamic_pointer_cast<IfcIdentifier>( m_ElectronicFormat->getDeepCopy() ); }
	if( m_ValidFrom ) { copy_self->m_ValidFrom = dynamic_pointer_cast<IfcDate>( m_ValidFrom->getDeepCopy() ); }
	if( m_ValidUntil ) { copy_self->m_ValidUntil = dynamic_pointer_cast<IfcDate>( m_ValidUntil->getDeepCopy() ); }
	if( m_Confidentiality ) { copy_self->m_Confidentiality = dynamic_pointer_cast<IfcDocumentConfidentialityEnum>( m_Confidentiality->getDeepCopy() ); }
	if( m_Status ) { copy_self->m_Status = dynamic_pointer_cast<IfcDocumentStatusEnum>( m_Status->getDeepCopy() ); }
	return copy_self;
}
void IfcDocumentInformation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDOCUMENTINFORMATION" << "(";
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
	if( m_DocumentOwner ) { m_DocumentOwner->getStepParameter( stream, true ); } else { stream << "$" ; }
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
void IfcDocumentInformation::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( m_Editors.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Editors_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Editors.begin(), m_Editors.end(), std::back_inserter( Editors_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Editors", Editors_vec_object ) );
	}
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
	IfcExternalInformation::getAttributesInverse( vec_attributes_inverse );
	if( m_DocumentInfoForObjects_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DocumentInfoForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_DocumentInfoForObjects_inverse.size(); ++i )
		{
			if( !m_DocumentInfoForObjects_inverse[i].expired() )
			{
				DocumentInfoForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesDocument>( m_DocumentInfoForObjects_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "DocumentInfoForObjects_inverse", DocumentInfoForObjects_inverse_vec_obj ) );
	}
	if( m_HasDocumentReferences_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasDocumentReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasDocumentReferences_inverse.size(); ++i )
		{
			if( !m_HasDocumentReferences_inverse[i].expired() )
			{
				HasDocumentReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcDocumentReference>( m_HasDocumentReferences_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasDocumentReferences_inverse", HasDocumentReferences_inverse_vec_obj ) );
	}
	if( m_IsPointedTo_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> IsPointedTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_IsPointedTo_inverse.size(); ++i )
		{
			if( !m_IsPointedTo_inverse[i].expired() )
			{
				IsPointedTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcDocumentInformationRelationship>( m_IsPointedTo_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "IsPointedTo_inverse", IsPointedTo_inverse_vec_obj ) );
	}
	if( m_IsPointer_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> IsPointer_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_IsPointer_inverse.size(); ++i )
		{
			if( !m_IsPointer_inverse[i].expired() )
			{
				IsPointer_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcDocumentInformationRelationship>( m_IsPointer_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "IsPointer_inverse", IsPointer_inverse_vec_obj ) );
	}
}
void IfcDocumentInformation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalInformation::setInverseCounterparts( ptr_self_entity );
}
void IfcDocumentInformation::unlinkSelf()
{
	IfcExternalInformation::unlinkSelf();
}

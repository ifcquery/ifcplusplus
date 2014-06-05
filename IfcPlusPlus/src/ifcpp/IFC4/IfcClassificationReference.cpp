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
#include "include/IfcClassification.h"
#include "include/IfcClassificationReference.h"
#include "include/IfcClassificationReferenceSelect.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcRelAssociatesClassification.h"
#include "include/IfcText.h"
#include "include/IfcURIReference.h"

// ENTITY IfcClassificationReference 
IfcClassificationReference::IfcClassificationReference() {}
IfcClassificationReference::IfcClassificationReference( int id ) { m_id = id; }
IfcClassificationReference::~IfcClassificationReference() {}

// method setEntity takes over all attributes from another instance of the class
void IfcClassificationReference::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcClassificationReference> other = dynamic_pointer_cast<IfcClassificationReference>(other_entity);
	if( !other) { return; }
	m_Location = other->m_Location;
	m_Identification = other->m_Identification;
	m_Name = other->m_Name;
	m_ReferencedSource = other->m_ReferencedSource;
	m_Description = other->m_Description;
	m_Sort = other->m_Sort;
}
void IfcClassificationReference::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCLASSIFICATIONREFERENCE" << "(";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReferencedSource ) { m_ReferencedSource->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Sort ) { m_Sort->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcClassificationReference::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcClassificationReference::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcClassificationReference, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcClassificationReference, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Location = IfcURIReference::createObjectFromStepData( args[0] );
	m_Identification = IfcIdentifier::createObjectFromStepData( args[1] );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_ReferencedSource = IfcClassificationReferenceSelect::createObjectFromStepData( args[3], map );
	m_Description = IfcText::createObjectFromStepData( args[4] );
	m_Sort = IfcIdentifier::createObjectFromStepData( args[5] );
}
void IfcClassificationReference::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExternalReference::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ReferencedSource", m_ReferencedSource ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "Sort", m_Sort ) );
}
void IfcClassificationReference::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> ClassificationRefForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ClassificationRefForObjects_inverse.size(); ++i ) { ClassificationRefForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesClassification>( m_ClassificationRefForObjects_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ClassificationRefForObjects_inverse", ClassificationRefForObjects_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasReferences_inverse.size(); ++i ) { HasReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcClassificationReference>( m_HasReferences_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasReferences_inverse", HasReferences_inverse_vec_obj ) );
}
void IfcClassificationReference::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalReference::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcClassificationReference> ptr_self = dynamic_pointer_cast<IfcClassificationReference>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcClassificationReference::setInverseCounterparts: type mismatch" ); }
	shared_ptr<IfcClassification>  ReferencedSource_IfcClassification = dynamic_pointer_cast<IfcClassification>( m_ReferencedSource );
	if( ReferencedSource_IfcClassification )
	{
		ReferencedSource_IfcClassification->m_HasReferences_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcClassificationReference>  ReferencedSource_IfcClassificationReference = dynamic_pointer_cast<IfcClassificationReference>( m_ReferencedSource );
	if( ReferencedSource_IfcClassificationReference )
	{
		ReferencedSource_IfcClassificationReference->m_HasReferences_inverse.push_back( ptr_self );
	}
}
void IfcClassificationReference::unlinkSelf()
{
	IfcExternalReference::unlinkSelf();
	shared_ptr<IfcClassification>  ReferencedSource_IfcClassification = dynamic_pointer_cast<IfcClassification>( m_ReferencedSource );
	if( ReferencedSource_IfcClassification )
	{
		std::vector<weak_ptr<IfcClassificationReference> >& HasReferences_inverse = ReferencedSource_IfcClassification->m_HasReferences_inverse;
		std::vector<weak_ptr<IfcClassificationReference> >::iterator it_HasReferences_inverse;
		for( it_HasReferences_inverse = HasReferences_inverse.begin(); it_HasReferences_inverse != HasReferences_inverse.end(); ++it_HasReferences_inverse)
		{
			shared_ptr<IfcClassificationReference> self_candidate( *it_HasReferences_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasReferences_inverse.erase( it_HasReferences_inverse );
				break;
			}
		}
	}
	shared_ptr<IfcClassificationReference>  ReferencedSource_IfcClassificationReference = dynamic_pointer_cast<IfcClassificationReference>( m_ReferencedSource );
	if( ReferencedSource_IfcClassificationReference )
	{
		std::vector<weak_ptr<IfcClassificationReference> >& HasReferences_inverse = ReferencedSource_IfcClassificationReference->m_HasReferences_inverse;
		std::vector<weak_ptr<IfcClassificationReference> >::iterator it_HasReferences_inverse;
		for( it_HasReferences_inverse = HasReferences_inverse.begin(); it_HasReferences_inverse != HasReferences_inverse.end(); ++it_HasReferences_inverse)
		{
			shared_ptr<IfcClassificationReference> self_candidate( *it_HasReferences_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasReferences_inverse.erase( it_HasReferences_inverse );
				break;
			}
		}
	}
}

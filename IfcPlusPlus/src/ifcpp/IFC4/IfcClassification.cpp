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
#include "include/IfcDate.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcRelAssociatesClassification.h"
#include "include/IfcText.h"
#include "include/IfcURIReference.h"

// ENTITY IfcClassification 
IfcClassification::IfcClassification() {}
IfcClassification::IfcClassification( int id ) { m_id = id; }
IfcClassification::~IfcClassification() {}

// method setEntity takes over all attributes from another instance of the class
void IfcClassification::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcClassification> other = dynamic_pointer_cast<IfcClassification>(other_entity);
	if( !other) { return; }
	m_Source = other->m_Source;
	m_Edition = other->m_Edition;
	m_EditionDate = other->m_EditionDate;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Location = other->m_Location;
	m_ReferenceTokens = other->m_ReferenceTokens;
}
void IfcClassification::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCLASSIFICATION" << "(";
	if( m_Source ) { m_Source->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Edition ) { m_Edition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EditionDate ) { m_EditionDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_ReferenceTokens );
	stream << ");";
}
void IfcClassification::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcClassification::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcClassification, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcClassification, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Source = IfcLabel::createObjectFromStepData( args[0] );
	m_Edition = IfcLabel::createObjectFromStepData( args[1] );
	m_EditionDate = IfcDate::createObjectFromStepData( args[2] );
	m_Name = IfcLabel::createObjectFromStepData( args[3] );
	m_Description = IfcText::createObjectFromStepData( args[4] );
	m_Location = IfcURIReference::createObjectFromStepData( args[5] );
	readTypeList( args[6], m_ReferenceTokens );
}
void IfcClassification::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExternalInformation::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Source", m_Source ) );
	vec_attributes.push_back( std::make_pair( "Edition", m_Edition ) );
	vec_attributes.push_back( std::make_pair( "EditionDate", m_EditionDate ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "Location", m_Location ) );
	shared_ptr<IfcPPAttributeObjectVector> ReferenceTokens_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_ReferenceTokens.begin(), m_ReferenceTokens.end(), std::back_inserter( ReferenceTokens_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "ReferenceTokens", ReferenceTokens_vec_object ) );
}
void IfcClassification::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> ClassificationForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ClassificationForObjects_inverse.size(); ++i ) { ClassificationForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesClassification>( m_ClassificationForObjects_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ClassificationForObjects_inverse", ClassificationForObjects_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasReferences_inverse.size(); ++i ) { HasReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcClassificationReference>( m_HasReferences_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasReferences_inverse", HasReferences_inverse_vec_obj ) );
}
void IfcClassification::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalInformation::setInverseCounterparts( ptr_self_entity );
}
void IfcClassification::unlinkSelf()
{
	IfcExternalInformation::unlinkSelf();
}

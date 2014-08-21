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
shared_ptr<IfcPPObject> IfcClassification::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcClassification> copy_self( new IfcClassification() );
	if( m_Source ) { copy_self->m_Source = dynamic_pointer_cast<IfcLabel>( m_Source->getDeepCopy(options) ); }
	if( m_Edition ) { copy_self->m_Edition = dynamic_pointer_cast<IfcLabel>( m_Edition->getDeepCopy(options) ); }
	if( m_EditionDate ) { copy_self->m_EditionDate = dynamic_pointer_cast<IfcDate>( m_EditionDate->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Location ) { copy_self->m_Location = dynamic_pointer_cast<IfcURIReference>( m_Location->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ReferenceTokens.size(); ++ii )
	{
		auto item_ii = m_ReferenceTokens[ii];
		if( item_ii )
		{
			copy_self->m_ReferenceTokens.push_back( dynamic_pointer_cast<IfcIdentifier>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcClassification::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCLASSIFICATION" << "(";
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
void IfcClassification::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcClassification, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Source = IfcLabel::createObjectFromSTEP( args[0] );
	m_Edition = IfcLabel::createObjectFromSTEP( args[1] );
	m_EditionDate = IfcDate::createObjectFromSTEP( args[2] );
	m_Name = IfcLabel::createObjectFromSTEP( args[3] );
	m_Description = IfcText::createObjectFromSTEP( args[4] );
	m_Location = IfcURIReference::createObjectFromSTEP( args[5] );
	readSelectList( args[6], m_ReferenceTokens, map );
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
	if( m_ReferenceTokens.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ReferenceTokens_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ReferenceTokens.begin(), m_ReferenceTokens.end(), std::back_inserter( ReferenceTokens_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ReferenceTokens", ReferenceTokens_vec_object ) );
	}
}
void IfcClassification::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcExternalInformation::getAttributesInverse( vec_attributes_inverse );
	if( m_ClassificationForObjects_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ClassificationForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_ClassificationForObjects_inverse.size(); ++i )
		{
			if( !m_ClassificationForObjects_inverse[i].expired() )
			{
				ClassificationForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesClassification>( m_ClassificationForObjects_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "ClassificationForObjects_inverse", ClassificationForObjects_inverse_vec_obj ) );
	}
	if( m_HasReferences_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasReferences_inverse.size(); ++i )
		{
			if( !m_HasReferences_inverse[i].expired() )
			{
				HasReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcClassificationReference>( m_HasReferences_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasReferences_inverse", HasReferences_inverse_vec_obj ) );
	}
}
void IfcClassification::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalInformation::setInverseCounterparts( ptr_self_entity );
}
void IfcClassification::unlinkSelf()
{
	IfcExternalInformation::unlinkSelf();
}

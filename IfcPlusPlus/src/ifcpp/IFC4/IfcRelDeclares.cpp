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
#include "include/IfcContext.h"
#include "include/IfcDefinitionSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertyDefinition.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcText.h"

// ENTITY IfcRelDeclares 
IfcRelDeclares::IfcRelDeclares() {}
IfcRelDeclares::IfcRelDeclares( int id ) { m_id = id; }
IfcRelDeclares::~IfcRelDeclares() {}
shared_ptr<IfcPPObject> IfcRelDeclares::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelDeclares> copy_self( new IfcRelDeclares() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( CreateCompressedGuidString22() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_RelatingContext ) { copy_self->m_RelatingContext = dynamic_pointer_cast<IfcContext>( m_RelatingContext->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedDefinitions.size(); ++ii )
	{
		auto item_ii = m_RelatedDefinitions[ii];
		if( item_ii )
		{
			copy_self->m_RelatedDefinitions.push_back( dynamic_pointer_cast<IfcDefinitionSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcRelDeclares::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELDECLARES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingContext ) { stream << "#" << m_RelatingContext->m_id; } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_RelatedDefinitions, true );
	stream << ");";
}
void IfcRelDeclares::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelDeclares::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelDeclares, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReference( args[4], m_RelatingContext, map );
	readSelectList( args[5], m_RelatedDefinitions, map );
}
void IfcRelDeclares::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelationship::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingContext", m_RelatingContext ) );
	if( m_RelatedDefinitions.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedDefinitions_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedDefinitions.begin(), m_RelatedDefinitions.end(), std::back_inserter( RelatedDefinitions_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedDefinitions", RelatedDefinitions_vec_object ) );
	}
}
void IfcRelDeclares::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelationship::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelDeclares::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelDeclares> ptr_self = dynamic_pointer_cast<IfcRelDeclares>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelDeclares::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedDefinitions.size(); ++i )
	{
		shared_ptr<IfcObjectDefinition>  RelatedDefinitions_IfcObjectDefinition = dynamic_pointer_cast<IfcObjectDefinition>( m_RelatedDefinitions[i] );
		if( RelatedDefinitions_IfcObjectDefinition )
		{
			RelatedDefinitions_IfcObjectDefinition->m_HasContext_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcPropertyDefinition>  RelatedDefinitions_IfcPropertyDefinition = dynamic_pointer_cast<IfcPropertyDefinition>( m_RelatedDefinitions[i] );
		if( RelatedDefinitions_IfcPropertyDefinition )
		{
			RelatedDefinitions_IfcPropertyDefinition->m_HasContext_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingContext )
	{
		m_RelatingContext->m_Declares_inverse.push_back( ptr_self );
	}
}
void IfcRelDeclares::unlinkSelf()
{
	IfcRelationship::unlinkSelf();
	for( size_t i=0; i<m_RelatedDefinitions.size(); ++i )
	{
		shared_ptr<IfcObjectDefinition>  RelatedDefinitions_IfcObjectDefinition = dynamic_pointer_cast<IfcObjectDefinition>( m_RelatedDefinitions[i] );
		if( RelatedDefinitions_IfcObjectDefinition )
		{
			std::vector<weak_ptr<IfcRelDeclares> >& HasContext_inverse = RelatedDefinitions_IfcObjectDefinition->m_HasContext_inverse;
			for( auto it_HasContext_inverse = HasContext_inverse.begin(); it_HasContext_inverse != HasContext_inverse.end(); ++it_HasContext_inverse)
			{
				shared_ptr<IfcRelDeclares> self_candidate( *it_HasContext_inverse );
				if( self_candidate.get() == this )
				{
					HasContext_inverse.erase( it_HasContext_inverse );
					break;
				}
			}
		}
		shared_ptr<IfcPropertyDefinition>  RelatedDefinitions_IfcPropertyDefinition = dynamic_pointer_cast<IfcPropertyDefinition>( m_RelatedDefinitions[i] );
		if( RelatedDefinitions_IfcPropertyDefinition )
		{
			std::vector<weak_ptr<IfcRelDeclares> >& HasContext_inverse = RelatedDefinitions_IfcPropertyDefinition->m_HasContext_inverse;
			for( auto it_HasContext_inverse = HasContext_inverse.begin(); it_HasContext_inverse != HasContext_inverse.end(); ++it_HasContext_inverse)
			{
				shared_ptr<IfcRelDeclares> self_candidate( *it_HasContext_inverse );
				if( self_candidate.get() == this )
				{
					HasContext_inverse.erase( it_HasContext_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingContext )
	{
		std::vector<weak_ptr<IfcRelDeclares> >& Declares_inverse = m_RelatingContext->m_Declares_inverse;
		for( auto it_Declares_inverse = Declares_inverse.begin(); it_Declares_inverse != Declares_inverse.end(); ++it_Declares_inverse)
		{
			shared_ptr<IfcRelDeclares> self_candidate( *it_Declares_inverse );
			if( self_candidate.get() == this )
			{
				Declares_inverse.erase( it_Declares_inverse );
				break;
			}
		}
	}
}

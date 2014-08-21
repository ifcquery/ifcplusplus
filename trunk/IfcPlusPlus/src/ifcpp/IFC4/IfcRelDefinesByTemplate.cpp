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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcPropertySetTemplate.h"
#include "include/IfcRelDefinesByTemplate.h"
#include "include/IfcText.h"

// ENTITY IfcRelDefinesByTemplate 
IfcRelDefinesByTemplate::IfcRelDefinesByTemplate() {}
IfcRelDefinesByTemplate::IfcRelDefinesByTemplate( int id ) { m_id = id; }
IfcRelDefinesByTemplate::~IfcRelDefinesByTemplate() {}
shared_ptr<IfcPPObject> IfcRelDefinesByTemplate::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelDefinesByTemplate> copy_self( new IfcRelDefinesByTemplate() );
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
	for( size_t ii=0; ii<m_RelatedPropertySets.size(); ++ii )
	{
		auto item_ii = m_RelatedPropertySets[ii];
		if( item_ii )
		{
			copy_self->m_RelatedPropertySets.push_back( dynamic_pointer_cast<IfcPropertySetDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatingTemplate ) { copy_self->m_RelatingTemplate = dynamic_pointer_cast<IfcPropertySetTemplate>( m_RelatingTemplate->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelDefinesByTemplate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELDEFINESBYTEMPLATE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_RelatedPropertySets );
	stream << ",";
	if( m_RelatingTemplate ) { stream << "#" << m_RelatingTemplate->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcRelDefinesByTemplate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelDefinesByTemplate::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelDefinesByTemplate, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReferenceList( args[4], m_RelatedPropertySets, map );
	readEntityReference( args[5], m_RelatingTemplate, map );
}
void IfcRelDefinesByTemplate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelDefines::getAttributes( vec_attributes );
	if( m_RelatedPropertySets.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedPropertySets_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedPropertySets.begin(), m_RelatedPropertySets.end(), std::back_inserter( RelatedPropertySets_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedPropertySets", RelatedPropertySets_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "RelatingTemplate", m_RelatingTemplate ) );
}
void IfcRelDefinesByTemplate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelDefines::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelDefinesByTemplate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelDefines::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelDefinesByTemplate> ptr_self = dynamic_pointer_cast<IfcRelDefinesByTemplate>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelDefinesByTemplate::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedPropertySets.size(); ++i )
	{
		if( m_RelatedPropertySets[i] )
		{
			m_RelatedPropertySets[i]->m_IsDefinedBy_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingTemplate )
	{
		m_RelatingTemplate->m_Defines_inverse.push_back( ptr_self );
	}
}
void IfcRelDefinesByTemplate::unlinkSelf()
{
	IfcRelDefines::unlinkSelf();
	for( size_t i=0; i<m_RelatedPropertySets.size(); ++i )
	{
		if( m_RelatedPropertySets[i] )
		{
			std::vector<weak_ptr<IfcRelDefinesByTemplate> >& IsDefinedBy_inverse = m_RelatedPropertySets[i]->m_IsDefinedBy_inverse;
			for( auto it_IsDefinedBy_inverse = IsDefinedBy_inverse.begin(); it_IsDefinedBy_inverse != IsDefinedBy_inverse.end(); ++it_IsDefinedBy_inverse)
			{
				shared_ptr<IfcRelDefinesByTemplate> self_candidate( *it_IsDefinedBy_inverse );
				if( self_candidate.get() == this )
				{
					IsDefinedBy_inverse.erase( it_IsDefinedBy_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingTemplate )
	{
		std::vector<weak_ptr<IfcRelDefinesByTemplate> >& Defines_inverse = m_RelatingTemplate->m_Defines_inverse;
		for( auto it_Defines_inverse = Defines_inverse.begin(); it_Defines_inverse != Defines_inverse.end(); ++it_Defines_inverse)
		{
			shared_ptr<IfcRelDefinesByTemplate> self_candidate( *it_Defines_inverse );
			if( self_candidate.get() == this )
			{
				Defines_inverse.erase( it_Defines_inverse );
				break;
			}
		}
	}
}

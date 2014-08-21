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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObject.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcPropertySetDefinitionSelect.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcText.h"

// ENTITY IfcRelDefinesByProperties 
IfcRelDefinesByProperties::IfcRelDefinesByProperties() {}
IfcRelDefinesByProperties::IfcRelDefinesByProperties( int id ) { m_id = id; }
IfcRelDefinesByProperties::~IfcRelDefinesByProperties() {}
shared_ptr<IfcPPObject> IfcRelDefinesByProperties::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelDefinesByProperties> copy_self( new IfcRelDefinesByProperties() );
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
	for( size_t ii=0; ii<m_RelatedObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedObjects.push_back( dynamic_pointer_cast<IfcObjectDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatingPropertyDefinition ) { copy_self->m_RelatingPropertyDefinition = dynamic_pointer_cast<IfcPropertySetDefinitionSelect>( m_RelatingPropertyDefinition->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelDefinesByProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELDEFINESBYPROPERTIES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_RelatedObjects );
	stream << ",";
	if( m_RelatingPropertyDefinition ) { m_RelatingPropertyDefinition->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcRelDefinesByProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelDefinesByProperties::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelDefinesByProperties, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatingPropertyDefinition = IfcPropertySetDefinitionSelect::createObjectFromSTEP( args[5], map );
}
void IfcRelDefinesByProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelDefines::getAttributes( vec_attributes );
	if( m_RelatedObjects.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedObjects_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedObjects.begin(), m_RelatedObjects.end(), std::back_inserter( RelatedObjects_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedObjects", RelatedObjects_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "RelatingPropertyDefinition", m_RelatingPropertyDefinition ) );
}
void IfcRelDefinesByProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelDefines::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelDefinesByProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelDefines::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelDefinesByProperties> ptr_self = dynamic_pointer_cast<IfcRelDefinesByProperties>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelDefinesByProperties::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcContext>  RelatedObjects_IfcContext = dynamic_pointer_cast<IfcContext>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcContext )
		{
			RelatedObjects_IfcContext->m_IsDefinedBy_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcObject>  RelatedObjects_IfcObject = dynamic_pointer_cast<IfcObject>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObject )
		{
			RelatedObjects_IfcObject->m_IsDefinedBy_inverse.push_back( ptr_self );
		}
	}
	shared_ptr<IfcPropertySetDefinition>  RelatingPropertyDefinition_IfcPropertySetDefinition = dynamic_pointer_cast<IfcPropertySetDefinition>( m_RelatingPropertyDefinition );
	if( RelatingPropertyDefinition_IfcPropertySetDefinition )
	{
		RelatingPropertyDefinition_IfcPropertySetDefinition->m_DefinesOccurrence_inverse.push_back( ptr_self );
	}
}
void IfcRelDefinesByProperties::unlinkSelf()
{
	IfcRelDefines::unlinkSelf();
	for( size_t i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcContext>  RelatedObjects_IfcContext = dynamic_pointer_cast<IfcContext>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcContext )
		{
			std::vector<weak_ptr<IfcRelDefinesByProperties> >& IsDefinedBy_inverse = RelatedObjects_IfcContext->m_IsDefinedBy_inverse;
			for( auto it_IsDefinedBy_inverse = IsDefinedBy_inverse.begin(); it_IsDefinedBy_inverse != IsDefinedBy_inverse.end(); ++it_IsDefinedBy_inverse)
			{
				shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_IsDefinedBy_inverse );
				if( self_candidate.get() == this )
				{
					IsDefinedBy_inverse.erase( it_IsDefinedBy_inverse );
					break;
				}
			}
		}
		shared_ptr<IfcObject>  RelatedObjects_IfcObject = dynamic_pointer_cast<IfcObject>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObject )
		{
			std::vector<weak_ptr<IfcRelDefinesByProperties> >& IsDefinedBy_inverse = RelatedObjects_IfcObject->m_IsDefinedBy_inverse;
			for( auto it_IsDefinedBy_inverse = IsDefinedBy_inverse.begin(); it_IsDefinedBy_inverse != IsDefinedBy_inverse.end(); ++it_IsDefinedBy_inverse)
			{
				shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_IsDefinedBy_inverse );
				if( self_candidate.get() == this )
				{
					IsDefinedBy_inverse.erase( it_IsDefinedBy_inverse );
					break;
				}
			}
		}
	}
	shared_ptr<IfcPropertySetDefinition>  RelatingPropertyDefinition_IfcPropertySetDefinition = dynamic_pointer_cast<IfcPropertySetDefinition>( m_RelatingPropertyDefinition );
	if( RelatingPropertyDefinition_IfcPropertySetDefinition )
	{
		std::vector<weak_ptr<IfcRelDefinesByProperties> >& DefinesOccurrence_inverse = RelatingPropertyDefinition_IfcPropertySetDefinition->m_DefinesOccurrence_inverse;
		for( auto it_DefinesOccurrence_inverse = DefinesOccurrence_inverse.begin(); it_DefinesOccurrence_inverse != DefinesOccurrence_inverse.end(); ++it_DefinesOccurrence_inverse)
		{
			shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_DefinesOccurrence_inverse );
			if( self_candidate.get() == this )
			{
				DefinesOccurrence_inverse.erase( it_DefinesOccurrence_inverse );
				break;
			}
		}
	}
}

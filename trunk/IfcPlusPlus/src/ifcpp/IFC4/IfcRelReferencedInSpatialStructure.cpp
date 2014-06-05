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
#include "include/IfcElement.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProduct.h"
#include "include/IfcRelReferencedInSpatialStructure.h"
#include "include/IfcSpatialElement.h"
#include "include/IfcText.h"

// ENTITY IfcRelReferencedInSpatialStructure 
IfcRelReferencedInSpatialStructure::IfcRelReferencedInSpatialStructure() {}
IfcRelReferencedInSpatialStructure::IfcRelReferencedInSpatialStructure( int id ) { m_id = id; }
IfcRelReferencedInSpatialStructure::~IfcRelReferencedInSpatialStructure() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelReferencedInSpatialStructure::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelReferencedInSpatialStructure> other = dynamic_pointer_cast<IfcRelReferencedInSpatialStructure>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedElements = other->m_RelatedElements;
	m_RelatingStructure = other->m_RelatingStructure;
}
void IfcRelReferencedInSpatialStructure::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELREFERENCEDINSPATIALSTRUCTURE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedElements );
	stream << ",";
	if( m_RelatingStructure ) { stream << "#" << m_RelatingStructure->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelReferencedInSpatialStructure::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelReferencedInSpatialStructure::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelReferencedInSpatialStructure, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelReferencedInSpatialStructure, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_RelatedElements, map );
	readEntityReference( args[5], m_RelatingStructure, map );
}
void IfcRelReferencedInSpatialStructure::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingStructure", m_RelatingStructure ) );
}
void IfcRelReferencedInSpatialStructure::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelReferencedInSpatialStructure::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelReferencedInSpatialStructure> ptr_self = dynamic_pointer_cast<IfcRelReferencedInSpatialStructure>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelReferencedInSpatialStructure::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedElements.size(); ++i )
	{
		shared_ptr<IfcElement>  RelatedElements_IfcElement = dynamic_pointer_cast<IfcElement>( m_RelatedElements[i] );
		if( RelatedElements_IfcElement )
		{
			RelatedElements_IfcElement->m_ReferencedInStructures_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingStructure )
	{
		m_RelatingStructure->m_ReferencesElements_inverse.push_back( ptr_self );
	}
}
void IfcRelReferencedInSpatialStructure::unlinkSelf()
{
	IfcRelConnects::unlinkSelf();
	for( int i=0; i<m_RelatedElements.size(); ++i )
	{
		shared_ptr<IfcElement>  RelatedElements_IfcElement = dynamic_pointer_cast<IfcElement>( m_RelatedElements[i] );
		if( RelatedElements_IfcElement )
		{
			std::vector<weak_ptr<IfcRelReferencedInSpatialStructure> >& ReferencedInStructures_inverse = RelatedElements_IfcElement->m_ReferencedInStructures_inverse;
			std::vector<weak_ptr<IfcRelReferencedInSpatialStructure> >::iterator it_ReferencedInStructures_inverse;
			for( it_ReferencedInStructures_inverse = ReferencedInStructures_inverse.begin(); it_ReferencedInStructures_inverse != ReferencedInStructures_inverse.end(); ++it_ReferencedInStructures_inverse)
			{
				shared_ptr<IfcRelReferencedInSpatialStructure> self_candidate( *it_ReferencedInStructures_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					ReferencedInStructures_inverse.erase( it_ReferencedInStructures_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingStructure )
	{
		std::vector<weak_ptr<IfcRelReferencedInSpatialStructure> >& ReferencesElements_inverse = m_RelatingStructure->m_ReferencesElements_inverse;
		std::vector<weak_ptr<IfcRelReferencedInSpatialStructure> >::iterator it_ReferencesElements_inverse;
		for( it_ReferencesElements_inverse = ReferencesElements_inverse.begin(); it_ReferencesElements_inverse != ReferencesElements_inverse.end(); ++it_ReferencesElements_inverse)
		{
			shared_ptr<IfcRelReferencedInSpatialStructure> self_candidate( *it_ReferencesElements_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ReferencesElements_inverse.erase( it_ReferencesElements_inverse );
				break;
			}
		}
	}
}

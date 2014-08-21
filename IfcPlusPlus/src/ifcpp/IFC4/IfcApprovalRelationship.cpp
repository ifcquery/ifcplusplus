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
#include "include/IfcApproval.h"
#include "include/IfcApprovalRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcText.h"

// ENTITY IfcApprovalRelationship 
IfcApprovalRelationship::IfcApprovalRelationship() {}
IfcApprovalRelationship::IfcApprovalRelationship( int id ) { m_id = id; }
IfcApprovalRelationship::~IfcApprovalRelationship() {}
shared_ptr<IfcPPObject> IfcApprovalRelationship::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcApprovalRelationship> copy_self( new IfcApprovalRelationship() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_RelatingApproval ) { copy_self->m_RelatingApproval = dynamic_pointer_cast<IfcApproval>( m_RelatingApproval->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedApprovals.size(); ++ii )
	{
		auto item_ii = m_RelatedApprovals[ii];
		if( item_ii )
		{
			copy_self->m_RelatedApprovals.push_back( dynamic_pointer_cast<IfcApproval>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcApprovalRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCAPPROVALRELATIONSHIP" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingApproval ) { stream << "#" << m_RelatingApproval->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedApprovals );
	stream << ");";
}
void IfcApprovalRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcApprovalRelationship::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcApprovalRelationship, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_RelatingApproval, map );
	readEntityReferenceList( args[3], m_RelatedApprovals, map );
}
void IfcApprovalRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcResourceLevelRelationship::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingApproval", m_RelatingApproval ) );
	if( m_RelatedApprovals.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedApprovals_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedApprovals.begin(), m_RelatedApprovals.end(), std::back_inserter( RelatedApprovals_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedApprovals", RelatedApprovals_vec_object ) );
	}
}
void IfcApprovalRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcResourceLevelRelationship::getAttributesInverse( vec_attributes_inverse );
}
void IfcApprovalRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcResourceLevelRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcApprovalRelationship> ptr_self = dynamic_pointer_cast<IfcApprovalRelationship>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcApprovalRelationship::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedApprovals.size(); ++i )
	{
		if( m_RelatedApprovals[i] )
		{
			m_RelatedApprovals[i]->m_IsRelatedWith_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingApproval )
	{
		m_RelatingApproval->m_Relates_inverse.push_back( ptr_self );
	}
}
void IfcApprovalRelationship::unlinkSelf()
{
	IfcResourceLevelRelationship::unlinkSelf();
	for( size_t i=0; i<m_RelatedApprovals.size(); ++i )
	{
		if( m_RelatedApprovals[i] )
		{
			std::vector<weak_ptr<IfcApprovalRelationship> >& IsRelatedWith_inverse = m_RelatedApprovals[i]->m_IsRelatedWith_inverse;
			for( auto it_IsRelatedWith_inverse = IsRelatedWith_inverse.begin(); it_IsRelatedWith_inverse != IsRelatedWith_inverse.end(); ++it_IsRelatedWith_inverse)
			{
				shared_ptr<IfcApprovalRelationship> self_candidate( *it_IsRelatedWith_inverse );
				if( self_candidate.get() == this )
				{
					IsRelatedWith_inverse.erase( it_IsRelatedWith_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingApproval )
	{
		std::vector<weak_ptr<IfcApprovalRelationship> >& Relates_inverse = m_RelatingApproval->m_Relates_inverse;
		for( auto it_Relates_inverse = Relates_inverse.begin(); it_Relates_inverse != Relates_inverse.end(); ++it_Relates_inverse)
		{
			shared_ptr<IfcApprovalRelationship> self_candidate( *it_Relates_inverse );
			if( self_candidate.get() == this )
			{
				Relates_inverse.erase( it_Relates_inverse );
				break;
			}
		}
	}
}

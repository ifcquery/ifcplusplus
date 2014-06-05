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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcBoundaryCondition.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelConnectsStructuralMember.h"
#include "include/IfcStructuralConnection.h"
#include "include/IfcStructuralConnectionCondition.h"
#include "include/IfcStructuralMember.h"
#include "include/IfcText.h"

// ENTITY IfcRelConnectsStructuralMember 
IfcRelConnectsStructuralMember::IfcRelConnectsStructuralMember() {}
IfcRelConnectsStructuralMember::IfcRelConnectsStructuralMember( int id ) { m_id = id; }
IfcRelConnectsStructuralMember::~IfcRelConnectsStructuralMember() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelConnectsStructuralMember::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelConnectsStructuralMember> other = dynamic_pointer_cast<IfcRelConnectsStructuralMember>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingStructuralMember = other->m_RelatingStructuralMember;
	m_RelatedStructuralConnection = other->m_RelatedStructuralConnection;
	m_AppliedCondition = other->m_AppliedCondition;
	m_AdditionalConditions = other->m_AdditionalConditions;
	m_SupportedLength = other->m_SupportedLength;
	m_ConditionCoordinateSystem = other->m_ConditionCoordinateSystem;
}
void IfcRelConnectsStructuralMember::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELCONNECTSSTRUCTURALMEMBER" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingStructuralMember ) { stream << "#" << m_RelatingStructuralMember->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RelatedStructuralConnection ) { stream << "#" << m_RelatedStructuralConnection->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AppliedCondition ) { stream << "#" << m_AppliedCondition->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AdditionalConditions ) { stream << "#" << m_AdditionalConditions->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_SupportedLength ) { m_SupportedLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ConditionCoordinateSystem ) { stream << "#" << m_ConditionCoordinateSystem->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelConnectsStructuralMember::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelConnectsStructuralMember::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelConnectsStructuralMember, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcRelConnectsStructuralMember, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_RelatingStructuralMember, map );
	readEntityReference( args[5], m_RelatedStructuralConnection, map );
	readEntityReference( args[6], m_AppliedCondition, map );
	readEntityReference( args[7], m_AdditionalConditions, map );
	m_SupportedLength = IfcLengthMeasure::createObjectFromStepData( args[8] );
	readEntityReference( args[9], m_ConditionCoordinateSystem, map );
}
void IfcRelConnectsStructuralMember::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingStructuralMember", m_RelatingStructuralMember ) );
	vec_attributes.push_back( std::make_pair( "RelatedStructuralConnection", m_RelatedStructuralConnection ) );
	vec_attributes.push_back( std::make_pair( "AppliedCondition", m_AppliedCondition ) );
	vec_attributes.push_back( std::make_pair( "AdditionalConditions", m_AdditionalConditions ) );
	vec_attributes.push_back( std::make_pair( "SupportedLength", m_SupportedLength ) );
	vec_attributes.push_back( std::make_pair( "ConditionCoordinateSystem", m_ConditionCoordinateSystem ) );
}
void IfcRelConnectsStructuralMember::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelConnectsStructuralMember::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelConnectsStructuralMember> ptr_self = dynamic_pointer_cast<IfcRelConnectsStructuralMember>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelConnectsStructuralMember::setInverseCounterparts: type mismatch" ); }
	if( m_RelatedStructuralConnection )
	{
		m_RelatedStructuralConnection->m_ConnectsStructuralMembers_inverse.push_back( ptr_self );
	}
	if( m_RelatingStructuralMember )
	{
		m_RelatingStructuralMember->m_ConnectedBy_inverse.push_back( ptr_self );
	}
}
void IfcRelConnectsStructuralMember::unlinkSelf()
{
	IfcRelConnects::unlinkSelf();
	if( m_RelatedStructuralConnection )
	{
		std::vector<weak_ptr<IfcRelConnectsStructuralMember> >& ConnectsStructuralMembers_inverse = m_RelatedStructuralConnection->m_ConnectsStructuralMembers_inverse;
		std::vector<weak_ptr<IfcRelConnectsStructuralMember> >::iterator it_ConnectsStructuralMembers_inverse;
		for( it_ConnectsStructuralMembers_inverse = ConnectsStructuralMembers_inverse.begin(); it_ConnectsStructuralMembers_inverse != ConnectsStructuralMembers_inverse.end(); ++it_ConnectsStructuralMembers_inverse)
		{
			shared_ptr<IfcRelConnectsStructuralMember> self_candidate( *it_ConnectsStructuralMembers_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ConnectsStructuralMembers_inverse.erase( it_ConnectsStructuralMembers_inverse );
				break;
			}
		}
	}
	if( m_RelatingStructuralMember )
	{
		std::vector<weak_ptr<IfcRelConnectsStructuralMember> >& ConnectedBy_inverse = m_RelatingStructuralMember->m_ConnectedBy_inverse;
		std::vector<weak_ptr<IfcRelConnectsStructuralMember> >::iterator it_ConnectedBy_inverse;
		for( it_ConnectedBy_inverse = ConnectedBy_inverse.begin(); it_ConnectedBy_inverse != ConnectedBy_inverse.end(); ++it_ConnectedBy_inverse)
		{
			shared_ptr<IfcRelConnectsStructuralMember> self_candidate( *it_ConnectedBy_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ConnectedBy_inverse.erase( it_ConnectedBy_inverse );
				break;
			}
		}
	}
}

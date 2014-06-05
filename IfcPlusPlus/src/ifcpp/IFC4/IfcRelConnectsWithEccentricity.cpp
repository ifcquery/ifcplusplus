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
#include "include/IfcConnectionGeometry.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelConnectsWithEccentricity.h"
#include "include/IfcStructuralConnection.h"
#include "include/IfcStructuralConnectionCondition.h"
#include "include/IfcStructuralMember.h"
#include "include/IfcText.h"

// ENTITY IfcRelConnectsWithEccentricity 
IfcRelConnectsWithEccentricity::IfcRelConnectsWithEccentricity() {}
IfcRelConnectsWithEccentricity::IfcRelConnectsWithEccentricity( int id ) { m_id = id; }
IfcRelConnectsWithEccentricity::~IfcRelConnectsWithEccentricity() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelConnectsWithEccentricity::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelConnectsWithEccentricity> other = dynamic_pointer_cast<IfcRelConnectsWithEccentricity>(other_entity);
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
	m_ConnectionConstraint = other->m_ConnectionConstraint;
}
void IfcRelConnectsWithEccentricity::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELCONNECTSWITHECCENTRICITY" << "(";
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
	stream << ",";
	if( m_ConnectionConstraint ) { stream << "#" << m_ConnectionConstraint->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelConnectsWithEccentricity::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelConnectsWithEccentricity::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<11 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelConnectsWithEccentricity, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>11 ){ std::cout << "Wrong parameter count for entity IfcRelConnectsWithEccentricity, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; }
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
	readEntityReference( args[10], m_ConnectionConstraint, map );
}
void IfcRelConnectsWithEccentricity::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnectsStructuralMember::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ConnectionConstraint", m_ConnectionConstraint ) );
}
void IfcRelConnectsWithEccentricity::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelConnectsWithEccentricity::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnectsStructuralMember::setInverseCounterparts( ptr_self_entity );
}
void IfcRelConnectsWithEccentricity::unlinkSelf()
{
	IfcRelConnectsStructuralMember::unlinkSelf();
}

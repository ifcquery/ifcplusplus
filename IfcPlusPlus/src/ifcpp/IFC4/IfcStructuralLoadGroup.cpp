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
#include "include/IfcActionSourceTypeEnum.h"
#include "include/IfcActionTypeEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLoadGroupTypeEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRatioMeasure.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToGroup.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcStructuralAnalysisModel.h"
#include "include/IfcStructuralLoadGroup.h"
#include "include/IfcStructuralResultGroup.h"
#include "include/IfcText.h"

// ENTITY IfcStructuralLoadGroup 
IfcStructuralLoadGroup::IfcStructuralLoadGroup() {}
IfcStructuralLoadGroup::IfcStructuralLoadGroup( int id ) { m_id = id; }
IfcStructuralLoadGroup::~IfcStructuralLoadGroup() {}

// method setEntity takes over all attributes from another instance of the class
void IfcStructuralLoadGroup::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcStructuralLoadGroup> other = dynamic_pointer_cast<IfcStructuralLoadGroup>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_PredefinedType = other->m_PredefinedType;
	m_ActionType = other->m_ActionType;
	m_ActionSource = other->m_ActionSource;
	m_Coefficient = other->m_Coefficient;
	m_Purpose = other->m_Purpose;
}
void IfcStructuralLoadGroup::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSTRUCTURALLOADGROUP" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActionType ) { m_ActionType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActionSource ) { m_ActionSource->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Coefficient ) { m_Coefficient->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Purpose ) { m_Purpose->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralLoadGroup::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadGroup::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralLoadGroup, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcStructuralLoadGroup, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_PredefinedType = IfcLoadGroupTypeEnum::createObjectFromStepData( args[5] );
	m_ActionType = IfcActionTypeEnum::createObjectFromStepData( args[6] );
	m_ActionSource = IfcActionSourceTypeEnum::createObjectFromStepData( args[7] );
	m_Coefficient = IfcRatioMeasure::createObjectFromStepData( args[8] );
	m_Purpose = IfcLabel::createObjectFromStepData( args[9] );
}
void IfcStructuralLoadGroup::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGroup::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "ActionType", m_ActionType ) );
	vec_attributes.push_back( std::make_pair( "ActionSource", m_ActionSource ) );
	vec_attributes.push_back( std::make_pair( "Coefficient", m_Coefficient ) );
	vec_attributes.push_back( std::make_pair( "Purpose", m_Purpose ) );
}
void IfcStructuralLoadGroup::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> SourceOfResultGroup_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_SourceOfResultGroup_inverse.size(); ++i ) { SourceOfResultGroup_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcStructuralResultGroup>( m_SourceOfResultGroup_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "SourceOfResultGroup_inverse", SourceOfResultGroup_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> LoadGroupFor_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_LoadGroupFor_inverse.size(); ++i ) { LoadGroupFor_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcStructuralAnalysisModel>( m_LoadGroupFor_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "LoadGroupFor_inverse", LoadGroupFor_inverse_vec_obj ) );
}
void IfcStructuralLoadGroup::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGroup::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadGroup::unlinkSelf()
{
	IfcGroup::unlinkSelf();
}

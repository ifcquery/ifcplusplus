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
#include "include/IfcAnalysisTheoryTypeEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
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

// ENTITY IfcStructuralResultGroup 
IfcStructuralResultGroup::IfcStructuralResultGroup() {}
IfcStructuralResultGroup::IfcStructuralResultGroup( int id ) { m_id = id; }
IfcStructuralResultGroup::~IfcStructuralResultGroup() {}

// method setEntity takes over all attributes from another instance of the class
void IfcStructuralResultGroup::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcStructuralResultGroup> other = dynamic_pointer_cast<IfcStructuralResultGroup>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_TheoryType = other->m_TheoryType;
	m_ResultForLoadGroup = other->m_ResultForLoadGroup;
	m_IsLinear = other->m_IsLinear;
}
void IfcStructuralResultGroup::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSTRUCTURALRESULTGROUP" << "(";
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
	if( m_TheoryType ) { m_TheoryType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ResultForLoadGroup ) { stream << "#" << m_ResultForLoadGroup->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_IsLinear == false ) { stream << ".F."; }
	else if( m_IsLinear == true ) { stream << ".T."; }
	stream << ");";
}
void IfcStructuralResultGroup::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralResultGroup::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralResultGroup, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcStructuralResultGroup, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_TheoryType = IfcAnalysisTheoryTypeEnum::createObjectFromStepData( args[5] );
	readEntityReference( args[6], m_ResultForLoadGroup, map );
	if( _stricmp( args[7].c_str(), ".F." ) == 0 ) { m_IsLinear = false; }
	else if( _stricmp( args[7].c_str(), ".T." ) == 0 ) { m_IsLinear = true; }
}
void IfcStructuralResultGroup::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGroup::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TheoryType", m_TheoryType ) );
	vec_attributes.push_back( std::make_pair( "ResultForLoadGroup", m_ResultForLoadGroup ) );
	vec_attributes.push_back( std::make_pair( "IsLinear", shared_ptr<IfcPPBool>( new IfcPPBool( m_IsLinear ) ) ) );
}
void IfcStructuralResultGroup::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> ResultGroupFor_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ResultGroupFor_inverse.size(); ++i ) { ResultGroupFor_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcStructuralAnalysisModel>( m_ResultGroupFor_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ResultGroupFor_inverse", ResultGroupFor_inverse_vec_obj ) );
}
void IfcStructuralResultGroup::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGroup::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcStructuralResultGroup> ptr_self = dynamic_pointer_cast<IfcStructuralResultGroup>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcStructuralResultGroup::setInverseCounterparts: type mismatch" ); }
	if( m_ResultForLoadGroup )
	{
		m_ResultForLoadGroup->m_SourceOfResultGroup_inverse.push_back( ptr_self );
	}
}
void IfcStructuralResultGroup::unlinkSelf()
{
	IfcGroup::unlinkSelf();
	if( m_ResultForLoadGroup )
	{
		std::vector<weak_ptr<IfcStructuralResultGroup> >& SourceOfResultGroup_inverse = m_ResultForLoadGroup->m_SourceOfResultGroup_inverse;
		std::vector<weak_ptr<IfcStructuralResultGroup> >::iterator it_SourceOfResultGroup_inverse;
		for( it_SourceOfResultGroup_inverse = SourceOfResultGroup_inverse.begin(); it_SourceOfResultGroup_inverse != SourceOfResultGroup_inverse.end(); ++it_SourceOfResultGroup_inverse)
		{
			shared_ptr<IfcStructuralResultGroup> self_candidate( *it_SourceOfResultGroup_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				SourceOfResultGroup_inverse.erase( it_SourceOfResultGroup_inverse );
				break;
			}
		}
	}
}

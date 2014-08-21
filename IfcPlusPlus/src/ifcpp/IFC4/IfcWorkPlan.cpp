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
#include "include/IfcDateTime.h"
#include "include/IfcDuration.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPerson.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToControl.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"
#include "include/IfcWorkPlan.h"
#include "include/IfcWorkPlanTypeEnum.h"

// ENTITY IfcWorkPlan 
IfcWorkPlan::IfcWorkPlan() {}
IfcWorkPlan::IfcWorkPlan( int id ) { m_id = id; }
IfcWorkPlan::~IfcWorkPlan() {}
shared_ptr<IfcPPObject> IfcWorkPlan::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWorkPlan> copy_self( new IfcWorkPlan() );
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
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy(options) ); }
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy(options) ); }
	if( m_CreationDate ) { copy_self->m_CreationDate = dynamic_pointer_cast<IfcDateTime>( m_CreationDate->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Creators.size(); ++ii )
	{
		auto item_ii = m_Creators[ii];
		if( item_ii )
		{
			copy_self->m_Creators.push_back( dynamic_pointer_cast<IfcPerson>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Purpose ) { copy_self->m_Purpose = dynamic_pointer_cast<IfcLabel>( m_Purpose->getDeepCopy(options) ); }
	if( m_Duration ) { copy_self->m_Duration = dynamic_pointer_cast<IfcDuration>( m_Duration->getDeepCopy(options) ); }
	if( m_TotalFloat ) { copy_self->m_TotalFloat = dynamic_pointer_cast<IfcDuration>( m_TotalFloat->getDeepCopy(options) ); }
	if( m_StartTime ) { copy_self->m_StartTime = dynamic_pointer_cast<IfcDateTime>( m_StartTime->getDeepCopy(options) ); }
	if( m_FinishTime ) { copy_self->m_FinishTime = dynamic_pointer_cast<IfcDateTime>( m_FinishTime->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcWorkPlanTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcWorkPlan::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCWORKPLAN" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_CreationDate ) { m_CreationDate->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Creators );
	stream << ",";
	if( m_Purpose ) { m_Purpose->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Duration ) { m_Duration->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TotalFloat ) { m_TotalFloat->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_StartTime ) { m_StartTime->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_FinishTime ) { m_FinishTime->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcWorkPlan::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcWorkPlan::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 14 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcWorkPlan, expecting 14, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	m_Identification = IfcIdentifier::createObjectFromSTEP( args[5] );
	m_CreationDate = IfcDateTime::createObjectFromSTEP( args[6] );
	readEntityReferenceList( args[7], m_Creators, map );
	m_Purpose = IfcLabel::createObjectFromSTEP( args[8] );
	m_Duration = IfcDuration::createObjectFromSTEP( args[9] );
	m_TotalFloat = IfcDuration::createObjectFromSTEP( args[10] );
	m_StartTime = IfcDateTime::createObjectFromSTEP( args[11] );
	m_FinishTime = IfcDateTime::createObjectFromSTEP( args[12] );
	m_PredefinedType = IfcWorkPlanTypeEnum::createObjectFromSTEP( args[13] );
}
void IfcWorkPlan::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcWorkControl::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcWorkPlan::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcWorkControl::getAttributesInverse( vec_attributes_inverse );
}
void IfcWorkPlan::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcWorkControl::setInverseCounterparts( ptr_self_entity );
}
void IfcWorkPlan::unlinkSelf()
{
	IfcWorkControl::unlinkSelf();
}

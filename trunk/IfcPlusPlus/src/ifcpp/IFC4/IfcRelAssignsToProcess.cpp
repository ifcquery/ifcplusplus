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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcMeasureWithUnit.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcObjectTypeEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProcess.h"
#include "include/IfcProcessSelect.h"
#include "include/IfcRelAssignsToProcess.h"
#include "include/IfcText.h"
#include "include/IfcTypeProcess.h"

// ENTITY IfcRelAssignsToProcess 
IfcRelAssignsToProcess::IfcRelAssignsToProcess() {}
IfcRelAssignsToProcess::IfcRelAssignsToProcess( int id ) { m_id = id; }
IfcRelAssignsToProcess::~IfcRelAssignsToProcess() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssignsToProcess::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssignsToProcess> other = dynamic_pointer_cast<IfcRelAssignsToProcess>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatedObjectsType = other->m_RelatedObjectsType;
	m_RelatingProcess = other->m_RelatingProcess;
	m_QuantityInProcess = other->m_QuantityInProcess;
}
void IfcRelAssignsToProcess::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSIGNSTOPROCESS" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedObjects );
	stream << ",";
	if( m_RelatedObjectsType ) { m_RelatedObjectsType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingProcess ) { m_RelatingProcess->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_QuantityInProcess ) { stream << "#" << m_QuantityInProcess->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssignsToProcess::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssignsToProcess::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssignsToProcess, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcRelAssignsToProcess, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatedObjectsType = IfcObjectTypeEnum::createObjectFromStepData( args[5] );
	m_RelatingProcess = IfcProcessSelect::createObjectFromStepData( args[6], map );
	readEntityReference( args[7], m_QuantityInProcess, map );
}
void IfcRelAssignsToProcess::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssigns::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingProcess", m_RelatingProcess ) );
	vec_attributes.push_back( std::make_pair( "QuantityInProcess", m_QuantityInProcess ) );
}
void IfcRelAssignsToProcess::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelAssignsToProcess::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssigns::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssignsToProcess> ptr_self = dynamic_pointer_cast<IfcRelAssignsToProcess>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssignsToProcess::setInverseCounterparts: type mismatch" ); }
	shared_ptr<IfcProcess>  RelatingProcess_IfcProcess = dynamic_pointer_cast<IfcProcess>( m_RelatingProcess );
	if( RelatingProcess_IfcProcess )
	{
		RelatingProcess_IfcProcess->m_OperatesOn_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcTypeProcess>  RelatingProcess_IfcTypeProcess = dynamic_pointer_cast<IfcTypeProcess>( m_RelatingProcess );
	if( RelatingProcess_IfcTypeProcess )
	{
		RelatingProcess_IfcTypeProcess->m_OperatesOn_inverse.push_back( ptr_self );
	}
}
void IfcRelAssignsToProcess::unlinkSelf()
{
	IfcRelAssigns::unlinkSelf();
	shared_ptr<IfcProcess>  RelatingProcess_IfcProcess = dynamic_pointer_cast<IfcProcess>( m_RelatingProcess );
	if( RelatingProcess_IfcProcess )
	{
		std::vector<weak_ptr<IfcRelAssignsToProcess> >& OperatesOn_inverse = RelatingProcess_IfcProcess->m_OperatesOn_inverse;
		std::vector<weak_ptr<IfcRelAssignsToProcess> >::iterator it_OperatesOn_inverse;
		for( it_OperatesOn_inverse = OperatesOn_inverse.begin(); it_OperatesOn_inverse != OperatesOn_inverse.end(); ++it_OperatesOn_inverse)
		{
			shared_ptr<IfcRelAssignsToProcess> self_candidate( *it_OperatesOn_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				OperatesOn_inverse.erase( it_OperatesOn_inverse );
				break;
			}
		}
	}
	shared_ptr<IfcTypeProcess>  RelatingProcess_IfcTypeProcess = dynamic_pointer_cast<IfcTypeProcess>( m_RelatingProcess );
	if( RelatingProcess_IfcTypeProcess )
	{
		std::vector<weak_ptr<IfcRelAssignsToProcess> >& OperatesOn_inverse = RelatingProcess_IfcTypeProcess->m_OperatesOn_inverse;
		std::vector<weak_ptr<IfcRelAssignsToProcess> >::iterator it_OperatesOn_inverse;
		for( it_OperatesOn_inverse = OperatesOn_inverse.begin(); it_OperatesOn_inverse != OperatesOn_inverse.end(); ++it_OperatesOn_inverse)
		{
			shared_ptr<IfcRelAssignsToProcess> self_candidate( *it_OperatesOn_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				OperatesOn_inverse.erase( it_OperatesOn_inverse );
				break;
			}
		}
	}
}

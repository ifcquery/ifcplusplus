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
#include "include/IfcLagTime.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProcess.h"
#include "include/IfcRelSequence.h"
#include "include/IfcSequenceEnum.h"
#include "include/IfcText.h"

// ENTITY IfcRelSequence 
IfcRelSequence::IfcRelSequence() {}
IfcRelSequence::IfcRelSequence( int id ) { m_id = id; }
IfcRelSequence::~IfcRelSequence() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelSequence::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelSequence> other = dynamic_pointer_cast<IfcRelSequence>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingProcess = other->m_RelatingProcess;
	m_RelatedProcess = other->m_RelatedProcess;
	m_TimeLag = other->m_TimeLag;
	m_SequenceType = other->m_SequenceType;
	m_UserDefinedSequenceType = other->m_UserDefinedSequenceType;
}
void IfcRelSequence::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELSEQUENCE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingProcess ) { stream << "#" << m_RelatingProcess->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RelatedProcess ) { stream << "#" << m_RelatedProcess->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TimeLag ) { stream << "#" << m_TimeLag->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_SequenceType ) { m_SequenceType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedSequenceType ) { m_UserDefinedSequenceType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRelSequence::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelSequence::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelSequence, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcRelSequence, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_RelatingProcess, map );
	readEntityReference( args[5], m_RelatedProcess, map );
	readEntityReference( args[6], m_TimeLag, map );
	m_SequenceType = IfcSequenceEnum::createObjectFromStepData( args[7] );
	m_UserDefinedSequenceType = IfcLabel::createObjectFromStepData( args[8] );
}
void IfcRelSequence::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingProcess", m_RelatingProcess ) );
	vec_attributes.push_back( std::make_pair( "RelatedProcess", m_RelatedProcess ) );
	vec_attributes.push_back( std::make_pair( "TimeLag", m_TimeLag ) );
	vec_attributes.push_back( std::make_pair( "SequenceType", m_SequenceType ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedSequenceType", m_UserDefinedSequenceType ) );
}
void IfcRelSequence::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelSequence::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelSequence> ptr_self = dynamic_pointer_cast<IfcRelSequence>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelSequence::setInverseCounterparts: type mismatch" ); }
	if( m_RelatedProcess )
	{
		m_RelatedProcess->m_IsSuccessorFrom_inverse.push_back( ptr_self );
	}
	if( m_RelatingProcess )
	{
		m_RelatingProcess->m_IsPredecessorTo_inverse.push_back( ptr_self );
	}
}
void IfcRelSequence::unlinkSelf()
{
	IfcRelConnects::unlinkSelf();
	if( m_RelatedProcess )
	{
		std::vector<weak_ptr<IfcRelSequence> >& IsSuccessorFrom_inverse = m_RelatedProcess->m_IsSuccessorFrom_inverse;
		std::vector<weak_ptr<IfcRelSequence> >::iterator it_IsSuccessorFrom_inverse;
		for( it_IsSuccessorFrom_inverse = IsSuccessorFrom_inverse.begin(); it_IsSuccessorFrom_inverse != IsSuccessorFrom_inverse.end(); ++it_IsSuccessorFrom_inverse)
		{
			shared_ptr<IfcRelSequence> self_candidate( *it_IsSuccessorFrom_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				IsSuccessorFrom_inverse.erase( it_IsSuccessorFrom_inverse );
				break;
			}
		}
	}
	if( m_RelatingProcess )
	{
		std::vector<weak_ptr<IfcRelSequence> >& IsPredecessorTo_inverse = m_RelatingProcess->m_IsPredecessorTo_inverse;
		std::vector<weak_ptr<IfcRelSequence> >::iterator it_IsPredecessorTo_inverse;
		for( it_IsPredecessorTo_inverse = IsPredecessorTo_inverse.begin(); it_IsPredecessorTo_inverse != IsPredecessorTo_inverse.end(); ++it_IsPredecessorTo_inverse)
		{
			shared_ptr<IfcRelSequence> self_candidate( *it_IsPredecessorTo_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				IsPredecessorTo_inverse.erase( it_IsPredecessorTo_inverse );
				break;
			}
		}
	}
}

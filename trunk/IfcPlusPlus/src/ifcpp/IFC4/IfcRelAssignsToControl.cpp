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
#include "include/IfcControl.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcObjectTypeEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssignsToControl.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssignsToControl 
IfcRelAssignsToControl::IfcRelAssignsToControl() {}
IfcRelAssignsToControl::IfcRelAssignsToControl( int id ) { m_id = id; }
IfcRelAssignsToControl::~IfcRelAssignsToControl() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssignsToControl::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssignsToControl> other = dynamic_pointer_cast<IfcRelAssignsToControl>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatedObjectsType = other->m_RelatedObjectsType;
	m_RelatingControl = other->m_RelatingControl;
}
void IfcRelAssignsToControl::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSIGNSTOCONTROL" << "(";
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
	if( m_RelatingControl ) { stream << "#" << m_RelatingControl->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssignsToControl::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssignsToControl::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssignsToControl, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcRelAssignsToControl, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatedObjectsType = IfcObjectTypeEnum::createObjectFromStepData( args[5] );
	readEntityReference( args[6], m_RelatingControl, map );
}
void IfcRelAssignsToControl::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssigns::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingControl", m_RelatingControl ) );
}
void IfcRelAssignsToControl::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelAssignsToControl::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssigns::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssignsToControl> ptr_self = dynamic_pointer_cast<IfcRelAssignsToControl>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssignsToControl::setInverseCounterparts: type mismatch" ); }
	if( m_RelatingControl )
	{
		m_RelatingControl->m_Controls_inverse.push_back( ptr_self );
	}
}
void IfcRelAssignsToControl::unlinkSelf()
{
	IfcRelAssigns::unlinkSelf();
	if( m_RelatingControl )
	{
		std::vector<weak_ptr<IfcRelAssignsToControl> >& Controls_inverse = m_RelatingControl->m_Controls_inverse;
		std::vector<weak_ptr<IfcRelAssignsToControl> >::iterator it_Controls_inverse;
		for( it_Controls_inverse = Controls_inverse.begin(); it_Controls_inverse != Controls_inverse.end(); ++it_Controls_inverse)
		{
			shared_ptr<IfcRelAssignsToControl> self_candidate( *it_Controls_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				Controls_inverse.erase( it_Controls_inverse );
				break;
			}
		}
	}
}

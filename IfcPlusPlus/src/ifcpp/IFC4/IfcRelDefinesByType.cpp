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
#include "include/IfcObject.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"

// ENTITY IfcRelDefinesByType 
IfcRelDefinesByType::IfcRelDefinesByType() {}
IfcRelDefinesByType::IfcRelDefinesByType( int id ) { m_id = id; }
IfcRelDefinesByType::~IfcRelDefinesByType() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelDefinesByType::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelDefinesByType> other = dynamic_pointer_cast<IfcRelDefinesByType>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatingType = other->m_RelatingType;
}
void IfcRelDefinesByType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELDEFINESBYTYPE" << "(";
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
	if( m_RelatingType ) { stream << "#" << m_RelatingType->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelDefinesByType::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelDefinesByType::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelDefinesByType, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelDefinesByType, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	readEntityReference( args[5], m_RelatingType, map );
}
void IfcRelDefinesByType::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelDefines::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingType", m_RelatingType ) );
}
void IfcRelDefinesByType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelDefinesByType::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelDefines::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelDefinesByType> ptr_self = dynamic_pointer_cast<IfcRelDefinesByType>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelDefinesByType::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		if( m_RelatedObjects[i] )
		{
			m_RelatedObjects[i]->m_IsTypedBy_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingType )
	{
		m_RelatingType->m_Types_inverse.push_back( ptr_self );
	}
}
void IfcRelDefinesByType::unlinkSelf()
{
	IfcRelDefines::unlinkSelf();
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		if( m_RelatedObjects[i] )
		{
			std::vector<weak_ptr<IfcRelDefinesByType> >& IsTypedBy_inverse = m_RelatedObjects[i]->m_IsTypedBy_inverse;
			std::vector<weak_ptr<IfcRelDefinesByType> >::iterator it_IsTypedBy_inverse;
			for( it_IsTypedBy_inverse = IsTypedBy_inverse.begin(); it_IsTypedBy_inverse != IsTypedBy_inverse.end(); ++it_IsTypedBy_inverse)
			{
				shared_ptr<IfcRelDefinesByType> self_candidate( *it_IsTypedBy_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					IsTypedBy_inverse.erase( it_IsTypedBy_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingType )
	{
		std::vector<weak_ptr<IfcRelDefinesByType> >& Types_inverse = m_RelatingType->m_Types_inverse;
		std::vector<weak_ptr<IfcRelDefinesByType> >::iterator it_Types_inverse;
		for( it_Types_inverse = Types_inverse.begin(); it_Types_inverse != Types_inverse.end(); ++it_Types_inverse)
		{
			shared_ptr<IfcRelDefinesByType> self_candidate( *it_Types_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				Types_inverse.erase( it_Types_inverse );
				break;
			}
		}
	}
}

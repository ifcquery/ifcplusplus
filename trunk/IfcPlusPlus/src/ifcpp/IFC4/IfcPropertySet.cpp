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
#include "include/IfcOwnerHistory.h"
#include "include/IfcProperty.h"
#include "include/IfcPropertySet.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByTemplate.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"

// ENTITY IfcPropertySet 
IfcPropertySet::IfcPropertySet() {}
IfcPropertySet::IfcPropertySet( int id ) { m_id = id; }
IfcPropertySet::~IfcPropertySet() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertySet::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertySet> other = dynamic_pointer_cast<IfcPropertySet>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_HasProperties = other->m_HasProperties;
}
void IfcPropertySet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYSET" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasProperties );
	stream << ");";
}
void IfcPropertySet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertySet::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertySet, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcPropertySet, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_HasProperties, map );
}
void IfcPropertySet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertySetDefinition::getAttributes( vec_attributes );
}
void IfcPropertySet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPropertySet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertySetDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcPropertySet> ptr_self = dynamic_pointer_cast<IfcPropertySet>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcPropertySet::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_HasProperties.size(); ++i )
	{
		if( m_HasProperties[i] )
		{
			m_HasProperties[i]->m_PartOfPset_inverse.push_back( ptr_self );
		}
	}
}
void IfcPropertySet::unlinkSelf()
{
	IfcPropertySetDefinition::unlinkSelf();
	for( int i=0; i<m_HasProperties.size(); ++i )
	{
		if( m_HasProperties[i] )
		{
			std::vector<weak_ptr<IfcPropertySet> >& PartOfPset_inverse = m_HasProperties[i]->m_PartOfPset_inverse;
			std::vector<weak_ptr<IfcPropertySet> >::iterator it_PartOfPset_inverse;
			for( it_PartOfPset_inverse = PartOfPset_inverse.begin(); it_PartOfPset_inverse != PartOfPset_inverse.end(); ++it_PartOfPset_inverse)
			{
				shared_ptr<IfcPropertySet> self_candidate( *it_PartOfPset_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					PartOfPset_inverse.erase( it_PartOfPset_inverse );
					break;
				}
			}
		}
	}
}

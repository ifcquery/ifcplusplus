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
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcText.h"

// ENTITY IfcRelDefinesByObject 
IfcRelDefinesByObject::IfcRelDefinesByObject() {}
IfcRelDefinesByObject::IfcRelDefinesByObject( int id ) { m_id = id; }
IfcRelDefinesByObject::~IfcRelDefinesByObject() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelDefinesByObject::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelDefinesByObject> other = dynamic_pointer_cast<IfcRelDefinesByObject>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatingObject = other->m_RelatingObject;
}
void IfcRelDefinesByObject::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELDEFINESBYOBJECT" << "(";
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
	if( m_RelatingObject ) { stream << "#" << m_RelatingObject->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelDefinesByObject::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelDefinesByObject::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelDefinesByObject, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelDefinesByObject, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	readEntityReference( args[5], m_RelatingObject, map );
}
void IfcRelDefinesByObject::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelDefines::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingObject", m_RelatingObject ) );
}
void IfcRelDefinesByObject::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelDefinesByObject::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelDefines::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelDefinesByObject> ptr_self = dynamic_pointer_cast<IfcRelDefinesByObject>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelDefinesByObject::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		if( m_RelatedObjects[i] )
		{
			m_RelatedObjects[i]->m_IsDeclaredBy_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingObject )
	{
		m_RelatingObject->m_Declares_inverse.push_back( ptr_self );
	}
}
void IfcRelDefinesByObject::unlinkSelf()
{
	IfcRelDefines::unlinkSelf();
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		if( m_RelatedObjects[i] )
		{
			std::vector<weak_ptr<IfcRelDefinesByObject> >& IsDeclaredBy_inverse = m_RelatedObjects[i]->m_IsDeclaredBy_inverse;
			std::vector<weak_ptr<IfcRelDefinesByObject> >::iterator it_IsDeclaredBy_inverse;
			for( it_IsDeclaredBy_inverse = IsDeclaredBy_inverse.begin(); it_IsDeclaredBy_inverse != IsDeclaredBy_inverse.end(); ++it_IsDeclaredBy_inverse)
			{
				shared_ptr<IfcRelDefinesByObject> self_candidate( *it_IsDeclaredBy_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					IsDeclaredBy_inverse.erase( it_IsDeclaredBy_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingObject )
	{
		std::vector<weak_ptr<IfcRelDefinesByObject> >& Declares_inverse = m_RelatingObject->m_Declares_inverse;
		std::vector<weak_ptr<IfcRelDefinesByObject> >::iterator it_Declares_inverse;
		for( it_Declares_inverse = Declares_inverse.begin(); it_Declares_inverse != Declares_inverse.end(); ++it_Declares_inverse)
		{
			shared_ptr<IfcRelDefinesByObject> self_candidate( *it_Declares_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				Declares_inverse.erase( it_Declares_inverse );
				break;
			}
		}
	}
}

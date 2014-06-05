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
#include "include/IfcContext.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObject.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcPropertySetDefinitionSelect.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcText.h"

// ENTITY IfcRelDefinesByProperties 
IfcRelDefinesByProperties::IfcRelDefinesByProperties() {}
IfcRelDefinesByProperties::IfcRelDefinesByProperties( int id ) { m_id = id; }
IfcRelDefinesByProperties::~IfcRelDefinesByProperties() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelDefinesByProperties::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelDefinesByProperties> other = dynamic_pointer_cast<IfcRelDefinesByProperties>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatingPropertyDefinition = other->m_RelatingPropertyDefinition;
}
void IfcRelDefinesByProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELDEFINESBYPROPERTIES" << "(";
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
	if( m_RelatingPropertyDefinition ) { m_RelatingPropertyDefinition->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcRelDefinesByProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelDefinesByProperties::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelDefinesByProperties, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelDefinesByProperties, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatingPropertyDefinition = IfcPropertySetDefinitionSelect::createObjectFromStepData( args[5], map );
}
void IfcRelDefinesByProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelDefines::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingPropertyDefinition", m_RelatingPropertyDefinition ) );
}
void IfcRelDefinesByProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelDefinesByProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelDefines::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelDefinesByProperties> ptr_self = dynamic_pointer_cast<IfcRelDefinesByProperties>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelDefinesByProperties::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcContext>  RelatedObjects_IfcContext = dynamic_pointer_cast<IfcContext>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcContext )
		{
			RelatedObjects_IfcContext->m_IsDefinedBy_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcObject>  RelatedObjects_IfcObject = dynamic_pointer_cast<IfcObject>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObject )
		{
			RelatedObjects_IfcObject->m_IsDefinedBy_inverse.push_back( ptr_self );
		}
	}
	shared_ptr<IfcPropertySetDefinition>  RelatingPropertyDefinition_IfcPropertySetDefinition = dynamic_pointer_cast<IfcPropertySetDefinition>( m_RelatingPropertyDefinition );
	if( RelatingPropertyDefinition_IfcPropertySetDefinition )
	{
		RelatingPropertyDefinition_IfcPropertySetDefinition->m_DefinesOccurrence_inverse.push_back( ptr_self );
	}
}
void IfcRelDefinesByProperties::unlinkSelf()
{
	IfcRelDefines::unlinkSelf();
	for( int i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcContext>  RelatedObjects_IfcContext = dynamic_pointer_cast<IfcContext>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcContext )
		{
			std::vector<weak_ptr<IfcRelDefinesByProperties> >& IsDefinedBy_inverse = RelatedObjects_IfcContext->m_IsDefinedBy_inverse;
			std::vector<weak_ptr<IfcRelDefinesByProperties> >::iterator it_IsDefinedBy_inverse;
			for( it_IsDefinedBy_inverse = IsDefinedBy_inverse.begin(); it_IsDefinedBy_inverse != IsDefinedBy_inverse.end(); ++it_IsDefinedBy_inverse)
			{
				shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_IsDefinedBy_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					IsDefinedBy_inverse.erase( it_IsDefinedBy_inverse );
					break;
				}
			}
		}
		shared_ptr<IfcObject>  RelatedObjects_IfcObject = dynamic_pointer_cast<IfcObject>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObject )
		{
			std::vector<weak_ptr<IfcRelDefinesByProperties> >& IsDefinedBy_inverse = RelatedObjects_IfcObject->m_IsDefinedBy_inverse;
			std::vector<weak_ptr<IfcRelDefinesByProperties> >::iterator it_IsDefinedBy_inverse;
			for( it_IsDefinedBy_inverse = IsDefinedBy_inverse.begin(); it_IsDefinedBy_inverse != IsDefinedBy_inverse.end(); ++it_IsDefinedBy_inverse)
			{
				shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_IsDefinedBy_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					IsDefinedBy_inverse.erase( it_IsDefinedBy_inverse );
					break;
				}
			}
		}
	}
	shared_ptr<IfcPropertySetDefinition>  RelatingPropertyDefinition_IfcPropertySetDefinition = dynamic_pointer_cast<IfcPropertySetDefinition>( m_RelatingPropertyDefinition );
	if( RelatingPropertyDefinition_IfcPropertySetDefinition )
	{
		std::vector<weak_ptr<IfcRelDefinesByProperties> >& DefinesOccurrence_inverse = RelatingPropertyDefinition_IfcPropertySetDefinition->m_DefinesOccurrence_inverse;
		std::vector<weak_ptr<IfcRelDefinesByProperties> >::iterator it_DefinesOccurrence_inverse;
		for( it_DefinesOccurrence_inverse = DefinesOccurrence_inverse.begin(); it_DefinesOccurrence_inverse != DefinesOccurrence_inverse.end(); ++it_DefinesOccurrence_inverse)
		{
			shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_DefinesOccurrence_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				DefinesOccurrence_inverse.erase( it_DefinesOccurrence_inverse );
				break;
			}
		}
	}
}

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
#include "include/IfcDefinitionSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLibraryInformation.h"
#include "include/IfcLibraryReference.h"
#include "include/IfcLibrarySelect.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssociatesLibrary.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssociatesLibrary 
IfcRelAssociatesLibrary::IfcRelAssociatesLibrary() {}
IfcRelAssociatesLibrary::IfcRelAssociatesLibrary( int id ) { m_id = id; }
IfcRelAssociatesLibrary::~IfcRelAssociatesLibrary() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssociatesLibrary::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssociatesLibrary> other = dynamic_pointer_cast<IfcRelAssociatesLibrary>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatingLibrary = other->m_RelatingLibrary;
}
void IfcRelAssociatesLibrary::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSOCIATESLIBRARY" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_RelatedObjects, true );
	stream << ",";
	if( m_RelatingLibrary ) { m_RelatingLibrary->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssociatesLibrary::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociatesLibrary::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssociatesLibrary, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelAssociatesLibrary, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readSelectList( args[4], m_RelatedObjects, map );
	m_RelatingLibrary = IfcLibrarySelect::createObjectFromStepData( args[5], map );
}
void IfcRelAssociatesLibrary::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssociates::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingLibrary", m_RelatingLibrary ) );
}
void IfcRelAssociatesLibrary::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelAssociatesLibrary::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssociates::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssociatesLibrary> ptr_self = dynamic_pointer_cast<IfcRelAssociatesLibrary>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssociatesLibrary::setInverseCounterparts: type mismatch" ); }
	shared_ptr<IfcLibraryInformation>  RelatingLibrary_IfcLibraryInformation = dynamic_pointer_cast<IfcLibraryInformation>( m_RelatingLibrary );
	if( RelatingLibrary_IfcLibraryInformation )
	{
		RelatingLibrary_IfcLibraryInformation->m_LibraryInfoForObjects_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcLibraryReference>  RelatingLibrary_IfcLibraryReference = dynamic_pointer_cast<IfcLibraryReference>( m_RelatingLibrary );
	if( RelatingLibrary_IfcLibraryReference )
	{
		RelatingLibrary_IfcLibraryReference->m_LibraryRefForObjects_inverse.push_back( ptr_self );
	}
}
void IfcRelAssociatesLibrary::unlinkSelf()
{
	IfcRelAssociates::unlinkSelf();
	shared_ptr<IfcLibraryInformation>  RelatingLibrary_IfcLibraryInformation = dynamic_pointer_cast<IfcLibraryInformation>( m_RelatingLibrary );
	if( RelatingLibrary_IfcLibraryInformation )
	{
		std::vector<weak_ptr<IfcRelAssociatesLibrary> >& LibraryInfoForObjects_inverse = RelatingLibrary_IfcLibraryInformation->m_LibraryInfoForObjects_inverse;
		std::vector<weak_ptr<IfcRelAssociatesLibrary> >::iterator it_LibraryInfoForObjects_inverse;
		for( it_LibraryInfoForObjects_inverse = LibraryInfoForObjects_inverse.begin(); it_LibraryInfoForObjects_inverse != LibraryInfoForObjects_inverse.end(); ++it_LibraryInfoForObjects_inverse)
		{
			shared_ptr<IfcRelAssociatesLibrary> self_candidate( *it_LibraryInfoForObjects_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				LibraryInfoForObjects_inverse.erase( it_LibraryInfoForObjects_inverse );
				break;
			}
		}
	}
	shared_ptr<IfcLibraryReference>  RelatingLibrary_IfcLibraryReference = dynamic_pointer_cast<IfcLibraryReference>( m_RelatingLibrary );
	if( RelatingLibrary_IfcLibraryReference )
	{
		std::vector<weak_ptr<IfcRelAssociatesLibrary> >& LibraryRefForObjects_inverse = RelatingLibrary_IfcLibraryReference->m_LibraryRefForObjects_inverse;
		std::vector<weak_ptr<IfcRelAssociatesLibrary> >::iterator it_LibraryRefForObjects_inverse;
		for( it_LibraryRefForObjects_inverse = LibraryRefForObjects_inverse.begin(); it_LibraryRefForObjects_inverse != LibraryRefForObjects_inverse.end(); ++it_LibraryRefForObjects_inverse)
		{
			shared_ptr<IfcRelAssociatesLibrary> self_candidate( *it_LibraryRefForObjects_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				LibraryRefForObjects_inverse.erase( it_LibraryRefForObjects_inverse );
				break;
			}
		}
	}
}

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
#include "include/IfcActorSelect.h"
#include "include/IfcDateTime.h"
#include "include/IfcLabel.h"
#include "include/IfcLibraryInformation.h"
#include "include/IfcLibraryReference.h"
#include "include/IfcRelAssociatesLibrary.h"
#include "include/IfcText.h"
#include "include/IfcURIReference.h"

// ENTITY IfcLibraryInformation 
IfcLibraryInformation::IfcLibraryInformation() {}
IfcLibraryInformation::IfcLibraryInformation( int id ) { m_id = id; }
IfcLibraryInformation::~IfcLibraryInformation() {}

// method setEntity takes over all attributes from another instance of the class
void IfcLibraryInformation::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcLibraryInformation> other = dynamic_pointer_cast<IfcLibraryInformation>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Version = other->m_Version;
	m_Publisher = other->m_Publisher;
	m_VersionDate = other->m_VersionDate;
	m_Location = other->m_Location;
	m_Description = other->m_Description;
}
void IfcLibraryInformation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCLIBRARYINFORMATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Version ) { m_Version->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Publisher ) { m_Publisher->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_VersionDate ) { m_VersionDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcLibraryInformation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLibraryInformation::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLibraryInformation, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcLibraryInformation, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Version = IfcLabel::createObjectFromStepData( args[1] );
	m_Publisher = IfcActorSelect::createObjectFromStepData( args[2], map );
	m_VersionDate = IfcDateTime::createObjectFromStepData( args[3] );
	m_Location = IfcURIReference::createObjectFromStepData( args[4] );
	m_Description = IfcText::createObjectFromStepData( args[5] );
}
void IfcLibraryInformation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExternalInformation::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Version", m_Version ) );
	vec_attributes.push_back( std::make_pair( "Publisher", m_Publisher ) );
	vec_attributes.push_back( std::make_pair( "VersionDate", m_VersionDate ) );
	vec_attributes.push_back( std::make_pair( "Location", m_Location ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcLibraryInformation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> LibraryInfoForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_LibraryInfoForObjects_inverse.size(); ++i ) { LibraryInfoForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesLibrary>( m_LibraryInfoForObjects_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "LibraryInfoForObjects_inverse", LibraryInfoForObjects_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasLibraryReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasLibraryReferences_inverse.size(); ++i ) { HasLibraryReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcLibraryReference>( m_HasLibraryReferences_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasLibraryReferences_inverse", HasLibraryReferences_inverse_vec_obj ) );
}
void IfcLibraryInformation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalInformation::setInverseCounterparts( ptr_self_entity );
}
void IfcLibraryInformation::unlinkSelf()
{
	IfcExternalInformation::unlinkSelf();
}

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
shared_ptr<IfcPPObject> IfcLibraryInformation::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLibraryInformation> copy_self( new IfcLibraryInformation() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Version ) { copy_self->m_Version = dynamic_pointer_cast<IfcLabel>( m_Version->getDeepCopy(options) ); }
	if( m_Publisher ) { copy_self->m_Publisher = dynamic_pointer_cast<IfcActorSelect>( m_Publisher->getDeepCopy(options) ); }
	if( m_VersionDate ) { copy_self->m_VersionDate = dynamic_pointer_cast<IfcDateTime>( m_VersionDate->getDeepCopy(options) ); }
	if( m_Location ) { copy_self->m_Location = dynamic_pointer_cast<IfcURIReference>( m_Location->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	return copy_self;
}
void IfcLibraryInformation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCLIBRARYINFORMATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Version ) { m_Version->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Publisher ) { m_Publisher->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_VersionDate ) { m_VersionDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcLibraryInformation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLibraryInformation::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLibraryInformation, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Version = IfcLabel::createObjectFromSTEP( args[1] );
	m_Publisher = IfcActorSelect::createObjectFromSTEP( args[2], map );
	m_VersionDate = IfcDateTime::createObjectFromSTEP( args[3] );
	m_Location = IfcURIReference::createObjectFromSTEP( args[4] );
	m_Description = IfcText::createObjectFromSTEP( args[5] );
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
	IfcExternalInformation::getAttributesInverse( vec_attributes_inverse );
	if( m_LibraryInfoForObjects_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LibraryInfoForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_LibraryInfoForObjects_inverse.size(); ++i )
		{
			if( !m_LibraryInfoForObjects_inverse[i].expired() )
			{
				LibraryInfoForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesLibrary>( m_LibraryInfoForObjects_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "LibraryInfoForObjects_inverse", LibraryInfoForObjects_inverse_vec_obj ) );
	}
	if( m_HasLibraryReferences_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasLibraryReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasLibraryReferences_inverse.size(); ++i )
		{
			if( !m_HasLibraryReferences_inverse[i].expired() )
			{
				HasLibraryReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcLibraryReference>( m_HasLibraryReferences_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasLibraryReferences_inverse", HasLibraryReferences_inverse_vec_obj ) );
	}
}
void IfcLibraryInformation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalInformation::setInverseCounterparts( ptr_self_entity );
}
void IfcLibraryInformation::unlinkSelf()
{
	IfcExternalInformation::unlinkSelf();
}

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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcLanguageId.h"
#include "include/IfcLibraryInformation.h"
#include "include/IfcLibraryReference.h"
#include "include/IfcRelAssociatesLibrary.h"
#include "include/IfcText.h"
#include "include/IfcURIReference.h"

// ENTITY IfcLibraryReference 
IfcLibraryReference::IfcLibraryReference() {}
IfcLibraryReference::IfcLibraryReference( int id ) { m_id = id; }
IfcLibraryReference::~IfcLibraryReference() {}
shared_ptr<IfcPPObject> IfcLibraryReference::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLibraryReference> copy_self( new IfcLibraryReference() );
	if( m_Location ) { copy_self->m_Location = dynamic_pointer_cast<IfcURIReference>( m_Location->getDeepCopy(options) ); }
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Language ) { copy_self->m_Language = dynamic_pointer_cast<IfcLanguageId>( m_Language->getDeepCopy(options) ); }
	if( m_ReferencedLibrary ) { copy_self->m_ReferencedLibrary = dynamic_pointer_cast<IfcLibraryInformation>( m_ReferencedLibrary->getDeepCopy(options) ); }
	return copy_self;
}
void IfcLibraryReference::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCLIBRARYREFERENCE" << "(";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Language ) { m_Language->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReferencedLibrary ) { stream << "#" << m_ReferencedLibrary->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcLibraryReference::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLibraryReference::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLibraryReference, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Location = IfcURIReference::createObjectFromSTEP( args[0] );
	m_Identification = IfcIdentifier::createObjectFromSTEP( args[1] );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_Language = IfcLanguageId::createObjectFromSTEP( args[4] );
	readEntityReference( args[5], m_ReferencedLibrary, map );
}
void IfcLibraryReference::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExternalReference::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "Language", m_Language ) );
	vec_attributes.push_back( std::make_pair( "ReferencedLibrary", m_ReferencedLibrary ) );
}
void IfcLibraryReference::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcExternalReference::getAttributesInverse( vec_attributes_inverse );
	if( m_LibraryRefForObjects_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LibraryRefForObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_LibraryRefForObjects_inverse.size(); ++i )
		{
			if( !m_LibraryRefForObjects_inverse[i].expired() )
			{
				LibraryRefForObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesLibrary>( m_LibraryRefForObjects_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "LibraryRefForObjects_inverse", LibraryRefForObjects_inverse_vec_obj ) );
	}
}
void IfcLibraryReference::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalReference::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcLibraryReference> ptr_self = dynamic_pointer_cast<IfcLibraryReference>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcLibraryReference::setInverseCounterparts: type mismatch" ); }
	if( m_ReferencedLibrary )
	{
		m_ReferencedLibrary->m_HasLibraryReferences_inverse.push_back( ptr_self );
	}
}
void IfcLibraryReference::unlinkSelf()
{
	IfcExternalReference::unlinkSelf();
	if( m_ReferencedLibrary )
	{
		std::vector<weak_ptr<IfcLibraryReference> >& HasLibraryReferences_inverse = m_ReferencedLibrary->m_HasLibraryReferences_inverse;
		for( auto it_HasLibraryReferences_inverse = HasLibraryReferences_inverse.begin(); it_HasLibraryReferences_inverse != HasLibraryReferences_inverse.end(); ++it_HasLibraryReferences_inverse)
		{
			shared_ptr<IfcLibraryReference> self_candidate( *it_HasLibraryReferences_inverse );
			if( self_candidate.get() == this )
			{
				HasLibraryReferences_inverse.erase( it_HasLibraryReferences_inverse );
				break;
			}
		}
	}
}

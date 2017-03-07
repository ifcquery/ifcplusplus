/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelServicesBuildings.h"
#include "include/IfcSpatialElement.h"
#include "include/IfcSystem.h"
#include "include/IfcText.h"

// ENTITY IfcRelServicesBuildings 
IfcRelServicesBuildings::IfcRelServicesBuildings() { m_entity_enum = IFCRELSERVICESBUILDINGS; }
IfcRelServicesBuildings::IfcRelServicesBuildings( int id ) { m_id = id; m_entity_enum = IFCRELSERVICESBUILDINGS; }
IfcRelServicesBuildings::~IfcRelServicesBuildings() {}
shared_ptr<IfcPPObject> IfcRelServicesBuildings::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelServicesBuildings> copy_self( new IfcRelServicesBuildings() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_RelatingSystem ) { copy_self->m_RelatingSystem = dynamic_pointer_cast<IfcSystem>( m_RelatingSystem->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedBuildings.size(); ++ii )
	{
		auto item_ii = m_RelatedBuildings[ii];
		if( item_ii )
		{
			copy_self->m_RelatedBuildings.push_back( dynamic_pointer_cast<IfcSpatialElement>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcRelServicesBuildings::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELSERVICESBUILDINGS" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingSystem ) { stream << "#" << m_RelatingSystem->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedBuildings );
	stream << ");";
}
void IfcRelServicesBuildings::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelServicesBuildings::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRelServicesBuildings, expecting 6, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReference( args[4], m_RelatingSystem, map );
	readEntityReferenceList( args[5], m_RelatedBuildings, map );
}
void IfcRelServicesBuildings::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingSystem", m_RelatingSystem ) );
	if( m_RelatedBuildings.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedBuildings_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedBuildings.begin(), m_RelatedBuildings.end(), std::back_inserter( RelatedBuildings_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedBuildings", RelatedBuildings_vec_object ) );
	}
}
void IfcRelServicesBuildings::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelConnects::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelServicesBuildings::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelServicesBuildings> ptr_self = dynamic_pointer_cast<IfcRelServicesBuildings>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelServicesBuildings::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedBuildings.size(); ++i )
	{
		if( m_RelatedBuildings[i] )
		{
			m_RelatedBuildings[i]->m_ServicedBySystems_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingSystem )
	{
		m_RelatingSystem->m_ServicesBuildings_inverse.push_back( ptr_self );
	}
}
void IfcRelServicesBuildings::unlinkFromInverseCounterparts()
{
	IfcRelConnects::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_RelatedBuildings.size(); ++i )
	{
		if( m_RelatedBuildings[i] )
		{
			std::vector<weak_ptr<IfcRelServicesBuildings> >& ServicedBySystems_inverse = m_RelatedBuildings[i]->m_ServicedBySystems_inverse;
			for( auto it_ServicedBySystems_inverse = ServicedBySystems_inverse.begin(); it_ServicedBySystems_inverse != ServicedBySystems_inverse.end(); )
			{
				shared_ptr<IfcRelServicesBuildings> self_candidate( *it_ServicedBySystems_inverse );
				if( self_candidate.get() == this )
				{
					it_ServicedBySystems_inverse= ServicedBySystems_inverse.erase( it_ServicedBySystems_inverse );
				}
				else
				{
					++it_ServicedBySystems_inverse;
				}
			}
		}
	}
	if( m_RelatingSystem )
	{
		std::vector<weak_ptr<IfcRelServicesBuildings> >& ServicesBuildings_inverse = m_RelatingSystem->m_ServicesBuildings_inverse;
		for( auto it_ServicesBuildings_inverse = ServicesBuildings_inverse.begin(); it_ServicesBuildings_inverse != ServicesBuildings_inverse.end(); )
		{
			shared_ptr<IfcRelServicesBuildings> self_candidate( *it_ServicesBuildings_inverse );
			if( self_candidate.get() == this )
			{
				it_ServicesBuildings_inverse= ServicesBuildings_inverse.erase( it_ServicesBuildings_inverse );
			}
			else
			{
				++it_ServicesBuildings_inverse;
			}
		}
	}
}

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
#include "include/IfcElement.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProduct.h"
#include "include/IfcRelReferencedInSpatialStructure.h"
#include "include/IfcSpatialElement.h"
#include "include/IfcText.h"

// ENTITY IfcRelReferencedInSpatialStructure 
IfcRelReferencedInSpatialStructure::IfcRelReferencedInSpatialStructure() { m_entity_enum = IFCRELREFERENCEDINSPATIALSTRUCTURE; }
IfcRelReferencedInSpatialStructure::IfcRelReferencedInSpatialStructure( int id ) { m_id = id; m_entity_enum = IFCRELREFERENCEDINSPATIALSTRUCTURE; }
IfcRelReferencedInSpatialStructure::~IfcRelReferencedInSpatialStructure() {}
shared_ptr<IfcPPObject> IfcRelReferencedInSpatialStructure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelReferencedInSpatialStructure> copy_self( new IfcRelReferencedInSpatialStructure() );
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
	for( size_t ii=0; ii<m_RelatedElements.size(); ++ii )
	{
		auto item_ii = m_RelatedElements[ii];
		if( item_ii )
		{
			copy_self->m_RelatedElements.push_back( dynamic_pointer_cast<IfcProduct>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatingStructure ) { copy_self->m_RelatingStructure = dynamic_pointer_cast<IfcSpatialElement>( m_RelatingStructure->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelReferencedInSpatialStructure::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELREFERENCEDINSPATIALSTRUCTURE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_RelatedElements );
	stream << ",";
	if( m_RelatingStructure ) { stream << "#" << m_RelatingStructure->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcRelReferencedInSpatialStructure::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelReferencedInSpatialStructure::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRelReferencedInSpatialStructure, expecting 6, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReferenceList( args[4], m_RelatedElements, map );
	readEntityReference( args[5], m_RelatingStructure, map );
}
void IfcRelReferencedInSpatialStructure::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	if( m_RelatedElements.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedElements_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedElements.begin(), m_RelatedElements.end(), std::back_inserter( RelatedElements_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedElements", RelatedElements_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "RelatingStructure", m_RelatingStructure ) );
}
void IfcRelReferencedInSpatialStructure::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelConnects::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelReferencedInSpatialStructure::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelReferencedInSpatialStructure> ptr_self = dynamic_pointer_cast<IfcRelReferencedInSpatialStructure>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelReferencedInSpatialStructure::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedElements.size(); ++i )
	{
		shared_ptr<IfcElement>  RelatedElements_IfcElement = dynamic_pointer_cast<IfcElement>( m_RelatedElements[i] );
		if( RelatedElements_IfcElement )
		{
			RelatedElements_IfcElement->m_ReferencedInStructures_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingStructure )
	{
		m_RelatingStructure->m_ReferencesElements_inverse.push_back( ptr_self );
	}
}
void IfcRelReferencedInSpatialStructure::unlinkFromInverseCounterparts()
{
	IfcRelConnects::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_RelatedElements.size(); ++i )
	{
		shared_ptr<IfcElement>  RelatedElements_IfcElement = dynamic_pointer_cast<IfcElement>( m_RelatedElements[i] );
		if( RelatedElements_IfcElement )
		{
			std::vector<weak_ptr<IfcRelReferencedInSpatialStructure> >& ReferencedInStructures_inverse = RelatedElements_IfcElement->m_ReferencedInStructures_inverse;
			for( auto it_ReferencedInStructures_inverse = ReferencedInStructures_inverse.begin(); it_ReferencedInStructures_inverse != ReferencedInStructures_inverse.end(); )
			{
				shared_ptr<IfcRelReferencedInSpatialStructure> self_candidate( *it_ReferencedInStructures_inverse );
				if( self_candidate.get() == this )
				{
					it_ReferencedInStructures_inverse= ReferencedInStructures_inverse.erase( it_ReferencedInStructures_inverse );
				}
				else
				{
					++it_ReferencedInStructures_inverse;
				}
			}
		}
	}
	if( m_RelatingStructure )
	{
		std::vector<weak_ptr<IfcRelReferencedInSpatialStructure> >& ReferencesElements_inverse = m_RelatingStructure->m_ReferencesElements_inverse;
		for( auto it_ReferencesElements_inverse = ReferencesElements_inverse.begin(); it_ReferencesElements_inverse != ReferencesElements_inverse.end(); )
		{
			shared_ptr<IfcRelReferencedInSpatialStructure> self_candidate( *it_ReferencesElements_inverse );
			if( self_candidate.get() == this )
			{
				it_ReferencesElements_inverse= ReferencesElements_inverse.erase( it_ReferencesElements_inverse );
			}
			else
			{
				++it_ReferencesElements_inverse;
			}
		}
	}
}

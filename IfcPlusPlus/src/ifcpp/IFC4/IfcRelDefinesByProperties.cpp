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
IfcRelDefinesByProperties::IfcRelDefinesByProperties() { m_entity_enum = IFCRELDEFINESBYPROPERTIES; }
IfcRelDefinesByProperties::IfcRelDefinesByProperties( int id ) { m_id = id; m_entity_enum = IFCRELDEFINESBYPROPERTIES; }
IfcRelDefinesByProperties::~IfcRelDefinesByProperties() {}
shared_ptr<IfcPPObject> IfcRelDefinesByProperties::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelDefinesByProperties> copy_self( new IfcRelDefinesByProperties() );
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
	for( size_t ii=0; ii<m_RelatedObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedObjects.push_back( dynamic_pointer_cast<IfcObjectDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatingPropertyDefinition ) { copy_self->m_RelatingPropertyDefinition = dynamic_pointer_cast<IfcPropertySetDefinitionSelect>( m_RelatingPropertyDefinition->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelDefinesByProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELDEFINESBYPROPERTIES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_RelatedObjects );
	stream << ",";
	if( m_RelatingPropertyDefinition ) { m_RelatingPropertyDefinition->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcRelDefinesByProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelDefinesByProperties::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRelDefinesByProperties, expecting 6, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatingPropertyDefinition = IfcPropertySetDefinitionSelect::createObjectFromSTEP( args[5], map );
}
void IfcRelDefinesByProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelDefines::getAttributes( vec_attributes );
	if( m_RelatedObjects.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedObjects_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedObjects.begin(), m_RelatedObjects.end(), std::back_inserter( RelatedObjects_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedObjects", RelatedObjects_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "RelatingPropertyDefinition", m_RelatingPropertyDefinition ) );
}
void IfcRelDefinesByProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelDefines::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelDefinesByProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelDefines::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelDefinesByProperties> ptr_self = dynamic_pointer_cast<IfcRelDefinesByProperties>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelDefinesByProperties::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedObjects.size(); ++i )
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
void IfcRelDefinesByProperties::unlinkFromInverseCounterparts()
{
	IfcRelDefines::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcContext>  RelatedObjects_IfcContext = dynamic_pointer_cast<IfcContext>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcContext )
		{
			std::vector<weak_ptr<IfcRelDefinesByProperties> >& IsDefinedBy_inverse = RelatedObjects_IfcContext->m_IsDefinedBy_inverse;
			for( auto it_IsDefinedBy_inverse = IsDefinedBy_inverse.begin(); it_IsDefinedBy_inverse != IsDefinedBy_inverse.end(); )
			{
				shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_IsDefinedBy_inverse );
				if( self_candidate.get() == this )
				{
					it_IsDefinedBy_inverse= IsDefinedBy_inverse.erase( it_IsDefinedBy_inverse );
				}
				else
				{
					++it_IsDefinedBy_inverse;
				}
			}
		}
		shared_ptr<IfcObject>  RelatedObjects_IfcObject = dynamic_pointer_cast<IfcObject>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObject )
		{
			std::vector<weak_ptr<IfcRelDefinesByProperties> >& IsDefinedBy_inverse = RelatedObjects_IfcObject->m_IsDefinedBy_inverse;
			for( auto it_IsDefinedBy_inverse = IsDefinedBy_inverse.begin(); it_IsDefinedBy_inverse != IsDefinedBy_inverse.end(); )
			{
				shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_IsDefinedBy_inverse );
				if( self_candidate.get() == this )
				{
					it_IsDefinedBy_inverse= IsDefinedBy_inverse.erase( it_IsDefinedBy_inverse );
				}
				else
				{
					++it_IsDefinedBy_inverse;
				}
			}
		}
	}
	shared_ptr<IfcPropertySetDefinition>  RelatingPropertyDefinition_IfcPropertySetDefinition = dynamic_pointer_cast<IfcPropertySetDefinition>( m_RelatingPropertyDefinition );
	if( RelatingPropertyDefinition_IfcPropertySetDefinition )
	{
		std::vector<weak_ptr<IfcRelDefinesByProperties> >& DefinesOccurrence_inverse = RelatingPropertyDefinition_IfcPropertySetDefinition->m_DefinesOccurrence_inverse;
		for( auto it_DefinesOccurrence_inverse = DefinesOccurrence_inverse.begin(); it_DefinesOccurrence_inverse != DefinesOccurrence_inverse.end(); )
		{
			shared_ptr<IfcRelDefinesByProperties> self_candidate( *it_DefinesOccurrence_inverse );
			if( self_candidate.get() == this )
			{
				it_DefinesOccurrence_inverse= DefinesOccurrence_inverse.erase( it_DefinesOccurrence_inverse );
			}
			else
			{
				++it_DefinesOccurrence_inverse;
			}
		}
	}
}

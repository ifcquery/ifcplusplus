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
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetTemplate.h"
#include "include/IfcPropertySetTemplateTypeEnum.h"
#include "include/IfcPropertyTemplate.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByTemplate.h"
#include "include/IfcText.h"

// ENTITY IfcPropertySetTemplate 
IfcPropertySetTemplate::IfcPropertySetTemplate() { m_entity_enum = IFCPROPERTYSETTEMPLATE; }
IfcPropertySetTemplate::IfcPropertySetTemplate( int id ) { m_id = id; m_entity_enum = IFCPROPERTYSETTEMPLATE; }
IfcPropertySetTemplate::~IfcPropertySetTemplate() {}
shared_ptr<IfcPPObject> IfcPropertySetTemplate::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertySetTemplate> copy_self( new IfcPropertySetTemplate() );
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
	if( m_TemplateType ) { copy_self->m_TemplateType = dynamic_pointer_cast<IfcPropertySetTemplateTypeEnum>( m_TemplateType->getDeepCopy(options) ); }
	if( m_ApplicableEntity ) { copy_self->m_ApplicableEntity = dynamic_pointer_cast<IfcIdentifier>( m_ApplicableEntity->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_HasPropertyTemplates.size(); ++ii )
	{
		auto item_ii = m_HasPropertyTemplates[ii];
		if( item_ii )
		{
			copy_self->m_HasPropertyTemplates.push_back( dynamic_pointer_cast<IfcPropertyTemplate>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcPropertySetTemplate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYSETTEMPLATE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TemplateType ) { m_TemplateType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ApplicableEntity ) { m_ApplicableEntity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertyTemplates );
	stream << ");";
}
void IfcPropertySetTemplate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertySetTemplate::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPropertySetTemplate, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_TemplateType = IfcPropertySetTemplateTypeEnum::createObjectFromSTEP( args[4] );
	m_ApplicableEntity = IfcIdentifier::createObjectFromSTEP( args[5] );
	readEntityReferenceList( args[6], m_HasPropertyTemplates, map );
}
void IfcPropertySetTemplate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyTemplateDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TemplateType", m_TemplateType ) );
	vec_attributes.push_back( std::make_pair( "ApplicableEntity", m_ApplicableEntity ) );
	if( m_HasPropertyTemplates.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasPropertyTemplates_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_HasPropertyTemplates.begin(), m_HasPropertyTemplates.end(), std::back_inserter( HasPropertyTemplates_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "HasPropertyTemplates", HasPropertyTemplates_vec_object ) );
	}
}
void IfcPropertySetTemplate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPropertyTemplateDefinition::getAttributesInverse( vec_attributes_inverse );
	if( m_Defines_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Defines_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_Defines_inverse.size(); ++i )
		{
			if( !m_Defines_inverse[i].expired() )
			{
				Defines_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByTemplate>( m_Defines_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "Defines_inverse", Defines_inverse_vec_obj ) );
	}
}
void IfcPropertySetTemplate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyTemplateDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcPropertySetTemplate> ptr_self = dynamic_pointer_cast<IfcPropertySetTemplate>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcPropertySetTemplate::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_HasPropertyTemplates.size(); ++i )
	{
		if( m_HasPropertyTemplates[i] )
		{
			m_HasPropertyTemplates[i]->m_PartOfPsetTemplate_inverse.push_back( ptr_self );
		}
	}
}
void IfcPropertySetTemplate::unlinkFromInverseCounterparts()
{
	IfcPropertyTemplateDefinition::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_HasPropertyTemplates.size(); ++i )
	{
		if( m_HasPropertyTemplates[i] )
		{
			std::vector<weak_ptr<IfcPropertySetTemplate> >& PartOfPsetTemplate_inverse = m_HasPropertyTemplates[i]->m_PartOfPsetTemplate_inverse;
			for( auto it_PartOfPsetTemplate_inverse = PartOfPsetTemplate_inverse.begin(); it_PartOfPsetTemplate_inverse != PartOfPsetTemplate_inverse.end(); )
			{
				shared_ptr<IfcPropertySetTemplate> self_candidate( *it_PartOfPsetTemplate_inverse );
				if( self_candidate.get() == this )
				{
					it_PartOfPsetTemplate_inverse= PartOfPsetTemplate_inverse.erase( it_PartOfPsetTemplate_inverse );
				}
				else
				{
					++it_PartOfPsetTemplate_inverse;
				}
			}
		}
	}
}

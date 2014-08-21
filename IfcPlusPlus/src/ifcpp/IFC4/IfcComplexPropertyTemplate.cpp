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
#include "include/IfcComplexPropertyTemplate.h"
#include "include/IfcComplexPropertyTemplateTypeEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetTemplate.h"
#include "include/IfcPropertyTemplate.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcText.h"

// ENTITY IfcComplexPropertyTemplate 
IfcComplexPropertyTemplate::IfcComplexPropertyTemplate() {}
IfcComplexPropertyTemplate::IfcComplexPropertyTemplate( int id ) { m_id = id; }
IfcComplexPropertyTemplate::~IfcComplexPropertyTemplate() {}
shared_ptr<IfcPPObject> IfcComplexPropertyTemplate::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcComplexPropertyTemplate> copy_self( new IfcComplexPropertyTemplate() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( CreateCompressedGuidString22() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_UsageName ) { copy_self->m_UsageName = dynamic_pointer_cast<IfcLabel>( m_UsageName->getDeepCopy(options) ); }
	if( m_TemplateType ) { copy_self->m_TemplateType = dynamic_pointer_cast<IfcComplexPropertyTemplateTypeEnum>( m_TemplateType->getDeepCopy(options) ); }
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
void IfcComplexPropertyTemplate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOMPLEXPROPERTYTEMPLATE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UsageName ) { m_UsageName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TemplateType ) { m_TemplateType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertyTemplates );
	stream << ");";
}
void IfcComplexPropertyTemplate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcComplexPropertyTemplate::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcComplexPropertyTemplate, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_UsageName = IfcLabel::createObjectFromSTEP( args[4] );
	m_TemplateType = IfcComplexPropertyTemplateTypeEnum::createObjectFromSTEP( args[5] );
	readEntityReferenceList( args[6], m_HasPropertyTemplates, map );
}
void IfcComplexPropertyTemplate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyTemplate::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UsageName", m_UsageName ) );
	vec_attributes.push_back( std::make_pair( "TemplateType", m_TemplateType ) );
	if( m_HasPropertyTemplates.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasPropertyTemplates_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_HasPropertyTemplates.begin(), m_HasPropertyTemplates.end(), std::back_inserter( HasPropertyTemplates_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "HasPropertyTemplates", HasPropertyTemplates_vec_object ) );
	}
}
void IfcComplexPropertyTemplate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPropertyTemplate::getAttributesInverse( vec_attributes_inverse );
}
void IfcComplexPropertyTemplate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyTemplate::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcComplexPropertyTemplate> ptr_self = dynamic_pointer_cast<IfcComplexPropertyTemplate>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcComplexPropertyTemplate::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_HasPropertyTemplates.size(); ++i )
	{
		if( m_HasPropertyTemplates[i] )
		{
			m_HasPropertyTemplates[i]->m_PartOfComplexTemplate_inverse.push_back( ptr_self );
		}
	}
}
void IfcComplexPropertyTemplate::unlinkSelf()
{
	IfcPropertyTemplate::unlinkSelf();
	for( size_t i=0; i<m_HasPropertyTemplates.size(); ++i )
	{
		if( m_HasPropertyTemplates[i] )
		{
			std::vector<weak_ptr<IfcComplexPropertyTemplate> >& PartOfComplexTemplate_inverse = m_HasPropertyTemplates[i]->m_PartOfComplexTemplate_inverse;
			for( auto it_PartOfComplexTemplate_inverse = PartOfComplexTemplate_inverse.begin(); it_PartOfComplexTemplate_inverse != PartOfComplexTemplate_inverse.end(); ++it_PartOfComplexTemplate_inverse)
			{
				shared_ptr<IfcComplexPropertyTemplate> self_candidate( *it_PartOfComplexTemplate_inverse );
				if( self_candidate.get() == this )
				{
					PartOfComplexTemplate_inverse.erase( it_PartOfComplexTemplate_inverse );
					break;
				}
			}
		}
	}
}

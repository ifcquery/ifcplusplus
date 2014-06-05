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
IfcPropertySetTemplate::IfcPropertySetTemplate() {}
IfcPropertySetTemplate::IfcPropertySetTemplate( int id ) { m_id = id; }
IfcPropertySetTemplate::~IfcPropertySetTemplate() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertySetTemplate::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertySetTemplate> other = dynamic_pointer_cast<IfcPropertySetTemplate>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_TemplateType = other->m_TemplateType;
	m_ApplicableEntity = other->m_ApplicableEntity;
	m_HasPropertyTemplates = other->m_HasPropertyTemplates;
}
void IfcPropertySetTemplate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYSETTEMPLATE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TemplateType ) { m_TemplateType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ApplicableEntity ) { m_ApplicableEntity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertyTemplates );
	stream << ");";
}
void IfcPropertySetTemplate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertySetTemplate::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertySetTemplate, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcPropertySetTemplate, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_TemplateType = IfcPropertySetTemplateTypeEnum::createObjectFromStepData( args[4] );
	m_ApplicableEntity = IfcIdentifier::createObjectFromStepData( args[5] );
	readEntityReferenceList( args[6], m_HasPropertyTemplates, map );
}
void IfcPropertySetTemplate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyTemplateDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TemplateType", m_TemplateType ) );
	vec_attributes.push_back( std::make_pair( "ApplicableEntity", m_ApplicableEntity ) );
}
void IfcPropertySetTemplate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> Defines_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_Defines_inverse.size(); ++i ) { Defines_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByTemplate>( m_Defines_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "Defines_inverse", Defines_inverse_vec_obj ) );
}
void IfcPropertySetTemplate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyTemplateDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcPropertySetTemplate> ptr_self = dynamic_pointer_cast<IfcPropertySetTemplate>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcPropertySetTemplate::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_HasPropertyTemplates.size(); ++i )
	{
		if( m_HasPropertyTemplates[i] )
		{
			m_HasPropertyTemplates[i]->m_PartOfPsetTemplate_inverse.push_back( ptr_self );
		}
	}
}
void IfcPropertySetTemplate::unlinkSelf()
{
	IfcPropertyTemplateDefinition::unlinkSelf();
	for( int i=0; i<m_HasPropertyTemplates.size(); ++i )
	{
		if( m_HasPropertyTemplates[i] )
		{
			std::vector<weak_ptr<IfcPropertySetTemplate> >& PartOfPsetTemplate_inverse = m_HasPropertyTemplates[i]->m_PartOfPsetTemplate_inverse;
			std::vector<weak_ptr<IfcPropertySetTemplate> >::iterator it_PartOfPsetTemplate_inverse;
			for( it_PartOfPsetTemplate_inverse = PartOfPsetTemplate_inverse.begin(); it_PartOfPsetTemplate_inverse != PartOfPsetTemplate_inverse.end(); ++it_PartOfPsetTemplate_inverse)
			{
				shared_ptr<IfcPropertySetTemplate> self_candidate( *it_PartOfPsetTemplate_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					PartOfPsetTemplate_inverse.erase( it_PartOfPsetTemplate_inverse );
					break;
				}
			}
		}
	}
}

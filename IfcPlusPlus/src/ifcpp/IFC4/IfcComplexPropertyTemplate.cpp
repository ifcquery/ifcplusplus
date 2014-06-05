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

// method setEntity takes over all attributes from another instance of the class
void IfcComplexPropertyTemplate::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcComplexPropertyTemplate> other = dynamic_pointer_cast<IfcComplexPropertyTemplate>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_UsageName = other->m_UsageName;
	m_TemplateType = other->m_TemplateType;
	m_HasPropertyTemplates = other->m_HasPropertyTemplates;
}
void IfcComplexPropertyTemplate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCOMPLEXPROPERTYTEMPLATE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UsageName ) { m_UsageName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TemplateType ) { m_TemplateType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertyTemplates );
	stream << ");";
}
void IfcComplexPropertyTemplate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcComplexPropertyTemplate::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcComplexPropertyTemplate, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcComplexPropertyTemplate, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_UsageName = IfcLabel::createObjectFromStepData( args[4] );
	m_TemplateType = IfcComplexPropertyTemplateTypeEnum::createObjectFromStepData( args[5] );
	readEntityReferenceList( args[6], m_HasPropertyTemplates, map );
}
void IfcComplexPropertyTemplate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyTemplate::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UsageName", m_UsageName ) );
	vec_attributes.push_back( std::make_pair( "TemplateType", m_TemplateType ) );
}
void IfcComplexPropertyTemplate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcComplexPropertyTemplate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyTemplate::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcComplexPropertyTemplate> ptr_self = dynamic_pointer_cast<IfcComplexPropertyTemplate>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcComplexPropertyTemplate::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_HasPropertyTemplates.size(); ++i )
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
	for( int i=0; i<m_HasPropertyTemplates.size(); ++i )
	{
		if( m_HasPropertyTemplates[i] )
		{
			std::vector<weak_ptr<IfcComplexPropertyTemplate> >& PartOfComplexTemplate_inverse = m_HasPropertyTemplates[i]->m_PartOfComplexTemplate_inverse;
			std::vector<weak_ptr<IfcComplexPropertyTemplate> >::iterator it_PartOfComplexTemplate_inverse;
			for( it_PartOfComplexTemplate_inverse = PartOfComplexTemplate_inverse.begin(); it_PartOfComplexTemplate_inverse != PartOfComplexTemplate_inverse.end(); ++it_PartOfComplexTemplate_inverse)
			{
				shared_ptr<IfcComplexPropertyTemplate> self_candidate( *it_PartOfComplexTemplate_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					PartOfComplexTemplate_inverse.erase( it_PartOfComplexTemplate_inverse );
					break;
				}
			}
		}
	}
}

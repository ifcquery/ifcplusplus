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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertyEnumeration.h"
#include "include/IfcPropertySetTemplate.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcSimplePropertyTemplate.h"
#include "include/IfcSimplePropertyTemplateTypeEnum.h"
#include "include/IfcStateEnum.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"

// ENTITY IfcSimplePropertyTemplate 
IfcSimplePropertyTemplate::IfcSimplePropertyTemplate() {}
IfcSimplePropertyTemplate::IfcSimplePropertyTemplate( int id ) { m_id = id; }
IfcSimplePropertyTemplate::~IfcSimplePropertyTemplate() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSimplePropertyTemplate::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSimplePropertyTemplate> other = dynamic_pointer_cast<IfcSimplePropertyTemplate>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_TemplateType = other->m_TemplateType;
	m_PrimaryMeasureType = other->m_PrimaryMeasureType;
	m_SecondaryMeasureType = other->m_SecondaryMeasureType;
	m_Enumerators = other->m_Enumerators;
	m_PrimaryUnit = other->m_PrimaryUnit;
	m_SecondaryUnit = other->m_SecondaryUnit;
	m_Expression = other->m_Expression;
	m_AccessState = other->m_AccessState;
}
void IfcSimplePropertyTemplate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSIMPLEPROPERTYTEMPLATE" << "(";
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
	if( m_PrimaryMeasureType ) { m_PrimaryMeasureType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SecondaryMeasureType ) { m_SecondaryMeasureType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Enumerators ) { stream << "#" << m_Enumerators->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PrimaryUnit ) { m_PrimaryUnit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_SecondaryUnit ) { m_SecondaryUnit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_Expression ) { m_Expression->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_AccessState ) { m_AccessState->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSimplePropertyTemplate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSimplePropertyTemplate::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<12 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSimplePropertyTemplate, expecting 12, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>12 ){ std::cout << "Wrong parameter count for entity IfcSimplePropertyTemplate, expecting 12, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_TemplateType = IfcSimplePropertyTemplateTypeEnum::createObjectFromStepData( args[4] );
	m_PrimaryMeasureType = IfcLabel::createObjectFromStepData( args[5] );
	m_SecondaryMeasureType = IfcLabel::createObjectFromStepData( args[6] );
	readEntityReference( args[7], m_Enumerators, map );
	m_PrimaryUnit = IfcUnit::createObjectFromStepData( args[8], map );
	m_SecondaryUnit = IfcUnit::createObjectFromStepData( args[9], map );
	m_Expression = IfcLabel::createObjectFromStepData( args[10] );
	m_AccessState = IfcStateEnum::createObjectFromStepData( args[11] );
}
void IfcSimplePropertyTemplate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyTemplate::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TemplateType", m_TemplateType ) );
	vec_attributes.push_back( std::make_pair( "PrimaryMeasureType", m_PrimaryMeasureType ) );
	vec_attributes.push_back( std::make_pair( "SecondaryMeasureType", m_SecondaryMeasureType ) );
	vec_attributes.push_back( std::make_pair( "Enumerators", m_Enumerators ) );
	vec_attributes.push_back( std::make_pair( "PrimaryUnit", m_PrimaryUnit ) );
	vec_attributes.push_back( std::make_pair( "SecondaryUnit", m_SecondaryUnit ) );
	vec_attributes.push_back( std::make_pair( "Expression", m_Expression ) );
	vec_attributes.push_back( std::make_pair( "AccessState", m_AccessState ) );
}
void IfcSimplePropertyTemplate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSimplePropertyTemplate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyTemplate::setInverseCounterparts( ptr_self_entity );
}
void IfcSimplePropertyTemplate::unlinkSelf()
{
	IfcPropertyTemplate::unlinkSelf();
}

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
shared_ptr<IfcPPObject> IfcSimplePropertyTemplate::getDeepCopy()
{
	shared_ptr<IfcSimplePropertyTemplate> copy_self( new IfcSimplePropertyTemplate() );
	if( m_GlobalId ) { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy() ); }
	if( m_OwnerHistory ) { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_TemplateType ) { copy_self->m_TemplateType = dynamic_pointer_cast<IfcSimplePropertyTemplateTypeEnum>( m_TemplateType->getDeepCopy() ); }
	if( m_PrimaryMeasureType ) { copy_self->m_PrimaryMeasureType = dynamic_pointer_cast<IfcLabel>( m_PrimaryMeasureType->getDeepCopy() ); }
	if( m_SecondaryMeasureType ) { copy_self->m_SecondaryMeasureType = dynamic_pointer_cast<IfcLabel>( m_SecondaryMeasureType->getDeepCopy() ); }
	if( m_Enumerators ) { copy_self->m_Enumerators = dynamic_pointer_cast<IfcPropertyEnumeration>( m_Enumerators->getDeepCopy() ); }
	if( m_PrimaryUnit ) { copy_self->m_PrimaryUnit = dynamic_pointer_cast<IfcUnit>( m_PrimaryUnit->getDeepCopy() ); }
	if( m_SecondaryUnit ) { copy_self->m_SecondaryUnit = dynamic_pointer_cast<IfcUnit>( m_SecondaryUnit->getDeepCopy() ); }
	if( m_Expression ) { copy_self->m_Expression = dynamic_pointer_cast<IfcLabel>( m_Expression->getDeepCopy() ); }
	if( m_AccessState ) { copy_self->m_AccessState = dynamic_pointer_cast<IfcStateEnum>( m_AccessState->getDeepCopy() ); }
	return copy_self;
}
void IfcSimplePropertyTemplate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSIMPLEPROPERTYTEMPLATE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TemplateType ) { m_TemplateType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PrimaryMeasureType ) { m_PrimaryMeasureType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SecondaryMeasureType ) { m_SecondaryMeasureType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Enumerators ) { stream << "#" << m_Enumerators->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PrimaryUnit ) { m_PrimaryUnit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_SecondaryUnit ) { m_SecondaryUnit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_Expression ) { m_Expression->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_AccessState ) { m_AccessState->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSimplePropertyTemplate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSimplePropertyTemplate::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	IfcPropertyTemplate::getAttributesInverse( vec_attributes_inverse );
}
void IfcSimplePropertyTemplate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyTemplate::setInverseCounterparts( ptr_self_entity );
}
void IfcSimplePropertyTemplate::unlinkSelf()
{
	IfcPropertyTemplate::unlinkSelf();
}

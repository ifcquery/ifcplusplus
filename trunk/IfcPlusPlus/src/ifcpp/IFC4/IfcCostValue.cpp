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
#include "include/IfcAppliedValue.h"
#include "include/IfcAppliedValueSelect.h"
#include "include/IfcArithmeticOperatorEnum.h"
#include "include/IfcCostValue.h"
#include "include/IfcDate.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcMeasureWithUnit.h"
#include "include/IfcText.h"

// ENTITY IfcCostValue 
IfcCostValue::IfcCostValue() {}
IfcCostValue::IfcCostValue( int id ) { m_id = id; }
IfcCostValue::~IfcCostValue() {}

// method setEntity takes over all attributes from another instance of the class
void IfcCostValue::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcCostValue> other = dynamic_pointer_cast<IfcCostValue>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_AppliedValue = other->m_AppliedValue;
	m_UnitBasis = other->m_UnitBasis;
	m_ApplicableDate = other->m_ApplicableDate;
	m_FixedUntilDate = other->m_FixedUntilDate;
	m_Category = other->m_Category;
	m_Condition = other->m_Condition;
	m_ArithmeticOperator = other->m_ArithmeticOperator;
	m_Components = other->m_Components;
}
void IfcCostValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCOSTVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_AppliedValue ) { m_AppliedValue->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_UnitBasis ) { stream << "#" << m_UnitBasis->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_ApplicableDate ) { m_ApplicableDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FixedUntilDate ) { m_FixedUntilDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Condition ) { m_Condition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ArithmeticOperator ) { m_ArithmeticOperator->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Components );
	stream << ");";
}
void IfcCostValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCostValue::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCostValue, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcCostValue, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_AppliedValue = IfcAppliedValueSelect::createObjectFromStepData( args[2], map );
	readEntityReference( args[3], m_UnitBasis, map );
	m_ApplicableDate = IfcDate::createObjectFromStepData( args[4] );
	m_FixedUntilDate = IfcDate::createObjectFromStepData( args[5] );
	m_Category = IfcLabel::createObjectFromStepData( args[6] );
	m_Condition = IfcLabel::createObjectFromStepData( args[7] );
	m_ArithmeticOperator = IfcArithmeticOperatorEnum::createObjectFromStepData( args[8] );
	readEntityReferenceList( args[9], m_Components, map );
}
void IfcCostValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcAppliedValue::getAttributes( vec_attributes );
}
void IfcCostValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcCostValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcAppliedValue::setInverseCounterparts( ptr_self_entity );
}
void IfcCostValue::unlinkSelf()
{
	IfcAppliedValue::unlinkSelf();
}

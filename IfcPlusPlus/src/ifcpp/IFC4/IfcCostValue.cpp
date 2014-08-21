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
shared_ptr<IfcPPObject> IfcCostValue::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCostValue> copy_self( new IfcCostValue() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_AppliedValue ) { copy_self->m_AppliedValue = dynamic_pointer_cast<IfcAppliedValueSelect>( m_AppliedValue->getDeepCopy(options) ); }
	if( m_UnitBasis ) { copy_self->m_UnitBasis = dynamic_pointer_cast<IfcMeasureWithUnit>( m_UnitBasis->getDeepCopy(options) ); }
	if( m_ApplicableDate ) { copy_self->m_ApplicableDate = dynamic_pointer_cast<IfcDate>( m_ApplicableDate->getDeepCopy(options) ); }
	if( m_FixedUntilDate ) { copy_self->m_FixedUntilDate = dynamic_pointer_cast<IfcDate>( m_FixedUntilDate->getDeepCopy(options) ); }
	if( m_Category ) { copy_self->m_Category = dynamic_pointer_cast<IfcLabel>( m_Category->getDeepCopy(options) ); }
	if( m_Condition ) { copy_self->m_Condition = dynamic_pointer_cast<IfcLabel>( m_Condition->getDeepCopy(options) ); }
	if( m_ArithmeticOperator ) { copy_self->m_ArithmeticOperator = dynamic_pointer_cast<IfcArithmeticOperatorEnum>( m_ArithmeticOperator->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Components.size(); ++ii )
	{
		auto item_ii = m_Components[ii];
		if( item_ii )
		{
			copy_self->m_Components.push_back( dynamic_pointer_cast<IfcAppliedValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcCostValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOSTVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_AppliedValue ) { m_AppliedValue->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_UnitBasis ) { stream << "#" << m_UnitBasis->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_ApplicableDate ) { m_ApplicableDate->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_FixedUntilDate ) { m_FixedUntilDate->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Condition ) { m_Condition->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ArithmeticOperator ) { m_ArithmeticOperator->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Components );
	stream << ");";
}
void IfcCostValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCostValue::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCostValue, expecting 10, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_AppliedValue = IfcAppliedValueSelect::createObjectFromSTEP( args[2], map );
	readEntityReference( args[3], m_UnitBasis, map );
	m_ApplicableDate = IfcDate::createObjectFromSTEP( args[4] );
	m_FixedUntilDate = IfcDate::createObjectFromSTEP( args[5] );
	m_Category = IfcLabel::createObjectFromSTEP( args[6] );
	m_Condition = IfcLabel::createObjectFromSTEP( args[7] );
	m_ArithmeticOperator = IfcArithmeticOperatorEnum::createObjectFromSTEP( args[8] );
	readEntityReferenceList( args[9], m_Components, map );
}
void IfcCostValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcAppliedValue::getAttributes( vec_attributes );
}
void IfcCostValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcAppliedValue::getAttributesInverse( vec_attributes_inverse );
}
void IfcCostValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcAppliedValue::setInverseCounterparts( ptr_self_entity );
}
void IfcCostValue::unlinkSelf()
{
	IfcAppliedValue::unlinkSelf();
}

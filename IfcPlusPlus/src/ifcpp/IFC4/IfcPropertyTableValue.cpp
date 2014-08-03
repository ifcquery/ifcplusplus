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
#include "include/IfcComplexProperty.h"
#include "include/IfcCurveInterpolationEnum.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcPropertyTableValue.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyTableValue 
IfcPropertyTableValue::IfcPropertyTableValue() {}
IfcPropertyTableValue::IfcPropertyTableValue( int id ) { m_id = id; }
IfcPropertyTableValue::~IfcPropertyTableValue() {}
shared_ptr<IfcPPObject> IfcPropertyTableValue::getDeepCopy()
{
	shared_ptr<IfcPropertyTableValue> copy_self( new IfcPropertyTableValue() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	for( size_t ii=0; ii<m_DefiningValues.size(); ++ii )
	{
		auto item_ii = m_DefiningValues[ii];
		if( item_ii )
		{
			copy_self->m_DefiningValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_DefinedValues.size(); ++ii )
	{
		auto item_ii = m_DefinedValues[ii];
		if( item_ii )
		{
			copy_self->m_DefinedValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_Expression ) { copy_self->m_Expression = dynamic_pointer_cast<IfcText>( m_Expression->getDeepCopy() ); }
	if( m_DefiningUnit ) { copy_self->m_DefiningUnit = dynamic_pointer_cast<IfcUnit>( m_DefiningUnit->getDeepCopy() ); }
	if( m_DefinedUnit ) { copy_self->m_DefinedUnit = dynamic_pointer_cast<IfcUnit>( m_DefinedUnit->getDeepCopy() ); }
	if( m_CurveInterpolation ) { copy_self->m_CurveInterpolation = dynamic_pointer_cast<IfcCurveInterpolationEnum>( m_CurveInterpolation->getDeepCopy() ); }
	return copy_self;
}
void IfcPropertyTableValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYTABLEVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeTypeList( stream, m_DefiningValues, true );
	stream << ",";
	writeTypeList( stream, m_DefinedValues, true );
	stream << ",";
	if( m_Expression ) { m_Expression->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DefiningUnit ) { m_DefiningUnit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_DefinedUnit ) { m_DefinedUnit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_CurveInterpolation ) { m_CurveInterpolation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertyTableValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyTableValue::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertyTableValue, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcPropertyTableValue, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readSelectList( args[2], m_DefiningValues, map );
	readSelectList( args[3], m_DefinedValues, map );
	m_Expression = IfcText::createObjectFromStepData( args[4] );
	m_DefiningUnit = IfcUnit::createObjectFromStepData( args[5], map );
	m_DefinedUnit = IfcUnit::createObjectFromStepData( args[6], map );
	m_CurveInterpolation = IfcCurveInterpolationEnum::createObjectFromStepData( args[7] );
}
void IfcPropertyTableValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSimpleProperty::getAttributes( vec_attributes );
	if( m_DefiningValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DefiningValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_DefiningValues.begin(), m_DefiningValues.end(), std::back_inserter( DefiningValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "DefiningValues", DefiningValues_vec_object ) );
	}
	if( m_DefinedValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DefinedValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_DefinedValues.begin(), m_DefinedValues.end(), std::back_inserter( DefinedValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "DefinedValues", DefinedValues_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "Expression", m_Expression ) );
	vec_attributes.push_back( std::make_pair( "DefiningUnit", m_DefiningUnit ) );
	vec_attributes.push_back( std::make_pair( "DefinedUnit", m_DefinedUnit ) );
	vec_attributes.push_back( std::make_pair( "CurveInterpolation", m_CurveInterpolation ) );
}
void IfcPropertyTableValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSimpleProperty::getAttributesInverse( vec_attributes_inverse );
}
void IfcPropertyTableValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSimpleProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyTableValue::unlinkSelf()
{
	IfcSimpleProperty::unlinkSelf();
}

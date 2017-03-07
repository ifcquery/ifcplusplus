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
#include "include/IfcComplexProperty.h"
#include "include/IfcCurveInterpolationEnum.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcPropertyTableValue.h"
#include "include/IfcResourceApprovalRelationship.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyTableValue 
IfcPropertyTableValue::IfcPropertyTableValue() { m_entity_enum = IFCPROPERTYTABLEVALUE; }
IfcPropertyTableValue::IfcPropertyTableValue( int id ) { m_id = id; m_entity_enum = IFCPROPERTYTABLEVALUE; }
IfcPropertyTableValue::~IfcPropertyTableValue() {}
shared_ptr<IfcPPObject> IfcPropertyTableValue::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertyTableValue> copy_self( new IfcPropertyTableValue() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_DefiningValues.size(); ++ii )
	{
		auto item_ii = m_DefiningValues[ii];
		if( item_ii )
		{
			copy_self->m_DefiningValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_DefinedValues.size(); ++ii )
	{
		auto item_ii = m_DefinedValues[ii];
		if( item_ii )
		{
			copy_self->m_DefinedValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Expression ) { copy_self->m_Expression = dynamic_pointer_cast<IfcText>( m_Expression->getDeepCopy(options) ); }
	if( m_DefiningUnit ) { copy_self->m_DefiningUnit = dynamic_pointer_cast<IfcUnit>( m_DefiningUnit->getDeepCopy(options) ); }
	if( m_DefinedUnit ) { copy_self->m_DefinedUnit = dynamic_pointer_cast<IfcUnit>( m_DefinedUnit->getDeepCopy(options) ); }
	if( m_CurveInterpolation ) { copy_self->m_CurveInterpolation = dynamic_pointer_cast<IfcCurveInterpolationEnum>( m_CurveInterpolation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPropertyTableValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYTABLEVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_DefiningValues.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcValue>& type_object = m_DefiningValues[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_DefinedValues.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcValue>& type_object = m_DefinedValues[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
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
void IfcPropertyTableValue::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPropertyTableValue, expecting 8, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readSelectList( args[2], m_DefiningValues, map );
	readSelectList( args[3], m_DefinedValues, map );
	m_Expression = IfcText::createObjectFromSTEP( args[4] );
	m_DefiningUnit = IfcUnit::createObjectFromSTEP( args[5], map );
	m_DefinedUnit = IfcUnit::createObjectFromSTEP( args[6], map );
	m_CurveInterpolation = IfcCurveInterpolationEnum::createObjectFromSTEP( args[7] );
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
void IfcPropertyTableValue::unlinkFromInverseCounterparts()
{
	IfcSimpleProperty::unlinkFromInverseCounterparts();
}

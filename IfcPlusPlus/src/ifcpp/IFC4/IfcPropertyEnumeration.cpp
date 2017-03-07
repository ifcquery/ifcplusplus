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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcPropertyEnumeration.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyEnumeration 
IfcPropertyEnumeration::IfcPropertyEnumeration() { m_entity_enum = IFCPROPERTYENUMERATION; }
IfcPropertyEnumeration::IfcPropertyEnumeration( int id ) { m_id = id; m_entity_enum = IFCPROPERTYENUMERATION; }
IfcPropertyEnumeration::~IfcPropertyEnumeration() {}
shared_ptr<IfcPPObject> IfcPropertyEnumeration::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertyEnumeration> copy_self( new IfcPropertyEnumeration() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_EnumerationValues.size(); ++ii )
	{
		auto item_ii = m_EnumerationValues[ii];
		if( item_ii )
		{
			copy_self->m_EnumerationValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcUnit>( m_Unit->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPropertyEnumeration::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYENUMERATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_EnumerationValues.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcValue>& type_object = m_EnumerationValues[ii];
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
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcPropertyEnumeration::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyEnumeration::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPropertyEnumeration, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	readSelectList( args[1], m_EnumerationValues, map );
	m_Unit = IfcUnit::createObjectFromSTEP( args[2], map );
}
void IfcPropertyEnumeration::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyAbstraction::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	if( m_EnumerationValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> EnumerationValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_EnumerationValues.begin(), m_EnumerationValues.end(), std::back_inserter( EnumerationValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "EnumerationValues", EnumerationValues_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
}
void IfcPropertyEnumeration::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPropertyAbstraction::getAttributesInverse( vec_attributes_inverse );
}
void IfcPropertyEnumeration::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyAbstraction::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyEnumeration::unlinkFromInverseCounterparts()
{
	IfcPropertyAbstraction::unlinkFromInverseCounterparts();
}

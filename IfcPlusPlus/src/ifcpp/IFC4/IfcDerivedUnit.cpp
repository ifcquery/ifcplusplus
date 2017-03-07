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
#include "include/IfcDerivedUnit.h"
#include "include/IfcDerivedUnitElement.h"
#include "include/IfcDerivedUnitEnum.h"
#include "include/IfcLabel.h"

// ENTITY IfcDerivedUnit 
IfcDerivedUnit::IfcDerivedUnit() { m_entity_enum = IFCDERIVEDUNIT; }
IfcDerivedUnit::IfcDerivedUnit( int id ) { m_id = id; m_entity_enum = IFCDERIVEDUNIT; }
IfcDerivedUnit::~IfcDerivedUnit() {}
shared_ptr<IfcPPObject> IfcDerivedUnit::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDerivedUnit> copy_self( new IfcDerivedUnit() );
	for( size_t ii=0; ii<m_Elements.size(); ++ii )
	{
		auto item_ii = m_Elements[ii];
		if( item_ii )
		{
			copy_self->m_Elements.push_back( dynamic_pointer_cast<IfcDerivedUnitElement>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_UnitType ) { copy_self->m_UnitType = dynamic_pointer_cast<IfcDerivedUnitEnum>( m_UnitType->getDeepCopy(options) ); }
	if( m_UserDefinedType ) { copy_self->m_UserDefinedType = dynamic_pointer_cast<IfcLabel>( m_UserDefinedType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcDerivedUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDERIVEDUNIT" << "(";
	writeEntityList( stream, m_Elements );
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedType ) { m_UserDefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDerivedUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDerivedUnit::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcDerivedUnit, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_Elements, map );
	m_UnitType = IfcDerivedUnitEnum::createObjectFromSTEP( args[1] );
	m_UserDefinedType = IfcLabel::createObjectFromSTEP( args[2] );
}
void IfcDerivedUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_Elements.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Elements_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Elements.begin(), m_Elements.end(), std::back_inserter( Elements_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Elements", Elements_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "UnitType", m_UnitType ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedType", m_UserDefinedType ) );
}
void IfcDerivedUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcDerivedUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcDerivedUnit::unlinkFromInverseCounterparts()
{
}

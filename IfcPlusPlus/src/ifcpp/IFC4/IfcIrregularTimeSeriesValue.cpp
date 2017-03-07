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
#include "include/IfcDateTime.h"
#include "include/IfcIrregularTimeSeriesValue.h"
#include "include/IfcValue.h"

// ENTITY IfcIrregularTimeSeriesValue 
IfcIrregularTimeSeriesValue::IfcIrregularTimeSeriesValue() { m_entity_enum = IFCIRREGULARTIMESERIESVALUE; }
IfcIrregularTimeSeriesValue::IfcIrregularTimeSeriesValue( int id ) { m_id = id; m_entity_enum = IFCIRREGULARTIMESERIESVALUE; }
IfcIrregularTimeSeriesValue::~IfcIrregularTimeSeriesValue() {}
shared_ptr<IfcPPObject> IfcIrregularTimeSeriesValue::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcIrregularTimeSeriesValue> copy_self( new IfcIrregularTimeSeriesValue() );
	if( m_TimeStamp ) { copy_self->m_TimeStamp = dynamic_pointer_cast<IfcDateTime>( m_TimeStamp->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ListValues.size(); ++ii )
	{
		auto item_ii = m_ListValues[ii];
		if( item_ii )
		{
			copy_self->m_ListValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcIrregularTimeSeriesValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCIRREGULARTIMESERIESVALUE" << "(";
	if( m_TimeStamp ) { m_TimeStamp->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_ListValues.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcValue>& type_object = m_ListValues[ii];
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
	stream << ");";
}
void IfcIrregularTimeSeriesValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIrregularTimeSeriesValue::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcIrregularTimeSeriesValue, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_TimeStamp = IfcDateTime::createObjectFromSTEP( args[0] );
	readSelectList( args[1], m_ListValues, map );
}
void IfcIrregularTimeSeriesValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "TimeStamp", m_TimeStamp ) );
	if( m_ListValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ListValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ListValues.begin(), m_ListValues.end(), std::back_inserter( ListValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ListValues", ListValues_vec_object ) );
	}
}
void IfcIrregularTimeSeriesValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcIrregularTimeSeriesValue::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcIrregularTimeSeriesValue::unlinkFromInverseCounterparts()
{
}

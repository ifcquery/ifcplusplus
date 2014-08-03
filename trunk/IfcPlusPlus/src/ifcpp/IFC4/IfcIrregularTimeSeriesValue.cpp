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
#include "include/IfcDateTime.h"
#include "include/IfcIrregularTimeSeriesValue.h"
#include "include/IfcValue.h"

// ENTITY IfcIrregularTimeSeriesValue 
IfcIrregularTimeSeriesValue::IfcIrregularTimeSeriesValue() {}
IfcIrregularTimeSeriesValue::IfcIrregularTimeSeriesValue( int id ) { m_id = id; }
IfcIrregularTimeSeriesValue::~IfcIrregularTimeSeriesValue() {}
shared_ptr<IfcPPObject> IfcIrregularTimeSeriesValue::getDeepCopy()
{
	shared_ptr<IfcIrregularTimeSeriesValue> copy_self( new IfcIrregularTimeSeriesValue() );
	if( m_TimeStamp ) { copy_self->m_TimeStamp = dynamic_pointer_cast<IfcDateTime>( m_TimeStamp->getDeepCopy() ); }
	for( size_t ii=0; ii<m_ListValues.size(); ++ii )
	{
		auto item_ii = m_ListValues[ii];
		if( item_ii )
		{
			copy_self->m_ListValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy() ) );
		}
	}
	return copy_self;
}
void IfcIrregularTimeSeriesValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCIRREGULARTIMESERIESVALUE" << "(";
	if( m_TimeStamp ) { m_TimeStamp->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_ListValues, true );
	stream << ");";
}
void IfcIrregularTimeSeriesValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIrregularTimeSeriesValue::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcIrregularTimeSeriesValue, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcIrregularTimeSeriesValue, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_TimeStamp = IfcDateTime::createObjectFromStepData( args[0] );
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
void IfcIrregularTimeSeriesValue::unlinkSelf()
{
}

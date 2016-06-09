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
#include "include/IfcTimeSeriesValue.h"
#include "include/IfcValue.h"

// ENTITY IfcTimeSeriesValue 
IfcTimeSeriesValue::IfcTimeSeriesValue() { m_entity_enum = IFCTIMESERIESVALUE; }
IfcTimeSeriesValue::IfcTimeSeriesValue( int id ) { m_id = id; m_entity_enum = IFCTIMESERIESVALUE; }
IfcTimeSeriesValue::~IfcTimeSeriesValue() {}
shared_ptr<IfcPPObject> IfcTimeSeriesValue::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTimeSeriesValue> copy_self( new IfcTimeSeriesValue() );
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
void IfcTimeSeriesValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTIMESERIESVALUE" << "(";
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
void IfcTimeSeriesValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTimeSeriesValue::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTimeSeriesValue, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readSelectList( args[0], m_ListValues, map );
}
void IfcTimeSeriesValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_ListValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ListValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ListValues.begin(), m_ListValues.end(), std::back_inserter( ListValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ListValues", ListValues_vec_object ) );
	}
}
void IfcTimeSeriesValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTimeSeriesValue::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcTimeSeriesValue::unlinkFromInverseCounterparts()
{
}

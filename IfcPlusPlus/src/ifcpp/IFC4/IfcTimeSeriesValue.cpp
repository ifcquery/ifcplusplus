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
#include "include/IfcTimeSeriesValue.h"
#include "include/IfcValue.h"

// ENTITY IfcTimeSeriesValue 
IfcTimeSeriesValue::IfcTimeSeriesValue() {}
IfcTimeSeriesValue::IfcTimeSeriesValue( int id ) { m_id = id; }
IfcTimeSeriesValue::~IfcTimeSeriesValue() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTimeSeriesValue::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTimeSeriesValue> other = dynamic_pointer_cast<IfcTimeSeriesValue>(other_entity);
	if( !other) { return; }
	m_ListValues = other->m_ListValues;
}
void IfcTimeSeriesValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTIMESERIESVALUE" << "(";
	writeTypeList( stream, m_ListValues, true );
	stream << ");";
}
void IfcTimeSeriesValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTimeSeriesValue::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTimeSeriesValue, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcTimeSeriesValue, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readSelectList( args[0], m_ListValues, map );
}
void IfcTimeSeriesValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	shared_ptr<IfcPPAttributeObjectVector> ListValues_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_ListValues.begin(), m_ListValues.end(), std::back_inserter( ListValues_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "ListValues", ListValues_vec_object ) );
}
void IfcTimeSeriesValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTimeSeriesValue::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcTimeSeriesValue::unlinkSelf()
{
}

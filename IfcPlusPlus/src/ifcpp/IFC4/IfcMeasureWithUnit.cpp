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
#include "include/IfcMeasureWithUnit.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcMeasureWithUnit 
IfcMeasureWithUnit::IfcMeasureWithUnit() {}
IfcMeasureWithUnit::IfcMeasureWithUnit( int id ) { m_id = id; }
IfcMeasureWithUnit::~IfcMeasureWithUnit() {}
shared_ptr<IfcPPObject> IfcMeasureWithUnit::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMeasureWithUnit> copy_self( new IfcMeasureWithUnit() );
	if( m_ValueComponent ) { copy_self->m_ValueComponent = dynamic_pointer_cast<IfcValue>( m_ValueComponent->getDeepCopy(options) ); }
	if( m_UnitComponent ) { copy_self->m_UnitComponent = dynamic_pointer_cast<IfcUnit>( m_UnitComponent->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMeasureWithUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMEASUREWITHUNIT" << "(";
	if( m_ValueComponent ) { m_ValueComponent->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_UnitComponent ) { m_UnitComponent->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcMeasureWithUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMeasureWithUnit::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMeasureWithUnit, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_ValueComponent = IfcValue::createObjectFromSTEP( args[0], map );
	m_UnitComponent = IfcUnit::createObjectFromSTEP( args[1], map );
}
void IfcMeasureWithUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "ValueComponent", m_ValueComponent ) );
	vec_attributes.push_back( std::make_pair( "UnitComponent", m_UnitComponent ) );
}
void IfcMeasureWithUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMeasureWithUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMeasureWithUnit::unlinkSelf()
{
}

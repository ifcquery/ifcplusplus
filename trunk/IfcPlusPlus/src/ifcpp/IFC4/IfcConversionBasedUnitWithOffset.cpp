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
#include "include/IfcConversionBasedUnitWithOffset.h"
#include "include/IfcDimensionalExponents.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcMeasureWithUnit.h"
#include "include/IfcReal.h"
#include "include/IfcUnitEnum.h"

// ENTITY IfcConversionBasedUnitWithOffset 
IfcConversionBasedUnitWithOffset::IfcConversionBasedUnitWithOffset() {}
IfcConversionBasedUnitWithOffset::IfcConversionBasedUnitWithOffset( int id ) { m_id = id; }
IfcConversionBasedUnitWithOffset::~IfcConversionBasedUnitWithOffset() {}

// method setEntity takes over all attributes from another instance of the class
void IfcConversionBasedUnitWithOffset::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcConversionBasedUnitWithOffset> other = dynamic_pointer_cast<IfcConversionBasedUnitWithOffset>(other_entity);
	if( !other) { return; }
	m_Dimensions = other->m_Dimensions;
	m_UnitType = other->m_UnitType;
	m_Name = other->m_Name;
	m_ConversionFactor = other->m_ConversionFactor;
	m_ConversionOffset = other->m_ConversionOffset;
}
void IfcConversionBasedUnitWithOffset::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCONVERSIONBASEDUNITWITHOFFSET" << "(";
	if( m_Dimensions ) { stream << "#" << m_Dimensions->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ConversionFactor ) { stream << "#" << m_ConversionFactor->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_ConversionOffset ) { m_ConversionOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcConversionBasedUnitWithOffset::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConversionBasedUnitWithOffset::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcConversionBasedUnitWithOffset, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcConversionBasedUnitWithOffset, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Dimensions, map );
	m_UnitType = IfcUnitEnum::createObjectFromStepData( args[1] );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	readEntityReference( args[3], m_ConversionFactor, map );
	m_ConversionOffset = IfcReal::createObjectFromStepData( args[4] );
}
void IfcConversionBasedUnitWithOffset::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConversionBasedUnit::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ConversionOffset", m_ConversionOffset ) );
}
void IfcConversionBasedUnitWithOffset::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcConversionBasedUnitWithOffset::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConversionBasedUnit::setInverseCounterparts( ptr_self_entity );
}
void IfcConversionBasedUnitWithOffset::unlinkSelf()
{
	IfcConversionBasedUnit::unlinkSelf();
}

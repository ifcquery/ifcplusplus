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
#include "include/IfcCurrencyRelationship.h"
#include "include/IfcDateTime.h"
#include "include/IfcLabel.h"
#include "include/IfcLibraryInformation.h"
#include "include/IfcMonetaryUnit.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcText.h"

// ENTITY IfcCurrencyRelationship 
IfcCurrencyRelationship::IfcCurrencyRelationship() {}
IfcCurrencyRelationship::IfcCurrencyRelationship( int id ) { m_id = id; }
IfcCurrencyRelationship::~IfcCurrencyRelationship() {}

// method setEntity takes over all attributes from another instance of the class
void IfcCurrencyRelationship::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcCurrencyRelationship> other = dynamic_pointer_cast<IfcCurrencyRelationship>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingMonetaryUnit = other->m_RelatingMonetaryUnit;
	m_RelatedMonetaryUnit = other->m_RelatedMonetaryUnit;
	m_ExchangeRate = other->m_ExchangeRate;
	m_RateDateTime = other->m_RateDateTime;
	m_RateSource = other->m_RateSource;
}
void IfcCurrencyRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCURRENCYRELATIONSHIP" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingMonetaryUnit ) { stream << "#" << m_RelatingMonetaryUnit->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RelatedMonetaryUnit ) { stream << "#" << m_RelatedMonetaryUnit->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_ExchangeRate ) { m_ExchangeRate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RateDateTime ) { m_RateDateTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RateSource ) { stream << "#" << m_RateSource->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcCurrencyRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurrencyRelationship::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCurrencyRelationship, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcCurrencyRelationship, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_RelatingMonetaryUnit, map );
	readEntityReference( args[3], m_RelatedMonetaryUnit, map );
	m_ExchangeRate = IfcPositiveRatioMeasure::createObjectFromStepData( args[4] );
	m_RateDateTime = IfcDateTime::createObjectFromStepData( args[5] );
	readEntityReference( args[6], m_RateSource, map );
}
void IfcCurrencyRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcResourceLevelRelationship::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingMonetaryUnit", m_RelatingMonetaryUnit ) );
	vec_attributes.push_back( std::make_pair( "RelatedMonetaryUnit", m_RelatedMonetaryUnit ) );
	vec_attributes.push_back( std::make_pair( "ExchangeRate", m_ExchangeRate ) );
	vec_attributes.push_back( std::make_pair( "RateDateTime", m_RateDateTime ) );
	vec_attributes.push_back( std::make_pair( "RateSource", m_RateSource ) );
}
void IfcCurrencyRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcCurrencyRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcResourceLevelRelationship::setInverseCounterparts( ptr_self_entity );
}
void IfcCurrencyRelationship::unlinkSelf()
{
	IfcResourceLevelRelationship::unlinkSelf();
}

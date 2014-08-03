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
#include "include/IfcAreaMeasure.h"
#include "include/IfcCountMeasure.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcReinforcementBarProperties.h"
#include "include/IfcReinforcingBarSurfaceEnum.h"

// ENTITY IfcReinforcementBarProperties 
IfcReinforcementBarProperties::IfcReinforcementBarProperties() {}
IfcReinforcementBarProperties::IfcReinforcementBarProperties( int id ) { m_id = id; }
IfcReinforcementBarProperties::~IfcReinforcementBarProperties() {}
shared_ptr<IfcPPObject> IfcReinforcementBarProperties::getDeepCopy()
{
	shared_ptr<IfcReinforcementBarProperties> copy_self( new IfcReinforcementBarProperties() );
	if( m_TotalCrossSectionArea ) { copy_self->m_TotalCrossSectionArea = dynamic_pointer_cast<IfcAreaMeasure>( m_TotalCrossSectionArea->getDeepCopy() ); }
	if( m_SteelGrade ) { copy_self->m_SteelGrade = dynamic_pointer_cast<IfcLabel>( m_SteelGrade->getDeepCopy() ); }
	if( m_BarSurface ) { copy_self->m_BarSurface = dynamic_pointer_cast<IfcReinforcingBarSurfaceEnum>( m_BarSurface->getDeepCopy() ); }
	if( m_EffectiveDepth ) { copy_self->m_EffectiveDepth = dynamic_pointer_cast<IfcLengthMeasure>( m_EffectiveDepth->getDeepCopy() ); }
	if( m_NominalBarDiameter ) { copy_self->m_NominalBarDiameter = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_NominalBarDiameter->getDeepCopy() ); }
	if( m_BarCount ) { copy_self->m_BarCount = dynamic_pointer_cast<IfcCountMeasure>( m_BarCount->getDeepCopy() ); }
	return copy_self;
}
void IfcReinforcementBarProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREINFORCEMENTBARPROPERTIES" << "(";
	if( m_TotalCrossSectionArea ) { m_TotalCrossSectionArea->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SteelGrade ) { m_SteelGrade->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BarSurface ) { m_BarSurface->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EffectiveDepth ) { m_EffectiveDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_NominalBarDiameter ) { m_NominalBarDiameter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BarCount ) { m_BarCount->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcReinforcementBarProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcReinforcementBarProperties::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcReinforcementBarProperties, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcReinforcementBarProperties, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_TotalCrossSectionArea = IfcAreaMeasure::createObjectFromStepData( args[0] );
	m_SteelGrade = IfcLabel::createObjectFromStepData( args[1] );
	m_BarSurface = IfcReinforcingBarSurfaceEnum::createObjectFromStepData( args[2] );
	m_EffectiveDepth = IfcLengthMeasure::createObjectFromStepData( args[3] );
	m_NominalBarDiameter = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
	m_BarCount = IfcCountMeasure::createObjectFromStepData( args[5] );
}
void IfcReinforcementBarProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedProperties::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TotalCrossSectionArea", m_TotalCrossSectionArea ) );
	vec_attributes.push_back( std::make_pair( "SteelGrade", m_SteelGrade ) );
	vec_attributes.push_back( std::make_pair( "BarSurface", m_BarSurface ) );
	vec_attributes.push_back( std::make_pair( "EffectiveDepth", m_EffectiveDepth ) );
	vec_attributes.push_back( std::make_pair( "NominalBarDiameter", m_NominalBarDiameter ) );
	vec_attributes.push_back( std::make_pair( "BarCount", m_BarCount ) );
}
void IfcReinforcementBarProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedProperties::getAttributesInverse( vec_attributes_inverse );
}
void IfcReinforcementBarProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedProperties::setInverseCounterparts( ptr_self_entity );
}
void IfcReinforcementBarProperties::unlinkSelf()
{
	IfcPreDefinedProperties::unlinkSelf();
}

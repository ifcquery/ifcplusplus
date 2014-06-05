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
#include "include/IfcAxis2Placement2D.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"
#include "include/IfcTrapeziumProfileDef.h"

// ENTITY IfcTrapeziumProfileDef 
IfcTrapeziumProfileDef::IfcTrapeziumProfileDef() {}
IfcTrapeziumProfileDef::IfcTrapeziumProfileDef( int id ) { m_id = id; }
IfcTrapeziumProfileDef::~IfcTrapeziumProfileDef() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTrapeziumProfileDef::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTrapeziumProfileDef> other = dynamic_pointer_cast<IfcTrapeziumProfileDef>(other_entity);
	if( !other) { return; }
	m_ProfileType = other->m_ProfileType;
	m_ProfileName = other->m_ProfileName;
	m_Position = other->m_Position;
	m_BottomXDim = other->m_BottomXDim;
	m_TopXDim = other->m_TopXDim;
	m_YDim = other->m_YDim;
	m_TopXOffset = other->m_TopXOffset;
}
void IfcTrapeziumProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTRAPEZIUMPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_BottomXDim ) { m_BottomXDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopXDim ) { m_TopXDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_YDim ) { m_YDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopXOffset ) { m_TopXOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTrapeziumProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTrapeziumProfileDef::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTrapeziumProfileDef, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcTrapeziumProfileDef, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ProfileType = IfcProfileTypeEnum::createObjectFromStepData( args[0] );
	m_ProfileName = IfcLabel::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_BottomXDim = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
	m_TopXDim = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
	m_YDim = IfcPositiveLengthMeasure::createObjectFromStepData( args[5] );
	m_TopXOffset = IfcLengthMeasure::createObjectFromStepData( args[6] );
}
void IfcTrapeziumProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BottomXDim", m_BottomXDim ) );
	vec_attributes.push_back( std::make_pair( "TopXDim", m_TopXDim ) );
	vec_attributes.push_back( std::make_pair( "YDim", m_YDim ) );
	vec_attributes.push_back( std::make_pair( "TopXOffset", m_TopXOffset ) );
}
void IfcTrapeziumProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTrapeziumProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcTrapeziumProfileDef::unlinkSelf()
{
	IfcParameterizedProfileDef::unlinkSelf();
}

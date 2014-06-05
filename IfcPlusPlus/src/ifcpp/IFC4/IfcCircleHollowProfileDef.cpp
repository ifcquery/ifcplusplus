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
#include "include/IfcCircleHollowProfileDef.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcCircleHollowProfileDef 
IfcCircleHollowProfileDef::IfcCircleHollowProfileDef() {}
IfcCircleHollowProfileDef::IfcCircleHollowProfileDef( int id ) { m_id = id; }
IfcCircleHollowProfileDef::~IfcCircleHollowProfileDef() {}

// method setEntity takes over all attributes from another instance of the class
void IfcCircleHollowProfileDef::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcCircleHollowProfileDef> other = dynamic_pointer_cast<IfcCircleHollowProfileDef>(other_entity);
	if( !other) { return; }
	m_ProfileType = other->m_ProfileType;
	m_ProfileName = other->m_ProfileName;
	m_Position = other->m_Position;
	m_Radius = other->m_Radius;
	m_WallThickness = other->m_WallThickness;
}
void IfcCircleHollowProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCIRCLEHOLLOWPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Radius ) { m_Radius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_WallThickness ) { m_WallThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCircleHollowProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCircleHollowProfileDef::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCircleHollowProfileDef, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcCircleHollowProfileDef, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ProfileType = IfcProfileTypeEnum::createObjectFromStepData( args[0] );
	m_ProfileName = IfcLabel::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_Radius = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
	m_WallThickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
}
void IfcCircleHollowProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCircleProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "WallThickness", m_WallThickness ) );
}
void IfcCircleHollowProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcCircleHollowProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCircleProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcCircleHollowProfileDef::unlinkSelf()
{
	IfcCircleProfileDef::unlinkSelf();
}

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
#include "include/IfcBoundedCurve.h"
#include "include/IfcCenterLineProfileDef.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcCenterLineProfileDef 
IfcCenterLineProfileDef::IfcCenterLineProfileDef() {}
IfcCenterLineProfileDef::IfcCenterLineProfileDef( int id ) { m_id = id; }
IfcCenterLineProfileDef::~IfcCenterLineProfileDef() {}

// method setEntity takes over all attributes from another instance of the class
void IfcCenterLineProfileDef::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcCenterLineProfileDef> other = dynamic_pointer_cast<IfcCenterLineProfileDef>(other_entity);
	if( !other) { return; }
	m_ProfileType = other->m_ProfileType;
	m_ProfileName = other->m_ProfileName;
	m_Curve = other->m_Curve;
	m_Thickness = other->m_Thickness;
}
void IfcCenterLineProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCENTERLINEPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Curve ) { stream << "#" << m_Curve->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Thickness ) { m_Thickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCenterLineProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCenterLineProfileDef::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCenterLineProfileDef, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcCenterLineProfileDef, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ProfileType = IfcProfileTypeEnum::createObjectFromStepData( args[0] );
	m_ProfileName = IfcLabel::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Curve, map );
	m_Thickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
}
void IfcCenterLineProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcArbitraryOpenProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Thickness", m_Thickness ) );
}
void IfcCenterLineProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcCenterLineProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcArbitraryOpenProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcCenterLineProfileDef::unlinkSelf()
{
	IfcArbitraryOpenProfileDef::unlinkSelf();
}

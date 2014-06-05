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
#include "include/IfcArbitraryProfileDefWithVoids.h"
#include "include/IfcCurve.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcArbitraryProfileDefWithVoids 
IfcArbitraryProfileDefWithVoids::IfcArbitraryProfileDefWithVoids() {}
IfcArbitraryProfileDefWithVoids::IfcArbitraryProfileDefWithVoids( int id ) { m_id = id; }
IfcArbitraryProfileDefWithVoids::~IfcArbitraryProfileDefWithVoids() {}

// method setEntity takes over all attributes from another instance of the class
void IfcArbitraryProfileDefWithVoids::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcArbitraryProfileDefWithVoids> other = dynamic_pointer_cast<IfcArbitraryProfileDefWithVoids>(other_entity);
	if( !other) { return; }
	m_ProfileType = other->m_ProfileType;
	m_ProfileName = other->m_ProfileName;
	m_OuterCurve = other->m_OuterCurve;
	m_InnerCurves = other->m_InnerCurves;
}
void IfcArbitraryProfileDefWithVoids::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCARBITRARYPROFILEDEFWITHVOIDS" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OuterCurve ) { stream << "#" << m_OuterCurve->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_InnerCurves );
	stream << ");";
}
void IfcArbitraryProfileDefWithVoids::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcArbitraryProfileDefWithVoids::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcArbitraryProfileDefWithVoids, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcArbitraryProfileDefWithVoids, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ProfileType = IfcProfileTypeEnum::createObjectFromStepData( args[0] );
	m_ProfileName = IfcLabel::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_OuterCurve, map );
	readEntityReferenceList( args[3], m_InnerCurves, map );
}
void IfcArbitraryProfileDefWithVoids::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcArbitraryClosedProfileDef::getAttributes( vec_attributes );
}
void IfcArbitraryProfileDefWithVoids::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcArbitraryProfileDefWithVoids::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcArbitraryClosedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcArbitraryProfileDefWithVoids::unlinkSelf()
{
	IfcArbitraryClosedProfileDef::unlinkSelf();
}

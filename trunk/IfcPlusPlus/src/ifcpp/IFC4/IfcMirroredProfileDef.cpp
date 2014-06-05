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
#include "include/IfcCartesianTransformationOperator2D.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcMirroredProfileDef.h"
#include "include/IfcProfileDef.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcMirroredProfileDef 
IfcMirroredProfileDef::IfcMirroredProfileDef() {}
IfcMirroredProfileDef::IfcMirroredProfileDef( int id ) { m_id = id; }
IfcMirroredProfileDef::~IfcMirroredProfileDef() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMirroredProfileDef::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMirroredProfileDef> other = dynamic_pointer_cast<IfcMirroredProfileDef>(other_entity);
	if( !other) { return; }
	m_ProfileType = other->m_ProfileType;
	m_ProfileName = other->m_ProfileName;
	m_ParentProfile = other->m_ParentProfile;
	m_Operator = other->m_Operator;
	m_Label = other->m_Label;
}
void IfcMirroredProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMIRROREDPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ParentProfile ) { stream << "#" << m_ParentProfile->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Operator ) { stream << "#" << m_Operator->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Label ) { m_Label->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMirroredProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMirroredProfileDef::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMirroredProfileDef, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcMirroredProfileDef, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ProfileType = IfcProfileTypeEnum::createObjectFromStepData( args[0] );
	m_ProfileName = IfcLabel::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_ParentProfile, map );
	readEntityReference( args[3], m_Operator, map );
	m_Label = IfcLabel::createObjectFromStepData( args[4] );
}
void IfcMirroredProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcDerivedProfileDef::getAttributes( vec_attributes );
}
void IfcMirroredProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMirroredProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcDerivedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcMirroredProfileDef::unlinkSelf()
{
	IfcDerivedProfileDef::unlinkSelf();
}

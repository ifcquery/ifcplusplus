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
#include "include/IfcCurve.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSweptDiskSolid.h"

// ENTITY IfcSweptDiskSolid 
IfcSweptDiskSolid::IfcSweptDiskSolid() {}
IfcSweptDiskSolid::IfcSweptDiskSolid( int id ) { m_id = id; }
IfcSweptDiskSolid::~IfcSweptDiskSolid() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSweptDiskSolid::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSweptDiskSolid> other = dynamic_pointer_cast<IfcSweptDiskSolid>(other_entity);
	if( !other) { return; }
	m_Directrix = other->m_Directrix;
	m_Radius = other->m_Radius;
	m_InnerRadius = other->m_InnerRadius;
	m_StartParam = other->m_StartParam;
	m_EndParam = other->m_EndParam;
}
void IfcSweptDiskSolid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSWEPTDISKSOLID" << "(";
	if( m_Directrix ) { stream << "#" << m_Directrix->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Radius ) { m_Radius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InnerRadius ) { m_InnerRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_StartParam ) { m_StartParam->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EndParam ) { m_EndParam->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSweptDiskSolid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSweptDiskSolid::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSweptDiskSolid, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcSweptDiskSolid, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Directrix, map );
	m_Radius = IfcPositiveLengthMeasure::createObjectFromStepData( args[1] );
	m_InnerRadius = IfcPositiveLengthMeasure::createObjectFromStepData( args[2] );
	m_StartParam = IfcParameterValue::createObjectFromStepData( args[3] );
	m_EndParam = IfcParameterValue::createObjectFromStepData( args[4] );
}
void IfcSweptDiskSolid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSolidModel::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Directrix", m_Directrix ) );
	vec_attributes.push_back( std::make_pair( "Radius", m_Radius ) );
	vec_attributes.push_back( std::make_pair( "InnerRadius", m_InnerRadius ) );
	vec_attributes.push_back( std::make_pair( "StartParam", m_StartParam ) );
	vec_attributes.push_back( std::make_pair( "EndParam", m_EndParam ) );
}
void IfcSweptDiskSolid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSweptDiskSolid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSolidModel::setInverseCounterparts( ptr_self_entity );
}
void IfcSweptDiskSolid::unlinkSelf()
{
	IfcSolidModel::unlinkSelf();
}

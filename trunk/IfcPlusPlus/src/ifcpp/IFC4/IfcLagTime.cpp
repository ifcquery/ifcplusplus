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
#include "include/IfcDataOriginEnum.h"
#include "include/IfcLabel.h"
#include "include/IfcLagTime.h"
#include "include/IfcTaskDurationEnum.h"
#include "include/IfcTimeOrRatioSelect.h"

// ENTITY IfcLagTime 
IfcLagTime::IfcLagTime() {}
IfcLagTime::IfcLagTime( int id ) { m_id = id; }
IfcLagTime::~IfcLagTime() {}

// method setEntity takes over all attributes from another instance of the class
void IfcLagTime::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcLagTime> other = dynamic_pointer_cast<IfcLagTime>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_DataOrigin = other->m_DataOrigin;
	m_UserDefinedDataOrigin = other->m_UserDefinedDataOrigin;
	m_LagValue = other->m_LagValue;
	m_DurationType = other->m_DurationType;
}
void IfcLagTime::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCLAGTIME" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LagValue ) { m_LagValue->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_DurationType ) { m_DurationType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcLagTime::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLagTime::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLagTime, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcLagTime, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromStepData( args[1] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromStepData( args[2] );
	m_LagValue = IfcTimeOrRatioSelect::createObjectFromStepData( args[3], map );
	m_DurationType = IfcTaskDurationEnum::createObjectFromStepData( args[4] );
}
void IfcLagTime::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSchedulingTime::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LagValue", m_LagValue ) );
	vec_attributes.push_back( std::make_pair( "DurationType", m_DurationType ) );
}
void IfcLagTime::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLagTime::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSchedulingTime::setInverseCounterparts( ptr_self_entity );
}
void IfcLagTime::unlinkSelf()
{
	IfcSchedulingTime::unlinkSelf();
}

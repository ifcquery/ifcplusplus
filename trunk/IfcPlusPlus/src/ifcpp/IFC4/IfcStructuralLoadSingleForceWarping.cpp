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
#include "include/IfcForceMeasure.h"
#include "include/IfcLabel.h"
#include "include/IfcStructuralLoadSingleForceWarping.h"
#include "include/IfcTorqueMeasure.h"
#include "include/IfcWarpingMomentMeasure.h"

// ENTITY IfcStructuralLoadSingleForceWarping 
IfcStructuralLoadSingleForceWarping::IfcStructuralLoadSingleForceWarping() {}
IfcStructuralLoadSingleForceWarping::IfcStructuralLoadSingleForceWarping( int id ) { m_id = id; }
IfcStructuralLoadSingleForceWarping::~IfcStructuralLoadSingleForceWarping() {}

// method setEntity takes over all attributes from another instance of the class
void IfcStructuralLoadSingleForceWarping::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcStructuralLoadSingleForceWarping> other = dynamic_pointer_cast<IfcStructuralLoadSingleForceWarping>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_ForceX = other->m_ForceX;
	m_ForceY = other->m_ForceY;
	m_ForceZ = other->m_ForceZ;
	m_MomentX = other->m_MomentX;
	m_MomentY = other->m_MomentY;
	m_MomentZ = other->m_MomentZ;
	m_WarpingMoment = other->m_WarpingMoment;
}
void IfcStructuralLoadSingleForceWarping::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSTRUCTURALLOADSINGLEFORCEWARPING" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ForceX ) { m_ForceX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ForceY ) { m_ForceY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ForceZ ) { m_ForceZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MomentX ) { m_MomentX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MomentY ) { m_MomentY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MomentZ ) { m_MomentZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_WarpingMoment ) { m_WarpingMoment->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralLoadSingleForceWarping::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadSingleForceWarping::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralLoadSingleForceWarping, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcStructuralLoadSingleForceWarping, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_ForceX = IfcForceMeasure::createObjectFromStepData( args[1] );
	m_ForceY = IfcForceMeasure::createObjectFromStepData( args[2] );
	m_ForceZ = IfcForceMeasure::createObjectFromStepData( args[3] );
	m_MomentX = IfcTorqueMeasure::createObjectFromStepData( args[4] );
	m_MomentY = IfcTorqueMeasure::createObjectFromStepData( args[5] );
	m_MomentZ = IfcTorqueMeasure::createObjectFromStepData( args[6] );
	m_WarpingMoment = IfcWarpingMomentMeasure::createObjectFromStepData( args[7] );
}
void IfcStructuralLoadSingleForceWarping::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoadSingleForce::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "WarpingMoment", m_WarpingMoment ) );
}
void IfcStructuralLoadSingleForceWarping::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcStructuralLoadSingleForceWarping::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoadSingleForce::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadSingleForceWarping::unlinkSelf()
{
	IfcStructuralLoadSingleForce::unlinkSelf();
}

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
#include "ifcpp/model/IfcPPGuid.h"
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
shared_ptr<IfcPPObject> IfcStructuralLoadSingleForceWarping::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralLoadSingleForceWarping> copy_self( new IfcStructuralLoadSingleForceWarping() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_ForceX ) { copy_self->m_ForceX = dynamic_pointer_cast<IfcForceMeasure>( m_ForceX->getDeepCopy(options) ); }
	if( m_ForceY ) { copy_self->m_ForceY = dynamic_pointer_cast<IfcForceMeasure>( m_ForceY->getDeepCopy(options) ); }
	if( m_ForceZ ) { copy_self->m_ForceZ = dynamic_pointer_cast<IfcForceMeasure>( m_ForceZ->getDeepCopy(options) ); }
	if( m_MomentX ) { copy_self->m_MomentX = dynamic_pointer_cast<IfcTorqueMeasure>( m_MomentX->getDeepCopy(options) ); }
	if( m_MomentY ) { copy_self->m_MomentY = dynamic_pointer_cast<IfcTorqueMeasure>( m_MomentY->getDeepCopy(options) ); }
	if( m_MomentZ ) { copy_self->m_MomentZ = dynamic_pointer_cast<IfcTorqueMeasure>( m_MomentZ->getDeepCopy(options) ); }
	if( m_WarpingMoment ) { copy_self->m_WarpingMoment = dynamic_pointer_cast<IfcWarpingMomentMeasure>( m_WarpingMoment->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStructuralLoadSingleForceWarping::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALLOADSINGLEFORCEWARPING" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ForceX ) { m_ForceX->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ForceY ) { m_ForceY->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ForceZ ) { m_ForceZ->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_MomentX ) { m_MomentX->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_MomentY ) { m_MomentY->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_MomentZ ) { m_MomentZ->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_WarpingMoment ) { m_WarpingMoment->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralLoadSingleForceWarping::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadSingleForceWarping::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralLoadSingleForceWarping, expecting 8, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_ForceX = IfcForceMeasure::createObjectFromSTEP( args[1] );
	m_ForceY = IfcForceMeasure::createObjectFromSTEP( args[2] );
	m_ForceZ = IfcForceMeasure::createObjectFromSTEP( args[3] );
	m_MomentX = IfcTorqueMeasure::createObjectFromSTEP( args[4] );
	m_MomentY = IfcTorqueMeasure::createObjectFromSTEP( args[5] );
	m_MomentZ = IfcTorqueMeasure::createObjectFromSTEP( args[6] );
	m_WarpingMoment = IfcWarpingMomentMeasure::createObjectFromSTEP( args[7] );
}
void IfcStructuralLoadSingleForceWarping::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoadSingleForce::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "WarpingMoment", m_WarpingMoment ) );
}
void IfcStructuralLoadSingleForceWarping::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralLoadSingleForce::getAttributesInverse( vec_attributes_inverse );
}
void IfcStructuralLoadSingleForceWarping::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoadSingleForce::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadSingleForceWarping::unlinkSelf()
{
	IfcStructuralLoadSingleForce::unlinkSelf();
}

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
#include "include/IfcLabel.h"
#include "include/IfcLinearForceMeasure.h"
#include "include/IfcLinearMomentMeasure.h"
#include "include/IfcStructuralLoadLinearForce.h"

// ENTITY IfcStructuralLoadLinearForce 
IfcStructuralLoadLinearForce::IfcStructuralLoadLinearForce() {}
IfcStructuralLoadLinearForce::IfcStructuralLoadLinearForce( int id ) { m_id = id; }
IfcStructuralLoadLinearForce::~IfcStructuralLoadLinearForce() {}
shared_ptr<IfcPPObject> IfcStructuralLoadLinearForce::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralLoadLinearForce> copy_self( new IfcStructuralLoadLinearForce() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_LinearForceX ) { copy_self->m_LinearForceX = dynamic_pointer_cast<IfcLinearForceMeasure>( m_LinearForceX->getDeepCopy(options) ); }
	if( m_LinearForceY ) { copy_self->m_LinearForceY = dynamic_pointer_cast<IfcLinearForceMeasure>( m_LinearForceY->getDeepCopy(options) ); }
	if( m_LinearForceZ ) { copy_self->m_LinearForceZ = dynamic_pointer_cast<IfcLinearForceMeasure>( m_LinearForceZ->getDeepCopy(options) ); }
	if( m_LinearMomentX ) { copy_self->m_LinearMomentX = dynamic_pointer_cast<IfcLinearMomentMeasure>( m_LinearMomentX->getDeepCopy(options) ); }
	if( m_LinearMomentY ) { copy_self->m_LinearMomentY = dynamic_pointer_cast<IfcLinearMomentMeasure>( m_LinearMomentY->getDeepCopy(options) ); }
	if( m_LinearMomentZ ) { copy_self->m_LinearMomentZ = dynamic_pointer_cast<IfcLinearMomentMeasure>( m_LinearMomentZ->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStructuralLoadLinearForce::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALLOADLINEARFORCE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LinearForceX ) { m_LinearForceX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LinearForceY ) { m_LinearForceY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LinearForceZ ) { m_LinearForceZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LinearMomentX ) { m_LinearMomentX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LinearMomentY ) { m_LinearMomentY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LinearMomentZ ) { m_LinearMomentZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralLoadLinearForce::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadLinearForce::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralLoadLinearForce, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_LinearForceX = IfcLinearForceMeasure::createObjectFromSTEP( args[1] );
	m_LinearForceY = IfcLinearForceMeasure::createObjectFromSTEP( args[2] );
	m_LinearForceZ = IfcLinearForceMeasure::createObjectFromSTEP( args[3] );
	m_LinearMomentX = IfcLinearMomentMeasure::createObjectFromSTEP( args[4] );
	m_LinearMomentY = IfcLinearMomentMeasure::createObjectFromSTEP( args[5] );
	m_LinearMomentZ = IfcLinearMomentMeasure::createObjectFromSTEP( args[6] );
}
void IfcStructuralLoadLinearForce::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoadStatic::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LinearForceX", m_LinearForceX ) );
	vec_attributes.push_back( std::make_pair( "LinearForceY", m_LinearForceY ) );
	vec_attributes.push_back( std::make_pair( "LinearForceZ", m_LinearForceZ ) );
	vec_attributes.push_back( std::make_pair( "LinearMomentX", m_LinearMomentX ) );
	vec_attributes.push_back( std::make_pair( "LinearMomentY", m_LinearMomentY ) );
	vec_attributes.push_back( std::make_pair( "LinearMomentZ", m_LinearMomentZ ) );
}
void IfcStructuralLoadLinearForce::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralLoadStatic::getAttributesInverse( vec_attributes_inverse );
}
void IfcStructuralLoadLinearForce::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoadStatic::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadLinearForce::unlinkSelf()
{
	IfcStructuralLoadStatic::unlinkSelf();
}

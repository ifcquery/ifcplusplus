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
#include "include/IfcAxis2Placement.h"
#include "include/IfcDimensionCount.h"
#include "include/IfcDirection.h"
#include "include/IfcGeometricProjectionEnum.h"
#include "include/IfcGeometricRepresentationContext.h"
#include "include/IfcGeometricRepresentationSubContext.h"
#include "include/IfcLabel.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcRepresentation.h"

// ENTITY IfcGeometricRepresentationSubContext 
IfcGeometricRepresentationSubContext::IfcGeometricRepresentationSubContext() {}
IfcGeometricRepresentationSubContext::IfcGeometricRepresentationSubContext( int id ) { m_id = id; }
IfcGeometricRepresentationSubContext::~IfcGeometricRepresentationSubContext() {}
shared_ptr<IfcPPObject> IfcGeometricRepresentationSubContext::getDeepCopy()
{
	shared_ptr<IfcGeometricRepresentationSubContext> copy_self( new IfcGeometricRepresentationSubContext() );
	if( m_ContextIdentifier ) { copy_self->m_ContextIdentifier = dynamic_pointer_cast<IfcLabel>( m_ContextIdentifier->getDeepCopy() ); }
	if( m_ContextType ) { copy_self->m_ContextType = dynamic_pointer_cast<IfcLabel>( m_ContextType->getDeepCopy() ); }
	if( m_CoordinateSpaceDimension ) { copy_self->m_CoordinateSpaceDimension = dynamic_pointer_cast<IfcDimensionCount>( m_CoordinateSpaceDimension->getDeepCopy() ); }
	if( m_Precision ) { copy_self->m_Precision = m_Precision; }
	if( m_WorldCoordinateSystem ) { copy_self->m_WorldCoordinateSystem = dynamic_pointer_cast<IfcAxis2Placement>( m_WorldCoordinateSystem->getDeepCopy() ); }
	if( m_TrueNorth ) { copy_self->m_TrueNorth = dynamic_pointer_cast<IfcDirection>( m_TrueNorth->getDeepCopy() ); }
	if( m_ParentContext ) { copy_self->m_ParentContext = dynamic_pointer_cast<IfcGeometricRepresentationContext>( m_ParentContext->getDeepCopy() ); }
	if( m_TargetScale ) { copy_self->m_TargetScale = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_TargetScale->getDeepCopy() ); }
	if( m_TargetView ) { copy_self->m_TargetView = dynamic_pointer_cast<IfcGeometricProjectionEnum>( m_TargetView->getDeepCopy() ); }
	if( m_UserDefinedTargetView ) { copy_self->m_UserDefinedTargetView = dynamic_pointer_cast<IfcLabel>( m_UserDefinedTargetView->getDeepCopy() ); }
	return copy_self;
}
void IfcGeometricRepresentationSubContext::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCGEOMETRICREPRESENTATIONSUBCONTEXT" << "(";
	if( m_ContextIdentifier ) { m_ContextIdentifier->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ContextType ) { m_ContextType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_CoordinateSpaceDimension ) { m_CoordinateSpaceDimension->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Precision == m_Precision ){ stream << m_Precision; } else { stream << "*"; }
	stream << ",";
	if( m_WorldCoordinateSystem ) { m_WorldCoordinateSystem->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_TrueNorth ) { stream << "#" << m_TrueNorth->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_ParentContext ) { stream << "#" << m_ParentContext->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TargetScale ) { m_TargetScale->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TargetView ) { m_TargetView->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedTargetView ) { m_UserDefinedTargetView->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcGeometricRepresentationSubContext::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGeometricRepresentationSubContext::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcGeometricRepresentationSubContext, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcGeometricRepresentationSubContext, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ContextIdentifier = IfcLabel::createObjectFromStepData( args[0] );
	m_ContextType = IfcLabel::createObjectFromStepData( args[1] );
	m_CoordinateSpaceDimension = IfcDimensionCount::createObjectFromStepData( args[2] );
	readRealValue( args[3], m_Precision );
	m_WorldCoordinateSystem = IfcAxis2Placement::createObjectFromStepData( args[4], map );
	readEntityReference( args[5], m_TrueNorth, map );
	readEntityReference( args[6], m_ParentContext, map );
	m_TargetScale = IfcPositiveRatioMeasure::createObjectFromStepData( args[7] );
	m_TargetView = IfcGeometricProjectionEnum::createObjectFromStepData( args[8] );
	m_UserDefinedTargetView = IfcLabel::createObjectFromStepData( args[9] );
}
void IfcGeometricRepresentationSubContext::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationContext::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ParentContext", m_ParentContext ) );
	vec_attributes.push_back( std::make_pair( "TargetScale", m_TargetScale ) );
	vec_attributes.push_back( std::make_pair( "TargetView", m_TargetView ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedTargetView", m_UserDefinedTargetView ) );
}
void IfcGeometricRepresentationSubContext::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationContext::getAttributesInverse( vec_attributes_inverse );
}
void IfcGeometricRepresentationSubContext::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationContext::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcGeometricRepresentationSubContext> ptr_self = dynamic_pointer_cast<IfcGeometricRepresentationSubContext>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcGeometricRepresentationSubContext::setInverseCounterparts: type mismatch" ); }
	if( m_ParentContext )
	{
		m_ParentContext->m_HasSubContexts_inverse.push_back( ptr_self );
	}
}
void IfcGeometricRepresentationSubContext::unlinkSelf()
{
	IfcGeometricRepresentationContext::unlinkSelf();
	if( m_ParentContext )
	{
		std::vector<weak_ptr<IfcGeometricRepresentationSubContext> >& HasSubContexts_inverse = m_ParentContext->m_HasSubContexts_inverse;
		std::vector<weak_ptr<IfcGeometricRepresentationSubContext> >::iterator it_HasSubContexts_inverse;
		for( it_HasSubContexts_inverse = HasSubContexts_inverse.begin(); it_HasSubContexts_inverse != HasSubContexts_inverse.end(); ++it_HasSubContexts_inverse)
		{
			shared_ptr<IfcGeometricRepresentationSubContext> self_candidate( *it_HasSubContexts_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasSubContexts_inverse.erase( it_HasSubContexts_inverse );
				break;
			}
		}
	}
}

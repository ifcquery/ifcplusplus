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
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRectangularTrimmedSurface.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcRectangularTrimmedSurface 
IfcRectangularTrimmedSurface::IfcRectangularTrimmedSurface() {}
IfcRectangularTrimmedSurface::IfcRectangularTrimmedSurface( int id ) { m_id = id; }
IfcRectangularTrimmedSurface::~IfcRectangularTrimmedSurface() {}
shared_ptr<IfcPPObject> IfcRectangularTrimmedSurface::getDeepCopy()
{
	shared_ptr<IfcRectangularTrimmedSurface> copy_self( new IfcRectangularTrimmedSurface() );
	if( m_BasisSurface ) { copy_self->m_BasisSurface = dynamic_pointer_cast<IfcSurface>( m_BasisSurface->getDeepCopy() ); }
	if( m_U1 ) { copy_self->m_U1 = dynamic_pointer_cast<IfcParameterValue>( m_U1->getDeepCopy() ); }
	if( m_V1 ) { copy_self->m_V1 = dynamic_pointer_cast<IfcParameterValue>( m_V1->getDeepCopy() ); }
	if( m_U2 ) { copy_self->m_U2 = dynamic_pointer_cast<IfcParameterValue>( m_U2->getDeepCopy() ); }
	if( m_V2 ) { copy_self->m_V2 = dynamic_pointer_cast<IfcParameterValue>( m_V2->getDeepCopy() ); }
	if( m_Usense ) { copy_self->m_Usense = m_Usense; }
	if( m_Vsense ) { copy_self->m_Vsense = m_Vsense; }
	return copy_self;
}
void IfcRectangularTrimmedSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRECTANGULARTRIMMEDSURFACE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_U1 ) { m_U1->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_V1 ) { m_V1->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_U2 ) { m_U2->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_V2 ) { m_V2->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Usense == false ) { stream << ".F."; }
	else if( m_Usense == true ) { stream << ".T."; }
	stream << ",";
	if( m_Vsense == false ) { stream << ".F."; }
	else if( m_Vsense == true ) { stream << ".T."; }
	stream << ");";
}
void IfcRectangularTrimmedSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRectangularTrimmedSurface::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRectangularTrimmedSurface, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcRectangularTrimmedSurface, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BasisSurface, map );
	m_U1 = IfcParameterValue::createObjectFromStepData( args[1] );
	m_V1 = IfcParameterValue::createObjectFromStepData( args[2] );
	m_U2 = IfcParameterValue::createObjectFromStepData( args[3] );
	m_V2 = IfcParameterValue::createObjectFromStepData( args[4] );
	if( boost::iequals( args[5], L".F." ) ) { m_Usense = false; }
	else if( boost::iequals( args[5], L".T." ) ) { m_Usense = true; }
	if( boost::iequals( args[6], L".F." ) ) { m_Vsense = false; }
	else if( boost::iequals( args[6], L".T." ) ) { m_Vsense = true; }
}
void IfcRectangularTrimmedSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	vec_attributes.push_back( std::make_pair( "U1", m_U1 ) );
	vec_attributes.push_back( std::make_pair( "V1", m_V1 ) );
	vec_attributes.push_back( std::make_pair( "U2", m_U2 ) );
	vec_attributes.push_back( std::make_pair( "V2", m_V2 ) );
	vec_attributes.push_back( std::make_pair( "Usense", shared_ptr<IfcPPBool>( new IfcPPBool( m_Usense ) ) ) );
	vec_attributes.push_back( std::make_pair( "Vsense", shared_ptr<IfcPPBool>( new IfcPPBool( m_Vsense ) ) ) );
}
void IfcRectangularTrimmedSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcRectangularTrimmedSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcRectangularTrimmedSurface::unlinkSelf()
{
	IfcBoundedSurface::unlinkSelf();
}

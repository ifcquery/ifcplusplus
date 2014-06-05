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

// method setEntity takes over all attributes from another instance of the class
void IfcRectangularTrimmedSurface::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRectangularTrimmedSurface> other = dynamic_pointer_cast<IfcRectangularTrimmedSurface>(other_entity);
	if( !other) { return; }
	m_BasisSurface = other->m_BasisSurface;
	m_U1 = other->m_U1;
	m_V1 = other->m_V1;
	m_U2 = other->m_U2;
	m_V2 = other->m_V2;
	m_Usense = other->m_Usense;
	m_Vsense = other->m_Vsense;
}
void IfcRectangularTrimmedSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRECTANGULARTRIMMEDSURFACE" << "(";
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
void IfcRectangularTrimmedSurface::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( _stricmp( args[5].c_str(), ".F." ) == 0 ) { m_Usense = false; }
	else if( _stricmp( args[5].c_str(), ".T." ) == 0 ) { m_Usense = true; }
	if( _stricmp( args[6].c_str(), ".F." ) == 0 ) { m_Vsense = false; }
	else if( _stricmp( args[6].c_str(), ".T." ) == 0 ) { m_Vsense = true; }
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
}
void IfcRectangularTrimmedSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcRectangularTrimmedSurface::unlinkSelf()
{
	IfcBoundedSurface::unlinkSelf();
}

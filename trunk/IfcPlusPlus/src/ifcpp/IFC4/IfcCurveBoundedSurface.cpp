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
#include "include/IfcBoundaryCurve.h"
#include "include/IfcCurveBoundedSurface.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcCurveBoundedSurface 
IfcCurveBoundedSurface::IfcCurveBoundedSurface() {}
IfcCurveBoundedSurface::IfcCurveBoundedSurface( int id ) { m_id = id; }
IfcCurveBoundedSurface::~IfcCurveBoundedSurface() {}

// method setEntity takes over all attributes from another instance of the class
void IfcCurveBoundedSurface::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcCurveBoundedSurface> other = dynamic_pointer_cast<IfcCurveBoundedSurface>(other_entity);
	if( !other) { return; }
	m_BasisSurface = other->m_BasisSurface;
	m_Boundaries = other->m_Boundaries;
	m_ImplicitOuter = other->m_ImplicitOuter;
}
void IfcCurveBoundedSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCURVEBOUNDEDSURFACE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Boundaries );
	stream << ",";
	if( m_ImplicitOuter == false ) { stream << ".F."; }
	else if( m_ImplicitOuter == true ) { stream << ".T."; }
	stream << ");";
}
void IfcCurveBoundedSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurveBoundedSurface::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCurveBoundedSurface, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcCurveBoundedSurface, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BasisSurface, map );
	readEntityReferenceList( args[1], m_Boundaries, map );
	if( _stricmp( args[2].c_str(), ".F." ) == 0 ) { m_ImplicitOuter = false; }
	else if( _stricmp( args[2].c_str(), ".T." ) == 0 ) { m_ImplicitOuter = true; }
}
void IfcCurveBoundedSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	vec_attributes.push_back( std::make_pair( "ImplicitOuter", shared_ptr<IfcPPBool>( new IfcPPBool( m_ImplicitOuter ) ) ) );
}
void IfcCurveBoundedSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcCurveBoundedSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveBoundedSurface::unlinkSelf()
{
	IfcBoundedSurface::unlinkSelf();
}

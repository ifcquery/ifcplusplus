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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcBoundedCurve.h"
#include "include/IfcPolygonalBoundedHalfSpace.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcPolygonalBoundedHalfSpace 
IfcPolygonalBoundedHalfSpace::IfcPolygonalBoundedHalfSpace() {}
IfcPolygonalBoundedHalfSpace::IfcPolygonalBoundedHalfSpace( int id ) { m_id = id; }
IfcPolygonalBoundedHalfSpace::~IfcPolygonalBoundedHalfSpace() {}
shared_ptr<IfcPPObject> IfcPolygonalBoundedHalfSpace::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPolygonalBoundedHalfSpace> copy_self( new IfcPolygonalBoundedHalfSpace() );
	if( m_BaseSurface ) { copy_self->m_BaseSurface = dynamic_pointer_cast<IfcSurface>( m_BaseSurface->getDeepCopy(options) ); }
	if( m_AgreementFlag ) { copy_self->m_AgreementFlag = m_AgreementFlag; }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_PolygonalBoundary ) { copy_self->m_PolygonalBoundary = dynamic_pointer_cast<IfcBoundedCurve>( m_PolygonalBoundary->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPolygonalBoundedHalfSpace::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPOLYGONALBOUNDEDHALFSPACE" << "(";
	if( m_BaseSurface ) { stream << "#" << m_BaseSurface->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_AgreementFlag == false ) { stream << ".F."; }
	else if( m_AgreementFlag == true ) { stream << ".T."; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_PolygonalBoundary ) { stream << "#" << m_PolygonalBoundary->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcPolygonalBoundedHalfSpace::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPolygonalBoundedHalfSpace::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPolygonalBoundedHalfSpace, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_BaseSurface, map );
	if( boost::iequals( args[1], L".F." ) ) { m_AgreementFlag = false; }
	else if( boost::iequals( args[1], L".T." ) ) { m_AgreementFlag = true; }
	readEntityReference( args[2], m_Position, map );
	readEntityReference( args[3], m_PolygonalBoundary, map );
}
void IfcPolygonalBoundedHalfSpace::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcHalfSpaceSolid::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Position", m_Position ) );
	vec_attributes.push_back( std::make_pair( "PolygonalBoundary", m_PolygonalBoundary ) );
}
void IfcPolygonalBoundedHalfSpace::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcHalfSpaceSolid::getAttributesInverse( vec_attributes_inverse );
}
void IfcPolygonalBoundedHalfSpace::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcHalfSpaceSolid::setInverseCounterparts( ptr_self_entity );
}
void IfcPolygonalBoundedHalfSpace::unlinkSelf()
{
	IfcHalfSpaceSolid::unlinkSelf();
}

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
#include "include/IfcReal.h"
#include "include/IfcSurfaceStyleRefraction.h"

// ENTITY IfcSurfaceStyleRefraction 
IfcSurfaceStyleRefraction::IfcSurfaceStyleRefraction() {}
IfcSurfaceStyleRefraction::IfcSurfaceStyleRefraction( int id ) { m_id = id; }
IfcSurfaceStyleRefraction::~IfcSurfaceStyleRefraction() {}
shared_ptr<IfcPPObject> IfcSurfaceStyleRefraction::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceStyleRefraction> copy_self( new IfcSurfaceStyleRefraction() );
	if( m_RefractionIndex ) { copy_self->m_RefractionIndex = dynamic_pointer_cast<IfcReal>( m_RefractionIndex->getDeepCopy(options) ); }
	if( m_DispersionFactor ) { copy_self->m_DispersionFactor = dynamic_pointer_cast<IfcReal>( m_DispersionFactor->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSurfaceStyleRefraction::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSURFACESTYLEREFRACTION" << "(";
	if( m_RefractionIndex ) { m_RefractionIndex->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DispersionFactor ) { m_DispersionFactor->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceStyleRefraction::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyleRefraction::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSurfaceStyleRefraction, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_RefractionIndex = IfcReal::createObjectFromSTEP( args[0] );
	m_DispersionFactor = IfcReal::createObjectFromSTEP( args[1] );
}
void IfcSurfaceStyleRefraction::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RefractionIndex", m_RefractionIndex ) );
	vec_attributes.push_back( std::make_pair( "DispersionFactor", m_DispersionFactor ) );
}
void IfcSurfaceStyleRefraction::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceStyleRefraction::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyleRefraction::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}

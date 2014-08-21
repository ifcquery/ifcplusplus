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
#include "include/IfcConnectionPointGeometry.h"
#include "include/IfcPointOrVertexPoint.h"

// ENTITY IfcConnectionPointGeometry 
IfcConnectionPointGeometry::IfcConnectionPointGeometry() {}
IfcConnectionPointGeometry::IfcConnectionPointGeometry( int id ) { m_id = id; }
IfcConnectionPointGeometry::~IfcConnectionPointGeometry() {}
shared_ptr<IfcPPObject> IfcConnectionPointGeometry::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcConnectionPointGeometry> copy_self( new IfcConnectionPointGeometry() );
	if( m_PointOnRelatingElement ) { copy_self->m_PointOnRelatingElement = dynamic_pointer_cast<IfcPointOrVertexPoint>( m_PointOnRelatingElement->getDeepCopy(options) ); }
	if( m_PointOnRelatedElement ) { copy_self->m_PointOnRelatedElement = dynamic_pointer_cast<IfcPointOrVertexPoint>( m_PointOnRelatedElement->getDeepCopy(options) ); }
	return copy_self;
}
void IfcConnectionPointGeometry::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCONNECTIONPOINTGEOMETRY" << "(";
	if( m_PointOnRelatingElement ) { m_PointOnRelatingElement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_PointOnRelatedElement ) { m_PointOnRelatedElement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcConnectionPointGeometry::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConnectionPointGeometry::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcConnectionPointGeometry, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_PointOnRelatingElement = IfcPointOrVertexPoint::createObjectFromSTEP( args[0], map );
	m_PointOnRelatedElement = IfcPointOrVertexPoint::createObjectFromSTEP( args[1], map );
}
void IfcConnectionPointGeometry::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConnectionGeometry::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PointOnRelatingElement", m_PointOnRelatingElement ) );
	vec_attributes.push_back( std::make_pair( "PointOnRelatedElement", m_PointOnRelatedElement ) );
}
void IfcConnectionPointGeometry::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConnectionGeometry::getAttributesInverse( vec_attributes_inverse );
}
void IfcConnectionPointGeometry::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConnectionGeometry::setInverseCounterparts( ptr_self_entity );
}
void IfcConnectionPointGeometry::unlinkSelf()
{
	IfcConnectionGeometry::unlinkSelf();
}

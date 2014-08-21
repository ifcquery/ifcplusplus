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
#include "include/IfcCsgPrimitive3D.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcCsgPrimitive3D 
IfcCsgPrimitive3D::IfcCsgPrimitive3D() {}
IfcCsgPrimitive3D::IfcCsgPrimitive3D( int id ) { m_id = id; }
IfcCsgPrimitive3D::~IfcCsgPrimitive3D() {}
shared_ptr<IfcPPObject> IfcCsgPrimitive3D::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCsgPrimitive3D> copy_self( new IfcCsgPrimitive3D() );
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCsgPrimitive3D::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCSGPRIMITIVE3D" << "(";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcCsgPrimitive3D::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCsgPrimitive3D::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCsgPrimitive3D, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_Position, map );
}
void IfcCsgPrimitive3D::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Position", m_Position ) );
}
void IfcCsgPrimitive3D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcCsgPrimitive3D::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcCsgPrimitive3D::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}

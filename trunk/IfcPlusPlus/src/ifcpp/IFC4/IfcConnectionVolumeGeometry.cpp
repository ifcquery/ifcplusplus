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
#include "include/IfcConnectionVolumeGeometry.h"
#include "include/IfcSolidOrShell.h"

// ENTITY IfcConnectionVolumeGeometry 
IfcConnectionVolumeGeometry::IfcConnectionVolumeGeometry() {}
IfcConnectionVolumeGeometry::IfcConnectionVolumeGeometry( int id ) { m_id = id; }
IfcConnectionVolumeGeometry::~IfcConnectionVolumeGeometry() {}

// method setEntity takes over all attributes from another instance of the class
void IfcConnectionVolumeGeometry::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcConnectionVolumeGeometry> other = dynamic_pointer_cast<IfcConnectionVolumeGeometry>(other_entity);
	if( !other) { return; }
	m_VolumeOnRelatingElement = other->m_VolumeOnRelatingElement;
	m_VolumeOnRelatedElement = other->m_VolumeOnRelatedElement;
}
void IfcConnectionVolumeGeometry::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCONNECTIONVOLUMEGEOMETRY" << "(";
	if( m_VolumeOnRelatingElement ) { m_VolumeOnRelatingElement->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_VolumeOnRelatedElement ) { m_VolumeOnRelatedElement->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcConnectionVolumeGeometry::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConnectionVolumeGeometry::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcConnectionVolumeGeometry, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcConnectionVolumeGeometry, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_VolumeOnRelatingElement = IfcSolidOrShell::createObjectFromStepData( args[0], map );
	m_VolumeOnRelatedElement = IfcSolidOrShell::createObjectFromStepData( args[1], map );
}
void IfcConnectionVolumeGeometry::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConnectionGeometry::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "VolumeOnRelatingElement", m_VolumeOnRelatingElement ) );
	vec_attributes.push_back( std::make_pair( "VolumeOnRelatedElement", m_VolumeOnRelatedElement ) );
}
void IfcConnectionVolumeGeometry::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcConnectionVolumeGeometry::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConnectionGeometry::setInverseCounterparts( ptr_self_entity );
}
void IfcConnectionVolumeGeometry::unlinkSelf()
{
	IfcConnectionGeometry::unlinkSelf();
}

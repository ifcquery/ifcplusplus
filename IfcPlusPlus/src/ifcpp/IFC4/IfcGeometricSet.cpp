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
#include "include/IfcGeometricSet.h"
#include "include/IfcGeometricSetSelect.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcGeometricSet 
IfcGeometricSet::IfcGeometricSet() {}
IfcGeometricSet::IfcGeometricSet( int id ) { m_id = id; }
IfcGeometricSet::~IfcGeometricSet() {}

// method setEntity takes over all attributes from another instance of the class
void IfcGeometricSet::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcGeometricSet> other = dynamic_pointer_cast<IfcGeometricSet>(other_entity);
	if( !other) { return; }
	m_Elements = other->m_Elements;
}
void IfcGeometricSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCGEOMETRICSET" << "(";
	writeTypeList( stream, m_Elements, true );
	stream << ");";
}
void IfcGeometricSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGeometricSet::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcGeometricSet, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcGeometricSet, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readSelectList( args[0], m_Elements, map );
}
void IfcGeometricSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	shared_ptr<IfcPPAttributeObjectVector> Elements_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Elements.begin(), m_Elements.end(), std::back_inserter( Elements_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Elements", Elements_vec_object ) );
}
void IfcGeometricSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcGeometricSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcGeometricSet::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}

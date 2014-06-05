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
#include "include/IfcFace.h"
#include "include/IfcFaceBound.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTextureMap.h"

// ENTITY IfcFace 
IfcFace::IfcFace() {}
IfcFace::IfcFace( int id ) { m_id = id; }
IfcFace::~IfcFace() {}

// method setEntity takes over all attributes from another instance of the class
void IfcFace::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcFace> other = dynamic_pointer_cast<IfcFace>(other_entity);
	if( !other) { return; }
	m_Bounds = other->m_Bounds;
}
void IfcFace::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCFACE" << "(";
	writeEntityList( stream, m_Bounds );
	stream << ");";
}
void IfcFace::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFace::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFace, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcFace, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Bounds, map );
}
void IfcFace::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTopologicalRepresentationItem::getAttributes( vec_attributes );
}
void IfcFace::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasTextureMaps_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasTextureMaps_inverse.size(); ++i ) { HasTextureMaps_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcTextureMap>( m_HasTextureMaps_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasTextureMaps_inverse", HasTextureMaps_inverse_vec_obj ) );
}
void IfcFace::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTopologicalRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcFace::unlinkSelf()
{
	IfcTopologicalRepresentationItem::unlinkSelf();
}

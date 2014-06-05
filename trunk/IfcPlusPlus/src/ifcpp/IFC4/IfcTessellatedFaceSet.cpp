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
#include "include/IfcCartesianPointList3D.h"
#include "include/IfcIndexedColourMap.h"
#include "include/IfcIndexedTextureMap.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTessellatedFaceSet.h"

// ENTITY IfcTessellatedFaceSet 
IfcTessellatedFaceSet::IfcTessellatedFaceSet() {}
IfcTessellatedFaceSet::IfcTessellatedFaceSet( int id ) { m_id = id; }
IfcTessellatedFaceSet::~IfcTessellatedFaceSet() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTessellatedFaceSet::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTessellatedFaceSet> other = dynamic_pointer_cast<IfcTessellatedFaceSet>(other_entity);
	if( !other) { return; }
	m_Coordinates = other->m_Coordinates;
	m_Normals = other->m_Normals;
	m_Closed = other->m_Closed;
}
void IfcTessellatedFaceSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTESSELLATEDFACESET" << "(";
	if( m_Coordinates ) { stream << "#" << m_Coordinates->getId(); } else { stream << "$"; }
	stream << ",";
	writeTypeOfRealList2D( stream, m_Normals );
	stream << ",";
	if( m_Closed == false ) { stream << ".F."; }
	else if( m_Closed == true ) { stream << ".T."; }
	stream << ");";
}
void IfcTessellatedFaceSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTessellatedFaceSet::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTessellatedFaceSet, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcTessellatedFaceSet, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Coordinates, map );
	readTypeOfRealList2D( args[1], m_Normals );
	if( _stricmp( args[2].c_str(), ".F." ) == 0 ) { m_Closed = false; }
	else if( _stricmp( args[2].c_str(), ".T." ) == 0 ) { m_Closed = true; }
}
void IfcTessellatedFaceSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTessellatedItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Coordinates", m_Coordinates ) );
	vec_attributes.push_back( std::make_pair( "Closed", shared_ptr<IfcPPBool>( new IfcPPBool( m_Closed ) ) ) );
}
void IfcTessellatedFaceSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasColours_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasColours_inverse.size(); ++i ) { HasColours_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcIndexedColourMap>( m_HasColours_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasColours_inverse", HasColours_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasTextures_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasTextures_inverse.size(); ++i ) { HasTextures_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcIndexedTextureMap>( m_HasTextures_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasTextures_inverse", HasTextures_inverse_vec_obj ) );
}
void IfcTessellatedFaceSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTessellatedItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTessellatedFaceSet::unlinkSelf()
{
	IfcTessellatedItem::unlinkSelf();
}

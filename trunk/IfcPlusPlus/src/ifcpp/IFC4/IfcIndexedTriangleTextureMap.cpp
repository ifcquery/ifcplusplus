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
#include "include/IfcIndexedTriangleTextureMap.h"
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTessellatedFaceSet.h"
#include "include/IfcTextureVertexList.h"

// ENTITY IfcIndexedTriangleTextureMap 
IfcIndexedTriangleTextureMap::IfcIndexedTriangleTextureMap() {}
IfcIndexedTriangleTextureMap::IfcIndexedTriangleTextureMap( int id ) { m_id = id; }
IfcIndexedTriangleTextureMap::~IfcIndexedTriangleTextureMap() {}

// method setEntity takes over all attributes from another instance of the class
void IfcIndexedTriangleTextureMap::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcIndexedTriangleTextureMap> other = dynamic_pointer_cast<IfcIndexedTriangleTextureMap>(other_entity);
	if( !other) { return; }
	m_Maps = other->m_Maps;
	m_MappedTo = other->m_MappedTo;
	m_TexCoords = other->m_TexCoords;
	m_TexCoordIndex = other->m_TexCoordIndex;
}
void IfcIndexedTriangleTextureMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCINDEXEDTRIANGLETEXTUREMAP" << "(";
	writeEntityList( stream, m_Maps );
	stream << ",";
	if( m_MappedTo ) { stream << "#" << m_MappedTo->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TexCoords ) { stream << "#" << m_TexCoords->getId(); } else { stream << "$"; }
	stream << ",";
	writeIntList2D( stream, m_TexCoordIndex );
	stream << ");";
}
void IfcIndexedTriangleTextureMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedTriangleTextureMap::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcIndexedTriangleTextureMap, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcIndexedTriangleTextureMap, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Maps, map );
	readEntityReference( args[1], m_MappedTo, map );
	readEntityReference( args[2], m_TexCoords, map );
	readIntList2D(  args[3], m_TexCoordIndex );
}
void IfcIndexedTriangleTextureMap::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcIndexedTextureMap::getAttributes( vec_attributes );
}
void IfcIndexedTriangleTextureMap::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcIndexedTriangleTextureMap::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcIndexedTextureMap::setInverseCounterparts( ptr_self_entity );
}
void IfcIndexedTriangleTextureMap::unlinkSelf()
{
	IfcIndexedTextureMap::unlinkSelf();
}

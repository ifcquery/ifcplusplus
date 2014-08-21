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
#include "include/IfcIndexedTriangleTextureMap.h"
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTessellatedFaceSet.h"
#include "include/IfcTextureVertexList.h"

// ENTITY IfcIndexedTriangleTextureMap 
IfcIndexedTriangleTextureMap::IfcIndexedTriangleTextureMap() {}
IfcIndexedTriangleTextureMap::IfcIndexedTriangleTextureMap( int id ) { m_id = id; }
IfcIndexedTriangleTextureMap::~IfcIndexedTriangleTextureMap() {}
shared_ptr<IfcPPObject> IfcIndexedTriangleTextureMap::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcIndexedTriangleTextureMap> copy_self( new IfcIndexedTriangleTextureMap() );
	for( size_t ii=0; ii<m_Maps.size(); ++ii )
	{
		auto item_ii = m_Maps[ii];
		if( item_ii )
		{
			copy_self->m_Maps.push_back( dynamic_pointer_cast<IfcSurfaceTexture>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_MappedTo ) { copy_self->m_MappedTo = dynamic_pointer_cast<IfcTessellatedFaceSet>( m_MappedTo->getDeepCopy(options) ); }
	if( m_TexCoords ) { copy_self->m_TexCoords = dynamic_pointer_cast<IfcTextureVertexList>( m_TexCoords->getDeepCopy(options) ); }
	if( m_TexCoordIndex.size() > 0 )
	{
		copy_self->m_TexCoordIndex.resize( m_TexCoordIndex.size() );
		for( size_t i = 0; i < m_TexCoordIndex.size(); ++i )
		{
			std::copy( m_TexCoordIndex[i].begin(), m_TexCoordIndex[i].end(), std::back_inserter( copy_self->m_TexCoordIndex[i] ) );
		}
	}
	return copy_self;
}
void IfcIndexedTriangleTextureMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCINDEXEDTRIANGLETEXTUREMAP" << "(";
	writeEntityList( stream, m_Maps );
	stream << ",";
	if( m_MappedTo ) { stream << "#" << m_MappedTo->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_TexCoords ) { stream << "#" << m_TexCoords->m_id; } else { stream << "*"; }
	stream << ",";
	writeIntList2D( stream, m_TexCoordIndex );
	stream << ");";
}
void IfcIndexedTriangleTextureMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedTriangleTextureMap::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcIndexedTriangleTextureMap, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
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
	IfcIndexedTextureMap::getAttributesInverse( vec_attributes_inverse );
}
void IfcIndexedTriangleTextureMap::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcIndexedTextureMap::setInverseCounterparts( ptr_self_entity );
}
void IfcIndexedTriangleTextureMap::unlinkSelf()
{
	IfcIndexedTextureMap::unlinkSelf();
}

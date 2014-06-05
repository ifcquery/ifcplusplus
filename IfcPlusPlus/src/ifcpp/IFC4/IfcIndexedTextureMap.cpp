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
#include "include/IfcIndexedTextureMap.h"
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTessellatedFaceSet.h"
#include "include/IfcTextureVertexList.h"

// ENTITY IfcIndexedTextureMap 
IfcIndexedTextureMap::IfcIndexedTextureMap() {}
IfcIndexedTextureMap::IfcIndexedTextureMap( int id ) { m_id = id; }
IfcIndexedTextureMap::~IfcIndexedTextureMap() {}

// method setEntity takes over all attributes from another instance of the class
void IfcIndexedTextureMap::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcIndexedTextureMap> other = dynamic_pointer_cast<IfcIndexedTextureMap>(other_entity);
	if( !other) { return; }
	m_Maps = other->m_Maps;
	m_MappedTo = other->m_MappedTo;
	m_TexCoords = other->m_TexCoords;
}
void IfcIndexedTextureMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCINDEXEDTEXTUREMAP" << "(";
	writeEntityList( stream, m_Maps );
	stream << ",";
	if( m_MappedTo ) { stream << "#" << m_MappedTo->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TexCoords ) { stream << "#" << m_TexCoords->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcIndexedTextureMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedTextureMap::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcIndexedTextureMap, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcIndexedTextureMap, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Maps, map );
	readEntityReference( args[1], m_MappedTo, map );
	readEntityReference( args[2], m_TexCoords, map );
}
void IfcIndexedTextureMap::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTextureCoordinate::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "MappedTo", m_MappedTo ) );
	vec_attributes.push_back( std::make_pair( "TexCoords", m_TexCoords ) );
}
void IfcIndexedTextureMap::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcIndexedTextureMap::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTextureCoordinate::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcIndexedTextureMap> ptr_self = dynamic_pointer_cast<IfcIndexedTextureMap>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcIndexedTextureMap::setInverseCounterparts: type mismatch" ); }
	if( m_MappedTo )
	{
		m_MappedTo->m_HasTextures_inverse.push_back( ptr_self );
	}
}
void IfcIndexedTextureMap::unlinkSelf()
{
	IfcTextureCoordinate::unlinkSelf();
	if( m_MappedTo )
	{
		std::vector<weak_ptr<IfcIndexedTextureMap> >& HasTextures_inverse = m_MappedTo->m_HasTextures_inverse;
		std::vector<weak_ptr<IfcIndexedTextureMap> >::iterator it_HasTextures_inverse;
		for( it_HasTextures_inverse = HasTextures_inverse.begin(); it_HasTextures_inverse != HasTextures_inverse.end(); ++it_HasTextures_inverse)
		{
			shared_ptr<IfcIndexedTextureMap> self_candidate( *it_HasTextures_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasTextures_inverse.erase( it_HasTextures_inverse );
				break;
			}
		}
	}
}

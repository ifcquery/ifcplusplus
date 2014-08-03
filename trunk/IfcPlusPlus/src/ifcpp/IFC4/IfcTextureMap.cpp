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
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTextureMap.h"
#include "include/IfcTextureVertex.h"

// ENTITY IfcTextureMap 
IfcTextureMap::IfcTextureMap() {}
IfcTextureMap::IfcTextureMap( int id ) { m_id = id; }
IfcTextureMap::~IfcTextureMap() {}
shared_ptr<IfcPPObject> IfcTextureMap::getDeepCopy()
{
	shared_ptr<IfcTextureMap> copy_self( new IfcTextureMap() );
	for( size_t ii=0; ii<m_Maps.size(); ++ii )
	{
		auto item_ii = m_Maps[ii];
		if( item_ii )
		{
			copy_self->m_Maps.push_back( dynamic_pointer_cast<IfcSurfaceTexture>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_Vertices.size(); ++ii )
	{
		auto item_ii = m_Vertices[ii];
		if( item_ii )
		{
			copy_self->m_Vertices.push_back( dynamic_pointer_cast<IfcTextureVertex>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_MappedTo ) { copy_self->m_MappedTo = dynamic_pointer_cast<IfcFace>( m_MappedTo->getDeepCopy() ); }
	return copy_self;
}
void IfcTextureMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTUREMAP" << "(";
	writeEntityList( stream, m_Maps );
	stream << ",";
	writeEntityList( stream, m_Vertices );
	stream << ",";
	if( m_MappedTo ) { stream << "#" << m_MappedTo->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcTextureMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextureMap::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextureMap, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcTextureMap, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Maps, map );
	readEntityReferenceList( args[1], m_Vertices, map );
	readEntityReference( args[2], m_MappedTo, map );
}
void IfcTextureMap::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTextureCoordinate::getAttributes( vec_attributes );
	if( m_Vertices.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Vertices_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Vertices.begin(), m_Vertices.end(), std::back_inserter( Vertices_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Vertices", Vertices_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "MappedTo", m_MappedTo ) );
}
void IfcTextureMap::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTextureCoordinate::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextureMap::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTextureCoordinate::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcTextureMap> ptr_self = dynamic_pointer_cast<IfcTextureMap>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcTextureMap::setInverseCounterparts: type mismatch" ); }
	if( m_MappedTo )
	{
		m_MappedTo->m_HasTextureMaps_inverse.push_back( ptr_self );
	}
}
void IfcTextureMap::unlinkSelf()
{
	IfcTextureCoordinate::unlinkSelf();
	if( m_MappedTo )
	{
		std::vector<weak_ptr<IfcTextureMap> >& HasTextureMaps_inverse = m_MappedTo->m_HasTextureMaps_inverse;
		std::vector<weak_ptr<IfcTextureMap> >::iterator it_HasTextureMaps_inverse;
		for( it_HasTextureMaps_inverse = HasTextureMaps_inverse.begin(); it_HasTextureMaps_inverse != HasTextureMaps_inverse.end(); ++it_HasTextureMaps_inverse)
		{
			shared_ptr<IfcTextureMap> self_candidate( *it_HasTextureMaps_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasTextureMaps_inverse.erase( it_HasTextureMaps_inverse );
				break;
			}
		}
	}
}

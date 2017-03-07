/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcIndexedTextureMap.h"
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTessellatedFaceSet.h"
#include "include/IfcTextureVertexList.h"

// ENTITY IfcIndexedTextureMap 
IfcIndexedTextureMap::IfcIndexedTextureMap() { m_entity_enum = IFCINDEXEDTEXTUREMAP; }
IfcIndexedTextureMap::IfcIndexedTextureMap( int id ) { m_id = id; m_entity_enum = IFCINDEXEDTEXTUREMAP; }
IfcIndexedTextureMap::~IfcIndexedTextureMap() {}
shared_ptr<IfcPPObject> IfcIndexedTextureMap::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcIndexedTextureMap> copy_self( new IfcIndexedTextureMap() );
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
	return copy_self;
}
void IfcIndexedTextureMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCINDEXEDTEXTUREMAP" << "(";
	writeEntityList( stream, m_Maps );
	stream << ",";
	if( m_MappedTo ) { stream << "#" << m_MappedTo->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_TexCoords ) { stream << "#" << m_TexCoords->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcIndexedTextureMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedTextureMap::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcIndexedTextureMap, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
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
	IfcTextureCoordinate::getAttributesInverse( vec_attributes_inverse );
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
void IfcIndexedTextureMap::unlinkFromInverseCounterparts()
{
	IfcTextureCoordinate::unlinkFromInverseCounterparts();
	if( m_MappedTo )
	{
		std::vector<weak_ptr<IfcIndexedTextureMap> >& HasTextures_inverse = m_MappedTo->m_HasTextures_inverse;
		for( auto it_HasTextures_inverse = HasTextures_inverse.begin(); it_HasTextures_inverse != HasTextures_inverse.end(); )
		{
			shared_ptr<IfcIndexedTextureMap> self_candidate( *it_HasTextures_inverse );
			if( self_candidate.get() == this )
			{
				it_HasTextures_inverse= HasTextures_inverse.erase( it_HasTextures_inverse );
			}
			else
			{
				++it_HasTextures_inverse;
			}
		}
	}
}

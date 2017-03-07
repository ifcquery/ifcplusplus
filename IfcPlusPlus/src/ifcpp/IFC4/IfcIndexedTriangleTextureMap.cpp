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
#include "include/IfcIndexedTriangleTextureMap.h"
#include "include/IfcPositiveInteger.h"
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTessellatedFaceSet.h"
#include "include/IfcTextureVertexList.h"

// ENTITY IfcIndexedTriangleTextureMap 
IfcIndexedTriangleTextureMap::IfcIndexedTriangleTextureMap() { m_entity_enum = IFCINDEXEDTRIANGLETEXTUREMAP; }
IfcIndexedTriangleTextureMap::IfcIndexedTriangleTextureMap( int id ) { m_id = id; m_entity_enum = IFCINDEXEDTRIANGLETEXTUREMAP; }
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
	copy_self->m_TexCoordIndex.resize( m_TexCoordIndex.size() );
	for( size_t ii=0; ii<m_TexCoordIndex.size(); ++ii )
	{
		std::vector<shared_ptr<IfcPositiveInteger> >& vec_ii = m_TexCoordIndex[ii];
		std::vector<shared_ptr<IfcPositiveInteger> >& vec_ii_target = copy_self->m_TexCoordIndex[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcPositiveInteger>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcPositiveInteger>( item_jj->getDeepCopy(options) ) );
			}
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
	stream << "("; 
	for( size_t ii = 0; ii < m_TexCoordIndex.size(); ++ii )
	{
		const std::vector<shared_ptr<IfcPositiveInteger> >&inner_vec = m_TexCoordIndex[ii];
		if( ii > 0 )
		{
			stream << "), (";
		}
		for( size_t jj = 0; jj < inner_vec.size(); ++jj )
		{
			if( jj > 0 )
			{
				stream << ", ";
			}
			const shared_ptr<IfcPositiveInteger>& type_object = inner_vec[jj];
			if( type_object )
			{
				type_object->getStepParameter( stream, false );
			}
			else
			{
				stream << "$";
			}
		}
	}
	stream << ")"; 
	stream << ");";
}
void IfcIndexedTriangleTextureMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedTriangleTextureMap::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcIndexedTriangleTextureMap, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_Maps, map );
	readEntityReference( args[1], m_MappedTo, map );
	readEntityReference( args[2], m_TexCoords, map );
	readEntityReferenceList2D( args[3], m_TexCoordIndex, map );
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
void IfcIndexedTriangleTextureMap::unlinkFromInverseCounterparts()
{
	IfcIndexedTextureMap::unlinkFromInverseCounterparts();
}

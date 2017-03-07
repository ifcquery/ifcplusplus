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
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTextureCoordinate.h"

// ENTITY IfcTextureCoordinate 
IfcTextureCoordinate::IfcTextureCoordinate() { m_entity_enum = IFCTEXTURECOORDINATE; }
IfcTextureCoordinate::IfcTextureCoordinate( int id ) { m_id = id; m_entity_enum = IFCTEXTURECOORDINATE; }
IfcTextureCoordinate::~IfcTextureCoordinate() {}
shared_ptr<IfcPPObject> IfcTextureCoordinate::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextureCoordinate> copy_self( new IfcTextureCoordinate() );
	for( size_t ii=0; ii<m_Maps.size(); ++ii )
	{
		auto item_ii = m_Maps[ii];
		if( item_ii )
		{
			copy_self->m_Maps.push_back( dynamic_pointer_cast<IfcSurfaceTexture>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTextureCoordinate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTURECOORDINATE" << "(";
	writeEntityList( stream, m_Maps );
	stream << ");";
}
void IfcTextureCoordinate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextureCoordinate::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTextureCoordinate, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_Maps, map );
}
void IfcTextureCoordinate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	if( m_Maps.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Maps_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Maps.begin(), m_Maps.end(), std::back_inserter( Maps_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Maps", Maps_vec_object ) );
	}
}
void IfcTextureCoordinate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextureCoordinate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcTextureCoordinate> ptr_self = dynamic_pointer_cast<IfcTextureCoordinate>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcTextureCoordinate::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_Maps.size(); ++i )
	{
		if( m_Maps[i] )
		{
			m_Maps[i]->m_IsMappedBy_inverse.push_back( ptr_self );
		}
	}
}
void IfcTextureCoordinate::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_Maps.size(); ++i )
	{
		if( m_Maps[i] )
		{
			std::vector<weak_ptr<IfcTextureCoordinate> >& IsMappedBy_inverse = m_Maps[i]->m_IsMappedBy_inverse;
			for( auto it_IsMappedBy_inverse = IsMappedBy_inverse.begin(); it_IsMappedBy_inverse != IsMappedBy_inverse.end(); )
			{
				shared_ptr<IfcTextureCoordinate> self_candidate( *it_IsMappedBy_inverse );
				if( self_candidate.get() == this )
				{
					it_IsMappedBy_inverse= IsMappedBy_inverse.erase( it_IsMappedBy_inverse );
				}
				else
				{
					++it_IsMappedBy_inverse;
				}
			}
		}
	}
}

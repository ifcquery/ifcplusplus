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
#include "include/IfcSurfaceStyleWithTextures.h"
#include "include/IfcSurfaceTexture.h"

// ENTITY IfcSurfaceStyleWithTextures 
IfcSurfaceStyleWithTextures::IfcSurfaceStyleWithTextures() { m_entity_enum = IFCSURFACESTYLEWITHTEXTURES; }
IfcSurfaceStyleWithTextures::IfcSurfaceStyleWithTextures( int id ) { m_id = id; m_entity_enum = IFCSURFACESTYLEWITHTEXTURES; }
IfcSurfaceStyleWithTextures::~IfcSurfaceStyleWithTextures() {}
shared_ptr<IfcPPObject> IfcSurfaceStyleWithTextures::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceStyleWithTextures> copy_self( new IfcSurfaceStyleWithTextures() );
	for( size_t ii=0; ii<m_Textures.size(); ++ii )
	{
		auto item_ii = m_Textures[ii];
		if( item_ii )
		{
			copy_self->m_Textures.push_back( dynamic_pointer_cast<IfcSurfaceTexture>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcSurfaceStyleWithTextures::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSURFACESTYLEWITHTEXTURES" << "(";
	writeEntityList( stream, m_Textures );
	stream << ");";
}
void IfcSurfaceStyleWithTextures::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyleWithTextures::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSurfaceStyleWithTextures, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_Textures, map );
}
void IfcSurfaceStyleWithTextures::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	if( m_Textures.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Textures_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Textures.begin(), m_Textures.end(), std::back_inserter( Textures_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Textures", Textures_vec_object ) );
	}
}
void IfcSurfaceStyleWithTextures::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceStyleWithTextures::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcSurfaceStyleWithTextures> ptr_self = dynamic_pointer_cast<IfcSurfaceStyleWithTextures>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcSurfaceStyleWithTextures::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_Textures.size(); ++i )
	{
		if( m_Textures[i] )
		{
			m_Textures[i]->m_UsedInStyles_inverse.push_back( ptr_self );
		}
	}
}
void IfcSurfaceStyleWithTextures::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_Textures.size(); ++i )
	{
		if( m_Textures[i] )
		{
			std::vector<weak_ptr<IfcSurfaceStyleWithTextures> >& UsedInStyles_inverse = m_Textures[i]->m_UsedInStyles_inverse;
			for( auto it_UsedInStyles_inverse = UsedInStyles_inverse.begin(); it_UsedInStyles_inverse != UsedInStyles_inverse.end(); )
			{
				shared_ptr<IfcSurfaceStyleWithTextures> self_candidate( *it_UsedInStyles_inverse );
				if( self_candidate.get() == this )
				{
					it_UsedInStyles_inverse= UsedInStyles_inverse.erase( it_UsedInStyles_inverse );
				}
				else
				{
					++it_UsedInStyles_inverse;
				}
			}
		}
	}
}

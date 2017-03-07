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
#include "include/IfcCartesianPointList3D.h"
#include "include/IfcIndexedColourMap.h"
#include "include/IfcIndexedTextureMap.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTessellatedFaceSet.h"

// ENTITY IfcTessellatedFaceSet 
IfcTessellatedFaceSet::IfcTessellatedFaceSet() { m_entity_enum = IFCTESSELLATEDFACESET; }
IfcTessellatedFaceSet::IfcTessellatedFaceSet( int id ) { m_id = id; m_entity_enum = IFCTESSELLATEDFACESET; }
IfcTessellatedFaceSet::~IfcTessellatedFaceSet() {}
shared_ptr<IfcPPObject> IfcTessellatedFaceSet::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTessellatedFaceSet> copy_self( new IfcTessellatedFaceSet() );
	if( m_Coordinates ) { copy_self->m_Coordinates = dynamic_pointer_cast<IfcCartesianPointList3D>( m_Coordinates->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTessellatedFaceSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTESSELLATEDFACESET" << "(";
	if( m_Coordinates ) { stream << "#" << m_Coordinates->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcTessellatedFaceSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTessellatedFaceSet::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTessellatedFaceSet, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Coordinates, map );
}
void IfcTessellatedFaceSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTessellatedItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Coordinates", m_Coordinates ) );
}
void IfcTessellatedFaceSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTessellatedItem::getAttributesInverse( vec_attributes_inverse );
	if( m_HasColours_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasColours_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasColours_inverse.size(); ++i )
		{
			if( !m_HasColours_inverse[i].expired() )
			{
				HasColours_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcIndexedColourMap>( m_HasColours_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasColours_inverse", HasColours_inverse_vec_obj ) );
	}
	if( m_HasTextures_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasTextures_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasTextures_inverse.size(); ++i )
		{
			if( !m_HasTextures_inverse[i].expired() )
			{
				HasTextures_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcIndexedTextureMap>( m_HasTextures_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasTextures_inverse", HasTextures_inverse_vec_obj ) );
	}
}
void IfcTessellatedFaceSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTessellatedItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTessellatedFaceSet::unlinkFromInverseCounterparts()
{
	IfcTessellatedItem::unlinkFromInverseCounterparts();
}

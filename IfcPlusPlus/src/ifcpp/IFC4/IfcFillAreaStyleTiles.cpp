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
#include "include/IfcFillAreaStyleTiles.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVector.h"

// ENTITY IfcFillAreaStyleTiles 
IfcFillAreaStyleTiles::IfcFillAreaStyleTiles() { m_entity_enum = IFCFILLAREASTYLETILES; }
IfcFillAreaStyleTiles::IfcFillAreaStyleTiles( int id ) { m_id = id; m_entity_enum = IFCFILLAREASTYLETILES; }
IfcFillAreaStyleTiles::~IfcFillAreaStyleTiles() {}
shared_ptr<IfcPPObject> IfcFillAreaStyleTiles::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFillAreaStyleTiles> copy_self( new IfcFillAreaStyleTiles() );
	for( size_t ii=0; ii<m_TilingPattern.size(); ++ii )
	{
		auto item_ii = m_TilingPattern[ii];
		if( item_ii )
		{
			copy_self->m_TilingPattern.push_back( dynamic_pointer_cast<IfcVector>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_Tiles.size(); ++ii )
	{
		auto item_ii = m_Tiles[ii];
		if( item_ii )
		{
			copy_self->m_Tiles.push_back( dynamic_pointer_cast<IfcStyledItem>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_TilingScale ) { copy_self->m_TilingScale = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_TilingScale->getDeepCopy(options) ); }
	return copy_self;
}
void IfcFillAreaStyleTiles::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCFILLAREASTYLETILES" << "(";
	writeEntityList( stream, m_TilingPattern );
	stream << ",";
	writeEntityList( stream, m_Tiles );
	stream << ",";
	if( m_TilingScale ) { m_TilingScale->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcFillAreaStyleTiles::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFillAreaStyleTiles::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcFillAreaStyleTiles, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_TilingPattern, map );
	readEntityReferenceList( args[1], m_Tiles, map );
	m_TilingScale = IfcPositiveRatioMeasure::createObjectFromSTEP( args[2] );
}
void IfcFillAreaStyleTiles::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	if( m_TilingPattern.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> TilingPattern_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_TilingPattern.begin(), m_TilingPattern.end(), std::back_inserter( TilingPattern_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "TilingPattern", TilingPattern_vec_object ) );
	}
	if( m_Tiles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Tiles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Tiles.begin(), m_Tiles.end(), std::back_inserter( Tiles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Tiles", Tiles_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "TilingScale", m_TilingScale ) );
}
void IfcFillAreaStyleTiles::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcFillAreaStyleTiles::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcFillAreaStyleTiles::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}

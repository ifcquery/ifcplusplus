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
#include "include/IfcFillAreaStyleTiles.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVector.h"

// ENTITY IfcFillAreaStyleTiles 
IfcFillAreaStyleTiles::IfcFillAreaStyleTiles() {}
IfcFillAreaStyleTiles::IfcFillAreaStyleTiles( int id ) { m_id = id; }
IfcFillAreaStyleTiles::~IfcFillAreaStyleTiles() {}
shared_ptr<IfcPPObject> IfcFillAreaStyleTiles::getDeepCopy()
{
	shared_ptr<IfcFillAreaStyleTiles> copy_self( new IfcFillAreaStyleTiles() );
	for( size_t ii=0; ii<m_TilingPattern.size(); ++ii )
	{
		auto item_ii = m_TilingPattern[ii];
		if( item_ii )
		{
			copy_self->m_TilingPattern.push_back( dynamic_pointer_cast<IfcVector>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_Tiles.size(); ++ii )
	{
		auto item_ii = m_Tiles[ii];
		if( item_ii )
		{
			copy_self->m_Tiles.push_back( dynamic_pointer_cast<IfcStyledItem>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_TilingScale ) { copy_self->m_TilingScale = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_TilingScale->getDeepCopy() ); }
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
void IfcFillAreaStyleTiles::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFillAreaStyleTiles, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcFillAreaStyleTiles, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_TilingPattern, map );
	readEntityReferenceList( args[1], m_Tiles, map );
	m_TilingScale = IfcPositiveRatioMeasure::createObjectFromStepData( args[2] );
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
void IfcFillAreaStyleTiles::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}

/* -*-c++-*- IfcPlusPlus - www.ifcquery.com - Copyright (C) 2011 Fabian Gerold
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
#include "include/IfcColourRgbList.h"
#include "include/IfcIndexedColourMap.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcPositiveInteger.h"
#include "include/IfcTessellatedFaceSet.h"

// ENTITY IfcIndexedColourMap 
IfcIndexedColourMap::IfcIndexedColourMap() { m_entity_enum = IFCINDEXEDCOLOURMAP; }
IfcIndexedColourMap::IfcIndexedColourMap( int id ) { m_id = id; m_entity_enum = IFCINDEXEDCOLOURMAP; }
IfcIndexedColourMap::~IfcIndexedColourMap() {}
shared_ptr<IfcPPObject> IfcIndexedColourMap::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcIndexedColourMap> copy_self( new IfcIndexedColourMap() );
	if( m_MappedTo ) { copy_self->m_MappedTo = dynamic_pointer_cast<IfcTessellatedFaceSet>( m_MappedTo->getDeepCopy(options) ); }
	if( m_Opacity ) { copy_self->m_Opacity = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Opacity->getDeepCopy(options) ); }
	if( m_Colours ) { copy_self->m_Colours = dynamic_pointer_cast<IfcColourRgbList>( m_Colours->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ColourIndex.size(); ++ii )
	{
		auto item_ii = m_ColourIndex[ii];
		if( item_ii )
		{
			copy_self->m_ColourIndex.push_back( dynamic_pointer_cast<IfcPositiveInteger>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcIndexedColourMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCINDEXEDCOLOURMAP" << "(";
	if( m_MappedTo ) { stream << "#" << m_MappedTo->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Opacity ) { m_Opacity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Colours ) { stream << "#" << m_Colours->m_id; } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_ColourIndex.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcPositiveInteger>& type_object = m_ColourIndex[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ");";
}
void IfcIndexedColourMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedColourMap::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcIndexedColourMap, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_MappedTo, map );
	m_Opacity = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Colours, map );
	readSelectList( args[3], m_ColourIndex, map );
}
void IfcIndexedColourMap::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "MappedTo", m_MappedTo ) );
	vec_attributes.push_back( std::make_pair( "Opacity", m_Opacity ) );
	vec_attributes.push_back( std::make_pair( "Colours", m_Colours ) );
	if( m_ColourIndex.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ColourIndex_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ColourIndex.begin(), m_ColourIndex.end(), std::back_inserter( ColourIndex_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ColourIndex", ColourIndex_vec_object ) );
	}
}
void IfcIndexedColourMap::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcIndexedColourMap::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcIndexedColourMap> ptr_self = dynamic_pointer_cast<IfcIndexedColourMap>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcIndexedColourMap::setInverseCounterparts: type mismatch" ); }
	if( m_MappedTo )
	{
		m_MappedTo->m_HasColours_inverse.push_back( ptr_self );
	}
}
void IfcIndexedColourMap::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
	if( m_MappedTo )
	{
		std::vector<weak_ptr<IfcIndexedColourMap> >& HasColours_inverse = m_MappedTo->m_HasColours_inverse;
		for( auto it_HasColours_inverse = HasColours_inverse.begin(); it_HasColours_inverse != HasColours_inverse.end(); )
		{
			shared_ptr<IfcIndexedColourMap> self_candidate( *it_HasColours_inverse );
			if( self_candidate.get() == this )
			{
				it_HasColours_inverse= HasColours_inverse.erase( it_HasColours_inverse );
			}
			else
			{
				++it_HasColours_inverse;
			}
		}
	}
}

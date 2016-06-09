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
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcBoolean.h"
#include "include/IfcCartesianPointList3D.h"
#include "include/IfcIndexedColourMap.h"
#include "include/IfcIndexedTextureMap.h"
#include "include/IfcParameterValue.h"
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
	copy_self->m_Normals.resize( m_Normals.size() );
	for( size_t ii=0; ii<m_Normals.size(); ++ii )
	{
		std::vector<shared_ptr<IfcParameterValue> >& vec_ii = m_Normals[ii];
		std::vector<shared_ptr<IfcParameterValue> >& vec_ii_target = copy_self->m_Normals[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcParameterValue>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcParameterValue>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	if( m_Closed ) { copy_self->m_Closed = dynamic_pointer_cast<IfcBoolean>( m_Closed->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTessellatedFaceSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTESSELLATEDFACESET" << "(";
	if( m_Coordinates ) { stream << "#" << m_Coordinates->m_id; } else { stream << "$"; }
	stream << ",";
	writeNumericTypeList2D( stream, m_Normals );
	stream << ",";
	if( m_Closed ) { m_Closed->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTessellatedFaceSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTessellatedFaceSet::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTessellatedFaceSet, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Coordinates, map );
	readTypeOfRealList2D( args[1], m_Normals );
	m_Closed = IfcBoolean::createObjectFromSTEP( args[2] );
}
void IfcTessellatedFaceSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTessellatedItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Coordinates", m_Coordinates ) );
	vec_attributes.push_back( std::make_pair( "Closed", m_Closed ) );
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

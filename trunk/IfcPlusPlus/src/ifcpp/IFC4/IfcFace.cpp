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
#include "include/IfcFaceBound.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTextureMap.h"

// ENTITY IfcFace 
IfcFace::IfcFace() {}
IfcFace::IfcFace( int id ) { m_id = id; }
IfcFace::~IfcFace() {}
shared_ptr<IfcPPObject> IfcFace::getDeepCopy()
{
	shared_ptr<IfcFace> copy_self( new IfcFace() );
	for( size_t ii=0; ii<m_Bounds.size(); ++ii )
	{
		auto item_ii = m_Bounds[ii];
		if( item_ii )
		{
			copy_self->m_Bounds.push_back( dynamic_pointer_cast<IfcFaceBound>(item_ii->getDeepCopy() ) );
		}
	}
	return copy_self;
}
void IfcFace::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCFACE" << "(";
	writeEntityList( stream, m_Bounds );
	stream << ");";
}
void IfcFace::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFace::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFace, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcFace, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Bounds, map );
}
void IfcFace::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTopologicalRepresentationItem::getAttributes( vec_attributes );
	if( m_Bounds.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Bounds_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Bounds.begin(), m_Bounds.end(), std::back_inserter( Bounds_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Bounds", Bounds_vec_object ) );
	}
}
void IfcFace::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTopologicalRepresentationItem::getAttributesInverse( vec_attributes_inverse );
	if( m_HasTextureMaps_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasTextureMaps_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasTextureMaps_inverse.size(); ++i )
		{
			if( !m_HasTextureMaps_inverse[i].expired() )
			{
				HasTextureMaps_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcTextureMap>( m_HasTextureMaps_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasTextureMaps_inverse", HasTextureMaps_inverse_vec_obj ) );
	}
}
void IfcFace::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTopologicalRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcFace::unlinkSelf()
{
	IfcTopologicalRepresentationItem::unlinkSelf();
}

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
#include "include/IfcLocalPlacement.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcProduct.h"

// ENTITY IfcObjectPlacement 
IfcObjectPlacement::IfcObjectPlacement() {}
IfcObjectPlacement::IfcObjectPlacement( int id ) { m_id = id; }
IfcObjectPlacement::~IfcObjectPlacement() {}
shared_ptr<IfcPPObject> IfcObjectPlacement::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcObjectPlacement> copy_self( new IfcObjectPlacement() );
	return copy_self;
}
void IfcObjectPlacement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCOBJECTPLACEMENT" << "(";
	stream << ");";
}
void IfcObjectPlacement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcObjectPlacement::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcObjectPlacement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}
void IfcObjectPlacement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_PlacesObject_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PlacesObject_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PlacesObject_inverse.size(); ++i )
		{
			if( !m_PlacesObject_inverse[i].expired() )
			{
				PlacesObject_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcProduct>( m_PlacesObject_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PlacesObject_inverse", PlacesObject_inverse_vec_obj ) );
	}
	if( m_ReferencedByPlacements_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ReferencedByPlacements_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_ReferencedByPlacements_inverse.size(); ++i )
		{
			if( !m_ReferencedByPlacements_inverse[i].expired() )
			{
				ReferencedByPlacements_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcLocalPlacement>( m_ReferencedByPlacements_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "ReferencedByPlacements_inverse", ReferencedByPlacements_inverse_vec_obj ) );
	}
}
void IfcObjectPlacement::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcObjectPlacement::unlinkSelf()
{
}

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
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRepresentationItem.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcRepresentationItem 
IfcRepresentationItem::IfcRepresentationItem() {}
IfcRepresentationItem::IfcRepresentationItem( int id ) { m_id = id; }
IfcRepresentationItem::~IfcRepresentationItem() {}
shared_ptr<IfcPPObject> IfcRepresentationItem::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRepresentationItem> copy_self( new IfcRepresentationItem() );
	return copy_self;
}
void IfcRepresentationItem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREPRESENTATIONITEM" << "(";
	stream << ");";
}
void IfcRepresentationItem::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRepresentationItem::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcRepresentationItem::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}
void IfcRepresentationItem::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_LayerAssignment_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LayerAssignment_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_LayerAssignment_inverse.size(); ++i )
		{
			if( !m_LayerAssignment_inverse[i].expired() )
			{
				LayerAssignment_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPresentationLayerAssignment>( m_LayerAssignment_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "LayerAssignment_inverse", LayerAssignment_inverse_vec_obj ) );
	}
	if( m_StyledByItem_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> StyledByItem_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_StyledByItem_inverse.size(); ++i )
		{
			if( !m_StyledByItem_inverse[i].expired() )
			{
				StyledByItem_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcStyledItem>( m_StyledByItem_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "StyledByItem_inverse", StyledByItem_inverse_vec_obj ) );
	}
}
void IfcRepresentationItem::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcRepresentationItem::unlinkSelf()
{
}

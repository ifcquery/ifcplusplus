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
#include "include/IfcCartesianTransformationOperator.h"
#include "include/IfcMappedItem.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcMappedItem 
IfcMappedItem::IfcMappedItem() {}
IfcMappedItem::IfcMappedItem( int id ) { m_id = id; }
IfcMappedItem::~IfcMappedItem() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMappedItem::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMappedItem> other = dynamic_pointer_cast<IfcMappedItem>(other_entity);
	if( !other) { return; }
	m_MappingSource = other->m_MappingSource;
	m_MappingTarget = other->m_MappingTarget;
}
void IfcMappedItem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMAPPEDITEM" << "(";
	if( m_MappingSource ) { stream << "#" << m_MappingSource->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_MappingTarget ) { stream << "#" << m_MappingTarget->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcMappedItem::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMappedItem::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMappedItem, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcMappedItem, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_MappingSource, map );
	readEntityReference( args[1], m_MappingTarget, map );
}
void IfcMappedItem::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "MappingSource", m_MappingSource ) );
	vec_attributes.push_back( std::make_pair( "MappingTarget", m_MappingTarget ) );
}
void IfcMappedItem::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMappedItem::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRepresentationItem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMappedItem> ptr_self = dynamic_pointer_cast<IfcMappedItem>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcMappedItem::setInverseCounterparts: type mismatch" ); }
	if( m_MappingSource )
	{
		m_MappingSource->m_MapUsage_inverse.push_back( ptr_self );
	}
}
void IfcMappedItem::unlinkSelf()
{
	IfcRepresentationItem::unlinkSelf();
	if( m_MappingSource )
	{
		std::vector<weak_ptr<IfcMappedItem> >& MapUsage_inverse = m_MappingSource->m_MapUsage_inverse;
		std::vector<weak_ptr<IfcMappedItem> >::iterator it_MapUsage_inverse;
		for( it_MapUsage_inverse = MapUsage_inverse.begin(); it_MapUsage_inverse != MapUsage_inverse.end(); ++it_MapUsage_inverse)
		{
			shared_ptr<IfcMappedItem> self_candidate( *it_MapUsage_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				MapUsage_inverse.erase( it_MapUsage_inverse );
				break;
			}
		}
	}
}

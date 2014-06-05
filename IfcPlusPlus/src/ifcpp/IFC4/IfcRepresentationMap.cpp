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
#include "include/IfcAxis2Placement.h"
#include "include/IfcMappedItem.h"
#include "include/IfcRepresentation.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcShapeAspect.h"

// ENTITY IfcRepresentationMap 
IfcRepresentationMap::IfcRepresentationMap() {}
IfcRepresentationMap::IfcRepresentationMap( int id ) { m_id = id; }
IfcRepresentationMap::~IfcRepresentationMap() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRepresentationMap::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRepresentationMap> other = dynamic_pointer_cast<IfcRepresentationMap>(other_entity);
	if( !other) { return; }
	m_MappingOrigin = other->m_MappingOrigin;
	m_MappedRepresentation = other->m_MappedRepresentation;
}
void IfcRepresentationMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCREPRESENTATIONMAP" << "(";
	if( m_MappingOrigin ) { m_MappingOrigin->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_MappedRepresentation ) { stream << "#" << m_MappedRepresentation->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRepresentationMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRepresentationMap::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRepresentationMap, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcRepresentationMap, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_MappingOrigin = IfcAxis2Placement::createObjectFromStepData( args[0], map );
	readEntityReference( args[1], m_MappedRepresentation, map );
}
void IfcRepresentationMap::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "MappingOrigin", m_MappingOrigin ) );
	vec_attributes.push_back( std::make_pair( "MappedRepresentation", m_MappedRepresentation ) );
}
void IfcRepresentationMap::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasShapeAspects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasShapeAspects_inverse.size(); ++i ) { HasShapeAspects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcShapeAspect>( m_HasShapeAspects_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasShapeAspects_inverse", HasShapeAspects_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> MapUsage_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_MapUsage_inverse.size(); ++i ) { MapUsage_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMappedItem>( m_MapUsage_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "MapUsage_inverse", MapUsage_inverse_vec_obj ) );
}
void IfcRepresentationMap::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcRepresentationMap> ptr_self = dynamic_pointer_cast<IfcRepresentationMap>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRepresentationMap::setInverseCounterparts: type mismatch" ); }
	if( m_MappedRepresentation )
	{
		m_MappedRepresentation->m_RepresentationMap_inverse.push_back( ptr_self );
	}
}
void IfcRepresentationMap::unlinkSelf()
{
	if( m_MappedRepresentation )
	{
		std::vector<weak_ptr<IfcRepresentationMap> >& RepresentationMap_inverse = m_MappedRepresentation->m_RepresentationMap_inverse;
		std::vector<weak_ptr<IfcRepresentationMap> >::iterator it_RepresentationMap_inverse;
		for( it_RepresentationMap_inverse = RepresentationMap_inverse.begin(); it_RepresentationMap_inverse != RepresentationMap_inverse.end(); ++it_RepresentationMap_inverse)
		{
			shared_ptr<IfcRepresentationMap> self_candidate( *it_RepresentationMap_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				RepresentationMap_inverse.erase( it_RepresentationMap_inverse );
				break;
			}
		}
	}
}

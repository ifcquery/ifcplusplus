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
#include "include/IfcAxis2Placement.h"
#include "include/IfcMappedItem.h"
#include "include/IfcRepresentation.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcShapeAspect.h"

// ENTITY IfcRepresentationMap 
IfcRepresentationMap::IfcRepresentationMap() { m_entity_enum = IFCREPRESENTATIONMAP; }
IfcRepresentationMap::IfcRepresentationMap( int id ) { m_id = id; m_entity_enum = IFCREPRESENTATIONMAP; }
IfcRepresentationMap::~IfcRepresentationMap() {}
shared_ptr<IfcPPObject> IfcRepresentationMap::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRepresentationMap> copy_self( new IfcRepresentationMap() );
	if( m_MappingOrigin ) { copy_self->m_MappingOrigin = dynamic_pointer_cast<IfcAxis2Placement>( m_MappingOrigin->getDeepCopy(options) ); }
	if( m_MappedRepresentation ) { copy_self->m_MappedRepresentation = dynamic_pointer_cast<IfcRepresentation>( m_MappedRepresentation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRepresentationMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREPRESENTATIONMAP" << "(";
	if( m_MappingOrigin ) { m_MappingOrigin->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_MappedRepresentation ) { stream << "#" << m_MappedRepresentation->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcRepresentationMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRepresentationMap::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRepresentationMap, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_MappingOrigin = IfcAxis2Placement::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_MappedRepresentation, map );
}
void IfcRepresentationMap::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "MappingOrigin", m_MappingOrigin ) );
	vec_attributes.push_back( std::make_pair( "MappedRepresentation", m_MappedRepresentation ) );
}
void IfcRepresentationMap::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_HasShapeAspects_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasShapeAspects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasShapeAspects_inverse.size(); ++i )
		{
			if( !m_HasShapeAspects_inverse[i].expired() )
			{
				HasShapeAspects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcShapeAspect>( m_HasShapeAspects_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasShapeAspects_inverse", HasShapeAspects_inverse_vec_obj ) );
	}
	if( m_MapUsage_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> MapUsage_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_MapUsage_inverse.size(); ++i )
		{
			if( !m_MapUsage_inverse[i].expired() )
			{
				MapUsage_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMappedItem>( m_MapUsage_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "MapUsage_inverse", MapUsage_inverse_vec_obj ) );
	}
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
void IfcRepresentationMap::unlinkFromInverseCounterparts()
{
	if( m_MappedRepresentation )
	{
		std::vector<weak_ptr<IfcRepresentationMap> >& RepresentationMap_inverse = m_MappedRepresentation->m_RepresentationMap_inverse;
		for( auto it_RepresentationMap_inverse = RepresentationMap_inverse.begin(); it_RepresentationMap_inverse != RepresentationMap_inverse.end(); )
		{
			shared_ptr<IfcRepresentationMap> self_candidate( *it_RepresentationMap_inverse );
			if( self_candidate.get() == this )
			{
				it_RepresentationMap_inverse= RepresentationMap_inverse.erase( it_RepresentationMap_inverse );
			}
			else
			{
				++it_RepresentationMap_inverse;
			}
		}
	}
}

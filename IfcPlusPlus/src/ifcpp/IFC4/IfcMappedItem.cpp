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
#include "include/IfcCartesianTransformationOperator.h"
#include "include/IfcMappedItem.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcMappedItem 
IfcMappedItem::IfcMappedItem() { m_entity_enum = IFCMAPPEDITEM; }
IfcMappedItem::IfcMappedItem( int id ) { m_id = id; m_entity_enum = IFCMAPPEDITEM; }
IfcMappedItem::~IfcMappedItem() {}
shared_ptr<IfcPPObject> IfcMappedItem::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMappedItem> copy_self( new IfcMappedItem() );
	if( m_MappingSource ) { copy_self->m_MappingSource = dynamic_pointer_cast<IfcRepresentationMap>( m_MappingSource->getDeepCopy(options) ); }
	if( m_MappingTarget ) { copy_self->m_MappingTarget = dynamic_pointer_cast<IfcCartesianTransformationOperator>( m_MappingTarget->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMappedItem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMAPPEDITEM" << "(";
	if( m_MappingSource ) { stream << "#" << m_MappingSource->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_MappingTarget ) { stream << "#" << m_MappingTarget->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcMappedItem::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMappedItem::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMappedItem, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
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
	IfcRepresentationItem::getAttributesInverse( vec_attributes_inverse );
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
void IfcMappedItem::unlinkFromInverseCounterparts()
{
	IfcRepresentationItem::unlinkFromInverseCounterparts();
	if( m_MappingSource )
	{
		std::vector<weak_ptr<IfcMappedItem> >& MapUsage_inverse = m_MappingSource->m_MapUsage_inverse;
		for( auto it_MapUsage_inverse = MapUsage_inverse.begin(); it_MapUsage_inverse != MapUsage_inverse.end(); )
		{
			shared_ptr<IfcMappedItem> self_candidate( *it_MapUsage_inverse );
			if( self_candidate.get() == this )
			{
				it_MapUsage_inverse= MapUsage_inverse.erase( it_MapUsage_inverse );
			}
			else
			{
				++it_MapUsage_inverse;
			}
		}
	}
}

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
#include "include/IfcGridAxis.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcVirtualGridIntersection.h"

// ENTITY IfcVirtualGridIntersection 
IfcVirtualGridIntersection::IfcVirtualGridIntersection() {}
IfcVirtualGridIntersection::IfcVirtualGridIntersection( int id ) { m_id = id; }
IfcVirtualGridIntersection::~IfcVirtualGridIntersection() {}
shared_ptr<IfcPPObject> IfcVirtualGridIntersection::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcVirtualGridIntersection> copy_self( new IfcVirtualGridIntersection() );
	for( size_t ii=0; ii<m_IntersectingAxes.size(); ++ii )
	{
		auto item_ii = m_IntersectingAxes[ii];
		if( item_ii )
		{
			copy_self->m_IntersectingAxes.push_back( dynamic_pointer_cast<IfcGridAxis>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_OffsetDistances.size(); ++ii )
	{
		auto item_ii = m_OffsetDistances[ii];
		if( item_ii )
		{
			copy_self->m_OffsetDistances.push_back( dynamic_pointer_cast<IfcLengthMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcVirtualGridIntersection::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCVIRTUALGRIDINTERSECTION" << "(";
	writeEntityList( stream, m_IntersectingAxes );
	stream << ",";
	writeTypeOfRealList( stream, m_OffsetDistances );
	stream << ");";
}
void IfcVirtualGridIntersection::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcVirtualGridIntersection::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcVirtualGridIntersection, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_IntersectingAxes, map );
	readTypeOfRealList( args[1], m_OffsetDistances );
}
void IfcVirtualGridIntersection::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_IntersectingAxes.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> IntersectingAxes_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_IntersectingAxes.begin(), m_IntersectingAxes.end(), std::back_inserter( IntersectingAxes_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "IntersectingAxes", IntersectingAxes_vec_object ) );
	}
	if( m_OffsetDistances.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> OffsetDistances_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_OffsetDistances.begin(), m_OffsetDistances.end(), std::back_inserter( OffsetDistances_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "OffsetDistances", OffsetDistances_vec_object ) );
	}
}
void IfcVirtualGridIntersection::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcVirtualGridIntersection::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcVirtualGridIntersection> ptr_self = dynamic_pointer_cast<IfcVirtualGridIntersection>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcVirtualGridIntersection::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_IntersectingAxes.size(); ++i )
	{
		if( m_IntersectingAxes[i] )
		{
			m_IntersectingAxes[i]->m_HasIntersections_inverse.push_back( ptr_self );
		}
	}
}
void IfcVirtualGridIntersection::unlinkSelf()
{
	for( size_t i=0; i<m_IntersectingAxes.size(); ++i )
	{
		if( m_IntersectingAxes[i] )
		{
			std::vector<weak_ptr<IfcVirtualGridIntersection> >& HasIntersections_inverse = m_IntersectingAxes[i]->m_HasIntersections_inverse;
			for( auto it_HasIntersections_inverse = HasIntersections_inverse.begin(); it_HasIntersections_inverse != HasIntersections_inverse.end(); ++it_HasIntersections_inverse)
			{
				shared_ptr<IfcVirtualGridIntersection> self_candidate( *it_HasIntersections_inverse );
				if( self_candidate.get() == this )
				{
					HasIntersections_inverse.erase( it_HasIntersections_inverse );
					break;
				}
			}
		}
	}
}

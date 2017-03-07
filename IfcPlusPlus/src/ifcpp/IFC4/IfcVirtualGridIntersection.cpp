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
#include "include/IfcGridAxis.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcVirtualGridIntersection.h"

// ENTITY IfcVirtualGridIntersection 
IfcVirtualGridIntersection::IfcVirtualGridIntersection() { m_entity_enum = IFCVIRTUALGRIDINTERSECTION; }
IfcVirtualGridIntersection::IfcVirtualGridIntersection( int id ) { m_id = id; m_entity_enum = IFCVIRTUALGRIDINTERSECTION; }
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
	writeNumericTypeList( stream, m_OffsetDistances );
	stream << ");";
}
void IfcVirtualGridIntersection::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcVirtualGridIntersection::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcVirtualGridIntersection, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
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
void IfcVirtualGridIntersection::unlinkFromInverseCounterparts()
{
	for( size_t i=0; i<m_IntersectingAxes.size(); ++i )
	{
		if( m_IntersectingAxes[i] )
		{
			std::vector<weak_ptr<IfcVirtualGridIntersection> >& HasIntersections_inverse = m_IntersectingAxes[i]->m_HasIntersections_inverse;
			for( auto it_HasIntersections_inverse = HasIntersections_inverse.begin(); it_HasIntersections_inverse != HasIntersections_inverse.end(); )
			{
				shared_ptr<IfcVirtualGridIntersection> self_candidate( *it_HasIntersections_inverse );
				if( self_candidate.get() == this )
				{
					it_HasIntersections_inverse= HasIntersections_inverse.erase( it_HasIntersections_inverse );
				}
				else
				{
					++it_HasIntersections_inverse;
				}
			}
		}
	}
}

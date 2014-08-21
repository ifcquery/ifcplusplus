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
#include "include/IfcCompositeCurve.h"
#include "include/IfcCompositeCurveSegment.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcCompositeCurve 
IfcCompositeCurve::IfcCompositeCurve() {}
IfcCompositeCurve::IfcCompositeCurve( int id ) { m_id = id; }
IfcCompositeCurve::~IfcCompositeCurve() {}
shared_ptr<IfcPPObject> IfcCompositeCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCompositeCurve> copy_self( new IfcCompositeCurve() );
	for( size_t ii=0; ii<m_Segments.size(); ++ii )
	{
		auto item_ii = m_Segments[ii];
		if( item_ii )
		{
			copy_self->m_Segments.push_back( dynamic_pointer_cast<IfcCompositeCurveSegment>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = m_SelfIntersect; }
	return copy_self;
}
void IfcCompositeCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOMPOSITECURVE" << "(";
	writeEntityList( stream, m_Segments );
	stream << ",";
	if( m_SelfIntersect == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_SelfIntersect == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ");";
}
void IfcCompositeCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCompositeCurve::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCompositeCurve, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_Segments, map );
	if( boost::iequals( args[1], L".F." ) ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( boost::iequals( args[1], L".T." ) ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( boost::iequals( args[1], L".U." ) ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
}
void IfcCompositeCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedCurve::getAttributes( vec_attributes );
	if( m_Segments.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Segments_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Segments.begin(), m_Segments.end(), std::back_inserter( Segments_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Segments", Segments_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "SelfIntersect", shared_ptr<IfcPPLogicalAttribute>( new IfcPPLogicalAttribute( m_SelfIntersect ) ) ) );
}
void IfcCompositeCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcCompositeCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedCurve::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcCompositeCurve> ptr_self = dynamic_pointer_cast<IfcCompositeCurve>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcCompositeCurve::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_Segments.size(); ++i )
	{
		if( m_Segments[i] )
		{
			m_Segments[i]->m_UsingCurves_inverse.push_back( ptr_self );
		}
	}
}
void IfcCompositeCurve::unlinkSelf()
{
	IfcBoundedCurve::unlinkSelf();
	for( size_t i=0; i<m_Segments.size(); ++i )
	{
		if( m_Segments[i] )
		{
			std::vector<weak_ptr<IfcCompositeCurve> >& UsingCurves_inverse = m_Segments[i]->m_UsingCurves_inverse;
			for( auto it_UsingCurves_inverse = UsingCurves_inverse.begin(); it_UsingCurves_inverse != UsingCurves_inverse.end(); ++it_UsingCurves_inverse)
			{
				shared_ptr<IfcCompositeCurve> self_candidate( *it_UsingCurves_inverse );
				if( self_candidate.get() == this )
				{
					UsingCurves_inverse.erase( it_UsingCurves_inverse );
					break;
				}
			}
		}
	}
}

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
#include "include/IfcBoolean.h"
#include "include/IfcCartesianPointList.h"
#include "include/IfcIndexedPolyCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcSegmentIndexSelect.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcIndexedPolyCurve 
IfcIndexedPolyCurve::IfcIndexedPolyCurve() { m_entity_enum = IFCINDEXEDPOLYCURVE; }
IfcIndexedPolyCurve::IfcIndexedPolyCurve( int id ) { m_id = id; m_entity_enum = IFCINDEXEDPOLYCURVE; }
IfcIndexedPolyCurve::~IfcIndexedPolyCurve() {}
shared_ptr<IfcPPObject> IfcIndexedPolyCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcIndexedPolyCurve> copy_self( new IfcIndexedPolyCurve() );
	if( m_Points ) { copy_self->m_Points = dynamic_pointer_cast<IfcCartesianPointList>( m_Points->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Segments.size(); ++ii )
	{
		auto item_ii = m_Segments[ii];
		if( item_ii )
		{
			copy_self->m_Segments.push_back( dynamic_pointer_cast<IfcSegmentIndexSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcBoolean>( m_SelfIntersect->getDeepCopy(options) ); }
	return copy_self;
}
void IfcIndexedPolyCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCINDEXEDPOLYCURVE" << "(";
	if( m_Points ) { stream << "#" << m_Points->m_id; } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_Segments.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcSegmentIndexSelect>& type_object = m_Segments[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcIndexedPolyCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedPolyCurve::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcIndexedPolyCurve, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Points, map );
	readSelectList( args[1], m_Segments, map );
	m_SelfIntersect = IfcBoolean::createObjectFromSTEP( args[2] );
}
void IfcIndexedPolyCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Points", m_Points ) );
	if( m_Segments.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Segments_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Segments.begin(), m_Segments.end(), std::back_inserter( Segments_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Segments", Segments_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "SelfIntersect", m_SelfIntersect ) );
}
void IfcIndexedPolyCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcIndexedPolyCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcIndexedPolyCurve::unlinkFromInverseCounterparts()
{
	IfcBoundedCurve::unlinkFromInverseCounterparts();
}

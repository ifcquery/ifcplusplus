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
#include "include/IfcCompositeCurveSegment.h"
#include "include/IfcLogical.h"
#include "include/IfcOuterBoundaryCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcOuterBoundaryCurve 
IfcOuterBoundaryCurve::IfcOuterBoundaryCurve() { m_entity_enum = IFCOUTERBOUNDARYCURVE; }
IfcOuterBoundaryCurve::IfcOuterBoundaryCurve( int id ) { m_id = id; m_entity_enum = IFCOUTERBOUNDARYCURVE; }
IfcOuterBoundaryCurve::~IfcOuterBoundaryCurve() {}
shared_ptr<IfcPPObject> IfcOuterBoundaryCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcOuterBoundaryCurve> copy_self( new IfcOuterBoundaryCurve() );
	for( size_t ii=0; ii<m_Segments.size(); ++ii )
	{
		auto item_ii = m_Segments[ii];
		if( item_ii )
		{
			copy_self->m_Segments.push_back( dynamic_pointer_cast<IfcCompositeCurveSegment>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	return copy_self;
}
void IfcOuterBoundaryCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCOUTERBOUNDARYCURVE" << "(";
	writeEntityList( stream, m_Segments );
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcOuterBoundaryCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOuterBoundaryCurve::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcOuterBoundaryCurve, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_Segments, map );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[1] );
}
void IfcOuterBoundaryCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundaryCurve::getAttributes( vec_attributes );
}
void IfcOuterBoundaryCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundaryCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcOuterBoundaryCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundaryCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcOuterBoundaryCurve::unlinkFromInverseCounterparts()
{
	IfcBoundaryCurve::unlinkFromInverseCounterparts();
}

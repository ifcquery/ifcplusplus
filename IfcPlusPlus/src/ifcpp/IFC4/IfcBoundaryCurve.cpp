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
#include "include/IfcBoundaryCurve.h"
#include "include/IfcCompositeCurveSegment.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBoundaryCurve 
IfcBoundaryCurve::IfcBoundaryCurve() {}
IfcBoundaryCurve::IfcBoundaryCurve( int id ) { m_id = id; }
IfcBoundaryCurve::~IfcBoundaryCurve() {}
shared_ptr<IfcPPObject> IfcBoundaryCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBoundaryCurve> copy_self( new IfcBoundaryCurve() );
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
void IfcBoundaryCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBOUNDARYCURVE" << "(";
	writeEntityList( stream, m_Segments );
	stream << ",";
	if( m_SelfIntersect == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_SelfIntersect == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ");";
}
void IfcBoundaryCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundaryCurve::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBoundaryCurve, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_Segments, map );
	if( boost::iequals( args[1], L".F." ) ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( boost::iequals( args[1], L".T." ) ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( boost::iequals( args[1], L".U." ) ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
}
void IfcBoundaryCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCompositeCurveOnSurface::getAttributes( vec_attributes );
}
void IfcBoundaryCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCompositeCurveOnSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcBoundaryCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCompositeCurveOnSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundaryCurve::unlinkSelf()
{
	IfcCompositeCurveOnSurface::unlinkSelf();
}

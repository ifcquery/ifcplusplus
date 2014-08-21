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
#include "include/IfcCurve.h"
#include "include/IfcCurveBoundedPlane.h"
#include "include/IfcPlane.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcCurveBoundedPlane 
IfcCurveBoundedPlane::IfcCurveBoundedPlane() {}
IfcCurveBoundedPlane::IfcCurveBoundedPlane( int id ) { m_id = id; }
IfcCurveBoundedPlane::~IfcCurveBoundedPlane() {}
shared_ptr<IfcPPObject> IfcCurveBoundedPlane::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCurveBoundedPlane> copy_self( new IfcCurveBoundedPlane() );
	if( m_BasisSurface ) { copy_self->m_BasisSurface = dynamic_pointer_cast<IfcPlane>( m_BasisSurface->getDeepCopy(options) ); }
	if( m_OuterBoundary ) { copy_self->m_OuterBoundary = dynamic_pointer_cast<IfcCurve>( m_OuterBoundary->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_InnerBoundaries.size(); ++ii )
	{
		auto item_ii = m_InnerBoundaries[ii];
		if( item_ii )
		{
			copy_self->m_InnerBoundaries.push_back( dynamic_pointer_cast<IfcCurve>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcCurveBoundedPlane::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCURVEBOUNDEDPLANE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_OuterBoundary ) { stream << "#" << m_OuterBoundary->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_InnerBoundaries );
	stream << ");";
}
void IfcCurveBoundedPlane::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurveBoundedPlane::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCurveBoundedPlane, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_BasisSurface, map );
	readEntityReference( args[1], m_OuterBoundary, map );
	readEntityReferenceList( args[2], m_InnerBoundaries, map );
}
void IfcCurveBoundedPlane::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	vec_attributes.push_back( std::make_pair( "OuterBoundary", m_OuterBoundary ) );
	if( m_InnerBoundaries.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> InnerBoundaries_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_InnerBoundaries.begin(), m_InnerBoundaries.end(), std::back_inserter( InnerBoundaries_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "InnerBoundaries", InnerBoundaries_vec_object ) );
	}
}
void IfcCurveBoundedPlane::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcCurveBoundedPlane::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveBoundedPlane::unlinkSelf()
{
	IfcBoundedSurface::unlinkSelf();
}

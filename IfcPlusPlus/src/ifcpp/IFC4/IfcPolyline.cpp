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
#include "include/IfcCartesianPoint.h"
#include "include/IfcPolyline.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcPolyline 
IfcPolyline::IfcPolyline() {}
IfcPolyline::IfcPolyline( int id ) { m_id = id; }
IfcPolyline::~IfcPolyline() {}
shared_ptr<IfcPPObject> IfcPolyline::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPolyline> copy_self( new IfcPolyline() );
	for( size_t ii=0; ii<m_Points.size(); ++ii )
	{
		auto item_ii = m_Points[ii];
		if( item_ii )
		{
			copy_self->m_Points.push_back( dynamic_pointer_cast<IfcCartesianPoint>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcPolyline::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPOLYLINE" << "(";
	writeEntityList( stream, m_Points );
	stream << ");";
}
void IfcPolyline::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPolyline::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPolyline, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_Points, map );
}
void IfcPolyline::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedCurve::getAttributes( vec_attributes );
	if( m_Points.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Points_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Points.begin(), m_Points.end(), std::back_inserter( Points_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Points", Points_vec_object ) );
	}
}
void IfcPolyline::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcPolyline::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcPolyline::unlinkSelf()
{
	IfcBoundedCurve::unlinkSelf();
}

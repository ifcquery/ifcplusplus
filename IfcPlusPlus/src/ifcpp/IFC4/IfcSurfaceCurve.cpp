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
#include "include/IfcPcurve.h"
#include "include/IfcPreferredSurfaceCurveRepresentation.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurfaceCurve.h"

// ENTITY IfcSurfaceCurve 
IfcSurfaceCurve::IfcSurfaceCurve() { m_entity_enum = IFCSURFACECURVE; }
IfcSurfaceCurve::IfcSurfaceCurve( int id ) { m_id = id; m_entity_enum = IFCSURFACECURVE; }
IfcSurfaceCurve::~IfcSurfaceCurve() {}
shared_ptr<IfcPPObject> IfcSurfaceCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceCurve> copy_self( new IfcSurfaceCurve() );
	if( m_Curve3D ) { copy_self->m_Curve3D = dynamic_pointer_cast<IfcCurve>( m_Curve3D->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_AssociatedGeometry.size(); ++ii )
	{
		auto item_ii = m_AssociatedGeometry[ii];
		if( item_ii )
		{
			copy_self->m_AssociatedGeometry.push_back( dynamic_pointer_cast<IfcPcurve>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_MasterRepresentation ) { copy_self->m_MasterRepresentation = dynamic_pointer_cast<IfcPreferredSurfaceCurveRepresentation>( m_MasterRepresentation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSurfaceCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSURFACECURVE" << "(";
	if( m_Curve3D ) { stream << "#" << m_Curve3D->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_AssociatedGeometry );
	stream << ",";
	if( m_MasterRepresentation ) { m_MasterRepresentation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceCurve::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSurfaceCurve, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Curve3D, map );
	readEntityReferenceList( args[1], m_AssociatedGeometry, map );
	m_MasterRepresentation = IfcPreferredSurfaceCurveRepresentation::createObjectFromSTEP( args[2] );
}
void IfcSurfaceCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Curve3D", m_Curve3D ) );
	if( m_AssociatedGeometry.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> AssociatedGeometry_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_AssociatedGeometry.begin(), m_AssociatedGeometry.end(), std::back_inserter( AssociatedGeometry_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "AssociatedGeometry", AssociatedGeometry_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "MasterRepresentation", m_MasterRepresentation ) );
}
void IfcSurfaceCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceCurve::unlinkFromInverseCounterparts()
{
	IfcCurve::unlinkFromInverseCounterparts();
}

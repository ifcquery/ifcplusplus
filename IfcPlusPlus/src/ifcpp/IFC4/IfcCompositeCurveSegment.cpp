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
#include "include/IfcCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTransitionCode.h"

// ENTITY IfcCompositeCurveSegment 
IfcCompositeCurveSegment::IfcCompositeCurveSegment() {}
IfcCompositeCurveSegment::IfcCompositeCurveSegment( int id ) { m_id = id; }
IfcCompositeCurveSegment::~IfcCompositeCurveSegment() {}
shared_ptr<IfcPPObject> IfcCompositeCurveSegment::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCompositeCurveSegment> copy_self( new IfcCompositeCurveSegment() );
	if( m_Transition ) { copy_self->m_Transition = dynamic_pointer_cast<IfcTransitionCode>( m_Transition->getDeepCopy(options) ); }
	if( m_SameSense ) { copy_self->m_SameSense = m_SameSense; }
	if( m_ParentCurve ) { copy_self->m_ParentCurve = dynamic_pointer_cast<IfcCurve>( m_ParentCurve->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCompositeCurveSegment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOMPOSITECURVESEGMENT" << "(";
	if( m_Transition ) { m_Transition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SameSense == false ) { stream << ".F."; }
	else if( m_SameSense == true ) { stream << ".T."; }
	stream << ",";
	if( m_ParentCurve ) { stream << "#" << m_ParentCurve->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcCompositeCurveSegment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCompositeCurveSegment::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCompositeCurveSegment, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Transition = IfcTransitionCode::createObjectFromSTEP( args[0] );
	if( boost::iequals( args[1], L".F." ) ) { m_SameSense = false; }
	else if( boost::iequals( args[1], L".T." ) ) { m_SameSense = true; }
	readEntityReference( args[2], m_ParentCurve, map );
}
void IfcCompositeCurveSegment::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Transition", m_Transition ) );
	vec_attributes.push_back( std::make_pair( "SameSense", shared_ptr<IfcPPBoolAttribute>( new IfcPPBoolAttribute( m_SameSense ) ) ) );
	vec_attributes.push_back( std::make_pair( "ParentCurve", m_ParentCurve ) );
}
void IfcCompositeCurveSegment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
	if( m_UsingCurves_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> UsingCurves_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_UsingCurves_inverse.size(); ++i )
		{
			if( !m_UsingCurves_inverse[i].expired() )
			{
				UsingCurves_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcCompositeCurve>( m_UsingCurves_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "UsingCurves_inverse", UsingCurves_inverse_vec_obj ) );
	}
}
void IfcCompositeCurveSegment::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcCompositeCurveSegment::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}

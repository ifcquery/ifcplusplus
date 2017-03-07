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
#include "include/IfcBSplineSurfaceForm.h"
#include "include/IfcBSplineSurfaceWithKnots.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcInteger.h"
#include "include/IfcKnotType.h"
#include "include/IfcLogical.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBSplineSurfaceWithKnots 
IfcBSplineSurfaceWithKnots::IfcBSplineSurfaceWithKnots() { m_entity_enum = IFCBSPLINESURFACEWITHKNOTS; }
IfcBSplineSurfaceWithKnots::IfcBSplineSurfaceWithKnots( int id ) { m_id = id; m_entity_enum = IFCBSPLINESURFACEWITHKNOTS; }
IfcBSplineSurfaceWithKnots::~IfcBSplineSurfaceWithKnots() {}
shared_ptr<IfcPPObject> IfcBSplineSurfaceWithKnots::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBSplineSurfaceWithKnots> copy_self( new IfcBSplineSurfaceWithKnots() );
	if( m_UDegree ) { copy_self->m_UDegree = dynamic_pointer_cast<IfcInteger>( m_UDegree->getDeepCopy(options) ); }
	if( m_VDegree ) { copy_self->m_VDegree = dynamic_pointer_cast<IfcInteger>( m_VDegree->getDeepCopy(options) ); }
	copy_self->m_ControlPointsList.resize( m_ControlPointsList.size() );
	for( size_t ii=0; ii<m_ControlPointsList.size(); ++ii )
	{
		std::vector<shared_ptr<IfcCartesianPoint> >& vec_ii = m_ControlPointsList[ii];
		std::vector<shared_ptr<IfcCartesianPoint> >& vec_ii_target = copy_self->m_ControlPointsList[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcCartesianPoint>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcCartesianPoint>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	if( m_SurfaceForm ) { copy_self->m_SurfaceForm = dynamic_pointer_cast<IfcBSplineSurfaceForm>( m_SurfaceForm->getDeepCopy(options) ); }
	if( m_UClosed ) { copy_self->m_UClosed = dynamic_pointer_cast<IfcLogical>( m_UClosed->getDeepCopy(options) ); }
	if( m_VClosed ) { copy_self->m_VClosed = dynamic_pointer_cast<IfcLogical>( m_VClosed->getDeepCopy(options) ); }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_UMultiplicities.size(); ++ii )
	{
		auto item_ii = m_UMultiplicities[ii];
		if( item_ii )
		{
			copy_self->m_UMultiplicities.push_back( dynamic_pointer_cast<IfcInteger>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_VMultiplicities.size(); ++ii )
	{
		auto item_ii = m_VMultiplicities[ii];
		if( item_ii )
		{
			copy_self->m_VMultiplicities.push_back( dynamic_pointer_cast<IfcInteger>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_UKnots.size(); ++ii )
	{
		auto item_ii = m_UKnots[ii];
		if( item_ii )
		{
			copy_self->m_UKnots.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_VKnots.size(); ++ii )
	{
		auto item_ii = m_VKnots[ii];
		if( item_ii )
		{
			copy_self->m_VKnots.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_KnotSpec ) { copy_self->m_KnotSpec = dynamic_pointer_cast<IfcKnotType>( m_KnotSpec->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBSplineSurfaceWithKnots::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBSPLINESURFACEWITHKNOTS" << "(";
	if( m_UDegree ) { m_UDegree->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_VDegree ) { m_VDegree->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList2D( stream, m_ControlPointsList );
	stream << ",";
	if( m_SurfaceForm ) { m_SurfaceForm->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UClosed ) { m_UClosed->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_VClosed ) { m_VClosed->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeNumericTypeList( stream, m_UMultiplicities );
	stream << ",";
	writeNumericTypeList( stream, m_VMultiplicities );
	stream << ",";
	writeNumericTypeList( stream, m_UKnots );
	stream << ",";
	writeNumericTypeList( stream, m_VKnots );
	stream << ",";
	if( m_KnotSpec ) { m_KnotSpec->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcBSplineSurfaceWithKnots::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBSplineSurfaceWithKnots::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 12 ){ std::stringstream err; err << "Wrong parameter count for entity IfcBSplineSurfaceWithKnots, expecting 12, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_UDegree = IfcInteger::createObjectFromSTEP( args[0] );
	m_VDegree = IfcInteger::createObjectFromSTEP( args[1] );
	readEntityReferenceList2D( args[2], m_ControlPointsList, map );
	m_SurfaceForm = IfcBSplineSurfaceForm::createObjectFromSTEP( args[3] );
	m_UClosed = IfcLogical::createObjectFromSTEP( args[4] );
	m_VClosed = IfcLogical::createObjectFromSTEP( args[5] );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[6] );
	readTypeOfIntList( args[7], m_UMultiplicities );
	readTypeOfIntList( args[8], m_VMultiplicities );
	readTypeOfRealList( args[9], m_UKnots );
	readTypeOfRealList( args[10], m_VKnots );
	m_KnotSpec = IfcKnotType::createObjectFromSTEP( args[11] );
}
void IfcBSplineSurfaceWithKnots::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBSplineSurface::getAttributes( vec_attributes );
	if( m_UMultiplicities.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> UMultiplicities_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_UMultiplicities.begin(), m_UMultiplicities.end(), std::back_inserter( UMultiplicities_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "UMultiplicities", UMultiplicities_vec_object ) );
	}
	if( m_VMultiplicities.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> VMultiplicities_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_VMultiplicities.begin(), m_VMultiplicities.end(), std::back_inserter( VMultiplicities_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "VMultiplicities", VMultiplicities_vec_object ) );
	}
	if( m_UKnots.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> UKnots_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_UKnots.begin(), m_UKnots.end(), std::back_inserter( UKnots_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "UKnots", UKnots_vec_object ) );
	}
	if( m_VKnots.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> VKnots_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_VKnots.begin(), m_VKnots.end(), std::back_inserter( VKnots_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "VKnots", VKnots_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "KnotSpec", m_KnotSpec ) );
}
void IfcBSplineSurfaceWithKnots::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBSplineSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcBSplineSurfaceWithKnots::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBSplineSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineSurfaceWithKnots::unlinkFromInverseCounterparts()
{
	IfcBSplineSurface::unlinkFromInverseCounterparts();
}

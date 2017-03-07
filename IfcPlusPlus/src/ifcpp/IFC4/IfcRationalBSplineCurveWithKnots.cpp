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
#include "include/IfcBSplineCurveForm.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcInteger.h"
#include "include/IfcKnotType.h"
#include "include/IfcLogical.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRationalBSplineCurveWithKnots.h"
#include "include/IfcReal.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcRationalBSplineCurveWithKnots 
IfcRationalBSplineCurveWithKnots::IfcRationalBSplineCurveWithKnots() { m_entity_enum = IFCRATIONALBSPLINECURVEWITHKNOTS; }
IfcRationalBSplineCurveWithKnots::IfcRationalBSplineCurveWithKnots( int id ) { m_id = id; m_entity_enum = IFCRATIONALBSPLINECURVEWITHKNOTS; }
IfcRationalBSplineCurveWithKnots::~IfcRationalBSplineCurveWithKnots() {}
shared_ptr<IfcPPObject> IfcRationalBSplineCurveWithKnots::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRationalBSplineCurveWithKnots> copy_self( new IfcRationalBSplineCurveWithKnots() );
	if( m_Degree ) { copy_self->m_Degree = dynamic_pointer_cast<IfcInteger>( m_Degree->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ControlPointsList.size(); ++ii )
	{
		auto item_ii = m_ControlPointsList[ii];
		if( item_ii )
		{
			copy_self->m_ControlPointsList.push_back( dynamic_pointer_cast<IfcCartesianPoint>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_CurveForm ) { copy_self->m_CurveForm = dynamic_pointer_cast<IfcBSplineCurveForm>( m_CurveForm->getDeepCopy(options) ); }
	if( m_ClosedCurve ) { copy_self->m_ClosedCurve = dynamic_pointer_cast<IfcLogical>( m_ClosedCurve->getDeepCopy(options) ); }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_KnotMultiplicities.size(); ++ii )
	{
		auto item_ii = m_KnotMultiplicities[ii];
		if( item_ii )
		{
			copy_self->m_KnotMultiplicities.push_back( dynamic_pointer_cast<IfcInteger>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_Knots.size(); ++ii )
	{
		auto item_ii = m_Knots[ii];
		if( item_ii )
		{
			copy_self->m_Knots.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_KnotSpec ) { copy_self->m_KnotSpec = dynamic_pointer_cast<IfcKnotType>( m_KnotSpec->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_WeightsData.size(); ++ii )
	{
		auto item_ii = m_WeightsData[ii];
		if( item_ii )
		{
			copy_self->m_WeightsData.push_back( dynamic_pointer_cast<IfcReal>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcRationalBSplineCurveWithKnots::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRATIONALBSPLINECURVEWITHKNOTS" << "(";
	if( m_Degree ) { m_Degree->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_ControlPointsList );
	stream << ",";
	if( m_CurveForm ) { m_CurveForm->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ClosedCurve ) { m_ClosedCurve->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeNumericTypeList( stream, m_KnotMultiplicities );
	stream << ",";
	writeNumericTypeList( stream, m_Knots );
	stream << ",";
	if( m_KnotSpec ) { m_KnotSpec->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeNumericTypeList( stream, m_WeightsData );
	stream << ");";
}
void IfcRationalBSplineCurveWithKnots::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRationalBSplineCurveWithKnots::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRationalBSplineCurveWithKnots, expecting 9, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Degree = IfcInteger::createObjectFromSTEP( args[0] );
	readEntityReferenceList( args[1], m_ControlPointsList, map );
	m_CurveForm = IfcBSplineCurveForm::createObjectFromSTEP( args[2] );
	m_ClosedCurve = IfcLogical::createObjectFromSTEP( args[3] );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[4] );
	readTypeOfIntList( args[5], m_KnotMultiplicities );
	readTypeOfRealList( args[6], m_Knots );
	m_KnotSpec = IfcKnotType::createObjectFromSTEP( args[7] );
	readTypeOfRealList( args[8], m_WeightsData );
}
void IfcRationalBSplineCurveWithKnots::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBSplineCurveWithKnots::getAttributes( vec_attributes );
	if( m_WeightsData.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> WeightsData_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_WeightsData.begin(), m_WeightsData.end(), std::back_inserter( WeightsData_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "WeightsData", WeightsData_vec_object ) );
	}
}
void IfcRationalBSplineCurveWithKnots::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBSplineCurveWithKnots::getAttributesInverse( vec_attributes_inverse );
}
void IfcRationalBSplineCurveWithKnots::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBSplineCurveWithKnots::setInverseCounterparts( ptr_self_entity );
}
void IfcRationalBSplineCurveWithKnots::unlinkFromInverseCounterparts()
{
	IfcBSplineCurveWithKnots::unlinkFromInverseCounterparts();
}

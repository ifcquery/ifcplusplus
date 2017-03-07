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
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPGlobal.h"
#include "IfcBSplineCurveWithKnots.h"
class IFCPP_EXPORT IfcReal;
//ENTITY
class IFCPP_EXPORT IfcRationalBSplineCurveWithKnots : public IfcBSplineCurveWithKnots
{ 
public:
	IfcRationalBSplineCurveWithKnots();
	IfcRationalBSplineCurveWithKnots( int id );
	~IfcRationalBSplineCurveWithKnots();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcRationalBSplineCurveWithKnots"; }


	// IfcRepresentationItem -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcPresentationLayerAssignment> >	m_LayerAssignment_inverse;
	//  std::vector<weak_ptr<IfcStyledItem> >					m_StyledByItem_inverse;

	// IfcGeometricRepresentationItem -----------------------------------------------------------

	// IfcCurve -----------------------------------------------------------

	// IfcBoundedCurve -----------------------------------------------------------

	// IfcBSplineCurve -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcInteger>									m_Degree;
	//  std::vector<shared_ptr<IfcCartesianPoint> >				m_ControlPointsList;
	//  shared_ptr<IfcBSplineCurveForm>							m_CurveForm;
	//  shared_ptr<IfcLogical>									m_ClosedCurve;
	//  shared_ptr<IfcLogical>									m_SelfIntersect;

	// IfcBSplineCurveWithKnots -----------------------------------------------------------
	// attributes:
	//  std::vector<shared_ptr<IfcInteger> >					m_KnotMultiplicities;
	//  std::vector<shared_ptr<IfcParameterValue> >				m_Knots;
	//  shared_ptr<IfcKnotType>									m_KnotSpec;

	// IfcRationalBSplineCurveWithKnots -----------------------------------------------------------
	// attributes:
	std::vector<shared_ptr<IfcReal> >						m_WeightsData;
};


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
#include "IfcGeometricRepresentationContext.h"
class IFCPP_EXPORT IfcGeometricRepresentationContext;
class IFCPP_EXPORT IfcPositiveRatioMeasure;
class IFCPP_EXPORT IfcGeometricProjectionEnum;
class IFCPP_EXPORT IfcLabel;
//ENTITY
class IFCPP_EXPORT IfcGeometricRepresentationSubContext : public IfcGeometricRepresentationContext
{ 
public:
	IfcGeometricRepresentationSubContext();
	IfcGeometricRepresentationSubContext( int id );
	~IfcGeometricRepresentationSubContext();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcGeometricRepresentationSubContext"; }


	// IfcRepresentationContext -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>											m_ContextIdentifier;		//optional
	//  shared_ptr<IfcLabel>											m_ContextType;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRepresentation> >						m_RepresentationsInContext_inverse;

	// IfcGeometricRepresentationContext -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcDimensionCount>									m_CoordinateSpaceDimension;
	//  shared_ptr<IfcReal>												m_Precision;				//optional
	//  shared_ptr<IfcAxis2Placement>									m_WorldCoordinateSystem;
	//  shared_ptr<IfcDirection>										m_TrueNorth;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcGeometricRepresentationSubContext> >	m_HasSubContexts_inverse;
	//  std::vector<weak_ptr<IfcCoordinateOperation> >					m_HasCoordinateOperation_inverse;

	// IfcGeometricRepresentationSubContext -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcGeometricRepresentationContext>					m_ParentContext;
	shared_ptr<IfcPositiveRatioMeasure>								m_TargetScale;				//optional
	shared_ptr<IfcGeometricProjectionEnum>							m_TargetView;
	shared_ptr<IfcLabel>											m_UserDefinedTargetView;	//optional
};


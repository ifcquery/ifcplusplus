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
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"
#include "IfcGeometricRepresentationContext.h"
class IfcGeometricRepresentationContext;
class IfcPositiveRatioMeasure;
class IfcGeometricProjectionEnum;
class IfcLabel;
//ENTITY
class IfcGeometricRepresentationSubContext : public IfcGeometricRepresentationContext
{
public:
	IfcGeometricRepresentationSubContext();
	IfcGeometricRepresentationSubContext( int id );
	~IfcGeometricRepresentationSubContext();

	// method setEntity takes over all attributes from another instance of the class
	virtual void setEntity( shared_ptr<IfcPPEntity> other );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkSelf();
	virtual const char* classname() const { return "IfcGeometricRepresentationSubContext"; }


	// IfcRepresentationContext -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>								m_ContextIdentifier;		//optional
	//  shared_ptr<IfcLabel>								m_ContextType;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRepresentation> >		m_RepresentationsInContext_inverse;

	// IfcGeometricRepresentationContext -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcDimensionCount>					m_CoordinateSpaceDimension;
	//  double											m_Precision;				//optional
	//  shared_ptr<IfcAxis2Placement>					m_WorldCoordinateSystem;
	//  shared_ptr<IfcDirection>							m_TrueNorth;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcGeometricRepresentationSubContext> >	m_HasSubContexts_inverse;

	// IfcGeometricRepresentationSubContext -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcGeometricRepresentationContext>	m_ParentContext;
	shared_ptr<IfcPositiveRatioMeasure>				m_TargetScale;				//optional
	shared_ptr<IfcGeometricProjectionEnum>			m_TargetView;
	shared_ptr<IfcLabel>								m_UserDefinedTargetView;	//optional
};


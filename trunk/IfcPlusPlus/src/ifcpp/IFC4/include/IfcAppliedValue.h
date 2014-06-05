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
#include "IfcMetricValueSelect.h"
#include "IfcObjectReferenceSelect.h"
#include "IfcResourceObjectSelect.h"
class IfcLabel;
class IfcText;
class IfcAppliedValueSelect;
class IfcMeasureWithUnit;
class IfcDate;
class IfcArithmeticOperatorEnum;
class IfcAppliedValue;
class IfcExternalReferenceRelationship;
//ENTITY
class IfcAppliedValue : public IfcMetricValueSelect, public IfcObjectReferenceSelect, public IfcResourceObjectSelect, public IfcPPEntity
{
public:
	IfcAppliedValue();
	IfcAppliedValue( int id );
	~IfcAppliedValue();

	// method setEntity takes over all attributes from another instance of the class
	virtual void setEntity( shared_ptr<IfcPPEntity> other );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkSelf();
	virtual const char* classname() const { return "IfcAppliedValue"; }


	// IfcAppliedValue -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLabel>										m_Name;						//optional
	shared_ptr<IfcText>										m_Description;				//optional
	shared_ptr<IfcAppliedValueSelect>						m_AppliedValue;				//optional
	shared_ptr<IfcMeasureWithUnit>							m_UnitBasis;				//optional
	shared_ptr<IfcDate>										m_ApplicableDate;			//optional
	shared_ptr<IfcDate>										m_FixedUntilDate;			//optional
	shared_ptr<IfcLabel>										m_Category;					//optional
	shared_ptr<IfcLabel>										m_Condition;				//optional
	shared_ptr<IfcArithmeticOperatorEnum>					m_ArithmeticOperator;		//optional
	std::vector<shared_ptr<IfcAppliedValue> >				m_Components;				//optional
	// inverse attributes:
	std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReference_inverse;
};


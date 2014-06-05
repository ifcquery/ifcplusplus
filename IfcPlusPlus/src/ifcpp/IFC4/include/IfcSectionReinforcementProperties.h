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
#include "IfcPreDefinedProperties.h"
class IfcLengthMeasure;
class IfcReinforcingBarRoleEnum;
class IfcSectionProperties;
class IfcReinforcementBarProperties;
//ENTITY
class IfcSectionReinforcementProperties : public IfcPreDefinedProperties
{
public:
	IfcSectionReinforcementProperties();
	IfcSectionReinforcementProperties( int id );
	~IfcSectionReinforcementProperties();

	// method setEntity takes over all attributes from another instance of the class
	virtual void setEntity( shared_ptr<IfcPPEntity> other );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkSelf();
	virtual const char* classname() const { return "IfcSectionReinforcementProperties"; }


	// IfcPropertyAbstraction -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReferences_inverse;

	// IfcPreDefinedProperties -----------------------------------------------------------

	// IfcSectionReinforcementProperties -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLengthMeasure>								m_LongitudinalStartPosition;
	shared_ptr<IfcLengthMeasure>								m_LongitudinalEndPosition;
	shared_ptr<IfcLengthMeasure>								m_TransversePosition;		//optional
	shared_ptr<IfcReinforcingBarRoleEnum>					m_ReinforcementRole;
	shared_ptr<IfcSectionProperties>							m_SectionDefinition;
	std::vector<shared_ptr<IfcReinforcementBarProperties> >	m_CrossSectionReinforcementDefinitions;
};


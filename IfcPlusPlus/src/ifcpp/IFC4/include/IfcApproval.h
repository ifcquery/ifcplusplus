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
#include "IfcResourceObjectSelect.h"
class IfcIdentifier;
class IfcLabel;
class IfcText;
class IfcDateTime;
class IfcActorSelect;
class IfcExternalReferenceRelationship;
class IfcRelAssociatesApproval;
class IfcResourceApprovalRelationship;
class IfcApprovalRelationship;
//ENTITY
class IfcApproval : public IfcResourceObjectSelect, public IfcPPEntity
{
public:
	IfcApproval();
	IfcApproval( int id );
	~IfcApproval();

	// method setEntity takes over all attributes from another instance of the class
	virtual void setEntity( shared_ptr<IfcPPEntity> other );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkSelf();
	virtual const char* classname() const { return "IfcApproval"; }


	// IfcApproval -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcIdentifier>								m_Identifier;				//optional
	shared_ptr<IfcLabel>										m_Name;						//optional
	shared_ptr<IfcText>										m_Description;				//optional
	shared_ptr<IfcDateTime>									m_TimeOfApproval;			//optional
	shared_ptr<IfcLabel>										m_Status;					//optional
	shared_ptr<IfcLabel>										m_Level;					//optional
	shared_ptr<IfcText>										m_Qualifier;				//optional
	shared_ptr<IfcActorSelect>								m_RequestingApproval;		//optional
	shared_ptr<IfcActorSelect>								m_GivingApproval;			//optional
	// inverse attributes:
	std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReferences_inverse;
	std::vector<weak_ptr<IfcRelAssociatesApproval> >			m_ApprovedObjects_inverse;
	std::vector<weak_ptr<IfcResourceApprovalRelationship> >	m_ApprovedResources_inverse;
	std::vector<weak_ptr<IfcApprovalRelationship> >			m_IsRelatedWith_inverse;
	std::vector<weak_ptr<IfcApprovalRelationship> >			m_Relates_inverse;
};


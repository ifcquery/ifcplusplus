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
#include "ifcpp/model/IfcPPGlobal.h"
#include "IfcPresentationLayerAssignment.h"
class IFCPP_EXPORT IfcLogical;
class IFCPP_EXPORT IfcPresentationStyle;
//ENTITY
class IFCPP_EXPORT IfcPresentationLayerWithStyle : public IfcPresentationLayerAssignment
{ 
public:
	IfcPresentationLayerWithStyle();
	IfcPresentationLayerWithStyle( int id );
	~IfcPresentationLayerWithStyle();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcPresentationLayerWithStyle"; }


	// IfcPresentationLayerAssignment -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>							m_Name;
	//  shared_ptr<IfcText>								m_Description;				//optional
	//  std::vector<shared_ptr<IfcLayeredItem> >		m_AssignedItems;
	//  shared_ptr<IfcIdentifier>						m_Identifier;				//optional

	// IfcPresentationLayerWithStyle -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLogical>							m_LayerOn;
	shared_ptr<IfcLogical>							m_LayerFrozen;
	shared_ptr<IfcLogical>							m_LayerBlocked;
	std::vector<shared_ptr<IfcPresentationStyle> >	m_LayerStyles;
};


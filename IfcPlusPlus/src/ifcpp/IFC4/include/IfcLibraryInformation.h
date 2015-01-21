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
#include "IfcLibrarySelect.h"
#include "IfcExternalInformation.h"
class IFCPP_EXPORT IfcLabel;
class IFCPP_EXPORT IfcActorSelect;
class IFCPP_EXPORT IfcDateTime;
class IFCPP_EXPORT IfcURIReference;
class IFCPP_EXPORT IfcText;
class IFCPP_EXPORT IfcRelAssociatesLibrary;
class IFCPP_EXPORT IfcLibraryReference;
//ENTITY
class IFCPP_EXPORT IfcLibraryInformation : virtual public IfcLibrarySelect, public IfcExternalInformation
{ 
public:
	IfcLibraryInformation();
	IfcLibraryInformation( int id );
	~IfcLibraryInformation();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcLibraryInformation"; }


	// IfcExternalInformation -----------------------------------------------------------

	// IfcLibraryInformation -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLabel>							m_Name;
	shared_ptr<IfcLabel>							m_Version;					//optional
	shared_ptr<IfcActorSelect>						m_Publisher;				//optional
	shared_ptr<IfcDateTime>							m_VersionDate;				//optional
	shared_ptr<IfcURIReference>						m_Location;					//optional
	shared_ptr<IfcText>								m_Description;				//optional
	// inverse attributes:
	std::vector<weak_ptr<IfcRelAssociatesLibrary> >	m_LibraryInfoForObjects_inverse;
	std::vector<weak_ptr<IfcLibraryReference> >		m_HasLibraryReferences_inverse;
};


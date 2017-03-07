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
#include "IfcDocumentSelect.h"
#include "IfcExternalInformation.h"
class IFCPP_EXPORT IfcIdentifier;
class IFCPP_EXPORT IfcLabel;
class IFCPP_EXPORT IfcText;
class IFCPP_EXPORT IfcURIReference;
class IFCPP_EXPORT IfcActorSelect;
class IFCPP_EXPORT IfcDateTime;
class IFCPP_EXPORT IfcDate;
class IFCPP_EXPORT IfcDocumentConfidentialityEnum;
class IFCPP_EXPORT IfcDocumentStatusEnum;
class IFCPP_EXPORT IfcRelAssociatesDocument;
class IFCPP_EXPORT IfcDocumentReference;
class IFCPP_EXPORT IfcDocumentInformationRelationship;
//ENTITY
class IFCPP_EXPORT IfcDocumentInformation : virtual public IfcDocumentSelect, public IfcExternalInformation
{ 
public:
	IfcDocumentInformation();
	IfcDocumentInformation( int id );
	~IfcDocumentInformation();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcDocumentInformation"; }


	// IfcExternalInformation -----------------------------------------------------------

	// IfcDocumentInformation -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcIdentifier>									m_Identification;
	shared_ptr<IfcLabel>										m_Name;
	shared_ptr<IfcText>											m_Description;				//optional
	shared_ptr<IfcURIReference>									m_Location;					//optional
	shared_ptr<IfcText>											m_Purpose;					//optional
	shared_ptr<IfcText>											m_IntendedUse;				//optional
	shared_ptr<IfcText>											m_Scope;					//optional
	shared_ptr<IfcLabel>										m_Revision;					//optional
	shared_ptr<IfcActorSelect>									m_DocumentOwner;			//optional
	std::vector<shared_ptr<IfcActorSelect> >					m_Editors;					//optional
	shared_ptr<IfcDateTime>										m_CreationTime;				//optional
	shared_ptr<IfcDateTime>										m_LastRevisionTime;			//optional
	shared_ptr<IfcIdentifier>									m_ElectronicFormat;			//optional
	shared_ptr<IfcDate>											m_ValidFrom;				//optional
	shared_ptr<IfcDate>											m_ValidUntil;				//optional
	shared_ptr<IfcDocumentConfidentialityEnum>					m_Confidentiality;			//optional
	shared_ptr<IfcDocumentStatusEnum>							m_Status;					//optional
	// inverse attributes:
	std::vector<weak_ptr<IfcRelAssociatesDocument> >			m_DocumentInfoForObjects_inverse;
	std::vector<weak_ptr<IfcDocumentReference> >				m_HasDocumentReferences_inverse;
	std::vector<weak_ptr<IfcDocumentInformationRelationship> >	m_IsPointedTo_inverse;
	std::vector<weak_ptr<IfcDocumentInformationRelationship> >	m_IsPointer_inverse;
};


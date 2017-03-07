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
#include "IfcAddress.h"
class IFCPP_EXPORT IfcLabel;
class IFCPP_EXPORT IfcURIReference;
//ENTITY
class IFCPP_EXPORT IfcTelecomAddress : public IfcAddress
{ 
public:
	IfcTelecomAddress();
	IfcTelecomAddress( int id );
	~IfcTelecomAddress();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcTelecomAddress"; }


	// IfcAddress -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcAddressTypeEnum>				m_Purpose;					//optional
	//  shared_ptr<IfcText>							m_Description;				//optional
	//  shared_ptr<IfcLabel>						m_UserDefinedPurpose;		//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcPerson> >			m_OfPerson_inverse;
	//  std::vector<weak_ptr<IfcOrganization> >		m_OfOrganization_inverse;

	// IfcTelecomAddress -----------------------------------------------------------
	// attributes:
	std::vector<shared_ptr<IfcLabel> >			m_TelephoneNumbers;			//optional
	std::vector<shared_ptr<IfcLabel> >			m_FacsimileNumbers;			//optional
	shared_ptr<IfcLabel>						m_PagerNumber;				//optional
	std::vector<shared_ptr<IfcLabel> >			m_ElectronicMailAddresses;	//optional
	shared_ptr<IfcURIReference>					m_WWWHomePageURL;			//optional
	std::vector<shared_ptr<IfcURIReference> >	m_MessagingIDs;				//optional
};


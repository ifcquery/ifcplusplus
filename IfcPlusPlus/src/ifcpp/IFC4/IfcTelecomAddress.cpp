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
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcAddressTypeEnum.h"
#include "include/IfcLabel.h"
#include "include/IfcOrganization.h"
#include "include/IfcPerson.h"
#include "include/IfcTelecomAddress.h"
#include "include/IfcText.h"
#include "include/IfcURIReference.h"

// ENTITY IfcTelecomAddress 
IfcTelecomAddress::IfcTelecomAddress() { m_entity_enum = IFCTELECOMADDRESS; }
IfcTelecomAddress::IfcTelecomAddress( int id ) { m_id = id; m_entity_enum = IFCTELECOMADDRESS; }
IfcTelecomAddress::~IfcTelecomAddress() {}
shared_ptr<IfcPPObject> IfcTelecomAddress::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTelecomAddress> copy_self( new IfcTelecomAddress() );
	if( m_Purpose ) { copy_self->m_Purpose = dynamic_pointer_cast<IfcAddressTypeEnum>( m_Purpose->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_UserDefinedPurpose ) { copy_self->m_UserDefinedPurpose = dynamic_pointer_cast<IfcLabel>( m_UserDefinedPurpose->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_TelephoneNumbers.size(); ++ii )
	{
		auto item_ii = m_TelephoneNumbers[ii];
		if( item_ii )
		{
			copy_self->m_TelephoneNumbers.push_back( dynamic_pointer_cast<IfcLabel>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_FacsimileNumbers.size(); ++ii )
	{
		auto item_ii = m_FacsimileNumbers[ii];
		if( item_ii )
		{
			copy_self->m_FacsimileNumbers.push_back( dynamic_pointer_cast<IfcLabel>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_PagerNumber ) { copy_self->m_PagerNumber = dynamic_pointer_cast<IfcLabel>( m_PagerNumber->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ElectronicMailAddresses.size(); ++ii )
	{
		auto item_ii = m_ElectronicMailAddresses[ii];
		if( item_ii )
		{
			copy_self->m_ElectronicMailAddresses.push_back( dynamic_pointer_cast<IfcLabel>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_WWWHomePageURL ) { copy_self->m_WWWHomePageURL = dynamic_pointer_cast<IfcURIReference>( m_WWWHomePageURL->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_MessagingIDs.size(); ++ii )
	{
		auto item_ii = m_MessagingIDs[ii];
		if( item_ii )
		{
			copy_self->m_MessagingIDs.push_back( dynamic_pointer_cast<IfcURIReference>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTelecomAddress::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTELECOMADDRESS" << "(";
	if( m_Purpose ) { m_Purpose->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedPurpose ) { m_UserDefinedPurpose->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_TelephoneNumbers.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcLabel>& type_object = m_TelephoneNumbers[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_FacsimileNumbers.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcLabel>& type_object = m_FacsimileNumbers[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_PagerNumber ) { m_PagerNumber->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_ElectronicMailAddresses.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcLabel>& type_object = m_ElectronicMailAddresses[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_WWWHomePageURL ) { m_WWWHomePageURL->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_MessagingIDs.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcURIReference>& type_object = m_MessagingIDs[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ");";
}
void IfcTelecomAddress::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTelecomAddress::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTelecomAddress, expecting 9, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Purpose = IfcAddressTypeEnum::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_UserDefinedPurpose = IfcLabel::createObjectFromSTEP( args[2] );
	readSelectList( args[3], m_TelephoneNumbers, map );
	readSelectList( args[4], m_FacsimileNumbers, map );
	m_PagerNumber = IfcLabel::createObjectFromSTEP( args[5] );
	readSelectList( args[6], m_ElectronicMailAddresses, map );
	m_WWWHomePageURL = IfcURIReference::createObjectFromSTEP( args[7] );
	readSelectList( args[8], m_MessagingIDs, map );
}
void IfcTelecomAddress::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcAddress::getAttributes( vec_attributes );
	if( m_TelephoneNumbers.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> TelephoneNumbers_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_TelephoneNumbers.begin(), m_TelephoneNumbers.end(), std::back_inserter( TelephoneNumbers_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "TelephoneNumbers", TelephoneNumbers_vec_object ) );
	}
	if( m_FacsimileNumbers.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> FacsimileNumbers_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_FacsimileNumbers.begin(), m_FacsimileNumbers.end(), std::back_inserter( FacsimileNumbers_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "FacsimileNumbers", FacsimileNumbers_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "PagerNumber", m_PagerNumber ) );
	if( m_ElectronicMailAddresses.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ElectronicMailAddresses_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ElectronicMailAddresses.begin(), m_ElectronicMailAddresses.end(), std::back_inserter( ElectronicMailAddresses_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ElectronicMailAddresses", ElectronicMailAddresses_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "WWWHomePageURL", m_WWWHomePageURL ) );
	if( m_MessagingIDs.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> MessagingIDs_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_MessagingIDs.begin(), m_MessagingIDs.end(), std::back_inserter( MessagingIDs_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "MessagingIDs", MessagingIDs_vec_object ) );
	}
}
void IfcTelecomAddress::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcAddress::getAttributesInverse( vec_attributes_inverse );
}
void IfcTelecomAddress::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcAddress::setInverseCounterparts( ptr_self_entity );
}
void IfcTelecomAddress::unlinkFromInverseCounterparts()
{
	IfcAddress::unlinkFromInverseCounterparts();
}

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
#include "include/IfcPostalAddress.h"
#include "include/IfcText.h"

// ENTITY IfcPostalAddress 
IfcPostalAddress::IfcPostalAddress() { m_entity_enum = IFCPOSTALADDRESS; }
IfcPostalAddress::IfcPostalAddress( int id ) { m_id = id; m_entity_enum = IFCPOSTALADDRESS; }
IfcPostalAddress::~IfcPostalAddress() {}
shared_ptr<IfcPPObject> IfcPostalAddress::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPostalAddress> copy_self( new IfcPostalAddress() );
	if( m_Purpose ) { copy_self->m_Purpose = dynamic_pointer_cast<IfcAddressTypeEnum>( m_Purpose->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_UserDefinedPurpose ) { copy_self->m_UserDefinedPurpose = dynamic_pointer_cast<IfcLabel>( m_UserDefinedPurpose->getDeepCopy(options) ); }
	if( m_InternalLocation ) { copy_self->m_InternalLocation = dynamic_pointer_cast<IfcLabel>( m_InternalLocation->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_AddressLines.size(); ++ii )
	{
		auto item_ii = m_AddressLines[ii];
		if( item_ii )
		{
			copy_self->m_AddressLines.push_back( dynamic_pointer_cast<IfcLabel>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_PostalBox ) { copy_self->m_PostalBox = dynamic_pointer_cast<IfcLabel>( m_PostalBox->getDeepCopy(options) ); }
	if( m_Town ) { copy_self->m_Town = dynamic_pointer_cast<IfcLabel>( m_Town->getDeepCopy(options) ); }
	if( m_Region ) { copy_self->m_Region = dynamic_pointer_cast<IfcLabel>( m_Region->getDeepCopy(options) ); }
	if( m_PostalCode ) { copy_self->m_PostalCode = dynamic_pointer_cast<IfcLabel>( m_PostalCode->getDeepCopy(options) ); }
	if( m_Country ) { copy_self->m_Country = dynamic_pointer_cast<IfcLabel>( m_Country->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPostalAddress::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPOSTALADDRESS" << "(";
	if( m_Purpose ) { m_Purpose->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedPurpose ) { m_UserDefinedPurpose->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_InternalLocation ) { m_InternalLocation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_AddressLines.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcLabel>& type_object = m_AddressLines[ii];
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
	if( m_PostalBox ) { m_PostalBox->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Town ) { m_Town->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Region ) { m_Region->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PostalCode ) { m_PostalCode->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Country ) { m_Country->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPostalAddress::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPostalAddress::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 10 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPostalAddress, expecting 10, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Purpose = IfcAddressTypeEnum::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_UserDefinedPurpose = IfcLabel::createObjectFromSTEP( args[2] );
	m_InternalLocation = IfcLabel::createObjectFromSTEP( args[3] );
	readSelectList( args[4], m_AddressLines, map );
	m_PostalBox = IfcLabel::createObjectFromSTEP( args[5] );
	m_Town = IfcLabel::createObjectFromSTEP( args[6] );
	m_Region = IfcLabel::createObjectFromSTEP( args[7] );
	m_PostalCode = IfcLabel::createObjectFromSTEP( args[8] );
	m_Country = IfcLabel::createObjectFromSTEP( args[9] );
}
void IfcPostalAddress::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcAddress::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "InternalLocation", m_InternalLocation ) );
	if( m_AddressLines.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> AddressLines_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_AddressLines.begin(), m_AddressLines.end(), std::back_inserter( AddressLines_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "AddressLines", AddressLines_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "PostalBox", m_PostalBox ) );
	vec_attributes.push_back( std::make_pair( "Town", m_Town ) );
	vec_attributes.push_back( std::make_pair( "Region", m_Region ) );
	vec_attributes.push_back( std::make_pair( "PostalCode", m_PostalCode ) );
	vec_attributes.push_back( std::make_pair( "Country", m_Country ) );
}
void IfcPostalAddress::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcAddress::getAttributesInverse( vec_attributes_inverse );
}
void IfcPostalAddress::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcAddress::setInverseCounterparts( ptr_self_entity );
}
void IfcPostalAddress::unlinkFromInverseCounterparts()
{
	IfcAddress::unlinkFromInverseCounterparts();
}

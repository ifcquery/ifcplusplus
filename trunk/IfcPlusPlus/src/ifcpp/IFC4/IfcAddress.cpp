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
#include "include/IfcAddress.h"
#include "include/IfcAddressTypeEnum.h"
#include "include/IfcLabel.h"
#include "include/IfcOrganization.h"
#include "include/IfcPerson.h"
#include "include/IfcText.h"

// ENTITY IfcAddress 
IfcAddress::IfcAddress() {}
IfcAddress::IfcAddress( int id ) { m_id = id; }
IfcAddress::~IfcAddress() {}
shared_ptr<IfcPPObject> IfcAddress::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAddress> copy_self( new IfcAddress() );
	if( m_Purpose ) { copy_self->m_Purpose = dynamic_pointer_cast<IfcAddressTypeEnum>( m_Purpose->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_UserDefinedPurpose ) { copy_self->m_UserDefinedPurpose = dynamic_pointer_cast<IfcLabel>( m_UserDefinedPurpose->getDeepCopy(options) ); }
	return copy_self;
}
void IfcAddress::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCADDRESS" << "(";
	if( m_Purpose ) { m_Purpose->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedPurpose ) { m_UserDefinedPurpose->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcAddress::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAddress::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcAddress, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Purpose = IfcAddressTypeEnum::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_UserDefinedPurpose = IfcLabel::createObjectFromSTEP( args[2] );
}
void IfcAddress::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Purpose", m_Purpose ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedPurpose", m_UserDefinedPurpose ) );
}
void IfcAddress::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_OfPerson_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> OfPerson_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_OfPerson_inverse.size(); ++i )
		{
			if( !m_OfPerson_inverse[i].expired() )
			{
				OfPerson_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPerson>( m_OfPerson_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "OfPerson_inverse", OfPerson_inverse_vec_obj ) );
	}
	if( m_OfOrganization_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> OfOrganization_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_OfOrganization_inverse.size(); ++i )
		{
			if( !m_OfOrganization_inverse[i].expired() )
			{
				OfOrganization_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcOrganization>( m_OfOrganization_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "OfOrganization_inverse", OfOrganization_inverse_vec_obj ) );
	}
}
void IfcAddress::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcAddress::unlinkSelf()
{
}

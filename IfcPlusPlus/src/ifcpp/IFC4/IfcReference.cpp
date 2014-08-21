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
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcReference.h"

// ENTITY IfcReference 
IfcReference::IfcReference() {}
IfcReference::IfcReference( int id ) { m_id = id; }
IfcReference::~IfcReference() {}
shared_ptr<IfcPPObject> IfcReference::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReference> copy_self( new IfcReference() );
	if( m_TypeIdentifier ) { copy_self->m_TypeIdentifier = dynamic_pointer_cast<IfcIdentifier>( m_TypeIdentifier->getDeepCopy(options) ); }
	if( m_AttributeIdentifier ) { copy_self->m_AttributeIdentifier = dynamic_pointer_cast<IfcIdentifier>( m_AttributeIdentifier->getDeepCopy(options) ); }
	if( m_InstanceName ) { copy_self->m_InstanceName = dynamic_pointer_cast<IfcLabel>( m_InstanceName->getDeepCopy(options) ); }
	if( m_ListPositions.size() > 0 ) { std::copy( m_ListPositions.begin(), m_ListPositions.end(), std::back_inserter( copy_self->m_ListPositions ) ); }
	if( m_InnerReference ) { copy_self->m_InnerReference = dynamic_pointer_cast<IfcReference>( m_InnerReference->getDeepCopy(options) ); }
	return copy_self;
}
void IfcReference::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREFERENCE" << "(";
	if( m_TypeIdentifier ) { m_TypeIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_AttributeIdentifier ) { m_AttributeIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InstanceName ) { m_InstanceName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeIntList( stream, m_ListPositions );
	stream << ",";
	if( m_InnerReference ) { stream << "#" << m_InnerReference->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcReference::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcReference::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcReference, expecting 5, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_TypeIdentifier = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_AttributeIdentifier = IfcIdentifier::createObjectFromSTEP( args[1] );
	m_InstanceName = IfcLabel::createObjectFromSTEP( args[2] );
	readIntList(  args[3], m_ListPositions );
	readEntityReference( args[4], m_InnerReference, map );
}
void IfcReference::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "TypeIdentifier", m_TypeIdentifier ) );
	vec_attributes.push_back( std::make_pair( "AttributeIdentifier", m_AttributeIdentifier ) );
	vec_attributes.push_back( std::make_pair( "InstanceName", m_InstanceName ) );
	if( m_ListPositions.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ListPositions_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_ListPositions.size(); ++i )
		{
			ListPositions_vec_obj->m_vec.push_back( shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute(m_ListPositions[i] ) ) );
		}
		vec_attributes.push_back( std::make_pair( "ListPositions", ListPositions_vec_obj ) );
	}
	vec_attributes.push_back( std::make_pair( "InnerReference", m_InnerReference ) );
}
void IfcReference::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcReference::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcReference::unlinkSelf()
{
}

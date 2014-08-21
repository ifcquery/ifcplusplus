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
#include "include/IfcComplexProperty.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcProperty.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcText.h"

// ENTITY IfcComplexProperty 
IfcComplexProperty::IfcComplexProperty() {}
IfcComplexProperty::IfcComplexProperty( int id ) { m_id = id; }
IfcComplexProperty::~IfcComplexProperty() {}
shared_ptr<IfcPPObject> IfcComplexProperty::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcComplexProperty> copy_self( new IfcComplexProperty() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_UsageName ) { copy_self->m_UsageName = dynamic_pointer_cast<IfcIdentifier>( m_UsageName->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_HasProperties.size(); ++ii )
	{
		auto item_ii = m_HasProperties[ii];
		if( item_ii )
		{
			copy_self->m_HasProperties.push_back( dynamic_pointer_cast<IfcProperty>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcComplexProperty::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOMPLEXPROPERTY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UsageName ) { m_UsageName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasProperties );
	stream << ");";
}
void IfcComplexProperty::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcComplexProperty::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcComplexProperty, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_UsageName = IfcIdentifier::createObjectFromSTEP( args[2] );
	readEntityReferenceList( args[3], m_HasProperties, map );
}
void IfcComplexProperty::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProperty::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UsageName", m_UsageName ) );
	if( m_HasProperties.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasProperties_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_HasProperties.begin(), m_HasProperties.end(), std::back_inserter( HasProperties_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "HasProperties", HasProperties_vec_object ) );
	}
}
void IfcComplexProperty::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcProperty::getAttributesInverse( vec_attributes_inverse );
}
void IfcComplexProperty::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProperty::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcComplexProperty> ptr_self = dynamic_pointer_cast<IfcComplexProperty>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcComplexProperty::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_HasProperties.size(); ++i )
	{
		if( m_HasProperties[i] )
		{
			m_HasProperties[i]->m_PartOfComplex_inverse.push_back( ptr_self );
		}
	}
}
void IfcComplexProperty::unlinkSelf()
{
	IfcProperty::unlinkSelf();
	for( size_t i=0; i<m_HasProperties.size(); ++i )
	{
		if( m_HasProperties[i] )
		{
			std::vector<weak_ptr<IfcComplexProperty> >& PartOfComplex_inverse = m_HasProperties[i]->m_PartOfComplex_inverse;
			for( auto it_PartOfComplex_inverse = PartOfComplex_inverse.begin(); it_PartOfComplex_inverse != PartOfComplex_inverse.end(); ++it_PartOfComplex_inverse)
			{
				shared_ptr<IfcComplexProperty> self_candidate( *it_PartOfComplex_inverse );
				if( self_candidate.get() == this )
				{
					PartOfComplex_inverse.erase( it_PartOfComplex_inverse );
					break;
				}
			}
		}
	}
}

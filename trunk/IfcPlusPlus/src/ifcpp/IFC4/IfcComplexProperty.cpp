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

// method setEntity takes over all attributes from another instance of the class
void IfcComplexProperty::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcComplexProperty> other = dynamic_pointer_cast<IfcComplexProperty>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_UsageName = other->m_UsageName;
	m_HasProperties = other->m_HasProperties;
}
void IfcComplexProperty::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCOMPLEXPROPERTY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UsageName ) { m_UsageName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasProperties );
	stream << ");";
}
void IfcComplexProperty::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcComplexProperty::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcComplexProperty, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcComplexProperty, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_UsageName = IfcIdentifier::createObjectFromStepData( args[2] );
	readEntityReferenceList( args[3], m_HasProperties, map );
}
void IfcComplexProperty::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProperty::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UsageName", m_UsageName ) );
}
void IfcComplexProperty::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcComplexProperty::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProperty::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcComplexProperty> ptr_self = dynamic_pointer_cast<IfcComplexProperty>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcComplexProperty::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_HasProperties.size(); ++i )
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
	for( int i=0; i<m_HasProperties.size(); ++i )
	{
		if( m_HasProperties[i] )
		{
			std::vector<weak_ptr<IfcComplexProperty> >& PartOfComplex_inverse = m_HasProperties[i]->m_PartOfComplex_inverse;
			std::vector<weak_ptr<IfcComplexProperty> >::iterator it_PartOfComplex_inverse;
			for( it_PartOfComplex_inverse = PartOfComplex_inverse.begin(); it_PartOfComplex_inverse != PartOfComplex_inverse.end(); ++it_PartOfComplex_inverse)
			{
				shared_ptr<IfcComplexProperty> self_candidate( *it_PartOfComplex_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					PartOfComplex_inverse.erase( it_PartOfComplex_inverse );
					break;
				}
			}
		}
	}
}

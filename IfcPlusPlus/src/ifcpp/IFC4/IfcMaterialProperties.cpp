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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcMaterialDefinition.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcProperty.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialProperties 
IfcMaterialProperties::IfcMaterialProperties() {}
IfcMaterialProperties::IfcMaterialProperties( int id ) { m_id = id; }
IfcMaterialProperties::~IfcMaterialProperties() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialProperties::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialProperties> other = dynamic_pointer_cast<IfcMaterialProperties>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Properties = other->m_Properties;
	m_Material = other->m_Material;
}
void IfcMaterialProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALPROPERTIES" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Properties );
	stream << ",";
	if( m_Material ) { stream << "#" << m_Material->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialProperties::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialProperties, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcMaterialProperties, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReferenceList( args[2], m_Properties, map );
	readEntityReference( args[3], m_Material, map );
}
void IfcMaterialProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExtendedProperties::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Material", m_Material ) );
}
void IfcMaterialProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExtendedProperties::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMaterialProperties> ptr_self = dynamic_pointer_cast<IfcMaterialProperties>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcMaterialProperties::setInverseCounterparts: type mismatch" ); }
	if( m_Material )
	{
		m_Material->m_HasProperties_inverse.push_back( ptr_self );
	}
}
void IfcMaterialProperties::unlinkSelf()
{
	IfcExtendedProperties::unlinkSelf();
	if( m_Material )
	{
		std::vector<weak_ptr<IfcMaterialProperties> >& HasProperties_inverse = m_Material->m_HasProperties_inverse;
		std::vector<weak_ptr<IfcMaterialProperties> >::iterator it_HasProperties_inverse;
		for( it_HasProperties_inverse = HasProperties_inverse.begin(); it_HasProperties_inverse != HasProperties_inverse.end(); ++it_HasProperties_inverse)
		{
			shared_ptr<IfcMaterialProperties> self_candidate( *it_HasProperties_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasProperties_inverse.erase( it_HasProperties_inverse );
				break;
			}
		}
	}
}

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
#include "include/IfcLabel.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialDefinitionRepresentation.h"
#include "include/IfcRepresentation.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialDefinitionRepresentation 
IfcMaterialDefinitionRepresentation::IfcMaterialDefinitionRepresentation() {}
IfcMaterialDefinitionRepresentation::IfcMaterialDefinitionRepresentation( int id ) { m_id = id; }
IfcMaterialDefinitionRepresentation::~IfcMaterialDefinitionRepresentation() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialDefinitionRepresentation::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialDefinitionRepresentation> other = dynamic_pointer_cast<IfcMaterialDefinitionRepresentation>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Representations = other->m_Representations;
	m_RepresentedMaterial = other->m_RepresentedMaterial;
}
void IfcMaterialDefinitionRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALDEFINITIONREPRESENTATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Representations );
	stream << ",";
	if( m_RepresentedMaterial ) { stream << "#" << m_RepresentedMaterial->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialDefinitionRepresentation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialDefinitionRepresentation::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialDefinitionRepresentation, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcMaterialDefinitionRepresentation, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReferenceList( args[2], m_Representations, map );
	readEntityReference( args[3], m_RepresentedMaterial, map );
}
void IfcMaterialDefinitionRepresentation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProductRepresentation::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RepresentedMaterial", m_RepresentedMaterial ) );
}
void IfcMaterialDefinitionRepresentation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialDefinitionRepresentation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProductRepresentation::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMaterialDefinitionRepresentation> ptr_self = dynamic_pointer_cast<IfcMaterialDefinitionRepresentation>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcMaterialDefinitionRepresentation::setInverseCounterparts: type mismatch" ); }
	if( m_RepresentedMaterial )
	{
		m_RepresentedMaterial->m_HasRepresentation_inverse.push_back( ptr_self );
	}
}
void IfcMaterialDefinitionRepresentation::unlinkSelf()
{
	IfcProductRepresentation::unlinkSelf();
	if( m_RepresentedMaterial )
	{
		std::vector<weak_ptr<IfcMaterialDefinitionRepresentation> >& HasRepresentation_inverse = m_RepresentedMaterial->m_HasRepresentation_inverse;
		std::vector<weak_ptr<IfcMaterialDefinitionRepresentation> >::iterator it_HasRepresentation_inverse;
		for( it_HasRepresentation_inverse = HasRepresentation_inverse.begin(); it_HasRepresentation_inverse != HasRepresentation_inverse.end(); ++it_HasRepresentation_inverse)
		{
			shared_ptr<IfcMaterialDefinitionRepresentation> self_candidate( *it_HasRepresentation_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasRepresentation_inverse.erase( it_HasRepresentation_inverse );
				break;
			}
		}
	}
}

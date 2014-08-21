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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByTemplate.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"

// ENTITY IfcPropertySetDefinition 
IfcPropertySetDefinition::IfcPropertySetDefinition() {}
IfcPropertySetDefinition::IfcPropertySetDefinition( int id ) { m_id = id; }
IfcPropertySetDefinition::~IfcPropertySetDefinition() {}
shared_ptr<IfcPPObject> IfcPropertySetDefinition::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertySetDefinition> copy_self( new IfcPropertySetDefinition() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( CreateCompressedGuidString22() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPropertySetDefinition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYSETDEFINITION" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcPropertySetDefinition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertySetDefinition::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertySetDefinition, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
}
void IfcPropertySetDefinition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyDefinition::getAttributes( vec_attributes );
}
void IfcPropertySetDefinition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPropertyDefinition::getAttributesInverse( vec_attributes_inverse );
	if( m_DefinesType_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DefinesType_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_DefinesType_inverse.size(); ++i )
		{
			if( !m_DefinesType_inverse[i].expired() )
			{
				DefinesType_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcTypeObject>( m_DefinesType_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "DefinesType_inverse", DefinesType_inverse_vec_obj ) );
	}
	if( m_IsDefinedBy_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> IsDefinedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_IsDefinedBy_inverse.size(); ++i )
		{
			if( !m_IsDefinedBy_inverse[i].expired() )
			{
				IsDefinedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByTemplate>( m_IsDefinedBy_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "IsDefinedBy_inverse", IsDefinedBy_inverse_vec_obj ) );
	}
	if( m_DefinesOccurrence_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DefinesOccurrence_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_DefinesOccurrence_inverse.size(); ++i )
		{
			if( !m_DefinesOccurrence_inverse[i].expired() )
			{
				DefinesOccurrence_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByProperties>( m_DefinesOccurrence_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "DefinesOccurrence_inverse", DefinesOccurrence_inverse_vec_obj ) );
	}
}
void IfcPropertySetDefinition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertySetDefinition::unlinkSelf()
{
	IfcPropertyDefinition::unlinkSelf();
}

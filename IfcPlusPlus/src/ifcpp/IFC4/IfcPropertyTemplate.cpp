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
#include "include/IfcComplexPropertyTemplate.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetTemplate.h"
#include "include/IfcPropertyTemplate.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcText.h"

// ENTITY IfcPropertyTemplate 
IfcPropertyTemplate::IfcPropertyTemplate() {}
IfcPropertyTemplate::IfcPropertyTemplate( int id ) { m_id = id; }
IfcPropertyTemplate::~IfcPropertyTemplate() {}
shared_ptr<IfcPPObject> IfcPropertyTemplate::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertyTemplate> copy_self( new IfcPropertyTemplate() );
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
void IfcPropertyTemplate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYTEMPLATE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcPropertyTemplate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyTemplate::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertyTemplate, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
}
void IfcPropertyTemplate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyTemplateDefinition::getAttributes( vec_attributes );
}
void IfcPropertyTemplate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPropertyTemplateDefinition::getAttributesInverse( vec_attributes_inverse );
	if( m_PartOfComplexTemplate_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PartOfComplexTemplate_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PartOfComplexTemplate_inverse.size(); ++i )
		{
			if( !m_PartOfComplexTemplate_inverse[i].expired() )
			{
				PartOfComplexTemplate_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcComplexPropertyTemplate>( m_PartOfComplexTemplate_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PartOfComplexTemplate_inverse", PartOfComplexTemplate_inverse_vec_obj ) );
	}
	if( m_PartOfPsetTemplate_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PartOfPsetTemplate_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PartOfPsetTemplate_inverse.size(); ++i )
		{
			if( !m_PartOfPsetTemplate_inverse[i].expired() )
			{
				PartOfPsetTemplate_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPropertySetTemplate>( m_PartOfPsetTemplate_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PartOfPsetTemplate_inverse", PartOfPsetTemplate_inverse_vec_obj ) );
	}
}
void IfcPropertyTemplate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyTemplateDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyTemplate::unlinkSelf()
{
	IfcPropertyTemplateDefinition::unlinkSelf();
}

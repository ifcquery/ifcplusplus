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
#include "include/IfcLayeredItem.h"
#include "include/IfcLogical.h"
#include "include/IfcPresentationLayerWithStyle.h"
#include "include/IfcPresentationStyle.h"
#include "include/IfcText.h"

// ENTITY IfcPresentationLayerWithStyle 
IfcPresentationLayerWithStyle::IfcPresentationLayerWithStyle() { m_entity_enum = IFCPRESENTATIONLAYERWITHSTYLE; }
IfcPresentationLayerWithStyle::IfcPresentationLayerWithStyle( int id ) { m_id = id; m_entity_enum = IFCPRESENTATIONLAYERWITHSTYLE; }
IfcPresentationLayerWithStyle::~IfcPresentationLayerWithStyle() {}
shared_ptr<IfcPPObject> IfcPresentationLayerWithStyle::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPresentationLayerWithStyle> copy_self( new IfcPresentationLayerWithStyle() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_AssignedItems.size(); ++ii )
	{
		auto item_ii = m_AssignedItems[ii];
		if( item_ii )
		{
			copy_self->m_AssignedItems.push_back( dynamic_pointer_cast<IfcLayeredItem>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Identifier ) { copy_self->m_Identifier = dynamic_pointer_cast<IfcIdentifier>( m_Identifier->getDeepCopy(options) ); }
	if( m_LayerOn ) { copy_self->m_LayerOn = dynamic_pointer_cast<IfcLogical>( m_LayerOn->getDeepCopy(options) ); }
	if( m_LayerFrozen ) { copy_self->m_LayerFrozen = dynamic_pointer_cast<IfcLogical>( m_LayerFrozen->getDeepCopy(options) ); }
	if( m_LayerBlocked ) { copy_self->m_LayerBlocked = dynamic_pointer_cast<IfcLogical>( m_LayerBlocked->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_LayerStyles.size(); ++ii )
	{
		auto item_ii = m_LayerStyles[ii];
		if( item_ii )
		{
			copy_self->m_LayerStyles.push_back( dynamic_pointer_cast<IfcPresentationStyle>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcPresentationLayerWithStyle::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPRESENTATIONLAYERWITHSTYLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_AssignedItems.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcLayeredItem>& type_object = m_AssignedItems[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_Identifier ) { m_Identifier->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LayerOn ) { m_LayerOn->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LayerFrozen ) { m_LayerFrozen->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LayerBlocked ) { m_LayerBlocked->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_LayerStyles );
	stream << ");";
}
void IfcPresentationLayerWithStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPresentationLayerWithStyle::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPresentationLayerWithStyle, expecting 8, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readSelectList( args[2], m_AssignedItems, map );
	m_Identifier = IfcIdentifier::createObjectFromSTEP( args[3] );
	m_LayerOn = IfcLogical::createObjectFromSTEP( args[4] );
	m_LayerFrozen = IfcLogical::createObjectFromSTEP( args[5] );
	m_LayerBlocked = IfcLogical::createObjectFromSTEP( args[6] );
	readEntityReferenceList( args[7], m_LayerStyles, map );
}
void IfcPresentationLayerWithStyle::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationLayerAssignment::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LayerOn", m_LayerOn ) );
	vec_attributes.push_back( std::make_pair( "LayerFrozen", m_LayerFrozen ) );
	vec_attributes.push_back( std::make_pair( "LayerBlocked", m_LayerBlocked ) );
	if( m_LayerStyles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LayerStyles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_LayerStyles.begin(), m_LayerStyles.end(), std::back_inserter( LayerStyles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "LayerStyles", LayerStyles_vec_object ) );
	}
}
void IfcPresentationLayerWithStyle::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationLayerAssignment::getAttributesInverse( vec_attributes_inverse );
}
void IfcPresentationLayerWithStyle::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationLayerAssignment::setInverseCounterparts( ptr_self_entity );
}
void IfcPresentationLayerWithStyle::unlinkFromInverseCounterparts()
{
	IfcPresentationLayerAssignment::unlinkFromInverseCounterparts();
}

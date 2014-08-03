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
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcLayeredItem.h"
#include "include/IfcPresentationLayerWithStyle.h"
#include "include/IfcPresentationStyle.h"
#include "include/IfcText.h"

// ENTITY IfcPresentationLayerWithStyle 
IfcPresentationLayerWithStyle::IfcPresentationLayerWithStyle() {}
IfcPresentationLayerWithStyle::IfcPresentationLayerWithStyle( int id ) { m_id = id; }
IfcPresentationLayerWithStyle::~IfcPresentationLayerWithStyle() {}
shared_ptr<IfcPPObject> IfcPresentationLayerWithStyle::getDeepCopy()
{
	shared_ptr<IfcPresentationLayerWithStyle> copy_self( new IfcPresentationLayerWithStyle() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	for( size_t ii=0; ii<m_AssignedItems.size(); ++ii )
	{
		auto item_ii = m_AssignedItems[ii];
		if( item_ii )
		{
			copy_self->m_AssignedItems.push_back( dynamic_pointer_cast<IfcLayeredItem>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_Identifier ) { copy_self->m_Identifier = dynamic_pointer_cast<IfcIdentifier>( m_Identifier->getDeepCopy() ); }
	if( m_LayerOn ) { copy_self->m_LayerOn = m_LayerOn; }
	if( m_LayerFrozen ) { copy_self->m_LayerFrozen = m_LayerFrozen; }
	if( m_LayerBlocked ) { copy_self->m_LayerBlocked = m_LayerBlocked; }
	for( size_t ii=0; ii<m_LayerStyles.size(); ++ii )
	{
		auto item_ii = m_LayerStyles[ii];
		if( item_ii )
		{
			copy_self->m_LayerStyles.push_back( dynamic_pointer_cast<IfcPresentationStyle>(item_ii->getDeepCopy() ) );
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
	writeTypeList( stream, m_AssignedItems, true );
	stream << ",";
	if( m_Identifier ) { m_Identifier->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LayerOn == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_LayerOn == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ",";
	if( m_LayerFrozen == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_LayerFrozen == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ",";
	if( m_LayerBlocked == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_LayerBlocked == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ",";
	writeEntityList( stream, m_LayerStyles );
	stream << ");";
}
void IfcPresentationLayerWithStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPresentationLayerWithStyle::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPresentationLayerWithStyle, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcPresentationLayerWithStyle, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readSelectList( args[2], m_AssignedItems, map );
	m_Identifier = IfcIdentifier::createObjectFromStepData( args[3] );
	if( boost::iequals( args[4], L".F." ) ) { m_LayerOn = LOGICAL_FALSE; }
	else if( boost::iequals( args[4], L".T." ) ) { m_LayerOn = LOGICAL_TRUE; }
	else if( boost::iequals( args[4], L".U." ) ) { m_LayerOn = LOGICAL_UNKNOWN; }
	if( boost::iequals( args[5], L".F." ) ) { m_LayerFrozen = LOGICAL_FALSE; }
	else if( boost::iequals( args[5], L".T." ) ) { m_LayerFrozen = LOGICAL_TRUE; }
	else if( boost::iequals( args[5], L".U." ) ) { m_LayerFrozen = LOGICAL_UNKNOWN; }
	if( boost::iequals( args[6], L".F." ) ) { m_LayerBlocked = LOGICAL_FALSE; }
	else if( boost::iequals( args[6], L".T." ) ) { m_LayerBlocked = LOGICAL_TRUE; }
	else if( boost::iequals( args[6], L".U." ) ) { m_LayerBlocked = LOGICAL_UNKNOWN; }
	readEntityReferenceList( args[7], m_LayerStyles, map );
}
void IfcPresentationLayerWithStyle::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationLayerAssignment::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LayerOn", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_LayerOn ) ) ) );
	vec_attributes.push_back( std::make_pair( "LayerFrozen", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_LayerFrozen ) ) ) );
	vec_attributes.push_back( std::make_pair( "LayerBlocked", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_LayerBlocked ) ) ) );
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
void IfcPresentationLayerWithStyle::unlinkSelf()
{
	IfcPresentationLayerAssignment::unlinkSelf();
}

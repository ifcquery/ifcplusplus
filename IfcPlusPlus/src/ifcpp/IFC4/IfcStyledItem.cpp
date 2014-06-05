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
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRepresentationItem.h"
#include "include/IfcStyleAssignmentSelect.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcStyledItem 
IfcStyledItem::IfcStyledItem() {}
IfcStyledItem::IfcStyledItem( int id ) { m_id = id; }
IfcStyledItem::~IfcStyledItem() {}

// method setEntity takes over all attributes from another instance of the class
void IfcStyledItem::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcStyledItem> other = dynamic_pointer_cast<IfcStyledItem>(other_entity);
	if( !other) { return; }
	m_Item = other->m_Item;
	m_Styles = other->m_Styles;
	m_Name = other->m_Name;
}
void IfcStyledItem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSTYLEDITEM" << "(";
	if( m_Item ) { stream << "#" << m_Item->getId(); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_Styles, true );
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStyledItem::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStyledItem::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStyledItem, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcStyledItem, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Item, map );
	readSelectList( args[1], m_Styles, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
}
void IfcStyledItem::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Item", m_Item ) );
	shared_ptr<IfcPPAttributeObjectVector> Styles_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Styles.begin(), m_Styles.end(), std::back_inserter( Styles_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Styles", Styles_vec_object ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
}
void IfcStyledItem::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcStyledItem::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRepresentationItem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcStyledItem> ptr_self = dynamic_pointer_cast<IfcStyledItem>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcStyledItem::setInverseCounterparts: type mismatch" ); }
	if( m_Item )
	{
		m_Item->m_StyledByItem_inverse.push_back( ptr_self );
	}
}
void IfcStyledItem::unlinkSelf()
{
	IfcRepresentationItem::unlinkSelf();
	if( m_Item )
	{
		std::vector<weak_ptr<IfcStyledItem> >& StyledByItem_inverse = m_Item->m_StyledByItem_inverse;
		std::vector<weak_ptr<IfcStyledItem> >::iterator it_StyledByItem_inverse;
		for( it_StyledByItem_inverse = StyledByItem_inverse.begin(); it_StyledByItem_inverse != StyledByItem_inverse.end(); ++it_StyledByItem_inverse)
		{
			shared_ptr<IfcStyledItem> self_candidate( *it_StyledByItem_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				StyledByItem_inverse.erase( it_StyledByItem_inverse );
				break;
			}
		}
	}
}

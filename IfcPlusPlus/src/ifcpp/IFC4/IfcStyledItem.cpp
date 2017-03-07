/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcLabel.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRepresentationItem.h"
#include "include/IfcStyleAssignmentSelect.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcStyledItem 
IfcStyledItem::IfcStyledItem() { m_entity_enum = IFCSTYLEDITEM; }
IfcStyledItem::IfcStyledItem( int id ) { m_id = id; m_entity_enum = IFCSTYLEDITEM; }
IfcStyledItem::~IfcStyledItem() {}
shared_ptr<IfcPPObject> IfcStyledItem::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStyledItem> copy_self( new IfcStyledItem() );
	if( m_Item ) { copy_self->m_Item = dynamic_pointer_cast<IfcRepresentationItem>( m_Item->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Styles.size(); ++ii )
	{
		auto item_ii = m_Styles[ii];
		if( item_ii )
		{
			copy_self->m_Styles.push_back( dynamic_pointer_cast<IfcStyleAssignmentSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStyledItem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTYLEDITEM" << "(";
	if( m_Item ) { stream << "#" << m_Item->m_id; } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_Styles.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcStyleAssignmentSelect>& type_object = m_Styles[ii];
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
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStyledItem::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStyledItem::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcStyledItem, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Item, map );
	readSelectList( args[1], m_Styles, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
}
void IfcStyledItem::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Item", m_Item ) );
	if( m_Styles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Styles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Styles.begin(), m_Styles.end(), std::back_inserter( Styles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Styles", Styles_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
}
void IfcStyledItem::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRepresentationItem::getAttributesInverse( vec_attributes_inverse );
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
void IfcStyledItem::unlinkFromInverseCounterparts()
{
	IfcRepresentationItem::unlinkFromInverseCounterparts();
	if( m_Item )
	{
		std::vector<weak_ptr<IfcStyledItem> >& StyledByItem_inverse = m_Item->m_StyledByItem_inverse;
		for( auto it_StyledByItem_inverse = StyledByItem_inverse.begin(); it_StyledByItem_inverse != StyledByItem_inverse.end(); )
		{
			shared_ptr<IfcStyledItem> self_candidate( *it_StyledByItem_inverse );
			if( self_candidate.get() == this )
			{
				it_StyledByItem_inverse= StyledByItem_inverse.erase( it_StyledByItem_inverse );
			}
			else
			{
				++it_StyledByItem_inverse;
			}
		}
	}
}

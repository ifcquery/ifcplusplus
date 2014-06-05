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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcObjectTypeEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProduct.h"
#include "include/IfcProductSelect.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcText.h"
#include "include/IfcTypeProduct.h"

// ENTITY IfcRelAssignsToProduct 
IfcRelAssignsToProduct::IfcRelAssignsToProduct() {}
IfcRelAssignsToProduct::IfcRelAssignsToProduct( int id ) { m_id = id; }
IfcRelAssignsToProduct::~IfcRelAssignsToProduct() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssignsToProduct::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssignsToProduct> other = dynamic_pointer_cast<IfcRelAssignsToProduct>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatedObjectsType = other->m_RelatedObjectsType;
	m_RelatingProduct = other->m_RelatingProduct;
}
void IfcRelAssignsToProduct::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSIGNSTOPRODUCT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedObjects );
	stream << ",";
	if( m_RelatedObjectsType ) { m_RelatedObjectsType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingProduct ) { m_RelatingProduct->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssignsToProduct::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssignsToProduct::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssignsToProduct, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcRelAssignsToProduct, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatedObjectsType = IfcObjectTypeEnum::createObjectFromStepData( args[5] );
	m_RelatingProduct = IfcProductSelect::createObjectFromStepData( args[6], map );
}
void IfcRelAssignsToProduct::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssigns::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingProduct", m_RelatingProduct ) );
}
void IfcRelAssignsToProduct::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelAssignsToProduct::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssigns::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssignsToProduct> ptr_self = dynamic_pointer_cast<IfcRelAssignsToProduct>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssignsToProduct::setInverseCounterparts: type mismatch" ); }
	shared_ptr<IfcProduct>  RelatingProduct_IfcProduct = dynamic_pointer_cast<IfcProduct>( m_RelatingProduct );
	if( RelatingProduct_IfcProduct )
	{
		RelatingProduct_IfcProduct->m_ReferencedBy_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcTypeProduct>  RelatingProduct_IfcTypeProduct = dynamic_pointer_cast<IfcTypeProduct>( m_RelatingProduct );
	if( RelatingProduct_IfcTypeProduct )
	{
		RelatingProduct_IfcTypeProduct->m_ReferencedBy_inverse.push_back( ptr_self );
	}
}
void IfcRelAssignsToProduct::unlinkSelf()
{
	IfcRelAssigns::unlinkSelf();
	shared_ptr<IfcProduct>  RelatingProduct_IfcProduct = dynamic_pointer_cast<IfcProduct>( m_RelatingProduct );
	if( RelatingProduct_IfcProduct )
	{
		std::vector<weak_ptr<IfcRelAssignsToProduct> >& ReferencedBy_inverse = RelatingProduct_IfcProduct->m_ReferencedBy_inverse;
		std::vector<weak_ptr<IfcRelAssignsToProduct> >::iterator it_ReferencedBy_inverse;
		for( it_ReferencedBy_inverse = ReferencedBy_inverse.begin(); it_ReferencedBy_inverse != ReferencedBy_inverse.end(); ++it_ReferencedBy_inverse)
		{
			shared_ptr<IfcRelAssignsToProduct> self_candidate( *it_ReferencedBy_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ReferencedBy_inverse.erase( it_ReferencedBy_inverse );
				break;
			}
		}
	}
	shared_ptr<IfcTypeProduct>  RelatingProduct_IfcTypeProduct = dynamic_pointer_cast<IfcTypeProduct>( m_RelatingProduct );
	if( RelatingProduct_IfcTypeProduct )
	{
		std::vector<weak_ptr<IfcRelAssignsToProduct> >& ReferencedBy_inverse = RelatingProduct_IfcTypeProduct->m_ReferencedBy_inverse;
		std::vector<weak_ptr<IfcRelAssignsToProduct> >::iterator it_ReferencedBy_inverse;
		for( it_ReferencedBy_inverse = ReferencedBy_inverse.begin(); it_ReferencedBy_inverse != ReferencedBy_inverse.end(); ++it_ReferencedBy_inverse)
		{
			shared_ptr<IfcRelAssignsToProduct> self_candidate( *it_ReferencedBy_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ReferencedBy_inverse.erase( it_ReferencedBy_inverse );
				break;
			}
		}
	}
}

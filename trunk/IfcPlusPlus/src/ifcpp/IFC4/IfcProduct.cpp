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
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProduct.h"
#include "include/IfcProductDefinitionShape.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcProduct 
IfcProduct::IfcProduct() {}
IfcProduct::IfcProduct( int id ) { m_id = id; }
IfcProduct::~IfcProduct() {}
shared_ptr<IfcPPObject> IfcProduct::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcProduct> copy_self( new IfcProduct() );
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
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy(options) ); }
	if( m_ObjectPlacement ) { copy_self->m_ObjectPlacement = dynamic_pointer_cast<IfcObjectPlacement>( m_ObjectPlacement->getDeepCopy(options) ); }
	if( m_Representation ) { copy_self->m_Representation = dynamic_pointer_cast<IfcProductRepresentation>( m_Representation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcProduct::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPRODUCT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectPlacement ) { stream << "#" << m_ObjectPlacement->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Representation ) { stream << "#" << m_Representation->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcProduct::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProduct::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcProduct, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
}
void IfcProduct::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObject::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ObjectPlacement", m_ObjectPlacement ) );
	vec_attributes.push_back( std::make_pair( "Representation", m_Representation ) );
}
void IfcProduct::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcObject::getAttributesInverse( vec_attributes_inverse );
	if( m_ReferencedBy_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ReferencedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_ReferencedBy_inverse.size(); ++i )
		{
			if( !m_ReferencedBy_inverse[i].expired() )
			{
				ReferencedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssignsToProduct>( m_ReferencedBy_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "ReferencedBy_inverse", ReferencedBy_inverse_vec_obj ) );
	}
}
void IfcProduct::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObject::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcProduct> ptr_self = dynamic_pointer_cast<IfcProduct>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcProduct::setInverseCounterparts: type mismatch" ); }
	if( m_ObjectPlacement )
	{
		m_ObjectPlacement->m_PlacesObject_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcProductDefinitionShape>  Representation_IfcProductDefinitionShape = dynamic_pointer_cast<IfcProductDefinitionShape>( m_Representation );
	if( Representation_IfcProductDefinitionShape )
	{
		Representation_IfcProductDefinitionShape->m_ShapeOfProduct_inverse.push_back( ptr_self );
	}
}
void IfcProduct::unlinkSelf()
{
	IfcObject::unlinkSelf();
	if( m_ObjectPlacement )
	{
		std::vector<weak_ptr<IfcProduct> >& PlacesObject_inverse = m_ObjectPlacement->m_PlacesObject_inverse;
		for( auto it_PlacesObject_inverse = PlacesObject_inverse.begin(); it_PlacesObject_inverse != PlacesObject_inverse.end(); ++it_PlacesObject_inverse)
		{
			shared_ptr<IfcProduct> self_candidate( *it_PlacesObject_inverse );
			if( self_candidate.get() == this )
			{
				PlacesObject_inverse.erase( it_PlacesObject_inverse );
				break;
			}
		}
	}
	shared_ptr<IfcProductDefinitionShape>  Representation_IfcProductDefinitionShape = dynamic_pointer_cast<IfcProductDefinitionShape>( m_Representation );
	if( Representation_IfcProductDefinitionShape )
	{
		std::vector<weak_ptr<IfcProduct> >& ShapeOfProduct_inverse = Representation_IfcProductDefinitionShape->m_ShapeOfProduct_inverse;
		for( auto it_ShapeOfProduct_inverse = ShapeOfProduct_inverse.begin(); it_ShapeOfProduct_inverse != ShapeOfProduct_inverse.end(); ++it_ShapeOfProduct_inverse)
		{
			shared_ptr<IfcProduct> self_candidate( *it_ShapeOfProduct_inverse );
			if( self_candidate.get() == this )
			{
				ShapeOfProduct_inverse.erase( it_ShapeOfProduct_inverse );
				break;
			}
		}
	}
}

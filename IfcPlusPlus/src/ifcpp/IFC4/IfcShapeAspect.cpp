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
#include "include/IfcLabel.h"
#include "include/IfcLogical.h"
#include "include/IfcProductDefinitionShape.h"
#include "include/IfcProductRepresentationSelect.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcShapeAspect.h"
#include "include/IfcShapeModel.h"
#include "include/IfcText.h"

// ENTITY IfcShapeAspect 
IfcShapeAspect::IfcShapeAspect() { m_entity_enum = IFCSHAPEASPECT; }
IfcShapeAspect::IfcShapeAspect( int id ) { m_id = id; m_entity_enum = IFCSHAPEASPECT; }
IfcShapeAspect::~IfcShapeAspect() {}
shared_ptr<IfcPPObject> IfcShapeAspect::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcShapeAspect> copy_self( new IfcShapeAspect() );
	for( size_t ii=0; ii<m_ShapeRepresentations.size(); ++ii )
	{
		auto item_ii = m_ShapeRepresentations[ii];
		if( item_ii )
		{
			copy_self->m_ShapeRepresentations.push_back( dynamic_pointer_cast<IfcShapeModel>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ProductDefinitional ) { copy_self->m_ProductDefinitional = dynamic_pointer_cast<IfcLogical>( m_ProductDefinitional->getDeepCopy(options) ); }
	if( m_PartOfProductDefinitionShape ) { copy_self->m_PartOfProductDefinitionShape = dynamic_pointer_cast<IfcProductRepresentationSelect>( m_PartOfProductDefinitionShape->getDeepCopy(options) ); }
	return copy_self;
}
void IfcShapeAspect::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSHAPEASPECT" << "(";
	writeEntityList( stream, m_ShapeRepresentations );
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProductDefinitional ) { m_ProductDefinitional->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PartOfProductDefinitionShape ) { m_PartOfProductDefinitionShape->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcShapeAspect::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcShapeAspect::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcShapeAspect, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_ShapeRepresentations, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[1] );
	m_Description = IfcText::createObjectFromSTEP( args[2] );
	m_ProductDefinitional = IfcLogical::createObjectFromSTEP( args[3] );
	m_PartOfProductDefinitionShape = IfcProductRepresentationSelect::createObjectFromSTEP( args[4], map );
}
void IfcShapeAspect::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_ShapeRepresentations.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ShapeRepresentations_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ShapeRepresentations.begin(), m_ShapeRepresentations.end(), std::back_inserter( ShapeRepresentations_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ShapeRepresentations", ShapeRepresentations_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "ProductDefinitional", m_ProductDefinitional ) );
	vec_attributes.push_back( std::make_pair( "PartOfProductDefinitionShape", m_PartOfProductDefinitionShape ) );
}
void IfcShapeAspect::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcShapeAspect::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcShapeAspect> ptr_self = dynamic_pointer_cast<IfcShapeAspect>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcShapeAspect::setInverseCounterparts: type mismatch" ); }
	shared_ptr<IfcProductDefinitionShape>  PartOfProductDefinitionShape_IfcProductDefinitionShape = dynamic_pointer_cast<IfcProductDefinitionShape>( m_PartOfProductDefinitionShape );
	if( PartOfProductDefinitionShape_IfcProductDefinitionShape )
	{
		PartOfProductDefinitionShape_IfcProductDefinitionShape->m_HasShapeAspects_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcRepresentationMap>  PartOfProductDefinitionShape_IfcRepresentationMap = dynamic_pointer_cast<IfcRepresentationMap>( m_PartOfProductDefinitionShape );
	if( PartOfProductDefinitionShape_IfcRepresentationMap )
	{
		PartOfProductDefinitionShape_IfcRepresentationMap->m_HasShapeAspects_inverse.push_back( ptr_self );
	}
	for( size_t i=0; i<m_ShapeRepresentations.size(); ++i )
	{
		if( m_ShapeRepresentations[i] )
		{
			m_ShapeRepresentations[i]->m_OfShapeAspect_inverse.push_back( ptr_self );
		}
	}
}
void IfcShapeAspect::unlinkFromInverseCounterparts()
{
	shared_ptr<IfcProductDefinitionShape>  PartOfProductDefinitionShape_IfcProductDefinitionShape = dynamic_pointer_cast<IfcProductDefinitionShape>( m_PartOfProductDefinitionShape );
	if( PartOfProductDefinitionShape_IfcProductDefinitionShape )
	{
		std::vector<weak_ptr<IfcShapeAspect> >& HasShapeAspects_inverse = PartOfProductDefinitionShape_IfcProductDefinitionShape->m_HasShapeAspects_inverse;
		for( auto it_HasShapeAspects_inverse = HasShapeAspects_inverse.begin(); it_HasShapeAspects_inverse != HasShapeAspects_inverse.end(); )
		{
			shared_ptr<IfcShapeAspect> self_candidate( *it_HasShapeAspects_inverse );
			if( self_candidate.get() == this )
			{
				it_HasShapeAspects_inverse= HasShapeAspects_inverse.erase( it_HasShapeAspects_inverse );
			}
			else
			{
				++it_HasShapeAspects_inverse;
			}
		}
	}
	shared_ptr<IfcRepresentationMap>  PartOfProductDefinitionShape_IfcRepresentationMap = dynamic_pointer_cast<IfcRepresentationMap>( m_PartOfProductDefinitionShape );
	if( PartOfProductDefinitionShape_IfcRepresentationMap )
	{
		std::vector<weak_ptr<IfcShapeAspect> >& HasShapeAspects_inverse = PartOfProductDefinitionShape_IfcRepresentationMap->m_HasShapeAspects_inverse;
		for( auto it_HasShapeAspects_inverse = HasShapeAspects_inverse.begin(); it_HasShapeAspects_inverse != HasShapeAspects_inverse.end(); )
		{
			shared_ptr<IfcShapeAspect> self_candidate( *it_HasShapeAspects_inverse );
			if( self_candidate.get() == this )
			{
				it_HasShapeAspects_inverse= HasShapeAspects_inverse.erase( it_HasShapeAspects_inverse );
			}
			else
			{
				++it_HasShapeAspects_inverse;
			}
		}
	}
	for( size_t i=0; i<m_ShapeRepresentations.size(); ++i )
	{
		if( m_ShapeRepresentations[i] )
		{
			std::vector<weak_ptr<IfcShapeAspect> >& OfShapeAspect_inverse = m_ShapeRepresentations[i]->m_OfShapeAspect_inverse;
			for( auto it_OfShapeAspect_inverse = OfShapeAspect_inverse.begin(); it_OfShapeAspect_inverse != OfShapeAspect_inverse.end(); )
			{
				shared_ptr<IfcShapeAspect> self_candidate( *it_OfShapeAspect_inverse );
				if( self_candidate.get() == this )
				{
					it_OfShapeAspect_inverse= OfShapeAspect_inverse.erase( it_OfShapeAspect_inverse );
				}
				else
				{
					++it_OfShapeAspect_inverse;
				}
			}
		}
	}
}

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
#include "include/IfcProduct.h"
#include "include/IfcProductDefinitionShape.h"
#include "include/IfcRepresentation.h"
#include "include/IfcShapeAspect.h"
#include "include/IfcText.h"

// ENTITY IfcProductDefinitionShape 
IfcProductDefinitionShape::IfcProductDefinitionShape() {}
IfcProductDefinitionShape::IfcProductDefinitionShape( int id ) { m_id = id; }
IfcProductDefinitionShape::~IfcProductDefinitionShape() {}
shared_ptr<IfcPPObject> IfcProductDefinitionShape::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcProductDefinitionShape> copy_self( new IfcProductDefinitionShape() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Representations.size(); ++ii )
	{
		auto item_ii = m_Representations[ii];
		if( item_ii )
		{
			copy_self->m_Representations.push_back( dynamic_pointer_cast<IfcRepresentation>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcProductDefinitionShape::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPRODUCTDEFINITIONSHAPE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Representations );
	stream << ");";
}
void IfcProductDefinitionShape::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProductDefinitionShape::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcProductDefinitionShape, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReferenceList( args[2], m_Representations, map );
}
void IfcProductDefinitionShape::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProductRepresentation::getAttributes( vec_attributes );
}
void IfcProductDefinitionShape::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcProductRepresentation::getAttributesInverse( vec_attributes_inverse );
	if( m_ShapeOfProduct_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ShapeOfProduct_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_ShapeOfProduct_inverse.size(); ++i )
		{
			if( !m_ShapeOfProduct_inverse[i].expired() )
			{
				ShapeOfProduct_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcProduct>( m_ShapeOfProduct_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "ShapeOfProduct_inverse", ShapeOfProduct_inverse_vec_obj ) );
	}
	if( m_HasShapeAspects_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasShapeAspects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasShapeAspects_inverse.size(); ++i )
		{
			if( !m_HasShapeAspects_inverse[i].expired() )
			{
				HasShapeAspects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcShapeAspect>( m_HasShapeAspects_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasShapeAspects_inverse", HasShapeAspects_inverse_vec_obj ) );
	}
}
void IfcProductDefinitionShape::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProductRepresentation::setInverseCounterparts( ptr_self_entity );
}
void IfcProductDefinitionShape::unlinkSelf()
{
	IfcProductRepresentation::unlinkSelf();
}

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
#include "include/IfcProductDefinitionShape.h"
#include "include/IfcProductRepresentationSelect.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcShapeAspect.h"
#include "include/IfcShapeModel.h"
#include "include/IfcText.h"

// ENTITY IfcShapeAspect 
IfcShapeAspect::IfcShapeAspect() {}
IfcShapeAspect::IfcShapeAspect( int id ) { m_id = id; }
IfcShapeAspect::~IfcShapeAspect() {}

// method setEntity takes over all attributes from another instance of the class
void IfcShapeAspect::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcShapeAspect> other = dynamic_pointer_cast<IfcShapeAspect>(other_entity);
	if( !other) { return; }
	m_ShapeRepresentations = other->m_ShapeRepresentations;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ProductDefinitional = other->m_ProductDefinitional;
	m_PartOfProductDefinitionShape = other->m_PartOfProductDefinitionShape;
}
void IfcShapeAspect::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSHAPEASPECT" << "(";
	writeEntityList( stream, m_ShapeRepresentations );
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProductDefinitional == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_ProductDefinitional == LOGICAL_TRUE ) { stream << ".T."; }
	else if( m_ProductDefinitional == LOGICAL_UNKNOWN ) { stream << ".U."; }
	stream << ",";
	if( m_PartOfProductDefinitionShape ) { m_PartOfProductDefinitionShape->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcShapeAspect::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcShapeAspect::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcShapeAspect, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcShapeAspect, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_ShapeRepresentations, map );
	m_Name = IfcLabel::createObjectFromStepData( args[1] );
	m_Description = IfcText::createObjectFromStepData( args[2] );
	if( _stricmp( args[3].c_str(), ".F." ) == 0 ) { m_ProductDefinitional = LOGICAL_FALSE; }
	else if( _stricmp( args[3].c_str(), ".T." ) == 0 ) { m_ProductDefinitional = LOGICAL_TRUE; }
	else if( _stricmp( args[3].c_str(), ".U." ) == 0 ) { m_ProductDefinitional = LOGICAL_UNKNOWN; }
	m_PartOfProductDefinitionShape = IfcProductRepresentationSelect::createObjectFromStepData( args[4], map );
}
void IfcShapeAspect::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "ProductDefinitional", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_ProductDefinitional ) ) ) );
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
	for( int i=0; i<m_ShapeRepresentations.size(); ++i )
	{
		if( m_ShapeRepresentations[i] )
		{
			m_ShapeRepresentations[i]->m_OfShapeAspect_inverse.push_back( ptr_self );
		}
	}
}
void IfcShapeAspect::unlinkSelf()
{
	shared_ptr<IfcProductDefinitionShape>  PartOfProductDefinitionShape_IfcProductDefinitionShape = dynamic_pointer_cast<IfcProductDefinitionShape>( m_PartOfProductDefinitionShape );
	if( PartOfProductDefinitionShape_IfcProductDefinitionShape )
	{
		std::vector<weak_ptr<IfcShapeAspect> >& HasShapeAspects_inverse = PartOfProductDefinitionShape_IfcProductDefinitionShape->m_HasShapeAspects_inverse;
		std::vector<weak_ptr<IfcShapeAspect> >::iterator it_HasShapeAspects_inverse;
		for( it_HasShapeAspects_inverse = HasShapeAspects_inverse.begin(); it_HasShapeAspects_inverse != HasShapeAspects_inverse.end(); ++it_HasShapeAspects_inverse)
		{
			shared_ptr<IfcShapeAspect> self_candidate( *it_HasShapeAspects_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasShapeAspects_inverse.erase( it_HasShapeAspects_inverse );
				break;
			}
		}
	}
	shared_ptr<IfcRepresentationMap>  PartOfProductDefinitionShape_IfcRepresentationMap = dynamic_pointer_cast<IfcRepresentationMap>( m_PartOfProductDefinitionShape );
	if( PartOfProductDefinitionShape_IfcRepresentationMap )
	{
		std::vector<weak_ptr<IfcShapeAspect> >& HasShapeAspects_inverse = PartOfProductDefinitionShape_IfcRepresentationMap->m_HasShapeAspects_inverse;
		std::vector<weak_ptr<IfcShapeAspect> >::iterator it_HasShapeAspects_inverse;
		for( it_HasShapeAspects_inverse = HasShapeAspects_inverse.begin(); it_HasShapeAspects_inverse != HasShapeAspects_inverse.end(); ++it_HasShapeAspects_inverse)
		{
			shared_ptr<IfcShapeAspect> self_candidate( *it_HasShapeAspects_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasShapeAspects_inverse.erase( it_HasShapeAspects_inverse );
				break;
			}
		}
	}
	for( int i=0; i<m_ShapeRepresentations.size(); ++i )
	{
		if( m_ShapeRepresentations[i] )
		{
			std::vector<weak_ptr<IfcShapeAspect> >& OfShapeAspect_inverse = m_ShapeRepresentations[i]->m_OfShapeAspect_inverse;
			std::vector<weak_ptr<IfcShapeAspect> >::iterator it_OfShapeAspect_inverse;
			for( it_OfShapeAspect_inverse = OfShapeAspect_inverse.begin(); it_OfShapeAspect_inverse != OfShapeAspect_inverse.end(); ++it_OfShapeAspect_inverse)
			{
				shared_ptr<IfcShapeAspect> self_candidate( *it_OfShapeAspect_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					OfShapeAspect_inverse.erase( it_OfShapeAspect_inverse );
					break;
				}
			}
		}
	}
}

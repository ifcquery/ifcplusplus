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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcPhysicalComplexQuantity.h"
#include "include/IfcPhysicalQuantity.h"
#include "include/IfcText.h"

// ENTITY IfcPhysicalQuantity 
IfcPhysicalQuantity::IfcPhysicalQuantity() {}
IfcPhysicalQuantity::IfcPhysicalQuantity( int id ) { m_id = id; }
IfcPhysicalQuantity::~IfcPhysicalQuantity() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPhysicalQuantity::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPhysicalQuantity> other = dynamic_pointer_cast<IfcPhysicalQuantity>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
}
void IfcPhysicalQuantity::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPHYSICALQUANTITY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPhysicalQuantity::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPhysicalQuantity::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPhysicalQuantity, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcPhysicalQuantity, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
}
void IfcPhysicalQuantity::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcPhysicalQuantity::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasExternalReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasExternalReferences_inverse.size(); ++i ) { HasExternalReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReferences_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasExternalReferences_inverse", HasExternalReferences_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> PartOfComplex_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_PartOfComplex_inverse.size(); ++i ) { PartOfComplex_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPhysicalComplexQuantity>( m_PartOfComplex_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "PartOfComplex_inverse", PartOfComplex_inverse_vec_obj ) );
}
void IfcPhysicalQuantity::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcPhysicalQuantity::unlinkSelf()
{
}

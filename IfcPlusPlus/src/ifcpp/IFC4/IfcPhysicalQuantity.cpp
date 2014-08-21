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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcPhysicalComplexQuantity.h"
#include "include/IfcPhysicalQuantity.h"
#include "include/IfcText.h"

// ENTITY IfcPhysicalQuantity 
IfcPhysicalQuantity::IfcPhysicalQuantity() {}
IfcPhysicalQuantity::IfcPhysicalQuantity( int id ) { m_id = id; }
IfcPhysicalQuantity::~IfcPhysicalQuantity() {}
shared_ptr<IfcPPObject> IfcPhysicalQuantity::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPhysicalQuantity> copy_self( new IfcPhysicalQuantity() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPhysicalQuantity::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPHYSICALQUANTITY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPhysicalQuantity::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPhysicalQuantity::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPhysicalQuantity, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
}
void IfcPhysicalQuantity::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcPhysicalQuantity::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_HasExternalReferences_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasExternalReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasExternalReferences_inverse.size(); ++i )
		{
			if( !m_HasExternalReferences_inverse[i].expired() )
			{
				HasExternalReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReferences_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasExternalReferences_inverse", HasExternalReferences_inverse_vec_obj ) );
	}
	if( m_PartOfComplex_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PartOfComplex_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PartOfComplex_inverse.size(); ++i )
		{
			if( !m_PartOfComplex_inverse[i].expired() )
			{
				PartOfComplex_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPhysicalComplexQuantity>( m_PartOfComplex_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PartOfComplex_inverse", PartOfComplex_inverse_vec_obj ) );
	}
}
void IfcPhysicalQuantity::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcPhysicalQuantity::unlinkSelf()
{
}

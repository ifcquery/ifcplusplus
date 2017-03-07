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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcPhysicalComplexQuantity.h"
#include "include/IfcPhysicalQuantity.h"
#include "include/IfcText.h"

// ENTITY IfcPhysicalQuantity 
IfcPhysicalQuantity::IfcPhysicalQuantity() { m_entity_enum = IFCPHYSICALQUANTITY; }
IfcPhysicalQuantity::IfcPhysicalQuantity( int id ) { m_id = id; m_entity_enum = IFCPHYSICALQUANTITY; }
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
void IfcPhysicalQuantity::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPhysicalQuantity, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
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
void IfcPhysicalQuantity::unlinkFromInverseCounterparts()
{
}

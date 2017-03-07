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
#include "include/IfcProfileDef.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcProfileDef 
IfcProfileDef::IfcProfileDef() { m_entity_enum = IFCPROFILEDEF; }
IfcProfileDef::IfcProfileDef( int id ) { m_id = id; m_entity_enum = IFCPROFILEDEF; }
IfcProfileDef::~IfcProfileDef() {}
shared_ptr<IfcPPObject> IfcProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcProfileDef> copy_self( new IfcProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	return copy_self;
}
void IfcProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProfileDef::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcProfileDef, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
}
void IfcProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "ProfileType", m_ProfileType ) );
	vec_attributes.push_back( std::make_pair( "ProfileName", m_ProfileName ) );
}
void IfcProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_HasExternalReference_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasExternalReference_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasExternalReference_inverse.size(); ++i )
		{
			if( !m_HasExternalReference_inverse[i].expired() )
			{
				HasExternalReference_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReference_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasExternalReference_inverse", HasExternalReference_inverse_vec_obj ) );
	}
	if( m_HasProperties_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasProperties_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasProperties_inverse.size(); ++i )
		{
			if( !m_HasProperties_inverse[i].expired() )
			{
				HasProperties_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcProfileProperties>( m_HasProperties_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasProperties_inverse", HasProperties_inverse_vec_obj ) );
	}
}
void IfcProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcProfileDef::unlinkFromInverseCounterparts()
{
}

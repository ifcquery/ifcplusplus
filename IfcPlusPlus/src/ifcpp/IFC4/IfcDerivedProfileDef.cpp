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
#include "include/IfcCartesianTransformationOperator2D.h"
#include "include/IfcDerivedProfileDef.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcProfileDef.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcDerivedProfileDef 
IfcDerivedProfileDef::IfcDerivedProfileDef() { m_entity_enum = IFCDERIVEDPROFILEDEF; }
IfcDerivedProfileDef::IfcDerivedProfileDef( int id ) { m_id = id; m_entity_enum = IFCDERIVEDPROFILEDEF; }
IfcDerivedProfileDef::~IfcDerivedProfileDef() {}
shared_ptr<IfcPPObject> IfcDerivedProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDerivedProfileDef> copy_self( new IfcDerivedProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_ParentProfile )
	{
		if( options.shallow_copy_IfcProfileDef ) { copy_self->m_ParentProfile = m_ParentProfile; }
		else { copy_self->m_ParentProfile = dynamic_pointer_cast<IfcProfileDef>( m_ParentProfile->getDeepCopy(options) ); }
	}
	if( m_Operator ) { copy_self->m_Operator = dynamic_pointer_cast<IfcCartesianTransformationOperator2D>( m_Operator->getDeepCopy(options) ); }
	if( m_Label ) { copy_self->m_Label = dynamic_pointer_cast<IfcLabel>( m_Label->getDeepCopy(options) ); }
	return copy_self;
}
void IfcDerivedProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDERIVEDPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ParentProfile ) { stream << "#" << m_ParentProfile->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Operator ) { stream << "#" << m_Operator->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Label ) { m_Label->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDerivedProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDerivedProfileDef::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcDerivedProfileDef, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_ParentProfile, map );
	readEntityReference( args[3], m_Operator, map );
	m_Label = IfcLabel::createObjectFromSTEP( args[4] );
}
void IfcDerivedProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ParentProfile", m_ParentProfile ) );
	vec_attributes.push_back( std::make_pair( "Operator", m_Operator ) );
	vec_attributes.push_back( std::make_pair( "Label", m_Label ) );
}
void IfcDerivedProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcDerivedProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcDerivedProfileDef::unlinkFromInverseCounterparts()
{
	IfcProfileDef::unlinkFromInverseCounterparts();
}

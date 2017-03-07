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
#include "include/IfcArbitraryClosedProfileDef.h"
#include "include/IfcCurve.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcArbitraryClosedProfileDef 
IfcArbitraryClosedProfileDef::IfcArbitraryClosedProfileDef() { m_entity_enum = IFCARBITRARYCLOSEDPROFILEDEF; }
IfcArbitraryClosedProfileDef::IfcArbitraryClosedProfileDef( int id ) { m_id = id; m_entity_enum = IFCARBITRARYCLOSEDPROFILEDEF; }
IfcArbitraryClosedProfileDef::~IfcArbitraryClosedProfileDef() {}
shared_ptr<IfcPPObject> IfcArbitraryClosedProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcArbitraryClosedProfileDef> copy_self( new IfcArbitraryClosedProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_OuterCurve ) { copy_self->m_OuterCurve = dynamic_pointer_cast<IfcCurve>( m_OuterCurve->getDeepCopy(options) ); }
	return copy_self;
}
void IfcArbitraryClosedProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCARBITRARYCLOSEDPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OuterCurve ) { stream << "#" << m_OuterCurve->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcArbitraryClosedProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcArbitraryClosedProfileDef::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcArbitraryClosedProfileDef, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_OuterCurve, map );
}
void IfcArbitraryClosedProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "OuterCurve", m_OuterCurve ) );
}
void IfcArbitraryClosedProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcArbitraryClosedProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcArbitraryClosedProfileDef::unlinkFromInverseCounterparts()
{
	IfcProfileDef::unlinkFromInverseCounterparts();
}

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
#include "include/IfcAxis2Placement2D.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"
#include "include/IfcTrapeziumProfileDef.h"

// ENTITY IfcTrapeziumProfileDef 
IfcTrapeziumProfileDef::IfcTrapeziumProfileDef() { m_entity_enum = IFCTRAPEZIUMPROFILEDEF; }
IfcTrapeziumProfileDef::IfcTrapeziumProfileDef( int id ) { m_id = id; m_entity_enum = IFCTRAPEZIUMPROFILEDEF; }
IfcTrapeziumProfileDef::~IfcTrapeziumProfileDef() {}
shared_ptr<IfcPPObject> IfcTrapeziumProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTrapeziumProfileDef> copy_self( new IfcTrapeziumProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement2D>( m_Position->getDeepCopy(options) ); }
	if( m_BottomXDim ) { copy_self->m_BottomXDim = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_BottomXDim->getDeepCopy(options) ); }
	if( m_TopXDim ) { copy_self->m_TopXDim = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_TopXDim->getDeepCopy(options) ); }
	if( m_YDim ) { copy_self->m_YDim = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_YDim->getDeepCopy(options) ); }
	if( m_TopXOffset ) { copy_self->m_TopXOffset = dynamic_pointer_cast<IfcLengthMeasure>( m_TopXOffset->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTrapeziumProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTRAPEZIUMPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_BottomXDim ) { m_BottomXDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopXDim ) { m_TopXDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_YDim ) { m_YDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopXOffset ) { m_TopXOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTrapeziumProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTrapeziumProfileDef::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTrapeziumProfileDef, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_BottomXDim = IfcPositiveLengthMeasure::createObjectFromSTEP( args[3] );
	m_TopXDim = IfcPositiveLengthMeasure::createObjectFromSTEP( args[4] );
	m_YDim = IfcPositiveLengthMeasure::createObjectFromSTEP( args[5] );
	m_TopXOffset = IfcLengthMeasure::createObjectFromSTEP( args[6] );
}
void IfcTrapeziumProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BottomXDim", m_BottomXDim ) );
	vec_attributes.push_back( std::make_pair( "TopXDim", m_TopXDim ) );
	vec_attributes.push_back( std::make_pair( "YDim", m_YDim ) );
	vec_attributes.push_back( std::make_pair( "TopXOffset", m_TopXOffset ) );
}
void IfcTrapeziumProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcParameterizedProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcTrapeziumProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcTrapeziumProfileDef::unlinkFromInverseCounterparts()
{
	IfcParameterizedProfileDef::unlinkFromInverseCounterparts();
}

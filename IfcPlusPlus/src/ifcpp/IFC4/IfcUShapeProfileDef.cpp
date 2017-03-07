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
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"
#include "include/IfcUShapeProfileDef.h"

// ENTITY IfcUShapeProfileDef 
IfcUShapeProfileDef::IfcUShapeProfileDef() { m_entity_enum = IFCUSHAPEPROFILEDEF; }
IfcUShapeProfileDef::IfcUShapeProfileDef( int id ) { m_id = id; m_entity_enum = IFCUSHAPEPROFILEDEF; }
IfcUShapeProfileDef::~IfcUShapeProfileDef() {}
shared_ptr<IfcPPObject> IfcUShapeProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcUShapeProfileDef> copy_self( new IfcUShapeProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement2D>( m_Position->getDeepCopy(options) ); }
	if( m_Depth ) { copy_self->m_Depth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Depth->getDeepCopy(options) ); }
	if( m_FlangeWidth ) { copy_self->m_FlangeWidth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_FlangeWidth->getDeepCopy(options) ); }
	if( m_WebThickness ) { copy_self->m_WebThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_WebThickness->getDeepCopy(options) ); }
	if( m_FlangeThickness ) { copy_self->m_FlangeThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_FlangeThickness->getDeepCopy(options) ); }
	if( m_FilletRadius ) { copy_self->m_FilletRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_FilletRadius->getDeepCopy(options) ); }
	if( m_EdgeRadius ) { copy_self->m_EdgeRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_EdgeRadius->getDeepCopy(options) ); }
	if( m_FlangeSlope ) { copy_self->m_FlangeSlope = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_FlangeSlope->getDeepCopy(options) ); }
	return copy_self;
}
void IfcUShapeProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCUSHAPEPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Depth ) { m_Depth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FlangeWidth ) { m_FlangeWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_WebThickness ) { m_WebThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FlangeThickness ) { m_FlangeThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FilletRadius ) { m_FilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EdgeRadius ) { m_EdgeRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FlangeSlope ) { m_FlangeSlope->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcUShapeProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcUShapeProfileDef::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 10 ){ std::stringstream err; err << "Wrong parameter count for entity IfcUShapeProfileDef, expecting 10, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_Depth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[3] );
	m_FlangeWidth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[4] );
	m_WebThickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[5] );
	m_FlangeThickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[6] );
	m_FilletRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[7] );
	m_EdgeRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[8] );
	m_FlangeSlope = IfcPlaneAngleMeasure::createObjectFromSTEP( args[9] );
}
void IfcUShapeProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Depth", m_Depth ) );
	vec_attributes.push_back( std::make_pair( "FlangeWidth", m_FlangeWidth ) );
	vec_attributes.push_back( std::make_pair( "WebThickness", m_WebThickness ) );
	vec_attributes.push_back( std::make_pair( "FlangeThickness", m_FlangeThickness ) );
	vec_attributes.push_back( std::make_pair( "FilletRadius", m_FilletRadius ) );
	vec_attributes.push_back( std::make_pair( "EdgeRadius", m_EdgeRadius ) );
	vec_attributes.push_back( std::make_pair( "FlangeSlope", m_FlangeSlope ) );
}
void IfcUShapeProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcParameterizedProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcUShapeProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcUShapeProfileDef::unlinkFromInverseCounterparts()
{
	IfcParameterizedProfileDef::unlinkFromInverseCounterparts();
}

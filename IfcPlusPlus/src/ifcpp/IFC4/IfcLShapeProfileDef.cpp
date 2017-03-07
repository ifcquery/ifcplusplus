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
#include "include/IfcLShapeProfileDef.h"
#include "include/IfcLabel.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcLShapeProfileDef 
IfcLShapeProfileDef::IfcLShapeProfileDef() { m_entity_enum = IFCLSHAPEPROFILEDEF; }
IfcLShapeProfileDef::IfcLShapeProfileDef( int id ) { m_id = id; m_entity_enum = IFCLSHAPEPROFILEDEF; }
IfcLShapeProfileDef::~IfcLShapeProfileDef() {}
shared_ptr<IfcPPObject> IfcLShapeProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLShapeProfileDef> copy_self( new IfcLShapeProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement2D>( m_Position->getDeepCopy(options) ); }
	if( m_Depth ) { copy_self->m_Depth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Depth->getDeepCopy(options) ); }
	if( m_Width ) { copy_self->m_Width = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Width->getDeepCopy(options) ); }
	if( m_Thickness ) { copy_self->m_Thickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Thickness->getDeepCopy(options) ); }
	if( m_FilletRadius ) { copy_self->m_FilletRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_FilletRadius->getDeepCopy(options) ); }
	if( m_EdgeRadius ) { copy_self->m_EdgeRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_EdgeRadius->getDeepCopy(options) ); }
	if( m_LegSlope ) { copy_self->m_LegSlope = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_LegSlope->getDeepCopy(options) ); }
	return copy_self;
}
void IfcLShapeProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCLSHAPEPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Depth ) { m_Depth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Width ) { m_Width->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Thickness ) { m_Thickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FilletRadius ) { m_FilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EdgeRadius ) { m_EdgeRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LegSlope ) { m_LegSlope->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcLShapeProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLShapeProfileDef::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcLShapeProfileDef, expecting 9, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_Depth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[3] );
	m_Width = IfcPositiveLengthMeasure::createObjectFromSTEP( args[4] );
	m_Thickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[5] );
	m_FilletRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[6] );
	m_EdgeRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[7] );
	m_LegSlope = IfcPlaneAngleMeasure::createObjectFromSTEP( args[8] );
}
void IfcLShapeProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Depth", m_Depth ) );
	vec_attributes.push_back( std::make_pair( "Width", m_Width ) );
	vec_attributes.push_back( std::make_pair( "Thickness", m_Thickness ) );
	vec_attributes.push_back( std::make_pair( "FilletRadius", m_FilletRadius ) );
	vec_attributes.push_back( std::make_pair( "EdgeRadius", m_EdgeRadius ) );
	vec_attributes.push_back( std::make_pair( "LegSlope", m_LegSlope ) );
}
void IfcLShapeProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcParameterizedProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcLShapeProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcLShapeProfileDef::unlinkFromInverseCounterparts()
{
	IfcParameterizedProfileDef::unlinkFromInverseCounterparts();
}

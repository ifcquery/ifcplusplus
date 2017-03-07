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
#include "include/IfcCShapeProfileDef.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcCShapeProfileDef 
IfcCShapeProfileDef::IfcCShapeProfileDef() { m_entity_enum = IFCCSHAPEPROFILEDEF; }
IfcCShapeProfileDef::IfcCShapeProfileDef( int id ) { m_id = id; m_entity_enum = IFCCSHAPEPROFILEDEF; }
IfcCShapeProfileDef::~IfcCShapeProfileDef() {}
shared_ptr<IfcPPObject> IfcCShapeProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCShapeProfileDef> copy_self( new IfcCShapeProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement2D>( m_Position->getDeepCopy(options) ); }
	if( m_Depth ) { copy_self->m_Depth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Depth->getDeepCopy(options) ); }
	if( m_Width ) { copy_self->m_Width = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Width->getDeepCopy(options) ); }
	if( m_WallThickness ) { copy_self->m_WallThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_WallThickness->getDeepCopy(options) ); }
	if( m_Girth ) { copy_self->m_Girth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Girth->getDeepCopy(options) ); }
	if( m_InternalFilletRadius ) { copy_self->m_InternalFilletRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_InternalFilletRadius->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCShapeProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCSHAPEPROFILEDEF" << "(";
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
	if( m_WallThickness ) { m_WallThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Girth ) { m_Girth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InternalFilletRadius ) { m_InternalFilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCShapeProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCShapeProfileDef::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCShapeProfileDef, expecting 8, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_Depth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[3] );
	m_Width = IfcPositiveLengthMeasure::createObjectFromSTEP( args[4] );
	m_WallThickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[5] );
	m_Girth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[6] );
	m_InternalFilletRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[7] );
}
void IfcCShapeProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Depth", m_Depth ) );
	vec_attributes.push_back( std::make_pair( "Width", m_Width ) );
	vec_attributes.push_back( std::make_pair( "WallThickness", m_WallThickness ) );
	vec_attributes.push_back( std::make_pair( "Girth", m_Girth ) );
	vec_attributes.push_back( std::make_pair( "InternalFilletRadius", m_InternalFilletRadius ) );
}
void IfcCShapeProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcParameterizedProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcCShapeProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcCShapeProfileDef::unlinkFromInverseCounterparts()
{
	IfcParameterizedProfileDef::unlinkFromInverseCounterparts();
}

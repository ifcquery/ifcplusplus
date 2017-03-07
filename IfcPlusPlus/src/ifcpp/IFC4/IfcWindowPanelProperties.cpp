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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByTemplate.h"
#include "include/IfcShapeAspect.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"
#include "include/IfcWindowPanelOperationEnum.h"
#include "include/IfcWindowPanelPositionEnum.h"
#include "include/IfcWindowPanelProperties.h"

// ENTITY IfcWindowPanelProperties 
IfcWindowPanelProperties::IfcWindowPanelProperties() { m_entity_enum = IFCWINDOWPANELPROPERTIES; }
IfcWindowPanelProperties::IfcWindowPanelProperties( int id ) { m_id = id; m_entity_enum = IFCWINDOWPANELPROPERTIES; }
IfcWindowPanelProperties::~IfcWindowPanelProperties() {}
shared_ptr<IfcPPObject> IfcWindowPanelProperties::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWindowPanelProperties> copy_self( new IfcWindowPanelProperties() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_OperationType ) { copy_self->m_OperationType = dynamic_pointer_cast<IfcWindowPanelOperationEnum>( m_OperationType->getDeepCopy(options) ); }
	if( m_PanelPosition ) { copy_self->m_PanelPosition = dynamic_pointer_cast<IfcWindowPanelPositionEnum>( m_PanelPosition->getDeepCopy(options) ); }
	if( m_FrameDepth ) { copy_self->m_FrameDepth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_FrameDepth->getDeepCopy(options) ); }
	if( m_FrameThickness ) { copy_self->m_FrameThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_FrameThickness->getDeepCopy(options) ); }
	if( m_ShapeAspectStyle ) { copy_self->m_ShapeAspectStyle = dynamic_pointer_cast<IfcShapeAspect>( m_ShapeAspectStyle->getDeepCopy(options) ); }
	return copy_self;
}
void IfcWindowPanelProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCWINDOWPANELPROPERTIES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OperationType ) { m_OperationType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PanelPosition ) { m_PanelPosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FrameDepth ) { m_FrameDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FrameThickness ) { m_FrameThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ShapeAspectStyle ) { stream << "#" << m_ShapeAspectStyle->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcWindowPanelProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcWindowPanelProperties::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcWindowPanelProperties, expecting 9, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_OperationType = IfcWindowPanelOperationEnum::createObjectFromSTEP( args[4] );
	m_PanelPosition = IfcWindowPanelPositionEnum::createObjectFromSTEP( args[5] );
	m_FrameDepth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[6] );
	m_FrameThickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[7] );
	readEntityReference( args[8], m_ShapeAspectStyle, map );
}
void IfcWindowPanelProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedPropertySet::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "OperationType", m_OperationType ) );
	vec_attributes.push_back( std::make_pair( "PanelPosition", m_PanelPosition ) );
	vec_attributes.push_back( std::make_pair( "FrameDepth", m_FrameDepth ) );
	vec_attributes.push_back( std::make_pair( "FrameThickness", m_FrameThickness ) );
	vec_attributes.push_back( std::make_pair( "ShapeAspectStyle", m_ShapeAspectStyle ) );
}
void IfcWindowPanelProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedPropertySet::getAttributesInverse( vec_attributes_inverse );
}
void IfcWindowPanelProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedPropertySet::setInverseCounterparts( ptr_self_entity );
}
void IfcWindowPanelProperties::unlinkFromInverseCounterparts()
{
	IfcPreDefinedPropertySet::unlinkFromInverseCounterparts();
}

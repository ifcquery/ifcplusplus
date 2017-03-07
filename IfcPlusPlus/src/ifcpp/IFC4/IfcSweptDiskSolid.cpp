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
#include "include/IfcCurve.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSweptDiskSolid.h"

// ENTITY IfcSweptDiskSolid 
IfcSweptDiskSolid::IfcSweptDiskSolid() { m_entity_enum = IFCSWEPTDISKSOLID; }
IfcSweptDiskSolid::IfcSweptDiskSolid( int id ) { m_id = id; m_entity_enum = IFCSWEPTDISKSOLID; }
IfcSweptDiskSolid::~IfcSweptDiskSolid() {}
shared_ptr<IfcPPObject> IfcSweptDiskSolid::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSweptDiskSolid> copy_self( new IfcSweptDiskSolid() );
	if( m_Directrix ) { copy_self->m_Directrix = dynamic_pointer_cast<IfcCurve>( m_Directrix->getDeepCopy(options) ); }
	if( m_Radius ) { copy_self->m_Radius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Radius->getDeepCopy(options) ); }
	if( m_InnerRadius ) { copy_self->m_InnerRadius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_InnerRadius->getDeepCopy(options) ); }
	if( m_StartParam ) { copy_self->m_StartParam = dynamic_pointer_cast<IfcParameterValue>( m_StartParam->getDeepCopy(options) ); }
	if( m_EndParam ) { copy_self->m_EndParam = dynamic_pointer_cast<IfcParameterValue>( m_EndParam->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSweptDiskSolid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSWEPTDISKSOLID" << "(";
	if( m_Directrix ) { stream << "#" << m_Directrix->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Radius ) { m_Radius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InnerRadius ) { m_InnerRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_StartParam ) { m_StartParam->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EndParam ) { m_EndParam->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSweptDiskSolid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSweptDiskSolid::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSweptDiskSolid, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Directrix, map );
	m_Radius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[1] );
	m_InnerRadius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[2] );
	m_StartParam = IfcParameterValue::createObjectFromSTEP( args[3] );
	m_EndParam = IfcParameterValue::createObjectFromSTEP( args[4] );
}
void IfcSweptDiskSolid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSolidModel::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Directrix", m_Directrix ) );
	vec_attributes.push_back( std::make_pair( "Radius", m_Radius ) );
	vec_attributes.push_back( std::make_pair( "InnerRadius", m_InnerRadius ) );
	vec_attributes.push_back( std::make_pair( "StartParam", m_StartParam ) );
	vec_attributes.push_back( std::make_pair( "EndParam", m_EndParam ) );
}
void IfcSweptDiskSolid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSolidModel::getAttributesInverse( vec_attributes_inverse );
}
void IfcSweptDiskSolid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSolidModel::setInverseCounterparts( ptr_self_entity );
}
void IfcSweptDiskSolid::unlinkFromInverseCounterparts()
{
	IfcSolidModel::unlinkFromInverseCounterparts();
}

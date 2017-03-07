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
#include "include/IfcCartesianPoint.h"
#include "include/IfcCartesianTransformationOperator3DnonUniform.h"
#include "include/IfcDirection.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcReal.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcCartesianTransformationOperator3DnonUniform 
IfcCartesianTransformationOperator3DnonUniform::IfcCartesianTransformationOperator3DnonUniform() { m_entity_enum = IFCCARTESIANTRANSFORMATIONOPERATOR3DNONUNIFORM; }
IfcCartesianTransformationOperator3DnonUniform::IfcCartesianTransformationOperator3DnonUniform( int id ) { m_id = id; m_entity_enum = IFCCARTESIANTRANSFORMATIONOPERATOR3DNONUNIFORM; }
IfcCartesianTransformationOperator3DnonUniform::~IfcCartesianTransformationOperator3DnonUniform() {}
shared_ptr<IfcPPObject> IfcCartesianTransformationOperator3DnonUniform::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCartesianTransformationOperator3DnonUniform> copy_self( new IfcCartesianTransformationOperator3DnonUniform() );
	if( m_Axis1 ) { copy_self->m_Axis1 = dynamic_pointer_cast<IfcDirection>( m_Axis1->getDeepCopy(options) ); }
	if( m_Axis2 ) { copy_self->m_Axis2 = dynamic_pointer_cast<IfcDirection>( m_Axis2->getDeepCopy(options) ); }
	if( m_LocalOrigin ) { copy_self->m_LocalOrigin = dynamic_pointer_cast<IfcCartesianPoint>( m_LocalOrigin->getDeepCopy(options) ); }
	if( m_Scale ) { copy_self->m_Scale = dynamic_pointer_cast<IfcReal>( m_Scale->getDeepCopy(options) ); }
	if( m_Axis3 ) { copy_self->m_Axis3 = dynamic_pointer_cast<IfcDirection>( m_Axis3->getDeepCopy(options) ); }
	if( m_Scale2 ) { copy_self->m_Scale2 = dynamic_pointer_cast<IfcReal>( m_Scale2->getDeepCopy(options) ); }
	if( m_Scale3 ) { copy_self->m_Scale3 = dynamic_pointer_cast<IfcReal>( m_Scale3->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCartesianTransformationOperator3DnonUniform::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCARTESIANTRANSFORMATIONOPERATOR3DNONUNIFORM" << "(";
	if( m_Axis1 ) { stream << "#" << m_Axis1->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Axis2 ) { stream << "#" << m_Axis2->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_LocalOrigin ) { stream << "#" << m_LocalOrigin->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Scale ) { m_Scale->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Axis3 ) { stream << "#" << m_Axis3->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Scale2 ) { m_Scale2->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Scale3 ) { m_Scale3->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCartesianTransformationOperator3DnonUniform::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCartesianTransformationOperator3DnonUniform::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCartesianTransformationOperator3DnonUniform, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Axis1, map );
	readEntityReference( args[1], m_Axis2, map );
	readEntityReference( args[2], m_LocalOrigin, map );
	m_Scale = IfcReal::createObjectFromSTEP( args[3] );
	readEntityReference( args[4], m_Axis3, map );
	m_Scale2 = IfcReal::createObjectFromSTEP( args[5] );
	m_Scale3 = IfcReal::createObjectFromSTEP( args[6] );
}
void IfcCartesianTransformationOperator3DnonUniform::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCartesianTransformationOperator3D::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Scale2", m_Scale2 ) );
	vec_attributes.push_back( std::make_pair( "Scale3", m_Scale3 ) );
}
void IfcCartesianTransformationOperator3DnonUniform::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCartesianTransformationOperator3D::getAttributesInverse( vec_attributes_inverse );
}
void IfcCartesianTransformationOperator3DnonUniform::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCartesianTransformationOperator3D::setInverseCounterparts( ptr_self_entity );
}
void IfcCartesianTransformationOperator3DnonUniform::unlinkFromInverseCounterparts()
{
	IfcCartesianTransformationOperator3D::unlinkFromInverseCounterparts();
}

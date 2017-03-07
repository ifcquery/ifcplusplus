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
#include "include/IfcCartesianTransformationOperator3D.h"
#include "include/IfcDirection.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcReal.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcCartesianTransformationOperator3D 
IfcCartesianTransformationOperator3D::IfcCartesianTransformationOperator3D() { m_entity_enum = IFCCARTESIANTRANSFORMATIONOPERATOR3D; }
IfcCartesianTransformationOperator3D::IfcCartesianTransformationOperator3D( int id ) { m_id = id; m_entity_enum = IFCCARTESIANTRANSFORMATIONOPERATOR3D; }
IfcCartesianTransformationOperator3D::~IfcCartesianTransformationOperator3D() {}
shared_ptr<IfcPPObject> IfcCartesianTransformationOperator3D::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCartesianTransformationOperator3D> copy_self( new IfcCartesianTransformationOperator3D() );
	if( m_Axis1 ) { copy_self->m_Axis1 = dynamic_pointer_cast<IfcDirection>( m_Axis1->getDeepCopy(options) ); }
	if( m_Axis2 ) { copy_self->m_Axis2 = dynamic_pointer_cast<IfcDirection>( m_Axis2->getDeepCopy(options) ); }
	if( m_LocalOrigin ) { copy_self->m_LocalOrigin = dynamic_pointer_cast<IfcCartesianPoint>( m_LocalOrigin->getDeepCopy(options) ); }
	if( m_Scale ) { copy_self->m_Scale = dynamic_pointer_cast<IfcReal>( m_Scale->getDeepCopy(options) ); }
	if( m_Axis3 ) { copy_self->m_Axis3 = dynamic_pointer_cast<IfcDirection>( m_Axis3->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCartesianTransformationOperator3D::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCARTESIANTRANSFORMATIONOPERATOR3D" << "(";
	if( m_Axis1 ) { stream << "#" << m_Axis1->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Axis2 ) { stream << "#" << m_Axis2->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_LocalOrigin ) { stream << "#" << m_LocalOrigin->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Scale ) { m_Scale->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Axis3 ) { stream << "#" << m_Axis3->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcCartesianTransformationOperator3D::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCartesianTransformationOperator3D::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCartesianTransformationOperator3D, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Axis1, map );
	readEntityReference( args[1], m_Axis2, map );
	readEntityReference( args[2], m_LocalOrigin, map );
	m_Scale = IfcReal::createObjectFromSTEP( args[3] );
	readEntityReference( args[4], m_Axis3, map );
}
void IfcCartesianTransformationOperator3D::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCartesianTransformationOperator::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Axis3", m_Axis3 ) );
}
void IfcCartesianTransformationOperator3D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCartesianTransformationOperator::getAttributesInverse( vec_attributes_inverse );
}
void IfcCartesianTransformationOperator3D::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCartesianTransformationOperator::setInverseCounterparts( ptr_self_entity );
}
void IfcCartesianTransformationOperator3D::unlinkFromInverseCounterparts()
{
	IfcCartesianTransformationOperator::unlinkFromInverseCounterparts();
}

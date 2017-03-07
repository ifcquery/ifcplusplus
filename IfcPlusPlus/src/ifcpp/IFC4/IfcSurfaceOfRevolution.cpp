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
#include "include/IfcAxis1Placement.h"
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProfileDef.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurfaceOfRevolution.h"

// ENTITY IfcSurfaceOfRevolution 
IfcSurfaceOfRevolution::IfcSurfaceOfRevolution() { m_entity_enum = IFCSURFACEOFREVOLUTION; }
IfcSurfaceOfRevolution::IfcSurfaceOfRevolution( int id ) { m_id = id; m_entity_enum = IFCSURFACEOFREVOLUTION; }
IfcSurfaceOfRevolution::~IfcSurfaceOfRevolution() {}
shared_ptr<IfcPPObject> IfcSurfaceOfRevolution::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceOfRevolution> copy_self( new IfcSurfaceOfRevolution() );
	if( m_SweptCurve )
	{
		if( options.shallow_copy_IfcProfileDef ) { copy_self->m_SweptCurve = m_SweptCurve; }
		else { copy_self->m_SweptCurve = dynamic_pointer_cast<IfcProfileDef>( m_SweptCurve->getDeepCopy(options) ); }
	}
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_AxisPosition ) { copy_self->m_AxisPosition = dynamic_pointer_cast<IfcAxis1Placement>( m_AxisPosition->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSurfaceOfRevolution::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSURFACEOFREVOLUTION" << "(";
	if( m_SweptCurve ) { stream << "#" << m_SweptCurve->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_AxisPosition ) { stream << "#" << m_AxisPosition->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceOfRevolution::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceOfRevolution::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSurfaceOfRevolution, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_SweptCurve, map );
	readEntityReference( args[1], m_Position, map );
	readEntityReference( args[2], m_AxisPosition, map );
}
void IfcSurfaceOfRevolution::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSweptSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "AxisPosition", m_AxisPosition ) );
}
void IfcSurfaceOfRevolution::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSweptSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceOfRevolution::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSweptSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceOfRevolution::unlinkFromInverseCounterparts()
{
	IfcSweptSurface::unlinkFromInverseCounterparts();
}

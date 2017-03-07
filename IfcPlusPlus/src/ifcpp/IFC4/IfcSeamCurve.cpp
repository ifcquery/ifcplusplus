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
#include "include/IfcPcurve.h"
#include "include/IfcPreferredSurfaceCurveRepresentation.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcSeamCurve.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcSeamCurve 
IfcSeamCurve::IfcSeamCurve() { m_entity_enum = IFCSEAMCURVE; }
IfcSeamCurve::IfcSeamCurve( int id ) { m_id = id; m_entity_enum = IFCSEAMCURVE; }
IfcSeamCurve::~IfcSeamCurve() {}
shared_ptr<IfcPPObject> IfcSeamCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSeamCurve> copy_self( new IfcSeamCurve() );
	if( m_Curve3D ) { copy_self->m_Curve3D = dynamic_pointer_cast<IfcCurve>( m_Curve3D->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_AssociatedGeometry.size(); ++ii )
	{
		auto item_ii = m_AssociatedGeometry[ii];
		if( item_ii )
		{
			copy_self->m_AssociatedGeometry.push_back( dynamic_pointer_cast<IfcPcurve>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_MasterRepresentation ) { copy_self->m_MasterRepresentation = dynamic_pointer_cast<IfcPreferredSurfaceCurveRepresentation>( m_MasterRepresentation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSeamCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSEAMCURVE" << "(";
	if( m_Curve3D ) { stream << "#" << m_Curve3D->m_id; } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_AssociatedGeometry );
	stream << ",";
	if( m_MasterRepresentation ) { m_MasterRepresentation->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcSeamCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSeamCurve::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSeamCurve, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Curve3D, map );
	readEntityReferenceList( args[1], m_AssociatedGeometry, map );
	m_MasterRepresentation = IfcPreferredSurfaceCurveRepresentation::createObjectFromSTEP( args[2] );
}
void IfcSeamCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSurfaceCurve::getAttributes( vec_attributes );
}
void IfcSeamCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSurfaceCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcSeamCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSurfaceCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcSeamCurve::unlinkFromInverseCounterparts()
{
	IfcSurfaceCurve::unlinkFromInverseCounterparts();
}

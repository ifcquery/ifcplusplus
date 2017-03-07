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
#include "include/IfcGeometricCurveSet.h"
#include "include/IfcGeometricSetSelect.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcGeometricCurveSet 
IfcGeometricCurveSet::IfcGeometricCurveSet() { m_entity_enum = IFCGEOMETRICCURVESET; }
IfcGeometricCurveSet::IfcGeometricCurveSet( int id ) { m_id = id; m_entity_enum = IFCGEOMETRICCURVESET; }
IfcGeometricCurveSet::~IfcGeometricCurveSet() {}
shared_ptr<IfcPPObject> IfcGeometricCurveSet::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcGeometricCurveSet> copy_self( new IfcGeometricCurveSet() );
	for( size_t ii=0; ii<m_Elements.size(); ++ii )
	{
		auto item_ii = m_Elements[ii];
		if( item_ii )
		{
			copy_self->m_Elements.push_back( dynamic_pointer_cast<IfcGeometricSetSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcGeometricCurveSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCGEOMETRICCURVESET" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_Elements.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcGeometricSetSelect>& type_object = m_Elements[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ");";
}
void IfcGeometricCurveSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGeometricCurveSet::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcGeometricCurveSet, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readSelectList( args[0], m_Elements, map );
}
void IfcGeometricCurveSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricSet::getAttributes( vec_attributes );
}
void IfcGeometricCurveSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricSet::getAttributesInverse( vec_attributes_inverse );
}
void IfcGeometricCurveSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricSet::setInverseCounterparts( ptr_self_entity );
}
void IfcGeometricCurveSet::unlinkFromInverseCounterparts()
{
	IfcGeometricSet::unlinkFromInverseCounterparts();
}

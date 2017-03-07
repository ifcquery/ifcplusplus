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
#include "include/IfcBoolean.h"
#include "include/IfcCurve.h"
#include "include/IfcGrid.h"
#include "include/IfcGridAxis.h"
#include "include/IfcLabel.h"
#include "include/IfcVirtualGridIntersection.h"

// ENTITY IfcGridAxis 
IfcGridAxis::IfcGridAxis() { m_entity_enum = IFCGRIDAXIS; }
IfcGridAxis::IfcGridAxis( int id ) { m_id = id; m_entity_enum = IFCGRIDAXIS; }
IfcGridAxis::~IfcGridAxis() {}
shared_ptr<IfcPPObject> IfcGridAxis::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcGridAxis> copy_self( new IfcGridAxis() );
	if( m_AxisTag ) { copy_self->m_AxisTag = dynamic_pointer_cast<IfcLabel>( m_AxisTag->getDeepCopy(options) ); }
	if( m_AxisCurve ) { copy_self->m_AxisCurve = dynamic_pointer_cast<IfcCurve>( m_AxisCurve->getDeepCopy(options) ); }
	if( m_SameSense ) { copy_self->m_SameSense = dynamic_pointer_cast<IfcBoolean>( m_SameSense->getDeepCopy(options) ); }
	return copy_self;
}
void IfcGridAxis::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCGRIDAXIS" << "(";
	if( m_AxisTag ) { m_AxisTag->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_AxisCurve ) { stream << "#" << m_AxisCurve->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_SameSense ) { m_SameSense->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcGridAxis::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGridAxis::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcGridAxis, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_AxisTag = IfcLabel::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_AxisCurve, map );
	m_SameSense = IfcBoolean::createObjectFromSTEP( args[2] );
}
void IfcGridAxis::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "AxisTag", m_AxisTag ) );
	vec_attributes.push_back( std::make_pair( "AxisCurve", m_AxisCurve ) );
	vec_attributes.push_back( std::make_pair( "SameSense", m_SameSense ) );
}
void IfcGridAxis::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_PartOfW_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PartOfW_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PartOfW_inverse.size(); ++i )
		{
			if( !m_PartOfW_inverse[i].expired() )
			{
				PartOfW_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcGrid>( m_PartOfW_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PartOfW_inverse", PartOfW_inverse_vec_obj ) );
	}
	if( m_PartOfV_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PartOfV_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PartOfV_inverse.size(); ++i )
		{
			if( !m_PartOfV_inverse[i].expired() )
			{
				PartOfV_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcGrid>( m_PartOfV_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PartOfV_inverse", PartOfV_inverse_vec_obj ) );
	}
	if( m_PartOfU_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PartOfU_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PartOfU_inverse.size(); ++i )
		{
			if( !m_PartOfU_inverse[i].expired() )
			{
				PartOfU_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcGrid>( m_PartOfU_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PartOfU_inverse", PartOfU_inverse_vec_obj ) );
	}
	if( m_HasIntersections_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasIntersections_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasIntersections_inverse.size(); ++i )
		{
			if( !m_HasIntersections_inverse[i].expired() )
			{
				HasIntersections_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcVirtualGridIntersection>( m_HasIntersections_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasIntersections_inverse", HasIntersections_inverse_vec_obj ) );
	}
}
void IfcGridAxis::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcGridAxis::unlinkFromInverseCounterparts()
{
}

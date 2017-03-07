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
#include "include/IfcColourRgbList.h"
#include "include/IfcNormalisedRatioMeasure.h"

// ENTITY IfcColourRgbList 
IfcColourRgbList::IfcColourRgbList() { m_entity_enum = IFCCOLOURRGBLIST; }
IfcColourRgbList::IfcColourRgbList( int id ) { m_id = id; m_entity_enum = IFCCOLOURRGBLIST; }
IfcColourRgbList::~IfcColourRgbList() {}
shared_ptr<IfcPPObject> IfcColourRgbList::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcColourRgbList> copy_self( new IfcColourRgbList() );
	copy_self->m_ColourList.resize( m_ColourList.size() );
	for( size_t ii=0; ii<m_ColourList.size(); ++ii )
	{
		std::vector<shared_ptr<IfcNormalisedRatioMeasure> >& vec_ii = m_ColourList[ii];
		std::vector<shared_ptr<IfcNormalisedRatioMeasure> >& vec_ii_target = copy_self->m_ColourList[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcNormalisedRatioMeasure>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcNormalisedRatioMeasure>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	return copy_self;
}
void IfcColourRgbList::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOLOURRGBLIST" << "(";
	stream << "("; 
	for( size_t ii = 0; ii < m_ColourList.size(); ++ii )
	{
		const std::vector<shared_ptr<IfcNormalisedRatioMeasure> >&inner_vec = m_ColourList[ii];
		if( ii > 0 )
		{
			stream << "), (";
		}
		for( size_t jj = 0; jj < inner_vec.size(); ++jj )
		{
			if( jj > 0 )
			{
				stream << ", ";
			}
			const shared_ptr<IfcNormalisedRatioMeasure>& type_object = inner_vec[jj];
			if( type_object )
			{
				type_object->getStepParameter( stream, false );
			}
			else
			{
				stream << "$";
			}
		}
	}
	stream << ")"; 
	stream << ");";
}
void IfcColourRgbList::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcColourRgbList::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcColourRgbList, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList2D( args[0], m_ColourList, map );
}
void IfcColourRgbList::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
}
void IfcColourRgbList::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcColourRgbList::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcColourRgbList::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}

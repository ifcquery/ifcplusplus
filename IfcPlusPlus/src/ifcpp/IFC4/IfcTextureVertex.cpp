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
#include "include/IfcParameterValue.h"
#include "include/IfcTextureVertex.h"

// ENTITY IfcTextureVertex 
IfcTextureVertex::IfcTextureVertex() { m_entity_enum = IFCTEXTUREVERTEX; }
IfcTextureVertex::IfcTextureVertex( int id ) { m_id = id; m_entity_enum = IFCTEXTUREVERTEX; }
IfcTextureVertex::~IfcTextureVertex() {}
shared_ptr<IfcPPObject> IfcTextureVertex::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextureVertex> copy_self( new IfcTextureVertex() );
	for( size_t ii=0; ii<m_Coordinates.size(); ++ii )
	{
		auto item_ii = m_Coordinates[ii];
		if( item_ii )
		{
			copy_self->m_Coordinates.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTextureVertex::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTUREVERTEX" << "(";
	writeNumericTypeList( stream, m_Coordinates );
	stream << ");";
}
void IfcTextureVertex::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextureVertex::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTextureVertex, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readTypeOfRealList( args[0], m_Coordinates );
}
void IfcTextureVertex::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	if( m_Coordinates.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Coordinates_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Coordinates.begin(), m_Coordinates.end(), std::back_inserter( Coordinates_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Coordinates", Coordinates_vec_object ) );
	}
}
void IfcTextureVertex::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextureVertex::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTextureVertex::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}

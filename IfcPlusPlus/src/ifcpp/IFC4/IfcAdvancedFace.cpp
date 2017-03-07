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
#include "include/IfcAdvancedFace.h"
#include "include/IfcBoolean.h"
#include "include/IfcFaceBound.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"
#include "include/IfcTextureMap.h"

// ENTITY IfcAdvancedFace 
IfcAdvancedFace::IfcAdvancedFace() { m_entity_enum = IFCADVANCEDFACE; }
IfcAdvancedFace::IfcAdvancedFace( int id ) { m_id = id; m_entity_enum = IFCADVANCEDFACE; }
IfcAdvancedFace::~IfcAdvancedFace() {}
shared_ptr<IfcPPObject> IfcAdvancedFace::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAdvancedFace> copy_self( new IfcAdvancedFace() );
	for( size_t ii=0; ii<m_Bounds.size(); ++ii )
	{
		auto item_ii = m_Bounds[ii];
		if( item_ii )
		{
			copy_self->m_Bounds.push_back( dynamic_pointer_cast<IfcFaceBound>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_FaceSurface ) { copy_self->m_FaceSurface = dynamic_pointer_cast<IfcSurface>( m_FaceSurface->getDeepCopy(options) ); }
	if( m_SameSense ) { copy_self->m_SameSense = dynamic_pointer_cast<IfcBoolean>( m_SameSense->getDeepCopy(options) ); }
	return copy_self;
}
void IfcAdvancedFace::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCADVANCEDFACE" << "(";
	writeEntityList( stream, m_Bounds );
	stream << ",";
	if( m_FaceSurface ) { stream << "#" << m_FaceSurface->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_SameSense ) { m_SameSense->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcAdvancedFace::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAdvancedFace::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcAdvancedFace, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_Bounds, map );
	readEntityReference( args[1], m_FaceSurface, map );
	m_SameSense = IfcBoolean::createObjectFromSTEP( args[2] );
}
void IfcAdvancedFace::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFaceSurface::getAttributes( vec_attributes );
}
void IfcAdvancedFace::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcFaceSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcAdvancedFace::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFaceSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcAdvancedFace::unlinkFromInverseCounterparts()
{
	IfcFaceSurface::unlinkFromInverseCounterparts();
}

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
#include "include/IfcAdvancedBrepWithVoids.h"
#include "include/IfcClosedShell.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcAdvancedBrepWithVoids 
IfcAdvancedBrepWithVoids::IfcAdvancedBrepWithVoids() { m_entity_enum = IFCADVANCEDBREPWITHVOIDS; }
IfcAdvancedBrepWithVoids::IfcAdvancedBrepWithVoids( int id ) { m_id = id; m_entity_enum = IFCADVANCEDBREPWITHVOIDS; }
IfcAdvancedBrepWithVoids::~IfcAdvancedBrepWithVoids() {}
shared_ptr<IfcPPObject> IfcAdvancedBrepWithVoids::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAdvancedBrepWithVoids> copy_self( new IfcAdvancedBrepWithVoids() );
	if( m_Outer ) { copy_self->m_Outer = dynamic_pointer_cast<IfcClosedShell>( m_Outer->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Voids.size(); ++ii )
	{
		auto item_ii = m_Voids[ii];
		if( item_ii )
		{
			copy_self->m_Voids.push_back( dynamic_pointer_cast<IfcClosedShell>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcAdvancedBrepWithVoids::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCADVANCEDBREPWITHVOIDS" << "(";
	if( m_Outer ) { stream << "#" << m_Outer->m_id; } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Voids );
	stream << ");";
}
void IfcAdvancedBrepWithVoids::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAdvancedBrepWithVoids::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcAdvancedBrepWithVoids, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Outer, map );
	readEntityReferenceList( args[1], m_Voids, map );
}
void IfcAdvancedBrepWithVoids::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcAdvancedBrep::getAttributes( vec_attributes );
	if( m_Voids.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Voids_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Voids.begin(), m_Voids.end(), std::back_inserter( Voids_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Voids", Voids_vec_object ) );
	}
}
void IfcAdvancedBrepWithVoids::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcAdvancedBrep::getAttributesInverse( vec_attributes_inverse );
}
void IfcAdvancedBrepWithVoids::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcAdvancedBrep::setInverseCounterparts( ptr_self_entity );
}
void IfcAdvancedBrepWithVoids::unlinkFromInverseCounterparts()
{
	IfcAdvancedBrep::unlinkFromInverseCounterparts();
}

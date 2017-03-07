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
#include "include/IfcCoordinateOperation.h"
#include "include/IfcCoordinateReferenceSystem.h"
#include "include/IfcCoordinateReferenceSystemSelect.h"
#include "include/IfcGeometricRepresentationContext.h"

// ENTITY IfcCoordinateOperation 
IfcCoordinateOperation::IfcCoordinateOperation() { m_entity_enum = IFCCOORDINATEOPERATION; }
IfcCoordinateOperation::IfcCoordinateOperation( int id ) { m_id = id; m_entity_enum = IFCCOORDINATEOPERATION; }
IfcCoordinateOperation::~IfcCoordinateOperation() {}
shared_ptr<IfcPPObject> IfcCoordinateOperation::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCoordinateOperation> copy_self( new IfcCoordinateOperation() );
	if( m_SourceCRS ) { copy_self->m_SourceCRS = dynamic_pointer_cast<IfcCoordinateReferenceSystemSelect>( m_SourceCRS->getDeepCopy(options) ); }
	if( m_TargetCRS ) { copy_self->m_TargetCRS = dynamic_pointer_cast<IfcCoordinateReferenceSystem>( m_TargetCRS->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCoordinateOperation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOORDINATEOPERATION" << "(";
	if( m_SourceCRS ) { m_SourceCRS->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TargetCRS ) { stream << "#" << m_TargetCRS->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcCoordinateOperation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCoordinateOperation::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCoordinateOperation, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_SourceCRS = IfcCoordinateReferenceSystemSelect::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_TargetCRS, map );
}
void IfcCoordinateOperation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "SourceCRS", m_SourceCRS ) );
	vec_attributes.push_back( std::make_pair( "TargetCRS", m_TargetCRS ) );
}
void IfcCoordinateOperation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcCoordinateOperation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcCoordinateOperation> ptr_self = dynamic_pointer_cast<IfcCoordinateOperation>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcCoordinateOperation::setInverseCounterparts: type mismatch" ); }
	shared_ptr<IfcCoordinateReferenceSystem>  SourceCRS_IfcCoordinateReferenceSystem = dynamic_pointer_cast<IfcCoordinateReferenceSystem>( m_SourceCRS );
	if( SourceCRS_IfcCoordinateReferenceSystem )
	{
		SourceCRS_IfcCoordinateReferenceSystem->m_HasCoordinateOperation_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcGeometricRepresentationContext>  SourceCRS_IfcGeometricRepresentationContext = dynamic_pointer_cast<IfcGeometricRepresentationContext>( m_SourceCRS );
	if( SourceCRS_IfcGeometricRepresentationContext )
	{
		SourceCRS_IfcGeometricRepresentationContext->m_HasCoordinateOperation_inverse.push_back( ptr_self );
	}
}
void IfcCoordinateOperation::unlinkFromInverseCounterparts()
{
	shared_ptr<IfcCoordinateReferenceSystem>  SourceCRS_IfcCoordinateReferenceSystem = dynamic_pointer_cast<IfcCoordinateReferenceSystem>( m_SourceCRS );
	if( SourceCRS_IfcCoordinateReferenceSystem )
	{
		std::vector<weak_ptr<IfcCoordinateOperation> >& HasCoordinateOperation_inverse = SourceCRS_IfcCoordinateReferenceSystem->m_HasCoordinateOperation_inverse;
		for( auto it_HasCoordinateOperation_inverse = HasCoordinateOperation_inverse.begin(); it_HasCoordinateOperation_inverse != HasCoordinateOperation_inverse.end(); )
		{
			shared_ptr<IfcCoordinateOperation> self_candidate( *it_HasCoordinateOperation_inverse );
			if( self_candidate.get() == this )
			{
				it_HasCoordinateOperation_inverse= HasCoordinateOperation_inverse.erase( it_HasCoordinateOperation_inverse );
			}
			else
			{
				++it_HasCoordinateOperation_inverse;
			}
		}
	}
	shared_ptr<IfcGeometricRepresentationContext>  SourceCRS_IfcGeometricRepresentationContext = dynamic_pointer_cast<IfcGeometricRepresentationContext>( m_SourceCRS );
	if( SourceCRS_IfcGeometricRepresentationContext )
	{
		std::vector<weak_ptr<IfcCoordinateOperation> >& HasCoordinateOperation_inverse = SourceCRS_IfcGeometricRepresentationContext->m_HasCoordinateOperation_inverse;
		for( auto it_HasCoordinateOperation_inverse = HasCoordinateOperation_inverse.begin(); it_HasCoordinateOperation_inverse != HasCoordinateOperation_inverse.end(); )
		{
			shared_ptr<IfcCoordinateOperation> self_candidate( *it_HasCoordinateOperation_inverse );
			if( self_candidate.get() == this )
			{
				it_HasCoordinateOperation_inverse= HasCoordinateOperation_inverse.erase( it_HasCoordinateOperation_inverse );
			}
			else
			{
				++it_HasCoordinateOperation_inverse;
			}
		}
	}
}

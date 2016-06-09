/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
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
void IfcCoordinateOperation::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
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

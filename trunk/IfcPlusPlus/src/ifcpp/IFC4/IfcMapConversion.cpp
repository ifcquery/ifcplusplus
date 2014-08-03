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
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcCoordinateReferenceSystem.h"
#include "include/IfcCoordinateReferenceSystemSelect.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcMapConversion.h"
#include "include/IfcReal.h"

// ENTITY IfcMapConversion 
IfcMapConversion::IfcMapConversion() {}
IfcMapConversion::IfcMapConversion( int id ) { m_id = id; }
IfcMapConversion::~IfcMapConversion() {}
shared_ptr<IfcPPObject> IfcMapConversion::getDeepCopy()
{
	shared_ptr<IfcMapConversion> copy_self( new IfcMapConversion() );
	if( m_SourceCRS ) { copy_self->m_SourceCRS = dynamic_pointer_cast<IfcCoordinateReferenceSystemSelect>( m_SourceCRS->getDeepCopy() ); }
	if( m_TargetCRS ) { copy_self->m_TargetCRS = dynamic_pointer_cast<IfcCoordinateReferenceSystem>( m_TargetCRS->getDeepCopy() ); }
	if( m_Eastings ) { copy_self->m_Eastings = dynamic_pointer_cast<IfcLengthMeasure>( m_Eastings->getDeepCopy() ); }
	if( m_Northings ) { copy_self->m_Northings = dynamic_pointer_cast<IfcLengthMeasure>( m_Northings->getDeepCopy() ); }
	if( m_OrthogonalHeight ) { copy_self->m_OrthogonalHeight = dynamic_pointer_cast<IfcLengthMeasure>( m_OrthogonalHeight->getDeepCopy() ); }
	if( m_XAxisAbscissa ) { copy_self->m_XAxisAbscissa = dynamic_pointer_cast<IfcReal>( m_XAxisAbscissa->getDeepCopy() ); }
	if( m_XAxisOrdinate ) { copy_self->m_XAxisOrdinate = dynamic_pointer_cast<IfcReal>( m_XAxisOrdinate->getDeepCopy() ); }
	if( m_Scale ) { copy_self->m_Scale = dynamic_pointer_cast<IfcReal>( m_Scale->getDeepCopy() ); }
	return copy_self;
}
void IfcMapConversion::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMAPCONVERSION" << "(";
	if( m_SourceCRS ) { m_SourceCRS->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_TargetCRS ) { stream << "#" << m_TargetCRS->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_Eastings ) { m_Eastings->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Northings ) { m_Northings->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OrthogonalHeight ) { m_OrthogonalHeight->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_XAxisAbscissa ) { m_XAxisAbscissa->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_XAxisOrdinate ) { m_XAxisOrdinate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Scale ) { m_Scale->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMapConversion::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMapConversion::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMapConversion, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcMapConversion, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_SourceCRS = IfcCoordinateReferenceSystemSelect::createObjectFromStepData( args[0], map );
	readEntityReference( args[1], m_TargetCRS, map );
	m_Eastings = IfcLengthMeasure::createObjectFromStepData( args[2] );
	m_Northings = IfcLengthMeasure::createObjectFromStepData( args[3] );
	m_OrthogonalHeight = IfcLengthMeasure::createObjectFromStepData( args[4] );
	m_XAxisAbscissa = IfcReal::createObjectFromStepData( args[5] );
	m_XAxisOrdinate = IfcReal::createObjectFromStepData( args[6] );
	m_Scale = IfcReal::createObjectFromStepData( args[7] );
}
void IfcMapConversion::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCoordinateOperation::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Eastings", m_Eastings ) );
	vec_attributes.push_back( std::make_pair( "Northings", m_Northings ) );
	vec_attributes.push_back( std::make_pair( "OrthogonalHeight", m_OrthogonalHeight ) );
	vec_attributes.push_back( std::make_pair( "XAxisAbscissa", m_XAxisAbscissa ) );
	vec_attributes.push_back( std::make_pair( "XAxisOrdinate", m_XAxisOrdinate ) );
	vec_attributes.push_back( std::make_pair( "Scale", m_Scale ) );
}
void IfcMapConversion::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCoordinateOperation::getAttributesInverse( vec_attributes_inverse );
}
void IfcMapConversion::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCoordinateOperation::setInverseCounterparts( ptr_self_entity );
}
void IfcMapConversion::unlinkSelf()
{
	IfcCoordinateOperation::unlinkSelf();
}

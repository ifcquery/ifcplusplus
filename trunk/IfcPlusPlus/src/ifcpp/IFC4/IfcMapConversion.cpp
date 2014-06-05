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

// method setEntity takes over all attributes from another instance of the class
void IfcMapConversion::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMapConversion> other = dynamic_pointer_cast<IfcMapConversion>(other_entity);
	if( !other) { return; }
	m_SourceCRS = other->m_SourceCRS;
	m_TargetCRS = other->m_TargetCRS;
	m_Eastings = other->m_Eastings;
	m_Northings = other->m_Northings;
	m_OrthogonalHeight = other->m_OrthogonalHeight;
	m_XAxisAbscissa = other->m_XAxisAbscissa;
	m_XAxisOrdinate = other->m_XAxisOrdinate;
	m_Scale = other->m_Scale;
}
void IfcMapConversion::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMAPCONVERSION" << "(";
	if( m_SourceCRS ) { m_SourceCRS->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TargetCRS ) { stream << "#" << m_TargetCRS->getId(); } else { stream << "$"; }
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
void IfcMapConversion::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
}
void IfcMapConversion::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCoordinateOperation::setInverseCounterparts( ptr_self_entity );
}
void IfcMapConversion::unlinkSelf()
{
	IfcCoordinateOperation::unlinkSelf();
}

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
#include "include/IfcCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTrimmedCurve.h"
#include "include/IfcTrimmingPreference.h"
#include "include/IfcTrimmingSelect.h"

// ENTITY IfcTrimmedCurve 
IfcTrimmedCurve::IfcTrimmedCurve() {}
IfcTrimmedCurve::IfcTrimmedCurve( int id ) { m_id = id; }
IfcTrimmedCurve::~IfcTrimmedCurve() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTrimmedCurve::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTrimmedCurve> other = dynamic_pointer_cast<IfcTrimmedCurve>(other_entity);
	if( !other) { return; }
	m_BasisCurve = other->m_BasisCurve;
	m_Trim1 = other->m_Trim1;
	m_Trim2 = other->m_Trim2;
	m_SenseAgreement = other->m_SenseAgreement;
	m_MasterRepresentation = other->m_MasterRepresentation;
}
void IfcTrimmedCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTRIMMEDCURVE" << "(";
	if( m_BasisCurve ) { stream << "#" << m_BasisCurve->getId(); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_Trim1, true );
	stream << ",";
	writeTypeList( stream, m_Trim2, true );
	stream << ",";
	if( m_SenseAgreement == false ) { stream << ".F."; }
	else if( m_SenseAgreement == true ) { stream << ".T."; }
	stream << ",";
	if( m_MasterRepresentation ) { m_MasterRepresentation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTrimmedCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTrimmedCurve::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTrimmedCurve, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcTrimmedCurve, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BasisCurve, map );
	readSelectList( args[1], m_Trim1, map );
	readSelectList( args[2], m_Trim2, map );
	if( _stricmp( args[3].c_str(), ".F." ) == 0 ) { m_SenseAgreement = false; }
	else if( _stricmp( args[3].c_str(), ".T." ) == 0 ) { m_SenseAgreement = true; }
	m_MasterRepresentation = IfcTrimmingPreference::createObjectFromStepData( args[4] );
}
void IfcTrimmedCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisCurve", m_BasisCurve ) );
	shared_ptr<IfcPPAttributeObjectVector> Trim1_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Trim1.begin(), m_Trim1.end(), std::back_inserter( Trim1_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Trim1", Trim1_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> Trim2_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Trim2.begin(), m_Trim2.end(), std::back_inserter( Trim2_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Trim2", Trim2_vec_object ) );
	vec_attributes.push_back( std::make_pair( "SenseAgreement", shared_ptr<IfcPPBool>( new IfcPPBool( m_SenseAgreement ) ) ) );
	vec_attributes.push_back( std::make_pair( "MasterRepresentation", m_MasterRepresentation ) );
}
void IfcTrimmedCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTrimmedCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcTrimmedCurve::unlinkSelf()
{
	IfcBoundedCurve::unlinkSelf();
}

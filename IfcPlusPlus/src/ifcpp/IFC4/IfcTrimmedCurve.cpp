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
shared_ptr<IfcPPObject> IfcTrimmedCurve::getDeepCopy()
{
	shared_ptr<IfcTrimmedCurve> copy_self( new IfcTrimmedCurve() );
	if( m_BasisCurve ) { copy_self->m_BasisCurve = dynamic_pointer_cast<IfcCurve>( m_BasisCurve->getDeepCopy() ); }
	for( size_t ii=0; ii<m_Trim1.size(); ++ii )
	{
		auto item_ii = m_Trim1[ii];
		if( item_ii )
		{
			copy_self->m_Trim1.push_back( dynamic_pointer_cast<IfcTrimmingSelect>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_Trim2.size(); ++ii )
	{
		auto item_ii = m_Trim2[ii];
		if( item_ii )
		{
			copy_self->m_Trim2.push_back( dynamic_pointer_cast<IfcTrimmingSelect>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_SenseAgreement ) { copy_self->m_SenseAgreement = m_SenseAgreement; }
	if( m_MasterRepresentation ) { copy_self->m_MasterRepresentation = dynamic_pointer_cast<IfcTrimmingPreference>( m_MasterRepresentation->getDeepCopy() ); }
	return copy_self;
}
void IfcTrimmedCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTRIMMEDCURVE" << "(";
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
void IfcTrimmedCurve::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTrimmedCurve, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcTrimmedCurve, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BasisCurve, map );
	readSelectList( args[1], m_Trim1, map );
	readSelectList( args[2], m_Trim2, map );
	if( boost::iequals( args[3], L".F." ) ) { m_SenseAgreement = false; }
	else if( boost::iequals( args[3], L".T." ) ) { m_SenseAgreement = true; }
	m_MasterRepresentation = IfcTrimmingPreference::createObjectFromStepData( args[4] );
}
void IfcTrimmedCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisCurve", m_BasisCurve ) );
	if( m_Trim1.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Trim1_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Trim1.begin(), m_Trim1.end(), std::back_inserter( Trim1_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Trim1", Trim1_vec_object ) );
	}
	if( m_Trim2.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Trim2_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Trim2.begin(), m_Trim2.end(), std::back_inserter( Trim2_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Trim2", Trim2_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "SenseAgreement", shared_ptr<IfcPPBool>( new IfcPPBool( m_SenseAgreement ) ) ) );
	vec_attributes.push_back( std::make_pair( "MasterRepresentation", m_MasterRepresentation ) );
}
void IfcTrimmedCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcTrimmedCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcTrimmedCurve::unlinkSelf()
{
	IfcBoundedCurve::unlinkSelf();
}

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
#include "include/IfcArbitraryProfileDefWithVoids.h"
#include "include/IfcCurve.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcArbitraryProfileDefWithVoids 
IfcArbitraryProfileDefWithVoids::IfcArbitraryProfileDefWithVoids() {}
IfcArbitraryProfileDefWithVoids::IfcArbitraryProfileDefWithVoids( int id ) { m_id = id; }
IfcArbitraryProfileDefWithVoids::~IfcArbitraryProfileDefWithVoids() {}
shared_ptr<IfcPPObject> IfcArbitraryProfileDefWithVoids::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcArbitraryProfileDefWithVoids> copy_self( new IfcArbitraryProfileDefWithVoids() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_OuterCurve ) { copy_self->m_OuterCurve = dynamic_pointer_cast<IfcCurve>( m_OuterCurve->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_InnerCurves.size(); ++ii )
	{
		auto item_ii = m_InnerCurves[ii];
		if( item_ii )
		{
			copy_self->m_InnerCurves.push_back( dynamic_pointer_cast<IfcCurve>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcArbitraryProfileDefWithVoids::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCARBITRARYPROFILEDEFWITHVOIDS" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OuterCurve ) { stream << "#" << m_OuterCurve->m_id; } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_InnerCurves );
	stream << ");";
}
void IfcArbitraryProfileDefWithVoids::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcArbitraryProfileDefWithVoids::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcArbitraryProfileDefWithVoids, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_OuterCurve, map );
	readEntityReferenceList( args[3], m_InnerCurves, map );
}
void IfcArbitraryProfileDefWithVoids::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcArbitraryClosedProfileDef::getAttributes( vec_attributes );
	if( m_InnerCurves.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> InnerCurves_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_InnerCurves.begin(), m_InnerCurves.end(), std::back_inserter( InnerCurves_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "InnerCurves", InnerCurves_vec_object ) );
	}
}
void IfcArbitraryProfileDefWithVoids::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcArbitraryClosedProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcArbitraryProfileDefWithVoids::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcArbitraryClosedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcArbitraryProfileDefWithVoids::unlinkSelf()
{
	IfcArbitraryClosedProfileDef::unlinkSelf();
}

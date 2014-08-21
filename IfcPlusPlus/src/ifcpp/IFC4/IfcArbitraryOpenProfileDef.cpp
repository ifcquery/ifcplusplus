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
#include "include/IfcArbitraryOpenProfileDef.h"
#include "include/IfcBoundedCurve.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcArbitraryOpenProfileDef 
IfcArbitraryOpenProfileDef::IfcArbitraryOpenProfileDef() {}
IfcArbitraryOpenProfileDef::IfcArbitraryOpenProfileDef( int id ) { m_id = id; }
IfcArbitraryOpenProfileDef::~IfcArbitraryOpenProfileDef() {}
shared_ptr<IfcPPObject> IfcArbitraryOpenProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcArbitraryOpenProfileDef> copy_self( new IfcArbitraryOpenProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_Curve ) { copy_self->m_Curve = dynamic_pointer_cast<IfcBoundedCurve>( m_Curve->getDeepCopy(options) ); }
	return copy_self;
}
void IfcArbitraryOpenProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCARBITRARYOPENPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Curve ) { stream << "#" << m_Curve->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcArbitraryOpenProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcArbitraryOpenProfileDef::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcArbitraryOpenProfileDef, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Curve, map );
}
void IfcArbitraryOpenProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Curve", m_Curve ) );
}
void IfcArbitraryOpenProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcArbitraryOpenProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcArbitraryOpenProfileDef::unlinkSelf()
{
	IfcProfileDef::unlinkSelf();
}

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
#include "include/IfcLabel.h"
#include "include/IfcStructuralLoadTemperature.h"
#include "include/IfcThermodynamicTemperatureMeasure.h"

// ENTITY IfcStructuralLoadTemperature 
IfcStructuralLoadTemperature::IfcStructuralLoadTemperature() {}
IfcStructuralLoadTemperature::IfcStructuralLoadTemperature( int id ) { m_id = id; }
IfcStructuralLoadTemperature::~IfcStructuralLoadTemperature() {}
shared_ptr<IfcPPObject> IfcStructuralLoadTemperature::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralLoadTemperature> copy_self( new IfcStructuralLoadTemperature() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_DeltaTConstant ) { copy_self->m_DeltaTConstant = dynamic_pointer_cast<IfcThermodynamicTemperatureMeasure>( m_DeltaTConstant->getDeepCopy(options) ); }
	if( m_DeltaTY ) { copy_self->m_DeltaTY = dynamic_pointer_cast<IfcThermodynamicTemperatureMeasure>( m_DeltaTY->getDeepCopy(options) ); }
	if( m_DeltaTZ ) { copy_self->m_DeltaTZ = dynamic_pointer_cast<IfcThermodynamicTemperatureMeasure>( m_DeltaTZ->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStructuralLoadTemperature::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALLOADTEMPERATURE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DeltaTConstant ) { m_DeltaTConstant->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DeltaTY ) { m_DeltaTY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DeltaTZ ) { m_DeltaTZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralLoadTemperature::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadTemperature::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralLoadTemperature, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_DeltaTConstant = IfcThermodynamicTemperatureMeasure::createObjectFromSTEP( args[1] );
	m_DeltaTY = IfcThermodynamicTemperatureMeasure::createObjectFromSTEP( args[2] );
	m_DeltaTZ = IfcThermodynamicTemperatureMeasure::createObjectFromSTEP( args[3] );
}
void IfcStructuralLoadTemperature::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoadStatic::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "DeltaTConstant", m_DeltaTConstant ) );
	vec_attributes.push_back( std::make_pair( "DeltaTY", m_DeltaTY ) );
	vec_attributes.push_back( std::make_pair( "DeltaTZ", m_DeltaTZ ) );
}
void IfcStructuralLoadTemperature::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralLoadStatic::getAttributesInverse( vec_attributes_inverse );
}
void IfcStructuralLoadTemperature::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoadStatic::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadTemperature::unlinkSelf()
{
	IfcStructuralLoadStatic::unlinkSelf();
}

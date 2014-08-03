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
#include "include/IfcCurvatureMeasure.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcStructuralLoadSingleDisplacementDistortion.h"

// ENTITY IfcStructuralLoadSingleDisplacementDistortion 
IfcStructuralLoadSingleDisplacementDistortion::IfcStructuralLoadSingleDisplacementDistortion() {}
IfcStructuralLoadSingleDisplacementDistortion::IfcStructuralLoadSingleDisplacementDistortion( int id ) { m_id = id; }
IfcStructuralLoadSingleDisplacementDistortion::~IfcStructuralLoadSingleDisplacementDistortion() {}
shared_ptr<IfcPPObject> IfcStructuralLoadSingleDisplacementDistortion::getDeepCopy()
{
	shared_ptr<IfcStructuralLoadSingleDisplacementDistortion> copy_self( new IfcStructuralLoadSingleDisplacementDistortion() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_DisplacementX ) { copy_self->m_DisplacementX = dynamic_pointer_cast<IfcLengthMeasure>( m_DisplacementX->getDeepCopy() ); }
	if( m_DisplacementY ) { copy_self->m_DisplacementY = dynamic_pointer_cast<IfcLengthMeasure>( m_DisplacementY->getDeepCopy() ); }
	if( m_DisplacementZ ) { copy_self->m_DisplacementZ = dynamic_pointer_cast<IfcLengthMeasure>( m_DisplacementZ->getDeepCopy() ); }
	if( m_RotationalDisplacementRX ) { copy_self->m_RotationalDisplacementRX = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_RotationalDisplacementRX->getDeepCopy() ); }
	if( m_RotationalDisplacementRY ) { copy_self->m_RotationalDisplacementRY = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_RotationalDisplacementRY->getDeepCopy() ); }
	if( m_RotationalDisplacementRZ ) { copy_self->m_RotationalDisplacementRZ = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_RotationalDisplacementRZ->getDeepCopy() ); }
	if( m_Distortion ) { copy_self->m_Distortion = dynamic_pointer_cast<IfcCurvatureMeasure>( m_Distortion->getDeepCopy() ); }
	return copy_self;
}
void IfcStructuralLoadSingleDisplacementDistortion::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALLOADSINGLEDISPLACEMENTDISTORTION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DisplacementX ) { m_DisplacementX->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DisplacementY ) { m_DisplacementY->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DisplacementZ ) { m_DisplacementZ->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RotationalDisplacementRX ) { m_RotationalDisplacementRX->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RotationalDisplacementRY ) { m_RotationalDisplacementRY->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RotationalDisplacementRZ ) { m_RotationalDisplacementRZ->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Distortion ) { m_Distortion->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralLoadSingleDisplacementDistortion::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadSingleDisplacementDistortion::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralLoadSingleDisplacementDistortion, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcStructuralLoadSingleDisplacementDistortion, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_DisplacementX = IfcLengthMeasure::createObjectFromStepData( args[1] );
	m_DisplacementY = IfcLengthMeasure::createObjectFromStepData( args[2] );
	m_DisplacementZ = IfcLengthMeasure::createObjectFromStepData( args[3] );
	m_RotationalDisplacementRX = IfcPlaneAngleMeasure::createObjectFromStepData( args[4] );
	m_RotationalDisplacementRY = IfcPlaneAngleMeasure::createObjectFromStepData( args[5] );
	m_RotationalDisplacementRZ = IfcPlaneAngleMeasure::createObjectFromStepData( args[6] );
	m_Distortion = IfcCurvatureMeasure::createObjectFromStepData( args[7] );
}
void IfcStructuralLoadSingleDisplacementDistortion::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoadSingleDisplacement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Distortion", m_Distortion ) );
}
void IfcStructuralLoadSingleDisplacementDistortion::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralLoadSingleDisplacement::getAttributesInverse( vec_attributes_inverse );
}
void IfcStructuralLoadSingleDisplacementDistortion::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoadSingleDisplacement::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadSingleDisplacementDistortion::unlinkSelf()
{
	IfcStructuralLoadSingleDisplacement::unlinkSelf();
}

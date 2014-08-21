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
#include "include/IfcLengthMeasure.h"
#include "include/IfcStructuralLoadConfiguration.h"
#include "include/IfcStructuralLoadOrResult.h"

// ENTITY IfcStructuralLoadConfiguration 
IfcStructuralLoadConfiguration::IfcStructuralLoadConfiguration() {}
IfcStructuralLoadConfiguration::IfcStructuralLoadConfiguration( int id ) { m_id = id; }
IfcStructuralLoadConfiguration::~IfcStructuralLoadConfiguration() {}
shared_ptr<IfcPPObject> IfcStructuralLoadConfiguration::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralLoadConfiguration> copy_self( new IfcStructuralLoadConfiguration() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Values.size(); ++ii )
	{
		auto item_ii = m_Values[ii];
		if( item_ii )
		{
			copy_self->m_Values.push_back( dynamic_pointer_cast<IfcStructuralLoadOrResult>(item_ii->getDeepCopy(options) ) );
		}
	}
	copy_self->m_Locations.resize( m_Locations.size() );
	for( size_t ii=0; ii<m_Locations.size(); ++ii )
	{
		std::vector<shared_ptr<IfcLengthMeasure> >& vec_ii = m_Locations[ii];
		std::vector<shared_ptr<IfcLengthMeasure> >& vec_ii_target = copy_self->m_Locations[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcLengthMeasure>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcLengthMeasure>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	return copy_self;
}
void IfcStructuralLoadConfiguration::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALLOADCONFIGURATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Values );
	stream << ",";
	writeTypeOfRealList2D( stream, m_Locations );
	stream << ");";
}
void IfcStructuralLoadConfiguration::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadConfiguration::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralLoadConfiguration, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	readEntityReferenceList( args[1], m_Values, map );
	readTypeOfRealList2D( args[2], m_Locations );
}
void IfcStructuralLoadConfiguration::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoad::getAttributes( vec_attributes );
	if( m_Values.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Values_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Values.begin(), m_Values.end(), std::back_inserter( Values_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Values", Values_vec_object ) );
	}
}
void IfcStructuralLoadConfiguration::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralLoad::getAttributesInverse( vec_attributes_inverse );
}
void IfcStructuralLoadConfiguration::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoad::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadConfiguration::unlinkSelf()
{
	IfcStructuralLoad::unlinkSelf();
}

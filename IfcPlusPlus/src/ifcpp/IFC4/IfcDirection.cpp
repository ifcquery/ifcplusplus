/* -*-c++-*- IfcPlusPlus - www.ifcquery.com - Copyright (C) 2011 Fabian Gerold
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
#include "include/IfcDirection.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcReal.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcDirection 
IfcDirection::IfcDirection() { m_entity_enum = IFCDIRECTION; }
IfcDirection::IfcDirection( int id ) { m_id = id; m_entity_enum = IFCDIRECTION; }
IfcDirection::~IfcDirection() {}
shared_ptr<IfcPPObject> IfcDirection::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDirection> copy_self( new IfcDirection() );
	for( size_t ii=0; ii<m_DirectionRatios.size(); ++ii )
	{
		auto item_ii = m_DirectionRatios[ii];
		if( item_ii )
		{
			copy_self->m_DirectionRatios.push_back( dynamic_pointer_cast<IfcReal>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcDirection::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDIRECTION" << "(";
	writeNumericTypeList( stream, m_DirectionRatios );
	stream << ");";
}
void IfcDirection::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDirection::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcDirection, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readTypeOfRealList( args[0], m_DirectionRatios );
}
void IfcDirection::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	if( m_DirectionRatios.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DirectionRatios_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_DirectionRatios.begin(), m_DirectionRatios.end(), std::back_inserter( DirectionRatios_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "DirectionRatios", DirectionRatios_vec_object ) );
	}
}
void IfcDirection::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcDirection::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcDirection::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}

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
#include "include/IfcCartesianPointList3D.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcCartesianPointList3D 
IfcCartesianPointList3D::IfcCartesianPointList3D() {}
IfcCartesianPointList3D::IfcCartesianPointList3D( int id ) { m_id = id; }
IfcCartesianPointList3D::~IfcCartesianPointList3D() {}
shared_ptr<IfcPPObject> IfcCartesianPointList3D::getDeepCopy()
{
	shared_ptr<IfcCartesianPointList3D> copy_self( new IfcCartesianPointList3D() );
	copy_self->m_CoordList.resize( m_CoordList.size() );
	for( size_t ii=0; ii<m_CoordList.size(); ++ii )
	{
		std::vector<shared_ptr<IfcLengthMeasure> >& vec_ii = m_CoordList[ii];
		std::vector<shared_ptr<IfcLengthMeasure> >& vec_ii_target = copy_self->m_CoordList[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcLengthMeasure>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcLengthMeasure>( item_jj->getDeepCopy() ) );
			}
		}
	}
	return copy_self;
}
void IfcCartesianPointList3D::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCARTESIANPOINTLIST3D" << "(";
	writeTypeOfRealList2D( stream, m_CoordList );
	stream << ");";
}
void IfcCartesianPointList3D::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCartesianPointList3D::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCartesianPointList3D, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcCartesianPointList3D, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readTypeOfRealList2D( args[0], m_CoordList );
}
void IfcCartesianPointList3D::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCartesianPointList::getAttributes( vec_attributes );
}
void IfcCartesianPointList3D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCartesianPointList::getAttributesInverse( vec_attributes_inverse );
}
void IfcCartesianPointList3D::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCartesianPointList::setInverseCounterparts( ptr_self_entity );
}
void IfcCartesianPointList3D::unlinkSelf()
{
	IfcCartesianPointList::unlinkSelf();
}

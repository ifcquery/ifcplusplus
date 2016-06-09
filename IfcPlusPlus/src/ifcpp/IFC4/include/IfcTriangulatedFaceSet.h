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
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPGlobal.h"
#include "IfcTessellatedFaceSet.h"
class IFCPP_EXPORT IfcPositiveInteger;
//ENTITY
class IFCPP_EXPORT IfcTriangulatedFaceSet : public IfcTessellatedFaceSet
{ 
public:
	IfcTriangulatedFaceSet();
	IfcTriangulatedFaceSet( int id );
	~IfcTriangulatedFaceSet();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcTriangulatedFaceSet"; }


	// IfcRepresentationItem -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcPresentationLayerAssignment> >	m_LayerAssignment_inverse;
	//  std::vector<weak_ptr<IfcStyledItem> >					m_StyledByItem_inverse;

	// IfcGeometricRepresentationItem -----------------------------------------------------------

	// IfcTessellatedItem -----------------------------------------------------------

	// IfcTessellatedFaceSet -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcCartesianPointList3D>						m_Coordinates;
	//  std::vector<std::vector<shared_ptr<IfcParameterValue> > >	m_Normals;					//optional
	//  shared_ptr<IfcBoolean>									m_Closed;					//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcIndexedColourMap> >				m_HasColours_inverse;
	//  std::vector<weak_ptr<IfcIndexedTextureMap> >			m_HasTextures_inverse;

	// IfcTriangulatedFaceSet -----------------------------------------------------------
	// attributes:
	std::vector<std::vector<shared_ptr<IfcPositiveInteger> > >	m_CoordIndex;
	std::vector<std::vector<shared_ptr<IfcPositiveInteger> > >	m_NormalIndex;				//optional
};


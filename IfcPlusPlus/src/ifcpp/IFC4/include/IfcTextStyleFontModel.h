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
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPGlobal.h"
#include "IfcPreDefinedTextFont.h"
class IFCPP_EXPORT IfcTextFontName;
class IFCPP_EXPORT IfcFontStyle;
class IFCPP_EXPORT IfcFontVariant;
class IFCPP_EXPORT IfcFontWeight;
class IFCPP_EXPORT IfcSizeSelect;
//ENTITY
class IFCPP_EXPORT IfcTextStyleFontModel : public IfcPreDefinedTextFont
{ 
public:
	IfcTextStyleFontModel();
	IfcTextStyleFontModel( int id );
	~IfcTextStyleFontModel();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcTextStyleFontModel"; }


	// IfcPresentationItem -----------------------------------------------------------

	// IfcPreDefinedItem -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>						m_Name;

	// IfcPreDefinedTextFont -----------------------------------------------------------

	// IfcTextStyleFontModel -----------------------------------------------------------
	// attributes:
	std::vector<shared_ptr<IfcTextFontName> >	m_FontFamily;
	shared_ptr<IfcFontStyle>					m_FontStyle;				//optional
	shared_ptr<IfcFontVariant>					m_FontVariant;				//optional
	shared_ptr<IfcFontWeight>					m_FontWeight;				//optional
	shared_ptr<IfcSizeSelect>					m_FontSize;
};


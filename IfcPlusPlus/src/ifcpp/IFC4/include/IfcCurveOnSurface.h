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
#include <iostream>
#include <sstream>
#include <map>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"

// TYPE IfcCurveOnSurface = SELECT	(IfcCompositeCurveOnSurface	,IfcPcurve);
class IfcCurveOnSurface : virtual public IfcPPObject
{
public:
	IfcCurveOnSurface();
	~IfcCurveOnSurface();
	virtual const char* classname() const { return "IfcCurveOnSurface"; }
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const {
	}; // overwritten, abstract class
	static shared_ptr<IfcCurveOnSurface> createObjectFromStepData( const std::string& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
};


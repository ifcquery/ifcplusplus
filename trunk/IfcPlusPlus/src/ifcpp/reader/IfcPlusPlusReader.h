/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
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
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPModel.h"
#include "ifcpp/model/StatusObservable.h"

class IfcPlusPlusReader : public StatusObservable
{
public:
	IfcPlusPlusReader();
	~IfcPlusPlusReader();
	void setModel( shared_ptr<IfcPPModel>& model );
	virtual void readStreamHeader(	const std::string& in ) = 0;
	virtual void readStreamData(	const std::string& in, std::map<int,shared_ptr<IfcPPEntity> >& map ) = 0;

protected:
	shared_ptr<IfcPPModel> m_model;
};

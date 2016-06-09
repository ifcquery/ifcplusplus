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

#include "IfcPPWriter.h"

class IFCPP_EXPORT IfcPPWriterXML : public IfcPPWriter
{
public:
	IfcPPWriterXML();
	~IfcPPWriterXML();
	virtual void writeModelToStream( std::stringstream& stream, shared_ptr<IfcPPModel> model ) {}
};

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

#include <ifcpp/model/shared_ptr.h>
#include "Command.h"

bool Command::mouseMoved( const osgGA::GUIEventAdapter& , osgGA::GUIActionAdapter&  ){ return false; }
bool Command::mouseClicked( const osgGA::GUIEventAdapter& , osgGA::GUIActionAdapter&  ){ return false; }
bool Command::mouseDragged( const osgGA::GUIEventAdapter& , osgGA::GUIActionAdapter&  ){ return false; }
bool Command::keyPressed( const osgGA::GUIEventAdapter& , osgGA::GUIActionAdapter&  ){ return false; }
void Command::itemSnapped( shared_ptr<IfcPPEntity> , bool  ){}
void Command::itemSelected( shared_ptr<IfcPPEntity> , bool  ){}

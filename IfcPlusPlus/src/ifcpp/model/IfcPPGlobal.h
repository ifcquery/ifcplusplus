/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
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

#pragma warning (disable: 4251)

// If IFC++ should be loaded as a dynamically linked library, define IFCPP_AS_DYNAMIC_LIBRARY in your project settings.
// Additionally, IFCPP_LIB needs to be defined in the settings of the library (IfcPlusPlus)

#if defined(_MSC_VER)
	#ifdef IFCPP_AS_DYNAMIC_LIBRARY
		#ifdef IFCPP_LIB
			#define IFCPP_EXPORT __declspec(dllexport)
		#else
			#define IFCPP_EXPORT __declspec(dllimport)
		#endif
	#endif
#endif

#ifndef IFCPP_EXPORT
#define IFCPP_EXPORT // Define empty if not necessary
#endif

/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

// for developing only:
#ifdef _DEBUG
#define GEOMETRY_DEBUG_CHECK
#endif


#ifdef GEOMETRY_DEBUG_CHECK
	#include <ifcpp/geometry/Carve/GeomDebugDump.h>
#endif
#include <ifcpp/geometry/Carve/GeometryConverter.h>
#include <ifcpp/geometry/Carve/ConverterOSG.h>
#include <ifcpp/geometry/Carve/GeomUtils.h>


//#ifdef GEOMETRY_DEBUG_CHECK
//	#include <ifcpp/geometry/OCC/GeomDebugDumpOCC.h>
//#endif
//#include <ifcpp/geometry/OCC/GeometryConverterOCC.h>
//#include <ifcpp/geometry/OCC/SceneGraphConverterOCC.h>
//#include <ifcpp/geometry/OCC/GeomUtilsOCC.h>

#include <ifcpp/geometry/SceneGraphUtils.h>
#include <ifcpp/geometry/GeometrySettings.h>

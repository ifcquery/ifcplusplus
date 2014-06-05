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

#ifdef  _DEBUG

#include <osg/Vec4>
#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"

void setRenderPolyhedronCallBack( void* obj_ptr, void (*func)(void*, const carve::input::PolyhedronData* poly, const osg::Vec4f& color, const bool wireframe) );
void setRenderMeshsetCallBack( void* obj_ptr, void (*func)(void*, const carve::mesh::MeshSet<3>* meshset, const osg::Vec4f& color, const bool wireframe) );
void setRenderPolylineCallBack( void* obj_ptr, void (*func)(void*, const carve::input::PolylineSetData* poly_line, const osg::Vec4f& color ) );
void setRenderPathsCallBack( void* obj_ptr, void (*func)(void*, const std::vector<std::vector<carve::geom::vector<2> > >& paths ) );

void renderShapeInputDataInDebugViewer( const ShapeInputData* product_shape, const osg::Vec4f& color, const bool wireframe );
void renderPolyhedronInDebugViewer( const carve::input::PolyhedronData* poly, const osg::Vec4f& color, const bool wireframe);
void renderMeshsetInDebugViewer(const carve::mesh::MeshSet<3>* meshset, const osg::Vec4f& color, const bool wireframe);
void renderPolylineInDebugViewer( const carve::input::PolylineSetData* poly_line, const osg::Vec4f& color );
void renderPathsInDebugViewer( const std::vector<std::vector<carve::geom::vector<2> > >& paths );

void createTest( osg::Group* group, osg::Group* root );
void createTest2( osg::Group* group );
void createTest4( osg::Group* group );
void dumpMeshsets( carve::mesh::MeshSet<3>* operand1, carve::mesh::MeshSet<3>* operand2, carve::mesh::MeshSet<3>* result, int id_operand1, int id_operand2 );
void dumpPathToVTK( const std::vector<std::vector<carve::geom::vector<2> > >& paths );
void drawVertexNumbers( const carve::input::PolyhedronData* poly, const osg::Vec4f& color, osg::Geode* geode );

#endif

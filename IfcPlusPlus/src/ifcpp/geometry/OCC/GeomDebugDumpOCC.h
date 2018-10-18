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

#ifdef GEOMETRY_DEBUG_CHECK

#include <vector>

#include <AIS_shape.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>

#include <osgDB/fstream>
#include <osgDB/WriteFile>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include "GeometryInputDataOCC.h"
#include "SceneGraphConverterOCC.h"
#include "GeomUtilsOCC.h"

namespace GeomDebugDumpOCC
{
	static double dump_y_pos_geom = 0;
	static bool save_occ_brep = false;
	static TopoDS_Compound dump_shape_compound;
	static std::vector<osg::ref_ptr<osg::Geode> > vec_dump_shapes;

	inline const char* getDumpFileName() { return "dump_mesh_debug.osg"; }
	inline void clearDebugDumpFile()
	{
		std::ofstream dump_ofstream( getDumpFileName(), std::ofstream::trunc );
		dump_ofstream.close();
	}
	inline void clearDumpShape()
	{
		dump_shape_compound.Nullify();
		dump_y_pos_geom = 0;
		vec_dump_shapes.clear();
	}
	inline void writeDumpFile()
	{
		if( save_occ_brep )
		{
			std::ofstream dump_ofstream( getDumpFileName(), std::ofstream::app );
			BRepTools::Write( dump_shape_compound, dump_ofstream );
			dump_ofstream.close();
		}

		osg::ref_ptr<osg::Group> root = new osg::Group();
		for( size_t ii = 0; ii < vec_dump_shapes.size(); ++ii )
		{
			root->addChild( vec_dump_shapes[ii] );
		}

		std::string file_name = getDumpFileName();
		osgDB::writeNodeFile( *root, file_name );
	}
	static void addShapeToDump( const TopoDS_Shape& shape, const vec4& color )
	{
		if( save_occ_brep )
		{
			BRep_Builder builder;
			TopoDS_Compound Comp;
			builder.MakeCompound( Comp );
			builder.Add( Comp, shape );

			if( !dump_shape_compound.IsNull() )
			{
				builder.Add( Comp, dump_shape_compound );
			}

			dump_shape_compound = Comp;
		}

		// create osg geometry with colors and save it
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		ScenegraphConverterOCC::RenderOptions render_options( osg::Vec4f( color.r(), color.g(), color.b(), color.a() ) );
		ScenegraphConverterOCC::drawShape( shape, geode, render_options );
		vec_dump_shapes.push_back( geode );
	}

	inline void dumpShape( const TopoDS_Shape& shape, const vec4& color, bool append, bool move_dump_position )
	{
		if( !append )
		{
			clearDumpShape();
		}
		clearDebugDumpFile();

		if( move_dump_position )
		{
			Bnd_Box bbox;
			BRepBndLib::Add( shape, bbox );
			double xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
			bbox.Get( xmin, ymin, zmin, xmax, ymax, zmax );

			dump_y_pos_geom += (ymax - ymin)*1.2 + 1.0;
			if( ymax > dump_y_pos_geom )
			{
				dump_y_pos_geom = ymax*1.1;
			}
		}

		BRepBuilderAPI_Copy A;
		A.Perform(shape);
		TopoDS_Shape shape_transformed;
		shape_transformed = A.Shape();
		GeomUtilsOCC::applyTranslationToShape( shape_transformed, gp_Vec( 0, dump_y_pos_geom, 0 ) );

		addShapeToDump( shape_transformed, color );
		writeDumpFile();
	}

	inline void dumpEdgesOfShape( const TopoDS_Shape& shape, const vec4& color, bool append, bool move_dump_position )
	{
		if( !append )
		{
			clearDumpShape();
		}
		clearDebugDumpFile();

		if( move_dump_position )
		{
			Bnd_Box bbox;
			BRepBndLib::Add( shape, bbox );
			double xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
			bbox.Get( xmin, ymin, zmin, xmax, ymax, zmax );

			dump_y_pos_geom += (ymax - ymin)*1.2 + 1.0;
			if( ymax > dump_y_pos_geom )
			{
				dump_y_pos_geom = ymax*1.1;
			}
		}

		BRepBuilderAPI_Copy A;
		A.Perform(shape);
		TopoDS_Shape shape_transformed;
		shape_transformed = A.Shape();
		//TopoDS_Shape shape_transformed = shape;
		GeomUtilsOCC::applyTranslationToShape( shape_transformed, gp_Vec( 0, dump_y_pos_geom, 0 ) );

		//addShapeToDump( shape_transformed, color );

		ShapeFix_Wire fix_wire;
		Handle_ShapeExtend_WireData wire_data = new ShapeExtend_WireData();

		TopExp_Explorer explorer1( shape_transformed, TopAbs_EDGE );
		for( ; explorer1.More(); explorer1.Next() )
		{
			const TopoDS_Edge& edge = TopoDS::Edge( explorer1.Current() );
			//addShapeToDump( shape_transformed, color );
			wire_data->Add( edge );
		}

		fix_wire.Load( wire_data );
		//fix_wire.FixSelfIntersection();
		fix_wire.Perform();

		TopoDS_Wire edges_wire = fix_wire.Wire();
		if( !edges_wire.IsNull() )
		{
			addShapeToDump( edges_wire, color );
		}

		writeDumpFile();
	}

	//void createWireFromPoints( const std::vector<gp_Vec>& coordinates, TopoDS_Wire& wire, bool close_wire_with_first_point )
	//{
	//	std::vector<TopoDS_Vertex> vertices;
	//	for( size_t ii = 0; ii < coordinates.size(); ++ii )
	//	{
	//		const gp_Vec& vec = coordinates[ii];
	//		vertices.push_back( BRepBuilderAPI_MakeVertex( gp_Pnt( vec.X(), vec.Y(), vec.Z() ) ) );
	//	}

	//	BRepBuilderAPI_MakeWire mk_wire;
	//	for( size_t ii = 0; ii < vertices.size(); ii++ )
	//	{
	//		size_t idx_next = (ii + 1) % vertices.size();
	//		TopoDS_Vertex& vertex_ii = vertices[ii];
	//		TopoDS_Vertex& vertex_next = vertices[idx_next];

	//		if( ii == vertices.size() - 1 )
	//		{
	//			if( !close_wire_with_first_point )
	//			{
	//				break;
	//			}
	//		}

	//		if( coordinates.size() == vertices.size() )
	//		{
	//			const gp_Vec& vec = coordinates[ii];
	//			const gp_Vec& vec_next = coordinates[idx_next];
	//			if( GeomUtils::equal( vec, vec_next ) )
	//			{
	//				// points are equal, length of edge would be zero
	//				continue;
	//			}
	//		}

	//		if( vertex_ii.IsNull() )
	//		{
	//			continue;
	//		}

	//		if( vertex_next.IsNull() )
	//		{
	//			continue;
	//		}

	//		BRepBuilderAPI_MakeEdge mk_edge( vertex_ii, vertex_next );
	//		if( !mk_edge.IsDone() )
	//		{
	//			continue;
	//		}
	//		mk_wire.Add( mk_edge );
	//	}
	//	wire = mk_wire.Wire();
	//}
	
	inline void dumpPolyline( const std::vector<gp_Vec>& vec_polyline, const vec4& color, bool append, bool move_dump_position )
	{
		TopoDS_Wire wire;
		GeomUtilsOCC::createWireFromPoints( vec_polyline, wire, true );
		dumpShape( wire, color, append, move_dump_position );
	}
	inline void dumpPolyline( const std::vector<gp_Pnt>& vec_polyline, const vec4& color, bool append, bool move_dump_position )
	{
		std::vector<gp_Vec> vec_points;
		for( size_t ii = 0; ii < vec_polyline.size(); ++ii )
		{
			const gp_Pnt& point = vec_polyline[ii];
			vec_points.push_back( gp_Vec( point.X(), point.Y(), point.Z() ) );
		}
		dumpPolyline( vec_points, color, append, move_dump_position );
	}

	inline void dumpPolyline( const std::vector<gp_Vec2d>& vec_polyline, const vec4& color, bool append, bool move_dump_position )
	{
		std::vector<gp_Vec> vec_polyline_3d;
		for( size_t ii = 0; ii < vec_polyline.size(); ++ii )
		{
			vec_polyline_3d.push_back( gp_Vec( vec_polyline[ii].X(), vec_polyline[ii].Y(), 0 ) );
		}
		dumpPolyline( vec_polyline_3d, color, append, move_dump_position );
	}
}
#endif

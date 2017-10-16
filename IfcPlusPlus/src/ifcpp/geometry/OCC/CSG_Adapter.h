/* -*-c++-*- IFC++ www.ifcquery.com
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

#include <BRepAlgoAPI_BooleanOperation.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopoDS_Shape.hxx>

#include <ifcpp/geometry/GeometryException.h>
#include <ifcpp/model/IfcPPBasicTypes.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/StatusCallback.h>
#include "GeometryInputData.h"
#include "GeomDebugDump.h"

enum CSG_Operation { CSG_UNION, CSG_INTERSECTION, CSG_A_MINUS_B };

namespace CSG_Adapter
{
	inline void computeCSG( const TopoDS_Shape& operand1, const TopoDS_Shape& operand2, TopoDS_Shape& result_shape, BRepAlgoAPI_BooleanOperation& boolean_brep_algo, double fuzzy_value,
		StatusCallback* report_callback, IfcPPEntity* entity )
	{
		std::stringstream strs_err;
		TopoDS_Shape result;
		try
		{
			TopTools_ListOfShape argument_shapes;
			argument_shapes.Append( operand1 );
			TopTools_ListOfShape tool_shapes;
			tool_shapes.Append( operand2 );
			
			boolean_brep_algo.SetFuzzyValue( fuzzy_value );
			boolean_brep_algo.SetArguments( argument_shapes );
			boolean_brep_algo.SetTools( tool_shapes );
			boolean_brep_algo.SetRunParallel( false );
			boolean_brep_algo.Build();

			if( boolean_brep_algo.IsDone() )
			{
				result = boolean_brep_algo.Shape();
				try
				{
					ShapeFix_Shape fix( result );
					fix.Perform();
					result = fix.Shape();
				}
				catch( Standard_Failure sf )
				{
					std::cout << sf.GetMessageString() << std::endl;
				}
				catch( ... )
				{
					std::cout << "Shape healing failed on boolean result" << std::endl;
				}

				if( !result.IsNull() )
				{
					result_shape = result;
				}

				// more thorough checking:
				//bool is_valid = BRepCheck_Analyzer(result).IsValid() != 0;
				//if( is_valid )
				//{
				//	result_shape = result;
				//}
			}
			else
			{
				std::cout << "BRepAlgoAPI_BooleanOperation error " << std::endl;
			}
		}
		catch( Standard_Failure sf )
		{
			strs_err << sf.GetMessageString() << ", csg operation failed" << std::endl;
		}
		catch( ... )
		{
			strs_err << "csg operation failed" << std::endl;
			std::cout << "Shape healing failed on boolean result" << std::endl;
		}


		if( strs_err.tellp() > 0 )
		{
			if( report_callback )
			{
				report_callback->messageCallback( strs_err.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNCTION__, entity );
			}

#ifdef IFCPP_GEOM_DEBUG
			GeomDebugDump::dumpShape( operand1, vec4( 0.7, 0.7, 0.7, 1.0 ), true, true );
			GeomDebugDump::dumpShape( operand2, vec4( 0.6, 0.2, 0.2, 1.0 ), true, true );

			if( !result.IsNull() )
			{
				GeomDebugDump::dumpShape( result, vec4( 0.6, 0.2, 0.2, 1.0 ), true, true );
			}
#endif

			BRepAlgoAPI_Cut* csg_a_minus_b = dynamic_cast<BRepAlgoAPI_Cut*>(&boolean_brep_algo);
			
			if( csg_a_minus_b )
			{
				result = operand1;
			}
			else
			{
				BRepAlgoAPI_Common* csg_union = dynamic_cast<BRepAlgoAPI_Common*>(&boolean_brep_algo);
				if( csg_union )
				{
					result = operand1;
				}
				else
				{
					//result.Nullify();
				}
			}
		}
	}

	inline void computeCSG( const TopoDS_Shape& operand1, const TopoDS_Shape& operand2, TopoDS_Shape& result_shape, CSG_Operation op, double fuzzy_value = GEOM_EPSILON_CSG_FUZZY, StatusCallback* report_callback = nullptr, IfcPPEntity* entity = nullptr )
	{
		if( op == CSG_UNION )
		{
			BRepAlgoAPI_Fuse brep_fuse;
			computeCSG( operand1, operand2, result_shape, brep_fuse, fuzzy_value, report_callback, entity );
		}
		else if( op == CSG_INTERSECTION )
		{
			BRepAlgoAPI_Common brep_common;
			computeCSG( operand1, operand2, result_shape, brep_common, fuzzy_value, report_callback, entity );
		}
		else if( op == CSG_A_MINUS_B )
		{
			BRepAlgoAPI_Cut brep_cut;
			computeCSG( operand1, operand2, result_shape, brep_cut, fuzzy_value, report_callback, entity );
		}
	}
};

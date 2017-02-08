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

#include <BRepAlgoAPI_BooleanOperation.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopoDS_Shape.hxx>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/StatusCallback.h>
#include "GeometryException.h"
#include "GeometryInputData.h"
#include "GeomDebugUtils.h"

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
			TopTools_ListOfShape s1s;
			s1s.Append( operand1 );
			TopTools_ListOfShape s2s;
			s2s.Append( operand2 );
			
			boolean_brep_algo.SetFuzzyValue( fuzzy_value );
			boolean_brep_algo.SetArguments( s1s );
			boolean_brep_algo.SetTools( s2s );
			boolean_brep_algo.Build();
			//	bool is_parallel = boolean_brep_algo.RunParallel();
			boolean_brep_algo.SetRunParallel( true );

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
				int error_status = boolean_brep_algo.ErrorStatus();
				std::cout << "BRepAlgoAPI_BooleanOperation error: " << error_status << std::endl;
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

#ifdef _DEBUG
			GeomDebugUtils::dumpShape( operand1, vec4( 0.7, 0.7, 0.7, 1.0 ), true, true );
			GeomDebugUtils::dumpShape( operand2, vec4( 0.6, 0.2, 0.2, 1.0 ), true, true );

			if( !result.IsNull() )
			{
				GeomDebugUtils::dumpShape( result, vec4( 0.6, 0.2, 0.2, 1.0 ), true, true );
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

	inline void computeCSG( const TopoDS_Shape& operand1, const TopoDS_Shape& operand2, TopoDS_Shape& result_shape, CSG_Operation op, double fuzzy_value = GEOM_CSG_FUZZY_EPSILON, StatusCallback* report_callback = nullptr, IfcPPEntity* entity = nullptr )
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

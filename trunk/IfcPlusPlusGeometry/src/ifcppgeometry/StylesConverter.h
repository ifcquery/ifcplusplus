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

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/IfcPPOpenMP.h>

class IfcStyledItem;
class IfcPresentationStyle;
class IfcPresentationStyleSelect;
class IfcCurveStyle;
class IfcSurfaceStyle;
class IfcComplexProperty;
class AppearanceData;

class StylesConverter : public StatusCallback
{
public:
	StylesConverter();
	~StylesConverter();
	
	void convertIfcStyledItem( weak_ptr<IfcStyledItem> item, std::vector<shared_ptr<AppearanceData> >& vec_appearance_data );
	void convertIfcComplexPropertyColor( shared_ptr<IfcComplexProperty> complex_property, carve::geom::vector<4>& vec_color );
	void convertIfcSurfaceStyle( shared_ptr<IfcSurfaceStyle> surface_style, shared_ptr<AppearanceData>& appearance_data );
	void convertIfcPresentationStyle( shared_ptr<IfcPresentationStyle> presentation_style, shared_ptr<AppearanceData>& appearance_data );
	void convertIfcCurveStyle( shared_ptr<IfcCurveStyle> curve_style, shared_ptr<AppearanceData>& appearance_data );
	void clearStylesCache()
	{
		m_map_ifc_styles.clear();
	}

protected:
	std::map<int, shared_ptr<AppearanceData> > m_map_ifc_styles;

#ifdef IFCPP_OPENMP
	Mutex m_writelock_styles_converter;
#endif
};

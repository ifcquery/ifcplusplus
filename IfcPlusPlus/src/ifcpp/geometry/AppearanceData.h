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

#include <ifcpp/IFC4X3/include/IfcTextStyle.h>
#include <ifcpp/model/BasicTypes.h>

class AppearanceData
{
public:
	enum GeometryTypeEnum { GEOM_TYPE_UNDEFINED, GEOM_TYPE_TEXT, GEOM_TYPE_CURVE, GEOM_TYPE_SURFACE, GEOM_TYPE_VOLUME, GEOM_TYPE_ANY };
	AppearanceData( int step_style_id ) : m_step_style_id( step_style_id )
	{
	}
	vec4 m_color_ambient;
	vec4  m_color_diffuse;
	vec4  m_color_specular;
	int m_step_style_id;
	double m_shininess = 10.0;
	double m_transparency = 1.0;
	double m_specular_exponent = 0.0;
	double m_specular_roughness = 0.0;
	bool m_set_transparent = false;
	bool m_complete = false;
	shared_ptr<IFC4X3::IfcTextStyle> m_text_style;
	GeometryTypeEnum m_apply_to_geometry_type = GEOM_TYPE_UNDEFINED;
};

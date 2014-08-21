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

#include <cstring>
#include <boost/algorithm/string.hpp>

#include <ifcpp/IFC4/include/IfcSpecularHighlightSelect.h>
#include <ifcpp/IFC4/include/IfcSpecularExponent.h>
#include <ifcpp/IFC4/include/IfcSpecularRoughness.h>
#include <ifcpp/IFC4/include/IfcColour.h>
#include <ifcpp/IFC4/include/IfcColourOrFactor.h>
#include <ifcpp/IFC4/include/IfcColourRgb.h>
#include <ifcpp/IFC4/include/IfcNormalisedRatioMeasure.h>
#include <ifcpp/IFC4/include/IfcDraughtingPreDefinedColour.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcStyleAssignmentSelect.h>
#include <ifcpp/IFC4/include/IfcSurfaceStyleElementSelect.h>
#include <ifcpp/IFC4/include/IfcSurfaceStyle.h>
#include <ifcpp/IFC4/include/IfcSurfaceStyleShading.h>
#include <ifcpp/IFC4/include/IfcSurfaceStyleRendering.h>
#include <ifcpp/IFC4/include/IfcStyledItem.h>
#include <ifcpp/IFC4/include/IfcPresentationStyleAssignment.h>
#include <ifcpp/IFC4/include/IfcPresentationStyleSelect.h>
#include <ifcpp/IFC4/include/IfcPresentationStyle.h>
#include <ifcpp/IFC4/include/IfcProperty.h>
#include <ifcpp/IFC4/include/IfcComplexProperty.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcPropertySingleValue.h>
#include <ifcpp/IFC4/include/IfcInteger.h>
#include <ifcpp/IFC4/include/IfcCurveStyle.h>
#include <ifcpp/IFC4/include/IfcFillAreaStyle.h>
#include <ifcpp/IFC4/include/IfcTextStyle.h>
#include <ifcpp/IFC4/include/IfcTextStyleTextModel.h>
#include <ifcpp/IFC4/include/IfcNullStyle.h>
#include <ifcpp/IFC4/include/IfcExternallyDefinedSurfaceStyle.h>
#include <ifcpp/IFC4/include/IfcSurfaceStyleLighting.h>
#include <ifcpp/IFC4/include/IfcSurfaceStyleRefraction.h>
#include <ifcpp/IFC4/include/IfcSurfaceStyleWithTextures.h>
#include <ifcpp/IFC4/include/IfcValue.h>

#include "GeometryInputData.h"
#include "StylesConverter.h"

StylesConverter::StylesConverter()
{
}
StylesConverter::~StylesConverter()
{
}

void convertIfcSpecularHighlightSelect(shared_ptr<IfcSpecularHighlightSelect> highlight_select, shared_ptr<AppearanceData>& appearance_data )
{
	float shininess = 64;

	if( dynamic_pointer_cast<IfcSpecularExponent>(highlight_select) )
	{
		shared_ptr<IfcSpecularExponent> spec = dynamic_pointer_cast<IfcSpecularExponent>(highlight_select);
		appearance_data->specular_exponent = spec->m_value;
	}
	else if( dynamic_pointer_cast<IfcSpecularRoughness>(highlight_select) )
	{
		shared_ptr<IfcSpecularRoughness> specRough = dynamic_pointer_cast<IfcSpecularRoughness>(highlight_select);
		appearance_data->specular_roughness = specRough->m_value;
	}
}

void convertIfcColourRgb( shared_ptr<IfcColourRgb> color_rgb, carve::geom::vector<4>& color )
{
	if( color_rgb->m_Red )
	{
		color.x = (float)color_rgb->m_Red->m_value;
	}
	if( color_rgb->m_Green)
	{
		color.y = (float)color_rgb->m_Green->m_value;
	}
	if( color_rgb->m_Blue)
	{
		color.z = (float)color_rgb->m_Blue->m_value;
	}
}

void convertIfcColourOrFactor( shared_ptr<IfcColourOrFactor> color_or_factor, carve::geom::vector<4>& src_color, carve::geom::vector<4>& target_color)
{
	// TYPE IfcColourOrFactor = SELECT ( IfcNormalisedRatioMeasure, IfcColourRgb);
	shared_ptr<IfcColourRgb> color_rgb = dynamic_pointer_cast<IfcColourRgb>(color_or_factor);
	if( color_rgb )
	{
		convertIfcColourRgb( color_rgb, target_color );
		return;
	}

	shared_ptr<IfcNormalisedRatioMeasure> ratio_measure = dynamic_pointer_cast<IfcNormalisedRatioMeasure>(color_or_factor);
	if( ratio_measure )
	{
		float factor = ratio_measure->m_value;
		target_color = carve::geom::VECTOR( src_color.x*factor, src_color.y*factor, src_color.z*factor, src_color.w );
		return;
	}
}

void convertIfcColour( shared_ptr<IfcColour> ifc_color_select, carve::geom::vector<4>& color )
{
	// IfcColour = SELECT ( IfcColourSpecification, IfcPreDefinedColour );
	shared_ptr<IfcColourSpecification> color_spec = dynamic_pointer_cast<IfcColourSpecification>(ifc_color_select);
	if( color_spec )
	{
		// ENTITY IfcColourSpecification ABSTRACT SUPERTYPE OF(IfcColourRgb);
		shared_ptr<IfcColourRgb> color_rgb = dynamic_pointer_cast<IfcColourRgb>(color_spec);
		if( color_rgb )
		{
			convertIfcColourRgb( color_rgb, color );
		}
		return;
	}

	shared_ptr<IfcPreDefinedColour> predefined_color = dynamic_pointer_cast<IfcPreDefinedColour>(ifc_color_select);
	if( predefined_color )
	{
		// ENTITY IfcPreDefinedColour ABSTRACT SUPERTYPE OF(IfcDraughtingPreDefinedColour)
		shared_ptr<IfcDraughtingPreDefinedColour> draughting_predefined_color = dynamic_pointer_cast<IfcDraughtingPreDefinedColour>(predefined_color);
		if( draughting_predefined_color )
		{
			if( draughting_predefined_color->m_Name )
			{
				std::wstring predefined_name = draughting_predefined_color->m_Name->m_value;
				if( boost::iequals(predefined_name, L"black") )			color = carve::geom::VECTOR( 0.0f, 0.0f, 0.0f, 1.f );
				else if( boost::iequals(predefined_name, L"red") )		color = carve::geom::VECTOR( 1.0f, 0.0f, 0.0f, 1.f );
				else if( boost::iequals(predefined_name, L"green") )	color = carve::geom::VECTOR( 0.0f, 1.0f, 0.0f, 1.f );
				else if( boost::iequals(predefined_name, L"blue") )		color = carve::geom::VECTOR( 0.0f, 0.0f, 1.0f, 1.f );
				else if( boost::iequals(predefined_name, L"yellow") )	color = carve::geom::VECTOR( 1.0f, 1.0f, 0.0f, 1.f );
				else if( boost::iequals(predefined_name, L"magenta") )	color = carve::geom::VECTOR( 1.0f, 0.0f, 1.0f, 1.f );
				else if( boost::iequals(predefined_name, L"cyan") )		color = carve::geom::VECTOR( 0.0f, 1.0f, 1.0f, 1.f );
				else if( boost::iequals(predefined_name, L"white") )	color = carve::geom::VECTOR( 1.0f, 1.0f, 1.0f, 1.f );
			}
		}
		return;
	}
}


void StylesConverter::convertIfcSurfaceStyle( shared_ptr<IfcSurfaceStyle> surface_style, shared_ptr<AppearanceData>& appearance_data )
{
	const int style_id = surface_style->m_id;
	auto it_find_existing_style = m_map_ifc_styles.find(style_id);
	if( it_find_existing_style != m_map_ifc_styles.end() )
	{
		// todo: check if appearance compare is faster here
		appearance_data = it_find_existing_style->second;
		return;
	}

	if( !appearance_data )
	{
		appearance_data = shared_ptr<AppearanceData>( new AppearanceData( style_id ) );
	}
	m_map_ifc_styles[style_id] = appearance_data;
	appearance_data->apply_to_geometry_type = AppearanceData::SURFACE;

	std::vector<shared_ptr<IfcSurfaceStyleElementSelect> >& vec_styles = surface_style->m_Styles;
	if( vec_styles.size() == 0 )
	{
		return;
	}

	for( size_t ii_styles = 0; ii_styles < vec_styles.size(); ++ii_styles )
	{
		shared_ptr<IfcSurfaceStyleElementSelect> surf_style_element_select = vec_styles[ii_styles];
		if( !surf_style_element_select )
		{
			continue;
		}
		// TYPE IfcSurfaceStyleElementSelect = SELECT	(IfcExternallyDefinedSurfaceStyle	,IfcSurfaceStyleLighting	,IfcSurfaceStyleRefraction	,IfcSurfaceStyleShading	,IfcSurfaceStyleWithTextures);
		shared_ptr<IfcSurfaceStyleShading> surface_style_shading = dynamic_pointer_cast<IfcSurfaceStyleShading>(surf_style_element_select);
		if( surface_style_shading )
		{
			carve::geom::vector<4> surface_color = carve::geom::VECTOR( 0.8, 0.82, 0.84, 1.f );
			if( surface_style_shading->m_SurfaceColour)
			{
				shared_ptr<IfcColourRgb> surf_color = surface_style_shading->m_SurfaceColour;
				convertIfcColourRgb( surf_color, surface_color );
			}

			if( surface_color.x < 0.05 && surface_color.y < 0.05 && surface_color.z < 0.05 )
			{
				surface_color = carve::geom::VECTOR( 0.1, 0.12, 0.15, surface_color.w );
			}

			carve::geom::vector<4> ambient_color( surface_color );
			//carve::geom::vector<4> emissive_color( 0.0f, 0.0f, 0.0f, 1.f );
			carve::geom::vector<4> diffuse_color( surface_color );
			carve::geom::vector<4> specular_color( surface_color );
			float shininess = 35.f;
			float transparency = surface_color.w;//0.7f;
			bool set_transparent = false;

			shared_ptr<IfcSurfaceStyleRendering> surf_style_rendering = dynamic_pointer_cast<IfcSurfaceStyleRendering>(surf_style_element_select);
			if( surf_style_rendering )
			{
				if( surf_style_rendering->m_DiffuseColour)
				{
					shared_ptr<IfcColourOrFactor> color_or_factor = surf_style_rendering->m_DiffuseColour;
					convertIfcColourOrFactor(color_or_factor, surface_color, diffuse_color);
				}

				if( surf_style_rendering->m_SpecularColour)
				{
					shared_ptr<IfcColourOrFactor> specular_color = surf_style_rendering->m_SpecularColour;
					//convertIfcColourOrFactor(specular_color, color, specularColor);
				}

				if( surf_style_rendering->m_Transparency)
				{
					// in IFC 1 is transparent, 0 is opaque. if not given, the value 0 (opaque) is assumed
					// in osg, 1 is opaque, 0 is transparent
					transparency = 1.f - (float)surf_style_rendering->m_Transparency->m_value;
					if( transparency < 0.1f )
					{
						transparency = 0.1f;
					}
					
					if( transparency > 1.f )
					{
						transparency = 1.f;
					}

					if( transparency < 0.99f )
					{
						set_transparent = true;
					}

				}

				if( surf_style_rendering->m_SpecularHighlight)
				{
					shared_ptr<IfcSpecularHighlightSelect> spec_highlight = surf_style_rendering->m_SpecularHighlight;
					convertIfcSpecularHighlightSelect( spec_highlight, appearance_data );
					shininess = appearance_data->specular_roughness*128;
					if( shininess <= 1.0 )
					{
						shininess = 1.0;
					}
				}
			}

			appearance_data->color_ambient = carve::geom::VECTOR(	ambient_color.x*0.8f,	ambient_color.y*0.8f,	ambient_color.z*0.8f,	transparency );
			appearance_data->color_diffuse = carve::geom::VECTOR(	diffuse_color.x,		diffuse_color.y,		diffuse_color.z,		transparency );
			appearance_data->color_specular = carve::geom::VECTOR(	specular_color.x*0.1,	specular_color.y*0.1,	specular_color.z*0.1,	transparency );
			appearance_data->shininess = shininess;
			appearance_data->set_transparent = set_transparent;
			appearance_data->transparency = transparency;

			continue;
		}

		shared_ptr<IfcExternallyDefinedSurfaceStyle> ext_surf_style = dynamic_pointer_cast<IfcExternallyDefinedSurfaceStyle>(surf_style_element_select);
		if (ext_surf_style)
		{
#ifdef _DEBUG
			std::cout << "IfcExternallyDefinedSurfaceStyle not implemented" << std::endl;
#endif
			continue;
		}

		shared_ptr<IfcSurfaceStyleLighting> style_lighting = dynamic_pointer_cast<IfcSurfaceStyleLighting>(surf_style_element_select);
		if (style_lighting)
		{
#ifdef _DEBUG
			std::cout << "IfcSurfaceStyleLighting not implemented" << std::endl;
#endif
			continue;
		}

		shared_ptr<IfcSurfaceStyleRefraction> style_refraction = dynamic_pointer_cast<IfcSurfaceStyleRefraction>(surf_style_element_select);
		if (style_refraction)
		{
#ifdef _DEBUG
			std::cout << "IfcSurfaceStyleRefraction not implemented" << std::endl;
#endif
			continue;
		}

		shared_ptr<IfcSurfaceStyleWithTextures> style_texture = dynamic_pointer_cast<IfcSurfaceStyleWithTextures>(surf_style_element_select);
		if( style_texture )
		{
#ifdef _DEBUG
			std::cout << "IfcSurfaceStyleWithTextures not implemented" << std::endl;
#endif
			continue;
		}
		
	}
}

void StylesConverter::convertIfcStyledItem( weak_ptr<IfcStyledItem> styled_item_weak, std::vector<shared_ptr<AppearanceData> >& vec_appearance_data )
{
	shared_ptr<IfcStyledItem> styled_item( styled_item_weak );
	const int style_id = styled_item->m_id;

	auto it_find_existing_style = m_map_ifc_styles.find(style_id);
	if( it_find_existing_style != m_map_ifc_styles.end() )
	{
		vec_appearance_data.push_back( it_find_existing_style->second );
		return;
	}

	std::vector<shared_ptr<IfcStyleAssignmentSelect> >& vec_style_assigns = styled_item->m_Styles;
	for( auto it_style_assigns=vec_style_assigns.begin(); it_style_assigns!=vec_style_assigns.end(); ++it_style_assigns )
	{
		// TYPE IfcStyleAssignmentSelect = SELECT	(IfcPresentationStyle	,IfcPresentationStyleAssignment);
		shared_ptr<IfcStyleAssignmentSelect> style_assign_select = (*it_style_assigns);
		if( !style_assign_select )
		{
			continue;
		}

		shared_ptr<IfcPresentationStyleAssignment> presentation_style_assign = dynamic_pointer_cast<IfcPresentationStyleAssignment>( style_assign_select );
		if( presentation_style_assign )
		{
			std::vector<shared_ptr<IfcPresentationStyleSelect> >& vec_styles = presentation_style_assign->m_Styles;

			for( auto it_presentation_styles = vec_styles.begin(); it_presentation_styles != vec_styles.end(); ++it_presentation_styles )
			{
				// TYPE IfcPresentationStyleSelect = SELECT	(IfcCurveStyle	,IfcFillAreaStyle	,IfcNullStyle	,IfcSurfaceStyle	,IfcSymbolStyle	,IfcTextStyle);
				shared_ptr<IfcPresentationStyleSelect> pres_style_select = ( *it_presentation_styles );

				shared_ptr<AppearanceData> appearance_data;
				convertIfcPresentationStyleSelect( pres_style_select, appearance_data );
				vec_appearance_data.push_back( appearance_data );
			}
			continue;
		}

		// ENTITY IfcPresentationStyle ABSTRACT SUPERTYPE OF(ONEOF(IfcCurveStyle, IfcFillAreaStyle, IfcSurfaceStyle, IfcSymbolStyle, IfcTextStyle));
		shared_ptr<IfcPresentationStyle> presentation_style = dynamic_pointer_cast<IfcPresentationStyle>( style_assign_select );
		if( presentation_style )
		{
			shared_ptr<AppearanceData> appearance_data;
			convertIfcPresentationStyle( presentation_style, appearance_data );
			vec_appearance_data.push_back( appearance_data );
			continue;
		}

		continue;
		
	}
}

void StylesConverter::convertIfcComplexPropertyColor( shared_ptr<IfcComplexProperty> complex_property, carve::geom::vector<4>& vec_color )
{
	std::vector<shared_ptr<IfcProperty> >& vec_HasProperties = complex_property->m_HasProperties;
	if( !complex_property->m_UsageName ) return;
	if( vec_HasProperties.size() < 3 ) return;
	std::wstring usage_name = complex_property->m_UsageName->m_value;
	if( !boost::iequals( usage_name.c_str(), L"Color" ) ) return;
	
	if( complex_property->m_HasProperties.size() > 2 )
	{
		shared_ptr<IfcPropertySingleValue> prop1 = dynamic_pointer_cast<IfcPropertySingleValue>(complex_property->m_HasProperties[0]);
		shared_ptr<IfcPropertySingleValue> prop2 = dynamic_pointer_cast<IfcPropertySingleValue>(complex_property->m_HasProperties[1]);
		shared_ptr<IfcPropertySingleValue> prop3 = dynamic_pointer_cast<IfcPropertySingleValue>(complex_property->m_HasProperties[2]);

		if( prop1 && prop2 && prop3 )
		{
			shared_ptr<IfcValue>	v1_select = prop1->m_NominalValue;
			shared_ptr<IfcValue>	v2_select = prop2->m_NominalValue;
			shared_ptr<IfcValue>	v3_select = prop3->m_NominalValue;
			if( v1_select && v2_select && v3_select )
			{
				shared_ptr<IfcInteger> v1_int = dynamic_pointer_cast<IfcInteger>(v1_select);
				shared_ptr<IfcInteger> v2_int = dynamic_pointer_cast<IfcInteger>(v2_select);
				shared_ptr<IfcInteger> v3_int = dynamic_pointer_cast<IfcInteger>(v3_select);

				if( v1_int && v2_int && v3_int )
				{
					double r = v1_int->m_value/255.0;
					double g = v2_int->m_value/255.0;
					double b = v3_int->m_value/255.0;

					if( r < 0.05 && g < 0.05 && b < 0.05 )
					{
						r = 0.1;
						g = 0.12;
						b = 0.15;
					}
					vec_color.x = r;
					vec_color.y = g;
					vec_color.z = b;
					vec_color.w = 1.0;

					//appearance_data->color_ambient = carve::geom::VECTOR( r, g, b, 1.f );
					//appearance_data->color_diffuse = carve::geom::VECTOR( r, g, b, 1.f );
					//appearance_data->color_specular = carve::geom::VECTOR( r, g, b, 1.f );
					//appearance_data->shininess = 35.f;

					//m_map_ifc_styles[complex_property_id] = appearance_data;

					return;
				}
			}
		}
	}
}

void StylesConverter::convertIfcPresentationStyle( shared_ptr<IfcPresentationStyle> presentation_style, shared_ptr<AppearanceData>& appearance_data )
{
	int style_id = presentation_style->m_id;
	auto it_find_existing_style = m_map_ifc_styles.find(style_id);
	if( it_find_existing_style != m_map_ifc_styles.end() )
	{
		// use existing appearance
		appearance_data = it_find_existing_style->second;
		return;
	}

	if( !appearance_data )
	{
		appearance_data = shared_ptr<AppearanceData>( new AppearanceData( style_id ) );
	}
	m_map_ifc_styles[style_id] = appearance_data;

	// ENTITY IfcPresentationStyle	ABSTRACT SUPERTYPE OF(ONEOF(IfcCurveStyle, IfcFillAreaStyle, IfcSurfaceStyle, IfcSymbolStyle, IfcTextStyle));
	shared_ptr<IfcCurveStyle> curve_style = dynamic_pointer_cast<IfcCurveStyle>( presentation_style );
	if( curve_style )
	{
		convertIfcCurveStyle( curve_style, appearance_data );
		return;
	}

	shared_ptr<IfcFillAreaStyle> fill_area_style = dynamic_pointer_cast<IfcFillAreaStyle>( presentation_style );
	if( fill_area_style )
	{
#ifdef _DEBUG
		std::cout << "IfcFillAreaStyle not implemented" << std::endl;
#endif
		return;
	}


	shared_ptr<IfcSurfaceStyle> surface_style = dynamic_pointer_cast<IfcSurfaceStyle>( presentation_style );
	if( surface_style )
	{
		convertIfcSurfaceStyle( surface_style, appearance_data );
		return;
	}

	shared_ptr<IfcTextStyle> text_style = dynamic_pointer_cast<IfcTextStyle>( presentation_style );
	if( text_style )
	{
		appearance_data->text_style = text_style;
		return;
	}

	return;
}

void StylesConverter::convertIfcPresentationStyleSelect( shared_ptr<IfcPresentationStyleSelect> presentation_style_select, shared_ptr<AppearanceData>& appearance_data )
{
	// TYPE IfcPresentationStyleSelect = SELECT	(IfcCurveStyle	,IfcFillAreaStyle	,IfcNullStyle	,IfcSurfaceStyle	,IfcSymbolStyle	,IfcTextStyle);
	shared_ptr<IfcCurveStyle> curve_style = dynamic_pointer_cast<IfcCurveStyle>( presentation_style_select );
	if( curve_style )
	{
		convertIfcCurveStyle( curve_style, appearance_data );
		return;
	}

	shared_ptr<IfcFillAreaStyle> fill_area_style = dynamic_pointer_cast<IfcFillAreaStyle>( presentation_style_select );
	if( fill_area_style )
	{
#ifdef _DEBUG
		std::cout << "IfcFillAreaStyle not implemented" << std::endl;
#endif
		return;
	}

	shared_ptr<IfcNullStyle> null_style = dynamic_pointer_cast<IfcNullStyle>( presentation_style_select );
	if( null_style )
	{
#ifdef _DEBUG
		std::cout << "IfcNullStyle not implemented" << std::endl;
#endif
		return;
	}

	shared_ptr<IfcSurfaceStyle> surface_style = dynamic_pointer_cast<IfcSurfaceStyle>( presentation_style_select );
	if( surface_style )
	{
		return convertIfcSurfaceStyle( surface_style, appearance_data );
	}

	shared_ptr<IfcTextStyle> text_style = dynamic_pointer_cast<IfcTextStyle>( presentation_style_select );
	if( text_style )
	{
		if (!appearance_data)
		{
			int style_id = text_style->m_id;
			appearance_data = shared_ptr<AppearanceData>(new AppearanceData(style_id));
		}

		appearance_data->text_style = text_style;
		appearance_data->apply_to_geometry_type = AppearanceData::TEXT;
		return;
	}
}


void StylesConverter::convertIfcCurveStyle( shared_ptr<IfcCurveStyle> curve_style, shared_ptr<AppearanceData>& appearance_data )
{
	int style_id = curve_style->m_id;
	auto it_find_existing_style = m_map_ifc_styles.find(style_id);
	if( it_find_existing_style != m_map_ifc_styles.end() )
	{
		appearance_data = it_find_existing_style->second;
		return;
	}
	
	if( !appearance_data )
	{
		appearance_data = shared_ptr<AppearanceData>( new AppearanceData( style_id ) );
	}
	m_map_ifc_styles[style_id] = appearance_data;
	appearance_data->apply_to_geometry_type = AppearanceData::CURVE;

	//CurveFont		: OPTIONAL IfcCurveFontOrScaledCurveFontSelect;
	//CurveWidth	: OPTIONAL IfcSizeSelect;
	//CurveColour	: OPTIONAL IfcColour;

	shared_ptr<IfcColour> curve_color = curve_style->m_CurveColour;
	if( curve_color )
	{
		carve::geom::vector<4> color = carve::geom::VECTOR( 0.2, 0.25, 0.3, 1.f );
		convertIfcColour( curve_color, color );

		if( color.x < 0.05 && color.y < 0.05 && color.z < 0.05 )
		{
			color = carve::geom::VECTOR( 0.1, 0.125, 0.15, color.w );
		}

		float shininess = 35.f;
		float transparency = 0.7f;

		appearance_data->color_ambient	= carve::geom::VECTOR( color.x*0.8,		color.y*0.8,	color.z*0.8,	color.w );
		appearance_data->color_diffuse	= carve::geom::VECTOR( color.x,			color.y,		color.z,		color.w );
		appearance_data->color_specular = carve::geom::VECTOR( color.x*0.1,		color.y*0.1,	color.z*0.1,	color.w );

		appearance_data->shininess = shininess;
		appearance_data->set_transparent = false;
	}
}

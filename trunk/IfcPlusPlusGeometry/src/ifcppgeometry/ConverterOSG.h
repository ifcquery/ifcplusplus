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

#include <osg/Material>
#include <osg/Geode>
#include <osg/CullFace>
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/StatusCallback.h>
#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"

class GeometrySettings;

class ConverterOSG : public StatusCallback
{
public:
	ConverterOSG( shared_ptr<GeometrySettings>& geom_settings );
	~ConverterOSG();
	static void drawFace(		const carve::mesh::Face<3>* face,					osg::Geode* geode, bool add_color_array = false );
	static void drawMesh(		const carve::mesh::Mesh<3>* mesh,					osg::Geode* geode, double intermediate_normal_angle = M_PI*0.05, bool add_color_array = false );
	static void drawMeshSet(	const carve::mesh::MeshSet<3>* mesh_set,			osg::Geode* geode, double intermediate_normal_angle = M_PI*0.05, bool add_color_array = false );
	static void drawPolyline(	const carve::input::PolylineSetData* polyline_data, osg::Geode* geode, bool add_color_array = false );
	static double computeSurfaceAreaOfGroup( const osg::Group* grp );
	void convertToOSG( shared_ptr<ShapeInputData>& product_shape, const double length_factor );

	void clearAppearanceCache()
	{
#ifdef IFCPP_OPENMP
		ScopedLock lock( m_writelock_appearance_cache );
#endif
		m_vec_existing_statesets.clear();
	}

	osg::StateSet* convertToOSGStateSet( const shared_ptr<AppearanceData>& appearence )
	{
		if( !appearence )
		{
			return nullptr;
		}
		const float shininess = appearence->m_shininess;
		const float transparency = appearence->m_transparency;
		const bool set_transparent = appearence->m_set_transparent;

		const float color_ambient_r = appearence->m_color_ambient.x;
		const float color_ambient_g = appearence->m_color_ambient.y;
		const float color_ambient_b = appearence->m_color_ambient.z;
		const float color_ambient_a = appearence->m_color_ambient.w;

		const float color_diffuse_r = appearence->m_color_diffuse.x;
		const float color_diffuse_g = appearence->m_color_diffuse.y;
		const float color_diffuse_b = appearence->m_color_diffuse.z;
		const float color_diffuse_a = appearence->m_color_diffuse.w;

		const float color_specular_r = appearence->m_color_specular.x;
		const float color_specular_g = appearence->m_color_specular.y;
		const float color_specular_b = appearence->m_color_specular.z;
		const float color_specular_a = appearence->m_color_specular.w;

	#ifdef IFCPP_OPENMP
		ScopedLock lock( m_writelock_appearance_cache );
	#endif

		for( int i=0; i<m_vec_existing_statesets.size(); ++i )
		{
			const osg::ref_ptr<osg::StateSet> stateset_existing = m_vec_existing_statesets[i];

			if( !stateset_existing.valid() )
			{
				continue;
			}

			osg::ref_ptr<osg::Material> mat_existing = (osg::Material*)stateset_existing->getAttribute(osg::StateAttribute::MATERIAL);
			if( !mat_existing )
			{
				continue;
			}

			// compare
			osg::Vec4f color_ambient_existing = mat_existing->getAmbient( osg::Material::FRONT_AND_BACK );
			if( fabs(color_ambient_existing.r() - color_ambient_r ) > 0.03 ) break;
			if( fabs(color_ambient_existing.g() - color_ambient_g ) > 0.03 ) break;
			if( fabs(color_ambient_existing.b() - color_ambient_b ) > 0.03 ) break;
			if( fabs(color_ambient_existing.a() - color_ambient_a ) > 0.03 ) break;

			osg::Vec4f color_diffuse_existing = mat_existing->getDiffuse( osg::Material::FRONT_AND_BACK );
			if( fabs(color_diffuse_existing.r() - color_diffuse_r ) > 0.03 ) break;
			if( fabs(color_diffuse_existing.g() - color_diffuse_g ) > 0.03 ) break;
			if( fabs(color_diffuse_existing.b() - color_diffuse_b ) > 0.03 ) break;
			if( fabs(color_diffuse_existing.a() - color_diffuse_a ) > 0.03 ) break;

			osg::Vec4f color_specular_existing = mat_existing->getSpecular( osg::Material::FRONT_AND_BACK );
			if( fabs(color_specular_existing.r() - color_specular_r ) > 0.03 ) break;
			if( fabs(color_specular_existing.g() - color_specular_g ) > 0.03 ) break;
			if( fabs(color_specular_existing.b() - color_specular_b ) > 0.03 ) break;
			if( fabs(color_specular_existing.a() - color_specular_a ) > 0.03 ) break;

			float shininess_existing = mat_existing->getShininess( osg::Material::FRONT_AND_BACK );
			if( fabs(shininess_existing - shininess ) > 0.03 ) break;

			bool blend_on_existing = stateset_existing->getMode( GL_BLEND ) == osg::StateAttribute::ON;
			if( blend_on_existing != set_transparent ) break;

			bool transparent_bin = stateset_existing->getRenderingHint() == osg::StateSet::TRANSPARENT_BIN;
			if( transparent_bin != set_transparent ) break;

			// if we get here, appearance is same as existing state set
			return stateset_existing;
		}

		osg::Vec4f ambientColor(	color_ambient_r,	color_ambient_g,	color_ambient_b,	transparency );
		osg::Vec4f diffuseColor(	color_diffuse_r,	color_diffuse_g,	color_diffuse_b,	transparency  );
		osg::Vec4f specularColor(	color_specular_r,	color_specular_g,	color_specular_b,	transparency );

		osg::ref_ptr<osg::Material> mat = new osg::Material();
		mat->setAmbient( osg::Material::FRONT_AND_BACK, ambientColor );
		mat->setDiffuse( osg::Material::FRONT_AND_BACK, diffuseColor );
		mat->setSpecular( osg::Material::FRONT_AND_BACK, specularColor );
		mat->setShininess( osg::Material::FRONT_AND_BACK, shininess );
		mat->setColorMode( osg::Material::SPECULAR );

		osg::StateSet* stateset = new osg::StateSet();
		stateset->setAttribute( mat, osg::StateAttribute::ON );
	
		if( appearence->m_set_transparent )
		{
			mat->setTransparency( osg::Material::FRONT_AND_BACK, transparency );	
			stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
			stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
		}

		if( appearence->m_specular_exponent != 0.f )
		{
			//osg::ref_ptr<osgFX::SpecularHighlights> spec_highlights = new osgFX::SpecularHighlights();
			//spec_highlights->setSpecularExponent( spec->m_value );
			// todo: add to scenegraph
		}

		m_vec_existing_statesets.push_back( stateset );
		return stateset;
	}

	shared_ptr<GeometrySettings>	m_geom_settings;
	osg::ref_ptr<osg::CullFace>		m_cull_back_off;
	osg::ref_ptr<osg::StateSet>		m_glass_stateset;
	//\brief StateSet caching and re-use
	std::vector<osg::ref_ptr<osg::StateSet> > m_vec_existing_statesets;
#ifdef IFCPP_OPENMP
	Mutex m_writelock_appearance_cache;
#endif
};

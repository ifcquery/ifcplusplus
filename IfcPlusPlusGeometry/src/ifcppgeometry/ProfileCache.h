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

#include <map>
#include <ifcpp/model/IfcPPOpenMP.h>
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/StatusCallback.h>

class GeometrySettings;
class UnitConverter;
class PointConverter;
class CurveConverter;
class SplineConverter;
class IfcProfileDef;
class ProfileConverter;

class ProfileCache : public StatusCallback
{
public:	
	ProfileCache(  shared_ptr<CurveConverter>& cc, shared_ptr<SplineConverter>& sc );
	~ProfileCache();

	shared_ptr<ProfileConverter> getProfileConverter( shared_ptr<IfcProfileDef>& ifc_profile );
	void clearProfileCache();

	shared_ptr<CurveConverter>					m_curve_converter;
	shared_ptr<SplineConverter>					m_spline_converter;

protected:
	std::map<int,shared_ptr<ProfileConverter> >	m_profile_cache;

#ifdef IFCPP_OPENMP
	Mutex m_writelock_profile_cache;
#endif
};

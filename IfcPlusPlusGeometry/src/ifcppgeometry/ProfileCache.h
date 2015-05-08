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
#include "ProfileConverter.h"
#include "CurveConverter.h"
#include "SplineConverter.h"

class ProfileCache : public StatusCallback
{
protected:
	shared_ptr<CurveConverter>					m_curve_converter;
	shared_ptr<SplineConverter>					m_spline_converter;
	std::map<int,shared_ptr<ProfileConverter> >	m_profile_cache;

#ifdef IFCPP_OPENMP
	Mutex m_writelock_profile_cache;
#endif

public:
	ProfileCache( shared_ptr<CurveConverter>& cc, shared_ptr<SplineConverter>& sc )
		: m_curve_converter( cc ), m_spline_converter( sc )
	{
	}

	~ProfileCache()
	{
	}

	void clearProfileCache()
	{
		m_profile_cache.clear();
	}

	shared_ptr<ProfileConverter> getProfileConverter( shared_ptr<IfcProfileDef>& ifc_profile )
	{
		if( !ifc_profile )
		{
			return shared_ptr<ProfileConverter>();
		}
		const int profile_id = ifc_profile->m_id;
		if( profile_id < 0 )
		{
			std::stringstream strs;
			strs << "Entity ID is invalid, type: " << ifc_profile->className();
			throw IfcPPException( strs.str().c_str(), __FUNC__ );
		}

		std::map<int, shared_ptr<ProfileConverter> >::iterator it_profile_cache = m_profile_cache.find( profile_id );
		if( it_profile_cache != m_profile_cache.end() )
		{
			return it_profile_cache->second;
		}

		shared_ptr<ProfileConverter> profile_converter = shared_ptr<ProfileConverter>( new ProfileConverter( m_curve_converter, m_spline_converter ) );
		if( !profile_converter )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		profile_converter->computeProfile( ifc_profile );

#ifdef IFCPP_OPENMP
		ScopedLock lock( m_writelock_profile_cache );
#endif
		m_profile_cache[profile_id] = profile_converter;

		return profile_converter;
	}
};

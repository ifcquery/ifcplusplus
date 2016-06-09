/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
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
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPGlobal.h"
#include "IfcSchedulingTime.h"
class IFCPP_EXPORT IfcDuration;
class IFCPP_EXPORT IfcPositiveRatioMeasure;
class IFCPP_EXPORT IfcDateTime;
class IFCPP_EXPORT IfcLabel;
class IFCPP_EXPORT IfcBoolean;
//ENTITY
class IFCPP_EXPORT IfcResourceTime : public IfcSchedulingTime
{ 
public:
	IfcResourceTime();
	IfcResourceTime( int id );
	~IfcResourceTime();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcResourceTime"; }


	// IfcSchedulingTime -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>				m_Name;						//optional
	//  shared_ptr<IfcDataOriginEnum>		m_DataOrigin;				//optional
	//  shared_ptr<IfcLabel>				m_UserDefinedDataOrigin;	//optional

	// IfcResourceTime -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcDuration>				m_ScheduleWork;				//optional
	shared_ptr<IfcPositiveRatioMeasure>	m_ScheduleUsage;			//optional
	shared_ptr<IfcDateTime>				m_ScheduleStart;			//optional
	shared_ptr<IfcDateTime>				m_ScheduleFinish;			//optional
	shared_ptr<IfcLabel>				m_ScheduleContour;			//optional
	shared_ptr<IfcDuration>				m_LevelingDelay;			//optional
	shared_ptr<IfcBoolean>				m_IsOverAllocated;			//optional
	shared_ptr<IfcDateTime>				m_StatusTime;				//optional
	shared_ptr<IfcDuration>				m_ActualWork;				//optional
	shared_ptr<IfcPositiveRatioMeasure>	m_ActualUsage;				//optional
	shared_ptr<IfcDateTime>				m_ActualStart;				//optional
	shared_ptr<IfcDateTime>				m_ActualFinish;				//optional
	shared_ptr<IfcDuration>				m_RemainingWork;			//optional
	shared_ptr<IfcPositiveRatioMeasure>	m_RemainingUsage;			//optional
	shared_ptr<IfcPositiveRatioMeasure>	m_Completion;				//optional
};


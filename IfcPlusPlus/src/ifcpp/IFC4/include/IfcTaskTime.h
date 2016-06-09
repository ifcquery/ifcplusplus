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
class IFCPP_EXPORT IfcTaskDurationEnum;
class IFCPP_EXPORT IfcDuration;
class IFCPP_EXPORT IfcDateTime;
class IFCPP_EXPORT IfcBoolean;
class IFCPP_EXPORT IfcPositiveRatioMeasure;
//ENTITY
class IFCPP_EXPORT IfcTaskTime : public IfcSchedulingTime
{ 
public:
	IfcTaskTime();
	IfcTaskTime( int id );
	~IfcTaskTime();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcTaskTime"; }


	// IfcSchedulingTime -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>				m_Name;						//optional
	//  shared_ptr<IfcDataOriginEnum>		m_DataOrigin;				//optional
	//  shared_ptr<IfcLabel>				m_UserDefinedDataOrigin;	//optional

	// IfcTaskTime -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcTaskDurationEnum>		m_DurationType;				//optional
	shared_ptr<IfcDuration>				m_ScheduleDuration;			//optional
	shared_ptr<IfcDateTime>				m_ScheduleStart;			//optional
	shared_ptr<IfcDateTime>				m_ScheduleFinish;			//optional
	shared_ptr<IfcDateTime>				m_EarlyStart;				//optional
	shared_ptr<IfcDateTime>				m_EarlyFinish;				//optional
	shared_ptr<IfcDateTime>				m_LateStart;				//optional
	shared_ptr<IfcDateTime>				m_LateFinish;				//optional
	shared_ptr<IfcDuration>				m_FreeFloat;				//optional
	shared_ptr<IfcDuration>				m_TotalFloat;				//optional
	shared_ptr<IfcBoolean>				m_IsCritical;				//optional
	shared_ptr<IfcDateTime>				m_StatusTime;				//optional
	shared_ptr<IfcDuration>				m_ActualDuration;			//optional
	shared_ptr<IfcDateTime>				m_ActualStart;				//optional
	shared_ptr<IfcDateTime>				m_ActualFinish;				//optional
	shared_ptr<IfcDuration>				m_RemainingTime;			//optional
	shared_ptr<IfcPositiveRatioMeasure>	m_Completion;				//optional
};


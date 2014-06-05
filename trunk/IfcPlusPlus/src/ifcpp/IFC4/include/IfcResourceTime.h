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
#include "IfcSchedulingTime.h"
class IfcDuration;
class IfcPositiveRatioMeasure;
class IfcDateTime;
class IfcLabel;
//ENTITY
class IfcResourceTime : public IfcSchedulingTime
{
public:
	IfcResourceTime();
	IfcResourceTime( int id );
	~IfcResourceTime();

	// method setEntity takes over all attributes from another instance of the class
	virtual void setEntity( shared_ptr<IfcPPEntity> other );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkSelf();
	virtual const char* classname() const { return "IfcResourceTime"; }


	// IfcSchedulingTime -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>							m_Name;						//optional
	//  shared_ptr<IfcDataOriginEnum>				m_DataOrigin;				//optional
	//  shared_ptr<IfcLabel>							m_UserDefinedDataOrigin;	//optional

	// IfcResourceTime -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcDuration>						m_ScheduleWork;				//optional
	shared_ptr<IfcPositiveRatioMeasure>			m_ScheduleUsage;			//optional
	shared_ptr<IfcDateTime>						m_ScheduleStart;			//optional
	shared_ptr<IfcDateTime>						m_ScheduleFinish;			//optional
	shared_ptr<IfcLabel>							m_ScheduleContour;			//optional
	shared_ptr<IfcDuration>						m_LevelingDelay;			//optional
	bool											m_IsOverAllocated;			//optional
	shared_ptr<IfcDateTime>						m_StatusTime;				//optional
	shared_ptr<IfcDuration>						m_ActualWork;				//optional
	shared_ptr<IfcPositiveRatioMeasure>			m_ActualUsage;				//optional
	shared_ptr<IfcDateTime>						m_ActualStart;				//optional
	shared_ptr<IfcDateTime>						m_ActualFinish;				//optional
	shared_ptr<IfcDuration>						m_RemainingWork;			//optional
	shared_ptr<IfcPositiveRatioMeasure>			m_RemainingUsage;			//optional
	shared_ptr<IfcPositiveRatioMeasure>			m_Completion;				//optional
};


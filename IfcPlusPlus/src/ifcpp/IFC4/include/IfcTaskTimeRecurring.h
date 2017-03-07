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
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPGlobal.h"
#include "IfcTaskTime.h"
class IFCPP_EXPORT IfcRecurrencePattern;
//ENTITY
class IFCPP_EXPORT IfcTaskTimeRecurring : public IfcTaskTime
{ 
public:
	IfcTaskTimeRecurring();
	IfcTaskTimeRecurring( int id );
	~IfcTaskTimeRecurring();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcTaskTimeRecurring"; }


	// IfcSchedulingTime -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>				m_Name;						//optional
	//  shared_ptr<IfcDataOriginEnum>		m_DataOrigin;				//optional
	//  shared_ptr<IfcLabel>				m_UserDefinedDataOrigin;	//optional

	// IfcTaskTime -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcTaskDurationEnum>		m_DurationType;				//optional
	//  shared_ptr<IfcDuration>				m_ScheduleDuration;			//optional
	//  shared_ptr<IfcDateTime>				m_ScheduleStart;			//optional
	//  shared_ptr<IfcDateTime>				m_ScheduleFinish;			//optional
	//  shared_ptr<IfcDateTime>				m_EarlyStart;				//optional
	//  shared_ptr<IfcDateTime>				m_EarlyFinish;				//optional
	//  shared_ptr<IfcDateTime>				m_LateStart;				//optional
	//  shared_ptr<IfcDateTime>				m_LateFinish;				//optional
	//  shared_ptr<IfcDuration>				m_FreeFloat;				//optional
	//  shared_ptr<IfcDuration>				m_TotalFloat;				//optional
	//  shared_ptr<IfcBoolean>				m_IsCritical;				//optional
	//  shared_ptr<IfcDateTime>				m_StatusTime;				//optional
	//  shared_ptr<IfcDuration>				m_ActualDuration;			//optional
	//  shared_ptr<IfcDateTime>				m_ActualStart;				//optional
	//  shared_ptr<IfcDateTime>				m_ActualFinish;				//optional
	//  shared_ptr<IfcDuration>				m_RemainingTime;			//optional
	//  shared_ptr<IfcPositiveRatioMeasure>	m_Completion;				//optional

	// IfcTaskTimeRecurring -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcRecurrencePattern>	m_Recurrence;
};


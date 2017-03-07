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
#include <map>
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "include/IfcAmountOfSubstanceMeasure.h"
#include "include/IfcAreaMeasure.h"
#include "include/IfcComplexNumber.h"
#include "include/IfcContextDependentMeasure.h"
#include "include/IfcCountMeasure.h"
#include "include/IfcDescriptiveMeasure.h"
#include "include/IfcElectricCurrentMeasure.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcLuminousIntensityMeasure.h"
#include "include/IfcMassMeasure.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcNumericMeasure.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPositivePlaneAngleMeasure.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcRatioMeasure.h"
#include "include/IfcSolidAngleMeasure.h"
#include "include/IfcThermodynamicTemperatureMeasure.h"
#include "include/IfcTimeMeasure.h"
#include "include/IfcVolumeMeasure.h"
#include "include/IfcValue.h"
#include "include/IfcMeasureValue.h"

// TYPE IfcMeasureValue = SELECT	(IfcAmountOfSubstanceMeasure	,IfcAreaMeasure	,IfcComplexNumber	,IfcContextDependentMeasure	,IfcCountMeasure	,IfcDescriptiveMeasure	,IfcElectricCurrentMeasure	,IfcLengthMeasure	,IfcLuminousIntensityMeasure	,IfcMassMeasure	,IfcNonNegativeLengthMeasure	,IfcNormalisedRatioMeasure	,IfcNumericMeasure	,IfcParameterValue	,IfcPlaneAngleMeasure	,IfcPositiveLengthMeasure	,IfcPositivePlaneAngleMeasure	,IfcPositiveRatioMeasure	,IfcRatioMeasure	,IfcSolidAngleMeasure	,IfcThermodynamicTemperatureMeasure	,IfcTimeMeasure	,IfcVolumeMeasure);
shared_ptr<IfcMeasureValue> IfcMeasureValue::createObjectFromSTEP( const std::wstring& arg, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	// read TYPE
	if( arg.size() == 0 ){ return shared_ptr<IfcMeasureValue>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcMeasureValue>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcMeasureValue>();
	}
	shared_ptr<IfcMeasureValue> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}

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
#include "include/IfcBinary.h"
#include "include/IfcBoolean.h"
#include "include/IfcDate.h"
#include "include/IfcDateTime.h"
#include "include/IfcDuration.h"
#include "include/IfcIdentifier.h"
#include "include/IfcInteger.h"
#include "include/IfcLabel.h"
#include "include/IfcLogical.h"
#include "include/IfcPositiveInteger.h"
#include "include/IfcReal.h"
#include "include/IfcText.h"
#include "include/IfcTime.h"
#include "include/IfcTimeStamp.h"
#include "include/IfcValue.h"
#include "include/IfcSimpleValue.h"

// TYPE IfcSimpleValue = SELECT	(IfcBinary	,IfcBoolean	,IfcDate	,IfcDateTime	,IfcDuration	,IfcIdentifier	,IfcInteger	,IfcLabel	,IfcLogical	,IfcPositiveInteger	,IfcReal	,IfcText	,IfcTime	,IfcTimeStamp);
shared_ptr<IfcSimpleValue> IfcSimpleValue::createObjectFromSTEP( const std::wstring& arg, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	// read TYPE
	if( arg.size() == 0 ){ return shared_ptr<IfcSimpleValue>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcSimpleValue>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcSimpleValue>();
	}
	shared_ptr<IfcSimpleValue> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}

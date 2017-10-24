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
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPModel.h"
#include "ifcpp/model/StatusCallback.h"

class IFCPP_EXPORT IfcPPReader : public StatusCallback
{
public:
	IfcPPReader();
	virtual ~IfcPPReader();
	virtual void removeComments( std::string& buffer ) = 0;
	virtual void readHeader(	const std::string& in, shared_ptr<IfcPPModel>& target_model ) = 0;
	virtual void readData( std::string& in, const IfcPPModel::IfcPPSchemaVersion& ifc_version, std::map<int, shared_ptr<IfcPPEntity> >& map ) = 0;
	virtual void readData( std::string& in, shared_ptr<IfcPPModel>& model ) = 0;
};

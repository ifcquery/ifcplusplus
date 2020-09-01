/* -*-c++-*- IfcQuery www.ifcquery.com
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

#include <exception>
#include "ifcpp/model/BasicTypes.h"
#include "AbstractReader.h"

class IFCQUERY_EXPORT ReaderXML : public AbstractReader
{
public:
	ReaderXML() = default;
	~ReaderXML() override = default;
	void removeComments( std::string& buffer ) override;
	virtual void readStreamHeader(	const std::string& in, shared_ptr<BuildingModel>& target_model );
	virtual void readStreamData( std::string& in, const BuildingModel::SchemaVersionEnum& ifc_version, std::map<int,shared_ptr<BuildingEntity> >& map );
	virtual void readStreamData( std::string& in, shared_ptr<BuildingModel>& model );
};

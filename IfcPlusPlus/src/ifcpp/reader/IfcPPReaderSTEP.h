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

#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPObject.h"
#include "IfcPPReader.h"

class IFCPP_EXPORT IfcPPReaderSTEP : public IfcPPReader
{
public:
	IfcPPReaderSTEP();
	~IfcPPReaderSTEP();
	virtual void removeComments( std::string& buffer );
	virtual void readHeader(	const std::string& in, shared_ptr<IfcPPModel>& target_model );
	virtual void readData( std::string& in, const IfcPPModel::IfcPPSchemaVersion& ifc_version, std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void readData( std::string& in, shared_ptr<IfcPPModel>& model );
	
	/*\brief Opens the given file, reads the content, and puts the entities into target_model.
	  \param[in] file_path Absolute path of the file to read.
	**/
	void loadModelFromFile( const std::wstring& file_path, shared_ptr<IfcPPModel>& target_model );
	void loadModelFromString( std::string& content, shared_ptr<IfcPPModel>& target_model );

	void splitIntoStepLines(	const std::string& read_in, std::vector<std::string>& step_lines );
	void readSingleStepLine(	const std::string& line, std::pair<std::string, shared_ptr<IfcPPEntity> >& target_read_object );
	void readStepLines(			const std::vector<std::string>& step_lines, std::vector<std::pair<std::string, shared_ptr<IfcPPEntity> > >& vec_target_entity );
	void readEntityArguments(	const IfcPPModel::IfcPPSchemaVersion& ifc_version, const std::vector<std::pair<std::string, shared_ptr<IfcPPEntity> > >& vec_entities, const std::map<int,shared_ptr<IfcPPEntity> >& map );
};

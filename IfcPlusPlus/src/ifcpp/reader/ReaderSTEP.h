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

#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "AbstractReader.h"

class IFCQUERY_EXPORT ReaderSTEP : public AbstractReader
{
public:
	ReaderSTEP();
	~ReaderSTEP() override;
	void removeComments( std::string& buffer ) override;
	void readHeader(	const std::string& in, shared_ptr<BuildingModel>& target_model ) override;
	void readData( std::string& in, const BuildingModel::SchemaVersion& ifc_version, std::map<int, shared_ptr<BuildingEntity> >& map ) override;
	void readData( std::string& in, shared_ptr<BuildingModel>& model ) override;
	
	/*\brief Opens the given file, reads the content, and puts the entities into target_model.
	  \param[in] file_path Absolute path of the file to read.
	**/
	virtual void loadModelFromFile( const std::wstring& filePath, shared_ptr<BuildingModel>& targetModel );
	virtual void loadModelFromString( std::string& content, shared_ptr<BuildingModel>& targetModel );

	void splitIntoStepLines(	const std::string& read_in, std::vector<std::string>& target_vec );
	void readSingleStepLine(	const std::string& line, std::pair<std::string, shared_ptr<BuildingEntity> >& target_read_object );
	void readStepLines(			const std::vector<std::string>& step_lines, std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > >& target_entity_vec );
	void readEntityArguments(	const BuildingModel::SchemaVersion& ifc_version, const std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > >& vec_entities, const std::map<int, shared_ptr<BuildingEntity> >& map );
};

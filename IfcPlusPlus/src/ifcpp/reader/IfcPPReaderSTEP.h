/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
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

#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"
#include "IfcPPReader.h"

class IFCPP_EXPORT IfcPPReaderSTEP : public IfcPPReader
{
public:
	IfcPPReaderSTEP();
	~IfcPPReaderSTEP();
	virtual void removeComments( std::string& buffer );
	virtual void readHeader(	const std::string& in, shared_ptr<IfcPPModel>& target_model );
	virtual void readData( std::string& in, const IfcPPModel::IfcPPSchemaVersion& ifc_version, boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void readData( std::string& in, shared_ptr<IfcPPModel>& model );
	
	/*\brief Opens the given file, reads the content, and puts the entities into target_model.
	  \param[in] file_path Absolute path of the file to read.
	**/
	void loadModelFromFile( const std::wstring& file_path, shared_ptr<IfcPPModel>& target_model );
	void loadModelFromString( std::string& content, shared_ptr<IfcPPModel>& target_model );

	void splitIntoStepLines(	const std::string& read_in, std::vector<std::string>& step_lines );
	void readSingleStepLine(	const std::string& line, std::pair<std::string, shared_ptr<IfcPPEntity> >& target_read_object );
	void readStepLines(			const std::vector<std::string>& step_lines, std::vector<std::pair<std::string, shared_ptr<IfcPPEntity> > >& vec_target_entity );
	void readEntityArguments(	const IfcPPModel::IfcPPSchemaVersion& ifc_version, const std::vector<std::pair<std::string, shared_ptr<IfcPPEntity> > >& vec_entities, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map );
};

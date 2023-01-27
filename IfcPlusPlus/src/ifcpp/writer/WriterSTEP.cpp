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

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <clocale>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "ifcpp/model/BuildingModel.h"
#include "ifcpp/writer/WriterUtil.h"
#include "IfcProduct.h"
#include "IfcProject.h"
#include "ifcpp/writer/WriterSTEP.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/IFC4X3/EntityFactory.h"

//#define EXTERNAL_WRITE_METHODS

void WriterSTEP::getAttributeStepArgument(uint32_t entityClassID, const std::string& attributeName, bool attributeIsSelectType, bool isOptional, const shared_ptr<BuildingObject>& attributeObject, std::stringstream& stream )
{
#ifdef EXTERNAL_WRITE_METHODS
	if( !attributeObject )
	{
		if( IFC4X3::EntityFactory::isAttributeRedeclaredAndDerived(entityClassID, attributeName ) )
		{
			// http://en.wikipedia.org/wiki/ISO_10303-21
			// Unset attribute values are given as "$".
			// Explicit attributes which got re - declared as derived in a subtype are encoded as "*" in the position of the supertype attribute
			stream << "*";
		}
		else
		{
			stream << "$";
		}
		return;
	}

	shared_ptr<BuildingEntity> attributeAsEntity = dynamic_pointer_cast<BuildingEntity>(attributeObject);
	if( attributeAsEntity )
	{
		stream << "#" << attributeAsEntity->m_tag;
		return;
	}

	if( attributeObject )
	{
		shared_ptr<BoolAttribute> attributeAsBool = dynamic_pointer_cast<BoolAttribute>(attributeObject);
		if( attributeAsBool )
		{
			if( attributeAsBool->m_value == true )
			{
				stream << ".T.";
			}
			else
			{
				stream << ".F.";
			}
			return;
		}

		shared_ptr<LogicalAttribute> attributeAsLogical = dynamic_pointer_cast<LogicalAttribute>(attributeObject);
		if( attributeAsLogical )
		{
			if( attributeAsLogical->m_value == LogicalEnum::LOGICAL_TRUE )
			{
				stream << ".T.";
			}
			else if( attributeAsLogical->m_value == LogicalEnum::LOGICAL_FALSE )
			{
				stream << ".F.";
			}
			else if( attributeAsLogical->m_value == LogicalEnum::LOGICAL_UNKNOWN )
			{
				stream << ".U.";
			}
			return;
		}

		shared_ptr<IntegerAttribute> attributeAsInteger = dynamic_pointer_cast<IntegerAttribute>(attributeObject);
		if( attributeAsInteger )
		{
			stream << attributeAsInteger->m_value;
			return;
		}

		shared_ptr<RealAttribute> attributeAsReal = dynamic_pointer_cast<RealAttribute>(attributeObject);
		if( attributeAsReal )
		{
			stream << attributeAsReal->m_value;
			return;
		}

		shared_ptr<StringAttribute> attributeAsString = dynamic_pointer_cast<StringAttribute>(attributeObject);
		if( attributeAsString )
		{
			stream << attributeAsString->m_value;
			return;
		}

		shared_ptr<AttributeObjectVector> attributeAsVector = dynamic_pointer_cast<AttributeObjectVector>(attributeObject);
		if( attributeAsVector )
		{
			if( attributeAsVector->m_vec.size() == 0 )
			{
				if( isOptional )
				{
					stream << "$";
				}
				else
				{
					stream << "()";
				}
			}
			else
			{
				stream << "(";
				for( auto it = attributeAsVector->m_vec.begin(); it != attributeAsVector->m_vec.end(); ++it )
				{
					if( it != attributeAsVector->m_vec.begin() )
					{
						stream << ",";
					}
					shared_ptr<BuildingObject> attributeInVector = *it;
					getAttributeStepArgument( entityClassID, attributeName,attributeIsSelectType, isOptional, attributeInVector, stream);
				}
				stream << ")";
			}
		}
		else
		{
			//uint32_t attributeTypeID = attributeObject->classID();
			//bool attributeIsSelectType = isSelectType(attributeTypeID);
			attributeObject->getStepParameter(stream, attributeIsSelectType );
		}
	}
#endif
}

void WriterSTEP::getStepLine( const shared_ptr<BuildingEntity>& be, std::stringstream& stream )
{
#ifdef EXTERNAL_WRITE_METHODS
	std::string ifcEntityName = IFC4X3::EntityFactory::getStringForClassID(be->classID());
	std::transform(ifcEntityName.begin(), ifcEntityName.end(), ifcEntityName.begin(), [](char c) {return std::toupper(c); });
	stream << "#" << be->m_tag << "= " << ifcEntityName << "(";

	//if( ifcEntityName.compare("IFCMEASUREWITHUNIT") == 0 )
	//{
	//	int wait = 0;

	//	//#9= IFCMEASUREWITHUNIT(IFCPLANEANGLEMEASURE(0.017453293),#8);
	//	// 
	//	//ENTITY IfcMeasureWithUnit; ValueComponent : IfcValue;	UnitComponent : IfcUnit; END_ENTITY;

	//	//TYPE IfcValue = SELECT (IfcDerivedMeasureValue ,IfcMeasureValue ,IfcSimpleValue); END_TYPE;
	//}

	std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes;
	be->getAttributes(vec_attributes);

	std::vector<uint32_t> attributeTypes;
	IFC4X3::EntityFactory::getAttributeTypesForClass(be->classID(), attributeTypes);

	std::vector<bool> attributesOptional;
	IFC4X3::EntityFactory::isAttributeOptionalForClass(be->classID(), attributesOptional);


	size_t ii = 0;
	for( auto it = vec_attributes.begin(); it != vec_attributes.end(); ++it )
	{
		if( it != vec_attributes.begin() )
		{
			stream << ",";
		}

		uint32_t entityClassID = be->classID();
		const std::string& attributeName = it->first;
		shared_ptr<BuildingObject>& attributeObject = it->second;

		bool attributeIsSelectType = false;
		if( attributeTypes.size() > ii )
		{
#ifdef _DEBUG
			//IFC4X3::IfcDimensionCount dimCount;
			//bool dimCount_attributeIsSelectType = IFC4X3::TypeFactory::isSelectType( dimCount.classID() );

			//IFC4X3::IfcPropertySingleValue singleValue;
			//case 3521532855: return "IfcValue";
			//IFC4X3::IfcValue	NominalValue;				//optional
			//bool NominalValue_attributeIsSelectType = IFC4X3::TypeFactory::isSelectType( NominalValue.classID() );

#endif

			attributeIsSelectType = IFC4X3::TypeFactory::isSelectType( attributeTypes[ii] );
		}

		bool isOptional = false;
		if( attributesOptional.size() > ii )
		{
			isOptional = attributesOptional[ii];
		}

		getAttributeStepArgument( entityClassID, attributeName, attributeIsSelectType, isOptional, attributeObject, stream);
		++ii;

	}

	stream << ");";
#endif
}

void WriterSTEP::writeModelToStream( std::stringstream& stream, shared_ptr<BuildingModel> model )
{
	//imbue C locale to always use dots as decimal separator
	stream.imbue(std::locale("C"));

	const std::string& file_header_str = model->getFileHeader();
	stream << "ISO-10303-21;\n";
	stream << file_header_str.c_str();
	stream << "DATA;\n";
	stream << std::setprecision( 15 );
	stream << std::setiosflags( std::ios::showpoint );
	stream << std::fixed;
	const std::map<int,shared_ptr<BuildingEntity> >& mapEntities = model->getMapIfcEntities();
	size_t i = 0;
	double last_progress = 0.0;
	double num_objects = double(mapEntities.size());
	for( auto it=mapEntities.begin(); it!=mapEntities.end(); ++it )
	{
		shared_ptr<BuildingEntity> obj = it->second;

		if( obj.use_count() < 2 )
		{
			// entity is referenced only in model map, not by other entities
			if( !dynamic_pointer_cast<IFC4X3::IfcProduct>(obj) && !dynamic_pointer_cast<IFC4X3::IfcProject>(obj) )
			{
				continue;
			}
		}
#ifdef EXTERNAL_WRITE_METHODS
		getStepLine(obj, stream);
#else
		obj->getStepLine( stream );
#endif
		stream << std::endl;

		if( i % 10 == 0 )
		{
			double progress = double( i ) / num_objects;
			if( progress - last_progress > 0.03 )
			{
				progressValueCallback( progress, "write" );
				last_progress = progress;
			}
		}
		++i;
	}

	stream << "ENDSEC;\n";
	stream << "END-ISO-10303-21; \n";
}

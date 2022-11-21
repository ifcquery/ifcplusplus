#include <locale>
#include <codecvt>
#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>
#include <QSize>
#include <QScreen>
#include <osg/Material>
#include <ifcpp/IFC4/include/IfcBoolean.h>
#include <ifcpp/IFC4/include/IfcAreaMeasure.h>
#include <ifcpp/IFC4/include/IfcDate.h>
#include <ifcpp/IFC4/include/IfcDateTime.h>
#include <ifcpp/IFC4/include/IfcDuration.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcInteger.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLogical.h>
#include <ifcpp/IFC4/include/IfcMassMeasure.h>
#include <ifcpp/IFC4/include/IfcMeasureValue.h>
#include <ifcpp/IFC4/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcProductRepresentation.h>
#include <ifcpp/IFC4/include/IfcPropertyListValue.h>
#include <ifcpp/IFC4/include/IfcReal.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcRelDefinesByProperties.h>
#include <ifcpp/IFC4/include/IfcRoot.h>
#include <ifcpp/IFC4/include/IfcShapeRepresentation.h>
#include <ifcpp/IFC4/include/IfcSpatialStructureElement.h>
#include <ifcpp/IFC4/include/IfcTime.h>
#include <ifcpp/IFC4/include/IfcTimeMeasure.h>
#include <ifcpp/IFC4/include/IfcTimeStamp.h>
#include <ifcpp/IFC4/include/IfcVolumeMeasure.h>
#include "Utils.h"

int getDPIAdjustedIconSize()
{
	QGuiApplication* gui_app = (QGuiApplication*)QCoreApplication::instance();
	if (!gui_app)
	{
		return 32;
	}
	QScreen* screen = gui_app->primaryScreen();
	if (!screen)
	{
		return 32;
	}
	int dpi = screen->physicalDotsPerInch();
	int width_height = 32;  // defaulting to 32
	if (dpi <= 96)
	{
		width_height = 32;
	}
	else if (dpi > 96 && dpi <= 120)
	{
		width_height = 40;
	}
	else if (dpi > 120 && dpi <= 144)
	{
		width_height = 48;
	}
	else if (dpi > 144)
	{
		width_height = 64;
	}

	return width_height;
}
QString readTextFile(QString path)
{
	QString data;
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		std::cout << "file not opened" << std::endl;
	}
	else
	{
		data = file.readAll();
	}
	file.close();
	return data;
}

QSize adjustIconSize(QAbstractButton* btn, int additional_width_height )
{
	int width_height = getDPIAdjustedIconSize();
	QSize icon_size(width_height + additional_width_height, width_height + additional_width_height);
	btn->setIconSize(icon_size);
	return icon_size;
}

std::string getGUID(const shared_ptr<BuildingObject>& obj)
{
	std::string guid;
	shared_ptr<IfcRoot> ifc_root = dynamic_pointer_cast<IfcRoot>(obj);
	if (ifc_root)
	{
		if (ifc_root->m_GlobalId)
		{
			guid = wstring2string(ifc_root->m_GlobalId->m_value);
		}
	}
	return guid;
}

std::wstring getGUID_wstr(const shared_ptr<BuildingObject>& obj)
{
	std::wstring guid;
	shared_ptr<IfcRoot> ifc_root = dynamic_pointer_cast<IfcRoot>(obj);
	if (ifc_root)
	{
		if (ifc_root->m_GlobalId)
		{
			guid = ifc_root->m_GlobalId->m_value;
			return guid;
		}
	}
	shared_ptr<IfcGloballyUniqueId> ifc_guid = dynamic_pointer_cast<IfcGloballyUniqueId>(obj);
	if (ifc_guid)
	{
		guid = ifc_guid->m_value;
		return guid;
	}
	return guid;
}

void hasShape(const shared_ptr<IfcObjectDefinition>& obj_def, bool& has3DShape, bool& has2DShape)
{
	shared_ptr<IfcProduct> product = dynamic_pointer_cast<IfcProduct>(obj_def);
	if (product)
	{
		if (product->m_Representation)
		{
			
			for (const shared_ptr<IfcRepresentation>& ifc_representation : product->m_Representation->m_Representations)
			{
				std::wstring ifc_representation_identifier = ifc_representation->m_RepresentationIdentifier->m_value;
				if (ifc_representation_identifier.compare(L"Axis") == 0)
				{
					has2DShape = true;
				}
				else if (ifc_representation_identifier.compare(L"Body") == 0)
				{
					has2DShape = true;
				}

				for (auto rep_item : ifc_representation->m_Items)
				{

				}
				shared_ptr<IfcShapeRepresentation> shape = dynamic_pointer_cast<IfcShapeRepresentation>(ifc_representation);
				if (shape)
				{
					has3DShape = true;
					break;
				}
			}
		}
	}
}

void getChildren(const shared_ptr<IfcObjectDefinition>& object_def, std::vector<shared_ptr<IfcObjectDefinition> >& vec_children)
{
	if (!object_def)
	{
		return;
	}
	std::vector<shared_ptr<IfcObjectDefinition> >::iterator it_object_def;
	std::vector<shared_ptr<IfcProduct> >::iterator it_product;

	if (object_def->m_IsDecomposedBy_inverse.size() > 0)
	{
		std::vector<weak_ptr<IfcRelAggregates> >& vec_IsDecomposedBy = object_def->m_IsDecomposedBy_inverse;
		std::vector<weak_ptr<IfcRelAggregates> >::iterator it;
		for (it = vec_IsDecomposedBy.begin(); it!=vec_IsDecomposedBy.end(); ++it)
		{
			shared_ptr<IfcRelAggregates> rel_agg(*it);
			std::vector<shared_ptr<IfcObjectDefinition> >& vec = rel_agg->m_RelatedObjects;

			for (it_object_def = vec.begin(); it_object_def!=vec.end(); ++it_object_def)
			{
				shared_ptr<IfcObjectDefinition> child_obj_def = (*it_object_def);
				vec_children.push_back(child_obj_def);
				getChildren(child_obj_def, vec_children);
			}
		}
	}

	shared_ptr<IfcSpatialStructureElement> spatial_ele = dynamic_pointer_cast<IfcSpatialStructureElement>(object_def);
	if (spatial_ele)
	{
		std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >& vec_contained = spatial_ele->m_ContainsElements_inverse;
		if (vec_contained.size() > 0)
		{
			std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >::iterator it_rel_contained;
			for (it_rel_contained = vec_contained.begin(); it_rel_contained!=vec_contained.end(); ++it_rel_contained)
			{
				shared_ptr<IfcRelContainedInSpatialStructure> rel_contained(*it_rel_contained);
				std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained->m_RelatedElements;
				std::vector<shared_ptr<IfcProduct> >::iterator it;

				for (it = vec_related_elements.begin(); it!=vec_related_elements.end(); ++it)
				{
					shared_ptr<IfcProduct> related_product = (*it);
					vec_children.push_back(related_product);
					getChildren(related_product, vec_children);
				}
			}
		}
	}
}


std::string wstring2string(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> StringConverter;
	return StringConverter.to_bytes(wstr);
}

std::wstring string2wstring(std::string& str_in)
{
	std::wstring wstr_out;
	wstr_out.assign(str_in.begin(), str_in.end());
	return wstr_out;
}

std::wstring string2wstring(const char* chr_in)
{
	std::string str_in(chr_in);
	std::wstring wstr_out;
	wstr_out.assign(str_in.begin(), str_in.end());
	return wstr_out;
}

void escapeSingleTicks(std::wstring& input)
{
	size_t len = input.length();

	for (size_t i = 0; i < len; ++i)
	{
		if (input[i] == '\'')
		{
			if (i > 0)
			{
				// check if escaped
				if (input[i-1] == '\\')
				{
					continue;
				}
			}
			input.insert(i, L"\\");
			++i;
			++len;
		}
	}
}

void escapeSingleTicks(std::string& input)
{
	size_t len = input.length();

	for (size_t i = 0; i < len; ++i)
	{
		if (input[i] == '\'')
		{
			if (i > 0)
			{
				// check if escaped
				if (input[i-1] == '\\')
				{
					continue;
				}
			}
			if (i < len)
			{
				input[i] = '&';
				input.insert(i + 1, "apos;");
				i += 5;
				len += 5;
			}
		}
	}
}

void applyTransparency(osg::Node* start_node, float transparency)
{
	osg::Group* group = dynamic_cast<osg::Group*>(start_node);
	if (group)
	{
		int num_children = group->getNumChildren();
		for (int i = 0; i<num_children; ++i)
		{
			osg::Node* child_node = group->getChild(i);
			applyTransparency(child_node, transparency);
		}
	}

	osg::StateSet* stateset = start_node->getStateSet();
	if (stateset)
	{
		osg::Material* material = dynamic_cast<osg::Material*>(stateset->getAttribute(osg::StateAttribute::MATERIAL));
		if (material)
		{
			material->setAlpha(osg::Material::FRONT_AND_BACK, transparency);
		}
	}
}

int findVertexIndex(const std::vector<carve::mesh::Vertex<3> >& vec_vertices, const carve::mesh::Vertex<3>* v)
{
	for (size_t iiv = 0; iiv < vec_vertices.size(); ++iiv)
	{
		const carve::mesh::Vertex<3>& vertex = vec_vertices[iiv];
		if (v == &vertex)
		{
			return iiv;
		}
	}
	return -1;
}

/** matrix operations */
void computeInverse(const carve::math::Matrix& matrix_a, carve::math::Matrix& inv)
{
	int i, j;	// col, row
	int s;		// step
	int prow;	// pivot
	int err_flag = 0;
	double factor;
	const double eps = 0.01;
	double max;
	int pivot = 1;
	double a[4][8];

	a[0][0] = matrix_a._11;
	a[0][1] = matrix_a._12;
	a[0][2] = matrix_a._13;
	a[0][3] = matrix_a._14;

	a[1][0] = matrix_a._21;
	a[1][1] = matrix_a._22;
	a[1][2] = matrix_a._23;
	a[1][3] = matrix_a._24;

	a[2][0] = matrix_a._31;
	a[2][1] = matrix_a._32;
	a[2][2] = matrix_a._33;
	a[2][3] = matrix_a._34;

	a[3][0] = matrix_a._41;
	a[3][1] = matrix_a._42;
	a[3][2] = matrix_a._43;
	a[3][3] = matrix_a._44;

	// append identity at the right
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			a[i][4 + j] = 0.0;
			if (i == j)
			{
				a[i][4 + j] = 1.0;
			}
		}
	}

	s = 0;
	do
	{
		max = std::abs(a[s][s]);
		if (pivot)
		{
			prow = s;
			for (i = s + 1; i < 4; ++i)
			{
				if (std::abs(a[i][s]) > max)
				{
					max = std::abs(a[i][s]);
					prow = i;
				}
			}
		}
		err_flag = max < eps;

		if (err_flag) break;

		if (pivot)
		{
			if (prow != s)
			{
				// change rows
				double temp;
				for (j = s; j < 2 * 4; ++j)
				{
					temp = a[s][j];
					a[s][j] = a[prow][j];
					a[prow][j] = temp;
				}
			}
		}

		// elimination: divide by pivot coefficient f = a[s][s]
		factor = a[s][s];
		for (j = s; j < 2 * 4; ++j)
		{
			a[s][j] = a[s][j] / factor;
		}

		for (i = 0; i < 4; ++i)
		{
			if (i != s)
			{
				factor = -a[i][s];
				for (j = s; j < 2 * 4; ++j)
				{
					a[i][j] += factor*a[s][j];
				}
			}
		}
		++s;
	} while (s < 4);

	if (err_flag)
	{
		throw std::exception("cannot compute inverse of matrix");
	}

	inv._11 = a[0][4];
	inv._12 = a[0][5];
	inv._13 = a[0][6];
	inv._14 = a[0][7];

	inv._21 = a[1][4];
	inv._22 = a[1][5];
	inv._23 = a[1][6];
	inv._24 = a[1][7];

	inv._31 = a[2][4];
	inv._32 = a[2][5];
	inv._33 = a[2][6];
	inv._34 = a[2][7];

	inv._41 = a[3][4];
	inv._42 = a[3][5];
	inv._43 = a[3][6];
	inv._44 = a[3][7];
}

vec3 getBBoxCorner(const carve::geom::aabb<3>& bbox, size_t corner)
{
	switch (corner)
	{
	case 0: return carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z - bbox.extent.z);
	case 1: return carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z - bbox.extent.z);
	case 2: return  carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z - bbox.extent.z);
	case 3: return  carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z - bbox.extent.z);
	case 4: return  carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z + bbox.extent.z);
	case 5: return  carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y - bbox.extent.y, bbox.pos.z + bbox.extent.z);
	case 6: return  carve::geom::VECTOR(bbox.pos.x + bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z + bbox.extent.z);
	case 7: return  carve::geom::VECTOR(bbox.pos.x - bbox.extent.x, bbox.pos.y + bbox.extent.y, bbox.pos.z + bbox.extent.z);
	}
	return  bbox.pos;
}


void autoIncrementFileName(QString& outputFileName, const char* fileExtension, bool& success)
{
	QFileInfo fi(outputFileName);
	size_t ii = 1;
	for (; ii < 1000; ++ii)
	{
		if (!QFile::exists(outputFileName))
		{
			break;
		}
		QDir absDir = fi.absoluteDir();
		QString absDirPath = absDir.absolutePath();
		QString baseName = fi.baseName();
		if (ii < 10)
		{
			outputFileName = absDirPath + "/" + baseName + "_0" + QString::number(ii) + fileExtension;
		}
		else
		{
			outputFileName = absDirPath + "/" + baseName + "_" + QString::number(ii) + fileExtension;
		}
	}
	if (ii == 99)
	{
		std::cout << "file already exists" << std::endl;
		success = false;
		return;
	}
	success = true;
}


void PsetContainer::addProperty(shared_ptr<PropertyValueContainer>& newPropertyContainer)
{
	QString newPsetName = QString::fromStdWString(newPropertyContainer->m_pset_name);    // z.B. Pset_Desite
	QString newPropertyName = QString::fromStdWString(newPropertyContainer->m_prop_name);   // z.B. PartMark

	

	for (size_t ii = 0; ii < m_vecProperties.size(); ++ii)
	{
		shared_ptr<PropertyValueContainer>& existingPropertyContainer = m_vecProperties[ii];

		QString existingPsetName = QString::fromStdWString(existingPropertyContainer->m_pset_name);
		QString existingPropertyName = QString::fromStdWString(existingPropertyContainer->m_prop_name);


	
	}
	//m_vecProperties.push_back(newPropertyContainer);
}

void PsetContainer::loadRelatedProperties(const shared_ptr<IfcObjectDefinition>& objectDef, shared_ptr<PsetContainer>& self)
{
	shared_ptr<IfcObject> ifc_object = dynamic_pointer_cast<IfcObject>(objectDef);
	if (ifc_object)
	{
		// handle property sets
		const std::vector<weak_ptr<IfcRelDefinesByProperties> >& vec_properties = ifc_object->m_IsDefinedBy_inverse;
		for (size_t ii = 0; ii < vec_properties.size(); ++ii)
		{
			shared_ptr<IfcRelDefinesByProperties> rel_prop(vec_properties[ii]);

			shared_ptr<IfcPropertySetDefinitionSelect>& relating_prop_set_def = rel_prop->m_RelatingPropertyDefinition;

			// TYPE IfcPropertySetDefinitionSelect = SELECT	(IfcPropertySetDefinition	,IfcPropertySetDefinitionSet);

			shared_ptr<IfcPropertySetDefinition> prop_set_def = dynamic_pointer_cast<IfcPropertySetDefinition>(relating_prop_set_def);
			if (prop_set_def)
			{
				//std::vector<weak_ptr<IfcTypeObject> >				m_DefinesType_inverse;
				//std::vector<weak_ptr<IfcRelDefinesByTemplate> >	m_IsDefinedBy_inverse;
				//std::vector<weak_ptr<IfcRelDefinesByProperties> >	m_DefinesOccurrence_inverse;

				shared_ptr<IfcPropertySet> prop_set = dynamic_pointer_cast<IfcPropertySet>(prop_set_def);
				if (prop_set)
				{
					const std::vector<shared_ptr<IfcProperty> >& vec_has_properties = prop_set->m_HasProperties;
					for (auto& ifc_prop : vec_has_properties)
					{
						readIfcProperty(prop_set, ifc_prop, self);
					}
				}
			}
		}
	}
}


PropertyValueContainer::PropertyValueContainer(const shared_ptr<IfcPropertySet>& pSet)
{
	if (pSet)
	{
		if (pSet->m_Name)
		{
			m_pset_name = pSet->m_Name->m_value;
		}
		else if (pSet->m_GlobalId)
		{
			m_pset_name = pSet->m_GlobalId->m_value;
		}
		else
		{
			m_pset_name = L"#";
			m_pset_name.append(std::to_wstring(pSet->m_tag));
		}
	}
}

PropertyValueContainer::PropertyValueContainer(const shared_ptr<IfcPropertySet>& pSet, std::wstring& prop_name, std::wstring& prop_description, std::wstring& prop_value, std::wstring& prop_unit, std::wstring& prop_type)
{
	if (pSet)
	{
		if (pSet->m_Name)
		{
			m_pset_name = pSet->m_Name->m_value;
		}
		else if (pSet->m_GlobalId)
		{
			m_pset_name = pSet->m_GlobalId->m_value;
		}
		else
		{
			m_pset_name = L"#";
			m_pset_name.append(std::to_wstring(pSet->m_tag));
		}
	}

	m_prop_name = prop_name;
	m_prop_description = prop_description;
	m_prop_value = prop_value;
	m_prop_unit = prop_unit;
	m_prop_type = prop_type;
}

void readIfcValue(const shared_ptr<IfcValue>& ifc_value, shared_ptr<PropertyValueContainer>& pvc)
{
	if (ifc_value)
	{
		// TYPE IfcValue = SELECT (IfcMeasureValue, IfcSimpleValue, IfcDerivedMeasureValue);

		std::string property_value_str = "";

		shared_ptr<IfcMeasureValue> nominal_value_measure = dynamic_pointer_cast<IfcMeasureValue>(ifc_value);
		if (nominal_value_measure)
		{
			readIfcMeasureValue(nominal_value_measure, pvc->m_prop_value);
		}
		else
		{
			shared_ptr<IfcSimpleValue> nominal_value_simple = dynamic_pointer_cast<IfcSimpleValue>(ifc_value);
			if (nominal_value_simple)
			{
				readIfcSimpleValue(nominal_value_simple, pvc->m_prop_value);
			}
			else
			{
				shared_ptr<IfcDerivedMeasureValue> derived_value = dynamic_pointer_cast<IfcDerivedMeasureValue>(ifc_value);
				if (derived_value)
				{
				}
				std::cout << "readIfcProperty:: unimplemented simple property: " << EntityFactory::getStringForClassID(ifc_value->classID()) << std::endl;
			}
		}
	}
}

void readIfcMeasureValue(shared_ptr<IfcMeasureValue>& ifc_measure, std::wstring& prop_value)
{
	// TYPE IfcMeasureValue = SELECT (	IfcVolumeMeasure, IfcTimeMeasure,  IfcThermodynamicTemperatureMeasure, IfcSolidAngleMeasure,  IfcPositiveRatioMeasure, 
	//  IfcRatioMeasure, IfcPositivePlaneAngleMeasure, IfcPlaneAngleMeasure, IfcParameterValue, IfcNumericMeasure, IfcMassMeasure, IfcPositiveLengthMeasure, IfcLengthMeasure, 
	//  IfcElectricCurrentMeasure, IfcDescriptiveMeasure, IfcCountMeasure, IfcContextDependentMeasure, IfcAreaMeasure, IfcAmountOfSubstanceMeasure, IfcLuminousIntensityMeasure, 
	//  IfcNormalisedRatioMeasure, IfcComplexNumber, IfcNonNegativeLengthMeasure); END_TYPE;

	shared_ptr<IfcVolumeMeasure> volume = dynamic_pointer_cast<IfcVolumeMeasure>(ifc_measure);
	if (volume)
	{
		prop_value = QString::number(volume->m_value).toStdWString();
		return;
	}

	shared_ptr<IfcTimeMeasure> time = dynamic_pointer_cast<IfcTimeMeasure>(ifc_measure);
	if (time)
	{
		prop_value = QString::number(time->m_value).toStdWString();
		return;
	}

	shared_ptr<IfcLengthMeasure> length_measure = dynamic_pointer_cast<IfcLengthMeasure>(ifc_measure);
	if (length_measure)
	{
		prop_value = QString::number(length_measure->m_value).toStdWString();
		return;
	}

	shared_ptr<IfcAreaMeasure> area = dynamic_pointer_cast<IfcAreaMeasure>(ifc_measure);
	if (area)
	{
		prop_value = QString::number(area->m_value).toStdWString();
		return;
	}

	shared_ptr<IfcMassMeasure> mass = dynamic_pointer_cast<IfcMassMeasure>(ifc_measure);
	if (mass)
	{
		prop_value = QString::number(mass->m_value).toStdWString();
		return;
	}


	std::cout << "readIfcMeasureValue:: unimplemented measure: " << EntityFactory::getStringForClassID(ifc_measure->classID()) << std::endl;
}

void readIfcSimpleValue(shared_ptr<IfcSimpleValue>& simple, std::wstring& prop_value)
{
	// TYPE IfcSimpleValue = SELECT	(IfcBoolean	,IfcDate	,IfcDateTime	,IfcDuration	,IfcIdentifier	,IfcInteger	,IfcLabel	,IfcLogical	,IfcReal	,IfcText	,IfcTime	,IfcTimeStamp);
	shared_ptr<IfcBoolean> ifc_boolean = dynamic_pointer_cast<IfcBoolean>(simple);
	if (ifc_boolean)
	{
		if (ifc_boolean->m_value == true)
		{
			prop_value = L"True";
		}
		else
		{
			prop_value = L"False";
		}
		return;
	}

	shared_ptr<IfcDate> date = dynamic_pointer_cast<IfcDate>(simple);
	if (date)
	{
		prop_value = date->m_value;
		return;
	}

	shared_ptr<IfcDateTime> date_and_time = dynamic_pointer_cast<IfcDateTime>(simple);
	if (date_and_time)
	{
		prop_value = date_and_time->m_value;
		return;
	}

	shared_ptr<IfcDuration> duration = dynamic_pointer_cast<IfcDuration>(simple);
	if (duration)
	{
		prop_value = duration->m_value;
		return;
	}

	shared_ptr<IfcIdentifier> identifier = dynamic_pointer_cast<IfcIdentifier>(simple);
	if (identifier)
	{
		prop_value = identifier->m_value;
		return;
	}

	shared_ptr<IfcInteger> ifc_integer = dynamic_pointer_cast<IfcInteger>(simple);
	if (ifc_integer)
	{
		prop_value = QString::number(ifc_integer->m_value).toStdWString();
		return;
	}

	shared_ptr<IfcLabel> label = dynamic_pointer_cast<IfcLabel>(simple);
	if (label)
	{
		prop_value = label->m_value;
		return;
	}

	shared_ptr<IfcLogical> ifc_logical = dynamic_pointer_cast<IfcLogical>(simple);
	if (ifc_logical)
	{
		if (ifc_logical->m_value == LOGICAL_TRUE)
		{
			prop_value = L"True";
		}
		else if (ifc_logical->m_value == LOGICAL_FALSE)
		{
			prop_value = L"False";
		}
		else if (ifc_logical->m_value == LOGICAL_UNKNOWN)
		{
			prop_value = L"Unknown";
		}
		return;
	}

	shared_ptr<IfcReal> ifc_real = dynamic_pointer_cast<IfcReal>(simple);
	if (ifc_real)
	{
		prop_value = QString::number(ifc_real->m_value).toStdWString();
		return;
	}

	shared_ptr<IfcText> ifc_text = dynamic_pointer_cast<IfcText>(simple);
	if (ifc_text)
	{
		prop_value = ifc_text->m_value;
		return;
	}


	shared_ptr<IfcTime> ifc_time = dynamic_pointer_cast<IfcTime>(simple);
	if (ifc_time)
	{
		prop_value = ifc_time->m_value;
		return;
	}

	shared_ptr<IfcTimeStamp> ifc_time_stamp = dynamic_pointer_cast<IfcTimeStamp>(simple);
	if (ifc_time_stamp)
	{
		prop_value = ifc_time_stamp->m_value;
		return;
	}

	std::cout << "readIfcSimpleValue:: unimplemented value: " << EntityFactory::getStringForClassID(simple->classID()) << std::endl;
}


void readIfcProperty(const shared_ptr<IfcPropertySet>& pset, const shared_ptr<IfcProperty>& ifc_prop, std::vector<shared_ptr<PropertyValueContainer> >& vecPropertiesOut)
{
	//ENTITY IfcProperty ABSTRACT SUPERTYPE OF(ONEOF(IfcComplexProperty, IfcSimpleProperty))
	shared_ptr<IfcComplexProperty> complex_property = dynamic_pointer_cast<IfcComplexProperty>(ifc_prop);
	if (complex_property)
	{

		//UsageName	 :	IfcIdentifier;
		//HasProperties	 :	SET [1:?] OF IfcProperty;
	}
	else
	{
		shared_ptr<IfcSimpleProperty> simple_property = dynamic_pointer_cast<IfcSimpleProperty>(ifc_prop);
		if (simple_property)
		{

			//ENTITY IfcSimpleProperty ABSTRACT SUPERTYPE OF(ONEOF(IfcPropertyBoundedValue, IfcPropertyEnumeratedValue, IfcPropertyListValue, IfcPropertyReferenceValue, IfcPropertySingleValue, IfcPropertyTableValue))

			shared_ptr<IfcPropertySingleValue> ifc_prop_single_value = dynamic_pointer_cast<IfcPropertySingleValue>(simple_property);
			if (ifc_prop_single_value)
			{
				std::wstring propertyName;
				if (ifc_prop->m_Name)
				{
					propertyName = ifc_prop->m_Name->m_value;
				}

				std::wstring propertyDescription;
				if (ifc_prop->m_Description)
				{
					propertyDescription = ifc_prop->m_Description->m_value;
				}
				shared_ptr<PropertyValueContainer> pvc(new PropertyValueContainer(pset));
				pvc->m_prop_name = propertyName;
				vecPropertiesOut.push_back(pvc);

				shared_ptr<IfcValue>& nominal_value = ifc_prop_single_value->m_NominalValue;			//optional
				shared_ptr<IfcUnit>& unit = ifc_prop_single_value->m_Unit;								//optional
				// TODO: handle unit

				readIfcValue(nominal_value, pvc);
			}
			else
			{

				shared_ptr<IfcPropertyListValue> ifc_prop_list_value = dynamic_pointer_cast<IfcPropertyListValue>(simple_property);
				if (ifc_prop_list_value)
				{
					std::vector<shared_ptr<IfcValue> >& list_values = ifc_prop_list_value->m_ListValues;				//optional
					//shared_ptr<IfcUnit>							m_Unit;						//optional
					// TODO: handle unit

					for (size_t i_value = 0; i_value < list_values.size(); ++i_value)
					{
						shared_ptr<IfcValue>& prop_list_value = list_values[i_value];

						shared_ptr<PropertyValueContainer> pvc(new PropertyValueContainer(pset));

						if (ifc_prop->m_Name)
						{
							pvc->m_prop_name = ifc_prop->m_Name->m_value;
						}

						if (ifc_prop->m_Description)
						{
							pvc->m_prop_description = ifc_prop->m_Description->m_value;
						}
						vecPropertiesOut.push_back(pvc);

						readIfcValue(prop_list_value, pvc);
					}
				}
				else
				{
					std::cout << "readIfcProperty:: unimplemented simple property: " << EntityFactory::getStringForClassID(simple_property->classID()) << std::endl;
				}
			}
		}
	}
}

void readIfcProperty(const shared_ptr<IfcPropertySet>& pset, const shared_ptr<IfcProperty>& ifc_prop, shared_ptr<PsetContainer>& psetContainer)
{
	//ENTITY IfcProperty ABSTRACT SUPERTYPE OF(ONEOF(IfcComplexProperty, IfcSimpleProperty))
	shared_ptr<IfcComplexProperty> complex_property = dynamic_pointer_cast<IfcComplexProperty>(ifc_prop);
	if (complex_property)
	{

		//UsageName	 :	IfcIdentifier;
		//HasProperties	 :	SET [1:?] OF IfcProperty;
	}
	else
	{
		shared_ptr<IfcSimpleProperty> simple_property = dynamic_pointer_cast<IfcSimpleProperty>(ifc_prop);
		if (simple_property)
		{

			//ENTITY IfcSimpleProperty ABSTRACT SUPERTYPE OF(ONEOF(IfcPropertyBoundedValue, IfcPropertyEnumeratedValue, IfcPropertyListValue, IfcPropertyReferenceValue, IfcPropertySingleValue, IfcPropertyTableValue))

			shared_ptr<IfcPropertySingleValue> ifc_prop_single_value = dynamic_pointer_cast<IfcPropertySingleValue>(simple_property);
			if (ifc_prop_single_value)
			{
				shared_ptr<PropertyValueContainer> pvc(new PropertyValueContainer(pset));

				if (ifc_prop->m_Name)
				{
					pvc->m_prop_name = ifc_prop->m_Name->m_value;
				}

				if (ifc_prop->m_Description)
				{
					pvc->m_prop_description = ifc_prop->m_Description->m_value;
				}
				psetContainer->addProperty(pvc);

				shared_ptr<IfcValue>& nominal_value = ifc_prop_single_value->m_NominalValue;			//optional
				shared_ptr<IfcUnit>& unit = ifc_prop_single_value->m_Unit;								//optional
				// TODO: handle unit

				readIfcValue(nominal_value, pvc);
			}
			else
			{

				shared_ptr<IfcPropertyListValue> ifc_prop_list_value = dynamic_pointer_cast<IfcPropertyListValue>(simple_property);
				if (ifc_prop_list_value)
				{
					std::vector<shared_ptr<IfcValue> >& list_values = ifc_prop_list_value->m_ListValues;				//optional
					//shared_ptr<IfcUnit>							m_Unit;						//optional
					// TODO: handle unit

					for (size_t i_value = 0; i_value < list_values.size(); ++i_value)
					{
						shared_ptr<IfcValue>& prop_list_value = list_values[i_value];

						shared_ptr<PropertyValueContainer> pvc(new PropertyValueContainer(pset));

						if (ifc_prop->m_Name)
						{
							pvc->m_prop_name = ifc_prop->m_Name->m_value;
						}

						if (ifc_prop->m_Description)
						{
							pvc->m_prop_description = ifc_prop->m_Description->m_value;
						}
						psetContainer->addProperty(pvc);

						readIfcValue(prop_list_value, pvc);
					}
				}
				else
				{
					std::cout << "readIfcProperty:: unimplemented simple property: " << EntityFactory::getStringForClassID(simple_property->classID()) << std::endl;
				}
			}
		}
	}
}

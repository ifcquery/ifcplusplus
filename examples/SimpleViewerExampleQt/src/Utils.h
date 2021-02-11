#pragma once

#include <string>
#include <QAbstractButton>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QTreeWidgetItem>

#include <osg/Group>
#include <ifcpp/IFC4/include/IfcComplexProperty.h>
#include <ifcpp/IFC4/include/IfcDerivedMeasureValue.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4/include/IfcPropertySet.h>
#include <ifcpp/IFC4/include/IfcPropertySingleValue.h>
#include <ifcpp/IFC4/include/IfcSimpleProperty.h>
#include <ifcpp/IFC4/include/IfcText.h>
#include <ifcpp/geometry/Carve/IncludeCarveHeaders.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BasicTypes.h>

class IfcSimpleValue;
class IfcMeasureValue;

class EditableTreeWidgetItem : public QTreeWidgetItem
{
public:
	EditableTreeWidgetItem(shared_ptr<BuildingObject> attribute_value)
	{
		m_attribute_value = attribute_value;
	}
	shared_ptr<BuildingObject> m_attribute_value;
};


int getDPIAdjustedIconSize();
QSize adjustIconSize(QAbstractButton* btn, int additional_width_height = 0);
QString readTextFile(QString path);

std::string getGUID(const shared_ptr<BuildingObject>& ent);
std::wstring getGUID_wstr(const shared_ptr<BuildingObject>& ent);
void hasShape(const shared_ptr<IfcObjectDefinition>& obj_def, bool& has3DShape, bool& has2DShape);
void getChildren(const shared_ptr<IfcObjectDefinition>& object_def, std::vector<shared_ptr<IfcObjectDefinition> >& vec_children);

class SemitransparentWidget : public QWidget
{
public:
	SemitransparentWidget(){}
	SemitransparentWidget(int alfa255) { m_alfa255 = alfa255; }
	int m_alfa255 = 200;

protected:
	void paintEvent(QPaintEvent* /*event*/) override
	{
		QColor backgroundColor = palette().light().color();
		backgroundColor.setAlpha(m_alfa255);
		QPainter customPainter(this);
		customPainter.fillRect(rect(), backgroundColor);
	}
};

std::string wstring2string(const std::wstring& wstr);
std::wstring string2wstring(std::string& str_in);
std::wstring string2wstring(const char* chr_in);
void escapeSingleTicks(std::wstring& step_line);
void escapeSingleTicks(std::string& step_line);

void applyTransparency(osg::Node* start_node, float transparency);

int findVertexIndex(const std::vector<carve::mesh::Vertex<3> >& vec_vertices, const carve::mesh::Vertex<3>* v);

/** matrix operations */
void computeInverse(const carve::math::Matrix& matrix_a, carve::math::Matrix& inv);

vec3 getBBoxCorner(const carve::geom::aabb<3>& bbox, size_t corner);

void autoIncrementFileName(QString& outputFileName, const char* fileExtension, bool& success);

struct PropertyValueContainer
{
	PropertyValueContainer(const shared_ptr<IfcPropertySet>& pSet);
	PropertyValueContainer(const shared_ptr<IfcPropertySet>& pSet, std::wstring& prop_name, std::wstring& prop_description, std::wstring& prop_value, std::wstring& prop_unit, std::wstring& prop_type);
	std::wstring m_pset_name;
	std::wstring m_prop_name;
	std::wstring m_prop_description;
	std::wstring m_prop_value;
	std::wstring m_prop_unit;
	std::wstring m_prop_type;
};

class PsetContainer
{
public:
	std::vector<shared_ptr<PropertyValueContainer> > m_vecProperties;
	void addProperty(shared_ptr<PropertyValueContainer>& propertyContainer);
	void loadRelatedProperties(const shared_ptr<IfcObjectDefinition>& objectDef, shared_ptr<PsetContainer>& self);
};

void readIfcValue(const shared_ptr<IfcValue>& ifc_value, shared_ptr<PropertyValueContainer>& pvc);
void readIfcSimpleValue(shared_ptr<IfcSimpleValue>& simple, std::wstring& prop_value);
void readIfcMeasureValue(shared_ptr<IfcMeasureValue>& ifc_measure, std::wstring& prop_value);
void readIfcProperty(const shared_ptr<IfcPropertySet>& pset, const shared_ptr<IfcProperty>& ifc_prop, shared_ptr<PsetContainer>& psetContainer);
void readIfcProperty(const shared_ptr<IfcPropertySet>& pset, const shared_ptr<IfcProperty>& ifc_prop, std::vector<shared_ptr<PropertyValueContainer> >& vecPropertiesOut);

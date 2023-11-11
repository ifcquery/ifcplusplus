#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QHeaderView>
#include <QLabel>

#include <ifcpp/model/BuildingObject.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/AttributeObject.h>
#include <ifcpp/IFC4X3/include/IfcAreaMeasure.h>
#include <ifcpp/IFC4X3/include/IfcAxis2Placement.h>
#include <ifcpp/IFC4X3/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4X3/include/IfcDate.h>
#include <ifcpp/IFC4X3/include/IfcDateTime.h>
#include <ifcpp/IFC4X3/include/IfcDerivedMeasureValue.h>
#include <ifcpp/IFC4X3/include/IfcDirection.h>
#include <ifcpp/IFC4X3/include/IfcDuration.h>
#include <ifcpp/IFC4X3/include/IfcGeometricRepresentationContext.h>
#include <ifcpp/IFC4X3/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4X3/include/IfcLogical.h>
#include <ifcpp/IFC4X3/include/IfcLabel.h>
#include <ifcpp/IFC4X3/include/IfcMeasureValue.h>
#include <ifcpp/IFC4X3/include/IfcMassMeasure.h>
#include <ifcpp/IFC4X3/include/IfcIdentifier.h>
#include <ifcpp/IFC4X3/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4X3/include/IfcProductDefinitionShape.h>
#include <ifcpp/IFC4X3/include/IfcProject.h>
#include <ifcpp/IFC4X3/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4X3/include/IfcRelAggregates.h>
#include <ifcpp/IFC4X3/include/IfcSpatialStructureElement.h>
#include <ifcpp/IFC4X3/include/IfcText.h>
#include <ifcpp/IFC4X3/include/IfcTime.h>
#include <ifcpp/IFC4X3/include/IfcTimeMeasure.h>
#include <ifcpp/IFC4X3/include/IfcTimeStamp.h>
#include <ifcpp/IFC4X3/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4X3/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4X3/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4X3/include/IfcProfileTypeEnum.h>
#include <ifcpp/IFC4X3/include/IfcPropertyListValue.h>
#include <ifcpp/IFC4X3/include/IfcVolumeMeasure.h>
#include <ifcpp/geometry/GeometryConverter.h>
#include <ifcpp/geometry/GeometryInputData.h>

#include "IfcPlusPlusSystem.h"
#include "ScopedLockAndBlock.h"
#include "EntityAttributeWidget.h"

class EditableTreeWidgetItem : public QTreeWidgetItem
{
public:
	EditableTreeWidgetItem( shared_ptr<BuildingObject> attribute_value )
	{
		m_attribute_value = attribute_value;
	}
	shared_ptr<BuildingObject> m_attribute_value;
};

EntityAttributeWidget::EntityAttributeWidget(IfcPlusPlusSystem* sys, QWidget* parent ) : QWidget(parent), m_system(sys)
{
	// TODO: IFC-Editor: widget to edit entity properties + update of geometry

	m_block_selection_signals = false;
	m_current_item = nullptr;
	m_expand_level = 0;

	m_tree_widget = new QTreeWidget();
	m_tree_widget->setColumnCount( 2 );
	QTreeWidgetItem* header_item = new QTreeWidgetItem();
	header_item->setText( 0, "Name" );
	header_item->setText( 1, "Value/Type(blue)" );
	m_tree_widget->setHeaderItem( header_item );
	
	m_tree_widget->setColumnWidth( 0, 100 );
	m_tree_widget->setColumnWidth( 1, 100 );
	//setColumnWidth( 2, 60 );
	m_tree_widget->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_tree_widget->setIndentation( 8 );

	//QPushButton* expand_level = new QPushButton("[+]");
	QToolButton* btn_expand = new QToolButton();
	btn_expand->setIcon( QIcon( ":img/Expand-plus.png" ) );
	connect( btn_expand,	SIGNAL( clicked() ),			this, SLOT( slotExpandOneClicked() ) );

	//QPushButton* collapse_level = new QPushButton("[-]");
	QToolButton* btn_collapse = new QToolButton();
	btn_collapse->setIcon( QIcon( ":img/Collapse-minus.png" ) );
	connect( btn_collapse,	SIGNAL( clicked() ),			this, SLOT( slotCollapseOneClicked() ) );


	m_computed_property_tree = new QTreeWidget(this);
	m_computed_property_tree->setColumnWidth( 0, 100 );
	m_computed_property_tree->setColumnWidth( 1, 100 );
	m_computed_property_tree->setColumnWidth( 2, 30 );
	m_computed_property_tree->setColumnCount(3);
	m_computed_property_tree->setIndentation( 0 );
	m_computed_property_tree->header()->hide();

	QHBoxLayout* btn_hbox = new QHBoxLayout();
	btn_hbox->addWidget( btn_expand );
	btn_hbox->addWidget( btn_collapse );

	QVBoxLayout* vbox_computed_values = new QVBoxLayout();
	vbox_computed_values->setContentsMargins( 2, 0, 2, 0 );
	vbox_computed_values->addWidget( new QLabel("Computed values:") );
	vbox_computed_values->addWidget( m_computed_property_tree );

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->setContentsMargins( 0, 0, 0, 0 );
#ifdef _DEBUG
	vbox->addLayout( btn_hbox, 0 );
#endif
	vbox->addWidget( m_tree_widget, 1 );
	vbox->addLayout( vbox_computed_values, 0 );
	setLayout( vbox );

	connect(m_tree_widget,	&QTreeWidget::currentItemChanged,		this, &EntityAttributeWidget::slotTreewidgetSelectionChanged);
	connect( m_tree_widget, &QTreeWidget::itemExpanded,				this, &EntityAttributeWidget::slotItemExpanded );
	connect( m_tree_widget, &QTreeWidget::itemChanged,				this, &EntityAttributeWidget::slotItemChanged );
	
	connect( m_system, &IfcPlusPlusSystem::signalObjectsSelected,	this, &EntityAttributeWidget::slotObjectsSelected );
	connect( m_system, &IfcPlusPlusSystem::signalObjectsUnselected,	this, &EntityAttributeWidget::slotObjectsUnselected );
	
	connect( m_system, &IfcPlusPlusSystem::signalModelCleared,		this, &EntityAttributeWidget::slotModelCleared );
	connect( m_system, &IfcPlusPlusSystem::signalModelLoadingStart,	this, &EntityAttributeWidget::slotModelLoadingStart );
	connect( m_system, &IfcPlusPlusSystem::signalModelLoadingDone,	this, &EntityAttributeWidget::slotModelLoadingDone );
}

EntityAttributeWidget::~EntityAttributeWidget(){}

void EntityAttributeWidget::appendAttributeChildren( shared_ptr<BuildingObject>& parent_obj, QTreeWidgetItem* parent_item, bool expand_children )
{
	size_t precision = 15;
	std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes;
	shared_ptr<BuildingEntity> entity = dynamic_pointer_cast<BuildingEntity>( parent_obj );
	size_t num_attributes = 0;
	if( entity )
	{
		entity->getAttributes( vec_attributes );
		num_attributes = vec_attributes.size();
		entity->getAttributesInverse( vec_attributes );
	}

	shared_ptr<AttributeObjectVector> attribute_vec = dynamic_pointer_cast<AttributeObjectVector>( parent_obj );
	if( attribute_vec )
	{
		std::vector<shared_ptr<BuildingObject> >& vec = attribute_vec->m_vec;
		for( size_t ii = 0; ii < vec.size(); ++ii )
		{
			std::string attribute_name = "[" + std::to_string( ii ) + "]";
			shared_ptr<BuildingObject>& attribute_value = vec[ii];
			vec_attributes.push_back( std::make_pair( attribute_name, attribute_value ) );
		}
	}

	for( size_t ii = 0; ii < vec_attributes.size(); ++ii )
	{
		std::string attribute_name = vec_attributes[ii].first;
		shared_ptr<BuildingObject>& attribute_value = vec_attributes[ii].second;

		EditableTreeWidgetItem* item = new EditableTreeWidgetItem( attribute_value );
		item->setText( 0, attribute_name.c_str() );
		if( ii >= num_attributes )
		{
			item->setForeground( 0, QBrush( Qt::gray ) );
		}
		item->setFlags( item->flags() | Qt::ItemIsEditable );
		m_map_item_objects.insert( std::make_pair( item, attribute_value ) );

		if( parent_item == nullptr )
		{
			m_tree_widget->addTopLevelItem( item );
		}
		else
		{
			parent_item->addChild( item );
		}


		if( !attribute_value )
		{
			//const char* att_type = attribute_value->classname();
			item->setText( 1, "-" );
			continue;
		}

		const char* class_name = EntityFactory::getStringForClassID( attribute_value->classID() );
		item->setToolTip( 1, class_name );

		shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>( attribute_value );
		if( cartesian_point )
		{
			//QString object_to_str( QString::fromStdWString( attribute_value->toString() ) );
			//item->setText( 1, object_to_str );
		}

		bool use_to_string = true;
		if( use_to_string )
		{
			//std::wstring object_to_wstr = attribute_value->toString();
			//QString object_to_str( QString::fromStdWString( object_to_wstr ) );
			//item->setText( 1, object_to_str );
		}
		else
		{
			shared_ptr<BoolAttribute> attribute_bool = dynamic_pointer_cast<BoolAttribute>( attribute_value );
			if( attribute_bool )
			{
				if( attribute_bool->m_value )
				{
					item->setText( 1, "true" );
				}
				else
				{
					item->setText( 1, "false" );
				}
				continue;
			}

			shared_ptr<LogicalAttribute> attribute_logical = dynamic_pointer_cast<LogicalAttribute>( attribute_value );
			if( attribute_logical )
			{
				if( attribute_logical->m_value == LogicalEnum::LOGICAL_TRUE )
				{
					item->setText( 1, "true" );
				}
				else if( attribute_logical->m_value == LogicalEnum::LOGICAL_FALSE )
				{
					item->setText( 1, "false" );
				}
				else if( attribute_logical->m_value == LogicalEnum::LOGICAL_UNKNOWN )
				{
					item->setText( 1, "unknown" );
				}
				continue;
			}

			shared_ptr<IntegerAttribute> attribute_int = dynamic_pointer_cast<IntegerAttribute>( attribute_value );
			if( attribute_int )
			{
				item->setText( 1, QString::number( attribute_int->m_value ) );
				continue;
			}

			shared_ptr<RealAttribute> attribute_double = dynamic_pointer_cast<RealAttribute>( attribute_value );
			if( attribute_double )
			{
				item->setText( 1, QString::number( attribute_double->m_value ) );
				continue;
			}

			shared_ptr<StringAttribute> attribute_string = dynamic_pointer_cast<StringAttribute>( attribute_value );
			if( attribute_string )
			{
				item->setText( 1, QString::fromStdString( attribute_string->m_value ) );
				continue;
			}

			shared_ptr<IfcLabel> label = dynamic_pointer_cast<IfcLabel>( attribute_value );
			if( label )
			{
				item->setText( 1, QString::fromStdString( label->m_value ) );
				continue;
			}

			shared_ptr<IfcIdentifier> identifier = dynamic_pointer_cast<IfcIdentifier>( attribute_value );
			if( identifier )
			{
				item->setText( 1, QString::fromStdString( identifier->m_value ) );
				continue;
			}

			shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>( attribute_value );
			if( cartesian_point )
			{
				QString coords_str = "(";

				{
					coords_str += QString::number( cartesian_point->m_Coordinates[0] );
					if( !std::isnan(cartesian_point->m_Coordinates[1] ) )
					{
						coords_str += ", " + QString::number( cartesian_point->m_Coordinates[1] );
						if( !std::isnan(cartesian_point->m_Coordinates[2] ) )
						{
							coords_str += ", " + QString::number( cartesian_point->m_Coordinates[2] );
						}
					}
				}
				coords_str += ")";
				item->setText( 1, coords_str );
				continue;
			}

			shared_ptr<IfcDirection> direction = dynamic_pointer_cast<IfcDirection>( attribute_value );
			if( direction )
			{
				QString coords_str = "(";

				if( direction->m_DirectionRatios.size() > 0 )
				{
					coords_str += QString::number( direction->m_DirectionRatios[0]->m_value );
					if( direction->m_DirectionRatios.size() > 1 )
					{
						coords_str += ", " + QString::number( direction->m_DirectionRatios[1]->m_value );
						if( direction->m_DirectionRatios.size() > 2 )
						{
							coords_str += ", " + QString::number( direction->m_DirectionRatios[2]->m_value );
						}
					}
				}
				coords_str += ")";
				item->setText( 1, coords_str );
				continue;
			}

			shared_ptr<IfcGloballyUniqueId> guid = dynamic_pointer_cast<IfcGloballyUniqueId>( attribute_value );
			if( guid )
			{
				item->setText( 1, QString::fromStdString( guid->m_value ) );
				continue;
			}

			shared_ptr<IfcPositiveLengthMeasure> pos_length_measure = dynamic_pointer_cast<IfcPositiveLengthMeasure>( attribute_value );
			if( pos_length_measure )
			{
				item->setText( 1, QString::number( pos_length_measure->m_value ) );
				continue;
			}

			shared_ptr<IfcLengthMeasure> length_measure = dynamic_pointer_cast<IfcLengthMeasure>( attribute_value );
			if( length_measure )
			{
				QString coords_str = QString::number( length_measure->m_value );
				item->setText( 1, coords_str );
				continue;
			}

			shared_ptr<IfcText> ifc_text = dynamic_pointer_cast<IfcText>( attribute_value );
			if( ifc_text )
			{
				item->setText( 1, QString::fromStdString( ifc_text->m_value ) );
				continue;
			}
		}
		shared_ptr<AttributeObjectVector> attribute_vec = dynamic_pointer_cast<AttributeObjectVector>( attribute_value );
		if( attribute_vec )
		{
			if( attribute_vec->m_vec.size() > 0 )
			{
				const char* vec_content_type = EntityFactory::getStringForClassID( attribute_vec->m_vec[0]->classID() );
				item->setText( 1, QString( vec_content_type ) +"[" + QString::number( attribute_vec->m_vec.size() ) + "]" );
				item->setForeground( 1, QBrush( Qt::blue ) );
				item->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );

			}
			else
			{
				item->setText( 1, "[-]" );
			}
			continue;
		}

		shared_ptr<BuildingEntity> attribute_entity = dynamic_pointer_cast<BuildingEntity>( attribute_value );
		if( attribute_entity )
		{
			//m_map_item_objects.insert( std::make_pair( item, attribute_entity ) );

			std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes_peek;
			attribute_entity->getAttributes( vec_attributes_peek );
			attribute_entity->getAttributesInverse( vec_attributes_peek );
			if( vec_attributes_peek.size() > 0 )
			{
				item->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
			}
			if( !use_to_string )
			{
				item->setText( 1, EntityFactory::getStringForClassID(attribute_entity->classID() ));
			}
			item->setForeground( 1, QBrush( Qt::blue ) );
			continue;
		}

		if( !use_to_string )
		{
			shared_ptr<IfcProfileTypeEnum> profile_type_enum = dynamic_pointer_cast<IfcProfileTypeEnum>( attribute_value );
			if( profile_type_enum )
			{
				std::stringstream strs;
				profile_type_enum->getStepParameter( strs, false, precision );
				item->setText( 1, strs.str().c_str() );
				continue;
			}

			if( attribute_value )
			{
				std::stringstream strs;
				strs << EntityFactory::getStringForClassID(attribute_value->classID());
				item->setText( 1, strs.str().c_str() );
			}
		}
	}
}

void EntityAttributeWidget::clearAttributeTree()
{
	m_map_item_objects.clear();
	m_expanded_entities.clear();
	
	ScopedBlockSignals block1( this, m_block_counter );
	ScopedBoolToTrue block2( m_block_selection_signals, m_block_counter );

	m_tree_widget->clear();
	m_computed_property_tree->clear();
}

void readIfcMeasureValue(shared_ptr<IfcMeasureValue>& ifc_measure, std::string& prop_value)
{
	// TYPE IfcMeasureValue = SELECT (	IfcVolumeMeasure, IfcTimeMeasure,  IfcThermodynamicTemperatureMeasure, IfcSolidAngleMeasure,  IfcPositiveRatioMeasure, 
	//  IfcRatioMeasure, IfcPositivePlaneAngleMeasure, IfcPlaneAngleMeasure, IfcParameterValue, IfcNumericMeasure, IfcMassMeasure, IfcPositiveLengthMeasure, IfcLengthMeasure, 
	//  IfcElectricCurrentMeasure, IfcDescriptiveMeasure, IfcCountMeasure, IfcContextDependentMeasure, IfcAreaMeasure, IfcAmountOfSubstanceMeasure, IfcLuminousIntensityMeasure, 
	//  IfcNormalisedRatioMeasure, IfcComplexNumber, IfcNonNegativeLengthMeasure); END_TYPE;

	shared_ptr<IfcVolumeMeasure> volume = dynamic_pointer_cast<IfcVolumeMeasure>(ifc_measure);
	if( volume )
	{
		prop_value = QString::number(volume->m_value).toStdString();
		return;
	}

	shared_ptr<IfcTimeMeasure> time = dynamic_pointer_cast<IfcTimeMeasure>(ifc_measure);
	if( time )
	{
		prop_value = QString::number(time->m_value).toStdString();
		return;
	}

	shared_ptr<IfcLengthMeasure> length_measure = dynamic_pointer_cast<IfcLengthMeasure>(ifc_measure);
	if( length_measure )
	{
		prop_value = QString::number(length_measure->m_value).toStdString();
		return;
	}

	shared_ptr<IfcAreaMeasure> area = dynamic_pointer_cast<IfcAreaMeasure>(ifc_measure);
	if( area )
	{
		prop_value = QString::number(area->m_value).toStdString();
		return;
	}

	shared_ptr<IfcMassMeasure> mass = dynamic_pointer_cast<IfcMassMeasure>(ifc_measure);
	if( mass )
	{
		prop_value = QString::number(mass->m_value).toStdString();
		return;
	}

	shared_ptr<IfcPlaneAngleMeasure> planeAngle = dynamic_pointer_cast<IfcPlaneAngleMeasure>(ifc_measure);
	if( planeAngle )
	{
		prop_value = std::to_string(planeAngle->m_value);
		return;
	}


	std::cout << "readIfcMeasureValue:: unimplemented measure: " << EntityFactory::getStringForClassID(ifc_measure->classID()) << std::endl;
}

void readIfcSimpleValue(shared_ptr<IfcSimpleValue>& simple, std::string& prop_value)
{
	// TYPE IfcSimpleValue = SELECT	(IfcBoolean	,IfcDate	,IfcDateTime	,IfcDuration	,IfcIdentifier	,IfcInteger	,IfcLabel	,IfcLogical	,IfcReal	,IfcText	,IfcTime	,IfcTimeStamp);
	shared_ptr<IfcBoolean> ifc_boolean = dynamic_pointer_cast<IfcBoolean>(simple);
	if( ifc_boolean )
	{
		if( ifc_boolean->m_value == true )
		{
			prop_value = "True";
		}
		else
		{
			prop_value = "False";
		}
		return;
	}

	shared_ptr<IfcDate> date = dynamic_pointer_cast<IfcDate>(simple);
	if( date )
	{
		prop_value = date->m_value;
		return;
	}

	shared_ptr<IfcDateTime> date_and_time = dynamic_pointer_cast<IfcDateTime>(simple);
	if( date_and_time )
	{
		prop_value = date_and_time->m_value;
		return;
	}

	shared_ptr<IfcDuration> duration = dynamic_pointer_cast<IfcDuration>(simple);
	if( duration )
	{
		prop_value = duration->m_value;
		return;
	}

	shared_ptr<IfcIdentifier> identifier = dynamic_pointer_cast<IfcIdentifier>(simple);
	if( identifier )
	{
		prop_value = identifier->m_value;
		return;
	}

	shared_ptr<IfcInteger> ifc_integer = dynamic_pointer_cast<IfcInteger>(simple);
	if( ifc_integer )
	{
		prop_value = QString::number(ifc_integer->m_value).toStdString();
		return;
	}

	shared_ptr<IfcLabel> label = dynamic_pointer_cast<IfcLabel>(simple);
	if( label )
	{
		prop_value = label->m_value;
		return;
	}

	shared_ptr<IfcLogical> ifc_logical = dynamic_pointer_cast<IfcLogical>(simple);
	if( ifc_logical )
	{
		if( ifc_logical->m_value == LOGICAL_TRUE )
		{
			prop_value = "True";
		}
		else if( ifc_logical->m_value == LOGICAL_FALSE )
		{
			prop_value = "False";
		}
		else if( ifc_logical->m_value == LOGICAL_UNKNOWN )
		{
			prop_value = "Unknown";
		}
	}

	shared_ptr<IfcReal> ifc_real = dynamic_pointer_cast<IfcReal>(simple);
	if( ifc_real )
	{
		prop_value = QString::number(ifc_real->m_value).toStdString();
		return;
	}

	shared_ptr<IfcText> ifc_text = dynamic_pointer_cast<IfcText>(simple);
	if( ifc_text )
	{
		prop_value = ifc_text->m_value;
		return;
	}


	shared_ptr<IfcTime> ifc_time = dynamic_pointer_cast<IfcTime>(simple);
	if( ifc_time )
	{
		prop_value = ifc_time->m_value;
		return;
	}

	shared_ptr<IfcTimeStamp> ifc_time_stamp = dynamic_pointer_cast<IfcTimeStamp>(simple);
	if( ifc_time_stamp )
	{
		prop_value = ifc_time_stamp->m_value;
		return;
	}

	std::cout << "readIfcSimpleValue:: unimplemented value: " << EntityFactory::getStringForClassID(simple->classID()) << std::endl;
}

struct PropertyValueContainer
{
	PropertyValueContainer(const shared_ptr<IfcPropertySet>& pSet);
	PropertyValueContainer(const shared_ptr<IfcPropertySet>& pSet, std::string& prop_name, std::string& prop_description, std::string& prop_value, std::string& prop_unit, std::string& prop_type);
	std::string m_pset_name;
	std::string m_prop_name;
	std::string m_prop_description;
	std::string m_prop_value;
	std::string m_prop_unit;
	std::string m_prop_type;
};

PropertyValueContainer::PropertyValueContainer(const shared_ptr<IfcPropertySet>& pSet)
{
	if( pSet )
	{
		if( pSet->m_Name )
		{
			m_pset_name = pSet->m_Name->m_value;
		}
		else if( pSet->m_GlobalId )
		{
			m_pset_name = pSet->m_GlobalId->m_value;
		}
		else
		{
			m_pset_name = "#";
			m_pset_name.append(std::to_string(pSet->m_tag));
		}
	}
}
PropertyValueContainer::PropertyValueContainer(const shared_ptr<IfcPropertySet>& pSet, std::string& prop_name, std::string& prop_description, std::string& prop_value, std::string& prop_unit, std::string& prop_type)
{
	if( pSet )
	{
		if( pSet->m_Name )
		{
			m_pset_name = pSet->m_Name->m_value;
		}
		else if( pSet->m_GlobalId )
		{
			m_pset_name = pSet->m_GlobalId->m_value;
		}
		else
		{
			m_pset_name = "#";
			m_pset_name.append(std::to_string(pSet->m_tag));
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
	if( ifc_value )
	{
		// TYPE IfcValue = SELECT (IfcMeasureValue, IfcSimpleValue, IfcDerivedMeasureValue);

		std::string property_value_str = "";

		shared_ptr<IfcMeasureValue> nominal_value_measure = dynamic_pointer_cast<IfcMeasureValue>(ifc_value);
		if( nominal_value_measure )
		{
			readIfcMeasureValue(nominal_value_measure, pvc->m_prop_value);
		}
		else
		{
			shared_ptr<IfcSimpleValue> nominal_value_simple = dynamic_pointer_cast<IfcSimpleValue>(ifc_value);
			if( nominal_value_simple )
			{

				readIfcSimpleValue(nominal_value_simple, pvc->m_prop_value);
			}
			else
			{
				shared_ptr<IfcDerivedMeasureValue> derived_value = dynamic_pointer_cast<IfcDerivedMeasureValue>(ifc_value);
				if( derived_value )
				{
				}
				std::cout << "readIfcProperty:: unimplemented simple property: " << EntityFactory::getStringForClassID(ifc_value->classID()) << std::endl;
			}
		}
	}
}

void readIfcProperty(const shared_ptr<IfcPropertySet>& pset, const shared_ptr<IfcProperty>& ifc_prop, std::vector<shared_ptr<PropertyValueContainer> >& vecPropertiesOut)
{
	//ENTITY IfcProperty ABSTRACT SUPERTYPE OF(ONEOF(IfcComplexProperty, IfcSimpleProperty))
	shared_ptr<IfcComplexProperty> complex_property = dynamic_pointer_cast<IfcComplexProperty>(ifc_prop);
	if( complex_property )
	{

		//UsageName	 :	IfcIdentifier;
		//HasProperties	 :	SET [1:?] OF IfcProperty;
	}
	else
	{
		shared_ptr<IfcSimpleProperty> simple_property = dynamic_pointer_cast<IfcSimpleProperty>(ifc_prop);
		if( simple_property )
		{

			//ENTITY IfcSimpleProperty ABSTRACT SUPERTYPE OF(ONEOF(IfcPropertyBoundedValue, IfcPropertyEnumeratedValue, IfcPropertyListValue, IfcPropertyReferenceValue, IfcPropertySingleValue, IfcPropertyTableValue))

			shared_ptr<IfcPropertySingleValue> ifc_prop_single_value = dynamic_pointer_cast<IfcPropertySingleValue>(simple_property);
			if( ifc_prop_single_value )
			{
				std::string propertyName;
				if( ifc_prop->m_Name )
				{
					propertyName = ifc_prop->m_Name->m_value;
				}

				std::string propertyDescription;
				if( ifc_prop->m_Specification )
				{
					propertyDescription = ifc_prop->m_Specification->m_value;
				}
				shared_ptr<PropertyValueContainer> pvc(new PropertyValueContainer(pset));
				pvc->m_prop_name = propertyName;
				vecPropertiesOut.push_back( pvc );

				shared_ptr<IfcValue>& nominal_value = ifc_prop_single_value->m_NominalValue;			//optional
				shared_ptr<IfcUnit>& unit = ifc_prop_single_value->m_Unit;								//optional
				// TODO: handle unit

				readIfcValue(nominal_value, pvc);
			}
			else
			{

				shared_ptr<IfcPropertyListValue> ifc_prop_list_value = dynamic_pointer_cast<IfcPropertyListValue>(simple_property);
				if( ifc_prop_list_value )
				{
					std::vector<shared_ptr<IfcValue> >& list_values = ifc_prop_list_value->m_ListValues;				//optional
					//shared_ptr<IfcUnit>							m_Unit;						//optional
					// TODO: handle unit

					for( size_t i_value = 0; i_value < list_values.size(); ++i_value )
					{
						shared_ptr<IfcValue>& prop_list_value = list_values[i_value];

						shared_ptr<PropertyValueContainer> pvc(new PropertyValueContainer(pset));

						if( ifc_prop->m_Name )
						{
							pvc->m_prop_name = ifc_prop->m_Name->m_value;
						}

						if( ifc_prop->m_Specification )
						{
							pvc->m_prop_description = ifc_prop->m_Specification->m_value;
						}
						vecPropertiesOut.push_back(pvc);

						readIfcValue(prop_list_value, pvc);
					}
				}
				else
				{
					std::cout << "readIfcProperty:: unimplemented simple property: " <<EntityFactory::getStringForClassID( simple_property->classID()) << std::endl;
				}
			}
		}
	}
}

void computeShapeMetrics(const shared_ptr<ItemShapeData>& shape_input_data, double& volume, double& surface_area)
{
	if (!shape_input_data)
	{
		return;
	}
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vec_meshsets = shape_input_data->m_meshsets;
	for (size_t jj = 0; jj < vec_meshsets.size(); ++jj)
	{
		shared_ptr<carve::mesh::MeshSet<3> >& meshset = vec_meshsets[jj];
		std::vector<carve::mesh::Mesh<3>* >& vec_meshes = meshset->meshes;
		for (size_t kk = 0; kk < vec_meshes.size(); ++kk)
		{
			carve::mesh::Mesh<3>* mesh = vec_meshes[kk];
			volume += mesh->volume();

			std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
			for (size_t mm = 0; mm < vec_faces.size(); ++mm)
			{
				carve::mesh::Face<3>* face = vec_faces[mm];
				if (face->nVertices() == 3)
				{
					if (surface_area >= 0)
					{

						carve::mesh::Edge<3>* edge = face->edge;
						if (edge)
						{
							carve::geom::vector<3>& v1 = edge->v1()->v;
							carve::geom::vector<3>& v2 = edge->v2()->v;
							if (edge->next)
							{
								carve::geom::vector<3>& v3 = edge->next->v2()->v;

								carve::geom::vector<3> side1 = v2 - v1;
								carve::geom::vector<3> side2 = v3 - v2;

								double area = (cross(side1, side2).length()) * 0.5;
								surface_area += abs(area);
							}
						}
					}
				}
				else
				{
					surface_area = -1.0;
				}
			}
		}
	}

	for (auto child : shape_input_data->m_child_items)
	{
		computeShapeMetrics(child, volume, surface_area);
	}
}


void computeShapeMetrics(const shared_ptr<ProductShapeData>& shape_input_data, double& volume, double& surface_area)
{
	if (!shape_input_data)
	{
		return;
	}

	for (auto geomItem : shape_input_data->getGeometricItems())
	{
		computeShapeMetrics(geomItem, volume, surface_area);
	}

	for (auto& child : shape_input_data->getChildElements())
	{
		computeShapeMetrics(child, volume, surface_area);
	}
}


void EntityAttributeWidget::slotObjectsSelected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map )
{
	clearAttributeTree();
	m_expand_level = 1;

	if( m_block_selection_signals )
	{
		return;
	}

	if( map.size() < 1 )
	{
		return;
	}

	// take the first object from map and show its attributes
	shared_ptr<BuildingEntity>& entity = (*(map.begin())).second;
	if( !entity )
	{
		return;
	}
	m_current_root = shared_ptr<BuildingObject>( entity );

	shared_ptr<IfcRoot> ifc_root = dynamic_pointer_cast<IfcRoot>(entity);
	if (!ifc_root)
	{
		return;
	}

	bool expand_children = false;
	ScopedBlockSignals block( m_tree_widget, m_block_counter );
	

	QTreeWidgetItem* root_item = new QTreeWidgetItem();
	root_item->setText( 0, EntityFactory::getStringForClassID(m_current_root->classID()) );
	root_item->setFlags(root_item->flags() | Qt::ItemIsEditable);
	m_tree_widget->addTopLevelItem( root_item );
	m_current_item = root_item;
	m_map_item_objects[root_item] = m_current_root;
	appendAttributeChildren( m_current_root, root_item, expand_children );
	root_item->setExpanded( true );
	
	// compute volume and surface area
	double volume = 0;
	double surface_area = 0;
	shared_ptr<GeometryConverter> geom_converter = m_system->m_geometry_converter;
	const std::map<std::string, shared_ptr<ProductShapeData> >& map_shape_input_data = geom_converter->getShapeInputData();
	std::string guid;
	if (ifc_root->m_GlobalId)
	{
		guid = ifc_root->m_GlobalId->m_value;
	}
	
	auto it_find = map_shape_input_data.find( guid );
	if( it_find != map_shape_input_data.end() )
	{
		const shared_ptr<ProductShapeData>& shape_input_data = it_find->second;
		if( shape_input_data )
		{
			computeShapeMetrics(shape_input_data, volume, surface_area);
		}
	}

	if( surface_area > 0 )
	{
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText( 0, "Surface area" );
		item->setText( 1, QString::number( surface_area ) );
		item->setText( 2, QStringLiteral("m²") );
		ScopedBlockSignals block( m_computed_property_tree, m_block_counter );
		m_computed_property_tree->addTopLevelItem( item );
	}
	if( volume > 0 )
	{
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText( 0, "Volume" );
		item->setText( 1, QString::number( volume ) );
		item->setText( 2, QStringLiteral("m³") );
		ScopedBlockSignals block( m_computed_property_tree, m_block_counter );
		m_computed_property_tree->addTopLevelItem( item );
	}


	shared_ptr<IfcObject> selectedEntity = dynamic_pointer_cast<IfcObject>(entity);
	if( selectedEntity )
	{
		// PropertySets sind dem IFC entity mit Hilfe eines Relationsobjektes zugeordnet.
		// Das Relationsobjekt hat einen Pointer auf das IFC Entity und einen auf das PropertySet.
		// Das Entity hat einen inversen Pointer auf das Relationsobjekt, von dort aus ist das PropertySet erreichbar.


		//             pointer                              pointer
		//    Entity    <---     IfcRelDefinesByProperties    ---->      IfcPropertySet
		//              ---->       
		//           inverser Pointer

		const std::vector<weak_ptr<IfcRelDefinesByProperties> >& vec_rel_defines = selectedEntity->m_IsDefinedBy_inverse;
		for( size_t ii = 0; ii < vec_rel_defines.size(); ++ii )
		{
			shared_ptr<IfcRelDefinesByProperties> rel_prop(vec_rel_defines[ii]);

			const std::vector<shared_ptr<IfcObjectDefinition> >& related_objects = rel_prop->m_RelatedObjects;
			shared_ptr<IfcPropertySetDefinitionSelect>& relating_prop_set_def = rel_prop->m_RelatingPropertyDefinition;

			shared_ptr<IfcPropertySetDefinition> property_set_def = dynamic_pointer_cast<IfcPropertySetDefinition>(relating_prop_set_def);
			if( property_set_def )
			{
				shared_ptr<IfcPropertySet> property_set = dynamic_pointer_cast<IfcPropertySet>(property_set_def);
				if( property_set )
				{
					std::string pset_name = "PropertySet";
					if( property_set->m_Name )
					{
						pset_name = property_set->m_Name->m_value;
					}
					// Treeview-Item fuer das aktuelle PropertySet
					QTreeWidgetItem* pset_item = new QTreeWidgetItem();
					pset_item->setText(0, QString::fromStdString(pset_name.c_str()));
					pset_item->setForeground(0, QBrush(Qt::darkGray));

					ScopedBlockSignals block(m_computed_property_tree, m_block_counter);
					m_tree_widget->addTopLevelItem(pset_item);

					// Konvertieren der IFC Properties in eigene Datenstruktur (PropertyValueContainer)
					std::vector<shared_ptr<PropertyValueContainer> > vecPropertiesOut;
					const std::vector<shared_ptr<IfcProperty> >& vec_has_properties = property_set->m_HasProperties;
					for( auto& ifc_prop : vec_has_properties )
					{
						readIfcProperty(property_set, ifc_prop, vecPropertiesOut);
					}

					// Zugriff auf Vector von PropertyValueContainer, Einfuegen von child items in TreeView
					for( auto& propertyContainer : vecPropertiesOut )
					{
						std::string propertyName = propertyContainer->m_prop_name;
						std::string propertyValue = propertyContainer->m_prop_value;

						QTreeWidgetItem* item = new QTreeWidgetItem();
						item->setText(0, QString::fromStdString(propertyName.c_str()));
						item->setText(1, QString::fromStdString(propertyValue.c_str()));
						item->setForeground(0, QBrush(Qt::darkGray));
						pset_item->addChild(item);
					}

					pset_item->setExpanded(true);
				}
			}
		}
	}
}

void EntityAttributeWidget::slotObjectsUnselected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map )
{
	clearAttributeTree();
	m_current_root.reset();
}

void EntityAttributeWidget::slotTreewidgetSelectionChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous )
{
	m_current_item = current;
	m_expand_level = 1;

	return;
	ScopedBoolToTrue block( m_block_selection_signals, m_block_counter );
	const std::map<int,shared_ptr<BuildingEntity> >& map_ifc_objects = m_system->getGeometryConverter()->getBuildingModel()->getMapIfcEntities();
	
	if( previous )
	{
		int id = previous->text(1).toUInt();
		auto it_find = map_ifc_objects.find(id);
		if( it_find != map_ifc_objects.end() )
		{
			shared_ptr<BuildingEntity> ifc_object = it_find->second;
			m_system->setObjectSelected( ifc_object, false );
		}
	}

	if( current )
	{
		int id = current->text(1).toUInt();
		auto it_find = map_ifc_objects.find(id);
		if( it_find != map_ifc_objects.end() )
		{
			shared_ptr<BuildingEntity> ifc_object = it_find->second;
			m_system->setObjectSelected( ifc_object, true );
		}
	}
}

void EntityAttributeWidget::slotItemExpanded(QTreeWidgetItem* item)
{
	std::map<QTreeWidgetItem*, shared_ptr<BuildingObject> >::iterator it_find = m_map_item_objects.find(item);
	if( it_find == m_map_item_objects.end() )
	{
		return;
	}

	shared_ptr<BuildingObject> ifcpp_object = it_find->second;

	bool expand_children = false;
	ScopedBlockSignals block(m_tree_widget, m_block_counter);
	appendAttributeChildren(ifcpp_object, item, expand_children);
}

void EntityAttributeWidget::slotModelCleared()
{
	clearAttributeTree();
}

void EntityAttributeWidget::slotModelLoadingStart()
{
	clearAttributeTree();
}

void EntityAttributeWidget::slotModelLoadingDone()
{
	clearAttributeTree();
}

void EntityAttributeWidget::expandToLevel( QTreeWidgetItem* item, int& current_level, int max_level  )
{
	++current_level;
	for( int i = 0; i < item->childCount(); ++i )
	{
		QTreeWidgetItem* child_item = item->child( i );
		if( child_item->childIndicatorPolicy() == QTreeWidgetItem::ShowIndicator )
		{
			if( current_level <= max_level )
			{
				if( !child_item->isExpanded() )
				{
					child_item->setExpanded( true );
				}

				std::map<QTreeWidgetItem*, shared_ptr<BuildingObject> >::iterator it_find = m_map_item_objects.find( child_item );
				if( it_find != m_map_item_objects.end() )
				{
					shared_ptr<BuildingObject>& ifcpp_obj = it_find->second;
					if( m_expanded_entities.find( ifcpp_obj.get() ) == m_expanded_entities.end() )
					{
						// TODO: expand only if not expanded elsewhere
					}
				}

				
				
				expandToLevel( child_item, current_level, max_level );
				
			}
			else
			{
				if( child_item->isExpanded() )
				{
					child_item->setExpanded( false );
				}
			}
				
			//slotItemExpanded( child_item );
		}
	}
	--current_level;
}

void EntityAttributeWidget::slotExpandOneClicked()
{
	if( m_current_item )
	{
		++m_expand_level;
		int current_level = 0;
		expandToLevel( m_current_item, current_level, m_expand_level );
	}

	//if( m_current_root )
	{
		//clearAttributeTree();
		//bool expand_children = true;
		//std::unordered_set<BuildingObject*> set_already_expanded;
		//ScopedBlockSignals block( m_tree_widget );
		//appendAttributeChildren( m_current_root, nullptr, expand_children, set_already_expanded );
		
	}
}

void EntityAttributeWidget::slotCollapseOneClicked()
{
	if( m_current_item )
	{
		--m_expand_level;
		int current_level = 0;
		expandToLevel( m_current_item, current_level, m_expand_level );
	}
}

void EntityAttributeWidget::slotItemChanged( QTreeWidgetItem* changed_item, int column )
{
	QString changed_text = changed_item->text( column );
	EditableTreeWidgetItem* editable_item = dynamic_cast<EditableTreeWidgetItem*>(changed_item);
	if( editable_item )
	{
		shared_ptr<BuildingObject> attribute_value = editable_item->m_attribute_value;

		shared_ptr<IntegerAttribute> attribute_int = dynamic_pointer_cast<IntegerAttribute>(attribute_value);
		if( attribute_int )
		{
			attribute_int->m_value = changed_text.toInt();
			return;
		}

		shared_ptr<RealAttribute> attribute_double = dynamic_pointer_cast<RealAttribute>(attribute_value);
		if( attribute_double )
		{
			attribute_double->m_value = changed_text.toDouble();
			return;
		}

		shared_ptr<StringAttribute> attribute_string = dynamic_pointer_cast<StringAttribute>(attribute_value);
		if( attribute_string )
		{
			attribute_string->m_value = changed_text.toStdString();
			return;
		}

		shared_ptr<IfcLabel> label = dynamic_pointer_cast<IfcLabel>(attribute_value);
		if( label )
		{
			label->m_value = changed_text.toStdString();
			return;
		}

		shared_ptr<IfcIdentifier> identifier = dynamic_pointer_cast<IfcIdentifier>(attribute_value);
		if( identifier )
		{
			identifier->m_value = changed_text.toStdString();
			return;
		}

		shared_ptr<IfcPositiveLengthMeasure> positive_length_measure = dynamic_pointer_cast<IfcPositiveLengthMeasure>(attribute_value);
		if( positive_length_measure )
		{
			positive_length_measure->m_value = changed_text.toDouble();
			return;
		}

		std::cout << __FUNCTION__ << ": saving changes for attribute not implemented: " << EntityFactory::getStringForClassID(attribute_value->classID()) << std::endl;
	}
}

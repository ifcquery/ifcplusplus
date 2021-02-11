#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QHeaderView>
#include <QLabel>

#include <ifcpp/model/BuildingObject.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/AttributeObject.h>
#include <ifcpp/IFC4/include/IfcAreaMeasure.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcDate.h>
#include <ifcpp/IFC4/include/IfcDateTime.h>
#include <ifcpp/IFC4/include/IfcDerivedMeasureValue.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcDuration.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationContext.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcLogical.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcMeasureValue.h>
#include <ifcpp/IFC4/include/IfcMassMeasure.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4/include/IfcProductDefinitionShape.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcSpatialStructureElement.h>
#include <ifcpp/IFC4/include/IfcText.h>
#include <ifcpp/IFC4/include/IfcTime.h>
#include <ifcpp/IFC4/include/IfcTimeMeasure.h>
#include <ifcpp/IFC4/include/IfcTimeStamp.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcProfileTypeEnum.h>
#include <ifcpp/IFC4/include/IfcPropertyListValue.h>
#include <ifcpp/IFC4/include/IfcVolumeMeasure.h>
#include <ifcpp/geometry/Carve/GeometryConverter.h>
#include <ifcpp/geometry/Carve/GeometryInputData.h>

#include "Utils.h"
#include "IfcPlusPlusSystem.h"
#include "ScopedLockAndBlock.h"
#include "EntityAttributeWidget.h"

class CustomTableWidget : public QTableWidget
{
public:
	QSize sizeHint() const override
	{
		return QSize(40, 20);
	}
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
	m_tree_widget->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_tree_widget->setIndentation( 8 );

	m_computed_property_tree = new CustomTableWidget();
	m_computed_property_tree->horizontalHeader()->hide();
	m_computed_property_tree->verticalHeader()->hide();
	m_computed_property_tree->setColumnCount(3);
	//m_computed_property_tree->horizontalHeader()->setSectionResizeMode(QTableWidget::resizeRowToContents);

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->setContentsMargins( 0, 0, 0, 0 );
	vbox->addWidget(new QLabel(tr("Attributes")), 0);
	vbox->addWidget( m_tree_widget, 10 );
	vbox->addWidget(new QLabel("Computed values:"), 0);
	vbox->addWidget(m_computed_property_tree, 0);
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

		const char* class_name = attribute_value->className();
		item->setToolTip( 1, class_name );

		shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>( attribute_value );
		if( cartesian_point )
		{
			QString object_to_str( QString::fromStdWString( attribute_value->toString() ) );
			item->setText( 1, object_to_str );
		}

		bool use_to_string = true;
		if( use_to_string )
		{
			std::wstring object_to_wstr = attribute_value->toString();
			QString object_to_str( QString::fromStdWString( object_to_wstr ) );
			item->setText( 1, object_to_str );
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
				item->setText( 1, QString::fromStdWString( attribute_string->m_value ) );
				continue;
			}

			shared_ptr<IfcLabel> label = dynamic_pointer_cast<IfcLabel>( attribute_value );
			if( label )
			{
				item->setText( 1, QString::fromStdWString( label->m_value ) );
				continue;
			}

			shared_ptr<IfcIdentifier> identifier = dynamic_pointer_cast<IfcIdentifier>( attribute_value );
			if( identifier )
			{
				item->setText( 1, QString::fromStdWString( identifier->m_value ) );
				continue;
			}

			shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>( attribute_value );
			if( cartesian_point )
			{
				QString coords_str = "(";

				if( cartesian_point->m_Coordinates.size() > 0 )
				{
					coords_str += QString::number( cartesian_point->m_Coordinates[0]->m_value );
					if( cartesian_point->m_Coordinates.size() > 1 )
					{
						coords_str += ", " + QString::number( cartesian_point->m_Coordinates[1]->m_value );
						if( cartesian_point->m_Coordinates.size() > 2 )
						{
							coords_str += ", " + QString::number( cartesian_point->m_Coordinates[2]->m_value );
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
				item->setText( 1, QString::fromStdWString( guid->m_value ) );
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
				item->setText( 1, QString::fromStdWString( ifc_text->m_value ) );
				continue;
			}
		}
		shared_ptr<AttributeObjectVector> attribute_vec = dynamic_pointer_cast<AttributeObjectVector>( attribute_value );
		if( attribute_vec )
		{
			if( attribute_vec->m_vec.size() > 0 )
			{
				const char* vec_content_type = attribute_vec->m_vec[0]->className();
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
				item->setText( 1, attribute_entity->className() );
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
				profile_type_enum->getStepParameter( strs, false );
				item->setText( 1, strs.str().c_str() );
				continue;
			}

			if( attribute_value )
			{
				std::stringstream strs;
				strs << attribute_value->className();
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

void EntityAttributeWidget::slotObjectsSelected( std::map<std::string, shared_ptr<BuildingEntity> >& map )
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
	root_item->setText( 0, m_current_root->className() );
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
	
	std::string guid = getGUID(ifc_root);
	auto it_find = map_shape_input_data.find( guid );
	if( it_find != map_shape_input_data.end() )
	{
		const shared_ptr<ProductShapeData>& shape_input_data = it_find->second;
		if( shape_input_data )
		{
			std::vector<shared_ptr<RepresentationData> >& vec_representations = shape_input_data->m_vec_representations;
			for( auto& representation : vec_representations )
			{
				std::vector<shared_ptr<ItemShapeData> >& vec_items = representation->m_vec_item_data;
				for( size_t ii = 0; ii < vec_items.size(); ++ii )
				{
					shared_ptr<ItemShapeData>& item_data = vec_items[ii];
					std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vec_meshsets = item_data->m_meshsets;
					for( size_t jj = 0; jj < vec_meshsets.size(); ++jj )
					{
						shared_ptr<carve::mesh::MeshSet<3> >& meshset = vec_meshsets[jj];
						std::vector<carve::mesh::Mesh<3>* >& vec_meshes = meshset->meshes;
						for( size_t kk = 0; kk < vec_meshes.size(); ++kk )
						{
							carve::mesh::Mesh<3>* mesh = vec_meshes[kk];
							volume += mesh->volume();

							std::vector<carve::mesh::Face<3>* >& vec_faces = mesh->faces;
							for( size_t mm = 0; mm < vec_faces.size(); ++mm )
							{
								carve::mesh::Face<3>* face = vec_faces[mm];
								if( face->nVertices() == 3 )
								{
									if( surface_area >= 0 )
									{

										carve::mesh::Edge<3>* edge = face->edge;
										if( edge )
										{
											carve::geom::vector<3>& v1 = edge->v1()->v;
											carve::geom::vector<3>& v2 = edge->v2()->v;
											if( edge->next )
											{
												carve::geom::vector<3>& v3 = edge->next->v2()->v;

												carve::geom::vector<3> side1 = v2 - v1;
												carve::geom::vector<3> side2 = v3 - v2;

												double area = ( cross( side1, side2 ).length() )*0.5;
												surface_area += abs( area );
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
				}
			}
		}
	}

	if( surface_area > 0 )
	{
		ScopedBlockSignals block( m_computed_property_tree, m_block_counter );
		m_computed_property_tree->setRowCount(1);
		m_computed_property_tree->setItem(0, 0, new QTableWidgetItem("Surface area") );
		m_computed_property_tree->setItem(0, 1, new QTableWidgetItem(QString::number(surface_area)));
		m_computed_property_tree->setItem(0, 2, new QTableWidgetItem( QString("m") + QChar('²') ));
	}
	if( volume > 0 )
	{
		ScopedBlockSignals block( m_computed_property_tree, m_block_counter );
		m_computed_property_tree->setRowCount(2);
		m_computed_property_tree->setItem(1, 0, new QTableWidgetItem("Volume"));
		m_computed_property_tree->setItem(1, 1, new QTableWidgetItem(QString::number(volume)));
		m_computed_property_tree->setItem(1, 2, new QTableWidgetItem(QString("m") + QChar(179)));
	}

	m_computed_property_tree->setColumnWidth(0, this->width()/3 + 10);
	m_computed_property_tree->setColumnWidth(1, this->width()/3 );
	m_computed_property_tree->setColumnWidth(2, this->width()/3 - 10);

	shared_ptr<IfcObject> selectedEntity = dynamic_pointer_cast<IfcObject>(entity);
	if( selectedEntity )
	{
		// PropertySets are assigned to IFC objects with a relation object

		//             pointer                              pointer
		//    Entity    <---     IfcRelDefinesByProperties    ---->      IfcPropertySet
		//              ---->       
		//           inverser Pointer

		const std::vector<weak_ptr<IfcRelDefinesByProperties> >& vec_rel_defines = selectedEntity->m_IsDefinedBy_inverse;
		for( size_t ii = 0; ii < vec_rel_defines.size(); ++ii )
		{
			shared_ptr<IfcRelDefinesByProperties> rel_prop(vec_rel_defines[ii]);

			
			shared_ptr<IfcPropertySetDefinitionSelect>& relating_prop_set_def = rel_prop->m_RelatingPropertyDefinition;

			shared_ptr<IfcPropertySetDefinition> property_set_def = dynamic_pointer_cast<IfcPropertySetDefinition>(relating_prop_set_def);
			if( property_set_def )
			{
				shared_ptr<IfcPropertySet> property_set = dynamic_pointer_cast<IfcPropertySet>(property_set_def);
				if( property_set )
				{
					std::wstring pset_name = L"PropertySet";
					if( property_set->m_Name )
					{
						pset_name = property_set->m_Name->m_value;
					}
					// Treeview item for current object
					QTreeWidgetItem* pset_item = new QTreeWidgetItem();
					pset_item->setText(0, QString::fromStdWString(pset_name.c_str()));
					pset_item->setForeground(0, QBrush(Qt::darkGray));

					ScopedBlockSignals block(m_computed_property_tree, m_block_counter);
					m_tree_widget->addTopLevelItem(pset_item);

					std::vector<shared_ptr<PropertyValueContainer> > vecPropertiesOut;
					const std::vector<shared_ptr<IfcProperty> >& vec_has_properties = property_set->m_HasProperties;
					for( auto& ifc_prop : vec_has_properties )
					{
						//void readIfcProperty(const shared_ptr<IfcPropertySet>& pset, const shared_ptr<IfcProperty>& ifc_prop, shared_ptr<PsetContainer>& psetContainer)
						readIfcProperty(property_set, ifc_prop, vecPropertiesOut);
					}

					for( auto& propertyContainer : vecPropertiesOut )
					{
						std::wstring propertyName = propertyContainer->m_prop_name;
						std::wstring propertyValue = propertyContainer->m_prop_value;

						QTreeWidgetItem* item = new QTreeWidgetItem();
						item->setText(0, QString::fromStdWString(propertyName.c_str()));
						item->setText(1, QString::fromStdWString(propertyValue.c_str()));
						item->setForeground(0, QBrush(Qt::darkGray));
						pset_item->addChild(item);
					}

					pset_item->setExpanded(true);
				}
			}
		}
	}
}

void EntityAttributeWidget::slotObjectsUnselected( std::map<std::string, shared_ptr<BuildingEntity> >& map )
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
	const std::map<int,shared_ptr<BuildingEntity> >& map_ifc_objects = m_system->m_geometry_converter->getBuildingModel()->getMapIfcEntities();
	
	if( previous )
	{
		int id = previous->text(1).toUInt();
		auto it_find = map_ifc_objects.find(id);
		if( it_find != map_ifc_objects.end() )
		{
			shared_ptr<BuildingEntity> ifc_object = it_find->second;
			m_system->setObjectSelected( ifc_object, false, ViewerUtils::SELECTION_TRIGGER_TREEVIEW );
		}
	}

	if( current )
	{
		int id = current->text(1).toUInt();
		auto it_find = map_ifc_objects.find(id);
		if( it_find != map_ifc_objects.end() )
		{
			shared_ptr<BuildingEntity> ifc_object = it_find->second;
			m_system->setObjectSelected( ifc_object, true, ViewerUtils::SELECTION_TRIGGER_TREEVIEW);
		}
	}
}

void EntityAttributeWidget::slotItemExpanded(QTreeWidgetItem* item)
{
	auto it_find = m_map_item_objects.find(item);
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

void EntityAttributeWidget::slotItemChanged( QTreeWidgetItem* changed_item, int column )
{
	QString changed_text = changed_item->text( column );
	EditableTreeWidgetItem* editable_item = dynamic_cast<EditableTreeWidgetItem*>(changed_item);
	if( editable_item )
	{
		shared_ptr<BuildingObject> attribute_value = editable_item->m_attribute_value;

		// possibility to change values in the IFC model here
		
	}
}

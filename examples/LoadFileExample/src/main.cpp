#include <unordered_set>
#include <ifcpp/IFC4X3/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4X3/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4X3/include/IfcLabel.h>
#include <ifcpp/IFC4X3/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4X3/include/IfcProject.h>
#include <ifcpp/IFC4X3/include/IfcRelAggregates.h>
#include <ifcpp/IFC4X3/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4X3/include/IfcText.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include <ifcpp/geometry/GeometryConverter.h>

using namespace IFC4X3;

class MyIfcTreeItem
{
public:
	MyIfcTreeItem() {}
	std::string m_name;
	std::string m_description;
	std::string m_entity_guid;
	std::string m_ifc_class_name;
	std::vector<shared_ptr<MyIfcTreeItem> > m_children;
};

static std::map<std::string, int> mapProgressChars;
class MessageHandler
{
public:
	MessageHandler()
	{

	}

	void slotMessageWrapper(shared_ptr<StatusCallback::Message> m)
	{
		{
			// log file etc can be implemented here
			std::lock_guard<std::mutex> lock(m_mutex_messages);

			StatusCallback::MessageType mType = m->m_message_type;
			if( mType == StatusCallback::MESSAGE_TYPE_PROGRESS_VALUE )
			{
				std::string progressType = m->m_progress_type;

#ifdef _MSC_VER
				int progressChars = 0;
				if( mapProgressChars.find(progressType) == mapProgressChars.end() )
				{
					mapProgressChars[progressType] = 0;
				}
				else
				{
					progressChars = mapProgressChars[progressType];
				}

				int progressPercent = int(m->m_progress_value * 100);
				for( int ii = 0; ii < progressChars; ++ii )
				{
					std::cout << '\b';
				}

				std::stringstream strs;
				strs << progressPercent << "%";

				mapProgressChars[progressType] = strs.str().size();
				std::cout << strs.str();
#endif
			}
		}
	}

	std::mutex m_mutex_messages;
};

shared_ptr<MyIfcTreeItem> resolveTreeItems(shared_ptr<BuildingObject> obj, std::unordered_set<int>& set_visited)
{
	shared_ptr<MyIfcTreeItem> item;
	
	shared_ptr<IfcObjectDefinition> obj_def = dynamic_pointer_cast<IfcObjectDefinition>(obj);
	if (obj_def)
	{
		if (set_visited.find(obj_def->m_tag) != set_visited.end())
		{
			return nullptr;
		}
		set_visited.insert(obj_def->m_tag);

		item = std::shared_ptr<MyIfcTreeItem>(new MyIfcTreeItem());
		item->m_ifc_class_name = EntityFactory::getStringForClassID(obj_def->classID());

		// access some attributes of IfcObjectDefinition
		if (obj_def->m_GlobalId)
		{
			item->m_entity_guid = obj_def->m_GlobalId->m_value;
		}

		if (obj_def->m_Name)
		{
			item->m_name = obj_def->m_Name->m_value;
		}

		if (obj_def->m_Description)
		{
			item->m_description = obj_def->m_Description->m_value;
		}
		
		// check if there are child elements of current IfcObjectDefinition
		// child elements can be related to current IfcObjectDefinition either by IfcRelAggregates, or IfcRelContainedInSpatialStructure, see IFC doc
		if (obj_def->m_IsDecomposedBy_inverse.size() > 0)
		{
			// use inverse attribute to navigate to child elements:
			std::vector<weak_ptr<IfcRelAggregates> >& vec_IsDecomposedBy = obj_def->m_IsDecomposedBy_inverse;
			for ( auto it = vec_IsDecomposedBy.begin(); it != vec_IsDecomposedBy.end(); ++it)
			{
				shared_ptr<IfcRelAggregates> rel_agg(*it);
				std::vector<shared_ptr<IfcObjectDefinition> >& vec_related_objects = rel_agg->m_RelatedObjects;
				for (shared_ptr<IfcObjectDefinition> child_obj_def : vec_related_objects)
				{
					shared_ptr<MyIfcTreeItem> child_tree_item = resolveTreeItems(child_obj_def, set_visited);
					if (child_tree_item)
					{
						item->m_children.push_back(child_tree_item);
					}
				}
			}
		}
		
		shared_ptr<IfcSpatialStructureElement> spatial_ele = dynamic_pointer_cast<IfcSpatialStructureElement>(obj_def);
		if (spatial_ele)
		{
			// use inverse attribute to navigate to child elements:
			std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >& vec_contained = spatial_ele->m_ContainsElements_inverse;
			if (vec_contained.size() > 0)
			{
				for (auto it_rel_contained = vec_contained.begin(); it_rel_contained != vec_contained.end(); ++it_rel_contained)
				{
					shared_ptr<IfcRelContainedInSpatialStructure> rel_contained(*it_rel_contained);
					std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained->m_RelatedElements;

					for (shared_ptr<IfcProduct> related_product : vec_related_elements)
					{
						shared_ptr<MyIfcTreeItem> child_tree_item = resolveTreeItems(related_product, set_visited);
						if (child_tree_item)
						{
							item->m_children.push_back(child_tree_item);
						}
					}
				}
			}
		}
	}

	return item;
}

void resolveGeometricItems(shared_ptr<ItemShapeData>& geometricItem, carve::math::Matrix& localTransform)
{
	// closed meshes
	for (auto meshset : geometricItem->m_meshsets)
	{
		std::vector<carve::mesh::Vertex<3> >& vertexData = meshset->vertex_storage;
		for (auto mesh : meshset->meshes)
		{
			for (auto face : mesh->faces)
			{
				carve::mesh::Edge<3>* edge = face->edge;
				for (size_t ii = 0; ii < face->n_edges; ++ii)
				{
					carve::mesh::Vertex<3>* vertex = edge->vert;
					carve::geom::vector<3> pointLocal = vertex->v;
					carve::geom::vector<3> pointGlobal = localTransform * pointLocal;
					double x = pointGlobal.x;
					double y = pointGlobal.y;
					double z = pointGlobal.z;
					std::cout << "point in mesh: (" << x << "/" << y << "/" << z << ")" << std::endl;
				}
			}
		}
	}

	// open meshes
	for (auto meshset : geometricItem->m_meshsets_open)
	{
		std::vector<carve::mesh::Vertex<3> >& vertexData = meshset->vertex_storage;
		for (auto mesh : meshset->meshes)
		{
			for (auto face : mesh->faces)
			{
				carve::mesh::Edge<3>* edge = face->edge;
				for (size_t ii = 0; ii < face->n_edges; ++ii)
				{
					carve::mesh::Vertex<3>* vertex = edge->vert;
					carve::geom::vector<3> pointLocal = vertex->v;
					carve::geom::vector<3> pointGlobal = localTransform * pointLocal;
					double x = pointGlobal.x;
					double y = pointGlobal.y;
					double z = pointGlobal.z;
					std::cout << "point in mesh: (" << x << "/" << y << "/" << z << ")" << std::endl;
				}
			}
		}
	}

	// traverse geometry
	for (auto childItem : geometricItem->m_child_items)
	{
		resolveGeometricItems(childItem, localTransform);
	}
}

void resolveShapeData(shared_ptr<ProductShapeData>& shapeData)
{
	carve::math::Matrix localTransform = shapeData->getTransform();

	// traverse geometry
	for (auto geometricItem : shapeData->getGeometricItems())
	{
		// geometric items can have child items too
		resolveGeometricItems(geometricItem, localTransform);
	}
	
	for (auto child_object : shapeData->getChildElements())
	{
		// child elements in case of IfcBuildingStorey, IfcElementAssembly etc.
		resolveShapeData(child_object);
	}
}
	

int main()
{
	// 1: create an IFC model and a reader for IFC files in STEP format:
	shared_ptr<BuildingModel> ifc_model(new BuildingModel());
	MessageHandler mh;

	shared_ptr<ReaderSTEP> step_reader(new ReaderSTEP());
	step_reader->setMessageCallBack(std::bind(&MessageHandler::slotMessageWrapper, &mh, std::placeholders::_1));
	
	// 2: load the model:
	std::cout << "Loading IFC model: ";
	step_reader->loadModelFromFile( "example.ifc", ifc_model);

	shared_ptr<GeometryConverter> geometry_converter(new GeometryConverter(ifc_model));
	geometry_converter->setMessageCallBack(std::bind(&MessageHandler::slotMessageWrapper, &mh, std::placeholders::_1));
	shared_ptr<GeometrySettings> geom_settings = geometry_converter->getGeomSettings();

	// the number of vertices per circle can be changed here: (default is 14)
	int numVerticesPerCircle = geom_settings->getNumVerticesPerCircle();
	std::cout << std::endl << "numVerticesPerCircle: " << numVerticesPerCircle << std::endl;
	geom_settings->setNumVerticesPerCircle(numVerticesPerCircle);

	// adjust epsilon for boolean operations
	geometry_converter->setCsgEps(1.5e-9);

	// convert IFC geometry representations to Carve meshes
#ifdef _DEBUG
	GeomDebugDump::clearMeshsetDump();
#endif
	std::cout << "Converting IFC geometry: ";
	geometry_converter->convertGeometry();

	// 3: get a flat map of all loaded IFC entities with geometry:
	const std::map<std::string, shared_ptr<ProductShapeData> >& map_entities = geometry_converter->getShapeInputData();
	shared_ptr<ProductShapeData> shapeDataIfcProject;

	for (auto it : map_entities)
	{
		shared_ptr<ProductShapeData> shapeData = it.second;

		if (shapeData->m_ifc_object_definition.expired())
		{
			continue;
		}

		shared_ptr<IfcObjectDefinition> ifcObject = shared_ptr<IfcObjectDefinition>(shapeData->m_ifc_object_definition);

		// check for certain type of the entity:
		shared_ptr<IfcProject> ifc_project = dynamic_pointer_cast<IfcProject>(ifcObject);
		if (ifc_project)
		{
			shapeDataIfcProject = shapeData;
			break;
		}
	}

	if (shapeDataIfcProject)
	{
		resolveShapeData(shapeDataIfcProject);
	}


	// 4: traverse tree structure of model, starting at root object (IfcProject)
	shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
	std::unordered_set<int> set_visited;
	shared_ptr<MyIfcTreeItem> root_item = resolveTreeItems(ifc_project, set_visited);

	// you can access the model as a flat map (step 3), or a tree (step 4), depending on your requirements
}

#include <unordered_set>
#include <IfcBuildingStorey.h>
#include <IfcGloballyUniqueId.h>
#include <IfcLabel.h>
#include <IfcObjectDefinition.h>
#include <IfcProject.h>
#include <IfcRelAggregates.h>
#include <IfcRelContainedInSpatialStructure.h>
#include <IfcText.h>
#include <ifcpp/IFC4X3/EntityFactory.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/reader/ReaderSTEP.h>

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

using namespace IFC4X3;

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

		std::cout << "#" << obj_def->m_tag << "=" << item->m_ifc_class_name << std::endl;

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

int main()
{
	// 1: create an IFC model and a reader for IFC files in STEP format:
	shared_ptr<BuildingModel> ifc_model(new BuildingModel());
	shared_ptr<ReaderSTEP> step_reader(new ReaderSTEP());

	// 2: load the model:
	std::cout << "loading file example.ifc" << std::endl;
	step_reader->loadModelFromFile( "example.ifc", ifc_model);

	// 3: get a flat map of all loaded IFC entities:
	const std::map<int, shared_ptr<BuildingEntity> >& map_entities = ifc_model->getMapIfcEntities();
	std::cout << "loaded " << map_entities.size() << " entities" << std::endl;

	for (auto it : map_entities)
	{
		shared_ptr<BuildingEntity> entity = it.second;
		std::cout << "entity " << EntityFactory::getStringForClassID(entity->classID());
		
		shared_ptr<IfcRoot> ifc_root = dynamic_pointer_cast<IfcRoot>(entity);
		if( ifc_root )
		{
			if( ifc_root->m_GlobalId )
			{
				std::cout << ", guid " << ifc_root->m_GlobalId->m_value;
			}
		}
		std::cout << std::endl;
		
		// check for certain type of the entity:
		shared_ptr<IfcBuildingStorey> ifc_storey = dynamic_pointer_cast<IfcBuildingStorey>(entity);
		if (ifc_storey)
		{
			// access attributes:
			if (ifc_storey->m_GlobalId)
			{
				std::cout << "found IfcBuildingStorey entity with GUID: " << ifc_storey->m_GlobalId->m_value << std::endl;
			}
		}
	}

	// 4: traverse tree structure of model, starting at root object (IfcProject)
	shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
	std::unordered_set<int> set_visited;
	shared_ptr<MyIfcTreeItem> root_item = resolveTreeItems(ifc_project, set_visited);

	// you can access the model as a flat map (step 3), or a tree (step 4), depending on your requirements
}

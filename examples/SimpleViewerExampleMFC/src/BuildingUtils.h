#pragma once
#include <locale>
#include <codecvt>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcSite.h>
#include <ifcpp/IFC4/include/IfcBuilding.h>
#include <ifcpp/IFC4/include/IfcBuildingStorey.h>

class BuildingUtils
{
public:
	/*\brief method getSitesInProject: collects all IfcSite objects that are related to the given IfcProject
	* Yes, this could be done with a template, but that makes it harder to understand, and saves just a few lines of code

	\param[in] ifc_project IfcProject
	\param[out] vec_sites Related IfcSite objects
	*/
	static void getSitesInProject( const shared_ptr<IfcProject>& ifc_project, std::vector<shared_ptr<IfcSite> >& vec_sites )
	{
		if( ifc_project )
		{
			for( auto rel_aggregates_weak : ifc_project->m_IsDecomposedBy_inverse )
			{
				if( rel_aggregates_weak.expired() )
				{
					continue;
				}
				shared_ptr<IfcRelAggregates> rel_aggregates( rel_aggregates_weak );

				for( auto related_object_def : rel_aggregates->m_RelatedObjects )
				{
					if( !related_object_def )
					{
						continue;
					}
					shared_ptr<IfcSite> related_object_as_site = dynamic_pointer_cast<IfcSite>(related_object_def);
					if( related_object_as_site )
					{
						vec_sites.push_back( related_object_as_site );
					}
				}
			}
		}
	}

	/*\brief method getBuildingsInSite: collects all IfcBuilding objects that are related to the given IfcSite

	\param[in] ifc_site IfcSite
	\param[out] vec_buildings Related IfcBuilding objects
	*/
	static void getBuildingsInSite( const shared_ptr<IfcSite>& ifc_site, std::vector<shared_ptr<IfcBuilding> >& vec_buildings )
	{
		if( ifc_site )
		{
			for( auto rel_aggregates_weak : ifc_site->m_IsDecomposedBy_inverse )
			{
				if( rel_aggregates_weak.expired() )
				{
					continue;
				}
				shared_ptr<IfcRelAggregates> rel_aggregates( rel_aggregates_weak );

				for( auto related_object_def : rel_aggregates->m_RelatedObjects )
				{
					if( !related_object_def )
					{
						continue;
					}
					shared_ptr<IfcBuilding> related_object_as_building = dynamic_pointer_cast<IfcBuilding>(related_object_def);
					if( related_object_as_building )
					{
						vec_buildings.push_back( related_object_as_building );
					}
				}
			}
		}
	}

	/*\brief method getBuildingStoreysInBuilding: collects all IfcBuildingStorey objects that are related to the given IfcBuilding

	\param[in] ifc_building IfcBuilding
	\param[out] vec_building_storeys Related IfcBuildingStorey objects
	*/
	static void getBuildingStoreysInBuilding( const shared_ptr<IfcBuilding>& ifc_building, std::vector<shared_ptr<IfcBuildingStorey> >& vec_building_storeys )
	{
		if( ifc_building )
		{
			for( auto rel_aggregates_weak : ifc_building->m_IsDecomposedBy_inverse )
			{
				if( rel_aggregates_weak.expired() )
				{
					continue;
				}
				shared_ptr<IfcRelAggregates> rel_aggregates( rel_aggregates_weak );

				for( auto related_object_def : rel_aggregates->m_RelatedObjects )
				{
					if( !related_object_def )
					{
						continue;
					}
					shared_ptr<IfcBuildingStorey> related_object_as_building_storey = dynamic_pointer_cast<IfcBuildingStorey>(related_object_def);
					if( related_object_as_building_storey )
					{
						vec_building_storeys.push_back( related_object_as_building_storey );
					}
				}
			}
		}
	}

	static void getPartsInBuildingStorey( const shared_ptr<IfcBuildingStorey>& building_storey, std::vector<shared_ptr<IfcProduct> >& vec_related_objects )
	{
		if( !building_storey )
		{
			return;
		}

		for( auto rel_contained_in_spatial_structure_weak : building_storey->m_ContainsElements_inverse )
		{
			if( rel_contained_in_spatial_structure_weak.expired() )
			{
				continue;
			}
			shared_ptr<IfcRelContainedInSpatialStructure> rel_contained_in_spatial_structure( rel_contained_in_spatial_structure_weak );

			const std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained_in_spatial_structure->m_RelatedElements;
			for( auto related_element : vec_related_elements )
			{
				if( !related_element )
				{
					continue;
				}
				vec_related_objects.push_back( related_element );
			}
		}
	}

	/*\brief method getRelatedElements: collects all ObjectDefinitions that are related to the given parent ObjectDefinition

	\param[in] ifc_object_def IfcObjectDefinition
	\param[out] vec_related_objects Related objects
	*/
	static void getRelatedElements( const shared_ptr<IfcObjectDefinition>& ifc_object_def, std::vector<shared_ptr<IfcObjectDefinition> >& vec_related_objects )
	{
		if( !ifc_object_def )
		{
			return;
		}
		for( auto rel_aggregates_weak : ifc_object_def->m_IsDecomposedBy_inverse )
		{
			if( rel_aggregates_weak.expired() )
			{
				continue;
			}
			shared_ptr<IfcRelAggregates> rel_aggregates( rel_aggregates_weak );

			for( const shared_ptr<IfcObjectDefinition>& related_object_def : rel_aggregates->m_RelatedObjects )
			{
				if( !related_object_def )
				{
					continue;
				}
				vec_related_objects.push_back( related_object_def );
			}
		}

		
		const shared_ptr<IfcSpatialElement> spatial_element = dynamic_pointer_cast<IfcSpatialElement>(ifc_object_def);
		if( spatial_element )
		{

			for( auto rel_contained_in_spatial_structure_weak : spatial_element->m_ContainsElements_inverse )
			{
				if( rel_contained_in_spatial_structure_weak.expired() )
				{
					continue;
				}
				shared_ptr<IfcRelContainedInSpatialStructure> rel_contained_in_spatial_structure( rel_contained_in_spatial_structure_weak );

				const std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained_in_spatial_structure->m_RelatedElements;
				for( auto related_element : vec_related_elements )
				{
					if( !related_element )
					{
						continue;
					}
					vec_related_objects.push_back( related_element );
				}
			}
		}
	}

};

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

// #define _DEBUG_LOOP_SEQENTIAL  // define for debugging geometry conversion

#include <map>
#include <thread>
#include <unordered_set>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/reader/ReaderUtil.h>
#include <ifcpp/IFC4X3/include/IfcBuilding.h>
#include <ifcpp/IFC4X3/include/IfcConnectionGeometry.h>
#include <ifcpp/IFC4X3/include/IfcCurtainWall.h>
#include <ifcpp/IFC4X3/include/IfcDistributionElement.h>
#include <ifcpp/IFC4X3/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4X3/include/IfcIndexedColourMap.h>
#include <ifcpp/IFC4X3/include/IfcDistributionPort.h>
#include <ifcpp/IFC4X3/include/IfcPropertySetDefinitionSet.h>
#include <ifcpp/IFC4X3/include/IfcRelAggregates.h>
#include <ifcpp/IFC4X3/include/IfcRelAssigns.h>
#include <ifcpp/IFC4X3/include/IfcRelAssignsToGroup.h>
#include <ifcpp/IFC4X3/include/IfcRelConnectsPortToElement.h>
#include <ifcpp/IFC4X3/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4X3/include/IfcRelDefinesByProperties.h>
#include <ifcpp/IFC4X3/include/IfcRelServicesBuildings.h>
#include <ifcpp/IFC4X3/include/IfcRelSpaceBoundary.h>
#include <ifcpp/IFC4X3/include/IfcShapeRepresentation.h>
#include <ifcpp/IFC4X3/include/IfcSite.h>
#include <ifcpp/IFC4X3/include/IfcSpace.h>
#include <ifcpp/IFC4X3/include/IfcSystem.h>
#include <ifcpp/IFC4X3/include/IfcTypeObject.h>
#include <ifcpp/IFC4X3/include/IfcWindow.h>
#include <ifcpp/IFC4X3/EntityFactory.h>

#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "RepresentationConverter.h"
#include "CSG_Adapter.h"
#include "MeshSimplifier.h"

class GeometryConverter : public StatusCallback
{
protected:
	shared_ptr<BuildingModel>				m_ifc_model;
	shared_ptr<GeometrySettings>			m_geom_settings;
	shared_ptr<RepresentationConverter>		m_representation_converter;

	std::unordered_map<std::string, shared_ptr<ProductShapeData> >	m_product_shape_data;
	std::unordered_map<std::string, shared_ptr<BuildingObject> >	m_map_outside_spatial_structure;
	std::unordered_set<int> m_setResolvedProjectStructure;
	vec3 m_siteOffset;
	double m_recent_progress = 0;
	bool m_convertDirectlyToBuffer = true;
	std::unordered_map<int, std::vector<shared_ptr<StatusCallback::Message> > > m_messages;

	std::mutex m_writelock_messages;
	std::mutex m_writelock_item_cache;
	std::mutex m_writelock_progress;

public:
	// getters and setters
	shared_ptr<BuildingModel>& getBuildingModel() { return m_ifc_model; }
	shared_ptr<RepresentationConverter>& getRepresentationConverter() { return m_representation_converter; }
	shared_ptr<GeometrySettings>& getGeomSettings() { return m_geom_settings; }
	void setGeomSettings(shared_ptr<GeometrySettings>& settings) { m_geom_settings = settings; }
	std::unordered_map<std::string, shared_ptr<ProductShapeData> >& getShapeInputData() { return m_product_shape_data; }
	std::unordered_map<std::string, shared_ptr<BuildingObject> >& getObjectsOutsideSpatialStructure() { return m_map_outside_spatial_structure; }
	bool m_clear_memory_immedeately = true;
	bool m_set_model_to_origin = false;

	//\ brief: ElementConvertedCallback will be called after each IfcProduct geometry is converted, in case you want to directly stream the meshes somewhere
	//using ElementConvertedCallbackType = std::function<void(shared_ptr<ProductShapeData>)>;
	//ElementConvertedCallbackType elementConvertedCallbackHandler;
	//void setElementConvertedCallback(const ElementConvertedCallbackType& cb)
	//{
	//	elementConvertedCallbackHandler = cb;
	//}

	GeometryConverter(shared_ptr<BuildingModel>& ifc_model, shared_ptr<GeometrySettings>& geom_settings)
	{
		m_ifc_model = ifc_model;
		m_geom_settings = geom_settings;
		resetNumVerticesPerCircle();
		shared_ptr<UnitConverter>& unit_converter = m_ifc_model->getUnitConverter();
		m_representation_converter = shared_ptr<RepresentationConverter>(new RepresentationConverter(m_geom_settings, unit_converter));
		m_geom_settings->m_callback_simplify_mesh = MeshSimplifier::simplifyMeshSet;

		// redirect all messages to this->messageTarget
		m_ifc_model->setMessageTarget(this);
		m_representation_converter->setMessageTarget(this);
	}

	void resetModel()
	{
		progressTextCallback("Unloading model, cleaning up memory...");
		clearInputCache();
		m_recent_progress = 0.0;

		m_ifc_model->clearCache();
		m_ifc_model->clearIfcModel();
		progressTextCallback("Unloading model done");
		progressValueCallback(0.0, "parse");

#ifdef _DEBUG
		GeomDebugDump::clearMeshsetDump();
#endif
	}

	void clearInputCache()
	{
		m_product_shape_data.clear();
		m_map_outside_spatial_structure.clear();
		m_setResolvedProjectStructure.clear();
		m_representation_converter->clearCache();
		m_messages.clear();
	}

	void clearIfcRepresentationsInModel(bool resetRepresentationInProducts, bool clearStyles, bool clearIfcElements )
	{
		std::unordered_map<int, shared_ptr<BuildingEntity> >& map_entities = m_ifc_model->getMapIfcEntities();
		for (auto it = map_entities.begin(); it != map_entities.end(); )
		{
			shared_ptr<BuildingEntity>& entity = it->second;
			if(entity)
			{
				shared_ptr<IfcElement> ele = dynamic_pointer_cast<IfcElement>(entity);
				if (ele)
				{
					if (clearIfcElements)
					{
						it = map_entities.erase(it);
						continue;
					}
					else
					{
						++it;
						continue;
					}
				}

				if (resetRepresentationInProducts)
				{
					shared_ptr<IfcProduct> product = dynamic_pointer_cast<IfcProduct>(entity);
					if (product)
					{
						product->m_Representation.reset();
						product->m_ObjectPlacement.reset();
						++it;
						continue;
					}
				}

				shared_ptr<IfcObjectPlacement> plc = dynamic_pointer_cast<IfcObjectPlacement>(entity);
				if (plc)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcProductRepresentation> rep = dynamic_pointer_cast<IfcProductRepresentation>(entity);
				if (rep)
				{
					it = map_entities.erase(it);
					continue;
				}
				
				shared_ptr<IfcRepresentationItem> representationItem = dynamic_pointer_cast<IfcRepresentationItem>(entity);
				if (representationItem)
				{
					shared_ptr<IfcStyledItem> style = dynamic_pointer_cast<IfcStyledItem>(entity);
					if (style)
					{
						if (!clearStyles)
						{
							++it;
							continue;
						}
					}


#if defined _DEBUG || defined _DEBUG_RELEASE
					shared_ptr<BuildingEntity> entityKeepAlive = entity;
					size_t useCount = entityKeepAlive.use_count();
#endif
					// this includes IfcCartesianPoint
					it = map_entities.erase(it);

#if defined _DEBUG || defined _DEBUG_RELEASE
					size_t useCount2 = entityKeepAlive.use_count();
					entityKeepAlive.reset();
					if (useCount2 > 1)
					{
						int notDeletedYet = 1;
					}

					if (useCount2 < 2)
					{
						int deletedHere = 1;
					}

#endif
					continue;
				}

				shared_ptr<IfcProfileDef> prof = dynamic_pointer_cast<IfcProfileDef>(entity);
				if (prof)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcPresentationStyle> style = dynamic_pointer_cast<IfcPresentationStyle>(entity);
				if (style)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcColourSpecification> colour = dynamic_pointer_cast<IfcColourSpecification>(entity);
				if (colour)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcMaterialUsageDefinition> materialUsage = dynamic_pointer_cast<IfcMaterialUsageDefinition>(entity);
				if (materialUsage)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcShapeRepresentation> shapeRep = dynamic_pointer_cast<IfcShapeRepresentation>(entity);
				if (shapeRep)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcRelSpaceBoundary> spaceBoundary = dynamic_pointer_cast<IfcRelSpaceBoundary>(entity);
				if (spaceBoundary)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcRepresentationMap> map = dynamic_pointer_cast<IfcRepresentationMap>(entity);
				if (map)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcConnectionGeometry> conn = dynamic_pointer_cast<IfcConnectionGeometry>(entity);
				if (conn)
				{
					it = map_entities.erase(it);
					continue;
				}

				shared_ptr<IfcGeometricRepresentationContext> context = dynamic_pointer_cast<IfcGeometricRepresentationContext>(entity);
				if (context)
				{
					it = map_entities.erase(it);
					continue;
				}
			}
			++it;
		}
	}

	void resetNumVerticesPerCircle()
	{
		m_geom_settings->resetNumVerticesPerCircle();
	}

	void setCsgEps(double eps)
	{
		m_geom_settings->setEpsilonMergePoints(eps);
	}

	void setModel(shared_ptr<BuildingModel> model)
	{
		if (m_ifc_model)
		{
			m_ifc_model->unsetMessageCallBack();
		}
		clearInputCache();
		m_ifc_model = model;
		m_representation_converter->clearCache();
		m_representation_converter->setUnitConverter(m_ifc_model->getUnitConverter());
		m_ifc_model->setMessageTarget(this);
	}

	void setIfcSiteToOrigin(shared_ptr<IfcSite>& ifc_site)
	{
		if (!ifc_site)
		{
			return;
		}

		if (!ifc_site->m_ObjectPlacement)
		{
			return;
		}

		shared_ptr<IfcLocalPlacement> local_placement = dynamic_pointer_cast<IfcLocalPlacement>(ifc_site->m_ObjectPlacement);
		if (!local_placement)
		{
			return;
		}

		if (local_placement->m_RelativePlacement)
		{
			shared_ptr<IfcAxis2Placement3D> axis_placement = dynamic_pointer_cast<IfcAxis2Placement3D>(local_placement->m_RelativePlacement);
			if (axis_placement)
			{
				if (axis_placement->m_Location)
				{
					shared_ptr<IfcCartesianPoint> placement_location = dynamic_pointer_cast<IfcCartesianPoint>(axis_placement->m_Location);
					if (placement_location)
					{
						double lengthFactor = m_ifc_model->getUnitConverter()->getLengthInMeterFactor();
						PointConverter::convertIfcCartesianPoint(placement_location, m_siteOffset, lengthFactor);

						if (m_siteOffset.length2() > 1000 * 1000)
						{
							for (double& coordinate : placement_location->m_Coordinates)
							{
								if (coordinate)
								{
									coordinate = 0;
								}
							}
						}
					}
					else
					{
						std::cout << "IfcAxis2Placement3D.Location is not an IfcCartesianPoint" << std::endl;
					}
				}
			}
		}
	}

	void resolveProjectStructure(shared_ptr<ProductShapeData>& product_data, bool resolveSecondaryStructure)
	{
		if (!product_data)
		{
			return;
		}
		product_data->m_added_to_spatial_structure = true;

		if (product_data->m_ifc_object_definition.expired())
		{
			return;
		}

		shared_ptr<IfcObjectDefinition> ifc_object_def(product_data->m_ifc_object_definition);
		if (!ifc_object_def)
		{
			return;
		}

#ifdef _DEBUG
		std::string className = IFC4X3::EntityFactory::getStringForClassID(ifc_object_def->classID());
#endif

		for (const weak_ptr<IfcRelAggregates>& relAggregates_weak_ptr : ifc_object_def->m_IsDecomposedBy_inverse)
		{
			if (relAggregates_weak_ptr.expired())
			{
				continue;
			}
			shared_ptr<IfcRelAggregates> relAggregates(relAggregates_weak_ptr);
			if (relAggregates)
			{
				for (const shared_ptr<IfcObjectDefinition>& related_obj_def : relAggregates->m_RelatedObjects)
				{
					if (related_obj_def)
					{
						int tag = related_obj_def->m_tag;
						if (tag < 0)
						{
							std::cout << "invalid tag: " << tag << std::endl;
							continue;
						}

						if (m_setResolvedProjectStructure.find(tag) != m_setResolvedProjectStructure.end())
						{
							continue;
						}
						m_setResolvedProjectStructure.insert(tag);

						if (related_obj_def->m_GlobalId)
						{
							std::string guid = related_obj_def->m_GlobalId->m_value;
							auto it_product_map = m_product_shape_data.find(guid);
							if (it_product_map != m_product_shape_data.end())
							{
								shared_ptr<ProductShapeData>& related_product_shape = it_product_map->second;
								if (related_product_shape)
								{
									product_data->addChildProduct(related_product_shape, product_data);
									resolveProjectStructure(related_product_shape, resolveSecondaryStructure);
								}
							}
						}
					}
				}
			}
		}

		shared_ptr<IfcSpatialElement> spatial_ele = dynamic_pointer_cast<IfcSpatialElement>(ifc_object_def);
		if (spatial_ele)
		{
			for (const weak_ptr<IfcRelContainedInSpatialStructure>& rel_contained_weak_ptr : spatial_ele->m_ContainsElements_inverse)
			{
				if (rel_contained_weak_ptr.expired())
				{
					continue;
				}
				shared_ptr<IfcRelContainedInSpatialStructure> rel_contained(rel_contained_weak_ptr);
				if (rel_contained)
				{
					for (const shared_ptr<IfcProduct>& related_product : rel_contained->m_RelatedElements)
					{
						if (related_product)
						{
							int tag = related_product->m_tag;
							if (tag < 0)
							{
								std::cout << "tag invalid: " << tag << std::endl;
								continue;
							}

							if (m_setResolvedProjectStructure.find(tag) != m_setResolvedProjectStructure.end())
							{
								continue;
							}
							m_setResolvedProjectStructure.insert(tag);

							if (related_product->m_GlobalId)
							{
								std::string related_guid = related_product->m_GlobalId->m_value;

								auto it_product_map = m_product_shape_data.find(related_guid);
								if (it_product_map != m_product_shape_data.end())
								{
									shared_ptr<ProductShapeData>& related_product_shape = it_product_map->second;
									if (related_product_shape)
									{
										product_data->addChildProduct(related_product_shape, product_data);
										resolveProjectStructure(related_product_shape, resolveSecondaryStructure);
									}
								}
							}
						}
					}
				}
			}
		}

		if (resolveSecondaryStructure)
		{
			// handle IfcRelAssigns
			shared_ptr<IfcSpatialElement> ifcSpatial = dynamic_pointer_cast<IfcSpatialElement>(ifc_object_def);
			if (ifcSpatial)
			{
				//std::cout << "ifcSpatial: " << ifcSpatial->m_tag << std::endl;

				//ServicedBySystems	 : 	SET OF IfcRelServicesBuildings FOR RelatedBuildings;
				for (auto servicedBy_weak_ptr : ifcSpatial->m_ServicedBySystems_inverse)
				{

					if (servicedBy_weak_ptr.expired())
					{
						continue;
					}
					shared_ptr<IfcRelServicesBuildings> servicedBy(servicedBy_weak_ptr);
					if (servicedBy)
					{
						//shared_ptr<IfcSystem>						m_RelatingSystem;
						//std::vector<shared_ptr<IfcSpatialElement> >	m_RelatedBuildings;
						shared_ptr<IfcSystem> sys = servicedBy->m_RelatingSystem;
						//std::cout << "m_ServicesBuildings_inverse: " << sys->m_ServicesBuildings_inverse.size() << std::endl;
						//std::cout << "m_ServicesFacilities_inverse: " << sys->m_ServicesFacilities_inverse.size() << std::endl;


						for (auto groupedBy_weak : sys->m_IsGroupedBy_inverse)
						{
							if (groupedBy_weak.expired()) { continue; }
							shared_ptr<IfcRelAssignsToGroup> groupedBy(groupedBy_weak);

							//  std::vector<shared_ptr<IfcObjectDefinition> >	m_RelatedObjects;
							//  shared_ptr<IfcObjectTypeEnum>					m_RelatedObjectsType;		//optional

							// IfcRelAssignsToGroup -----------------------------------------------------------
							// attributes:
							shared_ptr<IfcGroup>& group = groupedBy->m_RelatingGroup;

							for (auto related_object : groupedBy->m_RelatedObjects)
							{
								int tag = related_object->m_tag;
								if (tag < 0)
								{
									std::cout << "tag invalid: " << tag << std::endl;
									continue;
								}

								if (m_setResolvedProjectStructure.find(tag) != m_setResolvedProjectStructure.end())
								{
									// std::cout << "duplicate guid in model tree: " << related_guid << std::endl;
									continue;
								}
								m_setResolvedProjectStructure.insert(tag);

								if (related_object->m_GlobalId)
								{
									std::string related_guid = related_object->m_GlobalId->m_value;

									auto it_product_map = m_product_shape_data.find(related_guid);
									if (it_product_map != m_product_shape_data.end())
									{
										shared_ptr<ProductShapeData>& related_product_shape = it_product_map->second;
										if (related_product_shape)
										{
											product_data->addChildProduct(related_product_shape, product_data);
											resolveProjectStructure(related_product_shape, resolveSecondaryStructure);
										}
									}
								}
							}
						}
					}
				}
			}

			// handle IfcRelConnects
			shared_ptr<IfcDistributionElement> distributionElement = dynamic_pointer_cast<IfcDistributionElement>(ifc_object_def);
			if (distributionElement)
			{
				// #971193= IFCFLOWSEGMENT('2zIAtK02XAfPD15y9EpuCl',#41,'name',$,'description',#971178,#971191,'6021202');  - in spatial structure
				//#4542544= IFCDISTRIBUTIONPORT('0$K3Bu1NXCwviGJbnhv$tO',#41,'name','description',$,#4542542,$,.SOURCEANDSINK.);  
				//#4542546= IFCRELCONNECTSPORTTOELEMENT('0dih3rwKj6FhUYiBG4sVkO',#41,'name','description',#4542544,#971193);

				std::string className = IFC4X3::EntityFactory::getStringForClassID(distributionElement->classID());
				//std::cout << "#" <<  distributionElement->m_tag << "=" << className << " " << std::endl;

				for (auto RelConnectsPortToElement_weak_ptr : distributionElement->m_HasPorts_inverse)
				{
					if (RelConnectsPortToElement_weak_ptr.expired())
					{
						continue;
					}
					shared_ptr<IfcRelConnectsPortToElement> RelConnectsPortToElement(RelConnectsPortToElement_weak_ptr);
					if (RelConnectsPortToElement)
					{
						shared_ptr<IfcPort> related_object = RelConnectsPortToElement->m_RelatingPort;
						if (related_object)
						{
							//shared_ptr<IfcPort>					m_RelatingPort;
							//shared_ptr<IfcDistributionElement>	m_RelatedElement;

							int tag = related_object->m_tag;

							if (tag < 0)
							{
								std::cout << "tag invalid: " << tag << std::endl;
								continue;
							}

							if (m_setResolvedProjectStructure.find(tag) != m_setResolvedProjectStructure.end())
							{
								// std::cout << "duplicate guid in model tree: " << related_guid << std::endl;
								continue;
							}
							m_setResolvedProjectStructure.insert(tag);

							if (related_object->m_GlobalId)
							{
								std::string related_guid = related_object->m_GlobalId->m_value;
								auto it_product_map = m_product_shape_data.find(related_guid);
								if (it_product_map != m_product_shape_data.end())
								{
									shared_ptr<ProductShapeData>& related_product_shape = it_product_map->second;
									if (related_product_shape)
									{
										product_data->addChildProduct(related_product_shape, product_data);
										resolveProjectStructure(related_product_shape, resolveSecondaryStructure);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void fixModelHierarchy()
	{
		// sometimes there are IfcBuilding, not attached to IfcSite and IfcProject
		std::unordered_map<std::string, shared_ptr<ProductShapeData> >& map_shapeInputData = getShapeInputData();
		shared_ptr<BuildingModel> ifc_model = getBuildingModel();
		shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
		std::vector<shared_ptr<IfcSite> > vec_ifc_sites;
		std::vector<shared_ptr<ProductShapeData> > vec_ifc_buildings;

		for (auto it : map_shapeInputData)
		{
			std::string guid = it.first;
			shared_ptr<ProductShapeData>& product_shape = it.second;

			if (product_shape->m_ifc_object_definition.expired())
			{
				continue;
			}

			shared_ptr<IfcObjectDefinition> ifc_object_definition(product_shape->m_ifc_object_definition);
			if (!ifc_object_definition)
			{
				continue;
			}

			if (ifc_object_definition->classID() == IFC4X3::IFCBUILDING)
			{
				shared_ptr<IfcBuilding> ifc_building = dynamic_pointer_cast<IfcBuilding>(ifc_object_definition);
				if (ifc_building)
				{
					vec_ifc_buildings.push_back(product_shape);
					continue;
				}
			}

			if (ifc_object_definition->classID() == IFC4X3::IFCSITE)
			{
				shared_ptr<IfcSite> site = dynamic_pointer_cast<IfcSite>(ifc_object_definition);
				if (site)
				{
					vec_ifc_sites.push_back(site);
					continue;
				}
			}

			if (ifc_object_definition->classID() == IFC4X3::IFCPROJECT)
			{
				shared_ptr<IfcProject> project = dynamic_pointer_cast<IfcProject>(ifc_object_definition);
				if (project)
				{
					ifc_project = project;
					continue;
				}
			}
		}

		// if IfcBuilding is not connected to IfcSite and IfcProject, find IfcSite, and connect it
		for (auto ifc_site : vec_ifc_sites)
		{
			for (auto product_shape_building : vec_ifc_buildings)
			{
				shared_ptr<IfcObjectDefinition> ifc_object_definition(product_shape_building->m_ifc_object_definition);
				if (!ifc_object_definition)
				{
					continue;
				}
				shared_ptr<IfcBuilding> ifc_building = dynamic_pointer_cast<IfcBuilding>(ifc_object_definition);
				if (ifc_building)
				{
					if (ifc_building->m_Decomposes_inverse.size() == 0)
					{
						if (ifc_site->m_IsDecomposedBy_inverse.size() == 0)
						{
							shared_ptr<IfcRelAggregates> site_aggregates(new IfcRelAggregates());
							ifc_model->insertEntity(site_aggregates);
							site_aggregates->m_RelatingObject = ifc_site;

							ifc_site->m_IsDecomposedBy_inverse.push_back(site_aggregates);
						}

						weak_ptr<IfcRelAggregates> site_aggregates_weak = ifc_site->m_IsDecomposedBy_inverse[0];
						if (!site_aggregates_weak.expired())
						{
							shared_ptr<IfcRelAggregates> site_aggregates(site_aggregates_weak);
							site_aggregates->m_RelatingObject = ifc_site;
							site_aggregates->m_RelatedObjects.push_back(ifc_building);

							ifc_building->m_Decomposes_inverse.push_back(site_aggregates);
						}
					}
				}
			}

			if (ifc_project)
			{
				if (ifc_project->m_IsDecomposedBy_inverse.size() == 0)
				{
					shared_ptr<IfcRelAggregates> project_aggregates(new IfcRelAggregates());
					ifc_model->insertEntity(project_aggregates);
					project_aggregates->m_RelatingObject = ifc_project;
					project_aggregates->m_RelatedObjects.push_back(ifc_site);

					ifc_site->m_Decomposes_inverse.push_back(project_aggregates);
					ifc_project->m_IsDecomposedBy_inverse.push_back(project_aggregates);
				}
			}
		}
	}



	/*\brief method convertGeometry: Creates geometry for Carve from previously loaded BuildingModel model.
	**/
	void convertGeometry()
	{
		progressTextCallback("Creating geometry...");
		progressValueCallback(0, "geometry");
		printToDebugLog(__FUNC__, "start converting");
		m_product_shape_data.clear();
		m_map_outside_spatial_structure.clear();
		m_setResolvedProjectStructure.clear();
		m_representation_converter->clearCache();
		m_clear_memory_immedeately = false;

		if (!m_ifc_model)
		{
			return;
		}

		shared_ptr<ProductShapeData> ifcProjectData;
		std::vector<shared_ptr<IfcObjectDefinition> > vecObjectDefinitions;
		getAllObjectDefinitions(vecObjectDefinitions, ifcProjectData);

		// create geometry for for each IfcProduct independently, spatial structure will be resolved later
		const int num_object_definitions = (int)vecObjectDefinitions.size();

		std::mutex writelock_map, writelock_ifc_project, writelock_err;
		int ii = 0;
		FOR_EACH_LOOP vecObjectDefinitions.begin(), vecObjectDefinitions.end(), [&](shared_ptr<IfcObjectDefinition>& object_def) {

				if (m_ifc_model->isLoadingCancelled())
				{
					return;
				}

				const int tag = object_def->m_tag;
				std::string guid;
				if (object_def->m_GlobalId)
				{
					guid = object_def->m_GlobalId->m_value;
				}

				shared_ptr<ProductShapeData> product_geom_input_data = make_shared<ProductShapeData>(guid);
				product_geom_input_data->m_ifc_object_definition = object_def;

				// TODO: check for equal product shapes: each representation and each item must be equal, also openings must be equal: m_HasOpenings_inverse
				uint32_t classID = object_def->classID();
				std::stringstream thread_err;
#ifdef _DEBUG
				std::string className = IFC4X3::EntityFactory::getStringForClassID(classID);
#endif
				if (m_geom_settings->skipRenderObject(classID))
				{
					// IFC type is defined to be filtered out
					return;
				}

				if (classID == IFC4X3::IFCPROJECT)
				{
					std::lock_guard<std::mutex> lock(writelock_ifc_project);
					ifcProjectData = product_geom_input_data;
				}

				try
				{
					convertIfcProductShape(product_geom_input_data);
				}
				catch (BuildingException& e)
				{
					thread_err << e.what();
				}
				catch (carve::exception& e)
				{
					thread_err << e.str();
				}
				catch (std::exception& e)
				{
					thread_err << e.what();
				}
				catch (...)
				{
					thread_err << "undefined error, product id " << tag;
				}

				{
					std::lock_guard<std::mutex> lock(writelock_map);
					m_product_shape_data[guid] = product_geom_input_data;
				}

				if (thread_err.tellp() > 0)
				{
					messageCallback(thread_err.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
				}

				// progress callback
				double progress = (double)ii / (double)num_object_definitions;
				sendProgress(progress);
				++ii;
			});

		// subtract openings in assemblies etc, in case the opening is attached at the top level
		ii = 0;
		FOR_EACH_LOOP vecObjectDefinitions.begin(), vecObjectDefinitions.end(), [&](shared_ptr<IfcObjectDefinition>& object_def) {
			std::string guid;
			if (object_def->m_GlobalId)
			{
				guid = object_def->m_GlobalId->m_value;
			}
			auto it_find = m_product_shape_data.find(guid);
			if (it_find != m_product_shape_data.end())
			{
				shared_ptr<ProductShapeData> product_geom_input_data = it_find->second;
				subtractOpeningsInRelatedObjects(product_geom_input_data);
			}

			double progress = 0.8 + 0.1* (double)ii / (double)num_object_definitions;
			sendProgress(progress);
			++ii;
		});

		if (m_ifc_model->isLoadingCancelled())
		{
			m_product_shape_data.clear();
			return;
		}
				
		try
		{
			// now resolve spatial structure
			if (ifcProjectData)
			{
				fixModelHierarchy();

				// resove spatial structure first (IfcBuilding->IfcBuildingStorey->IfcBuildingElement)
				resolveProjectStructure(ifcProjectData, false);
				// resove secondary structure (IfcRelConnectsPortToElement etc)
				resolveProjectStructure(ifcProjectData, true);
			}

			// check if there are entities that are not in spatial structure
			for (auto it_product_shapes = m_product_shape_data.begin(); it_product_shapes != m_product_shape_data.end(); ++it_product_shapes)
			{
				shared_ptr<ProductShapeData> product_shape = it_product_shapes->second;
				if (!product_shape)
				{
					continue;
				}

				if (!product_shape->m_added_to_spatial_structure)
				{
					if (!product_shape->m_ifc_object_definition.expired())
					{
						shared_ptr<IfcObjectDefinition> ifc_object_def(product_shape->m_ifc_object_definition);

						if (m_geom_settings->skipRenderObject(ifc_object_def->classID()))
						{
							continue;
						}
						std::string guid;

						if (ifc_object_def->m_GlobalId)
						{
							guid = ifc_object_def->m_GlobalId->m_value;
						}

						shared_ptr<IfcTypeObject> typeObject = dynamic_pointer_cast<IfcTypeObject>(ifc_object_def);
						if (typeObject)
						{
							continue;
						}

						if (guid.size() >= 18)
						{
							shared_ptr<IfcRoot> ifc_object_def_as_root = ifc_object_def;
							m_map_outside_spatial_structure[guid] = ifc_object_def_as_root;
						}
						else
						{
							std::cout << "guid invalid: " << guid << std::endl;
						}
					}
				}
			}
		}
		catch (BuildingException& e)
		{
			messageCallback(e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "");
		}
		catch (std::exception& e)
		{
			messageCallback(e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "");
		}
		catch (...)
		{
			messageCallback("undefined error", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		}

		m_representation_converter->clearCache();
		progressTextCallback("Loading file done");
		progressValueCallback(1.0, "geometry");
	}

	void getAllObjectDefinitions(std::vector<shared_ptr<IfcObjectDefinition> >& vecObjectDefinitions, shared_ptr<ProductShapeData>& ifcProjectData)
	{
		std::unordered_map<int, shared_ptr<BuildingEntity> >& map_entities = m_ifc_model->getMapIfcEntities();
		std::unordered_set<std::string> setGuids;
		if (map_entities.size() > 0)
		{
			for (auto it = map_entities.begin(); it != map_entities.end(); )
			{
				shared_ptr<BuildingEntity> entity = it->second;
				if (entity)
				{
					shared_ptr<IfcRoot> root = dynamic_pointer_cast<IfcRoot>(entity);
					if (root)
					{
						std::string guid;
						if (root->m_GlobalId)
						{
							guid = root->m_GlobalId->m_value;

							auto it_find = setGuids.find(guid);
							if (it_find != setGuids.end())
							{
								std::string guid_duplicate = guid;
								size_t guid_append = 1;
								for (auto it = setGuids.begin(); it != setGuids.end(); ++it)
								{
									std::string guid_unique = guid + "_" + std::to_string(guid_append);
									auto it_find2 = setGuids.find(guid_unique);
									if (it_find2 == setGuids.end())
									{
										guid = guid_unique;
										break;
									}
									++guid_append;
								}

								std::string error = "duplicate GUID in model: " + guid_duplicate;
								messageCallback(error, StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);

								root->m_GlobalId->m_value = guid;
							}

							setGuids.insert(it_find, guid);
						}

						shared_ptr<IfcObjectDefinition> object_def = dynamic_pointer_cast<IfcObjectDefinition>(root);
						if (object_def)
						{
							vecObjectDefinitions.push_back(object_def);

							if (m_set_model_to_origin)
							{
								if (object_def->classID() == IFC4X3::IFCSITE)
								{
									shared_ptr<IfcSite> ifc_site = dynamic_pointer_cast<IfcSite>(object_def);
									if (ifc_site)
									{
										setIfcSiteToOrigin(ifc_site);
									}
								}
							}
						}
					}

					if (entity->classID() == IFC4X3::IFCCARTESIANPOINT)
					{
						// IfcCartesianPoint are referenced by IfcFace etc, so we don't need to keep them in the model
						it = map_entities.erase(it);
						continue;
					}

					if (entity->classID() == IFC4X3::IFCPRODUCTREPRESENTATION)
					{
						// IfcCartesianPoint are referenced by IfcFace etc, so we don't need to keep them in the model
						it = map_entities.erase(it);
						continue;
					}

					shared_ptr<IfcPropertyAbstraction> prop = dynamic_pointer_cast<IfcPropertyAbstraction>(entity);
					if (prop)
					{
						// IfcPropertySingleValue etc are referenced in PropertySet, don't need them in mapEntities
						it = map_entities.erase(it);
						continue;
					}

					//////////////  <TEMP>
					shared_ptr<IfcIndexedColourMap> indexedColorMap = dynamic_pointer_cast<IfcIndexedColourMap>(entity);
					if (indexedColorMap)
					{
						it = map_entities.erase(it);
						continue;
					}
					//////////////  </TEMP>

					shared_ptr<IfcRepresentationItem> reItem = dynamic_pointer_cast<IfcRepresentationItem>(entity);
					if (reItem)
					{
						// exclude IFCINDEXEDCOLOURMAP and IFCSTYLEDITEM, which are not referenced by anyone
						shared_ptr<IfcIndexedColourMap> colorMap = dynamic_pointer_cast<IfcIndexedColourMap>(entity);
						if (!colorMap)
						{
							shared_ptr<IfcStyledItem> styledItem = dynamic_pointer_cast<IfcStyledItem>(entity);
							if (!styledItem)
							{
								// IfcRelVoidsElement is not derived from IfcRepresentationItem
								// shared_ptr<IfcRelVoidsElement> relVoids = dynamic_pointer_cast<IfcRelVoidsElement>(entity);

								// enable early free up of memory during geometry processing
								it = map_entities.erase(it);
								continue;
							}
						}
					}
					++it;
				}
			}
		}
	}

	//\brief method convertIfcProduct: Creates geometry objects (meshset with connected vertex-edge-face graph) from an IfcProduct object
	// caution: when using threads, this method runs in parallel threads, so every write access to member variables needs a write lock
	void convertIfcProductShape(shared_ptr<ProductShapeData>& product_shape)
	{
		if (product_shape->m_ifc_object_definition.expired())
		{
			return;
		}

		shared_ptr<IfcObjectDefinition> ifc_object_def(product_shape->m_ifc_object_definition);
		shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(ifc_object_def);
		if (!ifc_product)
		{
			return;
		}

		shared_ptr<IfcProductRepresentation>& product_representation = ifc_product->m_Representation;
		if (!product_representation)
		{
			return;
		}

		// some linux debugging:
		int productTag = ifc_product->m_tag;
		printToDebugLog(__FUNC__, "converting element " + std::to_string(productTag));

		std::vector<weak_ptr<IfcRelVoidsElement> > vec_rel_voids;
		shared_ptr<IfcElement> ifc_element = dynamic_pointer_cast<IfcElement>(ifc_product);
		if (ifc_element)
		{
			vec_rel_voids = ifc_element->m_HasOpenings_inverse;
		}

		bool hasBoolOps = RepresentationConverter::hasBooleanOperations(product_representation);
		if (!hasBoolOps)
		{
			// TODO: cache items
		}

		// convert IFC geometry
		std::vector<shared_ptr<IfcRepresentation> >& vec_representations = product_representation->m_Representations;
		for (size_t i_representations = 0; i_representations < vec_representations.size(); ++i_representations)
		{
			const shared_ptr<IfcRepresentation>& representation = vec_representations[i_representations];
			if (!representation)
			{
				continue;
			}

			try
			{
				shared_ptr<ItemShapeData> representation_data(new ItemShapeData());
				representation_data->m_product = product_shape;
				bool clearIfcItems = false;
				m_representation_converter->convertIfcRepresentation(representation, representation_data, clearIfcItems);
				product_shape->addGeometricItem(representation_data, product_shape);
			}
			catch (BuildingException& e)
			{
				messageCallback(e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "");
			}
			catch (std::exception& e)
			{
				messageCallback(e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "");
			}
		}

		// IfcProduct has an ObjectPlacement that can be local or global
		product_shape->m_object_placement = ifc_product->m_ObjectPlacement;
		if (ifc_product->m_ObjectPlacement)
		{
			// IfcPlacement2Matrix follows related placements in case of local coordinate systems
			std::unordered_set<IfcObjectPlacement*> placement_already_applied;
			m_representation_converter->getPlacementConverter()->convertIfcObjectPlacement(ifc_product->m_ObjectPlacement, product_shape, placement_already_applied, false);
		}

		if (ifc_element)
		{
			// handle openings
			m_representation_converter->subtractOpenings(ifc_element, product_shape);

			// handle styles on IfcElement level
			std::vector<shared_ptr<StyleData> > vec_apperances;
			m_representation_converter->getStylesConverter()->convertElementStyle(ifc_element, vec_apperances);
			for (auto style_data : vec_apperances)
			{
				product_shape->addStyle(style_data);
			}
		}

		m_representation_converter->getStylesConverter()->readStylesFromRelatedObjects(ifc_product, product_shape);

		
		

		// check for existing meshes
		// TODO: make sure that the meshes are not changed after here, for example with boolean operations
		bool enableCaching = false;
		if (enableCaching)
		{
			bool equalItemFound = false;

			for (auto it : m_product_shape_data)
			{
				const shared_ptr<ProductShapeData>& existingProductShape = it.second;
				if (!existingProductShape)
				{
					continue;
				}
				for (auto item : existingProductShape->getChildElements())
				{
					for (auto childItem : item->getChildElements())
					{
						//bool itemIsEqual = isEqual(item, geom_item_data);
						//if( itemIsEqual )
						{
							//representation_data->m_vec_item_data.push_back(existingItem);
							equalItemFound = true;
						}
					}
				}
			}
			if (!equalItemFound)
			{
				//	representation_data->m_vec_item_data.push_back(geom_item_data);
				//	ScopedLock lock( m_writelock_item_cache );
				//	m_map_item_data_cache.push_back(geom_item_data);
			}
		}

		if (m_clear_memory_immedeately)
		{
			int productReferenceCount = ifc_product.use_count();
			if (productReferenceCount < 2)
			{
				ifc_product->m_Representation.reset();
			}
		}
	}

	void subtractOpeningsInRelatedObjects(shared_ptr<ProductShapeData>& product_shape)
	{
		if (product_shape->m_ifc_object_definition.expired())
		{
			return;
		}

		shared_ptr<IfcObjectDefinition> ifc_object_def(product_shape->m_ifc_object_definition);
		shared_ptr<IfcElement> ifc_element = dynamic_pointer_cast<IfcElement>(ifc_object_def);
		if (!ifc_element)
		{
			return;
		}

		if (ifc_element->m_HasOpenings_inverse.size() == 0)
		{
			//return;
		}

		// collect aggregated objects
		const std::vector<weak_ptr<IfcRelAggregates> >& vec_decomposed_by = ifc_element->m_IsDecomposedBy_inverse;

		for (auto& decomposed_by : vec_decomposed_by)
		{
			if (decomposed_by.expired())
			{
				continue;
			}
			shared_ptr<IfcRelAggregates> decomposed_by_aggregates(decomposed_by);
			std::vector<shared_ptr<IfcObjectDefinition> >& vec_related_objects = decomposed_by_aggregates->m_RelatedObjects;
			for (auto& related_object : vec_related_objects)
			{
				if (!related_object)
				{
					continue;
				}

				std::string guid;
				if (related_object->m_GlobalId)
				{
					guid = related_object->m_GlobalId->m_value;

					auto it_find_related_shape = m_product_shape_data.find(guid);
					if (it_find_related_shape != m_product_shape_data.end())
					{
						shared_ptr<ProductShapeData>& related_product_shape = it_find_related_shape->second;
						m_representation_converter->subtractOpenings(ifc_element, related_product_shape);
					}
				}
			}
		}
	}

	void sendProgress(double progress)
	{
		if (progress - m_recent_progress > 0.01)
		{
			std::lock_guard<std::mutex> lock(m_writelock_progress);

			// leave 20% of progress to openings, rendering, file export etc
			progressValueCallback(progress * 0.8, "geometry");
			m_recent_progress = progress;
		}
	}

	virtual void messageTarget(void* ptr, shared_ptr<StatusCallback::Message> m)
	{
		GeometryConverter* myself = (GeometryConverter*)ptr;
		if (myself)
		{
			if (m->m_entity)
			{
				std::lock_guard<std::mutex> lock(m_writelock_messages);

				// make sure that the same message for one entity does not appear several times
				auto ent = dynamic_cast<BuildingEntity*>(m->m_entity);
				if (ent)
				{
					const int tag = ent->m_tag;

					auto it = myself->m_messages.find(tag);
					if (it != myself->m_messages.end())
					{
						std::vector<shared_ptr<StatusCallback::Message> >& vec_message_for_entity = it->second;
						for (size_t i = 0; i < vec_message_for_entity.size(); ++i)
						{
							shared_ptr<StatusCallback::Message>& existing_message = vec_message_for_entity[i];
							if (existing_message->m_message_text.compare(m->m_message_text) == 0)
							{
								// same message for same entity is already there, so ignore message
								return;
							}
						}
						vec_message_for_entity.push_back(m);
					}
					else
					{
						std::vector<shared_ptr<StatusCallback::Message> >& vec = myself->m_messages.insert(std::make_pair(tag, std::vector<shared_ptr<StatusCallback::Message> >())).first->second;
						vec.push_back(m);
					}
				}
			}

			myself->messageCallback(m);
		}
	}
};

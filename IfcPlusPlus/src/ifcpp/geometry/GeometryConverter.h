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

#include <unordered_set>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/OpenMPIncludes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/reader/ReaderUtil.h>
#include <ifcpp/IFC4X3/include/IfcBuilding.h>
#include <ifcpp/IFC4X3/include/IfcCurtainWall.h>
#include <ifcpp/IFC4X3/include/IfcDistributionElement.h>
#include <ifcpp/IFC4X3/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4X3/include/IfcDistributionPort.h>
#include <ifcpp/IFC4X3/include/IfcPropertySetDefinitionSet.h>
#include <ifcpp/IFC4X3/include/IfcRelAggregates.h>
#include <ifcpp/IFC4X3/include/IfcRelAssigns.h>
#include <ifcpp/IFC4X3/include/IfcRelAssignsToGroup.h>
#include <ifcpp/IFC4X3/include/IfcRelConnectsPortToElement.h>
#include <ifcpp/IFC4X3/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4X3/include/IfcRelDefinesByProperties.h>
#include <ifcpp/IFC4X3/include/IfcRelServicesBuildings.h>
#include <ifcpp/IFC4X3/include/IfcSite.h>
#include <ifcpp/IFC4X3/include/IfcSpace.h>
#include <ifcpp/IFC4X3/include/IfcSystem.h>
#include <ifcpp/IFC4X3/include/IfcWindow.h>
#include <ifcpp/IFC4X3/EntityFactory.h>

#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "RepresentationConverter.h"
#include "CSG_Adapter.h"

//#undef _OPENMP   // temp

class GeometryConverter : public StatusCallback
{
protected:
	shared_ptr<BuildingModel>				m_ifc_model;
	shared_ptr<GeometrySettings>			m_geom_settings;
	shared_ptr<RepresentationConverter>		m_representation_converter;

	std::map<std::string, shared_ptr<ProductShapeData> >	m_product_shape_data;
	std::map<std::string, shared_ptr<BuildingObject> >		m_map_outside_spatial_structure;
	std::set<int> m_setResolvedProjectStructure;
	vec3 m_siteOffset;
	double m_recent_progress = 0;
	std::map<int, std::vector<shared_ptr<StatusCallback::Message> > > m_messages;

#ifdef _OPENMP
	Mutex m_writelock_messages;
	Mutex m_writelock_item_cache;
#endif

public:
	// getters and setters
	shared_ptr<BuildingModel>&							getBuildingModel() { return m_ifc_model; }
	shared_ptr<RepresentationConverter>&				getRepresentationConverter() { return m_representation_converter; }
	shared_ptr<GeometrySettings>&						getGeomSettings() { return m_geom_settings; }
	std::map<std::string, shared_ptr<ProductShapeData> >&	getShapeInputData() { return m_product_shape_data; }
	std::map<std::string, shared_ptr<BuildingObject> >&		getObjectsOutsideSpatialStructure() { return m_map_outside_spatial_structure; }
	bool m_clear_memory_immedeately = true;
	bool m_set_model_to_origin = false;

	//\brief Pointer to the object on which the message callback function is called.
	void* m_callback_object_geometry_converted = nullptr;
	void (*m_callback_func_geometry_converted)(void*, shared_ptr<ProductShapeData> t) = nullptr;

	GeometryConverter( shared_ptr<BuildingModel>& ifc_model )
	{
		m_ifc_model = ifc_model;
		m_geom_settings = shared_ptr<GeometrySettings>( new GeometrySettings() );
		resetNumVerticesPerCircle();
		shared_ptr<UnitConverter>& unit_converter = m_ifc_model->getUnitConverter();
		m_representation_converter = shared_ptr<RepresentationConverter>( new RepresentationConverter( m_geom_settings, unit_converter ) );

		// redirect all messages to this->messageTarget
		m_ifc_model->setMessageTarget( this );
		m_representation_converter->setMessageTarget( this );
	}
	virtual ~GeometryConverter()
	{
		m_callback_object_geometry_converted = nullptr;
		m_callback_func_geometry_converted = nullptr;
	}

	void setGeometryCallbackFunction( void* obj_ptr, void (*func)(void*, shared_ptr<ProductShapeData> t) )
	{
		m_callback_object_geometry_converted = obj_ptr;
		m_callback_func_geometry_converted = func;
	}

	void resetModel()
	{
		progressTextCallback( "Unloading model, cleaning up memory..." );
		clearInputCache();
		m_recent_progress = 0.0;

		m_ifc_model->clearCache();
		m_ifc_model->clearIfcModel();
		progressTextCallback( "Unloading model done" );
		progressValueCallback( 0.0, "parse" );

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

	void resetNumVerticesPerCircle()
	{
		m_geom_settings->resetNumVerticesPerCircle();
	}

	void setCsgEps(double eps)
	{
		m_geom_settings->setEpsilonCoplanarDistance(eps);
	}

	void setModel( shared_ptr<BuildingModel> model )
	{
		if( m_ifc_model )
		{
			m_ifc_model->unsetMessageCallBack();
		}
		clearInputCache();
		m_ifc_model = model;
		m_representation_converter->clearCache();
		m_representation_converter->setUnitConverter( m_ifc_model->getUnitConverter() );
		m_ifc_model->setMessageTarget( this );
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

						if( m_siteOffset.length2() > 1000 * 1000 )
						{
							for( double& coordinate : placement_location->m_Coordinates )
							{
								if( coordinate )
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

	void resolveProjectStructure(shared_ptr<ProductShapeData>& product_data, bool resolveSecondaryStructure )
	{
		if( !product_data )
		{
			return;
		}
		product_data->m_added_to_spatial_structure = true;

		if( product_data->m_ifc_object_definition.expired() )
		{
			return;
		}

		shared_ptr<IfcObjectDefinition> ifc_object_def(product_data->m_ifc_object_definition);
		if( !ifc_object_def )
		{
			return;
		}

#ifdef _DEBUG
		std::string className = IFC4X3::EntityFactory::getStringForClassID(ifc_object_def->classID());
#endif

		for( const weak_ptr<IfcRelAggregates>& relAggregates_weak_ptr : ifc_object_def->m_IsDecomposedBy_inverse )
		{
			if( relAggregates_weak_ptr.expired() )
			{
				continue;
			}
			shared_ptr<IfcRelAggregates> relAggregates(relAggregates_weak_ptr);
			if( relAggregates )
			{
				const std::vector<shared_ptr<IfcObjectDefinition> >& vec_related_objects = relAggregates->m_RelatedObjects;
				for( size_t jj = 0; jj < vec_related_objects.size(); ++jj )
				{
					const shared_ptr<IfcObjectDefinition>& related_obj_def = vec_related_objects[jj];
					if( related_obj_def )
					{
						int tag = related_obj_def->m_tag;
						if( tag < 0 )
						{
							std::cout << "invalid tag: " << tag << std::endl;
							continue;
						}

						if( m_setResolvedProjectStructure.find(tag) != m_setResolvedProjectStructure.end() )
						{
							continue;
						}
						m_setResolvedProjectStructure.insert(tag);

						if( related_obj_def->m_GlobalId )
						{
							std::string guid = related_obj_def->m_GlobalId->m_value;
							auto it_product_map = m_product_shape_data.find(guid);
							if( it_product_map != m_product_shape_data.end() )
							{
								shared_ptr<ProductShapeData>& related_product_shape = it_product_map->second;
								if( related_product_shape )
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

		shared_ptr<IfcSpatialStructureElement> spatial_ele = dynamic_pointer_cast<IfcSpatialStructureElement>(ifc_object_def);
		if( spatial_ele )
		{
			const std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >& vec_contains = spatial_ele->m_ContainsElements_inverse;
			for( size_t ii = 0; ii < vec_contains.size(); ++ii )
			{
				const weak_ptr<IfcRelContainedInSpatialStructure>& rel_contained_weak_ptr = vec_contains[ii];
				if( rel_contained_weak_ptr.expired() )
				{
					continue;
				}
				shared_ptr<IfcRelContainedInSpatialStructure> rel_contained(rel_contained_weak_ptr);
				if( rel_contained )
				{
					const std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained->m_RelatedElements;

					for( size_t jj = 0; jj < vec_related_elements.size(); ++jj )
					{
						const shared_ptr<IfcProduct>& related_product = vec_related_elements[jj];
						if( related_product )
						{
							int tag = related_product->m_tag;
							if( tag < 0 )
							{
								std::cout << "tag invalid: " << tag << std::endl;
								continue;
							}

							if( m_setResolvedProjectStructure.find(tag) != m_setResolvedProjectStructure.end() )
							{
								continue;
							}
							m_setResolvedProjectStructure.insert(tag);

							if( related_product->m_GlobalId )
							{
								std::string related_guid = related_product->m_GlobalId->m_value;

								auto it_product_map = m_product_shape_data.find(related_guid);
								if( it_product_map != m_product_shape_data.end() )
								{
									shared_ptr<ProductShapeData>& related_product_shape = it_product_map->second;
									if( related_product_shape )
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

		if( resolveSecondaryStructure )
		{
			// handle IfcRelAssigns
			shared_ptr<IfcSpatialStructureElement> ifcSpatial = dynamic_pointer_cast<IfcSpatialStructureElement>(ifc_object_def);
			if( ifcSpatial )
			{
				//std::cout << "ifcSpatial: " << ifcSpatial->m_tag << std::endl;

				//ServicedBySystems	 : 	SET OF IfcRelServicesBuildings FOR RelatedBuildings;
				for( auto servicedBy_weak_ptr : ifcSpatial->m_ServicedBySystems_inverse )
				{

					if( servicedBy_weak_ptr.expired() )
					{
						continue;
					}
					shared_ptr<IfcRelServicesBuildings> servicedBy(servicedBy_weak_ptr);
					if( servicedBy )
					{
						//shared_ptr<IfcSystem>						m_RelatingSystem;
						//std::vector<shared_ptr<IfcSpatialElement> >	m_RelatedBuildings;
						shared_ptr<IfcSystem> sys = servicedBy->m_RelatingSystem;
						//std::cout << "m_ServicesBuildings_inverse: " << sys->m_ServicesBuildings_inverse.size() << std::endl;
						//std::cout << "m_ServicesFacilities_inverse: " << sys->m_ServicesFacilities_inverse.size() << std::endl;


						for( auto groupedBy_weak : sys->m_IsGroupedBy_inverse )
						{
							if( groupedBy_weak.expired() ) { continue; }
							shared_ptr<IfcRelAssignsToGroup> groupedBy(groupedBy_weak);

							//  std::vector<shared_ptr<IfcObjectDefinition> >	m_RelatedObjects;
							//  shared_ptr<IfcObjectTypeEnum>					m_RelatedObjectsType;		//optional

							// IfcRelAssignsToGroup -----------------------------------------------------------
							// attributes:
							shared_ptr<IfcGroup>& group = groupedBy->m_RelatingGroup;

							for( auto related_object : groupedBy->m_RelatedObjects )
							{
								int tag = related_object->m_tag;
								if( tag < 0 )
								{
									std::cout << "tag invalid: " << tag << std::endl;
									continue;
								}

								if( m_setResolvedProjectStructure.find(tag) != m_setResolvedProjectStructure.end() )
								{
									// std::cout << "duplicate guid in model tree: " << related_guid << std::endl;
									continue;
								}
								m_setResolvedProjectStructure.insert(tag);

								if( related_object->m_GlobalId )
								{
									std::string related_guid = related_object->m_GlobalId->m_value;

									auto it_product_map = m_product_shape_data.find(related_guid);
									if( it_product_map != m_product_shape_data.end() )
									{
										shared_ptr<ProductShapeData>& related_product_shape = it_product_map->second;
										if( related_product_shape )
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
			if( distributionElement )
			{
				// #971193= IFCFLOWSEGMENT('2zIAtK02XAfPD15y9EpuCl',#41,'name',$,'description',#971178,#971191,'6021202');  - in spatial structure
				//#4542544= IFCDISTRIBUTIONPORT('0$K3Bu1NXCwviGJbnhv$tO',#41,'name','description',$,#4542542,$,.SOURCEANDSINK.);  
				//#4542546= IFCRELCONNECTSPORTTOELEMENT('0dih3rwKj6FhUYiBG4sVkO',#41,'name','description',#4542544,#971193);

				std::string className = IFC4X3::EntityFactory::getStringForClassID(distributionElement->classID());
				//std::cout << "#" <<  distributionElement->m_tag << "=" << className << " " << std::endl;

				for( auto RelConnectsPortToElement_weak_ptr : distributionElement->m_HasPorts_inverse )
				{
					if( RelConnectsPortToElement_weak_ptr.expired() )
					{
						continue;
					}
					shared_ptr<IfcRelConnectsPortToElement> RelConnectsPortToElement(RelConnectsPortToElement_weak_ptr);
					if( RelConnectsPortToElement )
					{
						shared_ptr<IfcPort> related_object = RelConnectsPortToElement->m_RelatingPort;
						if( related_object )
						{
							//shared_ptr<IfcPort>					m_RelatingPort;
							//shared_ptr<IfcDistributionElement>	m_RelatedElement;

							int tag = related_object->m_tag;

							if( tag < 0 )
							{
								std::cout << "tag invalid: " << tag << std::endl;
								continue;
							}

							if( m_setResolvedProjectStructure.find(tag) != m_setResolvedProjectStructure.end() )
							{
								// std::cout << "duplicate guid in model tree: " << related_guid << std::endl;
								continue;
							}
							m_setResolvedProjectStructure.insert(tag);

							if( related_object->m_GlobalId )
							{
								std::string related_guid = related_object->m_GlobalId->m_value;
								auto it_product_map = m_product_shape_data.find(related_guid);
								if( it_product_map != m_product_shape_data.end() )
								{
									shared_ptr<ProductShapeData>& related_product_shape = it_product_map->second;
									if( related_product_shape )
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
		std::map<std::string, shared_ptr<ProductShapeData> >& map_shapeInputData = getShapeInputData();
		shared_ptr<BuildingModel> ifc_model = getBuildingModel();
		shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
		std::vector<shared_ptr<IfcSite> > vec_ifc_sites;
		std::vector<shared_ptr<ProductShapeData> > vec_ifc_buildings;

		for( auto it : map_shapeInputData )
		{
			std::string guid = it.first;
			shared_ptr<ProductShapeData>& product_shape = it.second;

			if( product_shape->m_ifc_object_definition.expired() )
			{
				continue;
			}

			shared_ptr<IfcObjectDefinition> ifc_object_definition(product_shape->m_ifc_object_definition);
			if( !ifc_object_definition )
			{
				continue;
			}

			if( ifc_object_definition->classID() == IFC4X3::IFCBUILDING )
			{
				shared_ptr<IfcBuilding> ifc_building = dynamic_pointer_cast<IfcBuilding>(ifc_object_definition);
				if( ifc_building )
				{
					vec_ifc_buildings.push_back(product_shape);
					continue;
				}
			}

			if( ifc_object_definition->classID() == IFC4X3::IFCSITE )
			{
				shared_ptr<IfcSite> site = dynamic_pointer_cast<IfcSite>(ifc_object_definition);
				if( site )
				{
					vec_ifc_sites.push_back( site );
					continue;
				}
			}

			if( ifc_object_definition->classID() == IFC4X3::IFCPROJECT )
			{
				shared_ptr<IfcProject> project = dynamic_pointer_cast<IfcProject>(ifc_object_definition);
				if( project )
				{
					ifc_project = project;
					continue;
				}
			}
		}

		// if IfcBuilding is not connected to IfcSite and IfcProject, find IfcSite, and connect it
		for( auto ifc_site : vec_ifc_sites )
		{
			for( auto product_shape_building : vec_ifc_buildings )
			{
				shared_ptr<IfcObjectDefinition> ifc_object_definition(product_shape_building->m_ifc_object_definition);
				if( !ifc_object_definition )
				{
					continue;
				}
				shared_ptr<IfcBuilding> ifc_building = dynamic_pointer_cast<IfcBuilding>(ifc_object_definition);
				if( ifc_building )
				{
					if( ifc_building->m_Decomposes_inverse.size() == 0 )
					{
						if( ifc_site->m_IsDecomposedBy_inverse.size() == 0 )
						{
							shared_ptr<IfcRelAggregates> site_aggregates(new IfcRelAggregates());
							ifc_model->insertEntity(site_aggregates);
							site_aggregates->m_RelatingObject = ifc_site;

							ifc_site->m_IsDecomposedBy_inverse.push_back(site_aggregates);
						}

						weak_ptr<IfcRelAggregates> site_aggregates_weak = ifc_site->m_IsDecomposedBy_inverse[0];
						if( !site_aggregates_weak.expired() )
						{
							shared_ptr<IfcRelAggregates> site_aggregates(site_aggregates_weak);
							site_aggregates->m_RelatingObject = ifc_site;
							site_aggregates->m_RelatedObjects.push_back(ifc_building);

							ifc_building->m_Decomposes_inverse.push_back(site_aggregates);
						}
					}
				}
			}

			if( ifc_project )
			{
				if( ifc_project->m_IsDecomposedBy_inverse.size() == 0 )
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

	void readAppearanceFromPropertySet( const shared_ptr<IfcPropertySet>& prop_set, shared_ptr<ProductShapeData>& product_shape )
	{
		if( !prop_set )
		{
			return;
		}
		for( auto& ifc_property : prop_set->m_HasProperties )
		{
			if( !ifc_property )
			{
				continue;
			}

			shared_ptr<IfcSimpleProperty> simple_property = dynamic_pointer_cast<IfcSimpleProperty>(ifc_property);
			if( simple_property )
			{
				// ENTITY IfcSimpleProperty ABSTRACT SUPERTYPE OF(ONEOF( IfcPropertyBoundedValue, IfcPropertyEnumeratedValue, IfcPropertyListValue,
				// IfcPropertyReferenceValue, IfcPropertySingleValue, IfcPropertyTableValue))

				shared_ptr<IfcIdentifier> property_name = simple_property->m_Name;
				std::string name_str = property_name->m_value;
				if( name_str.compare( "LayerName" ) == 0 )
				{
					// TODO: implement layers
				}
				shared_ptr<IfcText> description = simple_property->m_Specification;


				shared_ptr<IfcPropertySingleValue> property_single_value = dynamic_pointer_cast<IfcPropertySingleValue>(simple_property);
				if( property_single_value )
				{
					//shared_ptr<IfcValue>& nominal_value = property_single_value->m_NominalValue;				//optional
					//shared_ptr<IfcUnit>& unit = property_single_value->m_Unit;						//optional

				}

				continue;
			}

			shared_ptr<IfcComplexProperty> complex_property = dynamic_pointer_cast<IfcComplexProperty>(ifc_property);
			if( complex_property )
			{
				if( !complex_property->m_UsageName ) continue;
				if( complex_property->m_UsageName->m_value.compare( "Color" ) == 0 )
				{
					vec4 vec_color;
					m_representation_converter->getStylesConverter()->convertIfcComplexPropertyColor( complex_property, vec_color );
					shared_ptr<AppearanceData> appearance_data( new AppearanceData( -1 ) );
					appearance_data->m_apply_to_geometry_type = AppearanceData::GEOM_TYPE_ANY;
					appearance_data->m_color_ambient.setColor( vec_color );
					appearance_data->m_color_diffuse.setColor( vec_color );
					appearance_data->m_color_specular.setColor( vec_color );
					appearance_data->m_shininess = 35.f;
					product_shape->addAppearance( appearance_data );
				}
			}
		}
	}

	/*\brief method convertGeometry: Creates geometry for Carve from previously loaded BuildingModel model.
	**/
	void convertGeometry()
	{
		progressTextCallback( "Creating geometry..." );
		progressValueCallback( 0, "geometry" );
		m_product_shape_data.clear();
		m_map_outside_spatial_structure.clear();
		m_setResolvedProjectStructure.clear();
		m_representation_converter->clearCache();
		m_clear_memory_immedeately = false;

		if( !m_ifc_model )
		{
			return;
		}

		shared_ptr<ProductShapeData> ifc_project_data;
		std::vector<shared_ptr<IfcObjectDefinition> > vec_object_definitions;
		const double length_in_meter = m_representation_converter->getUnitConverter()->getLengthInMeterFactor();
		setCsgEps(1.5e-08 * length_in_meter);
		if( std::abs(length_in_meter) > EPS_M14 )
		{
			double eps = m_geom_settings->getEpsilonCoplanarDistance();
			eps /= length_in_meter;
			m_geom_settings->setEpsilonCoplanarDistance(eps);
			m_geom_settings->setEpsilonCoplanarAngle(eps * 0.1);
		}

		const std::map<int, shared_ptr<BuildingEntity> >& map_entities = m_ifc_model->getMapIfcEntities();
		if( map_entities.size() > 0 )
		{
			for( auto it = map_entities.begin(); it != map_entities.end(); ++it )
			{
				shared_ptr<BuildingEntity> obj = it->second;
				if( obj )
				{
					shared_ptr<IfcObjectDefinition> object_def = dynamic_pointer_cast<IfcObjectDefinition>(obj);
					if( object_def )
					{
						vec_object_definitions.push_back(object_def);

						if( m_set_model_to_origin )
						{
							if( object_def->classID() == IFC4X3::IFCSITE )
							{
								shared_ptr<IfcSite> ifc_site = dynamic_pointer_cast<IfcSite>(object_def);
								if( ifc_site )
								{
									setIfcSiteToOrigin(ifc_site);
								}
							}
						}
					}
				}
			}
		}

		if( m_clear_memory_immedeately )
		{
			//m_ifc_model->getMapIfcEntities().clear();
		}

		// create geometry for for each IfcProduct independently, spatial structure will be resolved later
		std::map<std::string, shared_ptr<ProductShapeData> >* map_products_ptr = &m_product_shape_data;
		const int num_object_definitions = (int)vec_object_definitions.size();

#ifdef _OPENMP
		Mutex writelock_map;
		Mutex writelock_ifc_project;

		//omp_set_dynamic(0);     // Explicitly disable dynamic teams
		//omp_set_num_threads(2); // Use 2 threads for all consecutive parallel regions
#pragma omp parallel firstprivate(num_object_definitions) shared(map_products_ptr)
		{
			// time for one product may vary significantly, so schedule not so many
#pragma omp for schedule(dynamic,10)
#endif
			for( int i = 0; i < num_object_definitions; ++i )
			{
				shared_ptr<IfcObjectDefinition> object_def = vec_object_definitions[i];
				const int tag = object_def->m_tag;
				std::string guid;
				if (object_def->m_GlobalId)
				{
					guid = object_def->m_GlobalId->m_value;
				}

				shared_ptr<ProductShapeData> product_geom_input_data( new ProductShapeData(guid) );
				product_geom_input_data->m_ifc_object_definition = object_def;

				// TODO: check for equal product shapes: each representation and each item must be equal, also openings must be equal: m_HasOpenings_inverse
				std::stringstream thread_err;
				if( m_geom_settings->skipRenderObject( object_def->classID() ) )
				{
					// geometry will be created in method subtractOpenings
					continue;
				}
				else if( object_def->classID() == IFC4X3::IFCPROJECT )
				{
#ifdef _OPENMP
					ScopedLock scoped_lock( writelock_ifc_project );
#endif
					ifc_project_data = product_geom_input_data;
				}

				try
				{
					convertIfcProductShape( product_geom_input_data );
				}
				catch( BuildingException& e )
				{
					thread_err << e.what();
				}
				catch( carve::exception& e )
				{
					thread_err << e.str();
				}
				catch( std::exception& e )
				{
					thread_err << e.what();
				}
				catch( ... )
				{
					thread_err << "undefined error, product id " << tag;
				}

				{
#ifdef _OPENMP
					ScopedLock scoped_lock( writelock_map );
#endif
					auto it_find = map_products_ptr->find(guid);
					if( it_find != map_products_ptr->end() )
					{
						thread_err << "duplicate GUID in model: " << guid;

						size_t guid_append = 1;
						for( auto it = map_products_ptr->begin(); it != map_products_ptr->end(); ++it )
						{
							std::string guid_unique = guid + "_" + std::to_string(guid_append);
							auto it_find2 = map_products_ptr->find(guid_unique);
							if( it_find2 == map_products_ptr->end() )
							{
								object_def->m_GlobalId->m_value = guid_unique;
								guid = guid_unique;
								break;
							}
							++guid_append;
						}
					}

					map_products_ptr->insert( std::make_pair( guid, product_geom_input_data ) );

					if( thread_err.tellp() > 0 )
					{
						messageCallback( thread_err.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
					}
				}

				{
					if( m_callback_func_geometry_converted )
					{
						if( m_callback_object_geometry_converted )
						{
#ifdef _OPENMP
							//ScopedLock lock( m_writelock_geom_callback );
#endif
							m_callback_func_geometry_converted( m_callback_object_geometry_converted, product_geom_input_data );
						}
					}
				}

				// progress callback
				double progress = (double)i / (double)num_object_definitions;
				if( progress - m_recent_progress > 0.01 )
				{

#ifdef _OPENMP
					if( omp_get_thread_num() == 0 )
#endif
					{
						// leave 10% of progress to openscenegraph internals
						progressValueCallback( progress*0.9, "geometry" );
						m_recent_progress = progress;
					}
				}
			}
#ifdef _OPENMP
		} // implicit barrier
#endif

		  // subtract openings in related objects, such as IFCBUILDINGELEMENTPART connected to a window through IFCRELAGGREGATES
		for( auto it = map_products_ptr->begin(); it != map_products_ptr->end(); ++it )
		{
			shared_ptr<ProductShapeData> product_geom_input_data = it->second;
			try
			{
				subtractOpeningsInRelatedObjects(product_geom_input_data);
			}
			catch( BuildingException& e )
			{
				messageCallback(e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "");
			}
			catch( carve::exception& e )
			{
				messageCallback(e.str(), StatusCallback::MESSAGE_TYPE_ERROR, "");
			}
			catch( std::exception& e )
			{
				messageCallback(e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "");
			}
			catch( ... )
			{
				messageCallback("undefined error", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
			}
		}

		try
		{
			// now resolve spatial structure
			if( ifc_project_data )
			{
				fixModelHierarchy();

				// resove spatial structure first (IfcBuilding->IfcBuildingStorey->IfcBuildingElement)
				resolveProjectStructure( ifc_project_data, false );
				// resove secondary structure (IfcRelConnectsPortToElement etc)
				resolveProjectStructure( ifc_project_data, true );
			}

			// check if there are entities that are not in spatial structure
			for( auto it_product_shapes = m_product_shape_data.begin(); it_product_shapes != m_product_shape_data.end(); ++it_product_shapes )
			{
				shared_ptr<ProductShapeData> product_shape = it_product_shapes->second;
				if( !product_shape )
				{
					continue;
				}

				if( !product_shape->m_added_to_spatial_structure )
				{
					if( !product_shape->m_ifc_object_definition.expired() )
					{
						shared_ptr<IfcObjectDefinition> ifc_object_def( product_shape->m_ifc_object_definition );

						if( m_geom_settings->skipRenderObject(ifc_object_def->classID()) )
						{
							continue;
						}
						std::string guid;

						if (ifc_object_def->m_GlobalId)
						{
							guid = ifc_object_def->m_GlobalId->m_value;
						}

						if( guid.size() > 18 )
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
		catch( BuildingException& e )
		{
			messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
		}
		catch( std::exception& e )
		{
			messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
		}
		catch( ... )
		{
			messageCallback( "undefined error", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		}

		m_representation_converter->getProfileCache()->clearProfileCache();
		progressTextCallback( "Loading file done" );
		progressValueCallback( 1.0, "geometry" );
	}

	void addVector3D(const vec3& point, std::vector<float>& target_array)
	{
		bool m_roundCoords = false;
		if (m_roundCoords)
		{
			target_array.push_back(round(point.x*10000)*0.0001);
			target_array.push_back(round(point.y*10000)*0.0001);
			target_array.push_back(round(point.z*10000)*0.0001);
		}
		else
		{
			target_array.push_back(point.x);
			target_array.push_back(point.y);
			target_array.push_back(point.z);
		}
	}

	//\brief method convertIfcProduct: Creates geometry objects (meshset with connected vertex-edge-face graph) from an IfcProduct object
	// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
	void convertIfcProductShape( shared_ptr<ProductShapeData>& product_shape )
	{
		if( product_shape->m_ifc_object_definition.expired() )
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
		if( !product_representation )
		{
			return;
		}

		// convert IFC geometry
		std::vector<shared_ptr<IfcRepresentation> >& vec_representations = product_representation->m_Representations;
		for( size_t i_representations = 0; i_representations < vec_representations.size(); ++i_representations )
		{
			const shared_ptr<IfcRepresentation>& representation = vec_representations[i_representations];
			if( !representation )
			{
				continue;
			}

			try
			{
				shared_ptr<RepresentationData> representation_data( new RepresentationData() );
				m_representation_converter->convertIfcRepresentation( representation, representation_data );
				product_shape->m_vec_representations.push_back( representation_data );
				representation_data->m_parent_product = product_shape;
			}
			catch( BuildingException& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
			}
			catch( std::exception& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
			}
		}

		// IfcProduct has an ObjectPlacement that can be local or global
		product_shape->m_object_placement = ifc_product->m_ObjectPlacement;
		if( ifc_product->m_ObjectPlacement )
		{
			// IfcPlacement2Matrix follows related placements in case of local coordinate systems
			std::unordered_set<IfcObjectPlacement*> placement_already_applied;
			m_representation_converter->getPlacementConverter()->convertIfcObjectPlacement( ifc_product->m_ObjectPlacement, product_shape, placement_already_applied, false );
		}

		std::vector<shared_ptr<ProductShapeData> > vec_opening_data;
		const shared_ptr<IfcElement> ifc_element = dynamic_pointer_cast<IfcElement>(ifc_product);
		if( ifc_element )
		{
			// handle openings
			m_representation_converter->subtractOpenings(ifc_element, product_shape);

			// handle styles on IfcElement level
			std::vector<shared_ptr<AppearanceData> > vec_apperances;
			m_representation_converter->getStylesConverter()->convertElementStyle(ifc_element, vec_apperances);
			for (auto appearance_data : vec_apperances)
			{
				product_shape->addAppearance(appearance_data);
			}
		}

		// Fetch the IFCProduct relationships
		if( ifc_product->m_IsDefinedBy_inverse.size() > 0 )
		{
			std::vector<weak_ptr<IfcRelDefinesByProperties> >& vec_IsDefinedBy_inverse = ifc_product->m_IsDefinedBy_inverse;
			for( size_t i = 0; i < vec_IsDefinedBy_inverse.size(); ++i )
			{
				shared_ptr<IfcRelDefinesByProperties> rel_def( vec_IsDefinedBy_inverse[i] );
				shared_ptr<IfcPropertySetDefinitionSelect> relating_property_definition_select = rel_def->m_RelatingPropertyDefinition;
				if( relating_property_definition_select )
				{
					// TYPE IfcPropertySetDefinitionSelect = SELECT	(IfcPropertySetDefinition	,IfcPropertySetDefinitionSet);
					shared_ptr<IfcPropertySetDefinition> property_set_def = dynamic_pointer_cast<IfcPropertySetDefinition>(relating_property_definition_select);
					if( property_set_def )
					{
						shared_ptr<IfcPropertySet> property_set = dynamic_pointer_cast<IfcPropertySet>(property_set_def);
						if( property_set )
						{
							readAppearanceFromPropertySet( property_set, product_shape );
						}
						continue;
					}

					shared_ptr<IfcPropertySetDefinitionSet> property_set_def_set = dynamic_pointer_cast<IfcPropertySetDefinitionSet>(relating_property_definition_select);
					if( property_set_def_set )
					{
						std::vector<shared_ptr<IfcPropertySetDefinition> >& vec_propterty_set_def = property_set_def_set->m_vec;
						std::vector<shared_ptr<IfcPropertySetDefinition> >::iterator it_property_set_def;
						for( it_property_set_def = vec_propterty_set_def.begin(); it_property_set_def != vec_propterty_set_def.end(); ++it_property_set_def )
						{
							shared_ptr<IfcPropertySetDefinition> property_set_def2 = (*it_property_set_def);
							if( property_set_def2 )
							{
								shared_ptr<IfcPropertySet> property_set = dynamic_pointer_cast<IfcPropertySet>(property_set_def2);
								if( property_set )
								{
									readAppearanceFromPropertySet( property_set, product_shape );
								}
							}
						}
						continue;
					}
				}
			}
		}

		// check for existing meshes
		// TODO: make sure that the meshes are not changed after here, for example with boolean operations
		bool enableCaching = false;
		if( enableCaching )
		{
			bool equalItemFound = false;

			for( auto it : m_product_shape_data )
			{
				const shared_ptr<ProductShapeData>& existingProductShape = it.second;
				if( !existingProductShape )
				{
					continue;
				}
				for( auto rep : existingProductShape->m_vec_representations )
				{
					for( auto item : rep->m_vec_item_data )
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
			if( !equalItemFound )
			{
				//				representation_data->m_vec_item_data.push_back(geom_item_data);
				//#ifdef _OPENMP
				//				ScopedLock lock( m_writelock_item_cache );
				//#endif
				//				m_map_item_data_cache.push_back(geom_item_data);
			}
		}

		if( m_clear_memory_immedeately )
		{
			ifc_product->m_Representation.reset();
		}
	}

	bool hasRelatedOpenings(shared_ptr<ProductShapeData>& product_shape)
	{
		if (product_shape->m_ifc_object_definition.expired())
		{
			return false;
		}

		shared_ptr<IfcObjectDefinition> ifc_object_def(product_shape->m_ifc_object_definition);
		shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(ifc_object_def);
		if (!ifc_product)
		{
			return false;
		}

		shared_ptr<IfcElement> ifc_element = dynamic_pointer_cast<IfcElement>(ifc_product);
		if (!ifc_element)
		{
			return false;
		}

		if (ifc_element->m_HasOpenings_inverse.size() == 0)
		{
			return false;
		}

		// collect aggregated objects
		const std::vector<weak_ptr<IfcRelAggregates> >& vec_decomposed_by = ifc_object_def->m_IsDecomposedBy_inverse;
		if (vec_decomposed_by.size() > 0)
		{
			return true;
		}

		const std::vector<weak_ptr<IfcRelAggregates> >& vec_decomposes = ifc_object_def->m_Decomposes_inverse;
		if (vec_decomposes.size() > 0)
		{
			return true;
		}
		return false;
	}

	void subtractOpeningsInRelatedObjects(shared_ptr<ProductShapeData>& product_shape)
	{
		if( product_shape->m_ifc_object_definition.expired() )
		{
			return;
		}

		shared_ptr<IfcObjectDefinition> ifc_object_def(product_shape->m_ifc_object_definition);
		shared_ptr<IfcElement> ifc_element = dynamic_pointer_cast<IfcElement>(ifc_object_def);
		if( !ifc_element )
		{
			return;
		}

		if( ifc_element->m_HasOpenings_inverse.size() == 0 )
		{
			return;
		}

		// collect aggregated objects
		const std::vector<weak_ptr<IfcRelAggregates> >& vec_decomposed_by = ifc_element->m_IsDecomposedBy_inverse;

		for( auto& decomposed_by : vec_decomposed_by )
		{
			if( decomposed_by.expired() )
			{
				continue;
			}
			shared_ptr<IfcRelAggregates> decomposed_by_aggregates(decomposed_by);
			std::vector<shared_ptr<IfcObjectDefinition> >& vec_related_objects = decomposed_by_aggregates->m_RelatedObjects;
			for( auto& related_object : vec_related_objects )
			{
				if( !related_object )
				{
					continue;
				}

				std::string guid;
				if (related_object->m_GlobalId)
				{
					guid = related_object->m_GlobalId->m_value;

					auto it_find_related_shape = m_product_shape_data.find(guid);
					if( it_find_related_shape != m_product_shape_data.end() )
					{
						shared_ptr<ProductShapeData>& related_product_shape = it_find_related_shape->second;
						m_representation_converter->subtractOpenings(ifc_element, related_product_shape);
					}
				}
			}
		}
	}

	virtual void messageTarget( void* ptr, shared_ptr<StatusCallback::Message> m )
	{
		GeometryConverter* myself = (GeometryConverter*)ptr;
		if( myself )
		{
			if( m->m_entity )
			{
#ifdef _OPENMP
				ScopedLock lock( myself->m_writelock_messages );
#endif
				// make sure that the same message for one entity does not appear several times
				const int tag = m->m_entity->m_tag;

				auto it = myself->m_messages.find( tag );
				if( it != myself->m_messages.end() )
				{
					std::vector<shared_ptr<StatusCallback::Message> >& vec_message_for_entity = it->second;
					for( size_t i = 0; i < vec_message_for_entity.size(); ++i )
					{
						shared_ptr<StatusCallback::Message>& existing_message = vec_message_for_entity[i];
						if( existing_message->m_message_text.compare( m->m_message_text ) == 0 )
						{
							// same message for same entity is already there, so ignore message
							return;
						}
					}
					vec_message_for_entity.push_back( m );
				}
				else
				{
					std::vector<shared_ptr<StatusCallback::Message> >& vec = myself->m_messages.insert( std::make_pair( tag, std::vector<shared_ptr<StatusCallback::Message> >() ) ).first->second;
					vec.push_back( m );
				}
			}

			myself->messageCallback( m );
		}
	}
};

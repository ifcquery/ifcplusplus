/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#pragma once

#include <set>
#include <sstream>
#include <osg/Switch>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/IFC4/include/IfcComplexProperty.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcPropertySet.h>
#include <ifcpp/IFC4/include/IfcPropertySetDefinitionSet.h>
#include <ifcpp/IFC4/include/IfcPropertySingleValue.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcRelDefinesByProperties.h>
#include <ifcpp/IFC4/include/IfcSimpleProperty.h>

#include "RepresentationConverter.h"
#include "ConverterOSG.h"
#include "GeometrySettings.h"
#include "GeometryInputData.h"

class GeometryConverter : public StatusCallback
{
protected:
	shared_ptr<IfcPPModel>				m_ifc_model;
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<RepresentationConverter> m_representation_converter;
	shared_ptr<ConverterOSG>			m_converter_osg;

	std::map<int, shared_ptr<ProductShapeInputData> >	m_shape_input_data;
	boost::unordered_map<int, shared_ptr<IfcPPObject> >		m_map_outside_spatial_structure;
	double m_recent_progress;
	std::map<int, std::vector<shared_ptr<StatusCallback::Message> > > m_messages;
#ifdef IFCPP_OPENMP
	Mutex m_writelock_messages;
#endif

public:
	// getters and setters
	shared_ptr<IfcPPModel>&						getIfcPPModel()					{ return m_ifc_model; }
	shared_ptr<RepresentationConverter>&		getRepresentationConverter()	{ return m_representation_converter; }
	shared_ptr<ConverterOSG>&					getConverterOSG()				{ return m_converter_osg; }
	shared_ptr<GeometrySettings>&				getGeomSettings()				{ return m_geom_settings; }
	std::map<int, shared_ptr<ProductShapeInputData> >&		getShapeInputData()					{ return m_shape_input_data; }
	boost::unordered_map<int, shared_ptr<IfcPPObject> >&	getObjectsOutsideSpatialStructure()	{ return m_map_outside_spatial_structure; }

	virtual void slotMessageWrapper( void* ptr, shared_ptr<StatusCallback::Message> m )
	{
		GeometryConverter* myself = (GeometryConverter*)ptr;
		if( myself )
		{
			if( m->m_entity )
			{
#ifdef IFCPP_OPENMP
				ScopedLock lock( myself->m_writelock_messages );
#endif

				// make sure that the same message for one entity does not appear several times
				const int entity_id = m->m_entity->m_id;

				std::map<int, std::vector<shared_ptr<StatusCallback::Message> > >::iterator it = myself->m_messages.find( entity_id );
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
					std::vector<shared_ptr<StatusCallback::Message> >& vec = myself->m_messages.insert( std::make_pair( entity_id, std::vector<shared_ptr<StatusCallback::Message> >() ) ).first->second;
					vec.push_back( m );
				}
			}

			myself->messageCallback( m );
		}
	}

	GeometryConverter( shared_ptr<IfcPPModel>& ifc_model )
	{
		m_ifc_model = ifc_model;
		m_geom_settings = shared_ptr<GeometrySettings>( new GeometrySettings() );
		resetNumVerticesPerCircle();
		shared_ptr<UnitConverter>& unit_converter = m_ifc_model->getUnitConverter();
		m_representation_converter = shared_ptr<RepresentationConverter>( new RepresentationConverter( m_geom_settings, unit_converter ) );
		m_converter_osg = shared_ptr<ConverterOSG>( new ConverterOSG( m_geom_settings ) );

		// redirect all messages to slotMessageWrapper
		addCallbackChild( m_ifc_model.get() );
		addCallbackChild( m_representation_converter.get() );
		addCallbackChild( m_converter_osg.get() );
	}

	virtual ~GeometryConverter()
	{
	}

	void resetModel()
	{
		progressTextCallback( L"Unloading model, cleaning up memory..." );
		clearInputCache();
		m_recent_progress = 0.0;

		m_ifc_model->clearCache();
		m_ifc_model->clearIfcModel();
		progressTextCallback( L"Unloading model done" );
		progressValueCallback( 0.0, "parse" );

#ifdef _DEBUG
		GeomDebugUtils::clearMeshsetDump();
#endif
	}

	void clearInputCache()
	{
		m_shape_input_data.clear();
		m_map_outside_spatial_structure.clear();
		m_converter_osg->clearAppearanceCache();
		m_representation_converter->clearCache();
		m_messages.clear();
	}

	void resetNumVerticesPerCircle()
	{
		m_geom_settings->resetNumVerticesPerCircle();
	}

	void setModel( shared_ptr<IfcPPModel> model )
	{
		clearInputCache();
		m_ifc_model = model;
		m_representation_converter->clearCache();
		m_representation_converter->setUnitConverter( m_ifc_model->getUnitConverter() );
		//m_ifc_model->setMessageCallBack( this, &slotMessageWrapper );
		addCallbackChild( m_ifc_model.get() );
	}
	
	/*\brief method createGeometryOSG: Creates geometry for OpenSceneGraph from previously loaded model.
	  \param[out] parent_group Group to append the resulting geometry.
	**/
	void createGeometryOSG( osg::ref_ptr<osg::Switch> parent_group )
	{
		progressTextCallback( L"Creating geometry..." );
		progressValueCallback( 0, "geometry" );
		m_shape_input_data.clear();
		m_map_outside_spatial_structure.clear();
		m_representation_converter->clearCache();

		std::vector<shared_ptr<IfcProduct> > vec_products;
		const double length_to_meter_factor = m_ifc_model->getUnitConverter()->getLengthInMeterFactor();
		carve::setEpsilon( 1.4901161193847656e-05*length_to_meter_factor );

		const boost::unordered_map<int, shared_ptr<IfcPPEntity> >& map_entities = m_ifc_model->getMapIfcEntities();
		for( auto it = map_entities.begin(); it != map_entities.end(); ++it )
		{
			shared_ptr<IfcPPEntity> obj = it->second;
			shared_ptr<IfcProduct> product = dynamic_pointer_cast<IfcProduct>( obj );
			if( product )
			{
				vec_products.push_back( product );
			}
			// TODO: sort vec_products such, that products with complex geometry are at the beginning (better scheduling for omp). Rate complexity by number of CSG ops, and number of vertices. Check if sorting pays off.
		}

		// create geometry for for each IfcProduct independently, spatial structure will be resolved later
		std::map<int, shared_ptr<ProductShapeInputData> >* map_products_ptr = &m_shape_input_data;
		const int num_products = (int)vec_products.size();

#ifdef IFCPP_OPENMP
		Mutex writelock_map;

#pragma omp parallel firstprivate(num_products) shared(map_products_ptr)
		{
			// time for one product may vary significantly, so schedule not so many
#pragma omp for schedule(dynamic,10)
#endif
			for( int i = 0; i < num_products; ++i )
			{
				shared_ptr<IfcProduct> product = vec_products[i];
				std::stringstream thread_err;
				if( dynamic_pointer_cast<IfcFeatureElementSubtraction>( product ) )
				{
					// geometry will be created in method subtractOpenings
					continue;
				}
				if( !product->m_Representation )
				{
					continue;
				}

				const int product_id = product->m_id;
				shared_ptr<ProductShapeInputData> product_geom_input_data( new ProductShapeInputData() );
				product_geom_input_data->m_ifc_product = product;

				try
				{
					convertIfcProduct( product_geom_input_data );
					m_converter_osg->convertToOSG( product_geom_input_data, length_to_meter_factor );
				}
#ifdef _DEBUG
				catch( DebugBreakException& dbge )
				{
					throw dbge;
				}
#endif
				catch( IfcPPOutOfMemoryException& e )
				{
					throw e;
				}
				catch( IfcPPException& e )
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
					thread_err << "undefined error, product id " << product_id;
				}

				{
#ifdef IFCPP_OPENMP
					ScopedLock scoped_lock( writelock_map );
#endif
					map_products_ptr->insert( std::make_pair( product_id, product_geom_input_data ) );

					if( thread_err.tellp() > 0 )
					{
						messageCallback( thread_err.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
					}
				}

				// progress callback
				double progress = (double)i / (double)num_products;
				if( progress - m_recent_progress > 0.02 )
				{

#ifdef IFCPP_OPENMP
					if( omp_get_thread_num() == 0 )
#endif
					{
						// leave 10% of progress to openscenegraph internals
						progressValueCallback( progress*0.9, "geometry" );
						m_recent_progress = progress;
					}
				}
			}
#ifdef IFCPP_OPENMP
		} // implicit barrier
#endif

		try
		{
			// now resolve spatial structure
			shared_ptr<IfcProject> ifc_project = m_ifc_model->getIfcProject();
			if( ifc_project )
			{
				resolveProjectStructure( ifc_project, parent_group );
			}

			// check if there are entities that are not in spatial structure
			osg::ref_ptr<osg::Group> group_outside_spatial_structure = new osg::Group();
			group_outside_spatial_structure->setName( "Entities not in spatial structure" );

			for( auto it_product_shapes = m_shape_input_data.begin(); it_product_shapes != m_shape_input_data.end(); ++it_product_shapes )
			{
				shared_ptr<ProductShapeInputData> product_shape = it_product_shapes->second;
				shared_ptr<IfcProduct> ifc_product( product_shape->m_ifc_product );
				if( !product_shape )
				{
					continue;
				}

				if( !product_shape->m_added_to_node )
				{
					shared_ptr<IfcFeatureElementSubtraction> opening = dynamic_pointer_cast<IfcFeatureElementSubtraction>( ifc_product );
					if( opening )
					{
						continue;
					}

					if( product_shape->m_product_switch.valid() )
					{
						group_outside_spatial_structure->addChild( product_shape->m_product_switch );
					}

					product_shape->m_added_to_node = true;
				}
				m_map_outside_spatial_structure[ifc_product->m_id] = ifc_product;
			}

			if( group_outside_spatial_structure->getNumChildren() > 0 )
			{
				parent_group->addChild( group_outside_spatial_structure );
			}
		}
		catch( IfcPPOutOfMemoryException& e )
		{
			throw e;
		}
		catch( IfcPPException& e )
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
		progressTextCallback( L"Loading file done" );
		progressValueCallback( 1.0, "geometry" );
	}

	bool inParentList( const int entity_id, osg::Group* group )
	{
		if( !group )
		{
			return false;
		}

		const osg::Group::ParentList& vec_parents = group->getParents();
		for( size_t ii = 0; ii < vec_parents.size(); ++ii )
		{
			osg::Group* parent = vec_parents[ii];
			if( parent )
			{
				const std::string parent_name = parent->getName();
				if( parent_name.length() > 0 )
				{
					if( parent_name.at( 0 ) == '#' )
					{
						// extract entity id
						std::string parent_name_id = parent_name.substr( 1 );
						size_t last_index = parent_name_id.find_first_not_of( "0123456789" );
						std::string id_str = parent_name_id.substr( 0, last_index );
						const int id = std::stoi( id_str.c_str() );
						if( id == entity_id )
						{
							return true;
						}
						bool in_parent_list = inParentList( entity_id, parent );
						if( in_parent_list )
						{
							return true;
						}

					}
				}
			}
		}
		return false;
	}

	void resolveProjectStructure( const shared_ptr<IfcObjectDefinition>& obj_def, osg::ref_ptr<osg::Switch> group )
	{
		const int entity_id = obj_def->m_id;
		if( inParentList( entity_id, group ) )
		{
			messageCallback( "Cycle in project structure detected", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, obj_def.get() );
			return;
		}

		shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>( obj_def );
		if( ifc_product )
		{
			std::map<int, shared_ptr<ProductShapeInputData> >::iterator it_product_map = m_shape_input_data.find( entity_id );
			if( it_product_map != m_shape_input_data.end() )
			{
				shared_ptr<ProductShapeInputData>& product_shape = it_product_map->second;
				if( product_shape )
				{
					if( product_shape->m_product_switch )
					{
						group->addChild( product_shape->m_product_switch );
					}
				}
				if( product_shape->m_added_to_node )
				{
					// IfcRelContainsInSpatialStructure relationship is required to be hierarchical (an element can only be contained in exactly one spatial structure element)
					std::cout << "already product_shape->added_to_node" << std::endl;
				}
				product_shape->m_added_to_node = true;
			}
		}

		if( group->getName().size() < 1 )
		{
			std::stringstream switch_name;
			switch_name << "#" << entity_id << "=" << obj_def->className();
			group->setName( switch_name.str().c_str() );
		}

		const std::vector<weak_ptr<IfcRelAggregates> >& vec_IsDecomposedBy = obj_def->m_IsDecomposedBy_inverse;
		for( size_t ii = 0; ii < vec_IsDecomposedBy.size(); ++ii )
		{
			const weak_ptr<IfcRelAggregates>& rel_aggregates_weak_ptr = vec_IsDecomposedBy[ii];
			if( rel_aggregates_weak_ptr.expired() )
			{
				continue;
			}
			shared_ptr<IfcRelAggregates> rel_aggregates( rel_aggregates_weak_ptr );
			if( rel_aggregates )
			{
				const std::vector<shared_ptr<IfcObjectDefinition> >& vec_related_objects = rel_aggregates->m_RelatedObjects;
				for( size_t jj = 0; jj < vec_related_objects.size(); ++jj )
				{
					const shared_ptr<IfcObjectDefinition>& child_obj_def = vec_related_objects[jj];
					if( child_obj_def )
					{
						osg::ref_ptr<osg::Switch> group_subparts = new osg::Switch();
						group->addChild( group_subparts );
						resolveProjectStructure( child_obj_def, group_subparts );
					}
				}
			}
		}

		shared_ptr<IfcSpatialStructureElement> spatial_ele = dynamic_pointer_cast<IfcSpatialStructureElement>( obj_def );
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
				shared_ptr<IfcRelContainedInSpatialStructure> rel_contained( rel_contained_weak_ptr );
				if( rel_contained )
				{
					const std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained->m_RelatedElements;

					for( size_t jj = 0; jj < vec_related_elements.size(); ++jj )
					{
						const shared_ptr<IfcProduct>& related_product = vec_related_elements[jj];
						if( related_product )
						{
							osg::ref_ptr<osg::Switch> group_subparts = new osg::Switch();
							group->addChild( group_subparts );
							resolveProjectStructure( related_product, group_subparts );
						}
					}
				}
			}
		}

		// TODO: handle IfcRelAssignsToProduct
	}

	void readAppearanceFromPropertySet( const shared_ptr<IfcPropertySet>& prop_set, shared_ptr<ProductShapeInputData>& product_shape )
	{
		if( !prop_set )
		{
			return;
		}
		for( auto& ifc_property :  prop_set->m_HasProperties )
		{
			if( !ifc_property )
			{
				continue;
			}

			shared_ptr<IfcSimpleProperty> simple_property = dynamic_pointer_cast<IfcSimpleProperty>( ifc_property );
			if( simple_property )
			{
				// ENTITY IfcSimpleProperty ABSTRACT SUPERTYPE OF(ONEOF( IfcPropertyBoundedValue, IfcPropertyEnumeratedValue, IfcPropertyListValue,
				// IfcPropertyReferenceValue, IfcPropertySingleValue, IfcPropertyTableValue))

				shared_ptr<IfcIdentifier> property_name = simple_property->m_Name;
				std::wstring name_str = property_name->m_value;
				if( name_str.compare( L"LayerName" ) == 0 )
				{
					// TODO: implement layers
				}
				shared_ptr<IfcText> description = simple_property->m_Description;


				shared_ptr<IfcPropertySingleValue> property_single_value = dynamic_pointer_cast<IfcPropertySingleValue>( simple_property );
				if( property_single_value )
				{
					//shared_ptr<IfcValue>& nominal_value = property_single_value->m_NominalValue;				//optional
					//shared_ptr<IfcUnit>& unit = property_single_value->m_Unit;						//optional

				}
				
				continue;
			}

			shared_ptr<IfcComplexProperty> complex_property = dynamic_pointer_cast<IfcComplexProperty>( ifc_property );
			if( complex_property )
			{
				if( !complex_property->m_UsageName ) continue;
				if( complex_property->m_UsageName->m_value.compare( L"Color" ) == 0 )
				{

					carve::geom::vector<4> vec_color;
					m_representation_converter->getStylesConverter()->convertIfcComplexPropertyColor( complex_property, vec_color );
					shared_ptr<AppearanceData> appearance_data( new AppearanceData( -1 ) );
					if( !appearance_data )
					{
						throw IfcPPOutOfMemoryException( __FUNC__ );
					}
					appearance_data->m_apply_to_geometry_type = AppearanceData::ANY;
					appearance_data->m_color_ambient = vec_color;
					appearance_data->m_color_diffuse = vec_color;
					appearance_data->m_color_specular = vec_color;
					appearance_data->m_shininess = 35.f;
					product_shape->addAppearance( appearance_data );
				}
			}
		}
	}


	//\brief method convertIfcProduct: Creates geometry objects (meshset with connected vertex-edge-face graph) from an IfcProduct object
	// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
	void convertIfcProduct( shared_ptr<ProductShapeInputData>& product_shape )
	{
		shared_ptr<IfcProduct> ifc_product( product_shape->m_ifc_product );
		if( !ifc_product )
		{
			return;
		}
		if( !ifc_product->m_Representation )
		{
			return;
		}

		//const int product_id = ifc_product->m_id;
		const double length_factor = m_ifc_model->getUnitConverter()->getLengthInMeterFactor();
		product_shape->m_ifc_product = ifc_product;

		// evaluate IFC geometry
		shared_ptr<IfcProductRepresentation>& product_representation = ifc_product->m_Representation;
		std::vector<shared_ptr<IfcRepresentation> >& vec_representations = product_representation->m_Representations;
		for( size_t i_representations = 0; i_representations < vec_representations.size(); ++i_representations )
		{
			const shared_ptr<IfcRepresentation>& representation = vec_representations[i_representations];
			try
			{
				shared_ptr<ProductRepresentationData> representation_data( new ProductRepresentationData() );
				m_representation_converter->convertIfcRepresentation( representation, representation_data );
				product_shape->m_vec_representations.push_back( representation_data );
			}
			catch( IfcPPOutOfMemoryException& e )
			{
				throw e;
			}
			catch( IfcPPException& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
			}
			catch( std::exception& e )
			{
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
			}
		}

		// IfcProduct has an ObjectPlacement that can be local or global
		carve::math::Matrix product_placement_matrix( carve::math::Matrix::IDENT() );
		if( ifc_product->m_ObjectPlacement )
		{
			// IfcPlacement2Matrix follows related placements in case of local coordinate systems
			std::unordered_set<IfcObjectPlacement*> placement_already_applied;
			PlacementConverter::convertIfcObjectPlacement( ifc_product->m_ObjectPlacement, length_factor, product_placement_matrix, this, placement_already_applied );
			product_shape->applyPosition( product_placement_matrix );
		}

		// handle openings
		std::vector<shared_ptr<ProductShapeInputData> > vec_opening_data;
		const shared_ptr<IfcElement> ifc_element = dynamic_pointer_cast<IfcElement>( ifc_product );
		if( ifc_element )
		{
			m_representation_converter->subtractOpenings( ifc_element, product_shape );
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
					shared_ptr<IfcPropertySetDefinition> property_set_def = dynamic_pointer_cast<IfcPropertySetDefinition>( relating_property_definition_select );
					if( property_set_def )
					{
						shared_ptr<IfcPropertySet> property_set = dynamic_pointer_cast<IfcPropertySet>( property_set_def );
						if( property_set )
						{
							readAppearanceFromPropertySet( property_set, product_shape );
						}
						continue;
					}

					shared_ptr<IfcPropertySetDefinitionSet> property_set_def_set = dynamic_pointer_cast<IfcPropertySetDefinitionSet>( relating_property_definition_select );
					if( property_set_def_set )
					{
						std::vector<shared_ptr<IfcPropertySetDefinition> >& vec_propterty_set_def = property_set_def_set->m_vec;
						std::vector<shared_ptr<IfcPropertySetDefinition> >::iterator it_property_set_def;
						for( it_property_set_def = vec_propterty_set_def.begin(); it_property_set_def != vec_propterty_set_def.end(); ++it_property_set_def )
						{
							shared_ptr<IfcPropertySetDefinition> property_set_def = ( *it_property_set_def );
							if( property_set_def )
							{
								shared_ptr<IfcPropertySet> property_set = dynamic_pointer_cast<IfcPropertySet>( property_set_def );
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
	}
};

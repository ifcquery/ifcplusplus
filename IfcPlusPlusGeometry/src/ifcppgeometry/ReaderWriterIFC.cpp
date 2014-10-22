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

#include <utility>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <osg/MatrixTransform>
#include <osg/CullFace>
#include <osgText/Text>

#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcElement.h>
#include <ifcpp/IFC4/include/IfcRelVoidsElement.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcFeatureElementSubtraction.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>
#include <ifcpp/IFC4/include/IfcProductRepresentation.h>
#include <ifcpp/IFC4/include/IfcPropertySet.h>
#include <ifcpp/IFC4/include/IfcPropertySetDefinition.h>
#include <ifcpp/IFC4/include/IfcPropertySetDefinitionSet.h>
#include <ifcpp/IFC4/include/IfcPropertySetDefinitionSelect.h>
#include <ifcpp/IFC4/include/IfcRelDefinesByProperties.h>
#include <ifcpp/IFC4/include/IfcSpace.h>
#include <ifcpp/IFC4/include/IfcBeam.h>
#include <ifcpp/IFC4/include/IfcCurtainWall.h>
#include <ifcpp/IFC4/include/IfcWindow.h>
#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcSite.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLabel.h>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/IfcPPOpenMP.h>
#include <ifcpp/reader/IfcPPReaderSTEP.h>
#include <ifcpp/writer/IfcPPWriterSTEP.h>

#include "GeometrySettings.h"
#include "GeomUtils.h"
#include "GeometryException.h"
#include "RepresentationConverter.h"
#include "PlacementConverter.h"
#include "SolidModelConverter.h"
#include "ProfileCache.h"
#include "ConverterOSG.h"
#include "CSG_Adapter.h"
#include "ReaderWriterIFC.h"

ReaderWriterIFC::ReaderWriterIFC()
{
	m_ifc_model = shared_ptr<IfcPPModel>( new IfcPPModel() );
	m_step_reader = shared_ptr<IfcPPReaderSTEP>( new IfcPPReaderSTEP() );
	m_step_writer = shared_ptr<IfcPPWriterSTEP>( new IfcPPWriterSTEP() );

	m_geom_settings = shared_ptr<GeometrySettings>( new GeometrySettings() );
	resetNumVerticesPerCircle();
	shared_ptr<UnitConverter>& unit_converter = m_ifc_model->getUnitConverter();
	m_representation_converter = shared_ptr<RepresentationConverter>( new RepresentationConverter( m_geom_settings, unit_converter ) );

	// redirect all messages to ReaderWriterIFC::slotMessageWrapper
	m_ifc_model->setMessageCallBack( this, &ReaderWriterIFC::slotMessageWrapper );
	m_step_reader->setMessageCallBack( this, &ReaderWriterIFC::slotMessageWrapper );
	m_step_writer->setMessageCallBack( this, &ReaderWriterIFC::slotMessageWrapper );
	m_representation_converter->setMessageCallBack( this, &ReaderWriterIFC::slotMessageWrapper );

	// redirect progress callback to ReaderWriterIFC::slotProgressWrapper
	m_ifc_model->setProgressCallBack( this, &ReaderWriterIFC::slotProgressWrapper );
	m_step_reader->setProgressCallBack( this, &ReaderWriterIFC::slotProgressWrapper );
	m_step_writer->setProgressCallBack( this, &ReaderWriterIFC::slotProgressWrapper );
	m_representation_converter->setProgressCallBack( this, &ReaderWriterIFC::slotProgressWrapper );

	m_glass_stateset = new osg::StateSet();
	m_glass_stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
	m_glass_stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	m_cull_back_off = new osg::CullFace( osg::CullFace::BACK );
}

ReaderWriterIFC::~ReaderWriterIFC()
{
}

void ReaderWriterIFC::resetModel()
{
	progressCallback( -1, "", L"Unloading model, cleaning up memory..." );
	clearInputCache();
	m_recent_progress = 0.0;
	
	m_ifc_model->clearCache();
	m_ifc_model->clearIfcModel();
	progressCallback( 0.0, "parse", L"Unloading model done" );

#ifdef _DEBUG
	CSG_Adapter::clearMeshsetDump();
#endif
}

void ReaderWriterIFC::clearInputCache()
{
	m_shape_input_data.clear();
	m_map_visited.clear();
	m_map_outside_spatial_structure.clear();
	AppearanceManagerOSG::clearAppearanceCache();
	m_representation_converter->getUnitConverter()->resetUnitConverter();
	m_representation_converter->clearCache();
	m_tickets.clear();
}

void ReaderWriterIFC::resetNumVerticesPerCircle()
{
	m_geom_settings->m_num_vertices_per_circle = m_geom_settings->m_num_vertices_per_circle_default;
}

void ReaderWriterIFC::setModel( shared_ptr<IfcPPModel> model )
{
	clearInputCache();
	m_ifc_model = model;
	m_representation_converter->clearCache();
	m_representation_converter->setUnitConverter( m_ifc_model->getUnitConverter() );
}

void ReaderWriterIFC::loadModelFromFile( const std::string& file_path )
{
	// if file content needs to be loaded into a plain model, call resetModel() before loadModelFromFile
	
	std::string ext = file_path.substr( file_path.find_last_of( "." ) + 1 );
	
	if( boost::iequals( ext, "ifc" ) )
	{
		// ok, nothing to do here
	}
	else if( boost::iequals( ext, "ifcXML" ) )
	{
		// TODO: implement xml reader
		messageCallback( "ifcXML not yet implemented", StatusCallback::STATUS_SEVERITY_ERROR, __FUNC__ );
		return;
	}
	else if( boost::iequals( ext, "ifcZIP" ) )
	{
		// TODO: implement zip uncompress
		messageCallback( "ifcZIP not yet implemented", StatusCallback::STATUS_SEVERITY_ERROR, __FUNC__ );
		return;
	}
	else
	{
		std::stringstream strs;
		strs << "Unsupported file type: " << ext;
		messageCallback( strs.str().c_str(), StatusCallback::STATUS_SEVERITY_ERROR, __FUNC__ );
		return;
	}

	// open file
	std::ifstream infile;
	infile.open( file_path.c_str(), std::ifstream::in );

	if( !infile.is_open() )
	{
		std::stringstream strs;
		strs << "Could not open file: " << file_path.c_str();
		messageCallback( strs.str().c_str(), StatusCallback::STATUS_SEVERITY_ERROR, __FUNC__ );
		return;
	}

	// get length of file:
	infile.seekg( 0, std::ios::end );
	const int length = infile.tellg();
	infile.seekg( 0, std::ios::beg );

	// allocate memory:
	std::string buffer( length, '\0' );

	// read data as a block:
	infile.read( &buffer[0], length );
	infile.close();

	progressCallback( 0, "parse", L"Reading STEP data..." );

	try
	{
		m_step_reader->removeComments( buffer );
		m_step_reader->readStreamHeader( buffer, m_ifc_model );
		m_step_reader->readStreamData( buffer, m_ifc_model );
		m_ifc_model->resolveInverseAttributes();
		m_ifc_model->updateCache();
	}
	catch( IfcPPOutOfMemoryException& e)
	{
		throw e;
	}
	catch( IfcPPException& e )
	{
		messageCallback( e.what(), StatusCallback::STATUS_SEVERITY_ERROR, "" );
	}
	catch( std::exception& e )
	{
		messageCallback( e.what(), StatusCallback::STATUS_SEVERITY_ERROR, "" );
	}
	catch( ... )
	{
		messageCallback( "An error occured", StatusCallback::STATUS_SEVERITY_ERROR, __FUNC__ );
	}
}

void ReaderWriterIFC::createGeometryOSG( osg::ref_ptr<osg::Switch> parent_group )
{
	progressCallback( 0, "geometry", L"Creating geometry..." );
	m_shape_input_data.clear();
	m_map_outside_spatial_structure.clear();
	m_representation_converter->clearCache();
	
	std::vector<shared_ptr<IfcProduct> > vec_products;
	double length_to_meter_factor = m_ifc_model->getUnitConverter()->getLengthInMeterFactor();
	carve::setEpsilon( 1.4901161193847656e-08*length_to_meter_factor );

	const std::map<int, shared_ptr<IfcPPEntity> >& map_entities = m_ifc_model->getMapIfcEntities();
	for( std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it = map_entities.begin(); it != map_entities.end(); ++it )
	{
		shared_ptr<IfcPPEntity> obj = it->second;
		shared_ptr<IfcProduct> product = dynamic_pointer_cast<IfcProduct>( obj );
		if( product )
		{
			vec_products.push_back( product );
		}
	}

	// create geometry for for each IfcProduct independently, spatial structure will be resolved later
	std::map<int, shared_ptr<ShapeInputData> > *map_products_ptr = &m_shape_input_data;
	const int num_products = vec_products.size();

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
			shared_ptr<ShapeInputData> product_geom_input_data( new ShapeInputData() );
			product_geom_input_data->ifc_product = product;

			try
			{
				convertIfcProduct( product, product_geom_input_data );
			}
#ifdef _DEBUG
			catch( DebugBreakException& dbge )
			{
				throw dbge;
			}
#endif
			catch( IfcPPOutOfMemoryException& e)
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
					messageCallback( thread_err.str().c_str(), StatusCallback::STATUS_SEVERITY_ERROR, __FUNC__ );
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
					progressCallback( progress*0.9, "geometry" );
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
			m_map_visited.clear();
			resolveProjectStructure( ifc_project, parent_group );
		}

		// check if there are entities that are not in spatial structure
		osg::ref_ptr<osg::Group> group_outside_spatial_structure = new osg::Group();
		group_outside_spatial_structure->setName( "Entities not in spatial structure" );
		
		for( auto it_product_shapes = m_shape_input_data.begin(); it_product_shapes != m_shape_input_data.end(); ++it_product_shapes )
		{
			shared_ptr<ShapeInputData> product_shape = it_product_shapes->second;
			shared_ptr<IfcProduct> ifc_product( product_shape->ifc_product );
			if( !product_shape )
			{
				continue;
			}
			if( !product_shape->added_to_node )
			{
				shared_ptr<IfcFeatureElementSubtraction> opening = dynamic_pointer_cast<IfcFeatureElementSubtraction>( ifc_product );
				if( opening )
				{
					continue;
				}

				osg::ref_ptr<osg::Switch> product_switch = product_shape->product_switch;
				if( product_switch.valid() )
				{
					group_outside_spatial_structure->addChild( product_switch );
				}

				osg::ref_ptr<osg::Switch> product_switch_curves = product_shape->product_switch_curves;
				if( product_switch_curves.valid() )
				{
					group_outside_spatial_structure->addChild( product_switch_curves );
				}

				product_shape->added_to_node = true;
				m_map_outside_spatial_structure.insert( std::make_pair( ifc_product->m_id, ifc_product ) );
			}
		}

		if( group_outside_spatial_structure->getNumChildren() > 0 )
		{
			parent_group->addChild( group_outside_spatial_structure );
		}
	}
	catch( IfcPPOutOfMemoryException& e)
	{
		throw e;
	}
	catch( IfcPPException& e )
	{
		messageCallback( e.what(), StatusCallback::STATUS_SEVERITY_ERROR, "" );
	}
	catch( std::exception& e )
	{
		messageCallback( e.what(), StatusCallback::STATUS_SEVERITY_ERROR, "" );
	}
	catch( ... )
	{
		messageCallback( "undefined error", StatusCallback::STATUS_SEVERITY_ERROR, __FUNC__ );
	}

	m_representation_converter->getProfileCache()->clearProfileCache();
	progressCallback( 1.0, "geometry", L"Loading file done" );
}

void ReaderWriterIFC::resolveProjectStructure( const shared_ptr<IfcObjectDefinition>& parent_obj_def, osg::ref_ptr<osg::Switch> parent_group )
{
	int entity_id = parent_obj_def->m_id;
	if( m_map_visited.find( entity_id ) != m_map_visited.end() )
	{
		return;
	}
	m_map_visited[entity_id] = parent_obj_def;

	osg::ref_ptr<osg::Switch> next_parent;
	shared_ptr<IfcBuildingStorey> building_storey = dynamic_pointer_cast<IfcBuildingStorey>( parent_obj_def );
	if( building_storey )
	{
		// building storeys need a switch and also a transform node
		const int building_storey_id = building_storey->m_id;
		osg::Switch* switch_building_storey = new osg::Switch();
		if( !switch_building_storey )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		std::stringstream storey_switch_name;
		storey_switch_name << "#" << building_storey_id << "=IfcBuildingStorey switch";
		switch_building_storey->setName( storey_switch_name.str().c_str() );

		osg::ref_ptr<osg::MatrixTransform> transform_building_storey = new osg::MatrixTransform();
		if( !transform_building_storey )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		
		std::stringstream storey_name;
		storey_name << "#" << building_storey_id << "=IfcBuildingStorey transform";
		transform_building_storey->setName( storey_name.str().c_str() );

		parent_group->addChild( transform_building_storey );
		transform_building_storey->addChild( switch_building_storey );
		next_parent = switch_building_storey;
	}

	shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>( parent_obj_def );
	if( ifc_product )
	{
		std::map<int, shared_ptr<ShapeInputData> >::iterator it_product_map = m_shape_input_data.find( entity_id );
		if( it_product_map != m_shape_input_data.end() )
		{
			shared_ptr<ShapeInputData>& product_shape = it_product_map->second;

			if( product_shape->added_to_node )
			{
				std::cout << "already product_shape->added_to_node" << std::endl;
			}
			product_shape->added_to_node = true;

			osg::ref_ptr<osg::Switch> product_switch = product_shape->product_switch;
			if( product_switch.valid() )
			{
				parent_group->addChild( product_switch );
				next_parent = product_switch;
			}

			osg::ref_ptr<osg::Switch> product_switch_curves = product_shape->product_switch_curves;
			if( product_switch_curves.valid() )
			{
				parent_group->addChild( product_switch_curves );
			}
		}
	}

	if( parent_group->getName().size() < 1 )
	{
		std::stringstream switch_name;
		switch_name << "#" << entity_id << "=" << parent_obj_def->className();
		parent_group->setName( switch_name.str().c_str() );
	}

	shared_ptr<IfcSite> ifc_site = dynamic_pointer_cast<IfcSite>( parent_obj_def );
	if( ifc_site )
	{
		// Append representation of IfcSite (terrain) to a separate node.
		// When selecting or deleting terrain, children (buildings and all child items) should not be selected or deleted as well.
		//
		// m_group_result
		//    |- IfcProject
		//    |     |- IfcBuilding
		//    |     |     |- IfcBuildingStorey
		//    |     |     |      |----- IfcProduct
		//    |     |     |      |- ...
		//    |     |     |- IfcBuildingStorey
		//    |     |- IfcBuilding
		//    |- IfcSite    
		//                    

		next_parent = parent_group;
	}

	if( !next_parent )
	{
		next_parent = parent_group;
	}

	std::vector<weak_ptr<IfcRelAggregates> >& vec_IsDecomposedBy = parent_obj_def->m_IsDecomposedBy_inverse;
	for( auto it_decomposed_by = vec_IsDecomposedBy.begin(); it_decomposed_by != vec_IsDecomposedBy.end(); ++it_decomposed_by )
	{
		shared_ptr<IfcRelAggregates> rel_aggregates( *it_decomposed_by );
		std::vector<shared_ptr<IfcObjectDefinition> >& vec = rel_aggregates->m_RelatedObjects;

		for( auto it_object_def = vec.begin(); it_object_def != vec.end(); ++it_object_def )
		{
			shared_ptr<IfcObjectDefinition> child_obj_def = ( *it_object_def );
			resolveProjectStructure( child_obj_def, next_parent );
		}
	}

	shared_ptr<IfcSpatialStructureElement> spatial_ele = dynamic_pointer_cast<IfcSpatialStructureElement>( parent_obj_def );
	if( spatial_ele )
	{
		std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >& vec_contained = spatial_ele->m_ContainsElements_inverse;
		for( auto it_rel_contained = vec_contained.begin(); it_rel_contained != vec_contained.end(); ++it_rel_contained )
		{
			shared_ptr<IfcRelContainedInSpatialStructure> rel_contained( *it_rel_contained );
			std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained->m_RelatedElements;

			for( auto it_related = vec_related_elements.begin(); it_related != vec_related_elements.end(); ++it_related )
			{
				shared_ptr<IfcProduct> related_product = ( *it_related );
				resolveProjectStructure( related_product, next_parent );
			}
		}
	}
}


//\brief creates geometry objects from an IfcProduct object
// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
void ReaderWriterIFC::convertIfcProduct( const shared_ptr<IfcProduct>& product, shared_ptr<ShapeInputData>& product_shape )
{
	if( !product->m_Representation )
	{
		// IfcProduct needs to have a representation
		return;
	}

	const int product_id = product->m_id;
	osg::ref_ptr<osg::Switch> product_switch = new osg::Switch();
	osg::ref_ptr<osg::Switch> product_switch_curves = new osg::Switch();
	if( !product_switch || !product_switch_curves )
	{
		throw IfcPPOutOfMemoryException( __FUNC__ );
	}
	std::stringstream group_name;
	group_name << "#" << product_id << "=" << product->className() << " group";
	product_switch->setName( group_name.str().c_str() );
	product_switch_curves->setName( "CurveRepresentation" );
	const double length_factor = m_representation_converter->getUnitConverter()->getLengthInMeterFactor();

	// evaluate IFC geometry
	shared_ptr<IfcProductRepresentation>& product_representation = product->m_Representation;
	std::vector<shared_ptr<IfcRepresentation> >& vec_representations = product_representation->m_Representations;
	for( auto it_representations = vec_representations.begin(); it_representations != vec_representations.end(); ++it_representations )
	{
		shared_ptr<IfcRepresentation> representation = ( *it_representations );
		try
		{
			m_representation_converter->convertIfcRepresentation( representation, product_shape );
		}
		catch( IfcPPOutOfMemoryException& e)
		{
			throw e;
		}
		catch( IfcPPException& e )
		{
			messageCallback( e.what(), StatusCallback::STATUS_SEVERITY_ERROR, "" );
		}
		catch( std::exception& e )
		{
			messageCallback( e.what(), StatusCallback::STATUS_SEVERITY_ERROR, "" );
		}
	}

	// IfcProduct has an ObjectPlacement that can be local or global
	carve::math::Matrix product_placement_matrix( carve::math::Matrix::IDENT() );
	if( product->m_ObjectPlacement )
	{
		// IfcPlacement2Matrix follows related placements in case of local coordinate systems
		std::unordered_set<IfcObjectPlacement*> placement_already_applied;
		m_representation_converter->getPlacementConverter()->convertIfcObjectPlacement( product->m_ObjectPlacement, length_factor, product_placement_matrix, placement_already_applied );
	}

	std::vector<shared_ptr<ItemData> >& product_items = product_shape->vec_item_data;
	for( size_t i_item = 0; i_item < product_items.size(); ++i_item )
	{
		shared_ptr<ItemData> item_data = product_items[i_item];
		item_data->applyPosition( product_placement_matrix );
	}

	// handle openings
	std::vector<shared_ptr<ShapeInputData> > vec_opening_data;
	const shared_ptr<IfcElement> ifc_element = dynamic_pointer_cast<IfcElement>( product );
	if( ifc_element )
	{
		m_representation_converter->subtractOpenings( ifc_element, product_shape );
	}

	// create OSG objects
	for( size_t i_item = 0; i_item < product_items.size(); ++i_item )
	{
		shared_ptr<ItemData> item_data = product_items[i_item];
		osg::ref_ptr<osg::Group> item_group = new osg::Group();
		osg::ref_ptr<osg::Group> item_group_curves = new osg::Group();

		if( !item_group || !item_group_curves )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}

		// create shape for open shells
		for( auto it_meshsets = item_data->meshsets_open.begin(); it_meshsets != item_data->meshsets_open.end(); ++it_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = ( *it_meshsets );
			CSG_Adapter::retriangulateMeshSet( item_meshset );
			osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			if( !geode )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			ConverterOSG::drawMeshSet( item_meshset.get(), geode, m_geom_settings->m_intermediate_normal_angle );

			// disable back face culling for open meshes
			geode->getOrCreateStateSet()->setAttributeAndModes( m_cull_back_off.get(), osg::StateAttribute::OFF );
			item_group->addChild( geode );
		}

		// create shape for meshsets
		for( auto it_meshsets = item_data->meshsets.begin(); it_meshsets != item_data->meshsets.end(); ++it_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = ( *it_meshsets );
			CSG_Adapter::retriangulateMeshSet( item_meshset );
			osg::ref_ptr<osg::Geode> geode_result = new osg::Geode();
			if( !geode_result )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			ConverterOSG::drawMeshSet( item_meshset.get(), geode_result, m_geom_settings->m_intermediate_normal_angle );
			item_group->addChild( geode_result );
		}

		// create shape for polylines
		for( size_t polyline_i = 0; polyline_i < item_data->polylines.size(); ++polyline_i )
		{
			shared_ptr<carve::input::PolylineSetData>& polyline_data = item_data->polylines[polyline_i];
			osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			if( !geode )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			ConverterOSG::drawPolyline( polyline_data.get(), geode );
			item_group_curves->addChild( geode );
		}

		if( m_geom_settings->m_show_text_literals )
		{
			for( size_t text_literal_i = 0; text_literal_i < item_data->vec_text_literals.size(); ++text_literal_i )
			{
				shared_ptr<TextItemData>& text_data = item_data->vec_text_literals[text_literal_i];
				if( !text_data )
				{
					continue;
				}
				carve::math::Matrix& text_pos = text_data->m_text_position;
				// TODO: handle rotation

				std::string text_str;
				text_str.assign( text_data->m_text.begin(), text_data->m_text.end() );

				osg::Vec3 pos2( text_pos._41, text_pos._42, text_pos._43 );

				osgText::Text* txt = new osgText::Text();
				if( !txt )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}
				txt->setFont( "fonts/arial.ttf" );
				txt->setColor( osg::Vec4f( 0, 0, 0, 1 ) );
				txt->setCharacterSize( 0.1f );
				txt->setAutoRotateToScreen( true );
				txt->setPosition( pos2 );
				txt->setText( text_str.c_str() );
				txt->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

				osg::ref_ptr<osg::Geode> geode = new osg::Geode();
				if( !geode )
				{
					throw IfcPPOutOfMemoryException( __FUNC__ );
				}
				geode->addDrawable( txt );
				item_group->addChild( geode );
			}
		}

		// apply statesets if there are any
		if( item_data->vec_item_appearances.size() > 0 )
		{
			for( size_t i_appearance = 0; i_appearance < item_data->vec_item_appearances.size(); ++i_appearance )
			{
				shared_ptr<AppearanceData>& appearance = item_data->vec_item_appearances[i_appearance];
				if( appearance->apply_to_geometry_type == AppearanceData::SURFACE )
				{
					osg::StateSet* item_stateset = AppearanceManagerOSG::convertToStateSet( appearance );
					if( item_stateset != nullptr )
					{
						item_group->setStateSet( item_stateset );

						osg::StateSet* existing_item_stateset = item_group->getStateSet();
						if( existing_item_stateset )
						{
							osg::StateSet* merged_product_stateset = new osg::StateSet( *existing_item_stateset );
							if( !merged_product_stateset )
							{
								throw IfcPPOutOfMemoryException( __FUNC__ );
							}
							merged_product_stateset->merge( *item_stateset );
							item_group->setStateSet( merged_product_stateset );
						}
						else
						{
							item_group->setStateSet( item_stateset );
						}
					}
					else if( appearance->apply_to_geometry_type == AppearanceData::CURVE )
					{
						osg::StateSet* item_stateset = AppearanceManagerOSG::convertToStateSet( appearance );
						if( item_stateset != nullptr )
						{
							item_group->setStateSet( item_stateset );

							osg::StateSet* existing_item_stateset = item_group->getStateSet();

							if( existing_item_stateset )
							{
								osg::StateSet* merged_product_stateset = new osg::StateSet( *existing_item_stateset );
								if( !merged_product_stateset )
								{
									throw IfcPPOutOfMemoryException( __FUNC__ );
								}
								merged_product_stateset->merge( *item_stateset );
								item_group->setStateSet( merged_product_stateset );
							}
							else
							{
								item_group_curves->setStateSet( item_stateset );
							}
						}
					}
				}
			}
		}

		// If anything has been created, add it to the product group
		if( item_group->getNumChildren() > 0 )
		{
			product_switch->addChild( item_group );
		}

		if( item_group_curves->getNumChildren() > 0 )
		{
			product_switch_curves->addChild( item_group_curves );
		}
	}

	// Fetch the IFCProduct relationships
	if( product->m_IsDefinedBy_inverse.size() > 0 )
	{
		std::vector<weak_ptr<IfcRelDefinesByProperties> >& vec_IsDefinedBy_inverse = product->m_IsDefinedBy_inverse;
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
						m_representation_converter->convertIfcPropertySet( property_set, product_switch.get() );
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
								m_representation_converter->convertIfcPropertySet( property_set, product_switch.get() );
							}
						}
					}
					continue;
				}
			}
		}
	}

	for( size_t i = 0; i < product_shape->getAppearances().size(); ++i )
	{
		// TODO: handle appearances of curves separately
		const shared_ptr<AppearanceData>& appearance = product_shape->getAppearances()[i];
		if( !appearance )
		{
			continue;
		}

		osg::StateSet* next_product_stateset = AppearanceManagerOSG::convertToStateSet( appearance );
		osg::StateSet* existing_item_stateset = product_switch->getStateSet();
		if( existing_item_stateset )
		{
			osg::StateSet* merged_product_stateset = new osg::StateSet( *existing_item_stateset );
			if( !merged_product_stateset )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			merged_product_stateset->merge( *next_product_stateset );
			product_switch->setStateSet( merged_product_stateset );
		}
		else
		{
			product_switch->setStateSet( next_product_stateset );
		}
	}

	// enable transparency for certain objects
	if( dynamic_pointer_cast<IfcSpace>( product ) )
	{
		product_switch->setStateSet( m_glass_stateset );
	}
	else if( dynamic_pointer_cast<IfcCurtainWall>( product ) || dynamic_pointer_cast<IfcWindow>( product ) )
	{
		// TODO: make only glass part of window transparent
		product_switch->setStateSet( m_glass_stateset );
		GeomUtils::setMaterialAlpha( product_switch, 0.6f );
	}
	else if( dynamic_pointer_cast<IfcSite>( product ) )
	{
		std::stringstream group_name;
		group_name << "#" << product_id << "=IfcSite";
		product_switch->setName( group_name.str().c_str() );
	}
	// TODO: if no color or material is given, set color 231/219/169 for walls, 140/140/140 for slabs 

	if( product_switch->getNumChildren() > 0 )
	{
		product_shape->product_switch = product_switch;
	}

	if( product_switch_curves->getNumChildren() > 0 )
	{
		product_shape->product_switch_curves = product_switch_curves;
	}
}

void ReaderWriterIFC::slotProgressWrapper( void* obj_ptr, double value, const std::string& type, const std::wstring& txt )
{
	ReaderWriterIFC* myself = (ReaderWriterIFC*)obj_ptr;
	if( myself )
	{
		myself->progressCallback( value, type, txt );
	}
}

void ReaderWriterIFC::slotMessageWrapper( void* ptr, shared_ptr<StatusCallback::Ticket> t )
{
	ReaderWriterIFC* myself = (ReaderWriterIFC*)ptr;
	if( myself )
	{
		if( t->m_entity )
		{
#ifdef IFCPP_OPENMP
			ScopedLock lock( myself->m_writelock_tickets );
#endif

			// make sure that the same message for one entity does not appear several times
			const int entity_id = t->m_entity->m_id;

			std::map<int, std::vector<shared_ptr<StatusCallback::Ticket> > >::iterator it = myself->m_tickets.find( entity_id );
			if( it != myself->m_tickets.end() )
			{
				std::vector<shared_ptr<StatusCallback::Ticket> >& vec_tickets_for_entity = it->second;
				for( size_t i = 0; i < vec_tickets_for_entity.size(); ++i )
				{
					shared_ptr<StatusCallback::Ticket>& existing_ticket = vec_tickets_for_entity[i];
					if( existing_ticket->m_message.compare( t->m_message ) == 0 )
					{
						// same message for same entity is already there, so ignore ticket
						return;
					}
				}
				vec_tickets_for_entity.push_back( t );
			}
			else
			{
				std::vector<shared_ptr<StatusCallback::Ticket> >& vec = myself->m_tickets.insert( std::make_pair( entity_id, std::vector<shared_ptr<StatusCallback::Ticket> >() ) ).first->second;
				vec.push_back( t );
			}
		}

		myself->messageCallback( t );
	}
}

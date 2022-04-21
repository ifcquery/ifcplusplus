#include <fstream>
#include <sstream>

#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/reader/ReaderUtil.h>
#include <ifcpp/writer/WriterSTEP.h>
#include <ifcpp/writer/WriterUtil.h>

#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/BuildingGuid.h>

#include <ifcpp/IFC4/include/IfcArbitraryClosedProfileDef.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcBoolean.h>
#include <ifcpp/IFC4/include/IfcBoundingBox.h>
#include <ifcpp/IFC4/include/IfcBuilding.h>
#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcClosedShell.h>
#include <ifcpp/IFC4/include/IfcDimensionCount.h>
#include <ifcpp/IFC4/include/IfcDimensionalExponents.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcExtrudedAreaSolid.h>
#include <ifcpp/IFC4/include/IfcFace.h>
#include <ifcpp/IFC4/include/IfcFacetedBrep.h>
#include <ifcpp/IFC4/include/IfcFaceOuterBound.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationContext.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLocalPlacement.h>
#include <ifcpp/IFC4/include/IfcOpeningElement.h>
#include <ifcpp/IFC4/include/IfcOrganization.h>
#include <ifcpp/IFC4/include/IfcOwnerHistory.h>
#include <ifcpp/IFC4/include/IfcPerson.h>
#include <ifcpp/IFC4/include/IfcPersonAndOrganization.h>
#include <ifcpp/IFC4/include/IfcPolyline.h>
#include <ifcpp/IFC4/include/IfcPolyLoop.h>
#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcProductDefinitionShape.h>
#include <ifcpp/IFC4/include/IfcPropertySet.h>
#include <ifcpp/IFC4/include/IfcPropertySingleValue.h>
#include <ifcpp/IFC4/include/IfcReal.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcRelDefinesByProperties.h>
#include <ifcpp/IFC4/include/IfcRelVoidsElement.h>
#include <ifcpp/IFC4/include/IfcShapeRepresentation.h>
#include <ifcpp/IFC4/include/IfcSite.h>
#include <ifcpp/IFC4/include/IfcSIUnit.h>
#include <ifcpp/IFC4/include/IfcText.h>
#include <ifcpp/IFC4/include/IfcUnitAssignment.h>
#include <ifcpp/IFC4/include/IfcUnitEnum.h>
#include <ifcpp/IFC4/include/IfcWall.h>

inline void convertPlacement(double local_x[3], double local_z[3], double location[3], shared_ptr<IfcAxis2Placement3D>& axis2placement3d, std::vector<shared_ptr<BuildingEntity> >& vec_new_entities)
{
	if (!axis2placement3d)
	{
		axis2placement3d = shared_ptr<IfcAxis2Placement3D>(new IfcAxis2Placement3D());
		vec_new_entities.push_back(axis2placement3d);
	}

	if (!axis2placement3d->m_Location)
	{
		axis2placement3d->m_Location = shared_ptr<IfcCartesianPoint>(new IfcCartesianPoint());
		vec_new_entities.push_back(axis2placement3d->m_Location);
	}
	axis2placement3d->m_Location->m_Coordinates.clear();
	axis2placement3d->m_Location->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(location[0])));
	axis2placement3d->m_Location->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(location[1])));
	axis2placement3d->m_Location->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(location[2])));

	if (!axis2placement3d->m_Axis)
	{
		axis2placement3d->m_Axis = shared_ptr<IfcDirection>(new IfcDirection());
		vec_new_entities.push_back(axis2placement3d->m_Axis);
	}
	axis2placement3d->m_Axis->m_DirectionRatios.clear();
	axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[0])));
	axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[1])));
	axis2placement3d->m_Axis->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_z[2])));

	if (!axis2placement3d->m_RefDirection)
	{
		axis2placement3d->m_RefDirection = shared_ptr<IfcDirection>(new IfcDirection());
		vec_new_entities.push_back(axis2placement3d->m_RefDirection);
	}

	axis2placement3d->m_RefDirection->m_DirectionRatios.clear();
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[0])));
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[1])));
	axis2placement3d->m_RefDirection->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(local_x[2])));
}


shared_ptr<IfcDirection> createIfcDirection(double x, double y, double z, std::vector<shared_ptr<BuildingEntity> >& vec_new_entities)
{
	shared_ptr<IfcDirection> pt(new IfcDirection());
	pt->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(x)));
	pt->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(y)));
	pt->m_DirectionRatios.push_back(shared_ptr<IfcReal>(new IfcReal(z)));
	vec_new_entities.push_back(pt);
	return pt;
}


shared_ptr<IfcCartesianPoint> createIfcCartesianPoint(double x, double y, std::vector<shared_ptr<BuildingEntity> >& vec_new_entities)
{
	shared_ptr<IfcCartesianPoint> pt(new IfcCartesianPoint());
	pt->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(x)));
	pt->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(y)));
	vec_new_entities.push_back(pt);
	return pt;
}

shared_ptr<IfcCartesianPoint> createIfcCartesianPoint(double x, double y, double z, std::vector<shared_ptr<BuildingEntity> >& vec_new_entities)
{
	shared_ptr<IfcCartesianPoint> pt(new IfcCartesianPoint());
	pt->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(x)));
	pt->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(y)));
	pt->m_Coordinates.push_back(shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(z)));
	vec_new_entities.push_back(pt);
	return pt;
}

shared_ptr<IfcFace> createIfcFace(
	shared_ptr<IfcCartesianPoint> pt0,
	shared_ptr<IfcCartesianPoint> pt1,
	shared_ptr<IfcCartesianPoint> pt2,
	shared_ptr<IfcCartesianPoint> pt3,
	std::vector<shared_ptr<BuildingEntity> >& vec_new_entities)
{
	shared_ptr<IfcFace> face(new IfcFace());
	vec_new_entities.push_back(face);

	// a face needs a face bound and an orientation
	shared_ptr<IfcFaceOuterBound> face_bound1(new IfcFaceOuterBound());
	face_bound1->m_Orientation = shared_ptr<IfcBoolean>(new IfcBoolean(true));
	face->m_Bounds.push_back(face_bound1);
	vec_new_entities.push_back(face_bound1);

	shared_ptr<IfcPolyLoop> poly_loop(new IfcPolyLoop());
	face_bound1->m_Bound = poly_loop;
	poly_loop->m_Polygon.push_back(pt0);
	poly_loop->m_Polygon.push_back(pt1);
	poly_loop->m_Polygon.push_back(pt2);
	poly_loop->m_Polygon.push_back(pt3);
	vec_new_entities.push_back(poly_loop);
	return face;
}

/**
\param geometry_type: 1 for extruded solid, 2 for brep
*/
inline void LoadWallExample(shared_ptr<BuildingModel>& ifc_model, bool add_property_set = true, bool add_window = true, int geometry_type = 1)
{
	if( !ifc_model )
	{
		// create IFC model
		ifc_model = shared_ptr<BuildingModel>(new BuildingModel());
	}
	double unit_length_factor = 1.0;
	ifc_model->clearIfcModel();
	ifc_model->getUnitConverter()->setLengthInMeterFactor(unit_length_factor);
	std::vector<shared_ptr<BuildingEntity> > vec_new_entities;

	// create wall object
	shared_ptr<IfcWall> wall(new IfcWall());
	vec_new_entities.push_back(wall);
	wall->m_Name = shared_ptr<IfcLabel>(new IfcLabel());
	wall->m_Name->m_value = L"Hello Wall";
	wall->m_Description = shared_ptr<IfcText>(new IfcText());
	wall->m_Description->m_value = L"Wall example";

	if( add_property_set )
	{
		// Example for property set
		shared_ptr<IfcPropertySet> ifc_property_set(new IfcPropertySet());
		vec_new_entities.push_back(ifc_property_set);

		ifc_property_set->m_Name = shared_ptr<IfcLabel>(new IfcLabel());
		ifc_property_set->m_Name->m_value = L"Pset_Wall";   // name of the property set

		ifc_property_set->m_Description = shared_ptr<IfcText>(new IfcText());
		ifc_property_set->m_Description->m_value = L"Pset to define Wall";

		std::vector<shared_ptr<IfcProperty> >& vec_properties = ifc_property_set->m_HasProperties;

		{
			shared_ptr<IfcPropertySingleValue> prop1(new IfcPropertySingleValue());
			vec_new_entities.push_back(prop1);
			vec_properties.push_back(prop1);
			prop1->m_Name = shared_ptr<IfcIdentifier>(new IfcIdentifier());  // name of property 1
			prop1->m_Name->m_value = L"WallIentifier";

			shared_ptr<IfcIdentifier> prop1_value(new IfcIdentifier());  // value property
			prop1_value->m_value = L"W1";
			prop1->m_NominalValue = prop1_value;
		}

		{
			shared_ptr<IfcPropertySingleValue> prop2(new IfcPropertySingleValue());
			vec_new_entities.push_back(prop2);
			vec_properties.push_back(prop2);
			prop2->m_Name = shared_ptr<IfcIdentifier>(new IfcIdentifier());
			prop2->m_Name->m_value = L"Footprint area";

			shared_ptr<IfcReal> prop2_value(new IfcReal());
			prop2_value->m_value = 0.8;
			prop2->m_NominalValue = prop2_value;
		}

		shared_ptr<IfcRelDefinesByProperties> ifc_rel_defines(new IfcRelDefinesByProperties());
		vec_new_entities.push_back(ifc_rel_defines);

		ifc_rel_defines->m_RelatedObjects.clear();
		ifc_rel_defines->m_RelatedObjects.push_back(wall);
		ifc_rel_defines->m_RelatingPropertyDefinition = ifc_property_set;
	}

	// geometry
	double lower_level = 0.2*unit_length_factor;
	double upper_level = 3.5*unit_length_factor;
	double wall_length = 5.0*unit_length_factor;
	double wall_thickness = 0.25*unit_length_factor;

	shared_ptr<IfcShapeRepresentation> shape_representation(new IfcShapeRepresentation());
	vec_new_entities.push_back(shape_representation);
	shape_representation->m_RepresentationIdentifier = shared_ptr<IfcLabel>(new IfcLabel(L"Body"));

	if( geometry_type == 1 )
	{
		shape_representation->m_RepresentationType = shared_ptr<IfcLabel>(new IfcLabel(L"SweptSolid"));

		shared_ptr<IfcProductDefinitionShape> product_def_shape(new IfcProductDefinitionShape());
		vec_new_entities.push_back(product_def_shape);

		wall->m_Representation = product_def_shape;
		product_def_shape->m_Representations.push_back(shape_representation);

		// extruded solid
		shared_ptr<IfcExtrudedAreaSolid> extruded_solid(new IfcExtrudedAreaSolid());
		vec_new_entities.push_back(extruded_solid);

		// position of the solid
		extruded_solid->m_Position = shared_ptr<IfcAxis2Placement3D>(new IfcAxis2Placement3D());
		vec_new_entities.push_back(extruded_solid->m_Position);
		extruded_solid->m_Position->m_Axis = createIfcDirection(0, 0, 1, vec_new_entities);  // z axis
		vec_new_entities.push_back(extruded_solid->m_Position->m_Axis);

		extruded_solid->m_Position->m_RefDirection = createIfcDirection(1, 0, 0, vec_new_entities);  // x axis
		vec_new_entities.push_back(extruded_solid->m_Position->m_RefDirection);

		extruded_solid->m_Position->m_Location = createIfcCartesianPoint(0, 0, lower_level, vec_new_entities);
		vec_new_entities.push_back(extruded_solid->m_Position->m_Location);

		extruded_solid->m_ExtrudedDirection = createIfcDirection(0, 0, 1, vec_new_entities);   // extrusion direction
		vec_new_entities.push_back(extruded_solid->m_ExtrudedDirection);

		// length of extrusion:
		extruded_solid->m_Depth = shared_ptr<IfcPositiveLengthMeasure>(new IfcPositiveLengthMeasure());
		extruded_solid->m_Depth->m_value = (upper_level - lower_level);

		// swept area as closed profile:
		shared_ptr<IfcArbitraryClosedProfileDef> swept_area(new IfcArbitraryClosedProfileDef());
		vec_new_entities.push_back(swept_area);
		extruded_solid->m_SweptArea = swept_area;

		// outer curve of closed profile is a polyline:
		shared_ptr<IfcPolyline> poly_line(new IfcPolyline());
		vec_new_entities.push_back(poly_line);
		swept_area->m_OuterCurve = poly_line;

		// these four points define the 2d polyline that is going to be extruded in z-direction:
		shared_ptr<IfcCartesianPoint> ifc_point1 = createIfcCartesianPoint(0, 0, vec_new_entities);
		vec_new_entities.push_back(ifc_point1);
		poly_line->m_Points.push_back(ifc_point1);

		shared_ptr<IfcCartesianPoint> ifc_point2 = createIfcCartesianPoint(wall_length, 0, vec_new_entities);
		vec_new_entities.push_back(ifc_point2);
		poly_line->m_Points.push_back(ifc_point2);

		shared_ptr<IfcCartesianPoint> ifc_point3 = createIfcCartesianPoint(wall_length, wall_thickness, vec_new_entities);
		vec_new_entities.push_back(ifc_point3);
		poly_line->m_Points.push_back(ifc_point3);

		shared_ptr<IfcCartesianPoint> ifc_point4 = createIfcCartesianPoint(0.0, wall_thickness, vec_new_entities);
		vec_new_entities.push_back(ifc_point4);
		poly_line->m_Points.push_back(ifc_point4);

		shape_representation->m_Items.push_back(extruded_solid);
	}
	else
	{
		shape_representation->m_RepresentationType = shared_ptr<IfcLabel>(new IfcLabel(L"BRep"));

		shared_ptr<IfcProductDefinitionShape> product_def_shape(new IfcProductDefinitionShape());
		vec_new_entities.push_back(product_def_shape);

		wall->m_Representation = product_def_shape;
		product_def_shape->m_Representations.push_back(shape_representation);


		//#93 = IFCCLOSEDSHELL( (#100, #107, #110, #113, #116, #119) );
		//#94 = IFCPOLYLOOP( (#95, #96, #97, #98) );
		//#95 = IFCCARTESIANPOINT( (0., 0., 0.) );
		//#96 = IFCCARTESIANPOINT( (0., 200., 0.) );
		//#97 = IFCCARTESIANPOINT( (700., 200., 0.) );
		//#98 = IFCCARTESIANPOINT( (700., 0., 0.) );
		//#99 = IFCFACEOUTERBOUND( #94, .T. );
		//#100 = IFCFACE( (#99) );
		//#101 = IFCPOLYLOOP( (#102, #103, #104, #105) );
		//#102 = IFCCARTESIANPOINT( (0., 0., 600.) );
		//#103 = IFCCARTESIANPOINT( (700., 0., 600.) );
		//#104 = IFCCARTESIANPOINT( (700., 200., 600.) );
		//#105 = IFCCARTESIANPOINT( (0., 200., 600.) );
		//#106 = IFCFACEOUTERBOUND( #101, .T. );
		//#107 = IFCFACE( (#106) );
		//#108 = IFCPOLYLOOP( (#95, #102, #105, #96) );
		//#109 = IFCFACEOUTERBOUND( #108, .T. );
		//#110 = IFCFACE( (#109) );
		//#111 = IFCPOLYLOOP( (#96, #105, #104, #97) );
		//#112 = IFCFACEOUTERBOUND( #111, .T. );
		//#113 = IFCFACE( (#112) );
		//#114 = IFCPOLYLOOP( (#97, #104, #103, #98) );
		//#115 = IFCFACEOUTERBOUND( #114, .T. );
		//#116 = IFCFACE( (#115) );
		//#117 = IFCPOLYLOOP( (#98, #103, #102, #95) );
		//#118 = IFCFACEOUTERBOUND( #117, .T. );
		//#119 = IFCFACE( (#118) );
		//#120 = IFCFACETEDBREP( #93 );


		// faceted brep
		shared_ptr<IfcFacetedBrep> faceted_brep(new IfcFacetedBrep());
		vec_new_entities.push_back(faceted_brep);

		// the faceted brep's outer bound is a closed shell:
		shared_ptr<IfcClosedShell> closed_shell(new IfcClosedShell());
		vec_new_entities.push_back(closed_shell);
		faceted_brep->m_Outer = closed_shell;


		//       7----------------6
		//      /|                |
		//     / 4----------------5
		//    / /                /
		//   3 /                / <- 2 (hidden)
		//   |/                /
		//   0----------------1

		// we need 8 points to define a cube
		shared_ptr<IfcCartesianPoint> point0 = createIfcCartesianPoint(0.0, 0.0, lower_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point1 = createIfcCartesianPoint(wall_length, 0.0, lower_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point2 = createIfcCartesianPoint(wall_length, wall_thickness, lower_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point3 = createIfcCartesianPoint(0.0, wall_thickness, lower_level, vec_new_entities);

		shared_ptr<IfcCartesianPoint> point4 = createIfcCartesianPoint(0.0, 0.0, upper_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point5 = createIfcCartesianPoint(wall_length, 0.0, upper_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point6 = createIfcCartesianPoint(wall_length, wall_thickness, upper_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point7 = createIfcCartesianPoint(0.0, wall_thickness, upper_level, vec_new_entities);

		// the closed shell consists of several faces
		shared_ptr<IfcFace> face0 = createIfcFace(point0, point3, point2, point1, vec_new_entities);
		shared_ptr<IfcFace> face1 = createIfcFace(point4, point5, point6, point7, vec_new_entities);
		shared_ptr<IfcFace> face2 = createIfcFace(point0, point1, point5, point4, vec_new_entities);
		shared_ptr<IfcFace> face3 = createIfcFace(point1, point2, point6, point5, vec_new_entities);
		shared_ptr<IfcFace> face4 = createIfcFace(point2, point3, point7, point6, vec_new_entities);
		shared_ptr<IfcFace> face5 = createIfcFace(point3, point0, point4, point7, vec_new_entities);
		closed_shell->m_CfsFaces.push_back(face0);
		closed_shell->m_CfsFaces.push_back(face1);
		closed_shell->m_CfsFaces.push_back(face2);
		closed_shell->m_CfsFaces.push_back(face3);
		closed_shell->m_CfsFaces.push_back(face4);
		closed_shell->m_CfsFaces.push_back(face5);

		shape_representation->m_Items.push_back(faceted_brep);

		// add bounding box representation
		shared_ptr<IfcShapeRepresentation> shape_representation_bbox(new IfcShapeRepresentation());
		vec_new_entities.push_back(shape_representation_bbox);
		shape_representation_bbox->m_RepresentationIdentifier = shared_ptr<IfcLabel>(new IfcLabel(L"Box"));
		shape_representation_bbox->m_RepresentationType = shared_ptr<IfcLabel>(new IfcLabel(L"BoundingBox"));
		shared_ptr<IfcBoundingBox> bbox(new IfcBoundingBox());
		vec_new_entities.push_back(bbox);
		bbox->m_Corner = createIfcCartesianPoint(0, 0, lower_level, vec_new_entities);
		bbox->m_XDim = shared_ptr<IfcPositiveLengthMeasure>(new IfcPositiveLengthMeasure(wall_length));
		bbox->m_YDim = shared_ptr<IfcPositiveLengthMeasure>(new IfcPositiveLengthMeasure(wall_thickness));
		bbox->m_ZDim = shared_ptr<IfcPositiveLengthMeasure>(new IfcPositiveLengthMeasure(upper_level - lower_level));

		shape_representation_bbox->m_Items.push_back(bbox);

		product_def_shape->m_Representations.push_back(shape_representation_bbox);

	}




	shared_ptr<IfcOpeningElement> opening;
	if( add_window )
	{
		//#84 = IFCOPENINGELEMENT( '3yEl4kjB98e9jYEmmNx99a', #2, 'Bathroom Window Opening', 'Description of Opening', $, #85, #90, $ );
		//#85 = IFCLOCALPLACEMENT( #46, #86 );
		//#86 = IFCAXIS2PLACEMENT3D( #87, #88, #89 );
		//#87 = IFCCARTESIANPOINT( (200., 0., 3850.) );
		//#88 = IFCDIRECTION( (0., 0., 1.) );
		//#89 = IFCDIRECTION( (1., 0., 0.) );
		//#90 = IFCPRODUCTDEFINITIONSHAPE( $, $, (#92, #121) );
		//#91 = IFCRELVOIDSELEMENT( '2bYxol3FX8ReDpDLJ4pBH0', #2, $, $, #45, #84 );
		//#92 = IFCSHAPEREPRESENTATION( #20, 'Body', 'Brep', (#120) );
		//#93 = IFCCLOSEDSHELL( (#100, #107, #110, #113, #116, #119) );
		//#94 = IFCPOLYLOOP( (#95, #96, #97, #98) );
		//#95 = IFCCARTESIANPOINT( (0., 0., 0.) );
		//#96 = IFCCARTESIANPOINT( (0., 200., 0.) );
		//#97 = IFCCARTESIANPOINT( (700., 200., 0.) );
		//#98 = IFCCARTESIANPOINT( (700., 0., 0.) );
		//#99 = IFCFACEOUTERBOUND( #94, .T. );
		//#100 = IFCFACE( (#99) );
		//#101 = IFCPOLYLOOP( (#102, #103, #104, #105) );
		//#102 = IFCCARTESIANPOINT( (0., 0., 600.) );
		//#103 = IFCCARTESIANPOINT( (700., 0., 600.) );
		//#104 = IFCCARTESIANPOINT( (700., 200., 600.) );
		//#105 = IFCCARTESIANPOINT( (0., 200., 600.) );
		//#106 = IFCFACEOUTERBOUND( #101, .T. );
		//#107 = IFCFACE( (#106) );
		//#108 = IFCPOLYLOOP( (#95, #102, #105, #96) );
		//#109 = IFCFACEOUTERBOUND( #108, .T. );
		//#110 = IFCFACE( (#109) );
		//#111 = IFCPOLYLOOP( (#96, #105, #104, #97) );
		//#112 = IFCFACEOUTERBOUND( #111, .T. );
		//#113 = IFCFACE( (#112) );
		//#114 = IFCPOLYLOOP( (#97, #104, #103, #98) );
		//#115 = IFCFACEOUTERBOUND( #114, .T. );
		//#116 = IFCFACE( (#115) );
		//#117 = IFCPOLYLOOP( (#98, #103, #102, #95) );
		//#118 = IFCFACEOUTERBOUND( #117, .T. );
		//#119 = IFCFACE( (#118) );
		//#120 = IFCFACETEDBREP( #93 );

		// opening element, same structure as wall
		opening = shared_ptr<IfcOpeningElement>(new IfcOpeningElement());
		vec_new_entities.push_back(opening);


		shared_ptr<IfcShapeRepresentation> shape_representation(new IfcShapeRepresentation());
		vec_new_entities.push_back(shape_representation);
		shape_representation->m_RepresentationIdentifier = shared_ptr<IfcLabel>(new IfcLabel(L"Body"));
		shape_representation->m_RepresentationType = shared_ptr<IfcLabel>(new IfcLabel(L"BRep"));

		shared_ptr<IfcProductDefinitionShape> product_def_shape(new IfcProductDefinitionShape());
		vec_new_entities.push_back(product_def_shape);

		opening->m_Representation = product_def_shape;
		product_def_shape->m_Representations.push_back(shape_representation);

		// faceted brep
		shared_ptr<IfcFacetedBrep> faceted_brep(new IfcFacetedBrep());
		vec_new_entities.push_back(faceted_brep);

		// the faceted brep's outer bound is a closed shell:
		shared_ptr<IfcClosedShell> closed_shell(new IfcClosedShell());
		vec_new_entities.push_back(closed_shell);
		faceted_brep->m_Outer = closed_shell;


		//       7----------------6
		//      /|                |
		//     / 4----------------5
		//    / /                /
		//   3 /                / <- 2 (hidden)
		//   |/                /
		//   0----------------1

		// we need 8 points to define a cube
		lower_level += 1.5;
		shared_ptr<IfcCartesianPoint> point0 = createIfcCartesianPoint(0.3, 0.0, lower_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point1 = createIfcCartesianPoint(wall_length * 0.5, 0.0, lower_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point2 = createIfcCartesianPoint(wall_length * 0.5, wall_thickness, lower_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point3 = createIfcCartesianPoint(0.3, wall_thickness, lower_level, vec_new_entities);

		upper_level -= 0.3;
		shared_ptr<IfcCartesianPoint> point4 = createIfcCartesianPoint(0.3, 0.0, upper_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point5 = createIfcCartesianPoint(wall_length * 0.5, 0.0, upper_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point6 = createIfcCartesianPoint(wall_length * 0.5, wall_thickness, upper_level, vec_new_entities);
		shared_ptr<IfcCartesianPoint> point7 = createIfcCartesianPoint(0.3, wall_thickness, upper_level, vec_new_entities);

		// the closed shell consists of several faces
		shared_ptr<IfcFace> face0 = createIfcFace(point0, point3, point2, point1, vec_new_entities);
		shared_ptr<IfcFace> face1 = createIfcFace(point4, point5, point6, point7, vec_new_entities);
		shared_ptr<IfcFace> face2 = createIfcFace(point0, point1, point5, point4, vec_new_entities);
		shared_ptr<IfcFace> face3 = createIfcFace(point1, point2, point6, point5, vec_new_entities);
		shared_ptr<IfcFace> face4 = createIfcFace(point2, point3, point7, point6, vec_new_entities);
		shared_ptr<IfcFace> face5 = createIfcFace(point3, point0, point4, point7, vec_new_entities);
		closed_shell->m_CfsFaces.push_back(face0);
		closed_shell->m_CfsFaces.push_back(face1);
		closed_shell->m_CfsFaces.push_back(face2);
		closed_shell->m_CfsFaces.push_back(face3);
		closed_shell->m_CfsFaces.push_back(face4);
		closed_shell->m_CfsFaces.push_back(face5);

		shape_representation->m_Items.push_back(faceted_brep);

		// IfcRelVoidsElement connects the opening element to the wall
		shared_ptr<IfcRelVoidsElement> rel_voids(new IfcRelVoidsElement());
		vec_new_entities.push_back(rel_voids);
		rel_voids->m_RelatedOpeningElement = opening;
		rel_voids->m_RelatingBuildingElement = wall;
	}

	// building structure
	shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
	if( !ifc_project )
	{
		ifc_project = shared_ptr<IfcProject>(new IfcProject());
		vec_new_entities.push_back(ifc_project);
		ifc_model->setIfcProject(ifc_project);
	}

	shared_ptr<IfcSite> ifc_site(new IfcSite());
	vec_new_entities.push_back(ifc_site);

	shared_ptr<IfcBuilding> ifc_building(new IfcBuilding());
	vec_new_entities.push_back(ifc_building);

	shared_ptr<IfcBuildingStorey> ifc_building_storey(new IfcBuildingStorey());
	vec_new_entities.push_back(ifc_building_storey);

	// relations object to connect site to project
	shared_ptr<IfcRelAggregates> rel_aggregates_project_site(new IfcRelAggregates());
	rel_aggregates_project_site->m_RelatingObject = ifc_project;
	rel_aggregates_project_site->m_RelatedObjects.push_back(ifc_site);
	vec_new_entities.push_back(rel_aggregates_project_site);

	// relations object to connect building to site
	shared_ptr<IfcRelAggregates> rel_aggregates_site_building(new IfcRelAggregates());
	rel_aggregates_site_building->m_RelatingObject = ifc_site;
	rel_aggregates_site_building->m_RelatedObjects.push_back(ifc_building);
	vec_new_entities.push_back(rel_aggregates_site_building);

	// relations object to connect building storey to building
	shared_ptr<IfcRelAggregates> rel_aggregates_building_buildingstorey(new IfcRelAggregates());
	rel_aggregates_building_buildingstorey->m_RelatingObject = ifc_building;
	rel_aggregates_building_buildingstorey->m_RelatedObjects.push_back(ifc_building_storey);
	vec_new_entities.push_back(rel_aggregates_building_buildingstorey);

	// relations object to connect wall to building storey
	shared_ptr<IfcRelContainedInSpatialStructure> rel_contained_buildingstorey_wall(new IfcRelContainedInSpatialStructure());
	rel_contained_buildingstorey_wall->m_RelatingStructure = ifc_building_storey;
	rel_contained_buildingstorey_wall->m_RelatedElements.push_back(wall);
	vec_new_entities.push_back(rel_contained_buildingstorey_wall);


	// coordinate systems
	double local_x[3] = { 1,0,0 };
	double local_z[3] = { 0,0,1 };
	double location[3] = { 0,0,0 };

	// global placement
	shared_ptr<IfcAxis2Placement3D> axis_placement_origin;
	convertPlacement(local_x, local_z, location, axis_placement_origin, vec_new_entities);
	shared_ptr<IfcLocalPlacement> global_placement(new IfcLocalPlacement());
	vec_new_entities.push_back(global_placement);
	global_placement->m_RelativePlacement = axis_placement_origin;

	// local placement of building
	location[0] = 2.7;
	shared_ptr<IfcAxis2Placement3D> axis_placement_building;
	convertPlacement(local_x, local_z, location, axis_placement_building, vec_new_entities);
	shared_ptr<IfcLocalPlacement> building_placement(new IfcLocalPlacement());
	vec_new_entities.push_back(building_placement);
	building_placement->m_PlacementRelTo = global_placement;
	building_placement->m_RelativePlacement = axis_placement_building;
	ifc_building->m_ObjectPlacement = building_placement;

	// local placement of wall
	location[0] = 0.1;
	location[1] = 3.0;
	shared_ptr<IfcAxis2Placement3D> axis_placement_local;
	convertPlacement(local_x, local_z, location, axis_placement_local, vec_new_entities);
	shared_ptr<IfcLocalPlacement> wall_placement(new IfcLocalPlacement());
	vec_new_entities.push_back(wall_placement);
	wall_placement->m_PlacementRelTo = building_placement;
	wall_placement->m_RelativePlacement = axis_placement_local;
	wall->m_ObjectPlacement = wall_placement;

	if( opening )
	{
		opening->m_ObjectPlacement = wall_placement;
	}

	// general objects
	shared_ptr<IfcPerson> person(new IfcPerson());
	person->m_Identification = shared_ptr<IfcIdentifier>(new IfcIdentifier(L"MyID"));
	person->m_FamilyName = shared_ptr<IfcLabel>(new IfcLabel(L"MyFamilyName"));
	person->m_GivenName = shared_ptr<IfcLabel>(new IfcLabel(L"MyGivenName"));
	vec_new_entities.push_back(person);

	shared_ptr<IfcOrganization> orga(new IfcOrganization());
	orga->m_Identification = shared_ptr<IfcIdentifier>(new IfcIdentifier(L"MyOrganization"));
	orga->m_Name = shared_ptr<IfcLabel>(new IfcLabel(L"My organizations name"));
	orga->m_Description = shared_ptr<IfcText>(new IfcText(L"My organizations description"));
	vec_new_entities.push_back(orga);

	shared_ptr<IfcPersonAndOrganization> person_and_organization(new IfcPersonAndOrganization());
	person_and_organization->m_ThePerson = person;
	person_and_organization->m_TheOrganization = orga;
	vec_new_entities.push_back(person_and_organization);

	shared_ptr<IfcOwnerHistory> owner_history(new IfcOwnerHistory());
	owner_history->m_OwningUser = person_and_organization;
	vec_new_entities.push_back(owner_history);

	// define units of the model
	shared_ptr<IfcSIUnit> unit1(new IfcSIUnit());
	unit1->m_UnitType = shared_ptr<IfcUnitEnum>(new IfcUnitEnum(IfcUnitEnum::ENUM_LENGTHUNIT));
	unit1->m_Name = shared_ptr<IfcSIUnitName>(new IfcSIUnitName(IfcSIUnitName::ENUM_METRE));
	vec_new_entities.push_back(unit1);

	shared_ptr<IfcUnitAssignment> unit_assignment(new IfcUnitAssignment());
	unit_assignment->m_Units.push_back(unit1);
	vec_new_entities.push_back(unit_assignment);

	// define representation context
	shared_ptr<IfcGeometricRepresentationContext> geom_context(new IfcGeometricRepresentationContext());
	geom_context->m_ContextType = shared_ptr<IfcLabel>(new IfcLabel(L"Model"));
	geom_context->m_CoordinateSpaceDimension = shared_ptr<IfcDimensionCount>(new IfcDimensionCount(3));
	geom_context->m_Precision = shared_ptr<IfcReal>(new IfcReal(1.000E-5));
	geom_context->m_WorldCoordinateSystem = axis_placement_origin;
	vec_new_entities.push_back(geom_context);

	ifc_project->m_UnitsInContext = unit_assignment;
	ifc_project->m_RepresentationContexts.push_back(geom_context);

	std::map<BuildingEntity*, shared_ptr<BuildingEntity> > map_new_entities;
	shared_ptr<BuildingEntity> projectAsEntity = ifc_project;

	// insert entities into IFC model

	// insert entities into IFC model
	for( auto entity : vec_new_entities )
	{
		shared_ptr<IfcRoot> ifc_root_object = dynamic_pointer_cast<IfcRoot>(entity);
		if( ifc_root_object )
		{
			// each object that is derived from IfcRoot should have a GUID
			if( !ifc_root_object->m_GlobalId )
			{
				ifc_root_object->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId(createBase64Uuid_wstr().data()));
			}

			ifc_root_object->m_OwnerHistory = owner_history;
		}

		shared_ptr<IfcRepresentation> ifc_representation = dynamic_pointer_cast<IfcRepresentation>(entity);
		if( ifc_representation )
		{
			ifc_representation->m_ContextOfItems = geom_context;
		}

		ifc_model->insertEntity(entity);
	}

	ifc_model->unsetInverseAttributes();
	ifc_model->resolveInverseAttributes();
}


int main()
{
	shared_ptr<BuildingModel> ifc_model;
	LoadWallExample(ifc_model, true, true, 2);

	// write IFC file in STEP format
	std::wstring file_path = L"SimpleWall.ifc";
	ifc_model->initFileHeader(file_path);
	std::stringstream stream;

	shared_ptr<WriterSTEP> step_writer(new WriterSTEP());
	step_writer->writeModelToStream(stream, ifc_model);
	ifc_model->clearIfcModel();

	std::ofstream ofs(file_path, std::ofstream::out);
	ofs << stream.str().c_str();
	ofs.close();

	return 0;
}

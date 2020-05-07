/* -*-c++-*- IfcQuery www.ifcquery.com
*
Copyright (c) 2017 Fabian Gerold

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <map>
#include <carve/aabb.hpp>
#include <ifcpp/geometry/Carve/IncludeCarveHeaders.h>
#include <ifcpp/model/GlobalDefines.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>

#include "SceneGraph/SoPtr.h"

class SoMaterial;
class SoSeparator;
class SoShape;
class SoTranslation;
class SoTransform;
class AppearanceData;
class BuildingObject;
class GeometrySettings;
class ProductShapeData;

class IFCQUERY_EXPORT ConverterCarve2Coin3D : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings>				m_geom_settings;
	std::map<std::string, SoPtr<SoSeparator> >			m_map_entity_id_to_node;
	std::map<std::string, SoPtr<SoSeparator> >			m_map_representation_id_to_node;
	double										m_recent_progress;
	SoPtr<SoMaterial>							m_glass_material;

public:
	ConverterCarve2Coin3D( shared_ptr<GeometrySettings>& geom_settings );
	virtual ~ConverterCarve2Coin3D();

	// Map: IfcProduct ID -> scenegraph switch
	const std::map<std::string, SoPtr<SoSeparator> >& getMapEntityIdToNode() { return m_map_entity_id_to_node; }
	// Map: Representation Identifier -> scenegraph node
	const std::map<std::string, SoPtr<SoSeparator> >& getMapRepresentationIdToNode() { return m_map_representation_id_to_node; }

	void clearInputCache();

	static void drawBoundingBox( const carve::geom::aabb<3>& aabb, SoSeparator* target_shape );

	static void drawFace( const carve::mesh::Face<3>* face, SoSeparator* geode, bool add_color_array = false );

	static void drawMeshSet( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, SoSeparator* geode, double crease_angle = M_PI*0.05, bool add_color_array = false );

	static void drawPolyline( const carve::input::PolylineSetData* polyline_data, SoSeparator* geode, bool add_color_array = false );

	void computeCreaseEdgesFromMeshset( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::vector<carve::mesh::Edge<3>* >& vec_edges_out, const double crease_angle );

	void renderMeshsetCreaseEdges( const shared_ptr<carve::mesh::MeshSet<3> >& meshset, SoSeparator* target_geode, const double crease_angle );

	void applyAppearancesToGroup( const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances, SoSeparator* grp );
	void convertAppearanceToCoin3D( const shared_ptr<AppearanceData>& product_appearance, SoSeparator* grp );

	SoTransform* convertMatrixToSoTransform( const carve::math::Matrix& mat_in );

	//\brief method convertProductShapeToOSG: creates geometry objects from an IfcProduct object
	// caution: when using OpenMP, this method runs in parallel threads, so every write access to member variables needs a write lock
	void convertProductShapeToCoin3D( shared_ptr<ProductShapeData>& product_shape, std::map<std::string, SoSeparator* >& map_representation_switches );
	

	/*\brief method convertToCoin3D: Creates geometry for OpenSceneGraph from given ProductShapeData.
	\param[out] parent_group Group to append the geometry.
	**/
	void convertToCoin3D( const std::map<std::string, shared_ptr<ProductShapeData> >& map_shape_data, SoSeparator* parent_group );
	void addNodes( const std::map<std::string, shared_ptr<BuildingObject> >& map_shape_data, SoSeparator* target_group );
	void resolveProjectStructure( const shared_ptr<ProductShapeData>& product_data, SoSeparator* group );
};

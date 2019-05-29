/* -*-c++-*- IfcQuery www.ifcquery.com
*
Copyright (c) 2017 Fabian Gerold

The above copyright notice notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <Inventor/SbColor.h>
#include <ifcpp/model/GlobalDefines.h>
#include <ifcpp/model/BasicTypes.h>
#include "SoPtr.h"
class SbVec3f;
class SoCoordinate3;
class SoSeparator;
class SoMaterial;

class IFCQUERY_EXPORT SceneGraphUtils
{
public:
	static SbVec3f computePolygonNormal( const SoCoordinate3 *coords );
	static void createCoordinateAxes( float length, SoSeparator* group );

	/*!
	\method createCoordinateGrid 
	\brief Create grid of lines on ground plane in x/y direction
	\param parent_group to add geometry to
	\param num_lines_per_direction Number of lines in positive/negative x/y direction
	\param distance Distance between grid lines
	*/
	static void createCoordinateGrid( SoSeparator* parent_group, size_t num_lines_per_direction = 10, double distance = 5.0 );

	static SoMaterial* findFirstMaterialNode( SoSeparator* select_node );

	struct SelectionContainer
	{
		SoPtr<SoMaterial> m_material_selection_set;
		SoPtr<SoMaterial> m_material_selection_changed;
		SbColor m_previous_color;
		SoPtr<SoSeparator> m_node;
	};

	static void unselectNodeContainer( shared_ptr<SelectionContainer>& container );

	static void setMaterialToNode(shared_ptr<SceneGraphUtils::SelectionContainer>& select_node, SoMaterial* mat);
	static void removeMaterialFromNode(SoSeparator* select_node, SoMaterial* mat);
};

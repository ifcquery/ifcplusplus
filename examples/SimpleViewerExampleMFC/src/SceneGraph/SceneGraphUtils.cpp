/* -*-c++-*- IfcQuery www.ifcquery.com
*
Copyright (c) 2017 Fabian Gerold

The above copyright notice notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "SceneGraphUtils.h"

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>

SbVec3f SceneGraphUtils::computePolygonNormal( const SoCoordinate3 *coords )
{
	const SbVec3f* vec_field = coords->point.getValues( 0 );
	const int num_points = coords->point.getNum();

	float x = 0;
	float y = 0;
	float z = 0;
	
	for( int k = 0; k < num_points; ++k )
	{
		const SbVec3f& vertex_current = vec_field[k];
		const SbVec3f& vertex_next = vec_field[ ( k + 1 ) % num_points ];
		x += ( vertex_current[1] - vertex_next[1] )*( vertex_current[2] + vertex_next[2] );
		y += ( vertex_current[2] - vertex_next[2] )*( vertex_current[0] + vertex_next[0] );
		z += ( vertex_current[0] - vertex_next[0] )*( vertex_current[1] + vertex_next[1] );
	}
	
	SbVec3f polygon_normal( x, y, z );
	polygon_normal.normalize();
	return polygon_normal;
}

void SceneGraphUtils::createCoordinateAxes( float length, SoSeparator* group )
{
	std::vector<std::string> coords_labels = { "x", "y", "z" };
	std::vector<std::vector<float> > coords = { { 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } };
	for( size_t ii = 0; ii < 3; ++ii )
	{
		float x = coords[ii][0];
		float y = coords[ii][1];
		float z = coords[ii][2];

		//SoMaterial *material = new SoMaterial();
		//material->diffuseColor.setValue( x, y, z );

		SoVertexProperty* vprop = new SoVertexProperty();
		vprop->vertex.set1Value( 0, 0, 0, 0 );  // first vertex
		vprop->vertex.set1Value( 1, x*length, y*length, z*length);  // second vertex

		SoLineSet* line = new SoLineSet();
		line->vertexProperty = vprop;
		vprop->orderedRGBA = SbColor( x, y, z ).getPackedValue();
		SoSeparator* line_sep = new SoSeparator();
		line_sep->setName( coords_labels[ii].c_str() );
		SoPolygonOffset *polygonOffset = new SoPolygonOffset();
		polygonOffset->factor = 1.;
		polygonOffset->units = 1.;

		SoDrawStyle* lineStyle = new SoDrawStyle();
		lineStyle->style = SoDrawStyle::LINES;
		lineStyle->lineWidth = 2.5f;

		line_sep->addChild( polygonOffset );
		line_sep->addChild( lineStyle );

		//line_sep->addChild( material );
		line_sep->addChild( line );
		group->addChild( line_sep );

		SoSeparator* text_node = new SoSeparator();
		text_node->setName( "text_node" );

		SoTranslation * trans = new SoTranslation;
		float length_label = length + 1.f;
		trans->translation = SbVec3f( x*length_label, y*length_label, z*length_label );
		text_node->addChild( trans );

		SoFont* font = new SoFont();
		font->name.setValue( "Helvetica" );
		font->size = 14;


		text_node->addChild( font );
		//instructsep->addChild( product_transform );

		SoMaterial *material = new SoMaterial();
		material->diffuseColor.setValue( x, y, z );
		text_node->addChild( material );

		SoText2 * text_x = new SoText2;
		const char * str[] = { coords_labels[ii].c_str() };
		text_x->string.setValues( 0, sizeof( str ) / sizeof( char * ), str );
		text_x->justification = SoText2::LEFT;
		text_node->addChild( font );

		text_node->addChild( text_x );
		group->addChild( text_node );

	}


}

void SceneGraphUtils::createCoordinateGrid( SoSeparator* parent_group, size_t num_lines_per_direction, double distance )
{
	SoMaterial *material = new SoMaterial();
	material->diffuseColor.setValue( 0.6f, 0.61f, 0.62f );
	parent_group->addChild( material );

	SoVertexProperty* vprop = new SoVertexProperty();
	SoIndexedLineSet* ils = new SoIndexedLineSet();

	float max_extent = num_lines_per_direction*float(distance);
	int line_idx = 0;
	for( int ii = 0; ii < int(num_lines_per_direction*2) + 1; ++ii )
	{
		float x = ii*float(distance) - max_extent;
		
		vprop->vertex.set1Value( ii*4,		x, -max_extent, 0 );
		vprop->vertex.set1Value( ii*4 + 1,	x, max_extent, 0 );

		vprop->vertex.set1Value( ii*4 + 2, -max_extent, x, 0 );
		vprop->vertex.set1Value( ii*4 + 3, max_extent, x, 0 );

		ils->coordIndex.set1Value( line_idx++, ii*4 );
		ils->coordIndex.set1Value( line_idx++, ii*4+1 );
		ils->coordIndex.set1Value( line_idx++, -1 );

		ils->coordIndex.set1Value( line_idx++, ii*4+2 );
		ils->coordIndex.set1Value( line_idx++, ii*4+3 );
		ils->coordIndex.set1Value( line_idx++, -1 );
		
	}

	ils->vertexProperty = vprop;
	parent_group->addChild( ils );
}

void SceneGraphUtils::removeMaterialFromNode( SoSeparator* select_node, SoMaterial* mat )
{
	int num_children = select_node->getNumChildren();
	for( int ii = 0; ii < num_children; ++ii )
	{
		SoNode* child = select_node->getChild( ii );
		SoMaterial* child_as_mat = dynamic_cast<SoMaterial*>(child);
		if( child_as_mat )
		{
			if( child_as_mat == mat )
			{
				select_node->removeChild( child_as_mat );
				return;
			}
		}
	}
}

SoMaterial* SceneGraphUtils::findFirstMaterialNode( SoSeparator* select_node )
{
	int num_children = select_node->getNumChildren();
	for( int ii = 0; ii < num_children; ++ii )
	{
		SoNode* child = select_node->getChild( ii );
		SoMaterial* child_as_mat = dynamic_cast<SoMaterial*>(child);
		if( child_as_mat )
		{
			return child_as_mat;
		}
	}
	return nullptr;
}

void SceneGraphUtils::setMaterialToNode( shared_ptr<SceneGraphUtils::SelectionContainer>& select_node, SoMaterial* mat )
{
	if( !mat )
	{
		return;
	}
	SoSeparator* select_node_separator = select_node->m_node.get();
	int num_children = select_node_separator->getNumChildren();
	SoMaterial* existing_mat = nullptr;
	for( int ii = 0; ii < num_children; ++ii )
	{
		SoNode* child = select_node_separator->getChild( ii );
		SoMaterial* child_as_mat = dynamic_cast<SoMaterial*>(child);
		if( child_as_mat )
		{
			existing_mat = child_as_mat;
		}
	}

	if( existing_mat )
	{
		if( existing_mat->diffuseColor.getNum() > 0 )
		{
			float r = existing_mat->diffuseColor[0][0];
			float g = existing_mat->diffuseColor[0][1];
			float b = existing_mat->diffuseColor[0][2];
			select_node->m_previous_color.setValue( r, g, b );
		}
		float r = mat->diffuseColor[0][0];
		float g = mat->diffuseColor[0][1];
		float b = mat->diffuseColor[0][2];
		existing_mat->ambientColor.setValue( r, g, b );

	}
	else
	{
		if( num_children > 0 )
		{
			select_node_separator->insertChild( mat, 1 );
		}
		else
		{
			select_node_separator->addChild( mat );
		}
	}
}

void SceneGraphUtils::unselectNodeContainer( shared_ptr<SelectionContainer>& container )
{
	SoSeparator* select_node = container->m_node.get();
	SoMaterial* changed_material = container->m_material_selection_changed.get();
	if( changed_material )
	{
		// restore previous color
		float r = 0;
		float g = 0;
		float b = 0;
		container->m_previous_color.getValue( r, g, b );
		changed_material->ambientColor.setValue( r, g, b );
	}
	SoMaterial* material_for_selection = container->m_material_selection_set.get();
	if( material_for_selection )
	{
		SceneGraphUtils::removeMaterialFromNode( select_node, material_for_selection );
	}
}

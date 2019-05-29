
#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <math.h>

#define SOWIN_INTERNAL
#define SOWIN_DEBUG false
#include <config.h>
#include <Inventor/SbTime.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMotion3Event.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>

#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <Inventor/projectors/SbCylinderSheetProjector.h>
#include <Inventor/projectors/SbCylinderPlaneProjector.h>

#include <sowindefs.h>
#include <Inventor/Win/common/gl.h>
#include <Inventor/Win/common/pixmaps/ortho.xpm>
#include <Inventor/Win/common/pixmaps/perspective.xpm>
#include <Inventor/Win/widgets/SoWinBitmapButton.h>
#include <Inventor/Win/SoWinBasic.h>
#include <Inventor/Win/SoWinCursor.h>
#include <Inventor/Win/viewers/SoGuiFullViewerP.h>
#include "SceneGraph/SceneGraphUtils.h"
#include "SoWinCADViewer.h"

SOWIN_OBJECT_SOURCE( SoWinCADViewer );

SoWinCADViewer::SoWinCADViewer( HWND parent, const char * name, SbBool embed, SoWinFullViewer::BuildFlag flag, SoWinViewer::Type type )
	: inherited( parent, name, embed, flag, type, FALSE )
{
	initializeViewer( TRUE );
}

SoWinCADViewer::SoWinCADViewer( HWND parent, const char * name, SbBool embed, SoWinFullViewer::BuildFlag flag, SoWinViewer::Type type, SbBool build )
	: inherited( parent, name, embed, flag, type, FALSE )
{
	initializeViewer( build );
}

void SoWinCADViewer::initializeViewer(SbBool build)
{
	genericConstructor();
	m_rotate_center.setValue( 0, 0, 0 );

	setClassName("SoWinCADViewer");
	setPopupMenuString("CAD Viewer");

	if( !build )
	{
		return;
	}

	HWND widget = buildWidget(getParentWidget());
	setBaseWidget(widget);

	setLeftWheelString("RotX");
	setBottomWheelString("RotY");

	setCursorEnabled(TRUE);
	setAnimationEnabled(TRUE);
	setSize(SbVec2s(500, 421));

	m_material_selection = new SoMaterial();
	m_material_selection->diffuseColor.setValue( 0.f, 1.f, 0.f );
}

void SoWinCADViewer::cameraButtonProc(SoWinBitmapButton * b, void * userdata)
{
	SoWinCADViewer * that = (SoWinCADViewer *)userdata;
	if (that->getCamera()) that->toggleCameraType();
}

SoWinCADViewer::~SoWinCADViewer()
{
	genericDestructor();
}

void SoWinCADViewer::createViewerButtons(HWND parent, SbPList * buttonlist)
{
	inherited::createViewerButtons(parent, buttonlist);

	SoWinBitmapButton * b = camerabutton = new SoWinBitmapButton(parent, 24, "perspective", NULL);
	b->addBitmap(perspective_xpm);
	b->addBitmap(ortho_xpm);
	b->setBitmap(0);
	b->registerClickedProc(SoWinCADViewer::cameraButtonProc, this);
	buttonlist->append(b);
}

void SoWinCADViewer::unselectAllNodes()
{
	for( auto it = m_vec_selected_nodes.begin(); it != m_vec_selected_nodes.end(); ++it )
	{
		shared_ptr<SceneGraphUtils::SelectionContainer>& selected_node_container = (*it);
		SceneGraphUtils::unselectNodeContainer( selected_node_container );
	}
	m_vec_selected_nodes.clear();
}

void SoWinCADViewer::setNodeSelected( SoSeparator* select_node )
{
	if( !select_node )
	{
		return;
	}

	if( !m_ctrldown )
	{
		unselectAllNodes();
	}

	SoMaterial* mat_node = SceneGraphUtils::findFirstMaterialNode( select_node );
	bool node_already_selected = false;
	shared_ptr<SceneGraphUtils::SelectionContainer> existing_selected_node_container;

	for( auto it = m_vec_selected_nodes.begin(); it != m_vec_selected_nodes.end(); ++it )
	{
		shared_ptr<SceneGraphUtils::SelectionContainer>& selected_node_container = (*it);

		if( select_node == selected_node_container->m_node.get() )
		{
			node_already_selected = true;
			existing_selected_node_container = selected_node_container;
			SceneGraphUtils::unselectNodeContainer( selected_node_container );
			m_vec_selected_nodes.erase( it );
			return;
		}
	}

	if( !existing_selected_node_container )
	{
		existing_selected_node_container = std::make_shared<SceneGraphUtils::SelectionContainer>();
		existing_selected_node_container->m_node = select_node;

		if( mat_node )
		{
			if( mat_node->diffuseColor.getNum() > 0 )
			{
				float r = mat_node->diffuseColor[0][0];
				float g = mat_node->diffuseColor[0][1];
				float b = mat_node->diffuseColor[0][2];
				existing_selected_node_container->m_previous_color.setValue( r, g, b );
				existing_selected_node_container->m_material_selection_changed = mat_node;
			}
		}
	}

	if( mat_node )
	{
		// set green color to material
		mat_node->diffuseColor.setValue( 0.1f, 0.96f, 0.1f );
		//existing_selected_node_container->m_previous_color_set = true;
	}
	else
	{
		// create new SoMaterial node
		int num_children = select_node->getNumChildren();
		if( num_children > 0 )
		{
			select_node->insertChild( m_material_selection.get(), 1 );
		}
		else
		{
			select_node->addChild( m_material_selection.get() );
		}
		existing_selected_node_container->m_material_selection_set = m_material_selection.get();
	}

	//addMaterialToNode( existing_selected_node_container->m_node, m_material_selection.get() );

	m_vec_selected_nodes.push_back( existing_selected_node_container );
}

void SoWinCADViewer::intersectModel( const SbVec2s& screenpos )
{
	SoCamera * cam = getCamera();
	if( cam == NULL ) return;
	if( m_rootnode == nullptr ) return;
	initViewerGeometry();

	const SbViewportRegion& viewport_region = getViewportRegion();
	SoRayPickAction rpaction( viewport_region );
	rpaction.setPoint( screenpos );
	rpaction.setRadius( 2 );

	// Modification in SoWinViewer.cpp:  SoNode * SoWinViewer::getSceneRoot(void) { return PRIVATE( this )->sceneroot; }
	SoNode* scene_root = getSceneGraph();
	rpaction.apply( scene_root );

	SoPickedPoint * picked = rpaction.getPickedPoint();
	if( !picked )
	{
#if 1
		interactiveCountInc(); // decremented in setSeekMode(FALSE)
#endif // FIXME
		setSeekMode( FALSE );

		// send unselect all message
		//sendUnselectAllMessage();
		//unselectAllNodes();
		return;
	}

	SbVec3f hitpoint = picked->getPoint();

	if( false )
	{
		SoGetBoundingBoxAction bbaction( getViewportRegion() );
		bbaction.apply( picked->getPath() );
		SbBox3f bbox = bbaction.getBoundingBox();
		hitpoint = bbox.getCenter();
	}

	std::stringstream message_txt;
	message_txt << "clicked point " << hitpoint[0] << ", " << hitpoint[1] << ", " << hitpoint[2] << "\n";
#ifdef _DEBUG
	m_viewer_message_callback->messageCallback( message_txt.str().c_str(), StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, __FUNCTION__ );
#endif

	SoPath* picked_path = picked->getPath();
	if( !picked_path )
	{
		unselectAllNodes();
		return;
	}

	int path_length = picked_path->getLength();
	if( path_length == 0 )
	{
		unselectAllNodes();
		return;
	}

	for( int ii = 0; ii < path_length; ++ii )
	{
		SoNode* node = picked_path->getNodeFromTail( ii );
		if( !node )
		{
			continue;
		}

		std::string node_name = node->getName();
		if( node_name.length() == 0 )
		{
			continue;
		}

		if( node_name.compare( "root_node" ) == 0 )
		{
			continue;
		}

		if( m_intersect_sphere_transform )
		{
			m_intersect_sphere_transform->translation.setValue( (float)(hitpoint[0]), (float)(hitpoint[1]), (float)(hitpoint[2]) );
			m_intersect_sphere_switch->whichChild = SO_SWITCH_ALL;
		}

		if( !m_ctrldown )
		{
			//unselectAllNodes();
			//sendUnselectAllMessage();
		}

		SoSeparator* node_as_separator = dynamic_cast<SoSeparator*>(node);
		if( !node_as_separator )
		{
			continue;
		}

		m_btn_down_selected_node = node_as_separator;

		if( m_viewer_message_callback )
		{
			std::stringstream message_txt;
			message_txt << "clicked node " << node_name;
			if( node_name.size() == 0 )
			{
				message_txt << node->getNodeId();
			}
#ifdef _DEBUG
			m_viewer_message_callback->messageCallback( message_txt.str().c_str(), StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, __FUNCTION__ );
#endif
			break;
		}
	}
}

void SoWinCADViewer::sendUnselectAllMessage()
{
	if( m_viewer_message_callback )
	{
		m_viewer_message_callback->messageCallback("select:#-1", StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, __FUNCTION__);
	}
}

void SoWinCADViewer::sendSelectedMessage( SoNode* node )
{
	std::string node_name = node->getName();
	if( node_name.length() == 0 )
	{
		return;
	}

	if( node_name.compare( "root_node" ) == 0 )
	{
		return;
	}


	// extract entity id
	size_t begin_id_index = node_name.find_first_of( "#" );
	if( node_name.length() > begin_id_index + 1 )
	{
		std::string node_name_id = node_name.substr( begin_id_index + 1 );
		size_t last_index = node_name_id.find_first_not_of( "0123456789" );
		std::string id_str = node_name_id.substr( 0, last_index );
		const int id = atoi( id_str.c_str() );

		std::wstringstream wstr;
		wstr << L"select:#" << id;
		if( m_viewer_message_callback )
		{
			m_viewer_message_callback->messageCallback( wstr.str().c_str(), StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, __FUNCTION__ );
		}
	}
}

SbBool SoWinCADViewer::processSoEvent(const SoEvent * const ev)
{
	if (!isViewing()) { return inherited::processSoEvent(ev); }
	if (isSeekMode()) { return inherited::processSoEvent(ev); }

	const SoType type(ev->getTypeId());
	const SbVec2s size(getGLSize());
	const SbVec2f last_mouse_pos_normalized = m_last_mouse_pos_normalized;
	const SbVec2s pos(ev->getPosition());
	const SbVec2f posn((float)pos[0] / (float)SoWinMax((int)(size[0] - 1), 1), (float)pos[1] / (float)SoWinMax((int)(size[1] - 1), 1));

	m_last_mouse_pos_normalized = posn;

	SbBool processed = FALSE;
	const SoWinCADViewer::ViewerMode currentmode = m_current_mode;
	SoWinCADViewer::ViewerMode newmode = currentmode;

	m_ctrldown = ev->wasCtrlDown();
	m_shiftdown = ev->wasShiftDown();

	// mouse button
	if (type.isDerivedFrom(SoMouseButtonEvent::getClassTypeId()))
	{
		processed = TRUE;

		const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *)ev;
		const int button = event->getButton();
		const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

		switch (button)
		{
		case SoMouseButtonEvent::BUTTON1:
			m_button1down = press;
			if( press && (currentmode == SoWinCADViewer::SEEK_WAIT_MODE) )
			{
				newmode = SoWinCADViewer::SEEK_MODE;
				seekToPoint(pos);
			}

			if( press )
			{
				m_drag_after_btn_down = false;
				m_btn_down_selected_node.reset();
				intersectModel( pos );
				m_intersect_sphere_switch->whichChild = SO_SWITCH_ALL;
			}
			else
			{
				// relase event
				if( !m_drag_after_btn_down )
				{
					if( m_btn_down_selected_node.valid() )
					{
						m_select_called = true;
						sendSelectedMessage( m_btn_down_selected_node.get() );
						setNodeSelected( m_btn_down_selected_node.get() );
						m_select_called = false;
					}
					else
					{
						unselectAllNodes();
						sendUnselectAllMessage();
					}
				}

				//if( currentmode == SoWinCADViewer::DRAGGING )
				{
					m_intersect_sphere_switch->whichChild = SO_SWITCH_NONE;
				}
			}
			break;
		case SoMouseButtonEvent::BUTTON2:
			processed = FALSE; // pass on to superclass, so popup menu is shown
			break;
		case SoMouseButtonEvent::BUTTON3:
			m_button3down = press;
			break;
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
		case SoMouseButtonEvent::BUTTON4:
			if (press) SoGuiFullViewerP::zoom(getCamera(), 0.1f);
			break;
		case SoMouseButtonEvent::BUTTON5:
			if (press) SoGuiFullViewerP::zoom(getCamera(), -0.1f);
			break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5
		default:
			break;
		}
	}

	// Keyboard
	if (type.isDerivedFrom(SoKeyboardEvent::getClassTypeId()))
	{
		const SoKeyboardEvent * const event = (const SoKeyboardEvent *)ev;
		const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;
		switch (event->getKey()) {
		case SoKeyboardEvent::LEFT_CONTROL:
		case SoKeyboardEvent::RIGHT_CONTROL:
			processed = TRUE;
			m_ctrldown = press;
			break;
		case SoKeyboardEvent::LEFT_SHIFT:
		case SoKeyboardEvent::RIGHT_SHIFT:
			processed = TRUE;
			m_shiftdown = press;
			break;
		default:
			break;
		}
	}
	
	initViewerGeometry();

	// mouse movement
	if (type.isDerivedFrom(SoLocation2Event::getClassTypeId()))
	{
		const SoLocation2Event * const event = (const SoLocation2Event *)ev;

		processed = TRUE;

		if (m_current_mode == SoWinCADViewer::ZOOMING)
		{
			zoomByCursor(posn, last_mouse_pos_normalized );
		}
		else if (m_current_mode == SoWinCADViewer::PANNING)
		{
			SoGuiFullViewerP::pan(getCamera(), getGLAspectRatio(), m_panningplane, posn, last_mouse_pos_normalized );
		}
		else if (m_current_mode == SoWinCADViewer::DRAGGING)
		{
			addToLog(event->getPosition(), event->getTime());
			rotateCamera( posn );
		}
		else
		{
			processed = FALSE;
		}

		if( m_button1down )
		{
			newmode = SoWinCADViewer::DRAGGING;
			m_drag_after_btn_down = true;
		}
	}

	enum
	{
		BUTTON1DOWN = 1 << 0,
		BUTTON3DOWN = 1 << 1,
		CTRLDOWN = 1 << 2,
		SHIFTDOWN = 1 << 3
	};
	
	unsigned int combo = (m_button1down ? BUTTON1DOWN : 0) | (m_button3down ? BUTTON3DOWN : 0) | (m_ctrldown ? CTRLDOWN : 0) | (m_shiftdown ? SHIFTDOWN : 0);

	switch (combo)
	{
	case 0:
		if (currentmode == SoWinCADViewer::ANIMATING )
		{
			break;
		}
		newmode = SoWinCADViewer::IDLE;
		break;
	case BUTTON1DOWN:
		//newmode = SoWinCADViewer::DRAGGING;
		break;
	case BUTTON3DOWN:
	case CTRLDOWN | BUTTON1DOWN:
	case SHIFTDOWN | BUTTON1DOWN:
		newmode = SoWinCADViewer::PANNING;
		break;
	case BUTTON1DOWN | BUTTON3DOWN:
	case CTRLDOWN | BUTTON3DOWN:
	case CTRLDOWN | SHIFTDOWN | BUTTON1DOWN:
		newmode = SoWinCADViewer::ZOOMING;
		break;

	default:
		if ((currentmode != SoWinCADViewer::SEEK_WAIT_MODE) && (currentmode != SoWinCADViewer::SEEK_MODE))
		{
			newmode = SoWinCADViewer::IDLE;
		}
		break;
	}

	if (newmode != currentmode)
	{
		setMode(newmode);
	}

	if( processed )
	{
		// If handled here, no further event propagation
		return true;
	}
	return inherited::processSoEvent(ev);
}

void SoWinCADViewer::setSeekMode(SbBool on)
{
	if (isAnimating()) { stopAnimating(); }
	inherited::setSeekMode(on);
	setMode(on ? SoWinCADViewer::SEEK_WAIT_MODE : (isViewing() ? SoWinCADViewer::IDLE : SoWinCADViewer::INTERACT));
}

void SoWinCADViewer::setCursorEnabled(SbBool enable)
{
	inherited::setCursorEnabled(enable);
	setCursorRepresentation(m_current_mode);
}

void SoWinCADViewer::actualRedraw(void)
{
	SbTime now = SbTime::getTimeOfDay();
	double secs = now.getValue() - m_prevRedrawTime.getValue();

	m_prevRedrawTime = now;

	if (isAnimating())
	{
		// TODO: handle animation path
		
	}

	inherited::actualRedraw();
	if (isAnimating()) { scheduleRedraw(); }
}

void SoWinCADViewer::afterRealizeHook(void)
{
	inherited::afterRealizeHook();
	setCursorRepresentation(m_current_mode);
}

// make sure the mouse pointer cursor is updated.
void SoWinCADViewer::setViewing(SbBool enable)
{
	if (!!isViewing() == !!enable)
	{
#if _DEBUG
		SoDebugError::postWarning("SoWinCADViewer::setViewing", "current state already %s", enable ? "TRUE" : "FALSE");
#endif
		return;
	}

	setMode(enable ? SoWinCADViewer::IDLE : SoWinCADViewer::INTERACT);
	inherited::setViewing(enable);
}


void SoWinCADViewer::genericConstructor(void)
{
	m_current_mode = SoWinCADViewer::IDLE;

	m_prevRedrawTime = SbTime::getTimeOfDay();
	m_log_size = 10;
	m_log_position.resize(10);
	m_log_time.resize( 10 );
	m_log_historysize = 0;
	m_button1down = FALSE;
	m_button3down = FALSE;
	m_ctrldown = FALSE;
	m_shiftdown = FALSE;
}

void SoWinCADViewer::genericDestructor(void)
{
}

// rotate a camera around its focalpoint, in the direction around the
// given axis, by the given delta value (in radians)
void SoWinCADViewer::rotateCamera(SoCamera * cam, const SbVec3f & aroundaxis, const float delta)
{
	const SbVec3f DEFAULTDIRECTION(0, 0, -1);
	const SbRotation currentorientation = cam->orientation.getValue();

	SbVec3f currentdir;
	currentorientation.multVec(DEFAULTDIRECTION, currentdir);

	const SbVec3f focalpoint = cam->position.getValue() + cam->focalDistance.getValue() * currentdir;

	// set new orientation
	cam->orientation = SbRotation(aroundaxis, delta) * currentorientation;

	SbVec3f newdir;
	cam->orientation.getValue().multVec(DEFAULTDIRECTION, newdir);
	cam->position = focalpoint - cam->focalDistance.getValue() * newdir;
}

// The viewer is a state machine, and all changes to the current state are made through this call.
void SoWinCADViewer::setMode(const ViewerMode newmode)
{
	const ViewerMode oldmode = m_current_mode;
	if (newmode == oldmode) { return; }

	switch (newmode)
	{
	case DRAGGING:
		// Set up initial projection point for the projector object when
		// first starting a drag operation.
		interactiveCountInc();
		clearLog();
		break;

	case ANIMATING:
		interactiveCountInc();
		scheduleRedraw();
		break;

	case PANNING:
	{
		// The plane we're projecting the mouse coordinates to get 3D
		// coordinates should stay the same during the whole pan
		// operation, so we should calculate this value here.
		SoCamera * cam = getCamera();
		if (cam == NULL)
		{
			// can happen for empty scenegraph
			m_panningplane = SbPlane(SbVec3f(0, 0, 1), 0);
		}
		else
		{
			SbViewVolume vv = cam->getViewVolume(getGLAspectRatio());
			m_panningplane = vv.getPlane(cam->focalDistance.getValue());
		}
	}
	interactiveCountInc();
	break;

	case ZOOMING:
		interactiveCountInc();
		break;

	default: // include default to avoid compiler warnings.
		break;
	}

	switch (oldmode)
	{
	case ANIMATING:
	case DRAGGING:
	case PANNING:
	case ZOOMING:
		interactiveCountDec();
		break;

	default:
		break;
	}

#if _DEBUG && 0
	if (oldmode == ZOOMING)
	{
		SbVec3f v = getCamera()->position.getValue();
		SoDebugError::postInfo("SoWinCADViewerP::setMode", "new camera position after zoom: <%e, %e, %e>", v[0], v[1], v[2]);
	}
#endif

	setCursorRepresentation(newmode);
	m_current_mode = newmode;
}

void drawDebugLine( SoSeparator* node, SbVec3f pt0, SbVec3f pt1, SbVec3f color )
{
	if( node == nullptr )
	{
		return;
	}

#ifdef _DEBUG
	SoVertexProperty* vprop = new SoVertexProperty();
	vprop->vertex.set1Value( 0, pt0[0], pt0[1], pt0[2] );
	vprop->vertex.set1Value( 1, pt1[0], pt1[1], pt1[2]  );

	SoLineSet* line = new SoLineSet();
	line->vertexProperty = vprop;
	vprop->orderedRGBA = SbColor( color[0], color[1], color[2] ).getPackedValue();
	
	SoPolygonOffset *polygonOffset = new SoPolygonOffset();
	polygonOffset->factor = 1.;
	polygonOffset->units = 1.;

	SoDrawStyle* lineStyle = new SoDrawStyle();
	lineStyle->style = SoDrawStyle::LINES;
	lineStyle->lineWidth = 2.5f;
	
	SoSeparator* line_sep = new SoSeparator();
	line_sep->addChild( polygonOffset );
	line_sep->addChild( lineStyle );
	line_sep->addChild( line );
	node->addChild( line_sep );
#endif
}

void drawDebugText( SoSeparator* parent_node, const char* text, SbVec3f pos, SbVec3f color )
{
	SoSeparator* text_node = new SoSeparator();
	SoTranslation * trans = new SoTranslation();
	trans->translation = pos;
	text_node->addChild( trans );

	SoFont* font = new SoFont();
	font->name.setValue( "Helvetica" );
	font->size = 14;

	text_node->addChild( font );

	SoMaterial *material = new SoMaterial();
	material->diffuseColor = color;
	text_node->addChild( material );

	SoText2 * text_x = new SoText2;
	const char * str[] = { text };
	text_x->string.setValues( 0, sizeof( str ) / sizeof( char * ), str );
	text_x->justification = SoText2::LEFT;
	text_node->addChild( font );

	text_node->addChild( text_x );
	parent_node->addChild( text_node );
}

void SoWinCADViewer::initViewerGeometry()
{
	if( m_rootnode )
	{
		if( m_line1_sep == nullptr )
		{
			m_line1_sep = new SoSeparator();
			m_line2_sep = new SoSeparator();
			m_line3_sep = new SoSeparator();
			m_line4_sep = new SoSeparator();
			m_rootnode->addChild( m_line1_sep );
			m_rootnode->addChild( m_line2_sep );
			m_rootnode->addChild( m_line3_sep );
			m_rootnode->addChild( m_line4_sep );
		}
		else
		{
			m_line1_sep->removeAllChildren();
			m_line2_sep->removeAllChildren();
			m_line3_sep->removeAllChildren();
			m_line4_sep->removeAllChildren();
		}

		if( m_intersect_sphere_switch == nullptr )
		{
			m_intersect_sphere_switch = new SoSwitch();
			m_intersect_sphere_transform = new SoTransform();
			m_intersect_sphere_switch->addChild( m_intersect_sphere_transform );
			m_intersect_sphere_transform->translation.setValue( 0.f, 0.f, 0.f );

			SoMaterial* mat = new SoMaterial();
			mat->ambientColor.setValue( 0.9f, 0.75f, 0.2f );
			mat->diffuseColor.setValue( 0.9f, 0.75f, 0.2f );
			mat->shininess = 0.5f;
			SoSphere* sphere = new SoSphere;
			sphere->radius = 0.2f;
			m_intersect_sphere_switch->addChild( mat );
			m_intersect_sphere_switch->addChild( sphere );
			m_rootnode->addChild( m_intersect_sphere_switch );
			m_intersect_sphere_switch->whichChild = SO_SWITCH_NONE;
		}
	}
}

void SoWinCADViewer::rotateCamera(const SbVec2f& mousepos )
{
	// screen_pos coordinates:         vectors:
	// (0/1)          (1/1)            --------> (1/0)
	//                                |
	//      (0.5/0.5)                 |
	//                                |
	// (0/0)          (1/0)           v (0/1)

	if( m_log_historysize < 2 ) return;

	SbVec2s widget_size( getGLSize() );
	SbVec2f prevpos;
	prevpos[0] = float( m_log_position[1][0] ) / float( SoWinMax( (int)(widget_size[0] - 1), 1 ) );
	prevpos[1] = float( m_log_position[1][1] ) / float( SoWinMax( (int)(widget_size[1] - 1), 1 ) );

	initViewerGeometry();
	
	SoCamera * cam = getCamera();
	if (cam == NULL) return;

	float dx = mousepos[0] - prevpos[0];
	float dy = mousepos[1] - prevpos[1];

	const SbVec3f eye = cam->position.getValue();
	const SbRotation camrot = cam->orientation.getValue();
	SbVec3f direction;
	camrot.multVec( SbVec3f( 0, 0, -1 ), direction );
	SbVec3f focalpoint_model = cam->position.getValue() + cam->focalDistance.getValue() * direction;
	SbVec3f lookat = focalpoint_model;

	m_rotate_center = lookat;

	SbVec3f cam_upvec_model( 0, 1, 0 );
	camrot.multVec( cam_upvec_model, cam_upvec_model );
	SbVec3f rotate_center_up = m_rotate_center + cam_upvec_model*10.0;
	drawDebugLine( m_line2_sep, m_rotate_center, rotate_center_up, SbVec3f( 0, 1, 1 ) );
	drawDebugText( m_line2_sep, "Yaw axis", rotate_center_up, SbVec3f( 0, 1, 1 ) );

	SbVec3f cam_vec_right_model( 1, 0, 0 );
	camrot.multVec( cam_vec_right_model, cam_vec_right_model );
	SbVec3f rotate_center_right = m_rotate_center + cam_vec_right_model*10.0;
	drawDebugLine( m_line1_sep, m_rotate_center, rotate_center_right, SbVec3f( 1, 1, 0 ) );
	drawDebugText( m_line1_sep, "Pitch axis", rotate_center_right, SbVec3f( 1, 1, 0 ) );

	SbVec3f cam_lookat( 0, 0, -1 );
	camrot.multVec( cam_lookat, cam_lookat );

	bool m_center_camera_rotation_to_intersection_point = true;
	if (m_center_camera_rotation_to_intersection_point)
	{
		if (dx != 0.0 || dy != 0.0)
		{
			// pitch axis should be in the screen plane to rotate dy around
			SbVec3f pitch_axis_lateral = rotate_center_right - m_rotate_center;
			pitch_axis_lateral.normalize();

			SbVec3f up_axis = rotate_center_up - m_rotate_center;
			up_axis.normalize();

			SbVec3f yaw_axis_vertical = up_axis;// screen_top_in_model - screen_center_in_model;
			yaw_axis_vertical.normalize();

			// lookat is the center of the screen. rotation center can be different (intersection point)
			SbVec3f center_eye = eye - m_rotate_center;
			SbVec3f center_lookat = lookat - m_rotate_center;
			SbVec3f eye_lookat = lookat - eye;

			double dot_product_yaw_up = eye_lookat.dot( yaw_axis_vertical );
			if( dot_product_yaw_up < -0.995 )
			{
				SbVec3f new_lookat = lookat;
				SbVec3f view_axis_direction( 0, 1, 0 );

				//SbVec3f direction_screen_right;// = SbVec3f( 1, 0, 0 )*screen_model;
				//matrix_screen_model.multMatrixVec( SbVec3f( 1, 0, 0 ), direction_screen_right );
				//SbVec3f direction_screen_up;// = SbVec3f( 0, 1, 0 )*screen_model;
				//matrix_screen_model.multMatrixVec( SbVec3f( 0, 1, 0 ), direction_screen_up );
				//yaw_axis_vertical = direction_screen_up;
				//pitch_axis_lateral = direction_screen_right;

				//pitch_axis_lateral = SbVec3f(0, 1, 0);
				//yaw_axis_vertical = direction_screen_up.cross( pitch_axis_lateral );
				//yaw_axis_vertical.normalize();

				//new_eye = new_lookat - view_axis_direction;
				//m_viewer_context->getCoordinateSystem()->getUpDirectionForAxis( view_axis, new_up );

				//m_viewer_context->setEyeLookatUp( eye_new, lookat_new, new_up );
			}
			else if (dot_product_yaw_up > 0.99)
			{

			}

			SbRotation yaw_rotation( yaw_axis_vertical, -dx*13.0f );

			// pitch (horizontal, in screen)
			SbVec3f pitch_axis = yaw_axis_vertical.cross( center_eye );
			pitch_axis.normalize();

			SbRotation quatpitch( pitch_axis_lateral, dy*2.2f );

			SbVec3f center_eye_new;
			quatpitch.multVec( center_eye, center_eye_new );

			SbVec3f center_lookat_new;
			quatpitch.multVec( center_lookat, center_lookat_new );

			yaw_rotation.multVec( center_eye_new, center_eye_new );
			yaw_rotation.multVec( center_lookat_new, center_lookat_new );

			SbVec3f eye_new = m_rotate_center + center_eye_new;
			SbVec3f roate_center_new = m_rotate_center + center_lookat_new;

			SbVec3f up_vec( 0, 0, 1 );
			cam->position = eye_new;
			cam->pointAt( m_rotate_center, up_vec );
		}
	}
}

void SoWinCADViewer::zoomByCursor(const SbVec2f & thispos, const SbVec2f & prevpos)
{
	SoGuiFullViewerP::zoom( getCamera(), (thispos[1] - prevpos[1])*20.0f );
}

void SoWinCADViewer::clearLog(void)
{
	m_log_historysize = 0;
}

void SoWinCADViewer::addToLog(const SbVec2s pos, const SbTime time)
{
	if ( m_log_historysize > 0 && pos == m_log_position[0])
	{
#if _DEBUG && 0
		SoDebugError::postInfo("SoWinCADViewer::addToLog", "same position already added to log");
#endif
		return;
	}

	int lastidx = m_log_historysize;
	// If we've filled up the log, we should throw away the last item:
	if (lastidx == m_log_size) { lastidx--; }

	assert(lastidx < m_log_size);
	for (int i = lastidx; i > 0; i--)
	{
		m_log_position[i] = m_log_position[i - 1];
		m_log_time[i] = m_log_time[i - 1];
	}

	m_log_position[0] = pos;
	m_log_time[0] = time;
	if( m_log_historysize < m_log_size )
	{
		m_log_historysize += 1;
	}
}

// Set cursor graphics according to mode.
void SoWinCADViewer::setCursorRepresentation(int modearg)
{
	if (!isCursorEnabled())
	{
		setComponentCursor(SoWinCursor::getBlankCursor());
		return;
	}

	switch (modearg)
	{
	case SoWinCADViewer::INTERACT:
		setComponentCursor(SoWinCursor(SoWinCursor::DEFAULT));
		break;

	case SoWinCADViewer::IDLE:
		setComponentCursor( SoWinCursor( SoWinCursor::DEFAULT ) );
		break;

	case SoWinCADViewer::DRAGGING:
		setComponentCursor( SoWinCursor::getRotateCursor() );
		break;

	case SoWinCADViewer::ANIMATING:
		setComponentCursor( SoWinCursor( SoWinCursor::DEFAULT ) );
		break;

	case SoWinCADViewer::ZOOMING:
		setComponentCursor(SoWinCursor::getZoomCursor());
		break;

	case SoWinCADViewer::SEEK_MODE:
	case SoWinCADViewer::SEEK_WAIT_MODE:
		setComponentCursor(SoWinCursor(SoWinCursor::CROSSHAIR));
		break;

	case SoWinCADViewer::PANNING:
		setComponentCursor(SoWinCursor::getPanCursor());
		break;

	default: assert(0); break;
	}
}

// animation stuff. TODO: implement camera path
void SoWinCADViewer::setAnimationEnabled( const SbBool enable )
{
	if( !enable && isAnimating() ) { stopAnimating(); }
}

SbBool SoWinCADViewer::isAnimationEnabled( void ) const
{
	return false;
}

void SoWinCADViewer::stopAnimating( void )
{
	if( m_current_mode != SoWinCADViewer::ANIMATING )
	{
#if _DEBUG
		SoDebugError::postWarning( "SoWinCADViewer::stopAnimating", "not animating" );
#endif
		return;
	}
	setMode( isViewing() ? SoWinCADViewer::IDLE : SoWinCADViewer::INTERACT );
}

SbBool SoWinCADViewer::isAnimating( void ) const
{
	return m_current_mode == SoWinCADViewer::ANIMATING;
}


#pragma once
#include <ifcpp/model/StatusCallback.h>
#include <SceneGraph/SoPtr.h>
#include <Inventor/SbLinear.h>
#include <Inventor/Win/viewers/SoWinFullViewer.h>
#include "SceneGraph/SceneGraphUtils.h"

class SoMaterial;
class SoSelection;
class SoSeparator;
class SoSwitch;
class SoTransform;
class SoTranslation;
class SoScale;

class SoWinCADViewer : public SoWinFullViewer {
	SOWIN_OBJECT_HEADER(SoWinCADViewer, SoWinFullViewer);

public:
	SoWinCADViewer(HWND parent = NULL, const char * name = NULL, SbBool embed = TRUE, SoWinFullViewer::BuildFlag flag = BUILD_ALL, SoWinViewer::Type type = EDITOR);
	~SoWinCADViewer();
	void initializeViewer(SbBool build);
	void initViewerGeometry();

	void setAnimationEnabled(const SbBool enable);
	SbBool isAnimationEnabled(void) const;

	void stopAnimating(void);
	SbBool isAnimating(void) const;

	virtual void setViewing(SbBool enable);
	virtual void setCursorEnabled(SbBool enable);

	static void cameraButtonProc(class SoWinBitmapButton* b, void* userdata);
	
	void genericConstructor(void);
	void genericDestructor(void);

	static void rotateCamera(SoCamera* cam, const SbVec3f& aroundaxis, const float delta);
	void intersectModel( const SbVec2s& screenpos );
	void rotateCamera(const SbVec2f& mousepos );
	void zoomByCursor(const SbVec2f& mousepos, const SbVec2f& prevpos);
	void clearLog( void );
	void addToLog( const SbVec2s pos, const SbTime time );

	short m_log_size = 16;
	short m_log_historysize;
	std::vector<SbVec2s> m_log_position;
	std::vector<SbTime> m_log_time;

	SbBool m_select_called = false;
	SbBool m_button1down = false;
	SbBool m_button3down = false;
	SbBool m_drag_after_btn_down = false;
	SbBool m_ctrldown = false;
	SbBool m_shiftdown = false;
	SbVec2f m_last_mouse_pos_normalized;
	SbPlane m_panningplane;
	SbVec3f m_rotate_center;
	SbTime m_prevRedrawTime;
	SoSeparator* m_rootnode = nullptr;
	SoSelection* m_model_node = nullptr;
	SoSeparator* m_line1_sep = nullptr;
	SoSeparator* m_line2_sep = nullptr;
	SoSeparator* m_line3_sep = nullptr;
	SoSeparator* m_line4_sep = nullptr;
	SoTransform* m_intersect_sphere_transform = nullptr;
	SoSwitch* m_intersect_sphere_switch = nullptr;
	
	SoPtr<SoMaterial> m_material_selection;
	SoPtr<SoSeparator> m_btn_down_selected_node;
	class SoWinBitmapButton * camerabutton = NULL;
	shared_ptr<StatusCallback> m_viewer_message_callback;
	
	std::vector<shared_ptr<SceneGraphUtils::SelectionContainer> > m_vec_selected_nodes;
	void sendSelectedMessage( SoNode* sep );
	void setNodeSelected( SoSeparator* select_node );
	void sendUnselectAllMessage();
	void unselectAllNodes();

	enum ViewerMode {
		IDLE,
		INTERACT,
		ZOOMING,
		PANNING,
		DRAGGING,
		ANIMATING,
		SEEK_WAIT_MODE,
		SEEK_MODE
	} mode;

	ViewerMode m_current_mode;
	void setMode(const ViewerMode mode);
	void setCursorRepresentation(int mode);

protected:
	SoWinCADViewer(HWND parent, const char * name, SbBool embed, SoWinFullViewer::BuildFlag flag, SoWinViewer::Type type, SbBool build);

	virtual void createViewerButtons(HWND parent, SbPList * buttonlist);

	virtual SbBool processSoEvent(const SoEvent * const event);
	virtual void setSeekMode(SbBool enable);
	virtual void actualRedraw(void);

	virtual void afterRealizeHook(void);
};

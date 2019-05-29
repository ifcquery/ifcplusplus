#ifndef IVF_COMPONENT_H
#define IVF_COMPONENT_H

#include <afxwin.h>

class SbColor;
class SbViewportRegion;
class SoError;
class SoNode;
class SoGroup;
class SoCamera;
class SoEventCallback;
class SoGLRenderAction;
class SoSceneManager;
class SoSelection;
class SoPath;
class SoPathList;
class SoPickedPoint;
class SoOffscreenRenderer;

class SoWinClipboard;
class SoWinComponent;

#include <Ivf/IvfBasic.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfComponent {
  friend class CIvfMainFrame;
  friend class CIvfDocument;

public:
  CIvfComponent(void);

  virtual SoSelection * IvfGetSelectionNode(void);

  virtual void IvfCreateComponent(CWnd * window, void * createargs = NULL) = 0;
  virtual void IvfDestroyComponent(void) = 0;

  virtual SoWinClipboard * IvfGetClipboard(void);
  virtual SoWinComponent * IvfGetSoWinComponent(void);

  void IvfResetComponent(CIvfDocument * ivfdocument);
  virtual BOOL IvfIsSelected(void);

  void IvfViewmodesSelectionmode(void);
  void IvfViewmodesViewingmode(void);

  // IVF_COMPONENT_BASE:
  virtual void IvfSetSceneGraph(SoNode * root) = 0;
  virtual SoNode * IvfGetSceneGraph(void) = 0;
  virtual SoNode * IvfGetBaseViewerSceneGraph(void) = 0;
  virtual void IvfReleaseSceneGraph(void) = 0;
  virtual void IvfSetBackgroundColor(SbColor & background) = 0;
  virtual void IvfSetHeadlight(BOOL enable) = 0;
  virtual BOOL IvfIsHeadlight(void) = 0;
  virtual void IvfSetActiveCamera(int camera);
  virtual void IvfSetActiveCamera(SoCamera * camera) = 0;
  virtual SoCamera * IvfGetActiveCamera(void) = 0;
  virtual void IvfSaveHomePosition(void) = 0;
  virtual void IvfResetToHomePosition(void) = 0;
  virtual void IvfViewAll(void) = 0;
  virtual const SbViewportRegion & IvfGetViewportRegion(void) = 0;
  virtual SoGLRenderAction * IvfGetGLRenderAction(void) = 0;
  virtual SoSceneManager * IvfGetSceneManager(void) = 0;
  virtual BOOL IvfIsViewing(void) = 0;
  virtual void IvfSetViewing(BOOL enable) = 0;

  enum IvfViewerType {
    IVF_NONE,
    IVF_WALK,
    IVF_EXAMINER
  };

  static void setIgnoreError(WORD errormask);
  static void clearIgnoreError(WORD errormask);
  static BOOL isIgnoreError(WORD errormask);
 
  static WORD IVF_IGNORE_READERROR;
  static WORD IVF_IGNORE_MEMERROR;
  static WORD IVF_IGNORE_DBGINFO;
  static WORD IVF_IGNORE_DBGWARNING;
  static WORD IVF_IGNORE_DBGERROR;

protected:
  virtual ~CIvfComponent(void);

  virtual void IvfSceneGraphChanged(void);
 
  void IvfOnBeginPrinting(void);
  void IvfOnPrint(CDC *);
  void IvfOnEndPrinting(void);

  BOOL IvfSaveAsBitmap(LPCTSTR filename);

  virtual void IvfOnSelection(SoPath * path);
  virtual void IvfOnDeSelection(SoPath * path);
  static void IvfStaticSelectionCB(void * closure, SoPath * path);
  static void IvfStaticDeSelectionCB(void * closure, SoPath * path);

  virtual void IvfRemoveManips(void);
  virtual void IvfRestoreManips(void);

  void IvfOnEditCopy(void);
  void IvfOnUpdateEditCopy(CCmdUI * pCmdUI);
  void IvfOnEditCut(void);
  void IvfOnUpdateEditCut(CCmdUI * pCmdUI);
  void IvfOnEditPaste(void);
  void IvfOnUpdateEditPaste(CCmdUI * pCmdUI);
  void IvfOnUpdateEditUndo(CCmdUI * pCmdUI);

  virtual void IvfOnSelectViewpoint(int camera);

  virtual void IvfOnUrlDisplayChanged(const char * url);

  virtual SoPath * IvfOnSelectionPick(const SoPickedPoint * ppoint);

  virtual void IvfOnViewAnchor(SoPath * path);

  void IvfOnSelectionEnable(void);
  void IvfOnUpdateSelectionEnable(CCmdUI * pCmdUI);

  static void IvfErrorCallback(const SoError * error, void * data);

  virtual void IvfPrePasteAction(void);
  virtual void IvfPostPasteAction(void);
  virtual void IvfPreCopyAction(void);
  virtual void IvfPostCopyAction(void);
  virtual void IvfPreCutAction(void);

  virtual void IvfSetupUrlDisplayCallback(SoGroup * root);
  virtual void IvfRemoveUrlDisplayCallback(SoGroup * root);

  virtual void IvfLoc2Callback(void * closure, SoEventCallback * node);
  static void IvfStaticLoc2Callback(void * closure, SoEventCallback * node);

  virtual void IvfSetupSelectionPickCallback(void);
  virtual void IvfRemoveSelectionPickCallback(void);
 
  static SoPath * IvfStaticSelectionPickCB(void * closure, const SoPickedPoint * ppoint);

  static void IvfPasteDoneCB(void * closure, SoPathList * paths);
  void IvfPasteDone(SoPathList * paths);

  void IvfDestroySelectedObjects(void);

  virtual void IvfOnDestroy(void);

  virtual void IvfOnPaint(void);
  virtual void IvfOnPaint(CWnd * window);

  virtual BOOL IvfOnEraseBkgnd(CDC * pDC);

  virtual void IvfOnCreate(LPCREATESTRUCT cs);
  virtual void IvfOnInitialUpdate(CWnd * window);

  virtual void IvfOnActivateComponent(CView * activateview = NULL);

  virtual void IvfOnDeactivateComponent(void);
    
  virtual BOOL IvfOnCommand(WPARAM wParam, LPARAM lParam);

  virtual BOOL IvfOnCmdMsg(UINT id, int code, void * extra,
                           AFX_CMDHANDLERINFO * info) ;

  virtual void IvfOnSelectViewAnchors(void);
    
  virtual BOOL IvfIsViewpointMsg(UINT i, int code, void * extra,
                                 AFX_CMDHANDLERINFO * info);
 
  virtual BOOL IvfIsSceneViewer(void);

protected:
  SoWinComponent * m_pComponent;
  void * m_pCreateArgs;
  SoWinClipboard * m_pClipboard;
  SoSelection * m_pSelection;
  SoOffscreenRenderer * m_pOffscreen;
  CWnd * m_pWnd;
  SoEventCallback * m_pEventCB;
  BOOL m_nEnableSelection;
  BOOL m_bBgColorChangedSet;
  static WORD m_fIgnoreError;

};

#include <IvfLeaveScope.h>

#define IVF_COMPONENT_BASE()                                                \
public:                                                                     \
  virtual void IvfCreateComponent(CWnd * window, void * viewerargs = NULL); \
  virtual void IvfDestroyComponent(void);                                   \
  virtual void IvfSetSceneGraph(SoNode * root);                             \
  virtual SoNode * IvfGetSceneGraph(void);                                  \
  virtual SoNode * IvfGetBaseViewerSceneGraph(void);                        \
  virtual void IvfReleaseSceneGraph(void);                                  \
  virtual void IvfSetBackgroundColor(SbColor & background);                 \
  virtual void IvfSetHeadlight(BOOL enable);                                \
  virtual BOOL IvfIsHeadlight(void);                                        \
  virtual void IvfSetActiveCamera(SoCamera * camera);                       \
  virtual SoCamera * IvfGetActiveCamera(void);                              \
  virtual void IvfSaveHomePosition(void);                                   \
  virtual void IvfResetToHomePosition(void);                                \
  virtual void IvfViewAll(void);                                            \
  virtual SoGLRenderAction * IvfGetGLRenderAction(void);                    \
  virtual const SbViewportRegion & IvfGetViewportRegion(void);              \
  virtual SoSceneManager * IvfGetSceneManager(void);                        \
  virtual BOOL IvfIsViewing(void);                                          \
  virtual void IvfSetViewing(BOOL enable);

#define IVF_MFC_ON_METHOD_NAME(CmdName) On##CmdName
#define IVF_MFC_ON_UPD_METHOD_NAME(CmdName) OnUpdate##CmdName 

#define IVF_ON_METHOD_NAME(CmdName) IvfOn##CmdName
#define IVF_ON_UPD_METHOD_NAME(CmdName) IvfOnUpdate##CmdName 

#define IVF_COMMAND_HANDLER(ComponentName, CmdName)                         \
void                                                                        \
ComponentName::IVF_MFC_ON_METHOD_NAME(CmdName)(void)                        \
{                                                                           \
    IVF_ON_METHOD_NAME(CmdName)();                                          \
}                                                                           \
                                                                            \
void                                                                        \
ComponentName::IVF_MFC_ON_UPD_METHOD_NAME(CmdName)(CCmdUI * cmd)            \
{                                                                           \
  IVF_ON_UPD_METHOD_NAME(CmdName)(cmd);                                     \
}

#define IVF_COMMAND_HANDLER_BETA2(ComponentName, ViewerName, CmdName)       \
void                                                                        \
ComponentName::IVF_MFC_ON_METHOD_NAME(CmdName)(void)                        \
{                                                                           \
  IVF_ON_METHOD_NAME(CmdName)();                                            \
}                                                                           \
                                                                            \
void                                                                        \
ComponentName::IVF_MFC_ON_UPD_METHOD_NAME(CmdName)(CCmdUI * cmd)            \
{                                                                           \
  IVF_ON_UPD_METHOD_NAME(CmdName)(cmd);                                     \
}

#endif // !IVF_COMPONENT_H

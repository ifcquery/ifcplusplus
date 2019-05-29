#include <afxwin.h>
#include <assert.h>

#include <Ivf/IvfComponent.h>

WORD CIvfComponent::m_fIgnoreError = 0;
WORD CIvfComponent::IVF_IGNORE_READERROR = 1;
WORD CIvfComponent::IVF_IGNORE_MEMERROR = 2;
WORD CIvfComponent::IVF_IGNORE_DBGINFO = 4;
WORD CIvfComponent::IVF_IGNORE_DBGWARNING = 8;
WORD CIvfComponent::IVF_IGNORE_DBGERROR = 16;

// *************************************************************************

CIvfComponent::CIvfComponent(void)
{
}

CIvfComponent::~CIvfComponent(void)
{
}

void
CIvfComponent::IvfOnPaint(CWnd * window) // virtual
{
}

void
CIvfComponent::IvfViewmodesSelectionmode(void)
{
}

void
CIvfComponent::IvfViewmodesViewingmode(void)
{
}

void
CIvfComponent::IvfOnInitialUpdate(CWnd * window) // virtual
{
}

void
CIvfComponent::IvfResetComponent(CIvfDocument * ivfdocument)
{
}

void
CIvfComponent::IvfOnActivateComponent(CView * activateview) // virtual
{
}

void
CIvfComponent::IvfOnDeactivateComponent(void) // virtual
{
}
    
BOOL
CIvfComponent::IvfOnCmdMsg(UINT id, int code, void * extra, AFX_CMDHANDLERINFO * info) // virtual
{
  return FALSE;
}

void
CIvfComponent::IvfOnCreate(LPCREATESTRUCT cs) // virtual
{
}

void
CIvfComponent::IvfOnDestroy(void) // virtual
{
}

BOOL
CIvfComponent::IvfOnEraseBkgnd(CDC * pDC) // virtual
{
  return FALSE;
}

void
CIvfComponent::IvfOnBeginPrinting(void)
{
}

void
CIvfComponent::IvfOnPrint(CDC *)
{
}

void
CIvfComponent::IvfOnEndPrinting(void)
{
}

BOOL
CIvfComponent::IvfSaveAsBitmap(LPCTSTR filename)
{
  return FALSE;
}

// *************************************************************************

SoSelection *
CIvfComponent::IvfGetSelectionNode(void) // virtual
{
  return NULL;
}

SoWinClipboard *
CIvfComponent::IvfGetClipboard(void) // virtual
{
  return NULL;
}

SoWinComponent *
CIvfComponent::IvfGetSoWinComponent(void) // virtual
{
  return NULL;
}

BOOL
CIvfComponent::IvfIsSelected(void) // virtual
{
  return FALSE;
}

void
CIvfComponent::IvfSetActiveCamera(int camera) // virtual
{
}

void
CIvfComponent::setIgnoreError(WORD errormask) // static
{
}

void
CIvfComponent::clearIgnoreError(WORD errormask) // static
{
}

BOOL
CIvfComponent::isIgnoreError(WORD errormask) // static
{
  return FALSE;
}
 
// PROTECTED
  
void
CIvfComponent::IvfSceneGraphChanged(void) // virtual
{
}

void
CIvfComponent::IvfOnSelection(SoPath * path) // virtual
{
}

void
CIvfComponent::IvfOnDeSelection(SoPath * path) // virtual
{
}

void
CIvfComponent::IvfStaticSelectionCB(void * closure, SoPath * path) // static
{
}

void
CIvfComponent::IvfStaticDeSelectionCB(void * closure, SoPath * path) // static
{
}

void
CIvfComponent::IvfRemoveManips(void) // virtual
{
}

void
CIvfComponent::IvfRestoreManips(void) // virtual
{
}

void
CIvfComponent::IvfOnEditCopy(void)
{
}

void
CIvfComponent::IvfOnUpdateEditCopy(CCmdUI * pCmdUI)
{
}

void
CIvfComponent::IvfOnEditCut(void)
{
}

void
CIvfComponent::IvfOnUpdateEditCut(CCmdUI * pCmdUI)
{
}

void
CIvfComponent::IvfOnEditPaste(void)
{
}

void
CIvfComponent::IvfOnUpdateEditPaste(CCmdUI * pCmdUI)
{
}

void
CIvfComponent::IvfOnUpdateEditUndo(CCmdUI * pCmdUI)
{
}

void
CIvfComponent::IvfOnSelectViewpoint(int camera) // virtual
{
}

void
CIvfComponent::IvfOnUrlDisplayChanged(const char * url) // virtual
{
}

SoPath *
CIvfComponent::IvfOnSelectionPick(const SoPickedPoint * ppoint) // virtual
{
  return NULL;
}

void
CIvfComponent::IvfOnViewAnchor(SoPath * path) // virtual
{
}

void
CIvfComponent::IvfOnSelectionEnable(void)
{
}

void
CIvfComponent::IvfOnUpdateSelectionEnable(CCmdUI * pCmdUI)
{
}

void
CIvfComponent::IvfErrorCallback(const SoError * error, void * data) // static
{
}

void
CIvfComponent::IvfPrePasteAction(void) // virtual
{
}

void
CIvfComponent::IvfPostPasteAction(void) // virtual
{
}

void 
CIvfComponent::IvfPreCopyAction(void) // virtual
{
}

void
CIvfComponent::IvfPostCopyAction(void) // virtual
{
}

void
CIvfComponent::IvfPreCutAction(void) // virtual
{
}

void
CIvfComponent::IvfSetupUrlDisplayCallback(SoGroup * root) // virtual
{
}

void
CIvfComponent::IvfRemoveUrlDisplayCallback(SoGroup * root) // virtual
{
}

void
CIvfComponent::IvfLoc2Callback(void * closure, SoEventCallback * node) // virtual
{
}

void
CIvfComponent::IvfStaticLoc2Callback(void * closure, SoEventCallback * node) // static
{
}

void
CIvfComponent::IvfSetupSelectionPickCallback(void) // virtual
{
}

void
CIvfComponent::IvfRemoveSelectionPickCallback(void) // virtual
{
}
 
SoPath *
CIvfComponent::IvfStaticSelectionPickCB(void * closure, const SoPickedPoint * ppoint) // static
{
  return NULL;
}

void
CIvfComponent::IvfPasteDoneCB(void * closure, SoPathList * paths) // static
{
}

void
CIvfComponent::IvfPasteDone(SoPathList * paths)
{
}

void
CIvfComponent::IvfDestroySelectedObjects(void)
{
}

void
CIvfComponent::IvfOnPaint(void) // virtual
{
}

BOOL
CIvfComponent::IvfOnCommand(WPARAM wParam, LPARAM lParam) // virtual
{
  return FALSE;
}

void
CIvfComponent::IvfOnSelectViewAnchors(void) // virtual
{
}
    
BOOL
CIvfComponent::IvfIsViewpointMsg(UINT i, int code, void * extra, AFX_CMDHANDLERINFO * info) // virtual
{
  return FALSE;
}

BOOL
CIvfComponent::IvfIsSceneViewer(void) // virtual
{
  return FALSE;
}

// *************************************************************************

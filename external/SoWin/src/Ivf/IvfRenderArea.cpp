#include <afxwin.h>

#include <Inventor/SbViewportRegion.h>

#include <Ivf/Viewers/IvfRenderArea.h>

// *************************************************************************

CIvfRenderArea::CIvfRenderArea(void)
{
}

CIvfRenderArea::~CIvfRenderArea(void)
{
}

void
CIvfRenderArea::IvfSpinText(const char * text)
{
}

// *************************************************************************
// IVF_COMPONENT_BASE

void
CIvfRenderArea::IvfCreateComponent(CWnd * window, void * viewerargs) // virtual
{
}

void
CIvfRenderArea::IvfDestroyComponent(void) // virtual
{
}

void
CIvfRenderArea::IvfSetSceneGraph(SoNode * root) // virtual
{
}

SoNode *
CIvfRenderArea::IvfGetSceneGraph(void) // virtual
{
  return NULL;
}

SoNode *
CIvfRenderArea::IvfGetBaseViewerSceneGraph(void) // virtual
{
  return NULL;
}

void
CIvfRenderArea::IvfReleaseSceneGraph(void) // virtual
{
}

void
CIvfRenderArea::IvfSetBackgroundColor(SbColor & background) // virtual
{
}

void
CIvfRenderArea::IvfSetHeadlight(BOOL enable) // virtual
{
}

BOOL
CIvfRenderArea::IvfIsHeadlight(void) // virtual
{
  return FALSE;
}

void
CIvfRenderArea::IvfSetActiveCamera(SoCamera * camera) // virtual
{
}

SoCamera *
CIvfRenderArea::IvfGetActiveCamera(void) // virtual
{
  return NULL;
}

void
CIvfRenderArea::IvfSaveHomePosition(void) // virtual
{
}

void
CIvfRenderArea::IvfResetToHomePosition(void) // virtual
{
}

void
CIvfRenderArea::IvfViewAll(void) // virtual
{
}

SoGLRenderAction *
CIvfRenderArea::IvfGetGLRenderAction(void) // virtual
{
  return NULL;
}

const SbViewportRegion &
CIvfRenderArea::IvfGetViewportRegion(void) // virtual
{
  static SbViewportRegion vpr(100, 100);
  return vpr;
}

SoSceneManager *
CIvfRenderArea::IvfGetSceneManager(void) // virtual
{
  return NULL;
}

BOOL
CIvfRenderArea::IvfIsViewing(void) // virtual
{
  return FALSE;
}

void
CIvfRenderArea::IvfSetViewing(BOOL enable) // virtual
{
}

// *************************************************************************

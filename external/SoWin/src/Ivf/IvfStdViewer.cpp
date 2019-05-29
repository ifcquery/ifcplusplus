#include <afxwin.h>

#include <Inventor/SbViewportRegion.h>

#include <Ivf/Viewers/IvfStdViewer.h>

// *************************************************************************

CIvfStdViewer::CIvfStdViewer(void)
{
}

CIvfStdViewer::~CIvfStdViewer(void)
{
}

// *************************************************************************
// IVF_COMPONENT_BASE


void
CIvfStdViewer::IvfCreateComponent(CWnd * window, void * viewerargs) // virtual
{
}

void
CIvfStdViewer::IvfDestroyComponent(void) // virtual
{
}

void
CIvfStdViewer::IvfSetSceneGraph(SoNode * root) // virtual
{
}

SoNode *
CIvfStdViewer::IvfGetSceneGraph(void) // virtual
{
  return NULL;
}

SoNode *
CIvfStdViewer::IvfGetBaseViewerSceneGraph(void) // virtual
{
  return NULL;
}

void
CIvfStdViewer::IvfReleaseSceneGraph(void) // virtual
{
}

void
CIvfStdViewer::IvfSetBackgroundColor(SbColor & background) // virtual
{
}

void
CIvfStdViewer::IvfSetHeadlight(BOOL enable) // virtual
{
}

BOOL
CIvfStdViewer::IvfIsHeadlight(void) // virtual
{
  return FALSE;
}

void
CIvfStdViewer::IvfSetActiveCamera(SoCamera * camera) // virtual
{
}

SoCamera *
CIvfStdViewer::IvfGetActiveCamera(void) // virtual
{
  return NULL;
}

void
CIvfStdViewer::IvfSaveHomePosition(void) // virtual
{
}

void
CIvfStdViewer::IvfResetToHomePosition(void) // virtual
{
}

void
CIvfStdViewer::IvfViewAll(void) // virtual
{
}

SoGLRenderAction *
CIvfStdViewer::IvfGetGLRenderAction(void) // virtual
{
  return NULL;
}

const SbViewportRegion &
CIvfStdViewer::IvfGetViewportRegion(void) // virtual
{
  static SbViewportRegion vpr(100, 100);
  return vpr;
}

SoSceneManager *
CIvfStdViewer::IvfGetSceneManager(void) // virtual
{
  return NULL;
}

BOOL
CIvfStdViewer::IvfIsViewing(void) // virtual
{
  return FALSE;
}

void
CIvfStdViewer::IvfSetViewing(BOOL enable) // virtual
{
}

// *************************************************************************

#include <afxwin.h>
#include <assert.h>

#include <Inventor/Win/SoWin.h>

#include <Ivf/IvfApp.h>

// public static members
CIvfApp * CIvfApp::m_pApp = NULL;
CDocTemplate * CIvfApp::m_pDocTemplate = NULL;
void * CIvfApp::m_pCreateArgs = NULL;
BOOL CIvfApp::m_nMDI = FALSE;

CIvfApp::CIvfApp(void)
{
  assert(CIvfApp::m_pApp == NULL);
  CIvfApp::m_pApp = this;
}

CIvfApp::~CIvfApp(void)
{
}

void
CIvfApp::IvfInitInstance(CWinApp * app, CIvfApp * ivfapp, IvfCvtAfxViewProc afxviewproc, IvfCvtIvfComponentProc ivfcomponentproc, IvfCvtAfxWndProc afxwndproc, IvfCvtIvfComponentProcW ivfcomponentprocw, IvfCvtAfxDocProc afxdocproc, IvfCvtIvfDocProc ivfdocproc, IvfCvtAfxFrameProc afxframeproc, void * createargs, BOOL createcomponent)
{
  // IvfInitSoWin()
}

void
CIvfApp::IvfExitInstance(void)
{
}

void
CIvfApp::IvfOnIdle(LONG count)
{
}

void *
CIvfApp::IvfCreateArgs(void)
{
  return NULL;
}

// *************************************************************************
// rest is stubbed for now

void
CIvfApp::IvfInitSoWin(CWnd * toplevel)
{
  // SoWin::init();
}

CIvfApp *
CIvfApp::IvfGetApp(void)
{
  return CIvfApp::m_pApp;
}

CDocTemplate *
CIvfApp::IvfGetDocTemplate(void)
{
  return CIvfApp::m_pDocTemplate;
}

CIvfDocument *
CIvfApp::IvfGetDocument(CIvfComponent * ivfcomponent)
{
  return NULL;
}

CIvfComponent *
CIvfApp::IvfGetComponent(CIvfDocument * ivfdocument)
{
  return NULL;
}

CFrameWnd *
CIvfApp::IvfGetAfxFrame(CIvfComponent * ivfcomponent)
{
  return NULL;
}

CIvfMainFrame *
CIvfApp::IvfGetIvfFrame(void)
{
  return NULL;
}

CView *
CIvfApp::IvfGetAfxView(CIvfComponent * ivfcomponent)
{
  return NULL;
}

CWnd *
CIvfApp::IvfGetAfxWnd(CIvfComponent * ivfcomponent)
{
  return NULL;
}

CDocument *
CIvfApp::IvfGetAfxDocument(CIvfComponent * ivfcomponent)
{
  return NULL;
}

CDocument *
CIvfApp::IvfGetAfxDocument(CIvfDocument * ivfdocument)
{
  return NULL;
}

CIvfComponent *
CIvfApp::ToIvfComponent(CView * afxview)
{
  return NULL;
}

CView *
CIvfApp::ToAfxView(CIvfComponent * ivfcomponent)
{
  return NULL;
}

CIvfComponent *
CIvfApp::ToIvfComponentW(CWnd * afxwindow)
{
  return NULL;
}

CWnd *
CIvfApp::ToAfxWnd(CIvfComponent * ivfcomponent)
{
  return NULL;
}

CIvfDocument *
CIvfApp::ToIvfDocument(CDocument * afxdocument)
{
  return NULL;
}

CDocument *
CIvfApp::ToAfxDocument(CIvfDocument * ivfdocument)
{
  return NULL;
}

CIvfMainFrame *
CIvfApp::ToIvfFrame(CFrameWnd * frame)
{
  return NULL;
}

BOOL
CIvfApp::IvfIsMdi(void)
{
  return FALSE;
}

void
CIvfApp::IvfAddToIvfAfxObjectList(CObject * afxobject)
{
}

void
CIvfApp::IvfRemoveFromIvfAfxObjectList(CObject * afxobject)
{
}

BOOL
CIvfApp::IvfIsIvfComponent(CView * afxview)
{
  return FALSE;
}

BOOL
CIvfApp::IvfIsIvfDocument(CDocument * afxdocument)
{
  return FALSE;
}

void
CIvfApp::enableMultiThread(void)
{
}

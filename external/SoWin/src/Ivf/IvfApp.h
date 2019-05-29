#ifndef IVF_APP_H
#define IVF_APP_H

#include <afxwin.h>

class CDocument;
class CDocTemplate;
class CView;
class CWnd;
class CFrameWnd;

class CIvfDocument;
class CIvfMainFrame;
class CIvfComponent;

typedef CIvfComponent * (* IvfCvtAfxViewProc)(CView *);
typedef CView * (* IvfCvtIvfComponentProc)(CIvfComponent *);
typedef CIvfComponent * (* IvfCvtAfxWndProc)(CWnd *);
typedef CWnd * (* IvfCvtIvfComponentProcW)(CIvfComponent *);
typedef CIvfDocument * (* IvfCvtAfxDocProc)(CDocument *);
typedef CDocument * (* IvfCvtIvfDocProc)(CIvfDocument *);
typedef CIvfMainFrame * (* IvfCvtAfxFrameProc)(CFrameWnd *);

#include <Ivf/IvfBasic.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfApp {
public:
  CIvfApp(void);
  ~CIvfApp(void);

  static void IvfInitInstance(CWinApp * app, CIvfApp * ivfapp, IvfCvtAfxViewProc afxviewproc, IvfCvtIvfComponentProc ivfcomponentproc, IvfCvtAfxWndProc afxwndproc, IvfCvtIvfComponentProcW ivfcomponentprocw, IvfCvtAfxDocProc afxdocproc, IvfCvtIvfDocProc ivfdocproc, IvfCvtAfxFrameProc afxframeproc, void * createargs, BOOL createcomponent);
  static void IvfExitInstance(void);
  static void IvfOnIdle(LONG count);
  static void IvfInitSoWin(CWnd * toplevel);
  static CIvfApp * IvfGetApp(void);
  static CDocTemplate * IvfGetDocTemplate(void);
  static CIvfDocument * IvfGetDocument(CIvfComponent * ivfcomponent = NULL);
  static CIvfComponent * IvfGetComponent(CIvfDocument * ivfdocument = NULL);
  static CFrameWnd * IvfGetAfxFrame(CIvfComponent * ivfcomponent = NULL);
  static CIvfMainFrame * IvfGetIvfFrame(void);
  static CView * IvfGetAfxView(CIvfComponent * ivfcomponent = NULL);
  static CWnd * IvfGetAfxWnd(CIvfComponent * ivfcomponent = NULL);
  static CDocument * IvfGetAfxDocument(CIvfComponent * ivfcomponent = NULL);
  static CDocument * IvfGetAfxDocument(CIvfDocument * ivfdocument);
  static CIvfComponent * ToIvfComponent(CView * afxview);
  static CView * ToAfxView(CIvfComponent * ivfcomponent);
  static CIvfComponent * ToIvfComponentW(CWnd * afxwindow);
  static CWnd * ToAfxWnd(CIvfComponent * ivfcomponent);
  static CIvfDocument * ToIvfDocument(CDocument * afxdocument);
  static CDocument * ToAfxDocument(CIvfDocument * ivfdocument);
  static CIvfMainFrame * ToIvfFrame(CFrameWnd * frame);
  static void * IvfCreateArgs(void);
  static BOOL IvfIsMdi(void);
  static void IvfAddToIvfAfxObjectList(CObject * afxobject);
  static void IvfRemoveFromIvfAfxObjectList(CObject * afxobject);
  static BOOL IvfIsIvfComponent(CView * afxview);
  static BOOL IvfIsIvfDocument(CDocument * afxdocument);
  static void enableMultiThread(void);

public:
  static CIvfApp * m_pApp;
  static CDocTemplate * m_pDocTemplate;
  static void * m_pCreateArgs;
  static BOOL m_nMDI;

};

#ifndef __AFXOLE_H__

#define IVF_AFX_CONVERSION_METHOD(AppViewClassName, AppDocClassName) \
                                                                     \
static CIvfComponent *                                               \
IvfCvtAfxView(CView * afxview)                                       \
{                                                                    \
  return (AppViewClassName *) afxview;                               \
}                                                                    \
                                                                     \
static CView *                                                       \
IvfCvtIvfComponent(CIvfComponent * ivfcomponent)                     \
{                                                                    \
  return (AppViewClassName *) ivfcomponent;                          \
}                                                                    \
                                                                     \
static CIvfDocument *                                                \
IvfCvtAfxDoc(CDocument * afxdocument)                     Viewers           \
{                                                                    \
  return (AppDocClassName *) afxdocument;                            \
}                                                                    \
                                                                     \
static CDocument *                                                   \
IvfCvtIvfDoc(CIvfDocument * ivfdocument)                             \
{                                                                    \
  return (AppDocClassName *) ivfdocument;                            \
}                                                                    \
                                                                     \
static CIvfMainFrame *                                               \
IvfCvtAfxFrame(CFrameWnd * frame)                                    \
{                                                                    \
  if ( !frame->IsKindOf(RUNTIME_CLASS(CMainFrame)) ) return NULL;    \
  return (CIvfMainFrame *) ((CMainFrame *) frame);                   \
}
 
#else // __AFXOLE_H__

#define IVF_AFX_CONVERSION_METHOD(AppViewClassName, AppDocClassName) \
                                                                     \
static CIvfComponent *                                               \
IvfCvtAfxView(CView * afxview)                                       \
{                                                                    \
  return (AppViewClassName *) afxview;                               \
}                                                                    \
                                                                     \
static CView *                                                       \
IvfCvtIvfComponent(CIvfComponent * ivfcomponent)                     \
{                                                                    \
  return (AppViewClassName *) ivfcomponent;                          \
}                                                                    \
                                                                     \
static CIvfDocument *                                                \
IvfCvtAfxDoc(CDocument * afxdocument)                                \
{                                                                    \
  return (AppDocClassName *) afxdocument;                            \
}                                                                    \
                                                                     \
static CDocument *                                                   \
IvfCvtIvfDoc(CIvfDocument * ivfdocument)                             \
{                                                                    \
  return (AppDocClassName *) ivfdocument;                            \
}                                                                    \
                                                                     \
static CIvfMainFrame *                                               \
IvfCvtAfxFrame(CFrameWnd * frame)                                    \
{                                                                    \
  if ( frame->IsKindOf(RUNTIME_CLASS(CInPlaceFrame)) ) {             \
    return (CIvfMainFrame *) ((CInPlaceFrame *) frame);              \
  } else if ( frame->IsKindOf(RUNTIME_CLASS(CMainFrame)) ) {         \
    return (CIvfMainFrame *) ((CMainFrame *) frame);                 \
  } else {                                                           \
    return NULL;                                                     \
  }                                                                  \
}

#endif // __AFXOLE_H__

#define IVF_AFX_WND_CONVERSION_METHOD(AppWndClassName)               \
                                                                     \
static CIvfComponent *                                               \
IvfCvtAfxWnd(CWnd * afxwindow)                                       \
{                                                                    \
  return (AppWndClassName *) afxwindow;                              \
}                                                                    \
                                                                     \
static CWnd *                                                        \
IvfCvtIvfComponent(CIvfComponent * ivfcomponent)                     \
{                                                                    \
  return (AppWndClassName *) ivfcomponent;                           \
}

extern UINT WMAPPU_PROCPENDINGINLINE;

#include <IvfLeaveScope.h>

#endif // !IVF_APP_H

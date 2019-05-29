#ifndef IVF_MAINFRAME_H
#define IVF_MAINFRAME_H

#include <afxwin.h>

#include <Ivf/IvfBasic.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfMainFrame {
public:
  virtual void IvfOnCreate(CWnd * window);
  virtual BOOL IvfOnCommand(WPARAM wparam, LPARAM lparam);
  virtual BOOL IvfOnQueryNewPalette(void);
  virtual void IvfOnPaletteChanged(CWnd * focuswindow);
  virtual BOOL IvfPreCreateWindow(CREATESTRUCT & cs, UINT x, UINT y);
  virtual BOOL IvfOnUpdateFrameTitle(BOOL addtotitle, CString & title);
  virtual void IvfSetViewerText(CString & text);

  virtual void IvfSetStatusText(const char *) = 0;

protected: 
  CIvfMainFrame(void);
  virtual ~CIvfMainFrame(void);

};

#define IVF_MAINFRAME_BASE()                          \
  virtual void IvfSetStatusText(const char * text);   \
  virtual void IvfSetViewerText(CString &);

#include <IvfLeaveScope.h>

#endif // !IVF_MAINFRAME_H

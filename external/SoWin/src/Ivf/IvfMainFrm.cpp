#include <afxwin.h>
#include <assert.h>

#include <Ivf/IvfMainFrm.h>

// *************************************************************************

CIvfMainFrame::CIvfMainFrame(void)
{
}

CIvfMainFrame::~CIvfMainFrame(void)
{
}

void
CIvfMainFrame::IvfOnCreate(CWnd * window) // virtual
{
}

BOOL
CIvfMainFrame::IvfOnCommand(WPARAM wparam, LPARAM lparam) // virtual
{
  return FALSE;
}

BOOL
CIvfMainFrame::IvfOnQueryNewPalette(void) // virtual
{
  return FALSE;
}

void
CIvfMainFrame::IvfSetViewerText(CString & text) // virtual
{
}

// *************************************************************************

void
CIvfMainFrame::IvfOnPaletteChanged(CWnd * focuswindow) // virtual
{
}

BOOL
CIvfMainFrame::IvfPreCreateWindow(CREATESTRUCT & cs, UINT x, UINT y) // virtual
{
  return FALSE;
}

BOOL
CIvfMainFrame::IvfOnUpdateFrameTitle(BOOL addtotitle, CString & title) // virtual
{
  return FALSE;
}

// *************************************************************************

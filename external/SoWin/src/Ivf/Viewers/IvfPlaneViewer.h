#ifndef IVF_PLANEVIEWER_H
#define IVF_PLANEVIEWER_H

#include <Ivf/IvfBasic.h>
#include <Ivf/Viewers/IvfStdViewer.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfPlaneViewer : public CIvfStdViewer
{
public:
  CIvfPlaneViewer(void);

  virtual void IvfCreateComponent(CWnd * windows, void * createargs);
  virtual void IvfDestroyComponent(void);

protected:
  virtual ~CIvfPlaneViewer(void);
};

#include <IvfLeaveScope.h>

#endif // !IVF_PLANEVIEWER_H

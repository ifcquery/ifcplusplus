#ifndef IVF_FLYVIEWER_H
#define IVF_FLYVIEWER_H

#include <Ivf/IvfBasic.h>
#include <Ivf/Viewers/IvfStdViewer.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfFlyViewer : public CIvfStdViewer
{
public:
  CIvfFlyViewer(void);

  virtual void IvfCreateComponent(CWnd * window, void * createargs);
  virtual void IvfDestroyComponent(void);

protected:
  virtual ~CIvfFlyViewer(void);

};

#include <IvfLeaveScope.h>

#endif // !IVF_FLYVIEWER_H
